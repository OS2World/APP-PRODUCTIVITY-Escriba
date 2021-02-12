/***************************************************************** FORMAT.CPP
 *                                                                          *
 *                     Document Formatting Functions                        *
 *                                                                          *
 ****************************************************************************/

#include "System.h"

#include <stddef.h>

#include "DateFmt.h"
#include "Debug.h"
#include "DocSup.h"
#include "Document.h"
#include "Escriba.h"
#include "Process.h"
#include "Segment.h"
#include "Thread.h"

// #define DEBUG
// #define DEBUG_LOCK
// #define DEBUG_BLOCK
// #define DEBUG_DEQUEUE
// #define DEBUG_FORMATPAGE
// #define DEBUG_FORMATLINE
// #define DEBUG_FORMATLINE_DETAIL
// #define DEBUG_NEEDSPAINTING
// #define DEBUG_SEGMENT
// #define DEBUG_THREAD
// #define DEBUG_FOREGROUND
// #define DEBUG_SUPERVISOR

#ifdef DEBUG
   #define DEBUG_LOCK
   #define DEBUG_BLOCK
   #define DEBUG_DEQUEUE
   #define DEBUG_FORMATPAGE
   #define DEBUG_FORMATLINE
   #define DEBUG_FORMATLINE_DETAIL
   #define DEBUG_NEEDSPAINTING
   #define DEBUG_SEGMENT
   #define DEBUG_THREAD
   #define DEBUG_FOREGROUND
   #define DEBUG_SUPERVISOR
#endif

 
/****************************************************************************
 *                                                                          *
 *                         Definitions & Declarations                       *
 *                                                                          *
 ****************************************************************************/

  // Static Data

#ifdef DEBUG_DEQUEUE
static char *CommandNames [] = {
   "NOP",
   "InsertModeToggle",
   "InsertKey",
   "InsertText",
   "Clear",
   "Paste",
   "Undo",
   "Redo"
} ;
#endif

 
/****************************************************************************
 *                                                                          *
 *  Documento: Request permission to commence direct document editing       *
 *                                                                          *
 *  (Called by Foreground Thread)                                           *
 *                                                                          *
 ****************************************************************************/

void Documento::CommenceEdit ( ) {
   SupervisorObject->CommenceEdit ( ) ;
}
 
/****************************************************************************
 *                                                                          *
 *  Documento: Tell Format Supervisor that direct editing is done.          *
 *                                                                          *
 *  (Called by Foreground Thread)                                           *
 *                                                                          *
 ****************************************************************************/

void Documento::CompleteEdit ( ) {
   SupervisorObject->CompleteEdit ( ) ;
}
 
/****************************************************************************
 *                                                                          *
 *  Documento: Set Current Page Dirty                                       *
 *                                                                          *
 *  (Used by Supervisor Thread)                                             *
 *                                                                          *
 ****************************************************************************/

void Documento::SetPageDirty ( ) {
   for ( int i=0; i<NumberOfPages; i++ ) {
      if ( ( Pages[i].Offset <= Offset ) AND ( Pages[i].Offset+Pages[i].Length > Offset ) ) {
         Pages[i].NeedsFormat = TRUE ;
         break ;
      } /* endif */
   } /* endif */
}
 
/****************************************************************************
 *                                                                          *
 *  Documento: Set Changed Pages Dirty                                      *
 *                                                                          *
 *  (Called by Foreground Thread)                                           *
 *                                                                          *
 ****************************************************************************/

void Documento::SetPagesDirty ( long Start, long End ) {

   #ifdef DEBUG_FOREGROUND
      Log ( "Documento::SetPagesDirty(%i,%i)", Start, End ) ;
   #endif

   for ( int i=0; i<NumberOfPages; i++ ) {

      if ( Pages[i].Offset > End )
         continue ;

      if ( Pages[i].Offset + Pages[i].Length <= Start )
         continue ;

      #ifdef DEBUG_FOREGROUND
         Log ( "Documento::SetPagesDirty(%i,%i) Marking page %i dirty.", Start, End, i ) ;
      #endif

      Pages[i].NeedsFormat = TRUE ;

   } /* endif */

}
 
/****************************************************************************
 *                                                                          *
 *  Document: Format the first dirty page in the document                   *
 *                                                                          *
 *  (Used by Supervisor Thread)                                             *
 *                                                                          *
 ****************************************************************************/

