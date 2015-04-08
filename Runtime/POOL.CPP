/******************************************************************* POOL.CPP
 *                                                                          *
 *  POOL Container Class (for holding groups of like objects)               *
 *                                                                          *
 *  The purpose here is to allocate memory for a linked list of objects,    *
 *  which linked list can grow later, if necessary.                         *
 *                                                                          *
 ****************************************************************************/

#include "System.h"

#include <stdlib.h>
#include <string.h>

#include "Debug.h"
#include "Pool.h"

// #define DEBUG

PoolSection::PoolSection ( int objectsize, int sectioncount ) : Prev(0), Next(0) {

   // Allocate the section.
   EntrySize = sizeof(PoolEntry) + objectsize ;
   TotalSize = EntrySize * sectioncount ;
   Memory = (char*) malloc ( TotalSize ) ;
   if ( Memory == 0 ) {
      Log ( "PoolSection: Unable to allocate memory for pool section (entrysize %i x count %i -> %i bytes.", EntrySize, sectioncount, TotalSize ) ;
      exit ( 1 ) ;
   } /* endif */

   // Initialize the section.
   Clear ( ) ;

   #ifdef DEBUG
      Log ( "PoolSection(%08X)::PoolSection(objectsize=%i,sectioncount=%i) Allocated and initialized.  Memory %08X.", this, objectsize, sectioncount, Query_Memory() ) ;    
   #endif // DEBUG

} /* endmethod */

PoolSection::~PoolSection ( ) {

   if ( Memory ) free ( Memory ) ;

} /* endmethod */

void PoolSection::Clear ( PoolSection *NextSection ) {

   int Count = TotalSize / EntrySize ;

   for ( int i=0; i<Count; i++ ) {
      PoolEntry *ThisEntry = (PoolEntry*)               ( Memory +  i    * EntrySize ) ;
      PoolEntry *NextEntry = (PoolEntry*) ( i<Count-1 ? ( Memory + (i+1) * EntrySize ) : ( NextSection?NextSection->Query_Memory():0 ) ) ;
      ThisEntry->Set_Prev ( 0 ) ;
      ThisEntry->Set_Next ( NextEntry ) ;
   } /* endfor */

} /* endmethod */

Pool::Pool ( char *tag, int objectsize, int sectioncount ) : 
   Tag(tag), ObjectSize(objectsize), SectionCount(sectioncount), Count(0), FreeCount(0),
   First(0), Last(0), Free(0), FirstSection(0), LastSection(0) {

   #ifdef DEBUG
      Log ( "PoolSection(%08X,'%s')::PoolSection(objectsize=%i,sectioncount=%i) Created.", this, Tag, objectsize, sectioncount ) ;
   #endif // DEBUG

} /* endmethod */

Pool::Pool ( Pool &Original ) : 
   Tag(Original.Tag), ObjectSize(Original.ObjectSize), SectionCount(Original.SectionCount), Count(0), FreeCount(0),
   First(0), Last(0), Free(0), FirstSection(0), LastSection(0) {

   PoolEntry *PrevEntry = 0 ;
   PoolEntry *Entry = Original.First ;
   while ( Entry ) {
      PrevEntry = Insert ( PrevEntry, Entry->Data() ) ;
      Entry = Entry->Query_Next ( ) ;
   } /* endwhile */

} /* endmethod */

Pool::~Pool ( ) { 

   while ( FirstSection ) {
      PoolSection *NextSection = FirstSection->Query_Next() ;
      delete FirstSection ;
      FirstSection = NextSection ;
   } /* endwhile */

   #ifdef DEBUG
      Log ( "PoolSection(%08X,'%s')::~PoolSection() Destroyed.", this, Tag ) ;    
   #endif // DEBUG

} /* endmethod */

