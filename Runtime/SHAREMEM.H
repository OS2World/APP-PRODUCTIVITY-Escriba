/***************************************************************** SHAREMEM.H
 *                                                                          *
 *                 Named & Shared Public Memory (OS2/NT)                    *
 *                                                                          *
 ****************************************************************************/

#ifndef SHAREMEM_H
#define SHAREMEM_H

#include "System.h"

class PublicMemory {

   private:

      void *pMemory ;
      unsigned long Size ;
      int Created ;
      #ifdef __NT__
         HANDLE FileMapping ;
      #endif // __NT__

   public:

      PublicMemory ( char *name, unsigned long size=0, int WriteAccess=0 ) ;
      ~PublicMemory ( ) ;

      void *QueryMemory ( ) { return ( pMemory ) ; }
      unsigned long QuerySize ( ) { return ( Size ) ; }
      int QueryCreated ( ) { return ( Created ) ; }

} ;

#endif
