/*************************************************************** WORKPAGE.CPP
 *                                                                          *
 *                       Working Page Class Functions                       *
 *                                                                          *
 ****************************************************************************/

#include "System.h"

#include <time.h>

#include "Crc.h"
#include "DateFmt.h"
#include "Debug.h"
#include "Document.h"
#include "DocSup.h"

// #define DEBUG
// #define DEBUG_GRAPHIC
// #define DEBUG_EDIT
// #define DEBUG_INVALIDRECTANGLE
// #define DEBUG_NEEDSPAINTING
// #define DEBUG_UPDATEWORDLIST
// #define DEBUG_ASSIGNMENT

#ifdef DEBUG
   #define DEBUG_GRAPHIC
   #define DEBUG_EDIT
   #define DEBUG_INVALIDRECTANGLE
   #define DEBUG_NEEDSPAINTING
   #define DEBUG_UPDATEWORDLIST
#endif

 
/****************************************************************************
 ****************************************************************************
 **************************** Helper Functions ******************************
 ****************************************************************************
 ****************************************************************************/

/****************************************************************************
 *                                                                          *
 *  Check if token is Glyph.                                                *
 *                                                                          *
 ****************************************************************************/

extern BOOL IsGlyph ( PUCHAR pToken, PUCHAR DBCSVector ) {

   if ( IsHyphen ( *pToken ) )
      return ( TRUE ) ;

   if ( IsGlyphFunction ( pToken ) )
      return ( TRUE ) ;

   if ( IsDBCS ( *pToken, DBCSVector ) )
      return ( TRUE ) ;

   if ( IsEndOfWord ( pToken, DBCSVector ) )
      return ( FALSE ) ;

   if ( *pToken == 0 )
      return ( FALSE ) ;

   return ( TRUE ) ;
}
 
/****************************************************************************
 *                                                                          *
 *  Check if token is a Glyph Function (a function with displayable text)   *
 *                                                                          *
 ****************************************************************************/

extern BOOL IsGlyphFunction ( PUCHAR pToken ) {
   if ( IsFunction ( *pToken ) ) {
      switch ( *(pToken+1) ) {
         case FN__DATEFIXED:
         case FN__DATECURRENT:
         case FN__DATECHANGED:
         case FN__PAGECOUNT:
         case FN__PAGENUMBER:
         case FN__GLYPH:
         case FN__SEPARATOR:
            return ( TRUE ) ;
      } /* endswitch */
   } /* endif */
   return ( FALSE ) ;
}
 
/****************************************************************************
 *                                                                          *
 *  Check if token is a Graphic.                                            *
 *                                                                          *
 ****************************************************************************/

extern BOOL IsGraphic ( PUCHAR pToken ) {
   if ( IsFunction ( *pToken ) AND ( *(pToken+1) == FN__GRAPHIC ) )
      return ( TRUE ) ;
   return ( FALSE ) ;
}
 
/****************************************************************************
 *                                                                          *
 *  Check if token is an inline graphic.                                    *
 *                                                                          *
 ****************************************************************************/

extern BOOL IsGraphicInline ( PUCHAR pToken ) {
   if ( IsFunction ( *pToken ) AND ( *(pToken+1) == FN__GRAPHIC ) ) {
      POINTL Position = *PPOINTL(pToken+2) ;
      if ( Position.x == GRAPHIC_NEXT_CHAR ) 
         return ( TRUE ) ;
   } /* endif */
   return ( FALSE ) ;
}
 
/****************************************************************************
 *                                                                          *
 *  Check if token is a Separator.                                          *
 *                                                                          *
 ****************************************************************************/

extern BOOL IsSeparator ( PUCHAR pToken ) {
   if ( IsFunction ( *pToken ) AND ( *(pToken+1) == FN__SEPARATOR ) )
      return ( TRUE ) ;
   return ( FALSE ) ;
}
 
/****************************************************************************
 *                                                                          *
 *  Check if token is End-Of-Word.                                          *
 *                                                                          *
 ****************************************************************************/

extern BOOL IsEndOfWord ( PUCHAR pToken, PUCHAR DBCSVector ) {

   if ( IsEndOfLine ( pToken ) )
      return ( TRUE ) ;

   if ( IsSpace ( *pToken ) )
      return ( TRUE ) ;

   if ( IsTab ( *pToken ) )
      return ( TRUE ) ;

   if ( IsHyphen ( *pToken ) )
      return ( TRUE ) ;

   if ( IsFunction ( *pToken ) ) {
      switch ( *(pToken+1) ) {
         case FN__GLYPH:
            return ( FALSE ) ;
      } /* endswitch */
   } /* endif */

   if ( IsGlyphFunction ( pToken ) )
      return ( TRUE ) ;

   if ( IsGraphic ( pToken ) )
      return ( TRUE ) ;

   if ( IsDBCS ( *pToken, DBCSVector ) )
      return ( TRUE ) ;

   return ( FALSE ) ;
}

/****************************************************************************
 *                                                                          *
 *  Check if token is End-Of-Line.                                          *
 *                                                                          *
 ****************************************************************************/

extern BOOL IsEndOfLine ( PUCHAR pToken ) { 

   if ( IsEndOfPage ( *pToken ) ) 
      return ( TRUE ) ;

   if ( IsSeparator ( pToken ) ) 
      return ( TRUE ) ;

   return ( *pToken == '\n' ) ; 
}
 
/****************************************************************************
 *                                                                          *
 *  Check if token is DBCS.                                                 *
 *                                                                          *
 ****************************************************************************/

extern BOOL IsDBCS ( UCHAR Token, PUCHAR DBCSVector ) {

   if ( DBCSVector[0] ) {
      int Block = 0 ;
      while ( DBCSVector[Block*2+0] ) {
         if ( Token >= DBCSVector[Block*2+0] )
            if ( Token <= DBCSVector[Block*2+1] )
               return ( TRUE ) ;
         Block ++ ;
      } /* endwhile */
   } /* endif */

   return ( FALSE ) ;
}
 
/****************************************************************************
 *                                                                          *
 *  Collect tokens to form a word.                                          *
 *                                                                          *
 ****************************************************************************/

static void GetWord (
   Text *pText,         // In:  -> Text Object
   PUCHAR DBCSVector,   // In:  -> DBCS Vector
   long Offset,         // In:  Offset into text of word start.
   long &Length         // Out: Cooked length of parsed word.
) {

   // Reset word size.
   Length = 0 ;

   // Get pointer to data.  If past EOF, return with zero size.
   PUCHAR pByte ;
   if ( pText->QueryBytePtr ( Offset+Length, pByte ) == 0 ) 
      return ;

   // Until the end of word is reached . . .
   while ( 1 ) {

      // If end of word . . .
      if ( IsEndOfWord ( pByte, DBCSVector ) ) {
         if ( Length == 0 ) {                           // If word empty, 
            Length ++ ;                                 //   Then make a word out of the current token.
            if ( IsDBCS ( *pByte, DBCSVector ) ) {      //   If DBCS, 
               Length ++ ;                              //     It is two bytes long.
            } else if ( IsFunction ( *pByte ) ) {       //   Else if function token, 
               Length -- ;                              //     It needs measurement.
               long LengthIn ;
               ComputeFunctionSize ( pByte, LengthIn ) ;
               Length += LengthIn ;
            } /* endif */
         } /* endif */
         break ;

      // If not function token, then add the token to the word.
      } else if ( *pByte ) {
         Length ++ ;

      // Else, determine the function token size and add -that- to the word.
      } else {
         long LengthIn ;
         ComputeFunctionSize ( pByte, LengthIn ) ;
         Length += LengthIn ;

      } /* endif */

      // Get the next byte.
      if ( pText->QueryBytePtr ( Offset+Length, pByte ) == 0 )
         break ;

   } /* endwhile */
}


 
/****************************************************************************
 ****************************************************************************
 **************************** Class 'Grafico' *******************************
 ****************************************************************************
 ****************************************************************************/

/****************************************************************************
 *                                                                          *
 *  Grafico: Find Previous Graphic in list                                  *
 *                                                                          *
 ****************************************************************************/

Grafico *Grafico::PrevGraphic ( ) {
   PoolEntry *ThisEntry = ((PoolEntry*)this) - 1 ;
   PoolEntry *PrevEntry = ThisEntry->Query_Prev ( ) ;
   return ( (Grafico*) ( PrevEntry ? PrevEntry->Data() : 0 ) ) ;
} /* endmethod */

/****************************************************************************
 *                                                                          *
 *  Grafico: Find Next Graphic in list                                      *
 *                                                                          *
 ****************************************************************************/

Grafico *Grafico::NextGraphic ( ) {
   PoolEntry *ThisEntry = ((PoolEntry*)this) - 1 ;
   PoolEntry *NextEntry = ThisEntry->Query_Next ( ) ;
   return ( (Grafico*) ( NextEntry ? NextEntry->Data() : 0 ) ) ;
} /* endmethod */

/****************************************************************************
 *                                                                          *
 *  Grafico: Compute Original Graphic Size                                  *
 *                                                                          *
 ****************************************************************************/

void Grafico::ComputeImageSize ( WorkSpace *PS ) {
   if ( !strnicmp ( Type, "BM", 2 ) ) {
      PS->MeasureBitmap ( Handle, ImageSize ) ;
   } else if ( !strnicmp ( Type, "MF", 2 ) ) {
      RECTL ImageRectangle ;
      PS->MeasureMetafile ( Handle, ImageRectangle ) ;
      ImageSize.cx = ImageRectangle.xRight - ImageRectangle.xLeft ;
      if ( ImageSize.cx < 1 ) ImageSize.cx = 1 ;
      ImageSize.cy = ImageRectangle.yTop - ImageRectangle.yBottom ;
      if ( ImageSize.cy < 1 ) ImageSize.cy = 1 ;
   } else if ( !strnicmp ( Type, "PN", 2 ) ) {
      ImageSize.cx = 500 ; ImageSize.cy = 2500 ;
   } else {
      ImageSize.cx = ImageSize.cy = 1000 ;
   } /* endif */
} /* endmethod */

/****************************************************************************
 *                                                                          *
 *  Grafico: Compute Box                                                    *
 *                                                                          *
 ****************************************************************************/

void Grafico::ComputeBox ( Palabra *pWord, RECTL &Box, POINTL &PrinterResolution, BOOL IncludeBorders ) {
   ComputeBox ( pWord, Position, Size, Aspect, PrinterOptimize, Box, PrinterResolution, IncludeBorders ) ;
}

void Grafico::ComputeOldBox ( Palabra *pWord, RECTL &Box, POINTL &PrinterResolution, BOOL IncludeBorders ) {
   ComputeBox ( pWord, OldPosition, OldSize, OldAspect, OldPrinterOptimize, Box, PrinterResolution, IncludeBorders ) ;
}

