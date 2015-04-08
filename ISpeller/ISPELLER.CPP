/*************************************************************** ISPELLER.CPP
 *                                                                          *
 *                    ISPELLER Programmer's API functions                   *
 *                                                                          *
 ****************************************************************************/

#include "Debug.h"

#include <ctype.h>
#include <direct.h>
#include <process.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define INCL_BASE
#define INCL_PM
#include <os2.h>

extern "C" {
   #include "Config.h"
   #include "ISpell.h"
   #include "Proto.h"
   #include "Msgs.h"
   #include "Version.h"
}

#ifdef const
   #undef const
#endif

#define LOCAL_DEFINES
#include "ISpeller.h"

#include "ReString.h"

static void initckch ( char *wchars ) ;
static char *MakeHashName ( char *Path ) ;

// #define DEBUG
// #define DEBUG_PROFILE
// #define DEBUG_MUTEX


/****************************************************************************
 *                                                                          *
 *                       Definitions & Declarations                         *
 *                                                                          *
 ****************************************************************************/

  /* Macros & Constants */

#define PROGRAM_NAME    "ISPELLER"
#define LANGUAGE_NAME   "ISP__*.DLL"

#define NOT !
#define AND &&
#define OR  ||

enum MessageID {
   WM_BUILDDONE = WM_USER
} ;


  /* Function Prototypes */

extern "C" {
   extern int _CRT_init ( void ) ;
}


  /* Globals (private to the library, created by library instance) */

static char HomePath [_MAX_PATH] = { 0 } ;
static class LanguageModule *Library = 0 ;
static HMODULE LibraryHandle = 0 ;


/****************************************************************************
 *                                                                          *
 *                    Log Message (Debugging Function)                      *
 *                                                                          *
 ****************************************************************************/

static void Log ( char *Message, ... ) {

  /**************************************************************************
   * Open the log file.                                                     *
   **************************************************************************/

   char LogFile [_MAX_PATH] ;
   strcpy ( LogFile, HomePath ) ;
   strcat ( LogFile, "\\" PROGRAM_NAME ".log" ) ;
 
   FILE *File = fopen ( LogFile, "a" ) ;

  /**************************************************************************
   * If the file got opened, write the message to the log file and close it.*
   **************************************************************************/

   if ( File ) {
      char Time [9], Date [9] ;
      fprintf ( File, "%s %s " REVISION " %02i ", _strtime(Time), _strdate(Date), *_threadid ) ;
      va_list Marker ;
      va_start ( Marker, Message ) ;
      vfprintf ( File, Message, Marker ) ;
      va_end ( Marker ) ;
      fprintf ( File, "\n" ) ;
      fclose ( File ) ;
   } /* endif */
}


/****************************************************************************
 *                                                                          *
 *  Interpret WIN/GPI errors                                                *
 *                                                                          *
 ****************************************************************************/

static char *InterpretWinError ( HAB Anchor, char *Message ) {
   PERRINFO pErrorInfo = WinGetErrorInfo ( Anchor ) ;
   if ( pErrorInfo ) {
      strcpy ( Message, "" ) ;
      long *Offsets = (long*) ( ((char*)pErrorInfo) + pErrorInfo->offaoffszMsg ) ;
      long *BinaryData = (long*) ( ((char*)pErrorInfo) + pErrorInfo->offBinaryData ) ;
      for ( int i=0; i<pErrorInfo->cDetailLevel; i++ ) {
         char *FormatString = (char*)pErrorInfo + Offsets[i] ;
         if ( FormatString[strlen(FormatString)-1] == '\n' ) 
            FormatString[strlen(FormatString)-1] = 0 ;
         if ( FormatString[strlen(FormatString)-1] == '\r' )
            FormatString[strlen(FormatString)-1] = 0 ;
         vsprintf ( Message+strlen(Message), FormatString, (va_list)BinaryData ) ;
         if ( i < pErrorInfo->cDetailLevel-1 ) 
            strcat ( Message, ", " ) ;
         else
            strcat ( Message, "." ) ;
      } /* endfor */
      WinFreeErrorInfo ( pErrorInfo ) ;
      return ( Message ) ;
   } else {
      return ( 0 ) ;
   } /* endif */
}


/****************************************************************************
 *                                                                          *
 *  Class: Profile                                                          *
 *                                                                          *
 ****************************************************************************/

class Profile {

   private:

      HINI   Handle ;               // INI file handle.
      PSZ    Name ;                 // Application Name.

   public:

      Profile ( PSZ Name, HAB Anchor ) ;

      ~Profile ( ) ;

      HINI QueryHandle ( ) { return ( Handle ) ; }

      PSZ QueryName ( ) { return ( Name ) ; }

      BOOL GetItem ( char *ItemName, void *Item, int Size ) ;
      BOOL PutItem ( char *ItemName, void *Item, int Size ) ;

      BOOL GetString ( char *ItemName, char *Item, int MaxSize ) ;
      BOOL GetString ( char *ItemName, char* &Item ) ;
      BOOL PutString ( char *ItemName, char *Item ) ;

      VOID Reset ( char *ItemName ) { PrfWriteProfileData ( Handle, Name, ItemName, 0, 0 ) ; }
      VOID Reset ( ) { PrfWriteProfileData ( Handle, Name, 0, 0, 0 ) ; }
} ;

Profile::Profile ( PSZ name, HAB Anchor ) : Handle ( 0 ) {

   Name = PSZ ( malloc ( strlen(PCHAR(name)) + 1 ) ) ;
   strcpy ( PCHAR(Name), PCHAR(name) ) ;

   PSZ ProfilePath = 0 ;
   ULONG Size ;

   if ( PrfQueryProfileSize ( HINI_USERPROFILE, Name, PSZ("INIPATH"), &Size ) ) {

      // The info exists.  Fetch it.
      ProfilePath = PSZ ( malloc ( size_t ( Size ) ) )  ;
      PrfQueryProfileData ( HINI_USERPROFILE, Name, PSZ("INIPATH"), ProfilePath, &Size ) ;

      // Build the profile file name.
      UCHAR FullPath [_MAX_PATH] ;
      strcpy ( PCHAR(FullPath), PCHAR(ProfilePath) ) ;
      strcat ( PCHAR(FullPath), "\\" ) ;
      strcat ( PCHAR(FullPath), PCHAR(Name) ) ;
      strcat ( PCHAR(FullPath), ".INI" ) ;

      // Clean the name up and expand it to a full path.
      UCHAR Path [256] ;
      DosQueryPathInfo ( PSZ(FullPath), FIL_QUERYFULLNAME, Path, sizeof(Path) ) ;

      // Does the file exist?  If not, discard the name.
      FILESTATUS3 Status ;
      if ( DosQueryPathInfo ( PSZ(Path), FIL_STANDARD, &Status, sizeof(Status) ) ) {
         free ( ProfilePath ) ;
         ProfilePath = 0 ;
      } /* endif */

   } /* endif */

   if ( ProfilePath == 0 ) {

      // Set the default path.
      char Home[256], Path [256] ;
      strcpy ( Home, HomePath ) ;
      strcat ( Home, "\\." ) ;
      DosQueryPathInfo ( PSZ(Home), FIL_QUERYFULLNAME, PSZ(Path), sizeof(Path) ) ;

      ProfilePath = PSZ ( malloc ( strlen(Path) + 1 ) )  ;
      strcpy ( PCHAR(ProfilePath), Path ) ;

      if ( !PrfWriteProfileData ( HINI_USERPROFILE, Name, PSZ("INIPATH"), ProfilePath, strlen(PCHAR(ProfilePath))+1 ) ) {
         char Message [512] ;
         Log ( "Profile::Profile: Could not save location of INI file.  %s", InterpretWinError(0,Message) ) ;
      } /* endif */

   } /* endif */

   UCHAR ProfileName [_MAX_PATH] ;
   strcpy ( PCHAR(ProfileName), PCHAR(ProfilePath) ) ;
   strcat ( PCHAR(ProfileName), "\\"  ) ;
   strcat ( PCHAR(ProfileName), PCHAR(Name) ) ;
   strcat ( PCHAR(ProfileName), ".INI" ) ;

   free ( ProfilePath ) ;

   Handle = PrfOpenProfile ( Anchor, PSZ(ProfileName) ) ;
   if ( Handle == 0 ) {
      char Message [512] ;
      Log ( "Profile::Profile: Could not open INI file '%s' (%s).  %s", Name, ProfileName, InterpretWinError(0,Message) ) ;
   } /* endif */
}

Profile::~Profile ( ) {

   if ( Handle ) {
      if ( !PrfCloseProfile ( Handle ) ) {
         char Message [512] ;
         Log ( "Profile::~Profile: Could not close INI file '%s'.  %s", Name, InterpretWinError(0,Message) ) ;
      } /* endif */
   } /* endif */

   free ( Name ) ;
}

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

BOOL Profile::GetString ( char *ItemName, char *Item, int MaxSize ) {

   if ( Handle == 0 ) 
      return ( FALSE ) ;

   ULONG TrueSize ;
   if ( !PrfQueryProfileSize ( Handle, PSZ(Name), PSZ(ItemName), &TrueSize ) ) {
      return ( FALSE ) ;
   } /* endif */

   ULONG HowBig = min ( TrueSize, MaxSize ) ;
   if ( !PrfQueryProfileData ( Handle, PSZ(Name), PSZ(ItemName), Item, &HowBig ) ) {
      char Message [512] ;
      Log ( "Profile::GetString: Could not get INI item.  App %s, Item %s, TrueSize %i, MaxSize %i.  %s", Name, ItemName, TrueSize, MaxSize, InterpretWinError(0,Message) ) ;
      return ( FALSE ) ;
   } /* endif */

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

   Item = (char*) malloc ( size_t(TrueSize) ) ;
   if ( Item == 0 ) {
      Log ( "Profile::GetString: Unable to allocate memory to hold string.  App %s, Item %s, TrueSize %i.", Name, ItemName, TrueSize ) ;
      return ( FALSE ) ;
   } /* endif */

   if ( !PrfQueryProfileData ( Handle, PSZ(Name), PSZ(ItemName), Item, &TrueSize ) ) {
      char Message [512] ;
      Log ( "Profile::GetString: Could not get INI item.  App %s, Item %s, TrueSize %i.  %s", Name, ItemName, TrueSize, InterpretWinError(0,Message) ) ;
      free ( Item ) ;
      Item = 0 ;
      return ( FALSE ) ;
   } /* endif */

   return ( TRUE ) ;
}

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


/****************************************************************************
 *                                                                          *
 *  Class: Resource Module                                                  *
 *                                                                          *
 ****************************************************************************/

class Module {

   protected:
      char *Name ;
      HMODULE Handle ;

   public:
      Module ( char *Name = "" ) ;
      virtual ~Module ( ) ;
      char *QueryName () { return ( Name ) ; }
      HMODULE QueryHandle () { return ( Handle ) ; }
      PFN QueryAddress ( char *Name ) ;
      PFN QueryAddress ( int Ordinal ) ;
} ;

Module::Module ( char *name ) : Handle(0) {
   if ( name && *name ) {
      Name = (char*) malloc ( strlen(name) + 1 ) ;
      strcpy ( Name, name ) ;
      char Drive[_MAX_DRIVE+1], Dir[_MAX_DIR+1], FName[_MAX_FNAME+1], Ext[_MAX_EXT+1] ;
      _splitpath ( Name, Drive, Dir, FName, Ext ) ;
      char ModuleName [_MAX_PATH] ;
      strcpy ( ModuleName, Drive ) ;
      strcat ( ModuleName, Dir ) ;
      strcat ( ModuleName, FName ) ;
      if ( *Drive || *Dir )
         strcat ( ModuleName, Ext ) ;
      strupr ( ModuleName ) ;
      char MissingModule [_MAX_PATH] ;
      DosLoadModule ( PSZ(MissingModule), sizeof(MissingModule), PSZ(ModuleName), &Handle ) ;
   } else {
      Name = 0 ;
   } /* endif */
}

Module::~Module ( ) {
   if ( Handle )
      DosFreeModule ( Handle ) ;
   if ( Name ) 
      free ( Name ) ;
}

PFN Module::QueryAddress ( char *Name ) {
   PFN Address ;
   if ( DosQueryProcAddr ( Handle, 0, PSZ(Name), (PFN*)&Address ) )
      return ( 0 ) ;
   return ( Address ) ;
}

PFN Module::QueryAddress ( int Ordinal ) {
   PFN Address ;
   if ( DosQueryProcAddr ( Handle, Ordinal, 0, (PFN*)&Address ) )
      return ( 0 ) ;
   return ( Address ) ;
}


/****************************************************************************
 *                                                                          *
 *  Class: Language Module                                                  *
 *                                                                          *
 ****************************************************************************/

class LanguageModule : public Module {

   private:
      char *Language ;
      USHORT *Codepages ;

   public:
      LanguageModule ( char *AppName, char *Name = "" ) ;
      ~LanguageModule ( ) ;
      char   *QueryLanguage ( ) { return ( Language ) ; }
      USHORT *QueryCodepages ( ) { return ( Codepages ) ; }

} ;

