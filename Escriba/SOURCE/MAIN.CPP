/******************************************************************* MAIN.CPP
 *                                                                          *
 *                        Main Window Processor                             *
 *                                                                          *
 ****************************************************************************/

#include "System.h"

#include <direct.h>
#include <io.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys\types.h>
#include <sys\stat.h>

#include "Addons.h"
#include "Crc.h"
#include "DateFmt.h"
#include "Debug.h"
#include "Defaults.h"
#include "Dialog.h"
#include "Dialog2.h"
#include "DocFrame.h"
#include "DocWin.h"
#include "Document.h"
#include "Escriba.h"
#include "External.h"
#include "Frame.h"
#include "HourGlas.h"
#include "Main.h"
#include "MenuBmp.h"
#include "Module.h"
#include "Mutex.h"
#include "Prefer.h"
#include "Printer.h"
#include "Process.h"
#include "Profile2.h"
#include "Register.h"
#include "ReString.h"
#include "Support.h"
#include "Thread.h"
#include "WrkSpace.h"

// #define DEBUG
// #define DEBUG_MESSAGES
// #define DEBUG_INITLANGUAGEMENU


#define THREADED

/****************************************************************************
 *                                                                          *
 *                     Definitions & Declarations                           *
 *                                                                          *
 ****************************************************************************/

  // Constants

#define MAX_DOCUMENTS 100


  // Macros

#define STATIC static


  // Types

typedef struct {
   HWND  Window ;
   Documento *pDocument ;
} DATAWINDOW, *PDATAWINDOW ;

typedef struct {

   Process     *Proc ;
   LanguageModule *Library ;
   Module      *Registration ;
   Profile2    *IniFile ;
   INIDATA     *IniData ;
   Dictionary  *Speller ;
   BOOL         Trap ;

   DeviceContext *pDevice ;

   USHORT       FontMenuIndex ;
   USHORT       SizeMenuIndex ;
   USHORT       ExpandMenuIndex ;
   USHORT       LineColorMenuIndex ;
   USHORT       LineWidthMenuIndex ;
   USHORT       FillColorMenuIndex ;
   USHORT       FillTypeMenuIndex ;
   USHORT       LineEndMenuIndex ;
   USHORT       LineJoinMenuIndex ;

   USHORT       WindowMenuSize ;

   BOOL         Closing ;
   USHORT       LoadsActive ;
   USHORT       ThreadsActive ;

   SHORT        ActiveWindow ;
   DATAWINDOW   DataWindows [ MAX_DOCUMENTS ] ;

   HWND         LoadStatusWindow ;
   HWND         ContextMenu ;

} DATA, *PDATA ;

typedef struct {
   LanguageModule *Library ;
   PSZ Filename ;
   HWND WindowToNotify ;
   DocumentParms DocParms ;
   ImportExport *Importer ;
} LOAD_PARMS, *PLOAD_PARMS ;


  // Function Prototypes

STATIC WINDOWPROC Create ;
STATIC WINDOWPROC Close ;
STATIC WINDOWPROC Destroy ;
STATIC WINDOWPROC Move ;
STATIC WINDOWPROC Size ;
STATIC WINDOWPROC MinMaxFrame ;
STATIC WINDOWPROC SaveApplication ;
STATIC WINDOWPROC Paint ;
STATIC WINDOWPROC PresParamChanged ;
STATIC WINDOWPROC InitMenu ;
STATIC WINDOWPROC Command ;
STATIC WINDOWPROC SendToDocument ;
STATIC WINDOWPROC Refresh ;
STATIC WINDOWPROC NextDocument ;
STATIC WINDOWPROC FileNew ;
STATIC WINDOWPROC FileOpen ;
STATIC WINDOWPROC FileRecall ;
STATIC WINDOWPROC Preferences ;
STATIC WINDOWPROC Defaults ;
STATIC WINDOWPROC DefaultsContext ;
STATIC WINDOWPROC ConfigureExtensions ;
STATIC WINDOWPROC SelectLanguage ;
STATIC WINDOWPROC ClearClipboard ;
STATIC WINDOWPROC HideOnScreenClip ;
STATIC WINDOWPROC FontsPageUp ;
STATIC WINDOWPROC FontsPageDown ;
STATIC WINDOWPROC FontManager ;
STATIC WINDOWPROC SizesPageUp ;
STATIC WINDOWPROC SizesPageDown ;
STATIC WINDOWPROC SizeManager ;
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
STATIC WINDOWPROC WindowTile ;
STATIC WINDOWPROC WindowCascade ;
STATIC WINDOWPROC WindowArrange ;
STATIC WINDOWPROC About ;
STATIC WINDOWPROC ContextMenu ;
STATIC WINDOWPROC PresParamChanged ;
STATIC WINDOWPROC SysColorChange ;
STATIC WINDOWPROC QueryKeysHelp ;
STATIC WINDOWPROC HelpError ;
STATIC WINDOWPROC ExtHelpUndefined ;
STATIC WINDOWPROC HelpSubitemNotFound ;
STATIC WINDOWPROC DragOver ;
STATIC WINDOWPROC DragLeave ;
STATIC WINDOWPROC Drop ;
STATIC WINDOWPROC LoadFile ;
STATIC WINDOWPROC LoadDone ;
STATIC WINDOWPROC ThreadStarted ;
STATIC WINDOWPROC ThreadDone ;
STATIC WINDOWPROC Activated ;
STATIC WINDOWPROC CloseMe ;
STATIC WINDOWPROC SetDocColors ;

STATIC VOID UpdateMenu ( HWND Window, PDATA Data ) ;

STATIC void InitRecallMenu ( HWND Window, PDATA Data, HWND Menu, int SubmenuID, int ListID ) ;

STATIC void InitLanguageMenu ( HWND Window, PDATA Data, HWND Menu, int SubmenuID, int ListID ) ;

STATIC void InitWindowMenu ( HWND Window, PDATA Data, HWND Menu, int SubmenuID, int ListID ) ;

STATIC int  LoadFiles ( HWND Window, char *InitialPath, int argc, char *argv[] ) ;

STATIC BOOL StartLoad ( PSZ Name, PDATA Data, HWND WindowToNotify, int FileTypeIndex ) ;

STATIC VOID LoadThread ( PVOID Parameter ) ;

STATIC void PositionLoadStatusWindow ( HWND Frame, HWND LoadStatusWindow ) ;


  // Static Data
 
static int LanguageCount = 0 ;
static char Languages [IDM_LANGUAGES_MAX-IDM_LANGUAGES+1] [80] ;
static char Libraries [IDM_LANGUAGES_MAX-IDM_LANGUAGES+1] [CCHMAXPATH] ;


/****************************************************************************
 *                                                                          *
 *      Window Message Processor                                            *
 *                                                                          *
 ****************************************************************************/

