// Class PROCESS: Encapsulates the startup/shutdown logic for a OS/2-PM process.

#ifndef PROCESS_H
#define PROCESS_H

#include "System.h"

class Process {

   private:

      char *Name ;
      HAB Anchor ;
      HMQ Queue ;

   public:

      Process ( char *Name, HWND Owner, HMODULE Library, long QueueSize = 0, unsigned short CodePage = 0 ) ;
      ~Process ( ) ;
      inline HAB QueryAnchor    ( ) { return ( Anchor ) ; }
      inline HMQ QueryQueue     ( ) { return ( Queue ) ; }
} ;

extern BOOL SetQueueCodepage ( HWND Owner, HMQ Queue, unsigned short CodePage ) ;

#endif