LanguageModule::LanguageModule ( char *appname, char *Name ) : Module(Name), Language(0), Codepages(0) {

   if ( Handle == 0 )
      return ;

   PVOID Offset ;
   if ( DosGetResource ( Handle, RT_RCDATA, 1, &Offset ) ) {
      DosFreeModule ( Handle ) ;
      Handle = 0 ;
      return ;
   } /* endif */

   PCHAR DllType = PCHAR ( Offset ) ;                   
   if ( strcmpi ( DllType, "Language" ) ) {
      DosFreeModule ( Handle ) ;
      Handle = 0 ;
      return ;
   } /* endif */

   PCHAR AppName = DllType + strlen(DllType) + 1 ;      
   if ( strcmpi ( AppName, appname ) ) {
      DosFreeModule ( Handle ) ;
      Handle = 0 ;
      return ;
   } /* endif */

   // Save the language name.
   PCHAR language = AppName + strlen(AppName) + 1 ;
   Language = (char*) malloc ( strlen(language) + 1 ) ;
   strcpy ( Language, language ) ;

   // Save the list of valid codepages.
   PUSHORT pCodePage = PUSHORT ( language + strlen(language) + 1 ) ;
   PUSHORT pus = pCodePage ;
   while ( *pus ) {
      pus ++ ;
   } /* endwhile */
   Codepages = PUSHORT ( malloc ( (pus-pCodePage+1)*sizeof(USHORT) ) ) ;
   memcpy ( Codepages, pCodePage, (pus-pCodePage+1)*sizeof(USHORT) ) ;
}

LanguageModule::~LanguageModule ( ) {
   if ( Language ) free ( Language ) ;
   if ( Codepages ) free ( Codepages ) ;
}


/****************************************************************************
 *                                                                          *
 *  Utility Function: Create Application Language Object                    *
 *                                                                          *
 ****************************************************************************/

extern LanguageModule *Language_Create ( char *Application, char *Revision, int RevisionID, char *Language ) {

  /**************************************************************************
   * Set the default language to that requested, for now.                   *
   **************************************************************************/

   char DefaultLanguage [81] ;
   memcpy ( DefaultLanguage, Language, min(strlen(Language)+1,sizeof(DefaultLanguage)) ) ;
   DefaultLanguage [ sizeof(DefaultLanguage) - 1 ] = 0 ;

  /**************************************************************************
   * Find out what language libraries are available.                        *
   **************************************************************************/

   int LanguageCount = 0 ;
   char Languages [100] [80] ;
   char Libraries [100] [_MAX_PATH] ;

   FILEFINDBUF3 Found ;
   HDIR hDir = (HDIR) HDIR_CREATE ;
   ULONG Count = 1 ;

   char Wildcard [_MAX_PATH] ;
   strcpy ( Wildcard, HomePath ) ;
   strcat ( Wildcard, "\\" ) ;
   strcat ( Wildcard, LANGUAGE_NAME ) ;

   if ( !DosFindFirst2 ( Wildcard, &hDir, FILE_NORMAL, &Found, sizeof(Found), &Count, FIL_STANDARD ) ) {

      do {

         // Is it a valid language DLL?  Continue if not.
         char Filename [_MAX_PATH] ;
         strcpy ( Filename, HomePath ) ;
         strcat ( Filename, "\\" ) ;
         strcat ( Filename, Found.achName ) ;
         LanguageModule *Library = new LanguageModule ( Application, Filename ) ;
         if ( Library->QueryHandle() == 0 ) {
            delete Library ;
            continue ;
         } /* endif */

         // If the revision right?  Continue if not.
         ResourceString revision ( Library->QueryHandle(), RevisionID ) ;
         if ( strcmp ( PCHAR(revision), Revision ) ) {
            delete Library ;
            continue ;
         } /* endif */

         // If language matches given, then return at once.
         if ( !strcmpi ( Library->QueryLanguage(), DefaultLanguage ) ) 
            return ( Library ) ;

         // Save language name.  Abort if we have the max now.
         strcpy ( Languages[LanguageCount], Library->QueryLanguage() ) ;
         strcpy ( Libraries[LanguageCount], Filename ) ;
         LanguageCount ++ ;
         delete Library ;
         if ( LanguageCount >= sizeof(Languages)/sizeof(Languages[0]) )
            break ;

      } while ( !DosFindNext ( hDir, &Found, sizeof(Found), &Count ) ) ;

      DosFindClose ( hDir ) ;

   } /* endif */

   if ( LanguageCount == 0 ) 
      return ( 0 ) ;

  /**************************************************************************
   * Try to find the default language library.                              *
   **************************************************************************/

   strcpy ( DefaultLanguage, "English" ) ;
   char DefaultLibrary [_MAX_PATH] ;

   for ( int i=0; i<LanguageCount; i++ ) 
      if ( !strcmpi ( DefaultLanguage, Languages[i] ) ) 
         break ;

   if ( i >= LanguageCount ) {
      strcpy ( DefaultLanguage, Languages[0] ) ;
      strcpy ( DefaultLibrary, Libraries[0] ) ;
   } else {
      strcpy ( DefaultLanguage, Languages[i] ) ;
      strcpy ( DefaultLibrary, Libraries[i] ) ;
   } /* endif */

  /**************************************************************************
   * Open whatever language library was settled upon.                       *
   **************************************************************************/

   return ( new LanguageModule ( Application, DefaultLibrary ) ) ;
}


/****************************************************************************
 *                                                                          *
 *  Class: Help Window                                                      *
 *                                                                          *
 ****************************************************************************/

class HelpWindow {
   private:
      HAB Anchor ;
      HWND Handle ;
 
   public:
      HelpWindow ( HAB Anchor, HMODULE Module, USHORT Id, PSZ Filename, PSZ Title ) ;
      ~HelpWindow ( ) ;
      BOOL Associate ( HWND Window ) ;
      HWND QueryHandle () { return ( Handle ) ; }
} ;

  // Constructor

HelpWindow::HelpWindow ( HAB anchor, HMODULE Module, USHORT Id, PSZ Filename, PSZ Title ) : Anchor(anchor) {

   HELPINIT HelpInit ;
   memset ( &HelpInit, 0, sizeof(HelpInit) ) ;

   HelpInit.cb = sizeof(HelpInit) ;
   HelpInit.phtHelpTable = (PHELPTABLE)MAKEULONG(Id,0xFFFF);
   HelpInit.hmodHelpTableModule = Module ;
   HelpInit.pszHelpWindowTitle = Title ;
   HelpInit.fShowPanelId = CMIC_HIDE_PANEL_ID ;
   HelpInit.pszHelpLibraryName = Filename ;

   Handle = WinCreateHelpInstance ( Anchor, &HelpInit ) ;

   if ( Handle == 0 ) {
      ERRORID Error = WinGetLastError ( Anchor ) ;
      if ( ( Error & 0xFFFF ) != PMERR_NO_MSG_QUEUE ) {
         char Message [256] ;
         Log ( "ERROR: Unable to create help instance from file '%s'.  %s", Filename, InterpretWinError(Anchor,Message) ) ;
      } /* endif */
   } /* endif */

} /* endmethod */


  // Destructor

HelpWindow::~HelpWindow ( ) {

   if ( Handle ) 
      WinDestroyHelpInstance ( Handle ) ;

} /* endmethod */


  // Associate with a window.

BOOL HelpWindow::Associate ( HWND Window ) {

   if ( !WinIsWindow ( Anchor, Handle ) ) {
      Log ( "WARNING: Attempt to associate window %08p with invalid help instance %08p.", Window, Handle ) ;
      return ( FALSE ) ;
   } /* endif */

   if ( !WinIsWindow ( Anchor, Window ) ) {
      Log ( "WARNING: Attempt to associate help instance with invalid window %08p.", Window ) ;
      return ( FALSE ) ;
   } /* endif */

   if ( !WinAssociateHelpInstance ( Handle, Window ) ) {
      char Message [512] ;
      Log ( "ERROR: Unable to associate help instance %08p with window %08p.  %s", Handle, Window, InterpretWinError(0,Message) ) ;
      return ( FALSE ) ;
   } /* endif */

   return ( TRUE ) ;

} /* endmethod */


/****************************************************************************
 *                                                                          *
 *  Class: Mutual Exclusion Semaphore                                       *
 *                                                                          *
 ****************************************************************************/

class Mutex {

   private:

      char *Name ;
      HMTX Handle ;

   public:

      Mutex ( char *name, char *SystemName = 0 ) ;

      ~Mutex ( ) ;

      HMTX QueryHandle ( ) { return ( Handle ) ; }

      unsigned long Request ( int Timeout = SEM_INDEFINITE_WAIT ) ;

      unsigned long Release ( ) ;

} ;

Mutex::Mutex ( char *name, char *SystemName ) : Name(name), Handle(0) {
   APIRET Status = 1 ;
   if ( SystemName ) {
      Status = DosOpenMutexSem ( PSZ(SystemName), &Handle ) ;
      #ifdef DEBUG_MUTEX
         if ( Name && !Status ) {
            Log ( "MUTEX(%s): Opened existing named semaphore '%s'.  Handle %08lX.", Name, SystemName, Handle ) ;
         } /* endif */
      #endif
   } /* endif */
   if ( Status ) {
      Status = DosCreateMutexSem ( PSZ(SystemName), &Handle, DC_SEM_SHARED, FALSE ) ;
      if ( Name ) {
         if ( Status ) {
            Log ( "MUTEX(%s): Unable to create semaphore for '%s'.  Status %i.", Name, SystemName, Status ) ;
         #ifdef DEBUG_MUTEX
            } else {
               Log ( "MUTEX(%s): Created semaphore '%s'.  Handle %08lX.", Name, SystemName, Handle ) ;
         #endif
         } /* endif */
      } /* endif */
   } /* endif */
} /* endmethod */

Mutex::~Mutex ( ) {
   #ifdef __OS2__
      DosCloseMutexSem ( Handle ) ;
   #else
      CloseHandle ( Handle ) ;
   #endif
   #ifdef DEBUG_MUTEX
      if ( Name ) 
         Log ( "MUTEX(%s): Destroyed semaphore.  Handle %08lX.", Name, Handle ) ;
   #endif
} /* endmethod */

unsigned long Mutex::Request ( int Timeout ) {

   // If invalid handle, return an error at once.
   if ( Handle == 0 )
      return ( 0xFFFFFFFF ) ;

   // Try to grab the semaphore non-wait.  If successful, return.  Otherwise, report progress or error.
   unsigned long Status = DosRequestMutexSem ( Handle, 0 ) ;
   if ( Name ) {
      if ( ( Status == ERROR_SEM_TIMEOUT ) || ( Status == ERROR_TIMEOUT ) ) {
         #ifdef DEBUG_MUTEX
            Log ( "MUTEX(%s): Blocking on semaphore.", Name ) ;
         #endif
      } else if ( Status ) {
         Log ( "MUTEX(%s): Error %i requesting semaphore.  Handle %08lX.", Name, Status, Handle ) ;
         return ( Status ) ;
      } else {
         #ifdef DEBUG_MUTEX
            Log ( "MUTEX(%s): Semaphore obtained w/o wait.", Name ) ;
         #endif
         return ( Status ) ;
      } /* endif */
   } else {
      if ( ( Status == ERROR_SEM_TIMEOUT ) || ( Status == ERROR_TIMEOUT ) ) {
      } else if ( Status ) {
         return ( Status ) ;
      } else {
         return ( Status ) ;
      } /* endif */
   } /* endif */

   Status = DosRequestMutexSem ( Handle, Timeout ) ;
   if ( Name ) {
      if ( ( Status == ERROR_SEM_TIMEOUT ) || ( Status == ERROR_TIMEOUT ) ) {
         Log ( "MUTEX(%s): Timeout waiting for semaphore.", Name ) ;
      } else if ( Status ) {
         Log ( "MUTEX(%s): Error %i requesting semaphore.  Handle %08lX.", Name, Status, Handle ) ;
      } else {
         #ifdef DEBUG_MUTEX
            Log ( "MUTEX(%s): Semaphore obtained.", Name ) ;
         #endif
      } /* endif */
   } /* endif */
   return ( Status ) ;

} /* endmethod */

unsigned long Mutex::Release ( ) {
   if ( Handle == 0 )
      return ( 0xFFFFFFFF ) ;
   #ifdef DEBUG_MUTEX
      if ( Name ) 
         Log ( "MUTEX(%s): Releasing semaphore.", Name ) ;
   #endif
   unsigned long Status = DosReleaseMutexSem ( Handle ) ;
   if ( Name && Status )
      Log ( "MUTEX(%s): Error %i releasing semaphore.  Handle %08lX.", Name, Status, Handle ) ;
   return ( Status ) ;
} /* endmethod */


/****************************************************************************
 *                                                                          *
 *                       DLL Initialization Function                        *
 *                                                                          *
 ****************************************************************************/

