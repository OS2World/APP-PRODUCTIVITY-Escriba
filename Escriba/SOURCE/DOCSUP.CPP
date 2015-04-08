/***************************************************************** DOCSUP.CPP
 *                                                                          *
 *                        Document Class Support                            *
 *                                                                          *
 ****************************************************************************/

#include "DocSup.h"

#include "DateFmt.h"
#include "Document.h"
#include "External.h"

 
/****************************************************************************
 *                                                                          *
 *  Fetch Token from Data Stream                                            *
 *                                                                          *
 *  NOTE: Token must point to a double-byte receiving buffer, just in case  *
 *  the token found is just a text character.  Memory will be allocated for *
 *  the token ONLY if it is a function token.                               *
 *                                                                          *
 ****************************************************************************/

extern BOOL GetTokenData ( PUCHAR &pToken, long &Size, PREADER Reader, PREADER Peeker, PVOID ReaderData, char *BadToken, int BadTokenSize ) {

  /**************************************************************************
   * Get the first byte.  If EOF, return at once.                           *
   **************************************************************************/

   if ( Peeker ( ReaderData ) < 0 ) 
      return ( FALSE ) ;

   UCHAR Byte = UCHAR ( Reader ( ReaderData ) ) ;

  /**************************************************************************
   * If this byte is EOF character, and is the last in the file, report EOF.*
   **************************************************************************/

   if ( Byte == '\x1A' ) 
      if ( Peeker ( ReaderData ) < 0 ) 
         return ( FALSE ) ;

  /**************************************************************************
   * If this byte is carriage return, swallow any following line feed.      *
   **************************************************************************/

   if ( Byte == '\r' ) 
      if ( Peeker ( ReaderData ) == '\n' ) 
         Byte = UCHAR ( Reader ( ReaderData ) ) ;

  /**************************************************************************
   * Set the token's size.  It is now one byte long.                        *
   **************************************************************************/

   Size = 1 ;

  /**************************************************************************
   * If byte not the first of a function token, return length of one.       *
   **************************************************************************/

   if ( Byte != FUNCTION_START ) {
      *pToken = Byte ;            
      return ( TRUE ) ;
   } /* endif */

  /**************************************************************************
   * Fetch the entire function token, including imbedded tokens.            *
   **************************************************************************/

   ULONG BlockSize = 32 ;
   PUCHAR Block = PUCHAR ( malloc ( size_t ( BlockSize ) ) ) ;
   PUCHAR BlockPtr = Block ;
   *BlockPtr = Byte ;

   SHORT NextByte = Peeker ( ReaderData ) ;
   USHORT Nesting = 1 ;
   while ( Nesting ) {

      // If expecting more data, but we've hit EOF, then return an EOF,
      //   discarding this token as incomplete and invalid.
      if ( NextByte < 0 ) {
         if ( BadTokenSize ) {
            memcpy ( BadToken, Block, min(Size,BadTokenSize-1) ) ;
            BadToken[min(Size,BadTokenSize-1)] = 0 ;
         } /* endif */
         free ( Block ) ;
         return ( FALSE ) ;
      } /* endif */

      // Move to next position in the token buffer.  Grow the buffer if necessary.
      BlockPtr ++ ;
      if ( BlockPtr-Block >= BlockSize ) {
         PUCHAR NewBlock = PUCHAR ( malloc ( size_t ( BlockSize*2 ) ) ) ;
         memcpy ( NewBlock, Block, size_t(BlockSize) ) ;
         free ( Block ) ;
         Block = NewBlock ;
         BlockSize *= 2 ;
         BlockPtr = Block + Size ;
      } /* endif */

      // Save the current byte into the token buffer.
      *BlockPtr = UCHAR ( Reader ( ReaderData ) ) ;
      Size ++ ;

      // See what the next byte is.  If escaped function start or end, store it without nesting.
      NextByte = Peeker ( ReaderData ) ;
      if ( ( *BlockPtr == FUNCTION_ESCAPE ) AND ( ( NextByte == FUNCTION_ESCAPE ) OR ( NextByte == FUNCTION_START ) OR ( NextByte == FUNCTION_END ) ) ) {
         *BlockPtr = (UCHAR) Reader ( ReaderData ) ;
         NextByte = Peeker ( ReaderData ) ;
      } else {  // Else, compute the new nesting level.
         if ( *BlockPtr == FUNCTION_START ) {
            Nesting ++ ;
         } else if ( *BlockPtr == FUNCTION_END ) {
            Nesting -- ;
         } /* endif */
      } /* endif */

   } /* endwhile */

  /**************************************************************************
   * Null-terminate the token.                                              *
   **************************************************************************/

   BlockPtr ++ ;
   if ( BlockPtr-Block >= BlockSize ) {
      PUCHAR NewBlock = PUCHAR ( malloc ( size_t ( BlockSize+1 ) ) ) ;
      memcpy ( NewBlock, Block, size_t(BlockSize) ) ;
      free ( Block ) ;
      Block = NewBlock ;
      BlockSize ++ ;
      BlockPtr = Block + Size ;
   } /* endif */

   *BlockPtr = 0 ;

  /**************************************************************************
   * Return the address of the value and its size.                          *
   **************************************************************************/

   pToken = Block ;
   return ( TRUE ) ;
}
 
/****************************************************************************
 *                                                                          *
 *  Convert Raw Text to internal format.                                    *
 *                                                                          *
 *  This function accepts a contiguous block of external text with embedded *
 *  function tokens (in the external format), and converts the entire text  *
 *  to the internal format, storing the length (which cannot exceed 255     *
 *  bytes) in the first byte of the result buffer.                          *
 *                                                                          *
 ****************************************************************************/

extern int ImportText ( HAB Anchor, unsigned char *pExternal, int ExternalSize, unsigned char *pInternal, int &Version, char *BadToken, int BadTokenSize ) {

   // Reset the result.
   int InternalSize ( 0 ) ;
   if ( pInternal ) 
      pInternal[InternalSize] = 0 ;
   InternalSize ++ ;

   // Prepare to scan the input text.
   TEXTDATA TextData ;
   TextData.Buffer = (PUCHAR) pExternal ;
   TextData.Index = 0 ;
   TextData.Size = ExternalSize ;

   // Get tokens repeatedly.
   UCHAR Bytes [2] ; PUCHAR pToken ( Bytes ) ; 
   long Size ; LONG ResourceHandle ;  int ResourceHandleType ;  
   while ( GetTokenData ( pToken, Size, PREADER(ReadText), PREADER(PeekText), &TextData, BadToken, BadTokenSize ) ) {

      // Once we have a token, it needs to be converted to internal form.
      PUCHAR ConvertedToken = new UCHAR [ Size ] ;
      long ConvertedSize ;
      if ( ImportToken ( Anchor, ConvertedToken, ConvertedSize, pToken, Size, ResourceHandle, ResourceHandleType, Version, BadToken, BadTokenSize ) ) {

         // If it was a function token, store the converted form.
         if ( pInternal ) 
            memcpy ( pInternal+InternalSize, ConvertedToken, ConvertedSize ) ;
         InternalSize += ConvertedSize ;

      // Else if bad token, return at once.
      } else if ( BadTokenSize && BadToken[0] ) {
         return ( 0 ) ;

      // Else store the token verbatim.
      } else {
         if ( pInternal ) 
            memcpy ( pInternal+InternalSize, pToken, Size ) ;
         InternalSize += Size ;

      } /* endif */

      // Discard the converted token, now that we've copied it.
      delete [] ConvertedToken ;
      if ( *pToken == FUNCTION_START ) free ( pToken ) ;

   } /* endwhile */

   // If things didn't work out, return an error.
   if ( BadTokenSize && BadToken[0] ) 
      return ( 0 ) ;

   // Store the processed length in the first byte of the internal string.
   if ( pInternal ) 
      pInternal[0] = (unsigned char) ( InternalSize - 1 ) ; // Sizeof(importedtokens)

   // Return the total size, including the header byte-count.
   return ( InternalSize ) ;
}

