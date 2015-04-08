/****************************************************************** EATTR.CPP
 *                                                                          *
 *      Extended Attribute Support Functions                                *
 *                                                                          *
 ****************************************************************************/

#include "System.h"

#include <stdlib.h>
#include <string.h>

#include "EAttr.h"

/****************************************************************************
 *                                                                          *
 *      Build Extended Attribute Item                                       *
 *                                                                          *
 ****************************************************************************/

static ULONG BuildExtendedAttributeItem ( PFEA2 pFEA, PEADATA Item ) {

  /**************************************************************************
   * Store next entry's offset (set to zero at this point).                 *
   **************************************************************************/

   PUCHAR p = PUCHAR ( pFEA ) ;

   if ( pFEA )
      *(PULONG(p)) = 0 ;

   p += sizeof(ULONG) ;

  /**************************************************************************
   * Store flag byte.                                                       *
   **************************************************************************/

   if ( pFEA )
      *p = 0 ;

   p ++ ;

  /**************************************************************************
   * Store count of bytes in name.                                          *
   **************************************************************************/

   if ( pFEA )
      *p = UCHAR ( strlen ( PCHAR(Item->Name) ) ) ;

   p ++ ;

  /**************************************************************************
   * Store count of bytes in value.                                         *
   **************************************************************************/

   if ( pFEA )
      *(PUSHORT(p)) = USHORT ( Item->Length + 2 * sizeof(USHORT) ) ;

   p += sizeof(USHORT) ;

  /**************************************************************************
   * Store name.                                                            *
   **************************************************************************/

   if ( pFEA )
      strcpy ( PCHAR(p), PCHAR(Item->Name) ) ;

   p += strlen ( PCHAR(Item->Name) ) + 1 ;

  /**************************************************************************
   * Store value's type.                                                    *
   **************************************************************************/

   if ( pFEA )
      *(PUSHORT(p)) = Item->Type ;

   p += sizeof(USHORT) ;

  /**************************************************************************
   * Store value's length.                                                  *
   **************************************************************************/

   if ( pFEA )
      *(PUSHORT(p)) = Item->Length ;

   p += sizeof(USHORT) ;

  /**************************************************************************
   * Store value.                                                           *
   **************************************************************************/

   if ( pFEA )
      memcpy ( p, Item->Value, Item->Length ) ;

   p += Item->Length ;

  /**************************************************************************
   * Return count of bytes needed for item.                                 *
   **************************************************************************/

   return ( p - PUCHAR(pFEA) ) ;
}


/****************************************************************************
 *                                                                          *
 *      Build Extended Attributes                                           *
 *                                                                          *
 ****************************************************************************/

extern PEAOP2 BuildExtendedAttributes ( ULONG Count, EADATA Table[] ) {

  /**************************************************************************
   * Find out how much memory will be needed for the block.                 *
   **************************************************************************/

   ULONG cbEA = sizeof(FEA2LIST) - sizeof(FEA2) ;

   for ( int i=0; i<Count; i++ ) {
      cbEA += BuildExtendedAttributeItem ( 0, &Table[i] ) ;
      while ( cbEA % 4 )
         cbEA ++ ;
   } /* endfor i */

  /**************************************************************************
   * Allocate memory for the FEA2 list.                                     *
   **************************************************************************/

   PFEA2LIST pFEAList = PFEA2LIST ( malloc ( size_t(cbEA) ) ) ;

   if ( pFEAList == 0 )
      return ( 0 ) ;

  /**************************************************************************
   * Construct the extended attributes.                                     *
   **************************************************************************/

   PFEA2 pFEA = pFEAList->list ;

   for ( i=0; i<Count; i++ ) {
      PFEA2 Start = PFEA2 ( pFEA ) ;
      PUCHAR p = PUCHAR(pFEA) + BuildExtendedAttributeItem ( pFEA, &Table[i] ) ;
      pFEA = PFEA2 ( p ) ;
      while ( (PUCHAR(pFEA)-PUCHAR(pFEAList->list)) % 4 ) {
         PUCHAR p = PUCHAR ( pFEA ) + 1 ;
         pFEA = PFEA2 ( p ) ;
      } /* endwhile */
     if ( i < Count-1 )
        Start->oNextEntryOffset = PUCHAR(pFEA) - PUCHAR(Start) ;
     else
        Start->oNextEntryOffset = 0 ;
   } /* endfor i */

   pFEAList->cbList = PUCHAR(pFEA) - PUCHAR(pFEAList) ;

  /**************************************************************************
   * Allocate memory for the EA header block.                               *
   **************************************************************************/

   PEAOP2 pExtendedAttributes = PEAOP2 ( malloc ( sizeof(EAOP2) ) ) ;

   if ( pExtendedAttributes == 0 ) {
      free ( pFEAList ) ;
      return ( 0 ) ;
   } /* endif */

  /**************************************************************************
   * Fill in the extended attribute header block and return its address.    *
   **************************************************************************/

   pExtendedAttributes->fpGEA2List = 0 ;
   pExtendedAttributes->fpFEA2List = pFEAList ;
   pExtendedAttributes->oError = 0 ;

   return ( pExtendedAttributes ) ;
}


/****************************************************************************
 *                                                                          *
 *      Build Multi-Value Multi-Type EA Item's Value                        *
 *                                                                          *
 ****************************************************************************/

extern ULONG BuildMVMTValue ( PVOID Value, ULONG Count, MVMT_VALUE Table[] ) {

  /**************************************************************************
   * Store the number of values.                                            *
   **************************************************************************/

   PUCHAR p = PUCHAR ( Value ) ;

   if ( Value )
      *(PUSHORT(p)) = USHORT ( Count ) ;

   p += sizeof(USHORT) ;

  /**************************************************************************
   * Store the multiple values.                                             *
   **************************************************************************/

   for ( int i=0; i<Count; i++ ) {

      if ( Value )
         *(PUSHORT(p)) = Table[i].Type ;

      p += sizeof(USHORT) ;

      if ( Value )
         *(PUSHORT(p)) = Table[i].Length ;

      p += sizeof(USHORT) ;

      if ( Value )
         memcpy ( p, Table[i].Value, Table[i].Length ) ;

      p += Table[i].Length ;

   } /* endfor i */

  /**************************************************************************
   * Return the total byte count.                                           *
   **************************************************************************/

   return ( p - PUCHAR(Value) ) ;
}



