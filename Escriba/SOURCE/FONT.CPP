// Font and FontCache method functions.

#include "System.h"

#include "Debug.h"
#include "Font.h"

// #define DEBUG

Font::Font ( char *name, int bold, int italic, int underscore, int strikeout, int outline, int cp ) :
   Bold(bold), Italic(italic), Underscore(underscore), Strikeout(strikeout), Outline(outline), CodePage(cp) {
   Name = (char*) malloc ( size_t ( strlen(name) + 1 ) ) ;
   strcpy ( Name, name ) ;
} /* endmethod */

Font::~Font ( ) {
   free ( Name ) ;
} /* endmethod */

void Font::Dump ( int indent ) {
   Log ( "%*sDump of font entry:", indent, "" ) ;
   Log ( "%*s  Name:       '%s'", indent, "", Name ) ;
   Log ( "%*s  Attributes: %s%s%s%s%s", indent, "",
      Bold?"Bold ":"", Italic?"Italic ":"", Underscore?"Underscore ":"",
      Strikeout?"Strikeout ":"", Outline?"Outline ":"" ) ;
   Log ( "%*s  CodePage:   %i", indent, "", CodePage ) ;
} /* endmethod */

FontCache::FontCache ( ) : Count(0), DefaultCodePage(0), ReferencePS(0), FontInUse(0) { 
} /* endmethod */

FontCache::~FontCache ( ) {
   Reset ( ) ;
} /* endmethod */

void FontCache::Reset ( ) { 
   if ( ReferencePS && FontInUse ) 
      GpiSetCharSet ( ReferencePS, LCID_DEFAULT ) ;
   while ( Count-- ) {
      if ( !GpiDeleteSetId ( ReferencePS, Count+1 ) ) {
         char Message [512] ;
         Log ( "ERROR: Unable to delete set id %i from PS %08X.  %s", Count+1, ReferencePS, InterpretWinError(0,Message) ) ;
      } /* endif */
      delete Fonts[Count] ;
   } /* endwhile */
   Count = 0 ; ReferencePS = 0 ; 
} /* endmethod */