extern unsigned long _System _DLL_InitTerm ( unsigned long modhandle, unsigned long flag ) {

  /**************************************************************************
   * If starting up . . .                                                   *
   **************************************************************************/

   if ( flag == 0 ) {

     /***********************************************************************
      * Call C RunTime initialization.  Abort if error.                     *
      ***********************************************************************/

      if ( _CRT_init() == -1 )
         return ( 0 ) ;

     /***********************************************************************
      * Save the library handle.                                            *
      ***********************************************************************/

      LibraryHandle = modhandle ;

     /***********************************************************************
      * Find out where this module was loaded from.                         *
      ***********************************************************************/

      DosQueryModuleName ( LibraryHandle, sizeof(HomePath), HomePath ) ;
      char Drive[_MAX_DRIVE], Dir[_MAX_DIR], FName[_MAX_FNAME], Ext[_MAX_EXT] ;
      _splitpath ( HomePath, Drive, Dir, FName, Ext ) ;
      if ( Dir[strlen(Dir)-1] == '\\' )
         Dir[strlen(Dir)-1] = 0 ;
      strcpy ( HomePath, Drive ) ;
      strcat ( HomePath, Dir ) ;

      #ifdef DEBUG
         Log ( "ISpeller::_DLL_InitTerm: Home path set to '%s'.", HomePath ) ;
      #endif // DEBUG

  /**************************************************************************
   * Else, if shutting down . . .                                           *
   **************************************************************************/

   } else {

     /***********************************************************************
      * Delete the language module, if one's been created.                  *
      ***********************************************************************/

      if ( Library ) 
         delete Library, Library = 0 ;

   } /* endif */

  /**************************************************************************
   * Return NZ, indicating success.                                         *
   **************************************************************************/

   return ( 1 ) ;
}


/****************************************************************************
 *                                                                          *
 *                        Message Dispatcher                                *
 *                                                                          *
 ****************************************************************************/

typedef struct Method {
   USHORT Action ;
   PFNWP pFunction ;
} METHOD, *PMETHOD ;

static MRESULT DispatchMessage (
  HWND    hwnd,
  ULONG   msg,
  MPARAM  mp1,
  MPARAM  mp2,
  PMETHOD MethodTable,
  USHORT  MethodCount,
  PFNWP   DefaultProcessor
) {

 /***************************************************************************
  * Process messages according to object's class method table.              *
  ***************************************************************************/

  PMETHOD pMethod = MethodTable ;
  USHORT cNumberLeft = MethodCount ;

  while ( ( cNumberLeft ) AND ( pMethod->Action != msg ) ) {
    pMethod ++ ;
    cNumberLeft -- ;
  }

  MRESULT mr ;

  if ( cNumberLeft ) {
    mr = pMethod->pFunction ( hwnd, msg, mp1, mp2 ) ;
  } else {
    if ( DefaultProcessor )
      mr = DefaultProcessor ( hwnd, msg, mp1, mp2 ) ;
    else
      mr = 0 ;
  }

 /***************************************************************************
  * Return result from message processor.                                   *
  ***************************************************************************/

  return ( mr ) ;
}


/****************************************************************************
 *                                                                          *
 *  Identify DLL type.                                                      *
 *                                                                          *
 ****************************************************************************/

extern char* APIENTRY Identify ( void ) {
   return ( TYPE_ISPELLER );
}


/****************************************************************************
 *                                                                          *
 *  Set Language (must be called before anything using Library)             *
 *                                                                          *
 ****************************************************************************/

extern BOOL APIENTRY SetLanguage ( char *Language ) {

   #ifdef DEBUG
      Log ( "ISpeller::SetLanguage: Started." ) ;
   #endif // DEBUG

  /**************************************************************************
   * Try to create a new LanguageModule.  If successful, replace and        *
   *   discard the old one.                                                 *
   **************************************************************************/

   #ifdef DEBUG
      Log ( "ISpeller::SetLanguage: Creating language module." ) ;
   #endif // DEBUG

   LanguageModule *NewLibrary = Language_Create ( PROGRAM_NAME, REVISION, IDS_TITLE1, Language ) ;
   if ( NewLibrary ) {
      if ( Library ) 
         delete Library ;
      Library = NewLibrary ;
   } /* endif */

  /**************************************************************************
   * Done.                                                                  *
   **************************************************************************/

   #ifdef DEBUG
      Log ( "ISpeller::SetLanguage: Done." ) ;
   #endif // DEBUG

   return ( TRUE ) ;
}


/****************************************************************************
 *                                                                          *
 *  Give name/title.                                                        *
 *                                                                          *
 ****************************************************************************/

extern char* APIENTRY ProductName ( void ) {

   if ( Library == 0 ) {
      Log ( "ProductName aborted because SetLanguage not yet called." ) ;
      return ( "" ) ;
   } /* endif */

   ResourceString Name ( Library->QueryHandle(), IDS_TITLE ) ;

   static char Buffer [200] ;
   strcpy ( Buffer, PCHAR(Name) ) ;

   return ( Buffer ) ;
}


/****************************************************************************
 *                                                                          *
 *  Identify DLL version.  Returned in x100 form.                           *
 *                                                                          *
 ****************************************************************************/

extern int APIENTRY ProductVersion ( void ) {
   #ifdef DEBUG
      Log ( "ProductVersion started.  Revision %i.", REVISION_NO ) ;
   #endif
   return ( REVISION_NO );
}


/****************************************************************************
 *                                                                          *
 *  Product Information Dialog                                              *
 *                                                                          *
 ****************************************************************************/

typedef struct {
   HelpWindow *Help ;
} ABOUTPARMS, *PABOUTPARMS ;

static MRESULT EXPENTRY AboutProcessor ( HWND Window, ULONG msg, MPARAM mp1, MPARAM mp2 ) {

   switch ( msg ) {

      case WM_INITDLG: {
         PABOUTPARMS Parms = PABOUTPARMS ( malloc ( sizeof(ABOUTPARMS) ) ) ;
         WinSetWindowPtr ( Window, QWL_USER, Parms ) ;

         // Associate help instance with the dialog.
         char Drive [_MAX_DRIVE+1], Dir[_MAX_DIR+1], FName[_MAX_FNAME+1], Ext[_MAX_EXT+1] ;
         _splitpath ( Library->QueryName(), Drive, Dir, FName, Ext ) ;
         char HelpFileName [_MAX_PATH] ;
         sprintf ( HelpFileName, "%s\\%s.hlp", HomePath, FName ) ;
         ResourceString HelpTitle ( Library->QueryHandle(), IDS_HELPTITLE ) ;
         Parms->Help = new HelpWindow ( 0, LibraryHandle, ID_HELPTABLE, PSZ(HelpFileName), PSZ(HelpTitle) ) ;
         Parms->Help->Associate ( Window ) ;

         // Center the dialog over its owner.
         RECTL OwnerRectangle = { 0, 0, 0, 0 }, DialogRectangle = { 0, 0, 0, 0 } ;
         POINTL Position = { 0, 0 } ;
         WinQueryWindowRect ( WinQueryWindow(Window,QW_OWNER), &OwnerRectangle ) ;
         WinQueryWindowRect ( Window, &DialogRectangle ) ;
         Position.x = ( ( OwnerRectangle.xRight - OwnerRectangle.xLeft ) - ( DialogRectangle.xRight - DialogRectangle.xLeft ) ) / 2 + OwnerRectangle.xLeft ;
         Position.y = ( ( OwnerRectangle.yTop - OwnerRectangle.yBottom ) - ( DialogRectangle.yTop - DialogRectangle.yBottom ) ) / 2 + OwnerRectangle.yBottom ;
         WinMapWindowPoints ( WinQueryWindow(Window,QW_OWNER), HWND_DESKTOP, &Position, 1 ) ;
         WinSetWindowPos ( Window, 0, Position.x, Position.y, 0, 0, SWP_MOVE ) ;

         return ( 0 ) ; }

      case WM_DESTROY: {
         PABOUTPARMS Parms = PABOUTPARMS ( WinQueryWindowPtr ( Window, QWL_USER ) ) ;
         delete Parms->Help ;
         free ( Parms ) ;
         break ; }

   } /* endswitch */

   return ( WinDefDlgProc ( Window, msg, mp1, mp2 ) ) ;
}

extern void APIENTRY ProductInformation ( HWND Owner ) {

   WinDlgBox ( HWND_DESKTOP, Owner, AboutProcessor, Library->QueryHandle(), IDD_PRODUCTINFO, 0 ) ;

}


/****************************************************************************
 *                                                                          *
 *  Product Configuration Info                                              *
 *                                                                          *
 ****************************************************************************/

static FNWP Configure_Processor ;

extern int APIENTRY ProductConfigure ( int MaxPages, char Names[][64], char MajorTabs[][32], char MinorTabs[][32], PFNWP Processors[], HMODULE Libraries[], int DialogIDs[] ) {

   if ( MaxPages < 1 ) 
      return ( 0 ) ;

   ResourceString Title ( Library->QueryHandle(), IDS_TITLE ) ;
   ResourceString MajorTab ( Library->QueryHandle(), IDS_MAJORTAB ) ;

   strcpy ( Names[0], PCHAR(Title) ) ;
   strcpy ( MajorTabs[0], PCHAR(MajorTab) ) ;
   strcpy ( MinorTabs[0], "" ) ;
   Processors[0] = Configure_Processor ;
   Libraries[0] = Library->QueryHandle() ;
   DialogIDs[0] = IDD_CONFIGURE ;

   return ( 1 ) ;
}


/****************************************************************************
 *                                                                          *
 *  Set Working Directory.                                                  *
 *                                                                          *
 *  Parms:                                                                  *
 *     Path  ->  Full path to where ISpell files are to be found.           *
 *                                                                          *
 ****************************************************************************/

extern void APIENTRY Speller_SetHome ( char *Path ) {
   static char SetString [_MAX_PATH+10] ;
   sprintf ( SetString, "ISPELL=%s", Path ) ;
   _putenv ( SetString ) ;
}


/****************************************************************************
 *                                                                          *
 *  Return name of default dictionary file.                                 *
 *                                                                          *
 *  Returns char* pointer to name of the default dictionary file.           *
 *    If the name is not extended with the full path, then the file         *
 *    is assumed to be in the speller's working directory.                  *
 *                                                                          *
 ****************************************************************************/

extern char* APIENTRY Speller_DefaultDictionary ( void ) {

   // Get the name from the profile, if possible.
   Profile SetupData ( PROGRAM_NAME, 0 ) ;
   char CurrentDictionary [_MAX_PATH] ;
   if ( SetupData.GetString ( "Dictionary", CurrentDictionary, sizeof(CurrentDictionary) ) ) {
      if ( strlen(CurrentDictionary) ) {
         strcpy ( hashname, CurrentDictionary ) ;
         return ( hashname ) ;
      } /* endif */
   } /* endif */

   // If not, try to build from environment.
   ResourceString Dictionary ( Library->QueryHandle(), IDS_DICTIONARY  ) ;
   char *p = getenv ( "DICTIONARY" );
   if ( p )
      strcpy ( hashname, MakeHashName ( p ) ) ;
   else
      strcpy ( hashname, MakeHashName ( PCHAR(Dictionary) ) ) ;

   return ( hashname );
}


/****************************************************************************
 *                                                                          *
 *  Open Dictionary File                                                    *
 *                                                                          *
 *  Parms:                                                                  *
 *     Path  ->  Name of dictionary file.  If not a full path name, then    *
 *                 the working directory is assumed.  If no extension is    *
 *                 specified, then the default ".hsh" is appended.          *
 *                                                                          *
 *  Returns:                                                                *
 *     Non-zero if unable to open the dictionary file for some reason.      *
 *                                                                          *
 ****************************************************************************/

static char *LibDict = NULL;                            /* Pointer to name of $(LIBDIR)/dict */
static Mutex Lock ( "Dictionary Open/Close" ) ;         /* Serialization lock on open/close. */
static BOOL Initialized = FALSE ;                       /* Flag: Is everything ready?        */

extern int APIENTRY Speller_OpenDictionary ( char *Path ) {

   /* Local Declarations */
   static char libdictname [ _MAX_PATH ] ;
   char *cpd = NULL;
   char *p;
   char *wchars = NULL;

   /* Serialize access to this function. */
   Lock.Request ( ) ;

   /* Build the full hash file name. */
   strcpy ( hashname, MakeHashName ( Path ) );

   /* Try to load the hash table.  Exit if unable to do so. */
   if ( linit (  ) ) {
      Lock.Release() ;
      return ( -1 );
   } /* endif */

   /* Initialize the global flags. */
   if ( compoundflag < 0 )
      compoundflag = hashheader.compoundflag;
   if ( tryhardflag < 0 )
      tryhardflag = hashheader.defhardflag;

   /* Initialize some more . . . */
   initckch ( wchars );

   /* If no private dictionary specified, set the default name. */
   if ( LibDict == NULL ) {
      ResourceString Dictionary ( Library->QueryHandle(), IDS_DICTIONARY  ) ;
      ( void ) strcpy ( libdictname, PCHAR(Dictionary) );
      LibDict = libdictname;
      p = rindex ( libdictname, '.' );
      if ( p != NULL AND ( FILENAMECOMPARE ( p, HASHSUFFIX ) || FILENAMECOMPARE ( p, ".hash" ) ) )
         *p = '\0';             /* Don't want ext. in LibDict */
   } /* endif */

   /* Build the private dictionary tree. */
   treeinit ( cpd, LibDict );

   /* Return no error if we got this far. */
   Initialized = TRUE ;
   Lock.Release() ;
   return ( 0 );
}


