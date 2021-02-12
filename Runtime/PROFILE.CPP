/**************************************************************** PROFILE.CPP
 *                                                                          *
 *                     Profile (INI) File Object Class                      *
 *                                                                          *
 ****************************************************************************/

#include "System.h"

#include <direct.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Debug.h"
#include "Dialog.h"
#include "Module.h"
#include "Profile.h"
#include "ReString.h"
#include "Support.h"

/****************************************************************************
 *                                                                          *
 *                     Definitions & Declarations                           *
 *                                                                          *
 ****************************************************************************/

  // Constants

enum { ENTRYFIELD, ERRFIELD } ;


/****************************************************************************
 *                                                                          *
 *  Class: Profile Path Dialog                                              *
 *                                                                          *
 ****************************************************************************/

class ProfilePathDialog : public Dialog {

   private:
      int DialogID ;
      HelpWindow *Help ;
      char *Path ;
      int PathSize ;

   public:
      ProfilePathDialog ( HWND Parent, HWND Owner, HMODULE Resource, int Id, HelpWindow *Help, char *Path, int PathSize ) ;
      virtual ~ProfilePathDialog ( ) ;

      MRESULT InitDialog ( HWND Window ) ;
      MRESULT Command ( MPARAM1 mp1, MPARAM2 mp2 ) ;
      MRESULT MessageProcessor ( HWND hwnd, MESG msg, MPARAM1 mp1, MPARAM2 mp2 ) ;
} ;

ProfilePathDialog::ProfilePathDialog ( HWND Parent, HWND Owner, HMODULE Resource, int Id, HelpWindow *help, char *path, int pathsize ) :
   Dialog ( Parent, Owner, Resource, Id ), DialogID(Id), Help(help), Path(path), PathSize(pathsize) {
   Load ( ) ;
} /* endmethod */

ProfilePathDialog::~ProfilePathDialog ( ) {
} /* endmethod */

MRESULT ProfilePathDialog::InitDialog ( HWND Window ) {
   MRESULT Result = Dialog::InitDialog ( Window ) ;
   if ( Result == 0 ) {
      Help->Associate ( Window ) ;
      WinSetDlgItemText ( Window, DialogID+ENTRYFIELD, Path ) ;
   } /* endif */
   return ( Result ) ;
} /* endmethod */

MRESULT ProfilePathDialog::Command ( MPARAM1 mp1, MPARAM2 ) {
   switch ( SHORT1FROMMP(mp1) ) {
      case DID_OK: {
         char Name [256] ;
         WinQueryDlgItemText ( Handle, DialogID+ENTRYFIELD, sizeof(Name), PSZ(Name) ) ;
         char FullPath [256] ;
         if ( DosQueryPathInfo ( Name, FIL_QUERYFULLNAME, FullPath, sizeof(FullPath) ) ) {
            PSZ Message = PSZ ( "ERROR: Not a valid path." ) ;
            WinSetDlgItemText ( Handle, DialogID+ERRFIELD, Message ) ;
            Sys_BeepError ( ) ;
            WinSetFocus ( HWND_DESKTOP, WinWindowFromID ( Handle, DialogID+ENTRYFIELD ) ) ;
            return ( 0 ) ;
         } /* endif */
         FILESTATUS3 Status ;
         if ( DosQueryPathInfo ( FullPath, FIL_STANDARD, &Status, sizeof(Status) ) ) {
            PSZ Message = PSZ ( "ERROR: Path does not exist." ) ;
            WinSetDlgItemText ( Handle, DialogID+ERRFIELD, Message ) ;
            Sys_BeepError ( ) ;
            WinSetFocus ( HWND_DESKTOP, WinWindowFromID ( Handle, DialogID+ENTRYFIELD ) ) ;
            return ( 0 ) ;
         } /* endif */
         if ( ! ( Status.attrFile & FILE_DIRECTORY ) ) {
            PSZ Message = PSZ ( "ERROR: Specified path is not a directory." ) ;
            WinSetDlgItemText ( Handle, DialogID+ERRFIELD, Message ) ;
            Sys_BeepError ( ) ;
            WinSetFocus ( HWND_DESKTOP, WinWindowFromID ( Handle, DialogID+ENTRYFIELD ) ) ;
            return ( 0 ) ;
         } /* endif */
         strncpy ( Path, FullPath, PathSize ) ;
         Dismiss ( TRUE ) ;
         break; }
      case DID_CANCEL:
         Dismiss ( FALSE ) ;
         break;
   } /* endswitch */
   return ( 0 ) ;
} /* endmethod */

