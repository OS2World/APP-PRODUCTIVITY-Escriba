/*************************************************************** SHAREMEM.CPP
 *                                                                          *
 *                 Named & Shared Public Memory (OS2/NT)                    *
 *                                                                          *
 ****************************************************************************/

#include "System.h"

#include <stdlib.h>
#include <string.h>

#include "Debug.h"
#include "ShareMem.h"

PublicMemory::PublicMemory ( char *Name, unsigned long size, int WriteAccess ) : pMemory(0), Size(size), Created(0) {

   #ifdef __OS2__

      char FullName [_MAX_PATH] ;
      strcpy ( FullName, "\\SHAREMEM\\" ) ;
      strcat ( FullName, Name?Name:"" ) ;

      APIRET Status = DosGetNamedSharedMem ( &pMemory, FullName, PAG_READ | ( WriteAccess?PAG_WRITE:0 )  ) ;

      if ( Status && Size && WriteAccess ) {
         pMemory = 0 ;
         Status = DosAllocSharedMem ( &pMemory, FullName, Size, fALLOC ) ;
         if ( Status ) {
            Log ( "ERROR: Unable to allocate shared memory object '%s'.  Code %i.", Name, Status ) ;
         } else {
            Created = 1 ;
            memset ( pMemory, 0, Size ) ;
         } /* endif */
      } else if ( !Status && !Size ) {
         Size = 0x1000000 ;
         ULONG Flags ;
         DosQueryMem ( pMemory, &Size, &Flags ) ;
      } /* endif */

   #else

      FileMapping = CreateFileMapping ( 0xFFFFFFFF, 0, WriteAccess ? PAGE_READWRITE : PAGE_READONLY, 0, Size, Name ) ;
      if ( FileMapping == 0 ) {
         Log ( "ERROR: Unable to allocate shared memory object '%s'.  Code %i.", Name, GetLastError() ) ;
      } else if ( GetLastError() == ERROR_ALREADY_EXISTS ) {
         ;
      } else {
         Created = 1 ;
      } /* endif */

      if ( FileMapping ) {
         pMemory = MapViewOfFile ( FileMapping, ( WriteAccess ? FILE_MAP_WRITE : 0 ) | FILE_MAP_READ, 0, 0, Size ) ;
         if ( pMemory == 0 ) {
            Log ( "ERROR: Unable to map view of shared memory object '%s'.  Code %i.", Name, GetLastError() ) ;
            CloseHandle ( FileMapping ), FileMapping = 0 ;
         } else {
            if ( Created ) 
               memset ( pMemory, 0, Size ) ;
         } /* endif */
      } /* endif */

   #endif 

} /* endmethod */

PublicMemory::~PublicMemory ( ) {

   #ifdef __OS2__

      if ( pMemory ) 
         DosFreeMem ( pMemory ) ;

   #else

      if ( pMemory ) 
         UnmapViewOfFile ( pMemory ), pMemory = 0 ;

      if ( FileMapping ) 
         CloseHandle ( FileMapping ), FileMapping = 0 ;

   #endif

} /* endmethod */

