/******************************************************************** DEBUG.H
 *                                                                          *
 *  Debugging Aids                                                          *
 *                                                                          *
 ****************************************************************************/

#ifndef DEBUG_H
#define DEBUG_H

#include "System.h"

#include <stdio.h>
#include <stdlib.h>

#include "HRTimer.h"

extern char HomePath [_MAX_PATH+1] ;
extern FILE *Logfile ;
extern HFILE Timer ;
extern BOOL Trace ;

extern void Debug ( HWND hwnd, char *Message, ... ) ;
extern void Log ( char *Message, ... ) ;
extern void DumpMemory ( char *Note, PVOID pMemory, int Count, int Indent=0 ) ;

extern BOOL OpenTimer ( void ) ;
extern void CloseTimer ( void ) ;
extern BOOL GetTime ( PTIMESTAMP pts ) ;
extern ULONG ComputeElapsedTime ( PTIMESTAMP ptsStart, PTIMESTAMP ptsStop, PULONG pulNs ) ;

extern char *InterpretWinError ( HAB Anchor, char *MessageBuffer ) ;

extern char *InterpretMsg ( MESG msg, char *Buffer ) ;
extern void LogMsgRcvd ( int Indent, char *Prefix, MESG msg, MPARAM1 mp1, MPARAM2 mp2 ) ;
extern void LogMsgDone ( int Indent, char *Prefix, MESG msg, MRESULT Result ) ;

#endif
