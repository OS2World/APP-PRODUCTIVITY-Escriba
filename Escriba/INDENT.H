/******************************************************************* INDENT.H
 *                                                                          *
 *                      Indent Dialog definitions                           *
 *                                                                          *
 ****************************************************************************/

#ifndef INDENT_H
#define INDENT_H

#include "System.h"

typedef struct {
   HMODULE Library ;
   HWND    Window ;
   int     Metric ;
   HCINFO  FormInfo ;
   long    LeftMargin ;
   long    RightMargin ;
   int     Indent ;
} INDENT_PARMS, *PINDENT_PARMS ;

extern WINDOWPROC IndentProcessor ;

extern BOOL VerifyIndent ( 
   HWND hwndField,              // -> Entryfield window
   HWND hwndError,              // -> Errorfield window
   int &Indent,                 // Current indentation.
   HCINFO &FormInfo,            // Current form information.
   long LeftMargin,             // Current left margin.
   long RightMargin             // Current right margin.
) ;

#endif
