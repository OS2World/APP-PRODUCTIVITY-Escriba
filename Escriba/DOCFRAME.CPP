/*************************************************************** DOCFRAME.CPP
 *                                                                          *
 *                       New Document Frame Window                          *
 *                                                                          *
 ****************************************************************************/

#include "System.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Debug.h"

#include "Controls.h"
#include "DocFrame.h"
#include "DocWin.h"
#include "Document.h"
#include "Escriba.h"
#include "HRuler.h"
#include "ReString.h"
#include "Static.h"
#include "Support.h"
#include "VRuler.h"
#include "Window.h"

// #define DEBUG
// #define DEBUG_MESSAGES
 
/****************************************************************************
 *                                                                          *
 *  Class: Button (with Bubble Help)                                        *
 *                                                                          *
 ****************************************************************************/

class Button : public SubclassedWindow { 

   private:
      char *BubbleText ;
      Window *BubbleFrame ;
      StaticText *Bubble ;
      int Surfaced ;

   public:
      Button ( HWND handle, char *bubbletext ) ;
      virtual ~Button ( ) ;
      virtual MRESULT MessageProcessor ( HWND hwnd, MESG msg, MPARAM1 mp1, MPARAM2 mp2 ) ;

      void SetText ( char *bubbletext ) {
         delete [] BubbleText ;
         BubbleText = new char [strlen(bubbletext)+1] ;
         strcpy ( BubbleText, bubbletext ) ;
         Bubble->SetText ( BubbleText ) ;
      } /* endmethod */
} ;

Button::Button ( HWND handle, char *bubbletext ) : SubclassedWindow(handle), BubbleFrame(0), Bubble(0), Surfaced(FALSE) {
   BubbleText = new char [strlen(bubbletext)+1] ;
   strcpy ( BubbleText, bubbletext ) ;
   ULONG Style = FCF_BORDER ; HWND hwndBubbleClient ( 0 ) ;
   BubbleFrame = new Window ( WinCreateStdWindow ( HWND_DESKTOP, 0, &Style, WC_STATIC, "", SS_TEXT | DT_LEFT | DT_VCENTER, 0, 0, &hwndBubbleClient ) ) ;
   Bubble = new StaticText ( hwndBubbleClient ) ;
   Bubble->SetText ( BubbleText ) ;
   ResourceString Font ( LibraryHandle, IDS_BUBBLE_FONT ) ;
   Bubble->SetPresParam ( PP_FONTNAMESIZE, strlen((char*)Font)+1, (char*)Font ) ;
   COLOR Yellow ( 0xFFFF00 ) ; Bubble->SetPresParam ( PP_BACKGROUNDCOLOR, 4, &Yellow ) ;
} /* endmethod */

Button::~Button ( ) { 
   BubbleFrame->Destroy ( ) ;
   delete Bubble ;
   delete BubbleFrame ;
   delete [] BubbleText ;
} /* endmethod */

MRESULT Button::MessageProcessor ( HWND hwnd, MESG msg, MPARAM1 mp1, MPARAM2 mp2 ) {
   switch ( msg ) {
      case WM_MOUSEMOVE: {
         if ( !Surfaced ) {
            WorkSpace PS ( "Button::MessageProcessor::Create", 0, hwnd, 0 ) ;
            SWP Pos ; WinQueryWindowPos ( hwnd, &Pos ) ;
            RECTL Desktop ; WinQueryWindowRect ( HWND_DESKTOP, &Desktop ) ;
            SIZEL Size ; PS.MeasureText ( BubbleText, Size ) ;
            BubbleFrame->Size ( Size.cx+4, Size.cy+4 ) ;
            POINTL Point = { 0, -(Size.cy+6) } ;
            WinMapWindowPoints ( hwnd, HWND_DESKTOP, &Point, 1 ) ;
            if ( Point.y < 0 ) {
               Point.x = 0 ; 
               Point.y = Pos.cy + 2 ;
               WinMapWindowPoints ( hwnd, HWND_DESKTOP, &Point, 1 ) ;
            } /* endif */
            if ( Point.x + Size.cx + 4 >= Desktop.xRight ) 
               Point.x = Desktop.xRight - Size.cx - 4 ;
            if ( Point.x < 0 ) 
               Point.x = 0 ;
            BubbleFrame->Move ( Point.x, Point.y ) ;
            BubbleFrame->Show ( ) ;
            BubbleFrame->Surface ( ) ;
            Surfaced = TRUE ;
            Window ThisWindow ( hwnd ) ;
            ThisWindow.StartTimer ( 1, 50 ) ;
         } /* endif */
         break; }
      case WM_TIMER: {
         if ( SHORT1FROMMP(mp1) == 1 ) {
            SWP Position ; WinQueryWindowPos ( hwnd, &Position ) ;
            POINTL Point ; WinQueryPointerPos ( HWND_DESKTOP, &Point ) ;
            WinMapWindowPoints ( HWND_DESKTOP, PARENT(hwnd), &Point, 1 ) ;
            if ( ( Position.x > Point.x ) || ( Position.x+Position.cx < Point.x ) || ( Position.y > Point.y ) || ( Position.y+Position.cy < Point.y ) ) {
               BubbleFrame->Hide ( ) ;
               Window ThisWindow ( hwnd ) ;
               ThisWindow.StopTimer ( 1 ) ;
               Surfaced = FALSE ;
            } /* endif */
         } /* endif */
         break; }
   } /* endswitch */
   return ( SubclassedWindow::MessageProcessor ( hwnd, msg, mp1, mp2 ) ) ;
} /* endmethod */

/****************************************************************************
 *                                                                          *
 *                       Definitions & Declarations                         *
 *                                                                          *
 ****************************************************************************/

  // Macros

#define STATIC static


  // Types

typedef struct {
  PWINDOWPROC       FrameProcessor ;
  HelpWindow *Help ;
  INIDATA    *IniData ;
  LanguageModule *Library ;
  HWND        MainWindow ;
  DeviceContext *pDevice ;
  Documento  *pDocument ;
  Dictionary *pSpeller ;
  USHORT      WindowID ;
  BOOL        Trap ;
  Button     *Top, *PageUp, *PageDown, *Bottom ;

} DOCFRAMEDATA, *PDOCFRAMEDATA ;


  // Function Prototypes

