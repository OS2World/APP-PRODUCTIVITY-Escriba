/******************************************************************* TEXT.CPP
 *                                                                          *
 *                         Text Class (part of Document)                    *
 *                                                                          *
 ****************************************************************************/

#include "System.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Debug.h"
#include "DocSup.h"
#include "Document.h"
#include "Support.h"
#include "Text.h"

// #define DEBUG
// #define DEBUG_DELETE
// #define DEBUG_INSERT
// #define DEBUG_POSITION

#ifdef DEBUG
   #define DEBUG_DELETE
   #define DEBUG_INSERT
   #define DEBUG_POSITION
#endif

 
/****************************************************************************
 *                                                                          *
 *  Text: Constructor from memory data                                      *
 *                                                                          *
 ****************************************************************************/

Text::Text ( char *Data, long Size ) :
   First(0), Last(0), Current(0), CurrentPosition(0), CurrentOffset(0),
   CurrentLevel(0), MaxLevel(0), Editing(FALSE), Scanning(FALSE) {

   #ifdef DEBUG
      Log ( "Text(%08X)::Text(Data=%08X,Size=%i) Started.", this, Data, Size ) ;
   #endif // DEBUG

   UndoList = new long [ MAX_UNDO ] ;

   if ( Size ) {

      First = Last = Current = new TextBlock ( Data, Size, CurrentLevel ) ;

   } /* endif */

   #ifdef DEBUG
      Log ( "Text(%08X)::Text(Data=%08X,Size=%i) Done.", this, Data, Size ) ;
   #endif // DEBUG
}
 
/****************************************************************************
 *                                                                          *
 *  Text: Copy Constructor                                                  *
 *                                                                          *
 ****************************************************************************/

Text::Text ( Text & Original ) :
   First(0),
   Last(0),
   Current(0),
   CurrentPosition(Original.CurrentPosition),
   CurrentOffset(Original.CurrentOffset),
   CurrentLevel(Original.CurrentLevel),
   MaxLevel(Original.MaxLevel),
   Editing(Original.Editing), 
   Scanning(Original.Scanning) {

   #ifdef DEBUG
      Log ( "Text(%08X)::Text(Original=%08X) Started.", this, &Original ) ;
   #endif // DEBUG

   UndoList = new long [ MAX_UNDO ] ;

   TextBlock *Block = Original.First ;
   while ( Block ) {
      TextBlock *NewBlock = new TextBlock ( *Block ) ;
      if ( Last ) {
         Last->SetNext ( NewBlock ) ;
      } else {
         First = NewBlock ;
      } /* endif */
      NewBlock->SetPrev ( Last ) ;
      NewBlock->SetNext ( NULL ) ;
      Last = NewBlock ;
      if ( Block == Original.Current ) {
         Current = NewBlock ;
      } /* endif */
      Block = Block->QueryNext() ;
   } /* endwhile */

   #ifdef DEBUG
      Log ( "Text(%08X)::Text(Original=%08X) Done.", this, &Original ) ;
   #endif // DEBUG
}
 
/****************************************************************************
 *                                                                          *
 *  Text: Destructor                                                        *
 *                                                                          *
 ****************************************************************************/

Text::~Text ( ) {
   while ( First ) {
      TextBlock *Next = First->QueryNext() ;
      delete First ;
      First = Next ;
   } /* endwhile */
   delete [] UndoList ;
}
 
/****************************************************************************
 *                                                                          *
 *  Text: Clean all changes past current level out of structure             *
 *                                                                          *
 ****************************************************************************/

void Text::Cleanup ( ) {
   TextBlock *Block = First ;
   while ( Block ) {
      TextBlock *Prev = Block->QueryPrev() ;
      TextBlock *Next = Block->QueryNext() ;
      if ( Block->QueryInsertLevel() > CurrentLevel ) {
         if ( Prev ) {
            Prev->SetNext ( Next ) ;
         } else {
            First = Next ;
         } /* endif */
         if ( Next ) {
            Next->SetPrev ( Prev ) ;
         } else {
            Last = Prev ;
         } /* endif */
         delete Block ;
      } else if ( Block->QueryDeleteLevel() > CurrentLevel ) {
         Block->SetDeleteLevel ( 0 ) ;
      } /* endif */
      Block = Next ;
   } /* endwhile */
   MaxLevel = CurrentLevel ;
}
 
