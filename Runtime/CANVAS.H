/******************************************************************* Canvas.H
 *                                                                          *
 *                   Graphical Workspace (Canvas) Object                    *
 *                                                                          *
 ****************************************************************************/

#ifndef CANVAS_H
#define CANVAS_H

#include "Mutex.h"

enum WorkspaceType { WRKSPC_GETPS, WRKSPC_PAINT, WRKSPC_GPI } ;

class Canvas {

   private:
      char *Tag ;
      WorkspaceType Type ;
      HAB Anchor ;
      HPS Handle ;
      Mutex Lock ;

   public:
      // Constructors
      Canvas ( char *Tag, HWND Window ) ;

      // Destructor
      ~Canvas ( ) ;     

      // Method: Determine how many fonts are available to this PS.
      int QueryFontCount ( ) ;

      // Method: Obtain a list of the first 'Count' fonts available to this PS.
      int QueryFonts ( int Count, PFONTMETRICS pfm ) ;

} ;

#endif // End if not already included.
