/***************************************************************** MODULE.CPP
 *                                                                          *
 *  Classes 'Module' & 'LanguageModule': DLL wrappers                       *
 *                                                                          *
 ****************************************************************************/

#include "System.h"

#include <stdlib.h>
#include <string.h>

#include "Debug.h"
#include "Module.h"
#include "Process.h"
#include "Profile.h"
#include "ReString.h"

// #define DEBUG

/****************************************************************************
 *                                                                          *
 *  Class 'Module': Dynamic Link Library Module                             *
 *                                                                          *
 ****************************************************************************/

  // Constructor

Module::Module ( char *name, BOOL Required ) : Name(0), Handle(0) {
   if ( name && *name ) {
      Name = new char [ strlen(name) + 1 ] ;
      strcpy ( Name, name ) ;
      char Drive[_MAX_DRIVE], Dir[_MAX_DIR], Fname[_MAX_FNAME], Ext[_MAX_EXT] ;
      _splitpath ( Name, Drive, Dir, Fname, Ext ) ;
      char ModuleName [CCHMAXPATH] ;
      strcpy ( ModuleName, Drive ) ;
      strcat ( ModuleName, Dir ) ;
      strcat ( ModuleName, Fname ) ;
      if ( *Drive || *Dir )
         strcat ( ModuleName, Ext ) ;
      strupr ( ModuleName ) ;
      char MissingModule [CCHMAXPATH] ;
      APIRET Status = DosLoadModule ( PSZ(MissingModule), sizeof(MissingModule), PSZ(ModuleName), &Handle ) ;
      if ( Status ) {
         if ( Required ) {
            Log ( "ERROR: Unable to load module '%s' ('%s').  Status %i, missing module '%s'.", Name, ModuleName, Status, MissingModule ) ;
            Debug ( HWND_DESKTOP, "ERROR: Unable to load module '%s' ('%s').  Status %i, missing module '%s'.", Name, ModuleName, Status, MissingModule ) ;
            abort ( ) ;
         #ifdef DEBUG
         } else {
            Log ( "WARNING: Unable to load module '%s' ('%s').  Status %i, missing module '%s'.", Name, ModuleName, Status, MissingModule ) ;
         #endif
         } /* endif */
      } /* endif */
   } /* endif */
   #ifdef DEBUG
      Log ( "Module(%08p)::Module  Name '%s', Handle %08X.", this, ( Name && *Name ) ? Name : "???", Handle ) ;
   #endif
} /* endmethod */


  // Copy Constructor

Module::Module ( const Module &object ) : Name(0), Handle(0) {

   Name = new char [ strlen(object.Name) + 1 ] ;
   strcpy ( Name, object.Name ) ;

   if ( object.Handle ) {
      char Drive[_MAX_DRIVE], Dir[_MAX_DIR], Fname[_MAX_FNAME], Ext[_MAX_EXT] ;
      _splitpath ( Name, Drive, Dir, Fname, Ext ) ;
      char ModuleName [CCHMAXPATH] ;
      strcpy ( ModuleName, Drive ) ;
      strcat ( ModuleName, Dir ) ;
      strcat ( ModuleName, Fname ) ;
      if ( *Drive || *Dir )
         strcat ( ModuleName, Ext ) ;
      strupr ( ModuleName ) ;
      char MissingModule [CCHMAXPATH] ;
      APIRET Status = DosLoadModule ( PSZ(MissingModule), sizeof(MissingModule), PSZ(ModuleName), &Handle ) ;
      if ( Status ) {
         Log ( "ERROR: Unable to load module '%s' ('%s').  Status %i, missing module '%s'.", Name, ModuleName, Status, MissingModule ) ;
         Debug ( HWND_DESKTOP, "ERROR: Unable to load module '%s' ('%s').  Status %i, missing module '%s'.", Name, ModuleName, Status, MissingModule ) ;
         abort ( ) ;
      } /* endif */
   } /* endif */

} /* endmethod */


  // Destructor

Module::~Module ( ) {

   #ifdef DEBUG
      Log ( "Module(%08p)::~Module  Name '%s', Handle %08X.", this, ( Name && *Name ) ? Name : "???", Handle ) ;
   #endif

   if ( Handle )
      DosFreeModule ( Handle ), Handle = 0 ;

   if ( Name ) 
      delete [] Name, Name = 0 ;

   #ifdef DEBUG
      Log ( "Module(%08p)::~Module  Done.", this ) ;
   #endif

} /* endmethod */


  // Query Object Address by Name

