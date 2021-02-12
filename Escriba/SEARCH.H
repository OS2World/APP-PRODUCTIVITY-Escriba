/******************************************************************* SEARCH.H
 *                                                                          *
 *                         Search Dialog definitions                        *
 *                                                                          *
 ****************************************************************************/

#ifndef SEARCH_H
#define SEARCH_H

#include "System.h"

#include "Profile2.h"

typedef struct {
   HMODULE Library ;
   INIDATA *IniData ;
   Text *pText ;
   HWND Owner ;                
   BOOL FirstTime ;
} SEARCH_PARMS, *PSEARCH_PARMS ;

extern WINDOWPROC Search_Processor ;

#endif