STATIC WINDOWPROC Create ;
STATIC WINDOWPROC Destroy ;
STATIC WINDOWPROC QueryFrameCtlCount ;
STATIC WINDOWPROC FormatFrame ;
STATIC WINDOWPROC InitMenu ;
STATIC WINDOWPROC DragOver ;
STATIC WINDOWPROC DragLeave ;
STATIC WINDOWPROC AlterFrame ;
STATIC WINDOWPROC Set_Language ;

 
/****************************************************************************
 *                                                                          *
 *      Window Message Processor                                            *
 *                                                                          *
 ****************************************************************************/

extern MRESULT EXPENTRY DocFrameMessageProcessor ( HWND Window, MESG msg, MPARAM1 mp1, MPARAM2 mp2 ) {

  /**************************************************************************
   * Determine the address of the original message processor.               *
   **************************************************************************/

   PWINDOWPROC FrameProcessor = 0 ;
   if ( msg != WM_CREATE ) {
      PDOCFRAMEDATA Data = PDOCFRAMEDATA ( Sys_GetWindowData ( Window ) ) ;
      FrameProcessor = Data->FrameProcessor ;
   } /* endif */

  /**************************************************************************
   * Dispatch the message according to the method table and return the      *
   *   result.  Any messages not defined above get handled by the system    *
   *   default window processor.                                            *
   **************************************************************************/

   static METHOD Methods [] = {
      { WM_CREATE,                Create              },  // Required.
      { WM_DESTROY,               Destroy             },  // Required.
      { WM_QUERYFRAMECTLCOUNT,    QueryFrameCtlCount  },
      { WM_FORMATFRAME,           FormatFrame         },
      { WM_INITMENU,              InitMenu            },
      { DM_DRAGOVER,              DragOver            },
      { DM_DRAGLEAVE,             DragLeave           },
      { WM_ALTER_FRAME,           AlterFrame          },
      { WM_SET_LANGUAGE,          Set_Language        },
   } ;

   #ifdef DEBUG_MESSAGES
      static int Indent = 0 ;
      LogMsgRcvd ( Indent, "DOCFRAME", msg, mp1, mp2 ) ;
      Indent += 2 ;
   #endif

   MRESULT Result = DispatchMessage ( Window, msg, mp1, mp2, Methods, sizeof(Methods)/sizeof(Methods[0]), FrameProcessor ) ;

   #ifdef DEBUG_MESSAGES
      Indent -= 2 ;
      LogMsgDone ( Indent, "DOCFRAME", msg, Result ) ;
   #endif

   return ( Result ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Create the frame window.                                            *
 *                                                                          *
 ****************************************************************************/

STATIC MRESULT APIENTRY Create ( HWND Window, ULONG, MPARAM1 mp1, MPARAM2 mp2 ) {

 /***************************************************************************
  * Allocate instance data.                                                 *
  ***************************************************************************/

  PDOCFRAMEDATA Data = new DOCFRAMEDATA ;

  if ( Data == NULL ) {
     Debug ( Window, "ERROR(DOCFRAME): Unable to allocate instance memory." ) ;
     return ( MRFROMSHORT ( TRUE ) ) ;
  } /* endif */

  Sys_SetWindowData ( Window, Data ) ;

 /***************************************************************************
  * Grab any parameters from the WM_CREATE message.                         *
  ***************************************************************************/

  PDOCFRAME_PARMS Parms = PDOCFRAME_PARMS ( mp1 ) ;

  Data->Help         = Parms->Help ;
  Data->IniData      = Parms->IniData ;
  Data->Library      = Parms->Library ;
  Data->MainWindow   = Parms->MainWindow ;
  Data->pDocument    = Parms->pDocument ;
  Data->pSpeller     = Parms->pSpeller ;
  Data->WindowID     = Parms->WindowID ;
  Data->Trap         = Parms->Trap ;
  Data->Top          = 0 ;
  Data->PageUp       = 0 ;
  Data->PageDown     = 0 ;
  Data->Bottom       = 0 ;

 /***************************************************************************
  * Create the device context object for the window.                        *
  ***************************************************************************/

  WinOpenWindowDC ( Window ) ;
  Data->pDevice = new DeviceContext ( "DocFrame", Window ) ;

 /***************************************************************************
  * Get the program title.                                                  *
  ***************************************************************************/

  ResourceString Title ( Data->Library->QueryHandle(), IDS_TITLE ) ;

 /***************************************************************************
  * Register the horizontal ruler class.                                    *
  ***************************************************************************/

  if ( !WinRegisterClass ( WinQueryAnchorBlock(Window), PSZ(HORZRULER_CLASS_NAME),
     PWINDOWPROC(HorzRulerMessageProcessor), 0, sizeof(PVOID) ) ) {

     ResourceString Format ( Data->Library->QueryHandle(), IDS_ERROR_WINREGISTERCLASS ) ;
     UCHAR Message [200] ;
     sprintf ( PCHAR(Message), PCHAR(Format), HORZRULER_CLASS_NAME ) ;
     MessageBox ( HWND_DESKTOP, HWND_DESKTOP, PSZ(Message), PSZ(Title),
        0, MB_ENTER | MB_ICONEXCLAMATION, Data->Library->QueryHandle(), IDS_MESSAGE_BOX_STRINGS ) ;
     return ( MRFROMSHORT ( 1 ) ) ;
  } /* endif */

 /***************************************************************************
  * Register the vertical ruler class.                                      *
  ***************************************************************************/

  if ( !WinRegisterClass ( WinQueryAnchorBlock(Window), PSZ(VERTRULER_CLASS_NAME),
     PWINDOWPROC(VertRulerMessageProcessor), 0, sizeof(PVOID) ) ) {

     ResourceString Format ( Data->Library->QueryHandle(), IDS_ERROR_WINREGISTERCLASS ) ;
     UCHAR Message [200] ;
     sprintf ( PCHAR(Message), PCHAR(Format), VERTRULER_CLASS_NAME ) ;
     MessageBox ( HWND_DESKTOP, HWND_DESKTOP, PSZ(Message), PSZ(Title),
        0, MB_ENTER | MB_ICONEXCLAMATION, Data->Library->QueryHandle(), IDS_MESSAGE_BOX_STRINGS ) ;
     return ( MRFROMSHORT ( 1 ) ) ;
  } /* endif */

 /***************************************************************************
  * Register the static window class.                                       *
  ***************************************************************************/

  if ( !WinRegisterClass ( WinQueryAnchorBlock(Window), PSZ(STATIC_CLASS_NAME),
     PWINDOWPROC(StaticMessageProcessor), 0, sizeof(PVOID) ) ) {

     ResourceString Format ( Data->Library->QueryHandle(), IDS_ERROR_WINREGISTERCLASS ) ;
     UCHAR Message [200] ;
     sprintf ( PCHAR(Message), PCHAR(Format), STATIC_CLASS_NAME ) ;
     MessageBox ( HWND_DESKTOP, HWND_DESKTOP, PSZ(Message), PSZ(Title),
        0, MB_ENTER | MB_ICONEXCLAMATION, Data->Library->QueryHandle(), IDS_MESSAGE_BOX_STRINGS ) ;
     return ( MRFROMSHORT ( 1 ) ) ;
  } /* endif */

 /***************************************************************************
  * Find the system frame window processor.                                 *
  ***************************************************************************/

  CLASSINFO ClassInfo ;
  if ( WinQueryClassInfo ( WinQueryAnchorBlock(Window), WC_FRAME, &ClassInfo ) == FALSE ) {
     Debug ( Window, "ERROR(DOCFRAME): Unable to find frame processor." ) ;
     return ( MRFROMSHORT ( TRUE ) ) ;
  } /* endif */

  Data->FrameProcessor = ClassInfo.pfnWindowProc ;

 /***************************************************************************
  * Build the frame control data.                                           *
  ***************************************************************************/

  FRAMECDATA FrameControlData ;
  FrameControlData.cb = sizeof(FrameControlData) ;
  FrameControlData.flCreateFlags = FCF_TITLEBAR | FCF_SIZEBORDER | FCF_MINMAX | FCF_NOBYTEALIGN |
     FCF_SYSMENU | FCF_ICON | FCF_ACCELTABLE | FCF_VERTSCROLL | FCF_HORZSCROLL | FCF_DBE_APPSTAT ;
  FrameControlData.hmodResources = USHORT ( Data->Library->QueryHandle() ) ;
  FrameControlData.idResources = ID_DATAWINDOW ;

 /***************************************************************************
  * Build the window create structure.  Adjust the window class.            *
  ***************************************************************************/

  CREATESTRUCT CreateStructure ;
  CreateStructure = *((PCREATESTRUCT)mp2) ;
  CreateStructure.pszClass = WC_FRAME ;

 /***************************************************************************
  * Create the frame window.  Return if error.                              *
  ***************************************************************************/

  if ( Data->FrameProcessor ( Window, WM_CREATE,
     MPFROMP(&FrameControlData), MPFROMP(&CreateStructure) ) ) {
     Debug ( Window, "ERROR(DOCFRAME): Frame create failed." ) ;
     return ( MRFROMSHORT ( TRUE ) ) ;
  } /* endif */

 /***************************************************************************
  * Create the horizontal ruler.  Must be ready before Client.              *
  ***************************************************************************/

  HORZRULER_PARMS HorzRulerParms ;
  HorzRulerParms.IniData = Data->IniData ;
  HorzRulerParms.MainWindow = Data->MainWindow ;

  ResourceString DefaultFont ( Data->Library->QueryHandle(), IDS_DEFAULTFONT ) ;
  UCHAR *Font = Data->IniData->fRulerFont ? Data->IniData->RulerFont : PUCHAR(DefaultFont) ;
  COLOR FgndColor = Data->IniData->fRulerColors[0] ? Data->IniData->RulerColors[0] : RGB_WHITE ;
  COLOR BackColor = Data->IniData->fRulerColors[1] ? Data->IniData->RulerColors[1] : RGB_BLUE ;
  ULONG Ids[] = { PP_FONTNAMESIZE, PP_FOREGROUNDCOLOR, PP_BACKGROUNDCOLOR } ;
  ULONG ByteCounts[] = { strlen(PCHAR(Font))+1, sizeof(FgndColor), sizeof(BackColor) } ;
  PUCHAR Params[] = { Font, PUCHAR(&FgndColor), PUCHAR(&BackColor) } ;

  PPRESPARAMS PresParams = BuildPresParams ( sizeof(Ids)/sizeof(Ids[0]),
    Ids, ByteCounts, Params ) ;

  HWND HRuler = WinCreateWindow ( Window, PSZ(HORZRULER_CLASS_NAME), PSZ(""),
     WS_VISIBLE | WS_CLIPCHILDREN,
     0, 0, 0, 0, Window, HWND_BOTTOM, FID_HORZRULER, &HorzRulerParms, PresParams ) ;

 /***************************************************************************
  * Create the vertical ruler.  Must be ready before Client.                *
  ***************************************************************************/

  VERTRULER_PARMS VertRulerParms ;
  VertRulerParms.IniData = Data->IniData ;
  VertRulerParms.MainWindow = Data->MainWindow ;

  HWND VRuler = WinCreateWindow ( Window, PSZ(VERTRULER_CLASS_NAME), PSZ(""),
     WS_VISIBLE | WS_CLIPCHILDREN,
     0, 0, 0, 0, Window, HWND_BOTTOM, FID_VERTRULER, &VertRulerParms, PresParams ) ;

  delete PresParams ;

 /***************************************************************************
  * Create the status bar.  Must be ready before Client.                    *
  ***************************************************************************/

  STATIC_PARMS StaticParms ;
  StaticParms.IniData = Data->IniData ;
  StaticParms.MainWindow = Data->MainWindow ;
  StaticParms.CodePage = Data->pDocument->Query_Parms()->CodePage ;

  PresParams = BuildPresParams ( 1, Ids, ByteCounts, Params ) ;
  HWND StatusBar = WinCreateWindow ( Window, PSZ(STATIC_CLASS_NAME), PSZ(""),
     WS_VISIBLE | WS_CLIPCHILDREN,
     0, 0, 0, 0, Window, HWND_BOTTOM, FID_STATUSLINE, &StaticParms, PresParams ) ;
  delete PresParams ;

 /***************************************************************************
  * Create the client window.                                               *
  ***************************************************************************/

  DOCWINDOW_PARMS WindowParms ;
  WindowParms.Filler     = 0 ;
  WindowParms.IniData    = Data->IniData ;
  WindowParms.Library    = Data->Library ;
  WindowParms.MainWindow = Data->MainWindow ;
  WindowParms.pDocument  = Data->pDocument ;
  WindowParms.pSpeller   = Data->pSpeller ;

  FgndColor = Data->IniData->fDocColors[0] ? Data->IniData->DocColors[0] : RGB_WHITE ;
  BackColor = Data->IniData->fDocColors[1] ? Data->IniData->DocColors[1] : RGB_BLUE ;
  PresParams = BuildPresParams ( sizeof(Ids)/sizeof(Ids[0]), Ids, ByteCounts, Params ) ;

  HWND Client = WinCreateWindow ( Window, PSZ(DOCWINDOW_CLASS_NAME), PSZ(""),
     0, 0, 0, 0, 0, Window, HWND_TOP, FID_CLIENT, &WindowParms, PresParams ) ;

  delete PresParams ;

  if ( Client == 0 ) {
     Debug ( Window, "ERROR(DOCFRAME): Frame client creation failed." ) ;
     return ( MRFROMSHORT ( TRUE ) ) ;
  } else {
     #ifdef DEBUG_MESSAGES
     Log ( "DOCFRAME::WM_CREATE: Client window created.  Handle %08p.", Client ) ;
     #endif
  } /* endif */

  WinSetOwner ( StatusBar, Client ) ;
  WinSetOwner ( HRuler, Client ) ;
  WinSetOwner ( VRuler, Client ) ;

 /***************************************************************************
  * Create buttons for paging with the mouse.                               *
  ***************************************************************************/

  ResourceString Top ( Data->Library->QueryHandle(), IDS_BUBBLE_TOP ) ;
  Data->Top = new Button ( WinCreateWindow ( Window, PSZ(BUTTON_CLASS), PSZ(""),
     WS_VISIBLE | WS_CLIPCHILDREN | BS_USERBUTTON | BS_NOPOINTERFOCUS,
     0, 0, 0, 0, Client, HWND_BOTTOM, FID_TOP, NULL, NULL ), (char*)Top ) ;

  ResourceString PageUp ( Data->Library->QueryHandle(), IDS_BUBBLE_PAGEUP ) ;
  Data->PageUp = new Button ( WinCreateWindow ( Window, PSZ(BUTTON_CLASS), PSZ(""),
     WS_VISIBLE | WS_CLIPCHILDREN | BS_USERBUTTON | BS_NOPOINTERFOCUS,
     0, 0, 0, 0, Client, HWND_BOTTOM, FID_PAGEUP, NULL, NULL ), (char*)PageUp ) ;

  ResourceString PageDown ( Data->Library->QueryHandle(), IDS_BUBBLE_PAGEDOWN ) ;
  Data->PageDown = new Button ( WinCreateWindow ( Window, PSZ(BUTTON_CLASS), PSZ(""),
     WS_VISIBLE | WS_CLIPCHILDREN | BS_USERBUTTON | BS_NOPOINTERFOCUS,
     0, 0, 0, 0, Client, HWND_BOTTOM, FID_PAGEDOWN, NULL, NULL ), (char*)PageDown ) ;

  ResourceString Bottom ( Data->Library->QueryHandle(), IDS_BUBBLE_BOTTOM ) ;
  Data->Bottom = new Button ( WinCreateWindow ( Window, PSZ(BUTTON_CLASS), PSZ(""),
     WS_VISIBLE | WS_CLIPCHILDREN | BS_USERBUTTON | BS_NOPOINTERFOCUS,
     0, 0, 0, 0, Client, HWND_BOTTOM, FID_BOTTOM, NULL, NULL ), (char*)Bottom ) ;

 /***************************************************************************
  * Create the static windows.                                              *
  ***************************************************************************/

  WinCreateWindow ( Window, PSZ(STATIC_CLASS_NAME), PSZ(""),
     WS_VISIBLE | WS_CLIPCHILDREN,
     0, 0, 0, 0, Client, HWND_BOTTOM, FID_CORNER1, &StaticParms, NULL ) ;

  WinCreateWindow ( Window, PSZ(STATIC_CLASS_NAME), PSZ(""),
     WS_VISIBLE | WS_CLIPCHILDREN,
     0, 0, 0, 0, Client, HWND_BOTTOM, FID_CORNER2, &StaticParms, NULL ) ;

  WinCreateWindow ( Window, PSZ(STATIC_CLASS_NAME), PSZ(""),
     WS_VISIBLE | WS_CLIPCHILDREN,
     0, 0, 0, 0, Client, HWND_BOTTOM, FID_CORNER3, &StaticParms, NULL ) ;

  WinCreateWindow ( Window, PSZ(STATIC_CLASS_NAME), PSZ(""),
     WS_VISIBLE | WS_CLIPCHILDREN,
     0, 0, 0, 0, Client, HWND_BOTTOM, FID_CORNER4, &StaticParms, NULL ) ;

 /***************************************************************************
  * Associate the help instance with the frame window.                      *
  ***************************************************************************/

  WinSetWindowUShort ( Window, QWS_ID, Data->WindowID ) ;

  if ( Data->Help ) 
     Data->Help->Associate ( Window ) ;

 /***************************************************************************
  * Done.  Return OK.                                                       *
  ***************************************************************************/

  return ( MRFROMSHORT ( FALSE ) ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Destroy the frame window.                                           *
 *                                                                          *
 ****************************************************************************/

STATIC MRESULT APIENTRY Destroy ( HWND Window, MESG msg, MPARAM1 mp1, MPARAM2 mp2 ) {

 /***************************************************************************
  * Find the instance data.                                                 *
  ***************************************************************************/

  PDOCFRAMEDATA Data = PDOCFRAMEDATA ( Sys_GetWindowData ( Window ) ) ;

 /***************************************************************************
  * Get the original processor's address.                                   *
  ***************************************************************************/

  PWINDOWPROC FrameProcessor = Data->FrameProcessor ;

 /***************************************************************************
  * Destroy the device context object for the window.                       *
  ***************************************************************************/

  delete Data->pDevice ;

 /***************************************************************************
  * Destroy the button objects.                                             *
  ***************************************************************************/

  if ( Data->Bottom ) delete Data->Bottom ;
  if ( Data->PageDown ) delete Data->PageDown ;
  if ( Data->PageUp ) delete Data->PageUp ;
  if ( Data->Top ) delete Data->Top ;

 /***************************************************************************
  * Release the instance memory.                                            *
  ***************************************************************************/

  delete Data ;

 /***************************************************************************
  * Return through the original processor.                                  *
  ***************************************************************************/

  return ( FrameProcessor ( Window, msg, mp1, mp2 ) ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Query Frame Control Count                                           *
 *                                                                          *
 ****************************************************************************/

STATIC MRESULT APIENTRY QueryFrameCtlCount ( HWND, MESG, MPARAM1, MPARAM2 ) {

 /***************************************************************************
  * Return the number of frame controls for this class.                     *
  ***************************************************************************/

  return ( MRFROMSHORT ( FID_COUNT ) ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Format the frame window.                                            *
 *                                                                          *
 ****************************************************************************/

STATIC MRESULT APIENTRY FormatFrame ( HWND Window, MESG msg, MPARAM1 mp1, MPARAM2 mp2 ) {

 /***************************************************************************
  * Local Declarations                                                      *
  ***************************************************************************/

  #define BUTTON_WIDTH  25
  #define RULER_HEIGHT  30
  #define RULER_WIDTH   25

 /***************************************************************************
  * Find the instance data.                                                 *
  ***************************************************************************/

  PDOCFRAMEDATA Data = PDOCFRAMEDATA ( Sys_GetWindowData ( Window ) ) ;

 /***************************************************************************
  * Let the default system frame processor do its thing first.              *
  ***************************************************************************/

  int ControlCount = int ( Data->FrameProcessor ( Window, msg, mp1, mp2 ) ) ;

 /***************************************************************************
  * Get the address of the positions array.                                 *
  ***************************************************************************/

  PSWP Positions = PSWP ( PVOIDFROMMP ( mp1 ) ) ;

 /***************************************************************************
  * Gather the information we'll be needing to reorganize the frame.        *
  ***************************************************************************/

  int iClient, iHorzScroll, iVertScroll, iVertRuler, iLowerLeft, iLowerRight, iStatus ;

  HWND Client     = CHILD ( Window, FID_CLIENT ) ;
  HWND HorzScroll = CHILD ( Window, FID_HORZSCROLL ) ;
  HWND VertScroll = CHILD ( Window, FID_VERTSCROLL ) ;

  for ( int i=0; i<ControlCount; i++ ) {
     if ( Positions[i].hwnd == Client ) {
        iClient = i ;
     } else if ( Positions[i].hwnd == HorzScroll ) {
        iHorzScroll = i ;
     } else if ( Positions[i].hwnd == VertScroll ) {
        iVertScroll = i ;
     } /* endif */
  } /* endfor */

  WorkSpace PS ( "DocFrame::UpdateFrame", 0, Window, Data->pDevice ) ;
  PS.GetFontMetrics ( ) ;
  LONG cyStatusLine = PS.QueryAscent() + PS.QueryDescent() + 6 ;

  SWP Position ;
  WinQueryWindowPos ( Window, &Position ) ;

 /***************************************************************************
  * Hide the scrollbars if not requested.                                   *
  ***************************************************************************/

  if ( NOT Data->IniData->ScrollBars ) {

     // Adjust the client window's size.
     Positions[iClient].y -= Positions[iHorzScroll].cy - 1 ;
     Positions[iClient].cy += Positions[iHorzScroll].cy - 1 ;
     Positions[iClient].cx += Positions[iVertScroll].cx - 1 ;

     // Hide the horizontal scroll bar.
     Positions[iHorzScroll].cy = 0 ;
     Positions[iHorzScroll].fl = SWP_HIDE ;

     // Hide the vertical scroll bar.
     Positions[iVertScroll].cx = 0 ;
     Positions[iVertScroll].fl = SWP_HIDE ;

  } /* endif */

 /***************************************************************************
  * Position & size the rulers.                                             *
  ***************************************************************************/

  if ( Data->IniData->Rulers ) {

     // Adjust the client window's size.
     Positions[iClient].cy -= RULER_HEIGHT ;
     Positions[iClient].x  += RULER_WIDTH ;
     Positions[iClient].cx -= RULER_WIDTH ;

     // Adjust the horizontal ruler.
     if ( Data->IniData->ScrollBars ) {
        Positions[iHorzScroll].x += RULER_WIDTH ;
        Positions[iHorzScroll].cx -= RULER_WIDTH ;
     } /* endif */

     // Adjust the vertical ruler.
     if ( Data->IniData->ScrollBars ) {
        Positions[iVertScroll].cy -= RULER_HEIGHT ;
     } /* endif */

     // Add the horizontal ruler to the window list.
     WinQueryWindowPos ( CHILD ( Window, FID_HORZRULER ), &Positions[ControlCount] ) ;
     Positions[ControlCount].cx = Positions[iClient].cx + 2 ;
     Positions[ControlCount].cy = RULER_HEIGHT ;
     Positions[ControlCount].x = Positions[iClient].x - 1 ;
     Positions[ControlCount].y = Positions[iClient].y + Positions[iClient].cy ;
     Positions[ControlCount].fl = SWP_MOVE | SWP_SIZE | SWP_NOADJUST |
       ( ( Position.fl & SWP_MINIMIZE ) ? SWP_HIDE : SWP_SHOW ) ;
     ControlCount ++ ;

     // Add the vertical ruler to the window list.
     WinQueryWindowPos ( CHILD ( Window, FID_VERTRULER ), &Positions[ControlCount] ) ;
     Positions[ControlCount].cx = RULER_WIDTH ;
     Positions[ControlCount].cy = Positions[iClient].cy + 2 ;
     Positions[ControlCount].x = Positions[iClient].x - RULER_WIDTH ;
     Positions[ControlCount].y = Positions[iClient].y - 1 ;
     Positions[ControlCount].fl = SWP_MOVE | SWP_SIZE | SWP_NOADJUST |
       ( ( Position.fl & SWP_MINIMIZE ) ? SWP_HIDE : SWP_SHOW ) ;
     iVertRuler = ControlCount ;
     ControlCount ++ ;

     // Add the upper-left corner to the window list.
     WinQueryWindowPos ( CHILD ( Window, FID_CORNER1 ), &Positions[ControlCount] ) ;
     Positions[ControlCount].cx = RULER_WIDTH - 1 ;
     Positions[ControlCount].cy = RULER_HEIGHT - 1 ;
     Positions[ControlCount].x = Positions[iClient].x - RULER_WIDTH ;
     Positions[ControlCount].y = Positions[iClient].y + Positions[iClient].cy + 1 ;
     Positions[ControlCount].fl = SWP_MOVE | SWP_SIZE | SWP_NOADJUST |
       ( ( NOT Data->IniData->Rulers OR NOT Data->IniData->ScrollBars OR ( Position.fl & SWP_MINIMIZE ) ) ? SWP_HIDE : SWP_SHOW ) ;
     ControlCount ++ ;

     // Add the lower-left corner to the window list.
     WinQueryWindowPos ( CHILD ( Window, FID_CORNER2 ), &Positions[ControlCount] ) ;
     Positions[ControlCount].cx = RULER_WIDTH ;
     Positions[ControlCount].cy = Positions[iHorzScroll].cy - 2 ;
     Positions[ControlCount].x = Positions[iHorzScroll].x - RULER_WIDTH ;
     Positions[ControlCount].y = Positions[iHorzScroll].y + 1 ;
     Positions[ControlCount].fl = SWP_MOVE | SWP_SIZE | SWP_NOADJUST |
       ( ( NOT Data->IniData->Rulers OR ( Position.fl & SWP_MINIMIZE ) ) ? SWP_HIDE : SWP_SHOW ) ;
     iLowerLeft = ControlCount ;
     ControlCount ++ ;

     // Add the upper-right corner to the window list, if scroll bars are active.
     WinQueryWindowPos ( CHILD ( Window, FID_CORNER3 ), &Positions[ControlCount] ) ;
     if ( Data->IniData->ScrollBars ) {
        Positions[ControlCount].cx = Positions[iVertScroll].cx - 2 ;
        Positions[ControlCount].cy = RULER_HEIGHT - 1 ;
        Positions[ControlCount].x = Positions[iVertScroll].x + 1 ;
        Positions[ControlCount].y = Positions[iVertScroll].y + Positions[iVertScroll].cy ;
        Positions[ControlCount].fl = SWP_MOVE | SWP_SIZE | SWP_NOADJUST |
          ( ( NOT Data->IniData->Rulers OR NOT Data->IniData->ScrollBars OR ( Position.fl & SWP_MINIMIZE ) ) ? SWP_HIDE : SWP_SHOW ) ;
     } else {
        Positions[ControlCount].cx = 0 ;
        Positions[ControlCount].fl = SWP_HIDE ;
     } /* endif */
     ControlCount ++ ;

     // Add the lower-right corner to the window list, if scroll bars are active.
     WinQueryWindowPos ( CHILD ( Window, FID_CORNER4 ), &Positions[ControlCount] ) ;
     if ( Data->IniData->ScrollBars ) {
        Positions[ControlCount].cx = Positions[iVertScroll].cx - 2 ;
        Positions[ControlCount].cy = Positions[iHorzScroll].cy - 2 ;
        Positions[ControlCount].x = Positions[iVertScroll].x + 1 ;
        Positions[ControlCount].y = Positions[iHorzScroll].y + 1 ;
        Positions[ControlCount].fl = SWP_MOVE | SWP_SIZE | SWP_NOADJUST |
          ( ( NOT Data->IniData->Rulers OR NOT Data->IniData->ScrollBars OR ( Position.fl & SWP_MINIMIZE ) ) ? SWP_HIDE : SWP_SHOW ) ;
     } else {
        Positions[ControlCount].cx = 0 ;
        Positions[ControlCount].fl = SWP_HIDE ;
     } /* endif */
     iLowerRight = ControlCount ;
     ControlCount ++ ;

  } else {

     WinQueryWindowPos ( CHILD ( Window, FID_HORZRULER ), &Positions[ControlCount] ) ;
     Positions[ControlCount].cx = 0 ;
     Positions[ControlCount].fl = SWP_HIDE ;
     ControlCount ++ ;

     WinQueryWindowPos ( CHILD ( Window, FID_VERTRULER ), &Positions[ControlCount] ) ;
     Positions[ControlCount].cx = 0 ;
     Positions[ControlCount].fl = SWP_HIDE ;
     iVertRuler = ControlCount ;
     ControlCount ++ ;

     WinQueryWindowPos ( CHILD ( Window, FID_CORNER1 ), &Positions[ControlCount] ) ;
     Positions[ControlCount].cx = 0 ;
     Positions[ControlCount].fl = SWP_HIDE ;
     ControlCount ++ ;

     WinQueryWindowPos ( CHILD ( Window, FID_CORNER2 ), &Positions[ControlCount] ) ;
     Positions[ControlCount].cx = 0 ;
     Positions[ControlCount].fl = SWP_HIDE ;
     iLowerLeft = ControlCount ;
     ControlCount ++ ;

     WinQueryWindowPos ( CHILD ( Window, FID_CORNER3 ), &Positions[ControlCount] ) ;
     Positions[ControlCount].cx = 0 ;
     Positions[ControlCount].fl = SWP_HIDE ;
     ControlCount ++ ;

     WinQueryWindowPos ( CHILD ( Window, FID_CORNER4 ), &Positions[ControlCount] ) ;
     Positions[ControlCount].cx = 0 ;
     Positions[ControlCount].fl = SWP_HIDE ;
     iLowerRight = ControlCount ;
     ControlCount ++ ;

  } /* endif */

 /***************************************************************************
  * Position & size the status line and it's companion buttons.             *
  ***************************************************************************/

  if ( Data->IniData->StatusLine ) {

     // Position the status bar.
     WinQueryWindowPos ( CHILD ( Window, FID_STATUSLINE ), &Positions[ControlCount] ) ;
     Positions[ControlCount].cx = Positions[iClient].cx + ( Data->IniData->Rulers ? RULER_WIDTH : 0 ) + ( Data->IniData->ScrollBars ? Positions[iVertScroll].cx-1 : 0 ) - BUTTON_WIDTH * 4 ;
     Positions[ControlCount].cy = cyStatusLine ;
     Positions[ControlCount].x = Positions[iClient].x - ( Data->IniData->Rulers ? RULER_WIDTH : 0 ) ;
     Positions[ControlCount].y = Positions[iClient].y - ( Data->IniData->ScrollBars ? Positions[iHorzScroll].cy-1 : 0 ) ;
     Positions[ControlCount].fl = SWP_MOVE | SWP_SIZE | SWP_NOADJUST |
       ( ( Position.fl & SWP_MINIMIZE ) ? SWP_HIDE : SWP_SHOW ) ;
     iStatus = ControlCount ;
     ControlCount ++ ;

     // Position the top-of-document button.
     WinQueryWindowPos ( CHILD ( Window, FID_TOP ), &Positions[ControlCount] ) ;
     Positions[ControlCount].cx = BUTTON_WIDTH ;
     Positions[ControlCount].cy = cyStatusLine ;
     Positions[ControlCount].x = Positions[iStatus].x + Positions[iStatus].cx + BUTTON_WIDTH * 0 ;
     Positions[ControlCount].y = Positions[iClient].y - ( Data->IniData->ScrollBars ? Positions[iHorzScroll].cy-1 : 0 ) ;
     Positions[ControlCount].fl = SWP_MOVE | SWP_SIZE | SWP_NOADJUST |
       ( ( Position.fl & SWP_MINIMIZE ) ? SWP_HIDE : SWP_SHOW ) ;
     ControlCount ++ ;

     // Position the page-up button.
     WinQueryWindowPos ( CHILD ( Window, FID_PAGEUP ), &Positions[ControlCount] ) ;
     Positions[ControlCount].cx = BUTTON_WIDTH ;
     Positions[ControlCount].cy = cyStatusLine ;
     Positions[ControlCount].x = Positions[iStatus].x + Positions[iStatus].cx + BUTTON_WIDTH * 1 ;
     Positions[ControlCount].y = Positions[iClient].y - ( Data->IniData->ScrollBars ? Positions[iHorzScroll].cy-1 : 0 ) ;
     Positions[ControlCount].fl = SWP_MOVE | SWP_SIZE | SWP_NOADJUST |
       ( ( Position.fl & SWP_MINIMIZE ) ? SWP_HIDE : SWP_SHOW ) ;
     ControlCount ++ ;

     // Position the page-down button.
     WinQueryWindowPos ( CHILD ( Window, FID_PAGEDOWN ), &Positions[ControlCount] ) ;
     Positions[ControlCount].cx = BUTTON_WIDTH ;
     Positions[ControlCount].cy = cyStatusLine ;
     Positions[ControlCount].x = Positions[iStatus].x + Positions[iStatus].cx + BUTTON_WIDTH * 2 ;
     Positions[ControlCount].y = Positions[iClient].y - ( Data->IniData->ScrollBars ? Positions[iHorzScroll].cy-1 : 0 ) ;
     Positions[ControlCount].fl = SWP_MOVE | SWP_SIZE | SWP_NOADJUST |
       ( ( Position.fl & SWP_MINIMIZE ) ? SWP_HIDE : SWP_SHOW ) ;
     ControlCount ++ ;

     // Position the bottom-of-document button.
     WinQueryWindowPos ( CHILD ( Window, FID_BOTTOM ), &Positions[ControlCount] ) ;
     Positions[ControlCount].cx = BUTTON_WIDTH ;
     Positions[ControlCount].cy = cyStatusLine ;
     Positions[ControlCount].x = Positions[iStatus].x + Positions[iStatus].cx + BUTTON_WIDTH * 3 ;
     Positions[ControlCount].y = Positions[iClient].y - ( Data->IniData->ScrollBars ? Positions[iHorzScroll].cy-1 : 0 ) ;
     Positions[ControlCount].fl = SWP_MOVE | SWP_SIZE | SWP_NOADJUST |
       ( ( Position.fl & SWP_MINIMIZE ) ? SWP_HIDE : SWP_SHOW ) ;
     ControlCount ++ ;

     // Move the client window up.
     Positions[iClient].y += cyStatusLine ;
     Positions[iClient].cy -= cyStatusLine ;

     // Move the horizontal scroll bar up.
     if ( Data->IniData->ScrollBars ) 
        Positions[iHorzScroll].y += cyStatusLine ;

     // Move the vertical scroll bar's bottom up.
     if ( Data->IniData->ScrollBars ) {
        Positions[iVertScroll].y += cyStatusLine ;
        Positions[iVertScroll].cy -= cyStatusLine ;
     } /* endif */

     // Move the vertical ruler's bottom up.
     if ( Data->IniData->Rulers ) {
        Positions[iVertRuler].y += cyStatusLine ;
        Positions[iVertRuler].cy -= cyStatusLine ;
     } /* endif */

     // Move the lower-left corner up.
     if ( Data->IniData->Rulers ) 
        Positions[iLowerLeft].y += cyStatusLine ;

     // Move the lower-right corner up.
     if ( Data->IniData->ScrollBars )
        Positions[iLowerRight].y += cyStatusLine ;

  } else {

     WinQueryWindowPos ( CHILD ( Window, FID_STATUSLINE ), &Positions[ControlCount] ) ;
     Positions[ControlCount].cx = 0 ;
     Positions[ControlCount].fl = SWP_HIDE ;
     ControlCount ++ ;

     WinQueryWindowPos ( CHILD ( Window, FID_TOP ), &Positions[ControlCount] ) ;
     Positions[ControlCount].cx = 0 ;
     Positions[ControlCount].fl = SWP_HIDE ;
     ControlCount ++ ;

     WinQueryWindowPos ( CHILD ( Window, FID_PAGEUP ), &Positions[ControlCount] ) ;
     Positions[ControlCount].cx = 0 ;
     Positions[ControlCount].fl = SWP_HIDE ;
     ControlCount ++ ;

     WinQueryWindowPos ( CHILD ( Window, FID_PAGEDOWN ), &Positions[ControlCount] ) ;
     Positions[ControlCount].cx = 0 ;
     Positions[ControlCount].fl = SWP_HIDE ;
     ControlCount ++ ;

     WinQueryWindowPos ( CHILD ( Window, FID_BOTTOM ), &Positions[ControlCount] ) ;
     Positions[ControlCount].cx = 0 ;
     Positions[ControlCount].fl = SWP_HIDE ;
     ControlCount ++ ;

  } /* endif */

 /***************************************************************************
  * Return the number of frame controls.                                    *
  ***************************************************************************/

  return ( MRFROMSHORT ( ControlCount ) ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Initialize Menu                                                     *
 *                                                                          *
 ****************************************************************************/

STATIC MRESULT APIENTRY InitMenu ( HWND Window, MESG msg, MPARAM1 mp1, MPARAM2 mp2 ) {

 /***************************************************************************
  * Find the instance data.                                                 *
  ***************************************************************************/

  PDOCFRAMEDATA Data = PDOCFRAMEDATA ( Sys_GetWindowData ( Window ) ) ;

 /***************************************************************************
  * Let the default frame processor do it's stuff first.                    *
  ***************************************************************************/

  Data->FrameProcessor ( Window, msg, mp1, mp2 ) ;

 /***************************************************************************
  * If trapping, disable the Task Manager option of the system menu.        *
  ***************************************************************************/

  if ( Data->Trap ) {
     HWND SysMenu = CHILD ( Window, FID_SYSMENU ) ;
     USHORT idSysMenu = SHORT1FROMMR ( Sys_SendMessage ( SysMenu, MM_ITEMIDFROMPOSITION, NULL, NULL ) ) ;
     if ( USHORT(SHORT1FROMMP(mp1)) == idSysMenu ) {
        EnableMenuItem ( Window, FID_SYSMENU, SC_TASKMANAGER, FALSE ) ;
     } /* endif */
  } /* endif */

 /***************************************************************************
  * We're done.                                                             *
  ***************************************************************************/

  return ( 0 ) ;
}

/****************************************************************************
 *                                                                          *
 *      Notification of Object Drag over window.                            *
 *                                                                          *
 ****************************************************************************/

STATIC MRESULT APIENTRY DragOver ( HWND, ULONG, MPARAM, MPARAM ) {

 /***************************************************************************
  * We're done.  The normal frame would forward the message to the client.  *
  ***************************************************************************/

  return ( 0 ) ;
}

/****************************************************************************
 *                                                                          *
 *      Notification of Object Drag leaving window.                         *
 *                                                                          *
 ****************************************************************************/

STATIC MRESULT APIENTRY DragLeave ( HWND, ULONG, MPARAM, MPARAM ) {

 /***************************************************************************
  * We're done.  The normal frame would forward the message to the client.  *
  ***************************************************************************/

  return ( 0 ) ;
}

/****************************************************************************
 *                                                                          *
 *      Alter Frame Attributes                                              *
 *                                                                          *
 ****************************************************************************/

STATIC MRESULT APIENTRY AlterFrame ( HWND Window, ULONG, MPARAM, MPARAM ) {

  /**************************************************************************
   * Update the frame.                                                      *
   **************************************************************************/

   Sys_SendMessage ( Window, WM_UPDATEFRAME, 0, 0 ) ;

  /**************************************************************************
   * We're done.                                                            *
   **************************************************************************/

   return ( 0 ) ;
}

/****************************************************************************
 *                                                                          *
 *      Set Window Language                                                 *
 *                                                                          *
 ****************************************************************************/

STATIC MRESULT APIENTRY Set_Language ( HWND Window, MESG, MPARAM1 mp1, MPARAM2 ) {

  /**************************************************************************
   * Find the instance data.                                                *
   **************************************************************************/

   PDOCFRAMEDATA Data = PDOCFRAMEDATA ( Sys_GetWindowData ( Window ) ) ;

  /**************************************************************************
   * Store the new library handle.                                          *
   **************************************************************************/

   Data->Library = (LanguageModule*) mp1 ;

  /**************************************************************************
   * Update all the button help texts.                                      *
   **************************************************************************/

   ResourceString Top ( Data->Library->QueryHandle(), IDS_BUBBLE_TOP ) ;
   Data->Top->SetText ( (char*)Top ) ;

   ResourceString PageUp ( Data->Library->QueryHandle(), IDS_BUBBLE_PAGEUP ) ;
   Data->PageUp->SetText ( (char*)PageUp ) ;

   ResourceString PageDown ( Data->Library->QueryHandle(), IDS_BUBBLE_PAGEDOWN ) ;
   Data->PageDown->SetText ( (char*)PageDown ) ;

   ResourceString Bottom ( Data->Library->QueryHandle(), IDS_BUBBLE_BOTTOM ) ;
   Data->Bottom->SetText ( (char*)Bottom ) ;

  /**************************************************************************
   * Done.                                                                  *
   **************************************************************************/

   return ( 0 ) ;
}
