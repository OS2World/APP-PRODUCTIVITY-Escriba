/*************************************************************** CIRQUEUE.CPP
 *                                                                          *
 *                             Circular Queue                               *
 *                                                                          *
 ****************************************************************************/

#include "System.h"

#include <stdlib.h>
#include <string.h>

#include "CirQueue.h"
#include "Debug.h"

// #define DEBUG_LOCK

CircularQueue::CircularQueue ( char *name, int size, int count ) :
   Name(name), Size(size), Count(count), LastIn(0), LastOut(0), Array(0), 
   Lock("CircularQueue::Lock"), 
   DataAvailable("CircularQueue::DataAvailable"), 
   SpaceAvailable("CircularQueue::SpaceAvailable") {

   #ifdef DEBUG_LOCK
      Lock.SetDebug ( TRUE ) ;
   #endif // DEBUG_LOCK

   Array = new char [ Size * Count ] ;
   DataAvailable.Reset() ;
   SpaceAvailable.Post() ;

} /* endmethod */

CircularQueue::~CircularQueue ( ) {

   if ( Array ) 
      delete [] Array, Array = 0 ;

} /* endmethod */

void CircularQueue::Dump ( int indent ) {
   Log ( "%*sDump of circular queue '%s':", indent, "", Name ) ;
   Log ( "%*s  Size, Count: %i, %i", indent, "", Size, Count ) ;
   Log ( "%*s  LastIn, LastOut: %i, %i", indent, "", LastIn, LastOut ) ;
   Log ( "%*s  Array: %p (%i bytes)", indent, "", Array, Size*Count ) ;
} /* endmethod */

BOOL CircularQueue::Enqueue ( void *Entry, int Timeout ) {
   BOOL Done = FALSE ;
   while ( ! Done ) {
      Lock.Request ( ) ;
      int NewLastIn = LastIn + 1;
      if ( NewLastIn >= Count )
         NewLastIn = 0;
      if ( NewLastIn != LastOut ) {
         LastIn = NewLastIn ;
         memcpy ( Array+LastIn*Size, Entry, Size ) ;
         DataAvailable.Post() ;
         Done = TRUE ;
      } else {
         SpaceAvailable.Reset ( ) ;
      } /* endif */
      Lock.Release ( ) ;
      if ( !Done ) {
         if ( !Timeout )
            break ;
         if ( !SpaceAvailable.Wait ( Timeout ) )
            break ;
      } /* endif */
   } /* endwhile */
   return ( Done ) ;
} /* endmethod */

BOOL CircularQueue::Wait ( int Timeout ) {
   int Done = FALSE ;
   while ( ! Done ) {
      Lock.Request ( ) ;
      if ( LastIn != LastOut ) {
         Done = TRUE ;
      } else {
         DataAvailable.Reset ( ) ;
      } /* endif */
      Lock.Release ( ) ;
      if ( !Done && Timeout && !DataAvailable.Wait ( Timeout ) )
         break ;
   } /* endwhile */
   return ( Done ) ;
} /* endmethod */

BOOL CircularQueue::Peek ( void *Entry ) {
   int Done = FALSE ;
   Lock.Request ( ) ;
   if ( LastIn != LastOut ) {
      int NewLastOut = LastOut + 1 ;
      if ( NewLastOut >= Count )
         NewLastOut = 0 ;
      memcpy ( Entry, Array+NewLastOut*Size, Size ) ;
      Done = TRUE ;
   } /* endif */
   Lock.Release ( ) ;
   return ( Done ) ;
} /* endmethod */

BOOL CircularQueue::Dequeue ( void *Entry, int Timeout ) {
   int Done = FALSE ;
   while ( ! Done ) {
      Lock.Request ( ) ;
      if ( LastIn != LastOut ) {
         LastOut ++ ;
         if ( LastOut >= Count )
            LastOut = 0 ;
         memcpy ( Entry, Array+LastOut*Size, Size ) ;
         SpaceAvailable.Post() ;
         Done = TRUE ;
      } /* endif */
      if ( LastIn == LastOut ) 
         DataAvailable.Reset ( ) ;
      Lock.Release ( ) ;
      if ( !Done && Timeout && !DataAvailable.Wait ( Timeout ) )
         break ;
   } /* endwhile */
   return ( Done ) ;
} /* endmethod */

BOOL CircularQueue::Empty ( ) { 
   Lock.Request ( ) ;
   BOOL Result = LastIn == LastOut ;
   Lock.Release ( ) ;
   return ( Result ) ;
} /* endmethod */

