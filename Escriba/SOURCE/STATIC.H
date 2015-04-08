/******************************************************************* STATIC.H
 *                                                                          *
 *                         Static Window Class                              *
 *                                                                          *
 ****************************************************************************/

#ifndef STATIC_H
#define STATIC_H

#include "System.h"

#include "Profile2.h"

#define STATIC_CLASS_NAME "Static"

typedef struct {
   INIDATA *IniData ;
   HWND     MainWindow ;
   int      CodePage ;
} STATIC_PARMS, *PSTATIC_PARMS ;

extern WINDOWPROC StaticMessageProcessor ;

#endif