void Grafico::ComputeBox ( Palabra *pWord, POINTL &Position, SIZEL &Size, int Aspect, int PrinterOptimize, RECTL &Box, POINTL &PrinterResolution, BOOL IncludeBorders ) {

   // Take the initial values from the graphic object itself.
   POINTL BoxPosition = Position ;
   SIZEL BoxSize = Size ;

   // Is this a printer with an odd aspect ratio?
   POINTL WorkingResolution = PrinterResolution ;
   if ( PrinterResolution.x != PrinterResolution.y ) 
      WorkingResolution.x = WorkingResolution.y = min ( PrinterResolution.x, PrinterResolution.y ) ;

   // First, let's monkey with the size . . .  This we do for all graphics identically.

   // If optimizing for printer, this takes precedence over aspect-ratio fixing.
   if ( PrinterOptimize AND !strnicmp ( Type, "BM", 2 ) ) {
      long Width  = long ( ( double(ImageSize.cx) * 1000 ) / double(WorkingResolution.x) ) ;
      if ( Width < 1 ) Width = 1 ;
      long Height = long ( ( double(ImageSize.cy) * 1000 ) / double(WorkingResolution.y) ) ;
      if ( Height < 1 ) Height = 1 ;
      int Scale ;
      if ( ( Size.cx == GRAPHIC_EDGE_TO_EDGE ) OR ( Size.cy == GRAPHIC_EDGE_TO_EDGE ) ) {
         double ScaleX = double ( State.FormInfo.xRightClip - State.FormInfo.xLeftClip ) / double ( ImageSize.cx ) ;
         double ScaleY = double ( State.FormInfo.yTopClip - State.FormInfo.yBottomClip ) / double ( ImageSize.cy ) ;
         double Scale = min ( ScaleX, ScaleY ) ;
         BoxSize.cx = long ( ImageSize.cx * Scale ) ;
         BoxSize.cy = long ( ImageSize.cy * Scale ) ;
      } else if ( ( Size.cx == GRAPHIC_MARGIN_TO_MARGIN ) OR ( Size.cy == GRAPHIC_MARGIN_TO_MARGIN ) ) {
         double ScaleX = double ( State.Margins.xRight - State.Margins.xLeft ) / double ( ImageSize.cx ) ;
         double ScaleY = double ( State.Margins.yTop - State.Margins.yBottom ) / double ( ImageSize.cy ) ;
         double Scale = min ( ScaleX, ScaleY ) ;
         BoxSize.cx = long ( ImageSize.cx * Scale ) ;
         BoxSize.cy = long ( ImageSize.cy * Scale ) ;
      } /* endif */
      switch ( Aspect ) {
         case GRAPHIC_ASPECT_UNFIXED:
            Scale = int ( BoxSize.cx / Width ) ;
            Scale = int ( min ( Scale, BoxSize.cy / Height ) ) ;
            break;
         case GRAPHIC_ASPECT_FIX_X:
            Scale = int ( BoxSize.cx / Width ) ;
            break;
         case GRAPHIC_ASPECT_FIX_Y:
            Scale = int ( BoxSize.cy / Height ) ;
            break;
      } /* endswitch */
      Scale = max ( 1, Scale ) ;
      BoxSize.cx = Width * Scale ;
      BoxSize.cy = Height * Scale ;

   // If not optimizing, but fixing aspect ratio . . .
   } else if ( ( Aspect != GRAPHIC_ASPECT_UNFIXED ) AND ( !strnicmp ( Type, "BM", 2 ) OR !strnicmp ( Type, "MF", 2 ) ) ) {
      if ( ( Size.cx == GRAPHIC_EDGE_TO_EDGE ) OR ( Size.cy == GRAPHIC_EDGE_TO_EDGE ) ) {
         double ScaleX = double ( State.FormInfo.xRightClip - State.FormInfo.xLeftClip ) / double ( ImageSize.cx ) ;
         double ScaleY = double ( State.FormInfo.yTopClip - State.FormInfo.yBottomClip ) / double ( ImageSize.cy ) ;
         double Scale = min ( ScaleX, ScaleY ) ;
         BoxSize.cx = long ( ImageSize.cx * Scale ) ;
         BoxSize.cy = long ( ImageSize.cy * Scale ) ;
      } else if ( ( Size.cx == GRAPHIC_MARGIN_TO_MARGIN ) OR ( Size.cy == GRAPHIC_MARGIN_TO_MARGIN ) ) {
         double ScaleX = double ( State.Margins.xRight - State.Margins.xLeft ) / double ( ImageSize.cx ) ;
         double ScaleY = double ( State.Margins.yTop - State.Margins.yBottom ) / double ( ImageSize.cy ) ;
         double Scale = min ( ScaleX, ScaleY ) ;
         BoxSize.cx = long ( ImageSize.cx * Scale ) ;
         BoxSize.cy = long ( ImageSize.cy * Scale ) ;
      } else if ( Aspect == GRAPHIC_ASPECT_FIX_X ) {
         double Scale = double ( BoxSize.cx ) / double ( ImageSize.cx ) ;
         BoxSize.cy = long ( ImageSize.cy * Scale ) ;
      } else if ( Aspect == GRAPHIC_ASPECT_FIX_Y ) {
         double Scale = double ( BoxSize.cy ) / double ( ImageSize.cy ) ;
         BoxSize.cx = long ( ImageSize.cx * Scale ) ;
      } /* endif */

   // Else, we're not fixing the aspect ratio.
   } else {
      switch ( Size.cx ) {
         case GRAPHIC_EDGE_TO_EDGE:
            BoxPosition.x = State.FormInfo.xLeftClip ;
            BoxSize.cx = State.FormInfo.xRightClip - State.FormInfo.xLeftClip ;
            break;
         case GRAPHIC_MARGIN_TO_MARGIN:
            BoxPosition.x = State.Margins.xLeft ;
            BoxSize.cx = State.Margins.xRight - State.Margins.xLeft ;
            break;
      } /* endswitch */
      switch ( Size.cy ) {
         case GRAPHIC_EDGE_TO_EDGE:
            BoxPosition.y = State.FormInfo.yBottomClip ;
            BoxSize.cy = State.FormInfo.yTopClip - State.FormInfo.yBottomClip ;
            break;
         case GRAPHIC_MARGIN_TO_MARGIN:
            BoxPosition.y = State.Margins.yBottom ;
            BoxSize.cy = State.Margins.yTop - State.Margins.yBottom ;
            break;
      } /* endswitch */

   } /* endif */

   // Next, let's adjust the position.  

   switch ( BoxPosition.x ) {
      case GRAPHIC_NEXT_CHAR:
         Box.xLeft = pWord ? pWord->Position.x : 0 ;
         break;
      case GRAPHIC_EDGE_LEFT:
         Box.xLeft = State.FormInfo.xLeftClip ;
         break;
      case GRAPHIC_EDGE_RIGHT:
         Box.xLeft = State.FormInfo.xRightClip - BoxSize.cx ;
         break;
      case GRAPHIC_MARGIN_LEFT:
         Box.xLeft = State.Margins.xLeft ;
         break;
      case GRAPHIC_MARGIN_RIGHT:
         Box.xLeft = State.Margins.xRight - BoxSize.cx ;
         break;
      case GRAPHIC_CENTERED:
         Box.xLeft = ( State.Margins.xRight - State.Margins.xLeft - BoxSize.cx ) / 2 + State.Margins.xLeft ;
         break;
      default: 
         Box.xLeft = BoxPosition.x ;
   } /* endswitch */

   if ( BoxPosition.x == GRAPHIC_NEXT_CHAR ) {
      Box.yBottom = pWord ? pWord->Position.y : 0 ;
   } else {
      switch ( BoxPosition.y ) {
         case GRAPHIC_EDGE_BOTTOM:
            Box.yBottom = State.FormInfo.yBottomClip ;
            break;
         case GRAPHIC_EDGE_TOP:
            Box.yBottom = State.FormInfo.yTopClip - BoxSize.cy ;
            break;
         case GRAPHIC_MARGIN_BOTTOM:
            Box.yBottom = State.Margins.yBottom ;
            break;
         case GRAPHIC_MARGIN_TOP:
            Box.yBottom = State.Margins.yTop - BoxSize.cy ;
            break;
         case GRAPHIC_CENTERED:
            Box.yBottom = ( State.Margins.yTop - State.Margins.yBottom - BoxSize.cy ) / 2 + State.Margins.yBottom ;
            break;
         default: 
            Box.yBottom = BoxPosition.y ;
      } /* endswitch */
   } /* endif */

   // Now, having computed them both, build the box.

   Box.xRight = Box.xLeft + BoxSize.cx ;
   Box.yTop = Box.yBottom + BoxSize.cy ;

   if ( IncludeBorders ) {
      Box.xLeft   -= BorderWidth ;
      Box.yBottom -= BorderWidth ;
      Box.xRight  += BorderWidth ;
      Box.yTop    += BorderWidth ;
   } /* endif */
}

/****************************************************************************
 *                                                                          *
 *  Borradora: Split Word                                                   *
 *                                                                          *
 ****************************************************************************/

void Grafico::Split ( long offset ) {
   Length -= offset - Offset ;
   Offset = offset ;
}

/****************************************************************************
 *                                                                          *
 *  Grafico: Dump                                                           *
 *                                                                          *
 ****************************************************************************/

void Grafico::Dump ( int indent ) {

   Log ( "%*sGrafico %p:", indent, "", this ) ;

   Log ( "%*sOffset, Length: %i, %i", indent, "", Offset, Length ) ;
   Log ( "%*sNeedsPainting: %s", indent, "", NeedsPainting ? "TRUE" : "FALSE" ) ;

   char Posx[32], Posy[32], Sizx[32], Sizy[32] ;
   switch ( Position.x ) {
      case GRAPHIC_NEXT_CHAR:           strcpy ( Posx, "Inline" ) ;             break ;
      case GRAPHIC_EDGE_LEFT:           strcpy ( Posx, "LeftEdge" ) ;           break ;
      case GRAPHIC_EDGE_RIGHT:          strcpy ( Posx, "RightEdge" ) ;          break ;
      case GRAPHIC_MARGIN_LEFT:         strcpy ( Posx, "LeftMargin" ) ;         break ;
      case GRAPHIC_MARGIN_RIGHT:        strcpy ( Posx, "RightMargin" ) ;        break ;
      case GRAPHIC_CENTERED:            strcpy ( Posx, "Centered" ) ;           break ;
      default:                          sprintf ( Posx, "%i", Position.x ) ;
   } /* endswitch */
   if ( Position.x == GRAPHIC_NEXT_CHAR ) {
      strcpy ( Posy, "Inline" ) ;
   } else {
      switch ( Position.y ) {
         case GRAPHIC_EDGE_TOP:            strcpy ( Posy, "TopEdge" ) ;         break ;
         case GRAPHIC_EDGE_BOTTOM:         strcpy ( Posy, "BottomEdge" ) ;      break ;
         case GRAPHIC_MARGIN_TOP:          strcpy ( Posy, "TopMargin" ) ;       break ;
         case GRAPHIC_MARGIN_BOTTOM:       strcpy ( Posy, "BottomMargin" ) ;    break ;
         case GRAPHIC_CENTERED:            strcpy ( Posy, "Centered" ) ;        break ;
         default:                          sprintf ( Posy, "%i", Position.y ) ;
      } /* endswitch */
   } /* endif */
   switch ( Size.cx ) {
      case GRAPHIC_EDGE_TO_EDGE:        strcpy ( Sizx, "EdgeToEdge" ) ;         break ;
      case GRAPHIC_MARGIN_TO_MARGIN:    strcpy ( Sizx, "MarginToMargin" ) ;     break ;
      default:                          sprintf ( Sizx, "%i", Size.cx ) ;
   } /* endswitch */
   switch ( Size.cy ) {
      case GRAPHIC_EDGE_TO_EDGE:        strcpy ( Sizy, "EdgeToEdge" ) ;         break ;
      case GRAPHIC_MARGIN_TO_MARGIN:    strcpy ( Sizy, "MarginToMargin" ) ;     break ;
      default:                          sprintf ( Sizy, "%i", Size.cy ) ;
   } /* endswitch */

   Log ( "%*sPosition, Size, CRC: %s,%s (%sx%s), %04X", indent, "", Posx, Posy, Sizx, Sizy, CRC ) ;

   switch ( OldPosition.x ) {
      case GRAPHIC_NEXT_CHAR:           strcpy ( Posx, "Inline" ) ;             break ;
      case GRAPHIC_EDGE_LEFT:           strcpy ( Posx, "LeftEdge" ) ;           break ;
      case GRAPHIC_EDGE_RIGHT:          strcpy ( Posx, "RightEdge" ) ;          break ;
      case GRAPHIC_MARGIN_LEFT:         strcpy ( Posx, "LeftMargin" ) ;         break ;
      case GRAPHIC_MARGIN_RIGHT:        strcpy ( Posx, "RightMargin" ) ;        break ;
      case GRAPHIC_CENTERED:            strcpy ( Posx, "Centered" ) ;           break ;
      default:                          sprintf ( Posx, "%i", OldPosition.x ) ;
   } /* endswitch */
   if ( OldPosition.x == GRAPHIC_NEXT_CHAR ) {
      strcpy ( Posy, "Inline" ) ;
   } else {
      switch ( OldPosition.y ) {
         case GRAPHIC_EDGE_TOP:            strcpy ( Posy, "TopEdge" ) ;            break ;
         case GRAPHIC_EDGE_BOTTOM:         strcpy ( Posy, "BottomEdge" ) ;         break ;
         case GRAPHIC_MARGIN_TOP:          strcpy ( Posy, "TopMargin" ) ;          break ;
         case GRAPHIC_MARGIN_BOTTOM:       strcpy ( Posy, "BottomMargin" ) ;       break ;
         case GRAPHIC_CENTERED:            strcpy ( Posy, "Centered" ) ;           break ;
         default:                          sprintf ( Posy, "%i", OldPosition.y ) ;
      } /* endswitch */
   } /* endif */
   switch ( OldSize.cx ) {
      case GRAPHIC_EDGE_TO_EDGE:        strcpy ( Sizx, "EdgeToEdge" ) ;         break ;
      case GRAPHIC_MARGIN_TO_MARGIN:    strcpy ( Sizx, "MarginToMargin" ) ;     break ;
      default:                          sprintf ( Sizx, "%i", OldSize.cx ) ;
   } /* endswitch */
   switch ( OldSize.cy ) {
      case GRAPHIC_EDGE_TO_EDGE:        strcpy ( Sizy, "EdgeToEdge" ) ;         break ;
      case GRAPHIC_MARGIN_TO_MARGIN:    strcpy ( Sizy, "MarginToMargin" ) ;     break ;
      default:                          sprintf ( Sizy, "%i", OldSize.cy ) ;
   } /* endswitch */

   Log ( "%*sOld Position, Size, CRC: %s,%s (%sx%s), %04X", indent, "", Posx, Posy, Sizx, Sizy, OldCRC ) ;

   Log ( "%*sType: %0.2s", indent, "", Type ) ;

   if ( !strnicmp ( Type, "BM", 2 ) ) {
      Log ( "%*sBitmap: %08X", indent, "", Handle ) ;
   } else if ( !strnicmp ( Type, "MF", 2 ) ) {
      Log ( "%*sMetafile: %08X", indent, "", Handle ) ;
   } else if ( !strnicmp ( Type, "UP", 2 ) ) {
      Log ( "%*sUPC-A: %0.1s-%0.5s-%0.5s-%0.1s", indent, "", BarCode+0, BarCode+1, BarCode+6, BarCode+11 ) ;
   } else if ( !strnicmp ( Type, "UE", 2 ) ) {
      Log ( "%*sUPC-E: %0.6s", indent, "", BarCode ) ;
   } else if ( !strnicmp ( Type, "39", 2 ) ) {
      Log ( "%*sCode39: %0.80s", indent, "", BarCode ) ;
   } else if ( !strnicmp ( Type, "PN", 2 ) ) {
      Log ( "%*sPostalCode: %0.9s", indent, "", BarCode ) ;
   } else if ( !strnicmp ( Type, "E8", 2 ) ) {
      Log ( "%*sEAN8Code: %0.7s", indent, "", BarCode ) ;
   } else if ( !strnicmp ( Type, "EA", 2 ) ) {
      Log ( "%*sEAN13Code: %0.12s", indent, "", BarCode ) ;
   } /* endif */

   if ( !strnicmp(Type,"BM",2) || !strnicmp(Type,"MF",2) ) 
      Log ( "%*sOriginal Size: %ix%i", indent, "", ImageSize.cx, ImageSize.cy ) ;

   Log ( "%*sBorderWidth: %i", indent, "", BorderWidth ) ;
   Log ( "%*sBorderColor: %06X", indent, "", BorderColor ) ;
   Log ( "%*sBorderCaption: %0.80s", indent, "", BorderCaption ) ;
   Log ( "%*sBackground: %s", indent, "", Background?"TRUE":"FALSE" ) ;
   Log ( "%*sNoTextAside: %s", indent, "", NoTextAside?"TRUE":"FALSE" ) ;
   Log ( "%*sHalftone: %s", indent, "", Halftone?"TRUE":"FALSE" ) ;
   Log ( "%*sReverse: %s", indent, "", Reverse?"TRUE":"FALSE" ) ;

   char *Aspects[] = { "Unfixed", "Fixed wrt X", "Fixed wrt Y" } ;
   Log ( "%*sAspect: %s", indent, "", Aspects[Aspect] ) ;
   Log ( "%*sOldAspect: %s", indent, "", Aspects[OldAspect] ) ;

   Log ( "%*sPrinterOptimize: %s", indent, "", PrinterOptimize?"TRUE":"FALSE" ) ;
   Log ( "%*sOldPrinterOptimize: %s", indent, "", OldPrinterOptimize?"TRUE":"FALSE" ) ;

   Log ( "%*sState:", indent, "" ) ;  State.Dump ( indent+2 ) ;
}



 
/****************************************************************************
 ****************************************************************************
 **************************** Class 'Palabra' *******************************
 ****************************************************************************
 ****************************************************************************/