/****************************************************************************
 *                                                                          *
 *  Close dictionary file.                                                  *
 *                                                                          *
 ****************************************************************************/

extern void APIENTRY Speller_CloseDictionary ( ) {

   /* Serialize access to this function. */
   Lock.Request ( ) ;

   /* If the library is open . . . */
   if ( Initialized ) {

      /* Save the private dictionary. */
      treeoutput (  );

      /* Release the hash table and reset the spell-checker. */
      lclose ( ) ;

      /* Reset initialization flag. */
      Initialized = FALSE ;
   } /* endif */

   /* Release the serialization lock. */
   Lock.Release() ;
}


/****************************************************************************
 *                                                                          *
 *  Is the speller ready for use?                                           *
 *                                                                          *
 ****************************************************************************/

extern BOOL APIENTRY Speller_Ready ( ) {
   return ( Initialized ) ;
}


/****************************************************************************
 *                                                                          *
 *  Build Word to be checked.                                               *
 *                                                                          *
 *  Parms:                                                                  *
 *    GetPtr -> char* (*GetPtr) ( void *Object, long *Offset, long Delta )  *
 *             callback to get a pointer to a character at an offset plus   * 
 *             a displacement within a text object.                         *
 *    Start  -> long variable containing the current offset into the text  .*
 *    End    -> long variable to receive the ending offset into the text.   *
 *    Next   -> long variable to receive the next offset into the text.     *
 *                                                                          *
 *  Returns:                                                                *
 *    Non-zero if a word was found.                                         *
 *    Start -> long variable containing the starting offset of the word     *
 *               found, if any.                                             *
 *    End -> long variable containing the ending offset of the word found,  *
 *               if any.                                                    *
 *    Next -> long variable containing the next offset to start scanning    *
 *               from, if any.                                              *
 *                                                                          *
 ****************************************************************************/

static long SkipToWord ( char* (* APIENTRY GetPtr)(void *Object,long *Offset,long Delta), void *Object, long Offset ) {

   char *bufp = 0 ;
   while ( 
      ( bufp = GetPtr ( Object, &Offset, 0 ) )
      AND
      ( 
         ( 
            !isstringch ( bufp, 0 ) 
            AND
            !iswordch ( chartoichar ( *bufp ) ) 
         )
         OR 
         isboundarych ( chartoichar ( *bufp ) ) 
      )
   ) {
      if ( !GetPtr ( Object, &Offset, +1 ) )
         return ( -1 ) ;
   } /* endwhile */
   return ( bufp ? Offset : -1 ) ;
}

static long SkipOverWord ( char* (* APIENTRY GetPtr)(void *Object,long *Offset,long Delta), void *Object, long Offset ) {

   register char *bufp ;
   register long lastboundary;
   register int scharlen;       /* Length of a string character */
   lastboundary = -1;
   for ( ;; ) {
      char *OldPtr = bufp ;
      bufp = GetPtr ( Object, &Offset, 0 ) ;
      if ( bufp == 0 ) 
         break;
      else if ( ( *bufp == 0 ) OR ( *bufp == '\n' ) || ( *bufp == '\f' ) || ( *bufp == '\x1A' ) )
         break;
      else if ( l_isstringch ( bufp, scharlen, 0 ) ) {
         Offset += scharlen;
         lastboundary = -1 ;
      /* * Note that we get here if a character satisfies *
       * isstringstart() but isn't in the string table;  this * allows
       * string characters to start with word characters. */
      } else if ( iswordch ( chartoichar ( *bufp ) ) ) {
         if ( !GetPtr ( Object, &Offset, +1 ) )
            break ;
         lastboundary = -1;
      } else if ( isboundarych ( chartoichar ( *bufp ) ) ) {
         if ( lastboundary == -1 )
            lastboundary = Offset ;
         else {
            long Work = Offset ;
            if ( !GetPtr ( Object, &Work, -1 ) )
               break ;
            if ( lastboundary == Work ) 
               break;           /* Double boundary -- end of word */
         } /* endif */
         if ( !GetPtr ( Object, &Offset, +1 ) )
            break ;
      } else
         break;                 /* End of the word */
   }
   /* * If the word ended in one or more boundary characters, * the
    * address of the first of these is in lastboundary, and it * is the
    * end of the word.  Otherwise, bufp is the end. */
   return ( ( lastboundary != -1 ) ? lastboundary : Offset ) ;
}

extern int APIENTRY Speller_BuildWord ( 
   char* (* APIENTRY GetPtr)(void *Object,long *Offset,long Delta),
   void *Object,
   char *Word,
   long *Start,
   long *End,
   long *Next
) { 

   /* Local Declarations */
   char *p, *pWord ;
   long Offset ;

   /* Skip to start of word. */
   *Start = SkipToWord ( GetPtr, Object, *Start ) ;
   if ( *Start == -1 )
      return ( 0 ) ;

   /* If no word found, return an error. */
   p = GetPtr ( Object, Start, 0 ) ;
   if ( ( p == 0 ) || ( *p == 0 ) || ( *p == '\n' ) || ( *p == '\f' ) || ( *p == '\x1A' ) )
      return ( 0 ) ;

   /* Skip to end of word. */
   *End = SkipOverWord ( GetPtr, Object, *Start ) ;
   if ( *End <= 0 )
      return ( 0 ) ;

   /* Copy out the word text. */
   pWord = Word ;
   Offset = *Start ;
   p = GetPtr ( Object, &Offset, 0 ) ;
   while ( Offset < *End ) {
      *pWord++ = *p ;
      p = GetPtr ( Object, &Offset, +1 ) ;
      if ( p == 0 ) 
         break ;
   } /* endwhile */
   *pWord = 0 ;

   /* Return OK. */
   return ( 1 ) ;
}


/****************************************************************************
 *                                                                          *
 *  Find Word in Dictionary.  Return TRUE if found or accepted.             *
 *                                                                          *
 ****************************************************************************/

extern int APIENTRY Speller_FindWord ( char *Word ) {

   /* Local Declarations */
   int ilen;
   char *p ;

   /* Trim the word to it's true length. */
   p = skipoverword ( Word );
   *p = 0 ;

   /* Copy word to workspace.  If too long, return not found. */
   if ( strtoichar ( itoken, Word, INPUTWORDLEN * sizeof ( ichar_t ), 0 ) )
      return ( 0 );

   /* If word too short, return TRUE. */
   ilen = icharlen ( itoken );
   if ( ilen <= minword )
      return ( 1 );

   /* If OK as single word, return TRUE. */
   if ( good ( itoken, 0, 0, 0, 0 ) )
      return ( 1 );

   /* If compounds not allowed, return FALSE. */
   if ( cflag ) 
      return ( 0 ) ;

   /* If OK as compound word, return TRUE. */
   if ( compoundgood ( itoken, 0 ) )
      return ( 1 );

   /* If not found yet, return FALSE. */
   return ( 0 );
}


/****************************************************************************
 *                                                                          *
 *  Find Possible Replacement Words                                         *
 *                                                                          *
 ****************************************************************************/

extern int APIENTRY Speller_FindReplacements ( char *Word, int ListSize, int EntrySize, void *List, int *EasyCount ) {

   /* Local Declarations */
   int i;

   /* Clear the count of easy possibilities. */
   *EasyCount = 0 ;

   /* Copy word to workspace.  If too long, return not found. */
   if ( strtoichar ( itoken, Word, INPUTWORDLEN * sizeof ( ichar_t ), 0 ) )
      return ( 0 );

   /* Determine some possible replacements. */
   makepossibilities ( itoken );

   /* Copy the results to the user's list. */
   for ( i = 0; i < pcount AND i < ListSize; i++ ) {
      memcpy ( ( char * ) List + i * EntrySize, possibilities[i],
         min ( EntrySize, strlen ( possibilities[i] ) + 1 ) );
   }                            /* endif */

   /* Return the count of easy possibilities. */
   *EasyCount = easypossibilities ;

   /* Return the count of possibilities, or the list size, whichever is
    * less. */
   return ( min ( pcount, ListSize ) );
}


/****************************************************************************
 *                                                                          *
 *  Accept Word into session dictionary.                                    *
 *                                                                          *
 ****************************************************************************/

extern int APIENTRY Speller_Accept ( char *Word ) {
   ichar_t *itok = strtosichar ( Word, 0 );
   treeinsert ( ichartosstr ( itok, 1 ), ICHARTOSSTR_SIZE, 0 );
   return ( 0 ) ;
}


/****************************************************************************
 *                                                                          *
 *  Insert Word into personal dictionary, as is.                            *
 *                                                                          *
 ****************************************************************************/

extern int APIENTRY Speller_Insert ( char *Word ) {
   ichar_t *itok = strtosichar ( Word, 0 );
   treeinsert ( ichartosstr ( itok, 1 ), ICHARTOSSTR_SIZE, 1 ) ;
   return ( 0 ) ;
}


/****************************************************************************
 *                                                                          *
 *  Insert Word into personal dictionary, case insensitive.                 *
 *                                                                          *
 ****************************************************************************/

extern int APIENTRY Speller_UnCaps ( char *Word ) {
   ichar_t *itok = strtosichar ( Word, 0 );
   lowcase ( itok );
   treeinsert ( ichartosstr ( itok, 1 ), ICHARTOSSTR_SIZE, 1 );
   return ( 0 ) ;
}


/****************************************************************************
 *                                                                          *
 *  Build Dictionary from munched word list and affix file.                 *
 *                                                                          *
 ****************************************************************************/

static int Speller_BuildDictionary ( char *MunchFile, char *AffixFile, char *HashFile ) {

   /* Local Declarations */
   int CurrentDrive ;
   char CurrentDirectory [ _MAX_PATH ] ;
   char *p ;
   char Path[_MAX_PATH] ;
   enum { BUILD_OK, BUILD_NOMUNCH, BUILD_NOAFFIX,
      BUILD_NOHASH, BUILD_NOEXE, BUILD_EXEERR } Result = BUILD_OK ;

   /* Where is ISPELL? */
   p = getenv ( HOME ) ;

   /* Save current drive and directory. */
   CurrentDrive = _getdrive ( ) ;
   _getcwd ( CurrentDirectory, sizeof(CurrentDirectory) ) ;

   /* Move to ISPELL's home directory. */
   if ( p ) {
      char Drive[_MAX_DRIVE], Dir[_MAX_DIR], FName[_MAX_FNAME], Ext[_MAX_EXT] ;
      _fullpath ( Path, p, sizeof(Path) ) ;
      _splitpath ( p, Drive, Dir, FName, Ext ) ;
      strcat ( Dir, FName ) ;
      strcat ( Dir, Ext ) ;
      if ( Drive[0] )
         _chdrive ( Drive[0] - 'A' + 1 ) ;
      if ( Dir[0] ) {
         _chdir ( "\\" ) ;
         _chdir ( Dir ) ;
      } /* endif */
   } /* endif */

   /* Verify existence of munched file. */
   if ( access ( MunchFile, 4 ) ) {
      _chdrive ( CurrentDrive ) ;
      _chdir ( "\\" ) ;
      _chdir ( CurrentDirectory ) ;
      return ( BUILD_NOMUNCH ) ;
   } /* endif */

   /* Verify existence of affix file. */
   if ( access ( AffixFile, 4 ) ) {
      _chdrive ( CurrentDrive ) ;
      _chdir ( "\\" ) ;
      _chdir ( CurrentDirectory ) ;
      return ( BUILD_NOAFFIX ) ;
   } /* endif */

   /* Verify writability of hash file. */
   /* If hash file does exist, and it does not have write permission . . . */
   if ( !access ( HashFile, 0 ) AND access ( HashFile, 2 ) ) {
      _chdrive ( CurrentDrive ) ;
      _chdir ( "\\" ) ;
      _chdir ( CurrentDirectory ) ;
      return ( BUILD_NOHASH ) ;
   } /* endif */

   /* Find BUILDHASH. */
   _searchenv ( "BUILDHASH.EXE", "PATH", Path ) ;
   if ( Path[0] == 0 )
      _searchenv ( "BUILDHAS.EXE", "PATH", Path ) ;

   /* If found, execute it. */
   if ( Path[0] ) {
      char Command [256] ;
      sprintf ( Command, "%s -s %s %s %s", Path, MunchFile, AffixFile, HashFile ) ;
      printf ( "About to shell BUILDHASH.\n" ) ;
      if ( system ( Command ) )
         Result = BUILD_EXEERR ;
      printf ( "Done.\n" ) ;
   } else
      Result = BUILD_NOEXE ;

   /* Return to original drive and directory. */
   _chdrive ( CurrentDrive ) ;
   _chdir ( "\\" ) ;
   _chdir ( CurrentDirectory ) ;

   /* Return the final result. */
   return ( Result ) ;
}