/****************************************************************************
 *                                                                          *
 *  Convert Raw Function Token to internal format.                          *
 *                                                                          *
 *  Data[0] = 0                                                             *
 *  Data[1] = FunctionCode                                                  *
 *  Data[2...n] = Parameters (format dependent on function code)            *
 *  Data[n+1]..[n+4] = n+1                                                  *
 *  Data[n+5] = 0                                                           *
 *                                                                          *
 ****************************************************************************/

extern BOOL ImportToken ( HAB Anchor, PUCHAR &DataOut, long &SizeOut, PUCHAR DataIn, long SizeIn, LONG &Handle, int &HandleType, int &Version, char *BadToken, int BadTokenSize ) {

   // Reject token if not a function.
   if ( DataIn[0] != FUNCTION_START ) 
      return ( FALSE ) ;

   if ( DataIn[SizeIn-1] != FUNCTION_END ) 
      return ( FALSE ) ;

   // Store header byte (a zero).
   SizeOut = 0 ;
   DataOut[SizeOut++] = 0 ;

   // Convert function type.  Store as a byte.
   for ( int Type=0; Type<MAX_FUNCTIONS; Type++ ) 
      if ( !strnicmp ( PCHAR(DataIn+1), FunctionNames[Type], strlen(FunctionNames[Type]) ) ) 
         break ;
   if ( Type >= MAX_FUNCTIONS ) {
      if ( BadToken && BadTokenSize ) 
         strncpy ( BadToken, (char*)DataIn, BadTokenSize ) ;
      return ( FALSE ) ;
   } /* endif */
   DataOut[SizeOut++] = UCHAR(Type) ;

   // Get parameter data address.  Skip the start bracket and the function name.
   PCHAR pData = PCHAR(DataIn) + strlen(FunctionNames[Type]) + 1 ;

   // Get parameter data size.  Leave off the start and end brackets.
   int DataLen = int ( SizeIn - strlen(FunctionNames[Type]) - 2 ) ;

   // Process according to function type . . .
   switch ( Type ) {

      case FN__VERSION:
         Version = (int) atol ( pData ) ;
         *PLONG(DataOut+SizeOut) = atol ( pData ) ;  SizeOut += sizeof(LONG) ;
         break ;

      case FN__PREFACE:
      case FN__JOBSETTINGS:
         memcpy ( DataOut+SizeOut, pData, DataLen ) ;
         SizeOut += DataLen ;
         break ;

      case FN__PRINTER:
      case FN__FORM:
      case FN__FONT:
      case FN__AUTHOR:
      case FN__COMMENT:
      case FN__TITLE:
         memcpy ( DataOut+SizeOut, pData, DataLen ) ;
         SizeOut += DataLen ;
         DataOut[SizeOut++] = 0 ;
         break ;

      case FN__HEADER:
      case FN__FOOTER: {
         SizeOut += ImportText ( Anchor, (unsigned char*)pData, DataLen, DataOut+SizeOut, Version, BadToken, BadTokenSize ) ;
         break ; }

      case FN__DATEFORMAT: {
         char String [80] ;
         if ( Version < 92 ) 
            memcpy ( DataOut+SizeOut, GeneralDateFormat(String,pData), DataLen ) ;
         else
            memcpy ( DataOut+SizeOut, pData, DataLen ) ;
         SizeOut += DataLen ;
         DataOut[SizeOut++] = 0 ;
         break ; }

      case FN__SIZE:
         *PLONG(DataOut+SizeOut) = ULONG ( atof ( pData ) * 10.0 ) ;  SizeOut += sizeof(ULONG) ;
         break ;

      case FN__LINECOLOR:
      case FN__FILLCOLOR: {
         COLOR Color = 0 ;
         for ( int i=0; i<6; i++, pData++ ) {
            Color <<= 4 ;
            Color += ( (*pData>'9') ? (*pData-'A'+10) : (*pData-'0') ) ;
         }
         *PCOLOR(DataOut+SizeOut) = Color ;  SizeOut += sizeof(COLOR) ;
         break ; }

      case FN__BOLD:
      case FN__ITALIC:
      case FN__UNDERSCORE:
      case FN__STRIKEOUT:
      case FN__OUTLINE:
      case FN__CAPS:
      case FN__CENTERPAGE:
      case FN__METRIC:
      case FN__GLYPH:
         *PUCHAR(DataOut+SizeOut) = (UCHAR) atoi ( pData ) ;  SizeOut += sizeof(UCHAR) ;
         break ;

      case FN__CODEPAGE:
      case FN__LINEJOIN:
      case FN__LINEEND:
      case FN__FILLTYPE:
         *PSHORT(DataOut+SizeOut) = (SHORT) atol ( pData ) ;  SizeOut += sizeof(SHORT) ;
         break ;

      case FN__EXPAND:
      case FN__TOPMARGIN:
      case FN__BOTTOMMARGIN:
      case FN__LEFTMARGIN:
      case FN__RIGHTMARGIN:
      case FN__TABSET:
      case FN__TABCLEAR:
      case FN__TABSPACING:
      case FN__INDENT:
      case FN__DATEFIXED:
      case FN__LINESPACING:
      case FN__LINEWIDTH:
      case FN__CREATEDATE:
         *PLONG(DataOut+SizeOut) = atol ( pData ) ;  SizeOut += sizeof(LONG) ;
         break ;

      case FN__LEFTJUSTIFY:
      case FN__CENTERJUSTIFY:
      case FN__RIGHTJUSTIFY:
      case FN__FULLJUSTIFY:
      case FN__DATECURRENT:
      case FN__DATECHANGED:
      case FN__SEPARATOR:
         break ;

      case FN__GRAPHIC: {
         char Posx[32], Posy[32], Sizx[32], Sizy[32] ;
         int PrefaceLength ;
         sscanf ( pData, "%[^,:],%[^,:],%[^,:],%[^,:]%n", Posx, Posy, Sizx, Sizy, &PrefaceLength ) ;

         POINTL Position = { atoi(Posx), atoi(Posy) } ;
         SIZEL Size = { atoi(Sizx), atoi(Sizy) } ;

         if ( !stricmp ( Posx, "INLINE" ) ) {
            Position.x = GRAPHIC_NEXT_CHAR ;
         } else if ( !stricmp ( Posx, "LEFTEDGE" ) ) {
            Position.x = GRAPHIC_EDGE_LEFT ;
         } else if ( !stricmp ( Posx, "RIGHTEDGE" ) ) {
            Position.x = GRAPHIC_EDGE_RIGHT ;
         } else if ( !stricmp ( Posx, "LEFTMARGIN" ) ) {
            Position.x = GRAPHIC_MARGIN_LEFT ;
         } else if ( !stricmp ( Posx, "RIGHTMARGIN" ) ) {
            Position.x = GRAPHIC_MARGIN_RIGHT ;
         } else if ( !stricmp ( Posx, "CENTERED" ) ) {
            Position.x = GRAPHIC_CENTERED ;
         } /* endif */

         if ( !stricmp ( Posy, "TOPEDGE" ) ) {
            Position.y = GRAPHIC_EDGE_TOP ;
         } else if ( !stricmp ( Posy, "BOTTOMEDGE" ) ) {
            Position.y = GRAPHIC_EDGE_BOTTOM ;
         } else if ( !stricmp ( Posy, "TOPMARGIN" ) ) {
            Position.y = GRAPHIC_MARGIN_TOP ;
         } else if ( !stricmp ( Posy, "BOTTOMMARGIN" ) ) {
            Position.y = GRAPHIC_MARGIN_BOTTOM ;
         } else if ( !stricmp ( Posy, "CENTERED" ) ) {
            Position.y = GRAPHIC_CENTERED ;
         } /* endif */

         if ( !stricmp ( Sizx, "EDGETOEDGE" ) ) {
            Size.cx = GRAPHIC_EDGE_TO_EDGE ;
         } else if ( !stricmp ( Sizx, "MARGINTOMARGIN" ) ) {
            Size.cx = GRAPHIC_MARGIN_TO_MARGIN ;
         } /* endif */

         if ( !stricmp ( Sizy, "EDGETOEDGE" ) ) {
            Size.cy = GRAPHIC_EDGE_TO_EDGE ;
         } else if ( !stricmp ( Sizy, "MARGINTOMARGIN" ) ) {
            Size.cy = GRAPHIC_MARGIN_TO_MARGIN ;
         } /* endif */

         BOOL Background = FALSE ;
         if ( strstr ( pData+PrefaceLength, ",Background" ) )
            Background = TRUE ;

         BOOL NoTextAside = FALSE ;
         if ( strstr ( pData+PrefaceLength, ",NoTextAside" ) )
            NoTextAside = TRUE ;

         BOOL Halftone = FALSE ;
         if ( strstr ( pData+PrefaceLength, ",Halftone" ) )
            Halftone = TRUE ;

         BOOL Reverse = FALSE ;
         if ( strstr ( pData+PrefaceLength, ",Reverse" ) )
            Reverse = TRUE ;

         int Aspect = GRAPHIC_ASPECT_UNFIXED ;
         if ( strstr ( pData+PrefaceLength, ",KeepAspectFixX" ) )
            Aspect = GRAPHIC_ASPECT_FIX_X ;
         else if ( strstr ( pData+PrefaceLength, ",KeepAspectFixY" ) )
            Aspect = GRAPHIC_ASPECT_FIX_Y ;

         BOOL PrinterOptimize = FALSE ;
         if ( strstr ( pData+PrefaceLength, ",PrinterOptimize" ) )
            PrinterOptimize = TRUE ;

         char *Type = strchr ( pData, ':' ) + 1 ;

         *PPOINTL(DataOut+SizeOut) = Position ;  SizeOut += sizeof(POINTL) ;
         *PSIZEL(DataOut+SizeOut) = Size ;  SizeOut += sizeof(SIZEL) ;
         DataOut[SizeOut++] = UCHAR(Background) ;
         DataOut[SizeOut++] = UCHAR(NoTextAside) ;
         DataOut[SizeOut++] = UCHAR(Halftone) ;
         DataOut[SizeOut++] = UCHAR(Reverse) ;
         DataOut[SizeOut++] = UCHAR(Aspect) ;
         DataOut[SizeOut++] = UCHAR(PrinterOptimize) ;
         DataOut[SizeOut++] = Type [0] ;
         DataOut[SizeOut++] = Type [1] ;

         if ( !strnicmp ( Type, "BM", 2 ) ) {           // Bitmap
            if ( strlen(Type) == 2 ) {
               ULONG cbBitmapInfo = * PULONG ( PUCHAR(pData+strlen(pData)+1) ) ;
               PBITMAPINFO2 pbmi = PBITMAPINFO2 ( PUCHAR(pData+strlen(pData)+1) + 2*sizeof(ULONG) ) ;
               PUCHAR pbBuffer = PUCHAR ( pbmi ) + cbBitmapInfo ;
               DeviceContext Memory = DeviceContext ( "ImportToken", long(Anchor) ) ;
               WorkSpace PS ( "ImportToken", 0, &Memory, Anchor, TRUE ) ;
               Handle = LONG ( PS.CreateBitmap ( *PBITMAPINFOHEADER2(pbmi), pbBuffer, pbmi ) ) ;
               if ( Handle == GPI_ERROR ) {
                  ERRORID Code = Sys_GetLastError ( 0 ) ;
                  Log ( "ImportToken::Token: Unable to create bitmap.  Code %X.", Code ) ;
               } /* endif */
            } else {
               Handle = LONG ( atol ( Type + 2 ) ) ;
            } /* endif */
            *PLONG(DataOut+SizeOut) = Handle ;  SizeOut += sizeof(LONG) ;
            HandleType = 0 ;

         } else if ( !strnicmp ( Type, "MF", 2 ) ) {    // Metafile
            if ( strlen(Type) == 2 ) {
               ULONG cbBuffer = * PULONG ( PUCHAR(pData+strlen(pData)+1) ) ;
               PUCHAR pBuffer = PUCHAR(pData+strlen(pData)+1) + sizeof(ULONG) ;
               char FileName [_MAX_PATH] ;
               FILE *File = fopen ( tmpnam(FileName), "wb" ) ;
               if ( File ) {
                  fwrite ( pBuffer, size_t(cbBuffer), 1, File ) ;
                  fclose ( File ) ;
                  Handle = LONG ( GpiLoadMetaFile ( 0, PSZ(FileName) ) ) ;
                  remove ( FileName ) ;
               } /* endif */
            } else {
               Handle = LONG ( atol ( Type + 2 ) ) ;
            } /* endif */
            *PLONG(DataOut+SizeOut) = Handle ;  SizeOut += sizeof(LONG) ;
            HandleType = 1 ;

         } else if ( !strnicmp ( Type, "UP", 2 ) ) {    // UPC-A Barcode
            if ( strlen(Type) == 2 ) {
               memcpy ( DataOut+SizeOut, Type+3, strlen(Type+3)-1 ) ;
               SizeOut += strlen(Type+3) - 1 ;
            } else {
               memcpy ( DataOut+SizeOut, Type+2, strlen(Type+2)-1 ) ;
               SizeOut += strlen(Type+2) - 1 ;
            } /* endif */
            DataOut[SizeOut++] = 0 ;

         } else if ( !strnicmp ( Type, "UE", 2 ) ) {    // UPC-E Barcode
            if ( strlen(Type) == 2 ) {
               memcpy ( DataOut+SizeOut, Type+3, strlen(Type+3)-1 ) ;
               SizeOut += strlen(Type+3) - 1 ;
            } else {
               memcpy ( DataOut+SizeOut, Type+2, strlen(Type+2)-1 ) ;
               SizeOut += strlen(Type+2) - 1 ;
            } /* endif */
            DataOut[SizeOut++] = 0 ;

         } else if ( !strnicmp ( Type, "39", 2 ) ) {    // Code-39 Barcode
            if ( strlen(Type) == 2 ) {
               memcpy ( DataOut+SizeOut, Type+3, strlen(Type+3)-1 ) ;
               SizeOut += strlen(Type+3) - 1 ;
            } else {
               memcpy ( DataOut+SizeOut, Type+2, strlen(Type+2)-1 ) ;
               SizeOut += strlen(Type+2) - 1 ;
            } /* endif */
            DataOut[SizeOut++] = 0 ;

         } else if ( !strnicmp ( Type, "PN", 2 ) ) {    // POSTNET Barcode
            if ( strlen(Type) == 2 ) {
               memcpy ( DataOut+SizeOut, Type+3, strlen(Type+3)-1 ) ;
               SizeOut += strlen(Type+3) - 1 ;
            } else {
               memcpy ( DataOut+SizeOut, Type+2, strlen(Type+2)-1 ) ;
               SizeOut += strlen(Type+2) - 1 ;
            } /* endif */
            DataOut[SizeOut++] = 0 ;

         } else if ( !strnicmp ( Type, "E8", 2 ) ) {    // EAN-8 Barcode
            if ( strlen(Type) == 2 ) {
               memcpy ( DataOut+SizeOut, Type+3, strlen(Type+3)-1 ) ;
               SizeOut += strlen(Type+3) - 1 ;
            } else {
               memcpy ( DataOut+SizeOut, Type+2, strlen(Type+2)-1 ) ;
               SizeOut += strlen(Type+2) - 1 ;
            } /* endif */
            DataOut[SizeOut++] = 0 ;

         } else if ( !strnicmp ( Type, "EA", 2 ) ) {    // EAN-13 Barcode
            if ( strlen(Type) == 2 ) {
               memcpy ( DataOut+SizeOut, Type+3, strlen(Type+3)-1 ) ;
               SizeOut += strlen(Type+3) - 1 ;
            } else {
               memcpy ( DataOut+SizeOut, Type+2, strlen(Type+2)-1 ) ;
               SizeOut += strlen(Type+2) - 1 ;
            } /* endif */
            DataOut[SizeOut++] = 0 ;

         } else {                                       // Invalid graphic definition.
            Log ( "ImportToken: WARNING: Invalid graphic type '%0.2s'.", Type ) ;
            *PLONG(DataOut+SizeOut) = 0 ;  SizeOut += sizeof(LONG) ;

         } /* endif */
         break ;
      } /* endcase */

   } /* endswitch */

   // Store the length excluding the length byte itself and tne end marker.
   *((TOKEN_BACKUP_TYPE*)&DataOut[SizeOut]) = SizeOut ;
   SizeOut += sizeof(TOKEN_BACKUP_TYPE) ;

   // Store the end marker.
   DataOut[SizeOut++] = 0 ;

   // Return good status.
   return ( TRUE ) ;
}
 
