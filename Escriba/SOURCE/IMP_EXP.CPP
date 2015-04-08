/**************************************************************** IMP_EXP.CPP
 *                                                                          *
 *                         Import / Export Extender                         *
 *                                                                          *
 ****************************************************************************/

#include "System.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Document.h"
#include "Escriba.h"
#include "External.h"
#include "Imp_Exp.h"
#include "ReString.h"

// #define DEBUG_DESTRUCTOR

/****************************************************************************
 *                                                                          *
 *   ImportExport: Constructor for external modules (DLL loaded)            *
 *                                                                          *
 ****************************************************************************/

ImportExport::ImportExport ( Module *library ) : Addon ( library ), FileType(0), Wildcard(0) {

   if ( Library == 0 ) 
      return ;

   pQueryFileType = (char*(APIENTRY*)(void)) Library->QueryAddress ( "QueryFileType" ) ;
   if ( pQueryFileType == 0 ) {
      delete Library ;
      Library = 0 ;
      return ;
   } /* endif */

   pQueryWildcard = (char*(APIENTRY*)(void)) Library->QueryAddress ( "QueryWildcard" ) ;
   if ( pQueryWildcard == 0 ) {
      delete Library ;
      Library = 0 ;
      return ;
   } /* endif */

   pFilter = (BOOL(APIENTRY*)(char*,char*)) Library->QueryAddress ( "Filter" ) ;
   if ( pFilter == 0 ) {
      delete Library ;
      Library = 0 ;
      return ;
   } /* endif */

   pVerify = (BOOL(APIENTRY*)(HWND,char*)) Library->QueryAddress ( "Verify" ) ;
   if ( pVerify == 0 ) {
      delete Library ;
      Library = 0 ;
      return ;
   } /* endif */

   pImport = (void*(APIENTRY*)(HWND,char*,long*)) Library->QueryAddress ( "Import" ) ;
   if ( pImport == 0 ) {
      delete Library ;
      Library = 0 ;
      return ;
   } /* endif */

   pExport = (BOOL(APIENTRY*)(HWND,char*,void*,long)) Library->QueryAddress ( "Export" ) ;
   if ( pExport == 0 ) {
      delete Library ;
      Library = 0 ;
      return ;
   } /* endif */

} /* endmethod */

/****************************************************************************
 *                                                                          *
 *   ImportExport: Constructor for external modules (Load DLL)              *
 *                                                                          *
 ****************************************************************************/

ImportExport::ImportExport ( char *Path ) : Addon ( Path ), FileType(0), Wildcard(0) {

   if ( Library == 0 ) 
      return ;

   pQueryFileType = (char*(APIENTRY*)(void)) Library->QueryAddress ( "QueryFileType" ) ;
   if ( pQueryFileType == 0 ) {
      delete Library ;
      Library = 0 ;
      return ;
   } /* endif */

   pQueryWildcard = (char*(APIENTRY*)(void)) Library->QueryAddress ( "QueryWildcard" ) ;
   if ( pQueryWildcard == 0 ) {
      delete Library ;
      Library = 0 ;
      return ;
   } /* endif */

   pFilter = (BOOL(APIENTRY*)(char*,char*)) Library->QueryAddress ( "Filter" ) ;
   if ( pFilter == 0 ) {
      delete Library ;
      Library = 0 ;
      return ;
   } /* endif */

   pVerify = (BOOL(APIENTRY*)(HWND,char*)) Library->QueryAddress ( "Verify" ) ;
   if ( pVerify == 0 ) {
      delete Library ;
      Library = 0 ;
      return ;
   } /* endif */

   pImport = (void*(APIENTRY*)(HWND,char*,long*)) Library->QueryAddress ( "Import" ) ;
   if ( pImport == 0 ) {
      delete Library ;
      Library = 0 ;
      return ;
   } /* endif */

   pExport = (BOOL(APIENTRY*)(HWND,char*,void*,long)) Library->QueryAddress ( "Export" ) ;
   if ( pExport == 0 ) {
      delete Library ;
      Library = 0 ;
      return ;
   } /* endif */

} /* endmethod */

/****************************************************************************
 *                                                                          *
 *   ImportExport: Constructor for default module (native format).          *
 *                                                                          *
 ****************************************************************************/

