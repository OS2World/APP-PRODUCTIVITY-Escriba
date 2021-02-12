/*************************************************************** DEFAULTS.CPP
 *                                                                          *
 *                     New Documents Defaults Dialog                        *
 *                                                                          *
 ****************************************************************************/

#include "System.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "DateFmt.h"
#include "Debug.h"
#include "Defaults.h"
#include "Dialog2.h"
#include "Document.h"
#include "Escriba.h"
#include "Indent.h"
#include "Margins.h"
#include "Printer.h"
#include "ReString.h"
#include "Support.h"
#include "Window.h"
#include "WrkSpace.h"

// #define DEBUG
// #define DEBUG_MESSAGES

 
/****************************************************************************
 *                                                                          *
 *                     Definitions & Declarations                           *
 *                                                                          *
 ****************************************************************************/

  // Function Prototypes

static WINDOWPROC DefaultsProcessor ;

static WINDOWPROC InitDlg ;
static WINDOWPROC FormatFrame ;
static WINDOWPROC Control ;
static WINDOWPROC Command ;
static WINDOWPROC OK ;
static WINDOWPROC Apply ;
static WINDOWPROC Cancel ;
static WINDOWPROC Settings ;

static BOOL EditAll ( HWND hwnd, PDEFAULTS_PARMS Parms ) ;

static int _Optlink Compare_FONTMETRICS ( const void *Item1, const void *Item2 ) ;

static int _Optlink Compare_ULONG ( const void *Item1, const void *Item2 ) ;

static BOOL VerifyTabSpacing ( HWND hwnd, PDEFAULTS_PARMS Parms ) ;

static char *ShowCodepageName ( int Codepage ) ;

static WINDOWPROC Defaults_Author_Processor ;
static WINDOWPROC Defaults_Printer_Processor ;
static WINDOWPROC Defaults_Font_Processor ;
static WINDOWPROC Defaults_Layout1_Processor ;
static WINDOWPROC Defaults_Layout2_Processor ;
static WINDOWPROC Defaults_National_Processor ;


  // Global Data (private)

static USHORT Sizes [] = { 80, 90, 100, 110, 120, 140, 160, 180, 240, 320, 400, 480 } ;

static COLOR Colors [] = { 
  RGB_BLACK, 0x000080,  0x008000, 0x008080,  0x800000, 0x800080, 0x808000,   0x808080,
  0xCCCCCC,  RGB_BLUE,  RGB_GREEN, RGB_CYAN, RGB_RED,  RGB_PINK, RGB_YELLOW, RGB_WHITE } ;


 
/****************************************************************************
 *                                                                          *
 *      Invoke Document Defaults dialog                                     *
 *                                                                          *
 ****************************************************************************/

extern BOOL DefaultsDialog ( HWND hwnd, HMODULE Library, DEFAULTS_PARMS &Parms ) {

   return ( WinDlgBox ( HWND_DESKTOP, hwnd, PWINDOWPROC(DefaultsProcessor), Library, IDD_DEFAULTS, &Parms ) ) ;

}

/****************************************************************************
 *                                                                          *
 *      "Defaults" Dialog Processor                                         *
 *                                                                          *
 ****************************************************************************/

