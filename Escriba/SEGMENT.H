/****************************************************************** SEGMENT.H
 *                                                                          *
 *                           Class: Line Segment                            *
 *                                                                          *
 ****************************************************************************/

#ifndef SEGMENT_H
#define SEGMENT_H

#include "WorkPage.h"

class Segment {

   private:
      Segment *Next ;
      RECTL Rectangle ;
      Palabra *pFirst, *pLast ;
      int WordNumber ;
      void SetNext ( Segment *next ) { Next = next ; }

   public:
      Segment ( RECTL & rectangle ) : Next(0), Rectangle(rectangle), pFirst(0), pLast(0) { }

      ~Segment ( ) {
         if ( Next ) {
            delete Next ;
         } /* endif */
      } /* endmethod */

      void SetFirst ( Palabra *pWord, int wordnumber ) { pFirst = pWord ; WordNumber = wordnumber ; }
      void SetLast ( Palabra *pWord ) { pLast = pWord ; }

      Segment *QueryNext ( ) { return ( Next ) ; }
      PRECTL QueryRectangle ( ) { return ( & Rectangle ) ; }

      int Load ( WorkSpace *PS, Text *pText, Borradora &Page, Linea &Line, Palabra* &pWord, 
         Estado &EffectiveState, Estado &WorkingState, POINTL &PrinterResolution, int Paragraph, 
         int NumberOfPages, time_t FileDate ) ;

      void Justify ( Borradora *Page, WorkSpace *PS, Estado &State, int PageNumber, int NumberOfPages, time_t FileDate ) ;

      void Split ( RECTL & Cell ) ;
} ;

#endif // SEGMENT_H