void Documento::FormatFirstDirtyPage ( Event &RequestAnotherFormat, Event &CurrentPageDone, BOOL SetColumn ) {

   #ifdef DEBUG_SUPERVISOR
      Log ( "Documento::FormatFirstDirtyPage() Starting." ) ;
   #endif

   for ( int i=0; i<=NumberOfPages; i++ ) {

      #ifdef DEBUG_SUPERVISOR
         Log ( "Documento::FormatFirstDirtyPage: Checking page %i of %i.%s", 
            i+1, NumberOfPages, Pages[i].NeedsFormat?"  Needs format.":"" ) ;
      #endif

      if ( Pages[i].NeedsFormat AND ( Pages[i].Offset < pText->QueryCurrentSize() ) ) {

         // Format the dirty page.
         BOOL UsedCurrentPage = FormatPage ( i ) ;

         // If this is the current page, update the window.
         if ( UpdateWindow ( i, SetColumn, UsedCurrentPage ) ) {
            #ifdef DEBUG_SUPERVISOR
               Log ( "Documento::FormatFirstDirtyPage: Unlocking page." ) ;
            #endif
            SupervisorObject->PageLockRelease ( ) ;
            #ifdef DEBUG_SUPERVISOR
               Log ( "Documento::FormatFirstDirtyPage: Posting current page completion." ) ;
            #endif
            CurrentPageDone.Post() ;
            #ifdef DEBUG_SUPERVISOR
               Log ( "Documento::FormatFirstDirtyPage: Lowering formatter's priority." ) ;
            #endif
            DosSetPriority ( PRTYS_THREAD, PRTYC_REGULAR, PRTYD_MINIMUM, 0 ) ;
         } /* endif */

         // Post request over, to cause another search for dirty pages.
         #ifdef DEBUG_SUPERVISOR
            Log ( "Documento::FormatFirstDirtyPage: Requesting another format on next cycle." ) ;
         #endif
         RequestAnotherFormat.Post() ;

         break ;

      } /* endif */

      // If this page was the current one, then release it.  It didn't need formatting.
      if ( ( Offset >= Pages[i].Offset ) AND ( Offset < Pages[i].Offset+Pages[i].Length ) ) {

         // Release the current page.
         #ifdef DEBUG_SUPERVISOR
            Log ( "Documento::FormatFirstDirtyPage: Unlocking page." ) ;
         #endif
         SupervisorObject->PageLockRelease ( ) ;

         // Signal it's completion.
         #ifdef DEBUG_SUPERVISOR
            Log ( "Documento::FormatFirstDirtyPage: Posting current page completion." ) ;
         #endif
         CurrentPageDone.Post() ;

         // Put formatter down to low priority.
         #ifdef DEBUG_SUPERVISOR
            Log ( "Documento::FormatFirstDirtyPage: Lowering formatter's priority." ) ;
         #endif
         DosSetPriority ( PRTYS_THREAD, PRTYC_REGULAR, PRTYD_MINIMUM, 0 ) ;

      } /* endif */

   } /* endfor */

   // If document not completely formatted, continue the format.
   Pagina *LastPage = Pages + NumberOfPages ;
   if ( LastPage->Offset < pText->QueryCurrentSize() ) {
      LastPage->NeedsFormat = TRUE ;
      #ifdef DEBUG_SUPERVISOR
         Log ( "Documento::FormatFirstDirtyPage: Resuming incomplete format." ) ;
      #endif
      RequestAnotherFormat.Post() ;
      return ;
   } /* endif */

   // If nothing needed doing, release anybody waiting for the formatter to complete.
   #ifdef DEBUG_SUPERVISOR
      Log ( "Documento::FormatFirstDirtyPage: Unlocking page (nothing to do)." ) ;
   #endif
   SupervisorObject->PageLockRelease() ;

   #ifdef DEBUG_SUPERVISOR
      Log ( "Documento::FormatFirstDirtyPage: Posting current page completion." ) ;
   #endif
   CurrentPageDone.Post() ;

   #ifdef DEBUG_SUPERVISOR
      Log ( "Documento::FormatFirstDirtyPage: Lowering formatter's priority." ) ;
   #endif
   DosSetPriority ( PRTYS_THREAD, PRTYC_REGULAR, PRTYD_MINIMUM, 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *  Document: Page Format                                                   *
 *                                                                          *
 *  Returns TRUE if the current page buffer was used.                       *
 *                                                                          *
 *  (Mainly used by the Supervisor, but sometimes by the Window)            *
 *                                                                          *
 ****************************************************************************/

BOOL Documento::FormatPage ( int Page ) {

   #ifdef DEBUG_FORMATPAGE
      Log ( "Documento::FormatPage(%i) Starting at offset %i.", Page, Pages[Page].Offset ) ;
   #endif

   // Reset the page format request.
   Pages[Page].NeedsFormat = FALSE ;

   // If this appears to be the current page, restore the working page from it first.
   BOOL UseCurrentPage = FALSE ;
   if ( CurrentPageValid ) {
      if ( CurrentPage.Query_Offset() == Pages[Page].Offset ) {
         #ifdef DEBUG_FORMATPAGE
            Log ( "Documento::FormatPage(%i) Will use current page for formatting.", Page ) ;
         #endif
         UseCurrentPage = TRUE ;
      } else {
         #ifdef DEBUG_FORMATPAGE
            Log ( "Documento::FormatPage(%i) Will use working page for formatting because page offset is not current page start.", Page ) ;
         #endif
      } /* endif */
   } else {
      #ifdef DEBUG_FORMATPAGE
         Log ( "Documento::FormatPage(%i) Will use working page for formatting because current page is not valid yet.", Page ) ;
      #endif
   } /* endif */

   // Format the page, getting the new length and final state.
   Estado PageState = Pages[Page].State ;
   if ( UseCurrentPage ) {
      Pages[Page].Length = FormatPage ( Page, PageState, CurrentPage ) ;
   } else {
      WorkingPage.Reset ( PageState ) ;
      Pages[Page].Length = FormatPage ( Page, PageState, WorkingPage ) ;
   } /* endif */

   // If the number of pages has gone up, update it.
   if ( NumberOfPages < Page+1 ) {
      #ifdef DEBUG_FORMATPAGE
         Log ( "Documento::FormatPage(%i) Updating page count to %i because of successful page format completion.", Page, Page+1 ) ;
      #endif
      NumberOfPages = short ( Page + 1 ) ;
      if ( TitleWindow ) UpdateTitle ( Sys_ThreadNumber(*_threadid) == 1 ) ;
   } /* endif */

   // If this page's length has changed, reformat the next page.
   if ( Pages[Page].Offset+Pages[Page].Length != Pages[Page+1].Offset ) {

      // Store the new starting point of the next page.
      Pages[Page+1].Offset = Pages[Page].Offset + Pages[Page].Length ;

      // If the next page has data to format, mark it for formatting.
      if ( Pages[Page+1].Offset < pText->QueryCurrentSize() )
         Pages[Page+1].NeedsFormat = TRUE ;

      // Else, we've reached the end of document.  Reset the page count if it has changed.
      else if ( NumberOfPages != Page+1 ) {
         #ifdef DEBUG_FORMATPAGE
            Log ( "Documento::FormatPage(%i) Updating page count to %i because new EOF found.", Page, Page+1 ) ;
         #endif
         NumberOfPages = short ( Page + 1 ) ;
         if ( TitleWindow ) UpdateTitle ( Sys_ThreadNumber(*_threadid) == 1 ) ;

      } /* endif */
   } /* endif */

   // If not at end of document, and state has changed, reformat next page.
   if ( Pages[Page+1].Offset < pText->QueryCurrentSize() ) {
      if ( memcmp ( &PageState, &Pages[Page+1].State, sizeof(PageState) ) ) {
         Pages[Page+1].NeedsFormat = TRUE ;

      } /* endif */

   } /* endif */

   // Set the starting state of the next page.
   Pages[Page+1].State = PageState ;

   #ifdef DEBUG_FORMATPAGE
      Log ( "Documento::FormatPage(%i) Done.  Used %s page.", Page, UseCurrentPage ? "current" : "working" ) ;
   #endif

   return ( UseCurrentPage ) ;
}
 
/****************************************************************************
 *                                                                          *
 *  Document: Page Format                                                   *
 *                                                                          *
 *  Given page number, initial state & offset, returns formatted page and   *
 *    that page's internal size in bytes.                                   *
 *                                                                          *
 *  (Used by Supervisor Thread)                                             *
 *                                                                          *
 ****************************************************************************/

long Documento::FormatPage ( int PageNumber, Estado &PageState, Borradora &Page ) {

   #ifdef DEBUG_FORMATPAGE
      Log ( "Documento::FormatPage(%i,FinalState,Page=%p) Started.", PageNumber, &Page ) ;
      if ( Page.FindWord(0) ) 
         Log ( "Documento::FormatPage() First word offset %i.", (Page.FindWord(0))->Offset ) ;
   #endif

   // Set state to page's initial state.
   PageState = Pages[PageNumber].State ;

   // Set offset to page's offset.
   long PageOffset = Pages[PageNumber].Offset ;

   // Get file date.
   time_t Date = IsChanged() ? time(0) : FileDate ;

   // Update or reset the page word list.
   Page.UpdateWordList ( PS, int(NumberOfPages), Date, CurrentPageValid, PageOffset, PageNumber, PageState, Query_CurrentLevel()?time(0):FileDate ) ;

   // Save the page parameters.
   Page.Set_State ( PageState ) ;
   Page.Set_Offset ( PageOffset ) ;
   Page.Set_PageNumber ( short(PageNumber) ) ;

   // Compute the initial state for the page.
   Estado EffectiveState ;
   ComputeEffectiveState ( PS, pText, PS->Query_DBCS_Vector(), Page.Query_Offset(), *Page.Query_State(), EffectiveState, PageNumber, NumberOfPages, Query_CurrentLevel()?time(0):FileDate ) ;

   // Declare and reset the page height.
   long Height = 0 ;

   // Format repeatedly, until the page is full or otherwise done.
   Page.Set_RetainedLength ( 0 ) ;
   Page.Set_CurrentLength ( Page.Query_RetainedLength() ) ;
   BOOL Successful, Done ;
   do {
      #ifdef DEBUG_FORMATPAGE
         Log ( "Documento::FormatPage(%i,FinalState,Page=%p) Attempting to format page with current length %i.", PageNumber, &Page, Page.Query_CurrentLength() ) ;
      #endif // DEBUG_FORMATPAGE
      Successful = FormatPage ( Page, EffectiveState, 0, Height, Done ) ;
      if ( Successful ) {
         #ifdef DEBUG_FORMATPAGE
            Log ( "Documento::FormatPage(%i,FinalState,Page=%p) Format successful.  Setting retained length to %i.", PageNumber, &Page, Page.Query_CurrentLength() ) ;
         #endif // DEBUG_FORMATPAGE
         Page.Set_RetainedLength ( Page.Query_CurrentLength() ) ;
      } /* endif */
   } while ( NOT Successful AND NOT Done ); /* enddo */

   // If the last format was not successful, then reformat the page to the retained length.
   if ( NOT Successful ) {
      #ifdef DEBUG_FORMATPAGE
         Log ( "Documento::FormatPage(%i,FinalState,Page=%p) Reformatting page to the retained length.", PageNumber, &Page ) ;
      #endif // DEBUG_FORMATPAGE
      Page.Set_CurrentLength ( Page.Query_RetainedLength() ) ;
      FormatPage ( Page, EffectiveState, int(Page.Query_RetainedLength()), Height, Done ) ;
   } /* endif */
   
   #ifdef DEBUG_FORMATPAGE
      Log ( "Documento::FormatPage(%i,FinalState,Page=%p) Page format complete.", PageNumber, &Page ) ;
   #endif // DEBUG_FORMATPAGE

   // If no active graphics, and centering page, center the page now.
   if ( ( Page.Query_NumberOfGraphics(Page.Query_Offset()+Page.Query_CurrentLength()) == 0 ) AND EffectiveState.CenterPage ) 
      CenterPage ( Page, EffectiveState, Height ) ;

   // Compute the number of bytes used.
   int WordCount = 0 ;
   for ( int i=0; i<Page.Query_NumberOfLines(); i++ ) 
      WordCount += Page.FindLine(i)->WordCount ;

   // Determine the page length in bytes.
   #ifdef DEBUG_FORMATPAGE
      Log ( "Documento::FormatPage(%i,FinalState,Page=%p) Number of lines %i, number of words %i.",
         PageNumber, &Page, Page.Query_NumberOfLines(), WordCount ) ;
   #endif
   Palabra *pWord = Page.FindWord ( WordCount - 1 ) ;
   Page.Set_Length ( pWord->Offset + pWord->Length - Page.Query_Offset() ) ;

   // Update the working state.
   ComputeNextState ( PS, pText, Page.Query_Offset(), Page.Query_Length(), *Page.Query_State(), PageState, PageNumber, NumberOfPages, Query_CurrentLevel()?time(0):FileDate ) ;

   // Return the number of bytes used.
   #ifdef DEBUG_FORMATPAGE
      Log ( "Documento::FormatPage(%i,FinalState,Page=%p) Done.  Length %i.", PageNumber, &Page, Page.Query_Length() ) ;
   #endif
   return ( Page.Query_Length() ) ;
}

/****************************************************************************
 *                                                                          *
 *  Document: Page Format                                                   *
 *                                                                          *
 *  Given page and effective state, and the current working length,         *
 *    try to format the page.  Return TRUE for successful format, FALSE     *
 *    otherwise.  Return Done TRUE if page is complete, FALSE if it may be  *
 *    possible to extend the page yet.                                      *
 *                                                                          *
 *  (Used by Supervisor Thread)                                             *
 *                                                                          *
 ****************************************************************************/

BOOL Documento::FormatPage ( Borradora &Page, Estado &EffectiveState, int MaxLength, long &Height, BOOL &Done ) {

   #ifdef DEBUG_FORMATPAGE
      Log ( "Documento::FormatPage(Page=%p,EffectiveState,Done) Resetting page.", &Page ) ;
   #endif // DEBUG_FORMATPAGE

   // Reset the page height in world units and in lines.
   Height = 0 ;
   Page.ClearLines ( ) ;

   // Build lines until page full or done, or until an unexpected graphic shows up.
   while ( 1 ) {

      #ifdef DEBUG_FORMATPAGE
         Log ( "Documento::FormatPage(Page=%p,EffectiveState,Done) Building line %i.", &Page, Page.Query_NumberOfLines()+1 ) ;
      #endif // DEBUG_FORMATPAGE

      // Set working line top.
      long LineTop = EffectiveState.Margins.yTop - EffectiveState.HeaderHeight - Height ;
      Linea *pPrevLine = Page.Query_NumberOfLines() ? Page.FindLine ( Page.Query_NumberOfLines() - 1 ) : 0 ;
      Linea WorkingLine ;

      // Try to build a line.  Keep trying until successful or the page is declared full or done.
      do {

         #ifdef DEBUG_FORMATPAGE
            Log ( "Documento::FormatPage(Page=%p,EffectiveState,Done) Trying to build line %i with top at %i.", &Page, Page.Query_NumberOfLines()+1, LineTop ) ;
         #endif // DEBUG_FORMATPAGE

         // Reset the line.
         WorkingLine.Reset ( PS, Page, *Page.Query_State(), pPrevLine, LineTop, PageNumber, NumberOfPages, Query_CurrentLevel()?time(0):FileDate ) ;

         // Format as many words as possible into this line.
         // If we have a conflicting graphic, reset for another try.
         if ( FormatLine ( Page, WorkingLine, MaxLength ) == FALSE ) {
            #ifdef DEBUG_FORMATPAGE
               Log ( "Documento::FormatPage(Page=%p,EffectiveState,Done) Had to restart page formatting because of unexpected graphic.", &Page ) ;
            #endif // DEBUG_FORMATPAGE
            Palabra *pFirstWord = Page.FindWord ( WorkingLine.WordNumber ) ;
            Palabra *pLastWord = Page.FindWord ( pFirstWord, WorkingLine.WordCount-1 ) ;
            Page.Set_RetainedLength ( pLastWord->Offset - Page.Query_Offset() ) ;
            Page.Set_CurrentLength ( pLastWord->Offset + pLastWord->Length - Page.Query_Offset() ) ;
            #ifdef DEBUG_FORMATPAGE
               Log ( "Documento::FormatPage(Page=%p,EffectiveState,Done) Retained length %i, current length %i.", &Page, Page.Query_RetainedLength(), Page.Query_CurrentLength() ) ;
            #endif // DEBUG_FORMATPAGE
            Done = FALSE ;
            return ( FALSE ) ;
         } /* endif */
  
         // If the line had any words at all in it . . .
         if ( WorkingLine.WordCount ) {
            // Use the format generated.
            #ifdef DEBUG_FORMATPAGE
               Log ( "Documento::FormatPage(Page=%p,EffectiveState,Done) Built line with %i words.", &Page, WorkingLine.WordCount ) ;
            #endif // DEBUG_FORMATPAGE

         // Else if none could fit . . .
         } else {

            #ifdef DEBUG_FORMATPAGE
               Log ( "Documento::FormatPage(Page=%p,EffectiveState,Done) Line could not be built.", &Page ) ;
            #endif // DEBUG_FORMATPAGE

            // If the page has graphics . . .
            if ( Page.Query_NumberOfGraphics ( Page.Query_Offset()+Page.Query_CurrentLength() ) ) {
  
               #ifdef DEBUG_FORMATPAGE
                  Log ( "Documento::FormatPage(Page=%p,EffectiveState,Done) Advancing to bottom of current graphic.", &Page ) ;
               #endif // DEBUG_FORMATPAGE

               // Move top down below first graphic to terminate.
               long NextEnd = 0 ;
               Grafico *pGraphic = Page.FindGraphic ( (int)0 ) ;
               while ( pGraphic && ( pGraphic->Offset < Page.Query_Offset()+Page.Query_CurrentLength() ) ) {
                  if ( pGraphic->Position.y < LineTop ) {
                     NextEnd = max ( NextEnd, pGraphic->Position.y - pGraphic->BorderWidth ) ;
                   } /* endif */
                  pGraphic = pGraphic->NextGraphic ( ) ;
               } /* endwhile */
               LineTop = NextEnd - 1 ;

               // If top below bottom of page, end page.
               if ( LineTop < EffectiveState.Margins.yBottom ) {
                  #ifdef DEBUG_FORMATPAGE
                     Log ( "Documento::FormatPage(Page=%p,EffectiveState,Done) Page is full.", &Page ) ;
                  #endif // DEBUG_FORMATPAGE
                  Done = TRUE ;
                  return ( Page.Query_NumberOfLines() != 0 ) ;
               } /* endif */

            // Else if no graphics . . .
            } else {

               // If page already has no lines on it, force a line with one word.
               //   If we don't do this, the program will loop and die.
               if ( ( Page.Query_NumberOfLines() == 0 ) OR ( LineTop - WorkingLine.Height >= EffectiveState.Margins.yBottom ) ) {
                  #ifdef DEBUG_FORMATPAGE
                     Log ( "Documento::FormatPage(Page=%p,EffectiveState,Done) Forcing a single line.", &Page ) ;
                  #endif // DEBUG_FORMATPAGE
                  FormatLine ( Page, WorkingLine, MaxLength, TRUE ) ;

               } else {
                  #ifdef DEBUG_FORMATPAGE
                     Log ( "Documento::FormatPage(Page=%p,EffectiveState,Done) Declaring page done.", &Page ) ;
                  #endif // DEBUG_FORMATPAGE
                  Done = TRUE ;
                  return ( Page.Query_NumberOfLines() != 0 ) ;

               } /* endif forced line */

            } /* endif unforced format failed */

         } /* endif unforced line format */

      } while ( WorkingLine.WordCount == 0 ) ; /* endwhile trying to build a line */

      // If line too tall for rest of page, break out of this loop.
      if ( LineTop - WorkingLine.Height < EffectiveState.Margins.yBottom + EffectiveState.FooterHeight ) {
         #ifdef DEBUG_FORMATPAGE
            Log ( "Documento::FormatPage(Page=%p,EffectiveState,Done) Done building page.  Line too large to fit on rest of page.", &Page ) ;
         #endif // DEBUG_FORMATPAGE
         Done = TRUE ;
         return ( Page.Query_NumberOfLines() != 0 ) ;
      } /* endif */

      // Update page height.
      Height = EffectiveState.Margins.yTop - EffectiveState.HeaderHeight - ( LineTop - WorkingLine.Height ) ;

      // Adjust line spacing according to the initial state of the next line.
      Palabra *pFirstWord = Page.FindWord ( WorkingLine.WordNumber ) ;
      long OldOffset = pFirstWord->Offset ;
      Palabra *pLastWord = Page.FindWord ( pFirstWord, WorkingLine.WordCount - 1 ) ;
      long NewOffset = pLastWord->Offset + pLastWord->Length ;
      Estado NextLineState ;
      ComputeNextState ( PS, pText, OldOffset, NewOffset-OldOffset, WorkingLine.State, NextLineState, PageNumber, NumberOfPages, Query_CurrentLevel()?time(0):FileDate ) ;
      Estado NextEffectiveState ;
      ComputeEffectiveState ( PS, pText, PS->Query_DBCS_Vector(), NewOffset, NextLineState, NextEffectiveState, PageNumber, NumberOfPages, Query_CurrentLevel()?time(0):FileDate ) ;
      PS->SetState ( NextEffectiveState ) ;
      int NewHeight = PS->QueryAscent() + PS->QueryDescent() ;
      int AdjustFactor = int ( NextLineState.LineSpacing - 100 ) ;
      int Adjustment = ( NewHeight * AdjustFactor ) / 100 ;
      Height += Adjustment ;

      // Increment the line count and move to the next line.
      Linea *pLine = Page.AddLine ( WorkingLine ) ;

      // Update the current page length if larger than before.
      long Length = NewOffset - Page.Query_Offset() ;
      if ( Length > Page.Query_CurrentLength() ) {
         Page.Set_RetainedLength ( NewOffset - Page.Query_Offset() ) ;
         Page.Set_CurrentLength ( Page.Query_RetainedLength() ) ;
      } /* endif */

      #ifdef DEBUG_FORMATPAGE
         Log ( "Documento::FormatPage(Page=%p,EffectiveState,Done) Page current & retained length now %i.", &Page, Page.Query_RetainedLength() ) ;
      #endif // DEBUG_FORMATPAGE

      // See if the page ended in a end-of-page token.  If so, we will be done.
      Palabra *pWord = Page.FindWord ( pLine->WordNumber + pLine->WordCount - 1 ) ;
      PUCHAR pByte ;
      pText->QueryBytePtr ( pWord->Offset, pByte ) ;
      if ( IsEndOfPage ( *pByte ) ) {
         #ifdef DEBUG_FORMATPAGE
            Log ( "Documento::FormatPage(Page=%p,EffectiveState,Done) Line has end-of-page.  This will be the last line on the page.", &Page ) ;
         #endif // DEBUG_FORMATPAGE
         Done = TRUE ;
         return ( TRUE ) ;
      } /* endif */

   } /* endwhile trying to build a page */

} /* endmethod */

 
/****************************************************************************
 *                                                                          *
 *  Document: Center Page                                                   *
 *                                                                          *
 *  (Used by Supervisor Thread)                                             *
 *                                                                          *
 ****************************************************************************/

void Documento::CenterPage ( Borradora &Page, Estado &EffectiveState, long Height ) {

   long MaxHeight = EffectiveState.Margins.yTop - EffectiveState.Margins.yBottom ;
   long Baseline = EffectiveState.Margins.yTop - ( MaxHeight - Height ) / 2 ;

   Linea *pLine = Page.FindLine((int)0) ;
   for ( int i=0; i<Page.Query_NumberOfLines(); i++, pLine=pLine->NextLine() ) {

      pLine->Top = Baseline ;
      Baseline -= pLine->MaxAscent ;
      Palabra *pWord = Page.FindWord ( pLine->WordNumber ) ;
      for ( int j=0; j<pLine->WordCount; j++, pWord=pWord->NextWord() ) {
         if ( pWord->Position.y != Baseline ) {
            pWord->Position.y = Baseline ;
            pWord->NeedsPainting = TRUE ;
            #ifdef DEBUG_NEEDSPAINTING
               Log ( "Documento::CenterPage: word %i (offset %i) needs painting because it's y-position has changed.", pLine->WordNumber+j, pWord->Offset ) ;
            #endif
         } /* endif */
      } /* endfor */
      Baseline -= pLine->MaxDescent ;

      if ( i >= Page.Query_NumberOfLines()-1 )
         break ;

      Estado NextEffectiveState ;
      Linea *pNextLine = pLine->NextLine() ;
      ComputeEffectiveState ( PS, pText, PS->Query_DBCS_Vector(), Page.FindWord(pNextLine->WordNumber)->Offset, pNextLine->State, NextEffectiveState, PageNumber, NumberOfPages, Query_CurrentLevel()?time(0):FileDate ) ;
      PS->SetState ( NextEffectiveState ) ;
      int NewHeight = PS->QueryAscent() + PS->QueryDescent() ;
      int AdjustFactor = int ( pNextLine->State.LineSpacing - 100 ) ;
      int Adjustment = ( NewHeight * AdjustFactor ) / 100 ;
      Baseline -= Adjustment ;

   } /* endfor */
}
 
/****************************************************************************
 *                                                                          *
 *  Document: Line Format                                                   *
 *                                                                          *
 *  Given current page state, build a line for the page.                    *
 *                                                                          *
 *  Returns TRUE if there was no problem formatting the line, and the       *
 *    number of words placed in the line is saved in the line object.       *
 *    It is possible that no words fit in the line.                         *
 *  Returns FALSE if a graphic was encountered which would invalidate       *
 *    prior formatting.                                                     *
 *                                                                          *
 *  (Used by Supervisor Thread)                                             *
 *                                                                          *
 ****************************************************************************/

BOOL Documento::FormatLine ( Borradora &Page, Linea &Line, int /* MaxLength */, BOOL Force ) {

   // Get file date.
   time_t Date = IsChanged() ? time(0) : FileDate ;

   // Build the first word of the line, if it hasn't been built already.
   Page.BuildWord ( NumberOfPages, Date, Line.WordNumber, PS ) ;
   Palabra *pFirstWord = Page.FindWord ( Line.WordNumber ) ;

   // Determine the line's initial state.
   Estado EffectiveState ;
   ComputeEffectiveState ( PS, pText, PS->Query_DBCS_Vector(), pFirstWord->Offset, Line.State, EffectiveState, PageNumber, NumberOfPages, Query_CurrentLevel()?time(0):FileDate ) ;

   // Determine whether or not this line is a separator bar.
   PUCHAR pByte ;
   Page.Query_Text()->QueryBytePtr ( pFirstWord->Offset, pByte ) ;
   BOOL Separator = IsSeparator ( pByte ) ;

   // Determine whether or not this line starts a paragraph.
   BOOL Paragraph = pFirstWord->StartsParagraph ( pText, PS ) ;

   // Estimate the line's max ascent and descent.
   int MaxAscent(0), MaxDescent(0) ;
   Line.EstimateHeight ( PS, pText, Page, pFirstWord, EffectiveState, Paragraph, NumberOfPages, Date, Force, MaxAscent, MaxDescent ) ;

   // Until the estimated ascent/descent matches what gets formatted . . .
   //  (Just don't try too many times!)
   int WordsFit ( 0 ) ;
   for ( int Attempts=0; Attempts<10; Attempts++ ) {

      // Save the estimated ascent/descent.
      long EstimatedAscent = MaxAscent ;
      long EstimatedDescent = MaxDescent ;

      // Construct the line rectangle.
      RECTL Rectangle ;
      Rectangle.yTop = Line.Top ;
      Rectangle.yBottom = Line.Top - EstimatedAscent - EstimatedDescent ;
      Rectangle.xLeft = max ( 0, 
         EffectiveState.Margins.xLeft + 
         ( Separator ? 
            0 :
            ( Paragraph ? 
               ( EffectiveState.Indent>0 ? EffectiveState.Indent : 0 ) : 
               ( EffectiveState.Indent>=0 ? 0 : -EffectiveState.Indent ) 
            )
         ) 
      ) ;
      Rectangle.xRight = EffectiveState.Margins.xRight ;
      Segment SegmentList ( Rectangle ) ;

      // Break up the line into segments as needed.
      Grafico *pGraphic = Page.FindGraphic ( 0 ) ;
      while ( pGraphic ) {
         if ( NOT pGraphic->Background AND ( pGraphic->Offset < Page.Query_Offset() + Page.Query_CurrentLength() ) ) {
            RECTL Rectangle ;
            pGraphic->ComputeBox ( Page.FindWord(pGraphic->Offset), Rectangle, PrinterResolution, TRUE ) ;
            if ( pGraphic->NoTextAside ) {
               Rectangle.xLeft = EffectiveState.Margins.xLeft ;
               Rectangle.xRight = EffectiveState.Margins.xRight ;
            } /* endif */
            SegmentList.Split ( Rectangle ) ;
         } /* endif */
         pGraphic = pGraphic->NextGraphic ( ) ;
      } /* endfor */

      // Distribute the words amongst the segments.
      Segment *pSegment = &SegmentList ;
      WordsFit = 0 ;
      Estado WorkingState = EffectiveState ;
      Palabra *pWord = pFirstWord ;
      while ( pWord && pSegment ) {
         // Try loading the segment with words.  
         int WordsInSegment = pSegment->Load ( PS, pText, Page, Line, pWord, EffectiveState, WorkingState, PrinterResolution, Paragraph, NumberOfPages, Query_CurrentLevel()?time(0):FileDate ) ;
         // If an unexpected graphic was found, terminate the line and return an error.
         if ( WordsInSegment < 0 ) {
            Line.WordCount = WordsFit - WordsInSegment ;
            return ( FALSE ) ;
         } /* endif */
         // Update the count of words that fit.
         WordsFit += WordsInSegment ;
         // If there are more segments, move on to the next one.
         if ( pSegment->QueryNext() ) {
            PUCHAR pByte ;
            pText->QueryBytePtr ( pWord->Offset, pByte ) ;
            if ( IsEndOfLine(pByte) )
               break ;
         // Else, if not, and forcing, then accept the current word.
         } else if ( !WordsInSegment && Force ) {
            pSegment->SetFirst ( pWord, Line.WordNumber + WordsFit ) ;
            pSegment->SetLast ( pWord ) ;
            WordsFit ++ ;
            pWord = pWord->NextWord ( ) ;
         } /* endif */
         pSegment = pSegment->QueryNext() ;
      } /* endwhile */

      // If things haven't stabilized, loop again.              ELABORATE: We are not currently recomputing the ascent/descent.
      if ( MaxAscent != EstimatedAscent )
         continue ;
      if ( MaxDescent != EstimatedDescent )
         continue ;

      // Position the words vertically.
      pWord = pFirstWord ;
      for ( int i=0; i<WordsFit; i++, pWord=pWord->NextWord() ) {
         if ( pWord->Position.y != Line.Top - MaxAscent ) {
            pWord->Position.y = Line.Top - MaxAscent ;
            pWord->NeedsPainting = TRUE ;
         } /* endif */
      } /* endif */

      // Justify the segments horizontally.
      WorkingState = EffectiveState ;
      pSegment = &SegmentList ;
      while ( pSegment ) {
         pSegment->Justify ( &Page, PS, WorkingState, PageNumber, NumberOfPages, Query_CurrentLevel()?time(0):FileDate ) ;
         pSegment = pSegment->QueryNext() ;
      } /* endwhile */

      // Done.
      break ;

   } /* endwhile ascent/descent not yet stabilized */

   // Save the information we've found.
   Line.MaxAscent = MaxAscent ;
   Line.MaxDescent = MaxDescent ;
   Line.Height = MaxAscent + MaxDescent ;
   Line.WordCount = WordsFit ;

   // Compute the line size.
   Line.Offset = pFirstWord->Offset ;
   if ( Line.WordCount ) {
      Palabra *pLastWord = Page.FindWord ( pFirstWord, Line.WordCount - 1 ) ;
      Line.Length = pLastWord->Offset + pLastWord->Length - Line.Offset ;
   } else {
      Line.Length = 0 ;
   } /* endif */

   // Done.
   return ( TRUE ) ;
}

 
/****************************************************************************
 *                                                                          *
 *  Document: Update Window                                                 *
 *                                                                          *
 *  (Used by Supervisor Thread)                                             *
 *                                                                          *
 ****************************************************************************/

BOOL Documento::UpdateWindow ( int Page, BOOL SetColumn, BOOL UsedCurrentPage ) {

   #ifdef DEBUG_SUPERVISOR
      Log ( "Documento::UpdateWindow(Page=%i,SetColumn=%s,UsedCurrentPage=%s) Offset %i.",
         Page, SetColumn?"TRUE":"FALSE", UsedCurrentPage?"TRUE":"FALSE", Offset ) ;
   #endif

   // If this is the current page, update the current page copy.
   if ( ( Offset >= Pages[Page].Offset ) AND ( Offset < Pages[Page].Offset+Pages[Page].Length ) ) {

      // Save new current page data and mark it valid.  Save page number.
      if ( NOT UsedCurrentPage ) {
         #ifdef DEBUG_SUPERVISOR
            Log ( "Documento::UpdateWindow() Copying working page to current page." ) ;
         #endif
         CurrentPage = WorkingPage ;
      } /* endif */
      PageNumber = Page ;

      // Update the window now, now that the page image is updated.
      if ( Window ) {
         #ifdef DEBUG_SUPERVISOR
            Log ( "Documento::UpdateWindow() Asking painter to update the window." ) ;
         #endif
         PainterObject->UpdateWindow ( !CurrentPageValid, FALSE ) ;
      } /* endif */
      CurrentPageValid = TRUE ;

      // Compute the new state.
      #ifdef DEBUG_SUPERVISOR
         Log ( "Documento::UpdateWindow() Computing current state." ) ;
      #endif
      ComputeState ( ) ;

      // If window defined, update it and its controls.
      if ( Window ) {
         #ifdef DEBUG_SUPERVISOR
            Log ( "Documento::UpdateWindow() Updating scroll bars." ) ;
         #endif
         UpdateScrollBars ( ) ;
         #ifdef DEBUG_SUPERVISOR
            Log ( "Documento::UpdateWindow() Updating position." ) ;
         #endif
         UpdatePosition ( ) ;
         #ifdef DEBUG_SUPERVISOR
            Log ( "Documento::UpdateWindow() Updating title." ) ;
         #endif
         UpdateTitle ( ) ;
         #ifdef DEBUG_SUPERVISOR
            Log ( "Documento::UpdateWindow() Updating rulers." ) ;
         #endif
         UpdateRulers ( ) ;
         #ifdef DEBUG_SUPERVISOR
            Log ( "Documento::UpdateWindow() Updating status." ) ;
         #endif
         UpdateStatus ( ) ;
         #ifdef DEBUG_SUPERVISOR
            Log ( "Documento::UpdateWindow() Updating cursor." ) ;
         #endif
         UpdateCursor ( ) ;
         #ifdef DEBUG_SUPERVISOR
            Log ( "Documento::UpdateWindow() Updating mouse." ) ;
         #endif
         UpdateMouse ( ) ;
      } /* endif */

      if ( SetColumn )
         Column = Position.x ;

      #ifdef DEBUG_SUPERVISOR
         Log ( "Documento::UpdateWindow() Done.  Current page updated." ) ;
      #endif
      return ( TRUE ) ;

   } else {
      #ifdef DEBUG_SUPERVISOR
         Log ( "Documento::UpdateWindow() Done.  Not current page (start %i, end %i).",
            Pages[Page].Offset, Pages[Page].Offset+Pages[Page].Length-1 ) ;
      #endif
      return ( FALSE ) ;

   } /* endif */
}

 
/****************************************************************************
 *                                                                          *
 *  Supervisor: Constructor                                                 *
 *                                                                          *
 ****************************************************************************/

Supervisor::Supervisor ( Documento *pdocument ) :
   Active(TRUE), 
   BlockDone("Supervisor::BlockDone"),
   BlockPermit("Supervisor::BlockPermit"), 
   BlockRequest(FALSE), 
   Commands("Supervisor::Commands",sizeof(CommandQueueEntry)),
   Done("Supervisor::Done"),
   EditDone("Supervisor::EditDone"),
   EditPermit("Supervisor::EditPermit"), 
   EditRequest(FALSE), 
   FlushRequest(FALSE),
   PageLocked(FALSE), 
   pDocument(pdocument), 
   Request("Supervisor::Request"), 
   Thread(-1),
   ThreadDone("Supervisor::ThreadDone") {

   #ifdef DEBUG_SUPERVISOR
      Log ( "Supervisor(%08X)::Supervisor(pDocument=%08X) Started.", this, pDocument ) ;
   #endif
   #if defined(DEBUG_BLOCK) || defined(DEBUG_LOCK)
      BlockDone.SetDebug(TRUE) ;
      BlockPermit.SetDebug(TRUE) ;
      EditDone.SetDebug(TRUE) ;
      EditPermit.SetDebug(TRUE) ;
   #endif // DEBUG_BLOCK or DEBUG_LOCK

   Request.Reset() ;
   Done.Post() ;

   EditPermit.Reset() ;
   EditDone.Post() ;

   Thread = StartThread ( "SupervisorThread", Supervisor_Thread, 0x10000, this, PRTYC_FOREGROUNDSERVER, +20 ) ;

   #ifdef DEBUG_SUPERVISOR
      Log ( "Supervisor(%08X)::Supervisor Done.", this ) ;
   #endif
}
 
/****************************************************************************
 *                                                                          *
 *  Supervisor: Destructor                                                  *
 *                                                                          *
 ****************************************************************************/

Supervisor::~Supervisor ( ) {

   #ifdef DEBUG_SUPERVISOR
      Log ( "Supervisor::~Supervisor Started." ) ;
   #endif

   Active = FALSE ;

   if ( long(Thread) >= 0 ) {
      DosSetPriority ( PRTYS_THREAD, PRTYC_NOCHANGE, PRTYD_MAXIMUM, Thread ) ;
      Request.Post() ;
      ThreadDone.Wait() ;
   } /* endif */

   #ifdef DEBUG_SUPERVISOR
      Log ( "Supervisor::~Supervisor Done." ) ;
   #endif
}
 
/****************************************************************************
 *                                                                          *
 *  Supervisor: Enqueue Commands                                            *
 *                                                                          *
 *  (Called by Foreground Thread)                                           *
 *                                                                          *
 ****************************************************************************/

BOOL Supervisor::CommandsQueued ( ) {
   return ( NOT Commands.Empty ( ) ) ;
}

void Supervisor::EnqueueInsertToggle ( ) {
   CommandQueueEntry Entry = { DOCCMD_INSERT_TOGGLE, 0, 0 } ;
   Commands.Enqueue ( &Entry ) ;
   Request.Post() ;
}

void Supervisor::EnqueueKey ( int KeyCode ) {
   CommandQueueEntry Entry = { DOCCMD_INSERT_CHAR, (void*)KeyCode, 0 } ;
   Commands.Enqueue ( &Entry ) ;
   Request.Post() ;
}

void Supervisor::EnqueueString ( char *String, int Length ) {
   char *Buffer = new char [ Length ] ;
   memcpy ( Buffer, String, Length ) ;
   CommandQueueEntry Entry = { DOCCMD_INSERT_STRING, Buffer, Length } ;
   Commands.Enqueue ( &Entry ) ;
   Request.Post() ;
}

void Supervisor::EnqueueClear ( ) {
   CommandQueueEntry Entry = { DOCCMD_CLEAR, 0, 0 } ;
   Commands.Enqueue ( &Entry ) ;
   Request.Post() ;
}

void Supervisor::EnqueuePaste ( ) {
   CommandQueueEntry Entry = { DOCCMD_PASTE, 0, 0 } ;
   Commands.Enqueue ( &Entry ) ;
   Request.Post() ;
}

void Supervisor::EnqueueUndo ( ) {
   CommandQueueEntry Entry = { DOCCMD_UNDO, 0, 0 } ;
   Commands.Enqueue ( &Entry ) ;
   Request.Post() ;
}

void Supervisor::EnqueueRedo ( ) {
   CommandQueueEntry Entry = { DOCCMD_REDO, 0, 0 } ;
   Commands.Enqueue ( &Entry ) ;
   Request.Post() ;
}

 
/****************************************************************************
 *                                                                          *
 *  Supervisor: Flush Command Queue                                         *
 *                                                                          *
 *  (Called by Foreground Thread)                                           *
 *                                                                          *
 ****************************************************************************/

void Supervisor::FlushCommandQueue ( ) {

   if ( CommandsQueued ( ) ) {

      #ifdef DEBUG_FOREGROUND
         Log ( "Supervisor::FlushCommandQueue: Requesting command queue be flushed." ) ;
      #endif

      EditPermit.Reset() ;                 // Reset permission event.

      FlushRequest = TRUE ;                // Set flush request flag.

      Request.Post() ;                     // Tell the supervisor.
      if ( !EditPermit.Wait(60000) )       // Wait for permission.
         Log ( "FlushCommandQueue: Timeout waiting for edit permission." ) ;

      EditDone.Post() ;                    // Acknowledge completion of dummy edit.

      #ifdef DEBUG_FOREGROUND
         Log ( "Supervisor::FlushCommandQueue: Done." ) ;
      #endif

   } /* endif */
}
 
/****************************************************************************
 *                                                                          *
 *  Supervisor: Request Permission to Commence Edit                         *
 *                                                                          *
 *  (Called by Foreground Thread)                                           *
 *                                                                          *
 ****************************************************************************/

void Supervisor::CommenceEdit ( ) {
   #ifdef DEBUG_FOREGROUND
      Log ( "Supervisor::CommenceEdit: Started." ) ;
   #endif
   EditPermit.Reset() ;                 // Reset permission event.
   EditRequest = TRUE ;                 // Set edit request flag.
   Request.Post() ;                     // Wake up the supervisor.
   if ( !EditPermit.Wait(60000) )       // Wait for permission.
      Log ( "CommenceEdit: Timeout waiting for edit permission." ) ;
   #ifdef DEBUG_FOREGROUND
      Log ( "Supervisor::CommenceEdit: Done." ) ;
   #endif
}
 
/****************************************************************************
 *                                                                          *
 *  Supervisor: Notify of Edit Completion                                   *
 *                                                                          *
 *  (Called by Foreground Thread)                                           *
 *                                                                          *
 ****************************************************************************/

void Supervisor::CompleteEdit ( ) {
   #ifdef DEBUG_FOREGROUND
      Log ( "Supervisor::CompleteEdit: Started." ) ;
   #endif
   EditDone.Post() ;
   #ifdef DEBUG_FOREGROUND
      Log ( "Supervisor::CompleteDone: Started." ) ;
   #endif
}
 
/****************************************************************************
 *                                                                          *
 *  Supervisor: Format Page and eventually refresh the window.              *
 *                                                                          *
 *  (Called by Foreground Thread)                                           *
 *                                                                          *
 ****************************************************************************/

void Supervisor::FormatPage ( BOOL Wait ) {
   Done.Reset ( ) ;
   pDocument->SetPageDirty ( ) ;
   Request.Post ( ) ;
   if ( Wait ) {
      DosSetPriority ( PRTYS_THREAD, PRTYC_FOREGROUNDSERVER, PRTYD_MAXIMUM, Thread ) ;
      if ( !Done.Wait ( 60000 ) )
         Log ( "FormatPage: Timeout waiting for page formatting to complete." ) ;
   } /* endif */
}
 
/****************************************************************************
 *                                                                          *
 *  Supervisor: Wait for something to do                                    *
 *                                                                          *
 *  (Used by Supervisor Thread)                                             *
 *                                                                          *
 ****************************************************************************/

void Supervisor::WaitForRequest ( ) {
   if ( !CommandsQueued() && !FlushRequest && !EditRequest && !BlockRequest ) {
      Request.Wait() ;
      Request.Reset() ;
   } /* endif */
}
 
/****************************************************************************
 *                                                                          *
 *  Supervisor: Flush Commands                                              *
 *                                                                          *
 *  (Used by Supervisor Thread)                                             *
 *                                                                          *
 ****************************************************************************/

BOOL Supervisor::ExecuteCommands ( ) {

   #ifdef DEBUG_DEQUEUE
      Log ( "Supervisor::ExecuteCommands() Started." ) ;
   #endif // DEBUG_DEQUEUE

   if ( ! CommandsQueued ( ) ) {                                // Return FALSE if no commands queued.
      #ifdef DEBUG_DEQUEUE
         Log ( "Supervisor::ExecuteCommands() Done.  Nothing was queued." ) ;
      #endif // DEBUG_DEQUEUE
      return ( FALSE ) ;
   } /* endif */

   while ( CommandsQueued ( ) ) {                               // While more commands queued . . .

      #ifdef DEBUG_LOCK
         Log ( "Supervisor::ExecuteCommands: Locking current page." ) ;
      #endif
      PageLockRequest ( ) ;                                     //   Lock the current page.

      #ifdef DEBUG_DEQUEUE
         Log ( "Supervisor::ExecuteCommands() Trying to dequeue a command." ) ;
      #endif // DEBUG_DEQUEUE

      CommandQueueEntry Entry ;                                 //   Dequeue a command.
      if ( Commands.Dequeue ( &Entry, 0 ) == FALSE )
         break ;

      #ifdef DEBUG_DEQUEUE
         Log ( "Supervisor::ExecuteCommands: Executing %s request.", CommandNames[Entry.Command] ) ;
      #endif

      switch ( Entry.Command ) {                                //   According to command type . . .

         case DOCCMD_INSERT_TOGGLE: {                           //     Toggle Insert Mode.
            pDocument->Dequeue_InsertModeToggle ( ) ;
            break; }

         case DOCCMD_INSERT_CHAR: {                             //     Insert Character.
            pDocument->Dequeue_InsertKey ( *((USHORT*)&Entry.Parameter) ) ;
            break; }

         case DOCCMD_INSERT_STRING: {                           //     Insert String.
            pDocument->Dequeue_InsertText ( PCHAR(Entry.Parameter), Entry.Length ) ;
            break; }

         case DOCCMD_CLEAR: {                                   //     Clear selected text.
            pDocument->Dequeue_Clear ( ) ;
            break; }

         case DOCCMD_PASTE: {                                   //     Insert text from clipboard.
            pDocument->Dequeue_Paste ( ) ;
            break; }

         case DOCCMD_UNDO: {                                    //     Undo one change level:
            pDocument->Dequeue_Undo ( ) ;
            break; }

         case DOCCMD_REDO: {                                    //     Undo one change level:
            pDocument->Dequeue_Redo ( ) ;
            break; }

      } /* endswitch */                                         //   End switch.

   } /* endwhile */                                             // End while.

   #ifdef DEBUG_SUPERVISOR
      Log ( "Supervisor::ExecuteCommands() Done.  Some work was accomplished." ) ;
   #endif // DEBUG_SUPERVISOR

   return ( TRUE ) ;                                            // Return TRUE.
}
 
/****************************************************************************
 *                                                                          *
 *  Supervisor: Confirm Commands Flushed                                    *
 *                                                                          *
 *  (Used by Supervisor Thread)                                             *
 *                                                                          *
 ****************************************************************************/

BOOL Supervisor::FlushCommands ( ) {

   if ( !FlushRequest )
      return ( FALSE ) ;

   #ifdef DEBUG_SUPERVISOR
      Log ( "Supervisor::FlushCommands: Flushing command queue." ) ;
   #endif

   FlushRequest = FALSE ;
   EditDone.Reset() ;           // Reset notification of edit complete.
   EditPermit.Post() ;          // Grant edit permission.
   if ( !EditDone.Wait(60000) ) // Wait for edit to complete.
      Log ( "FlushCommands: Timeout waiting for dummy edit to complete." ) ;

   #ifdef DEBUG_SUPERVISOR
      Log ( "Supervisor::FlushCommands: Done." ) ;
   #endif

   return ( TRUE ) ;
}
 
/****************************************************************************
 *                                                                          *
 *  Supervisor: Allow Full Edit to commence                                 *
 *                                                                          *
 *  (Used by Supervisor Thread)                                             *
 *                                                                          *
 ****************************************************************************/

BOOL Supervisor::AllowEdit ( ) {

   if ( !EditRequest )
      return ( FALSE ) ;

   EditRequest = FALSE ;        // Reset request for edit permission.

   #ifdef DEBUG_SUPERVISOR
      Log ( "Supervisor::AllowEdit: Locking current page." ) ;
   #endif

   PageLockRequest() ;          // Lock the current page.

   #ifdef DEBUG_SUPERVISOR
      Log ( "Supervisor::AllowEdit: Granting edit permission." ) ;
   #endif

   EditDone.Reset() ;           // Reset notification of edit complete.
   EditPermit.Post() ;          // Grant edit permission.

   #ifdef DEBUG_SUPERVISOR
      Log ( "Supervisor::AllowEdit: Waiting for edit to complete." ) ;
   #endif

   if ( !EditDone.Wait(60000) ) // Wait for edit to complete.
      Log ( "AllowEdit: Timeout waiting for edit to complete." ) ;

   #ifdef DEBUG_SUPERVISOR
      Log ( "Supervisor::AllowEdit: Edit completed." ) ;
   #endif

   Request.Post() ;             // Request formatting.

   #ifdef DEBUG_SUPERVISOR
      Log ( "Supervisor::AllowEdit: Format requested." ) ;
   #endif

   return ( TRUE ) ;
}
 
/****************************************************************************
 *                                                                          *
 *  Supervisor: Format the first dirty page.                                *
 *                                                                          *
 *  (Used by Supervisor Thread)                                             *
 *                                                                          *
 ****************************************************************************/

void Supervisor::FormatFirstDirtyPage ( BOOL SetColumn ) {
   pDocument->FormatFirstDirtyPage ( Request, Done, SetColumn ) ;
}
 
/****************************************************************************
 *                                                                          *
 *  Supervisor: Lock / Unlock Current Page                                  *
 *                                                                          *
 *  (Used by Supervisor Thread)                                             *
 *                                                                          *
 ****************************************************************************/

void Supervisor::PageLockRequest ( ) {
   #ifdef DEBUG_LOCK
      Log ( "Supervisor::PageLockRequest: State %s.", PageLocked?"TRUE":"FALSE" ) ;
   #endif
   if ( NOT PageLocked ) {
      #ifdef DEBUG_LOCK
         Log ( "Supervisor::PageLockRequest: Locking current page." ) ;
      #endif
      pDocument->LockCurrentPage() ;
      #ifdef DEBUG_LOCK
         Log ( "Supervisor::PageLockRequest:   Current page locked." ) ;
      #endif
      PageLocked = TRUE ;
   } /* endif */
}

void Supervisor::PageLockRelease ( ) {
   #ifdef DEBUG_LOCK
      Log ( "Supervisor::PageLockRelease: State %s.", PageLocked?"TRUE":"FALSE" ) ;
   #endif
   if ( PageLocked ) {
      PageLocked = FALSE ;
      #ifdef DEBUG_LOCK
         Log ( "Supervisor::PageLockRelease: Unlocking current page." ) ;
      #endif
      pDocument->UnlockCurrentPage() ;
   } /* endif */
}
 
/****************************************************************************
 *                                                                          *
 *  Supervisor: Block/Unblock Formatting                                    *
 *                                                                          *
 *  (Used by Window thread)                                                 *
 *                                                                          *
 ****************************************************************************/

void Supervisor::Block ( ) {
   #ifdef DEBUG_BLOCK
      Log ( "Supervisor::Block: Started." ) ;
   #endif
   BlockPermit.Reset() ;
   BlockRequest = TRUE ;
   Request.Post() ;
   if ( !BlockPermit.Wait(60000) )
      Log ( "Block: Timeout waiting for block permission." ) ;
   #ifdef DEBUG_BLOCK
      Log ( "Supervisor::Block: Done." ) ;
   #endif
}

void Supervisor::Unblock ( ) {
   #ifdef DEBUG_BLOCK
      Log ( "Supervisor::Unblock: Started." ) ;
   #endif
   BlockDone.Post() ;
   #ifdef DEBUG_BLOCK
      Log ( "Supervisor::Unblock: Done." ) ;
   #endif
}

BOOL Supervisor::AllowBlock ( ) {

   if ( !BlockRequest )
      return ( FALSE ) ;

   #ifdef DEBUG_BLOCK
      Log ( "Supervisor::AllowBlock: Started." ) ;
   #endif

   BlockRequest = FALSE ;
   BlockDone.Reset() ;
   BlockPermit.Post() ;

   if ( !BlockDone.Wait(60000) )
      Log ( "AllowBlock: Timeout waiting for block to complete." ) ;

   #ifdef DEBUG_BLOCK
      Log ( "Supervisor::AllowBlock: Done." ) ;
   #endif

   return ( TRUE ) ;
}

 
/****************************************************************************
 ****************************************************************************
 *** Thread: Page Format Supervisor *****************************************
 ****************************************************************************
 ****************************************************************************/

extern void Supervisor_Thread ( void *Parameter ) {

  /**************************************************************************
   * Get parameters.                                                        *
   **************************************************************************/

   Supervisor *Parms = (Supervisor*) Parameter ;

  /**************************************************************************
   * Connect to PM.                                                         *
   **************************************************************************/

   #ifdef DEBUG_THREAD
      Log ( "Supervisor_Thread: About to connect to PM." ) ;
   #endif

   Process Proc ( "Supervisor_Thread", 0, LibraryHandle ) ;

  /**************************************************************************
   * The Supervisor starts out with everything locked.                      *
   **************************************************************************/

   #ifdef DEBUG_THREAD
      Log ( "Supervisor_Thread: Requesting page lock." ) ;
   #endif

   Parms->PageLockRequest ( ) ;

  /**************************************************************************
   * While yet active . . .                                                 *
   **************************************************************************/

   while ( Parms->Query_Active ( ) ) {

     /***********************************************************************
      * Wait for a request.  Reset the request at once.                     *
      ***********************************************************************/

      #ifdef DEBUG_THREAD
         Log ( "Supervisor_Thread: About to wait for request." ) ;
      #endif

      Parms->WaitForRequest ( ) ;
      if ( Parms->Query_Active ( ) ) {

        /********************************************************************
         * If there are any commands queued, process them now.              *
         ********************************************************************/

         #ifdef DEBUG_THREAD
            Log ( "Supervisor_Thread: Checking for commands to execute." ) ;
         #endif

         if ( Parms->ExecuteCommands ( ) ) {
            Parms->FormatFirstDirtyPage ( TRUE ) ;
            continue ;
         } /* endif */

        /********************************************************************
         * If somebody's requested command flush, tell them it's done.      *
         ********************************************************************/

         #ifdef DEBUG_THREAD
            Log ( "Supervisor_Thread: Checking for command flush request." ) ;
         #endif

         if ( Parms->FlushCommands ( ) ) {
            Parms->FormatFirstDirtyPage ( TRUE ) ;
            continue ;
         } /* endif */

        /********************************************************************
         * If somebody's requested full edit permission, grant it now.      *
         ********************************************************************/

         #ifdef DEBUG_THREAD
            Log ( "Supervisor_Thread: Checking for edit request." ) ;
         #endif

         if ( Parms->AllowEdit ( ) ) {
            Parms->FormatFirstDirtyPage ( TRUE ) ;
            continue ;
         } /* endif */

        /********************************************************************
         * If somebody's requested a formatting block, grant it now.        *
         ********************************************************************/

         #ifdef DEBUG_THREAD
            Log ( "Supervisor_Thread: Checking for block request." ) ;
         #endif

         if ( Parms->AllowBlock ( ) ) {
            Parms->FormatFirstDirtyPage ( TRUE ) ;
            continue ;
         } /* endif */

        /********************************************************************
         * Format the first dirty page in the document, if there is one.    *
         ********************************************************************/

         #ifdef DEBUG_THREAD
            Log ( "Supervisor_Thread: Defaulting to format first dirty page, if any." ) ;
         #endif

         Parms->FormatFirstDirtyPage ( ) ;

      } /* endif */

   } /* endwhile */

  /**************************************************************************
   * Release the locks before termination.                                  *
   **************************************************************************/

   #ifdef DEBUG_THREAD
      Log ( "Supervisor_Thread: Releasing page lock." ) ;
   #endif
   Parms->PageLockRelease ( ) ;

  /**************************************************************************
   * Signal completion.                                                     *
   **************************************************************************/

   #ifdef DEBUG_THREAD
      Log ( "Supervisor_Thread: Posting completion event." ) ;
   #endif
   Parms->Query_ThreadDone()->Post ( ) ;

   #ifdef DEBUG_THREAD
      Log ( "Supervisor_Thread: Done." ) ;
   #endif
} 