/****************************************************************************
 *                                                                          *
 *  Text: Set absolute position                                             *
 *    Alters CurrentPosition, Current and CurrentOffset.                    *
 *                                                                          *
 ****************************************************************************/

void Text::SetPosition ( long Offset ) {

   #ifdef DEBUG_POSITION
      Log ( "Text::SetPosition(%i) started.  CurrentPosition, Current, CurrentOffset: %i, %p, %i.",
         Offset, CurrentPosition, Current, CurrentOffset ) ;
   #endif

   // If positioning backwards from the current position,
   if ( Offset AND ( Offset < CurrentPosition ) ) {

      // Set the new current position.
      CurrentPosition = Offset ;
      #ifdef DEBUG_POSITION
         Log ( "Text::SetPosition(%i): CurrentPosition now: %i.", Offset, CurrentPosition ) ;
      #endif

      // While there are blocks to inspect . . .
      TextBlock *pBlock = Current ;
      while ( pBlock ) {
         if ( Scanning OR pBlock->IsPresent ( CurrentLevel, Editing ) ) {
            if ( CurrentPosition >= CurrentOffset ) {
               #ifdef DEBUG_POSITION
                  Log ( "Text::SetPosition(%i) done.  CurrentPosition, Current, CurrentOffset: %i, %p, %i.",
                     Offset, CurrentPosition, Current, CurrentOffset ) ;
               #endif
               return ;
            } /* endif */
         } /* endif */
         pBlock = pBlock->QueryPrev() ;
         if ( pBlock ) {
            if ( Scanning OR pBlock->IsPresent ( CurrentLevel, Editing ) ) {
               Current = pBlock ;
               #ifdef DEBUG_POSITION
                  Log ( "Text::SetPosition(%i): Current now: %p.", Offset, Current ) ;
               #endif
               CurrentOffset -= Current->QuerySize() ;
               #ifdef DEBUG_POSITION
                  Log ( "Text::SetPosition(%i): CurrentOffset now: %i.", Offset, CurrentOffset ) ;
               #endif
            } /* endif */
         } /* endif */
      } /* endwhile */

   // Else positioning forwards from the current position.
   } else {

      // Set the new current position.
      CurrentPosition = Offset ;
      #ifdef DEBUG_POSITION
         Log ( "Text::SetPosition(%i): CurrentPosition now: %i.", Offset, CurrentPosition ) ;
      #endif
 
      // If positioning to origin, reset current position entirely.
      if ( Offset == 0 ) {
         Current = First ;
         #ifdef DEBUG_POSITION
            Log ( "Text::SetPosition(%i): Current now: %p.", Offset, Current ) ;
         #endif
         CurrentOffset = 0 ;
         #ifdef DEBUG_POSITION
            Log ( "Text::SetPosition(%i): CurrentOffset now: %i.", Offset, CurrentOffset ) ;
         #endif
      } /* endif */

      // While there are blocks to inspect . . .
      TextBlock *Next = Current ;
      while ( Next ) {

         // If this block is present . . .
         if ( Scanning OR Next->IsPresent ( CurrentLevel, Editing ) ) {

            // Set new current block, as this might be it.  The previous sure wasn't.
            Current = Next ;
            #ifdef DEBUG_POSITION
               Log ( "Text::SetPosition(%i): Current now: %p.", Offset, Current ) ;
            #endif

            // If current position is before the end of this block, we've found the block.
            if ( CurrentPosition < CurrentOffset + Current->QuerySize() ) {
               #ifdef DEBUG_POSITION
                  Log ( "Text::SetPosition(%i) done.  CurrentPosition, Current, CurrentOffset: %i, %p, %i.",
                     Offset, CurrentPosition, Current, CurrentOffset ) ;
               #endif
               return ;
            } /* endif */

            // Otherwise, add the size of this block to the current block offset.
            if ( Current->QueryNext() ) {
               CurrentOffset += Current->QuerySize() ;
               #ifdef DEBUG_POSITION
                  Log ( "Text::SetPosition(%i): CurrentOffset now: %i.", Offset, CurrentOffset ) ;
               #endif
            } /* endif */

         } /* endif */

         // Move on to the next block, whether it be usable or not at this level.
         Next = Next->QueryNext() ;

      } /* endwhile */

   } /* endif */

   #ifdef DEBUG_POSITION
      Log ( "Text::SetPosition(%i) done.  CurrentPosition, Current, CurrentOffset: %i, %p, %i.",
         Offset, CurrentPosition, Current, CurrentOffset ) ;
   #endif
}
 
