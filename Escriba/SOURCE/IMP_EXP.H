/****************************************************************** IMP_EXP.H
 *                                                                          *
 *                         Import / Export Extender                         *
 *                                                                          *
 ****************************************************************************/

#ifndef IMP_EXP_H
#define IMP_EXP_H

#include "System.h"

#include "Addons.h"

class ImportExport : public Addon {

   private:
      char *FileType ;
      char *Wildcard ;
      char* (APIENTRY * pQueryFileType ) ( void ) ;
      char* (APIENTRY * pQueryWildcard ) ( void ) ;
      BOOL  (APIENTRY * pFilter ) ( char *Name, char *Type ) ;
      BOOL  (APIENTRY * pVerify ) ( HWND Owner, char *Name ) ;
      void* (APIENTRY * pImport ) ( HWND Owner, char *Name, long *Size ) ;
      BOOL  (APIENTRY * pExport ) ( HWND Owner, char *Name, void *Text, long Size ) ;

   public:

      ImportExport ( Module *Library ) ;
      ImportExport ( char *Path ) ;
      ImportExport ( char *filetype, char *wildcard,
         BOOL  (APIENTRY *pverify ) (HWND,char*),
         void* (APIENTRY *pimport ) (HWND,char*,long*),
         BOOL  (APIENTRY *pexport ) (HWND,char*,void*,long) ) ;
      ImportExport ( const ImportExport & object ) ;
      virtual ~ImportExport ( ) ;

      char *QueryFileType ( )                   { return ( pQueryFileType ? pQueryFileType() : FileType ) ; }
      char *QueryWildcard ( )                   { return ( pQueryWildcard ? pQueryWildcard() : Wildcard ) ; }
      BOOL  Filter ( char *Name, char *Type ) ;
      BOOL  Verify ( HWND Owner, char *Name )   { return ( pVerify ? pVerify ( Owner, Name ) : TRUE ) ; }
      void *Import ( HWND Owner, char *Name, long *Size )   { return ( pImport ? pImport ( Owner, Name, Size ) : 0 ) ; }
      BOOL  Export ( HWND Owner, char *Name, void *Text, long Size ) { return ( pExport ? pExport ( Owner, Name, Text, Size ) : FALSE ) ; }
} ;

class GraphicImporter : public Addon {

   private:
      char *FileType ;
      char *Wildcard ;
      char*   (APIENTRY * pQueryFileType ) ( void ) ;
      char*   (APIENTRY * pQueryWildcard ) ( void ) ;
      BOOL    (APIENTRY * pFilter ) ( char *Name, char *Type ) ;
      BOOL    (APIENTRY * pVerify ) ( HWND Owner, char *Name ) ;
      HBITMAP (APIENTRY * pImport ) ( HWND Owner, char *Name ) ;

   public:

      GraphicImporter ( Module *Library ) ;
      GraphicImporter ( char *Path ) ;
      GraphicImporter ( char *Type, char *Wildcard, BOOL (APIENTRY*pVerify)(HWND,char*), HBITMAP (APIENTRY*pImport)(HWND,char*) ) ;
      GraphicImporter ( const GraphicImporter & object ) ;
      virtual ~GraphicImporter ( ) ;

      char   *QueryFileType ( )                   { return ( pQueryFileType ? pQueryFileType() : FileType ) ; }
      char   *QueryWildcard ( )                   { return ( pQueryWildcard ? pQueryWildcard() : Wildcard ) ; }
      BOOL    Filter ( char *Name, char *Type ) ;
      BOOL    Verify ( HWND Owner, char *Name )   { return ( pVerify ? pVerify ( Owner, Name ) : TRUE ) ; }
      void   *Import ( HWND Owner, char *Name, long *Size ) ;
      HBITMAP Import ( HWND Owner, char *Name ) { return ( pImport ? pImport ( Owner, Name ) : 0 ) ; }
} ;

class MetafileImporter : public Addon {

   public:
      MetafileImporter ( ) ;
      MetafileImporter ( const MetafileImporter & object ) ;
      char   *QueryFileType ( )                   { return ( "Metafile" ) ; }
      char   *QueryWildcard ( )                   { return ( "*.MET" ) ; }
      BOOL    Filter ( char *Name, char *Type ) ;
      BOOL    Verify ( HWND Owner, char *Name ) ;
      void   *Import ( HWND Owner, char *Name, long *Size ) ;
      HMF     Import ( HWND Owner, char *Name ) ;
} ;

#endif