/****************************************************************************
 *                                                                          *
 *  Convert Text to raw external format.                                    *
 *                                                                          *
 *  In: Pointer to internal text, count of internal bytes.                  *
 *      Pointer to external text.                                           *
 *                                                                          *
 *  Out: Updated pointer to external text.                                  *
 *                                                                          *
 ****************************************************************************/

extern unsigned char *ExportText ( HAB Anchor, WorkSpace *PS, unsigned char *External, unsigned char *Internal, int InternalCount ) {
   unsigned char *Start = Internal ;
   unsigned char *Finish = Internal + InternalCount ;
   while ( Start < Finish ) {
      if ( IsFunction ( *Start ) ) {
         long LengthOut, LengthIn ;
         ComputeFunctionSize ( Anchor, PS, Start, LengthOut, LengthIn ) ;
         UCHAR Bytes [2] ; PUCHAR pDataOut = Bytes ;
         ExportToken ( Anchor, PS, pDataOut, LengthOut, Start, LengthIn ) ;
         memcpy ( External, pDataOut, size_t(LengthOut) ) ;
         if ( LengthOut > 2 )
            free ( pDataOut ) ;
         External += LengthOut ;
         Start += LengthIn ;
      } else {
         if ( *Start == 0x0A ) 
            *External++ = 0x0D ;
         *External++ = *Start ;
         if ( PS->IsDBCSHeader ( *Start ) ) {
            *External++ = *(Start+1) ;
            Start ++ ;
         } /* endif */
         Start ++ ;
      } /* endif */
   } /* endwhile */
   return ( External ) ;
}