/****************************************************************************
 *                                                                          *
 *  Text: Set Current Position and return Pointer to the token there.       *
 *    Returns FALSE if past end of file, TRUE if all's well.                *
 *    Alters CurrentPosition, Current and CurrentOffset.                    *
 *                                                                          *
 ****************************************************************************/

BOOL Text::QueryBytePtr ( long Offset, PUCHAR &Pointer ) {

   #ifdef DEBUG_POSITION
      Log ( "Text::QueryBytePtr(%i,%p) Started.", Offset, Pointer ) ;
   #endif

   SetPosition ( Offset ) ;

   if ( CurrentPosition >= CurrentOffset + Current->QuerySize() ) {
      #ifdef DEBUG_POSITION
         Log ( "Text::QueryBytePtr(%i,%p) Done EOF.", Offset, Pointer ) ;
      #endif
      return ( 0 ) ;
   } /* endif */

   Pointer = PUCHAR ( Current->QueryData() + CurrentPosition - CurrentOffset ) ;
   #ifdef DEBUG_POSITION
      Log ( "Text::QueryBytePtr(%i,%p) Done.", Offset, Pointer ) ;
   #endif
   return ( 1 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *  Text: Read Data from Text to Memory.                                    *
 *    Returns number of bytes read (0=EOF, NZ=OK).                          *
 *    Alters CurrentPosition, Current and CurrentOffset.                    *
 *                                                                          *
 ****************************************************************************/

long Text::Read ( long Offset, long Size, char *Data ) {

   // Position for the read.
   SetPosition ( Offset ) ;

   // Reset result to zero.
   long Copied = 0 ;

   // While there is still data left to copy . . .
   while ( Current && Size ) {

      // If the current block is present at the current change level, read it out.
      if ( Scanning OR Current->IsPresent ( CurrentLevel, Editing ) ) {
         long RemainingInBlock = Current->QuerySize() - ( CurrentPosition - CurrentOffset ) ;
         long AmountToCopy = min ( Size, RemainingInBlock ) ;
         memcpy ( Data, Current->QueryData()+CurrentPosition-CurrentOffset, size_t(AmountToCopy) ) ;
         Data   += AmountToCopy ;
         Copied += AmountToCopy ;
         Size   -= AmountToCopy ;
         CurrentPosition += AmountToCopy ;
      } /* endif */

      // Move to the next current block.
      TextBlock *Next = Current->QueryNext() ;
      if ( Next == 0 )
         break ;
      while ( Next AND ( CurrentPosition >= CurrentOffset + Current->QuerySize() ) ) {
         if ( Scanning OR Next->IsPresent ( CurrentLevel, Editing ) ) {
            CurrentOffset += Current->QuerySize() ;
            Current = Next ;
         } /* endif */
         Next = Next->QueryNext() ;
      } /* endwhile */

   } /* endwhile */

   // Return the number of bytes copied out.
   return ( Copied ) ;
}
 
/****************************************************************************
 *                                                                          *
 *  Text: Commence New Change Level                                         *
 *                                                                          *
 ****************************************************************************/

void Text::CommenceLevel ( ) {

   // Set edit mode.
   Editing = TRUE ;

   // Clean out all changes beyond the current level.
   Cleanup() ;
}
 
/****************************************************************************
 *                                                                          *
 *  Text: Complete Change Level                                             *
 *                                                                          *
 ****************************************************************************/

void Text::CompleteLevel ( long OffsetToSave ) {

   // Increase the current and maximum change levels and save the current position.
   UndoList[CurrentLevel++] = OffsetToSave ;
   MaxLevel ++ ;

   // Reset edit mode.
   Editing = FALSE ;
}
 
/****************************************************************************
 *                                                                          *
 *  Text: Insert Data                                                       *
 *                                                                          *
 ****************************************************************************/

void Text::Insert ( long Offset, char *Data, long Size ) {

   #ifdef DEBUG_INSERT
      Log ( "Text(%08X)::Insert(Offset=%i,Data=%08X,Size=%i) Started.  Finding block to split/append_to.", this, Offset, Data, Size ) ;
   #endif // DEBUG_INSERT

   // Find out what existing block is going to be split and/or appended to.
   TextBlock *Block = First ;
   TextBlock *LastBlock = 0 ;
   long BlockOffset = 0 ;
   while ( Block ) {
      if ( Block->IsPresent ( CurrentLevel, Editing ) ) {
         if ( Offset < BlockOffset+Block->QuerySize() )
            break ;
         LastBlock = Block ;
         BlockOffset += Block->QuerySize() ;
      } /* endif */
      Block = Block->QueryNext() ;
   } /* endwhile */

   // Split the block if necessary.
   if ( Block && ( Offset > BlockOffset ) && ( Offset < BlockOffset+Block->QuerySize() ) ) {
      #ifdef DEBUG_INSERT
         Log ( "Text(%08X)::Insert(Offset=%i,Data=%08X,Size=%i) Splitting block %08X.", this, Offset, Data, Size ) ;
      #endif // DEBUG_INSERT
      TextBlock *NewBlock = new TextBlock ( Block->QueryData()+Offset-BlockOffset,
         Block->QuerySize()-(Offset-BlockOffset), Block->QueryInsertLevel() ) ;
      Block->Resize ( Offset-BlockOffset ) ;
      NewBlock->SetPrev ( Block ) ;
      NewBlock->SetNext ( Block->QueryNext() ) ;
      if ( Block->QueryNext() ) {
         Block->QueryNext()->SetPrev ( NewBlock ) ;
      } else {
         Last = NewBlock ;
      } /* endif */
      Block->SetNext ( NewBlock ) ;
      LastBlock = Block ;
      Block = NewBlock ;
   } /* endif */

   // Create the new block to be inserted.
   #ifdef DEBUG_INSERT
      Log ( "Text(%08X)::Insert(Offset=%i,Data=%08X,Size=%i) Creating new block.", this, Offset, Data, Size ) ;
   #endif // DEBUG_INSERT
   TextBlock *NewBlock = new TextBlock ( Data, Size, short(CurrentLevel+1) ) ;

   // Append the new block after its predecessor block.
   #ifdef DEBUG_INSERT
      Log ( "Text(%08X)::Insert(Offset=%i,Data=%08X,Size=%i) Appending new block %08X to predecessor %08X.", this, Offset, Data, Size, NewBlock, LastBlock ) ;
   #endif // DEBUG_INSERT
   NewBlock->SetPrev ( LastBlock ) ;
   NewBlock->SetNext ( LastBlock ? LastBlock->QueryNext() : First ) ;
   if ( LastBlock ) {
      if ( LastBlock->QueryNext() )
         LastBlock->QueryNext()->SetPrev ( NewBlock ) ;
      else
         Last = NewBlock ;
      LastBlock->SetNext ( NewBlock ) ;
   } else {
      if ( First )
         First->SetPrev ( NewBlock ) ;
      else
         Last = NewBlock ;
      First = NewBlock ;
   } /* endif */

   // Reset Current Position
   #ifdef DEBUG_INSERT
      Log ( "Text(%08X)::Insert(Offset=%i,Data=%08X,Size=%i) Resetting current position.", this, Offset, Data, Size ) ;
   #endif // DEBUG_INSERT
   CurrentPosition = 0 ;  Current = First ;  CurrentOffset = 0 ;

   #ifdef DEBUG_INSERT
      Log ( "Text(%08X)::Insert(Offset=%i,Data=%08X,Size=%i) Done.", this, Offset, Data, Size ) ;
   #endif // DEBUG_INSERT
}
 
/****************************************************************************
 *                                                                          *
 *  Text: Remove Data                                                       *
 *                                                                          *
 ****************************************************************************/

void Text::Delete ( long Offset, long Size ) {

   #ifdef DEBUG_DELETE
      Log ( "Text::Delete(Offset=%i,Size=%i) Started.", Offset, Size ) ;
   #endif

   // Scan the text object, deleting those blocks that fall in the area specified.
   long BlockOffset = 0 ;
   TextBlock *Block = First ;
   while ( Block AND ( BlockOffset < Offset+Size ) ) {
      #ifdef DEBUG_DELETE
         Log ( "Text::Delete(Offset=%i,Size=%i) Checking block %08X (offset %i, size %i).", Offset, Size, Block, BlockOffset, Block->QuerySize() ) ;
      #endif
      if ( Block->IsPresent ( CurrentLevel, Editing ) ) {
         #ifdef DEBUG_DELETE
            Log ( "Text::Delete(Offset=%i,Size=%i) Block is present.", Offset, Size ) ;
         #endif
         if ( Offset >= BlockOffset+Block->QuerySize() ) {
            // Do nothing.  The block is before the area.
            #ifdef DEBUG_DELETE
               Log ( "Text::Delete(Offset=%i,Size=%i) Block before area being deleted.  Nothing done.", Offset, Size ) ;
            #endif

         } else if ( ( Offset <= BlockOffset ) AND ( Offset+Size < BlockOffset+Block->QuerySize() ) ) {
            // Remove the head from this block.
            #ifdef DEBUG_DELETE
               Log ( "Text::Delete(Offset=%i,Size=%i) Removing head of block.  Creating new block with size %i.", Offset, Size, Block->QuerySize()-(Offset+Size-BlockOffset) ) ;
            #endif
            TextBlock *NewBlock = new TextBlock ( Block->QueryData()+Offset+Size-BlockOffset,
               Block->QuerySize()-(Offset+Size-BlockOffset), Block->QueryInsertLevel() ) ;
            #ifdef DEBUG_DELETE
               Log ( "Text::Delete(Offset=%i,Size=%i) Reducing size of original block to %i.", Offset, Size, Offset+Size-BlockOffset ) ;
            #endif
            Block->Resize ( Offset+Size-BlockOffset ) ;
            #ifdef DEBUG_DELETE
               Log ( "Text::Delete(Offset=%i,Size=%i) Patching block chain.", Offset, Size, Offset+Size-BlockOffset ) ;
            #endif
            NewBlock->SetPrev ( Block ) ;
            NewBlock->SetNext ( Block->QueryNext() ) ;
            if ( Block->QueryNext() ) {
               Block->QueryNext()->SetPrev ( NewBlock ) ;
            } else {
               Last = NewBlock ;
            } /* endif */
            Block->SetNext ( NewBlock ) ;
            #ifdef DEBUG_DELETE
               Log ( "Text::Delete(Offset=%i,Size=%i) Setting original block delete level.", Offset, Size ) ;
            #endif
            Block->SetDeleteLevel ( short(CurrentLevel+1) ) ;

         } else if ( ( Offset > BlockOffset ) AND ( Offset+Size < BlockOffset+Block->QuerySize() ) ) {
            // Slice chunk out of middle of block.
            TextBlock *NewBlock = new TextBlock ( Block->QueryData()+Offset+Size-BlockOffset,
               Block->QuerySize()-(Offset+Size-BlockOffset), Block->QueryInsertLevel() ) ;
            Block->Resize ( Offset+Size-BlockOffset ) ;
            NewBlock->SetPrev ( Block ) ;
            NewBlock->SetNext ( Block->QueryNext() ) ;
            if ( Block->QueryNext() ) {
               Block->QueryNext()->SetPrev ( NewBlock ) ;
            } else {
               Last = NewBlock ;
            } /* endif */
            Block->SetNext ( NewBlock ) ;
            NewBlock = new TextBlock ( Block->QueryData()+Offset-BlockOffset,
               Block->QuerySize()-(Offset-BlockOffset), Block->QueryInsertLevel() ) ;
            Block->Resize ( Offset-BlockOffset ) ;
            NewBlock->SetPrev ( Block ) ;
            NewBlock->SetNext ( Block->QueryNext() ) ;
            if ( Block->QueryNext() ) {
               Block->QueryNext()->SetPrev ( NewBlock ) ;
            } else {
               Last = NewBlock ;
            } /* endif */
            Block->SetNext ( NewBlock ) ;

            BlockOffset += Block->QuerySize() ;
            Block = Block->QueryNext() ;
            Block->SetDeleteLevel ( short(CurrentLevel+1) ) ;

         } else if ( Offset > BlockOffset ) {
            // Remove the tail from this block.
            TextBlock *NewBlock = new TextBlock ( Block->QueryData()+Offset-BlockOffset,
               Block->QuerySize()-(Offset-BlockOffset), Block->QueryInsertLevel() ) ;
            Block->Resize ( Offset-BlockOffset ) ;
            NewBlock->SetPrev ( Block ) ;
            NewBlock->SetNext ( Block->QueryNext() ) ;
            if ( Block->QueryNext() ) {
               Block->QueryNext()->SetPrev ( NewBlock ) ;
            } else {
               Last = NewBlock ;
            } /* endif */
            Block->SetNext ( NewBlock ) ;

            BlockOffset += Block->QuerySize() ;
            Block = Block->QueryNext() ;
            Block->SetDeleteLevel ( short(CurrentLevel+1) ) ;

         } else {
            // Remove entire block.
            Block->SetDeleteLevel ( short(CurrentLevel+1) ) ;

         } /* endif */

         BlockOffset += Block->QuerySize() ;

      } /* endif */
      Block = Block->QueryNext() ;
   } /* endwhile */

   // Reset Current Position
   CurrentPosition = 0 ;  Current = First ;  CurrentOffset = 0 ;

   #ifdef DEBUG_DELETE
      Log ( "Text::Delete(Offset=%i,Size=%i) Done.", Offset, Size ) ;
   #endif
}
 
/****************************************************************************
 *                                                                          *
 *  Text: Undo Change                                                       *
 *                                                                          *
 ****************************************************************************/

int Text::Undo ( ) {
   if ( CurrentLevel == 0 ) {
      return ( 1 ) ;
   } /* endif */
   CurrentPosition = 0 ;  Current = First ;  CurrentOffset = 0 ;
   SetPosition ( UndoList[--CurrentLevel] ) ;
   return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *  Text: Redo Change                                                       *
 *                                                                          *
 ****************************************************************************/

int Text::Redo ( ) {
   if ( CurrentLevel == MaxLevel ) {
      return ( 1 ) ;
   } /* endif */
   CurrentPosition = 0 ;  Current = First ;  CurrentOffset = 0 ;
   SetPosition ( UndoList[++CurrentLevel] ) ;
   return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *  Text: Determine Change Range (between current Undo level and the next)  *
 *                                                                          *
 ****************************************************************************/

void Text::QueryChangeRange ( int Level, long &Start, long &End ) {

   Start = 0x7FFFFFFF ;
   End = 0 ;
   long BlockOffset = 0 ;

   TextBlock *Block = First ;
   while ( Block ) {

      BOOL HereNow = Block->IsPresent ( short(Level), Editing ) ;
      BOOL HereLater = Block->IsPresent ( short(Level+1), Editing ) ;
      long BlockSize = Block->QuerySize() ;

      if ( HereNow != HereLater ) {
         Start = min ( Start, BlockOffset ) ;
         End = max ( End, BlockOffset+BlockSize-1 ) ;
      } /* endif */

      if ( HereNow ) {
         BlockOffset += BlockSize ;
      } /* endif */

      Block = Block->QueryNext() ;

   } /* endwhile */
}
 
/****************************************************************************
 *                                                                          *
 *  Text: Determine Current Size                                            *
 *                                                                          *
 ****************************************************************************/

long Text::QueryCurrentSize ( ) {
   long Size = 0 ;
   TextBlock *Block = First ;
   while ( Block ) {
      if ( Scanning OR Block->IsPresent ( CurrentLevel, Editing ) ) {
         Size += Block->QuerySize() ;
      } /* endif */
      Block = Block->QueryNext() ;
   } /* endwhile */
   #ifdef DEBUG
      Log ( "Text::QueryCurrentSize gives %i", Size ) ;
   #endif
   return ( Size ) ;
} /* endmethod */
 
/****************************************************************************
 *                                                                          *
 *  Text: Find Previous Token                                               *
 *                                                                          *
 ****************************************************************************/

long Text::PrevToken ( WorkSpace *PS, long Offset ) {

   // If at start already, just return w/o moving.
   if ( Offset == 0 ) 
      return ( Offset ) ;

   // Move back one byte.  If function trailer, skip to start of function and return offset.
   Offset -- ;
   PUCHAR pByte ;
   QueryBytePtr ( Offset, pByte ) ;
   if ( *pByte == 0 ) {
      Offset -= sizeof(TOKEN_BACKUP_TYPE) ;
      pByte -= sizeof(TOKEN_BACKUP_TYPE) ;
      Offset -= *((TOKEN_BACKUP_TYPE*)pByte) ;
      return ( Offset ) ;
   } /* endif */

   // If we aren't at start yet, check to see if previous byte is a DBCS header.
   //   If so, back up one more before byte.
   if ( Offset ) {
      QueryBytePtr ( Offset-1, pByte ) ;
      if ( PS->IsDBCSHeader ( *pByte ) )
         Offset -- ;
   } /* endif */

   // Return the final offset.
   return ( Offset ) ;

} /* endmethod */

/****************************************************************************
 *                                                                          *
 *  Text: Find Next Token                                                   *
 *                                                                          *
 ****************************************************************************/

long Text::NextToken ( WorkSpace *PS, long Offset ) {
   PUCHAR pByte ;
   QueryBytePtr ( Offset, pByte ) ;
   if ( NOT IsEndOfDocument ( *pByte ) ) {
      if ( IsFunction ( *pByte ) ) {
         long LengthIn ;
         ComputeFunctionSize ( pByte, LengthIn ) ;
         Offset += LengthIn ;
      } else if ( PS->IsDBCSHeader ( *pByte ) ) {
         Offset += 2 ;
      } else {
         Offset ++ ;
      } /* endif */
   } /* endif */
   return ( Offset ) ;
} /* endmethod */

/****************************************************************************
 *                                                                          *
 *  Text: Dump                                                              *
 *                                                                          *
 ****************************************************************************/

void Text::Dump ( int Indent ) {
   Log ( "%*sText(%p)", Indent, "", this ) ;
   Log ( "%*s  First, Last, Current: %p %p %p", Indent, "", First, Last, Current ) ;
   Log ( "%*s  CurrentPosition, CurrentOffset: %li %li", Indent, "", CurrentPosition, CurrentOffset ) ;
   Log ( "%*s  CurrentLevel, MaxLevel: %i %i", Indent, "", CurrentLevel, MaxLevel ) ;
   for ( int i=0; i<MaxLevel; i++ ) {
      Log ( "%*s  Position @ change level %i: %i", Indent, "", i, UndoList[i] ) ;
   } /* endfor */
   Log ( "%*s  CurrentSize: %i", Indent, "", QueryCurrentSize() ) ;
   TextBlock *Next = First ;
   while ( Next ) {
      Next->Dump(Indent+2) ;
      Next = Next->QueryNext() ;
   } /* endwhile */
} /* endmethod */

