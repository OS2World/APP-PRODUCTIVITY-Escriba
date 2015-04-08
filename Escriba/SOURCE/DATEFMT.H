/****************************************************************** DATEFMT.H
 *                                                                          *
 *                     Date Format Dialog definitions                       *
 *                                                                          *
 ****************************************************************************/

#ifndef DATEFMT_H
#define DATEFMT_H

#include <time.h>

#include "System.h"

typedef struct {
   HMODULE Library ;
   CHAR    Format [80] ;
} DATEFMT_PARMS, *PDATEFMT_PARMS ;

extern WINDOWPROC DateFormatProcessor ;

extern BOOL VerifyDateFormat ( HWND hwndField, HWND hwndError, HWND hwndSample, char *DateFormat ) ;

extern void LoadDateFormattingResources ( void ) ;
extern void UnloadDateFormattingResources ( void ) ;

extern char *FormatDate ( time_t Time, char *Format, char *Buffer, int BufferSize ) ;

extern char *LocalDateFormat ( char *Local, char *General ) ;
extern char *GeneralDateFormat ( char *General, char *Local ) ;

#endif