/****************************************************************************
 *                                                                          *
 *  Convert Token to raw external format.                                   *
 *                                                                          *
 *  NOTE: Expects DataOut to contain a pointer to a valid two-byte area ton *
 *    used for the exportation of non-function tokens.  This pointer will   *
 *    be replaced with the address of an allocated memory block for passing *
 *    function tokens.  That memory will need releasing later.              *
 *                                                                          *
 *  Returns TRUE if valid function.                                         *
 *                                                                          *
 ****************************************************************************/

extern BOOL ExportToken ( HAB Anchor, WorkSpace *PS, PUCHAR &DataOut, long &SizeOut, PUCHAR DataIn, long &SizeIn ) {

   // If not a function, then simply copy the data out.
   if ( *DataIn ) {
      *DataOut = *DataIn ;
      SizeOut = SizeIn = 1 ;
      return ( TRUE ) ;
   } /* endif */

   // Measure the function.  If invalid, set size to one byte.
   if ( !ComputeFunctionSize ( Anchor, PS, DataIn, SizeOut, SizeIn ) ) {
      *DataOut = *DataIn ;
      SizeOut = SizeIn = 1 ;
      return ( FALSE ) ;
   } /* endif */

   // Allocate memory for the exported function text.
   DataOut = PUCHAR ( malloc ( size_t ( SizeOut + 1 ) ) ) ;

   // Set the external header and function name.
   PUCHAR p = DataOut ;
   *p++ = FUNCTION_START ;
   strcpy ( PCHAR(p), FunctionNames[*(++DataIn)] ) ;
   p += strlen(PCHAR(p)) ;

   // Format the parameter data, if any.
   switch ( *DataIn++ ) {

      case FN__FONT:
      case FN__DATEFORMAT:
      case FN__AUTHOR:
      case FN__COMMENT:
      case FN__TITLE:
         strcpy ( PCHAR(p), PCHAR(DataIn) ) ;
         p += strlen(PCHAR(p)) ;
         break ;

      case FN__HEADER:
      case FN__FOOTER: {
         p = ExportText ( Anchor, PS, p, DataIn+1, *DataIn ) ;
         break ; }

      case FN__SIZE:
         sprintf ( PCHAR(p), "%.1lf", (double) ( *PULONG(DataIn) ) / 10.0 ) ;
         p += strlen(PCHAR(p)) ;
         break ;

      case FN__LINEJOIN:
      case FN__LINEEND:
      case FN__FILLTYPE:
         sprintf ( PCHAR(p), "%i", *PUSHORT(DataIn) ) ;
         p += strlen(PCHAR(p)) ;
         break ;

      case FN__EXPAND:
      case FN__TOPMARGIN:
      case FN__BOTTOMMARGIN:
      case FN__LEFTMARGIN:
      case FN__RIGHTMARGIN:
      case FN__TABSET:
      case FN__TABCLEAR:
      case FN__TABSPACING:
      case FN__INDENT:
      case FN__DATEFIXED:
      case FN__LINESPACING:
      case FN__LINEWIDTH:
      case FN__CREATEDATE:
         sprintf ( PCHAR(p), "%li", *PULONG(DataIn) ) ;
         p += strlen(PCHAR(p)) ;
         break ;

      case FN__LINECOLOR:
      case FN__FILLCOLOR:
         sprintf ( PCHAR(p), "%06lX", *PCOLOR(DataIn) ) ;
         p += strlen(PCHAR(p)) ;
         break ;

      case FN__BOLD:
      case FN__ITALIC:
      case FN__UNDERSCORE:
      case FN__STRIKEOUT:
      case FN__OUTLINE:
      case FN__CAPS:
      case FN__CENTERPAGE:
         *p++ = (UCHAR) ( *DataIn ? '1' : '0' ) ;
         break ;

      case FN__GLYPH:
         sprintf ( PCHAR(p), "%i", *PUCHAR(DataIn) ) ;
         p += strlen(PCHAR(p)) ;
         break ;

      case FN__LEFTJUSTIFY:
      case FN__CENTERJUSTIFY:
      case FN__RIGHTJUSTIFY:
      case FN__FULLJUSTIFY:
      case FN__DATECURRENT:
      case FN__DATECHANGED:
      case FN__SEPARATOR:
         break ;

      case FN__GRAPHIC: {
         POINTL Position = *PPOINTL(DataIn) ;                   DataIn += sizeof(POINTL) ;
         SIZEL Size = *PSIZEL(DataIn) ;                         DataIn += sizeof(SIZEL) ;
         BOOL Background = *DataIn ? TRUE : FALSE ;             DataIn ++ ;
         BOOL NoTextAside = *DataIn ? TRUE : FALSE ;            DataIn ++ ;
         BOOL Halftone = *DataIn ? TRUE : FALSE ;               DataIn ++ ;
         BOOL Reverse = *DataIn ? TRUE : FALSE ;                DataIn ++ ;
         int  Aspect = *DataIn ;                                DataIn ++ ;
         BOOL PrinterOptimize = *DataIn ? TRUE : FALSE ;        DataIn ++ ;
         char *Type = PCHAR(DataIn) ;                           DataIn += 2 ;
         long Handle = *PLONG(DataIn) ;
         char *BarCode = PCHAR(DataIn) ;
         p = DataOut + CreateGraphicToken ( FALSE, PCHAR(DataOut), Position, Size, 
            Background, NoTextAside, Halftone, Reverse, Aspect, PrinterOptimize, Type, Handle, BarCode ) - 1 ;
         break ; }

   } /* endswitch */

   // Set the function trailer, plus a terminating zero.
   *p++ = FUNCTION_END ;
   *p = 0 ;

   // Compute the result size.
   SizeOut = p - DataOut ;

   // Sweet success!
   #ifdef DEBUG
   Log ( "ExportToken: Done.  SizeOut %i bytes.", SizeOut ) ;
   #endif
   return ( TRUE ) ;
}
 
