/******************************************************************* PREFER.H
 *                                                                          *
 *                        General Preferences Dialog                        *
 *                                                                          *
 ****************************************************************************/

#ifndef PREFER_H
#define PREFER_H

#include "System.h"

typedef struct {

   HMODULE Library ;

   BOOL Animate ;
   BOOL ScrollBars ;
   BOOL Rulers ;
   BOOL StatusLine ;
   BOOL FullNames ;
   BOOL SpoolPages ;
   BOOL PrintReverse ;
   BOOL LoadMostRecent ;
   int ZoomType ;

} PREFERENCES_PARMS, *PPREFERENCES_PARMS ;

extern WINDOWPROC PreferencesProcessor ;

#endif