MRESULT ProfilePathDialog::MessageProcessor ( HWND hwnd, MESG msg, MPARAM1 mp1, MPARAM2 mp2 ) {
   MRESULT Result ;
   switch ( msg ) {
      case WM_INITDLG:  Result = InitDialog ( hwnd ) ; break ;
      case WM_COMMAND:  Result = Command ( mp1, mp2 ) ; break ;
      default:          Result = Dialog::MessageProcessor ( hwnd, msg, mp1, mp2 ) ;
   } /* endswitch */
   return ( Result ) ;
} /* endmethod */

/****************************************************************************
 *                                                                          *
 *      Profile Class Constructor - Prompt for path if not obvious.         *
 *                                                                          *
 ****************************************************************************/

Profile::Profile ( char *name, HAB Anchor, HMODULE Library, int DialogID, HelpWindow *Help, BOOL ResetFlag ) {

  /**************************************************************************
   * Save the name.                                                         *
   **************************************************************************/

   Name = new char [ strlen(name) + 1 ] ;
   strcpy ( Name, name ) ;

  /**************************************************************************
   * If resetting the profile, clear the system profile information now.    *
   **************************************************************************/

   if ( ResetFlag ) 
      PrfWriteProfileData ( HINI_USERPROFILE, PSZ(Name), 0, 0, 0 ) ;

  /**************************************************************************
   * Query the system INI for the profile file's path.                      *
   **************************************************************************/

   PSZ ProfilePath = 0 ;
   ULONG Size ;

   if ( PrfQueryProfileSize ( HINI_USERPROFILE, PSZ(Name), PSZ("INIPATH"), &Size ) ) {

      // The info exists.  Fetch it.
      ProfilePath = new char [ Size ] ;
      PrfQueryProfileData ( HINI_USERPROFILE, PSZ(Name), PSZ("INIPATH"), ProfilePath, &Size ) ;

      // Build the profile file name.
      char FullPath [_MAX_PATH] ;
      strcpy ( FullPath, ProfilePath ) ;
      strcat ( FullPath, "\\" ) ;
      strcat ( FullPath, Name ) ;
      strcat ( FullPath, ".INI" ) ;

      // Clean the name up and expand it to a full path.
      char Path [256] ;
      DosQueryPathInfo ( FullPath, FIL_QUERYFULLNAME, Path, sizeof(Path) ) ;

      // Does the file exist?  If not, discard the name.
      FILESTATUS3 Status ;
      if ( DosQueryPathInfo ( Path, FIL_STANDARD, &Status, sizeof(Status) ) ) 
         delete [] ProfilePath, ProfilePath = 0 ;

   } /* endif */

  /**************************************************************************
   * If the profile file couldn't be found, ask the user for a path.        *
   **************************************************************************/

   if ( ProfilePath == 0 ) {

      // Set the default path.
      char Path [256] ;
      DosQueryPathInfo ( PSZ("."), FIL_QUERYFULLNAME, Path, sizeof(Path) ) ;

      // Call up the entry dialog.
      ProfilePathDialog ThisDialog ( HWND_DESKTOP, HWND_DESKTOP, Library, DialogID, Help, Path, sizeof(Path) ) ;
      if ( ThisDialog.Process ( ) ) {

         // Save the approved path in the system profile.
         ProfilePath = new char [ strlen(Path) + 1 ] ;
         strcpy ( ProfilePath, Path ) ;

         if ( !PrfWriteProfileData ( HINI_USERPROFILE, Name, PSZ("INIPATH"), ProfilePath, strlen(PCHAR(ProfilePath))+1 ) ) {
            char Message [512] ;
            Log ( "Profile::Profile: Could not save location of INI file.  %s", InterpretWinError(0,Message) ) ;
         } /* endif */

      } /* endif */

   } /* endif */

  /**************************************************************************
   * Reset profile handle.  If the path could be determined . . .           *
   **************************************************************************/

   Handle = 0 ;

   if ( ProfilePath ) {

     /***********************************************************************
      * Build the full profile file name.                                   *
      ***********************************************************************/

      char ProfileName [_MAX_PATH] ;
      strcpy ( ProfileName, ProfilePath ) ;
      strcat ( ProfileName, "\\"  ) ;
      strcat ( ProfileName, Name ) ;
      strcat ( ProfileName, ".INI" ) ;

     /***********************************************************************
      * Release the memory previously allocated to store the path.          *
      ***********************************************************************/

      if ( ProfilePath ) 
         delete [] ProfilePath, ProfilePath = 0 ;

     /***********************************************************************
      * Open/Create the profile file and return the resultant handle.       *
      ***********************************************************************/

      Handle = PrfOpenProfile ( Anchor, PSZ(ProfileName) ) ;
      if ( Handle == NULLHANDLE ) {
         char Message [512] ;
         Log ( "Profile::Profile: Could not open INI file.  %s", InterpretWinError(0,Message) ) ;
      } /* endif */

     /***********************************************************************
      * If resetting, then clean this profile out.                          *
      ***********************************************************************/

      if ( ResetFlag ) {
         if ( !PrfWriteProfileData ( Handle, PSZ(Name), 0, 0, 0 ) ) {
            char Message [512] ;
            Log ( "Profile::Profile: Could not reset INI file.  %s", InterpretWinError(0,Message) ) ;
         } /* endif */
      } /* endif */

   } /* endif */
}
 
