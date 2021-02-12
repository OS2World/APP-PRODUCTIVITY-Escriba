// FontCache and Font object definitions.

#ifndef FONT_H
#define FONT_H

#include <stdlib.h>
#include <string.h>

#include "System.h"

#include "Debug.h"

class Font {

   private:
      char *Name ;
      int Bold:1,
          Italic:1,
          Underscore:1,
          Strikeout:1,
          Outline:1 ;
      int CodePage ;

   public:
      Font ( char *name, int bold, int italic, int underscore, int strikeout, int outline, int cp ) ;
      ~Font ( ) ;

      char *Query_Name       ( ) { return ( Name ) ; }
      BOOL  Query_Bold       ( ) { return ( Bold ) ; }
      BOOL  Query_Italic     ( ) { return ( Italic ) ; }
      BOOL  Query_Underscore ( ) { return ( Underscore ) ; }
      BOOL  Query_Strikeout  ( ) { return ( Strikeout ) ; }
      BOOL  Query_Outline    ( ) { return ( Outline ) ; }
      int   Query_CodePage   ( ) { return ( CodePage ) ; }

      void Dump ( int indent ) ;

} ; /* endclass */

class FontCache {

   private:
      char *Tag ;
      int Count ; 
      int LastID ;
      int DefaultCodePage ;
      class Font *Fonts[254] ;
      HPS ReferencePS ;
      int FontInUse ;

   public:
      FontCache ( char *tag ) ;
      ~FontCache ( ) ;

      BOOL Select ( HPS hPS, char *Name, BOOL Bold=FALSE, BOOL Italic=FALSE, BOOL Underscore=FALSE, BOOL Strikeout=FALSE, BOOL Outline=FALSE ) ;
      int  Query_Font ( ) { return ( FontInUse ) ; } 
      BOOL Set_Font ( HPS hPS, int SetID ) ;
      void Reset ( ) ;
      void Set_CodePage ( int cp ) { DefaultCodePage = cp ; }
      void Dump ( int indent ) ;

} ; /* endclass */

#endif
