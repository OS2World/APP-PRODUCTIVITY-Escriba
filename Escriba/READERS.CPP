/**************************************************************** READERS.CPP
 *                                                                          *
 *  READER Functions (both file and text)                                   *
 *                                                                          *
 ****************************************************************************/

#include "System.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Readers.h"

//#define DEBUG

#ifdef DEBUG
   extern VOID Log ( char *Message, ... ) ;
#endif

 
/****************************************************************************
 *                                                                          *
 *  Read Data from File                                                     *
 *                                                                          *
 ****************************************************************************/

extern SHORT ReadFile ( PFILEDATA FileData ) {

  /**************************************************************************
   * If we've used up the buffer contents, reload the buffer.               *
   **************************************************************************/

   if ( FileData->Index >= FileData->Size ) {

      if ( DosRead ( FileData->Handle, FileData->Buffer, FileData->Capacity, &FileData->Size ) ) {
         #ifdef DEBUG
         Log ( "ReadFile: EOF reached." ) ;
         #endif
         return ( -1 ) ;
      } /* endif */

      if ( FileData->Size == 0 ) {
         #ifdef DEBUG
         Log ( "ReadFile: EOF reached." ) ;
         #endif
         FileData->Size = FileData->Index ;
         return ( -1 ) ;
      } /* endif */

      #ifdef DEBUG
      Log ( "ReadFile: %i bytes of file data read.", FileData->Size ) ;
      #endif

      FileData->Index = 0 ;

   } /* endif */

  /**************************************************************************
   * Get the next byte from the buffer and return it.                       *
   **************************************************************************/

   #ifdef DEBUG
   Log ( "ReadFile: Reading byte %i of file.", FileData->Index ) ;
   #endif

   UCHAR Byte = FileData->Buffer [ FileData->Index++ ] ;

   return ( UCHAR(Byte) ) ;
}
 
/****************************************************************************
 *                                                                          *
 *  Peek Data from File                                                     *
 *                                                                          *
 ****************************************************************************/

extern SHORT PeekFile ( PFILEDATA FileData ) {

  /**************************************************************************
   * If we've used up the buffer contents, reload the buffer.               *
   **************************************************************************/

   if ( FileData->Index >= FileData->Size ) {

      if ( DosRead ( FileData->Handle, FileData->Buffer, FileData->Capacity, &FileData->Size ) ) {
         #ifdef DEBUG
         Log ( "PeekFile: EOF reached." ) ;
         #endif
         return ( -1 ) ;
      } /* endif */

      if ( FileData->Size == 0 ) {
         #ifdef DEBUG
         Log ( "PeekFile: EOF reached." ) ;
         #endif
         FileData->Size = FileData->Index ;
         return ( -1 ) ;
      } /* endif */

      #ifdef DEBUG
      Log ( "PeekFile: %i bytes of file data read.", FileData->Size ) ;
      #endif

      FileData->Index = 0 ;

   } /* endif */

  /**************************************************************************
   * Get the next byte from the buffer and return it.                       *
   **************************************************************************/

   #ifdef DEBUG
   Log ( "PeekFile: Peeking byte %i of file.", FileData->Index ) ;
   #endif

   UCHAR Byte = FileData->Buffer [ FileData->Index ] ;

   return ( UCHAR(Byte) ) ;
}
 
/****************************************************************************
 *                                                                          *
 *  Tell File Position                                                      *
 *                                                                          *
 ****************************************************************************/

extern ULONG TellFile ( PFILEDATA FileData ) {

   ULONG Offset ;
   DosSetFilePtr ( FileData->Handle, 0, FILE_CURRENT, &Offset ) ;

   Offset -= FileData->Size - FileData->Index ;

   return ( Offset ) ;
}
 
/****************************************************************************
 *                                                                          *
 *  Read Data from Text Buffer                                              *
 *                                                                          *
 ****************************************************************************/

extern SHORT ReadText ( PTEXTDATA TextData ) {

  /**************************************************************************
   * If we've used up the buffer contents, reload the buffer.               *
   **************************************************************************/

   if ( TextData->Index >= TextData->Size ) 
      return ( -1 ) ;

  /**************************************************************************
   * Get the next byte from the buffer and return it.                       *
   **************************************************************************/

   #ifdef DEBUG
   Log ( "ReadText: Reading byte %i of buffer %08p.", TextData->Index, TextData->Buffer ) ;
   #endif

   UCHAR Byte = TextData->Buffer [ TextData->Index++ ] ;

   return ( UCHAR(Byte) ) ;
}
 
/****************************************************************************
 *                                                                          *
 *  Peek Data from Text Buffer                                              *
 *                                                                          *
 ****************************************************************************/

extern SHORT PeekText ( PTEXTDATA TextData ) {

  /**************************************************************************
   * If we've used up the buffer contents, reload the buffer.               *
   **************************************************************************/

   if ( TextData->Index >= TextData->Size ) 
      return ( -1 ) ;

  /**************************************************************************
   * Get the next byte from the buffer and return it.                       *
   **************************************************************************/

   #ifdef DEBUG
   Log ( "ReadText: Peeking at byte %i of buffer %08p.", TextData->Index, TextData->Buffer ) ;
   #endif

   UCHAR Byte = TextData->Buffer [ TextData->Index ] ;

   return ( UCHAR(Byte) ) ;
}
 
/****************************************************************************
 *                                                                          *
 *  Tell Text Position                                                      *
 *                                                                          *
 ****************************************************************************/

extern ULONG TellText ( PTEXTDATA TextData ) {
   return ( TextData->Index ) ;
}

