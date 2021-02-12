/******************************************************************** BMP.CPP
 *                                                                          *
 *        OS/2 & Windows Bitmap Graphic Import Add-on for Escriba           *
 *                                                                          *
 ****************************************************************************/

#include "System.h"

#include <direct.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "External.h"

#include "Debug.h"
#include "Dialog.h"
#include "HelpWin.h"
#include "LModule.h"
#include "Module.h"
#include "ReString.h"

#include "Bmp.h"

// #define DEBUG

/****************************************************************************
 *                                                                          *
 *                       Definitions & Declarations                         *
 *                                                                          *
 ****************************************************************************/

  // Macros & Constants 

#define PROGRAM_NAME    "BMP"
#define LANGUAGE_NAME   "BMP__*.DLL"


  // Function Prototypes 

extern "C" {
   extern int _CRT_init ( void ) ;
}

  // Globals (private to the library, created by library instance) 

extern char HomePath [_MAX_PATH+1] = { 0 } ;
static class LanguageModule *Library = 0 ;
static HMODULE LibraryHandle = 0 ;

static char ErrorMessage [500] ;


/****************************************************************************
 *                                                                          *
 *                    Log Message (Debugging Function)                      *
 *                                                                          *
 ****************************************************************************/

extern void Log ( char *Message, ... ) {

  /**************************************************************************
   * Open the log file.                                                     *
   **************************************************************************/

   char LogFile [_MAX_PATH] ;
   strcpy ( LogFile, HomePath ) ;
   strcat ( LogFile, "\\" PROGRAM_NAME ".log" ) ;
 
   FILE *File = fopen ( LogFile, "a" ) ;

  /**************************************************************************
   * If the file got opened, write the message to the log file and close it.*
   **************************************************************************/

   if ( File ) {
      char Time [9], Date [9] ;
      fprintf ( File, "%s %s " REVISION " %02i ", _strtime(Time), _strdate(Date), *_threadid ) ;
      va_list Marker ;
      va_start ( Marker, Message ) ;
      vfprintf ( File, Message, Marker ) ;
      va_end ( Marker ) ;
      fprintf ( File, "\n" ) ;
      fclose ( File ) ;
   } /* endif */
}


/****************************************************************************
 *                                                                          *
 *                       DLL Initialization Function                        *
 *                                                                          *
 ****************************************************************************/

extern unsigned long _System _DLL_InitTerm ( unsigned long modhandle, unsigned long flag ) {

  /**************************************************************************
   * If starting up (since cleanup will not be called this way) . . .       *
   **************************************************************************/

   if ( flag == 0 ) {

     /***********************************************************************
      * Call C RunTime initialization.  Abort if error.                     *
      ***********************************************************************/

      if ( _CRT_init() == -1 )
         return ( 0 ) ;

     /***********************************************************************
      * Save the library handle.                                            *
      ***********************************************************************/

      LibraryHandle = modhandle ;

     /***********************************************************************
      * Find out where this module was loaded from.                         *
      ***********************************************************************/

      DosQueryModuleName ( LibraryHandle, sizeof(HomePath), HomePath ) ;
      char Drive[_MAX_DRIVE], Dir[_MAX_DIR], Fname[_MAX_FNAME], Ext[_MAX_EXT] ;
      _splitpath ( HomePath, Drive, Dir, Fname, Ext ) ;
      if ( Dir[strlen(Dir)-1] == '\\' )
         Dir[strlen(Dir)-1] = 0 ;
      strcpy ( HomePath, Drive ) ;
      strcat ( HomePath, Dir ) ;

  /**************************************************************************
   * Else, if shutting down . . .                                           *
   **************************************************************************/

   } else {

     /***********************************************************************
      * Delete the language module, if one's been created.                  *
      ***********************************************************************/

      if ( Library ) 
         delete Library, Library = 0 ;

   } /* endif */

  /**************************************************************************
   * Return NZ, indicating success.                                         *
   **************************************************************************/

   return ( 1 ) ;
}


/****************************************************************************
 *                                                                          *
 *  Identify DLL type.                                                      *
 *                                                                          *
 ****************************************************************************/

extern char* APIENTRY Identify ( void ) {
   return ( TYPE_GRAPHICIMPORT );
}


/****************************************************************************
 *                                                                          *
 *  Set Language (must be called before anything using Library)             *
 *                                                                          *
 ****************************************************************************/

