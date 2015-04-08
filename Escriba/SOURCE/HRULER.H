/******************************************************************* HRULER.H
 *                                                                          *
 *                        Horizontal Ruler Class                            *
 *                                                                          *
 ****************************************************************************/

#ifndef HRULER_H
#define HRULER_H

#include "System.h"

#include "Profile2.h"
#include "State.h"

#define HORZRULER_CLASS_NAME "HorzRuler"

typedef struct {
   INIDATA *IniData ;
   HWND     MainWindow ;
} HORZRULER_PARMS, *PHORZRULER_PARMS ;

typedef struct {
   BOOL   Metric ;
   FIXED  fxZoom ;
   LONG   PageWidth ;
   LONG   TopLeft ;
   LONG   LeftMargin ;
   LONG   RightMargin ;
   int    TabCount ;
   int    Tabs [MAX_TABS] ;
} HORZRULER_SETPARMS, *PHORZRULER_SETPARMS ;

extern WINDOWPROC HorzRulerMessageProcessor ;

#endif
