/******************************************************************* VRULER.H
 *                                                                          *
 *                         Vertical Ruler Class                             *
 *                                                                          *
 ****************************************************************************/

#ifndef VRULER_H
#define VRULER_H

#include "System.h"

#include "Profile2.h"

#define VERTRULER_CLASS_NAME "VertRuler"

typedef struct {
   INIDATA *IniData ;
   HWND     MainWindow ;
} VERTRULER_PARMS, *PVERTRULER_PARMS ;

typedef struct {
   BOOL  Metric ;
   FIXED fxZoom ;
   LONG  PageHeight ;
   LONG  WindowHeight ;
   LONG  TopLeft ;
   LONG  BottomMargin ;
   LONG  TopMargin ;
} VERTRULER_SETPARMS, *PVERTRULER_SETPARMS ;

extern WINDOWPROC VertRulerMessageProcessor ;

#endif
