/********************************************************************* POOL.H
 *                                                                          *
 *  POOL Container Class (for holding groups of like objects)               *
 *                                                                          *
 *  The purpose here is to allocate memory for a linked list of objects,    *
 *  which linked list can grow later, if necessary.                         *
 *                                                                          *
 ****************************************************************************/

#ifndef POOL_H
#define POOL_H

#include "System.h"

class PoolEntry {

   private:
      PoolEntry *Prev, *Next ;

   public:

      PoolEntry ( ) : Prev(0), Next(0) {
      } /* endmethod */

      PoolEntry *Query_Prev ( ) { return ( Prev ) ; }
      PoolEntry *Query_Next ( ) { return ( Next ) ; }

      void Set_Prev ( PoolEntry *prev ) { Prev = prev ; } 
      void Set_Next ( PoolEntry *next ) { Next = next ; } 

      void *Data ( ) { return ( this + 1 ) ; }
} ;

class PoolSection {

   private:
      char *Memory ;
      PoolSection *Prev, *Next ;
      int EntrySize, TotalSize ;

   public:

      PoolSection ( int objectsize, int sectioncount ) ;
      virtual ~PoolSection ( ) ;

      void Clear ( PoolSection *NextSection = 0 ) ;

      PoolSection *Query_Prev ( ) { return ( Prev ) ; }
      PoolSection *Query_Next ( ) { return ( Next ) ; }

      void Set_Prev ( PoolSection *prev ) { Prev = prev ; } 
      void Set_Next ( PoolSection *next ) { Next = next ; } 

      void *Query_Memory ( ) { return ( Memory ) ; }
} ;

class Pool { 

   private:

      char *Tag ;
      int ObjectSize, SectionCount ;
      int Count, FreeCount ;
      PoolEntry *First, *Last, *Free ;
      PoolSection *FirstSection, *LastSection ;

   public:

      Pool ( char *tag, int objectsize, int sectioncount ) ;
      Pool ( Pool &Original ) ;
      virtual ~Pool ( ) ;

      Pool & operator= ( const Pool & Object ) ;

      PoolEntry *Find ( int Index ) const ;
      PoolEntry *Find ( PoolEntry *entry ) const ;

      PoolEntry *Prev ( int Index ) const ;
      PoolEntry *Prev ( PoolEntry *entry ) const ;

      PoolEntry *Next ( int Index ) const ;
      PoolEntry *Next ( PoolEntry *entry ) const ;

      PoolEntry *Insert ( PoolEntry *predecessor, void *object ) ;
      void       Delete ( PoolEntry *entry ) ;
      void       Clear ( ) ;

      int Query_Count ( ) { return ( Count ) ; }
      int Query_FreeCount ( ) { return ( FreeCount ) ; }

      void Dump ( int indent = 0 ) ;
} ;

#endif 