/****************************************************************************
 *                                                                          *
 *  Validate and compute internal and external sizes of a function token.   *
 *                                                                          *
 *  Returns TRUE if function token valid.                                   *
 *                                                                          *
 ****************************************************************************/

extern BOOL ComputeFunctionSize ( HAB Anchor, WorkSpace *PS, PUCHAR DataIn, long &SizeOut, long &SizeIn ) {

   // Validate and measure the internal size of the token.
   if ( !ComputeFunctionSize ( DataIn, SizeIn ) )
      return ( FALSE ) ;

   // Advance to the function code.
   DataIn ++ ;

   // All external functions have a header, trailer and name.
   SizeOut = 2 + strlen(FunctionNames[*DataIn]) ;

   // Determine the additional sizes according to the function type.
   switch ( *DataIn++ ) {

      case FN__FONT:
      case FN__DATEFORMAT:
      case FN__AUTHOR:
      case FN__COMMENT:
      case FN__TITLE:
         SizeOut += strlen(PCHAR(DataIn)) ;
         break ;

      case FN__HEADER:
      case FN__FOOTER: {
         unsigned char *Start = DataIn + 1 ;
         unsigned char *Finish = DataIn + 1 + *DataIn ;
         while ( Start < Finish ) {
            if ( IsFunction ( *Start ) ) {
               long LengthOut, LengthIn ;
               ComputeFunctionSize ( Anchor, PS, Start, LengthOut, LengthIn ) ;
               Start += LengthIn ;
               SizeOut += LengthOut ;
            } else {
               SizeOut ++ ;
               if ( PS->IsDBCSHeader ( *Start ) ) {
                  SizeOut ++ ;
                  Start ++ ;
               } /* endif */
               Start ++ ;
            } /* endif */
         } /* endwhile */
         break ; }

      case FN__LINEJOIN:
      case FN__LINEEND:
      case FN__FILLTYPE:
         SizeOut += 10 ;
         break ;

      case FN__SIZE:
      case FN__EXPAND:
      case FN__TOPMARGIN:
      case FN__BOTTOMMARGIN:
      case FN__LEFTMARGIN:
      case FN__RIGHTMARGIN:
      case FN__TABSET:
      case FN__TABCLEAR:
      case FN__TABSPACING:
      case FN__INDENT:
      case FN__DATEFIXED:
      case FN__LINESPACING:
      case FN__LINEWIDTH:
      case FN__CREATEDATE:
         SizeOut += 10 ;
         break ;

      case FN__LINECOLOR:
      case FN__FILLCOLOR:
         SizeOut += 6 ;
         break ;

      case FN__BOLD:
      case FN__ITALIC:
      case FN__UNDERSCORE:
      case FN__STRIKEOUT:
      case FN__OUTLINE:
      case FN__CAPS:
      case FN__CENTERPAGE:
         SizeOut ++ ;
         break ;

      case FN__GLYPH:
         SizeOut += 4 ;
         break ;

      case FN__LEFTJUSTIFY:
      case FN__CENTERJUSTIFY:
      case FN__RIGHTJUSTIFY:
      case FN__FULLJUSTIFY:
      case FN__DATECURRENT:
      case FN__DATECHANGED:
      case FN__SEPARATOR:
         break ;

      case FN__GRAPHIC: {
         POINTL Position = *PPOINTL(DataIn) ;                   DataIn += sizeof(POINTL) ;
         SIZEL Size = *PSIZEL(DataIn) ;                         DataIn += sizeof(SIZEL) ;
         /* BOOL Background = *DataIn ? TRUE : FALSE ;  */      DataIn ++ ;
         /* BOOL NoTextAside = *DataIn ? TRUE : FALSE ; */      DataIn ++ ;
         /* BOOL Halftone = *DataIn ? TRUE : FALSE ;    */      DataIn ++ ;
         /* BOOL Reverse = *DataIn ? TRUE : FALSE ;     */      DataIn ++ ;
         /* int  Aspect = *DataIn ;                     */      DataIn ++ ;
         /* BOOL PrinterOptimize = *DataIn ? TRUE : FALSE ; */  DataIn ++ ;
         char *Type = PCHAR(DataIn) ;                           DataIn += 2 ;

         if ( !strnicmp ( Type, "BM", 2 ) ) {           // Bitmap
            LONG Handle = *PLONG(DataIn) ;
            SizeOut += 80 ;
            BITMAPINFOHEADER2 BitmapInfoHeader ;
            BitmapInfoHeader.cbFix = sizeof(BitmapInfoHeader) ;
            GpiQueryBitmapInfoHeader ( Handle, &BitmapInfoHeader ) ;
            ULONG cbBuffer = ( ( ( BitmapInfoHeader.cBitCount * BitmapInfoHeader.cx ) + 31 ) / 32 )
               * 4 * BitmapInfoHeader.cy * BitmapInfoHeader.cPlanes ;
            PUCHAR pbBuffer ;
            DosAllocMem ( (PPVOID)&pbBuffer, cbBuffer, PAG_COMMIT | PAG_READ | PAG_WRITE ) ;
            ULONG cbBitmapInfo = sizeof(BITMAPINFO2) + ( sizeof(RGB2) * ( 1 << BitmapInfoHeader.cBitCount ) ) ;
            PBITMAPINFO2 pbmi ;
            DosAllocMem ( (PPVOID)&pbmi, cbBitmapInfo, PAG_COMMIT | PAG_READ | PAG_WRITE ) ;
            memcpy ( pbmi, &BitmapInfoHeader, sizeof(BitmapInfoHeader) ) ;
            DeviceContext Memory = DeviceContext ( "ComputeFunctionSize", long(Anchor) ) ;
            WorkSpace PS ( "ComputeFunctionSize", 0, &Memory, Anchor, TRUE ) ;
            PS.SetBitmap ( Handle ) ;
            PS.QueryBitmapBits ( 0, int(BitmapInfoHeader.cy), pbBuffer, pbmi ) ;
            SizeOut += WriteBinary ( PVOID(0), &cbBitmapInfo, sizeof(cbBitmapInfo) ) ;
            SizeOut += WriteBinary ( PVOID(0), &cbBuffer, sizeof(cbBuffer) ) ;
            SizeOut += WriteBinary ( PVOID(0), pbmi, cbBitmapInfo ) ;
            SizeOut += WriteBinary ( PVOID(0), pbBuffer, cbBuffer ) ;
            PS.SetBitmap ( 0 ) ;
            DosFreeMem ( pbmi ) ;
            DosFreeMem ( pbBuffer ) ;

         } else if ( !strnicmp ( Type, "MF", 2 ) ) {    // Metafile
            LONG Handle = *PLONG(DataIn) ;
            SizeOut += 80 ;
            LONG cbBuffer = GpiQueryMetaFileLength ( HMF(Handle) ) ;
            PUCHAR pBuffer ;
            DosAllocMem ( (PPVOID)&pBuffer, cbBuffer, PAG_COMMIT | PAG_READ | PAG_WRITE ) ;
            if ( GpiQueryMetaFileBits ( HMF(Handle), 0, cbBuffer, (PBYTE)pBuffer ) ) {
               SizeOut += WriteBinary ( PVOID(0), &cbBuffer, sizeof(cbBuffer) ) ;
               SizeOut += WriteBinary ( PVOID(0), pBuffer, cbBuffer ) ;
            } /* endif */
            DosFreeMem ( pBuffer ) ;

         } else if ( !strnicmp ( Type, "UP", 2 ) ) {    // UPC-A Barcode
            char *UpcCode = PCHAR(DataIn) ;
            SizeOut += strlen(UpcCode) ;

         } else if ( !strnicmp ( Type, "UE", 2 ) ) {    // UPC-E Barcode
            char *UpcCode = PCHAR(DataIn) ;
            SizeOut += strlen(UpcCode) ;

         } else if ( !strnicmp ( Type, "39", 2 ) ) {    // Code-39 Barcode
            char *BarCode = PCHAR(DataIn) ;
            SizeOut += strlen(BarCode) ;

         } else if ( !strnicmp ( Type, "PN", 2 ) ) {    // POSTNET Barcode
            char *BarCode = PCHAR(DataIn) ;
            SizeOut += strlen(BarCode) ;

         } else if ( !strnicmp ( Type, "E8", 2 ) ) {    // EAN-8 Barcode
            char *BarCode = PCHAR(DataIn) ;
            SizeOut += strlen(BarCode) ;

         } else if ( !strnicmp ( Type, "EA", 2 ) ) {    // EAN-13 Barcode
            char *BarCode = PCHAR(DataIn) ;
            SizeOut += strlen(BarCode) ;

         } else {
            Log ( "ComputeFunctionSize: WARNING: Invalid graphic type '%0.2s'.", Type ) ;
            SizeOut += 80 ;

         } /* endif */
         break ; }

   } /* endswitch */

   return ( TRUE ) ;
}
 
