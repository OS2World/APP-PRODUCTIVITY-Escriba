/******************************************************************** CRC.CPP
 *                                                                          *
 *                        CRC Check-digit Functions                         *
 *                                                                          *
 ****************************************************************************/

#include "System.h"

#include "Crc.h"

 
/***************************************************************************
 *                                                                         *
 *  Update CRC-16 with next byte in block to be verified.                  *
 *                                                                         *
 ***************************************************************************/

extern unsigned short UpdateCRC16 ( unsigned short Crc, void *Data, int Size ) {
   for ( int i=0; i<Size; i++ ) {
      Crc = UpdateCRC16 ( Crc, *(((unsigned char *)(Data))+i) ) ;
   } /* endfor */
   return ( Crc ) ;
}

extern unsigned short UpdateCRC16 ( unsigned short Crc, unsigned char Byte ) {

   register unsigned short r4, r5, r6, r7 ;

   r4 = Byte ;
   r5 = Crc ;

   r5 ^= r4 ;
   r6 = ( unsigned short ) ( r5 << 8 ) ;
   r5 = ( unsigned short ) ( r5 >> 8 ) ;
   r7 = r6 ;
   r7 = ( unsigned short ) ( r7 << 1 ) ;
   r7 ^= r6 ;
   r6 = r7 ;
   r7 = ( unsigned short ) ( r7 << 2 ) ;
   r7 ^= r6 ;
   r6 = r7 ;
   r7 = ( unsigned short ) ( r7 << 4 ) ;
   r7 ^= r6 ;
   r5 ^= r7 ;
   r7 = ( unsigned short ) ( r7 >> 2 ) ;
   r5 ^= r7 ;
   r7 = ( unsigned short ) ( r7 >> 13 ) ;
   r5 ^= r7 ;

   return ( r5 ) ;
}

