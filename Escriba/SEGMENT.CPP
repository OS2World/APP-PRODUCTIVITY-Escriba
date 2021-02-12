/**************************************************************** SEGMENT.CPP
 *                                                                          *
 *                           Class: Line Segment                            *
 *                                                                          *
 ****************************************************************************/

#include "System.h"

#include "DocSup.h"
#include "Document.h"
#include "Segment.h"

/****************************************************************************
 *                                                                          *
 *  Method: Load words into segment, as many as will fit.                   *
 *                                                                          *
 *  Returns: The number of words that fit, negative if a new graphic was    *
 *    encountered.                                                          *
 *                                                                          *
 ****************************************************************************/

int Segment::Load ( WorkSpace *PS, Text *pText, Borradora &Page, Linea &Line, Palabra* &pWord, 
   Estado &EffectiveState, Estado &WorkingState, POINTL &PrinterResolution, int Paragraph, 
   int NumberOfPages, time_t FileDate ) {

   // Set the segment starting word.
   pFirst = pWord ;
   WordNumber = Line.WordNumber ;

   // Set our current column position.
   ULONG Column = Rectangle.xLeft ;

   // We've found nothing yet.
   BOOL NothingYet ( TRUE ) ;

   // Zero words have fit in yet.
   int WordsFit ( 0 ) ;

   // While there are words available to pack in . . .
   while ( pWord ) {

      // Get a pointer to the current character.
      PUCHAR pByte ;
      pText->QueryBytePtr ( pWord->Offset, pByte ) ;

      // If space, and nothing but spaces precede it, and not paragraph start, make it dormant.
      if ( NothingYet ) {
         if ( NOT Paragraph ) {
            if ( IsSpace ( *pByte ) OR IsTab ( *pByte ) ) {
               pWord->Dormant = TRUE ;
            } else {
               NothingYet = FALSE ;
            } /* endif */
         } else {
            NothingYet = FALSE ;
         } /* endif */
      } /* endif */

      // If tab, compute its stop.
      if ( IsTab ( *pByte ) ) {
         for ( int i=0; i<WorkingState.TabCount; i++ ) 
            if ( WorkingState.Tabs[i] > (LONG)Column )
               break ;
         pWord->Size.cx = 0 ;
         if ( i < WorkingState.TabCount ) 
            pWord->Size.cx = WorkingState.Tabs[i] - Column ;
      } /* endif */

      // If word is a separator, compute its size.
      if ( IsSeparator ( pByte ) ) {
         pWord->Size.cx = Rectangle.xRight - Rectangle.xLeft ;
         pWord->Size.cy = PS->QueryAscent() + PS->QueryDescent() ;
      } /* endif */

      // If word is a graphic, determine if it interferes with prior formatting.  Return FALSE if so.
      if ( IsGraphic ( pByte ) ) {
         pWord->WasGraphic = TRUE ;
         Grafico *pGraphic = Page.FindGraphic ( pWord->Offset ) ;
         if ( pGraphic == 0 ) {
            WorkSpace PS = WorkSpace ( "Documento::FormatLine", 0, 0, FALSE, 8500, 11000 ) ;
            Page.BuildGraphic ( &PS, &Line, pWord, pByte, NumberOfPages, FileDate ) ;
            pGraphic = Page.FindGraphic ( pWord->Offset ) ;
         } /* endif */
         if ( pGraphic->Position.x == GRAPHIC_NEXT_CHAR ) {
            pWord->Size.cx = pGraphic->Size.cx + pGraphic->BorderWidth * 2 ;
            pWord->Size.cy = pGraphic->Size.cy + pGraphic->BorderWidth * 2 ;
         } else {
            if ( pWord->Offset >= Page.Query_Offset()+Page.Query_CurrentLength() ) {
               if ( pGraphic AND NOT pGraphic->Background ) {
                  RECTL Rectangle ;
                  pGraphic->ComputeBox ( Page.FindWord(pGraphic->Offset), Rectangle, PrinterResolution, TRUE ) ;
                  long FormattedBottom = Line.Top - Line.Height - 1 ;
                  if ( Rectangle.yTop >= FormattedBottom ) 
                     return ( -WordsFit-1 ) ;
               } /* endif */
            } /* endif */
         } /* endif */
      } /* endif */

      // If word will not fit in the segment, break out of here.
      if ( NOT pWord->Dormant AND pWord->Size.cx AND ( Column + pWord->Size.cx > Rectangle.xRight ) ) 
         break ;

      // It fit.  Move on to the next word unless the current word is EOL.
      pLast = pWord ;
      Column += pWord->Dormant ? 0 : pWord->Size.cx ;
      WordsFit ++ ;
      pText->QueryBytePtr ( pWord->Offset, pByte ) ;
      if ( IsEndOfLine(pByte) )
         break ;

      // Move on to the next word.
      pWord = pWord->NextWord ( ) ;

   } /* endwhile scanning over line words */

   // Return the number of words that fit.
   // The updated state and word pointer were passed in by reference, 
   //   so they are already updated and returned.
   return ( WordsFit ) ;

} /* endmethod */

