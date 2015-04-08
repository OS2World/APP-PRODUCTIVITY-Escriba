/********************************************************************* TABS.H
 *                                                                          *
 *                        Tabs Dialog definitions                           *
 *                                                                          *
 ****************************************************************************/

#ifndef TABS_H
#define TABS_H

#include "System.h"

#include "State.h"

typedef struct {

   HMODULE Library ;
   HWND    Window ;

   HWND    RulerWindow ;

   BOOL    Metric ;
   FIXED   fxZoom ;
   LONG    TopLeft ;
   LONG    PageWidth ;
   LONG    WindowWidth ;
   LONG    LeftMargin ;
   LONG    RightMargin ;
   int     TabCount ;
   int     Tabs [MAX_TABS] ;

} TABS_PARMS, *PTABS_PARMS ;

extern WINDOWPROC TabsProcessor ;

#endif
