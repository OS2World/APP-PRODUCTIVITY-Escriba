/***************************************************************** DIALOG.CPP
 *                                                                          *
 *                         Subclassed Basic Dialog                          *
 *                                                                          *
 ****************************************************************************/

#include "System.h"

#include "Debug.h"
#include "Dialog.h"

/***************************************************************************
 *                                                                         *
 *  Dialog: Constructor                                                    *
 *                                                                         *
 ***************************************************************************/

Dialog::Dialog ( HWND parent, HWND owner, HMODULE resource, int id ) : 
   Parent(parent), Owner(owner), Resource(resource), Id(id), 
   Handle ( 0 ), DebugMessages(FALSE), MessageIndent(0) {

} /* endmethod */

/***************************************************************************
 *                                                                         *
 *  Dialog: Destructor                                                     *
 *                                                                         *
 ***************************************************************************/

Dialog::~Dialog ( ) {

   if ( Handle ) WinDestroyWindow ( Handle ) ;

} /* endmethod */

/***************************************************************************
 *                                                                         *
 *  Dialog: Load Dialog.  Must be called after all constructors.           *
 *                                                                         *
 ***************************************************************************/

int Dialog::Load ( ) {
   Handle = WinLoadDlg ( Parent, Owner, DialogMessageProcessor, Resource, Id, this ) ;
   return ( Handle != 0 ) ;
} /* endmethod */

/***************************************************************************
 *                                                                         *
 *  Dialog: Process Dialog                                                 *
 *                                                                         *
 ***************************************************************************/

int Dialog::Process ( ) {

   if ( Handle ) 
      return ( int ( WinProcessDlg ( Handle ) ) ) ;

   return ( FALSE ) ;

} /* endmethod */

/***************************************************************************
 *                                                                         *
 *  Dialog: Dismiss Dialog                                                 *
 *                                                                         *
 ***************************************************************************/

void Dialog::Dismiss ( int Status ) {

   WinDismissDlg ( Handle, Status ) ;

} /* endmethod */

/***************************************************************************
 *                                                                         *
 *  Dialog: INITDLG message processor                                      *
 *                                                                         *
 ***************************************************************************/

MRESULT Dialog::InitDialog ( HWND hwnd ) {

   // Associate dialog with help instance of owner window.
   WinAssociateHelpInstance ( WinQueryHelpInstance(OWNER(hwnd)), hwnd ) ;

   // Center the dialog over its owner.
   RECTL OwnerRectangle, DialogRectangle, DesktopRectangle ;
   WinQueryWindowRect ( OWNER(hwnd), &OwnerRectangle ) ;
   WinQueryWindowRect ( hwnd, &DialogRectangle ) ;
   WinQueryWindowRect ( HWND_DESKTOP, &DesktopRectangle ) ;
   POINTL Position ;
   Position.x = ( ( OwnerRectangle.xRight - OwnerRectangle.xLeft ) - ( DialogRectangle.xRight - DialogRectangle.xLeft ) ) / 2 + OwnerRectangle.xLeft ;
   Position.y = ( ( OwnerRectangle.yTop - OwnerRectangle.yBottom ) - ( DialogRectangle.yTop - DialogRectangle.yBottom ) ) / 2 + OwnerRectangle.yBottom ;
   WinMapWindowPoints ( OWNER(hwnd), HWND_DESKTOP, &Position, 1 ) ;

   // Keep the dialog fully on the desktop, if possible.
   if ( Position.x+DialogRectangle.xRight-DialogRectangle.xLeft >= DesktopRectangle.xRight )
            Position.x = DesktopRectangle.xRight - ( DialogRectangle.xRight - DialogRectangle.xLeft ) ;
   if ( Position.y+DialogRectangle.yTop-DialogRectangle.yBottom >= DesktopRectangle.yTop )
            Position.y = DesktopRectangle.yTop - ( DialogRectangle.yTop - DialogRectangle.yBottom ) ;
   if ( Position.x < DesktopRectangle.xLeft ) 
            Position.x = DesktopRectangle.xLeft ;
   if ( Position.y < DesktopRectangle.yBottom ) 
            Position.y = DesktopRectangle.yBottom ;
   WinSetWindowPos ( hwnd, 0, Position.x, Position.y, 0, 0, SWP_MOVE ) ;

   return ( 0 ) ; 

} /* endmethod */

/***************************************************************************
 *                                                                         *
 *  Dialog: COMMAND message processor                                      *
 *                                                                         *
 ***************************************************************************/

MRESULT Dialog::Command ( MPARAM mp1, MPARAM ) {

   switch ( SHORT1FROMMP(mp1) ) {

      case DID_OK: {
         Dismiss ( TRUE ) ;
         break; }

      case DID_CANCEL: {
         Dismiss ( FALSE ) ;
         break; }

   } /* endswitch */

   return ( 0 ) ;

} /* endmethod */

/***************************************************************************
 *                                                                         *
 *  Dialog: Message Processor                                              *
 *                                                                         *
 ***************************************************************************/

MRESULT Dialog::MessageProcessor ( HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2 ) {

   if ( DebugMessages ) {
      LogMsgRcvd ( MessageIndent, "Dialog::MessageProcessor", msg, mp1, mp2 ) ;
      MessageIndent += 2 ;
   } /* endif */

   MRESULT Result ;

   switch ( msg ) {
      case WM_INITDLG:  Result = InitDialog ( hwnd ) ;                          break ;
      case WM_COMMAND:  Result = Command ( mp1, mp2 ) ;                         break ;
      default:          Result = WinDefDlgProc ( hwnd, msg, mp1, mp2 ) ;        break ;
   } /* endswitch */

   if ( DebugMessages ) {
      MessageIndent -= 2 ;
      LogMsgDone ( MessageIndent, "Dialog::MessageProcessor", msg, Result ) ;
   } /* endif */

   return ( Result ) ;
} /* endmethod */

/***************************************************************************
 *                                                                         *
 *  Dialog Message Processor                                               *
 *                                                                         *
 ***************************************************************************/

extern MRESULT APIENTRY DialogMessageProcessor ( HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2 ) {

   switch ( msg ) {

      case WM_INITDLG: {
         Dialog *This = (class Dialog *) PVOIDFROMMP ( mp2 ) ;
         WinSetWindowPtr ( hwnd, QWL_USER, This ) ;
         break ; }

   } /* endswitch */

   Dialog *This = (class Dialog *) WinQueryWindowPtr ( hwnd, QWL_USER ) ;
   return ( This ? This->MessageProcessor ( hwnd, msg, mp1, mp2 ) : WinDefDlgProc ( hwnd, msg, mp1, mp2 ) ) ;
} 