/****************************************************************************
 *                                                                          *
 *  Method: Justify words within segment.                                   *
 *                                                                          *
 ****************************************************************************/

void Segment::Justify ( Borradora *Page, WorkSpace *PS, Estado &State, int PageNumber, int NumberOfPages, time_t FileDate ) {

   // If the segment is empty, just return.
   if ( ( pFirst == NULL ) OR ( pLast == NULL ) )
      return ;

   // Compute effective state of the segment's start.
   Estado EffectiveState ;
   ComputeEffectiveState ( PS, Page->Query_Text(), PS->Query_DBCS_Vector(), pFirst->Offset, State, EffectiveState, PageNumber, NumberOfPages, FileDate ) ;

   // Determine whether or not this line is a separator bar.
   PUCHAR pByte ;
   Page->Query_Text()->QueryBytePtr ( pFirst->Offset, pByte ) ;
   BOOL Separator = IsSeparator ( pByte ) ;

   // Determine whether or not this line starts a paragraph.
   BOOL Paragraph = pFirst->StartsParagraph ( Page->Query_Text(), PS ) ;

   // Determine justification type and margins.
   int Justification = EffectiveState.Justify ;
   ULONG LeftMargin = max ( Rectangle.xLeft, max ( 0, EffectiveState.Margins.xLeft + ( Separator ? 0 : ( Paragraph ? ( EffectiveState.Indent>0 ? EffectiveState.Indent : 0 ) : ( EffectiveState.Indent>=0 ? 0 : -EffectiveState.Indent ) ) ) ) ) ;
   ULONG RightMargin = min ( Rectangle.xRight, EffectiveState.Margins.xRight ) ;

   //Mark the first word as starting the segment.  This will help Render.
   pFirst->SegmentStart = TRUE ;

   //Trim leading white space.  All leading spaces, and if not justifying
   //  to the left, all leading tabs, are made dormant.  Do not do this when
   //  this segment starts a paragraph.
   int Index = WordNumber ;
   if ( NOT Paragraph ) {
      Palabra *pWord = pFirst ; 
      while ( pWord ) {
         PUCHAR pByte ;
         Page->Query_Text()->QueryBytePtr ( pWord->Offset, pByte ) ;
         if ( NOT IsSpace ( *pByte ) AND NOT IsTab ( *pByte ) )
            break ;
         pWord->Dormant = TRUE ;
         if ( pWord == pLast ) 
            break ;
         pWord = pWord->NextWord ( ) ;
         Index ++ ;
      } /* endwhile */
   } /* endif */

   // Trim trailing white space.
   Palabra *pWord = pLast ;
   Index = 0 ;
   while ( pWord ) {
      PUCHAR pByte ;
      Page->Query_Text()->QueryBytePtr ( pWord->Offset, pByte ) ;
      if ( NOT IsSpace ( *pByte ) AND NOT IsTab ( *pByte ) )
         break ;
      pWord->Dormant = TRUE ;
      if ( pWord == pFirst ) 
         break ;
      pWord = pWord->PrevWord ( ) ;
      Index ++ ;
   } /* endwhile */

   // Determine the segment width.
   ULONG Width = 0 ;
   pWord = pFirst ;
   while ( pWord ) {
      if ( NOT pWord->Dormant ) 
         Width += pWord->Size.cx ;
      if ( pWord == pLast )
         break ;
      pWord = pWord->NextWord ( ) ;
   } /* endwhile */

   // Determine where to start the segment.
   POINTL Cursor ;
   ULONG Space ;
   switch ( Justification ) {

      case JUSTIFY_LEFT:
         Cursor.x = LeftMargin ;
         break ;

      case JUSTIFY_CENTER:
         Cursor.x = LeftMargin + ( RightMargin - LeftMargin - Width ) / 2 ;
         break ;

      case JUSTIFY_RIGHT:
         Cursor.x = RightMargin - Width ;
         break ;

      case JUSTIFY_FULL: {
         ULONG Default ;
         USHORT Spaces = 0 ;
         ULONG Text = 0 ;
         BOOL UseDefault = FALSE ;

         Cursor.x = LeftMargin ;
         pWord = pFirst ;
         while ( pWord ) {
            PUCHAR pByte ;
            Page->Query_Text()->QueryBytePtr ( pWord->Offset, pByte ) ;
            if ( NOT pWord->Dormant ) {
               if ( IsSpace ( *pByte ) ) {
                  Spaces ++ ;
                  Default = pWord->Size.cx ;
               } else if ( IsEndOfLine ( pByte ) ) {
                  UseDefault = TRUE ;
               } else {
                  Text += pWord->Size.cx ;
               } /* endif */
            } /* endif */
            if ( pWord == pLast ) 
               break ;
            pWord = pWord->NextWord ( ) ;
         } /* endwhile */
         if ( Spaces AND NOT UseDefault )
            Space = ( RightMargin - LeftMargin - Text ) / Spaces ;
         else
            Space = Default ;
         break ; }

   } /* endswitch */

   // Distribute the words across the segment.
   pWord = pFirst ;
   Index = WordNumber ;
   while ( pWord ) {
      if ( pWord->Position.x != Cursor.x ) {
         pWord->Position.x = Cursor.x ;
         pWord->NeedsPainting = TRUE ;
      } /* endif */
      if ( NOT pWord->Dormant ) {
         PUCHAR pByte ;
         Page->Query_Text()->QueryBytePtr ( pWord->Offset, pByte ) ;
         if ( pByte AND IsSpace ( *pByte ) AND ( Justification == JUSTIFY_FULL ) ) {
            pWord->Size.cx = Space ;
         } /* endif */
         Cursor.x += pWord->Size.cx ;
      } /* endif */
      if ( pWord == pLast )
         break ;
      pWord = pWord->NextWord ( ) ;
      Index ++ ;
   } /* endwhile */
}
 
