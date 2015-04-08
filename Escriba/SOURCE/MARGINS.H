/****************************************************************** MARGINS.H
 *                                                                          *
 *                      Margins Dialog definitions                          *
 *                                                                          *
 ****************************************************************************/

#ifndef MARGINS_H
#define MARGINS_H

#include "System.h"

#include "Document.h"
#include "State.h"

typedef struct {

   HMODULE Library ;           // -> Resource Library
   HWND    Window ;            // -> Document Window

   HWND    VertRuler ;         // -> Vertical Ruler Control
   HWND    HorzRuler ;         // -> Horizontal Ruler Control

   BOOL    Metric ;            // Ruler metric?
   FIXED   fxZoom ;            // Current zoom.
   SIZEL   PageSize ;          // Current page size.
   SIZEL   WindowSize ;        // Current window size.
   RECTL   Margins ;           // Current margins (top,bottom,left,right).
   POINTL  TopLeft ;           // Current scrolling position.
   int     TabCount ;          // Tab Count.
   int     Tabs [MAX_TABS] ;   // Tab List.

   DocumentParms DocParms ;    // Document Parameters (Printer name, form name, etc).

} MARGINS_PARMS, *PMARGINS_PARMS ;

extern WINDOWPROC MarginsProcessor ;

extern BOOL VerifyLeftMargin (
   HAB Anchor,
   HMODULE Library,
   DocumentParms &DocParms,
   HWND hwndField,
   HWND hwndError,
   PHCINFO pFormInfo,
   long &LeftMargin,
   long RightMargin
) ;

extern BOOL VerifyRightMargin (
   HAB Anchor,
   HMODULE Library,
   DocumentParms &DocParms,
   HWND hwndField,
   HWND hwndError,
   PHCINFO pFormInfo,
   long LeftMargin,
   long &RightMargin
) ;

extern BOOL VerifyTopMargin (
   HAB Anchor,
   HMODULE Library,
   DocumentParms &DocParms,
   HWND hwndField,
   HWND hwndError,
   PHCINFO pFormInfo,
   long BottomMargin,
   long &TopMargin
) ;

extern BOOL VerifyBottomMargin (
   HAB Anchor,
   HMODULE Library,
   DocumentParms &DocParms,
   HWND hwndField,
   HWND hwndError,
   PHCINFO pFormInfo,
   long &BottomMargin,
   long TopMargin
) ;

#endif
