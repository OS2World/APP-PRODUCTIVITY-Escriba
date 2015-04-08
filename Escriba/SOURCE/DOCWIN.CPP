/***************************************************************** DOCWIN.CPP
 *                                                                          *
 *  Document Window                                                         *
 *                                                                          *
 ****************************************************************************/

#include "System.h"

#include <direct.h>
#include <io.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Barcodes.h"
#include "DateFmt.h"
#include "Debug.h"
#include "Dialog.h"
#include "Dialog2.h"
#include "DocFrame.h"
#include "DocSup.h"
#include "DocWin.h"
#include "Document.h"
#include "Escriba.h"
#include "External.h"
#include "HRuler.h"
#include "Imp_Exp.h"
#include "Indent.h"
#include "Margins.h"
#include "MenuBmp.h"
#include "Mutex.h"
#include "Printer.h"
#include "Process.h"
#include "Restring.h"
#include "Search.h"
#include "SetZoom.h"
#include "Speller.h"
#include "String_.h"
#include "Support.h"
#include "Tabs.h"
#include "Thread.h"
#include "VRuler.h"
#include "Window.h"
#include "WrkSpace.h"

// #define DEBUG
// #define DEBUG_CLOSE
// #define DEBUG_MESSAGES
// #define DEBUG_KEYS

 
/****************************************************************************
 *                                                                          *
 *                       Definitions & Declarations                         *
 *                                                                          *
 ****************************************************************************/

  // Constants

#define STATIC

#define MAX_CHAR 20

enum { BOX_ZOOM, BOX_GRAPHIC, BOX_COPY } ;                      // Values for BoxType.
enum { BOX_DORMANT, BOX_AWAITINGPOINT0, BOX_AWAITINGPOINT1 } ;  // Values for BoxMode.
enum { MOVE_NULL, MOVE_TEXT, MOVE_GRAPHIC, SIZE_GRAPHIC } ;     // Values for MoveType.
enum { CURSOR_SET, CURSOR_SELECT, CURSOR_DRAG } ;               // Values for SelectFlag.


  // Types

typedef struct {
   LanguageModule *Library ;
   HWND        MainWindow ;
   Documento  *pDocument ;
   Dictionary *pSpeller ;
   INIDATA    *IniData ;

   DeviceContext *pDevice ;
   String     *Title ;

   HWND        PrintStatusWindow ; // Used for PRINTING status.
   USHORT      PrintsActive ;

   HWND        StatusWindow ;      // Used for both SAVING and MERGING statuses.
   USHORT      SavesActive ;
   Mutex       *SaveLock ;

   BOOL        Closing ;

   WorkSpace  *WindowPS ;

   HWND        TextMenu ;
   HWND        BitmapMenu ;
   HWND        BorderMenu ;
   HWND        NullMenu ;

   POINTL      MenuPoint ;

   USHORT      FontMenuIndex ;
   USHORT      SizeMenuIndex ;
   USHORT      ExpandMenuIndex ;
   USHORT      LineColorMenuIndex ;
   USHORT      LineWidthMenuIndex ;
   USHORT      FillColorMenuIndex ;
   USHORT      FillTypeMenuIndex ;
   USHORT      LineEndMenuIndex ;
   USHORT      LineJoinMenuIndex ;

   SHORT       ZoomCommand ;
   RECTL       ZoomRectangle ;

   Grafico     PasteGraphic ;

   SHORT       BoxType ;
   SHORT       BoxMode ;
   RECTL       BoxRectangle ;

   SHORT       MoveType ;
   POINTL      MovePosition ;
   RECTL       MoveRectangle ;

   Grafico     MoveGraphic ;

   int         SelectFlag ;
   BOOL        CaptureFlag ;
   BOOL        Button1Down ;
   BOOL        Button2Down ;
   POINTL      MouseOrigin ;
   POINTL      MouseLast ;
   BOOL        Minimized ;
   BOOL        SettingFont ;

} DATA, *PDATA ;

typedef struct {
   LanguageModule *Library ;
   Documento *pDocument ;
   USHORT PageNumber ;
   BOOL SpoolPages ;
   BOOL Reverse ;
   HWND WindowToNotify ;
   HWND MainWindow ;
} PRINT_PARMS, *PPRINT_PARMS ;

typedef struct {
   LanguageModule *Library ;
   Documento *pDocument ;
   UCHAR Filename [CCHMAXPATH] ;
   HWND WindowToNotify ;
   HWND MainWindow ;
   Mutex *Semaphore ;
   int PreviousSavedLevel ;
   ImportExport *Exporter ;
} SAVE_PARMS, *PSAVE_PARMS ;


  // Function Prototypes

STATIC WINDOWPROC SendToMain ;
STATIC WINDOWPROC Create ;
STATIC WINDOWPROC Close ;
STATIC WINDOWPROC Destroy ;
STATIC WINDOWPROC Size ;
STATIC WINDOWPROC MinMaxFrame ;
STATIC WINDOWPROC VScroll ;
STATIC WINDOWPROC HScroll ;
STATIC WINDOWPROC Button1Down ;
STATIC WINDOWPROC Button2Down ;
STATIC WINDOWPROC MouseMove ;
STATIC WINDOWPROC Button1Up ;
STATIC WINDOWPROC Button2Up ;
STATIC WINDOWPROC ContextMenu ;
STATIC WINDOWPROC Button1DblClk ;
STATIC WINDOWPROC Activate ;
STATIC WINDOWPROC SetFocus ;
STATIC WINDOWPROC Paint ;
STATIC WINDOWPROC PresParamChanged ;
STATIC WINDOWPROC DragOver ;
STATIC WINDOWPROC DragLeave ;
STATIC WINDOWPROC Drop ;
STATIC WINDOWPROC Control ;
STATIC WINDOWPROC Command ;
STATIC WINDOWPROC SendCommandToMain ;
STATIC WINDOWPROC FileMerge ;
STATIC WINDOWPROC FileSave ;
STATIC WINDOWPROC FileSaveAs ;
STATIC WINDOWPROC FileSaveClose ;
STATIC WINDOWPROC PrintAll ;
STATIC WINDOWPROC PrintAllNormal ;
STATIC WINDOWPROC PrintAllReverse ;
STATIC WINDOWPROC PrintPage ;
STATIC WINDOWPROC Undo ;
STATIC WINDOWPROC Redo ;
STATIC WINDOWPROC Cut ;
STATIC WINDOWPROC Copy ;
STATIC WINDOWPROC Paste ;
STATIC WINDOWPROC Clear ;
STATIC WINDOWPROC PasteBitmap ;
STATIC WINDOWPROC PasteMetafile ;
STATIC WINDOWPROC PasteScreenEntire ;
STATIC WINDOWPROC PasteScreenFrame ;
STATIC WINDOWPROC PasteScreenWindow ;
STATIC WINDOWPROC PasteScreenRect ;
STATIC WINDOWPROC PasteGraphic ;
STATIC WINDOWPROC Search ;
STATIC WINDOWPROC Find ;
STATIC WINDOWPROC FindNext ;
STATIC WINDOWPROC Change ;
STATIC WINDOWPROC ChangeFind ;
STATIC WINDOWPROC ChangeAll ;
STATIC WINDOWPROC GotoPage ;
STATIC WINDOWPROC SpellCheck ;
STATIC WINDOWPROC Zoom100 ;
STATIC WINDOWPROC ZoomFullWidth ;
STATIC WINDOWPROC ZoomFullHeight ;
STATIC WINDOWPROC ZoomMargins ;
STATIC WINDOWPROC ZoomAll ;
STATIC WINDOWPROC ZoomWindow ;
STATIC WINDOWPROC ZoomSet ;
STATIC WINDOWPROC ApplyZoom ;
STATIC WINDOWPROC SetMargins ;
STATIC WINDOWPROC ApplyMargins ;
STATIC WINDOWPROC SetTabs ;
STATIC WINDOWPROC ApplyTabs ;
STATIC WINDOWPROC SetIndent ;
STATIC WINDOWPROC ApplyIndent ;
STATIC WINDOWPROC SetLineSpacing ;
STATIC WINDOWPROC SetJustLeft ;
STATIC WINDOWPROC SetJustCenter ;
STATIC WINDOWPROC SetJustRight ;
STATIC WINDOWPROC SetJustFull ;
STATIC WINDOWPROC SetJustLeftOdd ;
STATIC WINDOWPROC SetJustLeftEven ;
STATIC WINDOWPROC SetPageCenter ;
STATIC WINDOWPROC PageBreak ;
STATIC WINDOWPROC DateToday ;
STATIC WINDOWPROC DateCurrent ;
STATIC WINDOWPROC DateChanged ;
STATIC WINDOWPROC SetDateFormat ;
STATIC WINDOWPROC ApplyDateFormat ;
STATIC WINDOWPROC CurrentPage ;
STATIC WINDOWPROC PageCount ;
STATIC WINDOWPROC Separator ;
STATIC WINDOWPROC BarCode ;
STATIC WINDOWPROC Header ;
STATIC WINDOWPROC ApplyHeader ;
STATIC WINDOWPROC Footer ;
STATIC WINDOWPROC ApplyFooter ;
STATIC WINDOWPROC FontsPageUp ;
STATIC WINDOWPROC FontsPageDown ;
STATIC WINDOWPROC FontManager ;
STATIC WINDOWPROC SizesPageUp ;
STATIC WINDOWPROC SizesPageDown ;
STATIC WINDOWPROC SizeManager ;
STATIC WINDOWPROC SetNormal ;
STATIC WINDOWPROC SetBold ;
STATIC WINDOWPROC SetItalic ;
STATIC WINDOWPROC SetUnderscore ;
STATIC WINDOWPROC SetStrikeout ;
STATIC WINDOWPROC SetOutline ;
STATIC WINDOWPROC SetCaps ;
STATIC WINDOWPROC SetExpand ;
STATIC WINDOWPROC ExpandPageUp ;
STATIC WINDOWPROC ExpandPageDown ;
STATIC WINDOWPROC ExpandManager ;
STATIC WINDOWPROC LineColorsPageUp ;
STATIC WINDOWPROC LineColorsPageDown ;
STATIC WINDOWPROC LineColorManager ;
STATIC WINDOWPROC LineWidthsPageUp ;
STATIC WINDOWPROC LineWidthsPageDown ;
STATIC WINDOWPROC LineWidthManager ;
STATIC WINDOWPROC FillColorsPageUp ;
STATIC WINDOWPROC FillColorsPageDown ;
STATIC WINDOWPROC FillColorManager ;
STATIC WINDOWPROC FillTypesPageUp ;
STATIC WINDOWPROC FillTypesPageDown ;
STATIC WINDOWPROC FillTypeManager ;
STATIC WINDOWPROC LineEndsPageUp ;
STATIC WINDOWPROC LineEndsPageDown ;
STATIC WINDOWPROC LineEndManager ;
STATIC WINDOWPROC LineJoinsPageUp ;
STATIC WINDOWPROC LineJoinsPageDown ;
STATIC WINDOWPROC LineJoinManager ;
STATIC WINDOWPROC Bitmap_Command ;
STATIC WINDOWPROC Border_Command ;
STATIC WINDOWPROC Default_Refresh ;
STATIC WINDOWPROC Dump ;
STATIC WINDOWPROC Char ;
STATIC WINDOWPROC Backspace ;
STATIC WINDOWPROC BackspaceWord ;
STATIC WINDOWPROC Insert ;
STATIC WINDOWPROC Left ;
STATIC WINDOWPROC Right ;
STATIC WINDOWPROC WordLeft ;
STATIC WINDOWPROC WordRight ;
STATIC WINDOWPROC LineUp ;
STATIC WINDOWPROC LineDown ;
STATIC WINDOWPROC PageUp ;
STATIC WINDOWPROC PageDown ;
STATIC WINDOWPROC PageTop ;
STATIC WINDOWPROC PageBottom ;
STATIC WINDOWPROC Top ;
STATIC WINDOWPROC Bottom ;
STATIC WINDOWPROC Home ;
STATIC WINDOWPROC End ;
STATIC WINDOWPROC SelectAll ;
STATIC WINDOWPROC PrintDone ;
STATIC WINDOWPROC SaveDone ;
STATIC WINDOWPROC QueryState ;
STATIC WINDOWPROC QueryZoom ;
STATIC WINDOWPROC SetFontname ;
STATIC WINDOWPROC SetSize ;
STATIC WINDOWPROC SetLineColor ;
STATIC WINDOWPROC SetLineWidth ;
STATIC WINDOWPROC SetFillColor ;
STATIC WINDOWPROC SetFillType ;
STATIC WINDOWPROC SetLineEnd ;
STATIC WINDOWPROC SetLineJoin ;
STATIC WINDOWPROC SetLeftMargin ;
STATIC WINDOWPROC SetRightMargin ;
STATIC WINDOWPROC SetBottomMargin ;
STATIC WINDOWPROC SetTopMargin ;
STATIC WINDOWPROC SetTab ;
STATIC WINDOWPROC ClearTab ;
STATIC WINDOWPROC MoveTab ;
STATIC WINDOWPROC SetDefaults ;
STATIC WINDOWPROC QueryDefaults ;
STATIC WINDOWPROC AlterWindow ;
STATIC WINDOWPROC UpdateMousePtr ;
STATIC WINDOWPROC QueryTextRectangle ;
STATIC WINDOWPROC SelectLanguage ;

STATIC void AdjustRulerTicks ( HWND Window, PDATA Data, PPOINTL Point ) ;
STATIC void Rezoom ( HWND Window, PDATA Data ) ;
STATIC void SetZoom ( HWND Window, PDATA Data, double Zoom ) ;
STATIC void DrawGraphicTrackingBox ( PDATA Data ) ;

STATIC BOOL Save ( HWND Window, PDATA Data, BOOL Rename ) ;
STATIC void SaveThread ( PVOID Parameter ) ;

STATIC void Print ( HWND Window, int PageNumber, BOOL Reverse ) ;
STATIC void PrintThread ( PVOID Parameter ) ;

STATIC BOOL Button1Down_AwaitingPoint0 ( HWND Window, PDATA Data, POINTL DevicePoint, POINTL ScaledPoint ) ;
STATIC void Button1Down_Default        ( HWND Window, PDATA Data, POINTL DevicePoint, POINTL ScaledPoint ) ;
STATIC BOOL Button1Move_AwaitingPoint1 ( HWND Window, PDATA Data, POINTL DevicePoint, POINTL ScaledPoint ) ;
STATIC void Button1Move_Default        ( HWND Window, PDATA Data, POINTL DevicePoint, POINTL ScaledPoint ) ;
STATIC BOOL Button1Up_AwaitingPoint1   ( HWND Window, PDATA Data, POINTL DevicePoint, POINTL ScaledPoint ) ;
STATIC void Button1Up_Default          ( HWND Window, PDATA Data, POINTL DevicePoint, POINTL ScaledPoint ) ;

STATIC BOOL Button2Down_TouchedGraphic       ( HWND Window, PDATA Data, POINTL DevicePoint, POINTL ScaledPoint, int TokenType ) ;
STATIC BOOL Button2Down_TouchedGraphicBorder ( HWND Window, PDATA Data, POINTL DevicePoint, POINTL ScaledPoint, int TokenType ) ;
STATIC BOOL Button2Down_TouchedSelectedText  ( HWND Window, PDATA Data, POINTL DevicePoint, POINTL ScaledPoint, int TokenType, BOOL Selected ) ;
STATIC void Button2Down_Default              ( HWND Window, PDATA Data, POINTL DevicePoint, POINTL ScaledPoint ) ;
STATIC BOOL Button2Move_MoveGraphic          ( HWND Window, PDATA Data, POINTL DevicePoint, POINTL ScaledPoint ) ;
STATIC BOOL Button2Move_SizeGraphic          ( HWND Window, PDATA Data, POINTL DevicePoint, POINTL ScaledPoint ) ;
STATIC BOOL Button2Move_MoveText             ( HWND Window, PDATA Data, POINTL DevicePoint, POINTL ScaledPoint ) ;
STATIC void Button2Move_Default              ( HWND Window, PDATA Data, POINTL DevicePoint, POINTL ScaledPoint ) ;
STATIC BOOL Button2Up_MoveGraphic            ( HWND Window, PDATA Data, POINTL DevicePoint, POINTL ScaledPoint, BOOL Move, BOOL Button1=FALSE ) ;
STATIC BOOL Button2Up_SizeGraphic            ( HWND Window, PDATA Data, POINTL DevicePoint, POINTL ScaledPoint, BOOL Button1=FALSE  ) ;
STATIC BOOL Button2Up_MoveText               ( HWND Window, PDATA Data, POINTL DevicePoint, POINTL ScaledPoint, BOOL Move ) ;
STATIC void Button2Up_Default                ( HWND Window, PDATA Data, POINTL DevicePoint, POINTL ScaledPoint ) ;

STATIC void UpdateMousePointer ( HWND Window, PDATA Data ) ;
STATIC void UpdateMousePointer ( PDATA Data, POINTL &DevicePoint ) ;

 
/****************************************************************************
 *                                                                          *
 *      Window Message Processor                                            *
 *                                                                          *
 ****************************************************************************/

extern MRESULT EXPENTRY DocWindowMessageProcessor ( HWND Window, MESG msg, MPARAM1 mp1, MPARAM2 mp2 ) {

  /**************************************************************************
   * Dispatch the message according to the method table and return the      *
   *   result.  Any messages not defined above get handled by the system    *
   *   default window processor.                                            *
   **************************************************************************/

   static METHOD Methods [] = {
      { WM_CREATE,                Create              },
      { WM_CLOSE,                 Close               },
      { WM_DESTROY,               Destroy             },
      { WM_SIZE,                  Size                },
      { WM_MINMAXFRAME,           MinMaxFrame         },
      { WM_VSCROLL,               VScroll             },
      { WM_HSCROLL,               HScroll             },
      { WM_BUTTON1DOWN,           Button1Down         },
      { WM_BUTTON2DOWN,           Button2Down         },
      { WM_MOUSEMOVE,             MouseMove           },
      { WM_BUTTON1UP,             Button1Up           },
      { WM_BUTTON2UP,             Button2Up           },
      { WM_CONTEXTMENU,           ContextMenu         },
      { WM_BUTTON1DBLCLK,         Button1DblClk       },
      { WM_ACTIVATE,              Activate            },
      { WM_SETFOCUS,              SetFocus            },
      { WM_PAINT,                 Paint               },
      { WM_INITMENU,              SendToMain          },
      { WM_PRESPARAMCHANGED,      PresParamChanged    },
      { DM_DRAGOVER,              DragOver            },
      { DM_DRAGLEAVE,             DragLeave           },
      { DM_DROP,                  Drop                },
      { WM_CONTROL,               Control             },
      { WM_COMMAND,               Command             },
      { WM_CHAR,                  Char                },
      { WM_PRINT_DONE,            PrintDone           },
      { WM_SAVE_DONE,             SaveDone            },
      { WM_QUERY_STATE,           QueryState          },
      { WM_QUERY_ZOOM,            QueryZoom           },
      { WM_SET_ZOOM,              ApplyZoom           },
      { WM_SET_MARGINS,           ApplyMargins        },
      { WM_SET_TABS,              ApplyTabs           },
      { WM_SET_INDENT,            ApplyIndent         },
      { WM_SET_DATEFMT,           ApplyDateFormat     },
      { WM_SET_FONT,              SetFontname         },
      { WM_SET_SIZE,              SetSize             },
      { WM_SET_EXPANSION,         SetExpand           },
      { WM_SET_LINECOLOR,         SetLineColor        },
      { WM_SET_LINEWIDTH,         SetLineWidth        },
      { WM_SET_FILLCOLOR,         SetFillColor        },
      { WM_SET_FILLTYPE,          SetFillType         },
      { WM_SET_LINEEND,           SetLineEnd          },
      { WM_SET_LINEJOIN,          SetLineJoin         },
      { WM_SET_LINESPACING,       SetLineSpacing      },
      { WM_SET_LEFTMARGIN,        SetLeftMargin       },
      { WM_SET_RIGHTMARGIN,       SetRightMargin      },
      { WM_SET_BOTTOMMARGIN,      SetBottomMargin     },
      { WM_SET_TOPMARGIN,         SetTopMargin        },
      { WM_SET_TAB,               SetTab              },
      { WM_CLEAR_TAB,             ClearTab            },
      { WM_MOVE_TAB,              MoveTab             },
      { WM_SET_DEFAULTS,          SetDefaults         },
      { WM_QUERY_DEFAULTS,        QueryDefaults       },
      { WM_ALTER_WINDOW,          AlterWindow         },
      { WM_UPDATE_MOUSE_POINTER,  UpdateMousePtr      },
      { WM_QUERY_TEXT_RECTANGLE,  QueryTextRectangle  },
      { WM_SET_LANGUAGE,          SelectLanguage      },
      { WM_SET_HEADER,            ApplyHeader         },
      { WM_SET_FOOTER,            ApplyFooter         },
   } ;

   #ifdef DEBUG_MESSAGES
   static int Indent = 0 ;
   LogMsgRcvd ( Indent, "DOCWIN", msg, mp1, mp2 ) ;
   Indent += 2 ;
   #endif

   MRESULT Result = DispatchMessage ( Window, msg, mp1, mp2, Methods, sizeof(Methods)/sizeof(Methods[0]), Sys_DefWindowProc ) ;

   #ifdef DEBUG_MESSAGES
   Indent -= 2 ;
   LogMsgDone ( Indent, "DOCWIN", msg, Result ) ;
   #endif

   return ( Result ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Send Message to Main Window.                                        *
 *                                                                          *
 ****************************************************************************/

STATIC MRESULT APIENTRY SendToMain ( HWND Window, MESG msg, MPARAM1 mp1, MPARAM2 mp2 ) {

  /**************************************************************************
   * Find the instance data.                                                *
   **************************************************************************/

   PDATA Data = PDATA ( Sys_GetWindowData ( Window ) ) ;

  /**************************************************************************
   * Send message to the main window.  Return the result.                   *
   **************************************************************************/

   return ( Sys_SendMessage ( Data->MainWindow, msg, mp1, mp2 ) ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Create the main window.                                             *
 *                                                                          *
 ****************************************************************************/

STATIC MRESULT APIENTRY Create ( HWND Window, MESG, MPARAM1 mp1, MPARAM2 ) {

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

   PDOCWINDOW_PARMS Parms = PDOCWINDOW_PARMS ( PVOIDFROMMP ( mp1 ) ) ;

   Data->Library = Parms->Library ;
   Data->MainWindow = Parms->MainWindow ;
   Data->pDocument = Parms->pDocument ;
   Data->pSpeller = Parms->pSpeller ;
   Data->IniData = Parms->IniData ;

  /**************************************************************************
   * Create the device context object for the window.                       *
   **************************************************************************/

   #ifdef DEBUG
      Log ( "DocWin::Create() Creating window device context object." ) ;
   #endif // DEBUG

   WinOpenWindowDC ( Window ) ;
   Data->pDevice = new DeviceContext ( "DocumentWindow", Window ) ;

  /**************************************************************************
   * Initialize instance data.                                              *
   **************************************************************************/

   #ifdef DEBUG
      Log ( "DocWin::Create() Initializing instance data." ) ;
   #endif // DEBUG

   Data->FontMenuIndex = 0 ;
   Data->SizeMenuIndex = 0 ;
   Data->ExpandMenuIndex = 0 ;
   Data->LineColorMenuIndex = 0 ;
   Data->LineWidthMenuIndex = 0 ;
   Data->FillColorMenuIndex = 0 ;
   Data->FillTypeMenuIndex = 0 ;
   Data->LineEndMenuIndex = 0 ;
   Data->LineJoinMenuIndex = 0 ;

   Data->PrintsActive = 0 ;
   Data->SavesActive = 0 ;
   Data->Closing = FALSE ;

   Data->SaveLock = new Mutex ( "DocWin::SaveLock" ) ;

   Data->Title = (String*)NULL ;

   Data->ZoomCommand = short ( Parms->IniData->ZoomType ) ;

   Data->BoxType = 0 ;
   Data->BoxMode = BOX_DORMANT ;

   Data->MoveType = MOVE_NULL ;

   Data->SelectFlag = CURSOR_SET ;
   Data->CaptureFlag = FALSE ;
   Data->Button1Down = FALSE ;
   Data->Button2Down = FALSE ;
   Data->Minimized = FALSE ;
   Data->SettingFont = FALSE ;

  /**************************************************************************
   * Create and initialize the working presentation space.                  *
   **************************************************************************/

   #ifdef DEBUG
      Log ( "DocWin::Create() Creating working presentation space." ) ;
   #endif // DEBUG

   Data->WindowPS = new WorkSpace ( "DataWindow::Create (unscaled)", 
      Data->pDocument->Query_Parms()->CodePage, Window, Data->pDevice ) ;

  /**************************************************************************
   * Tell the document object what it needs to know about the window.       *
   **************************************************************************/

   #ifdef DEBUG
      Log ( "DocWin::Create() Telling the document about its window." ) ;
   #endif // DEBUG

   HWND Frame = PARENT ( Window ) ;
   HWND HBar = CHILD ( Frame, FID_HORZSCROLL ) ;
   HWND HRuler = CHILD ( Frame, FID_HORZRULER ) ;
   HWND StatusLine = CHILD ( Frame, FID_STATUSLINE ) ;
   HWND TitleBar = CHILD ( Frame, FID_TITLEBAR ) ;
   HWND VBar = CHILD ( Frame, FID_VERTSCROLL ) ;
   HWND VRuler = CHILD ( Frame, FID_VERTRULER ) ;

   Data->pDocument->Set_FullNames ( Data->IniData->FullNames ) ;
   Data->pDocument->Set_Minimized ( Data->Minimized ) ;
   Data->pDocument->Set_RulerWindows ( HRuler, VRuler ) ;
   Data->pDocument->Set_ScrollBars ( HBar, VBar ) ;
   Data->pDocument->Set_StatusWindow ( StatusLine ) ;
   Data->pDocument->Set_TitleWindow ( TitleBar ) ;

   COLOR Colors[2] ;
   Colors[0] = Data->IniData->fDocColors[0] ? Data->IniData->DocColors[0] : RGB_WHITE ;
   Colors[1] = Data->IniData->fDocColors[1] ? Data->IniData->DocColors[1] : RGB_BLUE ;
   Data->pDocument->Set_Colors ( Colors ) ;

   Data->pDocument->Set_Window ( Window, Data->pDevice ) ;

  /**************************************************************************
   * Position & size the window to occupy all the main window.              *
   **************************************************************************/

   #ifdef DEBUG
      Log ( "DocWin::Create() Positioning document window." ) ;
   #endif // DEBUG

   RECTL Rectangle ;
   WinQueryWindowRect ( Data->MainWindow, &Rectangle ) ;

   WinSetWindowPos ( Frame, 0, Rectangle.xLeft, Rectangle.yBottom,
      Rectangle.xRight-Rectangle.xLeft, Rectangle.yTop-Rectangle.yBottom,
      SWP_SIZE | SWP_MOVE ) ;

  /**************************************************************************
   * Build the presentation parameters for the status window.               *
   **************************************************************************/

   #ifdef DEBUG
      Log ( "DocWin::Create() Building status window presentation parameters." ) ;
   #endif // DEBUG

   COLOR FgndColor = RGB_WHITE ;
   COLOR BackColor = RGB_RED ;
   ResourceString StatusFont ( Data->Library->QueryHandle(), IDS_STATUSFONT ) ;
   ULONG Ids[] = { PP_FOREGROUNDCOLOR, PP_BACKGROUNDCOLOR, PP_FONTNAMESIZE } ;
   ULONG ByteCounts[] = { sizeof(FgndColor), sizeof(BackColor), strlen(PCHAR(StatusFont))+1 } ;
   PUCHAR Params[] = { PUCHAR(&FgndColor), PUCHAR(&BackColor), PUCHAR(StatusFont) } ;

   PPRESPARAMS PresParams = BuildPresParams ( sizeof(Ids)/sizeof(Ids[0]), Ids, ByteCounts, Params ) ;

  /**************************************************************************
   * Get the title-bar window handle and alter its style a bit.             *
   **************************************************************************/

   #ifdef DEBUG
      Log ( "DocWin::Create() Customizing the title window." ) ;
   #endif // DEBUG

   ULONG TitleStyle = WinQueryWindowULong ( TitleBar, QWL_STYLE ) ;
   TitleStyle |= WS_CLIPCHILDREN ;
   WinSetWindowULong ( TitleBar, QWL_STYLE, TitleStyle ) ;

   SWP Position ;
   WinQueryWindowPos ( TitleBar, &Position ) ;

  /**************************************************************************
   * Create the background print status display window.                     *
   **************************************************************************/

   #ifdef DEBUG
      Log ( "DocWin::Create() Creating the printing status window." ) ;
   #endif // DEBUG

   ResourceString Printing ( Data->Library->QueryHandle(), IDS_PRINTING ) ;
   Data->PrintStatusWindow = WinCreateWindow ( TitleBar, WC_STATIC, PSZ(Printing),
      WS_CLIPCHILDREN | WS_CLIPSIBLINGS | SS_TEXT | DT_CENTER,
      Position.cx-Position.cy*12, 1, Position.cy*6-1, Position.cy-1,
      TitleBar, HWND_TOP, 0, NULL, PresParams ) ;

  /**************************************************************************
   * Create the background status display window.                           *
   **************************************************************************/

   #ifdef DEBUG
      Log ( "DocWin::Create() Creating the regular status window." ) ;
   #endif // DEBUG

   Data->StatusWindow = WinCreateWindow ( TitleBar, WC_STATIC, PSZ(""),
      WS_CLIPCHILDREN | WS_CLIPSIBLINGS | SS_TEXT | DT_CENTER,
      Position.cx-Position.cy*6, 1, Position.cy*6-1, Position.cy-1,
      TitleBar, HWND_TOP, 0, NULL, PresParams ) ;

   free ( PresParams ) ;

  /**************************************************************************
   * Load the context menus.                                                *
   **************************************************************************/

   #ifdef DEBUG
      Log ( "DocWin::Create() Loading context windows." ) ;
   #endif // DEBUG

   Data->TextMenu = WinLoadMenu ( HWND_DESKTOP, Data->Library->QueryHandle(), IDM_TEXTMENU ) ;
   Data->BitmapMenu = WinLoadMenu ( HWND_DESKTOP, Data->Library->QueryHandle(), IDM_BITMAPMENU ) ;
   Data->BorderMenu = WinLoadMenu ( HWND_DESKTOP, Data->Library->QueryHandle(), IDM_BORDERMENU ) ;
   Data->NullMenu = WinLoadMenu ( HWND_DESKTOP, Data->Library->QueryHandle(), IDM_DEFAULTMENU ) ;

  /**************************************************************************
   * Alter the Print All menu to make it conditional.                       *
   **************************************************************************/

   #ifdef DEBUG
      Log ( "DocWin::Create() Customizing the Print All menu." ) ;
   #endif // DEBUG

   MENUITEM MenuItem ;
   Sys_SendMessage ( Data->NullMenu, MM_QUERYITEM, MPFROM2SHORT(IDM_PRINT_ALL,TRUE), MPFROMP(&MenuItem) ) ;
   ULONG Style = WinQueryWindowULong ( MenuItem.hwndSubMenu, QWL_STYLE ) ;
   WinSetWindowULong ( MenuItem.hwndSubMenu, QWL_STYLE, Style | MS_CONDITIONALCASCADE ) ;

  /**************************************************************************
   * Set the window title.                                                  *
   **************************************************************************/

   #ifdef DEBUG
      Log ( "DocWin::Create() Setting the document title." ) ;
   #endif // DEBUG

   Data->pDocument->UpdateTitle ( ) ;

  /**************************************************************************
   * Success?  Return no error.                                             *
   **************************************************************************/

   #ifdef DEBUG
      Log ( "DocWin::Create() Done." ) ;
   #endif // DEBUG

   return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Close Main Window                                                   *
 *                                                                          *
 ****************************************************************************/

STATIC MRESULT APIENTRY Close ( HWND Window, MESG, MPARAM1, MPARAM2 ) {

  /**************************************************************************
   * Find the instance data.                                                *
   **************************************************************************/

   PDATA Data = PDATA ( Sys_GetWindowData ( Window ) ) ;

  /**************************************************************************
   * If the file has changed, save it.  Abort save if so requested.         *
   **************************************************************************/

   if ( Data->pDocument->IsChanged() ) {

      // Determine the file name, if any has been set yet.
      ResourceString NewDocument ( Data->Library->QueryHandle(), IDS_NEW_DOCUMENT ) ;
      PSZ Filename = Data->pDocument->Query_Filename ( ) ;
      if ( Filename == NULL )
         Filename = PSZ(NewDocument) ;

      // Build save prompt.
      ResourceString Format ( Data->Library->QueryHandle(), IDS_PROMPT_SAVE ) ;
      char Message [500] ;
      sprintf ( Message, PCHAR(Format), Filename ) ;

      // Ask user's opinion.
      ResourceString Title1 ( Data->Library->QueryHandle(), IDS_TITLE1 ) ;
      APIRET rc = MessageBox ( HWND_DESKTOP, Window, Message, PCHAR(Title1),
         IDD_PROMPT_SAVE, MB_YESNOCANCEL | MB_ICONQUESTION | MB_DEFBUTTON3 | MB_HELP, 
         Library->QueryHandle(), IDS_MESSAGE_BOX_STRINGS ) ; 

      // If user cancelled, exit w/o having done anything.
      if ( rc == MBID_CANCEL ) 
         return ( 0 ) ;

      // If user said Yes, then we must save the file.  Exit error if unable to do so or aborted.
      if ( rc == MBID_YES ) {
         PSZ Filename = Data->pDocument->Query_Filename ( ) ;
         BOOL Prompt = ( Filename == 0 ) OR ( Filename[0] == 0 ) ;
         if ( !Save ( Window, Data, Prompt ) )
            return ( 0 ) ;
      } /* endif */

   } /* endif */

  /**************************************************************************
   * Ask the main window to close the document window.                      *
   **************************************************************************/

   HWND Frame = PARENT ( Window ) ;
   Sys_PostMessage ( Data->MainWindow, WM_CLOSE_ME, MPFROMP(Frame), MPFROMLONG(FALSE) ) ;

  /**************************************************************************
   * Return good status to main program.                                    *
   **************************************************************************/

   return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Destroy the main window.                                            *
 *                                                                          *
 ****************************************************************************/

STATIC MRESULT APIENTRY Destroy ( HWND Window, MESG, MPARAM1, MPARAM2 ) {

  /**************************************************************************
   * Find the instance data.                                                *
   **************************************************************************/

   PDATA Data = PDATA ( Sys_GetWindowData ( Window ) ) ;

  /**************************************************************************
   * Close the save lock semaphore.                                         *
   **************************************************************************/

   delete Data->SaveLock ;

  /**************************************************************************
   * Destroy the working presentation spaces.                               *
   **************************************************************************/

   delete Data->WindowPS ;

  /**************************************************************************
   * Destroy the device context object for the window.                      *
   **************************************************************************/

   delete Data->pDevice ;

  /**************************************************************************
   * Destroy the title-string.                                              *
   **************************************************************************/

   if ( Data->Title )
      delete Data->Title ;

  /**************************************************************************
   * Release the instance memory.                                           *
   **************************************************************************/

   free ( Data ) ;

  /**************************************************************************
   * We're done.                                                            *
   **************************************************************************/

   return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Resize the main window.                                             *
 *                                                                          *
 ****************************************************************************/

STATIC MRESULT APIENTRY Size ( HWND Window, MESG, MPARAM1, MPARAM2 ) {

  /**************************************************************************
   * Find the instance data.                                                *
   **************************************************************************/

   PDATA Data = PDATA ( Sys_GetWindowData ( Window ) ) ;

  /**************************************************************************
   * Find out the window's new position and size.                           *
   **************************************************************************/

   HWND Frame = PARENT ( Window ) ;

   SWP Position ;
   WinQueryWindowPos ( Frame, &Position ) ;

  /**************************************************************************
   * Relocate the print and load status display windows.                    *
   **************************************************************************/

   HWND TitleBar = CHILD ( Frame, FID_TITLEBAR ) ;
   WinQueryWindowPos ( TitleBar, &Position ) ;

   WinSetWindowPos ( Data->PrintStatusWindow, 0,
      Position.cx-Position.cy*12, 1, Position.cy*6-1, Position.cy-1,
      SWP_MOVE | SWP_SIZE ) ;

   WinSetWindowPos ( Data->StatusWindow, 0,
      Position.cx-Position.cy*6, 1, Position.cy*6-1, Position.cy-1,
      SWP_MOVE | SWP_SIZE ) ;

  /**************************************************************************
   * Rezoom the window.                                                     *
   **************************************************************************/

   Rezoom ( Window, Data ) ;

  /**************************************************************************
   * We're done.                                                            *
   **************************************************************************/

   return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process frame minimize/maximize notification.                       *
 *                                                                          *
 ****************************************************************************/

STATIC MRESULT APIENTRY MinMaxFrame ( HWND Window, MESG, MPARAM1 mp1, MPARAM2 ) {

  /**************************************************************************
   * Find the instance data.                                                *
   **************************************************************************/

   PDATA Data = PDATA ( Sys_GetWindowData ( Window ) ) ;

  /**************************************************************************
   * Determine the new value of the Minimized flag.                         *
   **************************************************************************/

   PSWP Position = PSWP ( PVOIDFROMMP ( mp1 ) ) ;
   Data->Minimized = Position->fl & SWP_MINIMIZE ;
   Data->pDocument->Set_Minimized ( Data->Minimized ) ;

  /**************************************************************************
   * Update the window text as needed.                                      *
   **************************************************************************/

   Data->pDocument->UpdateTitle ( ) ;

  /**************************************************************************
   * We're done.  Return status FALSE to allow action to take place.        *
   **************************************************************************/

   return ( MRFROMSHORT ( FALSE ) ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process Vertical Scrolling.                                         *
 *                                                                          *
 ****************************************************************************/

STATIC MRESULT APIENTRY VScroll ( HWND Window, MESG, MPARAM1, MPARAM2 mp2 ) {

  /**************************************************************************
   * Find the instance data.                                                *
   **************************************************************************/

   PDATA Data = PDATA ( Sys_GetWindowData ( Window ) ) ;

  /**************************************************************************
   * Ask the document object to perform the vertical scroll adjustment.     *
   **************************************************************************/

   Data->pDocument->VerticalScroll ( SHORT(SHORT2FROMMP(mp2)), SHORT(SHORT1FROMMP(mp2)) ) ;

  /**************************************************************************
   * We're done.                                                            *
   **************************************************************************/

   return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process Horizontal Scrolling.                                       *
 *                                                                          *
 ****************************************************************************/

STATIC MRESULT APIENTRY HScroll ( HWND Window, MESG, MPARAM1, MPARAM2 mp2 ) {

  /**************************************************************************
   * Find the instance data.                                                *
   **************************************************************************/

   PDATA Data = PDATA ( Sys_GetWindowData ( Window ) ) ;

  /**************************************************************************
   * Ask the document object to perform the vertical scroll adjustment.     *
   **************************************************************************/

   Data->pDocument->HorizontalScroll ( SHORT(SHORT2FROMMP(mp2)), SHORT(SHORT1FROMMP(mp2)) ) ;

  /**************************************************************************
   * We're done.                                                            *
   **************************************************************************/

   return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process Mouse Button One Down                                       *
 *                                                                          *
 ****************************************************************************/

STATIC MRESULT APIENTRY Button1Down ( HWND Window, MESG msg, MPARAM1 mp1, MPARAM2 mp2 ) {

  /**************************************************************************
   * Find the instance data.                                                *
   **************************************************************************/

   PDATA Data = PDATA ( Sys_GetWindowData ( Window ) ) ;

  /*************w************************************************************
   * If this window doesn't have the focus, pass the message through.       *
   **************************************************************************/

   if ( Window != WinQueryFocus ( HWND_DESKTOP ) )
      return ( Sys_DefWindowProc ( Window, msg, mp1, mp2 ) ) ;

  /**************************************************************************
   * If mouse button two already down, ignore this message.                 *
   **************************************************************************/

   if ( Data->Button2Down ) {
      Sys_BeepNote ( ) ;
      return ( MRFROMSHORT(TRUE) ) ;
   } /* endif */

  /**************************************************************************
   * Set flag indicating that mouse button one is down.                     *
   **************************************************************************/

   Data->Button1Down = TRUE ;

  /**************************************************************************
   * Wait for queued editing to complete.                                   *
   **************************************************************************/

   Data->pDocument->FlushCommandQueue ( ) ;

  /**************************************************************************
   * Find out where the mouse touched.                                      *
   **************************************************************************/

   POINTL Point = { SHORT(SHORT1FROMMP(mp1)), SHORT(SHORT2FROMMP(mp1)) } ;
   POINTL ConvertedPoint = Point ;
   Data->pDocument->Transform ( CVTC_DEVICE, CVTC_DEFAULTPAGE, ConvertedPoint ) ;

   Data->MouseOrigin = ConvertedPoint ;
   Data->MouseLast = ConvertedPoint ;

  /**************************************************************************
   * If the mouse is off the page, just beep.                               *
   **************************************************************************/

   if ( ( ConvertedPoint.x < 0 ) OR ( ConvertedPoint.x > Data->pDocument->Query_State0()->FormInfo.cx ) ) {
      Sys_BeepNote ( ) ;
      return ( MRFROMSHORT(TRUE) ) ;
   } /* endif */

   if ( ( ConvertedPoint.y < 0 ) OR ( ConvertedPoint.y > Data->pDocument->Query_State0()->FormInfo.cy ) ) {
      Sys_BeepNote ( ) ;
      return ( MRFROMSHORT(TRUE) ) ;
   } /* endif */

  /**************************************************************************
   * Determine what's at the point where the mouse touched.                 *
   **************************************************************************/

   BOOL Selected ;
   int TokenType = Data->pDocument->TestXY ( ConvertedPoint, Selected ) ;

  /**************************************************************************
   * If we're defining a rectangle, grab the current location and switch    *
   *   to the next mode.                                                    *
   **************************************************************************/

   if ( Button1Down_AwaitingPoint0 ( Window, Data, Point, ConvertedPoint ) ) {

  /**************************************************************************
   * If graphic, set up to move/copy the graphic.                           *
   **************************************************************************/

   } else if ( Button2Down_TouchedGraphic ( Window, Data, Point, ConvertedPoint, TokenType ) ) {

  /**************************************************************************
   * Else, if graphic border, set up to resize graphic.                     *
   **************************************************************************/

   } else if ( Button2Down_TouchedGraphicBorder ( Window, Data, Point, ConvertedPoint, TokenType ) ) {

  /**************************************************************************
   * Else, perform default action.                                          *
   **************************************************************************/

   } else
      Button1Down_Default ( Window, Data, Point, ConvertedPoint ) ;

  /**************************************************************************
   * We're done. Acknowledge having processed the mouse click.              *
   **************************************************************************/

   return ( MRFROMSHORT(TRUE) ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process Mouse Button Two Down                                       *
 *                                                                          *
 ****************************************************************************/

STATIC MRESULT APIENTRY Button2Down ( HWND Window, MESG msg, MPARAM1 mp1, MPARAM2 mp2 ) {

  /**************************************************************************
   * Find the instance data.                                                *
   **************************************************************************/

   PDATA Data = PDATA ( Sys_GetWindowData ( Window ) ) ;

  /**************************************************************************
   * If this window doesn't have the focus, pass the message through.       *
   **************************************************************************/

   if ( Window != WinQueryFocus ( HWND_DESKTOP ) )
      return ( Sys_DefWindowProc ( Window, msg, mp1, mp2 ) ) ;

  /**************************************************************************
   * If mouse button one already down, ignore this message.                 *
   **************************************************************************/

   if ( Data->Button1Down ) {
      Sys_BeepNote ( ) ;
      return ( MRFROMSHORT(TRUE) ) ;
   } /* endif */

  /**************************************************************************
   * Set flag indicating that mouse button two is down.                     *
   **************************************************************************/

   Data->Button2Down = TRUE ;

  /**************************************************************************
   * Wait for queued editing to complete.                                   *
   **************************************************************************/

   Data->pDocument->FlushCommandQueue ( ) ;

  /**************************************************************************
   * Find out where the mouse touched.                                      *
   **************************************************************************/

   POINTL Point = { SHORT(SHORT1FROMMP(mp1)), SHORT(SHORT2FROMMP(mp1)) } ;
   POINTL ConvertedPoint = Point ;
   Data->pDocument->Transform ( CVTC_DEVICE, CVTC_DEFAULTPAGE, ConvertedPoint ) ;

   Data->MouseOrigin = ConvertedPoint ;
   Data->MouseLast = ConvertedPoint ;

  /**************************************************************************
   * If the mouse is off the page, just beep.                               *
   **************************************************************************/

   if ( ( ConvertedPoint.x < 0 ) OR ( ConvertedPoint.x > Data->pDocument->Query_State0()->FormInfo.cx ) ) {
      Sys_BeepNote ( ) ;
      return ( MRFROMSHORT(TRUE) ) ;
   } /* endif */

   if ( ( ConvertedPoint.y < 0 ) OR ( ConvertedPoint.y > Data->pDocument->Query_State0()->FormInfo.cy ) ) {
      Sys_BeepNote ( ) ;
      return ( MRFROMSHORT(TRUE) ) ;
   } /* endif */

  /**************************************************************************
   * Determine what's at the point where the mouse touched.                 *
   **************************************************************************/

   BOOL Selected ;
   int TokenType = Data->pDocument->TestXY ( ConvertedPoint, Selected ) ;

  /**************************************************************************
   * If graphic, set up to move/copy the graphic.                           *
   **************************************************************************/

   if ( Button2Down_TouchedGraphic ( Window, Data, Point, ConvertedPoint, TokenType ) ) {

  /**************************************************************************
   * Else, if graphic border, set up to resize graphic.                     *
   **************************************************************************/

   } else if ( Button2Down_TouchedGraphicBorder ( Window, Data, Point, ConvertedPoint, TokenType ) ) {

  /**************************************************************************
   * Else, if text selected, set up to move/copy text.                      *
   **************************************************************************/

   } else if ( Button2Down_TouchedSelectedText ( Window, Data, Point, ConvertedPoint, TokenType, Selected ) ) {

  /**************************************************************************
   * Else just initiate a null move.                                        *
   **************************************************************************/

   } else
      Button2Down_Default ( Window, Data, Point, ConvertedPoint ) ;

  /**************************************************************************
   * We're done.                                                            *
   **************************************************************************/

   return ( MRFROMSHORT(TRUE) ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process Mouse Motion                                                *
 *                                                                          *
 ****************************************************************************/

STATIC MRESULT APIENTRY MouseMove ( HWND Window, MESG, MPARAM1 mp1, MPARAM2 ) {

  /**************************************************************************
   * Find the instance data.                                                *
   **************************************************************************/

   PDATA Data = PDATA ( Sys_GetWindowData ( Window ) ) ;

  /**************************************************************************
   * Wait for queued editing to complete.                                   *
   **************************************************************************/

   Data->pDocument->FlushCommandQueue ( ) ;

  /**************************************************************************
   * Find out where the mouse is.                                           *
   **************************************************************************/

   POINTL Point = { SHORT(SHORT1FROMMP(mp1)), SHORT(SHORT2FROMMP(mp1)) } ;
   POINTL ConvertedPoint = Point ;
   Data->pDocument->Transform ( CVTC_DEVICE, CVTC_DEFAULTPAGE, ConvertedPoint ) ;

  /**************************************************************************
   * Set the mouse pointer type.                                            *
   **************************************************************************/

   UpdateMousePointer ( Data, Point ) ;

  /**************************************************************************
   * If mouse should be captured, but is not, capture it now.               *
   **************************************************************************/

   if ( ( Data->BoxMode != BOX_DORMANT ) AND NOT Data->CaptureFlag )
      Data->CaptureFlag = Sys_SetCapture ( Window ) ;

  /**************************************************************************
   * If mouse not captured, we're done for now.                             *
   **************************************************************************/

   if ( NOT Data->CaptureFlag )
      return ( MRFROMSHORT(TRUE) ) ;

  /**************************************************************************
   * If the mouse has not moved, do nothing.                                *
   **************************************************************************/

   if ( ( ConvertedPoint.x == Data->MouseLast.x ) AND ( ConvertedPoint.y == Data->MouseLast.y ) )
      return ( MRFROMSHORT(TRUE) ) ;

   Data->MouseLast = ConvertedPoint ;

  /**************************************************************************
   * If tracking mouse button one . . .                                     *
   **************************************************************************/

   if ( Data->Button1Down ) {

      if ( Button1Move_AwaitingPoint1 ( Window, Data, Point, ConvertedPoint ) ) {

      } else if ( Button2Move_MoveGraphic ( Window, Data, Point, ConvertedPoint ) ) {

      } else if ( Button2Move_SizeGraphic ( Window, Data, Point, ConvertedPoint ) ) {

      } else
         Button1Move_Default ( Window, Data, Point, ConvertedPoint ) ;

  /**************************************************************************
   * Else, if tracking mouse button two . . .                               *
   **************************************************************************/

   } else if ( Data->Button2Down ) {

      if ( Button2Move_MoveGraphic ( Window, Data, Point, ConvertedPoint ) ) {

      } else if ( Button2Move_SizeGraphic ( Window, Data, Point, ConvertedPoint ) ) {

      } else if ( Button2Move_MoveText ( Window, Data, Point, ConvertedPoint ) ) {

      } else
         Button2Move_Default ( Window, Data, Point, ConvertedPoint ) ;

  /**************************************************************************
   * Else, we're simply tracking the mouse.                                 *
   **************************************************************************/

   } else
      AdjustRulerTicks ( Window, Data, &ConvertedPoint ) ;

  /**************************************************************************
   * We're done.                                                            *
   **************************************************************************/

   return ( MRFROMSHORT(TRUE) ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process Mouse Button One Up                                         *
 *                                                                          *
 ****************************************************************************/

STATIC MRESULT APIENTRY Button1Up ( HWND Window, MESG msg, MPARAM1 mp1, MPARAM2 mp2 ) {

  /**************************************************************************
   * Find the instance data.                                                *
   **************************************************************************/

   PDATA Data = PDATA ( Sys_GetWindowData ( Window ) ) ;

  /**************************************************************************
   * If mouse button two already down, ignore this message.                 *
   **************************************************************************/

   if ( Data->Button2Down )
      return ( Sys_DefWindowProc ( Window, msg, mp1, mp2 ) ) ;

  /**************************************************************************
   * If mouse button one not recorded down, ignore this message.            *
   **************************************************************************/

   if ( !Data->Button1Down )
      return ( Sys_DefWindowProc ( Window, msg, mp1, mp2 ) ) ;

  /**************************************************************************
   * Reset state of mouse button one.                                       *
   **************************************************************************/

   Data->Button1Down = FALSE ;

  /**************************************************************************
   * If mouse not captured, ignore this message.                            *
   **************************************************************************/

   if ( NOT Data->CaptureFlag )
      return ( Sys_DefWindowProc ( Window, msg, mp1, mp2 ) ) ;

  /**************************************************************************
   * Release the mouse.                                                     *
   **************************************************************************/

   Sys_ReleaseCapture ( ) ;
   Data->CaptureFlag = FALSE ;

  /**************************************************************************
   * Wait for queued editing to complete.                                   *
   **************************************************************************/

   Data->pDocument->FlushCommandQueue ( ) ;

  /**************************************************************************
   * Find out where the mouse touched.                                      *
   **************************************************************************/

   POINTL Point = { SHORT(SHORT1FROMMP(mp1)), SHORT(SHORT2FROMMP(mp1)) } ;
   POINTL ConvertedPoint = Point ;
   Data->pDocument->Transform ( CVTC_DEVICE, CVTC_DEFAULTPAGE, ConvertedPoint ) ;

  /**************************************************************************
   * Determine if the control key is currently pressed.                     *
   **************************************************************************/

   BOOL Move = ( ( WinGetKeyState ( HWND_DESKTOP, VK_CTRL ) & 0x8000 ) == 0 ) ;

  /**************************************************************************
   * If we're defining a box and are expecting the second point, update     *
   *   the point location now.                                              *
   **************************************************************************/

   if ( Button1Up_AwaitingPoint1 ( Window, Data, Point, ConvertedPoint ) ) {

  /**************************************************************************
   * Or, complete the current graphic move/copy operation.                  *
   **************************************************************************/

   } else if ( Button2Up_MoveGraphic ( Window, Data, Point, ConvertedPoint, Move, TRUE ) ) {

  /**************************************************************************
   * Or, complete the current graphic size operation.                       *
   **************************************************************************/

   } else if ( Button2Up_SizeGraphic ( Window, Data, Point, ConvertedPoint, TRUE ) ) {

  /**************************************************************************
   * Else, perform the default action.                                      *
   **************************************************************************/

   } else
      Button1Up_Default ( Window, Data, Point, ConvertedPoint ) ;

  /**************************************************************************
   * We're done.                                                            *
   **************************************************************************/

   return ( MRFROMSHORT(TRUE) ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process Mouse Button Two Up                                         *
 *                                                                          *
 ****************************************************************************/

STATIC MRESULT APIENTRY Button2Up ( HWND Window, MESG msg, MPARAM1 mp1, MPARAM2 mp2 ) {

  /**************************************************************************
   * Find the instance data.                                                *
   **************************************************************************/

   PDATA Data = PDATA ( Sys_GetWindowData ( Window ) ) ;

  /**************************************************************************
   * If mouse button one already down, ignore this message.                 *
   **************************************************************************/

   if ( Data->Button1Down )
      return ( Sys_DefWindowProc ( Window, msg, mp1, mp2 ) ) ;

  /**************************************************************************
   * If mouse button two not recorded down, ignore this message.            *
   **************************************************************************/

   if ( !Data->Button2Down )
      return ( Sys_DefWindowProc ( Window, msg, mp1, mp2 ) ) ;

  /**************************************************************************
   * Reset state of mouse button two.                                       *
   **************************************************************************/

   Data->Button2Down = FALSE ;

  /**************************************************************************
   * If mouse not captured, ignore this message.                            *
   **************************************************************************/

   if ( NOT Data->CaptureFlag )
      return ( Sys_DefWindowProc ( Window, msg, mp1, mp2 ) ) ;

  /**************************************************************************
   * Release the mouse.                                                     *
   **************************************************************************/

   Sys_ReleaseCapture ( ) ;
   Data->CaptureFlag = FALSE ;

  /**************************************************************************
   * Clear the status line.                                                 *
   **************************************************************************/

   HWND Frame = PARENT ( Window ) ;
   HWND StatusWindow = CHILD ( Frame, FID_STATUSLINE ) ;
   Sys_SetWindowText ( StatusWindow, PSZ("") ) ;

  /**************************************************************************
   * Wait for queued editing to complete.                                   *
   **************************************************************************/

   Data->pDocument->FlushCommandQueue ( ) ;

  /**************************************************************************
   * Find out where the mouse touched.                                      *
   **************************************************************************/

   POINTL Point = { SHORT(SHORT1FROMMP(mp1)), SHORT(SHORT2FROMMP(mp1)) } ;
   POINTL ConvertedPoint = Point ;
   Data->pDocument->Transform ( CVTC_DEVICE, CVTC_DEFAULTPAGE, ConvertedPoint ) ;

  /**************************************************************************
   * Determine if the control key is currently pressed.                     *
   **************************************************************************/

   BOOL Move = ( ( WinGetKeyState ( HWND_DESKTOP, VK_CTRL ) & 0x8000 ) == 0 ) ;

  /**************************************************************************
   * Complete the current graphic move/copy operation.                      *
   **************************************************************************/

   if ( Button2Up_MoveGraphic ( Window, Data, Point, ConvertedPoint, Move ) ) {

  /**************************************************************************
   * Or, complete the current graphic size operation.                       *
   **************************************************************************/

   } else if ( Button2Up_SizeGraphic ( Window, Data, Point, ConvertedPoint ) ) {

  /**************************************************************************
   * Or, complete the current text move/copy operation.                     *
   **************************************************************************/

   } else if ( Button2Up_MoveText ( Window, Data, Point, ConvertedPoint, Move ) ) {

  /**************************************************************************
   * Or, complete the null move.                                            *
   **************************************************************************/

   } else
      Button2Up_Default ( Window, Data, Point, ConvertedPoint ) ;

  /**************************************************************************
   * We're done.                                                            *
   **************************************************************************/

   return ( MRFROMSHORT(TRUE) ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process Context Menu command.                                       *
 *                                                                          *
 ****************************************************************************/

STATIC MRESULT APIENTRY ContextMenu ( HWND Window, MESG, MPARAM1 mp1, MPARAM2 ) {

  /**************************************************************************
   * Find the instance data.                                                *
   **************************************************************************/

   PDATA Data = PDATA ( Sys_GetWindowData ( Window ) ) ;

  /**************************************************************************
   * Wait for queued editing to complete.                                   *
   **************************************************************************/

   Data->pDocument->FlushCommandQueue ( ) ;

  /**************************************************************************
   * Find out what the mouse touched.                                       *
   **************************************************************************/

   POINTL Point = { SHORT(SHORT1FROMMP(mp1)), SHORT(SHORT2FROMMP(mp1)) } ;
   POINTL ConvertedPoint = Point ;
   Data->pDocument->Transform ( CVTC_DEVICE, CVTC_DEFAULTPAGE, ConvertedPoint ) ;

   BOOL Selected ;
   int TokenType = Data->pDocument->TestXY ( ConvertedPoint, Selected ) ;

  /**************************************************************************
   * Figure out where the menu's going to go.                               *
   **************************************************************************/

   if ( !mp1 )          // If SHIFT+F10, get the mouse pointer location directly.
      WinQueryPointerPos ( HWND_DESKTOP, &Point ) ;
   else
      WinMapWindowPoints ( Window, HWND_DESKTOP, &Point, 1 ) ;

  /**************************************************************************
   * Invoke the appropriate context menu.                                   *
   **************************************************************************/

   switch ( TokenType ) {

      case TEST_GRAPHICBORDER: {
         WinPopupMenu ( HWND_DESKTOP, Window, Data->BorderMenu, Point.x, Point.y, 0,
            PU_NONE | PU_HCONSTRAIN | PU_VCONSTRAIN | PU_KEYBOARD | PU_MOUSEBUTTON1 | PU_MOUSEBUTTON2 ) ;
         break; }

      case TEST_GRAPHIC: {
         EnableMenuItem ( Data->BitmapMenu, IDM_BITMAP_HALFTONE,                !strnicmp(Data->MoveGraphic.Type,"BM",2) ) ;
         EnableMenuItem ( Data->BitmapMenu, IDM_BITMAP_REVERSE,                 !strnicmp(Data->MoveGraphic.Type,"BM",2) ) ;
         EnableMenuItem ( Data->BitmapMenu, IDM_BITMAP_OPTIMIZE,                !strnicmp(Data->MoveGraphic.Type,"BM",2) ) ;
         EnableMenuItem ( Data->BitmapMenu, IDM_BITMAP_ASPECT,                  !strnicmp(Data->MoveGraphic.Type,"BM",2) || !strnicmp(Data->MoveGraphic.Type,"MF",2) ) ;
         CheckMenuItem  ( Data->BitmapMenu, IDM_BITMAP_BACKGROUND,              Data->MoveGraphic.Background ) ;
         CheckMenuItem  ( Data->BitmapMenu, IDM_BITMAP_NOTEXTASIDE,             Data->MoveGraphic.NoTextAside ) ;
         CheckMenuItem  ( Data->BitmapMenu, IDM_BITMAP_HALFTONE,                Data->MoveGraphic.Halftone ) ;
         CheckMenuItem  ( Data->BitmapMenu, IDM_BITMAP_REVERSE,                 Data->MoveGraphic.Reverse ) ;
         CheckMenuItem  ( Data->BitmapMenu, IDM_BITMAP_OPTIMIZE,                Data->MoveGraphic.PrinterOptimize ) ;
         CheckMenuItem  ( Data->BitmapMenu, IDM_BITMAP_POSITION_NEXT_CHAR,      Data->MoveGraphic.Position.x == GRAPHIC_NEXT_CHAR     ) ;
         CheckMenuItem  ( Data->BitmapMenu, IDM_BITMAP_POSITION_EDGE_LEFT,      Data->MoveGraphic.Position.x == GRAPHIC_EDGE_LEFT     ) ;
         CheckMenuItem  ( Data->BitmapMenu, IDM_BITMAP_POSITION_MARGIN_LEFT,    Data->MoveGraphic.Position.x == GRAPHIC_MARGIN_LEFT   ) ;
         CheckMenuItem  ( Data->BitmapMenu, IDM_BITMAP_POSITION_CENTERED_X,     Data->MoveGraphic.Position.x == GRAPHIC_CENTERED      ) ;
         CheckMenuItem  ( Data->BitmapMenu, IDM_BITMAP_POSITION_MARGIN_RIGHT,   Data->MoveGraphic.Position.x == GRAPHIC_MARGIN_RIGHT  ) ;
         CheckMenuItem  ( Data->BitmapMenu, IDM_BITMAP_POSITION_EDGE_RIGHT,     Data->MoveGraphic.Position.x == GRAPHIC_EDGE_RIGHT    ) ;
         CheckMenuItem  ( Data->BitmapMenu, IDM_BITMAP_POSITION_EDGE_TOP,       Data->MoveGraphic.Position.y == GRAPHIC_EDGE_TOP      ) ;
         CheckMenuItem  ( Data->BitmapMenu, IDM_BITMAP_POSITION_MARGIN_TOP,     Data->MoveGraphic.Position.y == GRAPHIC_MARGIN_TOP    ) ;
         CheckMenuItem  ( Data->BitmapMenu, IDM_BITMAP_POSITION_CENTERED_Y,     Data->MoveGraphic.Position.y == GRAPHIC_CENTERED      ) ;
         CheckMenuItem  ( Data->BitmapMenu, IDM_BITMAP_POSITION_MARGIN_BOTTOM,  Data->MoveGraphic.Position.y == GRAPHIC_MARGIN_BOTTOM ) ;
         CheckMenuItem  ( Data->BitmapMenu, IDM_BITMAP_POSITION_EDGE_BOTTOM,    Data->MoveGraphic.Position.y == GRAPHIC_EDGE_BOTTOM   ) ;
         CheckMenuItem  ( Data->BitmapMenu, IDM_BITMAP_SIZE_EDGETOEDGE_X,       Data->MoveGraphic.Size.cx == GRAPHIC_EDGE_TO_EDGE     ) ;
         CheckMenuItem  ( Data->BitmapMenu, IDM_BITMAP_SIZE_MARGINTOMARGIN_X,   Data->MoveGraphic.Size.cx == GRAPHIC_MARGIN_TO_MARGIN ) ;
         CheckMenuItem  ( Data->BitmapMenu, IDM_BITMAP_SIZE_EDGETOEDGE_Y,       Data->MoveGraphic.Size.cy == GRAPHIC_EDGE_TO_EDGE     ) ;
         CheckMenuItem  ( Data->BitmapMenu, IDM_BITMAP_SIZE_MARGINTOMARGIN_Y,   Data->MoveGraphic.Size.cy == GRAPHIC_MARGIN_TO_MARGIN ) ;
         CheckMenuItem  ( Data->BitmapMenu, IDM_BITMAP_ASPECT_FLOAT,            Data->MoveGraphic.Aspect == GRAPHIC_ASPECT_UNFIXED    ) ;
         CheckMenuItem  ( Data->BitmapMenu, IDM_BITMAP_ASPECT_FIX_X,            Data->MoveGraphic.Aspect == GRAPHIC_ASPECT_FIX_X      ) ;
         CheckMenuItem  ( Data->BitmapMenu, IDM_BITMAP_ASPECT_FIX_Y,            Data->MoveGraphic.Aspect == GRAPHIC_ASPECT_FIX_Y      ) ;
         WinPopupMenu ( HWND_DESKTOP, Window, Data->BitmapMenu, Point.x, Point.y, 0,
            PU_NONE | PU_HCONSTRAIN | PU_VCONSTRAIN | PU_KEYBOARD | PU_MOUSEBUTTON1 | PU_MOUSEBUTTON2 ) ;
         break; }

      case TEST_TEXT: {
         if ( Selected ) {
            WinPopupMenu ( HWND_DESKTOP, Window, Data->TextMenu, Point.x, Point.y, 0,
               PU_NONE | PU_HCONSTRAIN | PU_VCONSTRAIN | PU_KEYBOARD | PU_MOUSEBUTTON1 | PU_MOUSEBUTTON2 ) ;
         } else {
            WinPopupMenu ( HWND_DESKTOP, Window, Data->NullMenu, Point.x, Point.y, 0,
               PU_NONE | PU_HCONSTRAIN | PU_VCONSTRAIN | PU_KEYBOARD | PU_MOUSEBUTTON1 | PU_MOUSEBUTTON2 ) ;
         } /* endif */
         break; }

   } /* endswitch */

  /**************************************************************************
   * Done.                                                                  *
   **************************************************************************/

   return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process Button 1 Double-Click (Select Object)                       *
 *                                                                          *
 ****************************************************************************/

STATIC MRESULT APIENTRY Button1DblClk ( HWND Window, MESG, MPARAM1 mp1, MPARAM2 ) {

  /**************************************************************************
   * Find the instance data.                                                *
   **************************************************************************/

   PDATA Data = PDATA ( Sys_GetWindowData ( Window ) ) ;

  /**************************************************************************
   * Wait for queued editing to complete.                                   *
   **************************************************************************/

   Data->pDocument->FlushCommandQueue ( ) ;

  /**************************************************************************
   * Find out what the mouse touched.                                       *
   **************************************************************************/

   POINTL Point = { SHORT(SHORT1FROMMP(mp1)), SHORT(SHORT2FROMMP(mp1)) } ;
   POINTL ConvertedPoint = Point ;
   Data->pDocument->Transform ( CVTC_DEVICE, CVTC_DEFAULTPAGE, ConvertedPoint ) ;

  /**************************************************************************
   * Select the object pointed to.                                          *
   **************************************************************************/

   Data->pDocument->SelectWord ( ConvertedPoint ) ;

  /**************************************************************************
   * Done.                                                                  *
   **************************************************************************/

   return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process Activate state changes.                                     *
 *                                                                          *
 ****************************************************************************/

STATIC MRESULT APIENTRY Activate ( HWND Window, MESG, MPARAM1 mp1, MPARAM2 mp2 ) {

  /**************************************************************************
   * Find the instance data.                                                *
   **************************************************************************/

   PDATA Data = PDATA ( Sys_GetWindowData ( Window ) ) ;

  /**************************************************************************
   * Send message back to parent notifying of activation.                   *
   **************************************************************************/

   if ( SHORT1FROMMP ( mp1 ) )
      Sys_PostMessage ( Data->MainWindow, WM_ACTIVATED, mp2, 0 ) ;

  /**************************************************************************
   * We're done.                                                            *
   **************************************************************************/

   return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process Input Focus changes.                                        *
 *                                                                          *
 ****************************************************************************/

STATIC MRESULT APIENTRY SetFocus ( HWND Window, MESG, MPARAM1, MPARAM2 mp2 ) {

  /**************************************************************************
   * Find the instance data.                                                *
   **************************************************************************/

   PDATA Data = PDATA ( Sys_GetWindowData ( Window ) ) ;

  /**************************************************************************
   * Wait for queued editing to complete.                                   *
   **************************************************************************/

   Data->pDocument->FlushCommandQueue ( ) ;

  /**************************************************************************
   * If we're receiving the focus, create and show the cursor.              *
   **************************************************************************/

   if ( SHORT1FROMMP ( mp2 ) )
      Data->pDocument->UpdateCursor() ;

  /**************************************************************************
   * Else, destroy the cursor.                                              *
   **************************************************************************/

   else
      WinDestroyCursor ( Window ) ;

  /**************************************************************************
   * We're done.                                                            *
   **************************************************************************/

   return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Repaint entire window.                                              *
 *                                                                          *
 ****************************************************************************/

STATIC MRESULT APIENTRY Paint ( HWND Window, MESG, MPARAM1, MPARAM2 ) {

  /**************************************************************************
   * Find the instance data.                                                *
   **************************************************************************/

   PDATA Data = PDATA ( Sys_GetWindowData ( Window ) ) ;

  /**************************************************************************
   * Paint the invalid regions.                                             *
   **************************************************************************/

   Data->pDocument->Paint ( ) ;

  /**************************************************************************
   * We're done.                                                            *
   **************************************************************************/

   return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process Presentation Parameter Changed notification.                *
 *                                                                          *
 ****************************************************************************/

STATIC MRESULT APIENTRY PresParamChanged ( HWND Window, MESG msg, MPARAM1 mp1, MPARAM2 mp2 ) {

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
         HWND Frame = PARENT ( Window ) ;
         WinSetPresParam ( CHILD(Frame,FID_STATUSLINE), PP_FONTNAMESIZE, strlen(Font)+1, PSZ(Font) ) ;
         WinSetPresParam ( CHILD(Frame,FID_HORZRULER), PP_FONTNAMESIZE, strlen(Font)+1, PSZ(Font) ) ;
         WinSetPresParam ( CHILD(Frame,FID_VERTRULER), PP_FONTNAMESIZE, strlen(Font)+1, PSZ(Font) ) ;
         Data->SettingFont = FALSE ;
         break ; }

     /***********************************************************************
      * If background color, note the fact and repaint the window.          *
      ***********************************************************************/

      case PP_BACKGROUNDCOLOR: {
         ULONG ppid ;
         if ( WinQueryPresParam ( Window, PP_BACKGROUNDCOLOR, 0, &ppid, sizeof(Data->IniData->DocColors[0]), &Data->IniData->DocColors[0], 0 ) ) {
            Data->IniData->fDocColors[0] = TRUE ;
         } else {
            Data->IniData->DocColors[0] = RGB_WHITE ;
            Data->IniData->fDocColors[0] = FALSE ;
         } /* endif */
         COLOR Colors[2] ;
         Colors[0] = Data->IniData->fDocColors[0] ? Data->IniData->DocColors[0] : RGB_WHITE ;
         Colors[1] = Data->IniData->fDocColors[1] ? Data->IniData->DocColors[1] : RGB_BLUE ;
         Sys_PostMessage ( Data->MainWindow, WM_SET_DOCCOLORS, MPFROMLONG(Colors[0]), MPFROMLONG(Colors[1]) ) ;
         break ; }

     /***********************************************************************
      * If foreground color, note the fact and repaint the window.          *
      ***********************************************************************/

      case PP_FOREGROUNDCOLOR: {
         ULONG ppid ;
         if ( WinQueryPresParam ( Window, PP_FOREGROUNDCOLOR, 0, &ppid, sizeof(Data->IniData->DocColors[1]), &Data->IniData->DocColors[1], 0 ) ) {
            Data->IniData->fDocColors[1] = TRUE ;
         } else {
            Data->IniData->DocColors[1] = RGB_BLUE ;
            Data->IniData->fDocColors[1] = FALSE ;
         } /* endif */
         COLOR Colors[2] ;
         Colors[0] = Data->IniData->fDocColors[0] ? Data->IniData->DocColors[0] : RGB_WHITE ;
         Colors[1] = Data->IniData->fDocColors[1] ? Data->IniData->DocColors[1] : RGB_BLUE ;
         Sys_PostMessage ( Data->MainWindow, WM_SET_DOCCOLORS, MPFROMLONG(Colors[0]), MPFROMLONG(Colors[1]) ) ;
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
 *      Process Drag Over Notification                                      *
 *                                                                          *
 ****************************************************************************/

STATIC MRESULT APIENTRY DragOver ( HWND, MESG, MPARAM1 mp1, MPARAM2 ) {

  /**************************************************************************
   * Find the instance data.                                                *
   **************************************************************************/

// PDATA Data = PDATA ( Sys_GetWindowData ( Window ) ) ;

  /**************************************************************************
   * Determine whether or not a drop would be possible here.                *
   **************************************************************************/

   PDRAGINFO pDraginfo = (PDRAGINFO) mp1 ;
   DrgAccessDraginfo ( pDraginfo ) ;
   USHORT Indicator = DOR_DROP, Operation = DO_COPY ;
   switch ( pDraginfo->usOperation ) {
      case DO_COPY:
      case DO_DEFAULT: {
         for ( int i=0; i<DrgQueryDragitemCount(pDraginfo); i++ ) {
            PDRAGITEM pDragitem = DrgQueryDragitemPtr ( pDraginfo, i ) ;
            if ( ! ( pDragitem->fsSupportedOps & DO_COPYABLE ) ) {
               Indicator = DOR_NODROPOP ;
               Operation = 0 ;
               break ;
            } /* endif */
            if ( !DrgVerifyRMF ( pDragitem, "DRM_OS2FILE", "DRF_TEXT" ) ) {
               Indicator = DOR_NEVERDROP ;
               break ;
            } /* endif */
         } /* endfor */
         break; }
      default:
         DrgFreeDraginfo ( pDraginfo ) ;
         return ( MRFROM2SHORT ( DOR_NODROPOP, 0 ) ) ;
   } /* endswitch */
   DrgFreeDraginfo ( pDraginfo ) ;

  /**************************************************************************
   * Done.  Report final status.                                            *
   **************************************************************************/

   return ( MRFROM2SHORT ( Indicator, Operation ) ) ;
}

/****************************************************************************
 *                                                                          *
 *      Process Drag Leave Notification                                     *
 *                                                                          *
 ****************************************************************************/

STATIC MRESULT APIENTRY DragLeave ( HWND, MESG, MPARAM1, MPARAM2 ) {

  /**************************************************************************
   * Done.                                                                  *
   **************************************************************************/

   return ( 0 ) ;
}

/****************************************************************************
 *                                                                          *
 *      Process Drop Notification                                           *
 *                                                                          *
 ****************************************************************************/

STATIC MRESULT APIENTRY Drop ( HWND Window, MESG, MPARAM1 mp1, MPARAM2 ) {

  /**************************************************************************
   * Find the instance data.                                                *
   **************************************************************************/

   PDATA Data = PDATA ( Sys_GetWindowData ( Window ) ) ;

  /**************************************************************************
   * Accept the dropped files if possible,                                  *
   **************************************************************************/

   PDRAGINFO pDraginfo = (PDRAGINFO) mp1 ;
   DrgAccessDraginfo ( pDraginfo ) ;
   USHORT Indicator = DOR_DROP, Operation = DO_COPY ;
// PDRAGTRANSFER pDragTransfer = DrgAllocDragtransfer ( DrgQueryDragitemCount ( pDraginfo ) ) ;
// HSTR hstrRMF  = DrgAddStrHandle ( "<DRM_OS2FILE, DRF_TEXT>" ) ;
   for ( int i=0; i<DrgQueryDragitemCount(pDraginfo); i++ ) {
      PDRAGITEM pDragitem = DrgQueryDragitemPtr ( pDraginfo, i ) ;
      if ( pDragitem->hstrSourceName ) {
         char Container [CCHMAXPATH] ;
         DrgQueryStrName ( pDragitem->hstrContainerName, sizeof(Container), Container ) ;
         char Source [CCHMAXPATH] ;
         DrgQueryStrName ( pDragitem->hstrSourceName, sizeof(Source), Source ) ;
         char FullPath [_MAX_PATH] ;
         strcpy ( FullPath, Container ) ;
         strcat ( FullPath, Source ) ;

         // What happens now depends on the current document state and what
         //   exactly it is that is being dropped.

         // If the current document is empty and unchanged, then we close it
         //   and accept the new document as a replacement.  Simply start it loading.
         if ( ( Data->pDocument->Query_Filename() == 0 ) && ( Data->pDocument->IsChanged() == FALSE ) ) {

            // Build a command-line string.
            char Parms [512] = { 0 } ;
            int ParmLength = 0 ;
            strcpy ( Parms, FullPath ) ;
            ParmLength += strlen(FullPath) + 1 ;
            Parms[++ParmLength] = 0 ;

            // Allocate shared memory to hold the current path.
            PVOID Memory1 ;
            DosAllocMem ( &Memory1, strlen(HomePath)+1, fALLOC ) ;
            strcpy ( PCHAR(Memory1), HomePath ) ;

            // Allocate shared memory to hold the command-line.
            PVOID Memory2 ;
            DosAllocMem ( &Memory2, ParmLength, fALLOC ) ;
            memcpy ( Memory2, Parms, ParmLength ) ;
     
            // Pass the information to the original process.
            Sys_PostMessage ( Data->MainWindow, WM_LOAD_FILE, MPFROMP(Memory1), MPFROMP(Memory2) ) ;

         // Otherwise, we perform a file merge.
         } else {

            int FileTypeIndex = GetFileTypeIndex ( Window, FullPath, FileTypeList, FileExtenderList, 0 ) ;

            if ( FileTypeIndex == 0 ) {
               ResourceString Title ( Data->Library->QueryHandle(), IDS_TITLE ) ;
               ResourceString Format ( Data->Library->QueryHandle(), IDS_ERROR_NOFILETYPE ) ;
               char Message [300] ;
               sprintf ( Message, PCHAR(Format), FullPath ) ;
               MessageBox ( HWND_DESKTOP, Window, PSZ(Message),
                  PSZ(Title), 0, MB_ENTER | MB_ICONEXCLAMATION, LibraryHandle, IDS_MESSAGE_BOX_STRINGS ) ;

            } else {
               Addon *Importer = FileExtenderList[FileTypeIndex>0?FileTypeIndex:1] ; 
               ResourceString Merging ( Data->Library->QueryHandle(), IDS_MERGING ) ;
               Sys_SetWindowText ( Data->StatusWindow, PSZ(Merging) ) ;
               Sys_ShowWindow ( Data->StatusWindow, TRUE ) ;
               Data->pDocument->MergeFile ( FullPath, Data->IniData->DocParms, Importer ) ;
               Sys_ShowWindow ( Data->StatusWindow, FALSE ) ;

            } /* endif */

         } /* endif */

      } /* endif */
   } /* endfor */

   // Release the drag information.
   DrgFreeDraginfo ( pDraginfo ) ;

  /**************************************************************************
   * Done.  Report final status.                                            *
   **************************************************************************/

   return ( MRFROM2SHORT ( Indicator, Operation ) ) ;
}

/****************************************************************************
 *                                                                          *
 *      Process control messages received by the Main Window.               *
 *                                                                          *
 ****************************************************************************/

STATIC MRESULT APIENTRY Control ( HWND, MESG, MPARAM1 mp1, MPARAM2 mp2 ) {

  /**************************************************************************
   * Decode the message.  Find out what control sent it, and what the       *
   *   control had to say.                                                  *
   **************************************************************************/

   USHORT id = SHORT1FROMMP ( mp1 ) ;
   USHORT Message = SHORT2FROMMP ( mp1 ) ;

  /**************************************************************************
   * If a button requested painting, do it now.                             *
   **************************************************************************/

   if ( Message == BN_PAINT ) {

      PUSERBUTTON UserButton = PUSERBUTTON ( mp2 ) ;

      RECTL Rectangle ;
      WinQueryWindowRect ( UserButton->hwnd, &Rectangle ) ;

      HPS hPS = UserButton->hps ;
      GpiCreateLogColorTable ( hPS, LCOL_RESET, LCOLF_RGB, 0, 0, 0 ) ;

      COLOR Light = WinQuerySysColor ( HWND_DESKTOP, SYSCLR_BUTTONLIGHT, 0 ) ;
      COLOR Middle = WinQuerySysColor ( HWND_DESKTOP, SYSCLR_BUTTONMIDDLE, 0 ) ;
      COLOR Dark = WinQuerySysColor ( HWND_DESKTOP, SYSCLR_BUTTONDARK, 0 ) ;

      GpiSetColor ( hPS, Dark ) ;
      POINTL Point ;
      Point.x = Rectangle.xLeft ;
      Point.y = Rectangle.yBottom ;
      GpiMove ( hPS, &Point ) ;
      Point.x = Rectangle.xRight - 1 ;
      Point.y = Rectangle.yTop - 1 ;
      GpiBox ( hPS, DRO_OUTLINE, &Point, 2, 2 ) ;

      GpiSetColor ( hPS, ( UserButton->fsState & BDS_HILITED ) ? Dark : Light ) ;
      Point.x = Rectangle.xLeft + 1 ;
      Point.y = Rectangle.yBottom + 2 ;
      GpiMove ( hPS, &Point ) ;
      Point.y = Rectangle.yTop - 2 ;
      GpiLine ( hPS, &Point ) ;
      Point.x = Rectangle.xRight - 2 ;
      GpiLine ( hPS, &Point ) ;

      GpiSetColor ( hPS, ( UserButton->fsState & BDS_HILITED ) ? Light : Dark ) ;
      Point.x = Rectangle.xLeft + 1 ;
      Point.y = Rectangle.yBottom + 1 ;
      GpiMove ( hPS, &Point ) ;
      Point.x = Rectangle.xRight - 2 ;
      GpiLine ( hPS, &Point ) ;
      Point.y = Rectangle.yTop - 3 ;
      GpiLine ( hPS, &Point ) ;

      GpiSetColor ( hPS, Middle ) ;
      Point.x = Rectangle.xLeft + 2 ;
      Point.y = Rectangle.yBottom + 2 ;
      GpiMove ( hPS, &Point ) ;
      Point.x = Rectangle.xRight - 3 ;
      Point.y = Rectangle.yTop - 3 ;
      GpiBox ( hPS, DRO_FILL, &Point, 0, 0 ) ;

      HBITMAP Bitmap ;
      switch ( id ) {
         case FID_TOP:
            Bitmap = GpiLoadBitmap ( hPS, 0, ID_TOP, 0, 0 ) ;
            break ;
         case FID_BOTTOM:
            Bitmap = GpiLoadBitmap ( hPS, 0, ID_BOTTOM, 0, 0 ) ;
            break ;
         case FID_PAGEUP:
            Bitmap = GpiLoadBitmap ( hPS, 0, ID_PGUP, 0, 0 ) ;
            break ;
         case FID_PAGEDOWN:
            Bitmap = GpiLoadBitmap ( hPS, 0, ID_PGDN, 0, 0 ) ;
            break ;
      } /* endswitch */

      GpiSetColor ( hPS, RGB_BLACK ) ;

      BITMAPINFOHEADER BitmapInfoHeader ;
      GpiQueryBitmapParameters ( Bitmap, &BitmapInfoHeader ) ;
      Point.x = ( Rectangle.xRight - Rectangle.xLeft - BitmapInfoHeader.cx ) / 2 ;
      Point.y = ( Rectangle.yTop - Rectangle.yBottom - BitmapInfoHeader.cy ) / 2 ;

      if ( UserButton->fsState & BDS_HILITED ) {
         Point.x ++ ;
         Point.y -- ;
      } /* endif */

      WinDrawBitmap ( hPS, Bitmap, 0, &Point, 0, 0, DBM_IMAGEATTRS |
         ( UserButton->fsState & BDS_DISABLED ? DBM_HALFTONE : DBM_NORMAL ) ) ;

      GpiDeleteBitmap ( Bitmap ) ;

      return ( 0 ) ;
   }

  /**************************************************************************
   * Return zero, indicating that the message was processed.                *
   **************************************************************************/

   return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process commands received by Main Window                            *
 *                                                                          *
 ****************************************************************************/

STATIC MRESULT APIENTRY Command ( HWND Window, MESG msg, MPARAM1 mp1, MPARAM2 mp2 ) {

  /**************************************************************************
   * Find the instance data.                                                *
   **************************************************************************/

   PDATA Data = PDATA ( Sys_GetWindowData ( Window ) ) ;

  /**************************************************************************
   * Extract the command code.                                              *
   **************************************************************************/

   USHORT Command = SHORT1FROMMP ( mp1 ) ;

  /**************************************************************************
   * Pass font selection messages to the main window.                       *
   **************************************************************************/

   if ( ( Command >= IDM_FONTS ) AND ( Command <= IDM_FONTS_MAX ) )
      return ( Sys_SendMessage ( Data->MainWindow, msg, mp1, mp2 ) ) ;

  /**************************************************************************
   * Pass size selection messages to the main window.                       *
   **************************************************************************/

   if ( ( Command >= IDM_SIZES ) AND ( Command <= IDM_SIZES_MAX ) )
      return ( Sys_SendMessage ( Data->MainWindow, msg, mp1, mp2 ) ) ;

  /**************************************************************************
   * Pass expand selection messages to the main window.                     *
   **************************************************************************/

   if ( ( Command >= IDM_EXPANDS ) AND ( Command <= IDM_EXPANDS_MAX ) )
      return ( Sys_SendMessage ( Data->MainWindow, msg, mp1, mp2 ) ) ;

  /**************************************************************************
   * Pass color selection messages to the main window.                      *
   **************************************************************************/

   if ( ( Command >= IDM_LINECOLORS ) AND ( Command <= IDM_LINECOLORS_MAX ) )
      return ( Sys_SendMessage ( Data->MainWindow, msg, mp1, mp2 ) ) ;

  /**************************************************************************
   * Pass color selection messages to the main window.                      *
   **************************************************************************/

   if ( ( Command >= IDM_LINEWIDTHS ) AND ( Command <= IDM_LINEWIDTHS_MAX ) )
      return ( Sys_SendMessage ( Data->MainWindow, msg, mp1, mp2 ) ) ;

  /**************************************************************************
   * Pass color selection messages to the main window.                      *
   **************************************************************************/

   if ( ( Command >= IDM_FILLCOLORS ) AND ( Command <= IDM_FILLCOLORS_MAX ) )
      return ( Sys_SendMessage ( Data->MainWindow, msg, mp1, mp2 ) ) ;

  /**************************************************************************
   * Pass color selection messages to the main window.                      *
   **************************************************************************/

   if ( ( Command >= IDM_FILLTYPES ) AND ( Command <= IDM_FILLTYPES_MAX ) )
      return ( Sys_SendMessage ( Data->MainWindow, msg, mp1, mp2 ) ) ;

  /**************************************************************************
   * Process line end selection messages.                                   *
   **************************************************************************/

   if ( ( Command >= IDM_LINEENDS ) AND ( Command <= IDM_LINEENDS_MAX ) )
      return ( Sys_SendMessage ( Data->MainWindow, msg, mp1, mp2 ) ) ;

  /**************************************************************************
   * Process line join selection messages.                                  *
   **************************************************************************/

   if ( ( Command >= IDM_LINEJOINS ) AND ( Command <= IDM_LINEJOINS_MAX ) ) 
      return ( Sys_SendMessage ( Data->MainWindow, msg, mp1, mp2 ) ) ;

  /**************************************************************************
   * Process all other command messages.                                    *
   **************************************************************************/

   static METHOD Methods [] = {

      { IDM_MERGE,                      FileMerge          },
      { IDM_SAVE,                       FileSave           },
      { IDM_SAVEAS,                     FileSaveAs         },
      { IDM_SAVE_CLOSE,                 FileSaveClose      },
      { IDM_CLOSE,                      Close              },
      { IDM_PRINT_ALL,                  PrintAll           },
      { IDM_PRINT_ALL_NORMAL,           PrintAllNormal     },
      { IDM_PRINT_ALL_REVERSE,          PrintAllReverse    },
      { IDM_PRINT_PAGE,                 PrintPage          },
      { IDM_DEFAULTS,                   SendCommandToMain  },

      { IDM_UNDO,                       Undo               },
      { IDM_REDO,                       Redo               },
      { IDM_CUT,                        Cut                },
      { IDM_COPY,                       Copy               },
      { IDM_PASTE,                      Paste              },
      { IDM_CLEAR,                      Clear              },
      { IDM_SELECT_ALL,                 SelectAll          },
      { IDM_PASTE_FROM_CLIP_BITMAP,     PasteBitmap        },
      { IDM_PASTE_FROM_CLIP_METAFILE,   PasteMetafile      },
      { IDM_PASTE_FROM_SCRN_ENTIRE,     PasteScreenEntire  },
      { IDM_PASTE_FROM_SCRN_FRAME,      PasteScreenFrame   },
      { IDM_PASTE_FROM_SCRN_WINDOW,     PasteScreenWindow  },
      { IDM_PASTE_FROM_SCRN_RECT,       PasteScreenRect    },
      { IDM_PASTE_FROM_FILE,            PasteGraphic       },
      { IDM_SEARCH,                     Search             },
      { IDM_FIND,                       Find               },
      { IDM_FINDNEXT,                   FindNext           },
      { IDM_CHANGE,                     Change             },
      { IDM_CHANGEFIND,                 ChangeFind         },
      { IDM_CHANGEALL,                  ChangeAll          },
      { IDM_GOTOPAGE,                   GotoPage           },
      { IDM_SPELL,                      SpellCheck         },
      { IDM_ZOOM_100,                   Zoom100            },
      { IDM_ZOOM_FULLWIDTH,             ZoomFullWidth      },
      { IDM_ZOOM_FULLHEIGHT,            ZoomFullHeight     },
      { IDM_ZOOM_MARGINS,               ZoomMargins        },
      { IDM_ZOOM_ALL,                   ZoomAll            },
      { IDM_ZOOM_WINDOW,                ZoomWindow         },
      { IDM_ZOOM_SET,                   ZoomSet            },

      { IDM_MARGINS,                    SetMargins         },
      { IDM_TABS,                       SetTabs            },
      { IDM_INDENT,                     SetIndent          },
      { IDM_JUST_LEFT,                  SetJustLeft        },
      { IDM_JUST_CENTER,                SetJustCenter      },
      { IDM_JUST_RIGHT,                 SetJustRight       },
      { IDM_JUST_FULL,                  SetJustFull        },
      { IDM_PAGECENTER,                 SetPageCenter      },
      { IDM_PAGEBREAK,                  PageBreak          },
      { IDM_DATETODAY,                  DateToday          },
      { IDM_DATECURRENT,                DateCurrent        },
      { IDM_DATECHANGED,                DateChanged        },
      { IDM_DATEFMT,                    SetDateFormat      },
      { IDM_CURRENTPAGE,                CurrentPage        },
      { IDM_PAGECOUNT,                  PageCount          },
      { IDM_SEPARATOR,                  Separator          },
      { IDM_BARCODE,                    BarCode            },
      { IDM_HEADER,                     Header             },
      { IDM_FOOTER,                     Footer             },

      { IDM_FONTS_PGUP,                 FontsPageUp        },
      { IDM_FONTS_PGDN,                 FontsPageDown      },
      { IDM_FONTMGR,                    FontManager        },

      { IDM_SIZES_PGUP,                 SizesPageUp        },
      { IDM_SIZES_PGDN,                 SizesPageDown      },
      { IDM_SIZEMGR,                    SizeManager        },

      { IDM_NORMAL,                     SetNormal          },
      { IDM_BOLD,                       SetBold            },
      { IDM_ITALIC,                     SetItalic          },
      { IDM_UNDERSCORE,                 SetUnderscore      },
      { IDM_STRIKEOUT,                  SetStrikeout       },
      { IDM_OUTLINE,                    SetOutline         },
      { IDM_CAPS,                       SetCaps            },

      { IDM_EXPANDS_PGUP,               ExpandPageUp       },
      { IDM_EXPANDS_PGDN,               ExpandPageDown     },
      { IDM_EXPANDMGR,                  ExpandManager      },
      { IDM_LINECOLORS_PGUP,            LineColorsPageUp   },
      { IDM_LINECOLORS_PGDN,            LineColorsPageDown },
      { IDM_LINECOLORMGR,               LineColorManager   },
      { IDM_LINEWIDTHS_PGUP,            LineWidthsPageUp   },
      { IDM_LINEWIDTHS_PGDN,            LineWidthsPageDown },
      { IDM_LINEWIDTHMGR,               LineWidthManager   },
      { IDM_FILLCOLORS_PGUP,            FillColorsPageUp   },
      { IDM_FILLCOLORS_PGDN,            FillColorsPageDown },
      { IDM_FILLCOLORMGR,               FillColorManager   },
      { IDM_FILLTYPES_PGUP,             FillTypesPageUp    },
      { IDM_FILLTYPES_PGDN,             FillTypesPageDown  },
      { IDM_FILLTYPEMGR,                FillTypeManager    },
      { IDM_LINEENDS_PGUP,              LineEndsPageUp     },
      { IDM_LINEENDS_PGDN,              LineEndsPageDown   },
      { IDM_LINEENDMGR,                 LineEndManager     },
      { IDM_LINEJOINS_PGUP,             LineJoinsPageUp    },
      { IDM_LINEJOINS_PGDN,             LineJoinsPageDown  },
      { IDM_LINEJOINMGR,                LineJoinManager    },

      { IDM_BITMAP_BACKGROUND,          Bitmap_Command     },
      { IDM_BITMAP_NOTEXTASIDE,         Bitmap_Command     },
      { IDM_BITMAP_HALFTONE,            Bitmap_Command     },
      { IDM_BITMAP_REVERSE,             Bitmap_Command     },
      { IDM_BITMAP_OPTIMIZE,            Bitmap_Command     },
      { IDM_BITMAP_POSITION_NEXT_CHAR,  Bitmap_Command     },
      { IDM_BITMAP_POSITION_EDGE_LEFT,  Bitmap_Command     },
      { IDM_BITMAP_POSITION_EDGE_RIGHT, Bitmap_Command     },
      { IDM_BITMAP_POSITION_EDGE_TOP,   Bitmap_Command     },
      { IDM_BITMAP_POSITION_EDGE_BOTTOM,Bitmap_Command     },
      { IDM_BITMAP_POSITION_MARGIN_LEFT,Bitmap_Command     },
      { IDM_BITMAP_POSITION_MARGIN_RIGHT,Bitmap_Command    },
      { IDM_BITMAP_POSITION_MARGIN_TOP, Bitmap_Command     },
      { IDM_BITMAP_POSITION_MARGIN_BOTTOM,Bitmap_Command   },
      { IDM_BITMAP_POSITION_CENTERED_X, Bitmap_Command     },
      { IDM_BITMAP_POSITION_CENTERED_Y, Bitmap_Command     },
      { IDM_BITMAP_SIZE_EDGETOEDGE_X,   Bitmap_Command     },
      { IDM_BITMAP_SIZE_EDGETOEDGE_Y,   Bitmap_Command     },
      { IDM_BITMAP_SIZE_MARGINTOMARGIN_X,Bitmap_Command    },
      { IDM_BITMAP_SIZE_MARGINTOMARGIN_Y,Bitmap_Command    },
      { IDM_BITMAP_ASPECT_FLOAT,        Bitmap_Command     },
      { IDM_BITMAP_ASPECT_FIX_X,        Bitmap_Command     },
      { IDM_BITMAP_ASPECT_FIX_Y,        Bitmap_Command     },

      { IDM_BORDER_SIZE,                Border_Command     },
      { IDM_BORDER_COLOR,               Border_Command     },
      { IDM_BORDER_CAPTION,             Border_Command     },

      { IDM_REFRESH,                    Default_Refresh    },

      { IDM_DUMP,                       Dump               },

      { FID_TOP,                        Top                },
      { FID_PAGEUP,                     PageUp             },
      { FID_PAGEDOWN,                   PageDown           },
      { FID_BOTTOM,                     Bottom             },
      { FID_CORNER1,                    SelectAll          },
   } ;

   return ( DispatchMessage ( Window, SHORT1FROMMP(mp1), mp1, mp2, Methods, sizeof(Methods)/sizeof(Methods[0]), 0 ) ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Send WM_COMMAND Message to Main Window.                             *
 *                                                                          *
 ****************************************************************************/

STATIC MRESULT APIENTRY SendCommandToMain ( HWND Window, MESG, MPARAM1 mp1, MPARAM2 mp2 ) {

  /**************************************************************************
   * Find the instance data.                                                *
   **************************************************************************/

   PDATA Data = PDATA ( Sys_GetWindowData ( Window ) ) ;

  /**************************************************************************
   * Send message to the main window.  Return the result.                   *
   **************************************************************************/

   Sys_PostMessage ( Data->MainWindow, WM_COMMAND, mp1, mp2 ) ;

  /**************************************************************************
   * Done.                                                                  *
   **************************************************************************/

   return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process File Merge command.                                         *
 *                                                                          *
 ****************************************************************************/

STATIC MRESULT APIENTRY FileMerge ( HWND Window, MESG, MPARAM1, MPARAM2 ) {

  /**************************************************************************
   * Find the instance data.                                                *
   **************************************************************************/

   PDATA Data = PDATA ( Sys_GetWindowData ( Window ) ) ;

  /**************************************************************************
   * Display the system File Open dialog.                                   *
   **************************************************************************/

   ResourceString MergeDocument ( Data->Library->QueryHandle(), IDS_MERGE_DOCUMENT ) ;
   PrepareFileTypeList ( ) ;
   FILEDIALOG_DATA FileDialogData = { TRUE, FileTypeList, FileWildcardList, FileExtenderList, 0 } ;
   FILEDLG DialogData ;
   memset ( &DialogData, 0, sizeof(DialogData) ) ;
   DialogData.cbSize = sizeof(DialogData) ;
   DialogData.fl = FDS_OPEN_DIALOG | FDS_CENTER | FDS_HELPBUTTON | FDS_FILTERUNION ;
   DialogData.pszTitle = PSZ(MergeDocument) ;
   strcpy ( DialogData.szFullFile, FileWildcardList[0] ) ;
   DialogData.pszIType = Data->IniData->FileType[0] ? Data->IniData->FileType : FileTypeList[0] ;
   DialogData.papszITypeList = PAPSZ(FileTypeList) ;
   DialogData.pfnDlgProc = FileDialog_Processor ;
   DialogData.ulUser = ULONG ( &FileDialogData ) ;
   WinFileDlg ( HWND_DESKTOP, Window, &DialogData ) ;

  /**************************************************************************
   * If the dialog failed, return.                                          *
   **************************************************************************/

   if ( DialogData.lReturn != DID_OK )
      return ( 0 ) ;

  /**************************************************************************
   * Save the most recently selected file type.                             *
   **************************************************************************/

   strcpy ( Data->IniData->FileType, FileDialogData.FilterIndex > 0 ? FileTypeList[FileDialogData.FilterIndex] : "" ) ;

  /**************************************************************************
   * Set the merging status.                                                *
   **************************************************************************/

   ResourceString Merging ( Data->Library->QueryHandle(), IDS_MERGING ) ;
   Sys_SetWindowText ( Data->StatusWindow, PSZ(Merging) ) ;
   Sys_ShowWindow ( Data->StatusWindow, TRUE ) ;

  /**************************************************************************
   * Merge the new file in to the document.                                 *
   **************************************************************************/

   Data->pDocument->MergeFile ( DialogData.szFullFile, Data->IniData->DocParms, 
      (ImportExport*)FileExtenderList[FileDialogData.Index>0?FileDialogData.Index:1] ) ;

  /**************************************************************************
   * Turn off the merging status display.                                   *
   **************************************************************************/

   Sys_ShowWindow ( Data->StatusWindow, FALSE ) ;

  /**************************************************************************
   * Done.                                                                  *
   **************************************************************************/

   return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process File Save command.                                          *
 *                                                                          *
 ****************************************************************************/

STATIC MRESULT APIENTRY FileSave ( HWND Window, MESG, MPARAM1, MPARAM2 ) {

  /**************************************************************************
   * Find the instance data.                                                *
   **************************************************************************/

   PDATA Data = PDATA ( Sys_GetWindowData ( Window ) ) ;

  /**************************************************************************
   * If the file has no name, prompt for one.                               *
   **************************************************************************/

   PSZ Filename = Data->pDocument->Query_Filename ( ) ;
   BOOL Prompt = ( Filename == 0 ) OR ( Filename[0] == 0 ) ;

  /**************************************************************************
   * Save the file.                                                         *
   **************************************************************************/

   Save ( Window, Data, Prompt ) ;

  /**************************************************************************
   * Done.                                                                  *
   **************************************************************************/

   return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process File Save As command.                                       *
 *                                                                          *
 ****************************************************************************/

STATIC MRESULT APIENTRY FileSaveAs ( HWND Window, MESG, MPARAM1, MPARAM2 ) {

  /**************************************************************************
   * Find the instance data.                                                *
   **************************************************************************/

   PDATA Data = PDATA ( Sys_GetWindowData ( Window ) ) ;

  /**************************************************************************
   * Save the file, prompting for a new name first.                         *
   **************************************************************************/

   Save ( Window, Data, TRUE ) ;

  /**************************************************************************
   * Done.                                                                  *
   **************************************************************************/

   return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process File Save & Close command.                                  *
 *                                                                          *
 ****************************************************************************/

STATIC MRESULT APIENTRY FileSaveClose ( HWND Window, MESG, MPARAM1, MPARAM2 ) {

  /**************************************************************************
   * Find the instance data.                                                *
   **************************************************************************/

   PDATA Data = PDATA ( Sys_GetWindowData ( Window ) ) ;

  /**************************************************************************
   * If the file has changed, save it.  Abort save if so requested.         *
   **************************************************************************/

   if ( Data->pDocument->IsChanged() ) {
      PSZ Filename = Data->pDocument->Query_Filename ( ) ;
      BOOL Prompt = ( Filename == 0 ) OR ( Filename[0] == 0 ) ;
      if ( !Save ( Window, Data, Prompt ) )
         return ( 0 ) ;
   } /* endif */

  /**************************************************************************
   * Start the document close process.                                      *
   **************************************************************************/

   HWND Frame = PARENT ( Window ) ;
   Sys_PostMessage ( Data->MainWindow, WM_CLOSE_ME, MPFROMP(Frame), MPFROMLONG(FALSE) ) ;

  /**************************************************************************
   * Done.                                                                  *
   **************************************************************************/

   return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process Print All command.                                          *
 *                                                                          *
 ****************************************************************************/

STATIC MRESULT APIENTRY PrintAll ( HWND Window, MESG, MPARAM1, MPARAM2 ) {

  /**************************************************************************
   * Find the instance data.                                                *
   **************************************************************************/

   PDATA Data = PDATA ( Sys_GetWindowData ( Window ) ) ;

  /**************************************************************************
   * Print all pages in normal or reverse order, according to preferences.  *
   **************************************************************************/

   Print ( Window, ALL, Data->IniData->PrintReverse ) ;

  /***************************************************************************
   * Done.                                                                   *
   ***************************************************************************/

   return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process Print All Normal command.                                   *
 *                                                                          *
 ****************************************************************************/

STATIC MRESULT APIENTRY PrintAllNormal ( HWND Window, MESG, MPARAM1, MPARAM2 ) {

  /**************************************************************************
   * Print all pages in normal order.                                       *
   **************************************************************************/

   Print ( Window, ALL, FALSE ) ;

  /***************************************************************************
   * Done.                                                                   *
   ***************************************************************************/

   return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process Print All Normal command.                                   *
 *                                                                          *
 ****************************************************************************/

STATIC MRESULT APIENTRY PrintAllReverse ( HWND Window, MESG, MPARAM1, MPARAM2 ) {

  /**************************************************************************
   * Print all pages in reverse order.                                      *
   **************************************************************************/

   Print ( Window, ALL, TRUE ) ;

  /***************************************************************************
   * Done.                                                                   *
   ***************************************************************************/

   return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process Print Page command.                                         *
 *                                                                          *
 ****************************************************************************/

STATIC MRESULT APIENTRY PrintPage ( HWND Window, MESG, MPARAM1, MPARAM2 ) {

  /**************************************************************************
   * Find the instance data.                                                *
   **************************************************************************/

   PDATA Data = PDATA ( Sys_GetWindowData ( Window ) ) ;

  /**************************************************************************
   * Print the current page.                                                *
   **************************************************************************/

   Print ( Window, Data->pDocument->Query_PageNumber(), FALSE ) ;

  /**************************************************************************
   * Done.                                                                  *
   **************************************************************************/

   return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process Undo command.                                               *
 *                                                                          *
 ****************************************************************************/

STATIC MRESULT APIENTRY Undo ( HWND Window, MESG, MPARAM1, MPARAM2 ) {

  /**************************************************************************
   * Find the instance data.                                                *
   **************************************************************************/

   PDATA Data = PDATA ( Sys_GetWindowData ( Window ) ) ;

  /**************************************************************************
   * Request one level of undo.                                             *
   **************************************************************************/

   Data->pDocument->Enqueue_Undo ( ) ;

  /**************************************************************************
   * Done.                                                                  *
   **************************************************************************/

   return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process Redo command.                                               *
 *                                                                          *
 ****************************************************************************/

STATIC MRESULT APIENTRY Redo ( HWND Window, MESG, MPARAM1, MPARAM2 ) {

  /**************************************************************************
   * Find the instance data.                                                *
   **************************************************************************/

   PDATA Data = PDATA ( Sys_GetWindowData ( Window ) ) ;

  /**************************************************************************
   * Request one level of redo.                                             *
   **************************************************************************/

   Data->pDocument->Enqueue_Redo ( ) ;

  /**************************************************************************
   * Done.                                                                  *
   **************************************************************************/

   return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process Cut command.                                                *
 *                                                                          *
 ****************************************************************************/

STATIC MRESULT APIENTRY Cut ( HWND Window, MESG, MPARAM1 mp1, MPARAM2 mp2 ) {

  /**************************************************************************
   * Process the Cut command by executing a Copy, then a Clear.             *
   **************************************************************************/

   Copy ( Window, IDM_COPY, mp1, mp2 ) ;

   return ( Clear ( Window, IDM_CLEAR, mp1, mp2 ) ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process Copy command.                                               *
 *                                                                          *
 ****************************************************************************/

STATIC MRESULT APIENTRY Copy ( HWND Window, MESG, MPARAM1, MPARAM2 ) {

  /**************************************************************************
   * Find the instance data.                                                *
   **************************************************************************/

   PDATA Data = PDATA ( Sys_GetWindowData ( Window ) ) ;

  /**************************************************************************
   * Ask the document to copy the currently selected text.                  *
   **************************************************************************/

   Data->pDocument->Copy ( ) ;

  /**************************************************************************
   * Done.                                                                  *
   **************************************************************************/

   return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process Paste command.                                              *
 *                                                                          *
 ****************************************************************************/

STATIC MRESULT APIENTRY Paste ( HWND Window, MESG, MPARAM1, MPARAM2 ) {

  /**************************************************************************
   * Find the instance data.                                                *
   **************************************************************************/

   PDATA Data = PDATA ( Sys_GetWindowData ( Window ) ) ;

  /**************************************************************************
   * Ask the document to delete the currently selected text and paste in    *
   *   whatever's on the clipboard at the time it gets to this command.     *
   **************************************************************************/

   Data->pDocument->Enqueue_Paste ( ) ;

  /**************************************************************************
   * Done.                                                                  *
   **************************************************************************/

   return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process Clear command.                                              *
 *                                                                          *
 ****************************************************************************/

STATIC MRESULT APIENTRY Clear ( HWND Window, MESG, MPARAM1, MPARAM2 ) {

  /**************************************************************************
   * Find the instance data.                                                *
   **************************************************************************/

   PDATA Data = PDATA ( Sys_GetWindowData ( Window ) ) ;

  /**************************************************************************
   * Ask the document to delete the currently selected text.                *
   **************************************************************************/

   Data->pDocument->Enqueue_Clear ( ) ;

  /**************************************************************************
   * Done.                                                                  *
   **************************************************************************/

   return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process Paste Bitmap command.                                       *
 *                                                                          *
 ****************************************************************************/

STATIC MRESULT APIENTRY PasteBitmap ( HWND Window, MESG, MPARAM1, MPARAM2 ) {

  /**************************************************************************
   * Find the instance data.                                                *
   **************************************************************************/

   PDATA Data = PDATA ( Sys_GetWindowData ( Window ) ) ;

  /**************************************************************************
   * Fetch the contents of the clipboard.                                   *
   **************************************************************************/

   if ( WinOpenClipbrd ( WinQueryAnchorBlock(Window) ) ) {
      HBITMAP Bitmap = HBITMAP ( WinQueryClipbrdData ( WinQueryAnchorBlock(Window), CF_BITMAP ) ) ;
      if ( Bitmap ) {
         DeviceContext Memory = DeviceContext ( "DocWin::PasteBitmap", long(WinQueryAnchorBlock(Window)) ) ;
         WorkSpace MemoryPS = WorkSpace ( "DocWin::PasteBitmap", 0, &Memory, WinQueryAnchorBlock(Window), TRUE ) ;
         long Handle = MemoryPS.CreateBitmap ( Bitmap ) ;
         if ( Handle ) {
            memset ( &Data->PasteGraphic, 0, sizeof(Data->PasteGraphic) ) ;
            Data->PasteGraphic.Type[0] = 'B' ;
            Data->PasteGraphic.Type[1] = 'M' ;
            Data->PasteGraphic.BorderWidth = GRAPHIC_BORDER ;
            Data->PasteGraphic.BorderColor = RGB_WHITE ;
            Data->PasteGraphic.Handle = Handle ;
            Data->PasteGraphic.ComputeImageSize ( &MemoryPS ) ;
            Data->BoxType = BOX_GRAPHIC ;
            Data->BoxMode = BOX_AWAITINGPOINT0 ;
            HWND Frame = PARENT ( Window ) ;
            HWND StatusWindow = CHILD ( Frame, FID_STATUSLINE ) ;
            ResourceString Message ( Data->Library->QueryHandle(), IDS_PASTEGRAPHIC0 ) ;
            Sys_SetWindowText ( StatusWindow, PSZ(Message) ) ;
         } else {
            Sys_BeepNote ( ) ;
         } /* endif */
      } /* endif */
      WinCloseClipbrd ( WinQueryAnchorBlock(Window) ) ;
   } /* endif */

  /**************************************************************************
   * Done.                                                                  *
   **************************************************************************/

   return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process Paste Metafile command.                                     *
 *                                                                          *
 ****************************************************************************/

STATIC MRESULT APIENTRY PasteMetafile ( HWND Window, MESG, MPARAM1, MPARAM2 ) {

  /**************************************************************************
   * Find the instance data.                                                *
   **************************************************************************/

   PDATA Data = PDATA ( Sys_GetWindowData ( Window ) ) ;

  /**************************************************************************
   * Fetch the contents of the clipboard.                                   *
   **************************************************************************/

   if ( WinOpenClipbrd ( WinQueryAnchorBlock(Window) ) ) {
      HMF Metafile = HMF ( WinQueryClipbrdData ( WinQueryAnchorBlock(Window), CF_METAFILE ) ) ;
      if ( Metafile ) {
         WorkSpace PS = WorkSpace ( "DocWin::PasteMetafile", 0, WinQueryAnchorBlock(Window), FALSE, 8500, 11000 ) ;
         long Handle = GpiCopyMetaFile ( Metafile ) ;
         if ( Handle ) {
            memset ( &Data->PasteGraphic, 0, sizeof(Data->PasteGraphic) ) ;
            Data->PasteGraphic.Type[0] = 'M' ;
            Data->PasteGraphic.Type[1] = 'F' ;
            Data->PasteGraphic.BorderWidth = GRAPHIC_BORDER ;
            Data->PasteGraphic.BorderColor = RGB_WHITE ;
            Data->PasteGraphic.Handle = Handle ;
            Data->PasteGraphic.ComputeImageSize ( &PS ) ;
            Data->BoxType = BOX_GRAPHIC ;
            Data->BoxMode = BOX_AWAITINGPOINT0 ;
            HWND Frame = PARENT ( Window ) ;
            HWND StatusWindow = CHILD ( Frame, FID_STATUSLINE ) ;
            ResourceString Message ( Data->Library->QueryHandle(), IDS_PASTEGRAPHIC0 ) ;
            Sys_SetWindowText ( StatusWindow, PSZ(Message) ) ;
         } else {
            Sys_BeepNote ( ) ;
         } /* endif */
      } /* endif */
      WinCloseClipbrd ( WinQueryAnchorBlock(Window) ) ;
   } /* endif */

  /**************************************************************************
   * Done.                                                                  *
   **************************************************************************/

   return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process Paste Screen Entire command.                                *
 *                                                                          *
 ****************************************************************************/

STATIC MRESULT APIENTRY PasteScreenEntire ( HWND Window, MESG, MPARAM1, MPARAM2 ) {

  /**************************************************************************
   * Find the instance data.                                                *
   **************************************************************************/

   PDATA Data = PDATA ( Sys_GetWindowData ( Window ) ) ;

  /**************************************************************************
   * If we're supposed to hide first, do so now.                            *
   **************************************************************************/

   if ( Data->IniData->HideOnScreenClip ) {
      Sys_ShowWindow ( PARENT(Data->MainWindow), FALSE ) ;
      Sys_UpdateWindow ( HWND_DESKTOP ) ;
   } /* endif */

  /**************************************************************************
   * Process the command.                                                   *
   **************************************************************************/

   // Create a memory PS to create the bitmap in.
   HAB Anchor = WinQueryAnchorBlock ( Window ) ;
   DeviceContext Memory = DeviceContext ( "DocWin::PasteScreenEntire", long(Anchor) ) ;
   WorkSpace MemoryPS = WorkSpace ( "DocWin::PasteScreenEntire", 0, &Memory, Anchor, TRUE ) ;

   // Create the bitmap, loading it with the contents of the entire desktop.
   RECTL Desktop ;
   WinQueryWindowRect ( HWND_DESKTOP, &Desktop ) ;
   WorkSpace DesktopPS = WorkSpace ( "DocWin::PasteScreenEntire" ) ;
   HBITMAP Handle = MemoryPS.CreateBitmap ( Desktop, DesktopPS ) ;

   // If successful, set things up for the user to describe it's rectangle.
   if ( Handle ) {
      memset ( &Data->PasteGraphic, 0, sizeof(Data->PasteGraphic) ) ;
      Data->PasteGraphic.Type[0] = 'B' ;
      Data->PasteGraphic.Type[1] = 'M' ;
      Data->PasteGraphic.BorderWidth = GRAPHIC_BORDER ;
      Data->PasteGraphic.BorderColor = RGB_WHITE ;
      Data->PasteGraphic.Handle = Handle ;
      Data->PasteGraphic.ComputeImageSize ( &MemoryPS ) ;
      Data->BoxType = BOX_GRAPHIC ;
      Data->BoxMode = BOX_AWAITINGPOINT0 ;
      HWND Frame = PARENT ( Window ) ;
      HWND StatusWindow = CHILD ( Frame, FID_STATUSLINE ) ;
      ResourceString Message ( Data->Library->QueryHandle(), IDS_PASTEGRAPHIC0 ) ;
      Sys_SetWindowText ( StatusWindow, PSZ(Message) ) ;
   } /* endif */

  /**************************************************************************
   * If we're hid ourselves, come out of hiding now.                        *
   **************************************************************************/

   if ( Data->IniData->HideOnScreenClip ) 
      Sys_ShowWindow ( PARENT(Data->MainWindow), TRUE ) ;

  /**************************************************************************
   * Done.                                                                  *
   **************************************************************************/

   return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process Paste Screen Frame command.                                 *
 *                                                                          *
 ****************************************************************************/

STATIC MRESULT APIENTRY PasteScreenFrame ( HWND Window, MESG, MPARAM1, MPARAM2 ) {

  /**************************************************************************
   * Find the instance data.                                                *
   **************************************************************************/

   PDATA Data = PDATA ( Sys_GetWindowData ( Window ) ) ;

  /**************************************************************************
   * If we're supposed to hide first, do so now.                            *
   **************************************************************************/

   if ( Data->IniData->HideOnScreenClip ) 
      Sys_ShowWindow ( PARENT(Data->MainWindow), FALSE ) ;

  /**************************************************************************
   * Process the command.                                                   *
   **************************************************************************/

   Sys_BeepNote ( ) ;

  /**************************************************************************
   * If we're hid ourselves, come out of hiding now.                        *
   **************************************************************************/

   if ( Data->IniData->HideOnScreenClip ) 
      Sys_ShowWindow ( PARENT(Data->MainWindow), TRUE ) ;

  /**************************************************************************
   * Done.                                                                  *
   **************************************************************************/

   return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process Paste Screen Window command.                                *
 *                                                                          *
 ****************************************************************************/

STATIC MRESULT APIENTRY PasteScreenWindow ( HWND Window, MESG, MPARAM1, MPARAM2 ) {

  /**************************************************************************
   * Find the instance data.                                                *
   **************************************************************************/

   PDATA Data = PDATA ( Sys_GetWindowData ( Window ) ) ;

  /**************************************************************************
   * If we're supposed to hide first, do so now.                            *
   **************************************************************************/

   if ( Data->IniData->HideOnScreenClip ) 
      Sys_ShowWindow ( PARENT(Data->MainWindow), FALSE ) ;

  /**************************************************************************
   * Process the command.                                                   *
   **************************************************************************/

   Sys_BeepNote ( ) ;

  /**************************************************************************
   * If we're hid ourselves, come out of hiding now.                        *
   **************************************************************************/

   if ( Data->IniData->HideOnScreenClip ) 
      Sys_ShowWindow ( PARENT(Data->MainWindow), TRUE ) ;

  /**************************************************************************
   * Done.                                                                  *
   **************************************************************************/

   return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process Paste Screen Rectangle command.                             *
 *                                                                          *
 ****************************************************************************/

STATIC MRESULT APIENTRY PasteScreenRect ( HWND Window, MESG, MPARAM1, MPARAM2 ) {

  /**************************************************************************
   * Find the instance data.                                                *
   **************************************************************************/

   PDATA Data = PDATA ( Sys_GetWindowData ( Window ) ) ;

  /**************************************************************************
   * If we're supposed to hide first, do so now.                            *
   **************************************************************************/

   if ( Data->IniData->HideOnScreenClip ) 
      Sys_ShowWindow ( PARENT(Data->MainWindow), FALSE ) ;

  /**************************************************************************
   * Process the command.                                                   *
   **************************************************************************/

   Sys_BeepNote ( ) ;

  /**************************************************************************
   * If we're hid ourselves, come out of hiding now.                        *
   **************************************************************************/

   if ( Data->IniData->HideOnScreenClip ) 
      Sys_ShowWindow ( PARENT(Data->MainWindow), TRUE ) ;

  /**************************************************************************
   * Done.                                                                  *
   **************************************************************************/

   return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process Paste Graphic File command.                                 *
 *                                                                          *
 ****************************************************************************/

STATIC MRESULT APIENTRY PasteGraphic ( HWND Window, MESG, MPARAM1, MPARAM2 ) {

  /**************************************************************************
   * Find the instance data.                                                *
   **************************************************************************/

   PDATA Data = PDATA ( Sys_GetWindowData ( Window ) ) ;

  /**************************************************************************
   * Display the system File Open dialog.                                   *
   **************************************************************************/

   ResourceString OpenGraphicFile ( Data->Library->QueryHandle(), IDS_OPEN_GRAPHIC ) ;
   PrepareGraphicTypeList ( ) ;
   FILEDIALOG_DATA FileDialogData = { TRUE, GraphicTypeList, GraphicWildcardList, GraphicExtenderList, 0 } ;
   FILEDLG DialogData ;
   memset ( &DialogData, 0, sizeof(DialogData) ) ;
   DialogData.cbSize = sizeof(DialogData) ;
   DialogData.fl = FDS_OPEN_DIALOG | FDS_CENTER | FDS_HELPBUTTON | FDS_FILTERUNION ;
   DialogData.pszTitle = PSZ(OpenGraphicFile) ;
   strcpy ( DialogData.szFullFile, GraphicWildcardList[0] ) ;
   DialogData.pszIType = Data->IniData->GraphicType[0] ? Data->IniData->GraphicType : GraphicTypeList[0] ;
   DialogData.papszITypeList = PAPSZ(GraphicTypeList) ;
   DialogData.pfnDlgProc = FileDialog_Processor ;
   DialogData.ulUser = ULONG ( &FileDialogData ) ;
   WinFileDlg ( HWND_DESKTOP, Window, &DialogData ) ;

  /**************************************************************************
   * If the dialog was successful, start loading the specified file(s).     *
   **************************************************************************/

   if ( DialogData.lReturn == DID_OK ) {

      // Save the most recently selected file type.
      strcpy ( Data->IniData->GraphicType, FileDialogData.FilterIndex > 0 ? GraphicTypeList[FileDialogData.FilterIndex] : "" ) ;

      // Change current directory to wherever the Open dialog wound up at.
      char Drive [_MAX_DRIVE + 1 ] ;
      char Dir   [_MAX_DIR   + 1 ] ;
      char Name  [_MAX_FNAME + 1 ] ;
      char Ext   [_MAX_EXT   + 1 ] ;
      _splitpath ( PCHAR(DialogData.szFullFile), Drive, Dir, Name, Ext ) ;
      strcat ( Name, Ext ) ;
      _chdrive ( Drive[0]-'A'+1 ) ;
      Dir[strlen(Dir)-1] = 0 ;
      _chdir ( "\\" ) ;
      _chdir ( Dir ) ;

      // Import the file.  Only bitmaps for now.
      HBITMAP Bitmap = 0 ;  HMF Metafile = 0 ;
      Addon *Importer = GraphicExtenderList[FileDialogData.Index] ;
      if ( ! strcmpi ( Importer->QueryType(), TYPE_GRAPHICIMPORT ) ) {
         Bitmap = ((GraphicImporter*)GraphicExtenderList[FileDialogData.Index])->Import ( Window, DialogData.szFullFile ) ;
      } else {
         Metafile = ((MetafileImporter*)GraphicExtenderList[FileDialogData.Index])->Import ( Window, DialogData.szFullFile ) ;
      } /* endif */

      if ( Bitmap OR Metafile ) {
         WorkSpace PS = WorkSpace ( "DocWin::PasteGraphic", 0, WinQueryAnchorBlock(Window), FALSE, 8500, 11000 ) ;
         memset ( &Data->PasteGraphic, 0, sizeof(Data->PasteGraphic) ) ;
         if ( Bitmap ) {
            Data->PasteGraphic.Type[0] = 'B' ;
            Data->PasteGraphic.Type[1] = 'M' ;
            Data->PasteGraphic.Handle = Bitmap ;
         } else {
            Data->PasteGraphic.Type[0] = 'M' ;
            Data->PasteGraphic.Type[1] = 'F' ;
            Data->PasteGraphic.Handle = Metafile ;
         } /* endif */
         Data->PasteGraphic.BorderWidth = GRAPHIC_BORDER ;
         Data->PasteGraphic.BorderColor = RGB_WHITE ;
         Data->PasteGraphic.Handle = Bitmap ? Bitmap : Metafile ;
         Data->PasteGraphic.ComputeImageSize ( &PS ) ;
         Data->BoxType = BOX_GRAPHIC ;
         Data->BoxMode = BOX_AWAITINGPOINT0 ;
         HWND Frame = PARENT ( Window ) ;
         HWND StatusWindow = CHILD ( Frame, FID_STATUSLINE ) ;
         ResourceString Message ( Data->Library->QueryHandle(), IDS_PASTEGRAPHIC0 ) ;
         Sys_SetWindowText ( StatusWindow, PSZ(Message) ) ;
      } else {
         Sys_BeepNote ( ) ;
      } /* endif */

   } /* endif */

  /**************************************************************************
   * Done.                                                                  *
   **************************************************************************/

   return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process Search command.                                             *
 *                                                                          *
 ****************************************************************************/

STATIC MRESULT APIENTRY Search ( HWND Window, MESG, MPARAM1, MPARAM2 ) {

  /**************************************************************************
   * Find the instance data.                                                *
   **************************************************************************/

   PDATA Data = PDATA ( Sys_GetWindowData ( Window ) ) ;

  /**************************************************************************
   * Process the command.                                                   *
   **************************************************************************/

   SEARCH_PARMS Parms ;
   Parms.Library = Data->Library->QueryHandle() ;
   Parms.IniData = Data->IniData ;
   Parms.pText = Data->pDocument->Query_Text() ;
   Parms.Owner = Window ;

   WinDlgBox ( HWND_DESKTOP, Window, Search_Processor, Data->Library->QueryHandle(), IDD_SEARCH, &Parms ) ;

  /**************************************************************************
   * Done.                                                                  *
   **************************************************************************/

   return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process Find command.                                               *
 *                                                                          *
 ****************************************************************************/

STATIC MRESULT APIENTRY Find ( HWND Window, MESG, MPARAM1, MPARAM2 ) {

  /**************************************************************************
   * Find the instance data.                                                *
   **************************************************************************/

   PDATA Data = PDATA ( Sys_GetWindowData ( Window ) ) ;

  /**************************************************************************
   * Process the command.                                                   *
   **************************************************************************/

   if ( Data->IniData->Search_String[0] == 0 ) {
      Sys_BeepNote ( ) ;
      return ( FALSE ) ;

   } else if ( Data->pDocument->FindString ( Data->IniData->Search_String, Data->IniData->Search_CaseSensitive, Data->IniData->Search_Regular ) ) {
      Sys_BeepNote ( ) ;
      return ( FALSE ) ;

   } /* endif */

  /**************************************************************************
   * Done.  Return TRUE for found.                                          *
   **************************************************************************/

   return ( MRFROMSHORT ( TRUE ) ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process Find Next command.                                          *
 *                                                                          *
 ****************************************************************************/

STATIC MRESULT APIENTRY FindNext ( HWND Window, MESG, MPARAM1, MPARAM2 ) {

  /**************************************************************************
   * Find the instance data.                                                *
   **************************************************************************/

   PDATA Data = PDATA ( Sys_GetWindowData ( Window ) ) ;

  /**************************************************************************
   * Process the command.                                                   *
   **************************************************************************/

   if ( Data->IniData->Search_String[0] == 0 ) {
      Sys_BeepNote ( ) ;
      return ( FALSE ) ;

   } else if ( Data->pDocument->FindStringNext ( Data->IniData->Search_String, Data->IniData->Search_CaseSensitive, Data->IniData->Search_Regular ) ) {
      Sys_BeepNote ( ) ;
      return ( FALSE ) ;

   } /* endif */

  /**************************************************************************
   * Done.  Return TRUE for found.                                          *
   **************************************************************************/

   return ( MRFROMSHORT ( TRUE ) ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process Change command.                                             *
 *                                                                          *
 ****************************************************************************/

STATIC MRESULT APIENTRY Change ( HWND Window, MESG, MPARAM1, MPARAM2 ) {

  /**************************************************************************
   * Find the instance data.                                                *
   **************************************************************************/

   PDATA Data = PDATA ( Sys_GetWindowData ( Window ) ) ;

  /**************************************************************************
   * Process the command.                                                   *
   **************************************************************************/

   if ( Data->IniData->Search_String[0] == 0 )
      Sys_BeepNote ( ) ;

   else if ( Data->pDocument->ChangeString ( Data->IniData->Search_String, Data->IniData->Search_Replace, Data->IniData->Search_CaseSensitive, Data->IniData->Search_Regular ) )
      Sys_BeepNote ( ) ;

  /**************************************************************************
   * Done.                                                                  *
   **************************************************************************/

   return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process Change Find command.                                        *
 *                                                                          *
 ****************************************************************************/

STATIC MRESULT APIENTRY ChangeFind ( HWND Window, MESG, MPARAM1, MPARAM2 ) {

  /**************************************************************************
   * Find the instance data.                                                *
   **************************************************************************/

   PDATA Data = PDATA ( Sys_GetWindowData ( Window ) ) ;

  /**************************************************************************
   * Process the command.                                                   *
   **************************************************************************/

   if ( Data->IniData->Search_String[0] == 0 )
      Sys_BeepNote ( ) ;

   else if ( Data->pDocument->ChangeString ( Data->IniData->Search_String, Data->IniData->Search_Replace, Data->IniData->Search_CaseSensitive, Data->IniData->Search_Regular ) )
      Sys_BeepNote ( ) ;

   else if ( Data->pDocument->FindStringNext ( Data->IniData->Search_String, Data->IniData->Search_CaseSensitive, Data->IniData->Search_Regular ) ) 
      Sys_BeepNote ( ) ;


  /**************************************************************************
   * Done.                                                                  *
   **************************************************************************/

   return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process Change All command.                                         *
 *                                                                          *
 ****************************************************************************/

STATIC MRESULT APIENTRY ChangeAll ( HWND Window, MESG, MPARAM1, MPARAM2 ) {

  /**************************************************************************
   * Find the instance data.                                                *
   **************************************************************************/

   PDATA Data = PDATA ( Sys_GetWindowData ( Window ) ) ;

  /**************************************************************************
   * Process the command.                                                   *
   **************************************************************************/

   if ( Data->IniData->Search_String[0] == 0 )
      Sys_BeepNote ( ) ;

   else if ( Data->pDocument->ChangeAll ( Data->IniData->Search_String, Data->IniData->Search_Replace, Data->IniData->Search_CaseSensitive, Data->IniData->Search_Regular ) )
      Sys_BeepNote ( ) ;

  /**************************************************************************
   * Done.                                                                  *
   **************************************************************************/

   return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process Go To Page command.                                         *
 *                                                                          *
 ****************************************************************************/

class GotoPage_Dialog : public Dialog {
   private:
      HMODULE Library ;
      StaticText *Err ;
      Entryfield *Field ;
      long *Page ;
      long PageCount ;

   public:
      GotoPage_Dialog ( HWND Parent, HWND Owner, HMODULE Resource, int ID, long &page, long pagecount ) : 
         Dialog ( Parent, Owner, Resource, ID ), Library(Resource), Page(&page), PageCount(pagecount) {
         if ( !Load ( ) )
            return ;
         Field = new Entryfield ( CHILD ( Handle, IDD_GOTOPAGE_ENTRY ) ) ;
         Err = new StaticText ( CHILD ( Handle, IDD_GOTOPAGE_ERR ) ) ;
         char Text [5] ; sprintf ( Text, "%i", *Page ) ;
         Field->SetLimit ( sizeof(Text)-1 ) ;
         Field->SetText ( Text ) ;
      } /* endmethod */

      ~GotoPage_Dialog ( ) {
         delete Err ;
         delete Field ;
      } /* endmethod */

      MRESULT Command ( MPARAM1 mp1, MPARAM2 mp2 ) ;
      MRESULT MessageProcessor ( HWND hwnd, MESG msg, MPARAM1 mp1, MPARAM2 mp2 ) ;
} ;

MRESULT GotoPage_Dialog::Command ( MPARAM1 mp1, MPARAM2 ) {
   switch ( SHORT1FROMMP(mp1) ) {
      case DID_OK: {
         char Text [5] ;
         Field->GetText ( Text, sizeof(Text) ) ;
         *Page = atoi ( Text ) - 1 ;
         if ( ( *Page < 0 ) || ( *Page > PageCount-1 ) ) {
            ResourceString Format ( Library, IDS_GOTOPAGE_ERROR1 ) ;
            char Message [200] ;
            sprintf ( Message, PCHAR(Format), PageCount ) ;
            Err->SetText ( Message ) ;
            Sys_BeepError ( ) ;
            Field->SetFocus ( ) ;
            return ( 0 ) ;
         } /* endif */
         Dismiss ( TRUE ) ;
         break; }
      case DID_CANCEL: {
         Dismiss ( FALSE ) ;
         break; }
   } /* endswitch */
   return ( 0 ) ;
} /* endmethod */

MRESULT GotoPage_Dialog::MessageProcessor ( HWND hwnd, MESG msg, MPARAM1 mp1, MPARAM2 mp2 ) {
   MRESULT Result ;
   switch ( msg ) {
      case WM_COMMAND:  Result = Command ( mp1, mp2 ) ; break ;
      default:          Result = Dialog::MessageProcessor ( hwnd, msg, mp1, mp2 ) ;
   } /* endswitch */
   return ( Result ) ;
} /* endmethod */

STATIC MRESULT APIENTRY GotoPage ( HWND Window, MESG, MPARAM1, MPARAM2 ) {

  /**************************************************************************
   * Find the instance data.                                                *
   **************************************************************************/

   PDATA Data = PDATA ( Sys_GetWindowData ( Window ) ) ;

  /**************************************************************************
   * Invoke the Go To Page dialog.  Go to the page if successful.           *
   **************************************************************************/

   long Page ( Data->pDocument->Query_PageNumber ( ) + 1 ) ;
   GotoPage_Dialog ThisDialog ( HWND_DESKTOP, Window, Data->Library->QueryHandle(), IDD_GOTOPAGE, Page, Data->pDocument->Query_PageCount() ) ;
   if ( ThisDialog.Process ( ) ) {
      if ( Data->pDocument->MovePageNumber ( Page, FALSE ) )
         Sys_BeepNote ( ) ;
   } /* endif */

  /**************************************************************************
   * Done.                                                                  *
   **************************************************************************/

   return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process Spell Check command.                                        *
 *                                                                          *
 ****************************************************************************/

STATIC MRESULT APIENTRY SpellCheck ( HWND Window, MESG, MPARAM1, MPARAM2 ) {

  /**************************************************************************
   * Find the instance data.                                                *
   **************************************************************************/

   PDATA Data = PDATA ( Sys_GetWindowData ( Window ) ) ;

  /**************************************************************************
   * If spell checker not available, sound off.                             *
   **************************************************************************/

   if ( !Data->pSpeller || !Data->pSpeller->Ready() ) {
      Sys_BeepNote ( ) ;
      return ( 0 ) ;
   } /* endif */

  /**************************************************************************
   * Process the command.                                                   *
   **************************************************************************/

   SpellChecker ( Window, Data->pDocument, Data->pSpeller ) ;

  /**************************************************************************
   * Done.                                                                  *
   **************************************************************************/

   return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process Zoom 100% command.                                          *
 *                                                                          *
 ****************************************************************************/

STATIC MRESULT APIENTRY Zoom100 ( HWND Window, MESG msg, MPARAM1, MPARAM2 ) {

  /**************************************************************************
   * Find the instance data.                                                *
   **************************************************************************/

   PDATA Data = PDATA ( Sys_GetWindowData ( Window ) ) ;

  /**************************************************************************
   * Process the Zoom 100% command.                                         *
   **************************************************************************/

   Data->ZoomCommand = short ( msg ) ;
   SetZoom ( Window, Data, 1.0 ) ;

  /**************************************************************************
   * Done.                                                                  *
   **************************************************************************/

   return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process Zomm Full Width command.                                    *
 *                                                                          *
 ****************************************************************************/

STATIC MRESULT APIENTRY ZoomFullWidth ( HWND Window, MESG msg, MPARAM1, MPARAM2 ) {

  /**************************************************************************
   * Find the instance data.                                                *
   **************************************************************************/

   PDATA Data = PDATA ( Sys_GetWindowData ( Window ) ) ;

  /**************************************************************************
   * Process the Zoom Full Width command.                                   *
   **************************************************************************/

   Data->ZoomCommand = short ( msg ) ;

   RECTL Rectangle ;
   WinQueryWindowRect ( Window, &Rectangle ) ;
   Data->pDocument->Transform ( CVTC_DEVICE, CVTC_PAGE, Rectangle ) ;

   SetZoom ( Window, Data, double(Rectangle.xRight-Rectangle.xLeft) / double(Data->pDocument->Query_State0()->FormInfo.cx) ) ;

  /**************************************************************************
   * Done.                                                                  *
   **************************************************************************/

   return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process Zoom Full Height command.                                   *
 *                                                                          *
 ****************************************************************************/

STATIC MRESULT APIENTRY ZoomFullHeight ( HWND Window, MESG msg, MPARAM1, MPARAM2 ) {

  /**************************************************************************
   * Find the instance data.                                                *
   **************************************************************************/

   PDATA Data = PDATA ( Sys_GetWindowData ( Window ) ) ;

  /**************************************************************************
   * Process the Zoom Full Height command.                                  *
   **************************************************************************/

   Data->ZoomCommand = short ( msg ) ;

   RECTL Rectangle ;
   WinQueryWindowRect ( Window, &Rectangle ) ;
   Data->pDocument->Transform ( CVTC_DEVICE, CVTC_PAGE, Rectangle ) ;

   SetZoom ( Window, Data, double(Rectangle.yTop-Rectangle.yBottom) / double(Data->pDocument->Query_State0()->FormInfo.cy) ) ;

  /**************************************************************************
   * Done.                                                                  *
   **************************************************************************/

   return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process Zomm Margins command.                                       *
 *                                                                          *
 ****************************************************************************/

STATIC MRESULT APIENTRY ZoomMargins ( HWND Window, MESG msg, MPARAM1, MPARAM2 ) {

  /**************************************************************************
   * Find the instance data.                                                *
   **************************************************************************/

   PDATA Data = PDATA ( Sys_GetWindowData ( Window ) ) ;

  /**************************************************************************
   * Process the Zoom Margins command.                                      *
   **************************************************************************/

   Data->ZoomCommand = short ( msg ) ;

   RECTL Rectangle ;
   WinQueryWindowRect ( Window, &Rectangle ) ;
   Data->pDocument->Transform ( CVTC_DEVICE, CVTC_PAGE, Rectangle ) ;

   SetZoom ( Window, Data,
      double ( Rectangle.xRight-Rectangle.xLeft ) /
      double ( Data->pDocument->Query_State()->Margins.xRight-Data->pDocument->Query_State()->Margins.xLeft ) ) ;

   Sys_PostMessage ( Window, WM_HSCROLL,
      MPFROM2SHORT ( FID_HORZSCROLL, 0 ),
      MPFROM2SHORT ( SHORT(Data->pDocument->Query_State()->Margins.xLeft), SB_SLIDERPOSITION ) ) ;

  /**************************************************************************
   * Done.                                                                  *
   **************************************************************************/

   return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process Zoom All command.                                           *
 *                                                                          *
 ****************************************************************************/

STATIC MRESULT APIENTRY ZoomAll ( HWND Window, MESG, MPARAM1, MPARAM2 mp2 ) {

  /**************************************************************************
   * Find the instance data.                                                *
   **************************************************************************/

   PDATA Data = PDATA ( Sys_GetWindowData ( Window ) ) ;

  /**************************************************************************
   * If this command came from the keyboard accelerator, use it as a toggle.*
   **************************************************************************/

   if ( ( SHORT1FROMMP(mp2) == CMDSRC_ACCELERATOR ) AND ( Data->ZoomCommand == IDM_ZOOM_ALL ) ) {
      if ( Data->IniData->ZoomType != IDM_ZOOM_ALL ) {
         Data->ZoomCommand = short ( Data->IniData->ZoomType ) ;
         Rezoom ( Window, Data ) ;
      } /* endif */

  /**************************************************************************
   * Else, do an unconditional Zoom All.                                    *
   **************************************************************************/

   } else {

      Data->ZoomCommand = IDM_ZOOM_ALL ;

      RECTL Rectangle ;
      WinQueryWindowRect ( Window, &Rectangle ) ;
      Data->pDocument->Transform ( CVTC_DEVICE, CVTC_PAGE, Rectangle ) ;

      SetZoom ( Window, Data,
         double( min (
            double(Rectangle.xRight-Rectangle.xLeft) / double(Data->pDocument->Query_State0()->FormInfo.cx),
            double(Rectangle.yTop-Rectangle.yBottom) / double(Data->pDocument->Query_State0()->FormInfo.cy)
         ) ) ) ;

   } /* endif */

  /**************************************************************************
   * Done.                                                                  *
   **************************************************************************/

   return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process Zoom Window command.                                        *
 *                                                                          *
 ****************************************************************************/

STATIC MRESULT APIENTRY ZoomWindow ( HWND Window, MESG msg, MPARAM1, MPARAM2 ) {

  /**************************************************************************
   * Find the instance data.                                                *
   **************************************************************************/

   PDATA Data = PDATA ( Sys_GetWindowData ( Window ) ) ;

  /**************************************************************************
   * Process the Zoom Window command.                                       *
   **************************************************************************/

   Data->ZoomCommand = short ( msg ) ;
   Data->BoxType = BOX_ZOOM ;
   Data->BoxMode = BOX_AWAITINGPOINT0 ;

   HWND Frame = PARENT ( Window ) ;
   HWND StatusWindow = CHILD ( Frame, FID_STATUSLINE ) ;
   ResourceString Message ( Data->Library->QueryHandle(), IDS_ZOOM0 ) ;
   Sys_SetWindowText ( StatusWindow, PSZ(Message) ) ;

  /**************************************************************************
   * Done.                                                                  *
   **************************************************************************/

   return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process Zoom Set command.                                           *
 *                                                                          *
 ****************************************************************************/

STATIC MRESULT APIENTRY ZoomSet ( HWND Window, MESG msg, MPARAM1, MPARAM2 ) {

  /**************************************************************************
   * Find the instance data.                                                *
   **************************************************************************/

   PDATA Data = PDATA ( Sys_GetWindowData ( Window ) ) ;

  /**************************************************************************
   * Process the Zoom Set command.                                          *
   **************************************************************************/

   double Zoom = double ( FIXEDINT ( Data->pDocument->Query_Zoom() ) ) ;
   Zoom += double ( FIXEDFRAC ( Data->pDocument->Query_Zoom() ) ) / 0x10000 ;

   SETZOOM_PARMS Parms ;
   Parms.Library = Data->Library->QueryHandle() ;
   Parms.Window = Window ;
   Parms.ZoomFactor = USHORT ( ( Zoom * 100.0 ) + 0.4 ) ;

   if ( WinDlgBox ( HWND_DESKTOP, Window, SetZoomProcessor, Data->Library->QueryHandle(), IDD_ZOOM_SET, &Parms ) ) {
      Data->ZoomCommand = short ( msg ) ;
      SetZoom ( Window, Data, double(Parms.ZoomFactor) / 100.0 ) ;
   } /* endif */

  /**************************************************************************
   * Done.                                                                  *
   **************************************************************************/

   return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process Zoom Apply message.                                         *
 *                                                                          *
 ****************************************************************************/

STATIC MRESULT APIENTRY ApplyZoom ( HWND Window, MESG, MPARAM1 mp1, MPARAM2 ) {

  /**************************************************************************
   * Find the instance data.                                                *
   **************************************************************************/

   PDATA Data = PDATA ( Sys_GetWindowData ( Window ) ) ;

  /**************************************************************************
   * Process the Zoom Apply message.                                        *
   **************************************************************************/

   Data->ZoomCommand = IDD_ZOOM_SET ;
   SetZoom ( Window, Data, double(SHORT(SHORT1FROMMP(mp1))) / 100.0 ) ;

  /**************************************************************************
   * Done.                                                                  *
   **************************************************************************/

   return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process Set Margins command                                         *
 *                                                                          *
 ****************************************************************************/

STATIC MRESULT APIENTRY SetMargins ( HWND Window, MESG, MPARAM1, MPARAM2 ) {

  /**************************************************************************
   * Find the instance data.                                                *
   **************************************************************************/

   PDATA Data = PDATA ( Sys_GetWindowData ( Window ) ) ;

  /**************************************************************************
   * Process the Set Margins command.                                       *
   **************************************************************************/

   HWND Frame = PARENT ( Window ) ;

   RECTL WindowRectangle ;  SIZEL WindowSize ;
   Data->pDocument->Query_WindowSize ( WindowRectangle, WindowSize ) ;

   MARGINS_PARMS Parms ;
   Parms.Library     = Data->Library->QueryHandle() ;
   Parms.Window      = Window ;
   Parms.VertRuler   = CHILD ( Frame, FID_VERTRULER ) ;
   Parms.HorzRuler   = CHILD ( Frame, FID_HORZRULER ) ;
   Parms.Metric      = Data->pDocument->Query_State()->Metric ;
   Parms.fxZoom      = Data->pDocument->Query_Zoom() ;
   Parms.PageSize.cx = Data->pDocument->Query_State0()->FormInfo.cx ;
   Parms.PageSize.cy = Data->pDocument->Query_State0()->FormInfo.cy ;
   Parms.WindowSize  = WindowSize ;
   Parms.Margins     = Data->pDocument->Query_State()->Margins ;
   Parms.TopLeft     = Data->pDocument->Query_TopLeft() ;
   Parms.TabCount    = Data->pDocument->Query_State()->TabCount ;
   memcpy ( Parms.Tabs, Data->pDocument->Query_State()->Tabs, sizeof(Parms.Tabs) ) ;
   memcpy ( &Parms.DocParms, Data->pDocument->Query_Parms(), sizeof(Parms.DocParms) ) ;

   if ( WinDlgBox ( HWND_DESKTOP, Window, MarginsProcessor, Data->Library->QueryHandle(), IDD_MARGINS, &Parms ) )
      ApplyMargins ( Window, WM_SET_MARGINS, MPFROMP(&Parms), 0 ) ;

  /**************************************************************************
   * Done.                                                                  *
   **************************************************************************/

   return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process Apply Margins command.                                      *
 *                                                                          *
 ****************************************************************************/

STATIC MRESULT APIENTRY ApplyMargins ( HWND Window, MESG, MPARAM1 mp1, MPARAM2 ) {

  /**************************************************************************
   * Find the instance data.                                                *
   **************************************************************************/

   PDATA Data = PDATA ( Sys_GetWindowData ( Window ) ) ;
   PMARGINS_PARMS Parms = PMARGINS_PARMS ( mp1 ) ;

  /**************************************************************************
   * Insert the document state change.                                      *
   **************************************************************************/

   Estado NewState ;
   NewState.Margins = Parms->Margins ;
   Data->pDocument->InsertStateChange ( *Data->pDocument->Query_State(), NewState, ADJUSTSTATE_MARGINS ) ;
   Data->pDocument->FormatPage ( ) ;
   Rezoom ( Window, Data ) ;

  /**************************************************************************
   * Done.                                                                  *
   **************************************************************************/

   return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process Set Tabs command.                                           *
 *                                                                          *
 ****************************************************************************/

STATIC MRESULT APIENTRY SetTabs ( HWND Window, MESG, MPARAM1, MPARAM2 ) {

  /**************************************************************************
   * Find the instance data.                                                *
   **************************************************************************/

   PDATA Data = PDATA ( Sys_GetWindowData ( Window ) ) ;

  /**************************************************************************
   * Process the Set Tabs command.                                          *
   **************************************************************************/

   TABS_PARMS Parms ;
   HWND Frame = PARENT ( Window ) ;

   RECTL WindowRectangle ;  SIZEL WindowSize ;
   Data->pDocument->Query_WindowSize ( WindowRectangle, WindowSize ) ;

   Parms.Library     = Data->Library->QueryHandle() ;
   Parms.Window      = Window ;
   Parms.RulerWindow = CHILD ( Frame, FID_HORZRULER ) ;
   Parms.Metric      = Data->pDocument->Query_State()->Metric ;
   Parms.fxZoom      = Data->pDocument->Query_Zoom() ;
   Parms.TopLeft     = Data->pDocument->Query_TopLeft().x ;
   Parms.PageWidth   = Data->pDocument->Query_State0()->FormInfo.cx ;
   Parms.WindowWidth = WindowSize.cx ;
   Parms.LeftMargin  = Data->pDocument->Query_State()->Margins.xLeft ;
   Parms.RightMargin = Data->pDocument->Query_State()->Margins.xRight ;
   Parms.TabCount    = Data->pDocument->Query_State()->TabCount ;
   memcpy ( Parms.Tabs, Data->pDocument->Query_State()->Tabs, sizeof(Parms.Tabs) ) ;

   if ( WinDlgBox ( HWND_DESKTOP, Window, TabsProcessor, Data->Library->QueryHandle(), IDD_TABS, &Parms ) )
      ApplyTabs ( Window, WM_SET_TABS, MPFROMP(&Parms), 0 ) ;

  /**************************************************************************
   * Done.                                                                  *
   **************************************************************************/

   return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process Apply Tabs command.                                         *
 *                                                                          *
 ****************************************************************************/

STATIC MRESULT APIENTRY ApplyTabs ( HWND Window, MESG, MPARAM1 mp1, MPARAM2 ) {

  /**************************************************************************
   * Find the instance data.                                                *
   **************************************************************************/

   PDATA Data = PDATA ( Sys_GetWindowData ( Window ) ) ;
   PTABS_PARMS Parms = PTABS_PARMS ( mp1 ) ;

  /**************************************************************************
   * Insert the document state change.                                      *
   **************************************************************************/

   Estado NewState ;
   NewState.TabCount = Parms->TabCount ;
   memcpy ( NewState.Tabs, Parms->Tabs, sizeof(NewState.Tabs) ) ;
   Data->pDocument->InsertStateChange ( *Data->pDocument->Query_State(), NewState, ADJUSTSTATE_TABS ) ;

  /**************************************************************************
   * Done.                                                                  *
   **************************************************************************/

   return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process Set Indent command.                                         *
 *                                                                          *
 ****************************************************************************/

STATIC MRESULT APIENTRY SetIndent ( HWND Window, MESG, MPARAM1, MPARAM2 ) {

  /**************************************************************************
   * Find the instance data.                                                *
   **************************************************************************/

   PDATA Data = PDATA ( Sys_GetWindowData ( Window ) ) ;

  /**************************************************************************
   * Process the Set Indent command.                                        *
   **************************************************************************/

   QueueInfo Info ( "DocWin::SetIndent", WinQueryAnchorBlock(Window), 
      Data->pDocument->Query_Parms()->Printer, Data->pDocument->Query_Parms()->FormName,
      PDRIVDATA(Data->pDocument->Query_Parms()->JobSettings) ) ;

   HCINFO FormInfo ;
   Info.QueryForm ( WinQueryAnchorBlock(Window), Data->pDocument->Query_State()->Metric, FormInfo ) ;

   INDENT_PARMS Parms ;
   Parms.Library     = Data->Library->QueryHandle() ;
   Parms.Window      = Window ;
   Parms.Metric      = Data->pDocument->Query_State()->Metric ;
   Parms.FormInfo    = FormInfo ;
   Parms.LeftMargin  = Data->pDocument->Query_State()->Margins.xLeft ;
   Parms.RightMargin = Data->pDocument->Query_State()->Margins.xRight ;
   Parms.Indent      = Data->pDocument->Query_State()->Indent ;

   if ( WinDlgBox ( HWND_DESKTOP, Window, IndentProcessor, Data->Library->QueryHandle(), IDD_INDENT, &Parms ) )
      ApplyIndent ( Window, WM_SET_INDENT, MPFROMLONG(Parms.Indent), 0 ) ;

  /**************************************************************************
   * Done.                                                                  *
   **************************************************************************/

   return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process Apply Indent command.                                       *
 *                                                                          *
 ****************************************************************************/

STATIC MRESULT APIENTRY ApplyIndent ( HWND Window, MESG, MPARAM1 mp1, MPARAM2 ) {

  /**************************************************************************
   * Find the instance data.                                                *
   **************************************************************************/

   PDATA Data = PDATA ( Sys_GetWindowData ( Window ) ) ;

  /**************************************************************************
   * Insert the document state change.                                      *
   **************************************************************************/

   Estado NewState ;
   NewState.Indent = int ( LONGFROMMP ( mp1 ) ) ;
   Data->pDocument->InsertStateChange ( *Data->pDocument->Query_State(), NewState, ADJUSTSTATE_INDENT ) ;

  /**************************************************************************
   * Done.                                                                  *
   **************************************************************************/

   return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process Set Justification Left command.                             *
 *                                                                          *
 ****************************************************************************/

STATIC MRESULT APIENTRY SetJustLeft ( HWND Window, MESG, MPARAM1, MPARAM2 ) {

  /**************************************************************************
   * Find the instance data.                                                *
   **************************************************************************/

   PDATA Data = PDATA ( Sys_GetWindowData ( Window ) ) ;

  /**************************************************************************
   * Insert the document state change.                                      *
   **************************************************************************/

   Estado NewState ;
   NewState.Justify = JUSTIFY_LEFT ;
   Data->pDocument->InsertStateChange ( *Data->pDocument->Query_State(), NewState, ADJUSTSTATE_JUSTIFY ) ;

  /**************************************************************************
   * Done.                                                                  *
   **************************************************************************/

   return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process Set Justification Center command.                           *
 *                                                                          *
 ****************************************************************************/

STATIC MRESULT APIENTRY SetJustCenter ( HWND Window, MESG, MPARAM1, MPARAM2 ) {

  /**************************************************************************
   * Find the instance data.                                                *
   **************************************************************************/

   PDATA Data = PDATA ( Sys_GetWindowData ( Window ) ) ;

  /**************************************************************************
   * Insert the document state change.                                      *
   **************************************************************************/

   Estado NewState ;
   NewState.Justify = JUSTIFY_CENTER ;
   Data->pDocument->InsertStateChange ( *Data->pDocument->Query_State(), NewState, ADJUSTSTATE_JUSTIFY ) ;

  /**************************************************************************
   * Done.                                                                  *
   **************************************************************************/

   return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process Set Justification Right command.                            *
 *                                                                          *
 ****************************************************************************/

STATIC MRESULT APIENTRY SetJustRight ( HWND Window, MESG, MPARAM1, MPARAM2 ) {

  /**************************************************************************
   * Find the instance data.                                                *
   **************************************************************************/

   PDATA Data = PDATA ( Sys_GetWindowData ( Window ) ) ;

  /**************************************************************************
   * Insert the document state change.                                      *
   **************************************************************************/

   Estado NewState ;
   NewState.Justify = JUSTIFY_RIGHT ;
   Data->pDocument->InsertStateChange ( *Data->pDocument->Query_State(), NewState, ADJUSTSTATE_JUSTIFY ) ;

  /**************************************************************************
   * Done.                                                                  *
   **************************************************************************/

   return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process Set Justification Full command.                             *
 *                                                                          *
 ****************************************************************************/

STATIC MRESULT APIENTRY SetJustFull ( HWND Window, MESG, MPARAM1, MPARAM2 ) {

  /**************************************************************************
   * Find the instance data.                                                *
   **************************************************************************/

   PDATA Data = PDATA ( Sys_GetWindowData ( Window ) ) ;

  /**************************************************************************
   * Insert the document state change.                                      *
   **************************************************************************/

   Estado NewState ;
   NewState.Justify = JUSTIFY_FULL ;
   Data->pDocument->InsertStateChange ( *Data->pDocument->Query_State(), NewState, ADJUSTSTATE_JUSTIFY ) ;

  /**************************************************************************
   * Done.                                                                  *
   **************************************************************************/

   return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process Set Page Center command.                                    *
 *                                                                          *
 ****************************************************************************/

STATIC MRESULT APIENTRY SetPageCenter ( HWND Window, MESG, MPARAM1, MPARAM2 ) {

  /**************************************************************************
   * Find the instance data.                                                *
   **************************************************************************/

   PDATA Data = PDATA ( Sys_GetWindowData ( Window ) ) ;

  /**************************************************************************
   * Insert the document state change.                                      *
   **************************************************************************/

   Estado NewState ;
   NewState.CenterPage = USHORT ( Data->pDocument->Query_State()->CenterPage AND NOT Data->pDocument->HasSelection() ? FALSE : TRUE ) ;
   Data->pDocument->InsertStateChange ( *Data->pDocument->Query_State(), NewState, ADJUSTSTATE_CENTERPAGE ) ;

  /**************************************************************************
   * Done.                                                                  *
   **************************************************************************/

   return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process Page Break command.                                         *
 *                                                                          *
 ****************************************************************************/

STATIC MRESULT APIENTRY PageBreak ( HWND Window, MESG, MPARAM1, MPARAM2 ) {

  /**************************************************************************
   * Find the instance data.                                                *
   **************************************************************************/

   PDATA Data = PDATA ( Sys_GetWindowData ( Window ) ) ;

  /**************************************************************************
   * Insert a page-break character into the data-stream.                    *
   **************************************************************************/

   Data->pDocument->Enqueue_InsertKey ( USHORT('\f') ) ;

  /**************************************************************************
   * Done.                                                                  *
   **************************************************************************/

   return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process Insert Today's Date command.                                *
 *                                                                          *
 ****************************************************************************/

STATIC MRESULT APIENTRY DateToday ( HWND Window, MESG, MPARAM1, MPARAM2 ) {

  /**************************************************************************
   * Find the instance data.                                                *
   **************************************************************************/

   PDATA Data = PDATA ( Sys_GetWindowData ( Window ) ) ;

  /**************************************************************************
   * Insert today's date into the document before the cursor.               *
   **************************************************************************/

   char Text [80] ;
   sprintf ( Text, "%c%s%i%c", FUNCTION_START, FunctionNames[FN__DATEFIXED], time(0), FUNCTION_END ) ;

   Data->pDocument->Enqueue_InsertText ( Text ) ;

  /**************************************************************************
   * Done.                                                                  *
   **************************************************************************/

   return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process Insert Current Date command.                                *
 *                                                                          *
 ****************************************************************************/

STATIC MRESULT APIENTRY DateCurrent ( HWND Window, MESG, MPARAM1, MPARAM2 ) {

  /**************************************************************************
   * Find the instance data.                                                *
   **************************************************************************/

   PDATA Data = PDATA ( Sys_GetWindowData ( Window ) ) ;

  /**************************************************************************
   * Insert current date function into the document before the cursor.      *
   **************************************************************************/

   char Text [80] ;
   sprintf ( Text, "%c%s%c", FUNCTION_START, FunctionNames[FN__DATECURRENT], FUNCTION_END ) ;

   Data->pDocument->Enqueue_InsertText ( Text ) ;

  /**************************************************************************
   * Done.                                                                  *
   **************************************************************************/

   return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process Insert Changed Date command.                                *
 *                                                                          *
 ****************************************************************************/

STATIC MRESULT APIENTRY DateChanged ( HWND Window, MESG, MPARAM1, MPARAM2 ) {

  /**************************************************************************
   * Find the instance data.                                                *
   **************************************************************************/

   PDATA Data = PDATA ( Sys_GetWindowData ( Window ) ) ;

  /**************************************************************************
   * Insert document change date function into the document before the curso*
   **************************************************************************/

   char Text [80] ;
   sprintf ( Text, "%c%s%c", FUNCTION_START, FunctionNames[FN__DATECHANGED], FUNCTION_END ) ;

   Data->pDocument->Enqueue_InsertText ( Text ) ;

  /**************************************************************************
   * Done.                                                                  *
   **************************************************************************/

   return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process Date Format command.                                        *
 *                                                                          *
 ****************************************************************************/

STATIC MRESULT APIENTRY SetDateFormat ( HWND Window, MESG, MPARAM1, MPARAM2 ) {

  /**************************************************************************
   * Find the instance data.                                                *
   **************************************************************************/

   PDATA Data = PDATA ( Sys_GetWindowData ( Window ) ) ;

  /**************************************************************************
   * Process the Date Format command.                                       *
   **************************************************************************/

   DATEFMT_PARMS Parms ;
   Parms.Library = Data->Library->QueryHandle() ;
   strcpy ( Parms.Format, Data->pDocument->Query_State()->DateFormat ) ;

   if ( WinDlgBox ( HWND_DESKTOP, Window, DateFormatProcessor, Data->Library->QueryHandle(), IDD_DATEFMT, &Parms ) )
      ApplyDateFormat ( Window, WM_SET_DATEFMT, MPFROMP(Parms.Format), 0 ) ;

  /**************************************************************************
   * Done.                                                                  *
   **************************************************************************/

   return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process Apply Date Format command.                                  *
 *                                                                          *
 ****************************************************************************/

STATIC MRESULT APIENTRY ApplyDateFormat ( HWND Window, MESG, MPARAM1 mp1, MPARAM2 ) {

  /**************************************************************************
   * Find the instance data.                                                *
   **************************************************************************/

   PDATA Data = PDATA ( Sys_GetWindowData ( Window ) ) ;

  /**************************************************************************
   * Insert the document state change.                                      *
   **************************************************************************/

   Estado NewState ;
   memset ( NewState.DateFormat, 0, sizeof(NewState.DateFormat) ) ;
   memcpy ( NewState.DateFormat, PCHAR(mp1), strlen(PCHAR(mp1)) ) ;
   Data->pDocument->InsertStateChange ( *Data->pDocument->Query_State(), NewState, ADJUSTSTATE_DATEFORMAT ) ;

  /**************************************************************************
   * Done.                                                                  *
   **************************************************************************/

   return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process Insert Current Page Number command.                         *
 *                                                                          *
 ****************************************************************************/

STATIC MRESULT APIENTRY CurrentPage ( HWND Window, MESG, MPARAM1, MPARAM2 ) {

  /**************************************************************************
   * Find the instance data.                                                *
   **************************************************************************/

   PDATA Data = PDATA ( Sys_GetWindowData ( Window ) ) ;

  /**************************************************************************
   * Insert the Current Page Number function code before the cursor.        *
   **************************************************************************/

   char Text [100] ;
   sprintf ( Text, "%c%s%c", FUNCTION_START, FunctionNames[FN__PAGENUMBER], FUNCTION_END ) ;

   Data->pDocument->Enqueue_InsertText ( Text ) ;

  /**************************************************************************
   * Done.                                                                  *
   **************************************************************************/

   return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process Insert Total Page Count command.                            *
 *                                                                          *
 ****************************************************************************/

STATIC MRESULT APIENTRY PageCount ( HWND Window, MESG, MPARAM1, MPARAM2 ) {

  /**************************************************************************
   * Find the instance data.                                                *
   **************************************************************************/

   PDATA Data = PDATA ( Sys_GetWindowData ( Window ) ) ;

  /**************************************************************************
   * Insert the Total Page Count function code before the cursor.           *
   **************************************************************************/

   char Text [100] ;
   sprintf ( Text, "%c%s%c", FUNCTION_START, FunctionNames[FN__PAGECOUNT], FUNCTION_END ) ;

   Data->pDocument->Enqueue_InsertText ( Text ) ;

  /**************************************************************************
   * Done.                                                                  *
   **************************************************************************/

   return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process Insert Separator command.                                   *
 *                                                                          *
 ****************************************************************************/

STATIC MRESULT APIENTRY Separator ( HWND Window, MESG, MPARAM1, MPARAM2 ) {

  /**************************************************************************
   * Find the instance data.                                                *
   **************************************************************************/

   PDATA Data = PDATA ( Sys_GetWindowData ( Window ) ) ;

  /**************************************************************************
   * Insert the Separator function code before the cursor.                  *
   **************************************************************************/

   char Text [100] ;
   sprintf ( Text, "%c%s%c", FUNCTION_START, FunctionNames[FN__SEPARATOR], FUNCTION_END ) ;

   Data->pDocument->Enqueue_InsertText ( Text ) ;

  /**************************************************************************
   * Done.                                                                  *
   **************************************************************************/

   return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process Insert BarCode command.                                     *
 *                                                                          *
 ****************************************************************************/

STATIC MRESULT APIENTRY BarCode ( HWND Window, MESG, MPARAM1, MPARAM2 ) {

  /**************************************************************************
   * Find the instance data.                                                *
   **************************************************************************/

   PDATA Data = PDATA ( Sys_GetWindowData ( Window ) ) ;

  /**************************************************************************
   * Get the barcode for insertion into the document.                       *
   **************************************************************************/

   BARCODES_PARMS _Parms ;
   _Parms.Library = Data->Library->QueryHandle() ;
   strcpy ( _Parms.Type, "" ) ;
   strcpy ( _Parms.Text, "" ) ;
   if ( WinDlgBox ( HWND_DESKTOP, Window, Barcodes_Processor, Data->Library->QueryHandle(), IDD_BARCODE, &_Parms ) ) {
      memset ( &Data->PasteGraphic, 0, sizeof(Data->PasteGraphic) ) ;
      Data->PasteGraphic.Type[0] = _Parms.Type[0] ;
      Data->PasteGraphic.Type[1] = _Parms.Type[1] ;
      strcpy ( Data->PasteGraphic.BarCode, _Parms.Text ) ;
      Data->PasteGraphic.BorderWidth = GRAPHIC_BORDER ;
      Data->PasteGraphic.BorderColor = RGB_WHITE ;
      Data->BoxType = BOX_GRAPHIC ;
      Data->BoxMode = BOX_AWAITINGPOINT0 ;
      HWND Frame = PARENT ( Window ) ;
      HWND StatusWindow = CHILD ( Frame, FID_STATUSLINE ) ;
      ResourceString Message ( Data->Library->QueryHandle(), IDS_PASTEGRAPHIC0 ) ;
      Sys_SetWindowText ( StatusWindow, PSZ(Message) ) ;
   } /* endif */

  /**************************************************************************
   * Done.                                                                  *
   **************************************************************************/

   return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process Header command.                                             *
 *                                                                          *
 ****************************************************************************/

class Header_Dialog : public Dialog {

   private:
      HMODULE Library ;
      Documento *pDocument ;
      StaticText *Err ;
      Entryfield *Entry ;
      char *Format ;
      int FormatSize ;

   public:
      Header_Dialog ( HWND Parent, HWND Owner, HMODULE Resource, Documento *pdoc, char *format, int formatsize ) : 
         Dialog ( Parent, Owner, Resource, IDD_HEADER ), Library(Resource), 
         pDocument(pdoc), Format(format), FormatSize(formatsize) {
         if ( !Load ( ) )
            return ;
         Entry = new Entryfield ( CHILD ( Handle, IDD_HEADER_ENTRY ) ) ;
         Err = new StaticText ( CHILD ( Handle, IDD_HEADER_ERR ) ) ;
         Entry->SetLimit ( FormatSize-1 ) ;
         Entry->SetText ( Format ) ;
      } /* endmethod */

      ~Header_Dialog ( ) {
         delete Err ;
         delete Entry ;
      } /* endmethod */

      MRESULT Command ( MPARAM1 mp1, MPARAM2 mp2 ) ;
      MRESULT MessageProcessor ( HWND hwnd, MESG msg, MPARAM1 mp1, MPARAM2 mp2 ) ;
} ;

MRESULT Header_Dialog::Command ( MPARAM1 mp1, MPARAM2 ) {
   switch ( SHORT1FROMMP(mp1) ) {
      case IDD_HEADER_APPLY: {
         Entry->GetText ( Format, FormatSize ) ;
         HAB Anchor ( WinQueryAnchorBlock(Handle) ) ; int Version ( 0 ) ; char BadToken [80] = { 0 } ;
         int InternalSize ( ImportText ( Anchor, (unsigned char*)Format, strlen(Format), 0, Version, BadToken, sizeof(BadToken) ) ) ;
         Estado State ;
         if ( InternalSize > sizeof(State.Header) ) {
            ResourceString Format ( Library, IDS_HEADER_TOO_LARGE ) ;
            char Message [200] ; sprintf ( Message, (char*)Format, InternalSize, sizeof(State.Header) ) ;
            Err->SetText ( Message ) ;
            Sys_BeepError ( ) ;
         } else if ( BadToken[0] ) {
            if ( strlen(BadToken) > 25 ) 
               strcpy ( BadToken+25, "..." ) ;
            ResourceString Format ( Library, IDS_HEADER_BAD_FUNCTION ) ;
            char Message [200] ; sprintf ( Message, (char*)Format, sizeof(BadToken), BadToken ) ;
            Err->SetText ( Message ) ;
            Sys_BeepError ( ) ;
         } else {
            Err->SetText ( "" ) ;
            ApplyHeader ( Owner, WM_SET_HEADER, MPFROMP(Format), 0 ) ;
         } /* endif */
         Entry->SetFocus ( ) ;
         break; }
      case DID_OK: {
         Entry->GetText ( Format, FormatSize ) ;
         HAB Anchor ( WinQueryAnchorBlock(Handle) ) ; int Version ( 0 ) ; char BadToken [80] = { 0 } ;
         int InternalSize ( ImportText ( Anchor, (unsigned char*)Format, strlen(Format), 0, Version, BadToken, sizeof(BadToken) ) ) ;
         Estado State ;
         if ( InternalSize > sizeof(State.Header) ) {
            ResourceString Format ( Library, IDS_HEADER_TOO_LARGE ) ;
            char Message [200] ; sprintf ( Message, (char*)Format, InternalSize, sizeof(State.Header) ) ;
            Err->SetText ( Message ) ;
            Sys_BeepError ( ) ;
            Entry->SetFocus ( ) ;
            return ( 0 ) ;
         } else if ( BadToken[0] ) {
            if ( strlen(BadToken) > 25 ) 
               strcpy ( BadToken+25, "..." ) ;
            ResourceString Format ( Library, IDS_HEADER_BAD_FUNCTION ) ;
            char Message [200] ; sprintf ( Message, (char*)Format, sizeof(BadToken), BadToken ) ;
            Err->SetText ( Message ) ;
            Sys_BeepError ( ) ;
            Entry->SetFocus ( ) ;
            return ( 0 ) ;
         } /* endif */
         Dismiss ( TRUE ) ;
         break; }
      case DID_CANCEL: {
         Dismiss ( FALSE ) ;
         break; }
   } /* endswitch */
   return ( 0 ) ;
} /* endmethod */

MRESULT Header_Dialog::MessageProcessor ( HWND hwnd, MESG msg, MPARAM1 mp1, MPARAM2 mp2 ) {
   MRESULT Result ;
   switch ( msg ) {
      case WM_COMMAND:  Result = Command ( mp1, mp2 ) ; break ;
      default:          Result = Dialog::MessageProcessor ( hwnd, msg, mp1, mp2 ) ;
   } /* endswitch */
   return ( Result ) ;
} /* endmethod */

STATIC MRESULT APIENTRY Header ( HWND Window, MESG, MPARAM1, MPARAM2 ) {

  /**************************************************************************
   * Find the instance data.                                                *
   **************************************************************************/

   PDATA Data = PDATA ( Sys_GetWindowData ( Window ) ) ;

  /**************************************************************************
   * Build an external version of the current header definition.            *
   **************************************************************************/

   unsigned char External [500] ;
   unsigned char *End = ExportText ( WinQueryAnchorBlock(Window), Data->WindowPS, External, 
      ((unsigned char*)(Data->pDocument->Query_State()->Header))+1, 
      *(unsigned char*)(Data->pDocument->Query_State()->Header) ) ;
   *End = 0 ;

  /**************************************************************************
   * Invoke the page header definition dialog.                              *
   **************************************************************************/

   Header_Dialog ThisDialog ( HWND_DESKTOP, Window, Data->Library->QueryHandle(), Data->pDocument, (char*)External, sizeof(External) ) ;
   if ( ThisDialog.Process ( ) )
      ApplyHeader ( Window, WM_SET_HEADER, MPFROMP(External), 0 ) ;

  /**************************************************************************
   * Done.                                                                  *
   **************************************************************************/

   return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process Apply Header command.                                       *
 *                                                                          *
 ****************************************************************************/

STATIC MRESULT APIENTRY ApplyHeader ( HWND Window, MESG, MPARAM1 mp1, MPARAM2 ) {

  /**************************************************************************
   * Find the instance data.                                                *
   **************************************************************************/

   PDATA Data = PDATA ( Sys_GetWindowData ( Window ) ) ;

  /**************************************************************************
   * Convert the new header definition to internal format.                  *
   **************************************************************************/

   Estado NewState ; int Version ( 0 ) ;
   ImportText ( WinQueryAnchorBlock(Window), (unsigned char*)mp1, strlen((char*)mp1), (unsigned char*)NewState.Header, Version ) ;

  /**************************************************************************
   * Apply the new header format.                                           *
   **************************************************************************/

   Data->pDocument->InsertStateChange ( *Data->pDocument->Query_State(), NewState, ADJUSTSTATE_HEADER ) ;

  /**************************************************************************
   * Done.                                                                  *
   **************************************************************************/

   return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process Footer command.                                             *
 *                                                                          *
 ****************************************************************************/

class Footer_Dialog : public Dialog {

   private:
      HMODULE Library ;
      Documento *pDocument ;
      StaticText *Err ;
      Entryfield *Entry ;
      char *Format ;
      int FormatSize ;

   public:
      Footer_Dialog ( HWND Parent, HWND Owner, HMODULE Resource, Documento *pdoc, char *format, int formatsize ) : 
         Dialog ( Parent, Owner, Resource, IDD_FOOTER ), Library(Resource), pDocument(pdoc), Format(format), FormatSize(formatsize) {
         if ( !Load ( ) )
            return ;
         Entry = new Entryfield ( CHILD ( Handle, IDD_FOOTER_ENTRY ) ) ;
         Err = new StaticText ( CHILD ( Handle, IDD_FOOTER_ERR ) ) ;
         Entry->SetLimit ( FormatSize-1 ) ;
         Entry->SetText ( Format ) ;
      } /* endmethod */

      ~Footer_Dialog ( ) {
         delete Err ;
         delete Entry ;
      } /* endmethod */

      MRESULT Command ( MPARAM1 mp1, MPARAM2 mp2 ) ;
      MRESULT MessageProcessor ( HWND hwnd, MESG msg, MPARAM1 mp1, MPARAM2 mp2 ) ;
} ;

MRESULT Footer_Dialog::Command ( MPARAM1 mp1, MPARAM2 ) {
   switch ( SHORT1FROMMP(mp1) ) {
      case IDD_FOOTER_APPLY: {
         Entry->GetText ( Format, FormatSize ) ;
         HAB Anchor ( WinQueryAnchorBlock(Handle) ) ; int Version ( 0 ) ; char BadToken [80] = { 0 } ;
         int InternalSize ( ImportText ( Anchor, (unsigned char*)Format, strlen(Format), 0, Version, BadToken, sizeof(BadToken) ) ) ;
         Estado State ;
         if ( InternalSize > sizeof(State.Footer) ) {
            ResourceString Format ( Library, IDS_HEADER_TOO_LARGE ) ;
            char Message [200] ; sprintf ( Message, (char*)Format, InternalSize, sizeof(State.Footer) ) ;
            Err->SetText ( Message ) ;
            Sys_BeepError ( ) ;
         } else if ( BadToken[0] ) {
            if ( strlen(BadToken) > 25 ) 
               strcpy ( BadToken+25, "..." ) ;
            ResourceString Format ( Library, IDS_HEADER_BAD_FUNCTION ) ;
            char Message [200] ; sprintf ( Message, (char*)Format, sizeof(BadToken), BadToken ) ;
            Err->SetText ( Message ) ;
            Sys_BeepError ( ) ;
         } else {
            Err->SetText ( "" ) ;
            ApplyFooter ( Owner, WM_SET_FOOTER, MPFROMP(Format), 0 ) ;
         } /* endif */
         Entry->SetFocus ( ) ;
         break; }
      case DID_OK: {
         Entry->GetText ( Format, FormatSize ) ;
         HAB Anchor ( WinQueryAnchorBlock(Handle) ) ; int Version ( 0 ) ; char BadToken [80] = { 0 } ;
         int InternalSize ( ImportText ( Anchor, (unsigned char*)Format, strlen(Format), 0, Version, BadToken, sizeof(BadToken) ) ) ;
         Estado State ;
         if ( InternalSize > sizeof(State.Footer) ) {
            ResourceString Format ( Library, IDS_HEADER_TOO_LARGE ) ;
            char Message [200] ; sprintf ( Message, (char*)Format, InternalSize, sizeof(State.Footer) ) ;
            Err->SetText ( Message ) ;
            Sys_BeepError ( ) ;
            Entry->SetFocus ( ) ;
            return ( 0 ) ;
         } else if ( BadToken[0] ) {
            if ( strlen(BadToken) > 25 ) 
               strcpy ( BadToken+25, "..." ) ;
            ResourceString Format ( Library, IDS_HEADER_BAD_FUNCTION ) ;
            char Message [200] ; sprintf ( Message, (char*)Format, sizeof(BadToken), BadToken ) ;
            Err->SetText ( Message ) ;
            Sys_BeepError ( ) ;
            Entry->SetFocus ( ) ;
            return ( 0 ) ;
         } /* endif */
         Dismiss ( TRUE ) ;
         break; }
      case DID_CANCEL: {
         Dismiss ( FALSE ) ;
         break; }
   } /* endswitch */
   return ( 0 ) ;
} /* endmethod */

MRESULT Footer_Dialog::MessageProcessor ( HWND hwnd, MESG msg, MPARAM1 mp1, MPARAM2 mp2 ) {
   MRESULT Result ;
   switch ( msg ) {
      case WM_COMMAND:  Result = Command ( mp1, mp2 ) ; break ;
      default:          Result = Dialog::MessageProcessor ( hwnd, msg, mp1, mp2 ) ;
   } /* endswitch */
   return ( Result ) ;
} /* endmethod */

STATIC MRESULT APIENTRY Footer ( HWND Window, MESG, MPARAM1, MPARAM2 ) {

  /**************************************************************************
   * Find the instance data.                                                *
   **************************************************************************/

   PDATA Data = PDATA ( Sys_GetWindowData ( Window ) ) ;

  /**************************************************************************
   * Build an external version of the current header definition.            *
   **************************************************************************/

   unsigned char External [500] ;
   unsigned char *End = ExportText ( WinQueryAnchorBlock(Window), Data->WindowPS, External, 
      ((unsigned char*)(Data->pDocument->Query_State()->Footer))+1, 
      *(unsigned char*)(Data->pDocument->Query_State()->Footer) ) ;
   *End = 0 ;

  /**************************************************************************
   * Invoke the page footer definition dialog.                              *
   **************************************************************************/

   Footer_Dialog ThisDialog ( HWND_DESKTOP, Window, Data->Library->QueryHandle(), Data->pDocument, (char*)External, sizeof(External) ) ;
   if ( ThisDialog.Process ( ) )
      ApplyFooter ( Window, WM_SET_FOOTER, MPFROMP(External), 0 ) ;

  /**************************************************************************
   * Done.                                                                  *
   **************************************************************************/

  return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process Apply Footer command.                                       *
 *                                                                          *
 ****************************************************************************/

STATIC MRESULT APIENTRY ApplyFooter ( HWND Window, MESG, MPARAM1 mp1, MPARAM2 ) {

  /**************************************************************************
   * Find the instance data.                                                *
   **************************************************************************/

   PDATA Data = PDATA ( Sys_GetWindowData ( Window ) ) ;

  /**************************************************************************
   * Convert the new header definition to internal format.                  *
   **************************************************************************/

   Estado NewState ; int Version ( 0 ) ;
   ImportText ( WinQueryAnchorBlock(Window), (unsigned char*)mp1, strlen((char*)mp1), (unsigned char*)NewState.Footer, Version ) ;

  /**************************************************************************
   * Apply the new header format.                                           *
   **************************************************************************/

   Data->pDocument->InsertStateChange ( *Data->pDocument->Query_State(), NewState, ADJUSTSTATE_FOOTER ) ;

  /**************************************************************************
   * Done.                                                                  *
   **************************************************************************/

   return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process Set Font Name command.                                      *
 *                                                                          *
 ****************************************************************************/

STATIC MRESULT APIENTRY SetFontname ( HWND Window, MESG, MPARAM1 mp1, MPARAM2 ) {

  /**************************************************************************
   * Find the instance data.                                                *
   **************************************************************************/

   PDATA Data = PDATA ( Sys_GetWindowData ( Window ) ) ;

  /**************************************************************************
   * Build the appropriate function text.                                   *
   **************************************************************************/

   PSZ Font = (PSZ) mp1 ;

  /**************************************************************************
   * Insert the document state change.                                      *
   **************************************************************************/

   Estado NewState ;
   memset ( NewState.Font, 0, sizeof(NewState.Font) ) ;
   strcpy ( NewState.Font, PCHAR(Font) ) ;
   Data->pDocument->InsertStateChange ( *Data->pDocument->Query_State(), NewState, ADJUSTSTATE_FONT ) ;

  /**************************************************************************
   * Done.                                                                  *
   **************************************************************************/

   return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process Font Menu Page Up command.                                  *
 *                                                                          *
 ****************************************************************************/

STATIC MRESULT APIENTRY FontsPageUp ( HWND Window, MESG, MPARAM1, MPARAM2 ) {

  /**************************************************************************
   * Find the instance data.                                                *
   **************************************************************************/

   PDATA Data = PDATA ( Sys_GetWindowData ( Window ) ) ;

  /**************************************************************************
   * Process the Fonts Page Up command.                                     *
   **************************************************************************/

   Data->FontMenuIndex = USHORT ( max ( 0, Data->FontMenuIndex - 20 ) ) ;

   WinPopupMenu ( HWND_DESKTOP, Window, Data->TextMenu, Data->MenuPoint.x, Data->MenuPoint.y,
      IDM_FONT_MENU, PU_HCONSTRAIN | PU_VCONSTRAIN | PU_KEYBOARD | PU_MOUSEBUTTON1 | PU_SELECTITEM ) ;

  /**************************************************************************
   * Done.                                                                  *
   **************************************************************************/

   return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process Font Menu Page Down command.                                *
 *                                                                          *
 ****************************************************************************/

STATIC MRESULT APIENTRY FontsPageDown ( HWND Window, MESG, MPARAM1, MPARAM2 ) {

  /**************************************************************************
   * Find the instance data.                                                *
   **************************************************************************/

   PDATA Data = PDATA ( Sys_GetWindowData ( Window ) ) ;

  /**************************************************************************
   * Process the Fonts Page Down command.                                   *
   **************************************************************************/

   Data->FontMenuIndex += 20 ;

   WinPopupMenu ( HWND_DESKTOP, Window, Data->TextMenu, Data->MenuPoint.x, Data->MenuPoint.y,
      IDM_FONT_MENU, PU_HCONSTRAIN | PU_VCONSTRAIN | PU_KEYBOARD | PU_MOUSEBUTTON1 | PU_SELECTITEM ) ;

  /**************************************************************************
   * Done.                                                                  *
   **************************************************************************/

   return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process Font Manager command.                                       *
 *                                                                          *
 ****************************************************************************/

STATIC MRESULT APIENTRY FontManager ( HWND, MESG, MPARAM1, MPARAM2 ) {

  /**************************************************************************
   * Find the instance data.                                                *
   **************************************************************************/

// PDATA Data = PDATA ( Sys_GetWindowData ( Window ) ) ;

  /**************************************************************************
   * Process the Font Manager command.                                      *
   **************************************************************************/

   Sys_BeepNote ( ) ;

  /**************************************************************************
   * Done.                                                                  *
   **************************************************************************/

   return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process Set Size command.                                           *
 *                                                                          *
 ****************************************************************************/

STATIC MRESULT APIENTRY SetSize ( HWND Window, MESG, MPARAM1 mp1, MPARAM2 ) {

  /**************************************************************************
   * Find the instance data.                                                *
   **************************************************************************/

   PDATA Data = PDATA ( Sys_GetWindowData ( Window ) ) ;

  /**************************************************************************
   * Insert the document state change.                                      *
   **************************************************************************/

   Estado NewState ;
   NewState.Size = SHORT1FROMMP ( mp1 ) ;
   Data->pDocument->InsertStateChange ( *Data->pDocument->Query_State(), NewState, ADJUSTSTATE_SIZE ) ;

  /**************************************************************************
   * Done.                                                                  *
   **************************************************************************/

   return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process Size Menu Page Up command.                                  *
 *                                                                          *
 ****************************************************************************/

STATIC MRESULT APIENTRY SizesPageUp ( HWND Window, MESG, MPARAM1, MPARAM2 ) {

  /**************************************************************************
   * Find the instance data.                                                *
   **************************************************************************/

   PDATA Data = PDATA ( Sys_GetWindowData ( Window ) ) ;

  /**************************************************************************
   * Process the Sizes Page Up command.                                     *
   **************************************************************************/

   Data->SizeMenuIndex = USHORT ( max ( 0, Data->SizeMenuIndex - 20 ) ) ;

   WinPopupMenu ( HWND_DESKTOP, Window, Data->TextMenu, Data->MenuPoint.x, Data->MenuPoint.y,
      IDM_SIZE_MENU, PU_HCONSTRAIN | PU_VCONSTRAIN | PU_KEYBOARD | PU_MOUSEBUTTON1 | PU_SELECTITEM ) ;

  /**************************************************************************
   * Done.                                                                  *
   **************************************************************************/

   return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process Size Menu Page Down command.                                *
 *                                                                          *
 ****************************************************************************/

STATIC MRESULT APIENTRY SizesPageDown ( HWND Window, MESG, MPARAM1, MPARAM2 ) {

  /**************************************************************************
   * Find the instance data.                                                *
   **************************************************************************/

   PDATA Data = PDATA ( Sys_GetWindowData ( Window ) ) ;

  /**************************************************************************
   * Process the Sizes Page Down command.                                   *
   **************************************************************************/

   Data->SizeMenuIndex += 20 ;

   WinPopupMenu ( HWND_DESKTOP, Window, Data->TextMenu, Data->MenuPoint.x, Data->MenuPoint.y,
      IDM_SIZE_MENU, PU_HCONSTRAIN | PU_VCONSTRAIN | PU_KEYBOARD | PU_MOUSEBUTTON1 | PU_SELECTITEM ) ;

  /**************************************************************************
   * Done.                                                                  *
   **************************************************************************/

   return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process Size Manager command.                                       *
 *                                                                          *
 ****************************************************************************/

STATIC MRESULT APIENTRY SizeManager ( HWND, MESG, MPARAM1, MPARAM2 ) {

  /**************************************************************************
   * Find the instance data.                                                *
   **************************************************************************/

// PDATA Data = PDATA ( Sys_GetWindowData ( Window ) ) ;

  /**************************************************************************
   * Process the Size Manager command.                                      *
   **************************************************************************/

   Sys_BeepNote ( ) ;

  /**************************************************************************
   * Done.                                                                  *
   **************************************************************************/

   return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process Set Normal command.                                         *
 *                                                                          *
 ****************************************************************************/

STATIC MRESULT APIENTRY SetNormal ( HWND Window, MESG, MPARAM1, MPARAM2 ) {

  /**************************************************************************
   * Find the instance data.                                                *
   **************************************************************************/

   PDATA Data = PDATA ( Sys_GetWindowData ( Window ) ) ;

  /**************************************************************************
   * Insert the document state change.                                      *
   **************************************************************************/

   Estado NewState ;
   NewState.Bold = FALSE ;
   NewState.Italic = FALSE ;
   NewState.Underscore = FALSE ;
   NewState.Strikeout = FALSE ;
   NewState.Outline = FALSE ;
   NewState.Caps = FALSE ;
   Data->pDocument->InsertStateChange ( *Data->pDocument->Query_State(), NewState, ADJUSTSTATE_ATTRIBUTES ) ;

  /**************************************************************************
   * Done.                                                                  *
   **************************************************************************/

   return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process Set Bold command.                                           *
 *                                                                          *
 ****************************************************************************/

STATIC MRESULT APIENTRY SetBold ( HWND Window, MESG, MPARAM1, MPARAM2 ) {

  /**************************************************************************
   * Find the instance data.                                                *
   **************************************************************************/

   PDATA Data = PDATA ( Sys_GetWindowData ( Window ) ) ;

  /**************************************************************************
   * Insert the document state change.                                      *
   **************************************************************************/

   Estado NewState ;
   NewState.Bold = USHORT ( Data->pDocument->Query_State()->Bold AND NOT Data->pDocument->HasSelection() ? FALSE : TRUE ) ;
   Data->pDocument->InsertStateChange ( *Data->pDocument->Query_State(), NewState, ADJUSTSTATE_BOLD ) ;

  /**************************************************************************
   * Done.                                                                  *
   **************************************************************************/

   return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process Set Italic command.                                         *
 *                                                                          *
 ****************************************************************************/

STATIC MRESULT APIENTRY SetItalic ( HWND Window, MESG, MPARAM1, MPARAM2 ) {

  /**************************************************************************
   * Find the instance data.                                                *
   **************************************************************************/

   PDATA Data = PDATA ( Sys_GetWindowData ( Window ) ) ;

  /**************************************************************************
   * Insert the document state change.                                      *
   **************************************************************************/

   Estado NewState ;
   NewState.Italic = USHORT ( Data->pDocument->Query_State()->Italic AND NOT Data->pDocument->HasSelection() ? FALSE : TRUE ) ;
   Data->pDocument->InsertStateChange ( *Data->pDocument->Query_State(), NewState, ADJUSTSTATE_ITALIC ) ;

  /**************************************************************************
   * Done.                                                                  *
   **************************************************************************/

   return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process Set Underscore command.                                     *
 *                                                                          *
 ****************************************************************************/

STATIC MRESULT APIENTRY SetUnderscore ( HWND Window, MESG, MPARAM1, MPARAM2 ) {

  /**************************************************************************
   * Find the instance data.                                                *
   **************************************************************************/

   PDATA Data = PDATA ( Sys_GetWindowData ( Window ) ) ;

  /**************************************************************************
   * Insert the document state change.                                      *
   **************************************************************************/

   Estado NewState ;
   NewState.Underscore = USHORT ( Data->pDocument->Query_State()->Underscore AND NOT Data->pDocument->HasSelection() ? FALSE : TRUE ) ;
   Data->pDocument->InsertStateChange ( *Data->pDocument->Query_State(), NewState, ADJUSTSTATE_UNDERSCORE ) ;

  /**************************************************************************
   * Done.                                                                  *
   **************************************************************************/

   return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process Set Strikeout command.                                      *
 *                                                                          *
 ****************************************************************************/

STATIC MRESULT APIENTRY SetStrikeout ( HWND Window, MESG, MPARAM1, MPARAM2 ) {

  /**************************************************************************
   * Find the instance data.                                                *
   **************************************************************************/

   PDATA Data = PDATA ( Sys_GetWindowData ( Window ) ) ;

  /**************************************************************************
   * Insert the document state change.                                      *
   **************************************************************************/

   Estado NewState ;
   NewState.Strikeout = USHORT ( Data->pDocument->Query_State()->Strikeout AND NOT Data->pDocument->HasSelection() ? FALSE : TRUE ) ;
   Data->pDocument->InsertStateChange ( *Data->pDocument->Query_State(), NewState, ADJUSTSTATE_STRIKEOUT ) ;

  /**************************************************************************
   * Done.                                                                  *
   **************************************************************************/

   return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process Set Outline command.                                        *
 *                                                                          *
 ****************************************************************************/

STATIC MRESULT APIENTRY SetOutline ( HWND Window, MESG, MPARAM1, MPARAM2 ) {

  /**************************************************************************
   * Find the instance data.                                                *
   **************************************************************************/

   PDATA Data = PDATA ( Sys_GetWindowData ( Window ) ) ;

  /**************************************************************************
   * Insert the document state change.                                      *
   **************************************************************************/

   Estado NewState ;
   NewState.Outline = USHORT ( Data->pDocument->Query_State()->Outline AND NOT Data->pDocument->HasSelection() ? FALSE : TRUE ) ;
   Data->pDocument->InsertStateChange ( *Data->pDocument->Query_State(), NewState, ADJUSTSTATE_OUTLINE ) ;

  /**************************************************************************
   * Done.                                                                  *
   **************************************************************************/

   return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process Set Caps command.                                           *
 *                                                                          *
 ****************************************************************************/

STATIC MRESULT APIENTRY SetCaps ( HWND Window, MESG, MPARAM1, MPARAM2 ) {

  /**************************************************************************
   * Find the instance data.                                                *
   **************************************************************************/

   PDATA Data = PDATA ( Sys_GetWindowData ( Window ) ) ;

  /**************************************************************************
   * Insert the document state change.                                      *
   **************************************************************************/

   Estado NewState ;
   NewState.Caps = USHORT ( Data->pDocument->Query_State()->Caps AND NOT Data->pDocument->HasSelection() ? FALSE : TRUE ) ;
   Data->pDocument->InsertStateChange ( *Data->pDocument->Query_State(), NewState, ADJUSTSTATE_CAPS ) ;

  /**************************************************************************
   * Done.                                                                  *
   **************************************************************************/

   return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process Expand commands.                                            *
 *                                                                          *
 ****************************************************************************/

STATIC MRESULT APIENTRY SetExpand ( HWND Window, MESG, MPARAM1 mp1, MPARAM2 ) {

  /**************************************************************************
   * Find the instance data.                                                *
   **************************************************************************/

   PDATA Data = PDATA ( Sys_GetWindowData ( Window ) ) ;

  /**************************************************************************
   * Insert the document state change.                                      *
   **************************************************************************/

   Estado NewState ;
   NewState.Expand = SHORT1FROMMP ( mp1 ) ;
   Data->pDocument->InsertStateChange ( *Data->pDocument->Query_State(), NewState, ADJUSTSTATE_EXPAND ) ;

  /**************************************************************************
   * Done.                                                                  *
   **************************************************************************/

   return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process Expand Menu Page Up command.                                *
 *                                                                          *
 ****************************************************************************/

STATIC MRESULT APIENTRY ExpandPageUp ( HWND Window, MESG, MPARAM1, MPARAM2 ) {

  /**************************************************************************
   * Find the instance data.                                                *
   **************************************************************************/

   PDATA Data = PDATA ( Sys_GetWindowData ( Window ) ) ;

  /**************************************************************************
   * Process the Expansion Page Up command.                                 *
   **************************************************************************/

   Data->ExpandMenuIndex = USHORT ( max ( 0, Data->ExpandMenuIndex - 20 ) ) ;

   WinPopupMenu ( HWND_DESKTOP, Window, Data->TextMenu, Data->MenuPoint.x, Data->MenuPoint.y,
      IDM_EXPAND_MENU, PU_HCONSTRAIN | PU_VCONSTRAIN | PU_KEYBOARD | PU_MOUSEBUTTON1 | PU_SELECTITEM ) ;

  /**************************************************************************
   * Done.                                                                  *
   **************************************************************************/

   return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process Expand Menu Page Down command.                              *
 *                                                                          *
 ****************************************************************************/

STATIC MRESULT APIENTRY ExpandPageDown ( HWND Window, MESG, MPARAM1, MPARAM2 ) {

  /**************************************************************************
   * Find the instance data.                                                *
   **************************************************************************/

   PDATA Data = PDATA ( Sys_GetWindowData ( Window ) ) ;

  /**************************************************************************
   * Process the Expansion Page Down command.                               *
   **************************************************************************/

   Data->ExpandMenuIndex += 20 ;

   WinPopupMenu ( HWND_DESKTOP, Window, Data->TextMenu, Data->MenuPoint.x, Data->MenuPoint.y,
      IDM_EXPAND_MENU, PU_HCONSTRAIN | PU_VCONSTRAIN | PU_KEYBOARD | PU_MOUSEBUTTON1 | PU_SELECTITEM ) ;

  /**************************************************************************
   * Done.                                                                  *
   **************************************************************************/

   return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process Expand Manager command.                                     *
 *                                                                          *
 ****************************************************************************/

STATIC MRESULT APIENTRY ExpandManager ( HWND, MESG, MPARAM1, MPARAM2 ) {

  /**************************************************************************
   * Find the instance data.                                                *
   **************************************************************************/

// PDATA Data = PDATA ( Sys_GetWindowData ( Window ) ) ;

  /**************************************************************************
   * Process the Color Manager command.                                     *
   **************************************************************************/

   Sys_BeepNote ( ) ;

  /**************************************************************************
   * Done.                                                                  *
   **************************************************************************/

   return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process Set Line Color command.                                     *
 *                                                                          *
 ****************************************************************************/

STATIC MRESULT APIENTRY SetLineColor ( HWND Window, MESG, MPARAM1 mp1, MPARAM2 ) {

  /**************************************************************************
   * Find the instance data.                                                *
   **************************************************************************/

   PDATA Data = PDATA ( Sys_GetWindowData ( Window ) ) ;

  /**************************************************************************
   * Insert the document state change.                                      *
   **************************************************************************/

   Estado NewState ;
   NewState.Color = COLOR ( mp1 ) ;
   Data->pDocument->InsertStateChange ( *Data->pDocument->Query_State(), NewState, ADJUSTSTATE_LINECOLOR ) ;

  /**************************************************************************
   * Done.                                                                  *
   **************************************************************************/

   return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process Set Line Width command.                                     *
 *                                                                          *
 ****************************************************************************/

STATIC MRESULT APIENTRY SetLineWidth ( HWND Window, MESG, MPARAM1 mp1, MPARAM2 ) {

  /**************************************************************************
   * Find the instance data.                                                *
   **************************************************************************/

   PDATA Data = PDATA ( Sys_GetWindowData ( Window ) ) ;

  /**************************************************************************
   * Insert the document state change.                                      *
   **************************************************************************/

   Estado NewState ;
   NewState.LineWidth = int ( ULONG ( mp1 ) ) ;
   Data->pDocument->InsertStateChange ( *Data->pDocument->Query_State(), NewState, ADJUSTSTATE_LINEWIDTH ) ;

  /**************************************************************************
   * Done.                                                                  *
   **************************************************************************/

   return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process Set Fill Color command.                                     *
 *                                                                          *
 ****************************************************************************/

STATIC MRESULT APIENTRY SetFillColor ( HWND Window, MESG, MPARAM1 mp1, MPARAM2 ) {

  /**************************************************************************
   * Find the instance data.                                                *
   **************************************************************************/

   PDATA Data = PDATA ( Sys_GetWindowData ( Window ) ) ;

  /**************************************************************************
   * Insert the document state change.                                      *
   **************************************************************************/

   Estado NewState ;
   NewState.FillColor = COLOR ( mp1 ) ;
   Data->pDocument->InsertStateChange ( *Data->pDocument->Query_State(), NewState, ADJUSTSTATE_FILLCOLOR ) ;

  /**************************************************************************
   * Done.                                                                  *
   **************************************************************************/

   return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process Set Fill Type command.                                      *
 *                                                                          *
 ****************************************************************************/

STATIC MRESULT APIENTRY SetFillType ( HWND Window, MESG, MPARAM1 mp1, MPARAM2 ) {

  /**************************************************************************
   * Find the instance data.                                                *
   **************************************************************************/

   PDATA Data = PDATA ( Sys_GetWindowData ( Window ) ) ;

  /**************************************************************************
   * Insert the document state change.                                      *
   **************************************************************************/

   Estado NewState ;
   NewState.FillType = SHORT1FROMMP ( mp1 ) ;
   Data->pDocument->InsertStateChange ( *Data->pDocument->Query_State(), NewState, ADJUSTSTATE_FILLTYPE ) ;

  /**************************************************************************
   * Done.                                                                  *
   **************************************************************************/

   return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process Set Line End command.                                       *
 *                                                                          *
 ****************************************************************************/

STATIC MRESULT APIENTRY SetLineEnd ( HWND Window, MESG, MPARAM1 mp1, MPARAM2 ) {

  /**************************************************************************
   * Find the instance data.                                                *
   **************************************************************************/

   PDATA Data = PDATA ( Sys_GetWindowData ( Window ) ) ;

  /**************************************************************************
   * Insert the document state change.                                      *
   **************************************************************************/

   Estado NewState ;
   NewState.LineEnd = SHORT1FROMMP ( mp1 ) ;
   Data->pDocument->InsertStateChange ( *Data->pDocument->Query_State(), NewState, ADJUSTSTATE_LINEEND ) ;

  /**************************************************************************
   * Done.                                                                  *
   **************************************************************************/

   return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process Set Line Join command.                                      *
 *                                                                          *
 ****************************************************************************/

STATIC MRESULT APIENTRY SetLineJoin ( HWND Window, MESG, MPARAM1 mp1, MPARAM2 ) {

  /**************************************************************************
   * Find the instance data.                                                *
   **************************************************************************/

   PDATA Data = PDATA ( Sys_GetWindowData ( Window ) ) ;

  /**************************************************************************
   * Insert the document state change.                                      *
   **************************************************************************/

   Estado NewState ;
   NewState.LineJoin = SHORT1FROMMP ( mp1 ) ;
   Data->pDocument->InsertStateChange ( *Data->pDocument->Query_State(), NewState, ADJUSTSTATE_LINEJOIN ) ;

  /**************************************************************************
   * Done.                                                                  *
   **************************************************************************/

   return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process Set Line Spacing command.                                   *
 *                                                                          *
 ****************************************************************************/

STATIC MRESULT APIENTRY SetLineSpacing ( HWND Window, MESG, MPARAM1 mp1, MPARAM2 ) {

  /**************************************************************************
   * Find the instance data.                                                *
   **************************************************************************/

   PDATA Data = PDATA ( Sys_GetWindowData ( Window ) ) ;

  /**************************************************************************
   * Insert the document state change.                                      *
   **************************************************************************/

   Estado NewState ;
   NewState.LineSpacing = SHORT1FROMMP ( mp1 ) ;
   Data->pDocument->InsertStateChange ( *Data->pDocument->Query_State(), NewState, ADJUSTSTATE_LINESPACING ) ;

  /**************************************************************************
   * Done.                                                                  *
   **************************************************************************/

   return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process Line Color Menu Page Up command.                            *
 *                                                                          *
 ****************************************************************************/

STATIC MRESULT APIENTRY LineColorsPageUp ( HWND Window, MESG, MPARAM1, MPARAM2 ) {

  /**************************************************************************
   * Find the instance data.                                                *
   **************************************************************************/

   PDATA Data = PDATA ( Sys_GetWindowData ( Window ) ) ;

  /**************************************************************************
   * Process the Line Colors Page Up command.                               *
   **************************************************************************/

   Data->LineColorMenuIndex = USHORT ( max ( 0, Data->LineColorMenuIndex - 20 ) ) ;

   WinPopupMenu ( HWND_DESKTOP, Window, Data->TextMenu, Data->MenuPoint.x, Data->MenuPoint.y,
      IDM_LINECOLOR_MENU, PU_HCONSTRAIN | PU_VCONSTRAIN | PU_KEYBOARD | PU_MOUSEBUTTON1 | PU_SELECTITEM ) ;

  /**************************************************************************
   * Done.                                                                  *
   **************************************************************************/

   return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process Line Color Menu Page Down command.                          *
 *                                                                          *
 ****************************************************************************/

STATIC MRESULT APIENTRY LineColorsPageDown ( HWND Window, MESG, MPARAM1, MPARAM2 ) {

  /**************************************************************************
   * Find the instance data.                                                *
   **************************************************************************/

   PDATA Data = PDATA ( Sys_GetWindowData ( Window ) ) ;

  /**************************************************************************
   * Process the Line Colors Page Down command.                             *
   **************************************************************************/

   Data->LineColorMenuIndex += 20 ;

   WinPopupMenu ( HWND_DESKTOP, Window, Data->TextMenu, Data->MenuPoint.x, Data->MenuPoint.y,
      IDM_LINECOLOR_MENU, PU_HCONSTRAIN | PU_VCONSTRAIN | PU_KEYBOARD | PU_MOUSEBUTTON1 | PU_SELECTITEM ) ;

  /**************************************************************************
   * Done.                                                                  *
   **************************************************************************/

   return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process Line Color Manager command.                                 *
 *                                                                          *
 ****************************************************************************/

STATIC MRESULT APIENTRY LineColorManager ( HWND, MESG, MPARAM1, MPARAM2 ) {

  /**************************************************************************
   * Find the instance data.                                                *
   **************************************************************************/

// PDATA Data = PDATA ( Sys_GetWindowData ( Window ) ) ;

  /**************************************************************************
   * Process the Line Color Manager command.                                *
   **************************************************************************/

   Sys_BeepNote ( ) ;

  /**************************************************************************
   * Done.                                                                  *
   **************************************************************************/

   return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process Line Width Menu Page Up command.                            *
 *                                                                          *
 ****************************************************************************/

STATIC MRESULT APIENTRY LineWidthsPageUp ( HWND Window, MESG, MPARAM1, MPARAM2 ) {

  /**************************************************************************
   * Find the instance data.                                                *
   **************************************************************************/

   PDATA Data = PDATA ( Sys_GetWindowData ( Window ) ) ;

  /**************************************************************************
   * Process the Line Widths Page Up command.                               *
   **************************************************************************/

   Data->LineWidthMenuIndex = USHORT ( max ( 0, Data->LineWidthMenuIndex - 20 ) ) ;

   WinPopupMenu ( HWND_DESKTOP, Window, Data->TextMenu, Data->MenuPoint.x, Data->MenuPoint.y,
      IDM_LINEWIDTH_MENU, PU_HCONSTRAIN | PU_VCONSTRAIN | PU_KEYBOARD | PU_MOUSEBUTTON1 | PU_SELECTITEM ) ;

  /**************************************************************************
   * Done.                                                                  *
   **************************************************************************/

   return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process Line Width Menu Page Down command.                          *
 *                                                                          *
 ****************************************************************************/

STATIC MRESULT APIENTRY LineWidthsPageDown ( HWND Window, MESG, MPARAM1, MPARAM2 ) {

  /**************************************************************************
   * Find the instance data.                                                *
   **************************************************************************/

   PDATA Data = PDATA ( Sys_GetWindowData ( Window ) ) ;

  /**************************************************************************
   * Process the Line Widths Page Down command.                             *
   **************************************************************************/

   Data->LineWidthMenuIndex += 20 ;

   WinPopupMenu ( HWND_DESKTOP, Window, Data->TextMenu, Data->MenuPoint.x, Data->MenuPoint.y,
      IDM_LINEWIDTH_MENU, PU_HCONSTRAIN | PU_VCONSTRAIN | PU_KEYBOARD | PU_MOUSEBUTTON1 | PU_SELECTITEM ) ;

  /**************************************************************************
   * Done.                                                                  *
   **************************************************************************/

   return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process Line Width Manager command.                                 *
 *                                                                          *
 ****************************************************************************/

STATIC MRESULT APIENTRY LineWidthManager ( HWND, MESG, MPARAM1, MPARAM2 ) {

  /**************************************************************************
   * Find the instance data.                                                *
   **************************************************************************/

// PDATA Data = PDATA ( Sys_GetWindowData ( Window ) ) ;

  /**************************************************************************
   * Process the Line Width Manager command.                                *
   **************************************************************************/

   Sys_BeepNote ( ) ;

  /**************************************************************************
   * Done.                                                                  *
   **************************************************************************/

   return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process Fill Color Menu Page Up command.                            *
 *                                                                          *
 ****************************************************************************/

STATIC MRESULT APIENTRY FillColorsPageUp ( HWND Window, MESG, MPARAM1, MPARAM2 ) {

  /**************************************************************************
   * Find the instance data.                                                *
   **************************************************************************/

   PDATA Data = PDATA ( Sys_GetWindowData ( Window ) ) ;

  /**************************************************************************
   * Process the Fill Colors Page Up command.                               *
   **************************************************************************/

   Data->FillColorMenuIndex = USHORT ( max ( 0, Data->FillColorMenuIndex - 20 ) ) ;

   WinPopupMenu ( HWND_DESKTOP, Window, Data->TextMenu, Data->MenuPoint.x, Data->MenuPoint.y,
      IDM_FILLCOLOR_MENU, PU_HCONSTRAIN | PU_VCONSTRAIN | PU_KEYBOARD | PU_MOUSEBUTTON1 | PU_SELECTITEM ) ;

  /**************************************************************************
   * Done.                                                                  *
   **************************************************************************/

   return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process Fill Color Menu Page Down command.                          *
 *                                                                          *
 ****************************************************************************/

STATIC MRESULT APIENTRY FillColorsPageDown ( HWND Window, MESG, MPARAM1, MPARAM2 ) {

  /**************************************************************************
   * Find the instance data.                                                *
   **************************************************************************/

   PDATA Data = PDATA ( Sys_GetWindowData ( Window ) ) ;

  /**************************************************************************
   * Process the Fill Colors Page Down command.                             *
   **************************************************************************/

   Data->FillColorMenuIndex += 20 ;

   WinPopupMenu ( HWND_DESKTOP, Window, Data->TextMenu, Data->MenuPoint.x, Data->MenuPoint.y,
      IDM_FILLCOLOR_MENU, PU_HCONSTRAIN | PU_VCONSTRAIN | PU_KEYBOARD | PU_MOUSEBUTTON1 | PU_SELECTITEM ) ;

  /**************************************************************************
   * Done.                                                                  *
   **************************************************************************/

   return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process Fill Color Manager command.                                 *
 *                                                                          *
 ****************************************************************************/

STATIC MRESULT APIENTRY FillColorManager ( HWND, MESG, MPARAM1, MPARAM2 ) {

  /**************************************************************************
   * Find the instance data.                                                *
   **************************************************************************/

// PDATA Data = PDATA ( Sys_GetWindowData ( Window ) ) ;

  /**************************************************************************
   * Process the Fill Color Manager command.                                *
   **************************************************************************/

   Sys_BeepNote ( ) ;

  /**************************************************************************
   * Done.                                                                  *
   **************************************************************************/

   return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process Fill Type Menu Page Up command.                             *
 *                                                                          *
 ****************************************************************************/

STATIC MRESULT APIENTRY FillTypesPageUp ( HWND Window, MESG, MPARAM1, MPARAM2 ) {

  /**************************************************************************
   * Find the instance data.                                                *
   **************************************************************************/

   PDATA Data = PDATA ( Sys_GetWindowData ( Window ) ) ;

  /**************************************************************************
   * Process the Fill Type Page Up command.                                 *
   **************************************************************************/

   Data->FillTypeMenuIndex = USHORT ( max ( 0, Data->FillTypeMenuIndex - 20 ) ) ;

   WinPopupMenu ( HWND_DESKTOP, Window, Data->TextMenu, Data->MenuPoint.x, Data->MenuPoint.y,
      IDM_FILLTYPE_MENU, PU_HCONSTRAIN | PU_VCONSTRAIN | PU_KEYBOARD | PU_MOUSEBUTTON1 | PU_SELECTITEM ) ;

  /**************************************************************************
   * Done.                                                                  *
   **************************************************************************/

   return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process Fill Type Menu Page Down command.                           *
 *                                                                          *
 ****************************************************************************/

STATIC MRESULT APIENTRY FillTypesPageDown ( HWND Window, MESG, MPARAM1, MPARAM2 ) {

  /**************************************************************************
   * Find the instance data.                                                *
   **************************************************************************/

   PDATA Data = PDATA ( Sys_GetWindowData ( Window ) ) ;

  /**************************************************************************
   * Process the Fill Type Page Down command.                               *
   **************************************************************************/

   Data->FillTypeMenuIndex += 20 ;

   WinPopupMenu ( HWND_DESKTOP, Window, Data->TextMenu, Data->MenuPoint.x, Data->MenuPoint.y,
      IDM_FILLTYPE_MENU, PU_HCONSTRAIN | PU_VCONSTRAIN | PU_KEYBOARD | PU_MOUSEBUTTON1 | PU_SELECTITEM ) ;

  /**************************************************************************
   * Done.                                                                  *
   **************************************************************************/

   return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process Fill Type Manager command.                                  *
 *                                                                          *
 ****************************************************************************/

STATIC MRESULT APIENTRY FillTypeManager ( HWND, MESG, MPARAM1, MPARAM2 ) {

  /**************************************************************************
   * Find the instance data.                                                *
   **************************************************************************/

// PDATA Data = PDATA ( Sys_GetWindowData ( Window ) ) ;

  /**************************************************************************
   * Process the Fill Type Manager command.                                 *
   **************************************************************************/

   Sys_BeepNote ( ) ;

  /**************************************************************************
   * Done.                                                                  *
   **************************************************************************/

   return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process Line End Menu Page Up command.                              *
 *                                                                          *
 ****************************************************************************/

STATIC MRESULT APIENTRY LineEndsPageUp ( HWND Window, MESG, MPARAM1, MPARAM2 ) {

  /**************************************************************************
   * Find the instance data.                                                *
   **************************************************************************/

   PDATA Data = PDATA ( Sys_GetWindowData ( Window ) ) ;

  /**************************************************************************
   * Process the Line End Page Up command.                                 *
   **************************************************************************/

   Data->LineEndMenuIndex = USHORT ( max ( 0, Data->LineEndMenuIndex - 20 ) ) ;

   WinPopupMenu ( HWND_DESKTOP, Window, Data->TextMenu, Data->MenuPoint.x, Data->MenuPoint.y,
      IDM_LINEEND_MENU, PU_HCONSTRAIN | PU_VCONSTRAIN | PU_KEYBOARD | PU_MOUSEBUTTON1 | PU_SELECTITEM ) ;

  /**************************************************************************
   * Done.                                                                  *
   **************************************************************************/

   return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process Line End Menu Page Down command.                            *
 *                                                                          *
 ****************************************************************************/

STATIC MRESULT APIENTRY LineEndsPageDown ( HWND Window, MESG, MPARAM1, MPARAM2 ) {

  /**************************************************************************
   * Find the instance data.                                                *
   **************************************************************************/

   PDATA Data = PDATA ( Sys_GetWindowData ( Window ) ) ;

  /**************************************************************************
   * Process the Line End Page Down command.                                *
   **************************************************************************/

   Data->LineEndMenuIndex += 20 ;

   WinPopupMenu ( HWND_DESKTOP, Window, Data->TextMenu, Data->MenuPoint.x, Data->MenuPoint.y,
      IDM_LINEEND_MENU, PU_HCONSTRAIN | PU_VCONSTRAIN | PU_KEYBOARD | PU_MOUSEBUTTON1 | PU_SELECTITEM ) ;

  /**************************************************************************
   * Done.                                                                  *
   **************************************************************************/

   return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process Line End Manager command.                                   *
 *                                                                          *
 ****************************************************************************/

STATIC MRESULT APIENTRY LineEndManager ( HWND, MESG, MPARAM1, MPARAM2 ) {

  /**************************************************************************
   * Find the instance data.                                                *
   **************************************************************************/

// PDATA Data = PDATA ( Sys_GetWindowData ( Window ) ) ;

  /**************************************************************************
   * Process the Line End Manager command.                                 *
   **************************************************************************/

   Sys_BeepNote ( ) ;

  /**************************************************************************
   * Done.                                                                  *
   **************************************************************************/

   return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process Line Join Menu Page Up command.                             *
 *                                                                          *
 ****************************************************************************/

STATIC MRESULT APIENTRY LineJoinsPageUp ( HWND Window, MESG, MPARAM1, MPARAM2 ) {

  /**************************************************************************
   * Find the instance data.                                                *
   **************************************************************************/

   PDATA Data = PDATA ( Sys_GetWindowData ( Window ) ) ;

  /**************************************************************************
   * Process the Line Join Page Up command.                                 *
   **************************************************************************/

   Data->LineJoinMenuIndex = USHORT ( max ( 0, Data->LineJoinMenuIndex - 20 ) ) ;

   WinPopupMenu ( HWND_DESKTOP, Window, Data->TextMenu, Data->MenuPoint.x, Data->MenuPoint.y,
      IDM_LINEJOIN_MENU, PU_HCONSTRAIN | PU_VCONSTRAIN | PU_KEYBOARD | PU_MOUSEBUTTON1 | PU_SELECTITEM ) ;

  /**************************************************************************
   * Done.                                                                  *
   **************************************************************************/

   return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process Line Join Menu Page Down command.                           *
 *                                                                          *
 ****************************************************************************/

STATIC MRESULT APIENTRY LineJoinsPageDown ( HWND Window, MESG, MPARAM1, MPARAM2 ) {

  /**************************************************************************
   * Find the instance data.                                                *
   **************************************************************************/

   PDATA Data = PDATA ( Sys_GetWindowData ( Window ) ) ;

  /**************************************************************************
   * Process the Line Join Page Down command.                               *
   **************************************************************************/

   Data->LineJoinMenuIndex += 20 ;

   WinPopupMenu ( HWND_DESKTOP, Window, Data->TextMenu, Data->MenuPoint.x, Data->MenuPoint.y,
      IDM_LINEJOIN_MENU, PU_HCONSTRAIN | PU_VCONSTRAIN | PU_KEYBOARD | PU_MOUSEBUTTON1 | PU_SELECTITEM ) ;

  /**************************************************************************
   * Done.                                                                  *
   **************************************************************************/

   return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process Line Join Manager command.                                  *
 *                                                                          *
 ****************************************************************************/

STATIC MRESULT APIENTRY LineJoinManager ( HWND, MESG, MPARAM1, MPARAM2 ) {

  /**************************************************************************
   * Find the instance data.                                                *
   **************************************************************************/

// PDATA Data = PDATA ( Sys_GetWindowData ( Window ) ) ;

  /**************************************************************************
   * Process the Line Join Manager command.                                 *
   **************************************************************************/

   Sys_BeepNote ( ) ;

  /**************************************************************************
   * Done.                                                                  *
   **************************************************************************/

   return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process Set Left Margin command.                                    *
 *                                                                          *
 ****************************************************************************/

STATIC MRESULT APIENTRY SetLeftMargin ( HWND Window, MESG, MPARAM1 mp1, MPARAM2 ) {

  /**************************************************************************
   * Find the instance data.                                                *
   **************************************************************************/

   PDATA Data = PDATA ( Sys_GetWindowData ( Window ) ) ;

  /**************************************************************************
   * Insert the document state change.                                      *
   **************************************************************************/

   Estado NewState ;
   NewState.Margins.xLeft = LONGFROMMP ( mp1 ) ;
   Data->pDocument->InsertStateChange ( *Data->pDocument->Query_State(), NewState, ADJUSTSTATE_LEFTMARGIN ) ;

  /**************************************************************************
   * Done.                                                                  *
   **************************************************************************/

   return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process Set Right Margin command.                                   *
 *                                                                          *
 ****************************************************************************/

STATIC MRESULT APIENTRY SetRightMargin ( HWND Window, MESG, MPARAM1 mp1, MPARAM2 ) {

  /**************************************************************************
   * Find the instance data.                                                *
   **************************************************************************/

   PDATA Data = PDATA ( Sys_GetWindowData ( Window ) ) ;

  /**************************************************************************
   * Insert the document state change.                                      *
   **************************************************************************/

   Estado NewState ;
   NewState.Margins.xRight = LONGFROMMP ( mp1 ) ;
   Data->pDocument->InsertStateChange ( *Data->pDocument->Query_State(), NewState, ADJUSTSTATE_RIGHTMARGIN ) ;

  /**************************************************************************
   * Done.                                                                  *
   **************************************************************************/

   return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process Set Bottom Margin command.                                  *
 *                                                                          *
 ****************************************************************************/

STATIC MRESULT APIENTRY SetBottomMargin ( HWND Window, MESG, MPARAM1 mp1, MPARAM2 ) {

  /**************************************************************************
   * Find the instance data.                                                *
   **************************************************************************/

   PDATA Data = PDATA ( Sys_GetWindowData ( Window ) ) ;

  /**************************************************************************
   * Insert the document state change.                                      *
   **************************************************************************/

   Estado NewState ;
   NewState.Margins.yBottom = LONGFROMMP ( mp1 ) ;
   Data->pDocument->InsertStateChange ( *Data->pDocument->Query_State(), NewState, ADJUSTSTATE_BOTTOMMARGIN ) ;

  /**************************************************************************
   * Done.                                                                  *
   **************************************************************************/

   return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process Set Top Margin command.                                     *
 *                                                                          *
 ****************************************************************************/

STATIC MRESULT APIENTRY SetTopMargin ( HWND Window, MESG, MPARAM1 mp1, MPARAM2 ) {

  /**************************************************************************
   * Find the instance data.                                                *
   **************************************************************************/

   PDATA Data = PDATA ( Sys_GetWindowData ( Window ) ) ;

  /**************************************************************************
   * Insert the document state change.                                      *
   **************************************************************************/

   Estado NewState ;
   NewState.Margins.yTop = LONGFROMMP ( mp1 ) ;
   Data->pDocument->InsertStateChange ( *Data->pDocument->Query_State(), NewState, ADJUSTSTATE_TOPMARGIN ) ;

  /**************************************************************************
   * Done.                                                                  *
   **************************************************************************/

   return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process Set Tab command.                                            *
 *                                                                          *
 ****************************************************************************/

STATIC MRESULT APIENTRY SetTab ( HWND Window, MESG, MPARAM1 mp1, MPARAM2 ) {

  /**************************************************************************
   * Find the instance data.                                                *
   **************************************************************************/

   PDATA Data = PDATA ( Sys_GetWindowData ( Window ) ) ;

  /**************************************************************************
   * Ask the document to set the new tab.                                   *
   **************************************************************************/

   Data->pDocument->SetTab ( int ( LONGFROMMP ( mp1 ) ) ) ;

  /**************************************************************************
   * Done.                                                                  *
   **************************************************************************/

   return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process Clear Tab command.                                          *
 *                                                                          *
 ****************************************************************************/

STATIC MRESULT APIENTRY ClearTab ( HWND Window, MESG, MPARAM1 mp1, MPARAM2 ) {

  /**************************************************************************
   * Find the instance data.                                                *
   **************************************************************************/

   PDATA Data = PDATA ( Sys_GetWindowData ( Window ) ) ;

  /**************************************************************************
   * Ask the document to clear the tab.                                     *
   **************************************************************************/

   Data->pDocument->ClearTab ( int ( LONGFROMMP ( mp1 ) ) ) ;

  /**************************************************************************
   * Done.                                                                  *
   **************************************************************************/

   return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process Move Tab command.                                           *
 *                                                                          *
 ****************************************************************************/

STATIC MRESULT APIENTRY MoveTab ( HWND Window, MESG, MPARAM1 mp1, MPARAM2 mp2 ) {

  /**************************************************************************
   * Find the instance data.                                                *
   **************************************************************************/

   PDATA Data = PDATA ( Sys_GetWindowData ( Window ) ) ;

  /**************************************************************************
   * Ask the document to move the tab.                                      *
   **************************************************************************/

   Data->pDocument->MoveTab ( int ( LONGFROMMP ( mp1 ) ), int ( LONGFROMMP ( mp2 ) ) ) ;

  /**************************************************************************
   * Done.                                                                  *
   **************************************************************************/

   return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process Set Defaults command.                                       *
 *                                                                          *
 ****************************************************************************/

STATIC MRESULT APIENTRY SetDefaults ( HWND Window, MESG, MPARAM1 mp1, MPARAM2 ) {

  /**************************************************************************
   * Find the instance data.                                                *
   **************************************************************************/

   PDATA Data = PDATA ( Sys_GetWindowData ( Window ) ) ;

  /**************************************************************************
   * Save the new document defaults.                                        *
   **************************************************************************/

   DocumentParms DocParms = * ( (DocumentParms*) ( PVOIDFROMMP ( mp1 ) ) ) ;

  /**************************************************************************
   * Update the window queue codepage.                                      *
   **************************************************************************/

   HMQ Queue = HMQ ( WinQueryWindowULong ( Window, QWL_HMQ ) ) ;
   SetQueueCodepage ( Window, Queue, USHORT(DocParms.CodePage) ) ;

  /**************************************************************************
   * Change the document defaults.                                          *
   **************************************************************************/

   Data->pDocument->SetDefaults ( DocParms ) ;
   Data->pDocument->FormatPage ( ) ;
   Rezoom ( Window, Data ) ;

  /**************************************************************************
   * Update the status window's codepage.                                   *
   **************************************************************************/

   HWND Frame = PARENT ( Window ) ;
   HWND StatusWindow = CHILD ( Frame, FID_STATUSLINE ) ;
   Sys_PostMessage ( StatusWindow, WM_SET_CODEPAGE, MPFROMLONG(DocParms.CodePage), 0 ) ;

  /**************************************************************************
   * Done.                                                                  *
   **************************************************************************/

   return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process Bitmap commands.                                            *
 *                                                                          *
 ****************************************************************************/

STATIC MRESULT APIENTRY Bitmap_Command ( HWND Window, MESG msg, MPARAM1, MPARAM2 ) {

  /**************************************************************************
   * Find the instance data.                                                *
   **************************************************************************/

   PDATA Data = PDATA ( Sys_GetWindowData ( Window ) ) ;

  /**************************************************************************
   * Act according to which command it is.                                  *
   **************************************************************************/

   switch ( msg ) {

      case IDM_BITMAP_BACKGROUND: {
         Data->MoveGraphic.Background = Data->MoveGraphic.Background ? FALSE : TRUE ;
         break; }

      case IDM_BITMAP_NOTEXTASIDE: {
         Data->MoveGraphic.NoTextAside = Data->MoveGraphic.NoTextAside ? FALSE : TRUE ;
         break; }

      case IDM_BITMAP_HALFTONE: {
         Data->MoveGraphic.Halftone = Data->MoveGraphic.Halftone ? FALSE : TRUE ;
         break; }

      case IDM_BITMAP_REVERSE: {
         Data->MoveGraphic.Reverse = Data->MoveGraphic.Reverse ? FALSE : TRUE ;
         break; }

      case IDM_BITMAP_OPTIMIZE: {
         Data->MoveGraphic.PrinterOptimize = Data->MoveGraphic.PrinterOptimize ? FALSE : TRUE ;
         break; }

      case IDM_BITMAP_POSITION_NEXT_CHAR: {
         Data->MoveGraphic.Position.x = GRAPHIC_NEXT_CHAR ;
         break; }

      case IDM_BITMAP_POSITION_EDGE_LEFT: {
         Data->MoveGraphic.Position.x = GRAPHIC_EDGE_LEFT ;
         break; }

      case IDM_BITMAP_POSITION_EDGE_RIGHT: {
         Data->MoveGraphic.Position.x = GRAPHIC_EDGE_RIGHT ;
         break; }

      case IDM_BITMAP_POSITION_EDGE_TOP: {
         Data->MoveGraphic.Position.y = GRAPHIC_EDGE_TOP ;
         break; }

      case IDM_BITMAP_POSITION_EDGE_BOTTOM: {
         Data->MoveGraphic.Position.y = GRAPHIC_EDGE_BOTTOM ;
         break; }

      case IDM_BITMAP_POSITION_MARGIN_LEFT: {
         Data->MoveGraphic.Position.x = GRAPHIC_MARGIN_LEFT ;
         break; }

      case IDM_BITMAP_POSITION_MARGIN_RIGHT: {
         Data->MoveGraphic.Position.x = GRAPHIC_MARGIN_RIGHT ;
         break; }

      case IDM_BITMAP_POSITION_MARGIN_TOP: {
         Data->MoveGraphic.Position.y = GRAPHIC_MARGIN_TOP ;
         break; }

      case IDM_BITMAP_POSITION_MARGIN_BOTTOM: {
         Data->MoveGraphic.Position.y = GRAPHIC_MARGIN_BOTTOM ;
         break; }

      case IDM_BITMAP_POSITION_CENTERED_X: {
         Data->MoveGraphic.Position.x = GRAPHIC_CENTERED ;
         break; }

      case IDM_BITMAP_POSITION_CENTERED_Y: {
         Data->MoveGraphic.Position.y = GRAPHIC_CENTERED ;
         break; }

      case IDM_BITMAP_SIZE_EDGETOEDGE_X: {
         Data->MoveGraphic.Size.cx = GRAPHIC_EDGE_TO_EDGE ;
         break; }

      case IDM_BITMAP_SIZE_EDGETOEDGE_Y: {
         Data->MoveGraphic.Size.cy = GRAPHIC_EDGE_TO_EDGE ;
         break; }

      case IDM_BITMAP_SIZE_MARGINTOMARGIN_X: {
         Data->MoveGraphic.Size.cx = GRAPHIC_MARGIN_TO_MARGIN ;
         break; }

      case IDM_BITMAP_SIZE_MARGINTOMARGIN_Y: {
         Data->MoveGraphic.Size.cy = GRAPHIC_MARGIN_TO_MARGIN ;
         break; }

      case IDM_BITMAP_ASPECT_FLOAT: {
         Data->MoveGraphic.Aspect = GRAPHIC_ASPECT_UNFIXED ;
         break; }

      case IDM_BITMAP_ASPECT_FIX_X: {
         Data->MoveGraphic.Aspect = GRAPHIC_ASPECT_FIX_X ;
         break; }

      case IDM_BITMAP_ASPECT_FIX_Y: {
         Data->MoveGraphic.Aspect = GRAPHIC_ASPECT_FIX_Y ;
         break; }

   } /* endswitch */

  /**************************************************************************
   * Ask the document to update the graphic.                                *
   **************************************************************************/

   Data->pDocument->UpdateGraphic ( Data->MoveGraphic ) ;

  /**************************************************************************
   * Done.                                                                  *
   **************************************************************************/

   return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process Border commands.                                            *
 *                                                                          *
 ****************************************************************************/

STATIC MRESULT APIENTRY Border_Command ( HWND Window, MESG msg, MPARAM1, MPARAM2 ) {

  /**************************************************************************
   * Find the instance data.                                                *
   **************************************************************************/

   PDATA Data = PDATA ( Sys_GetWindowData ( Window ) ) ;

  /**************************************************************************
   * Act according to which command it is.                                  *
   **************************************************************************/

   switch ( msg ) {

      case IDM_BORDER_SIZE: {
         break ; }

      case IDM_BORDER_COLOR: {
         break ; }

      case IDM_BORDER_CAPTION: {
         break ; }

   } /* endswitch */

  /**************************************************************************
   * Ask the document to update the graphic.                                *
   **************************************************************************/

   Data->pDocument->UpdateGraphic ( Data->MoveGraphic ) ;

  /**************************************************************************
   * Done.                                                                  *
   **************************************************************************/

   return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process Default Refresh command.                                    *
 *                                                                          *
 ****************************************************************************/

STATIC MRESULT APIENTRY Default_Refresh ( HWND Window, MESG, MPARAM1, MPARAM2 ) {

  /**************************************************************************
   * Find the instance data.                                                *
   **************************************************************************/

// PDATA Data = PDATA ( Sys_GetWindowData ( Window ) ) ;

  /**************************************************************************
   * Invalidate the entire window.                                          *
   **************************************************************************/

   Sys_InvalidateWindow ( Window ) ;

  /**************************************************************************
   * Done.                                                                  *
   **************************************************************************/

   return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process Dump command.                                               *
 *                                                                          *
 ****************************************************************************/

STATIC MRESULT APIENTRY Dump ( HWND Window, MESG, MPARAM1, MPARAM2 ) {

  /**************************************************************************
   * Find the instance data.                                                *
   **************************************************************************/

   PDATA Data = PDATA ( Sys_GetWindowData ( Window ) ) ;

  /**************************************************************************
   * Process the document dump command.                                     *
   **************************************************************************/

   Data->pDocument->Dump ( PSZ("Dump at user request:") ) ;

  /**************************************************************************
   * Done.                                                                  *
   **************************************************************************/

   return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process keystrokes.                                                 *
 *                                                                          *
 ****************************************************************************/

STATIC MRESULT APIENTRY Char ( HWND Window, MESG msg, MPARAM1 mp1, MPARAM2 mp2 ) {

  /**************************************************************************
   * Find the instance data.                                                *
   **************************************************************************/

   PDATA Data = PDATA ( Sys_GetWindowData ( Window ) ) ;

  /**************************************************************************
   * Get the message data.                                                  *
   **************************************************************************/

   USHORT Flags    = USHORT ( SHORT1FROMMP ( mp1 ) ) ;
   UCHAR  Repeat   = UCHAR  ( CHAR3FROMMP  ( mp1 ) ) ;
   #ifdef DEBUG_KEYS
   UCHAR  ScanCode = UCHAR  ( CHAR4FROMMP  ( mp1 ) ) ;
   #endif

   USHORT Char     = USHORT ( SHORT1FROMMP ( mp2 ) ) ;
   USHORT VKey     = USHORT ( SHORT2FROMMP ( mp2 ) ) ;

   #ifdef DEBUG_KEYS
   Log ( "DocWin::Char: Flags %04X (%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s), Repeat %i, ScanCode %02X, Char %04X, VKey %04X.",
      Flags, 
      Flags & KC_CHAR           ? "Char"        : "",
      Flags & KC_VIRTUALKEY     ? "Vkey"        : "",
      Flags & KC_SCANCODE       ? "Scancode"    : "",
      Flags & KC_SHIFT          ? "Shift"       : "",
      Flags & KC_CTRL           ? "Ctrl"        : "",
      Flags & KC_ALT            ? "Alt"         : "",
      Flags & KC_KEYUP          ? "Keyup"       : "",
      Flags & KC_PREVDOWN       ? "Prevdown"    : "",
      Flags & KC_LONEKEY        ? "Lonekey"     : "",
      Flags & KC_DEADKEY        ? "Deadkey"     : "",
      Flags & KC_COMPOSITE      ? "Composite"   : "",
      Flags & KC_INVALIDCOMP    ? "Invalidcomp" : "",
      Flags & KC_TOGGLE         ? "Toggle"      : "",
      Flags & KC_INVALIDCHAR    ? "Invalidchar" : "",
      Flags & KC_DBCSRSRVD1     ? "Dbcsrsrvd1"  : "",
      Flags & KC_DBCSRSRVD2     ? "Dbcsrsrvd2"  : "",
      Repeat, ScanCode, Char, VKey ) ;
   #endif

  /**************************************************************************
   * Ignore keys being released, invalid characters, dead keys and          *
   *   invalid composite characters.                                        *
   **************************************************************************/

   if ( ( Flags & KC_KEYUP ) OR ( Flags & KC_INVALIDCHAR ) OR ( Flags & KC_DEADKEY ) ) 
      return ( MRFROMSHORT ( FALSE ) ) ;

   if ( Flags & KC_INVALIDCOMP ) {
      Sys_BeepNote ( ) ;
      return ( MRFROMSHORT ( FALSE ) ) ;
   } /* endif */

  /**************************************************************************
   * Process virtual keys that aren't on the keypad.                        *
   **************************************************************************/

   BOOL Processed = FALSE ;

   if ( Flags & KC_VIRTUALKEY ) {
      Processed = TRUE ;

      switch ( VKey ) {

         case VK_ESC:
            if ( Data->BoxMode != BOX_DORMANT ) {
               Data->CaptureFlag = FALSE ;
               Sys_ReleaseCapture ( ) ;

               HWND Frame = PARENT ( Window ) ;
               HWND StatusWindow = CHILD ( Frame, FID_STATUSLINE ) ;
               Sys_SetWindowText ( StatusWindow, PSZ("") ) ;

               POINTL NullPoint = { 0, 0 } ;
               AdjustRulerTicks ( Window, Data, &NullPoint ) ;

               if ( Data->BoxMode > BOX_AWAITINGPOINT0 )
                  Data->WindowPS->DrawTrackingBox ( Data->BoxRectangle ) ;

               Data->BoxMode = BOX_DORMANT ;

            } else if ( Data->Button1Down ) {
               Data->Button1Down = FALSE ;
               Data->CaptureFlag = FALSE ;
               Sys_ReleaseCapture ( ) ;

               HWND Frame = PARENT ( Window ) ;
               HWND StatusWindow = CHILD ( Frame, FID_STATUSLINE ) ;
               Sys_SetWindowText ( StatusWindow, PSZ("") ) ;

               switch ( Data->MoveType ) {
                  case MOVE_GRAPHIC:
                  case SIZE_GRAPHIC: {
                     DrawGraphicTrackingBox ( Data ) ;
                     break; }
               } /* endswitch */

               Data->MoveType = MOVE_NULL ;

            } else if ( Data->Button2Down ) {
               Data->Button2Down = FALSE ;
               Data->CaptureFlag = FALSE ;
               Sys_ReleaseCapture ( ) ;

               HWND Frame = PARENT ( Window ) ;
               HWND StatusWindow = CHILD ( Frame, FID_STATUSLINE ) ;
               Sys_SetWindowText ( StatusWindow, PSZ("") ) ;

               switch ( Data->MoveType ) {
                  case MOVE_GRAPHIC:
                  case SIZE_GRAPHIC: {
                     DrawGraphicTrackingBox ( Data ) ;
                     break; }
                  case MOVE_TEXT:
                     break;
               } /* endswitch */

               Data->MoveType = MOVE_NULL ;

            } /* endif */

            break ;

         case VK_BACKSPACE: {
            for ( int i=0; i<Repeat; i++ ) 
               if ( Flags & KC_CTRL )
                  BackspaceWord ( Window, msg, mp1, mp2 ) ;
               else
                  Backspace ( Window, msg, mp1, mp2 ) ;
            break ; }

         case VK_ENTER:
         case VK_NEWLINE: {
            for ( int i=0; i<Repeat; i++ ) 
               Data->pDocument->Enqueue_InsertKey ( USHORT('\n') ) ;
            break ; }

         case VK_BACKTAB:
            Sys_BeepNote ( ) ;
            break ;

         default:
            Processed = FALSE ;

      } /* endswitch */

   } /* endif */

  /**************************************************************************
   * Process normal characters by inserting them before the cursor.         *
   **************************************************************************/

   if ( NOT Processed AND ( Flags & KC_CHAR ) ) {

      if ( Flags & KC_SCANCODE ) {              // If not entered via Alt+xxx . . .
         for ( int i=0; i<Repeat; i++ ) {
            Data->pDocument->Enqueue_InsertKey ( Char ) ;
         } /* endfor */

      } else if ( Char & 0xFF00 ) {             // Else if DBCS . . .
         for ( int i=0; i<Repeat; i++ ) {
            Data->pDocument->Enqueue_InsertKey ( Char ) ;
         } /* endfor */

      } else {                                  // Else if entered via keypad coding . . .
         for ( int i=0; i<Repeat; i++ ) {
            char Text [80] ;
            sprintf ( Text, "%c%s%i%c", FUNCTION_START, FunctionNames[FN__GLYPH], Char, FUNCTION_END ) ;
            Data->pDocument->Enqueue_InsertText ( Text ) ;
         } /* endfor */

      } /* endif */

      Processed = TRUE ;

   } /* endif */

  /**************************************************************************
   * If not yet processed, handle the virtual keys from the keypad.         *
   **************************************************************************/

   if ( NOT Processed ) {
      if ( ( Flags & KC_VIRTUALKEY ) AND NOT ( Flags & KC_ALT ) ) {
         switch ( VKey ) {

            case VK_INSERT: {
               for ( int i=0; i<Repeat; i++ ) 
                  if ( Flags & KC_SHIFT )
                     Paste ( Window, msg, mp1, mp2 ) ;
                  else if ( Flags & KC_CTRL )
                     Copy ( Window, msg, mp1, mp2 ) ;
                  else 
                     Insert ( Window, msg, mp1, mp2 ) ;
               break ; }
   
            case VK_DELETE: {
               for ( int i=0; i<Repeat; i++ ) 
                  if ( Flags & KC_SHIFT ) 
                     Cut ( Window, msg, mp1, mp2 ) ;
                  else 
                     Clear ( Window, msg, mp1, mp2 ) ;
               break ; }

            case VK_LEFT: {
               for ( int i=0; i<Repeat; i++ ) 
                  if ( Flags & KC_CTRL )
                     WordLeft ( Window, msg, mp1, mp2 ) ;
                  else
                     Left ( Window, msg, mp1, mp2 ) ;
               break ; }

            case VK_RIGHT: {
               for ( int i=0; i<Repeat; i++ ) 
                  if ( Flags & KC_CTRL )
                     WordRight ( Window, msg, mp1, mp2 ) ;
                  else
                     Right ( Window, msg, mp1, mp2 ) ;
               break ; }

            case VK_UP: {
               for ( int i=0; i<Repeat; i++ )
                  LineUp ( Window, msg, mp1, mp2 ) ;
               break ; }

            case VK_DOWN: {
               for ( int i=0; i<Repeat; i++ )
                  LineDown ( Window, msg, mp1, mp2 ) ;
               break ; }

            case VK_PAGEUP: {
               for ( int i=0; i<Repeat; i++ )
                  if ( Flags & KC_CTRL )
                     PageTop ( Window, msg, mp1, mp2 ) ;
                  else
                     PageUp ( Window, msg, mp1, mp2 ) ;
               break ; }
   
            case VK_PAGEDOWN: {
               for ( int i=0; i<Repeat; i++ )
                  if ( Flags & KC_CTRL )
                     PageBottom ( Window, msg, mp1, mp2 ) ;
                  else
                     PageDown ( Window, msg, mp1, mp2 ) ;
               break ; }

            case VK_HOME: {
               for ( int i=0; i<Repeat; i++ )
                  if ( Flags & KC_CTRL )
                     Top ( Window, msg, mp1, mp2 ) ;
                  else
                     Home ( Window, msg, mp1, mp2 ) ;
               break ; }

            case VK_END: {
               for ( int i=0; i<Repeat; i++ )
                  if ( Flags & KC_CTRL )
                     Bottom ( Window, msg, mp1, mp2 ) ;
                  else
                     End ( Window, msg, mp1, mp2 ) ;
               break ; }

         } /* endswitch */

      } /* endif */

   } /* endif */

  /**************************************************************************
   * Return TRUE, indicating that we've processed the character.            *
   **************************************************************************/

   return ( MRFROMSHORT ( TRUE ) ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process Backspace command.                                          *
 *                                                                          *
 ****************************************************************************/

STATIC MRESULT APIENTRY Backspace ( HWND Window, MESG, MPARAM1, MPARAM2 ) {

  /**************************************************************************
   * Find the instance data.                                                *
   **************************************************************************/

   PDATA Data = PDATA ( Sys_GetWindowData ( Window ) ) ;

  /**************************************************************************
   * Queue a backspace character, which will perform the rest of the work.  *
   **************************************************************************/

   Data->pDocument->Enqueue_InsertKey ( USHORT('\b') ) ;

  /**************************************************************************
   * Return TRUE, indicating that we've processed the character.            *
   **************************************************************************/

   return ( MRFROMSHORT ( TRUE ) ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process Backspace Word command.                                     *
 *                                                                          *
 ****************************************************************************/

STATIC MRESULT APIENTRY BackspaceWord ( HWND Window, MESG, MPARAM1, MPARAM2 ) {

  /**************************************************************************
   * Find the instance data.                                                *
   **************************************************************************/

   PDATA Data = PDATA ( Sys_GetWindowData ( Window ) ) ;

  /**************************************************************************
   * Move the previous word, selecting.  If that's OK, clear the newly      *
   *   selected area.                                                       *
   **************************************************************************/

   if ( Data->pDocument->MoveWordUp ( TRUE ) )
      Sys_BeepNote ( ) ;
   else 
      Data->pDocument->Enqueue_Clear ( ) ;

  /**************************************************************************
   * Return TRUE, indicating that we've processed the character.            *
   **************************************************************************/

   return ( MRFROMSHORT ( TRUE ) ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process Insert command.                                             *
 *                                                                          *
 ****************************************************************************/

STATIC MRESULT APIENTRY Insert ( HWND Window, MESG, MPARAM1, MPARAM2 ) {

  /**************************************************************************
   * Find the instance data.                                                *
   **************************************************************************/

   PDATA Data = PDATA ( Sys_GetWindowData ( Window ) ) ;

  /**************************************************************************
   * Toggle the document's insert mode.                                     *
   **************************************************************************/

   Data->pDocument->Enqueue_InsertModeToggle ( ) ;

  /**************************************************************************
   * Done.                                                                  *
   **************************************************************************/

   return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process Move Left command.                                          *
 *                                                                          *
 ****************************************************************************/

STATIC MRESULT APIENTRY Left ( HWND Window, MESG msg, MPARAM1, MPARAM2 ) {

  /**************************************************************************
   * Find the instance data.                                                *
   **************************************************************************/

   PDATA Data = PDATA ( Sys_GetWindowData ( Window ) ) ;

  /**************************************************************************
   * Move the previous token.                                               *
   **************************************************************************/

   if ( Data->pDocument->MoveUp ( BOOL ( CHARMSG(&msg)->fs & KC_SHIFT ) ) )
      Sys_BeepNote ( ) ;

  /**************************************************************************
   * Done.                                                                  *
   **************************************************************************/

   return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process Move Right command.                                         *
 *                                                                          *
 ****************************************************************************/

STATIC MRESULT APIENTRY Right ( HWND Window, MESG msg, MPARAM1, MPARAM2 ) {

  /**************************************************************************
   * Find the instance data.                                                *
   **************************************************************************/

   PDATA Data = PDATA ( Sys_GetWindowData ( Window ) ) ;

  /**************************************************************************
   * Move to the next token.                                                *
   **************************************************************************/

   if ( Data->pDocument->MoveDown ( BOOL ( CHARMSG(&msg)->fs & KC_SHIFT ) ) )
      Sys_BeepNote ( ) ;

  /**************************************************************************
   * Done.                                                                  *
   **************************************************************************/

   return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process Move Left Word command.                                     *
 *                                                                          *
 ****************************************************************************/

STATIC MRESULT APIENTRY WordLeft ( HWND Window, MESG msg, MPARAM1, MPARAM2 ) {

  /**************************************************************************
   * Find the instance data.                                                *
   **************************************************************************/

   PDATA Data = PDATA ( Sys_GetWindowData ( Window ) ) ;

  /**************************************************************************
   * Move the previous token.                                               *
   **************************************************************************/

   if ( Data->pDocument->MoveWordUp ( BOOL ( CHARMSG(&msg)->fs & KC_SHIFT ) ) )
      Sys_BeepNote ( ) ;

  /**************************************************************************
   * Done.                                                                  *
   **************************************************************************/

   return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process Move Right Word command.                                    *
 *                                                                          *
 ****************************************************************************/

STATIC MRESULT APIENTRY WordRight ( HWND Window, MESG msg, MPARAM1, MPARAM2 ) {

  /**************************************************************************
   * Find the instance data.                                                *
   **************************************************************************/

   PDATA Data = PDATA ( Sys_GetWindowData ( Window ) ) ;

  /**************************************************************************
   * Move to the next token.                                                *
   **************************************************************************/

   if ( Data->pDocument->MoveWordDown ( BOOL ( CHARMSG(&msg)->fs & KC_SHIFT ) ) )
      Sys_BeepNote ( ) ;

  /**************************************************************************
   * Done.                                                                  *
   **************************************************************************/

   return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process Move Line Up command.                                       *
 *                                                                          *
 ****************************************************************************/

STATIC MRESULT APIENTRY LineUp ( HWND Window, MESG msg, MPARAM1, MPARAM2 ) {

  /**************************************************************************
   * Find the instance data.                                                *
   **************************************************************************/

   PDATA Data = PDATA ( Sys_GetWindowData ( Window ) ) ;

  /**************************************************************************
   * Move the previous token.                                               *
   **************************************************************************/

   if ( Data->pDocument->MoveLineUp ( BOOL ( CHARMSG(&msg)->fs & KC_SHIFT ) ) )
      Sys_BeepNote ( ) ;

  /**************************************************************************
   * Done.                                                                  *
   **************************************************************************/

   return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process Move Line Down command.                                     *
 *                                                                          *
 ****************************************************************************/

STATIC MRESULT APIENTRY LineDown ( HWND Window, MESG msg, MPARAM1, MPARAM2 ) {

  /**************************************************************************
   * Find the instance data.                                                *
   **************************************************************************/

   PDATA Data = PDATA ( Sys_GetWindowData ( Window ) ) ;

  /**************************************************************************
   * Move to the next token.                                                *
   **************************************************************************/

   if ( Data->pDocument->MoveLineDown ( BOOL ( CHARMSG(&msg)->fs & KC_SHIFT ) ) )
      Sys_BeepNote ( ) ;

  /**************************************************************************
   * Done.                                                                  *
   **************************************************************************/

   return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process Move Up Page command.                                       *
 *                                                                          *
 ****************************************************************************/

STATIC MRESULT APIENTRY PageUp ( HWND Window, MESG msg, MPARAM1, MPARAM2 ) {

  /**************************************************************************
   * Find the instance data.                                                *
   **************************************************************************/

   PDATA Data = PDATA ( Sys_GetWindowData ( Window ) ) ;

  /**************************************************************************
   * Ask the document to move one page up.                                  *
   **************************************************************************/

   if ( Data->pDocument->MovePageUp ( BOOL ( CHARMSG(&msg)->fs & KC_SHIFT ) ) )
      Sys_BeepNote ( ) ;

  /**************************************************************************
   * Done.                                                                  *
   **************************************************************************/

   return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process Move Down Page command.                                     *
 *                                                                          *
 ****************************************************************************/

STATIC MRESULT APIENTRY PageDown ( HWND Window, MESG msg, MPARAM1, MPARAM2 ) {

  /**************************************************************************
   * Find the instance data.                                                *
   **************************************************************************/

   PDATA Data = PDATA ( Sys_GetWindowData ( Window ) ) ;

  /**************************************************************************
   * Ask the document to move one page down.                                *
   **************************************************************************/

   if ( Data->pDocument->MovePageDown ( BOOL ( CHARMSG(&msg)->fs & KC_SHIFT ) ) )
      Sys_BeepNote ( ) ;

  /**************************************************************************
   * Done.                                                                  *
   **************************************************************************/

   return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process Move Page Top command.                                      *
 *                                                                          *
 ****************************************************************************/

STATIC MRESULT APIENTRY PageTop ( HWND Window, MESG msg, MPARAM1, MPARAM2 ) {

  /**************************************************************************
   * Find the instance data.                                                *
   **************************************************************************/

   PDATA Data = PDATA ( Sys_GetWindowData ( Window ) ) ;

  /**************************************************************************
   * Ask the document to move to top of current page.                       *
   **************************************************************************/

   if ( Data->pDocument->MovePageTop ( BOOL ( CHARMSG(&msg)->fs & KC_SHIFT ) ) )
      Sys_BeepNote ( ) ;

  /**************************************************************************
   * Done.                                                                  *
   **************************************************************************/

   return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process Move Page Bottom command.                                   *
 *                                                                          *
 ****************************************************************************/

STATIC MRESULT APIENTRY PageBottom ( HWND Window, MESG msg, MPARAM1, MPARAM2 ) {

  /**************************************************************************
   * Find the instance data.                                                *
   **************************************************************************/

   PDATA Data = PDATA ( Sys_GetWindowData ( Window ) ) ;

  /**************************************************************************
   * Ask the document to move to bottom of current page.                    *
   **************************************************************************/

   if ( Data->pDocument->MovePageBottom ( BOOL ( CHARMSG(&msg)->fs & KC_SHIFT ) ) )
      Sys_BeepNote ( ) ;

  /**************************************************************************
   * Done.                                                                  *
   **************************************************************************/

   return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process Move Home command.                                          *
 *                                                                          *
 ****************************************************************************/

STATIC MRESULT APIENTRY Home ( HWND Window, MESG msg, MPARAM1, MPARAM2 ) {

  /**************************************************************************
   * Find the instance data.                                                *
   **************************************************************************/

   PDATA Data = PDATA ( Sys_GetWindowData ( Window ) ) ;

  /**************************************************************************
   * Move to the top of the document.                                       *
   **************************************************************************/

   if ( Data->pDocument->MoveLineTop ( BOOL ( CHARMSG(&msg)->fs & KC_SHIFT ) ) )
      Sys_BeepNote ( ) ;

  /**************************************************************************
   * Done.                                                                  *
   **************************************************************************/

   return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process Move End command.                                           *
 *                                                                          *
 ****************************************************************************/

STATIC MRESULT APIENTRY End ( HWND Window, MESG msg, MPARAM1, MPARAM2 ) {

  /**************************************************************************
   * Find the instance data.                                                *
   **************************************************************************/

   PDATA Data = PDATA ( Sys_GetWindowData ( Window ) ) ;

  /**************************************************************************
   * Move to the bottom of the document.                                    *
   **************************************************************************/

   if ( Data->pDocument->MoveLineBottom ( BOOL ( CHARMSG(&msg)->fs & KC_SHIFT ) ) )
      Sys_BeepNote ( ) ;

  /**************************************************************************
   * Done.                                                                  *
   **************************************************************************/

   return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process Move Top command.                                           *
 *                                                                          *
 ****************************************************************************/

STATIC MRESULT APIENTRY Top ( HWND Window, MESG msg, MPARAM1, MPARAM2 ) {

  /**************************************************************************
   * Find the instance data.                                                *
   **************************************************************************/

   PDATA Data = PDATA ( Sys_GetWindowData ( Window ) ) ;

  /**************************************************************************
   * Move to the top of the document.                                       *
   **************************************************************************/

   if ( Data->pDocument->MoveTop ( BOOL ( CHARMSG(&msg)->fs & KC_SHIFT ) ) )
      Sys_BeepNote ( ) ;

  /**************************************************************************
   * Done.                                                                  *
   **************************************************************************/

   return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process Move Bottom command.                                        *
 *                                                                          *
 ****************************************************************************/

STATIC MRESULT APIENTRY Bottom ( HWND Window, MESG msg, MPARAM1, MPARAM2 ) {

  /**************************************************************************
   * Find the instance data.                                                *
   **************************************************************************/

   PDATA Data = PDATA ( Sys_GetWindowData ( Window ) ) ;

  /**************************************************************************
   * Move to the bottom of the document.                                    *
   **************************************************************************/

   if ( Data->pDocument->MoveBottom ( BOOL ( CHARMSG(&msg)->fs & KC_SHIFT ) ) )
      Sys_BeepNote ( ) ;

  /**************************************************************************
   * Done.                                                                  *
   **************************************************************************/

   return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process Select All command.                                         *
 *                                                                          *
 ****************************************************************************/

STATIC MRESULT APIENTRY SelectAll ( HWND Window, MESG, MPARAM1, MPARAM2 ) {

  /**************************************************************************
   * Find the instance data.                                                *
   **************************************************************************/

   PDATA Data = PDATA ( Sys_GetWindowData ( Window ) ) ;

  /**************************************************************************
   * Select the entire document.                                            *
   **************************************************************************/

   Data->pDocument->SelectAll ( ) ;

  /**************************************************************************
   * Done.                                                                  *
   **************************************************************************/

   return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process "Print Done" notification                                   *
 *                                                                          *
 ****************************************************************************/

STATIC MRESULT APIENTRY PrintDone ( HWND Window, MESG, MPARAM1, MPARAM2 ) {

  /**************************************************************************
   * Find the instance data.                                                *
   **************************************************************************/

   PDATA Data = PDATA ( Sys_GetWindowData ( Window ) ) ;

  /**************************************************************************
   * Decrement the number of prints active.                                 *
   **************************************************************************/

   if ( Data->PrintsActive ) {
      Data->PrintsActive -- ;
      if ( !Data->PrintsActive ) {
         Sys_ShowWindow ( Data->PrintStatusWindow, FALSE ) ;
      } /* endif */
   } /* endif */

  /**************************************************************************
   * If no more prints active, and the program's closing, tell it to quit.  *
   **************************************************************************/

   if ( !Data->PrintsActive AND !Data->SavesActive AND Data->Closing ) {
      HWND Frame = PARENT ( Window ) ;
      Sys_PostMessage ( Data->MainWindow, WM_CLOSE_ME, MPFROMP(Frame), MPFROMLONG(FALSE) ) ;
   } /* endif */

  /**************************************************************************
   * Done.                                                                  *
   **************************************************************************/

   return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process "Save Done" notification                                    *
 *                                                                          *
 ****************************************************************************/

STATIC MRESULT APIENTRY SaveDone ( HWND Window, MESG, MPARAM1 mp1, MPARAM2 mp2 ) {

  /**************************************************************************
   * Find the instance data.                                                *
   **************************************************************************/

   PDATA Data = PDATA ( Sys_GetWindowData ( Window ) ) ;

  /**************************************************************************
   * Decrement the number of save active.                                   *
   **************************************************************************/

   if ( Data->SavesActive ) {
      Data->SavesActive -- ;
      if ( !Data->SavesActive ) {
         Sys_ShowWindow ( Data->StatusWindow, FALSE ) ;
      } /* endif */
   } /* endif */

  /**************************************************************************
   * If the save failed, set the document's change flag.                    *
   **************************************************************************/

   if ( LONGFROMMP(mp1) == FALSE ) {
      Data->pDocument->Set_SavedLevel ( int(LONGFROMMP(mp2)) ) ;
      Data->pDocument->UpdateTitle ( ) ;
   } /* endif */

  /**************************************************************************
   * If no more prints active, and the program's closing, tell it to quit.  *
   **************************************************************************/

   if ( !Data->PrintsActive AND !Data->SavesActive AND Data->Closing ) {
      HWND Frame = PARENT ( Window ) ;
      Sys_PostMessage ( Data->MainWindow, WM_CLOSE_ME, MPFROMP(Frame), MPFROMLONG(FALSE) ) ;
   } /* endif */

  /**************************************************************************
   * Done.                                                                  *
   **************************************************************************/

   return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process Query for Document State.                                   *
 *                                                                          *
 ****************************************************************************/

STATIC MRESULT APIENTRY QueryState ( HWND Window, MESG, MPARAM1 mp1, MPARAM2 ) {

  /**************************************************************************
   * Find the instance data.                                                *
   **************************************************************************/

   PDATA Data = PDATA ( Sys_GetWindowData ( Window ) ) ;

  /**************************************************************************
   * Wait for queued editing to complete.                                   *
   **************************************************************************/

   Data->pDocument->FlushCommandQueue ( ) ;

  /**************************************************************************
   * Return state to area pointed to by 'mp1'.                              *
   **************************************************************************/

   *((Estado*)PVOIDFROMMP(mp1)) = *Data->pDocument->Query_State() ;

  /**************************************************************************
   * Return.                                                                *
   **************************************************************************/

   return ( MRFROMSHORT ( TRUE ) ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process Query for Document Window Zoom Type & Factor                *
 *                                                                          *
 ****************************************************************************/

STATIC MRESULT APIENTRY QueryZoom ( HWND Window, MESG, MPARAM1 mp1, MPARAM2 ) {

  /**************************************************************************
   * Find the instance data.                                                *
   **************************************************************************/

   PDATA Data = PDATA ( Sys_GetWindowData ( Window ) ) ;

  /**************************************************************************
   * Return state to area pointed to by 'mp1'.                              *
   **************************************************************************/

   FIXED Zoom = Data->pDocument->Query_Zoom() ;
   memcpy ( PFIXED(PVOIDFROMMP(mp1)), &Zoom, sizeof(FIXED) ) ;

  /**************************************************************************
   * Return zoom type as the function value.                                *
   **************************************************************************/

   return ( MRFROMSHORT ( Data->ZoomCommand ) ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process Query Defaults command.                                     *
 *                                                                          *
 ****************************************************************************/

STATIC MRESULT APIENTRY QueryDefaults ( HWND Window, MESG, MPARAM1, MPARAM2 ) {

  /**************************************************************************
   * Find the instance data.                                                *
   **************************************************************************/

   PDATA Data = PDATA ( Sys_GetWindowData ( Window ) ) ;

  /**************************************************************************
   * Return a pointer to the document defaults structure.                   *
   **************************************************************************/

   return ( MRFROMP ( Data->pDocument->Query_Parms() ) ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process Alter Window command.                                       *
 *                                                                          *
 ****************************************************************************/

STATIC MRESULT APIENTRY AlterWindow ( HWND Window, MESG, MPARAM1, MPARAM2 ) {

  /**************************************************************************
   * Find the instance data.                                                *
   **************************************************************************/

   PDATA Data = PDATA ( Sys_GetWindowData ( Window ) ) ;

  /**************************************************************************
   * Update the window.                                                     *
   **************************************************************************/

   Data->pDocument->Set_FullNames ( Data->IniData->FullNames ) ;
   Data->pDocument->UpdateTitle ( ) ;

  /**************************************************************************
   * Done.                                                                  *
   **************************************************************************/

   return ( 0 ) ;
}


 
/****************************************************************************
 *                                                                          *
 *      Adjust Ruler Tick-Marks                                             *
 *                                                                          *
 ****************************************************************************/

STATIC void AdjustRulerTicks ( HWND Window, PDATA, PPOINTL Point ) {

  /**************************************************************************
   * Get the frame handle.                                                  *
   **************************************************************************/

   HWND Frame = PARENT ( Window ) ;

  /**************************************************************************
   * Adjust the horizontal ruler.                                           *
   **************************************************************************/

   HWND HorzRuler = CHILD ( Frame, FID_HORZRULER ) ;
   Sys_PostMessage ( HorzRuler, WM_SET_TICK, MPFROMLONG(Point->x), 0 ) ;

  /**************************************************************************
   * Adjust the vertical ruler.                                             *
   **************************************************************************/

   HWND VertRuler = CHILD ( Frame, FID_VERTRULER ) ;
   Sys_PostMessage ( VertRuler, WM_SET_TICK, MPFROMLONG(Point->y), 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *                          Recompute Zoom Factor                           *
 *                                                                          *
 ****************************************************************************/

STATIC void Rezoom ( HWND Window, PDATA Data ) {

  /**************************************************************************
   * Rezoom the window.                                                     *
   **************************************************************************/

   switch ( Data->ZoomCommand ) {

      default:
      case IDM_ZOOM_100: {
         SetZoom ( Window, Data, 1.0 ) ;
         break ; }

      case IDM_ZOOM_SET: {
         double Zoom = double ( FIXEDINT ( Data->pDocument->Query_Zoom() ) ) ;
         Zoom += double ( FIXEDFRAC ( Data->pDocument->Query_Zoom() ) ) / 0x10000 ;
         SetZoom ( Window, Data, Zoom ) ;
         break ; }

      case IDM_ZOOM_FULLWIDTH:
         ZoomFullWidth ( Window, IDM_ZOOM_FULLWIDTH, 0, 0 ) ;
         break ;

      case IDM_ZOOM_FULLHEIGHT:
         ZoomFullHeight ( Window, IDM_ZOOM_FULLHEIGHT, 0, 0 ) ;
         break ;

      case IDM_ZOOM_MARGINS:
         ZoomMargins ( Window, IDM_ZOOM_MARGINS, 0, 0 ) ;
         break ;

      case IDM_ZOOM_ALL:
         ZoomAll ( Window, IDM_ZOOM_ALL, 0, 0 ) ;
         break ;

      case IDM_ZOOM_WINDOW:
         RECTL Rectangle ;
         WinQueryWindowRect ( Window, &Rectangle ) ;
         Data->pDocument->Transform ( CVTC_DEVICE, CVTC_PAGE, Rectangle ) ;

         SetZoom ( Window, Data,
            double ( min (
               double(Rectangle.xRight-Rectangle.xLeft) / double(Data->ZoomRectangle.xRight-Data->ZoomRectangle.xLeft),
               double(Rectangle.yTop-Rectangle.yBottom) / double(Data->ZoomRectangle.yTop-Data->ZoomRectangle.yBottom)
            ) ) ) ;

         Sys_PostMessage ( Window, WM_HSCROLL,
            MPFROM2SHORT ( FID_HORZSCROLL, 0 ),
            MPFROM2SHORT ( SHORT(Data->ZoomRectangle.xLeft), SB_SLIDERPOSITION ) ) ;

         Sys_PostMessage ( Window, WM_VSCROLL,
           MPFROM2SHORT ( FID_VERTSCROLL, 0 ),
           MPFROM2SHORT ( SHORT(Data->pDocument->Query_State0()->FormInfo.cy-Data->ZoomRectangle.yTop), SB_SLIDERPOSITION ) ) ;

         break ;

   } /* endswitch */
}
 
/****************************************************************************
 *                                                                          *
 *                            Set Zoom Factor                               *
 *                                                                          *
 ****************************************************************************/

STATIC void SetZoom ( HWND, PDATA Data, double Zoom ) {

  /**************************************************************************
   * Tell the document what the new zoom factor is.                         *
   **************************************************************************/

   if ( Zoom ) {
      Zoom = max ( 0.10, min ( 30.0, Zoom ) ) ;
      USHORT Upper = USHORT ( Zoom ) ;
      USHORT Lower = USHORT ( ( Zoom - double(Upper) ) * 0x10000 ) ;
      Data->pDocument->Set_Zoom ( MAKEFIXED ( Upper, Lower ) ) ;

   } else {
      Data->pDocument->Rezoom ( ) ;

   } /* endif */
}
 
/****************************************************************************
 *                                                                          *
 *                       Draw Graphic Tracking Box                          *
 *                                                                          *
 ****************************************************************************/

STATIC void DrawGraphicTrackingBox ( PDATA Data ) {

   RECTL InnerBox = Data->MoveRectangle ;
   Data->WindowPS->DrawTrackingBox ( InnerBox ) ;

   Data->pDocument->Transform ( CVTC_DEVICE, CVTC_DEFAULTPAGE, InnerBox ) ;

   RECTL OuterBox = {
      InnerBox.xLeft   - Data->MoveGraphic.BorderWidth,
      InnerBox.yBottom - Data->MoveGraphic.BorderWidth,
      InnerBox.xRight  + Data->MoveGraphic.BorderWidth,
      InnerBox.yTop    + Data->MoveGraphic.BorderWidth
   } ;

   Data->pDocument->Transform ( CVTC_DEFAULTPAGE, CVTC_DEVICE, OuterBox ) ;

   Data->WindowPS->DrawTrackingBox ( OuterBox ) ;

}

 
/****************************************************************************
 *                                                                          *
 *  Initiate a Document Save                                                *
 *                                                                          *
 ****************************************************************************/

STATIC BOOL Save ( HWND Window, PDATA Data, BOOL Rename ) {

  /**************************************************************************
   * Wait for queued editing to complete.                                   *
   **************************************************************************/

   Data->pDocument->FlushCommandQueue ( ) ;

  /**************************************************************************
   * If file name is null, or renaming, get the new file name.              *
   **************************************************************************/

   PSZ Filename = Data->pDocument->Query_Filename() ;

   FILEDLG DialogData ;
   memset ( &DialogData, 0, sizeof(DialogData) ) ;

   PrepareFileTypeList ( TRUE ) ;
   FILEDIALOG_DATA FileDialogData = { FALSE, FileTypeList, FileWildcardList, FileExtenderList, 0 } ;
   for ( int i=1; FileTypeList[i]; i++ ) {
      if ( !stricmp ( FileTypeList[i], Data->pDocument->Query_FileType() ) ) {
         FileDialogData.Index = i ;
         break ;
      } /* endif */
   } /* endfor */

   if ( ( Filename == NULL ) OR Rename ) {
      ResourceString SaveDocument ( Data->Library->QueryHandle(), IDS_SAVE_DOCUMENT ) ;
      DialogData.cbSize = sizeof(DialogData) ;
      DialogData.fl = FDS_SAVEAS_DIALOG | FDS_CENTER | FDS_HELPBUTTON | FDS_FILTERUNION ;
      DialogData.pszTitle = PSZ(SaveDocument) ;
      if ( Filename ) {
         strcpy ( DialogData.szFullFile, Filename ) ;
      } else {
         strcpy ( DialogData.szFullFile, FileWildcardList[FileDialogData.Index>0?FileDialogData.Index:1] ) ;
      } /* endif */
      DialogData.pszIType = FileTypeList[FileDialogData.Index>0?FileDialogData.Index:1] ;
      DialogData.papszITypeList = PAPSZ(FileTypeList) ;
      DialogData.pfnDlgProc = FileDialog_Processor ;
      DialogData.ulUser = ULONG ( &FileDialogData ) ;
      #ifdef DEBUG_CLOSE
         Log ( "DocWin::Save: Invoking File Dialog to get new file name." ) ;
      #endif
      WinFileDlg ( HWND_DESKTOP, Window, &DialogData ) ;
      #ifdef DEBUG_CLOSE
         Log ( "DocWin::Save: Done invoking File Dialog." ) ;
      #endif
      if ( DialogData.lReturn != DID_OK )
         return ( FALSE ) ;
      Filename = PSZ(DialogData.szFullFile) ;
      Data->pDocument->Query_Parms()->CreateDate = time ( 0 ) ;
   } /* endif */

  /**************************************************************************
   * Handle case where file name changes.                                   *
   **************************************************************************/

   if ( ( Data->pDocument->Query_Filename()==NULL ) OR strcmp ( PCHAR(Filename), PCHAR(Data->pDocument->Query_Filename()) ) ) {
      if ( access ( PCHAR(Filename), 0 ) == 0 ) {
         ResourceString Title ( Data->Library->QueryHandle(), IDS_TITLE1 ) ;
         ResourceString Format ( Data->Library->QueryHandle(), IDS_PROMPT_SAVECONFIRM1 ) ;
         char Message [500] ;
         sprintf ( Message, PCHAR(Format), Filename ) ;
         #ifdef DEBUG_CLOSE
            Log ( "DocWin::Save: Asking if user wants to save over file of same name." ) ;
         #endif
         APIRET Result = MessageBox ( HWND_DESKTOP, PARENT(Data->MainWindow), PSZ(Message), PSZ(Title),
            IDD_PROMPT_SAVECONFIRM1, MB_YESNOCANCEL | MB_ICONEXCLAMATION | MB_DEFBUTTON2 | MB_HELP, LibraryHandle, IDS_MESSAGE_BOX_STRINGS ) ;
         #ifdef DEBUG_CLOSE
         Log ( "DocWin::Save: Done asking." ) ;
         #endif
         switch ( Result ) {
            case MBID_YES:
               break;
            case MBID_NO:
               return ( FALSE ) ;
            case MBID_CANCEL:
               return ( FALSE ) ;
         } /* endswitch */
      } /* endif */
      Data->pDocument->Set_Name ( Filename ) ;
      Data->pDocument->Query_Parms()->CreateDate = time ( 0 ) ;

   } else if ( strcmp ( Data->pDocument->Query_FileType(), FileTypeList[FileDialogData.Index>0?FileDialogData.Index:1] ) ) {
      ResourceString Title ( Data->Library->QueryHandle(), IDS_TITLE1 ) ;
      ResourceString Format ( Data->Library->QueryHandle(), IDS_PROMPT_SAVECONFIRM2 ) ;
      char Message [500] ;
      sprintf ( Message, PCHAR(Format), Data->pDocument->Query_FileType(), FileTypeList[FileDialogData.Index>0?FileDialogData.Index:1] ) ;
      #ifdef DEBUG_CLOSE
         Log ( "DocWin::Save: Asking if user wants to save file with new type." ) ;
      #endif
      APIRET Result = MessageBox ( HWND_DESKTOP, PARENT(Data->MainWindow), PSZ(Message), PSZ(Title),
         IDD_PROMPT_SAVECONFIRM2, MB_YESNOCANCEL | MB_ICONEXCLAMATION | MB_DEFBUTTON2 | MB_HELP, LibraryHandle, IDS_MESSAGE_BOX_STRINGS ) ;
      #ifdef DEBUG_CLOSE
         Log ( "DocWin::Save: Done asking." ) ;
      #endif
      switch ( Result ) {
         case MBID_YES:
            break;
         case MBID_NO:
            return ( FALSE ) ;
         case MBID_CANCEL:
            return ( FALSE ) ;
      } /* endswitch */

   } /* endif */

  /***************************************************************************
   * Move to the selected directory if drive local and fixed.                *
   ***************************************************************************/

   if ( DialogData.szFullFile[0] ) {
      char Drive [_MAX_DRIVE + 1 ] ;
      char Dir   [_MAX_DIR   + 1 ] ;
      char Name  [_MAX_FNAME + 1 ] ;
      char Ext   [_MAX_EXT   + 1 ] ;
      _splitpath ( PCHAR(DialogData.szFullFile), Drive, Dir, Name, Ext ) ;
      int Type ;
      strupr ( Drive ) ;
      if ( ( CheckDrive ( Drive[0]-'A'+1, Type ) == 0 ) AND ( Type > 0 ) ) {
         _chdrive ( Drive[0]-'A'+1 ) ;
         Dir[strlen(Dir)-1] = 0 ;
         _chdir ( "\\" ) ;
         _chdir ( Dir ) ;
      } /* endif */
   } /* endif */

  /**************************************************************************
   * Increment the number of active saves.                                  *
   **************************************************************************/

   Data->SavesActive ++ ;
   ResourceString Saving ( Data->Library->QueryHandle(), IDS_SAVING ) ;
   Sys_SetWindowText ( Data->StatusWindow, PSZ(Saving) ) ;
   Sys_ShowWindow ( Data->StatusWindow, TRUE ) ;
   Sys_PostMessage ( Data->MainWindow, WM_THREAD_STARTED, 0, 0 ) ;

  /**************************************************************************
   * Prepare the data for the save thread.                                  *
   **************************************************************************/

   PSAVE_PARMS Parms = PSAVE_PARMS ( malloc ( sizeof(SAVE_PARMS) ) ) ;
   Parms->Library = Data->Library ;
   Parms->pDocument = new Documento ( WinQueryAnchorBlock(Window), *Data->pDocument, TRUE ) ;
   strcpy ( PCHAR(Parms->Filename), PCHAR(Filename) ) ;
   Parms->WindowToNotify = Window ;
   Parms->MainWindow = Data->MainWindow ;
   Parms->Semaphore = Data->SaveLock ;
   Parms->PreviousSavedLevel = Data->pDocument->Query_SavedLevel() ;
   Parms->Exporter = (ImportExport*) FileExtenderList [ FileDialogData.Index > 0 ? FileDialogData.Index : 1 ] ;

  /**************************************************************************
   * Set the currently saved level, though the save has only just started.  *
   **************************************************************************/

   Data->pDocument->Set_SavedLevel ( Parms->pDocument->Query_CurrentLevel() ) ;

  /**************************************************************************
   * Set the saved type, though the save has only just started.             *
   **************************************************************************/

   Data->pDocument->Set_Type ( FileTypeList [ FileDialogData.Index > 0 ? FileDialogData.Index : 1 ] ) ;

  /**************************************************************************
   * Update the window title.                                               *
   **************************************************************************/

   Data->pDocument->UpdateTitle ( ) ;

  /**************************************************************************
   * Perform the save.                                                      *
   **************************************************************************/

   StartThread ( "SaveThread", SaveThread, 0x10000, Parms ) ;

  /**************************************************************************
   * Report save started successfully.                                      *
   **************************************************************************/

   return ( TRUE ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Saving Thread                                                       *
 *                                                                          *
 ****************************************************************************/

STATIC void SaveThread ( PVOID Parameter ) {

  /**************************************************************************
   * Get parameters.                                                        *
   **************************************************************************/

   PSAVE_PARMS Parms = PSAVE_PARMS ( Parameter ) ;

  /**************************************************************************
   * Connect to PM.                                                         *
   **************************************************************************/

   Process Proc ( "SaveThread", 0, LibraryHandle ) ;

  /**************************************************************************
   * Do the actual save.                                                    *
   **************************************************************************/

   Parms->Semaphore->Request ( ) ;
   BOOL Result = Parms->pDocument->Save ( Parms->WindowToNotify, (PSZ)Parms->Filename, Parms->Exporter ) ;
   Parms->Semaphore->Release ( ) ;

  /**************************************************************************
   * Notify the window that wanted to know when the save was finished.      *
   **************************************************************************/

   if ( !Sys_PostMessage ( Parms->WindowToNotify, WM_SAVE_DONE, MPFROMLONG(Result), MPFROMLONG(Parms->PreviousSavedLevel) ) ) {
      ERRORID Code = Sys_GetLastError ( Proc.QueryAnchor() ) ;
      if ( ( Code & 0xFFFF ) != PMERR_INVALID_HWND ) 
         Log ( "WARNING: Attempt to notify document window of save thread completion failed.  Error %lX.", Code ) ;
   } /* endif */

  /**************************************************************************
   * Notify the window that wanted to know when the thread was finished.    *
   **************************************************************************/

   if ( !Sys_PostMessage ( Parms->MainWindow, WM_THREAD_DONE, MPFROMLONG(Sys_ThreadNumber(*_threadid)), 0 ) ) {
      ERRORID Code = Sys_GetLastError ( Proc.QueryAnchor() ) ;
      if ( ( Code & 0xFFFF ) != PMERR_INVALID_HWND ) 
         Log ( "WARNING: Attempt to notify main window of save thread completion failed.  Error %lX.", Code ) ;
   } /* endif */

  /**************************************************************************
   * Destroy the document copy.                                             *
   **************************************************************************/

   delete Parms->pDocument ;

  /**************************************************************************
   * Release the parameter memory.                                          *
   **************************************************************************/

   free ( Parms ) ;
}


 
/****************************************************************************
 *                                                                          *
 *      Print Document                                                      *
 *                                                                          *
 ****************************************************************************/

STATIC void Print ( HWND Window, int PageNumber, BOOL Reverse ) {

  /**************************************************************************
   * Find the instance data.                                                *
   **************************************************************************/

   PDATA Data = PDATA ( Sys_GetWindowData ( Window ) ) ;

  /**************************************************************************
   * Reject this request if there are no printer queues defined.            *
   **************************************************************************/

   ULONG cTotal;
   ULONG cReturned ;
   ULONG cbNeeded ;
   SplEnumQueue( PSZ(NULL), 3, 0, 0, &cReturned, &cTotal, &cbNeeded, NULL ) ;
   if ( cTotal == 0 ) {
      Sys_BeepError ( ) ;
      return ;
   } /* endif */

  /**************************************************************************
   * Increment the number of active prints.                                 *
   **************************************************************************/

   Data->PrintsActive ++ ;
   Sys_ShowWindow ( Data->PrintStatusWindow, TRUE ) ;
   Sys_PostMessage ( Data->MainWindow, WM_THREAD_STARTED, 0, 0 ) ;

  /**************************************************************************
   * Wait for queued editing to complete.                                   *
   **************************************************************************/

   Data->pDocument->FlushCommandQueue ( ) ;

  /**************************************************************************
   * Start the print thread.                                                *
   **************************************************************************/

   PPRINT_PARMS Parms = PPRINT_PARMS ( malloc ( sizeof(PRINT_PARMS) ) ) ;

   if ( Parms == 0 ) {
      Sys_BeepError ( ) ;
      return ;
   } /* endif */

   Parms->Library = Data->Library ;
   Parms->pDocument = new Documento ( WinQueryAnchorBlock(Window), *Data->pDocument ) ;
   Parms->PageNumber = USHORT ( PageNumber ) ;
   Parms->SpoolPages = Data->IniData->SpoolPages ;
   Parms->Reverse = Reverse ;
   Parms->WindowToNotify = Window ;
   Parms->MainWindow = Data->MainWindow ;

   StartThread ( "PrintThread", PrintThread, 0x10000, Parms ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Open Spool Job                                                      *
 *                                                                          *
 ****************************************************************************/

static BOOL Job_Open ( HAB Anchor, HMODULE Library, Documento *pDocument, int PageNumber, DeviceContext* &Printer, WorkSpace* &PS ) {

  /**************************************************************************
   * Open printer device context.  Abort if unable to do so.                *
   **************************************************************************/

   Printer = new DeviceContext ( "Job_Open", Anchor,
      pDocument->Query_Parms()->Printer, pDocument->Query_Parms()->FormName,
      PDRIVDATA(pDocument->Query_Parms()->JobSettings), 1 ) ;

   if ( Printer->QueryHandle() == 0 ) {
      ERRORID Code = Sys_GetLastError ( Anchor ) ;
      Log ( "PrintPage: Unable to create printer device context.  Error %lX.", Code ) ;
      return ( TRUE ) ;
   } /* endif */

  /**************************************************************************
   * Tell the printer we're starting a new spool job.                       *
   **************************************************************************/

   ResourceString JobPrefix ( Library, IDS_JOB_NAME ) ;
   ResourceString Untitled ( Library, IDS_UNTITLED ) ;

   char Title [500] ;
   strcpy ( Title, PCHAR(JobPrefix) ) ;
   if ( pDocument->Query_Filename() AND *pDocument->Query_Filename() ) {
      char Drive [_MAX_DRIVE] ;
      char Dir [_MAX_DIR] ;
      char Fname [_MAX_FNAME] ;
      char Ext [_MAX_EXT] ;
      _splitpath ( PCHAR(pDocument->Query_Filename()), Drive, Dir, Fname, Ext ) ;
      strcat ( Title, Fname ) ;
      strcat ( Title, Ext ) ;

   } else {
      strcat ( Title, PCHAR(Untitled) ) ;

   } /* endif */

   if ( PageNumber != ALL ) {
      ResourceString Page ( Library, IDS_PAGE ) ;
      ResourceString Of ( Library, IDS_OF ) ;
      sprintf ( Title+strlen(Title), ", %s %i %s %i.",
         PSZ(Page), PageNumber+1, PSZ(Of), pDocument->Query_PageCount() ) ;
   } /* endif */

   DevEscape ( Printer->QueryHandle(), DEVESC_STARTDOC, strlen(Title), PSZ(Title), 0, 0 ) ;

  /**************************************************************************
   * Create a presentation space associated with the printer.               *
   **************************************************************************/

   PS = new WorkSpace ( "PrintPage", pDocument->Query_Parms()->CodePage, Printer, Anchor,
      int(pDocument->Query_Parms()->Metric), FALSE ) ;

  /**************************************************************************
   * Adjust position for form clipping.                                     *
   **************************************************************************/

   PS->SetTransform ( 1.0, 0, int(pDocument->Query_State0()->FormInfo.yBottomClip) ) ;

  /**************************************************************************
   * Done successfully.  Return no error.                                   *
   **************************************************************************/

   return ( FALSE ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Close Spool Job                                                     *
 *                                                                          *
 ****************************************************************************/

static void Job_Close ( DeviceContext* &Printer, WorkSpace* &PS ) {

  /**************************************************************************
   * Destroy the working presentation space.                                *
   **************************************************************************/

   delete PS ;

  /**************************************************************************
   * Release the completed spool job.                                       *
   **************************************************************************/

   USHORT Job ;
   LONG OutCount = sizeof(Job) ;
   DevEscape ( Printer->QueryHandle(), DEVESC_ENDDOC, 0, 0, &OutCount, PBYTE(&Job) ) ;

  /**************************************************************************
   * Close the device context.                                              *
   **************************************************************************/

   delete Printer ;
}
 
/****************************************************************************
 *                                                                          *
 *      Print Single Page, spooled separately (used by PrintThread)         *
 *                                                                          *
 ****************************************************************************/

static BOOL PrintSinglePage ( HAB Anchor, HMODULE Library, Documento *pDocument, int PageNumber ) {

  /**************************************************************************
   * Open printer device context.  Abort if unable to do so.                *
   **************************************************************************/

   DeviceContext *Printer ;  WorkSpace *PS ;
   if ( Job_Open ( Anchor, Library, pDocument, PageNumber, Printer, PS ) )
      return ( TRUE ) ;

  /**************************************************************************
   * Render the page.                                                       *
   **************************************************************************/

   pDocument->MovePageNumber ( PageNumber ) ;
   pDocument->Render ( PS ) ;

  /**************************************************************************
   * Release the completed spool job.                                       *
   **************************************************************************/

   Job_Close ( Printer, PS ) ;

  /**************************************************************************
   * Done successfully.  Return no error.                                   *
   **************************************************************************/

   return ( FALSE ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Printing Thread                                                     *
 *                                                                          *
 ****************************************************************************/

STATIC void PrintThread ( PVOID Parameter ) {

  /**************************************************************************
   * Get parameters.                                                        *
   **************************************************************************/

   PPRINT_PARMS Parms = PPRINT_PARMS ( Parameter ) ;

  /**************************************************************************
   * Connect to PM.                                                         *
   **************************************************************************/

   Process Proc ( "PrintThread", 0, Parms->Library->QueryHandle() ) ;

  /**************************************************************************
   * If rendering all pages . . .                                           *
   **************************************************************************/

   if ( Parms->PageNumber == ALL ) {

      Parms->pDocument->MoveBottom ( ) ;

      if ( Parms->SpoolPages ) {
         if ( Parms->Reverse ) {
            for ( int i=Parms->pDocument->Query_PageCount()-1; i>=0; i-- ) {
               PrintSinglePage ( Proc.QueryAnchor(), Parms->Library->QueryHandle(), Parms->pDocument, i ) ;
            } /* endfor */
         } else {
            for ( int i=0; i<Parms->pDocument->Query_PageCount(); i++ ) {
               PrintSinglePage ( Proc.QueryAnchor(), Parms->Library->QueryHandle(), Parms->pDocument, i ) ;
            } /* endfor */
         } /* endif */

      } else {
         DeviceContext *Printer ;  WorkSpace *PS ;
         if ( !Job_Open ( Proc.QueryAnchor(), Parms->Library->QueryHandle(), Parms->pDocument, ALL, Printer, PS ) ) {
            if ( Parms->Reverse ) {
               for ( int i=Parms->pDocument->Query_PageCount()-1; i>=0; i-- ) {
                  Parms->pDocument->MovePageNumber ( i ) ;
                  Parms->pDocument->Render ( PS ) ;
                  if ( i )
                     PS->NewFrame ( ) ;
               } /* endfor */
            } else {
               for ( int i=0; i<Parms->pDocument->Query_PageCount(); i++ ) {
                  Parms->pDocument->MovePageNumber ( i ) ;
                  Parms->pDocument->Render ( PS ) ;
                  if ( i<Parms->pDocument->Query_PageCount()-1 )
                     PS->NewFrame ( ) ;
               } /* endfor */
            } /* endif */
            Job_Close ( Printer, PS ) ;
         } /* endif */

      } /* endif */

  /**************************************************************************
   * Else, if rendering single page.                                        *
   **************************************************************************/

   } else {
      PrintSinglePage ( Proc.QueryAnchor(), Parms->Library->QueryHandle(), Parms->pDocument, Parms->PageNumber ) ;

   } /* endif */

  /**************************************************************************
   * Notify the window that wanted to know when the print was finished.     *
   **************************************************************************/

   if ( !Sys_PostMessage ( Parms->WindowToNotify, WM_PRINT_DONE, 0, 0 ) ) {
      ERRORID Code = Sys_GetLastError ( Proc.QueryAnchor() ) ;
      if ( ( Code & 0xFFFF ) != PMERR_INVALID_HWND ) 
         Log ( "WARNING: Attempt to notify document window of print thread completion failed.  Error %lX.", Code ) ;
   } /* endif */

  /**************************************************************************
   * Notify the window that wanted to know when the thread was finished.    *
   **************************************************************************/

   if ( !Sys_PostMessage ( Parms->MainWindow, WM_THREAD_DONE, MPFROMLONG(Sys_ThreadNumber(*_threadid)), 0 ) ) {
      ERRORID Code = Sys_GetLastError ( Proc.QueryAnchor() ) ;
      if ( ( Code & 0xFFFF ) != PMERR_INVALID_HWND ) 
         Log ( "WARNING: Attempt to notify main window of print thread completion failed.  Error %lX.", Code ) ;
   } /* endif */

  /**************************************************************************
   * Destroy the document copy.                                             *
   **************************************************************************/

   delete Parms->pDocument ;

  /**************************************************************************
   * Release the parameter memory.                                          *
   **************************************************************************/

   free ( Parms ) ;
}

 
/****************************************************************************
 *                                                                          *
 *      Button 1 Down: Case if awaiting first point of rectangle.           *
 *                                                                          *
 ****************************************************************************/

STATIC BOOL Button1Down_AwaitingPoint0 ( HWND Window, PDATA Data, POINTL DevicePoint, POINTL ScaledPoint ) {

   // If not awaiting first point of rectangle, return FALSE.
   if ( Data->BoxMode != BOX_AWAITINGPOINT0 ) {
      return ( FALSE ) ;
   } /* endif */

   // If point out of valid range, beep and return TRUE.
   if ( Data->BoxType == BOX_GRAPHIC ) {
      if ( ( ScaledPoint.x < Data->PasteGraphic.BorderWidth ) OR ( ScaledPoint.x > Data->pDocument->Query_State0()->FormInfo.cx-Data->PasteGraphic.BorderWidth ) ) {
         Sys_BeepNote ( ) ;
         return ( TRUE ) ;
      } /* endif */
      if ( ( ScaledPoint.y < Data->PasteGraphic.BorderWidth ) OR ( ScaledPoint.y > Data->pDocument->Query_State0()->FormInfo.cy-Data->PasteGraphic.BorderWidth ) ) {
         Sys_BeepNote ( ) ;
         return ( TRUE ) ;
      } /* endif */
   } /* endif */

   // If the mouse hasn't been captured yet, do so now.
   if ( NOT Data->CaptureFlag )
      Data->CaptureFlag = Sys_SetCapture ( Window ) ;

   // Update the status window with the appropriate prompt.
   HWND Frame = PARENT ( Window ) ;
   HWND StatusWindow = CHILD ( Frame, FID_STATUSLINE ) ;
   ResourceString Message ( Data->Library->QueryHandle(), ( Data->BoxType == BOX_ZOOM ) ? IDS_ZOOM1 : IDS_PASTEGRAPHIC1 ) ;
   Sys_SetWindowText ( StatusWindow, PSZ(Message) ) ;

   // Update the rulers.
   AdjustRulerTicks ( Window, Data, &ScaledPoint ) ;

   // Update the current tracking mode.
   Data->BoxMode = BOX_AWAITINGPOINT1 ;

   // Update the tracking box.
   Data->BoxRectangle.xLeft = DevicePoint.x ;
   Data->BoxRectangle.yBottom = DevicePoint.y ;
   Data->BoxRectangle.xRight = DevicePoint.x ;
   Data->BoxRectangle.yTop = DevicePoint.y ;
   Data->WindowPS->DrawTrackingBox ( Data->BoxRectangle ) ;

   // Return TRUE, indicating that we've handled the event.
   return ( TRUE ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Button 1 Down: Default processing.                                  *
 *                                                                          *
 ****************************************************************************/

STATIC void Button1Down_Default ( HWND Window, PDATA Data, POINTL, POINTL ScaledPoint ) {

   // If the mouse hasn't been captured yet, do so now.
   if ( NOT Data->CaptureFlag )
      Data->CaptureFlag = Sys_SetCapture ( Window ) ;

   // Move to the token found.
   Data->pDocument->MoveXY ( ScaledPoint, FALSE ) ;

   // Update the physical cursor.
   Data->pDocument->UpdateCursor ( ) ;

}
 
/****************************************************************************
 *                                                                          *
 *      Button 1 Move: Case if awaiting second point of rectangle.          *
 *                                                                          *
 ****************************************************************************/

STATIC BOOL Button1Move_AwaitingPoint1 ( HWND Window, PDATA Data, POINTL DevicePoint, POINTL ScaledPoint ) {

   // If not waiting for second point of rectangle, return FALSE.
   if ( Data->BoxMode != BOX_AWAITINGPOINT1 ) {
      return ( FALSE ) ;
   } /* endif */

   // Clip point to within valid range.
   if ( Data->BoxType == BOX_GRAPHIC ) {
      ScaledPoint.x = min ( Data->pDocument->Query_State0()->FormInfo.cx-Data->PasteGraphic.BorderWidth, max ( Data->PasteGraphic.BorderWidth, ScaledPoint.x ) ) ;
      ScaledPoint.y = min ( Data->pDocument->Query_State0()->FormInfo.cy-Data->PasteGraphic.BorderWidth, max ( Data->PasteGraphic.BorderWidth, ScaledPoint.y ) ) ;
      DevicePoint = ScaledPoint ;
      Data->pDocument->Transform ( CVTC_DEFAULTPAGE, CVTC_DEVICE, DevicePoint ) ;
   } else {
      ScaledPoint.x = min ( Data->pDocument->Query_State0()->FormInfo.cx, max ( 0, ScaledPoint.x ) ) ;
      ScaledPoint.y = min ( Data->pDocument->Query_State0()->FormInfo.cy, max ( 0, ScaledPoint.y ) ) ;
      DevicePoint = ScaledPoint ;
      Data->pDocument->Transform ( CVTC_DEFAULTPAGE, CVTC_DEVICE, DevicePoint ) ;
   } /* endif */

   // Adjust ruler tick-marks.
   AdjustRulerTicks ( Window, Data, &ScaledPoint ) ;

   // Erase tracking box.
   Data->WindowPS->DrawTrackingBox ( Data->BoxRectangle ) ;

   // Update tracking box.
   Data->BoxRectangle.xRight = DevicePoint.x ;
   Data->BoxRectangle.yTop = DevicePoint.y ;

   // Draw tracking box.
   Data->WindowPS->DrawTrackingBox ( Data->BoxRectangle ) ;

   // Return TRUE, indicating that we've handled the event.
   return ( TRUE ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Button 1 Move: Default processing.                                  *
 *                                                                          *
 ****************************************************************************/

STATIC void Button1Move_Default ( HWND, PDATA Data, POINTL, POINTL ScaledPoint ) {

   // See what's under the mouse pointer.
   BOOL Selected ;
   int TokenType = Data->pDocument->TestXY ( ScaledPoint, Selected ) ;

   // If graphic, do nothing.
   if ( TokenType == TEST_GRAPHIC ) {

   // Else, if graphic border, do nothing.
   } else if ( TokenType == TEST_GRAPHICBORDER ) {

   // Else, if text, adjust the selection range.
   } else {

      // Position the logical cursor.
      Data->pDocument->MoveXY ( ScaledPoint, TRUE ) ;

      // Update the physical cursor.
      Data->pDocument->UpdateCursor ( ) ;

   } /* endif */
}
 
/****************************************************************************
 *                                                                          *
 *      Button 1 Up: Case if awaiting second point of rectangle.            *
 *                                                                          *
 ****************************************************************************/

STATIC BOOL Button1Up_AwaitingPoint1 ( HWND Window, PDATA Data, POINTL DevicePoint, POINTL ScaledPoint ) {

   // If we're not awaiting the second point, return FALSE.
   if ( Data->BoxMode != BOX_AWAITINGPOINT1 ) {
      return ( FALSE ) ;
   } /* endif */

   // If forming box for graphic cell, limit it by the form size and the graphic border width.
   if ( Data->BoxType == BOX_GRAPHIC ) {
      ScaledPoint.x = min ( Data->pDocument->Query_State0()->FormInfo.cx-Data->PasteGraphic.BorderWidth, max ( Data->PasteGraphic.BorderWidth, ScaledPoint.x ) ) ;
      ScaledPoint.y = min ( Data->pDocument->Query_State0()->FormInfo.cy-Data->PasteGraphic.BorderWidth, max ( Data->PasteGraphic.BorderWidth, ScaledPoint.y ) ) ;
      DevicePoint = ScaledPoint ;
      Data->pDocument->Transform ( CVTC_DEFAULTPAGE, CVTC_DEVICE, DevicePoint ) ;
   } /* endif */

   // Clear the status line.
   HWND Frame = PARENT ( Window ) ;
   HWND StatusWindow = CHILD ( Frame, FID_STATUSLINE ) ;
   Sys_SetWindowText ( StatusWindow, PSZ("") ) ;

   // Reset the tracking mode.
   Data->BoxMode = BOX_DORMANT ;

   // Reset the ruler tick tracking.
   POINTL NullPoint = { 0, 0 } ;
   AdjustRulerTicks ( Window, Data, &NullPoint ) ;

   // Erase the tracking rectangle.
   Data->WindowPS->DrawTrackingBox ( Data->BoxRectangle ) ;

   // Update the box.
   Data->BoxRectangle.xRight = DevicePoint.x ;
   Data->BoxRectangle.yTop = DevicePoint.y ;

   // Straighten out the sense of the rectangle (left/right-top/bottom).
   RECTL Rectangle ;
   Rectangle.xLeft   = min ( Data->BoxRectangle.xLeft, Data->BoxRectangle.xRight ) ;
   Rectangle.xRight  = max ( Data->BoxRectangle.xLeft, Data->BoxRectangle.xRight ) ;
   Rectangle.yBottom = min ( Data->BoxRectangle.yBottom, Data->BoxRectangle.yTop ) ;
   Rectangle.yTop    = max ( Data->BoxRectangle.yBottom, Data->BoxRectangle.yTop ) ;

   // Convert the box rectangle from device to scaled coordinates.
   Data->pDocument->Transform ( CVTC_DEVICE, CVTC_DEFAULTPAGE, Rectangle ) ;

   // Determine the image width and height.
   long Width = Rectangle.xRight - Rectangle.xLeft ;
   long Height = Rectangle.yTop - Rectangle.yBottom ;

   // If the rectangle has no height or width, then default the bitmap to be 1x1 on the printer.
   //   If not bitmap, just make it one inch or two centimeters on a side.
   if ( ( Width == 0 ) AND ( Height == 0 ) ) {
      Width = Height = Data->pDocument->Query_State0()->Metric ? 2000 : 1000 ;
      if ( !strnicmp ( Data->PasteGraphic.Type, "BM", 2 ) OR !strnicmp ( Data->PasteGraphic.Type, "MF", 2 ) ) {
         Data->PasteGraphic.PrinterOptimize = TRUE ;
         Data->PasteGraphic.Aspect = GRAPHIC_ASPECT_FIX_X ;
         POINTL Position = { Rectangle.xLeft, Rectangle.yBottom } ;
         SIZEL Size = { Width, Height } ;
         RECTL Box ;
         Data->PasteGraphic.ComputeBox ( 0, Position, Size, GRAPHIC_ASPECT_FIX_X, 
            TRUE, Box, *Data->pDocument->Query_Resolution() ) ;
         Width = Box.xRight - Box.xLeft ;
         Height = Box.yTop - Box.yBottom ;
      } /* endif */
 
   // Else, if the rectangle has no width, then default the bitmap to be 1x1 aspect with height fixed.
   } else if ( Width == 0 ) {
      if ( !strnicmp ( Data->PasteGraphic.Type, "BM", 2 ) OR !strnicmp ( Data->PasteGraphic.Type, "MF", 2 ) ) {
         Data->PasteGraphic.Aspect = GRAPHIC_ASPECT_FIX_Y ;
      } else {
         Width = Height ;
      } /* endif */

   // Else, if the rectangle has no height, then default the bitmap to be 1x1 aspect with width fixed.
   } else if ( Height == 0 ) {
      if ( !strnicmp ( Data->PasteGraphic.Type, "BM", 2 ) OR !strnicmp ( Data->PasteGraphic.Type, "MF", 2 ) ) {
         Data->PasteGraphic.Aspect = GRAPHIC_ASPECT_FIX_X ;
      } else {
         Height = Width ;
      } /* endif */

   } /* endif */

   // Save the final rectangle complete with adjustments.
   Rectangle.xRight = Rectangle.xLeft + Width ;
   Rectangle.yBottom = Rectangle.yTop - Height ;
   Data->BoxRectangle = Rectangle ;

   // If zooming
   if ( Data->BoxType == BOX_ZOOM ) {

      // Get the visible window's scaled position and size.
      WinQueryWindowRect ( Window, &Rectangle ) ;
      Data->pDocument->Transform ( CVTC_DEVICE, CVTC_PAGE, Rectangle ) ;

      // Save the zoom rectangle.
      Data->ZoomRectangle = Data->BoxRectangle ;

      // Set the window zoom.
      SetZoom ( Window, Data,
         double ( min ( double(Rectangle.xRight-Rectangle.xLeft) / double(Data->ZoomRectangle.xRight-Data->ZoomRectangle.xLeft),
         double(Rectangle.yTop-Rectangle.yBottom) / double(Data->ZoomRectangle.yTop-Data->ZoomRectangle.yBottom) ) ) ) ;

      // Adjust the scrollbars.
      Sys_PostMessage ( Window, WM_HSCROLL, MPFROM2SHORT ( FID_HORZSCROLL, 0 ),
         MPFROM2SHORT ( SHORT(Data->ZoomRectangle.xLeft), SB_SLIDERPOSITION ) ) ;
      Sys_PostMessage ( Window, WM_VSCROLL, MPFROM2SHORT ( FID_VERTSCROLL, 0 ),
         MPFROM2SHORT ( SHORT(Data->pDocument->Query_State0()->FormInfo.cy-Data->ZoomRectangle.yTop), SB_SLIDERPOSITION ) ) ;

   // Else, if placing a graphic . . .
   } else if ( Data->BoxType == BOX_GRAPHIC ) {

      // Create the graphic function code to be inserted into the document.
      char Text [100] ;
      sprintf ( Text, "%c%s%li,%li,%li,%li", FUNCTION_START, FunctionNames[FN__GRAPHIC],
         Data->BoxRectangle.xLeft, Data->BoxRectangle.yBottom,
         Data->BoxRectangle.xRight-Data->BoxRectangle.xLeft,
         Data->BoxRectangle.yTop-Data->BoxRectangle.yBottom ) ;
      if ( Data->PasteGraphic.Background  ) strcat ( Text, ",Background" ) ;
      if ( Data->PasteGraphic.NoTextAside ) strcat ( Text, ",NoTextAside" ) ;
      if ( Data->PasteGraphic.Halftone    ) strcat ( Text, ",Halftone" ) ;
      if ( Data->PasteGraphic.Aspect == GRAPHIC_ASPECT_FIX_X ) strcat ( Text, ",KeepAspectFixX" ) ;
      if ( Data->PasteGraphic.Aspect == GRAPHIC_ASPECT_FIX_Y ) strcat ( Text, ",KeepAspectFixY" ) ;
      if ( Data->PasteGraphic.PrinterOptimize ) strcat ( Text, ",PrinterOptimize" ) ;
      sprintf ( Text+strlen(Text), ":%0.2s", Data->PasteGraphic.Type ) ;
      if ( !strnicmp ( Data->PasteGraphic.Type, "BM", 2 ) ) {
         sprintf ( Text+strlen(Text), "%lu", Data->PasteGraphic.Handle ) ;
      } else if ( !strnicmp ( Data->PasteGraphic.Type, "MF", 2 ) ) {
         sprintf ( Text+strlen(Text), "%lu", Data->PasteGraphic.Handle ) ;
      } else if ( !strnicmp ( Data->PasteGraphic.Type, "UP", 2 ) ) {
         sprintf ( Text+strlen(Text), "%0.11s", Data->PasteGraphic.BarCode ) ;
      } else if ( !strnicmp ( Data->PasteGraphic.Type, "UE", 2 ) ) {
         sprintf ( Text+strlen(Text), "%0.6s", Data->PasteGraphic.BarCode ) ;
      } else if ( !strnicmp ( Data->PasteGraphic.Type, "39", 2 ) ) {
         sprintf ( Text+strlen(Text), "%0.80s", Data->PasteGraphic.BarCode ) ;
      } else if ( !strnicmp ( Data->PasteGraphic.Type, "PN", 2 ) ) {
         sprintf ( Text+strlen(Text), "%0.9s", Data->PasteGraphic.BarCode ) ;
      } else if ( !strnicmp ( Data->PasteGraphic.Type, "E8", 2 ) ) {
         sprintf ( Text+strlen(Text), "%0.7s", Data->PasteGraphic.BarCode ) ;
      } else if ( !strnicmp ( Data->PasteGraphic.Type, "EA", 2 ) ) {
         sprintf ( Text+strlen(Text), "%0.12s", Data->PasteGraphic.BarCode ) ;
      } /* endif */
      sprintf ( Text+strlen(Text), "%c", FUNCTION_END ) ;

      // Insert the graphic function code.
      Data->pDocument->Enqueue_InsertText ( Text ) ;

   } /* endif */

   // Return TRUE, indicating that we've handled the event.
   return ( TRUE ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Button 1 Up: Default processing.                                    *
 *                                                                          *
 ****************************************************************************/

STATIC void Button1Up_Default ( HWND, PDATA Data, POINTL, POINTL ScaledPoint ) {

   // See what's under the mouse pointer.
   BOOL Selected ;
   int TokenType = Data->pDocument->TestXY ( ScaledPoint, Selected ) ;

   // If over graphic, do nothing.
   if ( TokenType == TEST_GRAPHIC ) {

   // Else, if over graphic border, do nothing.
   } else if ( TokenType == TEST_GRAPHICBORDER ) {

   // Else, we're over text.  Move the cursor.
   } else {

      // Position the logical cursor.
      Data->pDocument->MoveXY ( ScaledPoint, TRUE ) ;

      // Update the physical cursor.
      Data->pDocument->UpdateCursor ( ) ;

   } /* endif */

}
 
/****************************************************************************
 *                                                                          *
 *      Button 2 Down: Case if graphic was touched.                         *
 *                                                                          *
 ****************************************************************************/

STATIC BOOL Button2Down_TouchedGraphic ( HWND Window, PDATA Data, POINTL DevicePoint, POINTL ScaledPoint, int TokenType ) {

   // If not touching graphic, return FALSE.
   if ( TokenType != TEST_GRAPHIC ) 
      return ( FALSE ) ;

   // Update tracking state.
   Data->MovePosition = DevicePoint ;
   Data->pDocument->QueryGraphic ( ScaledPoint, Data->MoveGraphic, Data->MoveRectangle ) ;
   Data->pDocument->Transform ( CVTC_DEFAULTPAGE, CVTC_DEVICE, Data->MoveRectangle ) ;

   // Drawing tracking box.
   DrawGraphicTrackingBox ( Data ) ;

   // If the mouse hasn't been captured yet, do so now.
   if ( NOT Data->CaptureFlag )
      Data->CaptureFlag = Sys_SetCapture ( Window ) ;

   // Update the status window with the appropriate prompt.
   HWND Frame = PARENT ( Window ) ;
   HWND StatusWindow = CHILD ( Frame, FID_STATUSLINE ) ;
   ResourceString Message ( Data->Library->QueryHandle(), IDS_MOVEGRAPHIC ) ;
   Sys_SetWindowText ( StatusWindow, PSZ(Message) ) ;

   // Update tracking state.
   Data->MoveType = MOVE_GRAPHIC ;

   // Return TRUE, indicating that we've handled the event.
   return ( TRUE ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Button 2 Down: Case if graphic border was touched.                  *
 *                                                                          *
 ****************************************************************************/

STATIC BOOL Button2Down_TouchedGraphicBorder ( HWND Window, PDATA Data, POINTL DevicePoint, POINTL ScaledPoint, int TokenType ) {

   // If not touching graphic border, return FALSE.
   if ( TokenType != TEST_GRAPHICBORDER ) 
      return ( FALSE ) ;

   // Update tracking state.
   Data->MovePosition = DevicePoint ;
   Data->pDocument->QueryGraphic ( ScaledPoint, Data->MoveGraphic, Data->MoveRectangle ) ;
   Data->pDocument->Transform ( CVTC_DEFAULTPAGE, CVTC_DEVICE, Data->MoveRectangle ) ;

   // Drawing tracking box.
   DrawGraphicTrackingBox ( Data ) ;

   // If the mouse hasn't been captured yet, do so now.
   if ( NOT Data->CaptureFlag )
      Data->CaptureFlag = Sys_SetCapture ( Window ) ;

   // Update the status window with the appropriate prompt.
   HWND Frame = PARENT ( Window ) ;
   HWND StatusWindow = CHILD ( Frame, FID_STATUSLINE ) ;
   ResourceString Message ( Data->Library->QueryHandle(), IDS_SIZEGRAPHIC ) ;
   Sys_SetWindowText ( StatusWindow, PSZ(Message) ) ;

   // Update tracking state.
   Data->MoveType = SIZE_GRAPHIC ;

   // Return TRUE, indicating that we've handled the event.
   return ( TRUE ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Button 2 Down: Case if selected text was touched.                   *
 *                                                                          *
 ****************************************************************************/

STATIC BOOL Button2Down_TouchedSelectedText ( HWND Window, PDATA Data, POINTL, POINTL, int TokenType, BOOL Selected ) {

   if ( ( TokenType != TEST_TEXT ) AND NOT Selected ) 
      return ( FALSE ) ;

   // If the mouse hasn't been captured yet, do so now.
   if ( NOT Data->CaptureFlag )
      Data->CaptureFlag = Sys_SetCapture ( Window ) ;

   // Update the status window with the appropriate prompt.
   HWND Frame = PARENT ( Window ) ;
   HWND StatusWindow = CHILD ( Frame, FID_STATUSLINE ) ;
   ResourceString Message ( Data->Library->QueryHandle(), IDS_MOVETEXT ) ;
   Sys_SetWindowText ( StatusWindow, PSZ(Message) ) ;

   // Update tracking state.
   Data->MoveType = MOVE_TEXT ;

   // Return TRUE, indicating that we've handled the event.
   return ( TRUE ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Button 2 Down: Default action.                                      *
 *                                                                          *
 ****************************************************************************/

STATIC void Button2Down_Default ( HWND Window, PDATA Data, POINTL, POINTL ) {

   // If the mouse hasn't been captured yet, do so now.
   if ( NOT Data->CaptureFlag )
      Data->CaptureFlag = Sys_SetCapture ( Window ) ;

   // Update tracking state.
   Data->MoveType = MOVE_NULL ;
}
 
/****************************************************************************
 *                                                                          *
 *      Button 2 Move: Move/Copy Graphic                                    *
 *                                                                          *
 ****************************************************************************/

STATIC BOOL Button2Move_MoveGraphic ( HWND, PDATA Data, POINTL DevicePoint, POINTL ) {

   // If we aren't moving a graphic, return FALSE.
   if ( Data->MoveType != MOVE_GRAPHIC ) {
      return ( FALSE ) ;
   } /* endif */

   POINTL Offset ;
   Offset.x = DevicePoint.x - Data->MovePosition.x ;
   Offset.y = DevicePoint.y - Data->MovePosition.y ;
   RECTL Rectangle = Data->MoveRectangle ;
   Rectangle.xLeft   += Offset.x ;
   Rectangle.xRight  += Offset.x ;
   Rectangle.yBottom += Offset.y ;
   Rectangle.yTop    += Offset.y ;
   Data->pDocument->Transform ( CVTC_DEVICE, CVTC_DEFAULTPAGE, DevicePoint ) ;
   Data->pDocument->Transform ( CVTC_DEVICE, CVTC_DEFAULTPAGE, Rectangle ) ;
   if ( Rectangle.xLeft < Data->MoveGraphic.BorderWidth ) {
      DevicePoint.x -= Rectangle.xLeft - Data->MoveGraphic.BorderWidth ;
      Rectangle.xRight -= Rectangle.xLeft - Data->MoveGraphic.BorderWidth ;
      Rectangle.xLeft = Data->MoveGraphic.BorderWidth ;
   } /* endif */
   if ( Rectangle.xRight > Data->pDocument->Query_State0()->FormInfo.cx-Data->MoveGraphic.BorderWidth ) {
      DevicePoint.x -= Rectangle.xRight - ( Data->pDocument->Query_State0()->FormInfo.cx - Data->MoveGraphic.BorderWidth ) ;
      Rectangle.xLeft -= Rectangle.xRight - ( Data->pDocument->Query_State0()->FormInfo.cx - Data->MoveGraphic.BorderWidth ) ;
      Rectangle.xRight = Data->pDocument->Query_State0()->FormInfo.cx - Data->MoveGraphic.BorderWidth ;
   } /* endif */
   if ( Rectangle.yBottom < Data->MoveGraphic.BorderWidth ) {
      DevicePoint.y -= Rectangle.yBottom - Data->MoveGraphic.BorderWidth ;
      Rectangle.yTop -= Rectangle.yBottom - Data->MoveGraphic.BorderWidth ;
      Rectangle.yBottom = Data->MoveGraphic.BorderWidth ;
   } /* endif */
   if ( Rectangle.yTop > Data->pDocument->Query_State0()->FormInfo.cy-Data->MoveGraphic.BorderWidth ) {
      DevicePoint.y -= Rectangle.yTop - ( Data->pDocument->Query_State0()->FormInfo.cy - Data->MoveGraphic.BorderWidth ) ;
      Rectangle.yBottom -= Rectangle.yTop - ( Data->pDocument->Query_State0()->FormInfo.cy - Data->MoveGraphic.BorderWidth ) ;
      Rectangle.yTop = Data->pDocument->Query_State0()->FormInfo.cy - Data->MoveGraphic.BorderWidth ;
   } /* endif */
   Data->pDocument->Transform ( CVTC_DEFAULTPAGE, CVTC_DEVICE, Rectangle ) ;
   Data->pDocument->Transform ( CVTC_DEFAULTPAGE, CVTC_DEVICE, DevicePoint ) ;

   DrawGraphicTrackingBox ( Data ) ;
   Data->MoveRectangle = Rectangle ;
   Data->MovePosition = DevicePoint ;
   DrawGraphicTrackingBox ( Data ) ;

   // Return TRUE, indicating that we've handled the event.
   return ( TRUE ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Button 2 Move: Size Graphic                                         *
 *                                                                          *
 ****************************************************************************/

STATIC BOOL Button2Move_SizeGraphic ( HWND, PDATA Data, POINTL DevicePoint, POINTL ) {

   // If we aren't sizing a graphic, return FALSE.
   if ( Data->MoveType != SIZE_GRAPHIC ) {
      return ( FALSE ) ;
   } /* endif */

   RECTL Rectangle = Data->MoveRectangle ;
   POINTL Position = Data->MovePosition ;
   Data->pDocument->Transform ( CVTC_DEVICE, CVTC_DEFAULTPAGE, DevicePoint ) ;
   Data->pDocument->Transform ( CVTC_DEVICE, CVTC_DEFAULTPAGE, Rectangle ) ;
   Data->pDocument->Transform ( CVTC_DEVICE, CVTC_DEFAULTPAGE, Position ) ;
   if ( Position.x < Rectangle.xLeft ) {
      LONG Offset = Position.x - Rectangle.xLeft ;
      Rectangle.xLeft = max ( Data->MoveGraphic.BorderWidth, min ( Rectangle.xRight, DevicePoint.x - Offset ) ) ;
      DevicePoint.x = Rectangle.xLeft + Offset ;
   } /* endif */
   if ( Position.x > Rectangle.xRight ) {
      LONG Offset = Position.x - Rectangle.xRight ;
      Rectangle.xRight = min ( Data->pDocument->Query_State0()->FormInfo.cx-Data->MoveGraphic.BorderWidth, max ( Rectangle.xLeft, DevicePoint.x - Offset ) ) ;
      DevicePoint.x = Rectangle.xRight + Offset ;
   } /* endif */
   if ( Position.y < Rectangle.yBottom ) {
      LONG Offset = Position.y - Rectangle.yBottom ;
      Rectangle.yBottom = max ( Data->MoveGraphic.BorderWidth, min ( Rectangle.yTop, DevicePoint.y - Offset ) ) ;
      DevicePoint.y = Rectangle.yBottom + Offset ;
   } /* endif */
   if ( Position.y > Rectangle.yTop ) {
      LONG Offset = Position.y - Rectangle.yTop ;
      Rectangle.yTop = min ( Data->pDocument->Query_State0()->FormInfo.cy-Data->MoveGraphic.BorderWidth, max ( Rectangle.yBottom, DevicePoint.y - Offset ) ) ;
      DevicePoint.y = Rectangle.yTop + Offset ;
   } /* endif */
   Data->pDocument->Transform ( CVTC_DEFAULTPAGE, CVTC_DEVICE, Position ) ;
   Data->pDocument->Transform ( CVTC_DEFAULTPAGE, CVTC_DEVICE, Rectangle ) ;
   Data->pDocument->Transform ( CVTC_DEFAULTPAGE, CVTC_DEVICE, DevicePoint ) ;

   DrawGraphicTrackingBox ( Data ) ;
   Data->MoveRectangle = Rectangle ;
   Data->MovePosition = DevicePoint ;
   DrawGraphicTrackingBox ( Data ) ;

   // Return TRUE, indicating that we've handled the event.
   return ( TRUE ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Button 2 Move: Move/Copy Text                                       *
 *                                                                          *
 ****************************************************************************/

STATIC BOOL Button2Move_MoveText ( HWND, PDATA Data, POINTL, POINTL ScaledPoint ) {

   // If we aren't moving text, return FALSE.
   if ( Data->MoveType != MOVE_TEXT ) {
      return ( FALSE ) ;
   } /* endif */

   // Move logical text cursor w/o affecting selection.
   Data->pDocument->MoveXY ( ScaledPoint ) ;

   // Move physical text cursor.
   Data->pDocument->UpdateCursor ( ) ;

   // Return TRUE, indicating that we've handled the event.
   return ( TRUE ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Button 2 Move: Default processing.                                  *
 *                                                                          *
 ****************************************************************************/

STATIC void Button2Move_Default ( HWND, PDATA, POINTL, POINTL ) {

}
 
/****************************************************************************
 *                                                                          *
 *      Button 2 Up: Move/Copy Graphic                                      *
 *                                                                          *
 ****************************************************************************/

STATIC BOOL Button2Up_MoveGraphic ( HWND Window, PDATA Data, POINTL DevicePoint, POINTL ScaledPoint, BOOL Move, BOOL Button1 ) {

   // If we aren't moving a graphic, return FALSE.
   if ( Data->MoveType != MOVE_GRAPHIC ) 
      return ( FALSE ) ;

   POINTL Offset ;
   Offset.x = DevicePoint.x - Data->MovePosition.x ;
   Offset.y = DevicePoint.y - Data->MovePosition.y ;
   RECTL Rectangle = Data->MoveRectangle ;
   Rectangle.xLeft   += Offset.x ;
   Rectangle.xRight  += Offset.x ;
   Rectangle.yBottom += Offset.y ;
   Rectangle.yTop    += Offset.y ;
   Data->pDocument->Transform ( CVTC_DEVICE, CVTC_DEFAULTPAGE, DevicePoint ) ;
   Data->pDocument->Transform ( CVTC_DEVICE, CVTC_DEFAULTPAGE, Rectangle ) ;
   if ( Rectangle.xLeft < Data->MoveGraphic.BorderWidth ) {
      DevicePoint.x -= Rectangle.xLeft - Data->MoveGraphic.BorderWidth ;
      Rectangle.xRight -= Rectangle.xLeft - Data->MoveGraphic.BorderWidth ;
      Rectangle.xLeft = Data->MoveGraphic.BorderWidth ;
   } /* endif */
   if ( Rectangle.xRight > Data->pDocument->Query_State0()->FormInfo.cx-Data->MoveGraphic.BorderWidth ) {
      DevicePoint.x -= Rectangle.xRight - ( Data->pDocument->Query_State0()->FormInfo.cx - Data->MoveGraphic.BorderWidth ) ;
      Rectangle.xLeft -= Rectangle.xRight - ( Data->pDocument->Query_State0()->FormInfo.cx - Data->MoveGraphic.BorderWidth ) ;
      Rectangle.xRight = Data->pDocument->Query_State0()->FormInfo.cx - Data->MoveGraphic.BorderWidth ;
   } /* endif */
   if ( Rectangle.yBottom < Data->MoveGraphic.BorderWidth ) {
      DevicePoint.y -= Rectangle.yBottom - Data->MoveGraphic.BorderWidth ;
      Rectangle.yTop -= Rectangle.yBottom - Data->MoveGraphic.BorderWidth ;
      Rectangle.yBottom = Data->MoveGraphic.BorderWidth ;
   } /* endif */
   if ( Rectangle.yTop > Data->pDocument->Query_State0()->FormInfo.cy-Data->MoveGraphic.BorderWidth ) {
      DevicePoint.y -= Rectangle.yTop - ( Data->pDocument->Query_State0()->FormInfo.cy - Data->MoveGraphic.BorderWidth ) ;
      Rectangle.yBottom -= Rectangle.yTop - ( Data->pDocument->Query_State0()->FormInfo.cy - Data->MoveGraphic.BorderWidth ) ;
      Rectangle.yTop = Data->pDocument->Query_State0()->FormInfo.cy - Data->MoveGraphic.BorderWidth ;
   } /* endif */
   Data->pDocument->Transform ( CVTC_DEFAULTPAGE, CVTC_DEVICE, Rectangle ) ;
   Data->pDocument->Transform ( CVTC_DEFAULTPAGE, CVTC_DEVICE, DevicePoint ) ;

   DrawGraphicTrackingBox ( Data ) ;

   Data->MoveRectangle.xLeft   += DevicePoint.x - Data->MovePosition.x ;
   Data->MoveRectangle.xRight  += DevicePoint.x - Data->MovePosition.x ;
   Data->MoveRectangle.yTop    += DevicePoint.y - Data->MovePosition.y ;
   Data->MoveRectangle.yBottom += DevicePoint.y - Data->MovePosition.y ;

   // If the mouse didn't move, just return w/o processing.  Let normal Context Menu happen.
   if ( ( ScaledPoint.x == Data->MouseOrigin.x ) AND ( ScaledPoint.y == Data->MouseOrigin.y ) ) {
      if ( !Button1 ) 
         Sys_PostMessage ( Window, WM_CONTEXTMENU, MPFROM2SHORT(DevicePoint.x,DevicePoint.y), 0 ) ;

   // Otherwise, if the control key was not pressed, move the graphic.
   } else if ( Move ) {
      Data->pDocument->Transform ( CVTC_DEVICE, CVTC_DEFAULTPAGE, Data->MoveRectangle ) ;
      Data->MoveGraphic.Position.x = Data->MoveRectangle.xLeft ;
      Data->MoveGraphic.Position.y = Data->MoveRectangle.yBottom ;
      Data->pDocument->UpdateGraphic ( Data->MoveGraphic ) ;

   // Otherwise, copy the graphic.
   } else {
      Data->pDocument->Transform ( CVTC_DEVICE, CVTC_DEFAULTPAGE, Data->MoveRectangle ) ;
      Data->MoveGraphic.Position.x = Data->MoveRectangle.xLeft ;
      Data->MoveGraphic.Position.y = Data->MoveRectangle.yBottom ;
      Data->MoveGraphic.Size.cx = Data->MoveRectangle.xRight - Data->MoveRectangle.xLeft ;
      Data->MoveGraphic.Size.cy = Data->MoveRectangle.yTop - Data->MoveRectangle.yBottom ;
      Data->pDocument->CopyGraphic ( Data->MoveGraphic ) ;

   } /* endif */

   // Move has completed.  Reset the move type flag.
   Data->MoveType = MOVE_NULL ;

   // Return TRUE, indicating that we've handled the event.
   return ( TRUE ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Button 2 Up: Size Graphic                                           *
 *                                                                          *
 ****************************************************************************/

STATIC BOOL Button2Up_SizeGraphic ( HWND Window, PDATA Data, POINTL DevicePoint, POINTL ScaledPoint, BOOL Button1 ) {

   // If we aren't sizing a graphic, return FALSE.
   if ( Data->MoveType != SIZE_GRAPHIC ) 
      return ( FALSE ) ;

   RECTL Rectangle = Data->MoveRectangle ;
   POINTL Position = Data->MovePosition ;
   Data->pDocument->Transform ( CVTC_DEVICE, CVTC_DEFAULTPAGE, DevicePoint ) ;
   Data->pDocument->Transform ( CVTC_DEVICE, CVTC_DEFAULTPAGE, Rectangle ) ;
   Data->pDocument->Transform ( CVTC_DEVICE, CVTC_DEFAULTPAGE, Position ) ;
   if ( Position.x < Rectangle.xLeft ) {
      LONG Offset = Position.x - Rectangle.xLeft ;
      Rectangle.xLeft = max ( Data->MoveGraphic.BorderWidth, min ( Rectangle.xRight, DevicePoint.x - Offset ) ) ;
      DevicePoint.x = Rectangle.xLeft + Offset ;
   } /* endif */
   if ( Position.x > Rectangle.xRight ) {
      LONG Offset = Position.x - Rectangle.xRight ;
      Rectangle.xRight = min ( Data->pDocument->Query_State0()->FormInfo.cx-Data->MoveGraphic.BorderWidth, max ( Rectangle.xLeft, DevicePoint.x - Offset ) ) ;
      DevicePoint.x = Rectangle.xRight + Offset ;
   } /* endif */
   if ( Position.y < Rectangle.yBottom ) {
      LONG Offset = Position.y - Rectangle.yBottom ;
      Rectangle.yBottom = max ( Data->MoveGraphic.BorderWidth, min ( Rectangle.yTop, DevicePoint.y - Offset ) ) ;
      DevicePoint.y = Rectangle.yBottom + Offset ;
   } /* endif */
   if ( Position.y > Rectangle.yTop ) {
      LONG Offset = Position.y - Rectangle.yTop ;
      Rectangle.yTop = min ( Data->pDocument->Query_State0()->FormInfo.cy-Data->MoveGraphic.BorderWidth, max ( Rectangle.yBottom, DevicePoint.y - Offset ) ) ;
      DevicePoint.y = Rectangle.yTop + Offset ;
   } /* endif */
   Data->pDocument->Transform ( CVTC_DEFAULTPAGE, CVTC_DEVICE, Position ) ;
   Data->pDocument->Transform ( CVTC_DEFAULTPAGE, CVTC_DEVICE, Rectangle ) ;
   Data->pDocument->Transform ( CVTC_DEFAULTPAGE, CVTC_DEVICE, DevicePoint ) ;

   DrawGraphicTrackingBox ( Data ) ;

   // If the mouse didn't move, just return w/o processing.  Let normal Context Menu happen.
   if ( ( ScaledPoint.x == Data->MouseOrigin.x ) AND ( ScaledPoint.y == Data->MouseOrigin.y ) ) {
      if ( !Button1 ) 
         Sys_PostMessage ( Window, WM_CONTEXTMENU, MPFROM2SHORT(DevicePoint.x,DevicePoint.y), 0 ) ;

   // Otherwise, size the graphic.
   } else {
      Data->pDocument->Transform ( CVTC_DEVICE, CVTC_DEFAULTPAGE, Data->MoveRectangle ) ;
      Data->MoveGraphic.Position.x = Data->MoveRectangle.xLeft ;
      Data->MoveGraphic.Position.y = Data->MoveRectangle.yBottom ;
      Data->MoveGraphic.Size.cx = Data->MoveRectangle.xRight - Data->MoveRectangle.xLeft ;
      Data->MoveGraphic.Size.cy = Data->MoveRectangle.yTop - Data->MoveRectangle.yBottom ;
      Data->pDocument->UpdateGraphic ( Data->MoveGraphic ) ;

   } /* endif */

   // Size has completed.  Reset the move type flag.
   Data->MoveType = MOVE_NULL ;

   // Return TRUE, indicating that we've handled the event.
   return ( TRUE ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Button 2 Up: Move/Copy Text                                         *
 *                                                                          *
 ****************************************************************************/

STATIC BOOL Button2Up_MoveText ( HWND Window, PDATA Data, POINTL DevicePoint, POINTL ScaledPoint, BOOL Move ) {

   // If we aren't moving text, return FALSE.
   if ( Data->MoveType != MOVE_TEXT ) {
      return ( FALSE ) ;
   } /* endif */

   // If the mouse didn't move, call up the context menu.
   if ( ( ScaledPoint.x == Data->MouseOrigin.x ) AND ( ScaledPoint.y == Data->MouseOrigin.y ) ) {
      Sys_PostMessage ( Window, WM_CONTEXTMENU, MPFROM2SHORT(DevicePoint.x,DevicePoint.y), 0 ) ;

   // Otherwise, move/copy the text.
   } else {

      // Move the cursor to the mouse position.
      Data->pDocument->MoveXY ( ScaledPoint ) ;

      // Ask the document to perform the move/copy.
      Data->pDocument->MoveText ( NOT Move ) ;

   } /* endif */

   // Move has completed.  Reset the move type flag.
   Data->MoveType = MOVE_NULL ;

   // Return TRUE, indicating that we've handled the event.
   return ( TRUE ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Button 2 Up: Default processing.                                    *
 *                                                                          *
 ****************************************************************************/

STATIC void Button2Up_Default ( HWND Window, PDATA Data, POINTL DevicePoint, POINTL ScaledPoint ) {

   // If the mouse didn't move, call up the context menu.
   if ( ( ScaledPoint.x == Data->MouseOrigin.x ) AND ( ScaledPoint.y == Data->MouseOrigin.y ) ) {
      Sys_PostMessage ( Window, WM_CONTEXTMENU, MPFROM2SHORT(DevicePoint.x,DevicePoint.y), 0 ) ;
   } else {
      Sys_BeepNote ( ) ;
   } /* endif */
}
 
/****************************************************************************
 *                                                                          *
 *      Update Mouse Pointer                                                *
 *                                                                          *
 ****************************************************************************/

STATIC MRESULT APIENTRY UpdateMousePtr ( HWND Window, MESG, MPARAM1, MPARAM2 ) {
   PDATA Data = PDATA ( Sys_GetWindowData ( Window ) ) ;
   UpdateMousePointer ( Window, Data ) ;
   return ( 0 ) ;
}

STATIC void UpdateMousePointer ( HWND Window, PDATA Data ) {

   POINTL DevicePoint ;
   WinQueryPointerPos ( HWND_DESKTOP, &DevicePoint ) ;
   WinMapWindowPoints ( HWND_DESKTOP, Window, &DevicePoint, 1 ) ;

   RECTL Rectangle ;
   WinQueryWindowRect ( Window, &Rectangle ) ;

   BOOL Mouse_Captured = WinQueryCapture ( HWND_DESKTOP ) == Window ;

   BOOL Over_This_Window = TRUE ;
   Over_This_Window = Over_This_Window && ( DevicePoint.x >= Rectangle.xLeft ) ;
   Over_This_Window = Over_This_Window && ( DevicePoint.x <  Rectangle.xRight ) ;
   Over_This_Window = Over_This_Window && ( DevicePoint.y >= Rectangle.yBottom ) ;
   Over_This_Window = Over_This_Window && ( DevicePoint.y <  Rectangle.yTop ) ;

   if ( Mouse_Captured OR Over_This_Window ) 
      UpdateMousePointer ( Data, DevicePoint ) ;
}

STATIC void UpdateMousePointer ( PDATA Data, POINTL &DevicePoint ) {

   // Scale the device coordinates to get the document coordinates.
   POINTL ConvertedPoint = DevicePoint ;
   Data->pDocument->Transform ( CVTC_DEVICE, CVTC_DEFAULTPAGE, ConvertedPoint ) ;

   // Default to i-bar text pointer.
   int Type = SPTR_TEXT ;

   // If beyond page edges to left or right, set error pointer.
   if ( ( ConvertedPoint.x < 0 ) OR ( ConvertedPoint.x > Data->pDocument->Query_State0()->FormInfo.cx ) ) {
      Type = SPTR_ICONERROR ;

   // If beyond page edges to top or bottom, set error pointer.
   } else if ( ( ConvertedPoint.y < 0 ) OR ( ConvertedPoint.y > Data->pDocument->Query_State0()->FormInfo.cy ) ) {
      Type = SPTR_ICONERROR ;

   // If trying to define a box . . .
   } else if ( Data->BoxMode != BOX_DORMANT ) {

      // If first point hasn't been set yet, then set normal arrow pointer.
      if ( Data->BoxMode == BOX_AWAITINGPOINT0 ) {
         Type = SPTR_ARROW ;

      // Otherwise, set the pointer according to where we are with respect
      //   to the first point.
      } else {
         long DeltaX = DevicePoint.x - Data->BoxRectangle.xLeft ;
         long DeltaY = DevicePoint.y - Data->BoxRectangle.yBottom ;
         if ( ( DeltaX == 0 ) AND ( DeltaY == 0 ) ) {
            Type = SPTR_MOVE ;
         } else if ( DeltaX == 0 ) {
            Type = SPTR_SIZENS ;
         } else if ( DeltaY == 0 ) {
            Type = SPTR_SIZEWE ;
         } else if ( ( DeltaX > 0 ) AND ( DeltaY > 0 ) ) {
            Type = SPTR_SIZENESW ;
         } else if ( ( DeltaX > 0 ) AND ( DeltaY < 0 ) ) {
            Type = SPTR_SIZENWSE ;
         } else if ( ( DeltaX < 0 ) AND ( DeltaY > 0 ) ) {
            Type = SPTR_SIZENWSE ;
         } else if ( ( DeltaX < 0 ) AND ( DeltaY < 0 ) ) {
            Type = SPTR_SIZENESW ;
         } /* endif */
      } /* endif */

   // If we're moving or sizing something . . .
   } else if ( Data->Button2Down ) {           

      // If we're moving or copying a graphic, set the four-headed arrow.
      if ( Data->MoveType == MOVE_GRAPHIC ) {           
         Type = SPTR_MOVE ;

      // Else, if we're sizing a graphic, set a sizing pointer whose direction
      //   depends on which side of the graphic we're dealing with.
      } else if ( Data->MoveType == SIZE_GRAPHIC ) {    //   Sizing graphic.
         if ( Data->MovePosition.x < Data->MoveRectangle.xLeft ) {
            if ( Data->MovePosition.y < Data->MoveRectangle.yBottom ) {
               Type = SPTR_SIZENESW ;
            } else if ( Data->MovePosition.y > Data->MoveRectangle.yTop ) {
               Type = SPTR_SIZENWSE ;
            } else {
               Type = SPTR_SIZEWE ;
            } /* endif */
         } else if ( Data->MovePosition.x > Data->MoveRectangle.xRight ) {
            if ( Data->MovePosition.y < Data->MoveRectangle.yBottom ) {
               Type = SPTR_SIZENWSE ;
            } else if ( Data->MovePosition.y > Data->MoveRectangle.yTop ) {
               Type = SPTR_SIZENESW ;
            } else {
               Type = SPTR_SIZEWE ;
            } /* endif */
         } else if ( Data->MovePosition.y < Data->MoveRectangle.yBottom ) {
            if ( Data->MovePosition.x < Data->MoveRectangle.xLeft ) {
               Type = SPTR_SIZENESW ;
            } else if ( Data->MovePosition.x > Data->MoveRectangle.xRight ) {
               Type = SPTR_SIZENWSE ;
            } else {
               Type = SPTR_SIZENS ;
            } /* endif */
         } else if ( Data->MovePosition.y > Data->MoveRectangle.yTop ) {
            if ( Data->MovePosition.x < Data->MoveRectangle.xLeft ) {
               Type = SPTR_SIZENWSE ;
            } else if ( Data->MovePosition.x > Data->MoveRectangle.xRight ) {
               Type = SPTR_SIZENESW ;
            } else {
               Type = SPTR_SIZENS ;
            } /* endif */
         } /* endif */

      // Else, we're moving or copying text.  Set i-bar text pointer.
      } else {
         Type = SPTR_TEXT ;

      } /* endif */

   // Else, if doing nothing in particular . . .
   } else if ( !Data->Button1Down AND !Data->Button2Down ) { 

      // Determine what the pointer is sitting over.
      BOOL Selected ;
      int TokenType = Data->pDocument->TestXY ( ConvertedPoint, Selected ) ;

      // If it's a graphic, then set the four-pointed move pointer.
      if ( TokenType == TEST_GRAPHIC ) {
         Type = SPTR_MOVE ;

      // Else, if it's a graphic border, then set a sizing pointer whose direction
      //   depends on which side of the graphic we're over.
      } else if ( TokenType == TEST_GRAPHICBORDER ) {
         Grafico Graphic ; RECTL Box ;
         Data->pDocument->QueryGraphic ( ConvertedPoint, Graphic, Box ) ;
         if ( ConvertedPoint.x < Box.xLeft ) {
            if ( ConvertedPoint.y < Box.yBottom ) {
               Type = SPTR_SIZENESW ;
            } else if ( ConvertedPoint.y > Box.yTop ) {
               Type = SPTR_SIZENWSE ;
            } else {
               Type = SPTR_SIZEWE ;
            } /* endif */
         } else if ( ConvertedPoint.x > Box.xRight ) {
            if ( ConvertedPoint.y < Box.yBottom ) {
               Type = SPTR_SIZENWSE ;
            } else if ( ConvertedPoint.y > Box.yTop ) {
               Type = SPTR_SIZENESW ;
            } else {
               Type = SPTR_SIZEWE ;
            } /* endif */
         } else if ( ConvertedPoint.y < Box.yBottom ) {
            if ( ConvertedPoint.x < Box.xLeft ) {
               Type = SPTR_SIZENWSE ;
            } else if ( ConvertedPoint.x > Box.xRight ) {
               Type = SPTR_SIZENESW ;
            } else {
               Type = SPTR_SIZENS ;
            } /* endif */
         } else if ( ConvertedPoint.y > Box.yTop ) {
            if ( ConvertedPoint.x < Box.xLeft ) {
               Type = SPTR_SIZENESW ;
            } else if ( ConvertedPoint.x > Box.xRight ) {
               Type = SPTR_SIZENWSE ;
            } else {
               Type = SPTR_SIZENS ;
            } /* endif */
         } /* endif */

      // Else, default to the i-bar text pointer.
      } else {
         Type = SPTR_TEXT ;

      } /* endif */

   } /* endif */

   // Set the pointer type determined above.
   HPOINTER Pointer = WinQuerySysPointer ( HWND_DESKTOP, Type, FALSE ) ;
   WinSetPointer ( HWND_DESKTOP, Pointer ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Query Selected Text Rectangle (in device units)                     *
 *                                                                          *
 ****************************************************************************/

STATIC MRESULT APIENTRY QueryTextRectangle ( HWND Window, MESG, MPARAM1 mp1, MPARAM2 ) {

   PDATA Data = PDATA ( Sys_GetWindowData ( Window ) ) ;

   RECTL Rectangle ;
   Data->pDocument->QueryTextRectangle ( Rectangle ) ;
   Data->pDocument->Transform ( CVTC_DEFAULTPAGE, CVTC_DEVICE, Rectangle ) ;

   memcpy ( PVOIDFROMMP(mp1), &Rectangle, sizeof(Rectangle) ) ;

   return ( 0 ) ;
}

/****************************************************************************
 *                                                                          *
 *      Set Window Language                                                 *
 *                                                                          *
 ****************************************************************************/

STATIC MRESULT APIENTRY SelectLanguage ( HWND Window, MESG, MPARAM1 mp1, MPARAM2 ) {

   PDATA Data = PDATA ( Sys_GetWindowData ( Window ) ) ;

   Data->Library = (LanguageModule*) mp1 ;

   ResourceString Printing ( Data->Library->QueryHandle(), IDS_PRINTING ) ;
   Sys_SetWindowText ( Data->PrintStatusWindow, PSZ(Printing) ) ;

   WinDestroyWindow ( Data->TextMenu ) ;
   Data->TextMenu = WinLoadMenu ( HWND_DESKTOP, Data->Library->QueryHandle(), IDM_TEXTMENU ) ;

   WinDestroyWindow ( Data->BitmapMenu ) ;
   Data->BitmapMenu = WinLoadMenu ( HWND_DESKTOP, Data->Library->QueryHandle(), IDM_BITMAPMENU ) ;

   WinDestroyWindow ( Data->BorderMenu ) ;
   Data->BorderMenu = WinLoadMenu ( HWND_DESKTOP, Data->Library->QueryHandle(), IDM_BORDERMENU ) ;

   WinDestroyWindow ( Data->NullMenu ) ;
   Data->NullMenu = WinLoadMenu ( HWND_DESKTOP, Data->Library->QueryHandle(), IDM_DEFAULTMENU ) ;

   MENUITEM MenuItem ;
   Sys_SendMessage ( Data->NullMenu, MM_QUERYITEM, MPFROM2SHORT(IDM_PRINT_ALL,TRUE), MPFROMP(&MenuItem) ) ;
   ULONG Style = WinQueryWindowULong ( MenuItem.hwndSubMenu, QWL_STYLE ) ;
   WinSetWindowULong ( MenuItem.hwndSubMenu, QWL_STYLE, Style | MS_CONDITIONALCASCADE ) ;

   return ( 0 ) ;
}
