/***************************************************************** VRULER.CPP
 *                                                                          *
 *                         Vertical Ruler Class                             *
 *                                                                          *
 ****************************************************************************/

#include "System.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Debug.h"
#include "Escriba.h"
#include "Support.h"
#include "VRuler.h"
#include "WrkSpace.h"

// #define DEBUG
// #define DEBUG_MESSAGES

 
/****************************************************************************
 *                                                                          *
 *                     Definitions & Declarations                           *
 *                                                                          *
 ****************************************************************************/

  // Constants

enum { MOVETOP, MOVEBOTTOM } ;


  // Macros

#define TICK(Position) \
   ( Data->TopLeft - LONG(((( Data->TopLeft - Position - ((Data->Mode==MOVETOP)?ButtonSize.y:0) )+25)/50)*50) )


  // Types

typedef struct {
   INIDATA *IniData ;
   HWND     MainWindow ;

   DeviceContext *pDevice ;

   BOOL     Metric ;
   FIXED    fxZoom ;
   LONG     PageHeight ;
   LONG     WindowHeight ;
   LONG     TopLeft ;
   LONG     BottomMargin ;
   LONG     TopMargin ;
   LONG     Tick ;

   BOOL      Capture ;
   HWND      HadFocus ;
   USHORT    Mode ;
   HPOINTER  MarginPtr ;
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

extern MRESULT EXPENTRY VertRulerMessageProcessor ( HWND Window, ULONG msg, MPARAM mp1, MPARAM mp2 ) {

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
      LogMsgRcvd ( Indent, "VRULER", Window, msg, mp1, mp2 ) ;
      Indent += 2 ;
   #endif // DEBUG_MESSAGES

   MRESULT Result = DispatchMessage ( Window, msg, mp1, mp2, Methods, sizeof(Methods)/sizeof(Methods[0]), Sys_DefWindowProc ) ;

   #ifdef DEBUG_MESSAGES
      Indent -= 2 ;
      LogMsgDone ( Indent, "VRULER", msg, Result ) ;
   #endif // DEBUG_MESSAGES

   return ( Result ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Create the window.                                                  *
 *                                                                          *
 ****************************************************************************/

static MRESULT APIENTRY Create ( HWND Window, ULONG, MPARAM mp1, MPARAM ) {

  /**************************************************************************
   * Allocate instance data.                                                *
   **************************************************************************/

   PDATA Data = new DATA ;

   if ( Data == NULL ) {
      #ifdef DEBUG
         Log ( "ERROR: Unable to allocate instance memory for vertical ruler.\n" ) ;
      #endif // DEBUG
      return ( MRFROMSHORT ( 1 ) ) ;
   } /* endif */

   Sys_SetWindowData ( Window, Data ) ;

  /**************************************************************************
   * Grab any parameters from the WM_CREATE message.                        *
   **************************************************************************/

   PVERTRULER_PARMS Parms = (PVERTRULER_PARMS) PVOIDFROMMP ( mp1 ) ;

   Data->IniData = Parms->IniData ;
   Data->MainWindow = Parms->MainWindow ;

  /**************************************************************************
   * Create the device context object for the window.                       *
   **************************************************************************/

   WinOpenWindowDC ( Window ) ;
   Data->pDevice = new DeviceContext ( "VRuler", Window ) ;

  /**************************************************************************
   * Load the pointers.                                                     *
   **************************************************************************/

   Data->MarginPtr = WinLoadPointer ( HWND_DESKTOP, 0, ID_MARGIN ) ;

  /**************************************************************************
   * Perform all other instance initializations.                            *
   **************************************************************************/

   Data->Metric = FALSE ;
   Data->fxZoom = MAKEFIXED ( 1, 0 ) ;
   Data->PageHeight = 4000 ;
   Data->WindowHeight = 4000 ;
   Data->TopLeft = Data->PageHeight ;
   Data->BottomMargin = 0 ;
   Data->TopMargin = Data->PageHeight ;
   Data->Tick = 0 ;
   Data->Capture = FALSE ;
   Data->HadFocus = 0 ;
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

static MRESULT APIENTRY Destroy ( HWND Window, ULONG, MPARAM, MPARAM ) {

  /**************************************************************************
   * Find the instance data.                                                *
   **************************************************************************/

   PDATA Data = PDATA ( Sys_GetWindowData ( Window ) ) ;

  /**************************************************************************
   * Discard the pointers.                                                  *
   **************************************************************************/

   WinDestroyPointer ( Data->MarginPtr ) ;

  /**************************************************************************
   * Destroy the device context object for the window.                      *
   **************************************************************************/

   delete Data->pDevice ;

  /**************************************************************************
   * Release the instance data.                                             *
   **************************************************************************/

   delete Data ;

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

static MRESULT APIENTRY Paint ( HWND Window, ULONG, MPARAM, MPARAM ) {

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
   WorkSpace PS ( "VRuler::Paint", 0, Window, Data->pDevice, int(Data->Metric), ClippingRectangle ) ;
   PS.SetTransform ( Data->fxZoom, 0, int(Data->TopLeft-Data->WindowHeight) ) ;
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
   * Draw the top margin.                                                   *
   **************************************************************************/

   if ( Data->TopMargin <= ClippingRectangle.yTop ) {

      PS.SetColor ( Data->IniData->fRulerColors[1] ? Data->IniData->RulerColors[1] : RGB_BLUE ) ;
      PS.SetFillType ( PATSYM_HALFTONE ) ;
      RECTL Box = { ClippingRectangle.xLeft, Data->TopMargin, ClippingRectangle.xRight, ClippingRectangle.yTop } ;
      PS.FillBox ( Box ) ;
      PS.SetFillType ( PATSYM_DEFAULT ) ;

      PS.SetColor ( RGB_BLACK ) ;
      PS.SetLineType ( LINETYPE_ALTERNATE ) ;
      POINTL Point = { ClippingRectangle.xLeft, Data->TopMargin } ;
      PS.Move ( Point ) ;
      Point.x = ClippingRectangle.xRight ;
      PS.DrawLine ( Point ) ;
      PS.SetLineType ( LINETYPE_DEFAULT ) ;

   } /* endif */

   POINTL Point = { 0, Data->TopMargin } ;
   PS.Transform ( CVTC_DEFAULTPAGE, CVTC_DEVICE, 1, &Point ) ;
   Point.y -= 8 ;
   PS.DrawPointer ( Data->MarginPtr, Point, DP_NORMAL ) ;

  /**************************************************************************
   * Draw the bottom margin.                                                *
   **************************************************************************/

   if ( Data->BottomMargin >= ClippingRectangle.yBottom ) {

      PS.SetColor ( Data->IniData->fRulerColors[1] ? Data->IniData->RulerColors[1] : RGB_BLUE ) ;
      PS.SetFillType ( PATSYM_HALFTONE ) ;
      RECTL Box = { ClippingRectangle.xLeft, ClippingRectangle.yBottom, ClippingRectangle.xRight, Data->BottomMargin } ;
      PS.FillBox ( Box ) ;
      PS.SetFillType ( PATSYM_DEFAULT ) ;

      PS.SetColor ( RGB_BLACK ) ;
      PS.SetLineType ( LINETYPE_ALTERNATE ) ;
      Point.x = ClippingRectangle.xLeft ;
      Point.y = Data->BottomMargin ;
      PS.Move ( Point ) ;
      Point.x = ClippingRectangle.xRight ;
      PS.DrawLine ( Point ) ;
      PS.SetLineType ( LINETYPE_DEFAULT ) ;

      Point.x = 0 ;
      Point.y = Data->BottomMargin ;
      PS.Transform ( CVTC_DEFAULTPAGE, CVTC_DEVICE, 1, &Point ) ;
      PS.DrawPointer ( Data->MarginPtr, Point, DP_NORMAL ) ;
   } /* endif */

  /**************************************************************************
   * Draw the ruler.                                                        *
   **************************************************************************/

   for ( LONG y=WindowRectangle.yBottom; y<WindowRectangle.yTop; y++ ) {
      if ( ( Data->PageHeight - y ) % ( Data->Metric ? 200 : 250 ) == 0 ) {
         PS.SetColor ( RGB_BLACK ) ;
         Point.x = WindowRectangle.xRight - 1 ;
         Point.y = y ;
         PS.Move ( Point ) ;
         if ( ( Data->PageHeight - y ) % 1000 == 0 ) {
            Point.x -= LONG ( 200.0 / Zoom ) ;
            PS.DrawLine ( Point ) ;
            Point.x += LONG ( 200.0 / Zoom ) ;
            char Text [10] ;
            sprintf ( Text, "%i", (int) ( ( Data->PageHeight - y ) / 1000 ) ) ;
            SIZEL Size ;
            PS.MeasureText ( Text, Size ) ;
            Point.x -= Size.cx ;
            Point.x -= LONG ( 50.0 / Zoom ) ;
            Point.y += LONG ( 50.0 / Zoom ) ;
            PS.SetColor ( Data->IniData->fRulerColors[1] ? Data->IniData->RulerColors[1] : RGB_BLUE ) ;
            PS.Move ( Point ) ;
            PS.DrawText ( Text ) ;

         } else if ( ( Data->PageHeight - y ) % 500 == 0 ) {
            Point.x -= LONG ( 150.0 / Zoom ) ;
            PS.DrawLine ( Point ) ;

         } else {
            Point.x -= LONG ( 100.0 / Zoom ) ;
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

static MRESULT APIENTRY Button1Down ( HWND Window, ULONG, MPARAM mp1, MPARAM ) {

 /***************************************************************************
  * Find the instance data.                                                 *
  ***************************************************************************/

  PDATA Data = PDATA ( Sys_GetWindowData ( Window ) ) ;

 /***************************************************************************
  * Find out where the mouse touched.                                       *
  ***************************************************************************/

  RECTL Rectangle ;
  WinQueryWindowRect ( Window, &Rectangle ) ;

  WorkSpace PS ( "VRuler::Button1Down", 0, Window, Data->pDevice, int(Data->Metric) ) ;
  PS.SetTransform ( Data->fxZoom, 0, int(Data->TopLeft-Data->WindowHeight) ) ;

  POINTL Mouse = { SHORT1FROMMP(mp1), SHORT2FROMMP(mp1) } ;
  PS.Transform ( CVTC_DEVICE, CVTC_DEFAULTPAGE, 1, &Mouse ) ;
  PS.Transform ( CVTC_DEVICE, CVTC_DEFAULTPAGE, Rectangle ) ;

  POINTL ButtonSize = { 8, 8 } ;
  PS.Transform ( CVTC_DEVICE, CVTC_PAGE, 1, &ButtonSize ) ;

 /***************************************************************************
  * If the mouse isn't touching in the button area, return.                 *
  ***************************************************************************/

  if ( ( Mouse.x < 0 ) OR ( Mouse.x > ButtonSize.x ) ) {
    Sys_BeepNote ( ) ;
    return ( MRFROMSHORT ( FALSE ) ) ;
  }

 /***************************************************************************
  * If mouse is touching the top margin button, set mode to MOVETOP.        *
  ***************************************************************************/

  if ( ( Mouse.y >= Data->TopMargin-ButtonSize.y ) AND ( Mouse.y <= Data->TopMargin ) ) {
    Data->Mode = MOVETOP ;

 /***************************************************************************
  * If mouse is touching the bottom margin button, set mode to MOVEBOTTOM.  *
  ***************************************************************************/

  } else if ( ( Mouse.y >= Data->BottomMargin ) AND ( Mouse.y <= Data->BottomMargin+ButtonSize.y ) ) {
    Data->Mode = MOVEBOTTOM ;

 /***************************************************************************
  * Else complain.                                                          *
  ***************************************************************************/

  } else {
    Sys_BeepNote ( ) ;
    return ( MRFROMSHORT ( FALSE ) ) ;
  }

 /***************************************************************************
  * Capture the mouse.                                                      *
  ***************************************************************************/

  Data->Capture = TRUE ;
  Sys_SetCapture ( Window ) ;

 /***************************************************************************
  * Set the new mouse pointer.                                              *
  ***************************************************************************/

  WinSetPointer ( HWND_DESKTOP, Data->MarginPtr ) ;

 /***************************************************************************
  * Get the keyboard focus.                                                 *
  ***************************************************************************/

  Data->HadFocus = WinQueryFocus ( HWND_DESKTOP ) ;
  Sys_SetFocus ( Window ) ;

 /***************************************************************************
  * Save the current hairline position.                                     *
  ***************************************************************************/

  Data->Tick = TICK(Mouse.y) ;

 /***************************************************************************
  * Draw a hairline showing the current position.                           *
  ***************************************************************************/

  PS.SetMix ( FM_INVERT ) ;
  PS.SetLineType ( LINETYPE_ALTERNATE ) ;
  POINTL Point = { Rectangle.xLeft, Data->Tick } ;
  PS.Move ( Point ) ;
  Point.x = Rectangle.xRight ;
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

static MRESULT APIENTRY MouseMove ( HWND Window, ULONG msg, MPARAM mp1, MPARAM mp2 ) {

 /***************************************************************************
  * Find the instance data.                                                 *
  ***************************************************************************/

  PDATA Data = PDATA ( Sys_GetWindowData ( Window ) ) ;

 /***************************************************************************
  * If the mouse wasn't captured, return.                                   *
  ***************************************************************************/

  if ( NOT Data->Capture ) {
    return ( Sys_DefWindowProc ( Window, msg, mp1, mp2 ) ) ;
  }

 /***************************************************************************
  * Find out where the mouse touched.                                       *
  ***************************************************************************/

  RECTL Rectangle ;
  WinQueryWindowRect ( Window, &Rectangle ) ;

  WorkSpace PS ( "VRuler::MouseMove", 0, Window, Data->pDevice, int(Data->Metric) ) ;
  PS.SetTransform ( Data->fxZoom, 0, int(Data->TopLeft-Data->WindowHeight) ) ;

  POINTL Mouse = { SHORT1FROMMP(mp1), SHORT2FROMMP(mp1) } ;
  PS.Transform ( CVTC_DEVICE, CVTC_DEFAULTPAGE, 1, &Mouse ) ;

  PS.Transform ( CVTC_DEVICE, CVTC_DEFAULTPAGE, Rectangle ) ;

  POINTL ButtonSize = { 8, 8 } ;
  PS.Transform ( CVTC_DEVICE, CVTC_PAGE, 1, &ButtonSize ) ;

 /***************************************************************************
  * Erase the previous hairline.                                            *
  ***************************************************************************/

  PS.SetMix ( FM_INVERT ) ;
  PS.SetLineType ( LINETYPE_ALTERNATE ) ;
  POINTL Point = { Rectangle.xLeft, Data->Tick } ;
  PS.Move ( Point ) ;
  Point.x = Rectangle.xRight ;
  PS.DrawLine ( Point ) ;

 /***************************************************************************
  * Save the current hairline position.                                     *
  ***************************************************************************/

  Data->Tick = TICK(Mouse.y) ;

 /***************************************************************************
  * Draw a hairline showing the current position.                           *
  ***************************************************************************/

  PS.SetMix ( FM_INVERT ) ;
  PS.SetLineType ( LINETYPE_ALTERNATE ) ;
  Point.x = Rectangle.xLeft ;
  Point.y = Data->Tick ;
  PS.Move ( Point ) ;
  Point.x = Rectangle.xRight ;
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

static MRESULT APIENTRY Button1Up ( HWND Window, ULONG, MPARAM mp1, MPARAM ) {

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

  WorkSpace PS ( "VRuler::Button1Up", 0, Window, Data->pDevice, int(Data->Metric) ) ;
  PS.SetTransform ( Data->fxZoom, 0, int(Data->TopLeft-Data->WindowHeight) ) ;

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

 /***************************************************************************
  * Erase the previous hairline.                                            *
  ***************************************************************************/

  PS.SetMix ( FM_INVERT ) ;
  PS.SetLineType ( LINETYPE_ALTERNATE ) ;
  POINTL Point = { Rectangle.xLeft, Data->Tick } ;
  PS.Move ( Point ) ;
  Point.x = Rectangle.xRight ;
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

    case MOVETOP:
    {
      Data->Tick = TICK(Mouse.y) ;
      if ( ( Mouse.y <= Data->BottomMargin ) OR ( Mouse.y > Data->PageHeight ) )
      {
        Sys_BeepError ( ) ;
        break ;
      }
      Sys_SendMessage ( OWNER(Window), WM_SET_TOPMARGIN, MPFROMLONG(Data->Tick), 0 ) ;
      break ;
    }

    case MOVEBOTTOM:
    {
      Data->Tick = TICK(Mouse.y) ;
      if ( ( Mouse.y < 0 ) OR ( Mouse.y >= Data->TopMargin ) )
      {
        Sys_BeepError ( ) ;
        break ;
      }
      Sys_SendMessage ( OWNER(Window), WM_SET_BOTTOMMARGIN, MPFROMLONG(Data->Tick), 0 ) ;
      break ;
    }
  }

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

static MRESULT APIENTRY Char ( HWND Window, ULONG, MPARAM mp1, MPARAM mp2 ) {

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

      case VK_ESC:
      {
        if ( Data->Capture ) {
          Data->Capture = FALSE ;
          Sys_ReleaseCapture ( ) ;

          HPOINTER Ptr = WinQuerySysPointer ( HWND_DESKTOP, SPTR_ARROW, FALSE ) ;
          WinSetPointer ( HWND_DESKTOP, Ptr ) ;

          Sys_SetFocus ( Data->HadFocus ) ;

          WorkSpace PS ( "VRuler::Char", 0, Window, Data->pDevice, int(Data->Metric) ) ;
          PS.SetTransform ( Data->fxZoom, 0, int(Data->TopLeft-Data->WindowHeight) ) ;

          RECTL Rectangle ;
          WinQueryWindowRect ( Window, &Rectangle ) ;

          PS.Transform ( CVTC_DEVICE, CVTC_DEFAULTPAGE, Rectangle ) ;

          POINTL ButtonSize = { 8, 8 } ;
          PS.Transform ( CVTC_DEVICE, CVTC_PAGE, 1, &ButtonSize ) ;

          PS.SetMix ( FM_INVERT ) ;
          PS.SetLineType ( LINETYPE_ALTERNATE ) ;
          POINTL Point = { Rectangle.xLeft, Data->Tick } ;
          PS.Move ( Point ) ;
          Point.x = Rectangle.xRight ;
          PS.DrawLine ( Point ) ;
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

static MRESULT APIENTRY PresParamChanged ( HWND Window, ULONG msg, MPARAM mp1, MPARAM mp2 ) {

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

static MRESULT APIENTRY SetRuler ( HWND Window, ULONG, MPARAM mp1, MPARAM ) {

 /***************************************************************************
  * Find the instance data.                                                 *
  ***************************************************************************/

  PDATA Data = PDATA ( Sys_GetWindowData ( Window ) ) ;

 /***************************************************************************
  * If ruler parameters have changed, save them and invalidate the window.  *
  ***************************************************************************/

  PVERTRULER_SETPARMS Parms = PVERTRULER_SETPARMS ( PVOIDFROMMP ( mp1 ) ) ;

  if ( ( Data->Metric       != Parms->Metric )
    OR ( Data->fxZoom       != Parms->fxZoom )
    OR ( Data->PageHeight   != Parms->PageHeight )
    OR ( Data->WindowHeight != Parms->WindowHeight )
    OR ( Data->TopLeft      != Parms->TopLeft )
    OR ( Data->BottomMargin != Parms->BottomMargin )
    OR ( Data->TopMargin    != Parms->TopMargin ) )
  {
    Data->Metric       = Parms->Metric ;
    Data->fxZoom       = Parms->fxZoom ;
    Data->PageHeight   = Parms->PageHeight ;
    Data->WindowHeight = Parms->WindowHeight ;
    Data->TopLeft      = Parms->TopLeft ;
    Data->BottomMargin = Parms->BottomMargin ;
    Data->TopMargin    = Parms->TopMargin ;
    Sys_InvalidateWindow ( Window ) ;
  }

 /***************************************************************************
  * We're done.                                                             *
  ***************************************************************************/

  return ( MRFROMSHORT ( 0 ) ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Set Tick-Mark                                                       *
 *                                                                          *
 ****************************************************************************/

static MRESULT APIENTRY SetTick ( HWND Window, ULONG, MPARAM mp1, MPARAM ) {

 /***************************************************************************
  * Find the instance data.                                                 *
  ***************************************************************************/

  PDATA Data = PDATA ( Sys_GetWindowData ( Window ) ) ;

 /***************************************************************************
  * Get the new tick-mark position from the message.                        *
  ***************************************************************************/

  LONG Tick = LONGFROMMP ( mp1 ) ;

 /***************************************************************************
  * If only the tick-mark has changed, move it.                             *
  ***************************************************************************/

  if ( Data->Tick != Tick ) {

   /*************************************************************************
    * Get the presentation space.                                           *
    *************************************************************************/

    WorkSpace PS ( "VRuler::SetTick", 0, Window, Data->pDevice, int(Data->Metric) ) ;
    PS.SetTransform ( Data->fxZoom, 0, int(Data->TopLeft-Data->WindowHeight) ) ;

   /*******v*****************************************************************
    * Find out some background information.                                 *
    *************************************************************************/

    RECTL Rectangle ;
    WinQueryWindowRect ( Window, &Rectangle ) ;
    PS.Transform ( CVTC_DEVICE, CVTC_DEFAULTPAGE, Rectangle ) ;

   /*******************************************************v*****************
    * Erase the previous hairline.                                          *
    *************************************************************************/

    if ( Data->Tick ) {
      PS.SetMix ( FM_INVERT ) ;
      PS.SetLineType ( LINETYPE_ALTERNATE ) ;

      POINTL Point ;
      Point.x = Rectangle.xLeft ;
      Point.y = Data->Tick ;
      PS.Move ( Point ) ;
      Point.x = Rectangle.xRight ;
      PS.DrawLine ( Point ) ;
    }

   /*************************************************************************
    * Save the current hairline position.                                   *
    *************************************************************************/

    Data->Tick = Tick ;

   /*************************************************************************
    * Draw a hairline showing the current position.                         *
    *************************************************************************/

    if ( Data->Tick ) {
      PS.SetMix ( FM_INVERT ) ;
      PS.SetLineType ( LINETYPE_ALTERNATE ) ;

      POINTL Point ;
      Point.x = Rectangle.xLeft ;
      Point.y = Data->Tick ;
      PS.Move ( Point ) ;
      Point.x = Rectangle.xRight ;
      PS.DrawLine ( Point ) ;
    }
  }

 /***************************************************************************
  * We're done.                                                             *
  ***************************************************************************/

  return ( MRFROMSHORT ( 0 ) ) ;
}
