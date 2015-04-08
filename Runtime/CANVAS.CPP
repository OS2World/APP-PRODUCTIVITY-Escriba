/***************************************************************** Canvas.CPP
 *                                                                          *
 *                   Graphical Workspace (Canvas) Object                    *
 *                                                                          *
 ****************************************************************************/

#include "System.h"

#include "Canvas.h"
#include "Debug.h"

/****************************************************************************
 *                                                                          *
 *  Canvas: Simple unscaled workspace associated with a window.             *
 *                                                                          *
 ****************************************************************************/

Canvas::Canvas ( char *tag, HWND Window ) : Tag(tag), Anchor(0), Handle(0), Type(WRKSPC_GETPS) {
   Lock.Request ( ) ;
   Anchor = WinQueryAnchorBlock ( Window ) ;
   Handle = WinGetPS ( Window ) ;
   Lock.Release ( ) ;
} /* endmethod */

/****************************************************************************
 *                                                                          *
 *  Canvas: Destructor                                                      *
 *                                                                          *
 ****************************************************************************/

Canvas::~Canvas ( ) {

   Lock.Request ( ) ;

   switch ( Type ) {

      case WRKSPC_GETPS:
         if ( Handle ) { 
            if ( !WinReleasePS ( Handle ) ) {
               char Message [512] ;
               Log ( "Canvas(%08X:'%s')::~Canvas() Unable to release canvas: %s", this, Tag, InterpretWinError(Anchor,Message) ) ;
            } /* endif */
            Handle = 0 ;
         } /* endif */
         break;

      case WRKSPC_PAINT:
         if ( Handle ) { 
            if ( !WinEndPaint ( Handle ) ) {
               char Message [512] ;
               Log ( "Canvas(%08X:'%s')::~Canvas() Unable to end paint: %s", this, Tag, InterpretWinError(Anchor,Message) ) ;
            } /* endif */
            Handle = 0 ;
         } /* endif */
         break;

      case WRKSPC_GPI:
         if ( Handle ) { 
            if ( !GpiDestroyPS ( Handle ) ) {
               char Message [512] ;
               Log ( "Canvas(%08X:'%s')::~Canvas() Unable to destroy canvas: %s", this, Tag, InterpretWinError(Anchor,Message) ) ;
            } /* endif */
            Handle = 0 ;
         } /* endif */
         break;

      default: {
         Log ( "Canvas(%08X:'%s')::~Canvas() Undefined type!", this, Tag ) ;
         break; }

   } /* endswitch */

   Lock.Release ( ) ;

} /* endmethod */

/****************************************************************************
 *                                                                          *
 *  Method: Query Font Count                                                *
 *                                                                          *
 ****************************************************************************/

int Canvas::QueryFontCount ( ) {

   long Count = 0 ;
   Count = GpiQueryFonts ( Handle, QF_PUBLIC | QF_PRIVATE, 0, &Count, 0, 0 ) ;
   if ( Count == GPI_ALTERROR ) {
      char Message [512] ;
      Log ( "Canvas(%08X:'%s')::QueryFontCount: Unable to query font count: %s", this, Tag, InterpretWinError(Anchor,Message) ) ;
   } /* endif */

   return ( (int)Count ) ;
}
 
/****************************************************************************
 *                                                                          *
 *  Method: Query Font Information                                          *
 *                                                                          *
 ****************************************************************************/

int Canvas::QueryFonts ( int count, PFONTMETRICS pfm ) {

   long Count ( count ) ;
   int Result = GpiQueryFonts ( Handle, QF_PUBLIC | QF_PRIVATE, 0, &Count, sizeof(FONTMETRICS), pfm ) ;
   if ( Result == GPI_ALTERROR ) {
      char Message [512] ;
      Log ( "Canvas(%08X:'%s')::QueryFonts: Unable to query font information: %s", this, Tag, InterpretWinError(Anchor,Message) ) ;
   } /* endif */

   return ( Result ) ;
}
 