/****************************************************************************
 *                                                                          *
 *  Remove Dictionary from working directory.                               *
 *                                                                          *
 ****************************************************************************/

static int Speller_RemoveDictionary ( char *Path ) {

   /* Build the hash file name. */
   char *Filename = MakeHashName ( Path ) ;

   /* Try to delete the file.  Return the status of the removal. */
   return ( remove ( Filename ) ) ;
}


/****************************************************************************
 *                                                                          *
 *  Initialization function taken from ISPELL.C.                            *
 *                                                                          *
 ****************************************************************************/

static void initckch ( char *wchars ) {
   register ichar_t c;
   char num[4];
   for ( c = 0; c < ( ichar_t ) ( SET_SIZE + hashheader.nstrchars ); ++c ) {
      if ( iswordch ( c ) ) {
         if ( !mylower ( c ) ) {
            Try[Trynum] = c;
            ++Trynum;
         }
      } else if ( isboundarych ( c ) ) {
         Try[Trynum] = c;
         ++Trynum;
      }
   }
   if ( wchars != NULL ) {
      while ( Trynum < SET_SIZE AND *wchars != '\0' ) {
         if ( *wchars != 'n' AND *wchars != '\\' ) {
            c = *wchars;
            ++wchars;
         } else {
            ++wchars;
            num[0] = '\0';
            num[1] = '\0';
            num[2] = '\0';
            num[3] = '\0';
            if ( isdigit ( wchars[0] ) ) {
               num[0] = wchars[0];
               if ( isdigit ( wchars[1] ) ) {
                  num[1] = wchars[1];
                  if ( isdigit ( wchars[2] ) )
                     num[2] = wchars[2];
               }
            }
            if ( wchars[-1] == 'n' ) {
               wchars += strlen ( num );
               c = atoi ( num );
            } else {
               wchars += strlen ( num );
               c = 0;
               if ( num[0] )
                  c = num[0] - '0';
               if ( num[1] ) {
                  c <<= 3;
                  c += num[1] - '0';
               }
               if ( num[2] ) {
                  c <<= 3;
                  c += num[2] - '0';
               }
            }
         }
         c &= NOPARITY;
         if ( !hashheader.wordchars[c] ) {
            hashheader.wordchars[c] = 1;
            hashheader.sortorder[c] = hashheader.sortval++;
            Try[Trynum] = c;
            ++Trynum;
         }
      }
   }
}

/****************************************************************************
 *                                                                          *
 *  Determine default dictionary file name.                                 *
 *                                                                          *
 ****************************************************************************/

static char *MakeHashName ( char *Path ) {

   /* Local Declarations */
   char Filename [_MAX_PATH] ;
   char *p ;

   /* If it is a full path, use it as is. */
   if ( index ( Path, PATHSEPARATOR ) )
      strcpy ( Filename, Path );

   /* Else, */
   else {

      /* Is ISPELL specified in the environment? */
      p = getenv ( HOME ) ;

      /* If so, prepend it to the path given. */
      if ( p )
         sprintf ( Filename, "%s%c%s", p, PATHSEPARATOR, Path ) ;

      /* Else prepend the default library path instead. */
      else
         sprintf ( Filename, "%s%c%s", LIBDIR, PATHSEPARATOR, Path );

   } /* endif */

   /* Does the path have an extension? */
   p = rindex ( Path, '.' );

   /* If not, or not HSH nor HASH, then add on the default hashfile extension. */
   if ( p == NULL || ( FILENAMECOMPARE ( p, HASHSUFFIX ) && FILENAMECOMPARE ( p, ".hash" ) ) ) {
      char TestName [80] ;
      strcpy ( TestName, Filename ) ;
      strcat ( TestName, HASHSUFFIX ) ;
      if ( access ( TestName, 0 ) ) {
         strcpy ( TestName, Filename ) ;
         strcat ( TestName, ".hash" ) ;
         if ( access ( TestName, 0 ) ) {
            strcat ( Filename, HASHSUFFIX ) ;
         } else {
            strcat ( Filename, ".hash" ) ;
         } /* endif */
      } else {
         strcat ( Filename, HASHSUFFIX ) ;
      } /* endif */
   } /* endif */

   /* Get the full path. */
   static char FullPath [_MAX_PATH] ;
   _fullpath ( FullPath, Filename, sizeof(FullPath) ) ;

   /* Return the address of the newly constructed hash file name. */
   return ( FullPath ) ;
}


/****************************************************************************
 *                                                                          *
 *  Enumerate available dictionary files.                                   *
 *                                                                          *
 *  Parms:                                                                  *
 *    First - If non-zero, then the first dictionary in the working         *
 *              directory is returned.  Otherwise, the next dictionary      *
 *              is returned.                                                *
 *                                                                          *
 *  Returns:                                                                *
 *    char* pointer to the name of a dictionary file.  It is never a        *
 *    full path, but rather just the file name itself.                      *
 *                                                                          *
 ****************************************************************************/

static char *EnumDictionaries ( int First ) {

   static HDIR hDir = -1 ;
   static FILEFINDBUF3 Found ;
   static char *Suffix = ".hsh" ;
   char Path [_MAX_PATH] ;
   char Drive[_MAX_DRIVE+1], Dir[_MAX_DIR+1], FName[_MAX_FNAME+1], Ext[_MAX_EXT+1] ;

   ULONG Count = 1 ;
   char *p ;

   if ( First AND ( (int)hDir >= 0 ) ) {
      DosFindClose ( hDir ) ;
      hDir = -1 ;
   } /* endif */

   if ( (int)hDir >= 0 ) {
      if ( DosFindNext ( hDir, &Found, sizeof(Found), &Count ) ) {
         DosFindClose ( hDir ) ;
         hDir = -1 ;
         return ( 0 ) ;
      } /* endif */
      _splitpath ( Found.achName, Drive, Dir, FName, Ext ) ;
      while ( strcmpi ( Ext, ".hash" ) && strcmpi ( Ext, HASHSUFFIX ) ) {
         if ( DosFindNext ( hDir, &Found, sizeof(Found), &Count ) ) {
            DosFindClose ( hDir ) ;
            hDir = -1 ;
            return ( 0 ) ;
         } /* endif */
         _splitpath ( Found.achName, Drive, Dir, FName, Ext ) ;
      } /* endwhile */
      return ( Found.achName ) ;
   } /* endif */

   p = getenv ( HOME ) ;
   if ( p ) {
      strcpy ( Path, p ) ;
   } else {
      strcpy ( Path, LIBDIR ) ;
   } /* endif */
   strcat ( Path, "\\*.*" ) ;

   hDir = HDIR_CREATE ;
   if ( DosFindFirst ( Path, &hDir, FILE_NORMAL, &Found, sizeof(Found), &Count, FIL_STANDARD ) ) {
      hDir = -1 ;
      return ( 0 ) ;
   } /* endif */

   _splitpath ( Found.achName, Drive, Dir, FName, Ext ) ;
   while ( strcmpi ( Ext, ".hash" ) && strcmpi ( Ext, HASHSUFFIX ) ) {
      if ( DosFindNext ( hDir, &Found, sizeof(Found), &Count ) ) {
         DosFindClose ( hDir ) ;
         hDir = -1 ;
         return ( 0 ) ;
      } /* endif */
      _splitpath ( Found.achName, Drive, Dir, FName, Ext ) ;
   } /* endwhile */

   return ( Found.achName ) ;
}


/****************************************************************************
 *                                                                          *
 *            Definitions & Declarations for Configure Dialog               *
 *                                                                          *
 ****************************************************************************/

typedef struct {
   HelpWindow *Help ;
   HWND ErrorField ;
   char CurrentDictionary [_MAX_PATH] ;
   char WordList          [_MAX_PATH] ;
   char AffixFile         [_MAX_PATH] ;
   char DictFile          [_MAX_PATH] ;
} CONFIGURE_PARMS, *PCONFIGURE_PARMS ;

static FNWP Configure_Processor ;
static FNWP Configure_InitDlg ;
static FNWP Configure_Destroy ;
static FNWP Configure_Control ;
static FNWP Configure_Command ;
static FNWP Configure_Build ;
static FNWP Configure_Remove ;
static FNWP Configure_Help ;


/****************************************************************************
 *                                                                          *
 *      Refresh the Setup dialog.                                           *
 *                                                                          *
 ****************************************************************************/

static void Configure_Refresh ( HWND Window ) {

  /**************************************************************************
   * Find the instance data.                                                *
   **************************************************************************/

   PCONFIGURE_PARMS Parms = PCONFIGURE_PARMS ( WinQueryWindowPtr ( Window, QWL_USER ) ) ;

  /**************************************************************************
   * Load the list portion of the dictionary combo-box.                     *
   **************************************************************************/

   WinSendMsg ( WinWindowFromID(Window,IDD_CONFIGURE_DICT), LM_DELETEALL, 0, 0 ) ;
   int Count = 0 ;
   char *pDictionary = EnumDictionaries ( TRUE ) ;
   char Drive [_MAX_DRIVE], Dir[_MAX_DIR], Name[_MAX_FNAME], Ext[_MAX_EXT] ;
   while ( pDictionary ) {
      Count ++ ;
      _splitpath ( pDictionary, Drive, Dir, Name, Ext ) ;
      WinSendMsg ( WinWindowFromID(Window,IDD_CONFIGURE_DICT), LM_INSERTITEM,
         MPFROMSHORT ( LIT_SORTASCENDING ), MPFROMP ( Name ) ) ;
      pDictionary = EnumDictionaries ( FALSE ) ;
   } /* endwhile */

  /**************************************************************************
   * Get the current dictionary name, which may or may not actually exist.  *
   **************************************************************************/

   pDictionary = Parms->CurrentDictionary ;
   _splitpath ( pDictionary, Drive, Dir, Name, Ext ) ;

  /**************************************************************************
   * Select the appropriate entry in the listbox.                           *
   **************************************************************************/

   WinSetDlgItemText ( Window, IDD_CONFIGURE_DICT, PSZ("") ) ;
   WinEnableWindow ( WinWindowFromID(Window,IDD_CONFIGURE_REMOVE), FALSE ) ;
   for ( int i=0; i<Count; i++ ) {
      char Entry [_MAX_PATH] ;
      WinSendDlgItemMsg ( Window, IDD_CONFIGURE_DICT, LM_QUERYITEMTEXT,
         MPFROM2SHORT ( i, sizeof(Entry) ), MPFROMP ( Entry ) ) ;
      if ( !stricmp ( Name, Entry ) OR ( Count == 1 ) ) {
         WinSendDlgItemMsg ( Window, IDD_CONFIGURE_DICT, LM_SELECTITEM, MPFROMSHORT(i), MPFROMSHORT(TRUE) ) ;
         WinSetDlgItemText ( Window, IDD_CONFIGURE_DICT, PSZ(Entry) ) ;
         WinEnableWindow ( WinWindowFromID(Window,IDD_CONFIGURE_REMOVE), TRUE ) ;
         break ;
      } /* endif */
   } /* endfor */
}

 
/****************************************************************************
 *                                                                          *
 *      "Configure" Field Validation Function                               *
 *                                                                          *
 ****************************************************************************/

static BOOL Configure_Validate ( HWND Window ) {

  /**************************************************************************
   * Find the instance data.                                                *
   **************************************************************************/

   PCONFIGURE_PARMS Parms = PCONFIGURE_PARMS ( WinQueryWindowPtr ( Window, QWL_USER ) ) ;

  /**************************************************************************
   * Get the new dictionary name.                                           *
   **************************************************************************/

   #ifdef DEBUG
      Log ( "Configure::Validate: Getting new dictionary name." ) ;
   #endif

   char Dictionary [_MAX_PATH] ;
   WinQueryDlgItemText ( Window, IDD_CONFIGURE_DICT, sizeof(Dictionary), PSZ(Dictionary) ) ;

  /**************************************************************************
   * Try to open the dictionary.   Return FALSE if unable to do so.         *
   **************************************************************************/

   #ifdef DEBUG
      Log ( "Configure::Validate: Checking if dictionary name has changed." ) ;
   #endif
   if ( strcmpi ( Dictionary, hashname ) ) {
      #ifdef DEBUG
         Log ( "Configure::Validate: Closing old dictionary '%s'.", hashname ) ;
      #endif
      Speller_CloseDictionary ( ) ;
      #ifdef DEBUG
         Log ( "Configure::Validate: Opening new dictionary '%s'.", Dictionary ) ;
      #endif
      if ( Speller_OpenDictionary ( Dictionary ) ) {
         ResourceString Message ( Library->QueryHandle(), IDS_ERROR_CONFIGURE_BADDICT ) ;
         WinSetWindowText ( Parms->ErrorField, PSZ(Message) ) ;
         WinAlarm ( HWND_DESKTOP, WA_NOTE ) ;
         WinSetFocus ( HWND_DESKTOP, WinWindowFromID ( Window, IDD_CONFIGURE_DICT ) ) ;
         return ( FALSE ) ;
      } /* endif */
      #ifdef DEBUG
         Log ( "Configure::Validate: New dictionary opened OK." ) ;
      #endif
   } /* endif */

  /**************************************************************************
   * Save the new dictionary name.                                          *
   **************************************************************************/

   #ifdef DEBUG
      Log ( "Configure::Validate: Saving new dictionary name." ) ;
   #endif
   strcpy ( Parms->CurrentDictionary, Dictionary ) ;

  /**************************************************************************
   * Return TRUE for successful.                                            *
   **************************************************************************/

   return ( TRUE ) ;
}


