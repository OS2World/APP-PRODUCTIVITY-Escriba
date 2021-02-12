/**************************************************************** PAINTER.CPP
 *                                                                          *
 *                     Document Window Painting Thread                      *
 *                                                                          *
 ****************************************************************************/

#include "System.h"

#include "Document.h"
#include "Escriba.h"
#include "Painter.h"
#include "Process.h"
#include "Thread.h"
#include "WrkSpace.h"

// #define DEBUG
// #define DEBUG_LOCK
// #define DEBUG_INVALIDRECTANGLE
  
static void Painter_Thread ( void *Parameter ) ;

 
/****************************************************************************
 *                                                                          *
 *  Painter: Constructor                                                    *
 *                                                                          *
 ****************************************************************************/

Painter::Painter ( HWND window, DeviceContext *pdevice, Documento *pdocument ) :
   Active(TRUE), pDevice(pdevice), pDocument(pdocument), 
   Window(window), CursorFlag(0), CursorVisible(FALSE), ThreadDone("Painter::ThreadDone"),
   UpdateLock("Painter::UpdateLock"), Request("Painter::Request"), CursorBoxLock("Painter::CursorBoxLock"), 
   InvalidRectangleLock("Painter::InvalidRectangleLock") {

   #ifdef DEBUG
      Log ( "Painter::Painter(Window=%08X,pDevice=%08X,pDocument=%08X) Started.", Window, pDevice, pDocument ) ;
   #endif // DEBUG

   #ifdef DEBUG_LOCK
      UpdateLock.SetDebug ( TRUE ) ;
   #endif // DEBUG_LOCK

   InvalidRectangle.xLeft = InvalidRectangle.yBottom = 0x7FFFFFFF ;
   InvalidRectangle.xRight = InvalidRectangle.yTop = 0 ;

   memset ( &CursorBox, 0, sizeof(CursorBox) ) ;

   Metric = pDocument->Query_State0()->Metric ;
   CodePage = pDocument->Query_Parms()->CodePage ;
   #ifdef DEBUG
      Log ( "Painter::Painter(Window=%08X,pDevice=%08X,pDocument=%08X) Creating workspace.", Window, pDevice, pDocument ) ;
   #endif
   PS = new WorkSpace ( "PainterThread", CodePage, pDevice, 0, Metric, FALSE ) ;

   Request.Reset() ;
   Thread = StartThread ( "PainterThread", Painter_Thread, 0x8000, this, PRTYC_FOREGROUNDSERVER, PRTYD_MAXIMUM ) ;

   #ifdef DEBUG
      Log ( "Painter::Painter(Window=%08X,pDevice=%08X,pDocument=%08X) Done.", Window, pDevice, pDocument ) ;
   #endif // DEBUG
}
 
/****************************************************************************
 *                                                                          *
 *  Painter: Destructor                                                     *
 *                                                                          *
 ****************************************************************************/

Painter::~Painter ( ) {

   Active = FALSE ;
   DosSetPriority ( PRTYS_THREAD, PRTYC_TIMECRITICAL, PRTYD_MAXIMUM, Thread ) ;
   Request.Post() ;
   ThreadDone.Wait() ;

   delete PS ;

   #ifdef DEBUG
      Log ( "Painter::Painter: Destroyed. Window %p, document %p.", Window, pDocument ) ;
   #endif // DEBUG
}
 
/****************************************************************************
 *                                                                          *
 *  Painter: Paint Cursor                                                   *
 *                                                                          *
 ****************************************************************************/

void Painter::PaintCursor ( ) {

   CursorVisible = CursorVisible ? FALSE : TRUE ;

   #ifdef DEBUG
      Log ( "Painter::PaintCursor: Cursor is now %s.", CursorVisible?"visible":"invisible" ) ;
   #endif // DEBUG
}
 
/****************************************************************************
 *                                                                          *
 *  Painter: Hide Cursor                                                    *
 *                                                                          *
 ****************************************************************************/