extern BOOL APIENTRY SetLanguage ( char *Language ) {
   return ( _SetLanguage ( PROGRAM_NAME, REVISION, IDS_TITLE1, Language, LANGUAGE_NAME, HomePath, Library ) ) ;
}


/****************************************************************************
 *                                                                          *
 *  Give name/title.                                                        *
 *                                                                          *
 ****************************************************************************/

extern char* APIENTRY ProductName ( void ) {

   if ( Library == 0 ) {
      Log ( "ProductName aborted because SetLanguage not yet called." ) ;
      return ( "" ) ;
   } /* endif */

   ResourceString Name ( Library->QueryHandle(), IDS_TITLE ) ;

   static char Buffer [200] ;
   strcpy ( Buffer, PCHAR(Name) ) ;

   return ( Buffer ) ;
}


/****************************************************************************
 *                                                                          *
 *  Identify DLL version.  Returned in x100 form.                           *
 *                                                                          *
 ****************************************************************************/

extern int APIENTRY ProductVersion ( void ) {
   return ( REVISION_NO );
}


/****************************************************************************
 *                                                                          *
 *  Product Information Dialog                                              *
 *                                                                          *
 ****************************************************************************/

class AboutDialog : public Dialog {

   private:
      HelpWindow *Help ;

   public:

      AboutDialog ( HWND Parent, HWND Owner, HMODULE Resource, int Id ) : Dialog ( Parent, Owner, Resource, Id ), Help(0) {
         if ( !Load ( ) ) 
            return ;
         char Drive [_MAX_DRIVE+1], Dir[_MAX_DIR+1], Fname[_MAX_FNAME+1], Ext[_MAX_EXT+1] ;
         _splitpath ( Library->QueryName(), Drive, Dir, Fname, Ext ) ;
         char HelpFileName [CCHMAXPATH] ;
         sprintf ( HelpFileName, "%s\\%s.hlp", HomePath, Fname ) ;
         ResourceString HelpTitle ( Library->QueryHandle(), IDS_HELPTITLE ) ;
         Help = new HelpWindow ( 0, LibraryHandle, ID_HELPTABLE, PSZ(HelpFileName), PSZ(HelpTitle) ) ;
         Help->Associate ( Handle ) ;
      } /* endmethod */

      ~AboutDialog ( ) {
         if ( Help ) 
            delete Help ;
      } /* endmethod */
} ;

extern void APIENTRY ProductInformation ( HWND Owner ) {
   AboutDialog About ( HWND_DESKTOP, Owner, Library->QueryHandle(), IDD_PRODUCTINFO ) ;
   About.Process ( ) ;
}


/****************************************************************************
 *                                                                          *
 *  Query Filename EA Type                                                  *
 *                                                                          *
 ****************************************************************************/

extern char* APIENTRY QueryFileType ( void ) {
   return ( "Bitmap" ) ;
}


/****************************************************************************
 *                                                                          *
 *  Query Filename Wildcard                                                 *
 *                                                                          *
 ****************************************************************************/

extern char* APIENTRY QueryWildcard ( void ) {
   return ( "*.BMP" ) ;
}


/****************************************************************************
 *                                                                          *
 *  Filter Filename & Type                                                  *
 *                                                                          *
 ****************************************************************************/

static BOOL MatchWildcard ( char *Name, char *Wildcard ) {
   char Target [80] = { 0 } ;
   if ( !DosEditName ( 1, Name, Wildcard, Target, sizeof(Target) ) ) 
      if ( !stricmp ( Name, Target ) ) 
         return ( TRUE ) ;
   return ( FALSE ) ;
}

extern BOOL APIENTRY Filter ( char *Name, char *Type ) {

   // If extended type is set properly, return TRUE at once.
   if ( !stricmp ( Type, QueryFileType() ) )
      return ( TRUE ) ;

   // Match against *.BMP.
   if ( MatchWildcard ( Name, "*.BMP" ) )
      return ( TRUE ) ;

   // Otherwise, return FALSE.
   return ( FALSE ) ;
}


/****************************************************************************
 *                                                                          *
 *  Verify File by Inspection                                               *
 *                                                                          *
 ****************************************************************************/

