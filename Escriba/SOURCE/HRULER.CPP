/***************************************************************** HRULER.CPP
 *                                                                          *
 *                        Horizontal Ruler Class                            *
 *                                                                          *
 ****************************************************************************/

#include "System.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Debug.h"
#include "Escriba.h"
#include "HRuler.h"
#include "Support.h"
#include "WrkSpace.h"

// #define DEBUG
// #define DEBUG_MESSAGES

 
/****************************************************************************
 *                                                                          *
 *                     Definitions & Declarations                           *
 *                                                                          *
 ****************************************************************************/

  // Constants

enum { NOTMOVING, MOVELEFT, MOVERIGHT, MOVETAB, SETTAB } ;


  // Macros

#define TICK(Position) \
  LONG(((( Position + ((Data->Mode==MOVERIGHT)?ButtonSize.y:0) )+25)/50)*50)


  // Types

typedef struct {
   INIDATA *IniData ;
   HWND     MainWindow ;

   DeviceContext *pDevice ;

   BOOL     Metric ;
   FIXED    fxZoom ;
   LONG     PageWidth ;
   LONG     TopLeft ;
   LONG     LeftMargin ;
   LONG     RightMargin ;
   int      TabCount ;
   int      Tabs [MAX_TABS] ;
   LONG     Tick ;

   BOOL      Capture ;
   HWND      HadFocus ;
   USHORT    Mode ;
   USHORT    TabIndex ;
   HPOINTER  Ptr ;
   HPOINTER  MarginPtr ;
   HPOINTER  TabstopPtr ;
   BOOL      SettingFont ;

} DATA, *PDATA ;


  // Function Prototypes

static WINDOWPROC Create ;
static WINDOWPROC Destroy ;
static WINDOWPROC Paint ;
static WINDOWPROC Button1Down ;
static WINDOWPROC MouseMove ;
static WINDOWPROC Button1Up ;
static WINDOWPROC Char ;
static WINDOWPROC PresParamChanged ;
static WINDOWPROC SetRuler ;
static WINDOWPROC SetTick ;

 
/****************************************************************************
 *                                                                          *
 *      Window Message Processor                                            *
 *                                                                          *
 ****************************************************************************/

