/******************************************************************* DEVICE.H
 *                                                                          *
 *                      Graphic Device Context Class                        *
 *                                                                          *
 ****************************************************************************/

#ifndef DEVICE_H
#define DEVICE_H

#include "System.h"
#include "Mutex.h"
#include "Printer.h"

enum DeviceType {
   DEVICE_WINDOW,
   DEVICE_PRINTER,
   DEVICE_PRINTER_INFO,
   DEVICE_MEMORY
} ;

class DeviceContext {

   private:

      char *Name ;
      DeviceType Type ;
      HDC hDC ;
      BOOL Created ;
      Mutex LockSemaphore ;

   public:

      // Create a Window DC object.
      DeviceContext ( char *Name, HWND Window ) ;

      // Create a Printer DC object.
      DeviceContext ( char *Name, HAB Anchor, char *QueueName, char *FormName, PDRIVDATA Settings, int Copies ) ;

      // Create a Printer DC object (info only).
      DeviceContext ( char *Name, HAB Anchor, char *QueueName, char *FormName, PDRIVDATA Settings ) ;

      // Create a memory DC object.
      DeviceContext ( char *Name, long Anchor ) ;

      ~DeviceContext ( ) ;

      HDC QueryHandle ( ) { return ( hDC ) ; }

      DeviceType QueryType ( ) { return ( Type ) ; }

      APIRET Lock ( int Timeout = INFINITE ) {
         return ( LockSemaphore.Request ( Timeout ) ) ;
      } /* endmethod */

      void Unlock ( ) {
         LockSemaphore.Release ( ) ;
      } /* endmethod */

      BOOL Associate ( HPS hPS ) {
         return ( GpiAssociate ( hPS, hDC ) ) ;
      } /* endmethod */

      void Dump ( int Indent = 0 ) ;
} ;

#endif
