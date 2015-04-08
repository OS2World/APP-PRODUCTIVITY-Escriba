/********************************************************************* TEXT.H
 *                                                                          *
 *                     Text Class (part of Document)                        *
 *                                                                          *
 ****************************************************************************/

#ifndef TEXT_H
#define TEXT_H

#include "System.h"

#include "TxtBlock.h"
#include "WrkSpace.h"

#define MAX_UNDO (0x10000)

class Text {
   private:
      TextBlock *First ;        // The first TextBlock in the Text.
      TextBlock *Last ;         // The last TextBlock in the Text.
      TextBlock *Current ;      // The current TextBlock.
      long CurrentPosition ;    // The current position in bytes from the start of the Text.
      long CurrentOffset ;      // The offset of the current TextBlock in bytes from the start.
      short CurrentLevel ;      // The current change level.
      short MaxLevel ;          // The maximum change level in the Undo stack.
      long *UndoList ;          // The cursor offsets at each change level.
      BOOL Editing ;            // Flag: Are we editing right now?
      BOOL Scanning ;           // Flag: Are we scanning all data at this time?
      void Cleanup ( ) ;

   public:
      Text ( char *Data, long Size ) ;
      Text ( Text & Original ) ;
      ~Text ( ) ;

      void SetPosition ( long Offset ) ;
      BOOL QueryBytePtr ( long Offset, PUCHAR &Pointer ) ;
      long Read ( long Offset, long Size, char *Data ) ;
      void CommenceLevel ( ) ;
      void CompleteLevel ( long OffsetToSave ) ;
      void Insert ( long Offset, char *Data, long Size ) ;
      void Delete ( long Offset, long Size ) ;
      int Undo ( ) ;
      int Redo ( ) ;

      void Set_Scanning ( BOOL flag ) { Scanning = flag ; }

      long  QueryCurrentPosition ( ) { return ( CurrentPosition ) ; }
      short QueryCurrentLevel    ( ) { return ( CurrentLevel ) ; }
      short QueryMaxLevel        ( ) { return ( MaxLevel ) ; }

      void QueryChangeRange ( int Level, long &Start, long &End ) ;
      long QueryCurrentSize ( ) ;

      long PrevToken ( WorkSpace *PS, long Offset ) ;
      long NextToken ( WorkSpace *PS, long Offset ) ;

      void Dump ( int indent ) ;
} ;


#endif