extern BOOL APIENTRY Verify ( HWND Owner, char *Name ) {

   // Open file for verification.
   FILE *File = fopen ( Name, "rb" ) ;
   if ( File == 0 ) {
      if ( Owner ) {
         ResourceString Title ( Library->QueryHandle(), IDS_TITLE ) ;
         ResourceString Format ( Library->QueryHandle(), IDS_ERROR_OPEN_FOR_VERIFY ) ;
         char Message [500] ;
         sprintf ( Message, PCHAR(Format), Name ) ;
         Sys_MessageBox ( HWND_DESKTOP, Owner, PSZ(Message), PSZ(Title), 0, MB_ENTER | MB_ICONEXCLAMATION ) ;
      } /* endif */
      return ( FALSE ) ;
   } /* endif */

   // Get the file size.  If not big enough for header, return an error.
   fseek ( File, 0, SEEK_END ) ;
   long Size = ftell ( File ) ;
   if ( Size < sizeof(BITMAPFILEHEADER) ) {
      fclose ( File ) ;
      if ( Owner ) {
         ResourceString Title ( Library->QueryHandle(), IDS_TITLE ) ;
         ResourceString Message ( Library->QueryHandle(), IDS_ERROR_FILE_TOO_SMALL ) ;
         Sys_MessageBox ( HWND_DESKTOP, Owner, PSZ(Message), PSZ(Title), 0, MB_ENTER | MB_ICONEXCLAMATION ) ;
      } /* endif */
      return ( FALSE ) ;
   } /* endif */

   // Get the image header.
   BITMAPFILEHEADER Header ;
   fseek ( File, 0, SEEK_SET ) ;
   fread ( &Header, 1, sizeof(Header), File ) ;

   // Validate the file type.
   if ( Header.usType != BFT_BMAP ) {
      fclose ( File ) ;
      if ( Owner ) {
         ResourceString Title ( Library->QueryHandle(), IDS_TITLE ) ;
         ResourceString Message ( Library->QueryHandle(), IDS_ERROR_INVALID_HEADER_ID ) ;
         Sys_MessageBox ( HWND_DESKTOP, Owner, PSZ(Message), PSZ(Title), 0, MB_ENTER | MB_ICONEXCLAMATION ) ;
      } /* endif */
      return ( FALSE ) ;
   } /* endif */

   // Get pointers to the bitmap header and to the palette.
   char *pHeader = (char*) &Header.bmp ;
   ULONG HeaderSize = * (PULONG) pHeader ;

   // Get the bitmap dimensions according to the bitmap file version.
   switch ( HeaderSize ) {
      case 12:          // OS/2 1.x official
      case 16:          // Windows 3.0 variant
      case 40:          // Windows 3.0 official
      case 36:          // OS/2 2.x variant
      case 64:          // OS/2 2.x official
         break;
      default: 
         fclose ( File ) ;
         if ( Owner ) {
            ResourceString Title ( Library->QueryHandle(), IDS_TITLE ) ;
            ResourceString Format ( Library->QueryHandle(), IDS_ERROR_INVALID_HEADER_SIZE ) ;
            sprintf ( ErrorMessage, PCHAR(Format), HeaderSize ) ;
            Sys_MessageBox ( HWND_DESKTOP, Owner, PSZ(ErrorMessage), PSZ(Title), 0, MB_ENTER | MB_ICONEXCLAMATION ) ;
         } /* endif */
         return ( FALSE ) ;
   } /* endswitch */

   // Close up the file.
   fclose ( File ) ;

   // Return success.  It's still possible the file is invalid . . .
   return ( TRUE ) ;
}


/****************************************************************************
 *                                                                          *
 *  Build OS/2 Bitmap Data Buffer and Palette from BMP data.                *
 *                                                                          *
 *  Returns NZ if error.                                                    *
 *                                                                          *
 ****************************************************************************/

