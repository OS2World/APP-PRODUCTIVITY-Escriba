/***************************************************************** DOCFRAME.H
 *                                                                          *
 *                          Document Frame Window                           *
 *                                                                          *
 ****************************************************************************/

#ifndef DOCFRAME_H
#define DOCFRAME_H

#include "System.h"

#include "Document.h"
#include "HelpWin.h"
#include "Profile2.h"
#include "Speller.h"

#define DOCFRAME_CLASS_NAME "DocFrame"

enum {
   NULLID,
   FID_STATUSLINE,
   FID_HORZRULER,
   FID_VERTRULER,
   FID_TOP,
   FID_PAGEUP,
   FID_PAGEDOWN,
   FID_BOTTOM,
   FID_CORNER1,
   FID_CORNER2,
   FID_CORNER3,
   FID_CORNER4
} ;

#define FID_COUNT  ( 20 + FID_CORNER3 )

typedef struct {
   HelpWindow *Help ;
   INIDATA    *IniData ;
   LanguageModule *Library ;
   HWND        MainWindow ;
   Documento  *pDocument ;
   Dictionary *pSpeller ;
   USHORT      WindowID ;
   BOOL        Trap ;

} DOCFRAME_PARMS, *PDOCFRAME_PARMS ;

extern WINDOWPROC DocFrameMessageProcessor ;

#endif