void Painter::HideCursor ( ) {

   if ( CursorFlag == 0 ) {
      WinShowCursor ( Window, FALSE ) ;
      if ( CursorVisible )
         PaintCursor ( ) ;
   } /* endif */
   CursorFlag ++ ;

   #ifdef DEBUG
      Log ( "Painter::HideCursor: Cursor is now hidden %i levels.", CursorFlag ) ;
   #endif // DEBUG
}
 
/****************************************************************************
 *                                                                          *
 *  Painter: Show Cursor                                                    *
 *                                                                          *
 ****************************************************************************/

void Painter::ShowCursor ( ) {

   CursorFlag = max ( 0, CursorFlag-1 ) ;
   if ( CursorFlag == 0 ) {
      WinShowCursor ( Window, TRUE ) ;
      PaintCursor ( ) ;
   } /* endif */

   #ifdef DEBUG
      Log ( "Painter::ShowCursor: Cursor is now hidden %i levels.", CursorFlag ) ;
   #endif // DEBUG
}
 
/****************************************************************************
 *                                                                          *
 *  Painter: Set Cursor Box                                                 *
 *                                                                          *
 ****************************************************************************/

void Painter::SetCursorBox ( POINTL box[] ) {

   CursorBoxLock.Request() ;
   HideCursor ( ) ;
   memcpy ( CursorBox, box, sizeof(CursorBox) ) ;
   ShowCursor ( ) ;
   CursorBoxLock.Release() ;
}
 
/****************************************************************************
 *                                                                          *
 *  Painter: Invalidate Rectangle                                           *
 *                                                                          *
 ****************************************************************************/

void Painter::InvalidateRectangle ( RECTL &Rectangle, BOOL Update ) {

   #ifdef DEBUG_INVALIDRECTANGLE
      Log ( "Painter::InvalidateRectangle: Invalidating %i,%i-%i,%i.",
         Rectangle.xLeft, Rectangle.yBottom, Rectangle.xRight, Rectangle.yTop ) ;
   #endif // DEBUG_INVALIDRECTANGLE

   InvalidRectangleLock.Request() ;
   InvalidRectangle.xLeft   = min ( InvalidRectangle.xLeft,   Rectangle.xLeft   ) ;
   InvalidRectangle.xRight  = max ( InvalidRectangle.xRight,  Rectangle.xRight  ) ;
   InvalidRectangle.yBottom = min ( InvalidRectangle.yBottom, Rectangle.yBottom ) ;
   InvalidRectangle.yTop    = max ( InvalidRectangle.yTop,    Rectangle.yTop    ) ;

   #ifdef DEBUG_INVALIDRECTANGLE
      Log ( "Painter::InvalidateRectangle: Invalid rectangle is %i,%i-%i,%i.",
         InvalidRectangle.xLeft, InvalidRectangle.yBottom, InvalidRectangle.xRight, InvalidRectangle.yTop ) ;
   #endif // DEBUG_INVALIDRECTANGLE

   InvalidRectangleLock.Release() ;

   if ( Update ) {
      DosSetPriority ( PRTYS_THREAD, PRTYC_FOREGROUNDSERVER, PRTYD_MAXIMUM, Thread ) ;
      Request.Post() ;
   } /* endif */
}
 
/****************************************************************************
 *                                                                          *
 *  Painter: Update Window if invalid rectangle has anything.               *
 *                                                                          *
 ****************************************************************************/

void Painter::UpdateWindow ( BOOL All, BOOL Wait ) {

   #ifdef DEBUG
      Log ( "Painter::UpdateWindow: Started.  Invalidating portions (or all) of window." ) ;
   #endif // DEBUG

   if ( All ) {
      RECTL Box = { 0, 0, 100000, 100000 } ;
      InvalidateRectangle ( Box ) ;
   } else {
      InvalidateRectangle ( *pDocument->ComputeInvalidRectangle() ) ;
   } /* endif */

   InvalidRectangleLock.Request() ;

   if ( ( InvalidRectangle.xRight > InvalidRectangle.xLeft ) AND ( InvalidRectangle.yTop > InvalidRectangle.yBottom ) ) {
      if ( NOT Wait ) {
         DosSetPriority ( PRTYS_THREAD, PRTYC_FOREGROUNDSERVER, PRTYD_MAXIMUM, Thread ) ;
         Request.Post() ;
      } /* endif */
   } /* endif */

   InvalidRectangleLock.Release() ;

   if ( Wait ) 
      Paint ( PS ) ;

   #ifdef DEBUG
      Log ( "Painter::UpdateWindow: Done." ) ;
   #endif // DEBUG
}
 
