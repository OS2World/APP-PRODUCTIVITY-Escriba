/***************************************************************** TXTBLOCK.H
 *                                                                          *
 *                  Text Block Class (part of Text Class)                   *
 *                                                                          *
 ****************************************************************************/

#ifndef TXTBLOCK_H
#define TXTBLOCK_H

#include "System.h"

class TextBlock {

   private:

      TextBlock *Prev, *Next ;
      unsigned short InsertLevel, DeleteLevel ;
      char *Data ;
      long Size ;

   public:

      TextBlock ( char *data, long size, short Level ) ;
      TextBlock ( TextBlock & Original ) ;
      ~TextBlock ( ) ;

      void SetPrev ( class TextBlock *prev ) { Prev = prev ; }
      void SetNext ( class TextBlock *next ) { Next = next ; }
      void SetDeleteLevel ( unsigned short Level ) { DeleteLevel = Level ; }

      TextBlock     *QueryPrev        ( ) { return ( Prev ) ; }
      TextBlock     *QueryNext        ( ) { return ( Next ) ; }
      unsigned short QueryInsertLevel ( ) { return ( InsertLevel ) ; }
      unsigned short QueryDeleteLevel ( ) { return ( DeleteLevel ) ; }
      char          *QueryData        ( ) { if ( Size > 4 ) return ( Data ) ; else return ( (char*)&Data ) ; }
      long           QuerySize        ( ) { return ( Size ) ; }

      int IsPresent ( short Level, BOOL Editing ) ;
      void Resize ( long size ) ;
      void Dump ( int indent ) ;
} ;

#endif
