/****************************************************************** PAINTER.H
 *                                                                          *
 *                     Document Window Painting Thread                      *
 *                                                                          *
 ****************************************************************************/

#ifndef PAINTER_H
#define PAINTER_H

#include <stdlib.h>

#include "System.h"

#include "Event.h"
#include "Mutex.h"
#include "WrkSpace.h"

class Documento ;

class Painter {
   private:
      BOOL      Active ;
      Event     Request ;
      TID       Thread ;
      Event     ThreadDone ;

      HWND      Window ;
      DeviceContext *pDevice ;
      Documento *pDocument ;

      int       CodePage ;
      int       Metric ;
      WorkSpace *PS ;

      RECTL     InvalidRectangle ;
      Mutex     InvalidRectangleLock ;

      int       CursorFlag ;
      BOOL      CursorVisible ;
      POINTL    CursorBox [TXTBOX_COUNT] ;
      Mutex     CursorBoxLock ;

      Mutex     UpdateLock ;

   public:
      Painter ( HWND window, DeviceContext *pDevice, Documento *pdocument ) ;
      ~Painter ( ) ;

      BOOL       Query_Active     ( ) { return ( Active      ) ; }
      DeviceContext *Query_Device ( ) { return ( pDevice     ) ; }
      Documento *Query_pDocument  ( ) { return ( pDocument   ) ; }
      Event     *Query_Request    ( ) { return ( &Request    ) ; }
      Event     *Query_ThreadDone ( ) { return ( &ThreadDone ) ; }
      HWND       Query_Window     ( ) { return ( Window      ) ; }
      WorkSpace *Query_WorkSpace  ( ) { return ( PS          ) ; }

      void PaintCursor ( ) ;
      void HideCursor ( ) ;
      void ShowCursor ( ) ;
      void SetCursorBox ( POINTL box[] ) ;

      void InvalidateRectangle ( RECTL &Rectangle, BOOL Update=TRUE ) ;
      void UpdateWindow ( BOOL All=FALSE, BOOL Wait=FALSE ) ;
      void Paint ( WorkSpace *PS ) ;

      void LockUpdate ( ) ;
      void UnlockUpdate ( ) ;
} ;

#endif
