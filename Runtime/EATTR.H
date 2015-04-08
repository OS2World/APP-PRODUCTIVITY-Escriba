/******************************************************************** EATTR.H
 *                                                                          *
 *      Extended Attribute Support Functions                                *
 *                                                                          *
 ****************************************************************************/

#ifndef EATTR_H
#define EATTR_H

#pragma pack(2)

typedef struct {
   PSZ    Name ;
   USHORT Type ;
   USHORT Length ;
   PVOID  Value ;
} EADATA, *PEADATA ;

typedef struct {
   USHORT Type ;
   USHORT Length ;
   PVOID  Value ;
} MVMT_VALUE, *PMVMT_VALUE ;

#pragma pack()

extern PEAOP2 BuildExtendedAttributes ( ULONG Count, EADATA Table[] ) ;
extern ULONG BuildMVMTValue ( PVOID Value, ULONG Count, MVMT_VALUE Table[] ) ;

#endif