static int BMP_BuildBitmapData (
   PVOID Data,                  // -> Raw BMP data.
   ULONG /* Size */,            // BMP Data size.
   PCHAR *Buffer,               // -> pBuffer (allocated by this function)
   RGB2 Colors[],               // -> Colors[] (allocated by caller to 256 colors max)
   int *cx,                     // -> cx (image width)
   int *cy,                     // -> cy (image height)
   int *BitsPerPixel,           // -> Bits per Pixel
   int *PaddedLineWidth,        // -> Padded raster line width (for reference)
   int *Compression,            // -> Compression type
   int *ImageByteCount          // -> Image byte count
) {

   // Local Declarations
   ULONG                HeaderSize ;
   int                  i ;
   int                  PaletteEntrySize ;
   int                  PaletteSize ;
   PBITMAPFILEHEADER    pFileHeader = (PBITMAPFILEHEADER) Data ;
   char                *pHeader ;

   // Validate the file type.
   if ( pFileHeader->usType != BFT_BMAP ) {
      ResourceString Message ( Library->QueryHandle(), IDS_ERROR_INVALID_HEADER_ID ) ;
      sprintf ( ErrorMessage, PCHAR(Message) ) ;
      return ( 1 ) ;
   } /* endif */

   // Get pointers to the bitmap header and to the palette.
   pHeader = (char*) &pFileHeader->bmp ;
   HeaderSize = * (PULONG) pHeader ;

   // Get the bitmap dimensions according to the bitmap file version.
   switch ( HeaderSize ) {
      case 12:          // OS/2 1.x official
         *cx = * (PUSHORT) ( pHeader + sizeof(ULONG) ) ;
         *cy = * (PUSHORT) ( pHeader + sizeof(ULONG) + sizeof(USHORT) ) ;
         *BitsPerPixel = * (PUSHORT) ( pHeader + sizeof(ULONG) + sizeof(USHORT)*3 ) ;
         *Compression = 0 ;
         *ImageByteCount = 0 ;
         PaletteEntrySize = sizeof(RGB) ;
         break;
      case 16:          // Windows 3.0 variant
         *cx = int ( * (PULONG) ( pHeader + sizeof(ULONG) ) ) ;
         *cy = int ( * (PULONG) ( pHeader + sizeof(ULONG)*2 ) ) ;
         *BitsPerPixel = * (PUSHORT) ( pHeader + sizeof(ULONG)*3 + sizeof(USHORT) ) ;
         *Compression = 0 ;
         *ImageByteCount = 0 ;
         PaletteEntrySize = sizeof(RGB2) ;
         break;
      case 40:          // Windows 3.0 official
         *cx = int ( * (PULONG) ( pHeader + sizeof(ULONG) ) ) ;
         *cy = int ( * (PULONG) ( pHeader + sizeof(ULONG)*2 ) ) ;
         *BitsPerPixel = * (PUSHORT) ( pHeader + sizeof(ULONG)*3 + sizeof(USHORT) ) ;
         *Compression = int ( * (PULONG) ( pHeader + sizeof(ULONG)*3 + sizeof(USHORT)*2 ) ) ;
         *ImageByteCount = int ( * (PULONG) ( pHeader + sizeof(ULONG)*4 + sizeof(USHORT)*2 ) ) ;
         PaletteEntrySize = sizeof(RGB2) ;
         break;
      case 36:          // OS/2 2.x variant
      case 64:          // OS/2 2.x official
         *cx = int ( * (PULONG) ( pHeader + sizeof(ULONG) ) ) ;
         *cy = int ( * (PULONG) ( pHeader + sizeof(ULONG)*2 ) ) ;
         *BitsPerPixel = * (PUSHORT) ( pHeader + sizeof(ULONG)*3 + sizeof(USHORT) ) ;
         *Compression = int ( * (PULONG) ( pHeader + sizeof(ULONG)*3 + sizeof(USHORT)*2 ) ) ;
         *ImageByteCount = int ( * (PULONG) ( pHeader + sizeof(ULONG)*4 + sizeof(USHORT)*2 ) ) ;
         PaletteEntrySize = sizeof(RGB2) ;
         break;
      default: 
         ResourceString Message ( Library->QueryHandle(), IDS_ERROR_INVALID_HEADER_SIZE ) ;
         sprintf ( ErrorMessage, PCHAR(Message), HeaderSize ) ;
         return ( 1 ) ;
   } /* endswitch */

   // Determine palette size.
   PaletteSize = 1 << *BitsPerPixel ;
   if ( *BitsPerPixel > 8 )
      PaletteSize = 0 ;

   // Determine the bitmap buffer line length.
   *PaddedLineWidth = ( *cx * *BitsPerPixel ) / 8 ;
   if ( ( *cx * *BitsPerPixel ) % 8 ) (*PaddedLineWidth) ++ ;
   while ( *PaddedLineWidth % 4 ) (*PaddedLineWidth) ++ ;

   // Allocate the image buffer.
   *Buffer = (char*) malloc ( *PaddedLineWidth * *cy ) ;
   if ( *Buffer == 0 ) {
      ResourceString Message ( Library->QueryHandle(), IDS_ERROR_MEMORY1 ) ;
      sprintf ( ErrorMessage, PCHAR(Message) ) ;
      return ( 1 ) ;
   } /* endif */

   // Save the image data.
   memcpy ( *Buffer, (PCHAR)Data+pFileHeader->offBits, *PaddedLineWidth * *cy ) ;

   // Build the palette, if there is one.
   if ( PaletteSize ) {
      PUCHAR pBits = (PUCHAR) ( pHeader + HeaderSize ) ;
      PRGB2 pColor = Colors ;
      for ( i=0; i<PaletteSize; i++, pColor++ ) {
         pColor->fcOptions = 0 ;
         pColor->bBlue  = pBits[i*PaletteEntrySize+0] ;
         pColor->bGreen = pBits[i*PaletteEntrySize+1] ;
         pColor->bRed   = pBits[i*PaletteEntrySize+2] ;
      } /* endfor */
   } /* endif */

   // Return no error if we got this far.
   return ( 0 ) ;
}


