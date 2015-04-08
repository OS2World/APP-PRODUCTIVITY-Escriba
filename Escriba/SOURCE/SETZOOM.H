/****************************************************************** SETZOOM.H
 *                                                                          *
 *                        Dialog: Set Zoom Factor                           *
 *                                                                          *
 ****************************************************************************/

#ifndef SETZOOM_H
#define SETZOOM_H

#include "System.h"

typedef struct {
   HMODULE Library ;           // -> Resource Library
   HWND    Window ;            // -> Document Window
   USHORT  ZoomFactor ;        // Current zoom factor.
} SETZOOM_PARMS, *PSETZOOM_PARMS ;

extern WINDOWPROC SetZoomProcessor ;

#endif