Pool & Pool::operator= ( const Pool & Original ) {

   while ( FirstSection ) {
      PoolSection *NextSection = FirstSection->Query_Next() ;
      delete FirstSection ;
      FirstSection = NextSection ;
   } /* endwhile */

   Tag = Original.Tag ;
   ObjectSize = Original.ObjectSize ;
   SectionCount = Original.SectionCount ;
   Count = FreeCount = 0 ;
   First = Last = Free = 0 ;
   FirstSection = LastSection = 0 ;

   PoolEntry *PrevEntry = 0 ;
   PoolEntry *Entry = Original.First ;
   while ( Entry ) {
      PrevEntry = Insert ( PrevEntry, Entry->Data() ) ;
      Entry = Entry->Query_Next ( ) ;
   } /* endwhile */

   return ( *this ) ;

} /* endmethod */

PoolEntry *Pool::Find ( int Index ) const {
   PoolEntry *Entry = First ;
   while ( Entry && Index-- ) {
      Entry = Entry->Query_Next() ;
   } /* endwhile */
   return ( Entry ) ;
} /* endmethod */

PoolEntry *Pool::Find ( PoolEntry *entry ) const {
   PoolEntry *Entry = First ;
   while ( Entry && ( Entry != entry ) ) {
      Entry = Entry->Query_Next() ;
   } /* endwhile */
   return ( Entry ) ;
} /* endmethod */

PoolEntry *Pool::Prev ( int Index ) const {
   PoolEntry *Entry = (PoolEntry*) Find ( Index ) ;
   if ( Entry ) 
      Entry = Entry->Query_Prev() ;
   return ( Entry ) ;
} /* endmethod */

PoolEntry *Pool::Prev ( PoolEntry *entry ) const {
   return ( entry->Query_Prev() ) ;
} /* endmethod */

PoolEntry *Pool::Next ( int Index ) const {
   PoolEntry *Entry = Find ( Index ) ;
   if ( Entry ) 
      Entry = Entry->Query_Next() ;
   return ( Entry ) ;
} /* endmethod */

PoolEntry *Pool::Next ( PoolEntry *entry ) const {
   return ( entry->Query_Next() ) ;
} /* endmethod */

PoolEntry *Pool::Insert ( PoolEntry *predecessor, void *object ) {

   #ifdef DEBUG
      Log ( "Pool(%08X,'%s')::Insert(predecessor=%08X,object=%08X) Started.", this, Tag, predecessor, object ) ;    
   #endif // DEBUG

   // If the list is full, expand it.
   if ( !Free ) {
      PoolSection *NewSection = new PoolSection ( ObjectSize, SectionCount ) ;
      NewSection->Set_Prev ( LastSection ) ;
      NewSection->Set_Next ( 0 ) ;
      if ( LastSection ) {
         LastSection->Set_Next ( NewSection ) ;
      } else {
         FirstSection = NewSection ;
      } /* endif */
      LastSection = NewSection ;
      Free = (PoolEntry*) NewSection->Query_Memory ( ) ;
      FreeCount += SectionCount ;
   } /* endif */

   // If a predecessor is named, and found, the object will follow it.
   if ( predecessor ) {
      PoolEntry *Predecessor = Find ( predecessor ) ;
      if ( Predecessor ) {
         PoolEntry *NewEntry = Free ;
         Free = NewEntry->Query_Next ( ) ;
         NewEntry->Set_Prev ( Predecessor ) ;
         NewEntry->Set_Next ( Predecessor->Query_Next() ) ;
         if ( Predecessor->Query_Next() ) {
            Predecessor->Query_Next()->Set_Prev ( NewEntry ) ;
         } else {
            Last = NewEntry ;
         } /* endif */
         Predecessor->Set_Next ( NewEntry ) ;
         memcpy ( NewEntry->Data(), object, ObjectSize ) ;
         #ifdef DEBUG
            Log ( "Pool(%08X,'%s')::Insert() Done.  Item inserted after predecessor.", this, Tag ) ;    
            Dump ( 0 ) ;
         #endif // DEBUG
         FreeCount -- ; Count ++ ;
         return ( NewEntry ) ;
      } /* endif */

   // Else, the object will be inserted at the start of the list.
   } else {
      PoolEntry *NewEntry = Free ;
      Free = NewEntry->Query_Next ( ) ;
      NewEntry->Set_Prev ( 0 ) ;
      NewEntry->Set_Next ( First ) ;
      if ( First ) {
         First->Set_Prev ( NewEntry ) ;
      } else {
         Last = NewEntry ;
      } /* endif */
      First = NewEntry ;
      memcpy ( NewEntry->Data(), object, ObjectSize ) ;
      #ifdef DEBUG
         Log ( "Pool(%08X,'%s')::Insert() Done.  Item inserted at start of list.", this, Tag ) ;    
         Dump ( 0 ) ;
      #endif // DEBUG
      FreeCount -- ; Count ++ ;
      return ( NewEntry ) ;
   } /* endif */

   #ifdef DEBUG
      Log ( "Pool(%08X,'%s')::Insert() Done.  Nothing inserted.", this, Tag ) ;    
   #endif // DEBUG
   return ( 0 ) ;

} /* endmethod */

