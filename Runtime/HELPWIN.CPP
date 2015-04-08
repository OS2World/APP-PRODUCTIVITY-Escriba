// Class HELPWIN: Encapsulates the startup/shutdown logic for an OS/2-PM help window.

#include "System.h"

#include <string.h>

#include "Debug.h"
#include "HelpWin.h"


  // Constructor

HelpWindow::HelpWindow ( HAB anchor, HMODULE Module, USHORT Id, PSZ Filename, PSZ Title ) : Anchor(anchor) {

   HELPINIT HelpInit ;
   memset ( &HelpInit, 0, sizeof(HelpInit) ) ;

   HelpInit.cb = sizeof(HelpInit) ;
   HelpInit.phtHelpTable = (PHELPTABLE)MAKEULONG(Id,0xFFFF);
   HelpInit.hmodHelpTableModule = Module ;
   HelpInit.pszHelpWindowTitle = Title ;
   HelpInit.fShowPanelId = CMIC_HIDE_PANEL_ID ;
   HelpInit.pszHelpLibraryName = Filename ;

   Handle = WinCreateHelpInstance ( Anchor, &HelpInit ) ;

   if ( Handle == 0 ) {
      ERRORID Error = WinGetLastError ( Anchor ) ;
      if ( ( Error & 0xFFFF ) != PMERR_NO_MSG_QUEUE ) {
         char Message [512] ;
         Log ( "ERROR: Unable to create help instance from file '%s'.  %s", Filename, InterpretWinError(Anchor,Message) ) ;
      } /* endif */
   } /* endif */

} /* endmethod */


  // Destructor

HelpWindow::~HelpWindow ( ) {

   if ( Handle ) 
      WinDestroyHelpInstance ( Handle ) ;

} /* endmethod */


  // Associate with a window.

BOOL HelpWindow::Associate ( HWND Window ) {

   if ( !WinIsWindow ( Anchor, Handle ) ) {
      Log ( "WARNING: Attempt to associate window %08p with invalid help instance %08p.", Window, Handle ) ;
      return ( FALSE ) ;
   } /* endif */

   if ( !WinIsWindow ( Anchor, Window ) ) {
      Log ( "WARNING: Attempt to associate help instance with invalid window %08p.", Window ) ;
      return ( FALSE ) ;
   } /* endif */

   if ( !WinAssociateHelpInstance ( Handle, Window ) ) {
      char Message [512] ;
      Log ( "ERROR: Unable to associate help instance %08p with window %08p.  %s", Handle, Window, InterpretWinError(0,Message) ) ;
      return ( FALSE ) ;
   } /* endif */

   return ( TRUE ) ;

} /* endmethod */