/****************************************************************************
 *                                                                          *
 *  Validate and compute the internal size of a function token.             *
 *                                                                          *
 *  Returns TRUE if function token valid.                                   *
 *                                                                          *
 ****************************************************************************/

extern BOOL ComputeFunctionSize ( PUCHAR DataIn, long &SizeIn ) {

   // If not a function, return an error.
   if ( *DataIn ) {
      Log ( "ComputeFunctionSize: Not a function object." ) ;
      return ( FALSE ) ;
   } /* endif */

   // If invalid function, return a size of one both ways.
   DataIn ++ ;
   if ( *DataIn >= MAX_FUNCTIONS ) {
      Log ( "ComputeFunctionSize: Not a known function object %02X.", *DataIn ) ;
      return ( FALSE ) ;
   } /* endif */

   // All internal functions have a header, type, backup-up-size and trailer.
   SizeIn = sizeof(UCHAR) + sizeof(UCHAR) + sizeof(TOKEN_BACKUP_TYPE) + sizeof(UCHAR) ;

   // Determine the additional sizes according to the function type.
   switch ( *DataIn++ ) {

      case FN__FONT:
      case FN__DATEFORMAT:
      case FN__AUTHOR:
      case FN__COMMENT:
      case FN__TITLE:
         SizeIn += strlen(PCHAR(DataIn)) + 1 ;
         break ;

      case FN__HEADER:
      case FN__FOOTER:
         SizeIn += DataIn[0] + 1 ;
         break ;

      case FN__LINEJOIN:
      case FN__LINEEND:
      case FN__FILLTYPE:
         SizeIn += sizeof(USHORT) ;
         break ;

      case FN__SIZE:
      case FN__EXPAND:
      case FN__TOPMARGIN:
      case FN__BOTTOMMARGIN:
      case FN__LEFTMARGIN:
      case FN__RIGHTMARGIN:
      case FN__TABSET:
      case FN__TABCLEAR:
      case FN__TABSPACING:
      case FN__INDENT:
      case FN__DATEFIXED:
      case FN__LINESPACING:
      case FN__LINEWIDTH:
      case FN__CREATEDATE:
         SizeIn += sizeof(ULONG) ;
         break ;

      case FN__LINECOLOR:
      case FN__FILLCOLOR:
         SizeIn += sizeof(COLOR) ;
         break ;

      case FN__BOLD:
      case FN__ITALIC:
      case FN__UNDERSCORE:
      case FN__STRIKEOUT:
      case FN__OUTLINE:
      case FN__CAPS:
      case FN__CENTERPAGE:
      case FN__GLYPH:
         SizeIn ++ ;
         break ;

      case FN__LEFTJUSTIFY:
      case FN__CENTERJUSTIFY:
      case FN__RIGHTJUSTIFY:
      case FN__FULLJUSTIFY:
      case FN__DATECURRENT:
      case FN__DATECHANGED:
      case FN__SEPARATOR:
         break ;

      case FN__GRAPHIC: {
         POINTL Position = *PPOINTL(DataIn) ;                   DataIn += sizeof(POINTL) ;      SizeIn += sizeof(POINTL) ;
         SIZEL Size = *PSIZEL(DataIn) ;                         DataIn += sizeof(SIZEL) ;       SizeIn += sizeof(SIZEL) ;
         /* BOOL Background = *DataIn ? TRUE : FALSE ;  */      DataIn ++ ;                     SizeIn ++ ;
         /* BOOL NoTextAside = *DataIn ? TRUE : FALSE ; */      DataIn ++ ;                     SizeIn ++ ;
         /* BOOL Halftone = *DataIn ? TRUE : FALSE ;    */      DataIn ++ ;                     SizeIn ++ ;
         /* BOOL Reverse = *DataIn ? TRUE : FALSE ;     */      DataIn ++ ;                     SizeIn ++ ;
         /* int  Aspect = *DataIn ;                     */      DataIn ++ ;                     SizeIn ++ ;
         /* BOOL PrinterOptimize = *DataIn ? TRUE : FALSE ; */  DataIn ++ ;                     SizeIn ++ ;
         char *Type = PCHAR(DataIn) ;                           DataIn += 2 ;                   SizeIn += 2 ;

         if ( !strnicmp ( Type, "BM", 2 ) ) {           // Bitmap
            DataIn += sizeof(LONG) ;
            SizeIn += sizeof(LONG) ;

         } else if ( !strnicmp ( Type, "MF", 2 ) ) {    // Metafile
            DataIn += sizeof(LONG) ;
            SizeIn += sizeof(LONG) ;

         } else if ( !strnicmp ( Type, "UP", 2 ) ) {    // UPC-A Barcode
            char *UpcCode = PCHAR(DataIn) ;
            DataIn += strlen(UpcCode) + 1 ;
            SizeIn += strlen(UpcCode) + 1 ;

         } else if ( !strnicmp ( Type, "UE", 2 ) ) {    // UPC-E Barcode
            char *UpcCode = PCHAR(DataIn) ;
            DataIn += strlen(UpcCode) + 1 ;
            SizeIn += strlen(UpcCode) + 1 ;

         } else if ( !strnicmp ( Type, "39", 2 ) ) {    // Code-39 Barcode
            char *BarCode = PCHAR(DataIn) ;
            DataIn += strlen(BarCode) + 1 ;
            SizeIn += strlen(BarCode) + 1 ;

         } else if ( !strnicmp ( Type, "PN", 2 ) ) {    // POSTNET Barcode
            char *BarCode = PCHAR(DataIn) ;
            DataIn += strlen(BarCode) + 1 ;
            SizeIn += strlen(BarCode) + 1 ;

         } else if ( !strnicmp ( Type, "E8", 2 ) ) {    // EAN-8 Barcode
            char *BarCode = PCHAR(DataIn) ;
            DataIn += strlen(BarCode) + 1 ;
            SizeIn += strlen(BarCode) + 1 ;

         } else if ( !strnicmp ( Type, "EA", 2 ) ) {    // EAN-13 Barcode
            char *BarCode = PCHAR(DataIn) ;
            DataIn += strlen(BarCode) + 1 ;
            SizeIn += strlen(BarCode) + 1 ;

         } else {
            Log ( "ComputeFunctionSize: WARNING: Invalid graphic type '%0.2s'.", Type ) ;
            DataIn += sizeof(LONG) ;
            SizeIn += sizeof(LONG) ;

         } /* endif */
         break ; }

   } /* endswitch */

   return ( TRUE ) ;
}
 
/****************************************************************************
 *                                                                          *
 *  Get Pointer to Token (and its size) from cooked data.                   *
 *                                                                          *
 ****************************************************************************/

extern void GetToken ( Text *pText, long Offset, PUCHAR &pToken, long &Size ) {

   // Get pointer to data.  If past EOF, return empty token.
   if ( pText->QueryBytePtr ( Offset, pToken ) == 0 ) {
      pToken = 0 ;
      Size = 0 ;
      return ;
   } /* endif */

   // If not function token, return token with size 1.
   if ( *pToken ) {
      Size = 1 ;
      return ;
   } /* endif */

   // Compute function size.
   ComputeFunctionSize ( pToken, Size ) ;
}
 
