/******************************************************************* DOCSUP.H
 *                                                                          *
 *                        Document Class Support                            *
 *                                                                          *
 ****************************************************************************/

#ifndef DOCSUP_H
#define DOCSUP_H

#include "System.h"

#include "DocParms.h"
#include "Readers.h"
#include "State.h"
#include "Text.h"
#include "WorkPage.h"
#include "WrkSpace.h"

extern BOOL GetTokenData ( PUCHAR &Data, long &Size, PREADER Reader, PREADER Peeker, PVOID ReaderData, char *BadToken=0, int BadTokenSize=0 ) ;

extern int ImportText ( HAB Anchor, unsigned char *pExternal, int DataLen, unsigned char *pInternal, int &Version, char *BadToken=0, int BadTokenSize=0 ) ;
extern BOOL ImportToken ( HAB Anchor, PUCHAR &DataOut, long &SizeOut, PUCHAR DataIn, long SizeIn, LONG &Handle, int &HandleType, int &Version, char *BadToken=0, int BadTokenSize=0 ) ;

extern unsigned char *ExportText ( HAB Anchor, WorkSpace *PS, unsigned char *External, unsigned char *Internal, int InternalCount ) ;
extern BOOL ExportToken ( HAB Anchor, WorkSpace *PS, PUCHAR &DataOut, long &SizeOut, PUCHAR DataIn, long &SizeIn ) ;

extern BOOL ComputeFunctionSize ( HAB Anchor, WorkSpace *PS, PUCHAR DataIn, long &SizeOut, long &SizeIn ) ;
extern BOOL ComputeFunctionSize ( PUCHAR DataIn, long &SizeIn ) ;

extern void GetToken ( Text *pText, long Offset, PUCHAR &pToken, long &Size ) ;
extern void GetToken ( unsigned char *p, int len, int Index, PUCHAR &pToken, long &Size ) ;

extern ULONG WriteBinary ( HFILE Handle, PVOID Data, ULONG Size ) ;
extern ULONG WriteBinary ( PVOID Destination, PVOID Data, ULONG Size ) ;

extern void ComputeEffectiveState ( WorkSpace *PS, Text *pText, PUCHAR DBCSVector, long Offset, Estado &State, Estado &EffectiveState, int PageNumber, int NumberOfPages, time_t FileDate ) ;
extern void ComputeEffectiveState ( WorkSpace *PS, unsigned char *p, int len, PUCHAR DBCSVector, long Offset, _Estado &State, _Estado &EffectiveState ) ;

extern void ComputeNextState ( WorkSpace *PS, Text *pText, long Offset, long Length, Estado &State, Estado &NextState, int PageNumber, int NumberOfPages, time_t FileDate ) ;
extern void ComputeNextState ( WorkSpace *PS, unsigned char *p, int len, long Offset, long Length, _Estado &State, _Estado &NextState ) ;

extern long CreateGraphicToken ( // Returns size of token.
   BOOL InternalUseOnly,        // Flag: Is this token for internal use only?
   char *Text,                  // -> Text buffer for external token being created.
   POINTL &Position,            // Graphic position.
   SIZEL &Size,                 // Graphic size.
   BOOL Background,             // Flag: Display graphic as a background?
   BOOL NoTextAside,            // Flag: Prohibit text to either size of graphic?
   BOOL Halftone,               // Flag: Display graphic half-tone?
   BOOL Reverse,                // Flag: Display graphic reverse?
   int Aspect,                  // Aspect preservation type (unfixed, fixX, fixY).
   BOOL PrinterOptimize,        // Flag: Optimize size for printer?
   char *Type,                  // Graphic type: BM, MF, UP, 39, PN.
   long Handle,                 // Handle for BM, MF.
   char *BarCode                // Text for UP, 39, PN.
) ;

extern void CreateGraphicToken ( char Text[], Grafico &Graphic ) ;

#endif // If not already included.