ImportExport::ImportExport ( 

   char            *filetype,
   char            *wildcard,
   BOOL  (APIENTRY *pverify ) (HWND,char*),
   void* (APIENTRY *pimport ) (HWND,char*,long*),
   BOOL  (APIENTRY *pexport ) (HWND,char*,void*,long)

) : Addon ( (void*)TYPE_IMPORTEXPORT ), FileType(0), Wildcard(0), 
   pQueryFileType(0), pQueryWildcard(0), pFilter(0) {

   FileType = (char*) malloc ( strlen(filetype) + 1 ) ;
   strcpy ( FileType, filetype ) ;

   Wildcard = (char*) malloc ( strlen(wildcard) + 1 ) ;
   strcpy ( Wildcard, wildcard ) ;

   pVerify = pverify ;
   pImport = pimport ;
   pExport = pexport ;
}

/****************************************************************************
 *                                                                          *
 *   ImportExport: Copy Constructor                                         *
 *                                                                          *
 ****************************************************************************/

ImportExport::ImportExport ( const ImportExport & object ) :
   Addon ( object ), FileType(0), Wildcard(0),
   pQueryFileType(object.pQueryFileType), pQueryWildcard(object.pQueryWildcard),
   pFilter(object.pFilter), pVerify(object.pVerify), 
   pImport(object.pImport), pExport(object.pExport) {

   if ( object.FileType ) {
      FileType = (char*) malloc ( strlen(object.FileType) + 1 ) ;
      strcpy ( FileType, object.FileType ) ;
   } /* endif */

   if ( object.Wildcard ) {
      Wildcard = (char*) malloc ( strlen(object.Wildcard) + 1 ) ;
      strcpy ( Wildcard, object.Wildcard ) ;
   } /* endif */

} 

/****************************************************************************
 *                                                                          *
 *   ImportExport: Destructor                                               *
 *                                                                          *
 ****************************************************************************/

ImportExport::~ImportExport ( ) {

   #ifdef DEBUG_DESTRUCTOR
      Log ( "ImportExport(%08X)::~ImportExport() Started.  FileType %s, Wildcard %s.", this, QueryFileType(), QueryWildcard() ) ;
   #endif // DEBUG_DESTRUCTOR

   if ( FileType ) 
      free ( FileType ) ;

   if ( Wildcard ) 
      free ( Wildcard ) ;

   #ifdef DEBUG_DESTRUCTOR
      Log ( "ImportExport(%08X)::~ImportExport() Done.", this ) ;
   #endif // DEBUG_DESTRUCTOR
}

/****************************************************************************
 *                                                                          *
 *   ImportExport: Filter                                                   *
 *                                                                          *
 ****************************************************************************/

BOOL ImportExport::Filter ( char *Name, char *Type ) {

   // If there is an external filter, use it.
   if ( pFilter ) 
      return ( pFilter ( Name, Type ) ) ;

   // If type matches, return TRUE.
   if ( FileType && !stricmp ( Type, FileType ) )
      return ( TRUE ) ;

   // If module type was Escriba, return TRUE if file type was Pluma.
   if ( FileType && !stricmp ( FileType, "Escriba" ) )
      if ( !stricmp ( Type, "Pluma" ) )
         return ( TRUE ) ;

   // If the wildcard is not *.*, then check the filename against it.
   if ( Wildcard && stricmp ( Wildcard, "*.*" ) ) {
      char Target [80] = { 0 } ;
      if ( !DosEditName ( 1, Name, Wildcard, Target, sizeof(Target) ) )
         if ( !stricmp ( Name, Target ) )
            return ( TRUE ) ;
   } /* endif */

   // All failed.  Return FALSE.
   return ( FALSE ) ;
}

/****************************************************************************
 *                                                                          *
 *   GraphicImporter: Constructor for external modules (DLL loaded)         *
 *                                                                          *
 ****************************************************************************/