/****************************************************************************
 *                                                                          *
 *                     Build OS/2 Bitmap from BMP data                      *
 *                                                                          *
 *  NOTE: If *Palette is set to zero before the function call, the palette  *
 *    in the BMP file will be used.  If not, the palette provided by the    *
 *    caller will be used.                                                  *
 *                                                                          *
 ****************************************************************************/

static HBITMAP CreateBitmapFromBMP ( HAB Anchor, PVOID Data, ULONG Size, PHPAL Palette, BOOL FixedPalette ) {

   // Local Declarations
   HBITMAP              Bitmap = 0 ;
   PBITMAPINFO2         BitmapInfo ;
   int                  BitmapInfoSize ;
   int                  BitsPerPixel ;
   char                *Buffer ;
   RGB2                 Colors [256] ;
   LONG                 ColorCount [256] ;
   int                  Compression ;
   int                  cx, cy ;
   int                  DataShift ;
   int                  DataShift0 ;
   int                  i, j ;
   int                  ImageByteCount ;
   UCHAR                 Mask ;
   UCHAR                 Mask0 ;
   int                  MaskShift ;
   HDC                  MemoryDC ;
   HPS                  MemoryPS ;
   int                  PaddedLineWidth ;
   SIZEL                PageSize = { 0, 0 } ;
   int                  PaletteSize ;
   PSZ                  pszData [] = { 0, (PSZ)"Display" } ;
   int                  x, y ;

   // Obtain the image data.
   if ( BMP_BuildBitmapData ( Data, Size, &Buffer, Colors, &cx, &cy, 
      &BitsPerPixel, &PaddedLineWidth, &Compression, &ImageByteCount ) )
      return ( 0 ) ;

   // Determine palette size.
   PaletteSize = 1 << BitsPerPixel ;
   if ( BitsPerPixel > 8 )
      PaletteSize = 0 ;

   // Create the bitmap header.
   BitmapInfoSize = sizeof(BITMAPINFOHEADER2) + PaletteSize * sizeof(RGB2) ;
   BitmapInfo = (PBITMAPINFO2) malloc ( BitmapInfoSize ) ;
   if ( BitmapInfo == 0 ) {
      ResourceString Message ( Library->QueryHandle(), IDS_ERROR_MEMORY2 ) ;
      strcpy ( ErrorMessage, PCHAR(Message) ) ;
      free ( Buffer ) ;
      return ( 0 ) ;
   } /* endif */

   // Initialize the bitmap header (except the palette).
   memset ( BitmapInfo, 0, BitmapInfoSize ) ;
   BitmapInfo->cbFix = 24 ;
   BitmapInfo->cPlanes = 1 ;
   BitmapInfo->cBitCount = USHORT ( BitsPerPixel ) ;
   BitmapInfo->cx = cx ;
   BitmapInfo->cy = cy ;
   BitmapInfo->ulCompression = Compression ;
   BitmapInfo->cbImage = ImageByteCount ;
   if ( PaletteSize ) 
      memcpy ( (PUCHAR)BitmapInfo+BitmapInfo->cbFix, Colors, PaletteSize*sizeof(RGB2) ) ;

   // If we need to make or update a palette . . .
   if ( ! FixedPalette && PaletteSize ) {

      // Determine color counts.
      memset ( ColorCount, 0, sizeof(ColorCount) ) ;
      switch ( BitsPerPixel ) {
         case 8:  Mask0 = (UCHAR)0xFF ;  MaskShift = 0 ;  DataShift0 = 0 ;  break ;
         case 4:  Mask0 = (UCHAR)0xF0 ;  MaskShift = 4 ;  DataShift0 = 4 ;  break ;
         case 2:  Mask0 = (UCHAR)0xC0 ;  MaskShift = 2 ;  DataShift0 = 6 ;  break ;
         case 1:  Mask0 = (UCHAR)0x80 ;  MaskShift = 1 ;  DataShift0 = 7 ;  break ;
      } /* endswitch */
      for ( y=0; y<cy; y++ ) {
         char *pBuffer = Buffer + PaddedLineWidth * y ;
         Mask = Mask0 ;
         DataShift = DataShift0 ;
         for ( x=0; x<cx; x++ ) {
            UCHAR Pixel = (UCHAR) ( ( *pBuffer & Mask ) >> DataShift ) ;
            ColorCount [ Pixel ] ++ ;
            if ( MaskShift ) {
               Mask >>= MaskShift ;
               DataShift -= MaskShift ;
               if ( DataShift < 0 ) {
                  pBuffer ++ ;
                  Mask = Mask0 ;
                  DataShift = DataShift0 ;
               } /* endif */
            } else {
               pBuffer ++ ;
            } /* endif */
         } /* endfor */
      } /* endfor */

      // Sort the colors, placing the most-used colors first.
      for ( i=0; i<PaletteSize-1; i++ ) {
         for ( j=i+1; j<PaletteSize; j++ ) {
            if ( ColorCount[i] < ColorCount[j] ) {
               int Count = int ( ColorCount[j] ) ;
               RGB2 Color = Colors[j] ;
               ColorCount[j] = ColorCount[i] ;
               ColorCount[i] = Count ;
               Colors[j] = Colors[i] ;
               Colors[i] = Color ;
            } /* endif */
         } /* endfor */
      } /* endfor */

      // Determine how many colors are actually in the palette.
      while ( PaletteSize && ( ColorCount[PaletteSize-1] == 0 ) ) PaletteSize -- ;

      for ( int i=0; i<PaletteSize; i++ ) 
         Log ( "Bitmap palette entry %03i: %02X %02X %02X", i, PRGB2(Colors+i)->bRed, PRGB2(Colors+i)->bGreen, PRGB2(Colors+i)->bBlue ) ;

      // If palette already exists, replace it, adding the newly required colors.
      if ( *Palette ) {
         SIZEL PageSize = { 0, 0 } ;
         HPS hPS = GpiCreatePS ( Anchor, 0, &PageSize, PU_PELS ) ;
         long OldPaletteSize = GpiQueryPaletteInfo ( *Palette, hPS, 0, 0, 0, 0 ) ;
         PULONG PaletteColors = (PULONG) malloc ( size_t ( (OldPaletteSize+PaletteSize) * sizeof(ULONG) ) ) ;
         GpiQueryPaletteInfo ( *Palette, hPS, 0, 0, OldPaletteSize, PaletteColors ) ;
         GpiDestroyPS ( hPS ) ;
         memcpy ( PaletteColors+OldPaletteSize, Colors, PaletteSize*sizeof(RGB2) ) ;
         *Palette = GpiCreatePalette ( Anchor, 0, LCOLF_CONSECRGB, OldPaletteSize+PaletteSize, (PULONG)PaletteColors ) ;
         free ( PaletteColors ) ;

      // If not, create a new one.
      } else {
         *Palette = GpiCreatePalette ( Anchor, 0, LCOLF_CONSECRGB, PaletteSize, (PULONG)Colors ) ;
 
      } /* endif palette already exists or doesn't */

   } /* endif creating or updating a palette */

   // Open a memory device context compatible with the video display.
   MemoryDC = DevOpenDC ( Anchor, OD_MEMORY, (PSZ)"*", 2, (PDEVOPENDATA)pszData, 0 ) ;
   if ( MemoryDC == 0 ) {
      ERRORID Error = WinGetLastError ( Anchor ) ;
      ResourceString Format ( Library->QueryHandle(), IDS_ERROR_DC_OPEN ) ;
      sprintf ( ErrorMessage, PCHAR(Format), Error ) ;
      free ( BitmapInfo ) ;
      free ( Buffer ) ;
      return ( 0 ) ;
   } /* endif */

   // Create a memory presentation space associated with the memory device context.
   MemoryPS = GpiCreatePS ( Anchor, MemoryDC, &PageSize, PU_PELS | GPIA_ASSOC ) ;
   if ( MemoryPS == 0 ) {
      ERRORID Error = WinGetLastError ( Anchor ) ;
      ResourceString Format ( Library->QueryHandle(), IDS_ERROR_PS_CREATE ) ;
      sprintf ( ErrorMessage, PCHAR(Format), Error ) ;
      DevCloseDC ( MemoryDC ) ;
      free ( BitmapInfo ) ;
      free ( Buffer ) ;
      return ( 0 ) ;
   } /* endif */

   // Set the color mapping, if requested.
   if ( Palette ) 
      GpiSelectPalette ( MemoryPS, *Palette ) ;

   // Create and load the bitmap.
   Bitmap = GpiCreateBitmap ( MemoryPS, (PBITMAPINFOHEADER2)BitmapInfo, CBM_INIT, (PBYTE)Buffer, BitmapInfo ) ;
   if ( Bitmap == 0 ) {
      ERRORID Error = WinGetLastError ( Anchor ) ;
      ResourceString Format ( Library->QueryHandle(), IDS_ERROR_CREATE_BITMAP ) ;
      sprintf ( ErrorMessage, PCHAR(Format), Error ) ;
   } /* endif */

   // Destroy the memory presentation space and device context.
   GpiSelectPalette ( MemoryPS, 0 ) ;
   GpiDestroyPS ( MemoryPS ) ;
   DevCloseDC ( MemoryDC ) ;

   // Release the memory allocated.
   free ( BitmapInfo ) ;
   free ( Buffer ) ;

   // Return the bitmap handle.
   return ( Bitmap ) ;
}
 
