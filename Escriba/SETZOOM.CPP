/**************************************************************** SETZOOM.CPP
 *                                                                          *
 *                        Dialog: Set Zoom Factor                           *
 *                                                                          *
 ****************************************************************************/

#include "System.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Debug.h"
#include "Dialog2.h"
#include "Escriba.h"
#include "ReString.h"
#include "SetZoom.h"
#include "Support.h"

// #define DEBUG
// #define DEBUG_MESSAGES
 
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
 *      Dialog Processor                                                    *
 *                                                                          *
 ****************************************************************************/

extern MRESULT EXPENTRY SetZoomProcessor ( HWND Window, MESG msg, MPARAM1 mp1, MPARAM2 mp2 ) {

  /**************************************************************************
   * Dispatch the message according to the method table and return the      *
   *   result.  Any messages not defined above get handled by the system    *
   *   default dialog processor.                                            *
   **************************************************************************/

   static METHOD Methods [] = {
      { WM_INITDLG, InitDlg },
      { WM_COMMAND, Command }
   } ;

   #ifdef DEBUG_MESSAGES
      static int Indent = 0 ;
      LogMsgRcvd ( Indent, "SETZOOM", Window, msg, mp1, mp2 ) ;
      Indent += 2 ;
   #endif

   MRESULT Result = DispatchMessage ( Window, msg, mp1, mp2, Methods, sizeof(Methods)/sizeof(Methods[0]), Dialog_Processor ) ;

   #ifdef DEBUG_MESSAGES
      Indent -= 2 ;
      LogMsgDone ( Indent, "SETZOOM", msg, Result ) ;
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

   PSETZOOM_PARMS Parms = (PSETZOOM_PARMS) ( PVOIDFROMMP ( mp2 ) ) ;

   Sys_SetWindowData ( Window, Parms ) ;

  /**************************************************************************
   * Set the limits and initial value of the spin-button control.           *
   **************************************************************************/

   Sys_SendMessage ( CHILD(Window,IDD_ZOOM_SET), SPBM_SETLIMITS, (MPARAM)3000L, (MPARAM)10L ) ;

   Sys_SendMessage ( CHILD(Window,IDD_ZOOM_SET), SPBM_SETCURRENTVALUE, (MPARAM)(Parms->ZoomFactor), NULL ) ;

  /**************************************************************************
   * Return OK.                                                             *
   **************************************************************************/

   return ( Dialog_Processor ( Window, msg, mp1, mp2 ) ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process command messages.                                           *
 *                                                                          *
 ****************************************************************************/

static MRESULT APIENTRY Command ( HWND Window, MESG, MPARAM1 mp1, MPARAM2 mp2 ) {

 /***************************************************************************
  * Local Declarations                                                      *
  ***************************************************************************/

  static METHOD Methods [] = {
    { DID_OK,         OK     },
    { DID_CANCEL,     Cancel },
    { IDD_ZOOM_APPLY, Apply  },
  } ;

 /***************************************************************************
  * Dispatch the message without a default message processor.               *
  ***************************************************************************/

  return ( DispatchMessage ( Window, SHORT1FROMMP(mp1), mp1, mp2, Methods,
    sizeof(Methods)/sizeof(Methods[0]), 0 ) ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process acceptance of new timer value.                              *
 *                                                                          *
 ****************************************************************************/

static MRESULT APIENTRY OK ( HWND Window, MESG, MPARAM1, MPARAM2 ) {

 /***************************************************************************
  * Find the instance data.                                                 *
  ***************************************************************************/

  PSETZOOM_PARMS Parms = PSETZOOM_PARMS ( Sys_GetWindowData ( Window ) ) ;

 /***************************************************************************
  * Get the new value.                                                      *
  ***************************************************************************/

  Sys_SendMessage ( CHILD(Window,IDD_ZOOM_SET), SPBM_QUERYVALUE,
    MPFROMP(&Parms->ZoomFactor), MPFROM2SHORT(0,SPBQ_UPDATEIFVALID) ) ;

 /***************************************************************************
  * Validate it.                                                            *
  ***************************************************************************/

  if ( Parms->ZoomFactor < 10 ) {
    ResourceString Message ( Parms->Library, IDS_ZOOM_ERROR1 ) ;
    Sys_SetWindowText ( CHILD(Window,IDD_ZOOM_ERR), PSZ(Message) ) ;
    Sys_BeepError ( ) ;
    Sys_SetFocus ( CHILD ( Window, IDD_ZOOM_SET ) ) ;
    return ( 0 ) ;
  }

  if ( Parms->ZoomFactor > 3000 ) {
    ResourceString Message ( Parms->Library, IDS_ZOOM_ERROR2 ) ;
    Sys_SetWindowText ( CHILD(Window,IDD_ZOOM_ERR), PSZ(Message) ) ;
    Sys_BeepError ( ) ;
    Sys_SetFocus ( CHILD ( Window, IDD_ZOOM_SET ) ) ;
    return ( 0 ) ;
  }

 /***************************************************************************
  * Dismiss the dialog with a TRUE status.                                  *
  ***************************************************************************/

  Sys_EndDialog ( Window, TRUE ) ;

  return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process cancellation.                                               *
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
 *      Apply the zoom factor.                                              *
 *                                                                          *
 ****************************************************************************/

static MRESULT APIENTRY Apply ( HWND Window, MESG, MPARAM1, MPARAM2 ) {

 /***************************************************************************
  * Find the instance data.                                                 *
  ***************************************************************************/

  PSETZOOM_PARMS Parms = PSETZOOM_PARMS ( Sys_GetWindowData ( Window ) ) ;

 /***************************************************************************
  * Get the new value.                                                      *
  ***************************************************************************/

  Sys_SendMessage ( CHILD(Window,IDD_ZOOM_SET), SPBM_QUERYVALUE,
    MPFROMP(&Parms->ZoomFactor), MPFROM2SHORT(0,SPBQ_UPDATEIFVALID) ) ;

 /***************************************************************************
  * Validate it.                                                            *
  ***************************************************************************/

  if ( Parms->ZoomFactor < 10 ) {
    ResourceString Message ( Parms->Library, IDS_ZOOM_ERROR1 ) ;
    Sys_SetWindowText ( CHILD(Window,IDD_ZOOM_ERR), PSZ(Message) ) ;
    Sys_BeepError ( ) ;
    Sys_SetFocus ( CHILD ( Window, IDD_ZOOM_SET ) ) ;
    return ( 0 ) ;
  }

  if ( Parms->ZoomFactor > 3000 ) {
    ResourceString Message ( Parms->Library, IDS_ZOOM_ERROR2 ) ;
    Sys_SetWindowText ( CHILD(Window,IDD_ZOOM_ERR), PSZ(Message) ) ;
    Sys_BeepError ( ) ;
    Sys_SetFocus ( CHILD ( Window, IDD_ZOOM_SET ) ) ;
    return ( 0 ) ;
  }

 /***************************************************************************
  * Set the current document's zoom factor.                                 *
  ***************************************************************************/

  Sys_SendMessage ( Parms->Window, WM_SET_ZOOM, MPFROMSHORT(Parms->ZoomFactor), 0 ) ;

  return ( 0 ) ;
}
