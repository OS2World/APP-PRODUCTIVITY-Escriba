/*************************************************************** TXTBLOCK.CPP
 *                                                                          *
 *                   Text Block Class (for Text Class)                      *
 *                                                                          *
 ****************************************************************************/

#include "System.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Debug.h"
#include "Support.h"
#include "TxtBlock.h"

// #define DEBUG

 
/****************************************************************************
 *                                                                          *
 *  TextBlock: Constructor                                                  *
 *                                                                          *
 ****************************************************************************/

TextBlock::TextBlock ( char *data, long size, short Level ) :
   Prev(0), Next(0), InsertLevel(Level), DeleteLevel(0), Data(0), Size(size) {

   #ifdef DEBUG
      Log ( "TextBlock(%08X)::TextBlock(data=%08X,size=%i,Level=%i) Started.", this, data, size, Level ) ;
   #endif // DEBUG

   if ( Size > 4 ) {
      Data = (char*) malloc ( Size ) ;
      memcpy ( Data, data, Size ) ;
      #ifdef DEBUG
         Log ( "TextBlock(%08X)::TextBlock(data=%08X,size=%i,Level=%i) Allocated memory at %08X.", this, data, size, Level, Data ) ;
      #endif // DEBUG

   } else {
      memcpy ( &Data, data, Size ) ;

   } /* endif */

   #ifdef DEBUG
      Log ( "TextBlock(%08X)::TextBlock(data=%08X,size=%i,Level=%i) Done.", this, data, size, Level ) ;
   #endif // DEBUG
}

 
/****************************************************************************
 *                                                                          *
 *  TextBlock: Copy Constructor                                             *
 *                                                                          *
 ****************************************************************************/

TextBlock::TextBlock ( TextBlock & Original ) :
   Prev(0), Next(0), InsertLevel(Original.InsertLevel), DeleteLevel(Original.DeleteLevel), Data(0), Size(Original.Size) {

   #ifdef DEBUG
      Log ( "TextBlock(%08X)::TextBlock(Original=%08X) Started.  Size %i, Data %08X.", this, &Original, Original.Size, Original.Data ) ;
   #endif // DEBUG

   if ( Size > 4 ) {
      Data = (char*) malloc ( Size ) ;
      memcpy ( Data, Original.Data, size_t(Size) ) ;
      #ifdef DEBUG
         Log ( "TextBlock(%08X)::TextBlock(Original=%08X) Allocated and copyied data to %08X.", this, &Original, Data ) ;
      #endif // DEBUG

   } else {
      memcpy ( &Data, &Original.Data, size_t(Size) ) ;
      #ifdef DEBUG
         Log ( "TextBlock(%08X)::TextBlock(Original=%08X) Copyied data.", this, &Original ) ;
      #endif // DEBUG

   } /* endif */

   #ifdef DEBUG
      Log ( "TextBlock(%08X)::TextBlock(Original=%08X) Done.", this, &Original ) ;
   #endif // DEBUG
}


/****************************************************************************
 *                                                                          *
 *  TextBlock: Destructor                                                   *
 *                                                                          *
 ****************************************************************************/

TextBlock::~TextBlock ( ) { 

   #ifdef DEBUG
      Log ( "TextBlock(%08X)::~TextBlock() Started.  Size %i, Data %08X.", this, Size, Data ) ;
   #endif // DEBUG

   if ( Size > 4 ) {
      free ( Data ) ; 
      #ifdef DEBUG
         Log ( "TextBlock(%08X)::~TextBlock() Freed memory at %08X.", this, Data ) ;
      #endif // DEBUG
   } /* endif */

   #ifdef DEBUG
      Log ( "TextBlock(%08X)::~TextBlock() Done.", this ) ;
   #endif // DEBUG
}

 
/****************************************************************************
 *                                                                          *
 *  TextBlock: Is this block in use at this change level?                   *
 *                                                                          *
 ****************************************************************************/

int TextBlock::IsPresent ( short Level, BOOL Editing ) {
   if ( Editing ) {
      if ( DeleteLevel == 0 )
         return ( 1 ) ;
   } else {
      if ( DeleteLevel && ( Level >= DeleteLevel ) )
         return ( 0 ) ;
      else if ( Level >= InsertLevel )
         return ( 1 ) ;
   } /* endif */
   return ( 0 ) ;
}
 

/****************************************************************************
 *                                                                          *
 *  TextBlock: Resize (to make smaller)                                     *
 *                                                                          *
 ****************************************************************************/

void TextBlock::Resize ( long size ) {

   #ifdef DEBUG
      Log ( "TextBlock(%08X)::Resize(Size=%i) Started.  Old size %i.", this, size, Size ) ;
   #endif // DEBUG

   // If block was and remains malloced . . .
   if ( ( Size > 4 ) && ( size > 4 ) ) {
      #ifdef DEBUG
         Log ( "TextBlock(%08X)::Resize(Size=%i) Reallocated memory at %08X.", this, size, Data ) ;
      #endif // DEBUG
      if ( size < Size )
         Data = (char*) realloc ( Data, int(size) ) ;

   // Else if block was malloced and now will not be . . .
   } else if ( ( Size > 4 ) && ( size <= 4 ) ) {
      char *pData = Data ;
      memcpy ( &Data, pData, size_t(size) ) ;
      free ( pData ) ;
      #ifdef DEBUG
         Log ( "TextBlock(%08X)::Resize(Size=%i) Freed memory at %08X.", this, size, pData ) ;
      #endif // DEBUG

   // Else the block was not malloced and will remain that way.
   } else {
      // Do nothing.

   } /* endif */

   // Save the new size.
   Size = size ;

   #ifdef DEBUG
      Log ( "TextBlock(%08X)::Resize(Size=%i) Done", this, size ) ;
   #endif // DEBUG

}
 

/****************************************************************************
 *                                                                          *
 *  TextBlock: Dump                                                         *
 *                                                                          *
 ****************************************************************************/

void TextBlock::Dump ( int Indent ) {
   Log ( "%*sText Block %p:", Indent, "", this ) ;
   Log ( "%*s  Prev, Next: %p %p", Indent, "", Prev, Next ) ;
   Log ( "%*s  Insert, Delete levels: %i %i", Indent, "", InsertLevel, DeleteLevel ) ;
   Log ( "%*s  Data %08p, Size %li", Indent, "", Size > 4 ? Data : (char*)&Data, Size ) ;
   char Text [80] ;
   char *pData = Size > 4 ? Data : (char*)&Data ;
   for ( int i=0; i<Size; i+=16 ) {
      sprintf ( Text, "%04X:", i ) ;
      for ( int j=i; j<i+16 AND j<Size; j++ ) {
         sprintf ( Text+strlen(Text), " %02X", (unsigned char)(pData[j]) ) ;
      } /* endfor */
      while ( j<i+16 ) {
         strcat ( Text, "   " ) ;
         j ++ ;
      } /* endfor */
      strcat ( Text, " |" ) ;
      for ( j=i; j<i+16 AND j<Size; j++ ) {
         sprintf ( Text+strlen(Text), "%c", isprint(pData[j])?pData[j]:'.' ) ;
      } /* endfor */
      while ( j<i+16 ) {
         strcat ( Text, " " ) ;
         j ++ ;
      } /* endfor */
      strcat ( Text, "|" ) ;
      Log ( "%*s%s", Indent, "", Text ) ;
   } /* endfor */
}