/****************************************************************************
 *                                                                          *
 *  Painter: Paint (called by thread)                                       *
 *                                                                          *
 ****************************************************************************/

void Painter::Paint ( WorkSpace *PS ) {

   #ifdef DEBUG
      Log ( "Painter::Paint: Checking if painting required." ) ;
   #endif // DEBUG

   pDocument->LockCurrentPage ( ) ;

   UpdateLock.Request ( ) ;

   if ( ( pDocument->Query_State0()->Metric != BOOL(Metric) ) OR ( pDocument->Query_Parms()->CodePage != CodePage ) ) {
      Metric = int ( pDocument->Query_State0()->Metric ) ;
      CodePage = int ( pDocument->Query_Parms()->CodePage ) ;
      PS->Reset ( CodePage, int(Metric), 0, 0 ) ;
   } /* endif */

   InvalidRectangleLock.Request() ;

   RECTL ClippingRectangle = InvalidRectangle ;

   InvalidRectangle.xLeft = InvalidRectangle.yBottom = 0x7FFFFFFF ;
   InvalidRectangle.xRight = InvalidRectangle.yTop = 0 ;

   InvalidRectangleLock.Release() ;

   if ( ( ClippingRectangle.xRight > ClippingRectangle.xLeft ) AND ( ClippingRectangle.yTop > ClippingRectangle.yBottom ) ) {

      #ifdef DEBUG_INVALIDRECTANGLE
         Log ( "Painter::Paint: Painting rectangle %i,%i-%i,%i.",
            ClippingRectangle.xLeft, ClippingRectangle.yBottom, ClippingRectangle.xRight, ClippingRectangle.yTop ) ;
      #endif // DEBUG_INVALIDRECTANGLE

      pDocument->Paint ( PS, ClippingRectangle ) ;

   } /* endif */

   UpdateLock.Release ( ) ;

   pDocument->UnlockCurrentPage ( ) ;
}
 
/****************************************************************************
 *                                                                          *
 *  Painter: Lock Update                                                    *
 *                                                                          *
 ****************************************************************************/

void Painter::LockUpdate ( ) {

   #ifdef DEBUG
      Log ( "Painter::LockUpdate entered." ) ;
   #endif // DEBUG

   UpdateLock.Request ( ) ;

   #ifdef DEBUG
      Log ( "Painter::LockUpdate exit." ) ;
   #endif // DEBUG
}
 
/****************************************************************************
 *                                                                          *
 *  Painter: Unlock Update                                                  *
 *                                                                          *
 ****************************************************************************/

void Painter::UnlockUpdate ( ) {

   #ifdef DEBUG
      Log ( "Painter::UnlockUpdate." ) ;
   #endif // DEBUG

   UpdateLock.Release ( ) ;
}
 
/****************************************************************************
 ****************************************************************************
 *** Thread: Window Painter *************************************************
 ****************************************************************************
 ****************************************************************************/

static void Painter_Thread ( void *Parameter ) {

  /**************************************************************************
   * Get parameters.                                                        *
   **************************************************************************/

   Painter *Parms = (Painter*) Parameter ;

  /**************************************************************************
   * Connect to PM.                                                         *
   **************************************************************************/

   Process Proc ( "Painter_Thread", 0, LibraryHandle ) ;

  /**************************************************************************
   * Wait for paint requests and process them.                              *
   **************************************************************************/

   while ( Parms->Query_Active() ) {

      Parms->Query_Request()->Wait() ;
      Parms->Query_Request()->Reset() ;

      if ( Parms->Query_Active() ) {
         Parms->Paint ( Parms->Query_WorkSpace() ) ;
      } /* endif */

   } /* endwhile */

  /**************************************************************************
   * Notify anyone waiting that the thread is done.                         *
   **************************************************************************/

   Parms->Query_ThreadDone()->Post() ;
}
