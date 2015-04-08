/***************************************************************** SEARCH.CPP
 *                                                                          *
 *                            Text Search Dialog                            *
 *                                                                          *
 ****************************************************************************/

#include "System.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Dialog2.h"
#include "Escriba.h"
#include "GClass.h"
#include "ReString.h"
#include "Search.h"
#include "Support.h"

//#define DEBUG

 
/****************************************************************************
 *                                                                          *
 *                     Definitions & Declarations                           *
 *                                                                          *
 ****************************************************************************/

static WINDOWPROC InitDlg ;
static WINDOWPROC Control ;
static WINDOWPROC Command ;
static WINDOWPROC Find ;
static WINDOWPROC Change ;
static WINDOWPROC ChangeFind ;
static WINDOWPROC ChangeAll ;
static WINDOWPROC Cancel ;

static void RepositionDialog ( HWND Window ) ;

 
/****************************************************************************
 *                                                                          *
 *      "Search" Dialog Processor                                           *
 *                                                                          *
 ****************************************************************************/

extern MRESULT EXPENTRY Search_Processor ( HWND Window, MESG msg, MPARAM1 mp1, MPARAM2 mp2 ) {

  /**************************************************************************
   * Dispatch the message according to the method table and return the      *
   *   result.  Any messages not defined above get handled by the system    *
   *   default dialog processor.                                            *
   **************************************************************************/

   static METHOD Methods [] = {
      { WM_INITDLG, InitDlg },
      { WM_CONTROL, Control },
      { WM_COMMAND, Command }
   } ;

   #ifdef DEBUG
      static int Indent = 0 ;
      Log ( "%*sSEARCH: Message %08X received.  Parm1=%08X, Parm2=%08X.", Indent, "", msg, mp1, mp2 ) ;
      Indent += 2 ;
   #endif

   MRESULT Result = DispatchMessage ( Window, msg, mp1, mp2, Methods, sizeof(Methods)/sizeof(Methods[0]), Dialog_Processor ) ;

   #ifdef DEBUG
      Indent -= 2 ;
      Log ( "%*sSEARCH: Message %08X done.  Result %08X.", Indent, "", msg, Result ) ;
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
   * Get the dialog parameters.                                             *
   **************************************************************************/

   PSEARCH_PARMS Parms = PSEARCH_PARMS ( ( PVOIDFROMMP ( mp2 ) ) ) ;

   Sys_SetWindowData ( Window, Parms ) ;

  /**************************************************************************
   * Set flag for first search since dialog invocation.                     *
   **************************************************************************/

   Parms->FirstTime = TRUE ;

  /**************************************************************************
   * Initialize the entry fields.                                           *
   **************************************************************************/

   Sys_SetWindowText ( CHILD(Window,IDD_SEARCH_STRING), Parms->IniData->Search_String ) ;
   Sys_SendMessage ( CHILD(Window,IDD_SEARCH_STRING), EM_SETTEXTLIMIT, MPFROMSHORT(sizeof(Parms->IniData->Search_String)-1), 0 ) ;

   Sys_SetWindowText ( CHILD(Window,IDD_SEARCH_REPLACE), Parms->IniData->Search_Replace ) ;
   Sys_SendMessage ( CHILD(Window,IDD_SEARCH_REPLACE), EM_SETTEXTLIMIT, MPFROMSHORT(sizeof(Parms->IniData->Search_Replace)-1), 0 ) ;

   Sys_SendMessage ( CHILD(Window,IDD_SEARCH_CASESENSITIVE), BM_SETCHECK, MPFROMSHORT(Parms->IniData->Search_CaseSensitive), 0 ) ;
   Sys_SendMessage ( CHILD(Window,IDD_SEARCH_REGULAR), BM_SETCHECK, MPFROMSHORT(Parms->IniData->Search_Regular), 0 ) ;

   Sys_EnableWindow ( CHILD(Window,IDD_SEARCH_FIND), Parms->IniData->Search_String[0] ) ;
   Sys_EnableWindow ( CHILD(Window,IDD_SEARCH_CHANGE), Parms->IniData->Search_String[0] ) ;
   Sys_EnableWindow ( CHILD(Window,IDD_SEARCH_CHANGEFIND), Parms->IniData->Search_String[0] ) ;
   Sys_EnableWindow ( CHILD(Window,IDD_SEARCH_CHANGEALL), Parms->IniData->Search_String[0] ) ;

  /**************************************************************************
   * Perform normal default actions.                                        *
   **************************************************************************/

   Dialog_Processor ( Window, msg, mp1, mp2 ) ;

  /**************************************************************************
   * Position the dialog to the upper right.                                *
   **************************************************************************/

   RECTL OwnerRectangle, DialogRectangle ;
   WinQueryWindowRect ( Parms->Owner, &OwnerRectangle ) ;
   WinQueryWindowRect ( Window, &DialogRectangle ) ;
   POINTL Position ;
   Position.x = OwnerRectangle.xRight - ( DialogRectangle.xRight - DialogRectangle.xLeft ) - 10 ;
   Position.y = OwnerRectangle.yTop - ( DialogRectangle.yTop - DialogRectangle.yBottom ) - 10 ;
   WinMapWindowPoints ( Parms->Owner, HWND_DESKTOP, &Position, 1 ) ;
   WinSetWindowPos ( Window, 0, Position.x, Position.y, 0, 0, SWP_MOVE ) ;

  /**************************************************************************
   * Return all's well.  No focus change.                                   *
   **************************************************************************/

   return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process Control Notifications                                       *
 *                                                                          *
 ****************************************************************************/

static MRESULT APIENTRY Control ( HWND Window, MESG, MPARAM1 mp1, MPARAM2 ) {

  /**************************************************************************
   * Find the instance data.                                                *
   **************************************************************************/

   PSEARCH_PARMS Parms = PSEARCH_PARMS ( Sys_GetWindowData ( Window ) ) ;

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

     case IDD_SEARCH_STRING: {
        if ( Message == EN_CHANGE ) {
           Sys_GetWindowText ( CHILD(Window,IDD_SEARCH_STRING), Parms->IniData->Search_String, sizeof(Parms->IniData->Search_String) ) ;
           Sys_EnableWindow ( CHILD(Window,IDD_SEARCH_FIND), Parms->IniData->Search_String[0] ) ;
           Sys_EnableWindow ( CHILD(Window,IDD_SEARCH_CHANGE), Parms->IniData->Search_String[0] ) ;
           Sys_EnableWindow ( CHILD(Window,IDD_SEARCH_CHANGEFIND), Parms->IniData->Search_String[0] ) ;
           Sys_EnableWindow ( CHILD(Window,IDD_SEARCH_CHANGEALL), Parms->IniData->Search_String[0] ) ;
        } /* endif */
        return ( 0 ) ; }

   } /* endswitch */

  /**************************************************************************
   * Return no error.                                                       *
   **************************************************************************/

   return ( MRFROMSHORT ( FALSE ) ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process commands received by the Dialog                             *
 *                                                                          *
 ****************************************************************************/

static MRESULT APIENTRY Command ( HWND Window, MESG, MPARAM1 mp1, MPARAM2 mp2 ) {

  /**************************************************************************
   * Dispatch the message without a default message processor.              *
   **************************************************************************/

   static METHOD Methods [] = {
      { IDD_SEARCH_FIND,       Find       },
      { IDD_SEARCH_CHANGE,     Change     },
      { IDD_SEARCH_CHANGEFIND, ChangeFind },
      { IDD_SEARCH_CHANGEALL,  ChangeAll  },
      { DID_CANCEL,            Cancel     },
   } ;

   return ( DispatchMessage ( Window, SHORT1FROMMP(mp1), mp1, mp2, Methods, sizeof(Methods)/sizeof(Methods[0]), 0 ) ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process the Dialog's Find button being pressed.                     *
 *                                                                          *
 ****************************************************************************/

static MRESULT APIENTRY Find ( HWND Window, MESG, MPARAM1, MPARAM2 ) {

  /**************************************************************************
   * Find the instance data.                                                *
   **************************************************************************/

   PSEARCH_PARMS Parms = PSEARCH_PARMS ( Sys_GetWindowData ( Window ) ) ;

  /**************************************************************************
   * Validate and store the entered data.                                   *
   **************************************************************************/

   Sys_GetWindowText ( CHILD(Window,IDD_SEARCH_STRING), Parms->IniData->Search_String, sizeof(Parms->IniData->Search_String) ) ;
   if ( Parms->IniData->Search_String[0] == 0 ) {
      ResourceString Message ( Parms->Library, IDS_SEARCH_ERROR1 ) ;
      Sys_SetWindowText ( CHILD(Window,IDD_SEARCH_ERR), PSZ(Message) ) ;
      Sys_BeepError ( ) ;
      Sys_SetFocus ( CHILD ( Window, IDD_SEARCH_STRING ) ) ;
      return ( 0 ) ;
   } /* endif */

   Sys_GetWindowText ( CHILD(Window,IDD_SEARCH_REPLACE), Parms->IniData->Search_Replace, sizeof(Parms->IniData->Search_Replace) ) ;

   Parms->IniData->Search_CaseSensitive = BOOL ( SHORT1FROMMR ( Sys_SendMessage ( CHILD(Window,IDD_SEARCH_CASESENSITIVE), BM_QUERYCHECK, 0, 0 ) ) ) ;

   Parms->IniData->Search_Regular = BOOL ( SHORT1FROMMR ( Sys_SendMessage ( CHILD(Window,IDD_SEARCH_REGULAR), BM_QUERYCHECK, 0, 0 ) ) ) ;

   if ( Parms->IniData->Search_Regular ) {
      cGREP2 Grepper ( Parms->IniData->Search_String, Parms->pText ) ;
      if ( Grepper.error_string[0] ) {
         ResourceString Format ( Parms->Library, IDS_SEARCH_ERROR4 ) ;
         char Message [300] ;
         sprintf ( Message, PCHAR(Format), Grepper.error_string ) ;
         Sys_SetWindowText ( CHILD(Window,IDD_SEARCH_ERR), Message ) ;
         Sys_BeepError ( ) ;
         Sys_SetFocus ( CHILD ( Window, IDD_SEARCH_STRING ) ) ;
         return ( 0 ) ;
      } /* endif */
   } /* endif */

  /**************************************************************************
   * Request the Find action be performed.                                  *
   **************************************************************************/

   BOOL Found = BOOL ( LONGFROMMR ( Sys_SendMessage ( Parms->Owner, WM_COMMAND, MPFROMSHORT(Parms->FirstTime?IDM_FIND:IDM_FINDNEXT), MPFROM2SHORT(CMDSRC_OTHER,FALSE) ) ) ) ;
   if ( NOT Found ) {
      ResourceString Message ( Parms->Library, IDS_SEARCH_ERROR3 ) ;
      Sys_SetWindowText ( CHILD(Window,IDD_SEARCH_ERR), PSZ(Message) ) ;
      Sys_BeepError ( ) ;
      Sys_SetFocus ( CHILD ( Window, IDD_SEARCH_STRING ) ) ;
      return ( 0 ) ;
   } /* endif */

   Parms->FirstTime = FALSE ;

  /**************************************************************************
   * Reposition the dialog if necessary.                                    *
   **************************************************************************/

   RepositionDialog ( Window ) ;

  /**************************************************************************
   * All's well.                                                            *
   **************************************************************************/

   return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process the Dialog's Change button being pressed.                   *
 *                                                                          *
 ****************************************************************************/

static MRESULT APIENTRY Change ( HWND Window, MESG, MPARAM1, MPARAM2 ) {

  /**************************************************************************
   * Find the instance data.                                                *
   **************************************************************************/

   PSEARCH_PARMS Parms = PSEARCH_PARMS ( Sys_GetWindowData ( Window ) ) ;

  /**************************************************************************
   * Validate and store the entered data.                                   *
   **************************************************************************/

   Sys_GetWindowText ( CHILD(Window,IDD_SEARCH_STRING), Parms->IniData->Search_String, sizeof(Parms->IniData->Search_String) ) ;
   if ( Parms->IniData->Search_String[0] == 0 ) {
      ResourceString Message ( Parms->Library, IDS_SEARCH_ERROR1 ) ;
      Sys_SetWindowText ( CHILD(Window,IDD_SEARCH_ERR), PSZ(Message) ) ;
      Sys_BeepError ( ) ;
      Sys_SetFocus ( CHILD ( Window, IDD_SEARCH_STRING ) ) ;
      return ( 0 ) ;
   } /* endif */

   Sys_GetWindowText ( CHILD(Window,IDD_SEARCH_REPLACE), Parms->IniData->Search_Replace, sizeof(Parms->IniData->Search_Replace) ) ;
   if ( Parms->IniData->Search_Replace[0] == 0 ) {
      ResourceString Title1 ( Parms->Library, IDS_TITLE1 ) ;
      ResourceString Message ( Parms->Library, IDS_SEARCH_ERROR2 ) ;
      APIRET Result = MessageBox ( HWND_DESKTOP, Window, PSZ(Message), PSZ(Title1), 
         0, MB_YESNOCANCEL | MB_ICONQUESTION | MB_HELP, Parms->Library, IDS_MESSAGE_BOX_STRINGS ) ;
      if ( Result == MBID_CANCEL )
         return ( 0 ) ;
      if ( Result == MBID_NO ) {
         Sys_SetFocus ( CHILD ( Window, IDD_SEARCH_REPLACE ) ) ;
         return ( 0 ) ;
      } /* endif */
   } /* endif */

   Parms->IniData->Search_CaseSensitive = BOOL ( SHORT1FROMMR ( Sys_SendMessage ( CHILD(Window,IDD_SEARCH_CASESENSITIVE), BM_QUERYCHECK, 0, 0 ) ) ) ;

   Parms->IniData->Search_Regular = BOOL ( SHORT1FROMMR ( Sys_SendMessage ( CHILD(Window,IDD_SEARCH_REGULAR), BM_QUERYCHECK, 0, 0 ) ) ) ;

  /**************************************************************************
   * Request the Change action be performed.                                *
   **************************************************************************/

   Sys_SendMessage ( Parms->Owner, WM_COMMAND, MPFROMSHORT(IDM_CHANGE), MPFROM2SHORT(CMDSRC_OTHER,FALSE) ) ;

  /**************************************************************************
   * Reposition the dialog if necessary.                                    *
   **************************************************************************/

   RepositionDialog ( Window ) ;

   return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process the Dialog's Change & Find button being pressed.            *
 *                                                                          *
 ****************************************************************************/

static MRESULT APIENTRY ChangeFind ( HWND Window, MESG, MPARAM1, MPARAM2 ) {

  /**************************************************************************
   * Find the instance data.                                                *
   **************************************************************************/

   PSEARCH_PARMS Parms = PSEARCH_PARMS ( Sys_GetWindowData ( Window ) ) ;

  /**************************************************************************
   * Validate and store the entered data.                                   *
   **************************************************************************/

   Sys_GetWindowText ( CHILD(Window,IDD_SEARCH_STRING), Parms->IniData->Search_String, sizeof(Parms->IniData->Search_String) ) ;
   if ( Parms->IniData->Search_String[0] == 0 ) {
      ResourceString Message ( Parms->Library, IDS_SEARCH_ERROR1 ) ;
      Sys_SetWindowText ( CHILD(Window,IDD_SEARCH_ERR), PSZ(Message) ) ;
      Sys_BeepError ( ) ;
      Sys_SetFocus ( CHILD ( Window, IDD_SEARCH_STRING ) ) ;
      return ( 0 ) ;
   } /* endif */

   Sys_GetWindowText ( CHILD(Window,IDD_SEARCH_REPLACE), Parms->IniData->Search_Replace, sizeof(Parms->IniData->Search_Replace) ) ;
   if ( Parms->IniData->Search_Replace[0] == 0 ) {
      ResourceString Title1 ( Parms->Library, IDS_TITLE1 ) ;
      ResourceString Message ( Parms->Library, IDS_SEARCH_ERROR2 ) ;
      APIRET Result = MessageBox ( HWND_DESKTOP, Window, PSZ(Message), PSZ(Title1), 
         0, MB_YESNOCANCEL | MB_ICONQUESTION | MB_HELP, Parms->Library, IDS_MESSAGE_BOX_STRINGS ) ;
      if ( Result == MBID_CANCEL )
         return ( 0 ) ;
      if ( Result == MBID_NO ) {
         Sys_SetFocus ( CHILD ( Window, IDD_SEARCH_REPLACE ) ) ;
         return ( 0 ) ;
      } /* endif */
   } /* endif */

   Parms->IniData->Search_CaseSensitive = BOOL ( SHORT1FROMMR ( Sys_SendMessage ( CHILD(Window,IDD_SEARCH_CASESENSITIVE), BM_QUERYCHECK, 0, 0 ) ) ) ;

   Parms->IniData->Search_Regular = BOOL ( SHORT1FROMMR ( Sys_SendMessage ( CHILD(Window,IDD_SEARCH_REGULAR), BM_QUERYCHECK, 0, 0 ) ) ) ;

  /**************************************************************************
   * Request the Change & Find action be performed.                         *
   **************************************************************************/

   Sys_SendMessage ( Parms->Owner, WM_COMMAND, MPFROMSHORT(IDM_CHANGEFIND), MPFROM2SHORT(CMDSRC_OTHER,FALSE) ) ;

  /**************************************************************************
   * Reposition the dialog if necessary.                                    *
   **************************************************************************/

   RepositionDialog ( Window ) ;

   return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process the Dialog's Change All button being pressed.               *
 *                                                                          *
 ****************************************************************************/

static MRESULT APIENTRY ChangeAll ( HWND Window, MESG, MPARAM1, MPARAM2 ) {

  /**************************************************************************
   * Find the instance data.                                                *
   **************************************************************************/

   PSEARCH_PARMS Parms = PSEARCH_PARMS ( Sys_GetWindowData ( Window ) ) ;

  /**************************************************************************
   * Validate and store the entered data.                                   *
   **************************************************************************/

   Sys_GetWindowText ( CHILD(Window,IDD_SEARCH_STRING), Parms->IniData->Search_String, sizeof(Parms->IniData->Search_String) ) ;
   if ( Parms->IniData->Search_String[0] == 0 ) {
      ResourceString Message ( Parms->Library, IDS_SEARCH_ERROR1 ) ;
      Sys_SetWindowText ( CHILD(Window,IDD_SEARCH_ERR), PSZ(Message) ) ;
      Sys_BeepError ( ) ;
      Sys_SetFocus ( CHILD ( Window, IDD_SEARCH_STRING ) ) ;
      return ( 0 ) ;
   } /* endif */

   Sys_GetWindowText ( CHILD(Window,IDD_SEARCH_REPLACE), Parms->IniData->Search_Replace, sizeof(Parms->IniData->Search_Replace) ) ;
   if ( Parms->IniData->Search_Replace[0] == 0 ) {
      ResourceString Title1 ( Parms->Library, IDS_TITLE1 ) ;
      ResourceString Message ( Parms->Library, IDS_SEARCH_ERROR2 ) ;
      APIRET Result = MessageBox ( HWND_DESKTOP, Window, PSZ(Message), PSZ(Title1), 
         0, MB_YESNOCANCEL | MB_ICONQUESTION | MB_HELP, Parms->Library, IDS_MESSAGE_BOX_STRINGS ) ;
      if ( Result == MBID_CANCEL )
         return ( 0 ) ;
      if ( Result == MBID_NO ) {
         Sys_SetFocus ( CHILD ( Window, IDD_SEARCH_REPLACE ) ) ;
         return ( 0 ) ;
      } /* endif */
   } /* endif */

   Parms->IniData->Search_CaseSensitive = BOOL ( SHORT1FROMMR ( Sys_SendMessage ( CHILD(Window,IDD_SEARCH_CASESENSITIVE), BM_QUERYCHECK, 0, 0 ) ) ) ;

   Parms->IniData->Search_Regular = BOOL ( SHORT1FROMMR ( Sys_SendMessage ( CHILD(Window,IDD_SEARCH_REGULAR), BM_QUERYCHECK, 0, 0 ) ) ) ;

  /**************************************************************************
   * Request the Change All action be performed.                            *
   **************************************************************************/

   Sys_SendMessage ( Parms->Owner, WM_COMMAND, MPFROMSHORT(IDM_CHANGEALL), MPFROM2SHORT(CMDSRC_OTHER,FALSE) ) ;

  /**************************************************************************
   * Reposition the dialog if necessary.                                    *
   **************************************************************************/

   RepositionDialog ( Window ) ;

   return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process the Dialog's being cancelled.                               *
 *                                                                          *
 ****************************************************************************/

static MRESULT APIENTRY Cancel ( HWND Window, MESG, MPARAM1, MPARAM2 ) {

  /**************************************************************************
   * Dismiss the dialog with a FALSE status.                                *
   **************************************************************************/

   Sys_EndDialog ( Window, FALSE ) ;

   return ( 0 ) ;
}


 
/****************************************************************************
 *                                                                          *
 *      Reposition the Dialog if necessary to uncover selected text.        *
 *                                                                          *
 ****************************************************************************/

static void RepositionDialog ( HWND Window ) {

   PSEARCH_PARMS Parms = PSEARCH_PARMS ( Sys_GetWindowData ( Window ) ) ;

   RECTL Text ;
   Sys_SendMessage ( Parms->Owner, WM_QUERY_TEXT_RECTANGLE, MPFROMP(&Text), 0 ) ;
   WinMapWindowPoints ( Parms->Owner, HWND_DESKTOP, PPOINTL(&Text), 2 ) ;

   RECTL Desktop ;
   WinQueryWindowRect ( HWND_DESKTOP, &Desktop ) ;

   RECTL Dialog ;
   WinQueryWindowRect ( Window, &Dialog ) ;
   WinMapWindowPoints ( Window, HWND_DESKTOP, PPOINTL(&Dialog), 2 ) ;

   // If dialog covers the highlighted text at any point . . .
   if (  ( Dialog.xLeft   <= Text.xRight )
      && ( Dialog.xRight  >= Text.xLeft )
      && ( Dialog.yBottom <= Text.yTop )
      && ( Dialog.yTop    >= Text.yBottom ) ) {

      // Set default position first.
      RECTL OwnerRectangle, DialogRectangle ;
      WinQueryWindowRect ( Parms->Owner, &OwnerRectangle ) ;
      WinQueryWindowRect ( Window, &DialogRectangle ) ;
      POINTL Position ;
      Position.x = OwnerRectangle.xRight - ( DialogRectangle.xRight - DialogRectangle.xLeft ) - 10 ;
      Position.y = OwnerRectangle.yTop - ( DialogRectangle.yTop - DialogRectangle.yBottom ) - 10 ;
      WinMapWindowPoints ( Parms->Owner, HWND_DESKTOP, &Position, 1 ) ;
      WinSetWindowPos ( Window, 0, Position.x, Position.y, 0, 0, SWP_MOVE ) ;

      // If there is room below, relocate the dialog there.
      if ( Text.yBottom - Desktop.yBottom > Dialog.yTop - Dialog.yBottom ) {
         Position.y = ( ( Text.yBottom - Desktop.yBottom ) - ( Dialog.yTop - Dialog.yBottom ) ) / 2 + Desktop.yBottom ;
         WinSetWindowPos ( Window, 0, Position.x, Position.y, 0, 0, SWP_MOVE ) ;

      // Else, if there is room above, relocate the dialog there.
      } else if ( Desktop.yTop - Text.yTop > Dialog.yTop - Dialog.yBottom ) {
         Position.y = ( ( Desktop.yTop - Text.yTop ) - ( Dialog.yTop - Dialog.yBottom ) ) / 2 + Text.yTop ;
         WinSetWindowPos ( Window, 0, Position.x, Position.y, 0, 0, SWP_MOVE ) ;

      // Else, if there is room to the right, relocate there.
      } else if ( Desktop.xRight - Text.xRight > Dialog.xRight - Dialog.xLeft ) {
         Position.x = ( ( Desktop.xRight - Text.xRight ) - ( Dialog.xRight - Dialog.xLeft ) ) / 2 + Text.xRight ;
         WinSetWindowPos ( Window, 0, Position.x, Position.y, 0, 0, SWP_MOVE ) ;

      // Else, if there is room to the left, relocate there.
      } else if ( Text.xLeft - Desktop.xLeft > Dialog.xRight - Dialog.xLeft ) {
         Position.x = ( ( Text.xLeft - Desktop.xLeft ) - ( Dialog.xRight - Dialog.xLeft ) ) / 2 + Desktop.xLeft ;
         WinSetWindowPos ( Window, 0, Position.x, Position.y, 0, 0, SWP_MOVE ) ;

      // Else, don't relocate at all.  Just suffer.
      } else {

      } /* endif */

   } /* endif */
}