GraphicImporter::GraphicImporter ( Module *library ) : Addon ( library ), FileType(0), Wildcard(0) {

   if ( Library == 0 ) 
      return ;

   pQueryFileType = (char*(APIENTRY*)(void)) Library->QueryAddress ( "QueryFileType" ) ;
   if ( pQueryFileType == 0 ) {
      delete Library ;
      Library = 0 ;
      return ;
   } /* endif */

   pQueryWildcard = (char*(APIENTRY*)(void)) Library->QueryAddress ( "QueryWildcard" ) ;
   if ( pQueryWildcard == 0 ) {
      delete Library ;
      Library = 0 ;
      return ;
   } /* endif */

   pFilter = (BOOL(APIENTRY*)(char*,char*)) Library->QueryAddress ( "Filter" ) ;
   if ( pFilter == 0 ) {
      delete Library ;
      Library = 0 ;
      return ;
   } /* endif */

   pVerify = (BOOL(APIENTRY*)(HWND,char*)) Library->QueryAddress ( "Verify" ) ;
   if ( pVerify == 0 ) {
      delete Library ;
      Library = 0 ;
      return ;
   } /* endif */

   pImport = (HBITMAP(APIENTRY*)(HWND,char*)) Library->QueryAddress ( "GraphicImport" ) ;
   if ( pImport == 0 ) {
      delete Library ;
      Library = 0 ;
      return ;
   } /* endif */

} /* endmethod */

/****************************************************************************
 *                                                                          *
 *   GraphicImporter: Constructor for external modules (load DLL)           *
 *                                                                          *
 ****************************************************************************/

GraphicImporter::GraphicImporter ( char *Path ) : Addon ( Path ), FileType(0), Wildcard(0) {

   if ( Library == 0 ) 
      return ;

   pQueryFileType = (char*(APIENTRY*)(void)) Library->QueryAddress ( "QueryFileType" ) ;
   if ( pQueryFileType == 0 ) {
      delete Library ;
      Library = 0 ;
      return ;
   } /* endif */

   pQueryWildcard = (char*(APIENTRY*)(void)) Library->QueryAddress ( "QueryWildcard" ) ;
   if ( pQueryWildcard == 0 ) {
      delete Library ;
      Library = 0 ;
      return ;
   } /* endif */

   pFilter = (BOOL(APIENTRY*)(char*,char*)) Library->QueryAddress ( "Filter" ) ;
   if ( pFilter == 0 ) {
      delete Library ;
      Library = 0 ;
      return ;
   } /* endif */

   pVerify = (BOOL(APIENTRY*)(HWND,char*)) Library->QueryAddress ( "Verify" ) ;
   if ( pVerify == 0 ) {
      delete Library ;
      Library = 0 ;
      return ;
   } /* endif */

   pImport = (HBITMAP(APIENTRY*)(HWND,char*)) Library->QueryAddress ( "GraphicImport" ) ;
   if ( pImport == 0 ) {
      delete Library ;
      Library = 0 ;
      return ;
   } /* endif */

} /* endmethod */

/****************************************************************************
 *                                                                          *
 *   GraphicImporter: Constructor for internally support formats.           *
 *                                                                          *
 ****************************************************************************/

GraphicImporter::GraphicImporter ( char *filetype, char *wildcard, BOOL (APIENTRY*pverify)(HWND,char*), HBITMAP (APIENTRY*pimport)(HWND,char*) ) :
   Addon ( (void*)TYPE_GRAPHICIMPORT ), FileType(0), Wildcard(0), 
   pQueryFileType(0), pQueryWildcard(0), pFilter(0) {

   FileType = (char*) malloc ( strlen(filetype) + 1 ) ;
   strcpy ( FileType, filetype ) ;

   Wildcard = (char*) malloc ( strlen(wildcard) + 1 ) ;
   strcpy ( Wildcard, wildcard ) ;

   pVerify = pverify ;
   pImport = pimport ;
}

/****************************************************************************
 *                                                                          *
 *   GraphicImporter: Copy Constructor                                      *
 *                                                                          *
 ****************************************************************************/

GraphicImporter::GraphicImporter ( const GraphicImporter & object ) : 
   Addon ( object ), FileType(0), Wildcard(0),
   pQueryFileType(object.pQueryFileType), pQueryWildcard(object.pQueryWildcard),
   pFilter(object.pFilter), pVerify(object.pVerify), pImport(object.pImport) {

   if ( object.FileType ) {
      FileType = (char*) malloc ( strlen(object.FileType) + 1 ) ;
      strcpy ( FileType, object.FileType ) ;
   } /* endif */

   if ( object.Wildcard ) {
      Wildcard = (char*) malloc ( strlen(object.Wildcard) + 1 ) ;
      strcpy ( Wildcard, object.Wildcard ) ;
   } /* endif */

} /* endmethod */

/****************************************************************************
 *                                                                          *
 *   GraphicImporter: Destructor                                            *
 *                                                                          *
 ****************************************************************************/

