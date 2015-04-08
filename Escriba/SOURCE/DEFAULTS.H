/***************************************************************** DEFAULTS.H
 *                                                                          *
 *                     New Documents Defaults Dialog                        *
 *                                                                          *
 ****************************************************************************/

#ifndef DEFAULTS_H
#define DEFAULTS_H

#include "System.h"

#include "Document.h"

typedef struct {
   HMODULE              Library ;       // Language library module handle.
   BOOL                 Global ;        // Flag: Apply changes globally?
   DocumentParms        DocParms ;      // Document parameters.
   HWND                 Window ;        // Document window handle (for APPLY).
   HCINFO               FormInfo ;      // Current form information (internal use only).
   HWND                 ErrorField ;    // Dialog error field handle (internal use only).
   HWND                 hwndUnits ;
   HWND                 hwndFormName ;
   HWND                 hwndFormSize ;
   HWND                 hwndLeftMargin ;
   HWND                 hwndRightMargin ;
   HWND                 hwndTopMargin ;
   HWND                 hwndBottomMargin ;
   HWND                 hwndIndent ;
   HWND                 hwndTabSpacing ;
} DEFAULTS_PARMS, *PDEFAULTS_PARMS ;

extern BOOL DefaultsDialog ( HWND Window, HMODULE Library, DEFAULTS_PARMS &Parms ) ;

extern void ShowFormSize (
   HAB Anchor,
   HMODULE Library,
   DocumentParms &DocParms,
   HWND hwndField,
   PHCINFO pFormInfo,
   char *Preface
) ;

#endif