/****************************************************************************
 *                                                                          *
 *  Palabra: Find Previous Word in list                                     *
 *                                                                          *
 ****************************************************************************/

Palabra *Palabra::PrevWord ( ) {
   PoolEntry *ThisEntry = ((PoolEntry*)this) - 1 ;
   PoolEntry *PrevEntry = ThisEntry->Query_Prev ( ) ;
   return ( (Palabra*) ( PrevEntry ? PrevEntry->Data() : 0 ) ) ;
} /* endmethod */

/****************************************************************************
 *                                                                          *
 *  Palabra: Find Next Word in list                                         *
 *                                                                          *
 ****************************************************************************/

Palabra *Palabra::NextWord ( ) {
   PoolEntry *ThisEntry = ((PoolEntry*)this) - 1 ;
   PoolEntry *NextEntry = ThisEntry->Query_Next ( ) ;
   return ( (Palabra*) ( NextEntry ? NextEntry->Data() : 0 ) ) ;
} /* endmethod */

/****************************************************************************
 *                                                                          *
 *  Palabra: Does this word separate other words so they cannot merge?      *
 *                                                                          *
 ****************************************************************************/

BOOL Palabra::IsSeparator ( Text *pText, PUCHAR DBCSVector ) {
   PUCHAR pByte ;
   pText->QueryBytePtr ( Offset, pByte ) ;
   #ifdef DEBUG
      Log ( "Palabra::IsSeparator: Word(%08p)'s first byte %02X.", this, *pByte ) ;
   #endif
   return ( IsEndOfWord ( pByte, DBCSVector ) ) ;
}
 
/****************************************************************************
 *                                                                          *
 *  Palabra: Does this word represent a separator bar?                      *
 *                                                                          *
 ****************************************************************************/

BOOL Palabra::IsSeparator ( Text *pText ) {
   PUCHAR pByte ;
   pText->QueryBytePtr ( Offset, pByte ) ;
   return ( ::IsSeparator ( pByte ) ) ;
}
 
/****************************************************************************
 *                                                                          *
 *  Palabra: Does this word represent an inline graphic?                    *
 *                                                                          *
 ****************************************************************************/

BOOL Palabra::IsGraphicInline ( Text *pText ) {
   PUCHAR pByte ;
   pText->QueryBytePtr ( Offset, pByte ) ;
   return ( ::IsGraphicInline ( pByte ) ) ;
}
 
/****************************************************************************
 *                                                                          *
 *  Palabra: Is this word EndOfDocument?                                    *
 *                                                                          *
 ****************************************************************************/

BOOL Palabra::IsEndOfDocument ( Text *pText ) {
   PUCHAR pByte ;
   pText->QueryBytePtr ( Offset, pByte ) ;
   #ifdef DEBUG
      Log ( "Palabra::IsEndDocument: Word(%08p)'s first byte %02X.", this, *pByte ) ;
   #endif
   return ( ::IsEndOfDocument ( *pByte ) ) ;
}
 
/****************************************************************************
 *                                                                          *
 *  Palabra: Is this word EndOfPage?                                        *
 *                                                                          *
 ****************************************************************************/

BOOL Palabra::IsEndOfPage ( Text *pText ) {
   PUCHAR pByte ;
   pText->QueryBytePtr ( Offset, pByte ) ;
   #ifdef DEBUG
      Log ( "Palabra::IsEndPage: Word(%08p)'s first byte %02X.", this, *pByte ) ;
   #endif
   return ( ::IsEndOfPage ( *pByte ) ) ;
}
 
/****************************************************************************
 *                                                                          *
 *  Palabra: Is this word EndOfLIne?                                        *
 *                                                                          *
 ****************************************************************************/

BOOL Palabra::IsEndOfLine ( Text *pText ) {
   PUCHAR pByte ;
   pText->QueryBytePtr ( Offset, pByte ) ;
   #ifdef DEBUG
      Log ( "Palabra::IsEndLine: Word(%08p)'s first byte %02X.", this, *pByte ) ;
   #endif
   return ( ::IsEndOfLine ( pByte ) ) ;
}
 
/****************************************************************************
 *                                                                          *
 *  Palabra: Is this word a Tab?                                            *
 *                                                                          *
 ****************************************************************************/

BOOL Palabra::IsTab ( Text *pText ) {
   PUCHAR pByte ;
   pText->QueryBytePtr ( Offset, pByte ) ;
   #ifdef DEBUG
      Log ( "Palabra::IsTab: Word(%08p)'s first byte %02X.", this, *pByte ) ;
   #endif
   return ( ::IsTab ( *pByte ) ) ;
}
 
/****************************************************************************
 *                                                                          *
 *  Palabra: Is this word a Space?                                          *
 *                                                                          *
 ****************************************************************************/

BOOL Palabra::IsSpace ( Text *pText ) {
   PUCHAR pByte ;
   pText->QueryBytePtr ( Offset, pByte ) ;
   #ifdef DEBUG
      Log ( "Palabra::IsSpace: Word(%08p)'s first byte %02X.", this, *pByte ) ;
   #endif
   return ( ::IsSpace ( *pByte ) ) ;
}
 
/****************************************************************************
 *                                                                          *
 *  Palabra: Does this word start a paragraph?                              *
 *                                                                          *
 ****************************************************************************/

BOOL Palabra::StartsParagraph ( Text *pText, WorkSpace *PS ) {

   // Rule 1: Separator bars -never- start a paragraph.
   PUCHAR pByte ;
   pText->QueryBytePtr ( Offset, pByte ) ;
   if ( ::IsSeparator ( pByte ) ) 
      return ( FALSE ) ;

   // Rule 2: The start of the document is always the start of a paragraph.
   if ( !Offset ) 
      return ( TRUE ) ;

   // Rule 3: If the token just before the current one represents
   //   an end-of-line token, we are starting a paragraph with the new token.
   long PrevOffset ( pText->PrevToken ( PS, Offset ) ) ;
   pText->QueryBytePtr ( PrevOffset, pByte ) ;
   if ( ::IsEndOfLine ( pByte ) )
      return ( TRUE ) ;

   // Otherwise, we aren't starting a paragraph now.
   return ( FALSE ) ;
}
 
/****************************************************************************
 *                                                                          *
 *  Measure built word, obtaining it's display size, ascent & descent.      *
 *                                                                          *
 ****************************************************************************/

static void MeasureFlush (
   WorkSpace *PS,
   char *String,
   int &StringLength,
   SIZEL &Size,
   long &MaxAscent,
   long &MaxDescent
) {
   if ( StringLength ) {
      Size.cx += PS->MeasureText ( String, StringLength ) ;
      MaxAscent  = max ( MaxAscent,  PS->QueryAscent() ) ;
      MaxDescent = max ( MaxDescent, PS->QueryDescent() ) ;
      Size.cy = MaxAscent + MaxDescent ;
      StringLength = 0 ;
   } /* endif */
}

void Palabra::Measure (
   Text *pText,         // In: -> Text Object
   WorkSpace *PS,       // In: -> Graphics workspace
   Estado &State,       // In: State at start of word (will be updated)
   int PageNumber,      // In: Current Page Number
   int NumberOfPages,   // In: Current Page Count
   time_t FileDate      // In: File Date
) {

   // Reset the state.
   PS->SetState ( State ) ;

   // Reset the measurements.
   Length = 0 ;
   Size.cx = 0 ;
   Size.cy = 0 ;
   MaxAscent = 0 ;
   MaxDescent = 0 ;

   // Get pointer to data.
   PUCHAR pByte ;
   pText->QueryBytePtr ( Offset, pByte ) ;

   // Prepare to measure text.
   int StringLength = 0 ;
   char String [MAX_STRING] ;

   // Until the end of word is reached . . .
   while ( 1 ) {

      // If end of word . . .
      if ( IsEndOfWord ( pByte, PS->Query_DBCS_Vector() ) ) {
         if ( Length == 0 ) {
            if ( ::IsSpace(*pByte) OR IsHyphen(*pByte) ) {
               String[StringLength++] = *pByte ;
               Length ++ ;
            } else if ( IsDBCS ( *pByte, PS->Query_DBCS_Vector() ) ) {
               String[StringLength++] = *pByte ;
               Length ++ ;
               String[StringLength++] = *(pByte+1) ;
               Length ++ ;
            } else if ( IsFunction ( *pByte ) ) {
               long LengthIn ;
               ComputeFunctionSize ( pByte, LengthIn ) ;
               Length += LengthIn ;
               char Text [100] ;
               switch ( *(pByte+1) ) {
                  case FN__DATEFIXED: {
                     FormatDate ( *PULONG(pByte+2), State.DateFormat, Text, sizeof(Text) ) ;
                     if ( StringLength+strlen(Text)+1 >= sizeof(String) ) 
                        MeasureFlush ( PS, String, StringLength, Size, MaxAscent, MaxDescent ) ;
                     strcpy ( String+StringLength, Text ) ;
                     StringLength += strlen(Text) ;
                     break; }
                  case FN__DATECURRENT: {
                     FormatDate ( time(0), State.DateFormat, Text, sizeof(Text) ) ;
                     if ( StringLength+strlen(Text)+1 >= sizeof(String) ) 
                        MeasureFlush ( PS, String, StringLength, Size, MaxAscent, MaxDescent ) ;
                     strcpy ( String+StringLength, Text ) ;
                     StringLength += strlen(Text) ;
                     break; }
                  case FN__DATECHANGED: {
                     FormatDate ( FileDate, State.DateFormat, Text, sizeof(Text) ) ;
                     if ( StringLength+strlen(Text)+1 >= sizeof(String) ) 
                        MeasureFlush ( PS, String, StringLength, Size, MaxAscent, MaxDescent ) ;
                     strcpy ( String+StringLength, Text ) ;
                     StringLength += strlen(Text) ;
                     break; }
                  case FN__PAGENUMBER: {
                     sprintf ( Text, "%i", PageNumber+1 ) ;
                     if ( StringLength+strlen(Text)+1 >= sizeof(String) ) 
                        MeasureFlush ( PS, String, StringLength, Size, MaxAscent, MaxDescent ) ;
                     strcpy ( String+StringLength, Text ) ;
                     StringLength += strlen(Text) ;
                     break; }
                  case FN__PAGECOUNT: {
                     sprintf ( Text, "%i", NumberOfPages ) ;
                     if ( StringLength+strlen(Text)+1 >= sizeof(String) ) 
                        MeasureFlush ( PS, String, StringLength, Size, MaxAscent, MaxDescent ) ;
                     strcpy ( String+StringLength, Text ) ;
                     StringLength += strlen(Text) ;
                     break; }
                  case FN__GLYPH: {
                     sprintf ( Text, "%c", *PUCHAR(pByte+2) ) ;
                     if ( StringLength+strlen(Text)+1 >= sizeof(String) ) 
                        MeasureFlush ( PS, String, StringLength, Size, MaxAscent, MaxDescent ) ;
                     strcpy ( String+StringLength, Text ) ;
                     StringLength += strlen(Text) ;
                     break; }
               } /* endswitch */
            } else {
               Length ++ ;
            } /* endif */
         } /* endif */
         break ;

      // If glyph function that's not an end-of-word, then add it's text to the word.
      } else if ( IsGlyphFunction ( pByte ) ) {
         long LengthIn ;
         ComputeFunctionSize ( pByte, LengthIn ) ;
         Length += LengthIn ;
         char Text [100] ;
         switch ( *(pByte+1) ) {
            case FN__GLYPH: {
               sprintf ( Text, "%c", *PUCHAR(pByte+2) ) ;
               if ( StringLength+strlen(Text)+1 > sizeof(String) ) 
                  MeasureFlush ( PS, String, StringLength, Size, MaxAscent, MaxDescent ) ;
               strcpy ( String+StringLength, Text ) ;
               StringLength += strlen(Text) ;
               break; }
         } /* endswitch */

      // If not function token, then add the token to the word.
      } else if ( *pByte ) {
         Length ++ ;
         String[StringLength++] = *pByte ;
         if ( StringLength >= sizeof(String) ) 
            MeasureFlush ( PS, String, StringLength, Size, MaxAscent, MaxDescent ) ;

      // Else, determine the function token size and add -that- to the word.
      } else {
         long LengthIn ;
         ComputeFunctionSize ( pByte, LengthIn ) ;
         Length += LengthIn ;
         MeasureFlush ( PS, String, StringLength, Size, MaxAscent, MaxDescent ) ;
         State.ProcessFunction ( pByte, PS, PageNumber, NumberOfPages, FileDate ) ;

      } /* endif */

      // Get the next byte.
      if ( pText->QueryBytePtr ( Offset+Length, pByte ) == 0 )
         break ;

   } /* endwhile */

   // Add the size of any text left unmeasured.
   MeasureFlush ( PS, String, StringLength, Size, MaxAscent, MaxDescent ) ;

   // If final horizontal size was zero, then set the vertical size according to the state.
   if ( Size.cx == 0 ) {
      MaxAscent = PS->QueryAscent() ;
      MaxDescent = PS->QueryDescent() ;
      Size.cy = MaxAscent + MaxDescent ;
   } /* endif */
}
 
/****************************************************************************
 *                                                                          *
 *  Palabra: Do two words appear identical on the screen?                   *
 *                                                                          *
 ****************************************************************************/