void FontCache::Select ( HPS hPS, char *Name, BOOL Bold, BOOL Italic, BOOL Underscore, BOOL Strikeout, BOOL Outline ) {

   if ( ReferencePS ) {
      if ( ReferencePS != hPS ) {
         Log ( "FontCache(%08p)::Select(): WARNING: Attempt to use a font cache for more than one presentation space.", this ) ;
      } /* endif */
   } else {
      ReferencePS = hPS ;
   } /* endif */

   for ( int i=0; i<Count; i++ ) 
      if ( !stricmp ( Name, Fonts[i]->Query_Name() ) ) 
         if ( Bold == Fonts[i]->Query_Bold() ) 
            if ( Italic == Fonts[i]->Query_Italic() ) 
               if ( Underscore == Fonts[i]->Query_Underscore() ) 
                  if ( Strikeout == Fonts[i]->Query_Strikeout() ) 
                     if ( Outline == Fonts[i]->Query_Outline() ) 
                        break ;

   if ( i >= Count ) {

      CHAR Facename [FACESIZE*2] ;
      strcpy ( Facename, Name ) ;
      if ( Bold )
         strcat ( Facename, " Bold" ) ;
      if ( Italic )
         strcat ( Facename, " Italic" ) ;

      USHORT CodePage = DefaultCodePage ;
      LONG Match = 0 ;
      USHORT Registry = 0 ;
      BOOL ExtendedFont = FALSE ;

      LONG FontCount = 0 ;
      if ( Bold || Italic ) {
         FontCount = GpiQueryFonts ( hPS, QF_PUBLIC, PSZ(Facename), &FontCount, sizeof(FONTMETRICS), 0 ) ;
         if ( FontCount > 0 ) {
            PFONTMETRICS pfm = PFONTMETRICS ( malloc ( size_t ( FontCount * sizeof(FONTMETRICS) ) ) ) ;
            GpiQueryFonts ( hPS, QF_PUBLIC, PSZ(Facename), &FontCount, sizeof(FONTMETRICS), pfm ) ;
            for ( int j=0; j<FontCount; j++ ) {
               if ( pfm[j].fsDefn & FM_DEFN_OUTLINE ) {
                  if ( pfm[j].usCodePage )
                     CodePage = pfm[j].usCodePage ;
                  Match = pfm[j].lMatch ;
                  Registry = pfm[j].idRegistry ;
                  break ;
               } /* endif */
            } /* endfor */
            if ( j < FontCount ) {
               ExtendedFont = TRUE ;
            } /* endif */
            free ( pfm ) ;
         } /* endif */
      } /* endif */

      if ( ! ExtendedFont ) {
         strcpy ( Facename, Name ) ;
         FontCount = 0 ;
         FontCount = GpiQueryFonts ( hPS, QF_PUBLIC, PSZ(Facename), &FontCount, sizeof(FONTMETRICS), 0 ) ;
         if ( FontCount > 0 ) {
            PFONTMETRICS pfm = PFONTMETRICS ( malloc ( size_t ( FontCount * sizeof(FONTMETRICS) ) ) ) ;
            GpiQueryFonts ( hPS, QF_PUBLIC, PSZ(Facename), &FontCount, sizeof(FONTMETRICS), pfm ) ;
            for ( int j=0; j<FontCount; j++ ) {
               if ( pfm[j].fsDefn & FM_DEFN_OUTLINE ) {
                  if ( pfm[j].usCodePage )
                     CodePage = pfm[j].usCodePage ;
                  Match = pfm[j].lMatch ;
                  Registry = pfm[j].idRegistry ;
                  break ;
               } /* endif */
            } /* endfor */
            free ( pfm ) ;
         } /* endif */
      } /* endif */

      FATTRS FontAttributes ;
      memset ( &FontAttributes, 0, sizeof(FontAttributes) ) ;
      FontAttributes.usRecordLength = sizeof(FontAttributes) ;
      if ( ! ExtendedFont ) {
         FontAttributes.fsSelection |= Bold    ? FATTR_SEL_BOLD : 0 ;
         FontAttributes.fsSelection |= Italic  ? FATTR_SEL_ITALIC : 0 ;
      } /* endif */
      FontAttributes.fsSelection |= Underscore ? FATTR_SEL_UNDERSCORE : 0 ;
      FontAttributes.fsSelection |= Strikeout  ? FATTR_SEL_STRIKEOUT : 0 ;
      FontAttributes.fsSelection |= Outline    ? FATTR_SEL_OUTLINE : 0 ;
      FontAttributes.usCodePage = CodePage ;
      FontAttributes.lMatch = Match ;
      FontAttributes.idRegistry = Registry ;
      FontAttributes.fsFontUse = FATTR_FONTUSE_OUTLINE ;
      strcpy ( FontAttributes.szFacename, Facename ) ;

      switch ( GpiCreateLogFont ( hPS, 0, i+1, &FontAttributes ) ) {
         case FONT_MATCH: {
            #ifdef DEBUG
            Log ( "FontCache(%08p)::Select(): Font '%s' created for LCID %i.  %s%s%s%s%s",
               this, Facename, i+1, Bold?"Bold ":"", Italic?"Italic ":"",
               Underscore?"Underscore ":"", Strikeout?"Strikeout ":"", Outline?"Outline ":"" ) ;
            #endif
            break; }
         case FONT_DEFAULT: {
            GpiSetCharSet ( hPS, i+1 ) ;
            FONTMETRICS FontMetrics ;
            GpiQueryFontMetrics ( hPS, sizeof(FontMetrics), &FontMetrics ) ;
            Log ( "FontCache(%08p)::Select(): Font '%s' defaulted to '%s'.", this, Facename, FontMetrics.szFacename ) ;
            break; }
         case GPI_ERROR: {
            ERRORID Error = Sys_GetLastError ( 0 ) ;
            Log ( "FontCache(%08p)::Select(): Unable to create logical font for '%s'.  Error %lX.", this, Facename, Error ) ;
            break; }
      } /* endif */

      Fonts[Count++] = new Font ( Name, int(Bold), int(Italic), int(Underscore), int(Strikeout), int(Outline), int(CodePage) ) ;

   } /* endif */

   if ( ( Fonts[i]->Query_CodePage() != GpiQueryCp ( hPS ) ) && ( Fonts[i]->Query_CodePage() != 65400 ) ) {
      GpiSetCp ( hPS, Fonts[i]->Query_CodePage() ) ;
   } else if ( DefaultCodePage != GpiQueryCp ( hPS ) ) {
      GpiSetCp ( hPS, DefaultCodePage ) ;
   } /* endif */

   if ( GpiQueryCharSet ( hPS ) != i+1 ) {
      if ( !GpiSetCharSet ( hPS, i+1 ) ) {
         ERRORID Error = Sys_GetLastError ( 0 ) ;
         Log ( "FontCache(%08p)::Select(): Unable to set logical font %i.  Error %lX.", this, i+1, Error ) ;
      } else {
         FontInUse = i + 1 ;
      } /* endif */
   } /* endif */

} /* endmethod */

BOOL FontCache::Set_Font ( HPS hPS, int SetID ) {
   if ( SetID > Count ) 
      return ( FALSE ) ;
   if ( GpiQueryCharSet ( hPS ) != SetID ) {
      if ( !GpiSetCharSet ( hPS, SetID ) ) {
         ERRORID Error = Sys_GetLastError ( 0 ) ;
         Log ( "FontCache(%08p)::Set_Font(): Unable to set logical font %i.  Error %lX.", this, SetID, Error ) ;
         return ( FALSE ) ;
      } else {
         FontInUse = SetID ;
      } /* endif */
   } /* endif */
   return ( TRUE ) ;
} /* endmethod */

void FontCache::Dump ( int indent ) {

   Log ( "%*sDump of font cache %08p:", indent, "", this ) ;
   Log ( "%*s  CodePage:  %i", indent, "", DefaultCodePage ) ;
   Log ( "%*s  PS:        %08p", indent, "", ReferencePS ) ;
   Log ( "%*s  Count:     %i", indent, "", Count ) ;
   Log ( "%*s  FontInUse: %i", indent, "", FontInUse ) ;
   for ( int i=0; i<Count; i++ ) 
      Fonts[i]->Dump ( indent + 2 ) ;

} /* endmethod */