/****************************************************************************
 *                                                                          *
 *  Import File                                                             *
 *                                                                          *
 ****************************************************************************/

extern HBITMAP APIENTRY GraphicImport ( HWND Owner, char *Name ) {

   // Load file PSZ(DialogData.szFullFile).
   FILE *File = fopen ( Name, "rb" ) ;
   if ( File == 0 ) {
      ResourceString Title ( Library->QueryHandle(), IDS_TITLE ) ;
      ResourceString Format ( Library->QueryHandle(), IDS_ERROR_OPEN_FOR_IMPORT ) ;
      char Message [500] ;
      sprintf ( Message, PCHAR(Format), Name ) ;
      Sys_MessageBox ( HWND_DESKTOP, Owner, PSZ(Message), PSZ(Title), 0, MB_ENTER | MB_ICONEXCLAMATION ) ;
      return ( 0 ) ;
   } /* endif */

   fseek ( File, 0, SEEK_END ) ;
   long Size = ftell ( File ) ;
   void *pData = malloc ( (size_t)Size ) ;
   if ( pData == 0 ) {
      ResourceString Title ( Library->QueryHandle(), IDS_TITLE ) ;
      ResourceString Format ( Library->QueryHandle(), IDS_ERROR_MEMORY3 ) ;
      char Message [500] ;
      sprintf ( Message, PCHAR(Format), Size ) ;
      Sys_MessageBox ( HWND_DESKTOP, Owner, PSZ(Message), PSZ(Title), 0, MB_ENTER | MB_ICONEXCLAMATION ) ;
      return ( 0 ) ;
   } /* endif */

   fseek ( File, 0, SEEK_SET ) ;
   fread ( pData, 1, size_t(Size), File ) ;
   fclose ( File ) ;

   HBITMAP Bitmap = CreateBitmapFromBMP ( WinQueryAnchorBlock(Owner), pData, Size, 0, TRUE ) ;

   free ( pData ) ;

   if ( Bitmap == 0 ) {
      ResourceString Title ( Library->QueryHandle(), IDS_TITLE ) ;
      Sys_MessageBox ( HWND_DESKTOP, Owner, PSZ(ErrorMessage), PSZ(Title), 0, MB_ENTER | MB_ICONEXCLAMATION ) ;
   } /* endif */

   return ( Bitmap ) ;
}