extern void GetToken ( unsigned char *p, int len, int Index, PUCHAR &pToken, long &Size ) {

   // Get pointer to data.  If past EOF, return empty token.
   if ( Index >= len ) {
      pToken = 0 ;
      Size = 0 ;
      return ;
   } /* endif */

   // If not function token, return token with size 1.
   pToken = p + Index ;
   if ( *pToken ) {
      Size = 1 ;
      return ;
   } /* endif */

   // Compute function size.
   ComputeFunctionSize ( pToken, Size ) ;
}
 
/****************************************************************************
 *                                                                          *
 *  Write Binary Data (escaping where necessary)                            *
 *                                                                          *
 ****************************************************************************/

extern ULONG WriteBinary ( HFILE Handle, PVOID Data, ULONG Size ) {
   ULONG TotalWritten = 0 ;
   for ( int i=0; i<Size; i++ ) {
      ULONG Written ;
      if ( ( PUCHAR(Data)[i] == FUNCTION_ESCAPE ) OR ( PUCHAR(Data)[i] == FUNCTION_START ) OR ( PUCHAR(Data)[i] == FUNCTION_END ) ) {
         UCHAR Byte = FUNCTION_ESCAPE ;
         DosWrite ( Handle, &Byte, 1, &Written ) ;
         TotalWritten ++ ;
      } /* endif */
      DosWrite ( Handle, PUCHAR(Data)+i, 1, &Written ) ;
      TotalWritten ++ ;
   } /* endfor */
   return ( TotalWritten ) ;
}

extern ULONG WriteBinary ( PVOID Destination, PVOID Data, ULONG Size ) {
   ULONG TotalWritten = 0 ;
   for ( int i=0; i<Size; i++ ) {
      if ( ( PUCHAR(Data)[i] == FUNCTION_ESCAPE ) OR ( PUCHAR(Data)[i] == FUNCTION_START ) OR ( PUCHAR(Data)[i] == FUNCTION_END ) ) {
         UCHAR Byte = FUNCTION_ESCAPE ;
         if ( Destination ) {
            *PUCHAR(Destination) = Byte ;
            char *p = (char*) Destination ;  p++ ;  Destination = p ;
         } /* endif */
         TotalWritten ++ ;
      } /* endif */
      if ( Destination ) {
         *PUCHAR(Destination) = PUCHAR(Data)[i] ;
         char *p = (char*) Destination ;  p++ ;  Destination = p ;
      } /* endif */
      TotalWritten ++ ;
   } /* endfor */
   return ( TotalWritten ) ;
}
 
/****************************************************************************
 *                                                                          *
 *  Determine Effective State (from a given position & state)               *
 *                                                                          *
 *    That is, from a given position and state, scan forward past all       *
 *  non-printing codes, updating the state as you go.  The resulting state  *
 *  is the effective state used to determine the line or page's initial     *
 *  options (margins, justification, etc.).                                 *
 *                                                                          *
 ****************************************************************************/

extern void ComputeEffectiveState ( WorkSpace *PS, Text *pText, PUCHAR DBCSVector, long Offset, Estado &State, Estado &EffectiveState, int PageNumber, int NumberOfPages, time_t FileDate ) {

   // Set the initial state.
   EffectiveState = State ;

   // Get the first token.
   PUCHAR pToken ;
   long Size ;
   GetToken ( pText, Offset, pToken, Size ) ;

   // While there are more tokens . . .
   while ( Size ) {

      // If not a function token, or end of word, break out here.
      if ( *pToken OR IsEndOfWord ( pToken, DBCSVector ) )
         break ;

      // Update the effective state according to the function token.
      EffectiveState.ProcessFunction ( pToken, PS, PageNumber, NumberOfPages, FileDate ) ;

      // Advance to the next token.
      Offset += Size ;
      GetToken ( pText, Offset, pToken, Size ) ;

   } /* endwhile */
}
 
extern void ComputeEffectiveState ( WorkSpace *PS, unsigned char *p, int len, PUCHAR DBCSVector, long Offset, _Estado &State, _Estado &EffectiveState ) {

   // Set the initial state.
   EffectiveState = State ;

   // Get the first token.
   PUCHAR pToken ;
   long Size ;
   GetToken ( p, len, Offset, pToken, Size ) ;

   // While there are more tokens . . .
   while ( Size ) {

      // If not a function token, or end of word, break out here.
      if ( *pToken OR IsEndOfWord ( pToken, DBCSVector ) )
         break ;

      // Update the effective state according to the function token.
      EffectiveState.ProcessFunction ( pToken, PS ) ;

      // Advance to the next token.
      Offset += Size ;
      GetToken ( p, len, Offset, pToken, Size ) ;

   } /* endwhile */
}
 
/****************************************************************************
 *                                                                          *
 *  Determine Next State (from a given position & state over a length)      *
 *                                                                          *
 ****************************************************************************/

extern void ComputeNextState ( WorkSpace *PS, Text *pText, long Offset, long Length, Estado &State, Estado &NextState, int PageNumber, int NumberOfPages, time_t FileDate ) {

   // Set the initial state.
   NextState = State ;

   // Get the first token.
   PUCHAR pToken ;
   long Size ;
   GetToken ( pText, Offset, pToken, Size ) ;

   // While there are more tokens . . .
   while ( Size AND Length ) {

      // If function token, then update the effective state according to the function token.
      if ( IsFunction(*pToken) ) 
         NextState.ProcessFunction ( pToken, PS, PageNumber, NumberOfPages, FileDate ) ;

      // Advance to the next token.
      Offset += Size ;  Length -= Size ;

      if ( Length )
         GetToken ( pText, Offset, pToken, Size ) ;

   } /* endwhile */
}

extern void ComputeNextState ( WorkSpace *PS, unsigned char *p, int len, long Offset, long Length, _Estado &State, _Estado &NextState ) {

   // Set the initial state.
   NextState = State ;

   // Get the first token.
   PUCHAR pToken ;
   long Size ;
   GetToken ( p, len, Offset, pToken, Size ) ;

   // While there are more tokens . . .
   while ( Size AND Length ) {

      // If function token, then update the effective state according to the function token.
      if ( IsFunction(*pToken) ) 
         NextState.ProcessFunction ( pToken, PS ) ;

      // Advance to the next token.
      Offset += Size ;  Length -= Size ;

      if ( Length )
         GetToken ( p, len, Offset, pToken, Size ) ;

   } /* endwhile */
}

 
/****************************************************************************
 *                                                                          *
 *  Create Graphic Token from it's parameters (helper function)             *
 *                                                                          *
 ****************************************************************************/

