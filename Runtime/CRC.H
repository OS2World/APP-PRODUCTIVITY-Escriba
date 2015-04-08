/********************************************************************** CRC.H
 *                                                                          *
 *                        CRC Check-digit Functions                         *
 *                                                                          *
 ****************************************************************************/

#ifndef CRC_H
#define CRC_H

extern unsigned short UpdateCRC16 ( unsigned short Crc, void *Data, int Size ) ;
extern unsigned short UpdateCRC16 ( unsigned short Crc, unsigned char Byte ) ;

#endif