PFN Module::QueryAddress ( char *ObjectName ) {

   #ifdef DEBUG
      Log ( "Module(%08p)::QueryAddress(Name='%s')  Started.", this, ObjectName ) ;
   #endif

   PFN Address ;
   if ( DosQueryProcAddr ( Handle, 0, PSZ(ObjectName), (PFN*)&Address ) ) {
      #ifdef DEBUG
         Log ( "Module(%08p)::QueryAddress(Name='%s')  Aborted.  Object not found?", this, ObjectName ) ;
      #endif
      return ( 0 ) ;
   } /* endif */

   #ifdef DEBUG
      Log ( "Module(%08p)::QueryAddress(Name='%s')  Done.  Address %08p.", this, ObjectName, Address ) ;
   #endif
   return ( Address ) ;
}


  // Query Object Address by Ordinal

PFN Module::QueryAddress ( int Ordinal ) {

   #ifdef DEBUG
      Log ( "Module(%08p)::QueryAddress(Ordinal=%i)  Started", this, Ordinal ) ;
   #endif

   PFN Address ;
   if ( DosQueryProcAddr ( Handle, Ordinal, 0, (PFN*)&Address ) ) {
      #ifdef DEBUG
         Log ( "Module(%08p)::QueryAddress(Ordinal=%i)  Aborted.  Object not found?", this, Ordinal ) ;
      #endif
      return ( 0 ) ;
   } /* endif */

   #ifdef DEBUG
      Log ( "Module(%08p)::QueryAddress(Ordinal=%i)  Done.  Address %08p.", this, Ordinal, Address ) ;
   #endif

   return ( Address ) ;
}


/****************************************************************************
 *                                                                          *
 *  Class 'LanguageModule': Resource-only Library Module with Language Texts*
 *                                                                          *
 ****************************************************************************/

  // Constructor

LanguageModule::LanguageModule ( char *appname, char *Name, BOOL Required ) : 
   Module(Name,Required), Language(0), Codepages(0), NumberOfCodepages(0) {

   #ifdef DEBUG
      Log ( "LanguageModule(%08p)::LanguageModule  Handle %08X.", this, Handle ) ;
   #endif

   if ( Handle == 0 )
      return ;

   #ifdef DEBUG
      Log ( "LanguageModule(%08p)::LanguageModule  Checking language module identification.", this ) ;
   #endif

   PVOID Offset ;
   if ( DosGetResource ( Handle, RT_RCDATA, 1, &Offset ) ) {
      if ( Required ) {
         Log ( "ERROR: Unable to get language DLL identification." ) ;
         Process Proc ( appname, HWND_DESKTOP, Handle ) ;
         Debug ( HWND_DESKTOP, "ERROR: Unable to get language DLL identification." ) ;
      } /* endif */
      DosFreeModule ( Handle ) ;
      Handle = 0 ;
      return ;
   } /* endif */

   #ifdef DEBUG
      Log ( "LanguageModule(%08p)::LanguageModule  Checking language module type.", this ) ;
   #endif

   PCHAR DllType = PCHAR ( Offset ) ;                   
   if ( strcmpi ( DllType, "Language" ) ) {
      if ( Required ) {
         Log ( "ERROR: Incorrect DLL type: %s.  Should be 'Language'.", DllType ) ;
         Process Proc ( appname, HWND_DESKTOP, Handle ) ;
         Debug ( HWND_DESKTOP, "ERROR: Incorrect DLL type: %s.  Should be 'Language'.", DllType ) ;
      } /* endif */
      DosFreeModule ( Handle ) ;
      Handle = 0 ;
      return ;
   } /* endif */

   #ifdef DEBUG
      Log ( "LanguageModule(%08p)::LanguageModule  Checking language module application.", this ) ;
   #endif

   PCHAR AppName = DllType + strlen(DllType) + 1 ;      
   if ( strcmpi ( AppName, appname ) ) {
      if ( Required ) {
         Log ( "ERROR: Incorrect language DLL application: '%s'.  Should be '%s'.", AppName, appname ) ;
         Process Proc ( appname, HWND_DESKTOP, Handle ) ;
         Debug ( HWND_DESKTOP, "ERROR: Incorrect language DLL application: '%s'.  Should be '%s'.", AppName, appname ) ;
      } /* endif */
      DosFreeModule ( Handle ) ;
      Handle = 0 ;
      return ;
   } /* endif */

   #ifdef DEBUG
      Log ( "LanguageModule(%08p)::LanguageModule  Saving language name.", this ) ;
   #endif

   // Save the language name.
   PCHAR language = AppName + strlen(AppName) + 1 ;
   Language = new char [ strlen(language) + 1 ] ;
   strcpy ( Language, language ) ;

   #ifdef DEBUG
      Log ( "LanguageModule(%08p)::LanguageModule  Saving codepage list.", this ) ;
   #endif

   // Save the list of valid codepages.
   PUSHORT pCodePage = PUSHORT ( language + strlen(language) + 1 ) ;
   PUSHORT pus = pCodePage ;
   while ( *pus ) {
      #ifdef DEBUG
         Log ( "LanguageModule(%08p)::LanguageModule  Possible codepage %i.", this, *pus ) ;
      #endif
      pus ++ ;
   } /* endwhile */
   NumberOfCodepages = pus - pCodePage + 1 ;
   Codepages = new USHORT [ NumberOfCodepages ] ;
   memcpy ( Codepages, pCodePage, NumberOfCodepages*sizeof(USHORT) ) ;

   #ifdef DEBUG
      Log ( "LanguageModule(%08p)::LanguageModule  Done.", this ) ;
   #endif

} /* endmethod */


  // Copy Constructor

