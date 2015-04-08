/***************************************************************** REGISTER.H
 *                                                                          *
 *  Registration Support                                                    *
 *                                                                          *
 ****************************************************************************/

#ifndef REGISTER_H
#define REGISTER_H

#include "Crc.h"

extern "C" { PVOID DosSelToFlat ( ULONG ulSelector ) ; }

class Registration {

   private:
      BOOL Valid ;
      char *Name ;
      unsigned long Serial ;

   public:
      Registration ( HMODULE Library ) : Valid(FALSE), Name(0), Serial(0) { 
         if ( Library ) {
            // Get name and number, and validate them.
            PULONG ppName ;
            if ( DosQueryProcAddr ( Library, 0, PSZ("_NAME"), (PFN*)&ppName ) == 0 ) {
               PCHAR pName = PCHAR ( DosSelToFlat ( *ppName ) ) ;
               USHORT Crc = 0;
               for ( int i=0; i<strlen(pName); i++ ) {
                  Crc = UpdateCRC16 ( Crc, pName[i] ) ;
               } /* endfor */
               PULONG pSerial ;
               if ( DosQueryProcAddr ( Library, 0, PSZ("_SERIAL"), (PFN*)&pSerial ) == 0 ) {
                  Crc = UpdateCRC16 ( Crc, *(((unsigned char*)pSerial)+0) ) ;
                  Crc = UpdateCRC16 ( Crc, *(((unsigned char*)pSerial)+1) ) ;
                  Crc = UpdateCRC16 ( Crc, *(((unsigned char*)pSerial)+2) ) ;
                  Crc = UpdateCRC16 ( Crc, *(((unsigned char*)pSerial)+3) ) ;
                  PUSHORT pCheck ;
                  if ( DosQueryProcAddr ( Library, 0, PSZ("_CHECK"), (PFN*)&pCheck ) == 0 ) {
                     if ( Crc == *pCheck ) {
                        Valid = TRUE ;
                        Name = (char*) malloc ( strlen(pName)+1 ) ;
                        strcpy ( Name, pName ) ;
                        Serial = *pSerial ;
                     } /* endif */
                  } /* endif */
               } /* endif */
            } /* endif */
         } /* endif */
      } /* endmethod */

      ~Registration ( ) {
         if ( Name ) 
            free ( Name ) ;
      } /* endmethod */

      BOOL IsValid ( ) { return ( Valid ) ; }
      char *Query_Name ( ) { return ( Name ) ; }
      unsigned long Query_Serial ( ) { return ( Serial ) ; }
} ;

#endif // If not already included.