BOOL Palabra::LooksLike ( Palabra &OtherWord ) {
   if ( Position.x != OtherWord.Position.x ) 
      return ( FALSE ) ;
   if ( Position.y != OtherWord.Position.y ) 
      return ( FALSE ) ;
   if ( Size.cx != OtherWord.Size.cx ) 
      return ( FALSE ) ;
   if ( Size.cy != OtherWord.Size.cy ) 
      return ( FALSE ) ;
   if ( CRC != OtherWord.CRC ) 
      return ( FALSE ) ;
   return ( TRUE ) ;
} /* endmethod */

/****************************************************************************
 *                                                                          *
 *  Palabra: Dump                                                           *
 *                                                                          *
 ****************************************************************************/

void Palabra::Dump ( int indent, Text *pText ) {

   Log ( "%*sPalabra %p:", indent, "", this ) ;

   Log ( "%*sOffset, Length: %i, %i", indent, "", Offset, Length ) ;
   Log ( "%*sNeedsMeasuring: %s", indent, "", NeedsMeasuring ? "TRUE" : "FALSE" ) ;
   Log ( "%*sNeedsPainting: %s", indent, "", NeedsPainting ? "TRUE" : "FALSE" ) ;
   Log ( "%*sWasGraphic: %s", indent, "", WasGraphic ? "TRUE" : "FALSE" ) ;
   Log ( "%*sPosition, Size, CRC: %i,%i (%ix%i), %04X", indent, "", Position.x, Position.y, Size.cx, Size.cy, CRC ) ;
   Log ( "%*sOld Position, Size, CRC: %i,%i (%ix%i), %04X", indent, "", OldPosition.x, OldPosition.y, OldSize.cx, OldSize.cy, OldCRC ) ;
   Log ( "%*sMaxAscent, MaxDescent, OldMaxDescent: %i, %i", indent, "", MaxAscent, MaxDescent, OldMaxDescent ) ;
   Log ( "%*sDormant: %s", indent, "", Dormant ? "TRUE" : "FALSE" ) ;
   Log ( "%*sSegmentStart: %s", indent, "", SegmentStart ? "TRUE" : "FALSE" ) ;

   for ( int i=0; i<Length; i+=16 ) {

      char Text [80] ;
      sprintf ( Text, "%04X:", i ) ;

      for ( int j=i; j<i+16 AND j<Length; j++ ) {
         PUCHAR pByte ;
         pText->QueryBytePtr ( Offset+j, pByte ) ;
         sprintf ( Text+strlen(Text), "%02X ", *PUCHAR(pByte) ) ;
      } /* endfor */
      while ( j<i+16 ) {
         strcat ( Text, "   " ) ;
         j ++ ;
      } /* endfor */
      strcat ( Text, "|" ) ;

      for ( j=i; j<i+16 AND j<Length; j++ ) {
         PUCHAR pByte ;
         pText->QueryBytePtr ( Offset+j, pByte ) ;
         sprintf ( Text+strlen(Text), "%c", isprint(*pByte)?*pByte:'.' ) ;
      } /* endfor */
      while ( j<i+16 ) {
         strcat ( Text, " " ) ;
         j ++ ;
      } /* endfor */
      strcat ( Text, "|" ) ;

      Log ( "%*s%s", indent, "", Text ) ;

   } /* endfor */

}



 
/****************************************************************************
 ****************************************************************************
 ***************************** Class 'Linea' ********************************
 ****************************************************************************
 ****************************************************************************/

/****************************************************************************
 *                                                                          *
 *  Linea: Find Previous Line in list                                       *
 *                                                                          *
 ****************************************************************************/

Linea *Linea::PrevLine ( ) {
   PoolEntry *ThisEntry = ((PoolEntry*)this) - 1 ;
   PoolEntry *PrevEntry = ThisEntry->Query_Prev ( ) ;
   return ( (Linea*) ( PrevEntry ? PrevEntry->Data() : 0 ) ) ;
} /* endmethod */

/****************************************************************************
 *                                                                          *
 *  Linea: Find Next Line in list                                           *
 *                                                                          *
 ****************************************************************************/

Linea *Linea::NextLine ( ) {
   PoolEntry *ThisEntry = ((PoolEntry*)this) - 1 ;
   PoolEntry *NextEntry = ThisEntry->Query_Next ( ) ;
   return ( (Linea*) ( NextEntry ? NextEntry->Data() : 0 ) ) ;
} /* endmethod */

/****************************************************************************
 *                                                                          *
 *  Linea: Reset                                                            *
 *                                                                          *
 ****************************************************************************/

void Linea::Reset ( WorkSpace *PS, Borradora &Page, Estado &state, Linea *pPrevLine, long LineTop, int PageNumber, int NumberOfPages, time_t FileDate ) {

   State = state ;
   CRC = State.ComputeCRC ( ) ;
   WordNumber = 0 ;

   if ( pPrevLine ) {

      Palabra *pPrevLineWord0 = Page.FindWord ( (int) pPrevLine->WordNumber ) ;
      Palabra *pPrevLineWordx = Page.FindWord ( pPrevLineWord0, pPrevLine->WordCount - 1 ) ;

      WordNumber = pPrevLine->WordNumber + pPrevLine->WordCount ;
      long OldOffset = pPrevLineWord0->Offset ;
      long NewOffset = pPrevLineWordx->Offset + pPrevLineWordx->Length ;

      ComputeNextState ( PS, Page.Query_Text(), OldOffset, NewOffset-OldOffset, pPrevLine->State, State, PageNumber, NumberOfPages, FileDate ) ;
      CRC = State.ComputeCRC ( ) ;

   } /* endif */

   WordCount = 1 ;
   Top = LineTop ;
}

/****************************************************************************
 *                                                                          *
 *  Linea: Mark entire line as painted.                                     *
 *                                                                          *
 ****************************************************************************/

void Linea::MarkPainted ( Borradora &Page ) {
   Palabra *pWord = Page.FindWord ( WordNumber ) ;
   for ( int i=0; i<WordCount; i++, pWord=pWord->NextWord() ) {
      #ifdef DEBUG_NEEDSPAINTING
         Log ( "Linea::MarkPainted: Word %i: Resetting NeedsPainting because line clipped.", WordNumber+i ) ;
      #endif
      pWord->NeedsPainting = FALSE ;
   } /* endfor */
} /* endmethod */

/****************************************************************************
 *                                                                          *
 *  Linea: Estimate a line's dimensions.                                    *
 *                                                                          *
 ****************************************************************************/

void Linea::EstimateHeight ( WorkSpace *PS, Text *pText, Borradora &Page, Palabra *pFirstWord, 
   Estado &EffectiveState, int Paragraph, int NumberOfPages, time_t Date, int Force, int &MaxAscent, int &MaxDescent ) {
   Estado WorkingState = State ;
   MaxAscent = 0 ;
   MaxDescent = 0 ;
   int WordsFit = 0 ;
   PUCHAR pByte ;
   Page.Query_Text()->QueryBytePtr ( pFirstWord->Offset, pByte ) ;
   BOOL Separator = IsSeparator ( pByte ) ;
   long Column = max ( 0, EffectiveState.Margins.xLeft + ( Separator ? 0 : ( Paragraph ? ( EffectiveState.Indent>0 ? EffectiveState.Indent : 0 ) : ( EffectiveState.Indent>=0 ? 0 : -EffectiveState.Indent ) ) ) ) ;
   Palabra *pWord = pFirstWord ;
   while ( Column < EffectiveState.Margins.xRight ) {
      if ( NOT Page.BuildWord ( NumberOfPages, Date, WordNumber+WordsFit, PS ) )
         break ;
      pWord = Page.FindWord ( pFirstWord, WordsFit ) ;
      pWord->Dormant = FALSE ;
      pWord->SegmentStart = FALSE ;
      PUCHAR pByte ;
      pText->QueryBytePtr ( pWord->Offset, pByte ) ;
      if ( IsEndOfLine(pByte) ) {
         WordsFit ++ ;
         break ;
      } /* endif */
      long Width = pWord->Size.cx ;
      if ( IsTab(*pByte) OR IsSpace(*pByte) )
         Width = 0 ;
      if ( IsGraphic ( pByte ) ) {
         Grafico *pGraphic = Page.FindGraphic ( pWord->Offset ) ;
         if ( pGraphic == 0 ) {
            WorkSpace PS = WorkSpace ( "Documento::FormatLine", 0, 0, FALSE, 8500, 11000 ) ;
            Page.BuildGraphic ( &PS, this, pWord, pByte, 0, 0 ) ;
            pGraphic = Page.FindGraphic ( pWord->Offset ) ;
         } /* endif */
         if ( pGraphic && ( pGraphic->Position.x == GRAPHIC_NEXT_CHAR ) ) {
            pWord->Size.cx = pGraphic->Size.cx + pGraphic->BorderWidth * 2 ;
            pWord->Size.cy = pGraphic->Size.cy + pGraphic->BorderWidth * 2 ;
            pWord->MaxAscent = pWord->Size.cy ;
            pWord->MaxDescent = 0 ;
            Width = pWord->Size.cx ;
         } /* endif */
      } /* endif */
      if ( ( Column + Width <= EffectiveState.Margins.xRight ) OR ( NOT WordsFit AND Force ) ) {
         Column += Width ;
         if ( pWord->Size.cx ) {
            MaxAscent = max ( MaxAscent, pWord->MaxAscent ) ;
            MaxDescent = max ( MaxDescent, pWord->MaxDescent ) ;
         } /* endif */
      } else {
         break ;
      } /* endif */
      WordsFit ++ ;
   } /* endwhile */
   if ( ( MaxAscent == 0 ) AND ( MaxDescent == 0 ) ) {
      PS->SetState ( EffectiveState ) ;
      MaxAscent = max ( MaxAscent, PS->QueryAscent() ) ;
      MaxDescent = max ( MaxDescent, PS->QueryDescent() ) ;
   } /* endif */
} /* endmethod */

/****************************************************************************
 *                                                                          *
 *  Linea: Dump                                                             *
 *                                                                          *
 ****************************************************************************/

void Linea::Dump ( int indent ) {
   Log ( "%*sLinea %p:", indent, "", this ) ;
   Log ( "%*sOffset, Length: %i, %i", indent, "", Offset, Length ) ;
   Log ( "%*sState:", indent, "" ) ;  State.Dump ( indent+2 ) ;
   Log ( "%*sState CRC: %04X", indent, "", CRC ) ;
   Log ( "%*sWord number, count: %i, %i", indent, "", WordNumber, WordCount ) ;
   Log ( "%*sTop, Height: %i, %i", indent, "", Top, Height ) ;
   Log ( "%*sMaxAscent, MaxDescent: %i, %i", indent, "", MaxAscent, MaxDescent ) ;
} /* endmethod */



 
/****************************************************************************
 ****************************************************************************
 *************************** Class 'Borradora' ******************************
 ****************************************************************************
 ****************************************************************************/

/****************************************************************************
 *                                                                          *
 *  Borradora: Constructor                                                  *
 *                                                                          *
 ****************************************************************************/

Borradora::Borradora ( ) : 
   Words ( "Borradora::Words", sizeof(Palabra), 10 ), 
   Lines ( "Borradora::Lines", sizeof(Linea), 10 ), 
   Graphics ( "Borradora::Graphics", sizeof(Grafico), 10 ) {

} /* endmethod */

/****************************************************************************
 *                                                                          *
 *  Borradora: Destructor                                                   *
 *                                                                          *
 ****************************************************************************/

Borradora::~Borradora ( ) {
} /* endmethod */

/****************************************************************************
 *                                                                          *
 *  Borradora: Assignment Operator Overload                                 *
 *                                                                          *
 ****************************************************************************/

/****************************************************************************
 *                                                                          *
 *  Helper Function: Update invalid rectangle.                              *
 *                                                                          *
 ****************************************************************************/

static void InvalidateRectangle ( RECTL &Rectangle, RECTL &Cell ) {

   #ifdef DEBUG_INVALIDRECTANGLE
      Log ( "  Invalid area: %i,%i-%i,%i.", Cell.xLeft, Cell.yBottom, Cell.xRight, Cell.yTop ) ;
   #endif

   Rectangle.xLeft   = min ( Rectangle.xLeft,   Cell.xLeft   ) ;
   Rectangle.xRight  = max ( Rectangle.xRight,  Cell.xRight  ) ;
   Rectangle.yBottom = min ( Rectangle.yBottom, Cell.yBottom ) ;
   Rectangle.yTop    = max ( Rectangle.yTop,    Cell.yTop    ) ;

   #ifdef DEBUG_INVALIDRECTANGLE
      Log ( "  Invalid rectangle: %i,%i-%i,%i.",
         Rectangle.xLeft, Rectangle.yBottom, Rectangle.xRight, Rectangle.yTop ) ;
   #endif
}

static void InvalidateRectangle ( RECTL &Rectangle, POINTL &Position, SIZEL &Size, LONG MaxDescent ) {
   RECTL Cell ;
   Cell.xLeft   = Position.x ;
   Cell.yBottom = Position.y - MaxDescent + 1 ;
   Cell.xRight  = Cell.xLeft + Size.cx ;
   Cell.yTop    = Cell.yBottom + Size.cy ;
   InvalidateRectangle ( Rectangle, Cell ) ;
}

static void InvalidateRectangle ( RECTL &Rectangle, POINTL &Position, SIZEL &Size ) {
   RECTL Cell ;
   Cell.xLeft   = Position.x ;
   Cell.yBottom = Position.y ;
   Cell.xRight  = Cell.xLeft + Size.cx ;
   Cell.yTop    = Cell.yBottom + Size.cy ;
   InvalidateRectangle ( Rectangle, Cell ) ;
}

