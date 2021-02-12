/***************************************************************** BARCODES.H
 *                                                                          *
 *                       Barcode Dialog Definitions                         *
 *                                                                          *
 ****************************************************************************/

#ifndef BARCODES_H
#define BARCODES_H

#include "System.h"

typedef struct {

   HMODULE Library ;
   char Type [3] ;
   char Text [80] ;

} BARCODES_PARMS, *PBARCODES_PARMS ;

extern WINDOWPROC Barcodes_Processor ;

#endif