extern MRESULT EXPENTRY MessageProcessor ( HWND Window, MESG msg, MPARAM1 mp1, MPARAM2 mp2 ) {

  /**************************************************************************
   * Dispatch the message according to the method table and return the      *
   *   result.  Any messages not defined above get handled by the system    *
   *   default window processor.                                            *
   **************************************************************************/

   static METHOD Methods [] = {
      { WM_CREATE,                Create              },
      { WM_CLOSE,                 Close               },
      { WM_DESTROY,               Destroy             },
      { WM_MOVE,                  Move                },
      { WM_SIZE,                  Size                },
      { WM_MINMAXFRAME,           MinMaxFrame         },
      { WM_SAVEAPPLICATION,       SaveApplication     },
      { WM_PAINT,                 Paint               },
      { WM_INITMENU,              InitMenu            },
      { WM_PRESPARAMCHANGED,      PresParamChanged    },
      { WM_COMMAND,               Command             },
      { WM_CONTEXTMENU,           ContextMenu         },
      { WM_SYSCOLORCHANGE,        SysColorChange      },
      { HM_QUERY_KEYS_HELP,       QueryKeysHelp       },
      { HM_ERROR,                 HelpError           },
      { HM_EXT_HELP_UNDEFINED,    ExtHelpUndefined    },
      { HM_HELPSUBITEM_NOT_FOUND, HelpSubitemNotFound },
      { DM_DRAGOVER,              DragOver            },
      { DM_DRAGLEAVE,             DragLeave           },
      { DM_DROP,                  Drop                },
      { WM_LOAD_FILE,             LoadFile            },
      { WM_LOAD_DONE,             LoadDone            },
      { WM_THREAD_STARTED,        ThreadStarted       },
      { WM_THREAD_DONE,           ThreadDone          },
      { WM_ACTIVATED,             Activated           },
      { WM_CLOSE_ME,              CloseMe             },
      { WM_SET_DOCCOLORS,         SetDocColors        },
   } ;

   #ifdef DEBUG_MESSAGES
      static int Indent = 0 ;
      LogMsgRcvd ( Indent, "MAIN", msg, mp1, mp2 ) ;
      Indent += 2 ;
   #endif

   MRESULT Result = DispatchMessage ( Window, msg, mp1, mp2, Methods, sizeof(Methods)/sizeof(Methods[0]), Sys_DefWindowProc ) ;

   #ifdef DEBUG_MESSAGES
      Indent -= 2 ;
      LogMsgDone ( Indent, "MAIN", msg, Result ) ;
   #endif

   return ( Result ) ;
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

   #ifdef DEBUG
      Log ( "Main::Create: Allocating instance data." ) ;
   #endif

   PDATA Data = (PDATA) malloc ( sizeof(DATA) ) ;

   if ( Data == NULL ) 
      return ( MRFROMSHORT ( 1 ) ) ;

   Sys_SetWindowData ( Window, Data ) ;

  /**************************************************************************
   * Grab any parameters from the WM_CREATE message.                        *
   **************************************************************************/

   #ifdef DEBUG
      Log ( "Main::Create: Saving parameters." ) ;
   #endif

   PPARMS Parms = PPARMS ( PVOIDFROMMP ( mp1 ) ) ;

   Data->Library = Parms->Library ;
   Data->Registration = Parms->Registration ;
   Data->IniFile = Parms->IniFile ;
   Data->IniData = Parms->IniData ;
   Data->Speller = Parms->Speller ;
   Data->Trap = Parms->Trap ;

  /**************************************************************************
   * Create the device context object for the window.                       *
   **************************************************************************/

   #ifdef DEBUG
      Log ( "Main::Create: Creating device context object." ) ;
   #endif

   WinOpenWindowDC ( Window ) ;
   Data->pDevice = new DeviceContext ( "MainWindow", Window ) ;

  /**************************************************************************
   * If trapping, subclass the main window frame.                           *
   **************************************************************************/

   if ( Data->Trap ) {
      HWND Frame = PARENT ( Window ) ;
      WinSubclassWindow ( Frame, FrameMessageProcessor ) ;
   } /* endif */

  /**************************************************************************
   * Initialize instance data.                                              *
   **************************************************************************/

   #ifdef DEBUG
      Log ( "Main::Create: Initializing instance data." ) ;
   #endif

   Data->WindowMenuSize = 0 ;
   Data->Closing = FALSE ;
   Data->LoadsActive = 0 ;
   Data->ThreadsActive = 0 ;
   Data->ActiveWindow = -1 ;

   memset ( Data->DataWindows, 0, sizeof(Data->DataWindows) ) ;

  /**************************************************************************
   * Load the date formatting strings.                                      *
   **************************************************************************/

   #ifdef DEBUG
      Log ( "Main::Create: Loading data formatting strings." ) ;
   #endif

   LoadDateFormattingResources ( ) ;

  /**************************************************************************
   * Register the data frame class.                                         *
   **************************************************************************/

   #ifdef DEBUG
      Log ( "Main::Create: Registering data frame class." ) ;
   #endif

   CLASSINFO ClassInfo ;
   WinQueryClassInfo ( WinQueryAnchorBlock(Window), WC_FRAME, &ClassInfo ) ;
   ClassInfo.flClassStyle &= ~CS_PUBLIC ;

   if ( !WinRegisterClass ( WinQueryAnchorBlock(Window),
      PSZ(DOCFRAME_CLASS_NAME), DocFrameMessageProcessor,
      ClassInfo.flClassStyle, ClassInfo.cbWindowData ) ) {

      ResourceString Title ( Data->Library->QueryHandle(), IDS_TITLE ) ;
      ResourceString Format ( Data->Library->QueryHandle(), IDS_ERROR_WINREGISTERCLASS ) ;
      char Message [200] ;
      sprintf ( Message, PCHAR(Format), DOCFRAME_CLASS_NAME ) ;
      MessageBox ( HWND_DESKTOP, HWND_DESKTOP, PSZ(Message),
         PSZ(Title), IDD_ERROR_WINREGISTERCLASS, MB_ENTER | MB_ICONEXCLAMATION | MB_HELP, LibraryHandle, IDS_MESSAGE_BOX_STRINGS ) ;
      return ( MRFROMSHORT ( 1 ) ) ;

   } /* endif */

  /**************************************************************************
   * Register the data window class.                                        *
   **************************************************************************/

   #ifdef DEBUG
      Log ( "Main::Create: Registering data window class." ) ;
   #endif

   if ( !WinRegisterClass ( WinQueryAnchorBlock(Window),
      PSZ(DOCWINDOW_CLASS_NAME), DocWindowMessageProcessor,
      0, sizeof(PVOID) ) ) {

      ResourceString Title ( Data->Library->QueryHandle(), IDS_TITLE ) ;
      ResourceString Format ( Data->Library->QueryHandle(), IDS_ERROR_WINREGISTERCLASS ) ;
      char Message [200] ;
      sprintf ( Message, PCHAR(Format), DOCWINDOW_CLASS_NAME ) ;
      MessageBox ( HWND_DESKTOP, HWND_DESKTOP, PSZ(Message),
         PSZ(Title), IDD_ERROR_WINREGISTERCLASS, MB_ENTER | MB_ICONEXCLAMATION | MB_HELP, LibraryHandle, IDS_MESSAGE_BOX_STRINGS ) ;
      return ( MRFROMSHORT ( 1 ) ) ;

   } /* endif */

  /**************************************************************************
   * Get the frame handle.                                                  *
   **************************************************************************/

   HWND Frame = PARENT ( Window ) ;

  /**************************************************************************
   * Initialize the font menu.                                              *
   **************************************************************************/

   #ifdef DEBUG
      Log ( "Main::Create: Initializing the font menu." ) ;
   #endif

   Data->FontMenuIndex = 0 ;
   InitFontBitmaps ( Data->Library->QueryHandle(), Window ) ;

  /**************************************************************************
   * Initialize the size menu.                                              *
   **************************************************************************/

   #ifdef DEBUG
      Log ( "Main::Create: Initializing the size menu." ) ;
   #endif

   Data->SizeMenuIndex = 0 ;
   InitSizeBitmaps ( Data->Library->QueryHandle(), Window,
      PSZ(Data->IniData->DocParms.Font), Data->IniData->SizeCount, Data->IniData->Sizes ) ;

  /**************************************************************************
   * Initialize the attribute menu.                                         *
   **************************************************************************/

   #ifdef DEBUG
      Log ( "Main::Create: Initializing the attribute menu." ) ;
   #endif

   InitAttrBitmaps ( Data->Library->QueryHandle(), Window,
      PSZ(Data->IniData->DocParms.Font) ) ;

  /**************************************************************************
   * Initialize the expansion menu.                                         *
   **************************************************************************/

   #ifdef DEBUG
      Log ( "Main::Create: Initializing the expansion menu." ) ;
   #endif

   Data->ExpandMenuIndex = 0 ;
   InitExpandBitmaps ( Data->Library->QueryHandle(), Window,
      PSZ(Data->IniData->DocParms.Font), Data->IniData->ExpandCount, Data->IniData->Expansions ) ;

  /**************************************************************************
   * Initialize the line color menu.                                        *
   **************************************************************************/

   #ifdef DEBUG
      Log ( "Main::Create: Initializing the line color menu." ) ;
   #endif

   Data->LineColorMenuIndex = 0 ;
   InitLineColorBitmaps ( Data->Library->QueryHandle(), Window,
      Data->IniData->LineColorCount, Data->IniData->LineColors ) ;

  /**************************************************************************
   * Initialize the line width menu.                                        *
   **************************************************************************/

   #ifdef DEBUG
      Log ( "Main::Create: Initializing the line width menu." ) ;
   #endif

   Data->LineWidthMenuIndex = 0 ;
   InitLineWidthBitmaps ( Data->Library->QueryHandle(), Window,
      Data->IniData->LineWidthCount, Data->IniData->LineWidths ) ;

  /**************************************************************************
   * Initialize the fill color menu.                                        *
   **************************************************************************/

   #ifdef DEBUG
      Log ( "Main::Create: Initializing the fill color menu." ) ;
   #endif

   Data->FillColorMenuIndex = 0 ;
   InitFillColorBitmaps ( Data->Library->QueryHandle(), Window,
      Data->IniData->FillColorCount, Data->IniData->FillColors ) ;

  /**************************************************************************
   * Initialize the fill type menu.                                         *
   **************************************************************************/

   #ifdef DEBUG
      Log ( "Main::Create: Initializing the fill type menu." ) ;
   #endif

   Data->FillTypeMenuIndex = 0 ;
   InitFillTypeBitmaps ( Data->Library->QueryHandle(), Window,
      Data->IniData->FillTypeCount, Data->IniData->FillTypes ) ;

  /**************************************************************************
   * Initialize the line end menu.                                          *
   **************************************************************************/

   #ifdef DEBUG
      Log ( "Main::Create: Initializing the line end menu." ) ;
   #endif

   Data->LineEndMenuIndex = 0 ;
   InitLineEndBitmaps ( Data->Library->QueryHandle(), Window,
      Data->IniData->LineEndCount, Data->IniData->LineEnds ) ;

  /**************************************************************************
   * Initialize the line join menu.                                         *
   **************************************************************************/

   #ifdef DEBUG
      Log ( "Main::Create: Initializing the line join menu." ) ;
   #endif

   Data->LineJoinMenuIndex = 0 ;
   InitLineJoinBitmaps ( Data->Library->QueryHandle(), Window,
      Data->IniData->LineJoinCount, Data->IniData->LineJoins ) ;

  /**************************************************************************
   * If window hasn't been positioned before, set the default position.     *
   **************************************************************************/

   if ( NOT Data->IniData->fPosition ) {

      RECTL Rectangle ;
      WinQueryWindowRect ( HWND_DESKTOP, &Rectangle ) ;

      Rectangle.yBottom = Rectangle.yBottom + ( Rectangle.yTop - Rectangle.yBottom ) / 10 ;

      Data->IniData->Position.x = SHORT(Rectangle.xLeft) ;
      Data->IniData->Position.y = SHORT(Rectangle.yBottom) ;

      Data->IniData->Position.cx = SHORT( Rectangle.xRight - Rectangle.xLeft ) ;
      Data->IniData->Position.cy = SHORT( Rectangle.yTop - Rectangle.yBottom ) ;

   } /* endif */

  /**************************************************************************
   * Position & size the window.                                            *
   **************************************************************************/

   #ifdef DEBUG
      Log ( "Main::Create: Positioning window." ) ;
   #endif

   WinSetWindowPos ( Frame, 0,
      SHORT(Data->IniData->Position.x), SHORT(Data->IniData->Position.y),
      SHORT(Data->IniData->Position.cx), SHORT(Data->IniData->Position.cy),
      SWP_SIZE | SWP_MOVE | ( Data->Trap ? SWP_MAXIMIZE : (
      ( Data->IniData->Position.fl & SWP_MINIMIZE ) |
      ( Data->IniData->Position.fl & SWP_MAXIMIZE ) |
      ( Data->IniData->Position.fl & SWP_RESTORE ) ) ) ) ;

  /**************************************************************************
   * Add the program to the system task list.                               *
   **************************************************************************/
 
   #ifdef DEBUG
      Log ( "Main::Create: Adding application to system task list." ) ;
   #endif

   ResourceString Title ( Data->Library->QueryHandle(), IDS_TITLE ) ;
   Add2TaskList ( Frame, PSZ(Title) ) ;

  /**************************************************************************
   * Build the presentation parameters for the status windows.              *
   **************************************************************************/

   #ifdef DEBUG
      Log ( "Main::Create: Building presentation parameters for status windows." ) ;
   #endif

   COLOR FgndColor = RGB_WHITE ;
   COLOR BackColor = RGB_RED ;
   ResourceString StatusFont ( Data->Library->QueryHandle(), IDS_STATUSFONT ) ;
   ULONG Ids[] = { PP_FOREGROUNDCOLOR, PP_BACKGROUNDCOLOR, PP_FONTNAMESIZE } ;
   ULONG ByteCounts[] = { sizeof(FgndColor), sizeof(BackColor), strlen(PCHAR(StatusFont))+1 } ;
   PUCHAR Params[] = { PUCHAR(&FgndColor), PUCHAR(&BackColor), PUCHAR(StatusFont) } ;

   PPRESPARAMS PresParams = BuildPresParams ( sizeof(Ids)/sizeof(Ids[0]),
     Ids, ByteCounts, Params ) ;

  /**************************************************************************
   * Get the title-bar window handle and alter its style a bit.             *
   **************************************************************************/

   #ifdef DEBUG
      Log ( "Main::Create: Altering titlebar to clip on children." ) ;
   #endif

   HWND Titlebar = CHILD ( Frame, FID_TITLEBAR ) ;

   ULONG TitleStyle = WinQueryWindowULong ( Titlebar, QWL_STYLE ) ;
   TitleStyle |= WS_CLIPCHILDREN ;
   WinSetWindowULong ( Titlebar, QWL_STYLE, TitleStyle ) ;

   SWP Position ;
   WinQueryWindowPos ( Titlebar, &Position ) ;

  /**************************************************************************
   * Create the background load status display window.                      *
   **************************************************************************/

   #ifdef DEBUG
      Log ( "Main::Create: Creating load status window." ) ;
   #endif

   ResourceString Loading ( Data->Library->QueryHandle(), IDS_LOADING ) ;

   Data->LoadStatusWindow = WinCreateWindow ( Titlebar, WC_STATIC, PSZ(Loading),
      WS_CLIPCHILDREN | WS_CLIPSIBLINGS | SS_TEXT | DT_CENTER,
      0, 0, 0, 0, Titlebar, HWND_TOP, 0, NULL, PresParams ) ;

   if ( Data->LoadStatusWindow == 0 ) {
      ResourceString Title ( Data->Library->QueryHandle(), IDS_TITLE ) ;
      ResourceString Message ( Data->Library->QueryHandle(), IDS_ERROR_CREATELOADSTATUS ) ;
      MessageBox ( HWND_DESKTOP, HWND_DESKTOP, PSZ(Message),
         PSZ(Title), IDD_ERROR_CREATELOADSTATUS, MB_ENTER | MB_ICONEXCLAMATION | MB_HELP, LibraryHandle, IDS_MESSAGE_BOX_STRINGS ) ;
   } /* endif */

  /**************************************************************************
   * Release the presentation parameter block memory, now that we're done.  *
   **************************************************************************/

   #ifdef DEBUG
      Log ( "Main::Create: Releasing memory allocated for presentation parameters." ) ;
   #endif

   free ( PresParams ) ;

  /**************************************************************************
   * If no initial documents, create a new one.  Otherwise, start threads   *
   *   to load them.                                                        *
   **************************************************************************/

   #ifdef DEBUG
      Log ( "Main::Create: Processing command-line." ) ;
   #endif

   if ( LoadFiles ( Window, Parms->InitialPath, Parms->argc, Parms->argv ) == 0 ) {
      if ( Parms->IniData->LoadMostRecent ) {
         for ( int i=0; i<MAX_RECALL; i++ ) {
            if ( Parms->IniData->Files[i] && Parms->IniData->Files[i][0] && StartLoad ( PSZ(Parms->IniData->Files[i]), Data, Window, 0 ) )
               break ;
         } /* endfor */
         if ( i >= MAX_RECALL ) {
            #ifdef DEBUG 
               Log ( "Main::Create: Creating new file because none in the recall list could be found." ) ;
            #endif // DEBUG
            FileNew ( Window, 0, 0, 0 ) ;
         } /* endif */
      } else {
         #ifdef DEBUG 
            Log ( "Main::Create: Creating new file because nothing on the command-line could be found." ) ;
         #endif // DEBUG
         FileNew ( Window, 0, 0, 0 ) ;
      } /* endif */
   } /* endif */

  /**************************************************************************
   * Load the context menu.                                                 *
   **************************************************************************/

   #ifdef DEBUG
      Log ( "Main::Create: Loading the context menu." ) ;
   #endif

   Data->ContextMenu = WinLoadMenu ( HWND_DESKTOP, Data->Library->QueryHandle(), IDM_MAIN_CONTEXT ) ;

  /**************************************************************************
   * Enable/disable the different menu options appropriately.               *
   **************************************************************************/

   #ifdef DEBUG
      Log ( "Main::Create: Updating menu options." ) ;
   #endif

   UpdateMenu ( Window, Data ) ;

  /**************************************************************************
   * Now that the window's in order, make it visible.                       *
   **************************************************************************/

   #ifdef DEBUG
      Log ( "Main::Create: Showing window." ) ;
   #endif

   Sys_ShowWindow ( Frame, TRUE ) ;

  /**************************************************************************
   * And activate it.                                                       *
   **************************************************************************/

   #ifdef DEBUG
      Log ( "Main::Create: Setting active window." ) ;
   #endif

   Sys_SetActiveWindow ( Window ) ;

  /**************************************************************************
   * Success?  Return no error.                                             *
   **************************************************************************/

   #ifdef DEBUG
      Log ( "Main::Create: Done." ) ;
   #endif

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
   * Close each of the data windows.  Stop if they object.                  *
   **************************************************************************/

   for ( int i=0; i<MAX_DOCUMENTS; i++ ) {

      // Get window handle.  If this entry is unused, just skip it.
      HWND Window = Data->DataWindows[i].Window ;
      if ( Window == 0 )
         continue ;

      // Ask the document to destroy its window.  Abort shutdown if denied.
      Documento *pDocument = Data->DataWindows[i].pDocument ;
      if ( ! pDocument->DestroyWindow ( Window, FALSE ) )
         return ( 0 ) ;
      Data->DataWindows[i].Window = 0 ;

      // Update the file recall list.
      PSZ Name = pDocument->Query_Filename() ;
      if ( Name AND *Name )
         Data->IniData->UpdateFileList ( Name ) ;

      // Destroy the document.
      delete pDocument ;
      Data->DataWindows[i].pDocument = 0 ;

   } /* endfor */

  /**************************************************************************
   * Tell the bitmap threads to die.                                        *
   **************************************************************************/

   CancelBitmapThreads ( ) ;

  /**************************************************************************
   * If things are still going on, remember that we're to shut down, but    *
   *   don't shut down right now.  Do hide away though.                     *
   **************************************************************************/

   if ( Data->LoadsActive OR Data->ThreadsActive ) {
      Data->Closing = TRUE ;
      HWND Frame = PARENT ( Window ) ;
      Sys_ShowWindow ( Frame, FALSE ) ;
      WinSetWindowPos ( Frame, 0, 0, 0, 0, 0, SWP_DEACTIVATE ) ;
      Sys_EnableWindow ( Frame, FALSE ) ;
      return ( 0 ) ;
   } /* endif */

  /**************************************************************************
   * Call function to put all profile data out to the system.               *
   **************************************************************************/

   Data->IniFile->PutIniData ( *Data->IniData ) ;

  /**************************************************************************
   * Post WM_QUIT message to main window back to self to end program.       *
   **************************************************************************/

   Sys_PostMessage ( Window, WM_QUIT, 0, 0 ) ;

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
   * Destroy each of the remaining data windows.                            *
   **************************************************************************/

   for ( int i=0; i<MAX_DOCUMENTS; i++ ) {

      // Get window handle.  If this entry is unused, just skip it.
      HWND Window = Data->DataWindows[i].Window ;
      if ( Window == 0 )
         continue ;

      // Destroy the document window.
      Documento *pDocument = Data->DataWindows[i].pDocument ;
      pDocument->DestroyWindow ( 0, TRUE ) ;
      Data->DataWindows[i].Window = 0 ;

      // Update the file recall list.
      PSZ Name = pDocument->Query_Filename() ;
      if ( Name AND *Name )
         Data->IniData->UpdateFileList ( Name ) ;

      // Destroy the document.
      delete pDocument ;
      Data->DataWindows[i].pDocument = 0 ;

   } /* endfor */

  /**************************************************************************
   * Release the bitmap resources.                                          *
   **************************************************************************/

   DestroyBitmaps ( ) ;

  /**************************************************************************
   * Unload the date formatting strings.                                    *
   **************************************************************************/

   UnloadDateFormattingResources ( ) ;

  /**************************************************************************
   * Destroy the device context object for the window.                      *
   **************************************************************************/

   delete Data->pDevice ;

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
 *      Move the main window.                                               *
 *                                                                          *
 ****************************************************************************/

STATIC MRESULT APIENTRY Move ( HWND Window, MESG, MPARAM1, MPARAM2 ) {

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

   if ( NOT ( Position.fl & SWP_MINIMIZE ) AND NOT ( Position.fl & SWP_MAXIMIZE ) ) {
      Data->IniData->Position.x = Position.x ;
      Data->IniData->Position.y = Position.y ;
      Data->IniData->Position.cx = Position.cx ;
      Data->IniData->Position.cy = Position.cy ;
   } /* endif */

   Data->IniData->Position.fl = Position.fl ;

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

   if ( NOT ( Position.fl & SWP_MINIMIZE ) AND NOT ( Position.fl & SWP_MAXIMIZE ) ) {
      Data->IniData->Position.x = Position.x ;
      Data->IniData->Position.y = Position.y ;
      Data->IniData->Position.cx = Position.cx ;
      Data->IniData->Position.cy = Position.cy ;
   } /* endif */

   Data->IniData->Position.fl = Position.fl ;

  /**************************************************************************
   * Relocate the load status display window.                               *
   **************************************************************************/

   PositionLoadStatusWindow ( Frame, Data->LoadStatusWindow ) ;

  /**************************************************************************
   * If any of the data windows is maximized, restore it and remaximize it. *
   **************************************************************************/

   WinLockWindowUpdate ( HWND_DESKTOP, Window ) ;

   WindowArrange ( Window, 0, 0, 0 ) ;

   for ( int i=0; i<MAX_DOCUMENTS; i++ ) {
      SWP Position ;
      HWND Window = Data->DataWindows[i].Window ;
      if ( Window ) {
         WinQueryWindowPos ( Window, &Position ) ;
         if ( Position.fl & SWP_MAXIMIZE ) {
            WinSetWindowPos ( Window, 0, 0, 0, 0, 0, SWP_RESTORE ) ;
            WinQueryWindowPos ( Window, &Position ) ;
            WinSetWindowPos ( Window, 0,
               Position.y, Position.x, Position.cx, Position.cy,
               SWP_MOVE | SWP_SIZE | SWP_MAXIMIZE ) ;
         } /* endif */
      } /* endif */
   } /* endfor */

   WinLockWindowUpdate ( HWND_DESKTOP, 0 ) ;

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
   * Set appropriate window text.                                           *
   **************************************************************************/

   PSWP Position = PSWP ( PVOIDFROMMP ( mp1 ) ) ;

   ResourceString Title ( Data->Library->QueryHandle(), Position->fl & SWP_MINIMIZE ? IDS_TITLE1 : IDS_TITLE ) ;

   HWND Frame = PARENT ( Window ) ;
   Sys_SetWindowText ( Frame, PSZ(Title) ) ;

  /**************************************************************************
   * We're done.                                                            *
   **************************************************************************/

   return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process SAVE APPLICATION message.                                   *
 *                                                                          *
 ****************************************************************************/

STATIC MRESULT APIENTRY SaveApplication ( HWND Window, MESG, MPARAM1, MPARAM2 ) {

  /**************************************************************************
   * Find the instance data.                                                *
   **************************************************************************/

   PDATA Data = PDATA ( Sys_GetWindowData ( Window ) ) ;

  /**************************************************************************
   * Call function to put all profile data out to the system.               *
   **************************************************************************/

   Data->IniFile->PutIniData ( *Data->IniData ) ;

  /**************************************************************************
   * We're done.  Let the system complete default processing.               *
   **************************************************************************/

   return ( Sys_DefWindowProc ( Window, WM_SAVEAPPLICATION, 0, 0 ) ) ;
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
   * Get presentation space.                                                *
   **************************************************************************/

   RECTL Rectangle ;
   WorkSpace PS ( "Main::Paint", 0, Window, Data->pDevice, Rectangle ) ;

  /**************************************************************************
   * Clear the window.                                                      *
   **************************************************************************/

   COLOR Color = Data->IniData->MainColors[0] ;
   if ( Data->IniData->fMainColors[0] == FALSE ) {
      Color = WinQuerySysColor ( HWND_DESKTOP, SYSCLR_APPWORKSPACE, 0 ) ;
   } /* endif */

   PS.SetColor ( Color ) ;
   PS.FillBox ( Rectangle ) ;

  /**************************************************************************
   * We're done.                                                            *
   **************************************************************************/

   return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Initialize Menu                                                     *
 *                                                                          *
 ****************************************************************************/

STATIC MRESULT APIENTRY InitMenu ( HWND Window, MESG, MPARAM1 mp1, MPARAM2 mp2 ) {

  /**************************************************************************
   * Find the instance data.                                                *
   **************************************************************************/

   PDATA Data = PDATA ( Sys_GetWindowData ( Window ) ) ;

  /**************************************************************************
   * Get the message data.                                                  *
   **************************************************************************/

   int MenuID = SHORT1FROMMP ( mp1 ) ;
   HWND Menu = HWNDFROMMP ( mp2 ) ;

  /**************************************************************************
   * Determine the state of the current window, if there is one.            *
   **************************************************************************/

   Documento *pDocument = 0 ;
   PSZ Name = 0 ;
   BOOL Changed = FALSE ;
   BOOL HasSelection = FALSE ;
   if ( Data->ActiveWindow != -1 ) {
      pDocument = Data->DataWindows[Data->ActiveWindow].pDocument ;
      Name = pDocument->Query_Filename() ;
      Changed = pDocument->IsChanged() ;
      HasSelection = pDocument->HasSelection() ;
   } /* endif */

   DocumentParms DocParms ;
   Estado State ;
   USHORT ZoomCommand ;
   FIXED fxZoom ;

   if ( Data->ActiveWindow != -1 ) {
      DocParms = *pDocument->Query_Parms() ;
      Sys_SendMessage ( Data->DataWindows[Data->ActiveWindow].Window, WM_QUERY_STATE, MPFROMP(&State), 0 ) ;
      ZoomCommand = SHORT1FROMMR ( Sys_SendMessage ( Data->DataWindows[Data->ActiveWindow].Window, WM_QUERY_ZOOM, MPFROMP(&fxZoom), 0 ) ) ;

   } else {
      DocParms = Data->IniData->DocParms ;
      WorkSpace PS ( "Main::Paint", 0, Window, Data->pDevice, Data->IniData->DocParms.Metric ) ;
      State.Reset ( &PS, Data->IniData->DocParms, 1, 1, time(0) ) ;
      ZoomCommand = IDM_ZOOM_100 ;

   } /* endif */

  /**************************************************************************
   * Process according to which menu's about to be displayed.               *
   **************************************************************************/

   switch ( MenuID ) {

      case IDM_FILE_MENU: {

         Data->IniData->FileListLock.Request ( ) ;
         for ( int i=0; i<MAX_RECALL; i++ )
            if ( Data->IniData->Files[i] && Data->IniData->Files[i][0] )
               break ;
         Data->IniData->FileListLock.Release ( ) ;

         ULONG cTotal, cReturned, cbNeeded ;
         SplEnumQueue( 0, 3, 0, 0, &cReturned, &cTotal, &cbNeeded, NULL ) ;

         EnableMenuItem ( Menu, IDM_RECALL,                ( i < MAX_RECALL ) ) ;
         EnableMenuItem ( Menu, IDM_MERGE,                 pDocument != NULL ) ;
         EnableMenuItem ( Menu, IDM_SAVE,                  pDocument AND Name AND *Name AND Changed ) ;
         EnableMenuItem ( Menu, IDM_SAVEAS,                pDocument != NULL ) ;
         EnableMenuItem ( Menu, IDM_SAVE_CLOSE,            pDocument != NULL ) ;
         EnableMenuItem ( Menu, IDM_CLOSE,                 pDocument != NULL ) ;
         EnableMenuItem ( Menu, IDM_PRINT,                 pDocument AND cTotal ) ;
         EnableMenuItem ( Menu, IDM_CONFIGURE_EXTENSIONS,  AddonList.QueryConfigureCount() ) ;

         break ; }

      case IDM_RECALL: {
         InitRecallMenu ( Window, Data, Menu, IDM_RECALL, IDM_RECALL0 ) ;
         break ; }

      case IDM_PRINT: {
         // Ensure that the Print All item is a conditional cascade menu.
         MENUITEM MenuItem ;
         Sys_SendMessage ( Menu, MM_QUERYITEM, MPFROM2SHORT(IDM_PRINT_ALL,TRUE), MPFROMP(&MenuItem) ) ;
         ULONG Style = WinQueryWindowULong ( MenuItem.hwndSubMenu, QWL_STYLE ) ;
         WinSetWindowULong ( MenuItem.hwndSubMenu, QWL_STYLE, Style | MS_CONDITIONALCASCADE ) ;
         // Set the default item in the Print All menu.
         Sys_SendMessage ( MenuItem.hwndSubMenu, MM_SETDEFAULTITEMID,
            MPFROMLONG(Data->IniData->PrintReverse?IDM_PRINT_ALL_REVERSE:IDM_PRINT_ALL_NORMAL), 0 ) ;
         break ; }

      case IDM_PRINT_ALL: {
         // Reset any checks in the menu.
         CheckMenuItem ( Menu, IDM_PRINT_ALL_NORMAL, FALSE ) ;
         CheckMenuItem ( Menu, IDM_PRINT_ALL_REVERSE, FALSE ) ;
         // Set the default item in the Print All menu.
         Sys_SendMessage ( Menu, MM_SETDEFAULTITEMID,
            MPFROMLONG(Data->IniData->PrintReverse?IDM_PRINT_ALL_REVERSE:IDM_PRINT_ALL_NORMAL), 0 ) ;
         break ; }

      case IDM_LANGUAGE_MENU: {
         InitLanguageMenu ( Window, Data, Menu, IDM_LANGUAGE_MENU, IDM_LANGUAGES ) ;
         break ; }

      case IDM_EDIT_MENU: {
         EnableMenuItem ( Menu, IDM_UNDO, pDocument AND pDocument->Query_CurrentLevel() ) ;
         EnableMenuItem ( Menu, IDM_REDO, pDocument AND ( pDocument->Query_CurrentLevel() < pDocument->Query_MaxLevel() ) ) ;

         EnableMenuItem ( Menu, IDM_CUT,   pDocument AND HasSelection ) ;
         EnableMenuItem ( Menu, IDM_COPY,  pDocument AND HasSelection ) ;
         EnableMenuItem ( Menu, IDM_CLEAR, pDocument AND HasSelection ) ;
         EnableMenuItem ( Menu, IDM_SELECT_ALL, pDocument != 0 ) ;

         ULONG FormatInfo ;
         BOOL HasClippedText = WinQueryClipbrdFmtInfo ( WinQueryAnchorBlock(Window), CF_TEXT, &FormatInfo ) ;
         BOOL HasClippedBitmap = WinQueryClipbrdFmtInfo ( WinQueryAnchorBlock(Window), CF_BITMAP, &FormatInfo ) ;
         BOOL HasClippedMetafile = WinQueryClipbrdFmtInfo ( WinQueryAnchorBlock(Window), CF_METAFILE, &FormatInfo ) ;

         EnableMenuItem ( Menu, IDM_PASTE, HasClippedText ) ;
         EnableMenuItem ( Menu, IDM_PASTE_FROM_CLIP, HasClippedBitmap OR HasClippedMetafile ) ;
         EnableMenuItem ( Menu, IDM_PASTE_FROM_CLIP_BITMAP, HasClippedBitmap ) ;
         EnableMenuItem ( Menu, IDM_PASTE_FROM_CLIP_METAFILE, HasClippedMetafile ) ;
         CheckMenuItem  ( Menu, IDM_PASTE_FROM_SCRN_HIDE, Data->IniData->HideOnScreenClip ) ;
         EnableMenuItem ( Menu, IDM_PASTE_FROM_FILE, BOOL ( (GraphicImporter*) AddonList.FindFirstOfType ( TYPE_GRAPHICIMPORT ) != 0 ) ) ;
         EnableMenuItem ( Menu, IDM_CLEARCLIPBOARD, HasClippedText OR HasClippedBitmap OR HasClippedMetafile ) ;
         EnableMenuItem ( Menu, IDM_FINDNEXT, *Data->IniData->Search_String ) ;
         EnableMenuItem ( Menu, IDM_SPELL, Data->Speller && Data->Speller->Ready() ) ;

         break ; }

      case IDM_ZOOM: {
         CheckMenuItem ( Menu, IDM_ZOOM_100,        ZoomCommand == IDM_ZOOM_100        ) ;
         CheckMenuItem ( Menu, IDM_ZOOM_FULLWIDTH,  ZoomCommand == IDM_ZOOM_FULLWIDTH  ) ;
         CheckMenuItem ( Menu, IDM_ZOOM_FULLHEIGHT, ZoomCommand == IDM_ZOOM_FULLHEIGHT ) ;
         CheckMenuItem ( Menu, IDM_ZOOM_MARGINS,    ZoomCommand == IDM_ZOOM_MARGINS    ) ;
         CheckMenuItem ( Menu, IDM_ZOOM_ALL,        ZoomCommand == IDM_ZOOM_ALL        ) ;
         CheckMenuItem ( Menu, IDM_ZOOM_WINDOW,     ZoomCommand == IDM_ZOOM_WINDOW     ) ;
         CheckMenuItem ( Menu, IDM_ZOOM_SET,        ZoomCommand == IDM_ZOOM_SET        ) ;
         break ; }

      case IDM_LAYOUT_MENU: {
         CheckMenuItem ( Menu, IDM_LINESPACING_MENU, State.LineSpacing != DocParms.LineSpacing ) ;
         CheckMenuItem ( Menu, IDM_JUSTIFICATION, State.Justify != DocParms.Justify ) ;
         CheckMenuItem ( Menu, IDM_PAGECENTER, State.CenterPage ) ;
         break ; }

      case IDM_LINESPACING_MENU: {
         // Clear old menu out.
         LONG OldCount = LONGFROMMR ( Sys_SendMessage ( Menu, MM_QUERYITEMCOUNT, 0, 0 ) ) ;
         for ( int i=0; i<OldCount; i++ ) {
            LONG Id = LONGFROMMR ( Sys_SendMessage ( Menu, MM_ITEMIDFROMPOSITION, MPFROMSHORT(0), 0 ) ) ;
            Sys_SendMessage ( Menu, MM_REMOVEITEM, MPFROM2SHORT(Id,TRUE), 0 ) ;
         } /* endfor */
         // Prepare the basic menu item.
         MENUITEM MenuItem ;
         MenuItem.iPosition = MIT_END ;
         MenuItem.afAttribute = 0 ;
         MenuItem.afStyle = MIS_TEXT ;
         MenuItem.hwndSubMenu = 0 ;
         MenuItem.hItem = 0 ;
         MenuItem.id = USHORT ( IDM_LINESPACINGS ) ;
         // Add the line spacing entries.
         for ( i=0; i<Data->IniData->SpacingCount; i++ ) {
            char Label [20] ;
            sprintf ( Label, "%i%%", Data->IniData->Spacings[i] ) ;
            Sys_SendMessage ( Menu, MM_INSERTITEM, MPFROMP(&MenuItem), MPFROMP(Label) ) ;
            if ( State.LineSpacing == Data->IniData->Spacings[i] ) {
               Sys_SendMessage ( Menu, MM_SETITEMATTR, MPFROM2SHORT(MenuItem.id,TRUE), MPFROM2SHORT(MIA_CHECKED,MIA_CHECKED) ) ;
            } /* endif */
            MenuItem.id ++ ;
         } /* endfor */
         // Add a separator item.
         MenuItem.iPosition = MIT_END ;
         MenuItem.afAttribute = 0 ;
         MenuItem.hwndSubMenu = 0 ;
         MenuItem.id = 0 ;
         MenuItem.afStyle = MIS_SEPARATOR ;
         MenuItem.hItem = 0 ;
         Sys_SendMessage ( Menu, MM_INSERTITEM, MPFROMP(&MenuItem), MPFROMP("") ) ;
         // Add a manager item.
         ResourceString Manager ( Data->Library->QueryHandle(), IDS_MANAGER ) ;
         MenuItem.iPosition = MIT_END ;
         MenuItem.afAttribute = 0 ;
         MenuItem.hwndSubMenu = 0 ;
         MenuItem.id = IDM_LINESPACINGMGR ;
         MenuItem.afStyle = MIS_TEXT ;
         MenuItem.hItem = 0 ;
         Sys_SendMessage ( Menu, MM_INSERTITEM, MPFROMP(&MenuItem), MPFROMP(PSZ(Manager)) ) ;
         break ; }

      case IDM_JUSTIFICATION: {
         CheckMenuItem ( Menu, IDM_JUST_LEFT,      State.Justify == JUSTIFY_LEFT ) ;
         CheckMenuItem ( Menu, IDM_JUST_CENTER,    State.Justify == JUSTIFY_CENTER ) ;
         CheckMenuItem ( Menu, IDM_JUST_RIGHT,     State.Justify == JUSTIFY_RIGHT ) ;
         CheckMenuItem ( Menu, IDM_JUST_FULL,      State.Justify == JUSTIFY_FULL ) ;
         break ; }

      case IDM_FONT_MENU: {
         InitFontMenu ( Menu, State, Data->Library->QueryHandle(), Data->FontMenuIndex ) ;
         break ; }

      case IDM_SIZE_MENU: {
         InitSizeMenu ( Menu, State, Data->Library->QueryHandle(),
            Data->SizeMenuIndex, Data->IniData->SizeCount, Data->IniData->Sizes ) ;
         break ; }

      case IDM_ATTR_MENU: {
         InitAttrMenu ( Menu, State, DocParms, Data->Library->QueryHandle() ) ;
         break ; }

      case IDM_EXPAND_MENU: {
         InitExpandMenu ( Menu, State, Data->Library->QueryHandle(),
            Data->ExpandMenuIndex, Data->IniData->ExpandCount, Data->IniData->Expansions ) ;
         break ; }

      case IDM_LINECOLOR_MENU: {
         InitLineColorMenu ( Menu, State, Data->Library->QueryHandle(),
            Data->LineColorMenuIndex, Data->IniData->LineColorCount, Data->IniData->LineColors ) ;
         break ; }

      case IDM_LINEWIDTH_MENU: {
         InitLineWidthMenu ( Menu, State, Data->Library->QueryHandle(),
            Data->LineWidthMenuIndex, Data->IniData->LineWidthCount, Data->IniData->LineWidths ) ;
         break ; }

      case IDM_FILLCOLOR_MENU: {
         InitFillColorMenu ( Menu, State, Data->Library->QueryHandle(),
            Data->FillColorMenuIndex, Data->IniData->FillColorCount, Data->IniData->FillColors ) ;
         break ; }

      case IDM_FILLTYPE_MENU: {
         InitFillTypeMenu ( Menu, State, Data->Library->QueryHandle(),
            Data->FillTypeMenuIndex, Data->IniData->FillTypeCount, Data->IniData->FillTypes ) ;
         break ; }

      case IDM_LINEEND_MENU: {
         InitLineEndMenu ( Menu, State, Data->Library->QueryHandle(),
            Data->LineEndMenuIndex, Data->IniData->LineEndCount, Data->IniData->LineEnds ) ;
         break ; }

      case IDM_LINEJOIN_MENU: {
         InitLineJoinMenu ( Menu, State, Data->Library->QueryHandle(),
            Data->LineJoinMenuIndex, Data->IniData->LineJoinCount, Data->IniData->LineJoins ) ;
         break ; }

      case IDM_WINDOW_MENU: {
         InitWindowMenu ( Window, Data, Menu, IDM_WINDOW_MENU, IDM_WINDOWS ) ;
         break ; }

      case IDM_HELP_MENU: {
         MENUITEM MenuItem ;
         Sys_SendMessage ( Menu, MM_QUERYITEM, MPFROM2SHORT(IDM_ABOUT_MENU,TRUE), MPFROMP(&MenuItem) ) ;
         ULONG Style = WinQueryWindowULong ( MenuItem.hwndSubMenu, QWL_STYLE ) ;
         WinSetWindowULong ( MenuItem.hwndSubMenu, QWL_STYLE, Style | MS_CONDITIONALCASCADE ) ;
         break ; }

      case IDM_ABOUT_MENU: {
         LONG OldCount = LONGFROMMR ( Sys_SendMessage ( Menu, MM_QUERYITEMCOUNT, 0, 0 ) ) ;
         for ( int i=1; i<OldCount; i++ ) {
            LONG Id = LONGFROMMR ( Sys_SendMessage ( Menu, MM_ITEMIDFROMPOSITION, MPFROMSHORT(1), 0 ) ) ;
            Sys_SendMessage ( Menu, MM_REMOVEITEM, MPFROM2SHORT(Id,TRUE), 0 ) ;
         } /* endfor */
         AddonList.AddToMenu ( Menu ) ;
         break ; }

   } /* endswitch */

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

   BOOL Changed = FALSE ;
   ULONG ppid ;

   switch ( LONGFROMMP(mp1) ) {

     /***********************************************************************
      * If font, note the fact that we now have a font to be saved as       *
      *   part of the configuration.  Apply the font to all children.       *
      ***********************************************************************/

      case PP_FONTNAMESIZE: {
         ResourceString DefaultFont ( Data->Library->QueryHandle(), IDS_DEFAULTFONT ) ;
         char Font [80] ;
         if ( !WinQueryPresParam ( Window, PP_FONTNAMESIZE, 0, &ppid, sizeof(Font), PSZ(Font), 0 ) )
            strcpy ( Font, PCHAR(DefaultFont) ) ;
         if ( strcmpi ( PCHAR(Data->IniData->RulerFont), Font ) ) {
            strcpy ( PCHAR(Data->IniData->RulerFont), Font ) ;
            Changed = TRUE ;
         } /* endif */
         if ( strcmpi ( Font, PCHAR(DefaultFont) ) ) {
            Data->IniData->fRulerFont = TRUE ;
         } else {
            Data->IniData->fRulerFont = FALSE ;
         } /* endif */
         if ( Changed ) {
            for ( int i=0; i<MAX_DOCUMENTS; i++ ) {
               HWND Window = Data->DataWindows[i].Window ;
               if ( Window ) {
                  WinSetPresParam ( Window, PP_FONTNAMESIZE, strlen(Font)+1, PSZ(Font) ) ;
               } /* endif */
            } /* endfor */
         } /* endif */
         break ; }

     /***********************************************************************
      * If background color, note the fact and repaint the window.          *
      ***********************************************************************/

      case PP_BACKGROUNDCOLOR: {
         COLOR Default = WinQuerySysColor ( HWND_DESKTOP, SYSCLR_APPWORKSPACE, 0 ) ;
         COLOR Color ;
         if ( !WinQueryPresParam ( Window, PP_BACKGROUNDCOLOR, 0, &ppid, sizeof(Color), &Color, 0 ) )
            Color = Default ;
         if ( Color != Data->IniData->MainColors[0] ) {
            Data->IniData->MainColors[0] = Color ;
            Changed = TRUE ;
         } /* endif */
         if ( Color != Default ) {
            Data->IniData->fMainColors[0] = TRUE ;
         } else {
            Data->IniData->fMainColors[0] = FALSE ;
         } /* endif */
         if ( Changed ) {
            Sys_InvalidateWindow ( Window ) ;
         } /* endif */
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
   * Process recall messages.                                               *
   **************************************************************************/

   if ( ( Command >= IDM_RECALL0 ) AND ( Command <= IDM_RECALLMAX ) )
      return ( FileRecall ( Window, msg, mp1, mp2 ) ) ;

  /**************************************************************************
   * Process language selection messages.                                   *
   **************************************************************************/

   if ( ( Command >= IDM_LANGUAGES ) AND ( Command <= IDM_LANGUAGES_MAX ) )
      return ( SelectLanguage ( Window, msg, mp1, mp2 ) ) ;

  /**************************************************************************
   * Process spacing selection messages.                                    *
   **************************************************************************/

   if ( ( Command >= IDM_LINESPACINGS ) AND ( Command <= IDM_LINESPACINGS_MAX ) ) {
      Sys_PostMessage ( Data->DataWindows[Data->ActiveWindow].Window, WM_SET_LINESPACING,
         MPFROMSHORT(Data->IniData->Spacings[Command-IDM_LINESPACINGS]), 0 ) ;
      return ( 0 ) ;
   } /* endif */

  /**************************************************************************
   * Process font selection messages.                                       *
   **************************************************************************/

   if ( ( Command >= IDM_FONTS ) AND ( Command <= IDM_FONTS_MAX ) ) {
      Sys_PostMessage ( Data->DataWindows[Data->ActiveWindow].Window, WM_SET_FONT,
         MPFROMP((QueryFontMetrics())[Command-IDM_FONTS].szFacename), 0 ) ;
      return ( 0 ) ;
   } /* endif */

  /**************************************************************************
   * Process size selection messages.                                       *
   **************************************************************************/

   if ( ( Command >= IDM_SIZES ) AND ( Command <= IDM_SIZES_MAX ) ) {
      Sys_PostMessage ( Data->DataWindows[Data->ActiveWindow].Window, WM_SET_SIZE,
         MPFROMSHORT(Data->IniData->Sizes[Command-IDM_SIZES]), 0 ) ;
      return ( 0 ) ;
   } /* endif */

  /**************************************************************************
   * Process expansion selection messages.                                  *
   **************************************************************************/

   if ( ( Command >= IDM_EXPANDS ) AND ( Command <= IDM_EXPANDS_MAX ) ) {
      Sys_PostMessage ( Data->DataWindows[Data->ActiveWindow].Window, WM_SET_EXPANSION,
         MPFROMSHORT(Data->IniData->Expansions[Command-IDM_EXPANDS]), 0 ) ;
      return ( 0 ) ;
   } /* endif */

  /**************************************************************************
   * Process line color selection messages.                                 *
   **************************************************************************/

   if ( ( Command >= IDM_LINECOLORS ) AND ( Command <= IDM_LINECOLORS_MAX ) ) {
      Sys_PostMessage ( Data->DataWindows[Data->ActiveWindow].Window, WM_SET_LINECOLOR,
         MPFROMLONG(Data->IniData->LineColors[Command-IDM_LINECOLORS]), 0 ) ;
      return ( 0 ) ;
   } /* endif */

  /**************************************************************************
   * Process line width selection messages.                                 *
   **************************************************************************/

   if ( ( Command >= IDM_LINEWIDTHS ) AND ( Command <= IDM_LINEWIDTHS_MAX ) ) {
      Sys_PostMessage ( Data->DataWindows[Data->ActiveWindow].Window, WM_SET_LINEWIDTH,
         MPFROMLONG(Data->IniData->LineWidths[Command-IDM_LINEWIDTHS]), 0 ) ;
      return ( 0 ) ;
   } /* endif */

  /**************************************************************************
   * Process fill color selection messages.                                 *
   **************************************************************************/

   if ( ( Command >= IDM_FILLCOLORS ) AND ( Command <= IDM_FILLCOLORS_MAX ) ) {
      Sys_PostMessage ( Data->DataWindows[Data->ActiveWindow].Window, WM_SET_FILLCOLOR,
         MPFROMLONG(Data->IniData->FillColors[Command-IDM_FILLCOLORS]), 0 ) ;
      return ( 0 ) ;
   } /* endif */

  /**************************************************************************
   * Process fill type selection messages.                                  *
   **************************************************************************/

   if ( ( Command >= IDM_FILLTYPES ) AND ( Command <= IDM_FILLTYPES_MAX ) ) {
      Sys_PostMessage ( Data->DataWindows[Data->ActiveWindow].Window, WM_SET_FILLTYPE,
         MPFROMLONG(Data->IniData->FillTypes[Command-IDM_FILLTYPES]), 0 ) ;
      return ( 0 ) ;
   } /* endif */

  /**************************************************************************
   * Process line end selection messages.                                   *
   **************************************************************************/

   if ( ( Command >= IDM_LINEENDS ) AND ( Command <= IDM_LINEENDS_MAX ) ) {
      Sys_PostMessage ( Data->DataWindows[Data->ActiveWindow].Window, WM_SET_LINEEND,
         MPFROMLONG(Data->IniData->LineEnds[Command-IDM_LINEENDS]), 0 ) ;
      return ( 0 ) ;
   } /* endif */

  /**************************************************************************
   * Process line join selection messages.                                  *
   **************************************************************************/

   if ( ( Command >= IDM_LINEJOINS ) AND ( Command <= IDM_LINEJOINS_MAX ) ) {
      Sys_PostMessage ( Data->DataWindows[Data->ActiveWindow].Window, WM_SET_LINEJOIN,
         MPFROMLONG(Data->IniData->LineJoins[Command-IDM_LINEJOINS]), 0 ) ;
      return ( 0 ) ;
   } /* endif */

  /**************************************************************************
   * Process window select messages.                                        *
   **************************************************************************/

   if ( ( Command >= IDM_WINDOWS ) AND ( Command <= IDM_WINDOWS_MAX ) ) {
      int Offset = Command - IDM_WINDOWS ;
      for ( int i=0; i<MAX_DOCUMENTS; i++ ) {
         if ( Data->DataWindows[i].Window ) {
            if ( Offset == 0 ) {
               SWP Position ;
               WinQueryWindowPos ( Data->DataWindows[i].Window, &Position ) ;
               ULONG Flags = SWP_ZORDER | SWP_ACTIVATE |
                  ( Position.fl & SWP_MINIMIZE ? SWP_RESTORE : 0 ) ;
               WinSetWindowPos ( Data->DataWindows[i].Window,
                  HWND_TOP, 0, 0, 0, 0, Flags ) ;
               return ( 0 ) ;
            } /* endif */
            Offset -- ;
         } /* endif */
      } /* endfor */
      return ( 0 ) ;
   } /* endif */

  /**************************************************************************
   * Process the product information request messages.                      *
   **************************************************************************/

   if ( ( Command > IDM_PRODUCT_LIST ) AND ( Command <= IDM_PRODUCT_MAX ) ) {
      AddonList.ShowProductInformation ( Command, Window ) ;
      return ( 0 ) ;
   } /* endif */

  /**************************************************************************
   * Dispatch the message without a default message processor.              *
   **************************************************************************/

   static METHOD Methods [] = {

      { IDM_REFRESH,                      Refresh            },
      { IDM_NEXTDOC,                      NextDocument       },

      { IDM_NEW,                          FileNew            },
      { IDM_OPEN,                         FileOpen           },
      { IDM_MERGE,                        SendToDocument     },
      { IDM_SAVE,                         SendToDocument     },
      { IDM_SAVEAS,                       SendToDocument     },
      { IDM_SAVE_CLOSE,                   SendToDocument     },
      { IDM_CLOSE,                        SendToDocument     },
      { IDM_PRINT_ALL,                    SendToDocument     },
      { IDM_PRINT_ALL_NORMAL,             SendToDocument     },
      { IDM_PRINT_ALL_REVERSE,            SendToDocument     },
      { IDM_PRINT_PAGE,                   SendToDocument     },
      { IDM_PREFERENCES,                  Preferences        },
      { IDM_DEFAULTS,                     Defaults           },
      { IDM_DEFAULTS_CONTEXT,             DefaultsContext    },
      { IDM_CONFIGURE_EXTENSIONS,         ConfigureExtensions},
      { IDM_EXIT,                         Exit               },  // From SUPPORT

      { IDM_UNDO,                         SendToDocument     },
      { IDM_REDO,                         SendToDocument     },
      { IDM_CUT,                          SendToDocument     },
      { IDM_COPY,                         SendToDocument     },
      { IDM_PASTE,                        SendToDocument     },
      { IDM_CLEAR,                        SendToDocument     },
      { IDM_CLEARCLIPBOARD,               ClearClipboard     },
      { IDM_SELECT_ALL,                   SendToDocument     },
      { IDM_PASTE_FROM_CLIP_BITMAP,       SendToDocument     },
      { IDM_PASTE_FROM_CLIP_METAFILE,     SendToDocument     },
      { IDM_PASTE_FROM_SCRN_ENTIRE,       SendToDocument     },
      { IDM_PASTE_FROM_SCRN_FRAME,        SendToDocument     },
      { IDM_PASTE_FROM_SCRN_WINDOW,       SendToDocument     },
      { IDM_PASTE_FROM_SCRN_RECT,         SendToDocument     },
      { IDM_PASTE_FROM_SCRN_HIDE,         HideOnScreenClip   },
      { IDM_PASTE_FROM_FILE,              SendToDocument     },
      { IDM_SEARCH,                       SendToDocument     },
      { IDM_FIND,                         SendToDocument     },
      { IDM_FINDNEXT,                     SendToDocument     },
      { IDM_CHANGE,                       SendToDocument     },
      { IDM_CHANGEFIND,                   SendToDocument     },
      { IDM_CHANGEALL,                    SendToDocument     },
      { IDM_GOTOPAGE,                     SendToDocument     },
      { IDM_SPELL,                        SendToDocument     },
      { IDM_ZOOM_100,                     SendToDocument     },
      { IDM_ZOOM_FULLWIDTH,               SendToDocument     },
      { IDM_ZOOM_FULLHEIGHT,              SendToDocument     },
      { IDM_ZOOM_MARGINS,                 SendToDocument     },
      { IDM_ZOOM_ALL,                     SendToDocument     },
      { IDM_ZOOM_WINDOW,                  SendToDocument     },
      { IDM_ZOOM_SET,                     SendToDocument     },

      { IDM_MARGINS,                      SendToDocument     },
      { IDM_TABS,                         SendToDocument     },
      { IDM_INDENT,                       SendToDocument     },
      { IDM_JUST_LEFT,                    SendToDocument     },
      { IDM_JUST_CENTER,                  SendToDocument     },
      { IDM_JUST_RIGHT,                   SendToDocument     },
      { IDM_JUST_FULL,                    SendToDocument     },
      { IDM_PAGECENTER,                   SendToDocument     },
      { IDM_PAGEBREAK,                    SendToDocument     },
      { IDM_DATETODAY,                    SendToDocument     },
      { IDM_DATECURRENT,                  SendToDocument     },
      { IDM_DATECHANGED,                  SendToDocument     },
      { IDM_DATEFMT,                      SendToDocument     },
      { IDM_CURRENTPAGE,                  SendToDocument     },
      { IDM_PAGECOUNT,                    SendToDocument     },
      { IDM_SEPARATOR,                    SendToDocument     },
      { IDM_BARCODE,                      SendToDocument     },
      { IDM_HEADER,                       SendToDocument     },
      { IDM_FOOTER,                       SendToDocument     },

      { IDM_FONTS_PGUP,                   FontsPageUp        },
      { IDM_FONTS_PGDN,                   FontsPageDown      },
      { IDM_FONTMGR,                      FontManager        },

      { IDM_SIZES_PGUP,                   SizesPageUp        },
      { IDM_SIZES_PGDN,                   SizesPageDown      },
      { IDM_SIZEMGR,                      SizeManager        },

      { IDM_NORMAL,                       SendToDocument     },
      { IDM_BOLD,                         SendToDocument     },
      { IDM_ITALIC,                       SendToDocument     },
      { IDM_UNDERSCORE,                   SendToDocument     },
      { IDM_STRIKEOUT,                    SendToDocument     },
      { IDM_OUTLINE,                      SendToDocument     },
      { IDM_CAPS,                         SendToDocument     },

      { IDM_EXPANDS_PGUP,                 ExpandPageUp       },
      { IDM_EXPANDS_PGDN,                 ExpandPageDown     },
      { IDM_EXPANDMGR,                    ExpandManager      },

      { IDM_LINECOLORS_PGUP,              LineColorsPageUp   },
      { IDM_LINECOLORS_PGDN,              LineColorsPageDown },
      { IDM_LINECOLORMGR,                 LineColorManager   },

      { IDM_LINEWIDTHS_PGUP,              LineWidthsPageUp   },
      { IDM_LINEWIDTHS_PGDN,              LineWidthsPageDown },
      { IDM_LINEWIDTHMGR,                 LineWidthManager   },

      { IDM_FILLCOLORS_PGUP,              FillColorsPageUp   },
      { IDM_FILLCOLORS_PGDN,              FillColorsPageDown },
      { IDM_FILLCOLORMGR,                 FillColorManager   },

      { IDM_FILLTYPES_PGUP,               FillTypesPageUp    },
      { IDM_FILLTYPES_PGDN,               FillTypesPageDown  },
      { IDM_FILLTYPEMGR,                  FillTypeManager    },

      { IDM_LINEENDS_PGUP,                LineEndsPageUp     },
      { IDM_LINEENDS_PGDN,                LineEndsPageDown   },
      { IDM_LINEENDMGR,                   LineEndManager     },

      { IDM_LINEJOINS_PGUP,               LineJoinsPageUp    },
      { IDM_LINEJOINS_PGDN,               LineJoinsPageDown  },
      { IDM_LINEJOINMGR,                  LineJoinManager    },

      { IDM_TILE,                         WindowTile         },
      { IDM_CASCADE,                      WindowCascade      },
      { IDM_ARRANGEICONS,                 WindowArrange      },

      { IDM_HELP_FOR_HELP,                HelpForHelp        },  // From SUPPORT
      { IDM_EXTENDED_HELP,                ExtendedHelp       },  // From SUPPORT
      { IDM_KEYS_HELP,                    KeysHelp           },  // From SUPPORT
      { IDM_HELP_INDEX,                   HelpIndex          },  // From SUPPORT
      { IDM_ABOUT,                        About              },

   } ;

   return ( DispatchMessage ( Window, Command, mp1, mp2, Methods, sizeof(Methods)/sizeof(Methods[0]), 0 ) ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Send Command to Active Document                                     *
 *                                                                          *
 ****************************************************************************/

STATIC MRESULT APIENTRY SendToDocument ( HWND Window, MESG, MPARAM1 mp1, MPARAM2 mp2 ) {

  /**************************************************************************
   * Find the instance data.                                                *
   **************************************************************************/

   PDATA Data = PDATA ( Sys_GetWindowData ( Window ) ) ;

  /**************************************************************************
   * Send message to the document.                                          *
   **************************************************************************/

   Sys_PostMessage ( Data->DataWindows[Data->ActiveWindow].Window, WM_COMMAND, mp1, mp2 ) ;

  /**************************************************************************
   * We're done.                                                            *
   **************************************************************************/

   return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process Refresh command.                                            *
 *                                                                          *
 ****************************************************************************/

STATIC MRESULT APIENTRY Refresh ( HWND Window, MESG, MPARAM1, MPARAM2 ) {

  /**************************************************************************
   * Invalidate the window and force a repaint.                             *
   **************************************************************************/

   Sys_InvalidateWindow ( Window ) ;

  /**************************************************************************
   * Done.                                                                  *
   **************************************************************************/

   return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process Next Document command.                                      *
 *                                                                          *
 ****************************************************************************/

STATIC MRESULT APIENTRY NextDocument ( HWND Window, MESG, MPARAM1, MPARAM2 ) {

  /**************************************************************************
   * Find the instance data.                                                *
   **************************************************************************/

   PDATA Data = PDATA ( Sys_GetWindowData ( Window ) ) ;

  /**************************************************************************
   * Switch to the next document in sequence.                               *
   **************************************************************************/

   int i = Data->ActiveWindow + 1 ;

   while ( i != Data->ActiveWindow ) {

      if ( i >= MAX_DOCUMENTS )
         i = 0 ;

      if ( Data->DataWindows[i].Window ) {
         SWP Position ;
         WinQueryWindowPos ( Data->DataWindows[i].Window, &Position ) ;
         ULONG Flags = SWP_ZORDER | SWP_ACTIVATE | ( Position.fl & SWP_MINIMIZE ? SWP_RESTORE : 0 ) ;
         WinSetWindowPos ( Data->DataWindows[i].Window, HWND_TOP, 0, 0, 0, 0, Flags ) ;
         break ;
      } /* endif */

      i ++ ;

   } /* endwhile */

  /**************************************************************************
   * Done.                                                                  *
   **************************************************************************/

   return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process File New command.                                           *
 *                                                                          *
 ****************************************************************************/

STATIC MRESULT APIENTRY FileNew ( HWND Window, MESG, MPARAM1, MPARAM2 ) {

  /**************************************************************************
   * Find the instance data.                                                *
   **************************************************************************/

   PDATA Data = PDATA ( Sys_GetWindowData ( Window ) ) ;

  /**************************************************************************
   * Find an empty entry in the data window list.                           *
   **************************************************************************/

   for ( int i=0; i<MAX_DOCUMENTS; i++ )
      if ( Data->DataWindows[i].Window == 0 )
         break ;

   if ( i >= MAX_DOCUMENTS ) {
      ResourceString Title ( Data->Library->QueryHandle(), IDS_TITLE ) ;
      ResourceString Format ( Data->Library->QueryHandle(), IDS_ERROR_TOOMANYDOCUMENTS ) ;
      char Message [200] ;
      sprintf ( Message, PCHAR(Format), MAX_DOCUMENTS ) ;
      MessageBox ( HWND_DESKTOP, Window, PSZ(Message),
         PSZ(Title), IDD_ERROR_TOOMANYDOCUMENTS, MB_ENTER | MB_ICONEXCLAMATION | MB_HELP, LibraryHandle, IDS_MESSAGE_BOX_STRINGS ) ;
      return ( 0 ) ;
   } /* endif */

  /**************************************************************************
   * Minimize and deactivate all other document windows.                    *
   **************************************************************************/

   int DocumentCount = 0 ;
   for ( int j=0; j<MAX_DOCUMENTS; j++ ) {
      if ( Data->DataWindows[j].Window == 0 )
         continue ;

      DocumentCount ++ ;

      SWP Position ;
      WinQueryWindowPos ( Data->DataWindows[j].Window, &Position ) ;
      WinSetWindowPos ( Data->DataWindows[j].Window, HWND_BOTTOM,
         Position.x, Position.y, Position.cx, Position.cy,
         SWP_MOVE | SWP_SIZE | SWP_ZORDER | SWP_MINIMIZE | SWP_SHOW | SWP_DEACTIVATE ) ;

   } /* endif */

   WindowArrange ( Window, 0, 0, 0 ) ;

  /**************************************************************************
   * Get the standard empty document title.                                 *
   **************************************************************************/

   ResourceString NewDocument ( Data->Library->QueryHandle(), IDS_NEW_DOCUMENT ) ;

   UCHAR Title [CCHMAXPATH] ;
   sprintf ( PCHAR(Title), "%s, Page 1", PSZ(NewDocument) ) ;

  /**************************************************************************
   * Create an empty document.                                              *
   **************************************************************************/

   #ifdef DEBUG
      Log ( "FileNew: Creating new document." ) ;
   #endif // DEBUG

   Documento *pDocument = new Documento ( WinQueryAnchorBlock(Window), Data->Library, Data->IniData->DocParms ) ;

  /**************************************************************************
   * Create a new document window.                                          *
   **************************************************************************/

   #ifdef DEBUG
      Log ( "FileNew: Creating new document window." ) ;
   #endif // DEBUG

   Data->DataWindows[i].pDocument = pDocument ;
   Data->DataWindows[i].Window = pDocument->CreateWindow ( Window, Data->IniData, Data->Speller, 100+i, int(Data->Trap) ) ;

  /**************************************************************************
   * If there were other documents, tile the workspace.  If not, then       *
   *    just maximize the new document window.                              *
   **************************************************************************/

   #ifdef DEBUG
      Log ( "FileNew: Positioning new document window." ) ;
   #endif // DEBUG

   SWP Position ;
   WinQueryWindowPos ( Data->DataWindows[i].Window, &Position ) ;

   if ( DocumentCount ) {
      WindowTile ( Window, 0, 0, 0 ) ;
   } else {
      WinSetWindowPos ( Data->DataWindows[i].Window, HWND_TOP,
         Position.x, Position.y, Position.cx, Position.cy,
         SWP_MOVE | SWP_SIZE | SWP_MAXIMIZE ) ;
   } /* endif */

  /**************************************************************************
   * This window becomes the active one.                                    *
   **************************************************************************/

   Data->ActiveWindow = short ( i ) ;
   WinSetWindowPos ( Data->DataWindows[i].Window, HWND_TOP,
      Position.x, Position.y, Position.cx, Position.cy,
      SWP_ZORDER | SWP_ACTIVATE | SWP_SHOW ) ;

  /**************************************************************************
   * Update the menu, now that we have a current document.                  *
   **************************************************************************/

   UpdateMenu ( Window, Data ) ;

  /**************************************************************************
   * Done.                                                                  *
   **************************************************************************/

   return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process File Open command.                                          *
 *                                                                          *
 ****************************************************************************/

STATIC MRESULT APIENTRY FileOpen ( HWND Window, MESG, MPARAM1, MPARAM2 ) {

  /**************************************************************************
   * Find the instance data.                                                *
   **************************************************************************/

   PDATA Data = PDATA ( Sys_GetWindowData ( Window ) ) ;

  /**************************************************************************
   * Disable the File Open menu option.                                     *
   **************************************************************************/

   EnableMenuItem ( PARENT(Window), FID_MENU, IDM_OPEN, FALSE ) ;

  /**************************************************************************
   * Display the system File Open dialog.                                   *
   **************************************************************************/

   ResourceString OpenDocument ( Data->Library->QueryHandle(), IDS_OPEN_DOCUMENT ) ;
   PrepareFileTypeList ( ) ;
   FILEDIALOG_DATA FileDialogData = { TRUE, FileTypeList, FileWildcardList, FileExtenderList, 0 } ;
   FILEDLG DialogData ;
   memset ( &DialogData, 0, sizeof(DialogData) ) ;
   DialogData.cbSize = sizeof(DialogData) ;
   DialogData.fl = FDS_OPEN_DIALOG | FDS_CENTER | FDS_MULTIPLESEL |
      FDS_HELPBUTTON | FDS_FILTERUNION ;
   DialogData.pszTitle = PSZ(OpenDocument) ;
   strcpy ( DialogData.szFullFile, FileWildcardList[0] ) ;
   DialogData.pszIType = Data->IniData->FileType[0] ? Data->IniData->FileType : FileTypeList[0] ;
   DialogData.papszITypeList = (PAPSZ) FileTypeList ;
   DialogData.pfnDlgProc = FileDialog_Processor ;
   DialogData.ulUser = ULONG ( &FileDialogData ) ;
   WinFileDlg ( HWND_DESKTOP, Window, &DialogData ) ;

  /**************************************************************************
   * If the dialog was cancelled, just return.                              *
   **************************************************************************/

   if ( DialogData.lReturn == DID_CANCEL ) {
      EnableMenuItem ( PARENT(Window), FID_MENU, IDM_OPEN, TRUE ) ;
      return ( 0 ) ;
   } /* endif */

  /**************************************************************************
   * Save the most recently selected file type.                             *
   **************************************************************************/

   strcpy ( Data->IniData->FileType, FileDialogData.FilterIndex > 0 ? FileTypeList[FileDialogData.FilterIndex] : "" ) ;

  /**************************************************************************
   * Minimize all the other open documents.                                 *
   **************************************************************************/

   for ( int j=0; j<MAX_DOCUMENTS; j++ ) {
      if ( Data->DataWindows[j].Window == 0 )
         continue ;

      SWP Position ;
      WinQueryWindowPos ( Data->DataWindows[j].Window, &Position ) ;
      WinSetWindowPos ( Data->DataWindows[j].Window, HWND_BOTTOM,
         Position.x, Position.y, Position.cx, Position.cy,
         SWP_MOVE | SWP_SIZE | SWP_ZORDER | SWP_MINIMIZE | SWP_SHOW | SWP_DEACTIVATE ) ;
   } /* endfor */
   WindowArrange ( Window, 0, 0, 0 ) ;

  /**************************************************************************
   * Move to the selected directory.                                        *
   **************************************************************************/

   char Drive [_MAX_DRIVE + 1 ] ;
   char Dir   [_MAX_DIR   + 1 ] ;
   char Name  [_MAX_FNAME + 1 ] ;
   char Ext   [_MAX_EXT   + 1 ] ;
   _splitpath ( PCHAR(DialogData.szFullFile), Drive, Dir, Name, Ext ) ;
   strupr ( Drive ) ;
   int Type ;
   if ( ( CheckDrive ( Drive[0]-'A'+1, Type ) == 0 ) AND ( Type > 0 ) ) {
      _chdrive ( Drive[0]-'A'+1 ) ;
      Dir[strlen(Dir)-1] = 0 ;
      _chdir ( "\\" ) ;
      _chdir ( Dir ) ;
   } /* endif */

  /**************************************************************************
   * Load the selected file(s).                                             *
   **************************************************************************/

   if ( DialogData.papszFQFilename AND ( DialogData.ulFQFCount > 1 ) ) {

      PSZ *Name = (PSZ*) DialogData.papszFQFilename ;
      for ( int i=0; i<DialogData.ulFQFCount; i++, Name++ ) {
         StartLoad ( *Name, Data, Window, 0 ) ;
      } /* endfor */
      WinFreeFileDlgList ( DialogData.papszFQFilename ) ;

   } else {
      StartLoad ( PSZ(DialogData.szFullFile), Data, Window, FileDialogData.Index ) ;

   } /* endif */

  /**************************************************************************
   * Enable the File Open menu option.                                      *
   **************************************************************************/

   EnableMenuItem ( PARENT(Window), FID_MENU, IDM_OPEN, TRUE ) ;

  /**************************************************************************
   * Done.                                                                  *
   **************************************************************************/

   return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process File Recall command.                                        *
 *                                                                          *
 ****************************************************************************/

STATIC MRESULT APIENTRY FileRecall ( HWND Window, MESG, MPARAM1 mp1, MPARAM2 ) {

  /**************************************************************************
   * Find the instance data.                                                *
   **************************************************************************/

   PDATA Data = PDATA ( Sys_GetWindowData ( Window ) ) ;

  /**************************************************************************
   * Minimize all existing document windows and arrange them.               *
   **************************************************************************/

   // Minimize all other documents first.
   for ( int j=0; j<MAX_DOCUMENTS; j++ ) {
      if ( Data->DataWindows[j].Window == 0 )
         continue ;
      SWP Position ;
       WinQueryWindowPos ( Data->DataWindows[j].Window, &Position ) ;
      WinSetWindowPos ( Data->DataWindows[j].Window, HWND_BOTTOM,
         Position.x, Position.y, Position.cx, Position.cy,
         SWP_MOVE | SWP_SIZE | SWP_ZORDER | SWP_MINIMIZE | SWP_SHOW | SWP_DEACTIVATE ) ;
   } /* endfor */

   WindowArrange ( Window, 0, 0, 0 ) ;

  /**************************************************************************
   * Get the file name.                                                     *
   **************************************************************************/

   Data->IniData->FileListLock.Request ( ) ;
   UCHAR Path [_MAX_PATH] ;
   strcpy ( PCHAR(Path), PCHAR(Data->IniData->Files[SHORT1FROMMP(mp1)-IDM_RECALL0]) ) ;
   Data->IniData->FileListLock.Release ( ) ;

  /**************************************************************************
   * Load the file.                                                         *
   **************************************************************************/

   StartLoad ( PSZ(Path), Data, Window, 0 ) ;

  /**************************************************************************
   * Done.                                                                  *
   **************************************************************************/

   return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process General Preferences command.                                *
 *                                                                          *
 ****************************************************************************/

STATIC MRESULT APIENTRY Preferences ( HWND Window, MESG, MPARAM1, MPARAM2 ) {

  /**************************************************************************
   * Find the instance data.                                                *
   **************************************************************************/

   PDATA Data = PDATA ( Sys_GetWindowData ( Window ) ) ;

  /**************************************************************************
   * Call up the General Preferences dialog.                                *
   **************************************************************************/

   PREFERENCES_PARMS Parms ;
   Parms.Library = Data->Library->QueryHandle() ;
   Parms.Animate = Data->IniData->Animate ;
   Parms.Rulers = Data->IniData->Rulers ;
   Parms.ScrollBars = Data->IniData->ScrollBars ;
   Parms.StatusLine = Data->IniData->StatusLine ;
   Parms.FullNames = Data->IniData->FullNames ;
   Parms.SpoolPages = Data->IniData->SpoolPages ;
   Parms.PrintReverse = Data->IniData->PrintReverse ;
   Parms.LoadMostRecent = Data->IniData->LoadMostRecent ;
   Parms.ZoomType = int ( Data->IniData->ZoomType ) ;

   if ( WinDlgBox ( HWND_DESKTOP, Window, PreferencesProcessor, Data->Library->QueryHandle(), IDD_PREFERENCES, &Parms ) ) {

      if ( ( Data->IniData->Rulers != Parms.Rulers )
         OR ( Data->IniData->ScrollBars != Parms.ScrollBars )
         OR ( Data->IniData->StatusLine != Parms.StatusLine ) ) {

         Data->IniData->Rulers = Parms.Rulers ;
         Data->IniData->ScrollBars = Parms.ScrollBars ;
         Data->IniData->StatusLine = Parms.StatusLine ;
         for ( int i=0; i<MAX_DOCUMENTS; i++ ) {
            if ( Data->DataWindows[i].Window ) {
               Sys_PostMessage ( Data->DataWindows[i].Window, WM_ALTER_FRAME, 0, 0 ) ;
            } /* endif */
         } /* endfor */
      } /* endif */

      if ( Data->IniData->FullNames != Parms.FullNames ) {
         Data->IniData->FullNames = Parms.FullNames ;
         for ( int i=0; i<MAX_DOCUMENTS; i++ ) {
            if ( Data->DataWindows[i].Window ) {
               Sys_PostMessage ( Data->DataWindows[i].Window, WM_ALTER_WINDOW, 0, 0 ) ;
            } /* endif */
         } /* endfor */
      } /* endif */

      Data->IniData->Animate = Parms.Animate ;
      Data->IniData->SpoolPages = Parms.SpoolPages ;
      Data->IniData->PrintReverse = Parms.PrintReverse ;
      Data->IniData->LoadMostRecent = Parms.LoadMostRecent ;
      Data->IniData->ZoomType = Parms.ZoomType ;

   } /* endif */

  /**************************************************************************
   * Done.                                                                  *
   **************************************************************************/

   return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process Document Defaults command.                                  *
 *                                                                          *
 ****************************************************************************/

STATIC MRESULT APIENTRY Defaults ( HWND Window, MESG, MPARAM1, MPARAM2 ) {

  /**************************************************************************
   * Find the instance data.                                                *
   **************************************************************************/

   PDATA Data = PDATA ( Sys_GetWindowData ( Window ) ) ;

  /**************************************************************************
   * Use active document's defaults, unless there are none active.          *
   **************************************************************************/

   DocumentParms *Defaults = & Data->IniData->DocParms ;
   if ( Data->ActiveWindow != -1 ) {
      Defaults = (DocumentParms*) ( Sys_SendMessage (
         Data->DataWindows[Data->ActiveWindow].Window, WM_QUERY_DEFAULTS, 0, 0 ) ) ;
   } /* endif */

  /**************************************************************************
   * Call up the Document Defaults dialog.                                  *
   **************************************************************************/

   static DEFAULTS_PARMS Parms ;
   Parms.Library = Data->Library->QueryHandle() ;
   Parms.Global = ( Data->ActiveWindow == -1 ) ;
   Parms.DocParms = *Defaults ;
   Parms.Window = ( Data->ActiveWindow == -1 ) ? 0 : Data->DataWindows[Data->ActiveWindow].Window ;

   if ( DefaultsDialog ( Window, Data->Library->QueryHandle(), Parms ) ) {

      if ( Data->ActiveWindow != -1 )
         Sys_PostMessage ( Data->DataWindows[Data->ActiveWindow].Window,
            WM_SET_DEFAULTS, MPFROMP(&Parms.DocParms), 0 ) ;

      if ( Parms.Global OR ( Data->ActiveWindow == -1 ) )
         Data->IniData->DocParms = Parms.DocParms ;

   } /* endif */

  /**************************************************************************
   * Done.                                                                  *
   **************************************************************************/

   return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process Document Defaults command from Context Menu.                *
 *                                                                          *
 ****************************************************************************/

STATIC MRESULT APIENTRY DefaultsContext ( HWND Window, MESG, MPARAM1, MPARAM2 ) {

  /**************************************************************************
   * Find the instance data.                                                *
   **************************************************************************/

   PDATA Data = PDATA ( Sys_GetWindowData ( Window ) ) ;

  /**************************************************************************
   * Call up the Document Defaults dialog.                                  *
   **************************************************************************/

   DEFAULTS_PARMS Parms ;
   Parms.Library = Data->Library->QueryHandle() ;
   Parms.Global = TRUE ;
   Parms.DocParms = Data->IniData->DocParms ;
   Parms.Window = 0 ;

   if ( DefaultsDialog ( Window, Data->Library->QueryHandle(), Parms ) )
     Data->IniData->DocParms = Parms.DocParms ;

  /**************************************************************************
   * Done.                                                                  *
   **************************************************************************/

   return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process Configure Extensions command.                               *
 *                                                                          *
 ****************************************************************************/

STATIC MRESULT APIENTRY ConfigureExtensions ( HWND Window, MESG, MPARAM1, MPARAM2 ) {

  /**************************************************************************
   * Invoke the configuration dialog.                                       *
   **************************************************************************/

   AddonList.Configure ( Window ) ;

  /**************************************************************************
   * Done.                                                                  *
   **************************************************************************/

   return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process Select Language command.                                    *
 *                                                                          *
 ****************************************************************************/

STATIC MRESULT APIENTRY SelectLanguage ( HWND Window, MESG, MPARAM1 mp1, MPARAM2 ) {

  /**************************************************************************
   * Find the instance data.                                                *
   **************************************************************************/

   PDATA Data = PDATA ( Sys_GetWindowData ( Window ) ) ;

  /**************************************************************************
   * Determine the language & library name.                                 *
   **************************************************************************/

   int Index = int ( LONGFROMMP(mp1) - IDM_LANGUAGES ) ;
   char *LanguageName = Languages [ Index ] ;
   char *LibraryName = Libraries [ Index ] ;

  /**************************************************************************
   * If language has not changed, just return.                              *
   **************************************************************************/

   if ( !strcmpi ( Library->QueryLanguage(), LanguageName ) )
      return ( 0 ) ;

  /**************************************************************************
   * Move to the home path for a bit.                                       *
   **************************************************************************/

   int OldDrive = _getdrive ( ) ;
   char OldDirectory [ CCHMAXPATH ] ;
   getcwd ( OldDirectory, sizeof(OldDirectory) ) ;
   char Drive [_MAX_DRIVE+1], Dir[_MAX_DIR+1], Fname[_MAX_FNAME+1], Ext[_MAX_EXT+1] ;
   _splitpath ( HomePath, Drive, Dir, Fname, Ext ) ;
   strcat ( Dir, Fname ) ;
   strcat ( Dir, Ext ) ;
   _chdrive ( Drive[0] - 'A' + 1 ) ;
   _chdir ( "\\" ) ;
   _chdir ( Dir ) ;

  /**************************************************************************
   * Load the new library.                                                  *
   **************************************************************************/

   LanguageModule *NewLibrary = new LanguageModule ( PROGRAM_NAME, LibraryName ) ;
   ResourceString Revision ( Data->Library->QueryHandle(), IDS_TITLE1 ) ;
   if ( NewLibrary->QueryHandle() AND !strcmp ( PCHAR(Revision), REVISION ) ) {

      // Put up the hourglass.
      HourGlass Hold ( Window ) ;

      // Kill the bitmap threads, because they will have to be restarted.
      CancelBitmapThreads ( ) ;
      DestroyBitmaps ( ) ;

      // Replace the old library.
      delete Library ;
      Library = NewLibrary ;
      LibraryHandle = Library->QueryHandle() ;
      Data->Library = Library ;

      // Set new queue codepage.
      HMQ Queue = HMQ ( WinQueryWindowULong ( Window, QWL_HMQ ) ) ;
      USHORT OldCodepage = USHORT ( WinQueryCp ( Queue ) ) ;
      PUSHORT pCodePage = Library->QueryCodepages() ;
      while ( *pCodePage ) {
         if ( !DosSetProcessCp ( *pCodePage ) )
            break ;
         pCodePage ++ ;
      } /* endwhile */
      SetQueueCodepage ( Window, Queue, *pCodePage ? *pCodePage : OldCodepage ) ;
      if ( *pCodePage == 0 ) {
         ResourceString Title ( Data->Library->QueryHandle(), IDS_TITLE ) ;
         ResourceString Format ( Library->QueryHandle(), IDS_WARNING_BADCODEPAGE ) ;
         CHAR Message [200] ;
         sprintf ( Message, PCHAR(Format), *Library->QueryCodepages() ) ;
         MessageBox ( HWND_DESKTOP, HWND_DESKTOP, PSZ(Message),
            PSZ(Title), 0, MB_ENTER | MB_ICONEXCLAMATION, LibraryHandle, IDS_MESSAGE_BOX_STRINGS ) ;
      } /* endif */

      // Set the program title.
      ResourceString Title ( Data->Library->QueryHandle(), IDS_TITLE ) ;
      HWND Frame = PARENT ( Window ) ;
      Sys_SetWindowText ( Frame, PSZ(Title) ) ;

      // Update the system task list.
      UpdateTaskListEntry ( Frame, PCHAR(Title) ) ;

      // Update any add-on extensions.
      AddonList.SelectLanguage ( LanguageName ) ;

      // Reload the window frame menu.
      HWND Menu = CHILD ( Frame, FID_MENU ) ;
      WinDestroyWindow ( Menu ) ;
      Menu = WinLoadMenu ( Frame, Data->Library->QueryHandle(), IDM_MAIN ) ;
      Sys_SendMessage ( Frame, WM_UPDATEFRAME, MPFROMSHORT(FCF_MENU), 0 ) ;
      MENUITEM MenuItem ;
      Sys_SendMessage ( CHILD(Frame,FID_MENU), MM_QUERYITEM,
         MPFROM2SHORT(IDM_ABOUT_MENU,TRUE), MPFROMP(&MenuItem) ) ;
      ULONG Style = WinQueryWindowULong ( MenuItem.hwndSubMenu, QWL_STYLE ) ;
      WinSetWindowULong ( MenuItem.hwndSubMenu, QWL_STYLE, Style | MS_CONDITIONALCASCADE ) ;
      AddonList.AddToMenu ( MenuItem.hwndSubMenu ) ;

      // Reload the window context menu.
      WinDestroyWindow ( Data->ContextMenu ) ;
      Data->ContextMenu = WinLoadMenu ( HWND_DESKTOP, Data->Library->QueryHandle(), IDM_MAIN_CONTEXT ) ;
      Sys_SendMessage ( Data->ContextMenu, MM_QUERYITEM, MPFROM2SHORT(IDM_ABOUT_MENU,TRUE), MPFROMP(&MenuItem) ) ;
      Style = WinQueryWindowULong ( MenuItem.hwndSubMenu, QWL_STYLE ) ;
      WinSetWindowULong ( MenuItem.hwndSubMenu, QWL_STYLE, Style | MS_CONDITIONALCASCADE ) ;
      AddonList.AddToMenu ( MenuItem.hwndSubMenu ) ;

      // Reload the accelerator table.
      HACCEL Accelerator = WinQueryAccelTable ( WinQueryAnchorBlock(Window), Frame ) ;
      if ( Accelerator ) {
         if ( !WinDestroyAccelTable ( Accelerator ) ) {
            char Message [512] ;
            Log ( "Main::SelectLanguage: Unable to destroy old accelerator table.", InterpretWinError(0,Message) ) ;
         } /* endif */
      } else {
         char Message [512] ;
         Log ( "Main::SelectLanguage: Unable to query old accelerator table handle.", InterpretWinError(0,Message) ) ;
      } /* endif */
      Accelerator = WinLoadAccelTable ( WinQueryAnchorBlock(Window), Data->Library->QueryHandle(), ID_MAIN ) ;
      if ( Accelerator ) {
         if ( !WinSetAccelTable ( WinQueryAnchorBlock(Window), Accelerator, Frame ) ) {
            char Message [512] ;
            Log ( "Main::SelectLanguage: Unable to set new accelerator table.", InterpretWinError(0,Message) ) ;
         } /* endif */
      } else {
         char Message [512] ;
         Log ( "Main::SelectLanguage: Unable to load new accelerator table.", InterpretWinError(0,Message) ) ;
      } /* endif */

      // Reload the help facility.
      delete Help ;  Help = 0 ;
      _splitpath ( Library->QueryName(), Drive, Dir, Fname, Ext ) ;
      char HelpFileName [CCHMAXPATH] ;
      sprintf ( HelpFileName, "%s.hlp", Fname ) ;
      ResourceString HelpTitle ( Library->QueryHandle(), IDS_HELPTITLE ) ;
      Help = new HelpWindow ( WinQueryAnchorBlock(Window), 0, ID_MAIN, PSZ(HelpFileName), PSZ(HelpTitle) ) ;
      if ( Help->QueryHandle() == 0 ) {
         ERRORID Error = Sys_GetLastError ( WinQueryAnchorBlock(Window) ) ;
         ResourceString Format ( Library->QueryHandle(), IDS_ERROR_CREATEHELP ) ;
         char Message [200] ;
         sprintf ( Message, PCHAR(Format), Error ) ;
         Log ( "Main::SelectLanguage: %s", Message ) ;
         MessageBox ( HWND_DESKTOP, HWND_DESKTOP, PSZ(Message), PSZ(Title), 0, MB_ENTER | MB_ICONEXCLAMATION, LibraryHandle, IDS_MESSAGE_BOX_STRINGS ) ;
      } /* endif */
      Help->Associate ( Frame ) ;

      // Restart the bitmap threads.
      Data->FontMenuIndex = 0 ;
      InitFontBitmaps ( Data->Library->QueryHandle(), Window ) ;
      Data->SizeMenuIndex = 0 ;
      InitSizeBitmaps ( Data->Library->QueryHandle(), Window,
         PSZ(Data->IniData->DocParms.Font), Data->IniData->SizeCount, Data->IniData->Sizes ) ;
      InitAttrBitmaps ( Data->Library->QueryHandle(), Window,
         PSZ(Data->IniData->DocParms.Font) ) ;
      Data->ExpandMenuIndex = 0 ;
      InitExpandBitmaps ( Data->Library->QueryHandle(), Window,
         PSZ(Data->IniData->DocParms.Font), Data->IniData->ExpandCount, Data->IniData->Expansions ) ;
      Data->LineColorMenuIndex = 0 ;
      InitLineColorBitmaps ( Data->Library->QueryHandle(), Window,
         Data->IniData->LineColorCount, Data->IniData->LineColors ) ;
      Data->LineWidthMenuIndex = 0 ;
      InitLineWidthBitmaps ( Data->Library->QueryHandle(), Window,
         Data->IniData->LineWidthCount, Data->IniData->LineWidths ) ;
      Data->FillColorMenuIndex = 0 ;
      InitFillColorBitmaps ( Data->Library->QueryHandle(), Window,
         Data->IniData->FillColorCount, Data->IniData->FillColors ) ;
      Data->FillTypeMenuIndex = 0 ;
      InitFillTypeBitmaps ( Data->Library->QueryHandle(), Window,
         Data->IniData->FillTypeCount, Data->IniData->FillTypes ) ;
      Data->LineEndMenuIndex = 0 ;
      InitLineEndBitmaps ( Data->Library->QueryHandle(), Window,
         Data->IniData->LineEndCount, Data->IniData->LineEnds ) ;
      Data->LineJoinMenuIndex = 0 ;
      InitLineJoinBitmaps ( Data->Library->QueryHandle(), Window,
         Data->IniData->LineJoinCount, Data->IniData->LineJoins ) ;

      // Unload and reload the date formatting resources.
      UnloadDateFormattingResources ( ) ;
      LoadDateFormattingResources ( ) ;

      // Reload the loading status window text.
      ResourceString Loading ( Data->Library->QueryHandle(), IDS_LOADING ) ;
      Sys_SetWindowText ( Data->LoadStatusWindow, PSZ(Loading) ) ;

      // Save the new language setting.
      strcpy ( Data->IniData->Language, LanguageName ) ;

      // Update the document windows.
      for ( int i=0; i<MAX_DOCUMENTS; i++ ) {
         if ( Data->DataWindows[i].Window == 0 )
            continue ;
         Data->DataWindows[i].pDocument->Set_Language ( Data->Library ) ;
      } /* endfor */

   } else {
      ResourceString Format ( Data->Library->QueryHandle(), IDS_ERROR_LOADLANGUAGE ) ;
      char Message [200] ;
      sprintf ( Message, Format, LanguageName ) ;
      ResourceString Title ( Data->Library->QueryHandle(), IDS_TITLE ) ;
      MessageBox ( HWND_DESKTOP, HWND_DESKTOP, PSZ(Message),
         PSZ(Title), IDD_ERROR_LOADLANGUAGE, MB_ENTER | MB_ICONEXCLAMATION | MB_HELP, LibraryHandle, IDS_MESSAGE_BOX_STRINGS ) ;

   } /* endif */

  /**************************************************************************
   * Return to where you were.                                              *
   **************************************************************************/

   _chdrive ( OldDrive ) ;
   _chdir ( "\\" ) ;
   _chdir ( OldDirectory ) ;

  /**************************************************************************
   * Done.                                                                  *
   **************************************************************************/

   return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process Clear Clipboard command.                                    *
 *                                                                          *
 ****************************************************************************/

STATIC MRESULT APIENTRY ClearClipboard ( HWND Window, MESG, MPARAM1, MPARAM2 ) {

  /**************************************************************************
   * Empty the clipboard.                                                   *
   **************************************************************************/

   WinOpenClipbrd ( WinQueryAnchorBlock(Window) ) ;
   WinEmptyClipbrd ( WinQueryAnchorBlock(Window) ) ;
   WinCloseClipbrd ( WinQueryAnchorBlock(Window) ) ;

  /**************************************************************************
   * Done.                                                                  *
   **************************************************************************/

   return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process Hide On Screen Clip command.                                *
 *                                                                          *
 ****************************************************************************/

STATIC MRESULT APIENTRY HideOnScreenClip ( HWND Window, MESG, MPARAM1, MPARAM2 ) {

  /**************************************************************************
   * Find the instance data.                                                *
   **************************************************************************/

   PDATA Data = PDATA ( Sys_GetWindowData ( Window ) ) ;

  /**************************************************************************
   * Toggle the state of the hide-on-screen-clip flag.                      *
   **************************************************************************/

   Data->IniData->HideOnScreenClip = NOT Data->IniData->HideOnScreenClip ;

   MENUITEM MenuItem ;
   Sys_SendMessage ( CHILD(PARENT(Window),FID_MENU), MM_QUERYITEM,
      MPFROM2SHORT(IDM_PASTE_FROM_SCRN,TRUE), MPFROMP(&MenuItem) ) ;

   CheckMenuItem ( MenuItem.hwndSubMenu, IDM_PASTE_FROM_SCRN_HIDE, Data->IniData->HideOnScreenClip ) ;

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

 /***************************************************************************
  * Find the instance data.                                                 *
  ***************************************************************************/

  PDATA Data = PDATA ( Sys_GetWindowData ( Window ) ) ;

 /***************************************************************************
  * Process the Fonts Page Up command.                                      *
  ***************************************************************************/

  Data->FontMenuIndex = USHORT ( max ( 0, Data->FontMenuIndex - 20 ) ) ;

  HWND Frame = PARENT ( Window ) ;
  HWND Menu = CHILD ( Frame, FID_MENU ) ;
  Sys_SendMessage ( Menu, MM_SELECTITEM, MPFROM2SHORT(IDM_FONT_MENU,TRUE), MPFROMSHORT(FALSE) ) ;

 /***************************************************************************
  * Done.                                                                   *
  ***************************************************************************/

  return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process Font Menu Page Down command.                                *
 *                                                                          *
 ****************************************************************************/

STATIC MRESULT APIENTRY FontsPageDown ( HWND Window, MESG, MPARAM1, MPARAM2 ) {

 /***************************************************************************
  * Find the instance data.                                                 *
  ***************************************************************************/

  PDATA Data = PDATA ( Sys_GetWindowData ( Window ) ) ;

 /***************************************************************************
  * Process the Fonts Page Down command.                                    *
  ***************************************************************************/

  Data->FontMenuIndex += 20 ;

  HWND Frame = PARENT ( Window ) ;
  HWND Menu = CHILD ( Frame, FID_MENU ) ;
  Sys_SendMessage ( Menu, MM_SELECTITEM, MPFROM2SHORT(IDM_FONT_MENU,TRUE), MPFROMSHORT(FALSE) ) ;

 /***************************************************************************
  * Done.                                                                   *
  ***************************************************************************/

  return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process Font Manager command.                                       *
 *                                                                          *
 ****************************************************************************/

STATIC MRESULT APIENTRY FontManager ( HWND, MESG, MPARAM1, MPARAM2 ) {

 /***************************************************************************
  * Process the Size Manager command.                                       *
  ***************************************************************************/

  Sys_BeepNote ( ) ;

 /***************************************************************************
  * Done.                                                                   *
  ***************************************************************************/

  return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process Size Menu Page Up command.                                  *
 *                                                                          *
 ****************************************************************************/

STATIC MRESULT APIENTRY SizesPageUp ( HWND Window, MESG, MPARAM1, MPARAM2 ) {

 /***************************************************************************
  * Find the instance data.                                                 *
  ***************************************************************************/

  PDATA Data = PDATA ( Sys_GetWindowData ( Window ) ) ;

 /***************************************************************************
  * Process the Sizes Page Up command.                                      *
  ***************************************************************************/

  Data->SizeMenuIndex = USHORT ( max ( 0, Data->SizeMenuIndex - 20 ) ) ;

  HWND Frame = PARENT ( Window ) ;
  HWND Menu = CHILD ( Frame, FID_MENU ) ;
  Sys_SendMessage ( Menu, MM_SELECTITEM, MPFROM2SHORT(IDM_SIZE_MENU,TRUE), MPFROMSHORT(FALSE) ) ;

 /***************************************************************************
  * Done.                                                                   *
  ***************************************************************************/

  return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process Size Menu Page Down command.                                *
 *                                                                          *
 ****************************************************************************/

STATIC MRESULT APIENTRY SizesPageDown ( HWND Window, MESG, MPARAM1, MPARAM2 ) {

 /***************************************************************************
  * Find the instance data.                                                 *
  ***************************************************************************/

  PDATA Data = PDATA ( Sys_GetWindowData ( Window ) ) ;

 /***************************************************************************
  * Process the Sizes Page Down command.                                    *
  ***************************************************************************/

  Data->SizeMenuIndex += 20 ;

  HWND Frame = PARENT ( Window ) ;
  HWND Menu = CHILD ( Frame, FID_MENU ) ;
  Sys_SendMessage ( Menu, MM_SELECTITEM, MPFROM2SHORT(IDM_SIZE_MENU,TRUE), MPFROMSHORT(FALSE) ) ;

 /***************************************************************************
  * Done.                                                                   *
  ***************************************************************************/

  return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process Size Manager command.                                       *
 *                                                                          *
 ****************************************************************************/

STATIC MRESULT APIENTRY SizeManager ( HWND, MESG, MPARAM1, MPARAM2 ) {

 /***************************************************************************
  * Process the Size Manager command.                                       *
  ***************************************************************************/

  Sys_BeepNote ( ) ;

 /***************************************************************************
  * Done.                                                                   *
  ***************************************************************************/

  return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process Expand Menu Page Up command.                                *
 *                                                                          *
 ****************************************************************************/

STATIC MRESULT APIENTRY ExpandPageUp ( HWND Window, MESG, MPARAM1, MPARAM2 ) {

 /***************************************************************************
  * Find the instance data.                                                 *
  ***************************************************************************/

  PDATA Data = PDATA ( Sys_GetWindowData ( Window ) ) ;

 /***************************************************************************
  * Process the Expand Menu Page Up command.                                *
  ***************************************************************************/

  Data->ExpandMenuIndex = USHORT ( max ( 0, Data->ExpandMenuIndex - 20 ) ) ;

  HWND Frame = PARENT ( Window ) ;
  HWND Menu = CHILD ( Frame, FID_MENU ) ;
  Sys_SendMessage ( Menu, MM_SELECTITEM, MPFROM2SHORT(IDM_EXPAND_MENU,TRUE), MPFROMSHORT(FALSE) ) ;

 /***************************************************************************
  * Done.                                                                   *
  ***************************************************************************/

  return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process Expand Menu Page Down command.                              *
 *                                                                          *
 ****************************************************************************/

STATIC MRESULT APIENTRY ExpandPageDown ( HWND Window, MESG, MPARAM1, MPARAM2 ) {

 /***************************************************************************
  * Find the instance data.                                                 *
  ***************************************************************************/

  PDATA Data = PDATA ( Sys_GetWindowData ( Window ) ) ;

 /***************************************************************************
  * Process the Expansions Page Down command.                               *
  ***************************************************************************/

  Data->ExpandMenuIndex += 20 ;

  HWND Frame = PARENT ( Window ) ;
  HWND Menu = CHILD ( Frame, FID_MENU ) ;
  Sys_SendMessage ( Menu, MM_SELECTITEM, MPFROM2SHORT(IDM_EXPAND_MENU,TRUE), MPFROMSHORT(FALSE) ) ;

 /***************************************************************************
  * Done.                                                                   *
  ***************************************************************************/

  return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process Expansion Menu Manager command.                             *
 *                                                                          *
 ****************************************************************************/

STATIC MRESULT APIENTRY ExpandManager ( HWND, MESG, MPARAM1, MPARAM2 ) {

 /***************************************************************************
  * Process the Expand Manager command.                                     *
  ***************************************************************************/

  Sys_BeepNote ( ) ;

 /***************************************************************************
  * Done.                                                                   *
  ***************************************************************************/

  return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process Line Color Menu Page Up command.                            *
 *                                                                          *
 ****************************************************************************/

STATIC MRESULT APIENTRY LineColorsPageUp ( HWND Window, MESG, MPARAM1, MPARAM2 ) {

 /***************************************************************************
  * Find the instance data.                                                 *
  ***************************************************************************/

  PDATA Data = PDATA ( Sys_GetWindowData ( Window ) ) ;

 /***************************************************************************
  * Process the Line Colors Page Up command.                                *
  ***************************************************************************/

  Data->LineColorMenuIndex = USHORT ( max ( 0, Data->LineColorMenuIndex - 20 ) ) ;

  HWND Frame = PARENT ( Window ) ;
  HWND Menu = CHILD ( Frame, FID_MENU ) ;
  Sys_SendMessage ( Menu, MM_SELECTITEM, MPFROM2SHORT(IDM_LINECOLOR_MENU,TRUE), MPFROMSHORT(FALSE) ) ;

 /***************************************************************************
  * Done.                                                                   *
  ***************************************************************************/

  return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process Line Color Menu Page Down command.                          *
 *                                                                          *
 ****************************************************************************/

STATIC MRESULT APIENTRY LineColorsPageDown ( HWND Window, MESG, MPARAM1, MPARAM2 ) {

 /***************************************************************************
  * Find the instance data.                                                 *
  ***************************************************************************/

  PDATA Data = PDATA ( Sys_GetWindowData ( Window ) ) ;

 /***************************************************************************
  * Process the Colors Page Down command.                                   *
  ***************************************************************************/

  Data->LineColorMenuIndex += 20 ;

  HWND Frame = PARENT ( Window ) ;
  HWND Menu = CHILD ( Frame, FID_MENU ) ;
  Sys_SendMessage ( Menu, MM_SELECTITEM, MPFROM2SHORT(IDM_LINECOLOR_MENU,TRUE), MPFROMSHORT(FALSE) ) ;

 /***************************************************************************
  * Done.                                                                   *
  ***************************************************************************/

  return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process Line Color Manager command.                                 *
 *                                                                          *
 ****************************************************************************/

STATIC MRESULT APIENTRY LineColorManager ( HWND, MESG, MPARAM1, MPARAM2 ) {

 /***************************************************************************
  * Process the Line Color Manager command.                                 *
  ***************************************************************************/

  Sys_BeepNote ( ) ;

 /***************************************************************************
  * Done.                                                                   *
  ***************************************************************************/

  return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process Line Width Menu Page Up command.                            *
 *                                                                          *
 ****************************************************************************/

STATIC MRESULT APIENTRY LineWidthsPageUp ( HWND Window, MESG, MPARAM1, MPARAM2 ) {

 /***************************************************************************
  * Find the instance data.                                                 *
  ***************************************************************************/

  PDATA Data = PDATA ( Sys_GetWindowData ( Window ) ) ;

 /***************************************************************************
  * Process the Line Width Page Up command.                                 *
  ***************************************************************************/

  Data->LineWidthMenuIndex = USHORT ( max ( 0, Data->LineWidthMenuIndex - 20 ) ) ;

  HWND Frame = PARENT ( Window ) ;
  HWND Menu = CHILD ( Frame, FID_MENU ) ;
  Sys_SendMessage ( Menu, MM_SELECTITEM, MPFROM2SHORT(IDM_LINEWIDTH_MENU,TRUE), MPFROMSHORT(FALSE) ) ;

 /***************************************************************************
  * Done.                                                                   *
  ***************************************************************************/

  return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process Line Width Menu Page Down command.                          *
 *                                                                          *
 ****************************************************************************/

STATIC MRESULT APIENTRY LineWidthsPageDown ( HWND Window, MESG, MPARAM1, MPARAM2 ) {

 /***************************************************************************
  * Find the instance data.                                                 *
  ***************************************************************************/

  PDATA Data = PDATA ( Sys_GetWindowData ( Window ) ) ;

 /***************************************************************************
  * Process the Colors Page Down command.                                   *
  ***************************************************************************/

  Data->LineWidthMenuIndex += 20 ;

  HWND Frame = PARENT ( Window ) ;
  HWND Menu = CHILD ( Frame, FID_MENU ) ;
  Sys_SendMessage ( Menu, MM_SELECTITEM, MPFROM2SHORT(IDM_LINEWIDTH_MENU,TRUE), MPFROMSHORT(FALSE) ) ;

 /***************************************************************************
  * Done.                                                                   *
  ***************************************************************************/

  return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process Line Width Manager command.                                 *
 *                                                                          *
 ****************************************************************************/

STATIC MRESULT APIENTRY LineWidthManager ( HWND, MESG, MPARAM1, MPARAM2 ) {

 /***************************************************************************
  * Process the Line Width Manager command.                                 *
  ***************************************************************************/

  Sys_BeepNote ( ) ;

 /***************************************************************************
  * Done.                                                                   *
  ***************************************************************************/

  return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process Fill Color Menu Page Up command.                            *
 *                                                                          *
 ****************************************************************************/

STATIC MRESULT APIENTRY FillColorsPageUp ( HWND Window, MESG, MPARAM1, MPARAM2 ) {

 /***************************************************************************
  * Find the instance data.                                                 *
  ***************************************************************************/

  PDATA Data = PDATA ( Sys_GetWindowData ( Window ) ) ;

 /***************************************************************************
  * Process the Fill Colors Page Up command.                                *
  ***************************************************************************/

  Data->FillColorMenuIndex = USHORT ( max ( 0, Data->FillColorMenuIndex - 20 ) ) ;

  HWND Frame = PARENT ( Window ) ;
  HWND Menu = CHILD ( Frame, FID_MENU ) ;
  Sys_SendMessage ( Menu, MM_SELECTITEM, MPFROM2SHORT(IDM_FILLCOLOR_MENU,TRUE), MPFROMSHORT(FALSE) ) ;

 /***************************************************************************
  * Done.                                                                   *
  ***************************************************************************/

  return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process Fill Color Menu Page Down command.                          *
 *                                                                          *
 ****************************************************************************/

STATIC MRESULT APIENTRY FillColorsPageDown ( HWND Window, MESG, MPARAM1, MPARAM2 ) {

 /***************************************************************************
  * Find the instance data.                                                 *
  ***************************************************************************/

  PDATA Data = PDATA ( Sys_GetWindowData ( Window ) ) ;

 /***************************************************************************
  * Process the Fill Colors Page Down command.                              *
  ***************************************************************************/

  Data->FillColorMenuIndex += 20 ;

  HWND Frame = PARENT ( Window ) ;
  HWND Menu = CHILD ( Frame, FID_MENU ) ;
  Sys_SendMessage ( Menu, MM_SELECTITEM, MPFROM2SHORT(IDM_FILLCOLOR_MENU,TRUE), MPFROMSHORT(FALSE) ) ;

 /***************************************************************************
  * Done.                                                                   *
  ***************************************************************************/

  return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process Fill Color Manager command.                                 *
 *                                                                          *
 ****************************************************************************/

STATIC MRESULT APIENTRY FillColorManager ( HWND, MESG, MPARAM1, MPARAM2 ) {

 /***************************************************************************
  * Process the Fill Color Manager command.                                 *
  ***************************************************************************/

  Sys_BeepNote ( ) ;

 /***************************************************************************
  * Done.                                                                   *
  ***************************************************************************/

  return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process Fill Type Menu Page Up command.                             *
 *                                                                          *
 ****************************************************************************/

STATIC MRESULT APIENTRY FillTypesPageUp ( HWND Window, MESG, MPARAM1, MPARAM2 ) {

 /***************************************************************************
  * Find the instance data.                                                 *
  ***************************************************************************/

  PDATA Data = PDATA ( Sys_GetWindowData ( Window ) ) ;

 /***************************************************************************
  * Process the Colors Page Up command.                                     *
  ***************************************************************************/

  Data->FillTypeMenuIndex = USHORT ( max ( 0, Data->FillTypeMenuIndex - 20 ) ) ;

  HWND Frame = PARENT ( Window ) ;
  HWND Menu = CHILD ( Frame, FID_MENU ) ;
  Sys_SendMessage ( Menu, MM_SELECTITEM, MPFROM2SHORT(IDM_FILLTYPE_MENU,TRUE), MPFROMSHORT(FALSE) ) ;

 /***************************************************************************
  * Done.                                                                   *
  ***************************************************************************/

  return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process Fill Type Menu Page Down command.                           *
 *                                                                          *
 ****************************************************************************/

STATIC MRESULT APIENTRY FillTypesPageDown ( HWND Window, MESG, MPARAM1, MPARAM2 ) {

 /***************************************************************************
  * Find the instance data.                                                 *
  ***************************************************************************/

  PDATA Data = PDATA ( Sys_GetWindowData ( Window ) ) ;

 /***************************************************************************
  * Process the Line Type Page Down command.                                *
  ***************************************************************************/

  Data->FillTypeMenuIndex += 20 ;

  HWND Frame = PARENT ( Window ) ;
  HWND Menu = CHILD ( Frame, FID_MENU ) ;
  Sys_SendMessage ( Menu, MM_SELECTITEM, MPFROM2SHORT(IDM_FILLTYPE_MENU,TRUE), MPFROMSHORT(FALSE) ) ;

 /***************************************************************************
  * Done.                                                                   *
  ***************************************************************************/

  return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process Fill Type Manager command.                                  *
 *                                                                          *
 ****************************************************************************/

STATIC MRESULT APIENTRY FillTypeManager ( HWND, MESG, MPARAM1, MPARAM2 ) {

 /***************************************************************************
  * Process the Fill Type Manager command.                                  *
  ***************************************************************************/

  Sys_BeepNote ( ) ;

 /***************************************************************************
  * Done.                                                                   *
  ***************************************************************************/

  return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process Line End Menu Page Up command.                              *
 *                                                                          *
 ****************************************************************************/

STATIC MRESULT APIENTRY LineEndsPageUp ( HWND Window, MESG, MPARAM1, MPARAM2 ) {

 /***************************************************************************
  * Find the instance data.                                                 *
  ***************************************************************************/

  PDATA Data = PDATA ( Sys_GetWindowData ( Window ) ) ;

 /***************************************************************************
  * Process the Colors Page Up command.                                     *
  ***************************************************************************/

  Data->LineEndMenuIndex = USHORT ( max ( 0, Data->LineEndMenuIndex - 20 ) ) ;

  HWND Frame = PARENT ( Window ) ;
  HWND Menu = CHILD ( Frame, FID_MENU ) ;
  Sys_SendMessage ( Menu, MM_SELECTITEM, MPFROM2SHORT(IDM_LINEEND_MENU,TRUE), MPFROMSHORT(FALSE) ) ;

 /***************************************************************************
  * Done.                                                                   *
  ***************************************************************************/

  return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process Line End Menu Page Down command.                           *
 *                                                                          *
 ****************************************************************************/

STATIC MRESULT APIENTRY LineEndsPageDown ( HWND Window, MESG, MPARAM1, MPARAM2 ) {

 /***************************************************************************
  * Find the instance data.                                                 *
  ***************************************************************************/

  PDATA Data = PDATA ( Sys_GetWindowData ( Window ) ) ;

 /***************************************************************************
  * Process the Line Type Page Down command.                                *
  ***************************************************************************/

  Data->LineEndMenuIndex += 20 ;

  HWND Frame = PARENT ( Window ) ;
  HWND Menu = CHILD ( Frame, FID_MENU ) ;
  Sys_SendMessage ( Menu, MM_SELECTITEM, MPFROM2SHORT(IDM_LINEEND_MENU,TRUE), MPFROMSHORT(FALSE) ) ;

 /***************************************************************************
  * Done.                                                                   *
  ***************************************************************************/

  return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process Line End Manager command.                                  *
 *                                                                          *
 ****************************************************************************/

STATIC MRESULT APIENTRY LineEndManager ( HWND, MESG, MPARAM1, MPARAM2 ) {

 /***************************************************************************
  * Process the Line End Manager command.                                  *
  ***************************************************************************/

  Sys_BeepNote ( ) ;

 /***************************************************************************
  * Done.                                                                   *
  ***************************************************************************/

  return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process Line Join Menu Page Up command.                             *
 *                                                                          *
 ****************************************************************************/

STATIC MRESULT APIENTRY LineJoinsPageUp ( HWND Window, MESG, MPARAM1, MPARAM2 ) {

 /***************************************************************************
  * Find the instance data.                                                 *
  ***************************************************************************/

  PDATA Data = PDATA ( Sys_GetWindowData ( Window ) ) ;

 /***************************************************************************
  * Process the Colors Page Up command.                                     *
  ***************************************************************************/

  Data->LineJoinMenuIndex = USHORT ( max ( 0, Data->LineJoinMenuIndex - 20 ) ) ;

  HWND Frame = PARENT ( Window ) ;
  HWND Menu = CHILD ( Frame, FID_MENU ) ;
  Sys_SendMessage ( Menu, MM_SELECTITEM, MPFROM2SHORT(IDM_LINEJOIN_MENU,TRUE), MPFROMSHORT(FALSE) ) ;

 /***************************************************************************
  * Done.                                                                   *
  ***************************************************************************/

  return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process Line Join Menu Page Down command.                           *
 *                                                                          *
 ****************************************************************************/

STATIC MRESULT APIENTRY LineJoinsPageDown ( HWND Window, MESG, MPARAM1, MPARAM2 ) {

 /***************************************************************************
  * Find the instance data.                                                 *
  ***************************************************************************/

  PDATA Data = PDATA ( Sys_GetWindowData ( Window ) ) ;

 /***************************************************************************
  * Process the Line Type Page Down command.                                *
  ***************************************************************************/

  Data->LineJoinMenuIndex += 20 ;

  HWND Frame = PARENT ( Window ) ;
  HWND Menu = CHILD ( Frame, FID_MENU ) ;
  Sys_SendMessage ( Menu, MM_SELECTITEM, MPFROM2SHORT(IDM_LINEJOIN_MENU,TRUE), MPFROMSHORT(FALSE) ) ;

 /***************************************************************************
  * Done.                                                                   *
  ***************************************************************************/

  return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process Line Join Manager command.                                  *
 *                                                                          *
 ****************************************************************************/

STATIC MRESULT APIENTRY LineJoinManager ( HWND, MESG, MPARAM1, MPARAM2 ) {

 /***************************************************************************
  * Process the Line Join Manager command.                                  *
  ***************************************************************************/

  Sys_BeepNote ( ) ;

 /***************************************************************************
  * Done.                                                                   *
  ***************************************************************************/

  return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process Window Tile command.                                        *
 *                                                                          *
 ****************************************************************************/

STATIC MRESULT APIENTRY WindowTile ( HWND Window, MESG, MPARAM1, MPARAM2 ) {

 /***************************************************************************
  * Find the instance data.                                                 *
  ***************************************************************************/

  PDATA Data = PDATA ( Sys_GetWindowData ( Window ) ) ;

 /***************************************************************************
  * Arrange the icons first.                                                *
  ***************************************************************************/

  WindowArrange ( Window, 0, 0, 0 ) ;

 /***************************************************************************
  * Enumerate the child windows from the top down.  Ignore those minimized. *
  ***************************************************************************/

  SHORT Count = 0 ;
  SHORT IconHeight = 0 ;
  HENUM EnumerationHandle = WinBeginEnumWindows ( Window ) ;
  SWP Windows [MAX_DOCUMENTS] ;

  HWND DocWindow = WinGetNextWindow ( EnumerationHandle ) ;
  while ( DocWindow ) {
     for ( int i=0; i<MAX_DOCUMENTS; i++ )
        if ( DocWindow == Data->DataWindows[i].Window )
           break ;

     if ( i < MAX_DOCUMENTS ) {
        WinQueryWindowPos ( DocWindow, &Windows[Count] ) ;
        if ( Windows[Count].fl & SWP_MINIMIZE ) {
           IconHeight = SHORT ( max ( IconHeight, Windows[Count].y+Windows[Count].cy ) ) ;
        } else {
           Count ++ ;
        } /* endif */
     } /* endif */
     DocWindow = WinGetNextWindow ( EnumerationHandle ) ;
  }
  WinEndEnumWindows ( EnumerationHandle ) ;

 /***************************************************************************
  * If no windows are to be shuffled, return at once.                       *
  ***************************************************************************/

  if ( Count == 0 )
    return ( 0 ) ;

 /***************************************************************************
  * Determine their new positions.                                          *
  ***************************************************************************/

  RECTL Rectangle ;
  WinQueryWindowRect ( Window, &Rectangle ) ;

  SHORT Rows = 1 ;
  SHORT Columns = 1 ;
  while ( Rows*Columns < Count ) {
    if ( Columns > Rows )
      Rows ++ ;
    else
      Columns ++ ;
  } /* endwhile */

  SHORT StepX = SHORT ( ( Rectangle.xRight - Rectangle.xLeft ) / Columns ) ;
  SHORT StepY = SHORT ( ( Rectangle.yTop - Rectangle.yBottom - IconHeight ) / Rows ) ;
  SHORT Extra = SHORT ( Rows*Columns - Count ) ;

  int i = 0 ;
  for ( short Row=0; Row<Rows; Row++ ) {
    for ( short Column=0; Column<Columns; Column++ ) {
      if ( i < Count ) {
        SWP Position ;
        WinQueryWindowPos ( Windows[i].hwnd, &Position ) ;
        Windows[i].x  = SHORT ( Rectangle.xLeft + Column*StepX ) ;
        Windows[i].y  = SHORT ( Rectangle.yTop - (Row+1)*StepY ) ;
        Windows[i].cx = SHORT ( StepX * ( Extra + 1 ) ) ;
        Windows[i].cy = SHORT ( StepY ) ;
        Windows[i].fl = SWP_SIZE | SWP_MOVE | ( ( Position.fl & SWP_MAXIMIZE ) ? SWP_RESTORE : 0 ) ;
        Column += Extra ;
        Extra = 0 ;
      }
      i ++ ;
    }
  }

 /***************************************************************************
  * Set all the window positions as a group.                                *
  ***************************************************************************/

  WinSetMultWindowPos ( WinQueryAnchorBlock(Window), Windows, Count ) ;

 /***************************************************************************
  * Done.                                                                   *
  ***************************************************************************/

  return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process Window Cascade command.                                     *
 *                                                                          *
 ****************************************************************************/

STATIC MRESULT APIENTRY WindowCascade ( HWND Window, MESG, MPARAM1, MPARAM2 ) {

 /***************************************************************************
  * Find the instance data.                                                 *
  ***************************************************************************/

  PDATA Data = PDATA ( Sys_GetWindowData ( Window ) ) ;

 /***************************************************************************
  * Arrange the icons first.                                                *
  ***************************************************************************/

  WindowArrange ( Window, 0, 0, 0 ) ;

 /***************************************************************************
  * Enumerate the child windows from the top down.  Ignore those minimized. *
  ***************************************************************************/

  int Count = 0 ;
  int IconHeight = 0 ;
  HENUM EnumerationHandle = WinBeginEnumWindows ( Window ) ;
  SWP Windows [MAX_DOCUMENTS] ;
  HWND DocWindow = WinGetNextWindow ( EnumerationHandle ) ;
  while ( DocWindow ) {
     for ( int i=0; i<MAX_DOCUMENTS; i++ )
        if ( DocWindow == Data->DataWindows[i].Window )
           break ;

     if ( i < MAX_DOCUMENTS ) {
        WinQueryWindowPos ( DocWindow, &Windows[Count] ) ;
        if ( Windows[Count].fl & SWP_MINIMIZE ) {
           IconHeight = int ( max ( IconHeight, Windows[Count].y+Windows[Count].cy ) ) ;
        } else {
           Count ++ ;
        }
     }

     DocWindow = WinGetNextWindow ( EnumerationHandle ) ;
  }
  WinEndEnumWindows ( EnumerationHandle ) ;

 /***************************************************************************
  * If no windows are to be shuffled, return at once.                       *
  ***************************************************************************/

  if ( Count == 0 )
    return ( 0 ) ;

 /***************************************************************************
  * Determine their new positions.                                          *
  ***************************************************************************/

  RECTL Rectangle ;
  WinQueryWindowRect ( Window, &Rectangle ) ;

  LONG cxSizeBorder = WinQuerySysValue ( HWND_DESKTOP, SV_CXSIZEBORDER ) ;
  LONG cySizeBorder = WinQuerySysValue ( HWND_DESKTOP, SV_CYSIZEBORDER ) ;
  LONG cyTitleBar   = WinQuerySysValue ( HWND_DESKTOP, SV_CYTITLEBAR ) ;

  SHORT StepX = SHORT ( cxSizeBorder ) ;
  SHORT StepY = SHORT ( cySizeBorder + cyTitleBar ) ;

  for ( int i=0; i<Count; i++ ) {
     Windows[i].x  = SHORT ( Rectangle.xLeft + (Count-i-1)*StepX ) ;
     Windows[i].y  = SHORT ( Rectangle.yBottom + IconHeight + i*StepY ) ;
     Windows[i].cx = SHORT ( Rectangle.xRight - Rectangle.xLeft - (Count-1)*StepX ) ;
     Windows[i].cy = SHORT ( Rectangle.yTop - Rectangle.yBottom - IconHeight - (Count-1)*StepY ) ;
     Windows[i].fl = SWP_SIZE | SWP_MOVE | SWP_RESTORE ;
  } /* endfor */

 /***************************************************************************
  * Set all the window positions as a group.                                *
  ***************************************************************************/

  WinSetMultWindowPos ( WinQueryAnchorBlock(Window), Windows, Count ) ;

 /***************************************************************************
  * Done.                                                                   *
  ***************************************************************************/

  return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process Window Arrange Icons command.                               *
 *                                                                          *
 ****************************************************************************/

STATIC MRESULT APIENTRY WindowArrange ( HWND Window, MESG, MPARAM1, MPARAM2 ) {

 /***************************************************************************
  * Find the instance data.                                                 *
  ***************************************************************************/

  PDATA Data = PDATA ( Sys_GetWindowData ( Window ) ) ;

 /***************************************************************************
  * Enumerate the child windows from the top down.  Note those minimized.   *
  ***************************************************************************/

  int Count = 0 ;
  HENUM EnumerationHandle = WinBeginEnumWindows ( Window ) ;
  SWP Windows [MAX_DOCUMENTS] ;
  HWND DocWindow = WinGetNextWindow ( EnumerationHandle ) ;
  while ( DocWindow )  {
     for ( int i=0; i<MAX_DOCUMENTS; i++ )
        if ( DocWindow == Data->DataWindows[i].Window )
           break ;

     if ( i < MAX_DOCUMENTS ) {
        WinQueryWindowPos ( DocWindow, &Windows[Count] ) ;
        if ( Windows[Count].fl & SWP_MINIMIZE ) {
           Count ++ ;
        } /* endif */
     } /* endif */

     DocWindow = WinGetNextWindow ( EnumerationHandle ) ;
  } /* endwhile */
  WinEndEnumWindows ( EnumerationHandle ) ;

 /***************************************************************************
  * If no icons are to be shuffled, return at once.                         *
  ***************************************************************************/

  if ( Count == 0 )
    return ( 0 ) ;

 /***************************************************************************
  * Determine their new positions.                                          *
  ***************************************************************************/

  RECTL Rectangle ;
  WinQueryWindowRect ( Window, &Rectangle ) ;

  SHORT x = 35 ;
  SHORT y = 32 ;
  SHORT dx = 100 ;
  SHORT dy = 88 ;
  SHORT RightMargin = SHORT ( dx - x ) ;

  for ( int i=0; i<Count; i++ ) {
     Windows[i].x  = SHORT ( Rectangle.xLeft + x ) ;
     Windows[i].y  = SHORT ( Rectangle.yBottom + y ) ;
     Windows[i].fl = SWP_MOVE ;

     x += dx ;
     if ( x > SHORT ( Rectangle.xRight - RightMargin ) ) {
        x = 35 ;
        y += dy ;
     } /* endif */
  } /* endfor */

 /***************************************************************************
  * Set all the window positions as a group.                                *
  ***************************************************************************/

  WinSetMultWindowPos ( WinQueryAnchorBlock(Window), Windows, Count ) ;

 /***************************************************************************
  * Done.                                                                   *
  ***************************************************************************/

  return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process About menu command.                                         *
 *                                                                          *
 ****************************************************************************/

class About_Dialog : public Dialog {
   public:
      About_Dialog ( HWND Parent, HWND Owner, HMODULE Resource, int ID, char *Name, char *Number ) :
         Dialog ( Parent, Owner, Resource, ID ) {
         if ( !Load ( ) ) 
            return ;
         if ( Name && Name[0] ) {
            Sys_SetWindowText ( CHILD(Handle,1), Name ) ;
            Sys_SetWindowText ( CHILD(Handle,2), Number ) ;
            COLOR Color = WinQuerySysColor ( HWND_DESKTOP, SYSCLR_WINDOWSTATICTEXT, 0 ) ;
            WinSetPresParam ( CHILD(Handle,1), PP_FOREGROUNDCOLOR, sizeof(Color), &Color ) ;
            WinSetPresParam ( CHILD(Handle,2), PP_FOREGROUNDCOLOR, sizeof(Color), &Color ) ;
         } /* endif */
      } /* endif */
} ;

STATIC MRESULT APIENTRY About ( HWND Window, MESG, MPARAM1, MPARAM2 ) {

 /***************************************************************************
  * Find the instance data.                                                 *
  ***************************************************************************/

  PDATA Data = PDATA ( Sys_GetWindowData ( Window ) ) ;

 /***************************************************************************
  * Get registration information, if any.                                   *
  ***************************************************************************/

  char Name [200] = { 0 } ;
  char Number [100] = { 0 } ;

  Registration Info ( Data->Registration->QueryHandle() ) ;
  if ( Info.IsValid() ) {
     ResourceString RegisteredTo ( Data->Library->QueryHandle(), IDS_REGISTERED_TO ) ;
     strcpy ( Name, PCHAR(RegisteredTo) ) ;
     strcat ( Name, Info.Query_Name() ) ;
     ResourceString SerialNumber ( Data->Library->QueryHandle(), IDS_SERIAL_NUMBER ) ;
     sprintf ( Number, "%s%06lu", PSZ(SerialNumber), Info.Query_Serial() ) ;
  } /* endif */

 /***************************************************************************
  * Invoke the About dialog.                                                *
  ***************************************************************************/

  About_Dialog ThisDialog ( HWND_DESKTOP, Window, Data->Library->QueryHandle(), IDD_ABOUT, Name, Number ) ;
  ThisDialog.Process ( ) ;

 /***************************************************************************
  * Done.                                                                   *
  ***************************************************************************/

  return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process Character Input.                                            *
 *                                                                          *
 ****************************************************************************/

STATIC MRESULT APIENTRY ContextMenu ( HWND Window, MESG, MPARAM1 mp1, MPARAM2 ) {

  /**************************************************************************
   * Find the instance data.                                                *
   **************************************************************************/

   PDATA Data = PDATA ( Sys_GetWindowData ( Window ) ) ;

  /**************************************************************************
   * Figure out where the menu's going to go.                               *
   **************************************************************************/

   POINTL Point ;
   if ( !mp1 ) {        // If SHIFT+F10, get the mouse pointer location directly.
      WinQueryPointerPos ( HWND_DESKTOP, &Point ) ;
   } else {
      Point.x = SHORT1FROMMP ( mp1 ) ;
      Point.y = SHORT2FROMMP ( mp1 ) ;
      WinMapWindowPoints ( Window, HWND_DESKTOP, &Point, 1 ) ;
   } /* endif */

  /**************************************************************************
   * Invoke the window's context menu.                                      *
   **************************************************************************/

   WinPopupMenu ( HWND_DESKTOP, Window, Data->ContextMenu, Point.x, Point.y, 0,
      PU_NONE | PU_HCONSTRAIN | PU_VCONSTRAIN | PU_KEYBOARD | PU_MOUSEBUTTON1 | PU_MOUSEBUTTON2 ) ;

  /**************************************************************************
   * Done.                                                                  *
   **************************************************************************/

   return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process System Color Change notification.                           *
 *                                                                          *
 ****************************************************************************/

STATIC MRESULT APIENTRY SysColorChange ( HWND Window, MESG, MPARAM1, MPARAM2 ) {

 /***************************************************************************
  * Find the instance data.                                                 *
  ***************************************************************************/

  PDATA Data = PDATA ( Sys_GetWindowData ( Window ) ) ;

 /***************************************************************************
  * If we aren't using custom colors, then query for the new defaults.      *
  ***************************************************************************/

  if ( NOT Data->IniData->fMainColors[0] ) 
     Data->IniData->MainColors[0] = WinQuerySysColor ( HWND_DESKTOP, SYSCLR_APPWORKSPACE, 0 ) ;

 /***************************************************************************
  * Return value must be NULL, according to the documentation.              *
  ***************************************************************************/

  return ( MRFROMP ( NULL ) ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process Query for Keys Help resource id.                            *
 *                                                                          *
 ****************************************************************************/

STATIC MRESULT APIENTRY QueryKeysHelp ( HWND, MESG, MPARAM1, MPARAM2 ) {

 /***************************************************************************
  * Simply return the ID of the Keys Help panel.                            *
  ***************************************************************************/

  return ( (MRESULT) IDM_KEYS_HELP ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process Help Manager Error                                          *
 *                                                                          *
 ****************************************************************************/

STATIC MRESULT APIENTRY HelpError ( HWND Window, MESG, MPARAM1 mp1, MPARAM2 ) {

 /***************************************************************************
  * Find the instance data.                                                 *
  ***************************************************************************/

  PDATA Data = PDATA ( Sys_GetWindowData ( Window ) ) ;

 /***************************************************************************
  * Find the error code in the message table.                               *
  ***************************************************************************/

  static struct {
    ULONG Error ;
    USHORT StringId ;
  } HelpErrors [] = {
    { HMERR_NO_FRAME_WND_IN_CHAIN,     IDS_HMERR_NO_FRAME_WND_IN_CHAIN },
    { HMERR_INVALID_ASSOC_APP_WND,     IDS_HMERR_INVALID_ASSOC_APP_WND },
    { HMERR_INVALID_ASSOC_HELP_INST,   IDS_HMERR_INVALID_ASSOC_HELP_IN },
    { HMERR_INVALID_DESTROY_HELP_INST, IDS_HMERR_INVALID_DESTROY_HELP_ },
    { HMERR_NO_HELP_INST_IN_CHAIN,     IDS_HMERR_NO_HELP_INST_IN_CHAIN },
    { HMERR_INVALID_HELP_INSTANCE_HDL, IDS_HMERR_INVALID_HELP_INSTANCE },
    { HMERR_INVALID_QUERY_APP_WND,     IDS_HMERR_INVALID_QUERY_APP_WND },
    { HMERR_HELP_INST_CALLED_INVALID,  IDS_HMERR_HELP_INST_CALLED_INVA },
    { HMERR_HELPTABLE_UNDEFINE,        IDS_HMERR_HELPTABLE_UNDEFINE    },
    { HMERR_HELP_INSTANCE_UNDEFINE,    IDS_HMERR_HELP_INSTANCE_UNDEFIN },
    { HMERR_HELPITEM_NOT_FOUND,        IDS_HMERR_HELPITEM_NOT_FOUND    },
    { HMERR_INVALID_HELPSUBITEM_SIZE,  IDS_HMERR_INVALID_HELPSUBITEM_S },
    { HMERR_HELPSUBITEM_NOT_FOUND,     IDS_HMERR_HELPSUBITEM_NOT_FOUND },
    { HMERR_INDEX_NOT_FOUND,           IDS_HMERR_INDEX_NOT_FOUND       },
    { HMERR_CONTENT_NOT_FOUND,         IDS_HMERR_CONTENT_NOT_FOUND     },
    { HMERR_OPEN_LIB_FILE,             IDS_HMERR_OPEN_LIB_FILE         },
    { HMERR_READ_LIB_FILE,             IDS_HMERR_READ_LIB_FILE         },
    { HMERR_CLOSE_LIB_FILE,            IDS_HMERR_CLOSE_LIB_FILE        },
    { HMERR_INVALID_LIB_FILE,          IDS_HMERR_INVALID_LIB_FILE      },
    { HMERR_NO_MEMORY,                 IDS_HMERR_NO_MEMORY             },
    { HMERR_ALLOCATE_SEGMENT,          IDS_HMERR_ALLOCATE_SEGMENT      },
    { HMERR_FREE_MEMORY,               IDS_HMERR_FREE_MEMORY           },
    { HMERR_PANEL_NOT_FOUND,           IDS_HMERR_PANEL_NOT_FOUND       },
    { HMERR_DATABASE_NOT_OPEN,         IDS_HMERR_DATABASE_NOT_OPEN     },
    { 0,                               IDS_HMERR_UNKNOWN               }
  } ;

  ULONG ErrorCode = (ULONG) LONGFROMMP ( mp1 ) ;
  int Index = 0 ;
  while ( HelpErrors[Index].Error
    AND ( HelpErrors[Index].Error != ErrorCode ) ) {
    Index ++ ;
  }

 /***************************************************************************
  * Get the message texts.                                                  *
  ***************************************************************************/

  ResourceString Title ( Data->Library->QueryHandle(), IDS_HMERR ) ;
  ResourceString Message ( Data->Library->QueryHandle(), HelpErrors[Index].StringId ) ;

 /***************************************************************************
  * Display the error message.                                              *
  ***************************************************************************/

  MessageBox ( HWND_DESKTOP, Window, PSZ(Message), PSZ(Title),
    IDD_ERROR_HELPMANAGER, MB_OK | MB_WARNING | MB_HELP, LibraryHandle, IDS_MESSAGE_BOX_STRINGS ) ;

 /***************************************************************************
  * Return zero, indicating that the message was processed.                 *
  ***************************************************************************/

  return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process "Extended Help Undefined" notification                      *
 *                                                                          *
 ****************************************************************************/

STATIC MRESULT APIENTRY ExtHelpUndefined ( HWND Window, MESG, MPARAM1, MPARAM2 ) {

 /***************************************************************************
  * Find the instance data.                                                 *
  ***************************************************************************/

  PDATA Data = PDATA ( Sys_GetWindowData ( Window ) ) ;

 /***************************************************************************
  * Get the message texts.                                                  *
  ***************************************************************************/

  ResourceString Title ( Data->Library->QueryHandle(), IDS_HMERR ) ;
  ResourceString Message ( Data->Library->QueryHandle(), IDS_HMERR_EXTHELPUNDEFINED ) ;

 /***************************************************************************
  * Display the error message.                                              *
  ***************************************************************************/

  MessageBox ( HWND_DESKTOP, Window, PSZ(Message), PSZ(Title),
    IDD_ERROR_HELPMANAGER, MB_OK | MB_WARNING | MB_HELP, LibraryHandle, IDS_MESSAGE_BOX_STRINGS ) ;

 /***************************************************************************
  * Return zero, indicating that the message was processed.                 *
  ***************************************************************************/

  return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process "Help Subitem Not Found" notification                       *
 *                                                                          *
 ****************************************************************************/

STATIC MRESULT APIENTRY HelpSubitemNotFound ( HWND Window, MESG, MPARAM1 mp1, MPARAM2 mp2 ) {

  /**************************************************************************
   * Find the instance data.                                                *
   **************************************************************************/

   PDATA Data = PDATA ( Sys_GetWindowData ( Window ) ) ;

  /**************************************************************************
   * Interpret the help code, if possible.                                  *
   **************************************************************************/

   USHORT Subtopic = USHORT ( SHORT2FROMMP ( mp2 ) ) ;

   struct {
      short Low ;
      short High ;
      short HelpTopic ;
   } Table [] = {
      { IDM_RECALL0,      IDM_RECALLMAX,        IDM_RECALL0          },
      { IDM_LANGUAGES,    IDM_LANGUAGES_MAX,    IDM_LANGUAGES        },
      { IDM_LINESPACINGS, IDM_LINESPACINGS_MAX, IDM_LINESPACINGS     },
      { IDM_FONTS,        IDM_FONTS_MAX,        IDM_FONTS            },
      { IDM_SIZES,        IDM_SIZES_MAX,        IDM_SIZES            },
      { IDM_EXPANDS,      IDM_EXPANDS_MAX,      IDM_EXPANDS          },
      { IDM_LINECOLORS,   IDM_LINECOLORS_MAX,   IDM_LINECOLORS       },
      { IDM_LINEWIDTHS,   IDM_LINEWIDTHS_MAX,   IDM_LINEWIDTHS       },
      { IDM_FILLCOLORS,   IDM_FILLCOLORS_MAX,   IDM_FILLCOLORS       },
      { IDM_FILLTYPES,    IDM_FILLTYPES_MAX,    IDM_FILLTYPES        },
      { IDM_LINEENDS,     IDM_LINEENDS_MAX,     IDM_LINEENDS         },
      { IDM_LINEJOINS,    IDM_LINEJOINS_MAX,    IDM_LINEJOINS        },
      { IDM_WINDOWS,      IDM_WINDOWS_MAX,      IDM_WINDOWS          },
      { IDM_PRODUCT_LIST, IDM_PRODUCT_MAX,      IDM_ABOUT            },
   } ;

   for ( int i=0; i<sizeof(Table)/sizeof(Table[0]); i++ ) {
      if ( ( Subtopic >= Table[i].Low ) AND ( Subtopic <= Table[i].High ) ) {
         Sys_SendMessage ( WinQueryHelpInstance(Window), HM_DISPLAY_HELP, MPFROMSHORT(Table[i].HelpTopic), MPFROMSHORT(HM_RESOURCEID) ) ;
         return ( MRFROMSHORT ( TRUE ) ) ;
      } /* endif */
   } /* endfor */

  /**************************************************************************
   * Format an error message.                                               *
   **************************************************************************/

   USHORT Topic = USHORT ( SHORT1FROMMP ( mp2 ) ) ;

   i = SHORT1FROMMP ( mp1 ) ;

   int ModeIndex = IDS_HELPMODE_UNKNOWN ;
   switch ( i ) {
      case HLPM_FRAME:
         ModeIndex = IDS_HELPMODE_FRAME ;
         break ;

      case HLPM_MENU:
         ModeIndex = IDS_HELPMODE_MENU ;
         break ;

      case HLPM_WINDOW:
         ModeIndex = IDS_HELPMODE_WINDOW ;
         break ;
   }

   ResourceString Mode ( Data->Library->QueryHandle(), ModeIndex ) ;
   ResourceString Format ( Data->Library->QueryHandle(), IDS_HELPSUBITEMNOTFOUND ) ;

   UCHAR Message [200] ;
   sprintf ( PCHAR(Message), PCHAR(Format), PSZ(Mode), Topic, Subtopic ) ;

  /**************************************************************************
   * Display the error message.                                             *
   **************************************************************************/

   ResourceString Title ( Data->Library->QueryHandle(), IDS_HMERR ) ;

   MessageBox ( HWND_DESKTOP, Window, PSZ(Message), PSZ(Title),
      IDD_ERROR_HELPMANAGER, MB_OK | MB_WARNING | MB_HELP, LibraryHandle, IDS_MESSAGE_BOX_STRINGS ) ;

  /**************************************************************************
   * Return FALSE, indicating that general help is to be displayed.         *
   **************************************************************************/

   return ( MRFROMSHORT ( FALSE ) ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process Drag Over Notification                                      *
 *                                                                          *
 ****************************************************************************/

STATIC MRESULT APIENTRY DragOver ( HWND, MESG, MPARAM1 mp1, MPARAM2 ) {

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
         StartLoad ( FullPath, Data, Window, 0 ) ;
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
 *      Process "Load File" request.                                        *
 *                                                                          *
 ****************************************************************************/

STATIC MRESULT APIENTRY LoadFile ( HWND Window, MESG, MPARAM1 mp1, MPARAM2 mp2 ) {

  /**************************************************************************
   * Find the instance data.                                                *
   **************************************************************************/

// PDATA Data = PDATA ( Sys_GetWindowData ( Window ) ) ;

  /**************************************************************************
   * Get the message parameters.                                            *
   **************************************************************************/

   char *InitialPath = (char*) PVOIDFROMMP(mp1) ;
   char *CommandLine = (char*) PVOIDFROMMP(mp2) ;

  /**************************************************************************
   * Build the argc/argv arrays from the command-line.                      *
   **************************************************************************/

   int argc = 1 ;
   char *args[100] ;
   char *p = CommandLine ;
   while ( *p ) {
      args[argc++] = p ;
      p += strlen(p) + 1 ;
   } /* endwhile */

  /**************************************************************************
   * Process the command-line.                                              *
   **************************************************************************/

   LoadFiles ( Window, InitialPath, argc, args ) ;

  /**************************************************************************
   * Release the shared memory allocated for this message.                  *
   **************************************************************************/

   DosFreeMem ( PVOIDFROMMP(mp1) ) ;
   DosFreeMem ( PVOIDFROMMP(mp2) ) ;

  /**************************************************************************
   * Done.                                                                  *
   **************************************************************************/

   return ( 0 ) ;
}

/****************************************************************************
 *                                                                          *
 *      Process "Load Done" notification                                    *
 *                                                                          *
 ****************************************************************************/

STATIC MRESULT APIENTRY LoadDone ( HWND Window, MESG, MPARAM1 mp1, MPARAM2 mp2 ) {

  /**************************************************************************
   * Find the instance data.                                                *
   **************************************************************************/

   PDATA Data = PDATA ( Sys_GetWindowData ( Window ) ) ;

  /**************************************************************************
   * Get the message data.                                                  *
   **************************************************************************/

   BOOL Created = BOOL ( SHORT1FROMMP ( mp1 ) ) ;
   short MessageID = SHORT2FROMMP ( mp1 ) ;
   Documento *pDocument = (Documento*) mp2 ;

  /**************************************************************************
   * Decrement the number of loads active.                                  *
   **************************************************************************/

   if ( Data->LoadsActive ) {
      Data->LoadsActive -- ;
      if ( Data->LoadsActive == 0 ) {
         Sys_ShowWindow ( Data->LoadStatusWindow, FALSE ) ;
      } /* endif */
   } /* endif */

  /**************************************************************************
   * If no more things active, and the program's closing, tell it to quit.  *
   **************************************************************************/

   if ( !Data->LoadsActive AND !Data->ThreadsActive AND Data->Closing ) {
      Sys_PostMessage ( Window, WM_QUIT, 0, 0 ) ;
   } /* endif */

  /**************************************************************************
   * If error was returned, report the error and return.                    *
   **************************************************************************/

   if ( NOT Created ) {
      ResourceString Message ( Data->Library->QueryHandle(), MessageID ) ;
      Debug ( Window, "%s", PSZ(Message) ) ;
      return ( 0 ) ;
   } /* endif */

  /**************************************************************************
   * If any new and unchanged document windows are present, destroy them.   *
   **************************************************************************/

   for ( int i=0; i<MAX_DOCUMENTS; i++ ) {
      HWND Window = Data->DataWindows[i].Window ;
      if ( Window == 0 )
         continue ;
      if ( Data->DataWindows[i].pDocument->Query_Filename() == 0 ) {
         if ( Data->DataWindows[i].pDocument->IsChanged() == FALSE ) {
            delete Data->DataWindows[i].pDocument ;
            memset ( &Data->DataWindows[i], 0, sizeof(Data->DataWindows[i]) ) ;
            if ( i == Data->ActiveWindow ) 
               Data->ActiveWindow = -1 ;
         } /* endif */
      } /* endif */
   } /* endfor */

  /**************************************************************************
   * Find a slot in the data window list.  If unable to, destroy the        *
   *   document and give an error message.                                  *
   **************************************************************************/

   for ( i=0; i<MAX_DOCUMENTS; i++ ) {
      if ( Data->DataWindows[i].Window == 0 )
         break ;
   } /* endfor */

   if ( i >= MAX_DOCUMENTS ) {
      ResourceString Title ( Data->Library->QueryHandle(), IDS_TITLE ) ;
      ResourceString Format ( Data->Library->QueryHandle(), IDS_ERROR_TOOMANYLOADED ) ;
      char Message [200] ;
      sprintf ( Message, PCHAR(Format), pDocument->Query_Filename() ) ;
      MessageBox ( HWND_DESKTOP, Window, PSZ(Message),
         PSZ(Title), IDD_ERROR_TOOMANYLOADED, MB_ENTER | MB_ICONEXCLAMATION | MB_HELP, LibraryHandle, IDS_MESSAGE_BOX_STRINGS ) ;
      delete pDocument ;
      return ( 0 ) ;
   } /* endif */

  /**************************************************************************
   * Create the data window.                                                *
   **************************************************************************/

   Data->DataWindows[i].pDocument = pDocument ;
   Data->DataWindows[i].Window = pDocument->CreateWindow ( Window, Data->IniData, Data->Speller, 100+i, int(Data->Trap) ) ;

  /**************************************************************************
   * Set the window position.  If it will be the active window, maximize    *
   *   it and make it active.  If not, minimize it.                         *
   **************************************************************************/

   SWP Position ;
   WinQueryWindowPos ( Data->DataWindows[i].Window, &Position ) ;
   if ( Data->ActiveWindow == -1 ) {
      Data->ActiveWindow = short ( i ) ;
      WinSetWindowPos ( Data->DataWindows[i].Window, HWND_TOP,
         Position.x, Position.y, Position.cx, Position.cy,
         SWP_MOVE | SWP_SIZE | SWP_ZORDER | SWP_MAXIMIZE | SWP_SHOW | SWP_ACTIVATE ) ;
   } else {
      SWP ActivePosition ;
      WinQueryWindowPos ( Data->DataWindows[Data->ActiveWindow].Window, &ActivePosition ) ;
      if ( ActivePosition.fl & SWP_MINIMIZE ) {
         Data->ActiveWindow = short ( i ) ;
         WinSetWindowPos ( Data->DataWindows[i].Window, HWND_TOP,
            Position.x, Position.y, Position.cx, Position.cy,
            SWP_ZORDER | SWP_SHOW | SWP_ACTIVATE ) ;
      } else {
         WinSetWindowPos ( Data->DataWindows[i].Window, HWND_BOTTOM,
            Position.x, Position.y, Position.cx, Position.cy,
            SWP_MOVE | SWP_SIZE | SWP_ZORDER | SWP_MINIMIZE | SWP_SHOW | SWP_DEACTIVATE ) ;
      } /* endif */
      WindowTile ( Window, 0, 0, 0 ) ;
   } /* endif */

  /**************************************************************************
   * Update the menu, now that we have a current document.                  *
   **************************************************************************/

   UpdateMenu ( Window, Data ) ;

  /**************************************************************************
   * Done.                                                                  *
   **************************************************************************/

   return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process "Thread Started" notification                               *
 *                                                                          *
 ****************************************************************************/

STATIC MRESULT APIENTRY ThreadStarted ( HWND Window, MESG, MPARAM1, MPARAM2 ) {

  /**************************************************************************
   * Find the instance data.                                                *
   **************************************************************************/

   PDATA Data = PDATA ( Sys_GetWindowData ( Window ) ) ;

  /**************************************************************************
   * Increment the number of threads active.                                *
   **************************************************************************/

   Data->ThreadsActive ++ ;

  /**************************************************************************
   * Done.                                                                  *
   **************************************************************************/

   return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process "Thread Done" notification                                  *
 *                                                                          *
 ****************************************************************************/

STATIC MRESULT APIENTRY ThreadDone ( HWND Window, MESG, MPARAM1, MPARAM2 ) {

  /**************************************************************************
   * Find the instance data.                                                *
   **************************************************************************/

   PDATA Data = PDATA ( Sys_GetWindowData ( Window ) ) ;

  /**************************************************************************
   * Decrement the number of threads active.                                *
   **************************************************************************/

   if ( Data->ThreadsActive ) 
      Data->ThreadsActive -- ;

  /**************************************************************************
   * If no more things active, and the program's closing, tell it to quit.  *
   **************************************************************************/

   if ( !Data->LoadsActive AND !Data->ThreadsActive AND Data->Closing ) 
      Sys_PostMessage ( Window, WM_QUIT, 0, 0 ) ;

  /**************************************************************************
   * Done.                                                                  *
   **************************************************************************/

   return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process "Activated" notification                                    *
 *                                                                          *
 ****************************************************************************/

STATIC MRESULT APIENTRY Activated ( HWND Window, MESG, MPARAM1 mp1, MPARAM2 ) {

  /**************************************************************************
   * Find the instance data.                                                *
   **************************************************************************/

   PDATA Data = PDATA ( Sys_GetWindowData ( Window ) ) ;

  /**************************************************************************
   * Record which window reported itself as activated.                      *
   **************************************************************************/

   for ( int i=0; i<MAX_DOCUMENTS; i++ ) {
      if ( Data->DataWindows[i].Window == HWNDFROMMP(mp1) )
         break ;
   } /* endfor */

   if ( i < MAX_DOCUMENTS ) {
      Data->ActiveWindow = short ( i ) ;
   } /* endif */

  /**************************************************************************
   * Update the menu, now that we have a current document.                  *
   **************************************************************************/

   UpdateMenu ( Window, Data ) ;

  /***************************************************************************
   * Done.                                                                   *
   ***************************************************************************/

   return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process "Close Me" request.                                         *
 *                                                                          *
 ****************************************************************************/

STATIC MRESULT APIENTRY CloseMe ( HWND Window, MESG, MPARAM1 mp1, MPARAM2 ) {

  /**************************************************************************
   * Find the instance data.                                                *
   **************************************************************************/

   PDATA Data = PDATA ( Sys_GetWindowData ( Window ) ) ;

  /**************************************************************************
   * Get the message data.                                                  *
   **************************************************************************/

   HWND DocFrame = HWND ( mp1 ) ;

  /**************************************************************************
   * Determine which window requested closing.                              *
   **************************************************************************/

   for ( int i=0; i<MAX_DOCUMENTS; i++ ) {
      if ( Data->DataWindows[i].Window == DocFrame )
         break ;
   } /* endif */

   if ( i >= MAX_DOCUMENTS ) {
      HMQ Queue = HMQ ( WinQueryWindowULong ( DocFrame, QWL_HMQ ) ) ;
      WinPostQueueMsg ( Queue, WM_QUIT, 0, 0 ) ;
      return ( 0 ) ;
   } /* endif */

   Documento *pDocument = Data->DataWindows[Data->ActiveWindow].pDocument ;

  /**************************************************************************
   * If the document had a name, update the document recall list.           *
   **************************************************************************/

   PSZ Name = pDocument->Query_Filename() ;
   if ( Name AND *Name )
      Data->IniData->UpdateFileList ( Name ) ;

  /**************************************************************************
   * Reset the active window's information.                                 *
   **************************************************************************/

   memset ( &Data->DataWindows[Data->ActiveWindow], 0, sizeof(Data->DataWindows[Data->ActiveWindow]) ) ;
   Data->ActiveWindow = -1 ;

  /**************************************************************************
   * Destroy the document.  It will destroy the window.                     *
   **************************************************************************/

   delete pDocument ;

  /**************************************************************************
   * Update the menu, now that we might not have a current document anymore.*
   **************************************************************************/

   UpdateMenu ( Window, Data ) ;

  /**************************************************************************
   * Done.                                                                  *
   **************************************************************************/

   return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process "Set Document Colors" request.                              *
 *                                                                          *
 ****************************************************************************/

STATIC MRESULT APIENTRY SetDocColors ( HWND Window, MESG, MPARAM1 mp1, MPARAM2 mp2 ) {

  /**************************************************************************
   * Find the instance data.                                                *
   **************************************************************************/

   PDATA Data = PDATA ( Sys_GetWindowData ( Window ) ) ;

  /**************************************************************************
   * Send the new colors to all the open documents.                         *
   **************************************************************************/

   COLOR Colors[2] = { COLOR(mp1), COLOR(mp2) } ;

   for ( int i=0; i<MAX_DOCUMENTS; i++ ) {
      if ( Data->DataWindows[i].Window ) {
         Data->DataWindows[i].pDocument->Set_Colors ( Colors ) ;
         Sys_InvalidateWindow ( Data->DataWindows[i].Window ) ;
      } /* endif */
   } /* endif */

  /**************************************************************************
   * Done.                                                                  *
   **************************************************************************/

   return ( 0 ) ;
}

 
/****************************************************************************
 *                                                                          *
 *                        Update Menu Statuses                              *
 *                                                                          *
 ****************************************************************************/

STATIC VOID UpdateMenu ( HWND Window, PDATA Data ) {

  /**************************************************************************
   * Get the menu handle.                                                   *
   **************************************************************************/

   HWND Frame = PARENT ( Window ) ;
   HWND Menu = CHILD ( Frame, FID_MENU ) ;

  /**************************************************************************
   * Get the document pointer.                                              *
   **************************************************************************/

   PVOID pDocument = 0 ;

   if ( Data->ActiveWindow != -1 ) 
      pDocument = Data->DataWindows[Data->ActiveWindow].pDocument ;

  /**************************************************************************
   * Enable/disable the menus that require an active document.              *
   **************************************************************************/

   EnableMenuItem ( Menu, IDM_EDIT_MENU,           pDocument != NULL ) ;
   EnableMenuItem ( Menu, IDM_LAYOUT_MENU,         pDocument != NULL ) ;
   EnableMenuItem ( Menu, IDM_FONT_MENU,           pDocument != NULL ) ;
   EnableMenuItem ( Menu, IDM_SIZE_MENU,           pDocument != NULL ) ;
   EnableMenuItem ( Menu, IDM_ATTR_MENU,           pDocument != NULL ) ;
   EnableMenuItem ( Menu, IDM_WINDOW_MENU,         pDocument != NULL ) ;

   EnableMenuItem ( Data->ContextMenu, IDM_WINDOW_MENU, pDocument != NULL ) ;
}
 
/****************************************************************************
 *                                                                          *
 *  Initialize Recall Menu                                                  *
 *                                                                          *
 ****************************************************************************/

STATIC void InitRecallMenu ( HWND, PDATA Data, HWND Menu, int, int ListID ) {

  /**************************************************************************
   * Remove all the old menu entries.                                       *
   **************************************************************************/

   for ( int i=0; i<MAX_RECALL; i++ ) {
      Sys_SendMessage ( Menu, MM_REMOVEITEM, MPFROM2SHORT(ListID+i,TRUE), 0 ) ;
   } /* endfor */

  /**************************************************************************
   * Initialize the menu item structure.                                    *
   **************************************************************************/

   MENUITEM MenuItem ;
   MenuItem.iPosition = MIT_END ;
   MenuItem.afStyle = MIS_TEXT ;
   MenuItem.afAttribute = 0 ;
   MenuItem.hwndSubMenu = 0 ;

  /**************************************************************************
   * Build the recall menu entries.                                         *
   **************************************************************************/

   Data->IniData->FileListLock.Request ( ) ;

   for ( i=0; i<MAX_RECALL; i++ ) {

     /***********************************************************************
      * Skip this entry if null.                                            *
      ***********************************************************************/

      if ( ( Data->IniData->Files[i] == NULL ) || ( Data->IniData->Files[i][0] == 0 ) ) 
         continue ;

     /***********************************************************************
      * Skip this entry if the file's drive cannot be accessed.             *
      ***********************************************************************/

      char Drive [_MAX_DRIVE + 1 ] ;
      char Dir   [_MAX_DIR   + 1 ] ;
      char Name  [_MAX_FNAME + 1 ] ;
      char Ext   [_MAX_EXT   + 1 ] ;
      _splitpath ( PCHAR(Data->IniData->Files[i]), Drive, Dir, Name, Ext ) ;
      strupr ( Drive ) ;
      int Type ;
      if ( CheckDrive ( Drive[0]-'A'+1, Type ) )
         continue ;

     /***********************************************************************
      * Skip this entry if the file doesn't exist.                          *
      ***********************************************************************/

      FILESTATUS3 Status ;
      if ( DosQueryPathInfo ( Data->IniData->Files[i], FIL_STANDARD, &Status, sizeof(Status) ) )
         continue ;

      if ( Status.attrFile & FILE_DIRECTORY )
         continue ;

     /***********************************************************************
      * Add the menu entry.                                                 *
      ***********************************************************************/

      MenuItem.id = USHORT ( ListID + i ) ;
      Sys_SendMessage ( Menu, MM_INSERTITEM, MPFROMP(&MenuItem), MPFROMP(Data->IniData->Files[i]) ) ;

   } /* endfor */

   Data->IniData->FileListLock.Release ( ) ;
 
}
 
/****************************************************************************
 *                                                                          *
 *  Initialize Language Menu                                                *
 *                                                                          *
 ****************************************************************************/

STATIC void InitLanguageMenu ( HWND, PDATA Data, HWND Menu, int, int ListID ) {

  /**************************************************************************
   * Remove all the old menu entries.                                       *
   **************************************************************************/

   for ( int i=0; i<(IDM_LANGUAGES_MAX-IDM_LANGUAGES+1); i++ )
      Sys_SendMessage ( Menu, MM_REMOVEITEM, MPFROM2SHORT(ListID+i,TRUE), 0 ) ;

  /**************************************************************************
   * Build a list of the available language DLLs, including the default.    *
   **************************************************************************/

   // Change to home directory, saving current directory.
   int OldDrive = _getdrive ( ) ;
   char OldDirectory [ CCHMAXPATH ] ;
   getcwd ( OldDirectory, sizeof(OldDirectory) ) ;
   char Drive [_MAX_DRIVE+1], Dir[_MAX_DIR+1], Fname[_MAX_FNAME+1], Ext[_MAX_EXT+1] ;
   _splitpath ( HomePath, Drive, Dir, Fname, Ext ) ;
   strcat ( Dir, Fname ) ;
   strcat ( Dir, Ext ) ;
   #ifdef DEBUG_INITLANGUAGEMENU
      Log ( "InitLanguageMenu: Changing to %c:%s.", Drive[0], Dir ) ;
   #endif // DEBUG_INITLANGUAGEMENU
   _chdrive ( Drive[0] - 'A' + 1 ) ;
   _chdir ( "\\" ) ;
   _chdir ( Dir ) ;

   // Reset the language count and rebuild the language list.
   #ifdef DEBUG_INITLANGUAGEMENU
      Log ( "InitLanguageMenu: Scanning for language modules." ) ;
   #endif // DEBUG_INITLANGUAGEMENU
   LanguageCount = 0 ;
   FILEFINDBUF3 Found ;
   HDIR hDir = (HDIR) HDIR_CREATE ;
   ULONG Count = 1 ;
   int Result = DosFindFirst2 ( "*.DLL", &hDir, FILE_NORMAL, &Found, sizeof(Found), &Count, FIL_STANDARD ) ;
   if ( Result == 0 ) {
      do {
         #ifdef DEBUG_INITLANGUAGEMENU
            Log ( "InitLanguageMenu: Checking module '%s'.", Found.achName ) ;
         #endif // DEBUG_INITLANGUAGEMENU
         // Is it a valid DLL?  Continue if not.
         LanguageModule Library ( PROGRAM_NAME, Found.achName, FALSE ) ;
         if ( Library.QueryHandle() == 0 ) {
            #ifdef DEBUG_INITLANGUAGEMENU
               Log ( "InitLanguageMenu: Language Module rejected as invalid." ) ;
            #endif // DEBUG_INITLANGUAGEMENU
            continue ;
         } /* endif */
         // If the revision right?  Continue if not.
         ResourceString Revision ( Library.QueryHandle(), IDS_TITLE1 ) ;
         if ( strcmp ( PCHAR(Revision), REVISION ) ) {
            #ifdef DEBUG_INITLANGUAGEMENU
               Log ( "InitLanguageMenu: Language Module rejected as having the wrong revision." ) ;
            #endif // DEBUG_INITLANGUAGEMENU
            continue ;
         } /* endif */
         // Save language name.  Abort if we have the max now.
         strcpy ( Languages[LanguageCount], Library.QueryLanguage() ) ;
         strcpy ( Libraries[LanguageCount], Found.achName ) ;
         LanguageCount ++ ;
         #ifdef DEBUG_INITLANGUAGEMENU
            Log ( "InitLanguageMenu: Language Module accepted." ) ;
         #endif // DEBUG_INITLANGUAGEMENU
         if ( LanguageCount >= sizeof(Languages)/sizeof(Languages[0]) )
            break ;
      } while ( !DosFindNext ( hDir, &Found, sizeof(Found), &Count ) ) ;
      DosFindClose ( hDir ) ;
   } else {
      #ifdef DEBUG_INITLANGUAGEMENU
         Log ( "InitLanguageMenu: Failed to find any DLLs.  Result %i.", Result ) ;
      #endif // DEBUG_INITLANGUAGEMENU
   } /* endif */
   #ifdef DEBUG_INITLANGUAGEMENU
      Log ( "InitLanguageMenu: Done with scan." ) ;
   #endif // DEBUG_INITLANGUAGEMENU

   // Restore current directory.
   #ifdef DEBUG_INITLANGUAGEMENU
      Log ( "InitLanguageMenu: Changing to %s.", OldDirectory ) ;
   #endif // DEBUG_INITLANGUAGEMENU
   _chdrive ( OldDrive ) ;
   _chdir ( "\\" ) ;
   _chdir ( OldDirectory ) ;

  /**************************************************************************
   * Initialize the menu item structure.                                    *
   **************************************************************************/

   MENUITEM MenuItem ;
   MenuItem.iPosition = MIT_END ;
   MenuItem.afStyle = MIS_TEXT ;
   MenuItem.afAttribute = 0 ;
   MenuItem.hwndSubMenu = 0 ;

  /**************************************************************************
   * Build the language menu entries.                                       *
   **************************************************************************/

   for ( i=0; i<LanguageCount; i++ ) {

      MenuItem.id = USHORT ( ListID + i ) ;

      Sys_SendMessage ( Menu, MM_INSERTITEM, MPFROMP(&MenuItem), MPFROMP(Languages[i]) ) ;

      if ( !strcmpi ( Languages[i], Data->IniData->Language ) )
         Sys_SendMessage ( Menu, MM_SETITEMATTR, MPFROM2SHORT(MenuItem.id,TRUE), MPFROM2SHORT(MIA_CHECKED,MIA_CHECKED) ) ;

   } /* endif */
}
 
/****************************************************************************
 *                                                                          *
 *  Initialize the Window Menu                                              *
 *                                                                          *
 ****************************************************************************/

STATIC void InitWindowMenu ( HWND, PDATA Data, HWND Menu, int, int ListID ) {

  /**************************************************************************
   * Remove all the old menu entries.                                       *
   **************************************************************************/

   for ( int i=0; i<Data->WindowMenuSize; i++ )
      Sys_SendMessage ( Menu, MM_REMOVEITEM, MPFROM2SHORT(ListID+i,TRUE), 0 ) ;

  /**************************************************************************
   * Initialize the menu item structure.                                    *
   **************************************************************************/

   MENUITEM MenuItem ;
   MenuItem.iPosition = MIT_END ;
   MenuItem.afAttribute = 0 ;
   MenuItem.hwndSubMenu = 0 ;
   MenuItem.hItem = 0 ;

  /**************************************************************************
   * For each document that there might be . . .                            *
   **************************************************************************/

   Data->WindowMenuSize = 0 ;
   MenuItem.id = USHORT ( ListID ) ;

   for ( i=0; i<MAX_DOCUMENTS; i++ ) {

     /***********************************************************************
      * If this document entry isn't active, skip to the next.              *
      ***********************************************************************/

      if ( Data->DataWindows[i].Window == 0 )
         continue ;

     /***********************************************************************
      * Get the document's name.                                            *
      ***********************************************************************/

      Documento *pDocument = Data->DataWindows[i].pDocument ;
      PSZ Name = pDocument->Query_Filename() ;

     /***********************************************************************
      * Get the document's author & create date.                            *
      ***********************************************************************/

      char *Author = 0 ;
      char CreateDate [80] = { 0 } ;
      if ( !strcmpi ( pDocument->Query_FileType(), "Escriba" ) ) {

         Author = pDocument->Query_Parms()->Author ;

         if ( pDocument->Query_Parms()->CreateDate )
            FormatDate ( pDocument->Query_Parms()->CreateDate, pDocument->Query_Parms()->DateFormat, CreateDate, sizeof(CreateDate) ) ;

      } /* endif */

     /***********************************************************************
      * If the document's name is null, use 'New Document' instead.         *
      ***********************************************************************/

      ResourceString NewDocument ( Data->Library->QueryHandle(), IDS_NEW_DOCUMENT ) ;

      if ( ( Name == NULL ) OR ( *Name == 0 ) )
         Name = PSZ(NewDocument) ;

     /***********************************************************************
      * Construct the menu entry text.                                      *
      ***********************************************************************/


      char Text [200] ;
      if ( Author AND *Author AND strcmp ( Author, Data->IniData->DocParms.Author ) ) {
         ResourceString Format ( Data->Library->QueryHandle(), IDS_WINDOW_BY_OTHER ) ;
         sprintf ( Text, PCHAR(Format), PSZ(Name), Author, CreateDate ) ;
      } else if ( CreateDate[0] ) {
         ResourceString Format ( Data->Library->QueryHandle(), IDS_WINDOW_BY_ME ) ;
         sprintf ( Text, PCHAR(Format), PSZ(Name), CreateDate ) ;
      } else {
         ResourceString Format ( Data->Library->QueryHandle(), IDS_WINDOW_BY_ME_NODATE ) ;
         sprintf ( Text, PCHAR(Format), PSZ(Name) ) ;
      } /* endif */

     /***********************************************************************
      * Set the item to break the menu into columns if necessary.           *
      ***********************************************************************/

      MenuItem.afStyle = MIS_TEXT ;
      if ( ( MenuItem.id-ListID ) AND ( ( ( MenuItem.id-ListID ) % 20 ) == 0 ) )
         MenuItem.afStyle |= MIS_BREAKSEPARATOR ;

     /***********************************************************************
      * Add the menu entry.                                                 *
      ***********************************************************************/

      Sys_SendMessage ( Menu, MM_INSERTITEM, MPFROMP(&MenuItem), MPFROMP(Text) ) ;

     /***********************************************************************
      * If this is the currently active window, check it.                   *
      ***********************************************************************/

      if ( Data->ActiveWindow == SHORT(i) )
         Sys_SendMessage ( Menu, MM_SETITEMATTR, MPFROM2SHORT(MenuItem.id,TRUE),
            MPFROM2SHORT(MIA_CHECKED,MIA_CHECKED) ) ;

     /***********************************************************************
      * Get the next menu item ID.                                          *
      ***********************************************************************/

      MenuItem.id ++ ;

     /***********************************************************************
      * Update the window menu size.                                        *
      ***********************************************************************/

      Data->WindowMenuSize ++ ;

   } /* endfor */
}
 
/****************************************************************************
 *                                                                          *
 *      Load files according to command-line parameters                     *
 *                                                                          *
 ****************************************************************************/

STATIC int LoadFiles ( HWND Window, char *InitialPath, int argc, char *argv[] ) {

  /**************************************************************************
   * Find the instance data.                                                *
   **************************************************************************/

   PDATA Data = PDATA ( Sys_GetWindowData ( Window ) ) ;

  /**************************************************************************
   * Process the command-line.                                              *
   **************************************************************************/

   int Loaded = 0 ;

   while ( --argc ) {

      argv ++ ;

      char Name [CCHMAXPATH] ;

      if ( *argv[0] == '\"' ) {
         strcpy ( Name, (*argv)+1 ) ;
         argc -- ;
         argv ++ ;
         while ( argc AND ( *((*argv)+strlen(*argv)-1) != '\"' ) ) {
            strcat ( Name, " " ) ;
            strcat ( Name, *argv ) ;
            argc -- ;
            argv ++ ;
         } /* endwhile */
         if ( argc ) {
            strcat ( Name, " " ) ;
            strcat ( Name, *argv ) ;
            Name[strlen(Name)-1] = 0 ;
         } /* endif */

      } else {
         strcpy ( Name, *argv ) ;

      } /* endif */

      char Drive [_MAX_DRIVE+1], DefaultDrive [_MAX_DRIVE+1] ;
      char Dir   [_MAX_DIR  +1], DefaultDir   [_MAX_DIR  +1] ;
      char FName [_MAX_FNAME + 1 ] ;
      char Ext   [_MAX_EXT   + 1 ] ;
      _splitpath ( InitialPath, DefaultDrive, DefaultDir, FName, Ext ) ;
      _splitpath ( Name, Drive, Dir, FName, Ext ) ;
      if ( Drive[0] OR Dir[0] ) {
         if ( Drive[0] && ( Drive[0] != DefaultDrive[0] ) ) {
            strcpy ( Name, Drive ) ;
         } else {
            strcpy ( Name, DefaultDrive ) ;
            if ( Dir[0] != '\\' )
               strcat ( Name, DefaultDir ) ;
         } /* endif */
         strcat ( Name, Dir ) ;
      } else {
         strcpy ( Name, DefaultDrive ) ;
         strcat ( Name, DefaultDir ) ;
      } /* endif */
      strcat ( Name, FName ) ;
      strcat ( Name, Ext ) ;

      HDIR hDir = HDIR_CREATE ;
      FILEFINDBUF3 Found ;
      ULONG Count = 1 ;
      if ( !DosFindFirst ( PSZ(Name), &hDir, FILE_NORMAL, &Found, sizeof(Found), &Count, FIL_STANDARD ) ) {
         do {
            _splitpath ( Name, Drive, Dir, FName, Ext ) ;
            char FullPath [CCHMAXPATH] ;
            strcpy ( FullPath, Drive ) ;
            strcat ( FullPath, Dir ) ;
            strcat ( FullPath, Found.achName ) ;
            if ( StartLoad ( PSZ(FullPath), Data, Window, 0 ) )
               Loaded ++ ;
         } while ( !DosFindNext ( hDir, &Found, sizeof(Found), &Count ) ) ; /* enddo */
         DosFindClose ( hDir ) ;
      } else {
         if ( StartLoad ( PSZ(Name), Data, Window, 0 ) )
            Loaded ++ ;
      } /* endif */

   } /* endwhile */

   return ( Loaded ) ;
}

/****************************************************************************
 *                                                                          *
 *      Start a File Load                                                   *
 *                                                                          *
 ****************************************************************************/

STATIC BOOL StartLoad ( PSZ Name, PDATA Data, HWND WindowToNotify, int FileTypeIndex ) {

  /**************************************************************************
   * Allocate memory for the load thread parameters.                        *
   **************************************************************************/

   PLOAD_PARMS Parms = PLOAD_PARMS ( malloc ( sizeof(LOAD_PARMS) ) ) ;

   if ( Parms == NULL ) {
      ResourceString Title ( Data->Library->QueryHandle(), IDS_TITLE ) ;
      ResourceString Format ( Data->Library->QueryHandle(), IDS_ERROR_FILEMEMORY ) ;
      char Message [300] ;
      sprintf ( Message, PCHAR(Format), Name ) ;
      MessageBox ( HWND_DESKTOP, WindowToNotify, PSZ(Message),
         PSZ(Title), 0, MB_ENTER | MB_ICONEXCLAMATION, LibraryHandle, IDS_MESSAGE_BOX_STRINGS ) ;
      return ( FALSE ) ;
   } /* endif */

  /**************************************************************************
   * Fully qualify the file name.                                           *
   **************************************************************************/

   char FullPath [CCHMAXPATH] ;
   _fullpath ( FullPath, PCHAR(Name), sizeof(FullPath) ) ;

  /**************************************************************************
   * Determine the file type, if it exists.  Otherwise default to native.   *
   **************************************************************************/

   PrepareFileTypeList ( ) ;

   if ( access ( FullPath, 0 ) == 0 ) {

      if ( FileTypeIndex == 0 )
         FileTypeIndex = GetFileTypeIndex ( WindowToNotify, FullPath, FileTypeList, FileExtenderList, 0 ) ;

      if ( FileTypeIndex == 0 ) {
         ResourceString Title ( Data->Library->QueryHandle(), IDS_TITLE ) ;
         ResourceString Format ( Data->Library->QueryHandle(), IDS_ERROR_NOFILETYPE ) ;
         char Message [300] ;
         sprintf ( Message, PCHAR(Format), FullPath ) ;
         MessageBox ( HWND_DESKTOP, WindowToNotify, PSZ(Message),
            PSZ(Title), 0, MB_ENTER | MB_ICONEXCLAMATION, LibraryHandle, IDS_MESSAGE_BOX_STRINGS ) ;
         return ( FALSE ) ;
      } /* endif */

   } else {

      char Drive[_MAX_DRIVE], Dir[_MAX_DIR], Fname[_MAX_FNAME], Ext[_MAX_EXT] ;
      _splitpath ( FullPath, Drive, Dir, Fname, Ext ) ;
      char Directory [CCHMAXPATH] ;
      strcpy ( Directory, Drive ) ;
      strcat ( Directory, Dir ) ;
      strcat ( Directory, "." ) ;
      struct stat Status ;
      if ( _stat ( Directory, &Status ) == 0 ) {
         if ( ! ( Status.st_mode & S_IFDIR ) ) {
            ResourceString Title ( Data->Library->QueryHandle(), IDS_TITLE ) ;
            ResourceString Format ( Data->Library->QueryHandle(), IDS_ERROR_INVALIDPATH ) ;
            char Message [300] ;
            sprintf ( Message, PCHAR(Format), Drive, Dir ) ;
            MessageBox ( HWND_DESKTOP, WindowToNotify, PSZ(Message),
               PSZ(Title), 0, MB_ENTER | MB_ICONEXCLAMATION, LibraryHandle, IDS_MESSAGE_BOX_STRINGS ) ;
            return ( FALSE ) ;
         } /* endif */
      } else {
         ResourceString Title ( Data->Library->QueryHandle(), IDS_TITLE ) ;
         ResourceString Format ( Data->Library->QueryHandle(), IDS_ERROR_NOPATH ) ;
         char Message [300] ;
         sprintf ( Message, PCHAR(Format), Drive, Dir ) ;
         MessageBox ( HWND_DESKTOP, WindowToNotify, PSZ(Message),
            PSZ(Title), 0, MB_ENTER | MB_ICONEXCLAMATION, LibraryHandle, IDS_MESSAGE_BOX_STRINGS ) ;
         return ( FALSE ) ;
      } /* endif */

      FileTypeIndex = 0 ;

   } /* endif */

  /**************************************************************************
   * Allocate memory to hold the file name.                                 *
   **************************************************************************/

   Parms->Filename = PSZ ( malloc ( strlen(FullPath) + 1 ) ) ;
   if ( Parms->Filename == NULL ) {
      free ( Parms ) ;
      ResourceString Title ( Data->Library->QueryHandle(), IDS_TITLE ) ;
      ResourceString Format ( Data->Library->QueryHandle(), IDS_ERROR_FILEMEMORY ) ;
      char Message [300] ;
      sprintf ( Message, PCHAR(Format), Name ) ;
      MessageBox ( HWND_DESKTOP, WindowToNotify, PSZ(Message),
         PSZ(Title), 0, MB_ENTER | MB_ICONEXCLAMATION, LibraryHandle, IDS_MESSAGE_BOX_STRINGS ) ;
      return ( FALSE ) ;
   } /* endif */

  /**************************************************************************
   * Initialize the load parameters structure.                              *
   **************************************************************************/

   strcpy ( PCHAR(Parms->Filename), FullPath ) ;
   Parms->Library = Data->Library ;
   Parms->WindowToNotify = WindowToNotify ;
   Parms->DocParms = Data->IniData->DocParms ;
   Parms->Importer = (ImportExport*) FileExtenderList [ FileTypeIndex > 0 ? FileTypeIndex : 1 ] ;

  /**************************************************************************
   * Make the load status window visible.                                   *
   **************************************************************************/

   Data->LoadsActive ++ ;
   PositionLoadStatusWindow ( PARENT(WindowToNotify), Data->LoadStatusWindow ) ;
   Sys_ShowWindow ( Data->LoadStatusWindow, TRUE ) ;

  /**************************************************************************
   * Start the load.                                                        *
   **************************************************************************/

   StartThread ( "LoadThread", LoadThread, 0x10000, Parms ) ;

  /**************************************************************************
   * Return apparent success.                                               *
   **************************************************************************/

   return ( TRUE ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      File Load Thread                                                    *
 *                                                                          *
 ****************************************************************************/

STATIC VOID LoadThread ( PVOID Parameter ) {

  /**************************************************************************
   * Get parameters.                                                        *
   **************************************************************************/

   PLOAD_PARMS Parms = PLOAD_PARMS ( Parameter ) ;

  /**************************************************************************
   * Connect to PM.                                                         *
   **************************************************************************/

   Process Proc ( "LoadThread", 0, Parms->Library->QueryHandle() ) ;

  /**************************************************************************
   * Create the new document object.                                        *
   **************************************************************************/

   Documento *pNew = new Documento ( Proc.QueryAnchor(), Parms->Library, Parms->DocParms, Parms->Filename, Parms->Importer ) ;

   if ( pNew == NULL ) {
      Sys_PostMessage ( Parms->WindowToNotify, WM_LOAD_DONE, MPFROM2SHORT(FALSE,IDS_ERROR_DOCUMENT_LOAD), 0 ) ;
   } else {
      Sys_PostMessage ( Parms->WindowToNotify, WM_LOAD_DONE, MPFROMSHORT(TRUE), MPFROMP(pNew) ) ;
   } /* endif */

  /**************************************************************************
   * Release the memory allocated for the thread parameters.                *
   **************************************************************************/

   free ( Parms->Filename ) ;
   free ( Parms ) ;
}

 
/****************************************************************************
 *                                                                          *
 *      Position the Load Status Window.                                    *
 *                                                                          *
 ****************************************************************************/

STATIC void PositionLoadStatusWindow ( HWND Frame, HWND LoadStatusWindow ) {

   HWND Titlebar = CHILD ( Frame, FID_TITLEBAR ) ;

   SWP Position ;
   WinQueryWindowPos ( Titlebar, &Position ) ;

   RECTL TitleRectangle ;
   GetTitlebarRect ( WinQueryAnchorBlock(Frame), Titlebar, TitleRectangle ) ;
   Position.cx = TitleRectangle.xRight - TitleRectangle.xLeft ;

   WinSetWindowPos ( LoadStatusWindow, 0,
     Position.cx-Position.cy*6, 1, Position.cy*6-1, Position.cy-1,
     SWP_MOVE | SWP_SIZE ) ;
}