/****************************************************************************
 *                                                                          *
 *      "Configure" Dialog Processor                                        *
 *                                                                          *
 ****************************************************************************/

extern MRESULT EXPENTRY Configure_Processor ( HWND Window, ULONG msg, MPARAM mp1, MPARAM mp2 ) {

  /**************************************************************************
   * Dispatch the message according to the method table and return the      *
   *   result.  Any messages not defined above get handled by the system    *
   *   default dialog processor.                                            *
   **************************************************************************/

   static METHOD Methods [] = {
      { WM_INITDLG,             Configure_InitDlg },
      { WM_DESTROY,             Configure_Destroy },
      { WM_CONTROL,             Configure_Control },
      { WM_COMMAND,             Configure_Command },
      { WM_HELP,                Configure_Help    },
   } ;

   MRESULT Result = DispatchMessage ( Window, msg, mp1, mp2, Methods, sizeof(Methods)/sizeof(Methods[0]), WinDefDlgProc ) ;

   return ( Result ) ;
}

 
/****************************************************************************
 *                                                                          *
 *      Initialize Dialog                                                   *
 *                                                                          *
 ****************************************************************************/

static MRESULT APIENTRY Configure_InitDlg ( HWND Window, ULONG msg, MPARAM mp1, MPARAM mp2 ) {

  /**************************************************************************
   * Get the dialog parameters.                                             *
   **************************************************************************/

   PHWND phwndErrorField = PHWND ( PVOIDFROMMP ( mp2 ) ) ;

  /**************************************************************************
   * Allocate instance data.                                                *
   **************************************************************************/

   PCONFIGURE_PARMS Parms = PCONFIGURE_PARMS ( malloc ( sizeof(CONFIGURE_PARMS) ) ) ;

   WinSetWindowPtr ( Window, QWL_USER, Parms ) ;

  /**************************************************************************
   * Create a help instance.                                                *
   **************************************************************************/

   char Drive [_MAX_DRIVE+1], Dir[_MAX_DIR+1], FName[_MAX_FNAME+1], Ext[_MAX_EXT+1] ;
   _splitpath ( Library->QueryName(), Drive, Dir, FName, Ext ) ;
   char HelpFileName [_MAX_PATH] ;
   sprintf ( HelpFileName, "%s\\%s.hlp", HomePath, FName ) ;
   ResourceString HelpTitle ( Library->QueryHandle(), IDS_HELPTITLE ) ;
   Parms->Help = new HelpWindow ( 0, LibraryHandle, ID_HELPTABLE, PSZ(HelpFileName), PSZ(HelpTitle) ) ;
   Parms->Help->Associate ( Window ) ;

  /**************************************************************************
   * Initialize the instance data.                                          *
   **************************************************************************/

   #ifdef DEBUG
      Log ( "Configure::InitDlg: Loading profile information." ) ;
   #endif

   ResourceString WordList   ( Library->QueryHandle(), IDS_BUILD_WORDS ) ;
   ResourceString AffixFile  ( Library->QueryHandle(), IDS_BUILD_AFFIX ) ;
   ResourceString DictFile   ( Library->QueryHandle(), IDS_BUILD_DICT  ) ;

   Parms->ErrorField = *phwndErrorField ;
   strcpy ( Parms->CurrentDictionary, hashname ) ;
   strcpy ( Parms->WordList,          PCHAR(WordList)  ) ;
   strcpy ( Parms->AffixFile,         PCHAR(AffixFile) ) ;
   strcpy ( Parms->DictFile,          PCHAR(DictFile)  ) ;

   Profile SetupData ( PROGRAM_NAME, WinQueryAnchorBlock(Window) ) ;
   SetupData.GetString ( "Dictionary", Parms->CurrentDictionary, sizeof(Parms->CurrentDictionary) ) ;
   SetupData.GetString ( "WordList",   Parms->WordList,          sizeof(Parms->WordList)          ) ;
   SetupData.GetString ( "AffixFile",  Parms->AffixFile,         sizeof(Parms->AffixFile)         ) ;
   SetupData.GetString ( "DictFile",   Parms->DictFile,          sizeof(Parms->DictFile)          ) ;

  /**************************************************************************
   * Refresh the dialog.                                                    *
   **************************************************************************/

   #ifdef DEBUG
      Log ( "Configure::InitDlg: Refreshing dialog information." ) ;
   #endif

   Configure_Refresh ( Window ) ;

  /**************************************************************************
   * Return all's well.  No focus change.                                   *
   **************************************************************************/

   #ifdef DEBUG
      Log ( "Configure::InitDlg: Done." ) ;
   #endif

   return ( WinDefDlgProc ( Window, msg, mp1, mp2 ) ) ;
}
 

/****************************************************************************
 *                                                                          *
 *      Process Destroy Message                                             *
 *                                                                          *
 ****************************************************************************/

static MRESULT APIENTRY Configure_Destroy ( HWND Window, ULONG msg, MPARAM mp1, MPARAM mp2 ) {

  /**************************************************************************
   * Find the instance data.                                                *
   **************************************************************************/

   PCONFIGURE_PARMS Parms = PCONFIGURE_PARMS ( WinQueryWindowPtr ( Window, QWL_USER ) ) ;

  /**************************************************************************
   * Destroy the help instance.                                             *
   **************************************************************************/

   delete Parms->Help ;

  /**************************************************************************
   * Release the instance data.                                             *
   **************************************************************************/

   free ( Parms ) ;

  /**************************************************************************
   * Return no error.                                                       *
   **************************************************************************/

   return ( MRFROMSHORT ( FALSE ) ) ;
}


/****************************************************************************
 *                                                                          *
 *      Process Control Messages                                            *
 *                                                                          *
 ****************************************************************************/

static MRESULT APIENTRY Configure_Control ( HWND Window, ULONG msg, MPARAM mp1, MPARAM mp2 ) {

  /**************************************************************************
   * Find the instance data.                                                *
   **************************************************************************/

   PCONFIGURE_PARMS Parms = PCONFIGURE_PARMS ( WinQueryWindowPtr ( Window, QWL_USER ) ) ;

  /**************************************************************************
   * Decode the message.  Find out what control sent it, and what the       *
   *   control had to say.                                                  *
   **************************************************************************/

   SHORT Id = SHORT1FROMMP ( mp1 ) ;
   SHORT Message = SHORT2FROMMP ( mp1 ) ;

  /**************************************************************************
   * If we got a page selection pending, process it directly.               *
   **************************************************************************/

   if ( Message == BKN_PAGESELECTEDPENDING ) {

      #ifdef DEBUG
         Log ( "Configure::Control: Notified of page selection pending." ) ;
      #endif

      // Clear the error field.
      WinSetWindowText ( Parms->ErrorField, PSZ("") ) ;

      // If cannot validate what's in the dialog, abort the page change.
      if ( !Configure_Validate ( Window ) ) {
         PAGESELECTNOTIFY *pInfo = (PAGESELECTNOTIFY*) PVOIDFROMMP(mp2) ;
         pInfo->ulPageIdNew = 0 ;
      } /* endif */

      // Save new profile.
      Profile SetupData ( PROGRAM_NAME, WinQueryAnchorBlock(Window) ) ;
      SetupData.PutString ( "Dictionary", Parms->CurrentDictionary ) ;
      SetupData.PutString ( "WordList",   Parms->WordList ) ;
      SetupData.PutString ( "AffixFile",  Parms->AffixFile ) ;
      SetupData.PutString ( "DictFile",   Parms->DictFile ) ;

      // Return at once.
      return ( 0 ) ;

   } /* endif */

  /**************************************************************************
   * If we got a page selection notification, process it directly.          *
   **************************************************************************/

   if ( Message == BKN_PAGESELECTED ) {

      #ifdef DEBUG
         Log ( "Configure::Control: Notified of page selection." ) ;
      #endif

      WinSetFocus ( HWND_DESKTOP, WinWindowFromID(Window,IDD_CONFIGURE_DICT) ) ;
      WinSendMsg ( Parms->Help->QueryHandle(), HM_SET_ACTIVE_WINDOW, MPFROMHWND(Window), MPFROMHWND(Window) ) ;

      return ( 0 ) ;

   } /* endif */

  /**************************************************************************
   * Process notification according to which control sent it.               *
   **************************************************************************/

   switch ( Id ) {

      case IDD_CONFIGURE_DICT: {

         switch ( Message ) {

            case CBN_ENTER: {
               #ifdef DEBUG
                  Log ( "Configure::Control: Notified of new dictionary selection." ) ;
               #endif
               Configure_Validate ( Window ) ;
               break ; } /* endcase CBN_ENTER */

            case CBN_EFCHANGE: {
               #ifdef DEBUG
                  Log ( "Configure::Control: Notified of entryfield change." ) ;
               #endif
               char Dictionary [_MAX_PATH] ;
               WinQueryDlgItemText ( Window, IDD_CONFIGURE_DICT, sizeof(Dictionary), PSZ(Dictionary) ) ;
               WinEnableWindow ( WinWindowFromID(Window,IDD_CONFIGURE_REMOVE), Dictionary[0] != 0 ) ;
               break ; } /* endcase CBN_EFCHANGE */

         } /* endswitch Message */

         break ; } /* endcase IDD_CONFIGURE_DICT */

   } /* endswitch Id */

  /**************************************************************************
   * Return no error.                                                       *
   **************************************************************************/

   return ( 0 ) ;
}

 
/****************************************************************************
 *                                                                          *
 *      Process commands received by the dialog.                            *
 *                                                                          *
 ****************************************************************************/

static MRESULT APIENTRY Configure_Command ( HWND Window, ULONG msg, MPARAM mp1, MPARAM mp2 ) {

   switch ( SHORT1FROMMP(mp1) ) {

      case IDD_CONFIGURE_BUILD:
         return ( Configure_Build ( Window, msg, mp1, mp2 ) ) ;

      case IDD_CONFIGURE_REMOVE:
         return ( Configure_Remove ( Window, msg, mp1, mp2 ) ) ;

   } /* endswitch */

   return ( WinSendMsg ( WinQueryWindow(QW_PARENT,Window), msg, mp1, mp2 ) ) ; 

}


/****************************************************************************
 *                                                                          *
 *            Definitions & Declarations for Build Dialog                   *
 *                                                                          *
 ****************************************************************************/

typedef struct {
   HelpWindow *Help ;
   char WordList          [_MAX_PATH] ;
   char AffixFile         [_MAX_PATH] ;
   char DictFile          [_MAX_PATH] ;
   int  Busy ;
   HWND Dialog ;
} BUILD_PARMS, *PBUILD_PARMS ;

static FNWP Build_Processor ;
static FNWP Build_InitDlg ;
static FNWP Build_Destroy ;
static FNWP Build_Control ;
static FNWP Build_ControlPointer ;
static FNWP Build_Command ;
static FNWP Build_FindWords ;
static FNWP Build_FindAffix ;
static FNWP Build_OK ;
static FNWP Build_Escape ;
static FNWP Build_BuildDone ;

static void _Optlink BuildThread ( void *Parameter ) ;


/****************************************************************************
 *                                                                          *
 *      Process the Build button.                                           *
 *                                                                          *
 ****************************************************************************/

static MRESULT APIENTRY Configure_Build ( HWND Window, ULONG msg, MPARAM mp1, MPARAM mp2 ) {

  /**************************************************************************
   * Find the instance data.                                                *
   **************************************************************************/

   PCONFIGURE_PARMS Parms = PCONFIGURE_PARMS ( WinQueryWindowPtr ( Window, QWL_USER ) ) ;

  /**************************************************************************
   * Save the current directory.                                            *
   **************************************************************************/

   int CurrentDrive = _getdrive ( ) ;
   char CurrentDirectory [ _MAX_PATH ] ;
   _getcwd ( CurrentDirectory, sizeof(CurrentDirectory) ) ;

  /**************************************************************************
   * Set the current directory to ISPELL path, if specified.                *
   **************************************************************************/

   char *p = getenv ( "ISPELL" ) ;
   if ( p ) {
      char Path [_MAX_PATH] ;
      char Drive[_MAX_DRIVE], Dir[_MAX_DIR], FName[_MAX_FNAME], Ext[_MAX_EXT] ;
      _fullpath ( Path, p, sizeof(Path) ) ;
      _splitpath ( p, Drive, Dir, FName, Ext ) ;
      strcat ( Dir, FName ) ;
      strcat ( Dir, Ext ) ;
      if ( Drive[0] )
         _chdrive ( Drive[0] - 'A' + 1 ) ;
      if ( Dir[0] ) {
         _chdir ( "\\" ) ;
         _chdir ( Dir ) ;
      } /* endif */
   } /* endif */

  /**************************************************************************
   * Get the parameters necessary for the dictionary build.                 *
   **************************************************************************/

   BUILD_PARMS BuildParms ;
   memset ( &BuildParms, 0, sizeof(BuildParms) ) ;
   strcpy ( BuildParms.WordList, Parms->WordList ) ;
   strcpy ( BuildParms.AffixFile, Parms->AffixFile ) ;
   strcpy ( BuildParms.DictFile, Parms->DictFile ) ;

   if ( WinDlgBox ( HWND_DESKTOP, Window, Build_Processor, Library->QueryHandle(), IDD_BUILD, &BuildParms ) ) {

      strcpy ( Parms->WordList, BuildParms.WordList ) ;
      strcpy ( Parms->AffixFile, BuildParms.AffixFile ) ;
      strcpy ( Parms->DictFile, BuildParms.DictFile ) ;

   } /* endif */

  /**************************************************************************
   * Restore the current directory.                                         *
   **************************************************************************/

   _chdrive ( CurrentDrive ) ;
   _chdir ( "\\" ) ;
   _chdir ( CurrentDirectory ) ;

  /**************************************************************************
   * Refresh the dialog.                                                    *
   **************************************************************************/

   Configure_Refresh ( Window ) ;

  /**************************************************************************
   * Done.                                                                  *
   **************************************************************************/

   return ( 0 ) ;
}

 
/****************************************************************************
 *                                                                          *
 *      Process the Remove button.                                          *
 *                                                                          *
 ****************************************************************************/