Borradora & Borradora::operator= ( const Borradora & Original ) {

   #ifdef DEBUG_ASSIGNMENT
      Log ( "Borradora(%08X)::operator=(Original=%08X) Started.", this, &Original ) ;
   #endif // DEBUG_ASSIGNMENT

   // If the page offset has changed, invalidate the entire page image.
   if ( Offset != Original.Offset ) {
      #ifdef DEBUG_ASSIGNMENT
         Log ( "Borradora(%08X)::operator=() Invalidating entire page.", this ) ;
      #endif // DEBUG_ASSIGNMENT
      Offset = Original.Offset ;
      RECTL Box = { 0, 0, 100000, 100000 } ;
      InvalidRectangle = Box ;

   // Else, check the new word list against the old and invalidate accordingly.
   } else {
      #ifdef DEBUG_ASSIGNMENT
         Log ( "Borradora(%08X)::operator=() Comparing page headers.", this ) ;
      #endif // DEBUG_ASSIGNMENT
      if ( State.Header != Original.State.Header ) {
         POINTL Position = { Original.State.HeaderState.Margins.xLeft, Original.State.HeaderState.Margins.yTop-Original.State.HeaderHeight } ;
         SIZEL Size = { Original.State.HeaderState.Margins.xRight-Original.State.HeaderState.Margins.xLeft, Original.State.HeaderHeight } ;
         InvalidateRectangle ( InvalidRectangle, Position, Size, 0 ) ;
         Position.x = State.HeaderState.Margins.xLeft ;
         Position.y = State.HeaderState.Margins.yTop - State.HeaderHeight ;
         Size.cx = State.HeaderState.Margins.xRight - State.HeaderState.Margins.xLeft ;
         Size.cy = State.HeaderHeight ;
         InvalidateRectangle ( InvalidRectangle, Position, Size, 0 ) ;
      } /* endif */
      #ifdef DEBUG_ASSIGNMENT
         Log ( "Borradora(%08X)::operator=() Comparing page footers.", this ) ;
      #endif // DEBUG_ASSIGNMENT
      if ( State.Footer != Original.State.Footer ) {
         POINTL Position = { Original.State.FooterState.Margins.xLeft, Original.State.FooterState.Margins.yBottom } ;
         SIZEL Size = { Original.State.FooterState.Margins.xRight-Original.State.FooterState.Margins.xLeft, Original.State.FooterHeight } ;
         InvalidateRectangle ( InvalidRectangle, Position, Size, 0 ) ;
         Position.x = State.FooterState.Margins.xLeft ;
         Position.y = State.FooterState.Margins.yTop - State.FooterHeight ;
         Size.cx = State.FooterState.Margins.xRight - State.FooterState.Margins.xLeft ;
         Size.cy = State.FooterState.Margins.yTop - State.FooterState.Margins.yBottom ;
         InvalidateRectangle ( InvalidRectangle, Position, Size, 0 ) ;
      } /* endif */
      #ifdef DEBUG_ASSIGNMENT
         Log ( "Borradora(%08X)::operator=() Comparing page words.", this ) ;
      #endif // DEBUG_ASSIGNMENT
      Palabra *pNewWord = FindWord ( (int) 0 ) ;
      Palabra *pOldWord = Original.FindWord ( (int) 0 ) ;
      while ( pNewWord && pOldWord ) {
         if ( pOldWord->LooksLike ( *pNewWord ) ) {
            pNewWord = pNewWord->NextWord() ;
            pOldWord = pOldWord->NextWord() ;
         } else {
            InvalidateRectangle ( InvalidRectangle, pOldWord->Position, pOldWord->Size, pOldWord->MaxDescent ) ;
            InvalidateRectangle ( InvalidRectangle, pNewWord->Position, pNewWord->Size, pNewWord->MaxDescent ) ;
            pNewWord = pNewWord->NextWord() ;
            pOldWord = pOldWord->NextWord() ;
         } /* endif */
      } /* endwhile */
      while ( pOldWord ) {
         InvalidateRectangle ( InvalidRectangle, pOldWord->Position, pOldWord->Size, pOldWord->MaxDescent ) ;
         pOldWord = pOldWord->NextWord() ;
      } /* endwhile */
      while ( pNewWord ) {
         InvalidateRectangle ( InvalidRectangle, pNewWord->Position, pNewWord->Size, pNewWord->MaxDescent ) ;
         pNewWord = pNewWord->NextWord() ;
      } /* endwhile */

   } /* endif */

   Length = Original.Length ;
   PageNumber = Original.PageNumber ;
   State = Original.State ;
   CRC = Original.CRC ;
   LastState = Original.LastState ;
   pText = Original.pText ;
   RetainedLength = Original.RetainedLength ;
   CurrentLength = Original.CurrentLength ;
   Words = Original.Words ;
   Lines = Original.Lines ;
   Graphics = Original.Graphics ;
   InvalidRectangle = Original.InvalidRectangle ;

   #ifdef DEBUG_ASSIGNMENT
      Log ( "Borradora(%08X)::operator=() Done.", this ) ;
   #endif // DEBUG_ASSIGNMENT

   return ( *this ) ;

} /* endmethod */

/****************************************************************************
 *                                                                          *
 *  Borradora: Reset before formatting.                                     *
 *                                                                          *
 ****************************************************************************/

void Borradora::Reset ( Estado &State ) {

   // Set the state for the next word to be generated.
   LastState = State ;
   CRC = State.ComputeCRC ( ) ;

   // Reset the current and retained page lengths.
   RetainedLength = 0 ;
   CurrentLength = 0 ;

   // Clear the working graphics list.
   Graphics.Clear ( ) ;

   // Clear the working word list.
   Words.Clear ( ) ;

   // Clear the working line list.
   Lines.Clear ( ) ;

   // Reset the invalid rectangle.
   InvalidRectangle.xLeft = InvalidRectangle.yBottom = 0x7FFFFFFF ;
   InvalidRectangle.xRight = InvalidRectangle.yTop = 0 ;
}

/****************************************************************************
 *                                                                          *
 *  Borradora: Query for number of active graphics                          *
 *                                                                          *
 ****************************************************************************/

int Borradora::Query_NumberOfGraphics ( long Offset ) {

   int Count = 0 ;

   Grafico *pGraphic = FindGraphic ( (int) 0 ) ;
   while ( pGraphic ) {
      if ( pGraphic->Offset > Offset ) 
         break ;
      Count ++ ;
      pGraphic = pGraphic->NextGraphic() ;
   } /* endwhile */

   return ( Count ) ;

} /* endmethod */

/****************************************************************************
 *                                                                          *
 *  Build Word in Page (by Number, ( 0 -> (n-1) )                           *
 *    Return TRUE if successful, and word is now or was already in the list.*
 *    Return FALSE otherwise.                                               *
 *                                                                          *
 ****************************************************************************/

BOOL Borradora::BuildWord (
   int NumberOfPages,   // In: Number of pages
   time_t FileDate,     // In: File Date
   int WordNumber,      // In: Offset of word list entry that is requested.
   WorkSpace *PS        // In: -> Graphics workspace
) {

   // If word number within list already built, return TRUE automatically.
   if ( Words.Query_Count() > WordNumber ) 
      return ( TRUE ) ;

   #ifdef DEBUG
      Log ( "Borradora(%p)::BuildWord(NumberOfPages=%i,FileDate,WordNumber=%i,PS)", this, NumberOfPages, WordNumber ) ;
   #endif

   // Position to just after the last word built.
   long Position = Offset ;
   if ( Words.Query_Count() ) {
      Palabra *pWord = FindWord ( Words.Query_Count() - 1 ) ;
      Position = pWord->Offset + pWord->Length ;
   } /* endif */

   // Build words until we have the one we want.
   while ( Words.Query_Count() <= WordNumber ) {

      Palabra Word ;
      memset ( &Word, 0, sizeof(Word) ) ;

      Word.Offset = Position ;
      GetWord ( pText, PS->Query_DBCS_Vector(), Word.Offset, Word.Length ) ;

      if ( Word.Length == 0 )
         break ;

      Word.CRC = LastState.ComputeCRC ( ) ;
      Word.Measure ( pText, PS, LastState, PageNumber, NumberOfPages, FileDate ) ;

      PoolEntry *pLast = Words.Find ( Words.Query_Count() - 1 ) ;
      Words.Insert ( pLast, &Word ) ;

      Position += Word.Length ;

   } /* endwhile */

   #ifdef DEBUG
      Log ( "Borradora(%p)::BuildWord(NumberOfPages=%i,FileDate,WordNumber=%i,PS) Done %s.", this, NumberOfPages, WordNumber, Words.Query_Count() <= WordNumber ? "FALSE": "TRUE" ) ;
   #endif

   if ( Words.Query_Count() <= WordNumber ) 
      return ( FALSE ) ;

   return ( TRUE ) ;
}
/****************************************************************************
 *                                                                          *
 *  Grafico: Build Object                                                   *
 *                                                                          *
 ****************************************************************************/

BOOL Borradora::BuildGraphic ( WorkSpace *PS, Linea *pLine, Palabra *pWord, PUCHAR pByte, int NumberOfPages, time_t FileDate ) {

   #if defined(DEBUG) || defined(DEBUG_GRAPHIC)
      Log ( "Borradora(%p)::BuildGraphic(pLine=%p,pWord=%p,pByte=%p) Started.", this, pLine, pWord, pByte ) ;
   #endif

   // Find out if the object is already in the list.  Return FALSE if already there.
   Grafico *pPrevGraphic = 0 ;
   Grafico *pGraphic = FindGraphic ( (int) 0 ) ;
   while ( pGraphic ) {
      if ( pGraphic->Offset == pWord->Offset ) {
         #if defined(DEBUG) || defined(DEBUG_GRAPHIC)
            Log ( "Borradora(%p)::BuildGraphic(pLine=%p,pWord=%p,pByte=%p) Done.  Graphic already in list.", this, pLine, pWord, pByte ) ;
         #endif
         return ( FALSE ) ;
      } else if ( ( pGraphic->Offset < pWord->Offset ) && ( ( pPrevGraphic == 0 ) || ( pGraphic->Offset > pPrevGraphic->Offset ) ) ) {
         #if defined(DEBUG) || defined(DEBUG_GRAPHIC)
            Log ( "Borradora(%p)::BuildGraphic(pLine=%p,pWord=%p,pByte=%p) Skipped graphic %08X (offset %i) because before current offset %i.  Now best predecessor.", this, pLine, pWord, pByte, pGraphic, pGraphic->Offset, pWord->Offset ) ;
         #endif
         pPrevGraphic = pGraphic ;
      } else {
         #if defined(DEBUG) || defined(DEBUG_GRAPHIC)
            Log ( "Borradora(%p)::BuildGraphic(pLine=%p,pWord=%p,pByte=%p) Skipped graphic %08X (offset %i) because beyond current offset %i.", this, pLine, pWord, pByte, pGraphic, pGraphic->Offset, pWord->Offset ) ;
         #endif
      } /* endif */
      pGraphic = pGraphic->NextGraphic ( ) ;
   } /* endwhile */

   // Add the graphic to the list.
   #if defined(DEBUG) || defined(DEBUG_GRAPHIC)
      Log ( "Borradora(%p)::BuildGraphic(pLine=%p,pWord=%p,pByte=%p) Adding new graphic to list, after entry %08X.", this, pLine, pWord, pByte, pPrevGraphic ) ;
   #endif
   Grafico Graphic ;
   memset ( &Graphic, 0, sizeof(Graphic) ) ;
   pGraphic = (Grafico*) ( Graphics.Insert ( pPrevGraphic ? ((PoolEntry*)pPrevGraphic)-1 : 0, &Graphic ) ) -> Data ( ) ;

   // Get pointers to the graphic's data.
   #if defined(DEBUG) || defined(DEBUG_GRAPHIC)
      Log ( "Borradora(%p)::BuildGraphic(pLine=%p,pWord=%p,pByte=%p) Getting pointers to data.", this, pLine, pWord, pByte ) ;
   #endif
   PPOINTL pPosition = PPOINTL ( pByte + 2 ) ;
   PSIZEL pSize = PSIZEL ( pPosition + 1 ) ;
   PUCHAR pBackground = PUCHAR ( pSize + 1 ) ;
   PUCHAR pNoTextAside = PUCHAR ( pBackground + 1 ) ;
   PUCHAR pHalftone = PUCHAR ( pNoTextAside + 1 ) ;
   PUCHAR pReverse = PUCHAR ( pHalftone + 1 ) ;
   PUCHAR pAspect = PUCHAR ( pReverse + 1 ) ;
   PUCHAR pPrinterOptimize = PUCHAR ( pAspect + 1 ) ;
   PCHAR pType = PCHAR ( pPrinterOptimize + 1 ) ;
   PLONG pHandle = PLONG ( pType + 2 ) ;

   // Initialize the graphic object.
   #if defined(DEBUG) || defined(DEBUG_GRAPHIC)
      Log ( "Borradora(%p)::BuildGraphic(pLine=%p,pWord=%p,pByte=%p) Initializing object %p.", this, pLine, pWord, pByte, pGraphic ) ;
   #endif
   pGraphic->Offset = pWord->Offset ;
   pGraphic->Length = pWord->Length ;
   pGraphic->NeedsPainting = TRUE ;
   pGraphic->Position = *pPosition ;
   pGraphic->Size = *pSize ;
   ComputeNextState ( PS, pText, FindWord(pLine->WordNumber)->Offset,
      pGraphic->Offset-FindWord(pLine->WordNumber)->Offset, pLine->State, pGraphic->State, PageNumber, NumberOfPages, FileDate ) ;
   pGraphic->CRC = pGraphic->State.ComputeCRC ( ) ;
   pGraphic->Type[0] = pType [0] ;
   pGraphic->Type[1] = pType [1] ;
   pGraphic->BorderWidth = GRAPHIC_BORDER ;
   pGraphic->BorderColor = RGB_WHITE ;
   pGraphic->Background = *pBackground ? TRUE : FALSE ;
   pGraphic->NoTextAside = *pNoTextAside ? TRUE : FALSE ;
   pGraphic->Halftone = *pHalftone ? TRUE : FALSE ;
   pGraphic->Reverse = *pReverse ? TRUE : FALSE ;
   pGraphic->Aspect = *pAspect ;
   pGraphic->PrinterOptimize = *pPrinterOptimize ? TRUE : FALSE ;
   if ( !strnicmp ( pType, "UP", 2 ) ) {
      strncpy ( pGraphic->BarCode, PCHAR(pHandle), 13 ) ;
   } else if ( !strnicmp ( pType, "UE", 2 ) ) {
      strncpy ( pGraphic->BarCode, PCHAR(pHandle), 7 ) ;
   } else if ( !strnicmp ( pType, "39", 2 ) ) {
      strncpy ( pGraphic->BarCode, PCHAR(pHandle), 80 ) ;
   } else if ( !strnicmp ( pType, "PN", 2 ) ) {
      strncpy ( pGraphic->BarCode, PCHAR(pHandle), 10 ) ;
   } else if ( !strnicmp ( pType, "E8", 2 ) ) {
      strncpy ( pGraphic->BarCode, PCHAR(pHandle), 8 ) ;
   } else if ( !strnicmp ( pType, "EA", 2 ) ) {
      strncpy ( pGraphic->BarCode, PCHAR(pHandle), 14 ) ;
   } else {
      pGraphic->Handle = *pHandle ;
   } /* endif */

   // Update the word to show that at one time it was a graphic.
   pWord->WasGraphic = TRUE ;

   // Compute the original image size.
   pGraphic->ComputeImageSize ( PS ) ;

   // Return TRUE because we had to add the graphic to the list.
   #if defined(DEBUG) || defined(DEBUG_GRAPHIC)
      Log ( "Borradora(%p)::BuildGraphic(pLine=%p,pWord=%p,pByte=%p) Done.", this, pLine, pWord, pByte ) ;
   #endif
   return ( TRUE ) ;
}
 
