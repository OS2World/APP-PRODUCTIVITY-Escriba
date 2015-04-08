/***************************************************************** PRINTER.H
 *                                                                         *
 *                          PM Printer Support                             *
 *                                                                         *
 ***************************************************************************/

#ifndef PRINTER_H
#define PRINTER_H

#include <stdlib.h>
#include <string.h>

#include "System.h"

class QueueInfo {
   private:
      char *ObjectName ;
      char *Name ;
      char *Form ;
      PRQINFO3 Data ;

   public:
      QueueInfo ( char *objectname, HAB Anchor, char *QueueName, char *FormName, PDRIVDATA Settings ) ;
      ~QueueInfo ( ) ;

      void UpdateSettings ( PDRIVDATA Settings ) {
         if ( Data.pDriverData ) free ( Data.pDriverData ) ;
         Data.pDriverData = PDRIVDATA ( malloc ( size_t ( Settings->cb ) ) ) ;
         memcpy ( Data.pDriverData, Settings, size_t(Settings->cb) ) ;
      } /* endmethod */

      char *QueryDriverName ( )     { return ( Data.pszDriverName ) ; }
      char *QueryName ( )           { return ( Data.pszName ) ; }
      char *QueryPrinters ( )       { return ( Data.pszPrinters ) ; }
      PDRIVDATA QuerySettings ( ) { return ( Data.pDriverData ) ; }

      char *QueryForm ( ) { return ( Form?Form:"" ) ; }
      void QueryForm ( HAB Anchor, BOOL Metric, HCINFO &FormInfo ) ;
      BOOL QueryResolution ( HAB Anchor, BOOL Metric, long &Horizontal, long &Vertical ) ;
} ;

extern HDC OpenPrinterDC ( HAB Anchor, char *QueueName, char *FormName, PDRIVDATA Settings ) ;

extern HDC OpenPrinterDC ( HAB Anchor, char *QueueName, char *FormName, PDRIVDATA Settings, int Copies ) ;

#endif