GraphicImporter::~GraphicImporter ( ) {

   #ifdef DEBUG_DESTRUCTOR
      Log ( "GraphicImporter(%08X)::~GraphicImporter() Started.  FileType %s, Wildcard %s.", this, QueryFileType(), QueryWildcard() ) ;
   #endif // DEBUG_DESTRUCTOR

   if ( FileType ) 
      free ( FileType ) ;

   if ( Wildcard ) 
      free ( Wildcard ) ;

   #ifdef DEBUG_DESTRUCTOR
      Log ( "GraphicImporter(%08X)::~GraphicImporter() Done.", this ) ;
   #endif // DEBUG_DESTRUCTOR
}

/****************************************************************************
 *                                                                          *
 *   GraphicImporter: Filter                                                *
 *                                                                          *
 ****************************************************************************/

BOOL GraphicImporter::Filter ( char *Name, char *Type ) {

   // If there is an external filter, use it.
   if ( pFilter ) 
      return ( pFilter ( Name, Type ) ) ;

   // If type matches, return TRUE.
   if ( FileType && !stricmp ( Type, FileType ) )
      return ( TRUE ) ;

   // If the wildcard is not *.*, then check the filename against it.
   if ( Wildcard && stricmp ( Wildcard, "*.*" ) ) {
      char Target [80] = { 0 } ;
      if ( !DosEditName ( 1, Name, Wildcard, Target, sizeof(Target) ) )
         if ( !stricmp ( Name, Target ) )
            return ( TRUE ) ;
   } /* endif */

   // All failed.  Return FALSE.
   return ( FALSE ) ;
}

/****************************************************************************
 *                                                                          *
 *   GraphicImporter: Import Graphic as Document                            *
 *                                                                          *
 ****************************************************************************/

void * GraphicImporter::Import ( HWND Owner, char *Name, long *Size ) {

   HBITMAP Bitmap = Import ( Owner, Name ) ;

   if ( Bitmap == 0 ) 
      return ( 0 ) ;

   char *Text = PCHAR ( malloc ( 100 ) ) ;

   sprintf ( Text, "%c%sCentered,Centered,MarginToMargin,MarginToMargin,KeepAspectFixX:BM%lu%c", 
      FUNCTION_START, FunctionNames[FN__GRAPHIC], Bitmap, FUNCTION_END ) ;

   strcat ( Text, "\x1A" ) ;

   *Size = strlen(Text) ;

   return ( PVOID(Text) ) ;
}

/****************************************************************************
 *                                                                          *
 *   MetafileImporter: Constructor                                          *
 *                                                                          *
 ****************************************************************************/

MetafileImporter::MetafileImporter ( ) : Addon ( (void*)TYPE_METAFILEIMPORT ) {
} /* endmethod */

/****************************************************************************
 *                                                                          *
 *   MetafileImporter: Copy Constructor                                     *
 *                                                                          *
 ****************************************************************************/

MetafileImporter::MetafileImporter ( const MetafileImporter & object ) : 
   Addon ( object ) {
} /* endmethod */

/****************************************************************************
 *                                                                          *
 *   MetafileImporter: Filter                                               *
 *                                                                          *
 ****************************************************************************/

BOOL MetafileImporter::Filter ( char *Name, char *Type ) {

   // If type matches, return TRUE.
   if ( !stricmp ( Type, "Metafile" ) ) 
      return ( TRUE ) ;

   // If the wildcard is not *.*, then check the filename against it.
   char Target [80] = { 0 } ;
   if ( !DosEditName ( 1, Name, "*.MET", Target, sizeof(Target) ) ) 
      if ( !stricmp ( Name, Target ) ) 
         return ( TRUE ) ;

   // All failed.  Return FALSE.
   return ( FALSE ) ;

} /* endmethod */

/****************************************************************************
 *                                                                          *
 *   MetafileImporter: Verify                                               *
 *                                                                          *
 ****************************************************************************/

