/******************************************************************* DIALOG.H
 *                                                                          *
 *                       Basic Dialog Definitions                           *
 *                                                                          *
 ****************************************************************************/

#ifndef DIALOG_H
#define DIALOG_H

#include "System.h"

extern WINDOWPROC DialogMessageProcessor ;

class Dialog {

   protected:
      HWND Parent ;
      HWND Owner ;
      HMODULE Resource ;
      int Id ;
      HWND Handle ;
      BOOL DebugMessages ;
      int MessageIndent ;

   public:

      Dialog ( HWND Parent, HWND Owner, HMODULE Resource, int Id ) ;
      virtual ~Dialog ( ) ;

      HWND QueryHandle() { return ( Handle ) ; }

      int Load ( ) ;
      int Process ( ) ;
      void Dismiss ( int Status ) ;

      virtual MRESULT InitDialog ( HWND hwnd ) ;
      virtual MRESULT Command ( MPARAM1 mp1, MPARAM2 mp2 ) ;
      virtual MRESULT MessageProcessor ( HWND hwnd, MESG msg, MPARAM1 mp1, MPARAM2 mp2 ) ;

} ;

#endif 