LanguageModule::LanguageModule ( const LanguageModule &object ) : 
   Language(0), Codepages(0), NumberOfCodepages(object.NumberOfCodepages) {
   Language = new char [ strlen(object.Language) + 1 ] ;
   strcpy ( Language, object.Language ) ;
   Codepages = new USHORT [ NumberOfCodepages ] ;
   memcpy ( Codepages, object.Codepages, NumberOfCodepages*sizeof(USHORT) ) ;
} /* endmethod */


  // Destructor

LanguageModule::~LanguageModule ( ) {
   if ( Language ) delete [] Language, Language = 0 ;
   if ( Codepages ) delete [] Codepages, Codepages = 0 ;
} /* endmethod */


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
   memcpy ( DefaultLanguage, Language, min(strlen(Language),sizeof(DefaultLanguage)-1) ) ;
   DefaultLanguage [ sizeof(DefaultLanguage) - 1 ] = 0 ;

  /**************************************************************************
   * Find out what language libraries are available.                        *
   **************************************************************************/

   int LanguageCount = 0 ;
   char Languages [100] [80] ;
   char Libraries [100] [CCHMAXPATH] ;

   FILEFINDBUF3 Found ;
   HDIR hDir = (HDIR) HDIR_CREATE ;
   ULONG Count = 1 ;

   if ( !DosFindFirst2 ( "*.DLL", &hDir, FILE_NORMAL, &Found, sizeof(Found), &Count, FIL_STANDARD ) ) {

      do {

         // Is it a valid language DLL?  Continue if not.
         LanguageModule *Library = new LanguageModule ( Application, Found.achName, FALSE ) ;
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
         strcpy ( Libraries[LanguageCount], Found.achName ) ;
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
   * Failing to match the first time, query the application profile to find *
   *   out the current default language.                                    *
   **************************************************************************/

   DefaultLanguage [80] = 0 ;
   Profile IniFile ( PSZ(Application), 0, "." ) ;
   if ( IniFile.QueryHandle() ) 
      IniFile.GetString ( "Language", DefaultLanguage, sizeof(DefaultLanguage) ) ;

  /**************************************************************************
   * Try to find the default language library.                              *
   **************************************************************************/

   char DefaultLibrary [CCHMAXPATH] ;

   for ( int i=0; i<LanguageCount; i++ ) 
      if ( !strcmpi ( DefaultLanguage, Languages[i] ) ) 
         break ;

   if ( i >= LanguageCount ) {
      strcpy ( DefaultLanguage, "English" ) ;
      for ( i=0; i<LanguageCount; i++ ) 
         if ( !strcmpi ( DefaultLanguage, Languages[i] ) ) 
            break ;
   } /* endif */

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

   LanguageModule *Library = new LanguageModule ( Application, DefaultLibrary ) ;

   ResourceString revision ( Library->QueryHandle(), RevisionID ) ;
   if ( strcmp ( PCHAR(Revision), Revision ) ) {
      delete Library ;
      return ( 0 ) ;
   } /* endif */

   return ( Library ) ;
}