/****************************************************************************
 *                                                                          *
 *  Borradora: Find Line Functions                                          *
 *                                                                          *
 ****************************************************************************/

Linea *Borradora::FindLine ( int Index ) { 
   PoolEntry *pEntry = Lines.Find ( Index ) ;
   if ( pEntry )
      return ( (Linea*) pEntry->Data() ) ;
   else
      return ( 0 ) ;
} /* endmethod */

Linea *Borradora::FindLine ( long Offset, int &Line ) {
   Linea *pLine = FindLine ( (int) 0 ) ;
   for ( Line=0; Line<Lines.Query_Count(); Line++, pLine=pLine->NextLine() ) 
      if ( ( Offset >= pLine->Offset ) AND ( Offset < pLine->Offset + pLine->Length ) )
         return ( pLine ) ;
   return ( 0 ) ;
} /* endmethod */

Linea *Borradora::FindLine ( POINTL &Position, int &Line ) {
   Linea *pLine = FindLine ( (int) 0 ) ;
   for ( Line=0; Line<Lines.Query_Count()-1; Line++, pLine=pLine->NextLine() ) 
      if ( Position.y >= pLine->Top - pLine->Height )
         break ;
   return ( pLine ) ;
} /* endmethod */

/****************************************************************************
 *                                                                          *
 *  Borradora: Clear Lines                                                  *
 *                                                                          *
 ****************************************************************************/

void Borradora::ClearLines ( ) {
   Lines.Clear ( ) ;
}

/****************************************************************************
 *                                                                          *
 *  Borradora: Add Line                                                     *
 *                                                                          *
 ****************************************************************************/

Linea *Borradora::AddLine ( Linea &Line ) {
   return ( (Linea*) Lines.Insert(Lines.Find(Lines.Query_Count()-1),&Line)->Data() ) ;
}

/****************************************************************************
 *                                                                          *
 *  Borradora: Find Word Functions                                          *
 *                                                                          *
 ****************************************************************************/

Palabra *Borradora::FindWord ( int Index ) const { 
   PoolEntry *pEntry = Words.Find ( Index ) ;
   if ( pEntry )
      return ( (Palabra*) pEntry->Data() ) ;
   else
      return ( 0 ) ;
} /* endmethod */

Palabra *Borradora::FindWord ( long Offset ) {
   Palabra *pWord = FindWord ( (int) 0 ) ;
   while ( pWord ) {
      if ( Offset == pWord->Offset ) 
         break ;
      if ( ( Offset > pWord->Offset ) AND ( Offset < pWord->Offset + pWord->Length ) )
         break ;
      pWord = pWord->NextWord ( ) ;
   } /* endwhile */
   return ( pWord ) ;
} /* endmethod */
 
Palabra *Borradora::FindWord ( Palabra *pWord, int Offset ) {

   if ( Offset > 0 ) {
      while ( Offset-- AND pWord ) 
         pWord = pWord->NextWord ( ) ;

   } else if ( Offset < 0 ) {
      while ( Offset++ AND pWord ) 
         pWord = pWord->PrevWord ( ) ;

   } /* endif */

   return ( pWord ) ;

} /* endmethod */

Palabra *Borradora::FindWord ( int Line, int Column ) {

   // Determine our location within the line.
   Linea *pLine = FindLine ( Line ) ; 
   int WordNumber = pLine->WordNumber ;
   int WordCount = pLine->WordCount ;
   Palabra *pWord = FindWord ( WordNumber ) ;
   for ( int i=0; i<WordCount-1; i++, pWord=pWord->NextWord() ) {
      if ( Column < pWord->Position.x + pWord->Size.cx ) 
         break ;
   } /* endfor */

   // Return the word's address.
   return ( pWord ) ;

} /* endmethod */

/****************************************************************************
 *                                                                          *
 *  Borradora: Find Graphic Functions                                       *
 *                                                                          *
 ****************************************************************************/

Grafico *Borradora::FindGraphic ( int Index ) { 
   PoolEntry *pEntry = Graphics.Find ( Index ) ;
   if ( pEntry )
      return ( (Grafico*) pEntry->Data() ) ;
   else
      return ( 0 ) ;
} /* endmethod */

Grafico *Borradora::FindGraphic ( long Offset ) { 
   Grafico *pGraphic = FindGraphic ( (int) 0 ) ;
   while ( pGraphic ) {
      if ( Offset == pGraphic->Offset ) 
         break ;
      pGraphic = pGraphic->NextGraphic ( ) ;
   } /* endwhile */
   return ( pGraphic ) ;
} /* endmethod */

/****************************************************************************
 *                                                                          *
 *  Borradora: Insert - Insert Text at Offset                               *
 *                                                                          *
 ****************************************************************************/

void Borradora::Insert ( WorkSpace *PS, long offset, long length ) {

   #ifdef DEBUG_EDIT
      Log ( "Borradora(%p)::Insert(Offset=%i,Length=%i) Started.", this, offset, length ) ;
   #endif

   // Adjust all offsets.
   AdjustOffsets ( offset, length ) ;

   // If the change was not on this page, return.
   if ( offset+length < Offset ) 
      return ;
   if ( offset > Offset+Length ) 
      return ;

   // If we have a currently reset page (no words), just return.
   if ( Words.Query_Count() == 0 ) {
      #ifdef DEBUG_EDIT
         Log ( "Borradora(%p)::Insert(Offset=%i,Length=%i) Done.  Will not do anything to a reset page.", this, offset, length ) ;
      #endif
      return ;
   } /* endif */

   // Split the word with the inserted text starting point.
   Palabra *pWord = FindWord ( offset ) ;
   if ( offset > pWord->Offset ) {
      #ifdef DEBUG_EDIT
         Log ( "Borradora(%p)::Insert(Offset=%i,Length=%i) Splitting word %08X at insertion start.", this, offset, length, pWord ) ;
      #endif
      Split_Word ( pWord, offset ) ;
   } else {
      #ifdef DEBUG_EDIT
         Log ( "Borradora(%p)::Insert(Offset=%i,Length=%i) Did not need to split word at insertion start.", this, offset, length ) ;
      #endif
   } /* endif */

   // Split the word with the inserted text ending point.
   pWord = FindWord ( (long) ( offset + length ) ) ;
   if ( offset+length > pWord->Offset ) {
      #ifdef DEBUG_EDIT
         Log ( "Borradora(%p)::Insert(Offset=%i,Length=%i) Splitting word %08X at insertion end.", this, offset, length, pWord ) ;
      #endif
      Split_Word ( pWord, offset+length ) ;
   } else {
      #ifdef DEBUG_EDIT
         Log ( "Borradora(%p)::Insert(Offset=%i,Length=%i) Did not need to split word at insertion end.", this, offset, length ) ;
      #endif
   } /* endif */

   // Split up the inserted word.
   #ifdef DEBUG_EDIT
      Log ( "Borradora(%p)::Insert(Offset=%i,Length=%i) Breaking up the inserted text.", this, offset, length ) ;
   #endif
   long WorkingOffset = offset ;
   while ( WorkingOffset < offset+length ) {
      long WorkingLength ;
      GetWord ( pText, PS->Query_DBCS_Vector(), WorkingOffset, WorkingLength ) ;
      if ( WorkingOffset+WorkingLength > offset+length ) 
         WorkingLength -= (WorkingOffset+WorkingLength) - (offset+length) ;
      pWord = FindWord ( WorkingOffset ) ;
      if ( WorkingLength < pWord->Length ) 
         Split_Word ( pWord, WorkingOffset ) ;
      WorkingOffset += WorkingLength ;
   } /* endwhile */

   // Join the first inserted word with its predecessor if possible.
   #ifdef DEBUG_EDIT
      Log ( "Borradora(%p)::Insert(Offset=%i,Length=%i) Joining first word with predecessor if possible.", this, offset, length ) ;
   #endif
   pWord = FindWord ( offset ) ;
   if ( pWord ) {
      Palabra *pPrev = pWord->PrevWord() ;
      if ( pPrev ) {
         if ( NOT pWord->IsSeparator ( pText, PS->Query_DBCS_Vector() ) ) {
            if ( NOT pPrev->IsSeparator ( pText, PS->Query_DBCS_Vector() ) ) {
               Join_Words ( pPrev ) ;
            } /* endif */
         } /* endif */
      } /* endif */
   } /* endif */

   // Join the last inserted word with its predecessor if possible.
   #ifdef DEBUG_EDIT
      Log ( "Borradora(%p)::Insert(Offset=%i,Length=%i) Joining last word with predecessor if possible.", this, offset, length ) ;
   #endif
   pWord = FindWord ( (long) ( offset + length ) ) ;
   if ( pWord ) {
      Palabra *pPrev = pWord->PrevWord() ;
      if ( pPrev ) {
         if ( NOT pWord->IsSeparator ( pText, PS->Query_DBCS_Vector() ) ) {
            if ( NOT pPrev->IsSeparator ( pText, PS->Query_DBCS_Vector() ) ) {
               Join_Words ( pPrev ) ;
            } /* endif */
         } /* endif */
      } /* endif */
   } /* endif */

   #ifdef DEBUG_EDIT
      Log ( "Borradora(%p)::Insert(Offset=%i,Length=%i) Done.", this, offset, length ) ;
   #endif
}
 
/****************************************************************************
 *                                                                          *
 *  Borradora: Delete - Delete Text at Offset                               *
 *                                                                          *
 *    This function is used to update the word & graphic lists of a valid   *
 *  working page after a deletion and the accompanying AdjustOffsets has    *
 *  already occurred.                                                       *
 *                                                                          *
 ****************************************************************************/

void Borradora::Delete ( WorkSpace *PS, POINTL &PrinterResolution, long Offset, long Length ) {

   #ifdef DEBUG_EDIT
      Log ( "Borradora(%p)::Delete(Offset=%i,Length=%i) Started.", this, Offset, Length ) ;
   #endif

   // Delete from start to finish.
   pText->Delete ( Offset, Length ) ;

   // Mark the word where the deletion occurred for remeasuring.
   Palabra *pWord = FindWord ( Offset ) ;
   if ( pWord ) {
      #ifdef DEBUG_EDIT
         Log ( "Borradora(%p)::Delete(Offset=%i,Length=%i) Marking word %08X (offset %i, length %i) for remeasuring.", this, Offset, Length, pWord, pWord->Offset, pWord->Length ) ;
      #endif
      pWord->NeedsMeasuring = TRUE ;
   } /* endif */

   // Adjust all offsets.
   AdjustOffsets ( Offset, -Length ) ;

   // Split the word where the deletion took place.
   #ifdef DEBUG_EDIT
      Log ( "Borradora(%p)::Delete(Offset=%i,Length=%i) Splitting word if necessary.", this, Offset, Length ) ;
   #endif
   pWord = FindWord ( Offset ) ;
   if ( pWord ) 
      Split_Word ( pWord, Offset ) ;

   // Remove any words now of length zero from the working list.
   #ifdef DEBUG_EDIT
      Log ( "Borradora(%p)::Delete(Offset=%i,Length=%i) Removing any fully deleted words in offset range %i-%i.", this, Offset, Length, Offset?Offset-1:0, Offset+1 ) ;
   #endif
   pWord = FindWord ( long ( Offset ? Offset - 1 : 0 ) ) ;
   if ( pWord == 0 ) pWord = FindWord ( Offset ) ;
   while ( pWord AND ( pWord->Offset < Offset+1 ) ) {
      #ifdef DEBUG_EDIT
         Log ( "Borradora(%p)::Delete(Offset=%i,Length=%i)   Examining word %08X (offset %i, length %i).", this, Offset, Length, pWord, pWord->Offset, pWord->Length ) ;
      #endif
      Palabra *pNext = pWord->NextWord() ;
      if ( pWord->Length == 0 ) 
         Remove_Word ( pWord, PrinterResolution ) ;
      pWord = pNext ;
   } /* endwhile */

   // Join the words on either side of the deletion if possible.
   #ifdef DEBUG_EDIT
      Log ( "Borradora(%p)::Delete(Offset=%i,Length=%i) Joining words where possible.", this, Offset, Length ) ;
   #endif
   pWord = FindWord ( Offset ) ;
   if ( pWord ) {
      Palabra *pPrev = pWord->PrevWord() ;
      if ( pPrev ) {
         if ( NOT pWord->IsSeparator ( pText, PS->Query_DBCS_Vector() ) ) {
            if ( NOT pPrev->IsSeparator ( pText, PS->Query_DBCS_Vector() ) ) {
               Join_Words ( pPrev ) ;
            } /* endif */
         } /* endif */
      } /* endif */
   } /* endif */

   #ifdef DEBUG_EDIT
      Log ( "Borradora(%p)::Delete(Offset=%i,Length=%i) Done.", this, Offset, Length ) ;
   #endif
}
 
