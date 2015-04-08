// Class HELPWIN: Encapsulates the startup/shutdown logic for an OS/2-PM help window.

#ifndef HELPWIN_H
#define HELPWIN_H

#include "System.h"

class HelpWindow {
   private:
      HAB Anchor ;
      HWND Handle ;
 
   public:
      HelpWindow ( HAB Anchor, HMODULE Module, USHORT Id, PSZ Filename, PSZ Title ) ;
      ~HelpWindow ( ) ;
      BOOL Associate ( HWND Window ) ;
      HWND QueryHandle () { return ( Handle ) ; }
} ;

#endif