/****************************************************************************
 *                                                                          *
 *      Profile Class Constructor - Don't ask if you can't find it.         *
 *                                                                          *
 ****************************************************************************/

Profile::Profile ( char *name, HAB Anchor, char *HomePath ) {

  /**************************************************************************
   * Save the name.                                                         *
   **************************************************************************/

   Name = new char [ strlen(name) + 1 ] ;
   strcpy ( Name, name ) ;

  /**************************************************************************
   * Query the system INI for the profile file's path.                      *
   **************************************************************************/

   PSZ ProfilePath = 0 ;
   ULONG Size ;

   if ( PrfQueryProfileSize ( HINI_USERPROFILE, Name, PSZ("INIPATH"), &Size ) ) {

      // The info exists.  Fetch it.
      ProfilePath = new char [ Size ] ;
      PrfQueryProfileData ( HINI_USERPROFILE, PSZ(Name), PSZ("INIPATH"), ProfilePath, &Size ) ;

      // Build the profile file name.
      char FullPath [_MAX_PATH] ;
      strcpy ( FullPath, ProfilePath ) ;
      strcat ( FullPath, "\\" ) ;
      strcat ( FullPath, Name ) ;
      strcat ( FullPath, ".INI" ) ;

      // Clean the name up and expand it to a full path.
      char Path [256] ;
      DosQueryPathInfo ( FullPath, FIL_QUERYFULLNAME, Path, sizeof(Path) ) ;

      // Does the file exist?  If not, discard the name.
      FILESTATUS3 Status ;
      if ( DosQueryPathInfo ( Path, FIL_STANDARD, &Status, sizeof(Status) ) ) 
         delete [] ProfilePath, ProfilePath = 0 ;

   } /* endif */

  /**************************************************************************
   * If the profile file couldn't be found, use the home path.              *
   **************************************************************************/

   if ( ProfilePath == 0 ) {
      ProfilePath = new char [ strlen(HomePath) + 1 ] ;
      strcpy ( ProfilePath, HomePath ) ;
   } /* endif */

  /**************************************************************************
   * Reset profile handle.  If the path could be determined . . .           *
   **************************************************************************/

   Handle = 0 ;

   if ( ProfilePath ) {

     /***********************************************************************
      * Build the full profile file name.                                   *
      ***********************************************************************/

      char ProfileName [_MAX_PATH] ;
      strcpy ( ProfileName, ProfilePath ) ;
      strcat ( ProfileName, "\\"  ) ;
      strcat ( ProfileName, Name ) ;
      strcat ( ProfileName, ".INI" ) ;

     /***********************************************************************
      * Release the memory previously allocated to store the path.          *
      ***********************************************************************/

      if ( ProfilePath ) 
         delete [] ProfilePath, ProfilePath = 0 ;

     /***********************************************************************
      * Open/Create the profile file and return the resultant handle.       *
      ***********************************************************************/

      Handle = PrfOpenProfile ( Anchor, PSZ(ProfileName) ) ;
      if ( Handle == NULLHANDLE ) {
         char Message [512] ;
         Log ( "Profile::Profile: Could not open INI file.  %s", InterpretWinError(0,Message) ) ;
      } /* endif */

   } /* endif */
}
 
/****************************************************************************
 *                                                                          *
 *      Profile Class Destructor                                            *
 *                                                                          *
 ****************************************************************************/

Profile::~Profile ( ) {

  /**************************************************************************
   * Release allocated memory.                                              *
   **************************************************************************/

   delete [] Name, Name = 0 ;

  /**************************************************************************
   * Close the profile.                                                     *
   **************************************************************************/

   if ( Handle ) {
      if ( !PrfCloseProfile ( Handle ) ) {
         char Message [512] ;
         Log ( "Profile::~Profile: Could not close INI file.  %s", InterpretWinError(0,Message) ) ;
      } /* endif */
   } /* endif */
}
 
/****************************************************************************
 *                                                                          *
 *      Profile: Get Item                                                   *
 *                                                                          *
 ****************************************************************************/

