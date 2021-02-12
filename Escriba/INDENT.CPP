/***************************************************************** INDENT.CPP
 *                                                                          *
 *                             Indent Dialog                                *
 *                                                                          *
 ****************************************************************************/

#include "System.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Debug.h"
#include "Dialog2.h"
#include "Escriba.h"
#include "Indent.h"
#include "HRuler.h"
#include "ReString.h"
#include "Support.h"

//#define DEBUG

 
/****************************************************************************
 *                                                                          *
 *                     Definitions & Declarations                           *
 *                                                                          *
 ****************************************************************************/

  // Function Prototypes

static WINDOWPROC InitDlg ;
static WINDOWPROC Command ;
static WINDOWPROC OK ;
static WINDOWPROC Cancel ;
static WINDOWPROC Apply ;

 
/****************************************************************************
 *                                                                          *
 *      Dialog Message Processor                                            *
 *                                                                          *
 ****************************************************************************/

extern MRESULT EXPENTRY IndentProcessor ( HWND Window, MESG msg, MPARAM1 mp1, MPARAM2 mp2 ) {

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
   LogMsgRcvd ( Indent, "INDENT", msg, mp1, mp2 ) ;
   Indent += 2 ;
   #endif

   MRESULT Result = DispatchMessage ( Window, msg, mp1, mp2, Methods, sizeof(Methods)/sizeof(Methods[0]), Dialog_Processor ) ;

   #ifdef DEBUG
   Indent -= 2 ;
   LogMsgDone ( Indent, "INDENT", msg, Result ) ;
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

   PINDENT_PARMS Parms = PINDENT_PARMS ( PVOIDFROMMP ( mp2 ) ) ;

   Sys_SetWindowData ( Window, Parms ) ;

  /**************************************************************************
   * Set the current measurement units.                                     *
   **************************************************************************/

   ResourceString English ( Parms->Library, IDS_UNITS_ENGLISH ) ;
   ResourceString Metric ( Parms->Library, IDS_UNITS_METRIC ) ;

   Sys_SetWindowText ( CHILD(Window,IDD_INDENT_UNITS), Parms->Metric ? PSZ(Metric) : PSZ(English) ) ;

  /**************************************************************************
   * Set the page size.                                                     *
   **************************************************************************/

   ResourceString Margins ( Parms->Library, IDS_INDENT_MARGINS ) ;
   char String1 [20] ;  FormatDecimal ( String1, Parms->LeftMargin, 1000, 3 ) ;
   char String2 [20] ;  FormatDecimal ( String2, Parms->RightMargin, 1000, 3 ) ;
   char Text [100] ;
   sprintf ( Text, PCHAR(Margins), String1, String2 ) ;
   Sys_SetWindowText ( CHILD(Window,IDD_INDENT_SIZE), Text ) ;

  /**************************************************************************
   * Fill the entry field.                                                  *
   **************************************************************************/

   FormatDecimal ( Text, Parms->Indent, 1000, 3 ) ;
   Sys_SetWindowText ( CHILD(Window,IDD_INDENT_DISTANCE), Text ) ;

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
      { DID_OK,           OK       },
      { DID_CANCEL,       Cancel   },
      { IDD_INDENT_APPLY, Apply    },
   } ;

   return ( DispatchMessage ( Window, SHORT1FROMMP(mp1), mp1, mp2, Methods,
      sizeof(Methods)/sizeof(Methods[0]), 0 ) ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process the dialog's OK button being pressed.                       *
 *                                                                          *
 ****************************************************************************/

static MRESULT APIENTRY OK ( HWND Window, MESG, MPARAM1, MPARAM2 ) {

  /**************************************************************************
   * Find the instance data.                                                *
   **************************************************************************/

   PINDENT_PARMS Parms = PINDENT_PARMS ( Sys_GetWindowData ( Window ) ) ;

  /**************************************************************************
   * Validate it.                                                           *
   **************************************************************************/

   if ( !VerifyIndent ( CHILD(Window,IDD_INDENT_DISTANCE), CHILD(Window,IDD_INDENT_ERR), 
      Parms->Indent, Parms->FormInfo, Parms->LeftMargin, Parms->FormInfo.cx-Parms->RightMargin ) )

      return ( 0 ) ;

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
 *      Apply the indent.                                                   *
 *                                                                          *
 ****************************************************************************/

static MRESULT APIENTRY Apply ( HWND Window, MESG, MPARAM1, MPARAM2 ) {

  /**************************************************************************
   * Find the instance data.                                                *
   **************************************************************************/

   PINDENT_PARMS Parms = PINDENT_PARMS ( Sys_GetWindowData ( Window ) ) ;

  /**************************************************************************
   * Validate it.                                                           *
   **************************************************************************/

   if ( !VerifyIndent ( CHILD(Window,IDD_INDENT_DISTANCE), CHILD(Window,IDD_INDENT_ERR), 
      Parms->Indent, Parms->FormInfo, Parms->LeftMargin, Parms->FormInfo.cx-Parms->RightMargin ) )

      return ( 0 ) ;

  /**************************************************************************
   * Set the current document's indention distance.                         *
   **************************************************************************/

   Sys_SendMessage ( Parms->Window, WM_SET_INDENT, MPFROMSHORT(Parms->Indent), 0 ) ;

   return ( 0 ) ;
}

 
/****************************************************************************
 *                                                                          *
 *  Verify Indent                                                           *
 *                                                                          *
 ****************************************************************************/

extern BOOL VerifyIndent ( 
   HWND hwndField,              // -> Entryfield window
   HWND hwndError,              // -> Errorfield window
   int &Indent,                 // Current indentation.
   HCINFO &FormInfo,            // Current form information.
   long LeftMargin,             // Current left margin.
   long RightMargin             // Current right margin.
) {

  /**************************************************************************
   * Get the new value.                                                     *
   **************************************************************************/

   char Text [11] ;
   Sys_GetWindowText ( hwndField, PSZ(Text), sizeof(Text)-1 ) ;

  /**************************************************************************
   * Check for invalid value.                                               *
   **************************************************************************/

   double Value ;
   if ( !DecodeDecimal ( Text, Value ) ) {
      ResourceString Message ( LibraryHandle, IDS_INDENT_ERROR1 ) ;
      Sys_SetWindowText ( hwndError, Message ) ;
      Sys_BeepError ( ) ;
      Sys_SetFocus ( hwndField ) ;
      return ( FALSE ) ;
   } /* endif */

  /**************************************************************************
   * Check for value too low.  You can indent backwards to the left clip.   *
   **************************************************************************/

   if ( Value*1000 < double ( FormInfo.xLeftClip - LeftMargin ) ) {
      ResourceString Message ( LibraryHandle, IDS_INDENT_ERROR1 ) ;
      Sys_SetWindowText ( hwndError, Message ) ;
      Sys_BeepError ( ) ;
      Sys_SetFocus ( hwndField ) ;
      return ( FALSE ) ;
   } /* endif */

  /**************************************************************************
   * Check for value too high.  You can indent forwards to the right margin.*
   **************************************************************************/

   if ( Value*1000 > double ( FormInfo.cx - RightMargin - LeftMargin ) ) {
      ResourceString Message ( LibraryHandle, IDS_INDENT_ERROR2 ) ;
      Sys_SetWindowText ( hwndError, Message ) ;
      Sys_BeepError ( ) ;
      Sys_SetFocus ( hwndField ) ;
      return ( FALSE ) ;
   } /* endif */

  /**************************************************************************
   * Save the value and return no error.                                    *
   **************************************************************************/

   Indent = int ( Value * 1000 + ( Value >= 0.0 ? 0.4 : -0.4 ) ) ;

   return ( TRUE ) ;
}