static MRESULT APIENTRY Configure_Remove ( HWND Window, ULONG msg, MPARAM mp1, MPARAM mp2 ) {

  /**************************************************************************
   * Find the instance data.                                                *
   **************************************************************************/

   PCONFIGURE_PARMS Parms = PCONFIGURE_PARMS ( WinQueryWindowPtr ( Window, QWL_USER ) ) ;

  /**************************************************************************
   * Determine what is the selected dictionary.                             *
   **************************************************************************/

   char Dictionary [_MAX_PATH] ;
   WinQueryDlgItemText ( Window, IDD_CONFIGURE_DICT, sizeof(Dictionary), PSZ(Dictionary) ) ;

   if ( Dictionary[0] == 0 ) {
      ResourceString Message ( Library->QueryHandle(), IDS_ERROR_CONFIGURE_MUSTSELECT ) ;
      WinSetWindowText ( Parms->ErrorField, PSZ(Message) ) ;
      WinAlarm ( HWND_DESKTOP, WA_NOTE ) ;
      WinSetFocus ( HWND_DESKTOP, WinWindowFromID ( Window, IDD_CONFIGURE_DICT ) ) ;
      return ( 0 ) ;
   } /* endif */

  /**************************************************************************
   * Remove the selected dictionary.                                         *
   **************************************************************************/

   if ( Speller_RemoveDictionary ( Dictionary ) ) {
      ResourceString Message ( Library->QueryHandle(), IDS_ERROR_CONFIGURE_NODICT ) ;
      WinSetWindowText ( Parms->ErrorField, PSZ(Message) ) ;
      WinAlarm ( HWND_DESKTOP, WA_NOTE ) ;
      WinSetFocus ( HWND_DESKTOP, WinWindowFromID ( Window, IDD_CONFIGURE_DICT ) ) ;
      return ( 0 ) ;
   } /* endif */

  /**************************************************************************
   * Refresh the dialog.                                                    *
   **************************************************************************/

   Configure_Refresh ( Window ) ;

   return ( 0 ) ;
}
 

/****************************************************************************
 *                                                                          *
 *      Process help requests received by the dialog.                       *
 *                                                                          *
 ****************************************************************************/

static MRESULT APIENTRY Configure_Help ( HWND Window, ULONG msg, MPARAM mp1, MPARAM mp2 ) {

  /**************************************************************************
   * Find the instance data.                                                *
   **************************************************************************/

   PCONFIGURE_PARMS Parms = PCONFIGURE_PARMS ( WinQueryWindowPtr ( Window, QWL_USER ) ) ;

  /**************************************************************************
   * Have the help manager do the rest.                                     *
   **************************************************************************/

   WinSendMsg ( Parms->Help->QueryHandle(), HM_EXT_HELP, 0, 0 ) ;

  /**************************************************************************
   * Done.                                                                  *
   **************************************************************************/

   return ( 0 ) ;
}


/****************************************************************************
 *                                                                          *
 *      "Spell Build" Dialog Processor                                      *
 *                                                                          *
 ****************************************************************************/

static MRESULT EXPENTRY Build_Processor ( HWND Window, ULONG msg, MPARAM mp1, MPARAM mp2 ) {

  /**************************************************************************
   * Dispatch the message according to the method table and return the      *
   *   result.  Any messages not defined above get handled by the system    *
   *   default dialog processor.                                            *
   **************************************************************************/

   static METHOD Methods [] = {
      { WM_INITDLG,             Build_InitDlg        },
      { WM_DESTROY,             Build_Destroy        },
      { WM_CONTROL,             Build_Control        },
      { WM_CONTROLPOINTER,      Build_ControlPointer },
      { WM_COMMAND,             Build_Command        },
      { WM_BUILDDONE,           Build_BuildDone      },
   } ;

   MRESULT Result = DispatchMessage ( Window, msg, mp1, mp2, Methods, sizeof(Methods)/sizeof(Methods[0]), WinDefDlgProc ) ;

   return ( Result ) ;
}

 
/****************************************************************************
 *                                                                          *
 *      Initialize Dialog                                                   *
 *                                                                          *
 ****************************************************************************/

static MRESULT APIENTRY Build_InitDlg ( HWND Window, ULONG msg, MPARAM mp1, MPARAM mp2 ) {

  /**************************************************************************
   * Get the dialog parameters.                                             *
   **************************************************************************/

   PBUILD_PARMS Parms  = PBUILD_PARMS ( mp2 ) ;

   WinSetWindowPtr ( Window, QWL_USER, Parms ) ;

  /**************************************************************************
   * Create a help instance.                                                *
   **************************************************************************/

   char Drive [_MAX_DRIVE+1], Dir[_MAX_DIR+1], FName[_MAX_FNAME+1], Ext[_MAX_EXT+1] ;
   _splitpath ( Library->QueryName(), Drive, Dir, FName, Ext ) ;
   char HelpFileName [_MAX_PATH] ;
   sprintf ( HelpFileName, "%s\\%s.hlp", HomePath, FName ) ;
   ResourceString HelpTitle ( Library->QueryHandle(), IDS_HELPTITLE ) ;
   Parms->Help = new HelpWindow ( 0, LibraryHandle, ID_HELPTABLE, PSZ(HelpFileName), PSZ(HelpTitle) ) ;
   Parms->Help->Associate ( Window ) ;

  /**************************************************************************
   * Center the dialog over its owner.                                      *
   **************************************************************************/

   RECTL OwnerRectangle, DialogRectangle ;
   POINTL Position ;
   WinQueryWindowRect ( WinQueryWindow(Window,QW_OWNER), &OwnerRectangle ) ;
   WinQueryWindowRect ( Window, &DialogRectangle ) ;
   Position.x = ( ( OwnerRectangle.xRight - OwnerRectangle.xLeft ) - ( DialogRectangle.xRight - DialogRectangle.xLeft ) ) / 2 + OwnerRectangle.xLeft ;
   Position.y = ( ( OwnerRectangle.yTop - OwnerRectangle.yBottom ) - ( DialogRectangle.yTop - DialogRectangle.yBottom ) ) / 2 + OwnerRectangle.yBottom ;
   WinMapWindowPoints ( WinQueryWindow(Window,QW_OWNER), HWND_DESKTOP, &Position, 1 ) ;
   WinSetWindowPos ( Window, 0, Position.x, Position.y, 0, 0, SWP_MOVE ) ;

  /**************************************************************************
   * Set the entry-field lengths.                                           *
   **************************************************************************/

   WinSendMsg ( WinWindowFromID(Window,IDD_BUILD_WORDS), EM_SETTEXTLIMIT, MPFROM2SHORT(_MAX_PATH,0), 0 ) ;
   WinSendMsg ( WinWindowFromID(Window,IDD_BUILD_AFFIX), EM_SETTEXTLIMIT, MPFROM2SHORT(_MAX_PATH,0), 0 ) ;
   WinSendMsg ( WinWindowFromID(Window,IDD_BUILD_DICT),  EM_SETTEXTLIMIT, MPFROM2SHORT(_MAX_PATH,0), 0 ) ;

  /**************************************************************************
   * Load the entry fields.                                                 *
   **************************************************************************/

   WinSetDlgItemText ( Window, IDD_BUILD_WORDS, PSZ(Parms->WordList) ) ;
   WinSetDlgItemText ( Window, IDD_BUILD_AFFIX, PSZ(Parms->AffixFile) ) ;
   WinSetDlgItemText ( Window, IDD_BUILD_DICT, PSZ(Parms->DictFile) ) ;

  /**************************************************************************
   * Clear the error field.                                                 *
   **************************************************************************/

   WinSetDlgItemText ( Window, IDD_BUILD_ERR, PSZ("") ) ;

  /**************************************************************************
   * Return all's well.  No focus change.                                   *
   **************************************************************************/

   return ( WinDefDlgProc ( Window, msg, mp1, mp2 ) ) ;
}

/****************************************************************************
 *                                                                          *
 *      Process Destroy Message                                             *
 *                                                                          *
 ****************************************************************************/

static MRESULT APIENTRY Build_Destroy ( HWND Window, ULONG msg, MPARAM mp1, MPARAM mp2 ) {

  /**************************************************************************
   * Find the instance data.                                                *
   **************************************************************************/

   PBUILD_PARMS Parms = PBUILD_PARMS ( WinQueryWindowPtr ( Window, QWL_USER ) ) ;

  /**************************************************************************
   * Destroy the help instance.                                             *
   **************************************************************************/

   delete Parms->Help ;

  /**************************************************************************
   * Return no error.                                                       *
   **************************************************************************/

   return ( MRFROMSHORT ( FALSE ) ) ;
}

/****************************************************************************
 *                                                                          *
 *      Process control messages received by the dialog.                    *
 *                                                                          *
 ****************************************************************************/

static MRESULT APIENTRY Build_Control ( HWND Window, ULONG msg, MPARAM mp1, MPARAM mp2 ) {

  /**************************************************************************
   * Find the instance data.                                                *
   **************************************************************************/

   PBUILD_PARMS Parms = PBUILD_PARMS ( WinQueryWindowPtr ( Window, QWL_USER ) ) ;

  /**************************************************************************
   * Decode the message.  Find out what control sent it, and what the       *
   *   control had to say.                                                  *
   **************************************************************************/

   SHORT Id = SHORT1FROMMP ( mp1 ) ;
   SHORT Message = SHORT2FROMMP ( mp1 ) ;

  /**************************************************************************
   * Process notification according to which control sent it.               *
   **************************************************************************/

   switch ( Id ) {

      case IDD_BUILD_WORDS: {
         switch ( Message ) {
            case EN_CHANGE: {
               WinQueryDlgItemText ( Window, IDD_BUILD_WORDS, sizeof(Parms->WordList),  PSZ(Parms->WordList) ) ;
               char Path[_MAX_PATH+1], Drive[_MAX_DRIVE+1], Dir[_MAX_DIR+1], FName[_MAX_FNAME+1], Ext[_MAX_EXT+1] ;
               _splitpath ( Parms->WordList, Drive, Dir, FName, Ext ) ;
               WinSetDlgItemText ( Window, IDD_BUILD_DICT, PSZ(FName) ) ;
               break ; } /* endcase EN_CHANGE */
         } /* endswitch Message */
         break ; } /* endcase IDD_BUILD_WORDS */

      case IDD_BUILD_AFFIX: {
         switch ( Message ) {
            case EN_CHANGE: {
               WinQueryDlgItemText ( Window, IDD_BUILD_AFFIX, sizeof(Parms->WordList),  PSZ(Parms->WordList) ) ;
               char Path[_MAX_PATH+1], Drive[_MAX_DRIVE+1], Dir[_MAX_DIR+1], FName[_MAX_FNAME+1], Ext[_MAX_EXT+1] ;
               _splitpath ( Parms->WordList, Drive, Dir, FName, Ext ) ;
               WinSetDlgItemText ( Window, IDD_BUILD_DICT, PSZ(FName) ) ;
               break ; } /* endcase EN_CHANGE */
         } /* endswitch Message */
         break ; } /* endcase IDD_BUILD_AFFIX */

   } /* endswitch Id */

  /**************************************************************************
   * Return no error.                                                       *
   **************************************************************************/

   return ( 0 ) ;
}

 
/****************************************************************************
 *                                                                          *
 *      Process control pointer messages received by the dialog.            *
 *                                                                          *
 ****************************************************************************/