void Pool::Delete ( PoolEntry *entry ) {
   PoolEntry *Entry = (PoolEntry*) Find ( entry ) ;
   if ( Entry ) {
      if ( Entry->Query_Prev() ) {
         Entry->Query_Prev()->Set_Next ( Entry->Query_Next() ) ;
      } else {
         First = Entry->Query_Next() ;
      } /* endif */
      if ( Entry->Query_Next() ) {
         Entry->Query_Next()->Set_Prev ( Entry->Query_Prev() ) ;
      } else {
         Last = Entry->Query_Prev() ;
      } /* endif */
      Entry->Set_Prev ( 0 ) ;
      Entry->Set_Next ( Free ) ;
      Free = Entry ;
      FreeCount ++ ; Count -- ;
   } /* endif */
} /* endmethod */

void Pool::Clear ( ) {
   Count = FreeCount = 0 ;
   First = Last = 0 ;
   Free = FirstSection ? (PoolEntry*) FirstSection->Query_Memory() : 0 ;
   PoolSection *Section = FirstSection ;
   while ( Section ) {
      FreeCount += SectionCount ;
      Section->Clear ( Section->Query_Next() ) ;
      Section = Section->Query_Next() ;
   } /* endwhile */
} /* endmethod */

void Pool::Dump ( int indent ) {
   Log ( "%*sPool(%08X,'%s')::Dump", indent, "", this, Tag ) ;
   Log ( "%*s  ObjectSize %i, SectionCount %i.", indent, "", ObjectSize, SectionCount ) ;
   Log ( "%*s  Count %i, FreeCount %i.", indent, "", Count, FreeCount ) ;
   Log ( "%*s  Section list - First %08X, Last %08X.", indent, "", FirstSection, LastSection ) ;
   PoolSection *Section = FirstSection ;
   while ( Section ) {
      Log ( "%*s    Section %08X with memory at %08X.", indent, "", Section, Section->Query_Memory() ) ;
      Section = Section->Query_Next() ;
   } /* endwhile */
   Log ( "%*s  Entry list - First %08X, Last %08X.", indent, "", First, Last ) ;
   PoolEntry *Entry = First ;
   while ( Entry ) {
      Log ( "%*s    Entry %08X (Prev %08X, Next %08X)", indent, "", Entry, Entry->Query_Prev(), Entry->Query_Next() ) ;
      DumpMemory ( "", Entry->Data(), ObjectSize, indent+6 ) ;
      Entry = Entry->Query_Next() ;
   } /* endwhile */
   Log ( "%*s  Free pool - First %08X.", indent, "", Free ) ;
   Entry = Free ;
   while ( Entry ) {
      Log ( "%*s    Free Entry %08X (Prev %08X, Next %08X)", indent, "", Entry, Entry->Query_Prev(), Entry->Query_Next() ) ;
      DumpMemory ( "", Entry->Data(), ObjectSize, indent+6 ) ;
      Entry = Entry->Query_Next() ;
   } /* endwhile */
} /* endmethod */