/****************************************************************************
 *                                                                          *
 *  Borradora: Remove Word from active list                                 *
 *                                                                          *
 ****************************************************************************/

void Borradora::Remove_Word ( Palabra *pWord, POINTL &PrinterResolution ) {

   #ifdef DEBUG_EDIT
      Log ( "Borradora(%p)::Remove_Word(%08X) Started.", this, pWord ) ;
   #endif

   // If this word is a graphic, invalidate the area it covered and remove it from the graphic list.
   if ( pWord->WasGraphic ) {
      Grafico *pGraphic = FindGraphic ( pWord->Offset ) ;
      #ifdef DEBUG_EDIT
         Log ( "Borradora(%p)::Remove_Word(%08X) Invalidating graphic rectangle (offset %i, address %08X).", this, pWord, pWord->Offset, pGraphic ) ;
      #endif
      if ( pGraphic ) {
         if ( pGraphic->OldSize.cx AND pGraphic->OldSize.cy ) {
            #ifdef DEBUG_INVALIDRECTANGLE
               Log ( "Borradora(%p)::Remove_Word(%08X) Invalidating rectangle (graphic).", this, pWord ) ;
            #endif
            RECTL Box ;
            pGraphic->ComputeBox ( pWord, Box, PrinterResolution, TRUE ) ;
            InvalidateRectangle ( InvalidRectangle, Box ) ;
         } /* endif */
         Graphics.Delete ( ((PoolEntry*)pGraphic)-1 ) ;
      } /* endif */

   // Otherwise, merge it's painting information with the previous (or next) word.
   } else {
      if ( pWord->OldSize.cx AND pWord->OldSize.cy AND pWord->OldMaxDescent ) {
         #ifdef DEBUG_INVALIDRECTANGLE
            Log ( "Borradora(%p)::Remove_Word(%08X) Invalidating rectangle (word).", this, pWord ) ;
         #endif
         InvalidateRectangle ( InvalidRectangle, pWord->OldPosition, pWord->OldSize, pWord->OldMaxDescent ) ;
      } /* endif */
   } /* endif */

   // Remove the word from the list.
   Words.Delete ( ((PoolEntry*)pWord)-1 ) ;

   #ifdef DEBUG_EDIT
      Log ( "Borradora(%p)::Remove_Word(%08X) Done.", this, pWord ) ;
   #endif
}
 
/****************************************************************************
 *                                                                          *
 *  Borradora: Join Word with the next one.                                 *
 *                                                                          *
 ****************************************************************************/

void Borradora::Join_Words ( Palabra *pWord ) {

   // Get pointer to next word.  Return if there isn't one.
   Palabra *pNext = pWord->NextWord ( ) ;
   if ( pNext == 0 ) 
      return ;

   #ifdef DEBUG_EDIT
      Log ( "Borradora(%p)::Join_Words(%08X) Started.  Old lengths %i,%i, Boxes %i,%i-%i,%i, %i,%i-%i,%i, Descents %i,%i.",
         this, pWord, pWord->Length, pNext->Length, 
         pWord->OldPosition.x, pWord->OldPosition.y-pWord->OldMaxDescent+1, 
         pWord->OldPosition.x+pWord->OldSize.cx, pWord->OldPosition.y-pWord->OldMaxDescent+1+pWord->OldSize.cy,
         pNext->OldPosition.x, pNext->OldPosition.y-pNext->OldMaxDescent+1,
         pNext->OldPosition.x+pNext->OldSize.cx, pNext->OldPosition.y-pNext->OldMaxDescent+1+pNext->OldSize.cy,
         pWord->OldMaxDescent, pNext->OldMaxDescent ) ;
   #endif

   // Invalidate the next word's rectangle, before it can be lost.
   if ( pNext->OldSize.cx AND pNext->OldSize.cy AND pNext->OldMaxDescent ) {
      #ifdef DEBUG_INVALIDRECTANGLE
         Log ( "Borradora(%p)::Join_Words(%08X) Invalidating rectangle.", this, pWord ) ;
      #endif
      InvalidateRectangle ( InvalidRectangle, pNext->OldPosition, pNext->OldSize, pNext->OldMaxDescent ) ;
   } /* endif */

   // Merge their information.
   pWord->Length += pNext->Length ;
   pWord->NeedsMeasuring = TRUE ;

   // Remove the next word from the list.
   #ifdef DEBUG_EDIT
      Log ( "Borradora(%p)::Join_Words(%08X) Removing word pool entry %08X.", this, pWord, ((PoolEntry*)pNext)-1 ) ;
   #endif
   Words.Delete ( ((PoolEntry*)pNext)-1 ) ;

   #ifdef DEBUG_EDIT
      Log ( "Borradora(%p)::Join_Words(%08X) Done.  New length %i, Size %ix%i, Descent %i.",
         this, pWord, pWord->Length, pWord->OldSize.cx, pWord->OldSize.cy, pWord->OldMaxDescent ) ;
   #endif
}
 
/****************************************************************************
 *                                                                          *
 *  Borradora: Split Word                                                   *
 *                                                                          *
 ****************************************************************************/

void Borradora::Split_Word ( Palabra *pWord, long Offset ) {

   // If the word needn't (or can't) be split, return at once.
   if ( Offset <= pWord->Offset )
      return ;
   if ( Offset >= pWord->Offset + pWord->Length )
      return ;

   #ifdef DEBUG_EDIT
      Log ( "Borradora(%p)::Split_Word(%08X,%i) Started.", this, pWord, Offset ) ;
   #endif

   // Insert the new word into the page word list.
   Palabra NewWord ;
   memset ( &NewWord, 0, sizeof(NewWord) ) ;
   Palabra *pNewWord = (Palabra*) ( Words.Insert ( ((PoolEntry*)pWord)-1, &NewWord ) ) -> Data ( ) ;

   // Update the new word.
   pNewWord->Offset = Offset ;
   pNewWord->Length = pWord->Length - ( Offset - pWord->Offset ) ;
   pNewWord->NeedsMeasuring = TRUE ;

   // Update the old word.
   pWord->Length = Offset - pWord->Offset ;
   pWord->NeedsMeasuring = TRUE ;

   #ifdef DEBUG_EDIT
      Log ( "Borradora(%p)::Split_Word(%08X,%i) Done.  Word %08X inserted after word %08X.", this, pWord, Offset, pNewWord, pWord ) ;
   #endif
}
 
/****************************************************************************
 *                                                                          *
 *  Adjust Offset & Length, plus or minus.                                  *
 *                                                                          *
 ****************************************************************************/

extern void AdjustOffset ( long Origin, long Delta, long &Offset, long &Length, BOOL FixedLength ) {

   #ifdef DEBUG_EDIT
      Log ( "AdjustOffset(%i,%i,%i,%i)", Origin, Delta, Offset, Length ) ;
   #endif

   // Insertion
   if ( Delta > 0 ) {

      // If the insert occurred before the object, increment the object's offset.
      if ( Offset > Origin ) {
         #ifdef DEBUG_EDIT
            Log ( "AdjustOffset: Insertion before object.  Offset %i incremented by %i.", Offset, Delta ) ;
         #endif
         Offset += Delta ;

      // Else, if the insert occurred before the object's end, increment the object's length.
      } else if ( Offset+Length > Origin ) {
         if ( FixedLength ) {
            #ifdef DEBUG_EDIT
               Log ( "AdjustOffset: Insertion before fixed object's end.  Offset %i incremented by %i.", Length, Delta ) ;
            #endif
            Offset += Delta ;
         } else {
            #ifdef DEBUG_EDIT
               Log ( "AdjustOffset: Insertion before object's end.  Length %i incremented by %i.", Length, Delta ) ;
            #endif
            Length += Delta ;
         } /* endif */

      // Else, if the insert occurred after the object's end, do nothing.
      } else {
         #ifdef DEBUG_EDIT
            Log ( "AdjustOffset: Insertion after object.  Nothing done." ) ;
         #endif

      } /* endif */

   // Deletion
   } else {

      // If deletion occurred entirely before the object, decrement the object's offset.
      if ( Origin-Delta <= Offset ) {
         #ifdef DEBUG_EDIT
            Log ( "AdjustOffset: Deletion entirely before object.  Offset %i decremented by %i.", Offset, -Delta ) ;
         #endif
         Offset += Delta ;

      // Else, if the deletion occurred entirely after the object, do nothing.
      } else if ( Origin >= Offset+Length ) {
         #ifdef DEBUG_EDIT
            Log ( "AdjustOffset: Deletion entirely after object.  Nothing done." ) ;
         #endif

      // Else, if the deletion completely covered the object, zero it.
      } else if ( ( Origin <= Offset ) AND ( Origin-Delta >= Offset+Length ) ) {
         #ifdef DEBUG_EDIT
            Log ( "AdjustOffset: Deletion of entire object." ) ;
         #endif
         Length = 0 ;
         Offset = Origin ;

      // Else, if the deletion occurred entirely inside the object, adjust the length.
      } else if ( ( Origin >= Offset ) AND ( Origin-Delta < Offset+Length ) ) {
         #ifdef DEBUG_EDIT
            Log ( "AdjustOffset: Deletion entirely within object.  Length %i decremented by %i.", Length, -Delta ) ;
         #endif
         Length += Delta ;

      // Else, if the deletion extended into the start of the object, adjust offset and length.
      } else if ( ( Origin < Offset ) AND ( Origin-Delta < Offset+Length ) ) {
         #ifdef DEBUG_EDIT
            Log ( "AdjustOffset: Deletion of head of object.  Offset %i adjusted to %i, length %i decremented by %i.", Offset, Origin, Length, Origin-Delta-Offset ) ;
         #endif
         Length -= Origin - Delta - Offset ;
         Offset = Origin ;

      // Else, if the deletion removed the tail end of the object, adjust length.
      } else if ( ( Origin >= Offset ) AND ( Origin-Delta >= Offset+Length ) ) {
         #ifdef DEBUG_EDIT
            Log ( "AdjustOffset: Deletion of tail of object.  Length %i adjusted by %i.", Length, Origin-Offset ) ;
         #endif
         Length = Origin - Offset ;

      } /* endif */

   } /* endif */
}

 
/****************************************************************************
 *                                                                          *
 *  Borradora: Adjust Offsets                                               *
 *                                                                          *
 ****************************************************************************/

void Borradora::AdjustOffsets ( long Origin, long Delta ) {

   #ifdef DEBUG_EDIT
      Log ( "Borradora(%p)::AdjustOffsets(%i,%i) Adjusting 'Offset' and 'Length' for page.", this, Origin, Delta ) ;
   #endif
   AdjustOffset ( Origin, Delta, Offset, Length, FALSE ) ;

   #ifdef DEBUG_EDIT
      Log ( "Borradora(%p)::AdjustOffsets(%i,%i) Adjusting 'Offset' and 'Length' for words.", this, Origin, Delta ) ;
   #endif
   Palabra *pWord = FindWord ( (int)0 ) ;
   while ( pWord ) {
      #ifdef DEBUG_EDIT
         Log ( "Borradora(%p)::AdjustOffsets(%i,%i) Adjusting 'Offset' and 'Length' for word %08X.", this, Origin, Delta, pWord ) ;
      #endif
      AdjustOffset ( Origin, Delta, pWord->Offset, pWord->Length, FALSE ) ;
      pWord = pWord->NextWord() ;
   } /* endwhile */

   #ifdef DEBUG_EDIT
      Log ( "Borradora(%p)::AdjustOffsets(%i,%i) Adjusting 'Offset' and 'Length' for lines.", this, Origin, Delta ) ;
   #endif
   Linea *pLine = FindLine ( (int)0 ) ;
   while ( pLine ) {
      #ifdef DEBUG_EDIT
         Log ( "Borradora(%p)::AdjustOffsets(%i,%i) Adjusting 'Offset' and 'Length' for line %08X.", this, Origin, Delta, pLine ) ;
      #endif
      AdjustOffset ( Origin, Delta, pLine->Offset, pLine->Length, FALSE ) ;
      pLine = pLine->NextLine() ;
   } /* endwhile */

   #ifdef DEBUG_EDIT
      Log ( "Borradora(%p)::AdjustOffsets(%i,%i) Adjusting 'Offset' and 'Length' for graphics.", this, Origin, Delta ) ;
   #endif
   Grafico *pGraphic = FindGraphic ( (int)0 ) ;
   while ( pGraphic ) {
      #ifdef DEBUG_EDIT
         Log ( "Borradora(%p)::AdjustOffsets(%i,%i) Adjusting 'Offset' and 'Length' for graphic %08X.", this, Origin, Delta, pGraphic ) ;
      #endif
      AdjustOffset ( Origin, Delta, pGraphic->Offset, pGraphic->Length, TRUE ) ;
      pGraphic = pGraphic->NextGraphic() ;
   } /* endwhile */

   #ifdef DEBUG_EDIT
      Log ( "Borradora(%p)::AdjustOffsets(%i,%i) Done.", this, Origin, Delta ) ;
   #endif
}
 
/****************************************************************************
 *                                                                          *
 *  Borradora: Update Word List                                             *
 *                                                                          *
 ****************************************************************************/