static MRESULT APIENTRY Build_ControlPointer ( HWND Window, ULONG msg, MPARAM mp1, MPARAM mp2 ) {

  /**************************************************************************
   * Find the instance data.                                                *
   **************************************************************************/

   PBUILD_PARMS Parms = PBUILD_PARMS ( WinQueryWindowPtr ( Window, QWL_USER ) ) ;

  /**************************************************************************
   * Set the appropriate mouse pointer.                                     *
   **************************************************************************/

   if ( Parms->Busy ) {
      HPOINTER Hourglass = WinQuerySysPointer ( HWND_DESKTOP, SPTR_WAIT, FALSE ) ;
      return ( MRFROMP ( Hourglass ) ) ;
   } /* endif */

  /**************************************************************************
   * Return no error.                                                       *
   **************************************************************************/

   return ( MRFROMP ( mp2 ) ) ;
}

 
/****************************************************************************
 *                                                                          *
 *      Process commands received by the dialog.                            *
 *                                                                          *
 ****************************************************************************/

static MRESULT APIENTRY Build_Command ( HWND Window, ULONG msg, MPARAM mp1, MPARAM mp2 ) {

  /**************************************************************************
   * Dispatch the message without a default message processor.              *
   **************************************************************************/

   static METHOD Methods [] = {
      { IDD_BUILD_WORDSF,       Build_FindWords  },
      { IDD_BUILD_AFFIXF,       Build_FindAffix  },
      { DID_OK,                 Build_OK         },
      { DID_CANCEL,             Build_Escape     },
   } ;

   return ( DispatchMessage ( Window, SHORT1FROMMP(mp1), mp1, mp2, Methods, sizeof(Methods)/sizeof(Methods[0]), 0 ) ) ;
}

 
/****************************************************************************
 *                                                                          *
 *      Process the Find Word List button.                                  *
 *                                                                          *
 ****************************************************************************/

static MRESULT APIENTRY Build_FindWords ( HWND Window, ULONG msg, MPARAM mp1, MPARAM mp2 ) {

   PBUILD_PARMS Parms = PBUILD_PARMS ( WinQueryWindowPtr ( Window, QWL_USER ) ) ;

   PSZ TypeList[] = { 0 } ;
   char *FindWordList = "Find Word List" ;

   FILEDLG DialogData ;
   memset ( &DialogData, 0, sizeof(DialogData) ) ;
   DialogData.cbSize = sizeof(DialogData) ;
   DialogData.fl = FDS_OPEN_DIALOG | FDS_CENTER | FDS_HELPBUTTON ;
   DialogData.pszTitle = PSZ(FindWordList) ;
   strcpy ( DialogData.szFullFile, "*.dic" ) ;
   DialogData.pszIType = TypeList[0] ;
   DialogData.papszITypeList = PAPSZ ( TypeList ) ;

   WinFileDlg ( HWND_DESKTOP, Window, &DialogData ) ;

   if ( DialogData.lReturn == DID_CANCEL )
      return ( 0 ) ;

   char Path[_MAX_PATH+1], Drive[_MAX_DRIVE+1], Dir[_MAX_DIR+1], FName[_MAX_FNAME+1], Ext[_MAX_EXT+1] ;
   _splitpath ( DialogData.szFullFile, Drive, Dir, FName, Ext ) ;
   if ( Dir[strlen(Dir)-1] == '\\' ) 
      Dir[strlen(Dir)-1] = 0 ;
   strcpy ( Path, Drive ) ;
   strcat ( Path, Dir ) ;
   if ( strcmpi ( Path, HomePath ) ) {
      WinSetDlgItemText ( Window, IDD_BUILD_WORDS, PSZ(DialogData.szFullFile) ) ;
   } else {
      strcat ( FName, Ext ) ;
      WinSetDlgItemText ( Window, IDD_BUILD_WORDS, PSZ(FName) ) ;
   } /* endif */

   return ( 0 ) ;
}

 
/****************************************************************************
 *                                                                          *
 *      Process the Find Affix File button.                                 *
 *                                                                          *
 ****************************************************************************/

static MRESULT APIENTRY Build_FindAffix ( HWND Window, ULONG msg, MPARAM mp1, MPARAM mp2 ) {

   PBUILD_PARMS Parms = PBUILD_PARMS ( WinQueryWindowPtr ( Window, QWL_USER ) ) ;

   PSZ TypeList[] = { 0 } ;
   char *FindAffixFileName = "Find Language Affix File" ;

   FILEDLG DialogData ;
   memset ( &DialogData, 0, sizeof(DialogData) ) ;
   DialogData.cbSize = sizeof(DialogData) ;
   DialogData.fl = FDS_OPEN_DIALOG | FDS_CENTER | FDS_HELPBUTTON ;
   DialogData.pszTitle = PSZ(FindAffixFileName) ;
   strcpy ( DialogData.szFullFile, "*.aff" ) ;
   DialogData.pszIType = TypeList[0] ;
   DialogData.papszITypeList = PAPSZ ( TypeList ) ;

   WinFileDlg ( HWND_DESKTOP, Window, &DialogData ) ;

   if ( DialogData.lReturn == DID_CANCEL )
      return ( 0 ) ;

   char Path[_MAX_PATH+1], Drive[_MAX_DRIVE+1], Dir[_MAX_DIR+1], FName[_MAX_FNAME+1], Ext[_MAX_EXT+1] ;
   _splitpath ( DialogData.szFullFile, Drive, Dir, FName, Ext ) ;
   strcpy ( Path, Drive ) ;
   strcat ( Path, Dir ) ;
   if ( strcmpi ( Path, HomePath ) ) {
      WinSetDlgItemText ( Window, IDD_BUILD_AFFIX, PSZ(DialogData.szFullFile) ) ;
   } else {
      strcat ( FName, Ext ) ;
      WinSetDlgItemText ( Window, IDD_BUILD_AFFIX, PSZ(FName) ) ;
   } /* endif */

   return ( 0 ) ;
}

 
/****************************************************************************
 *                                                                          *
 *      Process the OK button.                                              *
 *                                                                          *
 ****************************************************************************/

static MRESULT APIENTRY Build_OK ( HWND Window, ULONG msg, MPARAM mp1, MPARAM mp2 ) {

  /**************************************************************************
   * Get the dialog parameters.                                             *
   **************************************************************************/

   PBUILD_PARMS Parms = PBUILD_PARMS ( WinQueryWindowPtr ( Window, QWL_USER ) ) ;

  /**************************************************************************
   * Get the word list file name and validate it.                           *
   **************************************************************************/

   WinQueryDlgItemText ( Window, IDD_BUILD_WORDS, sizeof(Parms->WordList),  PSZ(Parms->WordList) ) ;

   if ( ( Parms->WordList[0] == 0 ) || access ( Parms->WordList, 4 ) ) {
      ResourceString Message ( Library->QueryHandle(), IDS_ERROR_BUILD_NOWORDLIST ) ;
      WinSetDlgItemText ( Window, IDD_BUILD_ERR, PSZ(Message) ) ;
      WinAlarm ( HWND_DESKTOP, WA_NOTE ) ;
      WinSetFocus ( HWND_DESKTOP, WinWindowFromID ( Window, IDD_BUILD_WORDS ) ) ;
      return ( 0 ) ;
   } /* endif */

  /**************************************************************************
   * Get the language affix file name and validate it.                      *
   **************************************************************************/

   WinQueryDlgItemText ( Window, IDD_BUILD_AFFIX, sizeof(Parms->AffixFile), PSZ(Parms->AffixFile) ) ;

   if ( Parms->AffixFile AND !strrchr ( Parms->AffixFile, '.' ) )
      strcat ( Parms->AffixFile, ".aff" ) ;

   if ( ( Parms->AffixFile[0] == 0 ) || access ( Parms->AffixFile, 4 ) ) {
      ResourceString Message ( Library->QueryHandle(), IDS_ERROR_BUILD_NOAFFIX ) ;
      WinSetDlgItemText ( Window, IDD_BUILD_ERR, PSZ(Message) ) ;
      WinAlarm ( HWND_DESKTOP, WA_NOTE ) ;
      WinSetFocus ( HWND_DESKTOP, WinWindowFromID ( Window, IDD_BUILD_AFFIX ) ) ;
      return ( 0 ) ;
   } /* endif */

  /**************************************************************************
   * Get the dictionary file name and validate it.                          *
   **************************************************************************/

   WinQueryDlgItemText ( Window, IDD_BUILD_DICT, sizeof(Parms->DictFile),  PSZ(Parms->DictFile) ) ;

   if ( Parms->DictFile AND Parms->DictFile[0] AND !strrchr ( Parms->DictFile, '.' ) )
      strcat ( Parms->DictFile, ".hsh" ) ;

   if ( ( Parms->DictFile[0] == 0 ) || ( !access ( Parms->DictFile, 0 ) AND access ( Parms->DictFile, 2 ) ) ) {
      ResourceString Message ( Library->QueryHandle(), IDS_ERROR_BUILD_NODICT ) ;
      WinSetDlgItemText ( Window, IDD_BUILD_ERR, PSZ(Message) ) ;
      WinAlarm ( HWND_DESKTOP, WA_NOTE ) ;
      WinSetFocus ( HWND_DESKTOP, WinWindowFromID ( Window, IDD_BUILD_DICT ) ) ;
      return ( 0 ) ;
   } /* endif */

  /**************************************************************************
   * Build the dictionary file.                                             *
   **************************************************************************/

   Parms->Busy = TRUE ;
   Parms->Dialog = Window ;
   WinShowPointer ( HWND_DESKTOP, FALSE ) ;
   WinShowPointer ( HWND_DESKTOP, TRUE ) ;

   _beginthread ( BuildThread, 0, 0x8000, Parms ) ;

  /**************************************************************************
   * Return no error.  We'll deal with the build completion status later.   *
   **************************************************************************/

   return ( 0 ) ;
}

 
/****************************************************************************
 *                                                                          *
 *      Process the Escape button.                                          *
 *                                                                          *
 ****************************************************************************/

static MRESULT APIENTRY Build_Escape ( HWND Window, ULONG msg, MPARAM mp1, MPARAM mp2 ) {

  /**************************************************************************
   * Dismiss the dialog with a failed status.                               *
   **************************************************************************/

   WinDismissDlg ( Window, FALSE ) ;

   return ( 0 ) ;
}

/****************************************************************************
 *                                                                          *
 *      Process the Build Done notification.                                *
 *                                                                          *
 ****************************************************************************/

static MRESULT APIENTRY Build_BuildDone ( HWND Window, ULONG msg, MPARAM mp1, MPARAM mp2 ) {

  /**************************************************************************
   * Get the dialog parameters.                                             *
   **************************************************************************/

   PBUILD_PARMS Parms = PBUILD_PARMS ( WinQueryWindowPtr ( Window, QWL_USER ) ) ;

  /**************************************************************************
   * Get result of build.                                                   *
   **************************************************************************/

   int Result = int ( mp1 ) ;

  /**************************************************************************
   * Reset dialog busy state.                                               *
   **************************************************************************/

   Parms->Busy = FALSE ;
   WinShowPointer ( HWND_DESKTOP, FALSE ) ;
   WinShowPointer ( HWND_DESKTOP, TRUE ) ;

  /**************************************************************************
   * Interpret and deal with the result.                                    *
   **************************************************************************/

   if ( Result ) {
      char *Messages[] = {
         PCHAR ( ResourceString ( Library->QueryHandle(), IDS_ERROR_BUILD_NOTAVAIL ) ),
         "",
         PCHAR ( ResourceString ( Library->QueryHandle(), IDS_ERROR_BUILD_NOWORDLIST ) ),
         PCHAR ( ResourceString ( Library->QueryHandle(), IDS_ERROR_BUILD_NOAFFIX ) ),
         PCHAR ( ResourceString ( Library->QueryHandle(), IDS_ERROR_BUILD_NODICT ) ),
         PCHAR ( ResourceString ( Library->QueryHandle(), IDS_ERROR_BUILD_NOBUILDER ) ),
         PCHAR ( ResourceString ( Library->QueryHandle(), IDS_ERROR_BUILD_BUILDERR ) ),
      } ;
      WinSetDlgItemText ( Window, IDD_BUILD_ERR, PSZ(Messages[Result+1]) ) ;
      WinAlarm ( HWND_DESKTOP, WA_NOTE ) ;
      WinSetFocus ( HWND_DESKTOP, WinWindowFromID ( Window, IDD_BUILD_WORDS ) ) ;
      return ( 0 ) ;
   } /* endif */

   WinDismissDlg ( Window, TRUE ) ;

   return ( 0 ) ;
}

/****************************************************************************
 *                                                                          *
 *      Thread for building dictionary.                                     *
 *                                                                          *
 ****************************************************************************/

static void _Optlink BuildThread ( void *Parameter ) {
   PBUILD_PARMS Parms = PBUILD_PARMS ( Parameter ) ;
   HAB Anchor = WinInitialize ( 0 ) ;
   HMQ Queue = WinCreateMsgQueue ( Anchor, 0 ) ;
   int Result = Speller_BuildDictionary ( Parms->WordList, Parms->AffixFile, Parms->DictFile ) ;
   WinPostMsg ( Parms->Dialog, WM_BUILDDONE, MPFROMLONG(Result), 0 ) ;
   WinDestroyMsgQueue ( Queue ) ;
   WinTerminate ( Anchor ) ;
}
