/****************************************************************** FRAME.CPP
 *                                                                          *
 *                      Main Window Frame (Subclassed)                      *
 *                                                                          *
 ****************************************************************************/

#include "System.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Frame.h"
#include "Support.h"

 
/****************************************************************************
 *                                                                          *
 *      Window Message Processor                                            *
 *                                                                          *
 ****************************************************************************/

extern MRESULT EXPENTRY FrameMessageProcessor ( HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2 ) {

   // Get parent class information.
   CLASSINFO ClassInfo ;
   WinQueryClassInfo ( 0, WC_FRAME, &ClassInfo ) ;

   // Modify the frame behavior on certain messages.
   switch ( msg ) {

      case WM_QUERYTRACKINFO: {         // Don't allow title-bar dragging.
         return ( FALSE ) ; }

      case WM_INITMENU: {               // Disable most system menu options.
         ClassInfo.pfnWindowProc ( hwnd, msg, mp1, mp2 ) ;
         HWND hwndSysMenu = CHILD ( hwnd, FID_SYSMENU ) ;
         USHORT idSysMenu = SHORT1FROMMR ( Sys_SendMessage ( hwndSysMenu, MM_ITEMIDFROMPOSITION, NULL, NULL ) ) ;
         if ( USHORT(SHORT1FROMMP(mp1)) == idSysMenu ) {
            EnableMenuItem ( hwnd, FID_SYSMENU, SC_RESTORE,     FALSE ) ;
            EnableMenuItem ( hwnd, FID_SYSMENU, SC_MOVE,        FALSE ) ;
            EnableMenuItem ( hwnd, FID_SYSMENU, SC_SIZE,        FALSE ) ;
            EnableMenuItem ( hwnd, FID_SYSMENU, SC_MINIMIZE,    FALSE ) ;
            EnableMenuItem ( hwnd, FID_SYSMENU, SC_MAXIMIZE,    FALSE ) ;
            EnableMenuItem ( hwnd, FID_SYSMENU, SC_HIDE,        FALSE ) ;
            EnableMenuItem ( hwnd, FID_SYSMENU, SC_CLOSE,       TRUE ) ;
            EnableMenuItem ( hwnd, FID_SYSMENU, SC_TASKMANAGER, FALSE ) ;
         } /* endif */
         return ( FALSE ) ; }

      case DM_DRAGOVER:
      case DM_DRAGLEAVE:
         return ( 0 ) ;

   } /* endswitch */

   // Otherwise, simply call the original frame message processor.
   return ( ClassInfo.pfnWindowProc ( hwnd, msg, mp1, mp2 ) ) ;
}
