/******************************************************************* DOCWIN.H
 *                                                                          *
 *  Document Window                                                         *
 *                                                                          *
 ****************************************************************************/

#ifndef DOCWIN_H
#define DOCWIN_H

#include "System.h"

#include "Document.h"

#define DOCWINDOW_CLASS_NAME "DocWindow"

typedef struct {
   short       Filler ;
   INIDATA    *IniData ;
   LanguageModule *Library ;
   HWND        MainWindow ;
   Documento  *pDocument ;
   Dictionary *pSpeller ;

} DOCWINDOW_PARMS, *PDOCWINDOW_PARMS ;

extern WINDOWPROC DocWindowMessageProcessor ;

#endif
