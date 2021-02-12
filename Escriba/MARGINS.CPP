/**************************************************************** MARGINS.CPP
 *                                                                          *
 *                             Margins Dialog                               *
 *                                                                          *
 ****************************************************************************/

#include "System.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Debug.h"
#include "Defaults.h"
#include "Dialog2.h"
#include "Document.h"
#include "Escriba.h"
#include "HRuler.h"
#include "Margins.h"
#include "Printer.h"
#include "ReString.h"
#include "Support.h"
#include "VRuler.h"

//#define DEBUG

 
/****************************************************************************
 *                                                                          *
 *                     Definitions & Declarations                           *
 *                                                                          *
 ****************************************************************************/

  // Function Prototypes

static WINDOWPROC InitDlg ;
static WINDOWPROC Control ;
static WINDOWPROC Command ;
static WINDOWPROC OK ;
static WINDOWPROC Cancel ;
static WINDOWPROC Apply ;

 
/****************************************************************************
 *                                                                          *
 *      Dialog Message Processor                                            *
 *                                                                          *
 ****************************************************************************/

extern MRESULT EXPENTRY MarginsProcessor ( HWND Window, MESG msg, MPARAM1 mp1, MPARAM2 mp2 ) {

  /**************************************************************************
   * Dispatch the message according to the method table and return the      *
   *   result.  Any messages not defined above get handled by the system    *
   *   default dialog processor.                                            *
   **************************************************************************/

   static METHOD Methods [] = {
      { WM_INITDLG, InitDlg },
      { WM_CONTROL, Control },
      { WM_COMMAND, Command },
   } ;

   #ifdef DEBUG
      static int Indent = 0 ;
      LogMsgRcvd ( Indent, "MARGINS", msg, mp1, mp2 ) ;
      Indent += 2 ;
   #endif

   MRESULT Result = DispatchMessage ( Window, msg, mp1, mp2, Methods, sizeof(Methods)/sizeof(Methods[0]), Dialog_Processor ) ;

   #ifdef DEBUG
      Indent -= 2 ;
      LogMsgDone ( Indent, "MARGINS", msg, Result ) ;
   #endif

   return ( Result ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Initialize Dialog                                                   *
 *                                                                          *
 ****************************************************************************/

static MRESULT APIENTRY InitDlg ( HWND Window, MESG msg, MPARAM1 mp1, MPARAM2 mp2 ) {

  /**************************************************************************
   * Get parameters from initialization message.                            *
   **************************************************************************/

   PMARGINS_PARMS Parms = PMARGINS_PARMS ( PVOIDFROMMP ( mp2 ) ) ;

   Sys_SetWindowData ( Window, Parms ) ;

  /**************************************************************************
   * Fill the entry fields.                                                 *
   **************************************************************************/

   char Text [80] ;

   FormatDecimal ( Text, Parms->Margins.xLeft, 1000, 3 ) ;
   Sys_SetWindowText ( CHILD(Window,IDD_MARGINS_LEFT), PSZ(Text) ) ;

   FormatDecimal ( Text, Parms->PageSize.cx-Parms->Margins.xRight, 1000, 3 ) ;
   Sys_SetWindowText ( CHILD(Window,IDD_MARGINS_RIGHT), PSZ(Text) ) ;

   FormatDecimal ( Text, Parms->PageSize.cy-Parms->Margins.yTop, 1000, 3 ) ;
   Sys_SetWindowText ( CHILD(Window,IDD_MARGINS_TOP), PSZ(Text) ) ;

   FormatDecimal ( Text, Parms->Margins.yBottom, 1000, 3 ) ;
   Sys_SetWindowText ( CHILD(Window,IDD_MARGINS_BOTTOM), PSZ(Text) ) ;

  /**************************************************************************
   * Set the current measurement units.                                     *
   **************************************************************************/

   ResourceString English ( Parms->Library, IDS_UNITS_ENGLISH ) ;
   ResourceString Metric ( Parms->Library, IDS_UNITS_METRIC ) ;

   Sys_SetWindowText ( CHILD(Window,IDD_MARGINS_UNITS), Parms->Metric ? PSZ(Metric) : PSZ(English) ) ;

  /**************************************************************************
   * Show the page size.                                                    *
   **************************************************************************/

   ResourceString PageSize ( Parms->Library, IDS_MARGINS_PAGESIZE ) ;
   ShowFormSize ( WinQueryAnchorBlock(Window), Parms->Library, Parms->DocParms, CHILD(Window,IDD_MARGINS_SIZE), 0, PCHAR(PageSize) ) ;

  /**************************************************************************
   * Return no error.                                                       *
   **************************************************************************/

   return ( Dialog_Processor ( Window, msg, mp1, mp2 ) ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process control notifications.                                      *
 *                                                                          *
 ****************************************************************************/

static MRESULT APIENTRY Control ( HWND Window, MESG, MPARAM1 mp1, MPARAM2 ) {

  /**************************************************************************
   * Find the instance data.                                                *
   **************************************************************************/

   PMARGINS_PARMS Parms = PMARGINS_PARMS ( Sys_GetWindowData ( Window ) ) ;

  /**************************************************************************
   * Decode the message.  Find out what control sent it, and what the       *
   *   control had to say.                                                  *
   **************************************************************************/

   USHORT id = SHORT1FROMMP ( mp1 ) ;
   USHORT Message = SHORT2FROMMP ( mp1 ) ;

  /**************************************************************************
   * Handle changes according to which control reported them.               *
   **************************************************************************/

   switch ( id ) {

      case IDD_MARGINS_LEFT: {
         if ( Message == EN_KILLFOCUS ) {
            Sys_SetWindowText ( CHILD(Window,IDD_MARGINS_ERR), "" ) ;
            VerifyLeftMargin ( WinQueryAnchorBlock(Window), Parms->Library, Parms->DocParms,
               CHILD(Window,IDD_MARGINS_LEFT), CHILD(Window,IDD_MARGINS_ERR), 0,
               Parms->Margins.xLeft, Parms->Margins.xRight ) ;
         } /* endif */
         return ( 0 ) ; } /* endcase */

      case IDD_MARGINS_RIGHT: {
         if ( Message == EN_KILLFOCUS ) {
            Sys_SetWindowText ( CHILD(Window,IDD_MARGINS_ERR), "" ) ;
            VerifyRightMargin ( WinQueryAnchorBlock(Window), Parms->Library, Parms->DocParms,
               CHILD(Window,IDD_MARGINS_RIGHT), CHILD(Window,IDD_MARGINS_ERR), 0,
               Parms->Margins.xLeft, Parms->Margins.xRight ) ;
         } /* endif */
         return ( 0 ) ; } /* endcase */

      case IDD_MARGINS_TOP: {
         if ( Message == EN_KILLFOCUS ) {
            Sys_SetWindowText ( CHILD(Window,IDD_MARGINS_ERR), "" ) ;
            VerifyTopMargin ( WinQueryAnchorBlock(Window), Parms->Library, Parms->DocParms,
               CHILD(Window,IDD_MARGINS_TOP), CHILD(Window,IDD_MARGINS_ERR), 0,
               Parms->Margins.yBottom, Parms->Margins.yTop ) ;
         } /* endif */
         return ( 0 ) ; } /* endcase */

      case IDD_MARGINS_BOTTOM: {
         if ( Message == EN_KILLFOCUS ) {
            Sys_SetWindowText ( CHILD(Window,IDD_MARGINS_ERR), "" ) ;
            VerifyBottomMargin ( WinQueryAnchorBlock(Window), Parms->Library, Parms->DocParms,
               CHILD(Window,IDD_MARGINS_BOTTOM), CHILD(Window,IDD_MARGINS_ERR), 0,
               Parms->Margins.yBottom, Parms->Margins.yTop ) ;
         } /* endif */
         return ( 0 ) ; } /* endcase */

   } /* endswitch */

   return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process commands received by the dialog window.                     *
 *                                                                          *
 ****************************************************************************/

static MRESULT APIENTRY Command ( HWND Window, MESG, MPARAM1 mp1, MPARAM2 mp2 ) {

  /**************************************************************************
   * Dispatch the message without a default message processor.              *
   **************************************************************************/

   static METHOD Methods [] = {
      { DID_OK,            OK     },
      { DID_CANCEL,        Cancel },
      { IDD_MARGINS_APPLY, Apply  },
   } ;

   return ( DispatchMessage ( Window, SHORT1FROMMP(mp1), mp1, mp2, Methods, sizeof(Methods)/sizeof(Methods[0]), 0 ) ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process the dialog's OK button being pressed.                       *
 *                                                                          *
 ****************************************************************************/

static MRESULT APIENTRY OK ( HWND Window, MESG, MPARAM1, MPARAM2 ) {

 /***************************************************************************
  * Find the instance data.                                                 *
  ***************************************************************************/

  PMARGINS_PARMS Parms = PMARGINS_PARMS ( Sys_GetWindowData ( Window ) ) ;

 /***************************************************************************
  * Fetch the new margin values and validate them.                          *
  ***************************************************************************/

  QueueInfo Info ( "Margins::OK", WinQueryAnchorBlock(Window), Parms->DocParms.Printer, Parms->DocParms.FormName, PDRIVDATA(Parms->DocParms.JobSettings) ) ;
  HCINFO FormInfo ;
  Info.QueryForm ( WinQueryAnchorBlock(Window), Parms->DocParms.Metric, FormInfo ) ;

  Sys_SetWindowText ( CHILD(Window,IDD_MARGINS_ERR), "" ) ;

  if ( !VerifyLeftMargin ( WinQueryAnchorBlock(Window), Parms->Library, Parms->DocParms,
     CHILD(Window,IDD_MARGINS_LEFT), CHILD(Window,IDD_MARGINS_ERR), &FormInfo,
     Parms->Margins.xLeft, Parms->Margins.xRight ) ) {
     return ( 0 ) ;
  } /* endif */

  if ( !VerifyRightMargin ( WinQueryAnchorBlock(Window), Parms->Library, Parms->DocParms,
     CHILD(Window,IDD_MARGINS_RIGHT), CHILD(Window,IDD_MARGINS_ERR), &FormInfo,
     Parms->Margins.xLeft, Parms->Margins.xRight ) ) {
     return ( 0 ) ;
  } /* endif */

  if ( !VerifyTopMargin ( WinQueryAnchorBlock(Window), Parms->Library, Parms->DocParms,
     CHILD(Window,IDD_MARGINS_TOP), CHILD(Window,IDD_MARGINS_ERR), &FormInfo,
     Parms->Margins.yBottom, Parms->Margins.yTop ) ) {
     return ( 0 ) ;
  } /* endif */

  if ( !VerifyBottomMargin ( WinQueryAnchorBlock(Window), Parms->Library, Parms->DocParms,
     CHILD(Window,IDD_MARGINS_BOTTOM), CHILD(Window,IDD_MARGINS_ERR), &FormInfo,
     Parms->Margins.yBottom, Parms->Margins.yTop ) ) {
     return ( 0 ) ;
  } /* endif */

 /***************************************************************************
  * Restore the new values.                                                 *
  ***************************************************************************/

  char Text [20] ;

  FormatDecimal ( Text, Parms->Margins.xLeft, 1000, 3 ) ;
  Sys_SetWindowText ( CHILD(Window,IDD_MARGINS_LEFT), Text ) ;

  FormatDecimal ( Text, Parms->PageSize.cx-Parms->Margins.xRight, 1000, 3 ) ;
  Sys_SetWindowText ( CHILD(Window,IDD_MARGINS_RIGHT), Text ) ;

  FormatDecimal ( Text, Parms->PageSize.cy-Parms->Margins.yTop, 1000, 3 ) ;
  Sys_SetWindowText ( CHILD(Window,IDD_MARGINS_TOP), Text ) ;

  FormatDecimal ( Text, Parms->Margins.yBottom, 1000, 3 ) ;
  Sys_SetWindowText ( CHILD(Window,IDD_MARGINS_BOTTOM), Text ) ;

 /***************************************************************************
  * Dismiss the dialog with a TRUE status.                                  *
  ***************************************************************************/

  Sys_EndDialog ( Window, TRUE ) ;

  return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process the dialog's cancel button being pressed.                   *
 *                                                                          *
 ****************************************************************************/

static MRESULT APIENTRY Cancel ( HWND Window, MESG, MPARAM1, MPARAM2 ) {

 /***************************************************************************
  * Dismiss the dialog with a TRUE status.                                  *
  ***************************************************************************/

  Sys_EndDialog ( Window, FALSE ) ;

  return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Apply the indent.                                                   *
 *                                                                          *
 ****************************************************************************/

static MRESULT APIENTRY Apply ( HWND Window, MESG, MPARAM1, MPARAM2 ) {

 /***************************************************************************
  * Find the instance data.                                                 *
  ***************************************************************************/

  PMARGINS_PARMS Parms = PMARGINS_PARMS ( Sys_GetWindowData ( Window ) ) ;

 /***************************************************************************
  * Fetch the new margin values and validate them.                          *
  ***************************************************************************/

  if ( !VerifyLeftMargin ( WinQueryAnchorBlock(Window), Parms->Library, Parms->DocParms,
     CHILD(Window,IDD_MARGINS_LEFT), CHILD(Window,IDD_MARGINS_ERR), 
     0, Parms->Margins.xLeft, Parms->Margins.xRight ) )
     return ( 0 ) ;

  if ( !VerifyRightMargin ( WinQueryAnchorBlock(Window), Parms->Library, Parms->DocParms,
     CHILD(Window,IDD_MARGINS_RIGHT), CHILD(Window,IDD_MARGINS_ERR),
     0, Parms->Margins.xLeft, Parms->Margins.xRight ) )
     return ( 0 ) ;

  if ( !VerifyTopMargin ( WinQueryAnchorBlock(Window), Parms->Library, Parms->DocParms,
     CHILD(Window,IDD_MARGINS_TOP), CHILD(Window,IDD_MARGINS_ERR),
     0, Parms->Margins.yBottom, Parms->Margins.yTop ) )
     return ( 0 ) ;

  if ( !VerifyBottomMargin ( WinQueryAnchorBlock(Window), Parms->Library, Parms->DocParms,
     CHILD(Window,IDD_MARGINS_BOTTOM), CHILD(Window,IDD_MARGINS_ERR),
     0, Parms->Margins.yBottom, Parms->Margins.yTop ) )
     return ( 0 ) ;

 /***************************************************************************
  * Restore the new values.                                                 *
  ***************************************************************************/

  char Text [20] ;

  FormatDecimal ( Text, Parms->Margins.xLeft, 1000, 3 ) ;
  Sys_SetWindowText ( CHILD(Window,IDD_MARGINS_LEFT), Text ) ;

  FormatDecimal ( Text, Parms->PageSize.cx-Parms->Margins.xRight, 1000, 3 ) ;
  Sys_SetWindowText ( CHILD(Window,IDD_MARGINS_RIGHT), Text ) ;

  FormatDecimal ( Text, Parms->PageSize.cy-Parms->Margins.yTop, 1000, 3 ) ;
  Sys_SetWindowText ( CHILD(Window,IDD_MARGINS_TOP), Text ) ;

  FormatDecimal ( Text, Parms->Margins.yBottom, 1000, 3 ) ;
  Sys_SetWindowText ( CHILD(Window,IDD_MARGINS_BOTTOM), Text ) ;

 /***************************************************************************
  * Set the current document's margins.                                     *
  ***************************************************************************/

  Sys_SendMessage ( Parms->Window, WM_SET_MARGINS, MPFROMP(Parms), 0 ) ;

  return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *  Verify Left Margin                                                      *
 *                                                                          *
 ****************************************************************************/

extern BOOL VerifyLeftMargin (
   HAB Anchor,
   HMODULE Library,
   DocumentParms &DocParms,
   HWND hwndField,
   HWND hwndError,
   PHCINFO pFormInfo,
   long &LeftMargin,
   long RightMargin
) {

 /***************************************************************************
  * If not passed in, get the current form size.                            *
  ***************************************************************************/

  HCINFO FormInfo ;
  if ( pFormInfo == 0 ) {
     QueueInfo Info ( "VerifyLeftMargin", Anchor, DocParms.Printer, DocParms.FormName, PDRIVDATA(DocParms.JobSettings) ) ;
     Info.QueryForm ( Anchor, DocParms.Metric, FormInfo ) ;
     pFormInfo = & FormInfo ;
  } /* endif */

 /***************************************************************************
  * Get the new value.                                                      *
  ***************************************************************************/

  char Text [11] ;
  Sys_GetWindowText ( hwndField, Text, sizeof(Text)-1 ) ;

 /***************************************************************************
  * Check for invalid value.                                                *
  ***************************************************************************/

  double Value ;
  if ( !DecodeDecimal ( Text, Value ) ) {
     ResourceString Message ( Library, IDS_MARGINS_ERROR0 ) ;
     Sys_SetWindowText ( hwndError, PCHAR(Message) ) ;
     Sys_BeepError ( ) ;
     Sys_SetFocus ( hwndField ) ;
     return ( FALSE ) ;
  } /* endif */

 /***************************************************************************
  * Check if too low.                                                       *
  ***************************************************************************/

  if ( long(Value*1000) < pFormInfo->xLeftClip ) {
     ResourceString Format ( Library, IDS_MARGINS_ERROR1 ) ;
     char String [20] ;  FormatDecimal ( String, pFormInfo->xLeftClip, 1000, 3 ) ;
     char Message [200] ;  sprintf ( Message, PCHAR(Format), String ) ;
     Sys_SetWindowText ( hwndError, Message ) ;
     Sys_BeepError ( ) ;
     Sys_SetFocus ( hwndField ) ;
     return ( FALSE ) ;

 /***************************************************************************
  * Check if too high.                                                      *
  ***************************************************************************/

  } else if ( long(Value*1000) >= RightMargin ) {
     ResourceString Message ( Library, IDS_MARGINS_ERROR3 ) ;
     Sys_SetWindowText ( hwndError, PCHAR(Message) ) ;
     Sys_BeepError ( ) ;
     Sys_SetFocus ( hwndField ) ;
     return ( FALSE ) ;
  } /* endif */

 /***************************************************************************
  * Save the value and return no error.                                     *
  ***************************************************************************/

  LeftMargin = (long)( Value * 1000 + 0.4 ) ;

  return ( TRUE ) ;
}
 
/****************************************************************************
 *                                                                          *
 *  Verify Right Margin                                                     *
 *                                                                          *
 ****************************************************************************/

extern BOOL VerifyRightMargin (
   HAB Anchor,
   HMODULE Library,
   DocumentParms &DocParms,
   HWND hwndField,
   HWND hwndError,
   PHCINFO pFormInfo,
   long LeftMargin,
   long &RightMargin
) {

 /***************************************************************************
  * If not passed in, get the current form size.                            *
  ***************************************************************************/

  HCINFO FormInfo ;
  if ( pFormInfo == 0 ) {
     QueueInfo Info ( "VerifyRightMargin", Anchor, DocParms.Printer, DocParms.FormName, PDRIVDATA(DocParms.JobSettings) ) ;
     Info.QueryForm ( Anchor, DocParms.Metric, FormInfo ) ;
     pFormInfo = & FormInfo ;
  } /* endif */

 /***************************************************************************
  * Get the new value.                                                      *
  ***************************************************************************/

  char Text [11] ;
  Sys_GetWindowText ( hwndField, Text, sizeof(Text)-1 ) ;

 /***************************************************************************
  * Check for invalid value.                                                *
  ***************************************************************************/

  double Value ;
  if ( !DecodeDecimal ( Text, Value ) ) {
     ResourceString Message ( Library, IDS_MARGINS_ERROR0 ) ;
     Sys_SetWindowText ( hwndError, PCHAR(Message) ) ;
     Sys_BeepError ( ) ;
     Sys_SetFocus ( hwndField ) ;
     return ( FALSE ) ;
  } /* endif */

  Value = ( pFormInfo->cx - long(Value*1000) ) / 1000.0 ;

 /***************************************************************************
  * Check if too high.                                                      *
  ***************************************************************************/

  if ( long(Value*1000) > pFormInfo->xRightClip ) {
     ResourceString Format ( Library, IDS_MARGINS_ERROR2 ) ;
     char String [20] ;  FormatDecimal ( String, pFormInfo->cx-pFormInfo->xRightClip, 1000, 3 ) ;
     char Message [200] ;  sprintf ( Message, PCHAR(Format), String ) ;
     Sys_SetWindowText ( hwndError, Message ) ;
     Sys_BeepError ( ) ;
     Sys_SetFocus ( hwndField ) ;
     return ( FALSE ) ;

 /***************************************************************************
  * Check if too low.                                                       *
  ***************************************************************************/

  } else if ( long(Value*1000) <= LeftMargin ) {
     ResourceString Message ( Library, IDS_MARGINS_ERROR3 ) ;
     Sys_SetWindowText ( hwndError, PCHAR(Message) ) ;
     Sys_BeepError ( ) ;
     Sys_SetFocus ( hwndField ) ;
     return ( FALSE ) ;
  } /* endif */

 /***************************************************************************
  * Save the value and return no error.                                     *
  ***************************************************************************/

  RightMargin = (long)( Value * 1000 + 0.4 ) ;

  return ( TRUE ) ;
}
 
/****************************************************************************
 *                                                                          *
 *  Verify Top Margin                                                       *
 *                                                                          *
 ****************************************************************************/

extern BOOL VerifyTopMargin (
   HAB Anchor,
   HMODULE Library,
   DocumentParms &DocParms,
   HWND hwndField,
   HWND hwndError,
   PHCINFO pFormInfo,
   long BottomMargin,
   long &TopMargin
) {

 /***************************************************************************
  * If not passed in, get the current form size.                            *
  ***************************************************************************/

  HCINFO FormInfo ;
  if ( pFormInfo == 0 ) {
     QueueInfo Info ( "VerifyTopMargin", Anchor, DocParms.Printer, DocParms.FormName, PDRIVDATA(DocParms.JobSettings) ) ;
     Info.QueryForm ( Anchor, DocParms.Metric, FormInfo ) ;
     pFormInfo = & FormInfo ;
  } /* endif */

 /***************************************************************************
  * Get the new value.                                                      *
  ***************************************************************************/

  char Text [11] ;
  Sys_GetWindowText ( hwndField, Text, sizeof(Text)-1 ) ;

 /***************************************************************************
  * Check for invalid value.                                                *
  ***************************************************************************/

  double Value ;
  if ( !DecodeDecimal ( Text, Value ) ) {
     ResourceString Message ( Library, IDS_MARGINS_ERROR0 ) ;
     Sys_SetWindowText ( hwndError, PCHAR(Message) ) ;
     Sys_BeepError ( ) ;
     Sys_SetFocus ( hwndField ) ;
     return ( FALSE ) ;
  } /* endif */

  Value = ( pFormInfo->cy - long(Value*1000) ) / 1000.0 ;

 /***************************************************************************
  * Check if too high.                                                      *
  ***************************************************************************/

  if ( long(Value*1000) > pFormInfo->yTopClip ) {
     ResourceString Format ( Library, IDS_MARGINS_ERROR5 ) ;
     char String [20] ;  FormatDecimal ( String, pFormInfo->cy-pFormInfo->yTopClip, 1000, 3 ) ;
     char Message [200] ;  sprintf ( Message, PCHAR(Format), String ) ;
     Sys_SetWindowText ( hwndError, Message ) ;
     Sys_BeepError ( ) ;
     Sys_SetFocus ( hwndField ) ;
     return ( FALSE ) ;

 /***************************************************************************
  * Check if too low.                                                       *
  ***************************************************************************/

  } else if ( long(Value*1000) <= BottomMargin ) {
     ResourceString Message ( Library, IDS_MARGINS_ERROR6 ) ;
     Sys_SetWindowText ( hwndError, PCHAR(Message) ) ;
     Sys_BeepError ( ) ;
     Sys_SetFocus ( hwndField ) ;
     return ( FALSE ) ;
  } /* endif */

 /***************************************************************************
  * Save the value and return no error.                                     *
  ***************************************************************************/

  TopMargin = (long)( Value * 1000 + 0.4 ) ;

  return ( TRUE ) ;
}
 
/****************************************************************************
 *                                                                          *
 *  Verify Bottom Margin                                                    *
 *                                                                          *
 ****************************************************************************/

extern BOOL VerifyBottomMargin (
   HAB Anchor,
   HMODULE Library,
   DocumentParms &DocParms,
   HWND hwndField,
   HWND hwndError,
   PHCINFO pFormInfo,
   long &BottomMargin,
   long TopMargin
) {

 /***************************************************************************
  * If not passed in, get the current form size.                            *
  ***************************************************************************/

  HCINFO FormInfo ;
  if ( pFormInfo == 0 ) {
     QueueInfo Info ( "VerifyBottomMargin", Anchor, DocParms.Printer, DocParms.FormName, PDRIVDATA(DocParms.JobSettings) ) ;
     Info.QueryForm ( Anchor, DocParms.Metric, FormInfo ) ;
     pFormInfo = & FormInfo ;
  } /* endif */

 /***************************************************************************
  * Get the new value.                                                      *
  ***************************************************************************/

  char Text [11] ;
  Sys_GetWindowText ( hwndField, Text, sizeof(Text)-1 ) ;

 /***************************************************************************
  * Check for invalid value.                                                *
  ***************************************************************************/

  double Value ;
  if ( !DecodeDecimal ( Text, Value ) ) {
     ResourceString Message ( Library, IDS_MARGINS_ERROR0 ) ;
     Sys_SetWindowText ( hwndError, PCHAR(Message) ) ;
     Sys_BeepError ( ) ;
     Sys_SetFocus ( hwndField ) ;
     return ( FALSE ) ;
  } /* endif */

 /***************************************************************************
  * Check if too low.                                                       *
  ***************************************************************************/

  if ( long(Value*1000) < pFormInfo->yBottomClip ) {
     ResourceString Format ( Library, IDS_MARGINS_ERROR4 ) ;
     char String [20] ;  FormatDecimal ( String, pFormInfo->yBottomClip, 1000, 3 ) ;
     char Message [200] ;  sprintf ( Message, PCHAR(Format), String ) ;
     Sys_SetWindowText ( hwndError, Message ) ;
     Sys_BeepError ( ) ;
     Sys_SetFocus ( hwndField ) ;
     return ( FALSE ) ;

 /***************************************************************************
  * Check if too high.                                                      *
  ***************************************************************************/

  } else if ( long(Value*1000) >= TopMargin ) {
     ResourceString Message ( Library, IDS_MARGINS_ERROR6 ) ;
     Sys_SetWindowText ( hwndError, PCHAR(Message) ) ;
     Sys_BeepError ( ) ;
     Sys_SetFocus ( hwndField ) ;
     return ( FALSE ) ;
  } /* endif */

 /***************************************************************************
  * Save the value and return no error.                                     *
  ***************************************************************************/

  BottomMargin = (long)( Value * 1000 + 0.4 ) ;

  return ( TRUE ) ;
}