static MRESULT EXPENTRY DefaultsProcessor ( HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2 ) {

  /**************************************************************************
   * Dispatch the message according to the method table and return the      *
   *   result.  Any messages not defined above get handled by the system    *
   *   default dialog processor.                                            *
   **************************************************************************/

   static METHOD Methods [] = {
      { WM_INITDLG,     InitDlg     },
      { WM_FORMATFRAME, FormatFrame },
      { WM_CONTROL,     Control     },
      { WM_COMMAND,     Command     }
   } ;

   #ifdef DEBUG_MESSAGES
      static int Indent = 0 ;
      LogMsgRcvd ( Indent, "DEFAULTS", msg, mp1, mp2 ) ;
      Indent += 2 ;
   #endif

   MRESULT Result = DispatchMessage ( hwnd, msg, mp1, mp2, Methods, sizeof(Methods)/sizeof(Methods[0]), Dialog_Processor ) ;

   #ifdef DEBUG_MESSAGES
      Indent -= 2 ;
      LogMsgDone ( Indent, "DEFAULTS", msg, Result ) ;
   #endif

   return ( Result ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Initialize Dialog                                                   *
 *                                                                          *
 ****************************************************************************/

static MRESULT APIENTRY InitDlg ( HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2 ) {

  /**************************************************************************
   * Get parameter block address.                                           *
   **************************************************************************/

   PDEFAULTS_PARMS Parms = PDEFAULTS_PARMS ( PVOIDFROMMP ( mp2 ) ) ;

   Sys_SetWindowData ( hwnd, Parms ) ;

  /**************************************************************************
   * Get the current form information.                                      *
   **************************************************************************/

   QueueInfo Info ( "Defaults::InitDlg", WinQueryAnchorBlock(hwnd), Parms->DocParms.Printer, Parms->DocParms.FormName, PDRIVDATA(Parms->DocParms.JobSettings) ) ;
   Info.QueryForm ( WinQueryAnchorBlock(hwnd), Parms->DocParms.Metric, Parms->FormInfo ) ;

  /**************************************************************************
   * Save the window handles that will be needed later.                     *
   **************************************************************************/

   Parms->ErrorField = CHILD ( hwnd, IDD_DEFAULTS_ERR ) ;
   Parms->hwndUnits = CHILD ( hwnd, IDD_DEFAULTS_UNITS ) ;

  /**************************************************************************
   * Create the notebook pages.                                             *
   **************************************************************************/

   ResourceString AuthorPageName   ( Parms->Library, IDS_DEFAULTS_PAGE_AUTHOR   ) ;
   ResourceString PrinterPageName  ( Parms->Library, IDS_DEFAULTS_PAGE_PRINTER  ) ;
   ResourceString FontPageName     ( Parms->Library, IDS_DEFAULTS_PAGE_FONT     ) ;
   ResourceString LayoutPageName   ( Parms->Library, IDS_DEFAULTS_PAGE_LAYOUT   ) ;
   ResourceString NationalPageName ( Parms->Library, IDS_DEFAULTS_PAGE_NATIONAL ) ;

   HWND hwndNotebook = CHILD ( hwnd, IDD_DEFAULTS_NOTEBOOK ) ;

   Sys_SendMessage ( hwndNotebook, BKM_SETNOTEBOOKCOLORS,
      MPFROMLONG(SYSCLR_FIELDBACKGROUND), MPFROMSHORT(BKA_BACKGROUNDPAGECOLORINDEX) ) ;

   struct {
      char *Name ;               WINDOWPROC *Processor ;        int   DialogID ;
   } Pages [] = {
      { PCHAR(AuthorPageName),   Defaults_Author_Processor,     IDD_DEFAULTS_PAGE_AUTHOR        },
      { PCHAR(PrinterPageName),  Defaults_Printer_Processor,    IDD_DEFAULTS_PAGE_PRINTER       },
      { PCHAR(FontPageName),     Defaults_Font_Processor,       IDD_DEFAULTS_PAGE_FONT          },
      { PCHAR(LayoutPageName),   Defaults_Layout1_Processor,    IDD_DEFAULTS_PAGE_LAYOUT1       },
      { 0,                       Defaults_Layout2_Processor,    IDD_DEFAULTS_PAGE_LAYOUT2       },
      { PCHAR(NationalPageName), Defaults_National_Processor,   IDD_DEFAULTS_PAGE_NATIONAL      },
   } ;

   WinOpenWindowDC ( hwnd ) ;
   DeviceContext Device ( "Defaults::InitDialog", hwnd ) ;
   WorkSpace PS ( "Defaults::InitDialog", 0, hwnd, &Device ) ;
   PS.GetFontMetrics() ;

   long MajorTabWidth = 0 ;
   HWND Dialog0 ;
   for ( int i=0; i<sizeof(Pages)/sizeof(Pages[0]); i++ ) {

      BOOL MajorTab ( ( i == 0 ) || Pages[i].Name ) ;
      int PageNumber(1), PageCount(1) ;  char *Name(0);
      for ( int j=i; j>=0; j-- ) 
         if ( Pages[j].Name ) 
            break ;
      Name = Pages[j].Name ;
      for ( j=j+1; j<sizeof(Pages)/sizeof(Pages[0]) && !Pages[j].Name; j++ ) {
         PageCount ++ ;
         if ( j <= i ) 
            PageNumber ++ ;
      } /* endfor */

      ULONG PageHandle = ULONG ( Sys_SendMessage ( hwndNotebook, BKM_INSERTPAGE,
         MPFROMLONG(0), MPFROM2SHORT( (MajorTab?BKA_MAJOR:0) | BKA_STATUSTEXTON, BKA_LAST ) ) ) ;

      char StatusText [80] ;
      if ( PageCount == 1 ) {
         strcpy ( StatusText, Pages[i].Name ) ;
      } else {
         ResourceString Page ( LibraryHandle, IDS_PAGE ) ;
         ResourceString Of ( LibraryHandle, IDS_OF ) ;
         sprintf ( StatusText, "%s, %s %i %s %i", Name, PCHAR(Page), PageNumber, PCHAR(Of), PageCount ) ;
      } /* endif */
      Sys_SendMessage ( hwndNotebook, BKM_SETSTATUSLINETEXT, MPFROMLONG(PageHandle), MPFROMP(StatusText) ) ;

      if ( MajorTab ) {
         Sys_SendMessage ( hwndNotebook, BKM_SETTABTEXT, MPFROMLONG(PageHandle), MPFROMP(Pages[i].Name) ) ;
         POINTL TextBox [TXTBOX_COUNT] ;
         PS.QueryTextBox ( Pages[i].Name, TextBox ) ;
         MajorTabWidth = max ( MajorTabWidth, TextBox[TXTBOX_CONCAT].x ) ;
      } /* endif */

      HWND Dialog = WinLoadDlg ( hwndNotebook, hwnd, Pages[i].Processor, LibraryHandle, Pages[i].DialogID, Parms );
      Sys_SendMessage ( hwndNotebook, BKM_SETPAGEWINDOWHWND, MPFROMP(PageHandle), MPFROMLONG(Dialog) ) ;

      if ( i == 0 ) { 
         PAGESELECTNOTIFY Info = { hwndNotebook, PageHandle, PageHandle } ;
         Sys_SendMessage ( Dialog, WM_CONTROL, MPFROM2SHORT(IDD_DEFAULTS_NOTEBOOK,BKN_PAGESELECTED), MPFROMP(&Info) ) ;
         Dialog0 = Dialog ;
      } /* endif */

   } /* endfor */

   Sys_SendMessage ( hwndNotebook, BKM_SETDIMENSIONS,
      MPFROM2SHORT( SHORT(MajorTabWidth+PS.QueryAvgWidth()*2), SHORT(((PS.QueryAscent()+PS.QueryDescent())*3)/2) ),
      MPFROMSHORT( BKA_MAJORTAB ) ) ;

  /**************************************************************************
   * Set all the check-boxes.                                               *
   **************************************************************************/

   if ( Parms->Global )
      Sys_SendMessage ( CHILD(hwnd,IDD_DEFAULTS_GLOBAL), BM_SETCHECK, MPFROMSHORT(TRUE), 0 ) ;

  /**************************************************************************
   * Set the current measurement units.                                     *
   **************************************************************************/

   ResourceString Units ( Parms->Library, Parms->DocParms.Metric ? IDS_UNITS_METRIC : IDS_UNITS_ENGLISH ) ;
   Sys_SetWindowText ( Parms->hwndUnits, PSZ(Units) ) ;

  /**************************************************************************
   * If no active window, disable the APPLY button.                         *
   **************************************************************************/

   if ( Parms->Window == 0 ) {
      Sys_EnableWindow ( CHILD(hwnd,IDD_DEFAULTS_APPLY), FALSE ) ;
      Sys_EnableWindow ( CHILD(hwnd,IDD_DEFAULTS_GLOBAL), FALSE ) ;
   } /* endif */

  /**************************************************************************
   * Set the initial focus.                                                 *
   **************************************************************************/

   Sys_SetFocus ( Dialog0 ) ;

  /**************************************************************************
   * Perform default dialog processing.                                     *
   **************************************************************************/

   Dialog_Processor ( hwnd, msg, mp1, mp2 ) ;

  /**************************************************************************
   * Return status for keeping focus changes.                               *
   **************************************************************************/

   return ( MRFROMSHORT ( TRUE ) ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process 'Format Frame' Message                                      *
 *                                                                          *
 ****************************************************************************/

static MRESULT APIENTRY FormatFrame ( HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2 ) {

  /**************************************************************************
   * First let the default dialog processor do its stuff.                   *
   **************************************************************************/

   MRESULT Result = Dialog_Processor ( hwnd, msg, mp1, mp2 ) ;

  /**************************************************************************
   * Set the new notebook size, preserving the space for the controls below.*
   **************************************************************************/

   SWP DialogPosition ;
   WinQueryWindowPos ( hwnd, &DialogPosition ) ;

   SWP NotebookPosition ;
   WinQueryWindowPos ( CHILD(hwnd,IDD_DEFAULTS_NOTEBOOK), &NotebookPosition ) ;

   LONG cyBorder = WinQuerySysValue ( HWND_DESKTOP, SV_CYSIZEBORDER ) ;
   LONG cyTitle = WinQuerySysValue ( HWND_DESKTOP, SV_CYTITLEBAR ) ;

   NotebookPosition.cx = DialogPosition.cx - NotebookPosition.x*2 ;
   NotebookPosition.cy = DialogPosition.cy - NotebookPosition.y - cyBorder*2 - cyTitle ;

   WinSetWindowPos ( CHILD(hwnd,IDD_DEFAULTS_NOTEBOOK), 
      0, 0, 0, NotebookPosition.cx, NotebookPosition.cy, SWP_SIZE ) ;

  /**************************************************************************
   * Return whatever the default said.                                      *
   **************************************************************************/

   return ( Result ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process Control Messages                                            *
 *                                                                          *
 ****************************************************************************/

static MRESULT APIENTRY Control ( HWND, ULONG msg, MPARAM mp1, MPARAM mp2 ) {

  /**************************************************************************
   * Decode the message.  Find out what control sent it, and what the       *
   *   control had to say.                                                  *
   **************************************************************************/
 
   SHORT Id = SHORT1FROMMP ( mp1 ) ;
   SHORT Message = SHORT2FROMMP ( mp1 ) ;

  /**************************************************************************
   * Process notification according to which control sent it.               *
   **************************************************************************/

   switch ( Id ) {

      case IDD_DEFAULTS_NOTEBOOK: {
         switch ( Message ) {

            case BKN_PAGESELECTEDPENDING: {
               PAGESELECTNOTIFY *pInfo = (PAGESELECTNOTIFY*) PVOIDFROMMP(mp2) ;
               HWND Dialog = HWND ( Sys_SendMessage ( pInfo->hwndBook, BKM_QUERYPAGEWINDOWHWND, MPFROMLONG(pInfo->ulPageIdCur), 0 ) ) ;
               return ( Sys_SendMessage ( Dialog, msg, mp1, mp2 ) ) ; } /* endcase */

            case BKN_PAGESELECTED: {
               PAGESELECTNOTIFY *pInfo = (PAGESELECTNOTIFY*) PVOIDFROMMP(mp2) ;
               HWND Dialog = HWND ( Sys_SendMessage ( pInfo->hwndBook, BKM_QUERYPAGEWINDOWHWND, MPFROMLONG(pInfo->ulPageIdNew), 0 ) ) ;
               return ( Sys_SendMessage ( Dialog, msg, mp1, mp2 ) ) ; } /* endcase */

         } /* endswitch */
         break ; } /* endcase */

   } /* endswitch */

  /**************************************************************************
   * Return no error.                                                       *
   **************************************************************************/

   return ( MRFROMSHORT ( FALSE ) ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process commands received by the Defaults Dialog                    *
 *                                                                          *
 ****************************************************************************/

static MRESULT APIENTRY Command ( HWND hwnd, ULONG, MPARAM mp1, MPARAM mp2 ) {

 /***************************************************************************
  * Local Declarations                                                      *
  ***************************************************************************/

  static METHOD Methods [] = {
    { DID_OK,                OK       },
    { IDD_DEFAULTS_APPLY,    Apply    },
    { DID_CANCEL,            Cancel   },
  } ;

 /***************************************************************************
  * Dispatch the message without a default message processor.               *
  ***************************************************************************/

  return ( DispatchMessage ( hwnd, SHORT1FROMMP(mp1), mp1, mp2, Methods, sizeof(Methods)/sizeof(Methods[0]), 0 ) ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process the Defaults Dialog's OK button being pressed.              *
 *                                                                          *
 ****************************************************************************/

static MRESULT APIENTRY OK ( HWND hwnd, ULONG, MPARAM, MPARAM ) {

 /***************************************************************************
  * Find the instance data.                                                 *
  ***************************************************************************/

  PDEFAULTS_PARMS Parms = PDEFAULTS_PARMS ( Sys_GetWindowData ( hwnd ) ) ;

 /***************************************************************************
  * Perform all edits.  Return if any errors.                               *
  ***************************************************************************/

  if ( !EditAll ( hwnd, Parms ) )
     return ( 0 ) ;

 /***************************************************************************
  * Dismiss the dialog with a TRUE status.                                  *
  ***************************************************************************/

  Sys_EndDialog ( hwnd, TRUE ) ;

  return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Apply the Defaults                                                  *
 *                                                                          *
 ****************************************************************************/

static MRESULT APIENTRY Apply ( HWND hwnd, ULONG, MPARAM, MPARAM ) {

 /***************************************************************************
  * Find the instance data.                                                 *
  ***************************************************************************/

  PDEFAULTS_PARMS Parms = PDEFAULTS_PARMS ( Sys_GetWindowData ( hwnd ) ) ;

 /***************************************************************************
  * Perform all edits.  Return if any errors.                               *
  ***************************************************************************/

  if ( !EditAll ( hwnd, Parms ) ) 
     return ( 0 ) ;

 /***************************************************************************
  * Set the current document's defaults.                                    *
  ***************************************************************************/

  Sys_PostMessage ( Parms->Window, WM_SET_DEFAULTS, MPFROMP(&Parms->DocParms), 0 ) ;

  return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process the Defaults Dialog's being cancelled.                      *
 *                                                                          *
 ****************************************************************************/

static MRESULT APIENTRY Cancel ( HWND hwnd, ULONG, MPARAM, MPARAM ) {

 /***************************************************************************
  * Dismiss the dialog with a TRUE status.                                  *
  ***************************************************************************/

  Sys_EndDialog ( hwnd, FALSE ) ;

  return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process the Printer Settings button's being pushed.                 *
 *                                                                          *
 ****************************************************************************/

static MRESULT APIENTRY Settings ( HWND hwnd, ULONG, MPARAM, MPARAM ) {

 /***************************************************************************
  * Find the instance data.                                                 *
  ***************************************************************************/

  PDEFAULTS_PARMS Parms = PDEFAULTS_PARMS ( Sys_GetWindowData ( hwnd ) ) ;

 /***************************************************************************
  * Get the selected queue's information.                                   *
  ***************************************************************************/

  QueueInfo Info ( "Defaults::Settings", WinQueryAnchorBlock(hwnd), Parms->DocParms.Printer, Parms->DocParms.FormName, PDRIVDATA(Parms->DocParms.JobSettings) ) ;

 /***************************************************************************
  * Get the printer name.                                                   *
  ***************************************************************************/

  char Printer [80] ;
  strcpy ( Printer, PCHAR(Info.QueryPrinters()) ) ;
  char *p = strchr ( Printer, ',' ) ;
  if ( p )
     *p = 0 ;

 /***************************************************************************
  * Get the device name.                                                    *
  ***************************************************************************/

  char Device [80] ;
  p = strchr ( PCHAR(Info.QueryDriverName()), '.' ) ;
  if ( p )
     strcpy ( Device, p+1 ) ;
  else
     strcpy ( Device, "" ) ;

 /***************************************************************************
  * Get the driver name.                                                    *
  ***************************************************************************/

  char Driver [80] ;
  strcpy ( Driver, PCHAR(Info.QueryDriverName()) ) ;
  p = strchr ( Driver, '.' ) ;
  if ( p )
     *p = 0 ;

 /***************************************************************************
  * Call up the settings dialog.                                            *
  ***************************************************************************/

  DevPostDeviceModes ( WinQueryAnchorBlock(hwnd), PDRIVDATA(Parms->DocParms.JobSettings),
     PSZ(Driver), PSZ(Device), PSZ(Printer), DPDM_POSTJOBPROP ) ;

 /***************************************************************************
  * Update the dialog's form name and size.                                 *
  ***************************************************************************/

  HCINFO OldFormInfo ;
  Info.QueryForm ( WinQueryAnchorBlock(hwnd), Parms->DocParms.Metric, OldFormInfo ) ;

  Info.UpdateSettings ( PDRIVDATA(Parms->DocParms.JobSettings) ) ;

  Info.QueryForm ( WinQueryAnchorBlock(hwnd), Parms->DocParms.Metric, Parms->FormInfo ) ;

  ResourceString SelectedForm ( Parms->Library, IDS_SELECTED_FORM ) ;
  char FormName [80] ;
  sprintf ( FormName, PCHAR(SelectedForm), Parms->FormInfo.szFormname ) ;
  Sys_SetWindowText ( Parms->hwndFormName, PSZ(FormName) ) ;

  ShowFormSize ( WinQueryAnchorBlock(hwnd), Parms->Library, Parms->DocParms,
     Parms->hwndFormSize, &Parms->FormInfo, "" ) ;

 /***************************************************************************
  * Adjust the margins.                                                     *
  ***************************************************************************/

#if 0

  Parms->DocParms.RightMargin = max ( 0, OldFormInfo.cx - Parms->DocParms.RightMargin ) ;
  Parms->DocParms.BottomMargin = max ( 0, OldFormInfo.cy - Parms->DocParms.BottomMargin ) ;

  Parms->DocParms.RightMargin = max ( 0, Parms->FormInfo.cx - Parms->DocParms.RightMargin ) ;
  Parms->DocParms.BottomMargin = max ( 0, Parms->FormInfo.cy - Parms->DocParms.BottomMargin ) ;

  char Text [11] ;

  FormatDecimal ( Text, Parms->DocParms.RightMargin, 1000, 3 ) ;
  Sys_SetWindowText ( Parms->hwndRightMargin, PSZ(Text) ) ;

  FormatDecimal ( Text, Parms->DocParms.BottomMargin, 1000, 3 ) ;
  Sys_SetWindowText ( Parms->hwndBottomMargin, PSZ(Text) ) ;

#endif

 /***************************************************************************
  * Done.                                                                   *
  ***************************************************************************/

  return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Perform All Edits.  Return TRUE if all OK.                          *
 *                                                                          *
 ****************************************************************************/

static BOOL EditAll ( HWND hwnd, PDEFAULTS_PARMS Parms ) {

 /***************************************************************************
  * Validate all notebook pages.  Return FALSE if there are problems.       *
  ***************************************************************************/

  HWND hwndNotebook = CHILD(hwnd,IDD_DEFAULTS_NOTEBOOK) ;
  ULONG Page = ULONG ( Sys_SendMessage ( hwndNotebook, BKM_QUERYPAGEID, 0, MPFROM2SHORT(BKA_FIRST,0) ) ) ;
  while ( Page ) {
     PAGESELECTNOTIFY Info = { hwndNotebook, Page, Page } ;
     HWND hwndPage = HWND ( Sys_SendMessage ( hwndNotebook, BKM_QUERYPAGEWINDOWHWND, MPFROMLONG(Page), 0 ) ) ;
     Sys_SendMessage ( hwndPage, WM_CONTROL, MPFROM2SHORT(IDD_DEFAULTS_NOTEBOOK,BKN_PAGESELECTEDPENDING), MPFROMP(&Info) ) ;
     if ( Info.ulPageIdNew == 0 ) {
        Sys_SendMessage ( hwndNotebook, BKM_TURNTOPAGE, MPFROMLONG(Page), 0 ) ;
        return ( FALSE ) ;
     } /* endif */
     Page = ULONG ( Sys_SendMessage ( hwndNotebook, BKM_QUERYPAGEID, MPFROMLONG(Page), MPFROM2SHORT(BKA_NEXT,0) ) ) ;
  } /* endwhile */

 /***************************************************************************
  * Pull the global change flag, which isn't in the notebooks.              *
  ***************************************************************************/

  Parms->Global = BOOL ( SHORT1FROMMR ( Sys_SendMessage ( CHILD(hwnd,IDD_DEFAULTS_GLOBAL), BM_QUERYCHECK, 0, 0 ) ) ) ;

 /***************************************************************************
  * All went well.  Return TRUE.                                            *
  ***************************************************************************/

  return ( TRUE ) ;
}
 
/****************************************************************************
 *                                                                          *
 *  Compare FONTMETRICS structures                                          *
 *                                                                          *
 ****************************************************************************/

static int _Optlink Compare_FONTMETRICS ( const void *Item1, const void *Item2 ) {
  PFONTMETRICS Font1 = PFONTMETRICS ( Item1 ) ;
  PFONTMETRICS Font2 = PFONTMETRICS ( Item2 ) ;
  return ( strcmp ( Font1->szFacename, Font2->szFacename ) ) ;
}
 
/****************************************************************************
 *                                                                          *
 *  Compare ULONGs                                                          *
 *                                                                          *
 ****************************************************************************/

static int _Optlink Compare_ULONG ( const void *Item1, const void *Item2 ) {
  PULONG pValue1 = PULONG ( Item1 ) ;
  PULONG pValue2 = PULONG ( Item2 ) ;
  return ( int ( *pValue1 - *pValue2 ) ) ;
}
 
/****************************************************************************
 *                                                                          *
 *  Show Form Size                                                          *
 *                                                                          *
 ****************************************************************************/

extern void ShowFormSize (
   HAB Anchor,
   HMODULE Library,
   DocumentParms &DocParms,
   HWND hwndField,
   PHCINFO pFormInfo,
   char *Preface
) {

 /***************************************************************************
  * If not passed in, get the current form size.                            *
  ***************************************************************************/

  HCINFO FormInfo ;
  if ( pFormInfo == 0 ) {
     QueueInfo Info ( "Defaults::ShowFormSize", Anchor, DocParms.Printer, DocParms.FormName, PDRIVDATA(DocParms.JobSettings) ) ;
     Info.QueryForm ( Anchor, DocParms.Metric, FormInfo ) ;
     pFormInfo = & FormInfo ;
  } /* endif */

 /***************************************************************************
  * Build the form size text.                                               *
  ***************************************************************************/

  char Message [200] ;
  strcpy ( Message, Preface ) ;

  ResourceString Metric ( Library, IDS_DEFAULTS_METRIC1 ) ;
  ResourceString English ( Library, IDS_DEFAULTS_ENGLISH1 ) ;
  PCHAR Format = DocParms.Metric ? PCHAR(Metric) : PCHAR(English) ;
  char Text1 [20] ;  FormatDecimal ( Text1, pFormInfo->cx, 1000, DocParms.Metric ? 1 : 2 ) ;
  char Text2 [20] ;  FormatDecimal ( Text2, pFormInfo->cy, 1000, DocParms.Metric ? 1 : 2 ) ;
  sprintf ( Message+strlen(Message), Format, Text1, Text2 ) ;

  if ( ( pFormInfo->xLeftClip != 0 ) OR ( pFormInfo->yBottomClip != 0 )
     OR ( pFormInfo->xRightClip != pFormInfo->cx ) OR ( pFormInfo->yTopClip != pFormInfo->cy ) ) {

     ResourceString Metric ( Library, IDS_DEFAULTS_METRIC2 ) ;
     ResourceString English ( Library, IDS_DEFAULTS_ENGLISH2 ) ;
     PCHAR Format = DocParms.Metric ? PCHAR(Metric) : PCHAR(English) ;
     char Text1 [20] ;  FormatDecimal ( Text1, pFormInfo->xLeftClip, 1000, DocParms.Metric ? 1 : 2 ) ;
     char Text2 [20] ;  FormatDecimal ( Text2, pFormInfo->yBottomClip, 1000, DocParms.Metric ? 1 : 2 ) ;
     char Text3 [20] ;  FormatDecimal ( Text3, pFormInfo->xRightClip, 1000, DocParms.Metric ? 1 : 2 ) ;
     char Text4 [20] ;  FormatDecimal ( Text4, pFormInfo->yTopClip, 1000, DocParms.Metric ? 1 : 2 ) ;
     sprintf ( Message+strlen(Message), Format, Text1, CountryInfo.szDataSeparator, 
        Text2, Text3, CountryInfo.szDataSeparator, Text4 ) ;

  } /* endif */

 /***************************************************************************
  * Update the static field.                                                *
  ***************************************************************************/

  Sys_SetWindowText ( hwndField, PSZ(Message) ) ;
}
 
/****************************************************************************
 *                                                                          *
 *  Verify Tab Spacing                                                      *
 *                                                                          *
 ****************************************************************************/

static BOOL VerifyTabSpacing ( HWND hwndField, HWND hwndError, PDEFAULTS_PARMS Parms ) {

  /**************************************************************************
   * Get the new value.                                                     *
   **************************************************************************/

   char Text [11] ;
   Sys_GetWindowText ( hwndField, Text, sizeof(Text)-1 ) ;

  /**************************************************************************
   * Check for invalid value.                                               *
   **************************************************************************/

   double Value ;
   if ( ( !DecodeDecimal ( Text, Value ) ) OR ( Value <= 0.0 ) ) {
      ResourceString Message ( Parms->Library, IDS_DEFAULTS_ERROR1 ) ;
      Sys_SetWindowText ( hwndError, PSZ(Message) ) ;
      Sys_BeepError ( ) ;
      Sys_SetFocus ( hwndField ) ;
      return ( FALSE ) ;
   } /* endif */

  /**************************************************************************
   * Save the value and return no error.                                    *
   **************************************************************************/

   Parms->DocParms.TabSpacing = int ( Value * 1000 + 0.4 ) ;

   return ( TRUE ) ;
}
 
/****************************************************************************
 *                                                                          *
 *  Show Codepage Name                                                      *
 *                                                                          *
 ****************************************************************************/

static char *ShowCodepageName ( int Codepage ) {
   ResourceString Name ( LibraryHandle, IDS_CODEPAGES + Codepage ) ;
   return ( PCHAR(Name) ? PCHAR(Name) : "???" ) ;
}

/****************************************************************************
 *                                                                          *
 *  Defaults Author Page message processor                                  *
 *                                                                          *
 ****************************************************************************/

static MRESULT EXPENTRY Defaults_Author_Processor ( HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2 ) {

   #ifdef DEBUG_MESSAGES
      static int Indent = 0 ;
      LogMsgRcvd ( Indent, "Defaults::Author", msg, mp1, mp2 ) ;
      Indent += 2 ;
   #endif

   switch ( msg ) {

      case WM_INITDLG: {
         PDEFAULTS_PARMS Parms = PDEFAULTS_PARMS ( PVOIDFROMMP ( mp2 ) ) ;
         Sys_SetWindowData ( hwnd, Parms ) ;
         WinAssociateHelpInstance ( WinQueryHelpInstance(Parms->Window), hwnd ) ;
         Sys_SetWindowText ( CHILD(hwnd,IDD_DEFAULTS_AUTHOR), PSZ(Parms->DocParms.Author) ) ;
         if ( Parms->Window ) {
            Sys_SetWindowText ( CHILD(hwnd,IDD_DEFAULTS_TITLE), PSZ(Parms->DocParms.Title) ) ;
            char CreateDate [80] = { 0 } ;
            if ( Parms->DocParms.CreateDate )
               FormatDate ( Parms->DocParms.CreateDate, Parms->DocParms.DateFormat, CreateDate, sizeof(CreateDate) ) ;
            Sys_SetWindowText ( CHILD(hwnd,IDD_DEFAULTS_CREATEDATE), CreateDate ) ;
         } else {
            Sys_ShowWindow ( CHILD(hwnd,IDD_DEFAULTS_TITLE_TAG), FALSE ) ;
            Sys_ShowWindow ( CHILD(hwnd,IDD_DEFAULTS_TITLE), FALSE ) ;
            Sys_SetWindowText ( CHILD(hwnd,IDD_DEFAULTS_TITLE), PSZ("") ) ;
            Sys_ShowWindow ( CHILD(hwnd,IDD_DEFAULTS_CREATEDATE_TAG), FALSE ) ;
            Sys_ShowWindow ( CHILD(hwnd,IDD_DEFAULTS_CREATEDATE), FALSE ) ;
            Sys_SetWindowText ( CHILD(hwnd,IDD_DEFAULTS_CREATEDATE), PSZ("") ) ;
         } /* endif */
         break; }

      case WM_CONTROL: {
         PDEFAULTS_PARMS Parms = PDEFAULTS_PARMS ( Sys_GetWindowData ( hwnd ) ) ;
         SHORT Id = SHORT1FROMMP ( mp1 ) ;
         SHORT Message = SHORT2FROMMP ( mp1 ) ;
         switch ( Id ) {
            case IDD_DEFAULTS_NOTEBOOK: {
               switch ( Message ) {
                  case BKN_PAGESELECTEDPENDING: {
//                   PAGESELECTNOTIFY *pInfo = (PAGESELECTNOTIFY*) PVOIDFROMMP(mp2) ;
                     // Clear the error field.
                     Sys_SetWindowText ( Parms->ErrorField, PSZ("") ) ;
                     // Get new author name.
                     Sys_GetWindowText ( CHILD(hwnd,IDD_DEFAULTS_AUTHOR), Parms->DocParms.Author, sizeof(Parms->DocParms.Author) ) ;
                     // Get new title.
                     Sys_GetWindowText ( CHILD(hwnd,IDD_DEFAULTS_TITLE), Parms->DocParms.Title, sizeof(Parms->DocParms.Title) ) ;
                     break ; } /* endcase */
                  case BKN_PAGESELECTED: {
                     Sys_SetFocus ( CHILD(hwnd,IDD_DEFAULTS_AUTHOR) ) ;
                     Sys_SendMessage ( WinQueryHelpInstance(Parms->Window), HM_SET_ACTIVE_WINDOW, MPFROMHWND(hwnd), MPFROMHWND(hwnd) ) ;
                     break ; } /* endcase */
               } /* endswitch */
               break ; } /* endcase */
         } /* endswitch */
         break; }

      case WM_COMMAND: {
         MRESULT Result = Sys_SendMessage ( PARENT(hwnd), msg, mp1, mp2 ) ;
         #ifdef DEBUG_MESSAGES
            Indent -= 2 ;
            LogMsgDone ( Indent, "Defaults::Author", msg, Result ) ;
         #endif
         return ( Result ) ; }

      case WM_HELP: {
         PDEFAULTS_PARMS Parms = PDEFAULTS_PARMS ( Sys_GetWindowData ( hwnd ) ) ;
         Sys_SendMessage ( WinQueryHelpInstance(Parms->Window), HM_EXT_HELP, 0, 0 ) ;
         #ifdef DEBUG_MESSAGES
            Indent -= 2 ;
            LogMsgDone ( Indent, "Defaults::Author", msg, 0 ) ;
         #endif
         return ( 0 ) ; }

   } /* endswitch */

   MRESULT Result = Sys_DefDialogProc ( hwnd, msg, mp1, mp2 ) ;

   #ifdef DEBUG_MESSAGES
      Indent -= 2 ;
      LogMsgDone ( Indent, "Defaults::Author", msg, Result ) ;
   #endif

   return ( Result ) ;
}

/****************************************************************************
 *                                                                          *
 *  Defaults Printer Page message processor                                 *
 *                                                                          *
 ****************************************************************************/

static MRESULT EXPENTRY Defaults_Printer_Processor ( HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2 ) {

   #ifdef DEBUG_MESSAGES
      static int Indent = 0 ;
      LogMsgRcvd ( Indent, "Defaults::Printer", msg, mp1, mp2 ) ;
      Indent += 2 ;
   #endif

   switch ( msg ) {

      case WM_INITDLG: {
         PDEFAULTS_PARMS Parms = PDEFAULTS_PARMS ( PVOIDFROMMP ( mp2 ) ) ;
         Sys_SetWindowData ( hwnd, Parms ) ;
         WinAssociateHelpInstance ( WinQueryHelpInstance(Parms->Window), hwnd ) ;

         SHORT Index = 0 ;
         ULONG cTotal;
         ULONG cReturned ;
         ULONG cbNeeded ;
         APIRET Status = SplEnumQueue( 0, 3, 0, 0, &cReturned, &cTotal, &cbNeeded, 0 ) ;
         if ( ( Status == ERROR_MORE_DATA ) OR ( Status == NERR_BufTooSmall ) ) {
            PVOID pBuf ;
            if ( !DosAllocMem ( &pBuf, cbNeeded, PAG_READ|PAG_WRITE|PAG_COMMIT ) ) {
               ULONG cbBuf = cbNeeded ;
               Status = SplEnumQueue ( 0, 3, pBuf, cbBuf, &cReturned, &cTotal, &cbNeeded, 0 ) ;
               if ( Status == NO_ERROR ) {
                  PPRQINFO3 prq = PPRQINFO3(pBuf) ;
                  for ( int i=0; i<cReturned; i++ ) {
                     Sys_SendMessage ( CHILD(hwnd,IDD_DEFAULTS_PRINTER), LM_INSERTITEM,
                        MPFROMSHORT ( LIT_SORTASCENDING ), MPFROMP ( prq->pszComment ) ) ;
                     Index ++ ;
                     prq ++ ;
                  } /* endfor */
               } /* endif */
               DosFreeMem ( pBuf ) ;
            } /* endif */
         } /* endif */

         BOOL PrinterSelected = FALSE ;
         while ( Index-- ) {
            UCHAR Printer [CCHMAXPATH] ;
            Sys_SendMessage ( CHILD(hwnd,IDD_DEFAULTS_PRINTER), LM_QUERYITEMTEXT,
               MPFROM2SHORT ( Index, sizeof(Printer) ), MPFROMP ( Printer ) ) ;
            if ( !strcmp ( PCHAR(Parms->DocParms.Printer), PCHAR(Printer) ) ) {
               Sys_SendMessage ( CHILD(hwnd,IDD_DEFAULTS_PRINTER), LM_SELECTITEM,
                  MPFROMSHORT(Index), MPFROMSHORT(TRUE) ) ;
               Sys_SetWindowText ( CHILD(hwnd,IDD_DEFAULTS_PRINTER), Parms->DocParms.Printer ) ;
               PrinterSelected = TRUE ;
               break ;
            } /* endif */
         } /* endwhile */
         if ( NOT PrinterSelected )
            Sys_SetWindowText ( CHILD(hwnd,IDD_DEFAULTS_PRINTER), "" ) ;

         Sys_EnableWindow ( CHILD(hwnd,IDD_DEFAULTS_SETTINGS), PrinterSelected ) ;

         ResourceString SelectedForm ( Parms->Library, IDS_SELECTED_FORM ) ;
         char FormName [80] ;
         sprintf ( FormName, PCHAR(SelectedForm), Parms->FormInfo.szFormname ) ;
         Parms->hwndFormName = CHILD ( hwnd, IDD_DEFAULTS_FORMNAME ) ;
         Sys_SetWindowText ( Parms->hwndFormName, PSZ(FormName) ) ;
         Parms->hwndFormSize = CHILD ( hwnd, IDD_DEFAULTS_FORMSIZE ) ;
         ShowFormSize ( WinQueryAnchorBlock(hwnd), Parms->Library, Parms->DocParms, Parms->hwndFormSize, &Parms->FormInfo, "" ) ;

         break; }

      case WM_CONTROL: {
         PDEFAULTS_PARMS Parms = PDEFAULTS_PARMS ( Sys_GetWindowData ( hwnd ) ) ;
         SHORT Id = SHORT1FROMMP ( mp1 ) ;
         SHORT Message = SHORT2FROMMP ( mp1 ) ;
         switch ( Id ) {
            case IDD_DEFAULTS_NOTEBOOK: {
               switch ( Message ) {
                  case BKN_PAGESELECTEDPENDING: {
                     PAGESELECTNOTIFY *pInfo = (PAGESELECTNOTIFY*) PVOIDFROMMP(mp2) ;
                     // Clear the error field.
                     Sys_SetWindowText ( Parms->ErrorField, PSZ("") ) ;
                     // Validate printer selection.
                     char Printer [CCHMAXPATH] ;
                     Sys_GetWindowText ( CHILD(hwnd,IDD_DEFAULTS_PRINTER), Printer, sizeof(Printer) ) ;
                     if ( Printer[0] == 0 ) {
                        ULONG cTotal;
                        ULONG cReturned ;
                        ULONG cbNeeded ;
                        SplEnumQueue( 0, 3, 0, 0, &cReturned, &cTotal, &cbNeeded, 0 ) ;
                        if ( cTotal ) {
                           ResourceString Message ( Parms->Library, IDS_DEFAULTS_ERROR2 ) ;
                           Sys_SetWindowText ( Parms->ErrorField, PSZ(Message) ) ;
                           Sys_BeepError ( ) ;
                           Sys_SetFocus ( CHILD ( hwnd, IDD_DEFAULTS_PRINTER ) ) ;
                           pInfo->ulPageIdNew = 0 ;
                           break ;
                        } /* endif */
                     } /* endif */
                     memcpy ( Parms->DocParms.Printer, Printer, sizeof(Parms->DocParms.Printer) ) ;
                     break ; } /* endcase */
                  case BKN_PAGESELECTED: {
                     Sys_SetFocus ( CHILD(hwnd,IDD_DEFAULTS_PRINTER) ) ;
                     Sys_SendMessage ( WinQueryHelpInstance(Parms->Window), HM_SET_ACTIVE_WINDOW, MPFROMHWND(hwnd), MPFROMHWND(hwnd) ) ;
                     break ; } /* endcase */
               } /* endswitch */
               break ; } /* endcase */
            case IDD_DEFAULTS_PRINTER: {
               switch ( Message ) {
                  case CBN_ENTER:
                  case CBN_EFSCROLL:
                  case CBN_LBSCROLL:
                  case CBN_LBSELECT: {
                     SHORT Index = SHORT1FROMMR ( Sys_SendMessage ( CHILD(hwnd,IDD_DEFAULTS_PRINTER), LM_QUERYSELECTION, MPFROMSHORT(LIT_FIRST), 0 ) ) ;
                     if ( Index == LIT_NONE )
                        break ;
                     UCHAR Printer [CCHMAXPATH] ;
                     Sys_SendMessage ( CHILD(hwnd,IDD_DEFAULTS_PRINTER), LM_QUERYITEMTEXT,
                        MPFROM2SHORT ( Index, sizeof(Printer) ), MPFROMP ( Printer ) ) ;
                     memcpy ( Parms->DocParms.Printer, Printer, sizeof(Parms->DocParms.Printer) ) ;
                     ResourceString SelectedForm ( Parms->Library, IDS_SELECTED_FORM ) ;
                     char FormName [80] ;
                     sprintf ( FormName, PCHAR(SelectedForm), Parms->FormInfo.szFormname ) ;
                     Sys_SetWindowText ( Parms->hwndFormName, PSZ(FormName) ) ;
                     ShowFormSize ( WinQueryAnchorBlock(hwnd), Parms->Library, Parms->DocParms,
                        Parms->hwndFormSize, &Parms->FormInfo, "" ) ;
                     Sys_EnableWindow ( CHILD(hwnd,IDD_DEFAULTS_SETTINGS), TRUE ) ;
                     break ; } /* endcase */
               } /* endswitch */
               break ; } /* endcase */
         } /* endswitch */
         break; }

      case WM_COMMAND: {
         MRESULT Result ;
         if ( SHORT1FROMMP(mp1) == IDD_DEFAULTS_SETTINGS ) 
            Result = Settings ( hwnd, msg, mp1, mp2 ) ;
         else
            Result = Sys_SendMessage ( PARENT(hwnd), msg, mp1, mp2 ) ; 
         #ifdef DEBUG_MESSAGES
            Indent -= 2 ;
            LogMsgDone ( Indent, "Defaults::Printer", msg, Result ) ;
         #endif
         return ( Result ) ; }

      case WM_HELP: {
         PDEFAULTS_PARMS Parms = PDEFAULTS_PARMS ( Sys_GetWindowData ( hwnd ) ) ;
         Sys_SendMessage ( WinQueryHelpInstance(Parms->Window), HM_EXT_HELP, 0, 0 ) ;
         #ifdef DEBUG_MESSAGES
            Indent -= 2 ;
            LogMsgDone ( Indent, "Defaults::Printer", msg, 0 ) ;
         #endif
         return ( 0 ) ; }

   } /* endswitch */

   MRESULT Result = Sys_DefDialogProc ( hwnd, msg, mp1, mp2 ) ;

   #ifdef DEBUG_MESSAGES
      Indent -= 2 ;
      LogMsgDone ( Indent, "Defaults::Printer", msg, Result ) ;
   #endif

   return ( Result ) ;
}

/****************************************************************************
 *                                                                          *
 *  Defaults Font Page message processor                                    *
 *                                                                          *
 ****************************************************************************/

static MRESULT EXPENTRY Defaults_Font_Processor ( HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2 ) {

   #ifdef DEBUG_MESSAGES
      static int Indent = 0 ;
      LogMsgRcvd ( Indent, "Defaults::Font", msg, mp1, mp2 ) ;
      Indent += 2 ;
   #endif

   switch ( msg ) {

      case WM_INITDLG: {
         PDEFAULTS_PARMS Parms = PDEFAULTS_PARMS ( PVOIDFROMMP ( mp2 ) ) ;
         Sys_SetWindowData ( hwnd, Parms ) ;
         WinAssociateHelpInstance ( WinQueryHelpInstance(Parms->Window), hwnd ) ;

         WorkSpace PS ( "Defaults::InitDialog", 0, hwnd, FALSE ) ;
         LONG FontCount = PS.QueryFontCount ( ) ;
         PFONTMETRICS pfm = new FONTMETRICS [ FontCount ] ;
         PS.QueryFonts ( FontCount, pfm ) ;
         qsort ( pfm, (size_t)FontCount, sizeof(FONTMETRICS), Compare_FONTMETRICS ) ;
         for ( int i=0, j=0; i<FontCount; i++ ) {
            if ( pfm[i].fsDefn & FM_DEFN_OUTLINE ) {
               if ( i != j ) 
                  pfm[j] = pfm[i] ;
               j ++ ;
            } /* endif */
         } /* endfor */
         FontCount = j ;
         ResourceString Bold ( Parms->Library, IDS_BOLD ) ;
         ResourceString Italic ( Parms->Library, IDS_ITALIC ) ;
         for ( i=0, j=0; i<(USHORT)FontCount; i++ ) {
            char Facename [FACESIZE+1] ;
            strcpy ( Facename, PCHAR(pfm[i].szFacename) ) ;
            char *p = strstr ( Facename, PCHAR(Bold) ) ;
            if ( p ) {
               char *p2 = p + strlen(PCHAR(Bold)) ;
               while ( *p2 ) *p++ = *p2++ ;
               *p = 0 ;
            } /* endif */
            p = strstr ( Facename, PCHAR(Italic) ) ;
            if ( p ) {
               char *p2 = p + strlen(PCHAR(Italic)) ;
               while ( *p2 ) *p++ = *p2++ ;
               *p = 0 ;
            } /* endif */
            if ( ( j == 0 ) OR strcmp ( Facename, pfm[j-1].szFacename ) ) {
               if ( i != j ) 
                  pfm[j] = pfm[i] ;
               j ++ ;
            } /* endif */
         } /* endfor */
         FontCount = j ;
         for ( i=0; i<FontCount; i++ ) {
            Sys_SendMessage ( CHILD(hwnd,IDD_DEFAULTS_FONT), LM_INSERTITEM,
               MPFROMSHORT ( LIT_SORTASCENDING ), MPFROMP ( pfm[i].szFacename ) ) ;
         } /* endfor */
         Sys_SetWindowText ( CHILD(hwnd,IDD_DEFAULTS_FONT), Parms->DocParms.Font ) ;
         for ( i=0; i<FontCount; i++ ) {
            UCHAR Font [FACESIZE] ;
            Sys_SendMessage ( CHILD(hwnd,IDD_DEFAULTS_FONT), LM_QUERYITEMTEXT,
               MPFROM2SHORT ( i, sizeof(Font) ), MPFROMP ( Font ) ) ;
            if ( !strcmp ( PCHAR(Parms->DocParms.Font), PCHAR(Font) ) ) {
               Sys_SendMessage ( CHILD(hwnd,IDD_DEFAULTS_FONT), LM_SELECTITEM,
                  MPFROMSHORT(i), MPFROMSHORT(TRUE) ) ;
               break ;
            } /* endif */
         } /* endfor */
         delete [] pfm ;

         ResourceString Point ( Parms->Library, IDS_POINT ) ;
         for ( i=0; i<sizeof(Sizes)/sizeof(Sizes[0]); i++ ) {
            char Text [10] ;
            sprintf ( Text, "%.1lf%s", (double)Sizes[i]/10.0, PSZ(Point) ) ;
            Sys_SendMessage ( CHILD(hwnd,IDD_DEFAULTS_SIZE), LM_INSERTITEM,
               MPFROMSHORT ( LIT_END ), MPFROMP ( Text ) ) ;
            if ( Sizes[i] == Parms->DocParms.Size ) {
               Sys_SetWindowText ( CHILD(hwnd,IDD_DEFAULTS_SIZE), Text ) ;
               Sys_SendMessage ( CHILD(hwnd,IDD_DEFAULTS_SIZE), LM_SELECTITEM, MPFROMSHORT(i), MPFROMSHORT(TRUE) ) ;
            } /* endif */
         } /* endfor */

         for ( i=0; i<sizeof(Colors)/sizeof(Colors[0]); i++ ) {
            Sys_SendMessage ( CHILD(hwnd,IDD_DEFAULTS_COLOR), VM_SETITEM, MPFROM2SHORT((i/8)+1,(i%8)+1), MPFROMLONG(Colors[i]) ) ;
            if ( Colors[i] == Parms->DocParms.Color ) 
               Sys_SendMessage ( CHILD(hwnd,IDD_DEFAULTS_COLOR), VM_SELECTITEM, MPFROM2SHORT((i/8)+1,(i%8)+1), 0 ) ;
         } /* endfor */

         if ( Parms->DocParms.Bold )
            Sys_SendMessage ( CHILD(hwnd,IDD_DEFAULTS_BOLD), BM_SETCHECK, MPFROMSHORT(TRUE), 0 ) ;

         if ( Parms->DocParms.Italic )
            Sys_SendMessage ( CHILD(hwnd,IDD_DEFAULTS_ITALIC), BM_SETCHECK, MPFROMSHORT(TRUE), 0 ) ;

         if ( Parms->DocParms.Underscore )
            Sys_SendMessage ( CHILD(hwnd,IDD_DEFAULTS_UNDERSCORE), BM_SETCHECK, MPFROMSHORT(TRUE), 0 ) ;

         if ( Parms->DocParms.Strikeout )
            Sys_SendMessage ( CHILD(hwnd,IDD_DEFAULTS_STRIKEOUT), BM_SETCHECK, MPFROMSHORT(TRUE), 0 ) ;

         if ( Parms->DocParms.Outline )
            Sys_SendMessage ( CHILD(hwnd,IDD_DEFAULTS_OUTLINE), BM_SETCHECK, MPFROMSHORT(TRUE), 0 ) ;

         if ( Parms->DocParms.Caps )
            Sys_SendMessage ( CHILD(hwnd,IDD_DEFAULTS_CAPS), BM_SETCHECK, MPFROMSHORT(TRUE), 0 ) ;

         break; }

      case WM_CONTROL: {
         PDEFAULTS_PARMS Parms = PDEFAULTS_PARMS ( Sys_GetWindowData ( hwnd ) ) ;
         SHORT Id = SHORT1FROMMP ( mp1 ) ;
         SHORT Message = SHORT2FROMMP ( mp1 ) ;
         switch ( Id ) {
            case IDD_DEFAULTS_NOTEBOOK: {
               switch ( Message ) {
                  case BKN_PAGESELECTEDPENDING: {
//                   PAGESELECTNOTIFY *pInfo = (PAGESELECTNOTIFY*) PVOIDFROMMP(mp2) ;
                     // Clear the error field.
                     Sys_SetWindowText ( Parms->ErrorField, PSZ("") ) ;
                     // Get the new font.
                     char Face [FACESIZE] ;
                     Sys_GetWindowText ( CHILD(hwnd,IDD_DEFAULTS_FONT), Face, sizeof(Face) ) ;
                     memcpy ( Parms->DocParms.Font, Face, sizeof(Parms->DocParms.Font) ) ;
                     // Get the new size.
                     SHORT Index = SHORT1FROMMR ( Sys_SendMessage ( CHILD(hwnd,IDD_DEFAULTS_SIZE), LM_QUERYSELECTION, MPFROMSHORT(LIT_FIRST), 0 ) ) ;
                     Parms->DocParms.Size = ( Index == LIT_NONE ) ? 120 : Sizes[Index] ;
                     // Get the new line/text color.
                     ULONG ColorIndex = ULONG ( Sys_SendMessage ( CHILD(hwnd,IDD_DEFAULTS_COLOR), VM_QUERYSELECTEDITEM, 0, 0 ) ) ;
                     Parms->DocParms.Color = COLOR ( Sys_SendMessage ( CHILD(hwnd,IDD_DEFAULTS_COLOR), VM_QUERYITEM, MPFROMLONG(ColorIndex), 0 ) ) ;
                     // Get the new Bold flag.
                     Parms->DocParms.Bold = SHORT1FROMMR ( Sys_SendMessage ( CHILD(hwnd,IDD_DEFAULTS_BOLD), BM_QUERYCHECK, 0, 0 ) ) ;
                     // Get the new Italic flag.
                     Parms->DocParms.Italic = SHORT1FROMMR ( Sys_SendMessage ( CHILD(hwnd,IDD_DEFAULTS_ITALIC), BM_QUERYCHECK, 0, 0 ) ) ;
                     // Get the new Underscore flag.
                     Parms->DocParms.Underscore = SHORT1FROMMR ( Sys_SendMessage ( CHILD(hwnd,IDD_DEFAULTS_UNDERSCORE), BM_QUERYCHECK, 0, 0 ) ) ;
                     // Get the new Strikeout flag.
                     Parms->DocParms.Strikeout = SHORT1FROMMR ( Sys_SendMessage ( CHILD(hwnd,IDD_DEFAULTS_STRIKEOUT), BM_QUERYCHECK, 0, 0 ) ) ;
                     // Get the new Outline flag.
                     Parms->DocParms.Outline = SHORT1FROMMR ( Sys_SendMessage ( CHILD(hwnd,IDD_DEFAULTS_OUTLINE), BM_QUERYCHECK, 0, 0 ) ) ;
                     // Get the new Caps flag.
                     Parms->DocParms.Caps = SHORT1FROMMR ( Sys_SendMessage ( CHILD(hwnd,IDD_DEFAULTS_CAPS), BM_QUERYCHECK, 0, 0 ) ) ;
                     break ; } /* endcase */
                  case BKN_PAGESELECTED: {
                     Sys_SetFocus ( CHILD(hwnd,IDD_DEFAULTS_FONT) ) ;
                     Sys_SendMessage ( WinQueryHelpInstance(Parms->Window), HM_SET_ACTIVE_WINDOW, MPFROMHWND(hwnd), MPFROMHWND(hwnd) ) ;
                     break ; } /* endcase */
               } /* endswitch */
               break ; } /* endcase */
         } /* endswitch */
         break; }

      case WM_COMMAND: {
         MRESULT Result = Sys_SendMessage ( PARENT(hwnd), msg, mp1, mp2 ) ; 
         #ifdef DEBUG_MESSAGES
            Indent -= 2 ;
            LogMsgDone ( Indent, "Defaults::Font", msg, Result ) ;
         #endif
         return ( Result ) ; }

      case WM_HELP: {
         PDEFAULTS_PARMS Parms = PDEFAULTS_PARMS ( Sys_GetWindowData ( hwnd ) ) ;
         Sys_SendMessage ( WinQueryHelpInstance(Parms->Window), HM_EXT_HELP, 0, 0 ) ;
         #ifdef DEBUG_MESSAGES
            Indent -= 2 ;
            LogMsgDone ( Indent, "Defaults::Font", msg, 0 ) ;
         #endif
         return ( 0 ) ; }

   } /* endswitch */

   MRESULT Result = Sys_DefDialogProc ( hwnd, msg, mp1, mp2 ) ;

   #ifdef DEBUG_MESSAGES
      Indent -= 2 ;
      LogMsgDone ( Indent, "Defaults::Font", msg, Result ) ;
   #endif

   return ( Result ) ;
}

/****************************************************************************
 *                                                                          *
 *  Defaults Layout Page 1 message processor                                *
 *                                                                          *
 ****************************************************************************/

static MRESULT EXPENTRY Defaults_Layout1_Processor ( HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2 ) {

   #ifdef DEBUG_MESSAGES
      static int Indent = 0 ;
      LogMsgRcvd ( Indent, "Defaults::Layout1", msg, mp1, mp2 ) ;
      Indent += 2 ;
   #endif

   switch ( msg ) {

      case WM_INITDLG: {
         PDEFAULTS_PARMS Parms = PDEFAULTS_PARMS ( PVOIDFROMMP ( mp2 ) ) ;
         Sys_SetWindowData ( hwnd, Parms ) ;
         WinAssociateHelpInstance ( WinQueryHelpInstance(Parms->Window), hwnd ) ;

         WinOpenWindowDC ( hwnd ) ;
         DeviceContext Device ( "Defaults_Layout1::WM_INITDLG", hwnd ) ;
         WorkSpace PS ( "Defaults_Layout1::WM_INITDLG", 0, hwnd, &Device ) ;
         for ( int i=0; i<4; i++ ) {
            HBITMAP Bitmap = PS.CreateBitmap ( 0, 100+i ) ;
            Sys_SendMessage ( CHILD(hwnd,IDD_DEFAULTS_JUSTIFY), VM_SETITEM, MPFROM2SHORT(1,i+1), MPFROMLONG(Bitmap) ) ;
            if ( i == Parms->DocParms.Justify )
               Sys_SendMessage ( CHILD(hwnd,IDD_DEFAULTS_JUSTIFY), VM_SELECTITEM, MPFROM2SHORT(1,i+1), 0 ) ;
         } /* endfor */

         if ( Parms->DocParms.CenterPage )
            Sys_SendMessage ( CHILD(hwnd,IDD_DEFAULTS_CENTERPAGE), BM_SETCHECK, MPFROMSHORT(TRUE), 0 ) ;

         char Text [80] ;
         FormatDecimal ( Text, Parms->DocParms.LeftMargin, 1000, 3 ) ;
         Parms->hwndLeftMargin = CHILD ( hwnd, IDD_DEFAULTS_LEFT ) ;
         Sys_SendMessage ( Parms->hwndLeftMargin, EM_SETTEXTLIMIT, MPFROMSHORT(10), 0 ) ;
         Sys_SetWindowText ( Parms->hwndLeftMargin, PSZ(Text) ) ;

         FormatDecimal ( Text, Parms->DocParms.RightMargin, 1000, 3 ) ;
         Parms->hwndRightMargin = CHILD ( hwnd, IDD_DEFAULTS_RIGHT ) ;
         Sys_SendMessage ( Parms->hwndRightMargin, EM_SETTEXTLIMIT, MPFROMSHORT(10), 0 ) ;
         Sys_SetWindowText ( Parms->hwndRightMargin, PSZ(Text) ) ;

         FormatDecimal ( Text, Parms->DocParms.TopMargin, 1000, 3 ) ;
         Parms->hwndTopMargin = CHILD ( hwnd, IDD_DEFAULTS_TOP ) ;
         Sys_SendMessage ( Parms->hwndTopMargin, EM_SETTEXTLIMIT, MPFROMSHORT(10), 0 ) ;
         Sys_SetWindowText ( Parms->hwndTopMargin, PSZ(Text) ) ;

         FormatDecimal ( Text, Parms->DocParms.BottomMargin, 1000, 3 ) ;
         Parms->hwndBottomMargin = CHILD ( hwnd, IDD_DEFAULTS_BOTTOM ) ;
         Sys_SendMessage ( Parms->hwndBottomMargin, EM_SETTEXTLIMIT, MPFROMSHORT(10), 0 ) ;
         Sys_SetWindowText ( Parms->hwndBottomMargin, PSZ(Text) ) ;

         break; }

      case WM_CONTROL: {
         PDEFAULTS_PARMS Parms = PDEFAULTS_PARMS ( Sys_GetWindowData ( hwnd ) ) ;
         SHORT Id = SHORT1FROMMP ( mp1 ) ;
         SHORT Message = SHORT2FROMMP ( mp1 ) ;
         switch ( Id ) {
            case IDD_DEFAULTS_NOTEBOOK: {
               switch ( Message ) {
                  case BKN_PAGESELECTEDPENDING: {
                     PAGESELECTNOTIFY *pInfo = (PAGESELECTNOTIFY*) PVOIDFROMMP(mp2) ;
                     // Clear the error field.
                     Sys_SetWindowText ( Parms->ErrorField, PSZ("") ) ;
                     // Get the new justification type.
                     Parms->DocParms.Justify = SHORT2FROMMR ( Sys_SendMessage ( CHILD(hwnd,IDD_DEFAULTS_JUSTIFY), VM_QUERYSELECTEDITEM, 0, 0 ) ) - 1 ;
                     // Get the new CenterPage flag.
                     Parms->DocParms.CenterPage = SHORT1FROMMR ( Sys_SendMessage ( CHILD(hwnd,IDD_DEFAULTS_CENTERPAGE), BM_QUERYCHECK, 0, 0 ) ) ;
                     // Get the new left margin.
                     if ( !VerifyLeftMargin ( WinQueryAnchorBlock(hwnd), Parms->Library, Parms->DocParms,
                        Parms->hwndLeftMargin, Parms->ErrorField,
                        &Parms->FormInfo, Parms->DocParms.LeftMargin, Parms->FormInfo.cx-Parms->DocParms.RightMargin ) ) {
                        pInfo->ulPageIdNew = 0 ;
                        break ;
                     } /* endif */
                     // Get the new right margin.
                     if ( !VerifyRightMargin ( WinQueryAnchorBlock(hwnd), Parms->Library, Parms->DocParms,
                        Parms->hwndRightMargin, Parms->ErrorField,
                        &Parms->FormInfo, Parms->DocParms.LeftMargin, Parms->DocParms.RightMargin ) ) {
                        pInfo->ulPageIdNew = 0 ;
                        break ;
                     } /* endif */
                     Parms->DocParms.RightMargin = Parms->FormInfo.cx - Parms->DocParms.RightMargin ;
                     // Get the new top margin.
                     if ( !VerifyTopMargin ( WinQueryAnchorBlock(hwnd), Parms->Library, Parms->DocParms,
                        Parms->hwndTopMargin, Parms->ErrorField,
                        &Parms->FormInfo, Parms->DocParms.BottomMargin, Parms->DocParms.TopMargin ) ) {
                        pInfo->ulPageIdNew = 0 ;
                        break ;
                     } /* endif */
                     Parms->DocParms.TopMargin = Parms->FormInfo.cy - Parms->DocParms.TopMargin ;
                     // Get the new bottom margin.
                     if ( !VerifyBottomMargin ( WinQueryAnchorBlock(hwnd), Parms->Library, Parms->DocParms,
                        Parms->hwndBottomMargin, Parms->ErrorField,
                        &Parms->FormInfo, Parms->DocParms.BottomMargin, Parms->FormInfo.cy-Parms->DocParms.TopMargin ) ) {
                        pInfo->ulPageIdNew = 0 ;
                        break ;
                     } /* endif */
                     break ; } /* endcase */
                  case BKN_PAGESELECTED: {
                     Sys_SetFocus ( CHILD(hwnd,IDD_DEFAULTS_JUSTIFY) ) ;
                     Sys_SendMessage ( WinQueryHelpInstance(Parms->Window), HM_SET_ACTIVE_WINDOW, MPFROMHWND(hwnd), MPFROMHWND(hwnd) ) ;
                     break ; } /* endcase */
               } /* endswitch */
               break ; } /* endcase */
            case IDD_DEFAULTS_LEFT: {
               if ( Message == EN_KILLFOCUS ) {
                  Sys_SetWindowText ( Parms->ErrorField, PSZ("") ) ;
                  VerifyLeftMargin ( WinQueryAnchorBlock(hwnd), Parms->Library, Parms->DocParms,
                     Parms->hwndLeftMargin, Parms->ErrorField,
                     &Parms->FormInfo, Parms->DocParms.LeftMargin, Parms->FormInfo.cx-Parms->DocParms.RightMargin ) ;
               } /* endif */
               break ; } /* endcase */
            case IDD_DEFAULTS_RIGHT: {
               if ( Message == EN_KILLFOCUS ) {
                  Sys_SetWindowText ( Parms->ErrorField, PSZ("") ) ;
                  VerifyRightMargin ( WinQueryAnchorBlock(hwnd), Parms->Library, Parms->DocParms,
                     Parms->hwndRightMargin, Parms->ErrorField,
                     &Parms->FormInfo, Parms->DocParms.LeftMargin, Parms->DocParms.RightMargin ) ;
                  Parms->DocParms.RightMargin = Parms->FormInfo.cx - Parms->DocParms.RightMargin ;
               } /* endif */
               break ; } /* endcase */
            case IDD_DEFAULTS_TOP: {
               if ( Message == EN_KILLFOCUS ) {
                  Sys_SetWindowText ( Parms->ErrorField, PSZ("") ) ;
                  VerifyTopMargin ( WinQueryAnchorBlock(hwnd), Parms->Library, Parms->DocParms,
                     Parms->hwndTopMargin, Parms->ErrorField,
                     &Parms->FormInfo, Parms->DocParms.BottomMargin, Parms->DocParms.TopMargin ) ;
                  Parms->DocParms.TopMargin = Parms->FormInfo.cy - Parms->DocParms.TopMargin ;
               } /* endif */
               break ; } /* endcase */
            case IDD_DEFAULTS_BOTTOM: {
               if ( Message == EN_KILLFOCUS ) {
                  Sys_SetWindowText ( Parms->ErrorField, PSZ("") ) ;
                  VerifyBottomMargin ( WinQueryAnchorBlock(hwnd), Parms->Library, Parms->DocParms,
                     Parms->hwndBottomMargin, Parms->ErrorField,
                     &Parms->FormInfo, Parms->DocParms.BottomMargin, Parms->FormInfo.cy-Parms->DocParms.TopMargin ) ;
               } /* endif */
               break ; } /* endcase */
         } /* endswitch */
         break; }

      case WM_COMMAND: {
         MRESULT Result = Sys_SendMessage ( PARENT(hwnd), msg, mp1, mp2 ) ; 
         #ifdef DEBUG_MESSAGES
            Indent -= 2 ;
            LogMsgDone ( Indent, "Defaults::Layout1", msg, Result ) ;
         #endif
         return ( Result ) ; }

      case WM_HELP: {
         PDEFAULTS_PARMS Parms = PDEFAULTS_PARMS ( Sys_GetWindowData ( hwnd ) ) ;
         Sys_SendMessage ( WinQueryHelpInstance(Parms->Window), HM_EXT_HELP, 0, 0 ) ;
         #ifdef DEBUG_MESSAGES
            Indent -= 2 ;
            LogMsgDone ( Indent, "Defaults::Layout1", msg, 0 ) ;
         #endif
         return ( 0 ) ; }

   } /* endswitch */

   MRESULT Result = Sys_DefDialogProc ( hwnd, msg, mp1, mp2 ) ;

   #ifdef DEBUG_MESSAGES
      Indent -= 2 ;
      LogMsgDone ( Indent, "Defaults::Layout1", msg, Result ) ;
   #endif

   return ( Result ) ;
}

/****************************************************************************
 *                                                                          *
 *  Defaults Layout Page 2 message processor                                *
 *                                                                          *
 ****************************************************************************/

static MRESULT EXPENTRY Defaults_Layout2_Processor ( HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2 ) {

   #ifdef DEBUG_MESSAGES
      static int Indent = 0 ;
      LogMsgRcvd ( Indent, "Defaults::Layout2", msg, mp1, mp2 ) ;
      Indent += 2 ;
   #endif

   switch ( msg ) {

      case WM_INITDLG: {
         PDEFAULTS_PARMS Parms = PDEFAULTS_PARMS ( PVOIDFROMMP ( mp2 ) ) ;
         Sys_SetWindowData ( hwnd, Parms ) ;
         WinAssociateHelpInstance ( WinQueryHelpInstance(Parms->Window), hwnd ) ;

         char Text [80] ;
         FormatDecimal ( Text, Parms->DocParms.TabSpacing, 1000, 3 ) ;
         Parms->hwndTabSpacing = CHILD ( hwnd, IDD_DEFAULTS_TAB ) ;
         Sys_SendMessage ( Parms->hwndTabSpacing, EM_SETTEXTLIMIT, MPFROMSHORT(10), 0 ) ;
         Sys_SetWindowText ( Parms->hwndTabSpacing, PSZ(Text) ) ;

         FormatDecimal ( Text, Parms->DocParms.Indent, 1000, 3 ) ;
         Parms->hwndIndent = CHILD ( hwnd, IDD_DEFAULTS_INDENT ) ;
         Sys_SendMessage ( Parms->hwndIndent, EM_SETTEXTLIMIT, MPFROMSHORT(10), 0 ) ;
         Sys_SetWindowText ( Parms->hwndIndent, PSZ(Text) ) ;

         switch ( Parms->DocParms.LineSpacing ) {
            case 100:
               Sys_SendMessage ( CHILD(hwnd,IDD_DEFAULTS_LINE_100), BM_SETCHECK, MPFROMSHORT(TRUE), 0 ) ;
               break;
            case 150:
               Sys_SendMessage ( CHILD(hwnd,IDD_DEFAULTS_LINE_150), BM_SETCHECK, MPFROMSHORT(TRUE), 0 ) ;
               break;
            case 200:
               Sys_SendMessage ( CHILD(hwnd,IDD_DEFAULTS_LINE_200), BM_SETCHECK, MPFROMSHORT(TRUE), 0 ) ;
               break;
         } /* endswitch */

         break; }

      case WM_CONTROL: {
         PDEFAULTS_PARMS Parms = PDEFAULTS_PARMS ( Sys_GetWindowData ( hwnd ) ) ;
         SHORT Id = SHORT1FROMMP ( mp1 ) ;
         SHORT Message = SHORT2FROMMP ( mp1 ) ;
         switch ( Id ) {
            case IDD_DEFAULTS_NOTEBOOK: {
               switch ( Message ) {
                  case BKN_PAGESELECTEDPENDING: {
                     PAGESELECTNOTIFY *pInfo = (PAGESELECTNOTIFY*) PVOIDFROMMP(mp2) ;
                     // Clear the error field.
                     Sys_SetWindowText ( Parms->ErrorField, PSZ("") ) ;
                     // Get the new default tab spacing.
                     if ( !VerifyTabSpacing ( Parms->hwndTabSpacing, Parms->ErrorField, Parms ) ) {
                        pInfo->ulPageIdNew = 0 ;
                        break ;
                     } /* endif */
                     // Get the new indent.
                     if ( !VerifyIndent ( Parms->hwndIndent, Parms->ErrorField, 
                        Parms->DocParms.Indent, Parms->FormInfo, Parms->DocParms.LeftMargin, Parms->DocParms.RightMargin ) ) {
                        pInfo->ulPageIdNew = 0 ;
                        break ;
                     } /* endif */
                     // Get the new default line spacing.
                     if ( SHORT1FROMMR ( Sys_SendMessage ( CHILD(hwnd,IDD_DEFAULTS_LINE_100), BM_QUERYCHECK, 0, 0 ) ) ) {
                        Parms->DocParms.LineSpacing = 100 ;
                     } else if ( SHORT1FROMMR ( Sys_SendMessage ( CHILD(hwnd,IDD_DEFAULTS_LINE_150), BM_QUERYCHECK, 0, 0 ) ) ) {
                        Parms->DocParms.LineSpacing = 150 ;
                     } else if ( SHORT1FROMMR ( Sys_SendMessage ( CHILD(hwnd,IDD_DEFAULTS_LINE_200), BM_QUERYCHECK, 0, 0 ) ) ) {
                        Parms->DocParms.LineSpacing = 200 ;
                     } /* endif */
                     break ; } /* endcase */
                  case BKN_PAGESELECTED: {
                     Sys_SetFocus ( CHILD(hwnd,IDD_DEFAULTS_TAB) ) ;
                     Sys_SendMessage ( WinQueryHelpInstance(Parms->Window), HM_SET_ACTIVE_WINDOW, MPFROMHWND(hwnd), MPFROMHWND(hwnd) ) ;
                     break ; } /* endcase */
               } /* endswitch */
               break ; } /* endcase */
            case IDD_DEFAULTS_TAB: {
               if ( Message == EN_KILLFOCUS ) {
                  Sys_SetWindowText ( Parms->ErrorField, PSZ("") ) ;
                  VerifyTabSpacing ( Parms->hwndTabSpacing, Parms->ErrorField, Parms ) ;
               } /* endif */
               break ; } /* endcase */
            case IDD_DEFAULTS_INDENT: {
               if ( Message == EN_KILLFOCUS ) {
                  Sys_SetWindowText ( Parms->ErrorField, PSZ("") ) ;
                  VerifyIndent ( Parms->hwndIndent, Parms->ErrorField, 
                     Parms->DocParms.Indent, Parms->FormInfo, Parms->DocParms.LeftMargin, Parms->DocParms.RightMargin ) ;
               } /* endif */
               break ; } /* endcase */
         } /* endswitch */
         break; }

      case WM_COMMAND: {
         MRESULT Result = Sys_SendMessage ( PARENT(hwnd), msg, mp1, mp2 ) ; 
         #ifdef DEBUG_MESSAGES
            Indent -= 2 ;
            LogMsgDone ( Indent, "Defaults::Layout2", msg, Result ) ;
         #endif
         return ( Result ) ; }

      case WM_HELP: {
         PDEFAULTS_PARMS Parms = PDEFAULTS_PARMS ( Sys_GetWindowData ( hwnd ) ) ;
         Sys_SendMessage ( WinQueryHelpInstance(Parms->Window), HM_EXT_HELP, 0, 0 ) ;
         #ifdef DEBUG_MESSAGES
            Indent -= 2 ;
            LogMsgDone ( Indent, "Defaults::Layout2", msg, 0 ) ;
         #endif
         return ( 0 ) ; }

   } /* endswitch */

   MRESULT Result = Sys_DefDialogProc ( hwnd, msg, mp1, mp2 ) ;

   #ifdef DEBUG_MESSAGES
      Indent -= 2 ;
      LogMsgDone ( Indent, "Defaults::Layout2", msg, Result ) ;
   #endif

   return ( Result ) ;
}

/****************************************************************************
 *                                                                          *
 *  Defaults National Page message processor                                *
 *                                                                          *
 ****************************************************************************/

static MRESULT EXPENTRY Defaults_National_Processor ( HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2 ) {

   #ifdef DEBUG_MESSAGES
      static int Indent = 0 ;
      LogMsgRcvd ( Indent, "Defaults::National", msg, mp1, mp2 ) ;
      Indent += 2 ;
   #endif

   switch ( msg ) {

      case WM_INITDLG: {
         PDEFAULTS_PARMS Parms = PDEFAULTS_PARMS ( PVOIDFROMMP ( mp2 ) ) ;
         Sys_SetWindowData ( hwnd, Parms ) ;
         WinAssociateHelpInstance ( WinQueryHelpInstance(Parms->Window), hwnd ) ;

         if ( Parms->DocParms.Metric )
            Sys_SendMessage ( CHILD(hwnd,IDD_DEFAULTS_METRIC), BM_SETCHECK, MPFROMSHORT(TRUE), 0 ) ;

         ULONG CodePages [200] ;
         ULONG CodePageCount = WinQueryCpList ( WinQueryAnchorBlock(hwnd), sizeof(CodePages)/sizeof(CodePages[0]), CodePages ) ;
         qsort ( CodePages, (size_t)CodePageCount, sizeof(ULONG), Compare_ULONG ) ;
         char Text [200] ;
         for ( int i=0; i<CodePageCount && i<(sizeof(CodePages)/sizeof(CodePages[0])); i++ ) {
            sprintf ( Text, "%li: %s", CodePages[i], ShowCodepageName(int(CodePages[i])) ) ;
            Sys_SendMessage ( CHILD(hwnd,IDD_DEFAULTS_CODEPAGE), LM_INSERTITEM,
               MPFROMSHORT ( LIT_END ), MPFROMP ( PCHAR(Text) ) ) ;
            if ( CodePages[i] == Parms->DocParms.CodePage ) {
               Sys_SendMessage ( CHILD(hwnd,IDD_DEFAULTS_CODEPAGE), LM_SELECTITEM, MPFROMSHORT(i), MPFROMSHORT(TRUE) ) ;
            } /* endif */
         } /* endfor */
         sprintf ( Text, "%li: %s", Parms->DocParms.CodePage, ShowCodepageName(Parms->DocParms.CodePage) ) ;
         Sys_SetWindowText ( CHILD(hwnd,IDD_DEFAULTS_CODEPAGE), PSZ(Text) ) ;

         UCHAR Local [sizeof(Parms->DocParms.DateFormat)] ;
         LocalDateFormat ( PCHAR(Local), PCHAR(Parms->DocParms.DateFormat) ) ;
         Sys_SendMessage ( CHILD(hwnd,IDD_DEFAULTS_DATEFMT), EM_SETTEXTLIMIT, MPFROMLONG(sizeof(Parms->DocParms.DateFormat)-1), 0 ) ;
         Sys_SetWindowText ( CHILD(hwnd,IDD_DEFAULTS_DATEFMT), PSZ(Local) ) ;

         break; }

      case WM_CONTROL: {
         PDEFAULTS_PARMS Parms = PDEFAULTS_PARMS ( Sys_GetWindowData ( hwnd ) ) ;
         SHORT Id = SHORT1FROMMP ( mp1 ) ;
         SHORT Message = SHORT2FROMMP ( mp1 ) ;
         switch ( Id ) {
            case IDD_DEFAULTS_NOTEBOOK: {
               switch ( Message ) {
                  case BKN_PAGESELECTEDPENDING: {
                     PAGESELECTNOTIFY *pInfo = (PAGESELECTNOTIFY*) PVOIDFROMMP(mp2) ;
                     // Clear the error field.
                     Sys_SetWindowText ( Parms->ErrorField, PSZ("") ) ;
                     // Get the new codepage.
                     char Text [200] ;
                     Sys_GetWindowText ( CHILD(hwnd,IDD_DEFAULTS_CODEPAGE), Text, sizeof(Text) ) ;
                     Parms->DocParms.CodePage = USHORT ( atoi ( Text ) ) ;
                     // Get the new date format.
                     if ( !VerifyDateFormat ( CHILD(hwnd,IDD_DEFAULTS_DATEFMT), Parms->ErrorField, CHILD(hwnd,IDD_DEFAULTS_DATESMP), Parms->DocParms.DateFormat ) ) {
                        pInfo->ulPageIdNew = 0 ;
                        break ;
                     } /* endif */
                     // Get the new metric flag.
                     Parms->DocParms.Metric = SHORT1FROMMR ( Sys_SendMessage ( CHILD(hwnd,IDD_DEFAULTS_METRIC), BM_QUERYCHECK, 0, 0 ) ) ;
                     break ; } /* endcase */
                  case BKN_PAGESELECTED: {
                     Sys_SetFocus ( CHILD(hwnd,IDD_DEFAULTS_CODEPAGE) ) ;
                     Sys_SendMessage ( WinQueryHelpInstance(Parms->Window), HM_SET_ACTIVE_WINDOW, MPFROMHWND(hwnd), MPFROMHWND(hwnd) ) ;
                     break ; } /* endcase */
               } /* endswitch */
               break ; } /* endcase */
            case IDD_DEFAULTS_DATEFMT: {
               if ( ( Message == EN_KILLFOCUS ) OR ( Message == EN_CHANGE ) ) {
                  Sys_SetWindowText ( Parms->ErrorField, PSZ("") ) ;
                  VerifyDateFormat ( CHILD(hwnd,IDD_DEFAULTS_DATEFMT), Parms->ErrorField, CHILD(hwnd,IDD_DEFAULTS_DATESMP), Parms->DocParms.DateFormat ) ;
               } /* endif */
               break ; } /* endcase */
            case IDD_DEFAULTS_METRIC: {
               switch ( Message ) {
                  case BN_CLICKED: {
                     // Get new metric units flag.  If not changed, just break out.
                     BOOL Metric = int ( SHORT1FROMMR ( Sys_SendMessage ( CHILD(hwnd,IDD_DEFAULTS_METRIC), BM_QUERYCHECK, 0, 0 ) ) ) ;
                     if ( Metric == Parms->DocParms.Metric ) 
                        break ;
                     // Save the new metric units flag.
                     Parms->DocParms.Metric = int ( Metric ) ;
                     // Update the units text.
                     ResourceString Units ( Parms->Library, Parms->DocParms.Metric ? IDS_UNITS_METRIC : IDS_UNITS_ENGLISH ) ;
                     Sys_SetWindowText ( Parms->hwndUnits, PSZ(Units) ) ;
                     // Update the printer form measurements.
                     QueueInfo Info ( "Defaults::Control", WinQueryAnchorBlock(hwnd), Parms->DocParms.Printer, Parms->DocParms.FormName, PDRIVDATA(Parms->DocParms.JobSettings) ) ;
                     Info.QueryForm ( WinQueryAnchorBlock(hwnd), Parms->DocParms.Metric, Parms->FormInfo ) ;
                     ResourceString SelectedForm ( Parms->Library, IDS_SELECTED_FORM ) ;
                     char FormName [80] ;
                     sprintf ( FormName, PCHAR(SelectedForm), Parms->FormInfo.szFormname ) ;
                     Sys_SetWindowText ( Parms->hwndFormName, PSZ(FormName) ) ;
                     ShowFormSize ( WinQueryAnchorBlock(hwnd), Parms->Library, Parms->DocParms,
                        Parms->hwndFormSize, &Parms->FormInfo, "" ) ;
                     char Text [11] ;
                     double Value ;
                     // Convert the left margin.
                     Sys_GetWindowText ( Parms->hwndLeftMargin, Text, sizeof(Text)-1 ) ;
                     DecodeDecimal ( Text, Value ) ;
                     Value *= ( Parms->DocParms.Metric ? 2.540 : 0.3938 ) ;
                     Parms->DocParms.LeftMargin = (long)( Value * 1000 + 0.4 ) ;
                     FormatDecimal ( Text, Parms->DocParms.LeftMargin, 1000, 3 ) ;
                     Sys_SetWindowText ( Parms->hwndLeftMargin, PSZ(Text) ) ;
                     // Convert the right margin.
                     Sys_GetWindowText ( Parms->hwndRightMargin, Text, sizeof(Text)-1 ) ;
                     DecodeDecimal ( Text, Value ) ;
                     Value *= ( Parms->DocParms.Metric ? 2.540 : 0.3938 ) ;
                     Parms->DocParms.RightMargin = (long)( Value * 1000 + 0.4 ) ;
                     FormatDecimal ( Text, Parms->DocParms.RightMargin, 1000, 3 ) ;
                     Sys_SetWindowText ( Parms->hwndRightMargin, PSZ(Text) ) ;
                     // Convert the top margin.
                     Sys_GetWindowText ( Parms->hwndTopMargin, Text, sizeof(Text)-1 ) ;
                     DecodeDecimal ( Text, Value ) ;
                     Value *= ( Parms->DocParms.Metric ? 2.540 : 0.3938 ) ;
                     Parms->DocParms.TopMargin = (long)( Value * 1000 + 0.4 ) ;
                     FormatDecimal ( Text, Parms->DocParms.TopMargin, 1000, 3 ) ;
                     Sys_SetWindowText ( Parms->hwndTopMargin, PSZ(Text) ) ;
                     // Convert the bottom margin.
                     Sys_GetWindowText ( Parms->hwndBottomMargin, Text, sizeof(Text)-1 ) ;
                     DecodeDecimal ( Text, Value ) ;
                     Value *= ( Parms->DocParms.Metric ? 2.540 : 0.3938 ) ;
                     Parms->DocParms.BottomMargin = (long)( Value * 1000 + 0.4 ) ;
                     FormatDecimal ( Text, Parms->DocParms.BottomMargin, 1000, 3 ) ;
                     Sys_SetWindowText ( Parms->hwndBottomMargin, PSZ(Text) ) ;
                     // Convert the indent.
                     Sys_GetWindowText ( Parms->hwndIndent, Text, sizeof(Text)-1 ) ;
                     DecodeDecimal ( Text, Value ) ;
                     Value *= ( Parms->DocParms.Metric ? 2.540 : 0.3938 ) ;
                     Parms->DocParms.Indent = int ( Value * 1000 + ( Value >= 0.0 ? 0.4 : -0.4 ) ) ;
                     FormatDecimal ( Text, Parms->DocParms.Indent, 1000, 3 ) ;
                     Sys_SetWindowText ( Parms->hwndIndent, PSZ(Text) ) ;
                     // Convert the tab spacing.
                     Sys_GetWindowText ( Parms->hwndTabSpacing, Text, sizeof(Text)-1 ) ;
                     DecodeDecimal ( Text, Value ) ;
                     Value *= ( Parms->DocParms.Metric ? 2.540 : 0.3938 ) ;
                     Parms->DocParms.TabSpacing = int ( Value * 1000 + 0.4 ) ;
                     FormatDecimal ( Text, Parms->DocParms.TabSpacing, 1000, 3 ) ;
                     Sys_SetWindowText ( Parms->hwndTabSpacing, PSZ(Text) ) ;
                     break ; } /* endcase */
               } /* endswitch */
               break ; } /* endcase */
         } /* endswitch */
         break; }

      case WM_COMMAND: {
         MRESULT Result = Sys_SendMessage ( PARENT(hwnd), msg, mp1, mp2 ) ; 
         #ifdef DEBUG_MESSAGES
            Indent -= 2 ;
            LogMsgDone ( Indent, "Defaults::National", msg, Result ) ;
         #endif
         return ( Result ) ; }

      case WM_HELP: {
         PDEFAULTS_PARMS Parms = PDEFAULTS_PARMS ( Sys_GetWindowData ( hwnd ) ) ;
         Sys_SendMessage ( WinQueryHelpInstance(Parms->Window), HM_EXT_HELP, 0, 0 ) ;
         #ifdef DEBUG_MESSAGES
            Indent -= 2 ;
            LogMsgDone ( Indent, "Defaults::National", msg, 0 ) ;
         #endif
         return ( 0 ) ; }

   } /* endswitch */

   MRESULT Result = Sys_DefDialogProc ( hwnd, msg, mp1, mp2 ) ;

   #ifdef DEBUG_MESSAGES
      Indent -= 2 ;
      LogMsgDone ( Indent, "Defaults::National", msg, Result ) ;
   #endif

   return ( Result ) ;
}

