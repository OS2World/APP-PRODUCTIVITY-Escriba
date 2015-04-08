/***************************************************************** CIRQUEUE.H
 *                                                                          *
 *                             Circular Queue                               *
 *                                                                          *
 ****************************************************************************/

#ifndef CIRQUEUE_H
#define CIRQUEUE_H

#include <string.h>

#include "System.h"

#include "Event.h"
#include "Mutex.h"

class CircularQueue {

   private:
      char *Name ;
      int Size, Count ;
      int LastIn, LastOut ;
      char *Array ;
      Mutex Lock ;
      Event DataAvailable, SpaceAvailable ;

   public:
      CircularQueue ( char *name, int size, int count=100 ) ;

      ~CircularQueue ( ) ;

      void Dump ( int indent = 0 ) ;

      BOOL Enqueue ( void *Entry, int Timeout=INFINITE ) ;

      BOOL Wait ( int Timeout=INFINITE ) ;
      BOOL Peek ( void *Entry ) ;
      BOOL Dequeue ( void *Entry, int Timeout=INFINITE ) ;

      BOOL Empty ( ) ;
} ;

#endif
