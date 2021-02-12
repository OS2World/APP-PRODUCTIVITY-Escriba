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

/****************************************************************************
 *                                                                          *
 *  TextBlock: Constructor                                                  *
 *                                                                          *
 ****************************************************************************/

TextBlock::TextBlock ( char *data, long size, short Level ) :
   Prev(0), Next(0), InsertLevel(Level), DeleteLevel(0), Data(0), Size(size) {

   if ( Size > 4 ) {
      Data = new char [ Size ] ; 
      memcpy ( Data, data, Size ) ;

   } else {
      memcpy ( &Data, data, Size ) ;

   } /* endif */
}

 
/****************************************************************************
 *                                                                          *
 *  TextBlock: Copy Constructor                                             *
 *                                                                          *
 ****************************************************************************/

TextBlock::TextBlock ( TextBlock & Original ) :
   Prev(0), Next(0), InsertLevel(Original.InsertLevel), DeleteLevel(Original.DeleteLevel), Data(0), Size(Original.Size) {

   if ( Size > 4 ) {
      Data = new char [ Size ] ;
      memcpy ( Data, Original.Data, size_t(Size) ) ;

   } else {
      memcpy ( &Data, &Original.Data, size_t(Size) ) ;

   } /* endif */
}


/****************************************************************************
 *                                                                          *
 *  TextBlock: Destructor                                                   *
 *                                                                          *
 ****************************************************************************/

TextBlock::~TextBlock ( ) { 
   if ( Size > 4 ) 
      delete [] Data ; 
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

   // If block was and remains allocated . . .
   if ( ( Size > 4 ) && ( size > 4 ) ) {
      // Do nothing.

   // Else if block was allocated and now will not be . . .
   } else if ( ( Size > 4 ) && ( size <= 4 ) ) {
      char *pData = Data ;
      memcpy ( &Data, pData, size_t(size) ) ;
      delete [] pData ;

   // Else the block was not allocated and will remain that way.
   } else {
      // Do nothing.

   } /* endif */

   // Save the new size.
   Size = size ;
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