/**************************************************************** LMODULE.CPP
 *                                                                          *
 *  Class: Language Module                                                  *
 *                                                                          *
 ****************************************************************************/

#include "System.h"

#include <direct.h>
#include <stdlib.h>
#include <string.h>

#include "LModule.h"
#include "ReString.h"

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

extern LanguageModule *Language_Create ( char *Application, char *Revision, int RevisionID, char *Language, char *Wildcard ) {

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
   char Libraries [100] [CCHMAXPATH] ;

   FILEFINDBUF3 Found ;
   HDIR hDir = (HDIR) HDIR_CREATE ;
   ULONG Count = 1 ;

   if ( !DosFindFirst2 ( Wildcard, &hDir, FILE_NORMAL, &Found, sizeof(Found), &Count, FIL_STANDARD ) ) {

      do {

         // Is it a valid language DLL?  Continue if not.
         LanguageModule *Library = new LanguageModule ( Application, Found.achName ) ;
         if ( Library->QueryHandle() == 0 ) {
            delete Library ;
            continue ;
         } /* endif */

         // If the revision right?  Continue if not.
         ResourceString revision ( Library->QueryHandle(), RevisionID, __FILE__, __LINE__ ) ;
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
   * Try to find the default language library.                              *
   **************************************************************************/

   strcpy ( DefaultLanguage, "English" ) ;
   char DefaultLibrary [CCHMAXPATH] ;

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
 *  Set Language (must be called before anything using Library)             *
 *                                                                          *
 ****************************************************************************/

extern BOOL _SetLanguage ( char *Application, char *Revision, int RevisionID, char *Language, char *Wildcard, char *HomePath, LanguageModule* &Library ) {

  /**************************************************************************
   * Move to home directory.                                                *
   **************************************************************************/

   int OldDrive = _getdrive ( ) ;
   char OldDirectory [ CCHMAXPATH ] ;
   getcwd ( OldDirectory, sizeof(OldDirectory) ) ;
   char Drive [_MAX_DRIVE+1], Dir[_MAX_DIR+1], Fname[_MAX_FNAME+1], Ext[_MAX_EXT+1] ;
   _splitpath ( HomePath, Drive, Dir, Fname, Ext ) ;
   strcat ( Dir, Fname ) ;
   strcat ( Dir, Ext ) ;
   _chdrive ( Drive[0] - 'A' + 1 ) ;
   _chdir ( "\\" ) ;
   _chdir ( Dir ) ;

  /**************************************************************************
   * Try to create a new LanguageModule.  If successful, replace and        *
   *   discard the old one.                                                 *
   **************************************************************************/

   LanguageModule *NewLibrary = Language_Create ( Application, Revision, RevisionID, Language, Wildcard ) ;
   if ( NewLibrary ) {
      if ( Library ) 
         delete Library ;
      Library = NewLibrary ;
   } /* endif */

  /**************************************************************************
   * Return to working directory and return success.                        *
   **************************************************************************/

   _chdrive ( OldDrive ) ;
   _chdir ( "\\" ) ;
   _chdir ( OldDirectory ) ;

   return ( Library != 0 ) ;
}

