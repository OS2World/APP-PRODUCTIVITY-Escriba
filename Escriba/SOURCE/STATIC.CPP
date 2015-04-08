/***************************************************************** STATIC.CPP
 *                                                                          *
 *                         Static Window Class                              *
 *                                                                          *
 ****************************************************************************/

#include "System.h"

#include <math.h>
#include <process.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "Debug.h"
#include "Escriba.h"
#include "Static.h"
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

#define MAX_TEXT 200


  // Types

typedef struct {
   INIDATA *IniData ;
   HWND     MainWindow ;
   int      CodePage ;
   DeviceContext *pDevice ;
   char     Text [MAX_TEXT] ;
   BOOL     SettingFont ;
} DATA, *PDATA ;


  // Function Prototypes

static WINDOWPROC Create ;
static WINDOWPROC Destroy ;
static WINDOWPROC Paint ;
static WINDOWPROC SetWindowParams ;
static WINDOWPROC PresParamChanged ;
static WINDOWPROC ButtonDoubleClick ;
static WINDOWPROC SetCodePage ;

 
/****************************************************************************
 *                                                                          *
 *      Window Message Processor                                            *
 *                                                                          *
 ****************************************************************************/

extern MRESULT EXPENTRY StaticMessageProcessor ( HWND Window, MESG msg, MPARAM1 mp1, MPARAM2 mp2 ) {

  /**************************************************************************
   * Dispatch the message according to the method table and return the      *
   *   result.  Any messages not defined above get handled by the system    *
   *   default window processor.                                            *
   **************************************************************************/

   static METHOD Methods [] = {
      { WM_CREATE,                Create              },
      { WM_DESTROY,               Destroy             },
      { WM_PAINT,                 Paint               },
      { WM_SETWINDOWPARAMS,       SetWindowParams     },
      { WM_PRESPARAMCHANGED,      PresParamChanged    },
      { WM_BUTTON1DBLCLK,         ButtonDoubleClick   },
      { WM_SET_CODEPAGE,          SetCodePage         },
   } ;

   #ifdef DEBUG_MESSAGES
      static int Indent = 0 ;
      LogMsgRcvd ( Indent, "STATIC", Window, msg, mp1, mp2 ) ;
      Indent += 2 ;
   #endif // DEBUG_MESSAGES

   MRESULT Result = DispatchMessage ( Window, msg, mp1, mp2, Methods, sizeof(Methods)/sizeof(Methods[0]), Sys_DefWindowProc ) ;

   #ifdef DEBUG_MESSAGES
      Indent -= 2 ;
      LogMsgDone ( Indent, "STATIC", msg, Result ) ;
   #endif // DEBUG_MESSAGES

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

   if ( Data == NULL )
     return ( MRFROMSHORT ( 1 ) ) ;

   Sys_SetWindowData ( Window, Data ) ;

  /**************************************************************************
   * Grab any parameters from the WM_CREATE message.                        *
   **************************************************************************/

   PSTATIC_PARMS Parms = (PSTATIC_PARMS) PVOIDFROMMP ( mp1 ) ;

   Data->IniData = Parms->IniData ;
   Data->MainWindow = Parms->MainWindow ;
   Data->CodePage = Parms->CodePage ;

  /**************************************************************************
   * Create the device context object for the window.                       *
   **************************************************************************/

   WinOpenWindowDC ( Window ) ;
   Data->pDevice = new DeviceContext ( "StaticWindow", Window ) ;

  /**************************************************************************
   * Initialize the instance data.                                          *
   **************************************************************************/

   Data->Text [0] = 0 ;
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
   * Satisfy the window manager by beginning and ending paint.              *
   **************************************************************************/

   RECTL Rectangle ;
   WorkSpace PS ( "Static::Paint", Data->CodePage, Window, Data->pDevice, Rectangle ) ;

  /**************************************************************************
   * Get the colors we will need.                                           *
   **************************************************************************/

   COLOR Light = WinQuerySysColor ( HWND_DESKTOP, SYSCLR_BUTTONLIGHT, 0 ) ;
   COLOR Middle = WinQuerySysColor ( HWND_DESKTOP, SYSCLR_BUTTONMIDDLE, 0 ) ;
   COLOR Dark = WinQuerySysColor ( HWND_DESKTOP, SYSCLR_BUTTONDARK, 0 ) ;
   COLOR TextColor = WinQuerySysColor ( HWND_DESKTOP, SYSCLR_OUTPUTTEXT, 0L ) ;

  /**************************************************************************
   * Clear the window and draw a border.                                    *
   **************************************************************************/

   WinQueryWindowRect ( Window, &Rectangle ) ;

   RECTL MiddleBox = { Rectangle.xLeft+1, Rectangle.yBottom+1, Rectangle.xRight-2, Rectangle.yTop-2 } ;
   PS.SetColor ( Middle ) ;
   PS.FillBox ( MiddleBox ) ;

   PS.SetColor ( Dark ) ;

   POINTL Point ;
   Point.x = Rectangle.xLeft ;
   Point.y = Rectangle.yBottom ;
   PS.Move ( Point ) ;

   Point.y = Rectangle.yTop - 1 ;
   PS.DrawLine ( Point ) ;

   Point.x = Rectangle.xRight - 1 ;
   PS.DrawLine ( Point ) ;

   PS.SetColor ( Light ) ;

   Point.x = Rectangle.xLeft ;
   Point.y = Rectangle.yBottom ;
   PS.Move ( Point ) ;

   Point.x = Rectangle.xRight - 1 ;
   PS.DrawLine ( Point ) ;

   Point.y = Rectangle.yTop - 1 ;
   PS.DrawLine ( Point ) ;

  /**************************************************************************
   * Paint the window text.                                                 *
   **************************************************************************/

   if ( Data->Text [0] ) {
      PS.SetColor ( TextColor ) ;
      PS.GetFontMetrics ( ) ;
      int y = int ( ( Rectangle.yTop - Rectangle.yBottom - ( PS.QueryAscent() + PS.QueryDescent() ) ) / 2 + PS.QueryDescent() ) ;
      PS.Move ( 2, y ) ;
      PS.DrawText ( PCHAR(Data->Text) ) ;
   } /* endif */

// PS.DrawText ( PCHAR(Data->Text), strlen(PCHAR(Data->Text)), MiddleBox, TextColor, Middle, DT_LEFT | DT_VCENTER | DT_WORDBREAK ) ;

  /**************************************************************************
   * We're done.                                                            *
   **************************************************************************/

   return ( MRFROMSHORT ( 0 ) ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Set Window Params                                                   *
 *                                                                          *
 ****************************************************************************/

static MRESULT APIENTRY SetWindowParams ( HWND Window, MESG, MPARAM1 mp1, MPARAM2 ) {

  /**************************************************************************
   * Find the instance data.                                                *
   **************************************************************************/

   PDATA Data = PDATA ( Sys_GetWindowData ( Window ) ) ;

  /**************************************************************************
   * If this message does not have to do with window text, return FALSE.    *
   **************************************************************************/

   PWNDPARAMS WindowParams = PWNDPARAMS ( mp1 ) ;

   if ( ! ( WindowParams->fsStatus & WPM_TEXT ) )
      return ( MRFROMSHORT ( FALSE ) ) ;

  /**************************************************************************
   * Save the new window text.                                              *
   **************************************************************************/

   memcpy ( Data->Text, WindowParams->pszText, size_t(WindowParams->cchText) ) ;
   Data->Text[WindowParams->cchText] = 0 ;

  /**************************************************************************
   * Invalidate the window so it gets repainted.                            *
   **************************************************************************/

   Sys_InvalidateWindow ( Window ) ;

  /**************************************************************************
   * We're done.  Return whatever the default procedure did.                *
   **************************************************************************/

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

   } /* endswitch */

  /**************************************************************************
   * Return through the default processor, letting window activation        *
   *   and other system functions occur.                                    *
   **************************************************************************/

   return ( Sys_DefWindowProc ( Window, msg, mp1, mp2 ) ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Button Double Click                                                 *
 *                                                                          *
 ****************************************************************************/

static MRESULT APIENTRY ButtonDoubleClick ( HWND Window, MESG, MPARAM1, MPARAM2 ) {

  /**************************************************************************
   * Send WM_COMMAND message to owner.                                      *
   **************************************************************************/

   USHORT Id = WinQueryWindowUShort ( Window, QWS_ID ) ;

   Sys_SendMessage ( OWNER(Window), WM_COMMAND, MPFROMSHORT(Id), MPFROM2SHORT(CMDSRC_OTHER,TRUE) ) ;

  /**************************************************************************
   * We're done.  Return whatever the default procedure did.                *
   **************************************************************************/

   return ( MRFROMSHORT ( TRUE ) ) ;
}

/****************************************************************************
 *                                                                          *
 *      Set Code-Page                                                       *
 *                                                                          *
 ****************************************************************************/

static MRESULT APIENTRY SetCodePage ( HWND Window, MESG, MPARAM1 mp1, MPARAM2 ) {

  /**************************************************************************
   * Find the instance data.                                                *
   **************************************************************************/

   PDATA Data = PDATA ( Sys_GetWindowData ( Window ) ) ;

  /**************************************************************************
   * If the codepage hasn't changed, just return.                           *
   **************************************************************************/

   #ifdef DEBUG
      Log ( "Static::SetCodePage: Setting codepage to %i.", LONGFROMMP(mp1) ) ;
   #endif // DEBUG

   if ( LONGFROMMP(mp1) == Data->CodePage ) 
      return ( 0 ) ;

  /**************************************************************************
   * Save the new codepage value.                                           *
   **************************************************************************/

   Data->CodePage = int ( LONGFROMMP ( mp1 ) ) ;

  /**************************************************************************
   * Invalidate the window so it gets repainted.                            *
   **************************************************************************/

   Sys_InvalidateWindow ( Window ) ;

  /**************************************************************************
   * We're done.  Return whatever the default procedure did.                *
   **************************************************************************/

   return ( 0 ) ;
}
 