BOOL MetafileImporter::Verify ( HWND Owner, char *Name ) {

   ResourceString Title ( LibraryHandle, IDS_TITLE ) ;

   FILE *File = fopen ( Name, "rb" ) ;
   if ( File == 0 ) {
      if ( Owner ) {
         ResourceString Format ( LibraryHandle, IDS_METAFILE_VERIFY1 ) ;
         char Message [512] ;
         sprintf ( Message, PCHAR(Format), Name ) ;
         MessageBox ( HWND_DESKTOP, Owner, PSZ(Message), PSZ(Title), IDD_ERROR_VERIFY1, MB_ENTER | MB_ICONEXCLAMATION | MB_HELP, LibraryHandle, IDS_MESSAGE_BOX_STRINGS ) ;
      } /* endif */
      return ( FALSE ) ;
   } /* endif */

   fseek ( File, 0, SEEK_END ) ;
   size_t Size = (size_t) ftell ( File ) ;
   if ( Size < 5 ) {
      if ( Owner ) {
         ResourceString Format ( LibraryHandle, IDS_METAFILE_VERIFY2 ) ;
         char Message [512] ;
         sprintf ( Message, PCHAR(Format), Name ) ;
         MessageBox ( HWND_DESKTOP, Owner, PSZ(Message), PSZ(Title), IDD_ERROR_VERIFY2, MB_ENTER | MB_ICONEXCLAMATION | MB_HELP, LibraryHandle, IDS_MESSAGE_BOX_STRINGS ) ;
      } /* endif */
      fclose ( File ) ;
      return ( FALSE ) ;
   } /* endif */

   fseek ( File, 0, SEEK_SET ) ;
   char Buffer [5] ;
   if ( fread ( Buffer, 1, 5, File ) < 5 ) {
      if ( Owner ) {
         ResourceString Format ( LibraryHandle, IDS_METAFILE_VERIFY3 ) ;
         char Message [512] ;
         sprintf ( Message, PCHAR(Format), Name ) ;
         MessageBox ( HWND_DESKTOP, Owner, PSZ(Message), PSZ(Title), IDD_ERROR_VERIFY3, MB_ENTER | MB_ICONEXCLAMATION | MB_HELP, LibraryHandle, IDS_MESSAGE_BOX_STRINGS ) ;
      } /* endif */
      fclose ( File ) ;
      return ( FALSE ) ;
   } /* endif */

   if ( ( Buffer [2] != 0xD3 ) || ( Buffer [3] != 0xA8 ) || ( Buffer [4] != 0xA8 ) ) {
      if ( Owner ) {
         ResourceString Format ( LibraryHandle, IDS_METAFILE_VERIFY4 ) ;
         char Message [512] ;
         sprintf ( Message, PCHAR(Format), Name ) ;
         MessageBox ( HWND_DESKTOP, Owner, PSZ(Message), PSZ(Title), IDD_ERROR_VERIFY4, MB_ENTER | MB_ICONEXCLAMATION | MB_HELP, LibraryHandle, IDS_MESSAGE_BOX_STRINGS ) ;
      } /* endif */
      fclose ( File ) ;
      return ( FALSE ) ;
   } /* endif */

   fclose ( File ) ;

   return ( TRUE ) ;

} /* endmethod */

/****************************************************************************
 *                                                                          *
 *   MetafileImporter: Import as graphic                                    *
 *                                                                          *
 ****************************************************************************/

HMF MetafileImporter::Import ( HWND, char *Name ) {
   HMF Metafile = GpiLoadMetaFile ( 0, PSZ(Name) ) ;
   if ( Metafile == GPI_ERROR ) {
      ERRORID Error = Sys_GetLastError ( 0 ) ;
      Log ( "PasteGraphic: Unable to load metafile '%s'.  Error %lX.", Name, Error ) ;
      return ( 0 ) ;
   } /* endif */
   return ( Metafile ) ;
} /* endmethod */

/****************************************************************************
 *                                                                          *
 *   MetafileImporter: Import as document                                   *
 *                                                                          *
 ****************************************************************************/

void *MetafileImporter::Import ( HWND Owner, char *Name, long *Size ) {

   HMF Metafile = Import ( Owner, Name ) ;

   if ( Metafile == 0 )
      return ( 0 ) ;

   char *Text = PCHAR ( malloc ( 100 ) ) ;

   sprintf ( Text, "%c%sCentered,Centered,MarginToMargin,MarginToMargin,KeepAspectFixX:MF%lu%c",
      FUNCTION_START, FunctionNames[FN__GRAPHIC], Metafile, FUNCTION_END ) ;

   strcat ( Text, "\x1A" ) ;

   *Size = strlen(Text) ;

   return ( PVOID(Text) ) ;

} /* endmethod */

