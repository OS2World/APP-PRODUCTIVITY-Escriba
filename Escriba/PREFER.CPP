/***************************************************************** PREFER.CPP
 *                                                                          *
 *                       General Preferences Dialog                         *
 *                                                                          *
 ****************************************************************************/

#include "System.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Debug.h"
#include "Dialog2.h"
#include "Escriba.h"
#include "Prefer.h"
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

 
/****************************************************************************
 *                                                                          *
 *      Dialog Processor                                                    *
 *                                                                          *
 ****************************************************************************/

extern MRESULT EXPENTRY PreferencesProcessor ( HWND Window, MESG msg, MPARAM1 mp1, MPARAM2 mp2 ) {

  /**************************************************************************
   * Dispatch the message according to the method table and return the      *
   *   result.  Any messages not defined above get handled by the system    *
   *   default dialog processor.                                            *
   **************************************************************************/
 
   static METHOD Methods [] = {
      { WM_INITDLG, InitDlg },
      { WM_COMMAND, Command }
   } ;

   #ifdef DEBUG
      static int Indent = 0 ;
      Log ( "%*sPREFER: Message %08X received.  Parm1=%08X, Parm2=%08X.", Indent, "", msg, mp1, mp2 ) ;
      Indent += 2 ;
   #endif

   MRESULT Result = DispatchMessage ( Window, msg, mp1, mp2, Methods, sizeof(Methods)/sizeof(Methods[0]), Dialog_Processor ) ;

   #ifdef DEBUG
      Indent -= 2 ;
      Log ( "%*sPREFER: Message %08X done.  Result %08X.", Indent, "", msg, Result ) ;
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
   * Get parameter block address.                                           *
   **************************************************************************/

   PPREFERENCES_PARMS Parms = PPREFERENCES_PARMS ( PVOIDFROMMP ( mp2 ) ) ;

   Sys_SetWindowData ( Window, Parms ) ;

  /**************************************************************************
   * Fill in the initial values.                                            *
   **************************************************************************/

   Sys_SendMessage ( CHILD(Window,IDD_PREFERENCES_ANIMATE), BM_SETCHECK, MPFROMSHORT(Parms->Animate), 0 ) ;

   Sys_SendMessage ( CHILD(Window,IDD_PREFERENCES_RULERS), BM_SETCHECK, MPFROMSHORT(Parms->Rulers), 0 ) ;

   Sys_SendMessage ( CHILD(Window,IDD_PREFERENCES_SCBARS), BM_SETCHECK, MPFROMSHORT(Parms->ScrollBars), 0 ) ;

   Sys_SendMessage ( CHILD(Window,IDD_PREFERENCES_STATUSLINE), BM_SETCHECK, MPFROMSHORT(Parms->StatusLine), 0 ) ;

   Sys_SendMessage ( CHILD(Window,IDD_PREFERENCES_FULLNAMES), BM_SETCHECK, MPFROMSHORT(Parms->FullNames), 0 ) ;

   Sys_SendMessage ( CHILD(Window,IDD_PREFERENCES_SPOOLPAGES), BM_SETCHECK, MPFROMSHORT(Parms->SpoolPages), 0 ) ;

   Sys_SendMessage ( CHILD(Window,IDD_PREFERENCES_PRINTREVERSE), BM_SETCHECK, MPFROMSHORT(Parms->PrintReverse), 0 ) ;

   Sys_SendMessage ( CHILD(Window,IDD_PREFERENCES_LOADMOSTRECENT), BM_SETCHECK, MPFROMSHORT(Parms->LoadMostRecent), 0 ) ;

   switch ( Parms->ZoomType ) {
      case IDM_ZOOM_FULLWIDTH:
         Sys_SendMessage ( CHILD(Window,IDD_PREFERENCES_ZOOM_FULLWIDTH), BM_SETCHECK, MPFROMSHORT(TRUE), 0 ) ;
         break ;
      case IDM_ZOOM_FULLHEIGHT:
         Sys_SendMessage ( CHILD(Window,IDD_PREFERENCES_ZOOM_FULLHEIGHT), BM_SETCHECK, MPFROMSHORT(TRUE), 0 ) ;
         break ;
      case IDM_ZOOM_MARGINS:
         Sys_SendMessage ( CHILD(Window,IDD_PREFERENCES_ZOOM_MARGINS), BM_SETCHECK, MPFROMSHORT(TRUE), 0 ) ;
         break ;
      case IDM_ZOOM_ALL:
         Sys_SendMessage ( CHILD(Window,IDD_PREFERENCES_ZOOM_ALL), BM_SETCHECK, MPFROMSHORT(TRUE), 0 ) ;
         break ;
      default:
         Sys_SendMessage ( CHILD(Window,IDD_PREFERENCES_ZOOM_100), BM_SETCHECK, MPFROMSHORT(TRUE), 0 ) ;
         break ;
   } /* endswitch */

  /**************************************************************************
   * Return no error.                                                       *
   **************************************************************************/

   return ( Dialog_Processor ( Window, msg, mp1, mp2 ) ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process commands received by the Dialog                             *
 *                                                                          *
 ****************************************************************************/

static MRESULT APIENTRY Command ( HWND Window, MESG, MPARAM1 mp1, MPARAM2 mp2 ) {

 /***************************************************************************
  * Dispatch the message without a default message processor.               *
  ***************************************************************************/

  static METHOD Methods [] =
  {
    { DID_OK,     OK     },
    { DID_CANCEL, Cancel },
  } ;

  return ( DispatchMessage ( Window, SHORT1FROMMP(mp1), mp1, mp2, Methods, sizeof(Methods)/sizeof(Methods[0]), 0 ) ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process the Dialog's OK button being pressed.                       *
 *                                                                          *
 ****************************************************************************/

static MRESULT APIENTRY OK ( HWND Window, MESG, MPARAM1, MPARAM2 ) {

 /***************************************************************************
  * Find the instance data.                                                 *
  ***************************************************************************/

  PPREFERENCES_PARMS Parms = PPREFERENCES_PARMS ( Sys_GetWindowData ( Window ) ) ;

 /***************************************************************************
  * Pull the new values.                                                    *
  ***************************************************************************/

  Parms->Animate = (BOOL) SHORT1FROMMR ( Sys_SendMessage ( CHILD(Window,IDD_PREFERENCES_ANIMATE), BM_QUERYCHECK, 0, 0 ) ) ;

  Parms->Rulers = (BOOL) SHORT1FROMMR ( Sys_SendMessage ( CHILD(Window,IDD_PREFERENCES_RULERS), BM_QUERYCHECK, 0, 0 ) ) ;

  Parms->ScrollBars = (BOOL) SHORT1FROMMR ( Sys_SendMessage ( CHILD(Window,IDD_PREFERENCES_SCBARS), BM_QUERYCHECK, 0, 0 ) ) ;

  Parms->StatusLine = (BOOL) SHORT1FROMMR ( Sys_SendMessage ( CHILD(Window,IDD_PREFERENCES_STATUSLINE), BM_QUERYCHECK, 0, 0 ) ) ;

  Parms->FullNames = (BOOL) SHORT1FROMMR ( Sys_SendMessage ( CHILD(Window,IDD_PREFERENCES_FULLNAMES), BM_QUERYCHECK, 0, 0 ) ) ;

  Parms->SpoolPages = (BOOL) SHORT1FROMMR ( Sys_SendMessage ( CHILD(Window,IDD_PREFERENCES_SPOOLPAGES), BM_QUERYCHECK, 0, 0 ) ) ;

  Parms->PrintReverse = (BOOL) SHORT1FROMMR ( Sys_SendMessage ( CHILD(Window,IDD_PREFERENCES_PRINTREVERSE), BM_QUERYCHECK, 0, 0 ) ) ;

  Parms->LoadMostRecent = (BOOL) SHORT1FROMMR ( Sys_SendMessage ( CHILD(Window,IDD_PREFERENCES_LOADMOSTRECENT), BM_QUERYCHECK, 0, 0 ) ) ;

  if ( SHORT1FROMMR ( Sys_SendMessage ( CHILD(Window,IDD_PREFERENCES_ZOOM_FULLWIDTH), BM_QUERYCHECK, 0, 0 ) ) ) {
     Parms->ZoomType = IDM_ZOOM_FULLWIDTH ;
  } else if ( SHORT1FROMMR ( Sys_SendMessage ( CHILD(Window,IDD_PREFERENCES_ZOOM_FULLHEIGHT), BM_QUERYCHECK, 0, 0 ) ) ) {
     Parms->ZoomType = IDM_ZOOM_FULLHEIGHT ;
  } else if ( SHORT1FROMMR ( Sys_SendMessage ( CHILD(Window,IDD_PREFERENCES_ZOOM_MARGINS), BM_QUERYCHECK, 0, 0 ) ) ) {
     Parms->ZoomType = IDM_ZOOM_MARGINS ;
  } else if ( SHORT1FROMMR ( Sys_SendMessage ( CHILD(Window,IDD_PREFERENCES_ZOOM_ALL), BM_QUERYCHECK, 0, 0 ) ) ) {
     Parms->ZoomType = IDM_ZOOM_ALL ;
  } else {
     Parms->ZoomType = IDM_ZOOM_100 ;
  } /* endif */

 /***************************************************************************
  * Dismiss the dialog with a TRUE status.                                  *
  ***************************************************************************/

  Sys_EndDialog ( Window, TRUE ) ;

  return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process the Defaults Dialog's being cancelled.                      *
 *                                                                          *
 ****************************************************************************/

static MRESULT APIENTRY Cancel ( HWND Window, MESG, MPARAM1, MPARAM2 ) {

 /***************************************************************************
  * Dismiss the dialog with a TRUE status.                                  *
  ***************************************************************************/

  Sys_EndDialog ( Window, FALSE ) ;

  return ( 0 ) ;
}