BOOL Profile::GetItem ( char *ItemName, void *Item, int Size ) {

   if ( Handle == 0 ) 
      return ( FALSE ) ;

   ULONG HowBig ;
   if ( !PrfQueryProfileSize ( Handle, PSZ(Name), PSZ(ItemName), &HowBig ) ) {
      return ( FALSE ) ;
   } /* endif */

   if ( Size != HowBig ) {
      Log ( "Profile::GetItem: Could not get INI item.  App %s, Item %s.  Wrong size.", Name, ItemName ) ;
      return ( FALSE ) ;
   } /* endif */

   if ( !PrfQueryProfileData ( Handle, PSZ(Name), PSZ(ItemName), Item, &HowBig ) ) {
      char Message [512] ;
      Log ( "Profile::GetItem: Could not get INI item.  App %s, Item %s, size %i.  %s", Name, ItemName, HowBig, InterpretWinError(0,Message) ) ;
      return ( FALSE ) ;
   } /* endif */

   return ( TRUE ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Profile: Set Item                                                   *
 *                                                                          *
 ****************************************************************************/

BOOL Profile::PutItem ( char *ItemName, void *Item, int Size ) {

   if ( Handle == 0 ) 
      return ( FALSE ) ;

   if ( !PrfWriteProfileData ( Handle, PSZ(Name), PSZ(ItemName), Item, Size ) ) {
      char Message [512] ;
      Log ( "Profile::PutItem: Could not put INI item.  App %s, Item %s, size %i.  %s", Name, ItemName, Size, InterpretWinError(0,Message) ) ;
      return ( FALSE ) ;
   } /* endif */

   return ( TRUE ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Profile: Get String                                                 *
 *                                                                          *
 ****************************************************************************/

BOOL Profile::GetString ( char *ItemName, char *Item, int MaxSize ) {

   if ( Handle == 0 ) 
      return ( FALSE ) ;

   ULONG TrueSize ;
   if ( !PrfQueryProfileSize ( Handle, PSZ(Name), PSZ(ItemName), &TrueSize ) ) {
      return ( FALSE ) ;
   } /* endif */

   ULONG HowBig = min ( TrueSize+1, MaxSize ) ;
   if ( !PrfQueryProfileData ( Handle, PSZ(Name), PSZ(ItemName), Item, &HowBig ) ) {
      char Message [512] ;
      Log ( "Profile::GetString: Could not get INI item.  App %s, Item %s, TrueSize %i, MaxSize %i.  %s", Name, ItemName, TrueSize, MaxSize, InterpretWinError(0,Message) ) ;
      return ( FALSE ) ;
   } /* endif */

   Item[HowBig] = 0 ;

   return ( TRUE ) ;
}
 
BOOL Profile::GetString ( char *ItemName, char* &Item ) {

   if ( Handle == 0 ) {
      Item = 0 ;
      return ( FALSE ) ;
   } /* endif */

   ULONG TrueSize ;
   if ( !PrfQueryProfileSize ( Handle, PSZ(Name), PSZ(ItemName), &TrueSize ) ) {
      Item = 0 ;
      return ( FALSE ) ;
   } /* endif */

   Item = new char [ TrueSize + 1 ] ;
   if ( Item == 0 ) {
      Log ( "Profile::GetString: Unable to allocate memory to hold string.  App %s, Item %s, TrueSize %i.", Name, ItemName, TrueSize ) ;
      return ( FALSE ) ;
   } /* endif */

   if ( !PrfQueryProfileData ( Handle, PSZ(Name), PSZ(ItemName), Item, &TrueSize ) ) {
      char Message [512] ;
      Log ( "Profile::GetString: Could not get INI item.  App %s, Item %s, TrueSize %i.  %s", Name, ItemName, TrueSize, InterpretWinError(0,Message) ) ;
      delete [] Item ;
      Item = 0 ;
      return ( FALSE ) ;
   } /* endif */

   Item[TrueSize] = 0 ;

   return ( TRUE ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Profile: Set Item                                                   *
 *                                                                          *
 ****************************************************************************/

BOOL Profile::PutString ( char *ItemName, char *Item ) {

   if ( Handle == 0 ) 
      return ( FALSE ) ;

   if ( !PrfWriteProfileData ( Handle, PSZ(Name), PSZ(ItemName), Item, strlen(Item)+1 ) ) {
      char Message [512] ;
      Log ( "Profile::PutString: Could not put INI item.  App %s, Item %s, Value %s.  %s", Name, ItemName, Item, InterpretWinError(0,Message) ) ;
      return ( FALSE ) ;
   } /* endif */

   return ( TRUE ) ;
}