extern MRESULT EXPENTRY HorzRulerMessageProcessor ( HWND Window, MESG msg, MPARAM1 mp1, MPARAM2 mp2 ) {

  /**************************************************************************
   * Dispatch the message according to the method table and return the      *
   *   result.  Any messages not defined above get handled by the system    *
   *   default window processor.                                            *
   **************************************************************************/

   static METHOD Methods [] = {
      { WM_CREATE,                Create              },
      { WM_DESTROY,               Destroy             },
      { WM_PAINT,                 Paint               },
      { WM_BUTTON1DOWN,           Button1Down         },
      { WM_BUTTON2DOWN,           Button1Down         },
      { WM_MOUSEMOVE,             MouseMove           },
      { WM_BUTTON1UP,             Button1Up           },
      { WM_BUTTON2UP,             Button1Up           },
      { WM_CHAR,                  Char                },
      { WM_PRESPARAMCHANGED,      PresParamChanged    },
      { WM_SET_RULER,             SetRuler            },
      { WM_SET_TICK,              SetTick             },
   } ;

   #ifdef DEBUG_MESSAGES
      static int Indent = 0 ;
      LogMsgRcvd ( Indent, "HRULER", msg, mp1, mp2 ) ;
      Indent += 2 ;
   #endif

   MRESULT Result = DispatchMessage ( Window, msg, mp1, mp2, Methods, sizeof(Methods)/sizeof(Methods[0]), Sys_DefWindowProc ) ;

   #ifdef DEBUG_MESSAGES
      Indent -= 2 ;
      LogMsgDone ( Indent, "HRULER", msg, Result ) ;
   #endif

   return ( Result ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Create the window.                                                  *
 *                                                                          *
 ****************************************************************************/

static MRESULT APIENTRY Create ( HWND Window, MESG, MPARAM1 mp1, MPARAM2 ) {

  /**************************************************************************
   * Allocate instance data.                                                *
   **************************************************************************/

   PDATA Data = PDATA ( malloc ( sizeof(DATA) ) ) ;

   if ( Data == NULL ) {
      Log ( "ERROR: Unable to allocate instance memory for horizontal ruler." ) ;
      return ( MRFROMSHORT ( 1 ) ) ;
   } /* endif */

   Sys_SetWindowData ( Window, Data ) ;

  /**************************************************************************
   * Grab any parameters from the WM_CREATE message.                        *
   **************************************************************************/

   PHORZRULER_PARMS Parms = PHORZRULER_PARMS ( PVOIDFROMMP ( mp1 ) ) ;

   Data->IniData = Parms->IniData ;
   Data->MainWindow = Parms->MainWindow ;

  /**************************************************************************
   * Create the device context object for the window.                       *
   **************************************************************************/

   WinOpenWindowDC ( Window ) ;
   Data->pDevice = new DeviceContext ( "HRuler", Window ) ;

  /**************************************************************************
   * Load the pointers.                                                     *
   **************************************************************************/

   Data->MarginPtr = WinLoadPointer ( HWND_DESKTOP, 0, ID_MARGIN ) ;
   Data->TabstopPtr = WinLoadPointer ( HWND_DESKTOP, 0, ID_TABSTOP ) ;

  /**************************************************************************
   * Perform all other instance initializations.                            *
   **************************************************************************/

   Data->Metric = FALSE ;
   Data->fxZoom = MAKEFIXED ( 1, 0 ) ;
   Data->PageWidth = 9000 ;
   Data->TopLeft = 9000 ;
   Data->LeftMargin = 0 ;
   Data->RightMargin = 9000 ;
   Data->TabCount = 0 ;
   memset ( Data->Tabs, 0, sizeof(Data->Tabs) ) ;
   Data->Tick = 0 ;
   Data->Capture = FALSE ;
   Data->Mode = NOTMOVING ;
   Data->SettingFont = FALSE ;

  /**************************************************************************
   * Success?  Return no error.                                             *
   **************************************************************************/

   return ( MRFROMSHORT ( 0 ) ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Destroy Window.                                                     *
 *                                                                          *
 ****************************************************************************/

static MRESULT APIENTRY Destroy ( HWND Window, MESG, MPARAM1, MPARAM2 ) {

  /**************************************************************************
   * Find the instance data.                                                *
   **************************************************************************/

   PDATA Data = PDATA ( Sys_GetWindowData ( Window ) ) ;

  /**************************************************************************
   * Discard the pointers.                                                  *
   **************************************************************************/

   WinDestroyPointer ( Data->MarginPtr ) ;
   WinDestroyPointer ( Data->TabstopPtr ) ;

  /**************************************************************************
   * Destroy the device context object for the window.                      *
   **************************************************************************/

   delete Data->pDevice ;

  /**************************************************************************
   * Release the instance data.                                             *
   **************************************************************************/

   free ( Data ) ;

  /**************************************************************************
   * We're done.                                                            *
   **************************************************************************/

   return ( MRFROMSHORT ( 0 ) ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Repaint entire window.                                              *
 *                                                                          *
 ****************************************************************************/

static MRESULT APIENTRY Paint ( HWND Window, MESG, MPARAM1, MPARAM2 ) {

  /**************************************************************************
   * Find the instance data.                                                *
   **************************************************************************/

   PDATA Data = PDATA ( Sys_GetWindowData ( Window ) ) ;

  /**************************************************************************
   * Compute the current zoom factor.                                       *
   **************************************************************************/

   double Zoom = (double) FIXEDINT ( Data->fxZoom ) ;
   Zoom += (double) FIXEDFRAC ( Data->fxZoom ) / 0x10000L ;

  /**************************************************************************
   * Get presentation space and prepare it for use.                         *
   **************************************************************************/

   RECTL ClippingRectangle ;
   WorkSpace PS ( "HRuler::Paint", 0, Window, Data->pDevice, int(Data->Metric), ClippingRectangle ) ;
   PS.SetTransform ( Data->fxZoom, int(Data->TopLeft), 0 ) ;
   PS.Transform ( CVTC_DEVICE, CVTC_DEFAULTPAGE, ClippingRectangle ) ;

  /**************************************************************************
   * Clear the window and draw a border.                                    *
   **************************************************************************/

   RECTL WindowRectangle ;
   WinQueryWindowRect ( Window, &WindowRectangle ) ;

   WindowRectangle.xRight -- ;
   WindowRectangle.yTop -- ;

   PS.Transform ( CVTC_DEVICE, CVTC_DEFAULTPAGE, WindowRectangle ) ;

   PS.SetColor ( Data->IniData->fRulerColors[0] ? Data->IniData->RulerColors[0] : RGB_WHITE ) ;
   PS.FillBox ( WindowRectangle ) ;

   PS.SetColor ( RGB_BLACK ) ;
   PS.DrawBox ( WindowRectangle ) ;

  /**************************************************************************
   * Draw the left margin.                                                  *
   **************************************************************************/

   if ( ClippingRectangle.xLeft <= Data->LeftMargin ) {

      PS.SetColor ( Data->IniData->fRulerColors[1] ? Data->IniData->RulerColors[1] : RGB_BLUE ) ;
      PS.SetFillType ( PATSYM_HALFTONE ) ;
      RECTL Box = { ClippingRectangle.xLeft, ClippingRectangle.yBottom,
         Data->LeftMargin, ClippingRectangle.yTop } ;
      PS.FillBox ( Box ) ;
      PS.SetFillType ( PATSYM_DEFAULT ) ;

      PS.SetColor ( RGB_BLACK ) ;
      PS.SetLineType ( LINETYPE_ALTERNATE ) ;
      POINTL Point = { Data->LeftMargin, ClippingRectangle.yTop } ;
      PS.Move ( Point ) ;
      Point.y = ClippingRectangle.yTop ;
      PS.DrawLine ( Point ) ;
      PS.SetLineType ( LINETYPE_SOLID ) ;

   } /* endif */

   POINTL Point = { Data->LeftMargin, WindowRectangle.yTop } ;
   PS.Transform ( CVTC_DEFAULTPAGE, CVTC_DEVICE, 1, &Point ) ;
   Point.y -= 8 ;
   PS.DrawPointer ( Data->MarginPtr, Point, DP_NORMAL ) ;

  /**************************************************************************
   * Draw the right margin.                                                 *
   **************************************************************************/

   if ( ClippingRectangle.xRight >= Data->RightMargin ) {

      PS.SetColor ( Data->IniData->fRulerColors[1] ? Data->IniData->RulerColors[1] : RGB_BLUE ) ;
      PS.SetFillType ( PATSYM_HALFTONE ) ;
      RECTL Box = { Data->RightMargin, ClippingRectangle.yBottom,
         ClippingRectangle.xRight, ClippingRectangle.yTop } ;
      PS.FillBox ( Box ) ;
      PS.SetFillType ( PATSYM_DEFAULT ) ;

      PS.SetColor ( RGB_BLACK ) ;
      PS.SetLineType ( LINETYPE_ALTERNATE ) ;
      Point.x = Data->RightMargin ;
      Point.y = ClippingRectangle.yBottom ;
      PS.Move ( Point ) ;
      Point.y = ClippingRectangle.yTop ;
      PS.DrawLine ( Point ) ;
      PS.SetLineType ( LINETYPE_SOLID ) ;

   } /* endif */

   Point.x = Data->RightMargin ;
   Point.y = WindowRectangle.yTop ;
   PS.Transform ( CVTC_DEFAULTPAGE, CVTC_DEVICE, 1, &Point ) ;
   Point.x -= 8 ;
   Point.y -= 8 ;
   PS.DrawPointer ( Data->MarginPtr, Point, DP_NORMAL ) ;

  /**************************************************************************
   * Draw the tab-stops.                                                    *
   **************************************************************************/

   for ( int i=0; i<Data->TabCount; i++ ) {
      Point.x = Data->Tabs[i] ;
      Point.y = WindowRectangle.yTop ;
      PS.Transform ( CVTC_DEFAULTPAGE, CVTC_DEVICE, 1, &Point ) ;
      Point.y -= 8 ;
      PS.DrawPointer ( Data->TabstopPtr, Point, DP_NORMAL ) ;
   } /* endfor */

  /**************************************************************************
   * Draw the ruler.                                                        *
   **************************************************************************/

   for ( long x=WindowRectangle.xLeft; x<WindowRectangle.xRight; x++ ) {
      if ( x % ( Data->Metric ? 200 : 250 ) == 0 ) {
         PS.SetColor ( RGB_BLACK ) ;
         Point.x = x ;
         Point.y = 0 ;
         PS.Move ( Point ) ;
         if ( x % 1000 == 0 ) {
            Point.y = LONG ( 200.0 / Zoom ) ;
            PS.DrawLine ( Point ) ;
            Point.x += LONG ( 50.0 / Zoom ) ;
            Point.y = LONG ( 50.0 / Zoom ) ;
            char Text [10] ;
            sprintf ( Text, "%i", int(x/1000) ) ;
            PS.SetColor ( Data->IniData->fRulerColors[1] ? Data->IniData->RulerColors[1] : RGB_BLUE ) ;
            PS.Move ( Point ) ;
            PS.DrawText ( Text ) ;
         } else if ( x % 500 == 0 ) {
            Point.y = LONG ( 150.0 / Zoom ) ;
            PS.DrawLine ( Point ) ;
         } else {
            Point.y = LONG ( 100.0 / Zoom ) ;
            PS.DrawLine ( Point ) ;
         } /* endif */
      } /* endif */
   } /* endfor */

  /**************************************************************************
   * We're done.                                                            *
   **************************************************************************/

   return ( MRFROMSHORT ( 0 ) ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process Mouse Button 1 Down                                         *
 *                                                                          *
 ****************************************************************************/

static MRESULT APIENTRY Button1Down ( HWND Window, MESG, MPARAM1 mp1, MPARAM2 ) {

 /***************************************************************************
  * Find the instance data.                                                 *
  ***************************************************************************/

  PDATA Data = PDATA ( Sys_GetWindowData ( Window ) ) ;

 /***************************************************************************
  * Find out where the mouse touched.                                       *
  ***************************************************************************/

  RECTL Rectangle ;
  WinQueryWindowRect ( Window, &Rectangle ) ;

  WorkSpace PS ( "HRuler::Button1Down", 0, Window, Data->pDevice, int(Data->Metric) ) ;
  PS.SetTransform ( Data->fxZoom, int(Data->TopLeft), 0 ) ;

  POINTL Mouse = { SHORT1FROMMP(mp1), SHORT2FROMMP(mp1) } ;
  PS.Transform ( CVTC_DEVICE, CVTC_DEFAULTPAGE, 1, &Mouse ) ;

  PS.Transform ( CVTC_DEVICE, CVTC_DEFAULTPAGE, Rectangle ) ;

  POINTL ButtonSize = { 8, 8 } ;
  PS.Transform ( CVTC_DEVICE, CVTC_PAGE, 1, &ButtonSize ) ;

  #ifdef DEBUG
     Log ( "HRuler::Button1Down: Mouse at %i,%i (%i,%i).  Rectangle %i,%i-%i,%i.  ButtonSize %i.%i (8x8).",
        Mouse.x, Mouse.y, SHORT1FROMMP(mp1), SHORT2FROMMP(mp1), 
        Rectangle.xLeft, Rectangle.yBottom, Rectangle.xRight, Rectangle.yTop,
        ButtonSize.x, ButtonSize.y ) ;
  #endif

 /***************************************************************************
  * If the mouse isn't touching in the button area, return.                 *
  ***************************************************************************/

  if ( Mouse.y < Rectangle.yTop - ButtonSize.y ) {
     Sys_BeepNote ( ) ;
     return ( MRFROMSHORT ( FALSE ) ) ;
  } /* endif */

 /***************************************************************************
  * If the mouse is beyond the margins, return.                             *
  ***************************************************************************/

  if ( ( Mouse.x < Data->LeftMargin ) OR ( Mouse.x > Data->RightMargin+ButtonSize.x ) ) {
     Sys_BeepNote ( ) ;
     return ( MRFROMSHORT ( FALSE ) ) ;
  } /* endif */

 /***************************************************************************
  * If mouse is touching the left margin button, set mode to MOVELEFT.      *
  ***************************************************************************/

  if ( ( Mouse.x >= Data->LeftMargin ) AND ( Mouse.x <= Data->LeftMargin+ButtonSize.x ) ) {
     Data->Mode = MOVELEFT ;
     Data->Ptr = Data->MarginPtr ;
  } /* endif */

 /***************************************************************************
  * If mouse is touching the right margin button, set mode to MOVERIGHT.    *
  ***************************************************************************/

  else if ( ( Mouse.x >= Data->RightMargin-ButtonSize.x ) AND ( Mouse.x <= Data->RightMargin ) ) {
     Data->Mode = MOVERIGHT ;
     Data->Ptr = Data->MarginPtr ;
  } /* endif */

 /***************************************************************************
  * Else, find which tab-stop, if any, we're touching.                      *
  ***************************************************************************/

  else {
     for ( int i=0; i<Data->TabCount; i++ ) {
        if ( ( Mouse.x >= Data->Tabs[i] ) AND ( Mouse.x <= Data->Tabs[i]+ButtonSize.x ) ) {
           Data->Mode = MOVETAB ;
           Data->TabIndex = USHORT ( i ) ;
           break ;
        } /* endif */
     } /* endfor */

     if ( i >= Data->TabCount ) {
        Data->Mode = SETTAB ;
     } /* endif */

     Data->Ptr = Data->TabstopPtr ;
  } /* endif */

 /***************************************************************************
  * Capture the mouse.                                                      *
  ***************************************************************************/

  Data->Capture = TRUE ;
  Sys_SetCapture ( Window ) ;

 /***************************************************************************
  * Set the new mouse pointer.                                              *
  ***************************************************************************/

  WinSetPointer ( HWND_DESKTOP, Data->Ptr ) ;

 /***************************************************************************
  * Get the keyboard focus.                                                 *
  ***************************************************************************/

  Data->HadFocus = WinQueryFocus ( HWND_DESKTOP ) ;
  Sys_SetFocus ( Window ) ;

 /***************************************************************************
  * Save the current hairline position.                                     *
  ***************************************************************************/

  Data->Tick = TICK(Mouse.x) ;

 /***************************************************************************
  * Draw a hairline showing the current position.                           *
  ***************************************************************************/

  PS.SetMix ( FM_INVERT ) ;
  PS.SetLineType ( LINETYPE_ALTERNATE ) ;
  POINTL Point = { Data->Tick, Rectangle.yBottom } ;
  PS.Move ( Point ) ;
  Point.y = Rectangle.yTop ;
  PS.DrawLine ( Point ) ;

 /***************************************************************************
  * We're done.                                                             *
  ***************************************************************************/

  return ( MRFROMSHORT ( TRUE ) ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process Mouse Motion                                                *
 *                                                                          *
 ****************************************************************************/

static MRESULT APIENTRY MouseMove ( HWND Window, MESG msg, MPARAM1 mp1, MPARAM2 mp2 ) {

 /***************************************************************************
  * Find the instance data.                                                 *
  ***************************************************************************/

  PDATA Data = PDATA ( Sys_GetWindowData ( Window ) ) ;

 /***************************************************************************
  * If the mouse wasn't captured, return.                                   *
  ***************************************************************************/

  if ( NOT Data->Capture )
    return ( Sys_DefWindowProc ( Window, msg, mp1, mp2 ) ) ;

 /***************************************************************************
  * Get the presentation space.                                             *
  ***************************************************************************/

  WorkSpace PS ( "HRuler::MouseMove", 0, Window, Data->pDevice, int(Data->Metric) ) ;
  PS.SetTransform ( Data->fxZoom, int(Data->TopLeft), 0 ) ;

 /***************************************************************************
  * Find out where the mouse touched.                                       *
  ***************************************************************************/

  RECTL Rectangle ;
  WinQueryWindowRect ( Window, &Rectangle ) ;

  POINTL Mouse = { SHORT1FROMMP(mp1), SHORT2FROMMP(mp1) } ;
  PS.Transform ( CVTC_DEVICE, CVTC_DEFAULTPAGE, 1, &Mouse ) ;

  PS.Transform ( CVTC_DEVICE, CVTC_DEFAULTPAGE, Rectangle ) ;

  POINTL ButtonSize = { 8, 8 } ;
  PS.Transform ( CVTC_DEVICE, CVTC_PAGE, 1, &ButtonSize ) ;

  #ifdef DEBUG
     Log ( "HRuler::MouseMove:   Mouse at %i,%i (%i,%i).  Rectangle %i,%i-%i,%i.  ButtonSize %i.%i (8x8).",
        Mouse.x, Mouse.y, SHORT1FROMMP(mp1), SHORT2FROMMP(mp1), 
        Rectangle.xLeft, Rectangle.yBottom, Rectangle.xRight, Rectangle.yTop,
        ButtonSize.x, ButtonSize.y ) ;
  #endif

 /***************************************************************************
  * Erase the previous hairline.                                            *
  ***************************************************************************/

  PS.SetMix ( FM_INVERT ) ;
  PS.SetLineType ( LINETYPE_ALTERNATE ) ;

  POINTL Point = { Data->Tick, Rectangle.yBottom } ;
  PS.Move ( Point ) ;
  Point.y = Rectangle.yTop ;
  PS.DrawLine ( Point ) ;

 /***************************************************************************
  * Save the current hairline position.                                     *
  ***************************************************************************/

  Data->Tick = TICK(Mouse.x) ;

 /***************************************************************************
  * Draw a hairline showing the current position.                           *
  ***************************************************************************/

  Point.x = Data->Tick ;
  Point.y = Rectangle.yBottom ;
  PS.Move ( Point ) ;
  Point.y = Rectangle.yTop ;
  PS.DrawLine ( Point ) ;

 /***************************************************************************
  * We're done.                                                             *
  ***************************************************************************/

  return ( MRFROMSHORT ( TRUE ) ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process Mouse Button 1 Up                                           *
 *                                                                          *
 ****************************************************************************/

static MRESULT APIENTRY Button1Up ( HWND Window, MESG, MPARAM1 mp1, MPARAM2 ) {

 /***************************************************************************
  * Find the instance data.                                                 *
  ***************************************************************************/

  PDATA Data = PDATA ( Sys_GetWindowData ( Window ) ) ;

 /***************************************************************************
  * If the mouse wasn't captured, return.                                   *
  ***************************************************************************/

  if ( NOT Data->Capture )
    return ( MRFROMSHORT ( FALSE ) ) ;

 /***************************************************************************
  * Get the presentation space.                                             *
  ***************************************************************************/

  WorkSpace PS ( "HRuler::Button1Up", 0, Window, Data->pDevice, int(Data->Metric) ) ;
  PS.SetTransform ( Data->fxZoom, int(Data->TopLeft), 0 ) ;

 /***************************************************************************
  * Find out where the mouse touched.                                       *
  ***************************************************************************/

  RECTL Rectangle ;
  WinQueryWindowRect ( Window, &Rectangle ) ;

  POINTL Mouse = { SHORT1FROMMP(mp1), SHORT2FROMMP(mp1) } ;
  PS.Transform ( CVTC_DEVICE, CVTC_DEFAULTPAGE, 1, &Mouse ) ;

  PS.Transform ( CVTC_DEVICE, CVTC_DEFAULTPAGE, Rectangle ) ;

  POINTL ButtonSize = { 8, 8 } ;
  PS.Transform ( CVTC_DEVICE, CVTC_PAGE, 1, &ButtonSize ) ;

  #ifdef DEBUG
     Log ( "HRuler::Button1Up:   Mouse at %i,%i (%i,%i).  Rectangle %i,%i-%i,%i.  ButtonSize %i.%i (8x8).",
        Mouse.x, Mouse.y, SHORT1FROMMP(mp1), SHORT2FROMMP(mp1), 
        Rectangle.xLeft, Rectangle.yBottom, Rectangle.xRight, Rectangle.yTop,
        ButtonSize.x, ButtonSize.y ) ;
  #endif

 /***************************************************************************
  * Erase the previous hairline.                                            *
  ***************************************************************************/

  PS.SetMix ( FM_INVERT ) ;
  PS.SetLineType ( LINETYPE_ALTERNATE ) ;
  POINTL Point = { Data->Tick, Rectangle.yBottom } ;
  PS.Move ( Point ) ;
  Point.y = Rectangle.yTop ;
  PS.DrawLine ( Point ) ;

 /***************************************************************************
  * Release the mouse.                                                      *
  ***************************************************************************/

  Data->Capture = FALSE ;
  Sys_ReleaseCapture ( ) ;

 /***************************************************************************
  * Restore the normal mouse pointer.                                       *
  ***************************************************************************/

  HPOINTER Ptr = WinQuerySysPointer ( HWND_DESKTOP, SPTR_ARROW, FALSE ) ;
  WinSetPointer ( HWND_DESKTOP, Ptr ) ;

 /***************************************************************************
  * Restore the keyboard focus.                                             *
  ***************************************************************************/

  Sys_SetFocus ( Data->HadFocus ) ;

 /***************************************************************************
  * Process final location of mouse.                                        *
  ***************************************************************************/

  switch ( Data->Mode ) {

    case MOVELEFT:
    {
      Data->Tick = TICK(Mouse.x) ;
      if ( ( Data->Tick < 0 ) OR ( Data->Tick >= Data->RightMargin ) )
      {
        Sys_BeepError ( ) ;
        break ;
      }
      Sys_SendMessage ( OWNER(Window), WM_SET_LEFTMARGIN, MPFROMLONG(Data->Tick), 0 ) ;
      break ;
    }

    case MOVERIGHT:
    {
      Data->Tick = TICK(Mouse.x) ;
      if ( ( Data->Tick <= Data->LeftMargin ) OR ( Data->Tick > Data->PageWidth ) )
      {
        Sys_BeepError ( ) ;
        break ;
      }
      Sys_SendMessage ( OWNER(Window), WM_SET_RIGHTMARGIN, MPFROMLONG(Data->Tick), 0 ) ;
      break ;
    }

    case MOVETAB:
    {
      Data->Tick = TICK(Mouse.x) ;
      if ( ( Mouse.y > Rectangle.yTop )
        OR ( Mouse.y < Rectangle.yBottom )
        OR ( Data->Tick <= Data->LeftMargin )
        OR ( Data->Tick >= Data->RightMargin ) )
      {
        Sys_SendMessage ( OWNER(Window), WM_CLEAR_TAB, MPFROMLONG(Data->Tabs[Data->TabIndex]), 0 ) ;
        break ;
      }

      Sys_SendMessage ( OWNER(Window), WM_MOVE_TAB, MPFROMLONG(Data->Tabs[Data->TabIndex]), MPFROMLONG(Data->Tick) ) ;
      break ;
    }

    case SETTAB:
    {
      Data->Tick = TICK(Mouse.x) ;
      if ( ( Mouse.y > Rectangle.yTop )
        OR ( Mouse.y < Rectangle.yBottom )
        OR ( Data->Tick <= Data->LeftMargin )
        OR ( Data->Tick >= Data->RightMargin ) )
      {
        Sys_BeepError ( ) ;
        break ;
      }

      Sys_SendMessage ( OWNER(Window), WM_SET_TAB, MPFROMLONG(Data->Tick), 0 ) ;
      break ;
    }
  }

 /***************************************************************************
  * Reset mode.                                                             *
  ***************************************************************************/

  Data->Mode = NOTMOVING ;

 /***************************************************************************
  * We're done.                                                             *
  ***************************************************************************/

  return ( MRFROMSHORT ( TRUE ) ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process keystrokes.                                                 *
 *                                                                          *
 ****************************************************************************/

static MRESULT APIENTRY Char ( HWND Window, MESG, MPARAM1 mp1, MPARAM2 mp2 ) {

 /***************************************************************************
  * Find the instance data.                                                 *
  ***************************************************************************/

  PDATA Data = PDATA ( Sys_GetWindowData ( Window ) ) ;

 /***************************************************************************
  * Get the message data.                                                   *
  ***************************************************************************/

  USHORT Flags    = (USHORT) SHORT1FROMMP ( mp1 ) ;
//UCHAR  Repeat   = (UCHAR)   CHAR3FROMMP ( mp1 ) ;
//UCHAR  ScanCode = (UCHAR)   CHAR4FROMMP ( mp1 ) ;

//USHORT Char     = (USHORT) SHORT1FROMMP ( mp2 ) ;
  USHORT VKey     = (USHORT) SHORT2FROMMP ( mp2 ) ;

 /***************************************************************************
  * Ignore keys being released, invalid characters, dead keys and           *
  *   invalid composite characters.                                         *
  ***************************************************************************/

  if ( ( Flags & KC_KEYUP )
    OR ( Flags & KC_INVALIDCHAR )
    OR ( Flags & KC_DEADKEY )
    OR ( Flags & KC_INVALIDCOMP ) )
  {
    Sys_BeepNote ( ) ;
    return ( MRFROMSHORT ( FALSE ) ) ;
  }

 /***************************************************************************
  * Intercept the ESC virtual key to abort margin set mode.                 *
  ***************************************************************************/

  if ( Flags & KC_VIRTUALKEY ) {

    switch ( VKey ) {

      case VK_ESC: {

        if ( Data->Capture ) {

          Data->Capture = FALSE ;
          Sys_ReleaseCapture ( ) ;

          HPOINTER Ptr = WinQuerySysPointer ( HWND_DESKTOP, SPTR_ARROW, FALSE ) ;
          WinSetPointer ( HWND_DESKTOP, Ptr ) ;

          Sys_SetFocus ( Data->HadFocus ) ;

          WorkSpace PS ( "HRuler::Char", 0, Window, Data->pDevice, int(Data->Metric) ) ;
          PS.SetTransform ( Data->fxZoom, int(Data->TopLeft), 0 ) ;

          RECTL Rectangle ;
          WinQueryWindowRect ( Window, &Rectangle ) ;
          PS.Transform ( CVTC_DEVICE, CVTC_DEFAULTPAGE, Rectangle ) ;

          POINTL ButtonSize = { 8, 8 } ;
          PS.Transform ( CVTC_DEVICE, CVTC_PAGE, 1, &ButtonSize ) ;

          PS.SetMix ( FM_INVERT ) ;
          PS.SetLineType ( LINETYPE_ALTERNATE ) ;
          POINTL Point = { Data->Tick, Rectangle.yBottom } ;
          PS.Move ( Point ) ;
          Point.y = Rectangle.yTop ;
          PS.DrawLine ( Point ) ;

          Data->Mode = NOTMOVING ;
        }

        break ;
      }
    }
  }

 /***************************************************************************
  * Return.                                                                 *
  ***************************************************************************/

  return ( MRFROMSHORT ( TRUE ) ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process Presentation Parameter Changed notification.                *
 *                                                                          *
 ****************************************************************************/

static MRESULT APIENTRY PresParamChanged ( HWND Window, MESG msg, MPARAM1 mp1, MPARAM2 mp2 ) {

  /**************************************************************************
   * Find the instance data.                                                *
   **************************************************************************/

   PDATA Data = PDATA ( Sys_GetWindowData ( Window ) ) ;

  /**************************************************************************
   * Get the presentation parameter that changed.                           *
   **************************************************************************/

   switch ( LONGFROMMP(mp1) ) {

     /***********************************************************************
      * If font, note the fact that we now have a font to be saved as       *
      *   part of the configuration.  Apply the font to the Main window.    *
      ***********************************************************************/

      case PP_FONTNAMESIZE: {
         if ( Data->SettingFont )
            break ;
         Data->SettingFont = TRUE ;
         ULONG ppid ;
         char Font [80] ;
         if ( WinQueryPresParam ( Window, PP_FONTNAMESIZE, 0, &ppid, sizeof(Font), PSZ(Font), 0 ) ) {
            if ( strcmpi ( Font, PCHAR(Data->IniData->RulerFont) ) ) {
               WinSetPresParam ( Data->MainWindow, PP_FONTNAMESIZE, strlen(Font)+1, PSZ(Font) ) ;
            } /* endif */
         } /* endif */
         Data->SettingFont = FALSE ;
         break ; }

     /***********************************************************************
      * If background color, note the fact and repaint the window.          *
      ***********************************************************************/

      case PP_BACKGROUNDCOLOR: {
         ULONG ppid ;
         if ( WinQueryPresParam ( Window, PP_BACKGROUNDCOLOR, 0, &ppid,
            sizeof(Data->IniData->RulerColors[0]), &Data->IniData->RulerColors[0], 0 ) ) {
            Data->IniData->fRulerColors[0] = TRUE ;
         } else {
            Data->IniData->RulerColors[0] = RGB_WHITE ;
            Data->IniData->fRulerColors[0] = FALSE ;
         } /* endif */
         Sys_InvalidateWindow ( Data->MainWindow ) ;
         break ; }

     /***********************************************************************
      * If foreground color, note the fact and repaint the window.          *
      ***********************************************************************/

      case PP_FOREGROUNDCOLOR: {
         ULONG ppid ;
         if ( WinQueryPresParam ( Window, PP_FOREGROUNDCOLOR, 0, &ppid,
            sizeof(Data->IniData->RulerColors[1]), &Data->IniData->RulerColors[1], 0 ) ) {
            Data->IniData->fRulerColors[1] = TRUE ;
         } else {
            Data->IniData->RulerColors[1] = RGB_BLUE ;
            Data->IniData->fRulerColors[1] = FALSE ;
         } /* endif */
         Sys_InvalidateWindow ( Data->MainWindow ) ;
         break ; }

   } /* endswitch */

  /**************************************************************************
   * Return through the default processor, letting window activation        *
   *   and other system functions occur.                                    *
   **************************************************************************/

   return ( Sys_DefWindowProc ( Window, msg, mp1, mp2 ) ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Set Ruler Parameters                                                *
 *                                                                          *
 ****************************************************************************/

static MRESULT APIENTRY SetRuler ( HWND Window, MESG, MPARAM1 mp1, MPARAM2 ) {

  /**************************************************************************
   * Find the instance data.                                                *
   **************************************************************************/

   PDATA Data = PDATA ( Sys_GetWindowData ( Window ) ) ;

  /**************************************************************************
   * If ruler parameters have changed, save them and invalidate the window. *
   **************************************************************************/

   PHORZRULER_SETPARMS Parms = PHORZRULER_SETPARMS ( PVOIDFROMMP ( mp1 ) ) ;

   if ( ( Data->Metric      != Parms->Metric      )
      OR ( Data->fxZoom      != Parms->fxZoom      )
      OR ( Data->TopLeft     != Parms->TopLeft     )
      OR ( Data->PageWidth   != Parms->PageWidth   )
      OR ( Data->LeftMargin  != Parms->LeftMargin  )
      OR ( Data->RightMargin != Parms->RightMargin )
      OR ( Data->TabCount    != Parms->TabCount    )
      OR memcmp ( Data->Tabs, Parms->Tabs, sizeof(Data->Tabs) ) ) {

      #ifdef DEBUG
         Log ( "HRuler::SetRuler: Setting Margins to %i, %i.", Parms->LeftMargin, Parms->RightMargin ) ;
      #endif
      Data->Metric      = Parms->Metric ;
      Data->fxZoom      = Parms->fxZoom ;
      Data->TopLeft     = Parms->TopLeft ;
      Data->PageWidth   = Parms->PageWidth ;
      Data->LeftMargin  = Parms->LeftMargin ;
      Data->RightMargin = Parms->RightMargin ;
      Data->TabCount    = Parms->TabCount ;
      memcpy ( Data->Tabs, Parms->Tabs, sizeof(Data->Tabs) ) ;
      Sys_InvalidateWindow ( Window ) ;

   } /* endif */

  /********************************h*****************************************
   * We're done.                                                            *
   **************************************************************************/

   return ( MRFROMSHORT ( 0 ) ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Set Tick-Mark                                                       *
 *                                                                          *
 ****************************************************************************/

static MRESULT APIENTRY SetTick ( HWND Window, MESG, MPARAM1 mp1, MPARAM2 ) {

  /**************************************************************************
   * Find the instance data.                                                *
   **************************************************************************/

   PDATA Data = PDATA ( Sys_GetWindowData ( Window ) ) ;

  /**************************************************************************
   * Get the new tick-mark position from the message.                       *
   **************************************************************************/

   LONG Tick = LONGFROMMP ( mp1 ) ;

  /**************************************************************************
   * If only the tick-mark has changed, move it.                            *
   **************************************************************************/

   if ( Data->Tick != Tick ) {

     /***********************************************************************
      * Get the presentation space.                                         *
      ***********************************************************************/

      WorkSpace PS ( "HRuler::SetTick", 0, Window, Data->pDevice, int(Data->Metric) ) ;
      PS.SetTransform ( Data->fxZoom, int(Data->TopLeft), 0 ) ;

     /***********************************************************************
      * Find out some background information.                               *
      ***********************************************************************/

      RECTL Rectangle ;
      WinQueryWindowRect ( Window, &Rectangle ) ;
      PS.Transform ( CVTC_DEVICE, CVTC_DEFAULTPAGE, Rectangle ) ;

     /***********************************************************************
      * Erase the previous hairline.                                        *
      ***********************************************************************/

      if ( Data->Tick ) {
         PS.SetMix ( FM_INVERT ) ;
         PS.SetLineType ( LINETYPE_ALTERNATE ) ;
         POINTL Point = { Data->Tick, Rectangle.yBottom } ;
         PS.Move ( Point ) ;
         Point.y = Rectangle.yTop ;
         PS.DrawLine ( Point ) ;
      } /* endif */

     /***********************************************************************
      * Save the current hairline position.                                 *
      ***********************************************************************/

      Data->Tick = Tick ;

     /***********************************************************************
      * Draw a hairline showing the current position.                       *
      ***********************************************************************/

      if ( Data->Tick ) {
         PS.SetMix ( FM_INVERT ) ;
         PS.SetLineType ( LINETYPE_ALTERNATE ) ;
         POINTL Point = { Data->Tick, Rectangle.yBottom } ;
         PS.Move ( Point ) ;
         Point.y = Rectangle.yTop ;
         PS.DrawLine ( Point ) ;
      } /* endif */

   } /* endif */
 
  /**************************************************************************
   * We're done.                                                            *
   **************************************************************************/

   return ( MRFROMSHORT ( 0 ) ) ;
}