/****************************************************************************
 *                                                                          *
 *  Method: Split segment if graphic intersects it.                         *
 *                                                                          *
 ****************************************************************************/

void Segment::Split ( RECTL & Cell ) {

   // If graphic above the segment, just return.
   if ( Cell.yBottom > Rectangle.yTop )
      return ;

   // If graphic below the segment, just return.
   if ( Cell.yTop < Rectangle.yBottom )
      return ;

   // If graphic entirely before this segment, just return.
   if ( Cell.xRight < Rectangle.xLeft )
      return ;

   // If graphic entirely after this segment, pass the request on to the next.
   if ( Cell.xLeft > Rectangle.xRight ) {
      if ( Next )
         Next->Split ( Cell ) ;
      return ;
   } /* endif */

   // If graphic completely covers this segment, reduce this segment to nothing.
   if ( ( Cell.xLeft < Rectangle.xLeft ) AND ( Cell.xRight > Rectangle.xRight ) ) {
      Rectangle.xRight = Rectangle.xLeft - 1 ;
      if ( Next )
         Next->Split ( Cell ) ;
      return ;
   } /* endif */

   // If graphic covers left end of this segment, chop the left end of the segment off.
   if ( ( Cell.xLeft < Rectangle.xLeft ) AND ( Cell.xRight < Rectangle.xRight ) ) {
      Rectangle.xLeft = Cell.xRight ;
      return ;
   } /* endif */

   // If graphic covers right end of this segment, chop the right end of the segment off
   //   and pass the information on to the next segment.
   if ( ( Cell.xLeft > Rectangle.xLeft ) AND ( Cell.xRight > Rectangle.xRight ) ) {
      Rectangle.xRight = Cell.xLeft ;
      if ( Next )
         Next->Split ( Cell ) ;
      return ;
   } /* endif */

   // Otherwise, we're taking a bite out of this segment.  Split the segment.
   RECTL Cell1, Cell2 ;
   Cell1.xLeft = Rectangle.xLeft ;
   Cell2.xLeft = Cell.xRight ;
   Cell1.xRight = Cell.xLeft ;
   Cell2.xRight = Rectangle.xRight ;
   Cell1.yTop = Cell2.yTop = Rectangle.yTop ;
   Cell1.yBottom = Cell2.yBottom = Rectangle.yBottom ;

   Rectangle = Cell1 ;
   Segment *NewSegment = new Segment ( Cell2 ) ;

   NewSegment->SetNext ( Next ) ;
   Next = NewSegment ;
}