extern long CreateGraphicToken ( // Returns size of token.
   BOOL InternalUseOnly,        // Flag: Is this token for internal use only?
   char *Text,                  // -> Text buffer for external token being created.
   POINTL &Position,            // Graphic position.
   SIZEL &Size,                 // Graphic size.
   BOOL Background,             // Flag: Display graphic as a background?
   BOOL NoTextAside,            // Flag: Prohibit text to either size of graphic?
   BOOL Halftone,               // Flag: Display graphic half-tone?
   BOOL Reverse,                // Flag: Display graphic reverse?
   int Aspect,                  // Aspect preservation type (unfixed, fixX, fixY).
   BOOL PrinterOptimize,        // Flag: Optimize size for printer?
   char *Type,                  // Graphic type: BM, MF, UP, 39, PN.
   long Handle,                 // Handle for BM, MF.
   char *BarCode                // Text for UP, 39, PN.
) {

   char Posx[32], Posy[32], Sizx[32], Sizy[32] ;

   switch ( Position.x ) {
      case GRAPHIC_NEXT_CHAR:           strcpy ( Posx, "Inline" ) ;             break ;
      case GRAPHIC_EDGE_LEFT:           strcpy ( Posx, "LeftEdge" ) ;           break ;
      case GRAPHIC_EDGE_RIGHT:          strcpy ( Posx, "RightEdge" ) ;          break ;
      case GRAPHIC_MARGIN_LEFT:         strcpy ( Posx, "LeftMargin" ) ;         break ;
      case GRAPHIC_MARGIN_RIGHT:        strcpy ( Posx, "RightMargin" ) ;        break ;
      case GRAPHIC_CENTERED:            strcpy ( Posx, "Centered" ) ;           break ;
      default:                          sprintf ( Posx, "%li", Position.x ) ;
   } /* endswitch */

   switch ( Position.y ) {
      case GRAPHIC_EDGE_TOP:            strcpy ( Posy, "TopEdge" ) ;            break ;
      case GRAPHIC_EDGE_BOTTOM:         strcpy ( Posy, "BottomEdge" ) ;         break ;
      case GRAPHIC_MARGIN_TOP:          strcpy ( Posy, "TopMargin" ) ;          break ;
      case GRAPHIC_MARGIN_BOTTOM:       strcpy ( Posy, "BottomMargin" ) ;       break ;
      case GRAPHIC_CENTERED:            strcpy ( Posy, "Centered" ) ;           break ;
      default:                          sprintf ( Posy, "%li", Position.y ) ;
   } /* endswitch */

   switch ( Size.cx ) {
      case GRAPHIC_EDGE_TO_EDGE:        strcpy ( Sizx, "EdgeToEdge" ) ;         break ;
      case GRAPHIC_MARGIN_TO_MARGIN:    strcpy ( Sizx, "MarginToMargin" ) ;     break ;
      default:                          sprintf ( Sizx, "%li", Size.cx ) ;
   } /* endswitch */

   switch ( Size.cy ) {
      case GRAPHIC_EDGE_TO_EDGE:        strcpy ( Sizy, "EdgeToEdge" ) ;         break ;
      case GRAPHIC_MARGIN_TO_MARGIN:    strcpy ( Sizy, "MarginToMargin" ) ;     break ;
      default:                          sprintf ( Sizy, "%li", Size.cy ) ;
   } /* endswitch */

   sprintf ( Text, "%c%s%s,%s,%s,%s", FUNCTION_START, FunctionNames[FN__GRAPHIC], Posx, Posy, Sizx, Sizy ) ;

   if ( Background ) strcat ( Text, ",Background" ) ;

   if ( NoTextAside ) strcat ( Text, ",NoTextAside" ) ;

   if ( Halftone ) strcat ( Text, ",Halftone" ) ;

   if ( Reverse ) strcat ( Text, ",Reverse" ) ;

   if ( Aspect == GRAPHIC_ASPECT_FIX_X ) 
      strcat ( Text, ",KeepAspectFixX" ) ;
   else if ( Aspect == GRAPHIC_ASPECT_FIX_Y )
      strcat ( Text, ",KeepAspectFixY" ) ;

   if ( PrinterOptimize ) strcat ( Text, ",PrinterOptimize" ) ;

   sprintf ( Text+strlen(Text), ":%0.2s", Type ) ;

   long TokenSize = strlen(Text) ;

   if ( !strnicmp ( Type, "BM", 2 ) ) {
      if ( InternalUseOnly ) {
         TokenSize += sprintf ( Text+strlen(Text), "%lu", Handle ) ;
      } else {
         char *p = Text + strlen(Text) + 1 ;
         BITMAPINFOHEADER2 BitmapInfoHeader ;
         BitmapInfoHeader.cbFix = sizeof(BitmapInfoHeader) ;
         GpiQueryBitmapInfoHeader ( Handle, &BitmapInfoHeader ) ;
         ULONG cbBuffer = ( ( ( BitmapInfoHeader.cBitCount * BitmapInfoHeader.cx ) + 31 ) / 32 )
            * 4 * BitmapInfoHeader.cy * BitmapInfoHeader.cPlanes ;
         PUCHAR pbBuffer ;
         DosAllocMem ( PPVOID(&pbBuffer), cbBuffer, PAG_COMMIT | PAG_READ | PAG_WRITE ) ;
         ULONG cbBitmapInfo = sizeof(BITMAPINFO2) + ( sizeof(RGB2) * ( 1 << BitmapInfoHeader.cBitCount ) ) ;
         PBITMAPINFO2 pbmi ;
         DosAllocMem ( PPVOID(&pbmi), cbBitmapInfo, PAG_COMMIT | PAG_READ | PAG_WRITE ) ;
         memcpy ( pbmi, &BitmapInfoHeader, sizeof(BitmapInfoHeader) ) ;
         DeviceContext Memory = DeviceContext ( "CreateGraphicToken", long(0) ) ;
         WorkSpace PS ( "CreateGraphicToken", 0, &Memory, 0, TRUE ) ;
         PS.SetBitmap ( Handle ) ;
         PS.QueryBitmapBits ( 0, int(BitmapInfoHeader.cy), pbBuffer, pbmi ) ;
         p += WriteBinary ( p, &cbBitmapInfo, sizeof(cbBitmapInfo) ) ;
         p += WriteBinary ( p, &cbBuffer, sizeof(cbBuffer) ) ;
         p += WriteBinary ( p, pbmi, cbBitmapInfo ) ;
         p += WriteBinary ( p, pbBuffer, cbBuffer ) ;
         PS.SetBitmap ( 0 ) ;
         DosFreeMem ( pbmi ) ;
         DosFreeMem ( pbBuffer ) ;
         TokenSize = p - Text ;
      } /* endif */

   } else if ( !strnicmp ( Type, "MF", 2 ) ) {
      if ( InternalUseOnly ) {
         TokenSize += sprintf ( Text+strlen(Text), "%lu", Handle ) ;
      } else {
         char *p = Text + strlen(Text) + 1 ;
         LONG cbBuffer = GpiQueryMetaFileLength ( HMF(Handle) ) ;
         PUCHAR pBuffer ;
         DosAllocMem ( (PPVOID)&pBuffer, cbBuffer, PAG_COMMIT | PAG_READ | PAG_WRITE ) ;
         if ( GpiQueryMetaFileBits ( HMF(Handle), 0, cbBuffer, (PBYTE)pBuffer ) ) {
            p += WriteBinary ( p, &cbBuffer, sizeof(cbBuffer) ) ;
            p += WriteBinary ( p, pBuffer, cbBuffer ) ;
         } /* endif */
         DosFreeMem ( pBuffer ) ;
         TokenSize = p - Text ;
      } /* endif */

   } else if ( !strnicmp ( Type, "UP", 2 ) ) {
      TokenSize += sprintf ( Text+strlen(Text), "%0.13s", BarCode ) ;

   } else if ( !strnicmp ( Type, "UE", 2 ) ) {
      TokenSize += sprintf ( Text+strlen(Text), "%0.7s", BarCode ) ;

   } else if ( !strnicmp ( Type, "39", 2 ) ) {
      TokenSize += sprintf ( Text+strlen(Text), "%0.80s", BarCode ) ;

   } else if ( !strnicmp ( Type, "PN", 2 ) ) {
      TokenSize += sprintf ( Text+strlen(Text), "%0.80s", BarCode ) ;

   } else if ( !strnicmp ( Type, "E8", 2 ) ) {
      TokenSize += sprintf ( Text+strlen(Text), "%0.7s", BarCode ) ;

   } else if ( !strnicmp ( Type, "EA", 2 ) ) {
      TokenSize += sprintf ( Text+strlen(Text), "%0.12s", BarCode ) ;

   } else {
      Log ( "CreateGraphicToken: WARNING: Invalid graphic type '%0.2s'.", Type ) ;
      TokenSize ++ ;

   } /* endif */

   TokenSize += sprintf ( Text+TokenSize, "%c", FUNCTION_END ) ;

   return ( TokenSize ) ;
}

        // Easier version to call for use with UpdateGraphic and CopyGraphic.

extern void CreateGraphicToken ( char Text[], Grafico &Graphic ) {
   CreateGraphicToken ( TRUE, Text, Graphic.Position, Graphic.Size, Graphic.Background,
      Graphic.NoTextAside, Graphic.Halftone, Graphic.Reverse, Graphic.Aspect, 
      Graphic.PrinterOptimize, Graphic.Type, Graphic.Handle, Graphic.BarCode ) ;
}


