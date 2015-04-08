/******************************************************************* TABS.CPP
 *                                                                          *
 *                               Tabs Dialog                                *
 *                                                                          *
 ****************************************************************************/

#include "System.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Debug.h"
#include "Dialog2.h"
#include "Escriba.h"
#include "HRuler.h"
#include "ReString.h"
#include "Support.h"
#include "Tabs.h"

//#define DEBUG
 
/****************************************************************************
 *                                                                          *
 *                     Definitions & Declarations                           *
 *                                                                          *
 ****************************************************************************/

  // Function Prototypes

static WINDOWPROC InitDlg ;
static WINDOWPROC Command ;
static WINDOWPROC TabSet ;
static WINDOWPROC TabClear ;
static WINDOWPROC OK ;
static WINDOWPROC Cancel ;
static WINDOWPROC Apply ;

 
/****************************************************************************
 *                                                                          *
 *      Dialog Message Processor                                            *
 *                                                                          *
 ****************************************************************************/

extern MRESULT EXPENTRY TabsProcessor ( HWND Window, MESG msg, MPARAM1 mp1, MPARAM2 mp2 ) {

  /**************************************************************************
   * Dispatch the message according to the method table and return the      *
   *   result.  Any messages not defined above get handled by the system    *
   *   default dialog processor.                                            *
   **************************************************************************/

   static METHOD Methods [] = {
      { WM_INITDLG, InitDlg },
      { WM_COMMAND, Command },
   } ;

   #ifdef DEBUG
      static int Indent = 0 ;
      LogMsgRcvd ( Indent, "TABS", Window, msg, mp1, mp2 ) ;
      Indent += 2 ;
   #endif

   MRESULT Result = DispatchMessage ( Window, msg, mp1, mp2, Methods, sizeof(Methods)/sizeof(Methods[0]), Dialog_Processor ) ;

   #ifdef DEBUG
      Indent -= 2 ;
      LogMsgDone ( Indent, "TABS", msg, Result ) ;
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

   PTABS_PARMS Parms = PTABS_PARMS ( PVOIDFROMMP ( mp2 ) ) ;

   Sys_SetWindowData ( Window, Parms ) ;

  /**************************************************************************
   * Set the current measurement units.                                     *
   **************************************************************************/

   ResourceString English ( Parms->Library, IDS_UNITS_ENGLISH ) ;
   ResourceString Metric ( Parms->Library, IDS_UNITS_METRIC ) ;

   Sys_SetWindowText ( CHILD(Window,IDD_TABS_UNITS), Parms->Metric ? PSZ(Metric) : PSZ(English) ) ;

  /**************************************************************************
   * Set the page size.                                                     *
   **************************************************************************/

   ResourceString Margins ( Parms->Library, IDS_TABS_MARGINS ) ;
   char String1 [20] ;  FormatDecimal ( String1, Parms->LeftMargin, 1000, 3 ) ;
   char String2 [20] ;  FormatDecimal ( String2, Parms->RightMargin, 1000, 3 ) ;
   char Text [100] ;
   sprintf ( Text, PCHAR(Margins), String1, String2 ) ;
   Sys_SetWindowText ( CHILD(Window,IDD_TABS_SIZE), Text ) ;

  /**************************************************************************
   * Return no error.                                                       *
   **************************************************************************/

   return ( Dialog_Processor ( Window, msg, mp1, mp2 ) ) ;
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
      { IDD_TABS_SET,   TabSet   },
      { IDD_TABS_CLEAR, TabClear },
      { DID_OK,         OK       },
      { DID_CANCEL,     Cancel   },
      { IDD_TABS_APPLY, Apply    },
   } ;

   return ( DispatchMessage ( Window, SHORT1FROMMP(mp1), mp1, mp2, Methods,
      sizeof(Methods)/sizeof(Methods[0]), 0 ) ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process a Tab Set request.                                          *
 *                                                                          *
 ****************************************************************************/

static MRESULT APIENTRY TabSet ( HWND Window, MESG, MPARAM1, MPARAM2 ) {

  /**************************************************************************
   * Find the instance data.                                                *
   **************************************************************************/

   PTABS_PARMS Parms = PTABS_PARMS ( Sys_GetWindowData ( Window ) ) ;

  /**************************************************************************
   * Query the entry field for the tab value to be set.                     *
   **************************************************************************/

   char Text [80] ;
   Sys_GetWindowText ( CHILD(Window,IDD_TABS_POSITION), Text, sizeof(Text) ) ;

   double Double ;
   if ( !DecodeDecimal ( Text, Double ) ) {
      ResourceString Message ( Parms->Library, IDS_TABS_ERROR1 ) ;
      Sys_SetWindowText ( CHILD(Window,IDD_TABS_ERR), PSZ(Message) ) ;
      Sys_BeepError ( ) ;
      Sys_SetFocus ( CHILD ( Window, IDD_TABS_POSITION ) ) ;
      return ( 0 ) ;
   } /* endif */

   int Tab = int ( Double * 1000 + 0.4 ) ;
   FormatDecimal ( Text, Tab, 1000, 3 ) ;
   Sys_SetWindowText ( CHILD(Window,IDD_TABS_POSITION), Text ) ;

   if ( Tab < Parms->LeftMargin ) {
      ResourceString Message ( Parms->Library, IDS_TABS_ERROR2 ) ;
      Sys_SetWindowText ( CHILD(Window,IDD_TABS_ERR), PSZ(Message) ) ;
      Sys_BeepError ( ) ;
      Sys_SetFocus ( CHILD ( Window, IDD_TABS_POSITION ) ) ;
      return ( 0 ) ;
   } /* endif */

   if ( Tab > Parms->RightMargin ) {
      ResourceString Message ( Parms->Library, IDS_TABS_ERROR3 ) ;
      Sys_SetWindowText ( CHILD(Window,IDD_TABS_ERR), PSZ(Message) ) ;
      Sys_BeepError ( ) ;
      Sys_SetFocus ( CHILD ( Window, IDD_TABS_POSITION ) ) ;
      return ( 0 ) ;
   } /* endif */

   for ( int i=0; i<Parms->TabCount; i++ ) {
      if ( Parms->Tabs[i] == Tab ) {
         ResourceString Message ( Parms->Library, IDS_TABS_ERROR4 ) ;
         Sys_SetWindowText ( CHILD(Window,IDD_TABS_ERR), PSZ(Message) ) ;
         Sys_BeepError ( ) ;
         Sys_SetFocus ( CHILD ( Window, IDD_TABS_POSITION ) ) ;
         return ( 0 ) ;
      } /* endif */
   } /* endfor */

   if ( Parms->TabCount >= MAX_TABS ) {
      ResourceString Message ( Parms->Library, IDS_TABS_ERROR6 ) ;
      Sys_SetWindowText ( CHILD(Window,IDD_TABS_ERR), PSZ(Message) ) ;
      Sys_BeepError ( ) ;
      Sys_SetFocus ( CHILD ( Window, IDD_TABS_POSITION ) ) ;
      return ( 0 ) ;
   } /* endif */

  /**************************************************************************
   * Update the tab list.                                                   *
   **************************************************************************/

   for ( i=Parms->TabCount; i>0; i-- ) {
      if ( Parms->Tabs[i-1] > Tab ) {
         Parms->Tabs[i] = Parms->Tabs[i-1] ;
      } else {
         Parms->Tabs[i] = Tab ;
         break ;
      } /* endif */
   } /* endfor */

   if ( i == 0 ) {
      Parms->Tabs[i] = Tab ;
   } /* endif */

   Parms->TabCount ++ ;

  /**************************************************************************
   * Update the owner window's horizontal ruler.                            *
   **************************************************************************/

   HORZRULER_SETPARMS HorzRulerParms ;
   HorzRulerParms.Metric      = Parms->Metric ;
   HorzRulerParms.fxZoom      = Parms->fxZoom ;
   HorzRulerParms.TopLeft     = Parms->TopLeft ;
   HorzRulerParms.PageWidth   = Parms->PageWidth ;
   HorzRulerParms.LeftMargin  = Parms->LeftMargin ;
   HorzRulerParms.RightMargin = Parms->RightMargin ;
   HorzRulerParms.TabCount    = Parms->TabCount ;
   memcpy ( HorzRulerParms.Tabs, Parms->Tabs, sizeof(HorzRulerParms.Tabs) ) ;

   Sys_SendMessage ( Parms->RulerWindow, WM_SET_RULER, MPFROMP(&HorzRulerParms), 0 ) ;

  /**************************************************************************
   * Clear the error field and return the focus to the entry field.         *
   **************************************************************************/

   Sys_SetWindowText ( CHILD(Window,IDD_TABS_ERR), "" ) ;

   Sys_SetFocus ( CHILD ( Window, IDD_TABS_POSITION ) ) ;

   return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process a Tab Clear request.                                        *
 *                                                                          *
 ****************************************************************************/

static MRESULT APIENTRY TabClear ( HWND Window, MESG, MPARAM1, MPARAM2 ) {

  /**************************************************************************
   * Find the instance data.                                                *
   **************************************************************************/

   PTABS_PARMS Parms = PTABS_PARMS ( Sys_GetWindowData ( Window ) ) ;

  /**************************************************************************
   * Query the entry field for the tab value to be set.                     *
   **************************************************************************/

   char Text [80] ;
   Sys_GetWindowText ( CHILD(Window,IDD_TABS_POSITION), Text, sizeof(Text) ) ;

   double Double = 0 ;
   if ( !DecodeDecimal ( Text, Double ) ) {
      ResourceString Message ( Parms->Library, IDS_TABS_ERROR1 ) ;
      Sys_SetWindowText ( CHILD(Window,IDD_TABS_ERR), PSZ(Message) ) ;
      Sys_BeepError ( ) ;
      Sys_SetFocus ( CHILD ( Window, IDD_TABS_POSITION ) ) ;
      return ( 0 ) ;
   } /* endif */

   LONG Tab = LONG ( Double * 1000 + 0.4 ) ;
   FormatDecimal ( Text, Tab, 1000, 3 ) ;
   Sys_SetWindowText ( CHILD(Window,IDD_TABS_POSITION), Text ) ;

   for ( int i=0; i<Parms->TabCount; i++ ) {
      if ( Parms->Tabs[i] == Tab ) {
         break ;
      } /* endif */
   } /* endfor */

   if ( i >= Parms->TabCount ) {
      ResourceString Message ( Parms->Library, IDS_TABS_ERROR5 ) ;
      Sys_SetWindowText ( CHILD(Window,IDD_TABS_ERR), PSZ(Message) ) ;
      Sys_BeepError ( ) ;
      Sys_SetFocus ( CHILD ( Window, IDD_TABS_POSITION ) ) ;
      return ( 0 ) ;
   } /* endif */

  /**************************************************************************
   * Update the tab list.                                                   *
   **************************************************************************/

   for ( i=0; i<Parms->TabCount; i++ ) {
      if ( Parms->Tabs[i] == Tab ) {
         while ( i<Parms->TabCount-1 ) {
            Parms->Tabs[i] = Parms->Tabs[i+1] ;
            i++ ;
         } /* endwhile */
         Parms->TabCount-- ;
         break ;
      } /* endif */
   } /* endfor */

  /**************************************************************************
   * Update the owner window's horizontal ruler.                            *
   **************************************************************************/

   HORZRULER_SETPARMS HorzRulerParms ;
   HorzRulerParms.Metric      = Parms->Metric ;
   HorzRulerParms.fxZoom      = Parms->fxZoom ;
   HorzRulerParms.TopLeft     = Parms->TopLeft ;
   HorzRulerParms.PageWidth   = Parms->PageWidth ;
   HorzRulerParms.LeftMargin  = Parms->LeftMargin ;
   HorzRulerParms.RightMargin = Parms->RightMargin ;
   HorzRulerParms.TabCount    = Parms->TabCount ;
   memcpy ( HorzRulerParms.Tabs, Parms->Tabs, sizeof(HorzRulerParms.Tabs) ) ;

   Sys_SendMessage ( Parms->RulerWindow, WM_SET_RULER, MPFROMP(&HorzRulerParms), 0 ) ;

  /**************************************************************************
   * Clear the error field and return the focus to the entry field.         *
   **************************************************************************/

   Sys_SetWindowText ( CHILD(Window,IDD_TABS_ERR), "" ) ;

   Sys_SetFocus ( CHILD ( Window, IDD_TABS_POSITION ) ) ;

   return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process the dialog's OK button being pressed.                       *
 *                                                                          *
 ****************************************************************************/

static MRESULT APIENTRY OK ( HWND Window, MESG, MPARAM1, MPARAM2 ) {

  /**************************************************************************
   * Dismiss the dialog with a TRUE status.                                 *
   **************************************************************************/

   Sys_EndDialog ( Window, TRUE ) ;

   return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process the dialog's cancel button being pressed.                   *
 *                                                                          *
 ****************************************************************************/

static MRESULT APIENTRY Cancel ( HWND Window, MESG, MPARAM1, MPARAM2 ) {

  /**************************************************************************
   * Dismiss the dialog with a TRUE status.                                 *
   **************************************************************************/

   Sys_EndDialog ( Window, FALSE ) ;

   return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Apply the Tabs                                                      *
 *                                                                          *
 ****************************************************************************/

static MRESULT APIENTRY Apply ( HWND Window, MESG, MPARAM1, MPARAM2 ) {

  /**************************************************************************
   * Find the instance data.                                                *
   **************************************************************************/

   PTABS_PARMS Parms = PTABS_PARMS ( Sys_GetWindowData ( Window ) ) ;

  /**************************************************************************
   * Set the current document's tabs.                                       *
   **************************************************************************/

   Sys_SendMessage ( Parms->Window, WM_SET_TABS, MPFROMP(Parms), 0 ) ;

   return ( 0 ) ;
}