void Borradora::UpdateWordList ( WorkSpace *PS, int NumberOfPages, time_t Date, BOOL CurrentPageValid, long offset, int number, Estado &state, time_t FileDate  ) {

   #ifdef DEBUG_UPDATEWORDLIST
      Log ( "Borradora(%p)::UpdateWordList(&PS,NumberOfPages=%i,Date,CurrentPageValid=%s,offset=%i,number=%i,State) Started.  Offset %i.", 
         this, NumberOfPages, CurrentPageValid?"TRUE":"FALSE", offset, number, Offset ) ;
   #endif

   // If the page no longer valid, or starting conditions have changed, then reset it and return.
   if ( NOT CurrentPageValid OR ( offset != Offset ) OR ( number != PageNumber ) ) {
      Reset ( state ) ;
      #ifdef DEBUG_UPDATEWORDLIST
         Log ( "Borradora(%p)::UpdateWordList() Done.  Word list had to be reset.", this ) ;
      #endif
      return ;
   } /* endif */

   // Review the word list, remeasuring those words that have need of it.
   Estado WorkingState = State = state ;
   USHORT WorkingCRC = WorkingState.ComputeCRC ( ) ;
   long WorkingStateOffset = Offset ;
   Palabra *pLast(0), *pWord = FindWord ( 0 ) ;
   int WordNumber = 0 ;
   while ( pWord ) {
      #ifdef DEBUG_UPDATEWORDLIST
         Log ( "Borradora(%p)::UpdateWordList() Examining word %i (offset %i, length %i).  NeedsMeasuring:%s, WorkingCRC %04X, WordCRC %04X.",
            this, WordNumber, pWord->Offset, pWord->Length, pWord->NeedsMeasuring?"TRUE":"FALSE", WorkingCRC, pWord->CRC ) ;
      #endif
      if ( pWord->NeedsMeasuring OR ( WorkingCRC != pWord->CRC ) ) {
         if ( WorkingStateOffset != pWord->Offset ) {
            ComputeNextState ( PS, pText, WorkingStateOffset, pWord->Offset - WorkingStateOffset, WorkingState, WorkingState, number, NumberOfPages, FileDate ) ;
            WorkingCRC = WorkingState.ComputeCRC ( ) ;
            WorkingStateOffset = pWord->Offset ;
            #ifdef DEBUG_UPDATEWORDLIST
               Log ( "Borradora(%p)::UpdateWordList() Working state updated.  New working CRC %04X.", this, WorkingCRC ) ;
            #endif
         } /* endif */
         if ( pWord->NeedsMeasuring OR ( WorkingCRC != pWord->CRC ) ) {
            #ifdef DEBUG_UPDATEWORDLIST
               Log ( "Borradora(%p)::UpdateWordList() Measuring word.", this ) ;
            #endif
            pWord->CRC = WorkingCRC ;
            pWord->Measure ( pText, PS, WorkingState, number, NumberOfPages, Date ) ;
            WorkingCRC = WorkingState.ComputeCRC ( ) ;
            #ifdef DEBUG_UPDATEWORDLIST
               Log ( "Borradora(%p)::UpdateWordList() Working state updated.  New working CRC %04X.", this, WorkingCRC ) ;
            #endif
            pWord->NeedsMeasuring = FALSE ;
            pWord->NeedsPainting = TRUE ;
            #ifdef DEBUG_NEEDSPAINTING
               Log ( "Borradora(%p)::UpdateWordList() Word %i marked for painting because it needed measuring or its working CRC has changed.", this, WordNumber ) ;
            #endif
         } /* endif */
      } /* endif */
      pLast = pWord ;
      pWord = pWord->NextWord ( ) ;
      WordNumber ++ ;
   } /* endwhile */

   // If necessary, update the working state to the end of the word list.
   if ( pLast && ( WorkingStateOffset != pLast->Offset+pLast->Length ) )
      ComputeNextState ( PS, pText, WorkingStateOffset, pLast->Offset+pLast->Length-WorkingStateOffset, 
         WorkingState, WorkingState, number, NumberOfPages, FileDate ) ;

   // Save the state after the last word generated, for generating the next if necessary.
   LastState = WorkingState ;

   #ifdef DEBUG_UPDATEWORDLIST
      Log ( "Borradora(%p)::UpdateWordList() Done.", this ) ;
   #endif
}
 
/****************************************************************************
 *                                                                          *
 *  Borradora: Compute Invalid Rectangle                                    *
 *                                                                          *
 ****************************************************************************/

RECTL *Borradora::ComputeInvalidRectangle ( WorkSpace *PS, POINTL &PrinterResolution, int NumberOfPages, time_t FileDate ) {

   #ifdef DEBUG_INVALIDRECTANGLE
      Log ( "Borradora(%p)::ComputeInvalidRectangle() Started.", this ) ;
      Log ( "  Invalid rectangle: %i,%i-%i,%i.",
         InvalidRectangle.xLeft, InvalidRectangle.yBottom, InvalidRectangle.xRight, InvalidRectangle.yTop ) ;
   #endif

   // Check header & footer.
   PS->SetState ( State ) ; Estado EffectiveState ;
   ComputeEffectiveState ( PS, pText, PS->Query_DBCS_Vector(), Offset, State, EffectiveState, PageNumber, NumberOfPages, FileDate ) ;
   if ( EffectiveState.ComputeCRC() != OldEffCRC ) {

      POINTL Position = { OldEffState.HeaderState.Margins.xLeft, OldEffState.HeaderState.Margins.yTop-OldEffState.HeaderHeight } ;
      SIZEL Size = { OldEffState.HeaderState.Margins.xRight-OldEffState.HeaderState.Margins.xLeft, OldEffState.HeaderHeight } ;
      InvalidateRectangle ( InvalidRectangle, Position, Size, 0 ) ;
      Position.x = EffectiveState.HeaderState.Margins.xLeft ;
      Position.y = EffectiveState.HeaderState.Margins.yTop - EffectiveState.HeaderHeight ;
      Size.cx = EffectiveState.HeaderState.Margins.xRight - EffectiveState.HeaderState.Margins.xLeft ;
      Size.cy = EffectiveState.HeaderHeight ;
      InvalidateRectangle ( InvalidRectangle, Position, Size, 0 ) ;

      Position.x = OldEffState.FooterState.Margins.xLeft ; 
      Position.y = OldEffState.FooterState.Margins.yBottom ;
      Size.cx = OldEffState.FooterState.Margins.xRight - OldEffState.FooterState.Margins.xLeft ; 
      Size.cy = OldEffState.FooterHeight ;
      InvalidateRectangle ( InvalidRectangle, Position, Size, 0 ) ;
      Position.x = EffectiveState.FooterState.Margins.xLeft ;
      Position.y = EffectiveState.FooterState.Margins.yBottom ;
      Size.cx = EffectiveState.FooterState.Margins.xRight - EffectiveState.FooterState.Margins.xLeft ;
      Size.cy = EffectiveState.FooterHeight ;
      InvalidateRectangle ( InvalidRectangle, Position, Size, 0 ) ;

      OldEffState = EffectiveState ;
      OldEffCRC = EffectiveState.ComputeCRC ( ) ;

   } /* endif */

   // Check each graphic.
   #ifdef DEBUG_INVALIDRECTANGLE
      Log ( "Borradora(%p)::ComputeInvalidRectangle() Checking %i graphics.", this, Graphics.Query_Count() ) ;
   #endif
   Grafico *pGraphic = FindGraphic ( (int) 0 ) ;
   while ( pGraphic ) {
      #ifdef DEBUG_INVALIDRECTANGLE
         Log ( "Borradora(%p)::ComputeInvalidRectangle() Checking graphic %08X.", this, pGraphic ) ;
      #endif
      if ( pGraphic->NeedsPainting ) {
         if ( pGraphic->OldSize.cx AND pGraphic->OldSize.cy AND (
            ( pGraphic->CRC != pGraphic->OldCRC )
            OR memcmp ( &pGraphic->Position, &pGraphic->OldPosition, sizeof(pGraphic->Position) )
            OR memcmp ( &pGraphic->Size, &pGraphic->OldSize, sizeof(pGraphic->Size) ) ) ) {
            #ifdef DEBUG_INVALIDRECTANGLE
               Log ( "Borradora(%p)::ComputeInvalidRectangle() Invalidating graphic %08X's old area.", this, pGraphic ) ;
            #endif
            RECTL Box ;
            pGraphic->ComputeOldBox ( FindWord(pGraphic->Offset), Box, PrinterResolution, TRUE ) ;
            InvalidateRectangle ( InvalidRectangle, Box ) ;
         } /* endif */
         #ifdef DEBUG_INVALIDRECTANGLE
            Log ( "Borradora(%p)::ComputeInvalidRectangle() Invalidating graphic %08X's current area.", this, pGraphic ) ;
         #endif
         RECTL Box ;
         pGraphic->ComputeBox ( FindWord(pGraphic->Offset), Box, PrinterResolution, TRUE ) ;
         InvalidateRectangle ( InvalidRectangle, Box ) ;
         pGraphic->NeedsPainting = FALSE ;
         pGraphic->OldPosition   = pGraphic->Position ;
         pGraphic->OldSize       = pGraphic->Size ;
         pGraphic->OldAspect     = pGraphic->Aspect ;
         pGraphic->OldCRC        = pGraphic->CRC ;
      } /* endif */
      pGraphic = pGraphic->NextGraphic ( ) ;
   } /* endwhile */

   // Check every word.
   Linea *pLine = FindLine ( Lines.Query_Count() - 1 ) ;
   int WordCount = pLine->WordNumber + pLine->WordCount ;
   #ifdef DEBUG_INVALIDRECTANGLE
      Log ( "Borradora(%p)::ComputeInvalidRectangle() Checking %i words.", this, WordCount ) ;
   #endif
   Palabra *pWord = FindWord ( (int) 0 ) ;
   for ( int i=0; i<WordCount; i++, pWord=pWord->NextWord() ) {
      #ifdef DEBUG_INVALIDRECTANGLE
         Log ( "Borradora(%p)::ComputeInvalidRectangle() Checking word %i.", this, i ) ;
      #endif
      if ( pWord->NeedsPainting ) {
         #ifdef DEBUG_INVALIDRECTANGLE
            Log ( "Borradora(%p)::ComputeInvalidRectangle() Word %i needs painting.  OldPosition %i,%i, OldSize %ix%i, OldDescent %i.", 
               this, i, pWord->OldPosition.x, pWord->OldPosition.y, pWord->OldSize.cx, pWord->OldSize.cy, pWord->OldMaxDescent ) ;
         #endif
         if ( pWord->OldSize.cx AND pWord->OldSize.cy AND pWord->OldMaxDescent AND (
            ( pWord->CRC != pWord->OldCRC )
            OR memcmp ( &pWord->Position, &pWord->OldPosition, sizeof(pWord->Position) )
            OR memcmp ( &pWord->Size, &pWord->OldSize, sizeof(pWord->Size) )
            OR ( pWord->MaxDescent != pWord->OldMaxDescent ) ) ) {
            #ifdef DEBUG_INVALIDRECTANGLE
               Log ( "Borradora(%p)::ComputeInvalidRectangle() Invalidating word %i's old cell.", this, i ) ;
            #endif
            InvalidateRectangle ( InvalidRectangle, pWord->OldPosition, pWord->OldSize, pWord->OldMaxDescent ) ;
         } /* endif */
         if ( pWord->Size.cx ) {
            #ifdef DEBUG_INVALIDRECTANGLE
               Log ( "Borradora(%p)::ComputeInvalidRectangle() Invalidating word %i's current cell.", this, i ) ;
            #endif
            InvalidateRectangle ( InvalidRectangle, pWord->Position, pWord->Size, pWord->MaxDescent ) ;
         } /* endif */
         pWord->NeedsPainting = FALSE ;
         pWord->OldPosition   = pWord->Position ;
         pWord->OldSize       = pWord->Size ;
         pWord->OldMaxDescent = pWord->MaxDescent ;
         pWord->OldCRC        = pWord->CRC ;
      } /* endif */
   } /* endfor */

   // Return the rectangle needing update.  Reset the retained rectangle.
   static RECTL Rectangle ;
   Rectangle = InvalidRectangle ;
   InvalidRectangle.xLeft = InvalidRectangle.yBottom = 0x7FFFFFFF ;
   InvalidRectangle.xRight = InvalidRectangle.yTop = 0 ;
   #ifdef DEBUG_INVALIDRECTANGLE
      Log ( "Borradora(%p)::ComputeInvalidRectangle() Done.", this ) ;
   #endif
   return ( &Rectangle ) ;
}
 
/****************************************************************************
 *                                                                          *
 *  Borradora: Dump                                                         *
 *                                                                          *
 ****************************************************************************/

void Borradora::Dump ( int indent ) {

   Log ( "%*sBorradora %p:", indent, "", this ) ;
   Log ( "%*sOffset, Length: %i, %i", indent, "", Offset, Length ) ;
   Log ( "%*sState:", indent, "" ) ;  State.Dump ( indent+2 ) ;
   Log ( "%*sState CRC: %04X", indent, "", CRC ) ;
   Log ( "%*sPageNumber: %i", indent, "", PageNumber ) ;
   Log ( "%*sLastState:", indent, "" ) ;  LastState.Dump ( indent+2 ) ;

   Log ( "%*sRetainedLength: %i", indent, "", RetainedLength ) ;
   Log ( "%*sCurrentLength: %i", indent, "", CurrentLength ) ;

   Log ( "%*sWords:", indent, "" ) ;
   Palabra *pWord = FindWord ( (int) 0 ) ;
   int Count = 0 ;
   while ( pWord ) {
      Count ++ ;
      Log ( "%*s  Word %i(%08X):", indent, "", Count, pWord ) ;
      pWord->Dump ( indent+4, pText ) ;
      pWord = pWord->NextWord() ;
   } /* endwhile */

   Log ( "%*sLines:", indent, "" ) ;
   Linea *pLine = FindLine ( (int) 0 ) ;
   Count = 0 ;
   while ( pLine ) {
      Count ++ ;
      Log ( "%*s  Line %i(%08X):", indent, "", Count, pLine ) ;
      pLine->Dump ( indent+4 ) ;
      pLine = pLine->NextLine() ;
   } /* endwhile */

   Log ( "%*sGraphics:", indent, "" ) ;
   Grafico *pGraphic = FindGraphic ( (int) 0 ) ;
   Count = 0 ;
   while ( pGraphic ) {
      Count ++ ;
      Log ( "%*s  Graphic %i(%08X):", indent, "", Count, pGraphic ) ;
      pGraphic->Dump ( indent+4 ) ;
      pGraphic = pGraphic->NextGraphic() ;
   } /* endwhile */
}

