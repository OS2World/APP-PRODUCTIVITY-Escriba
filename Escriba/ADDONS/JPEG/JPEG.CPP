/******************************************************************* JPEG.CPP
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

#include "JPEG.h"

// #define DEBUG

/****************************************************************************
 *                                                                          *
 *                       Definitions & Declarations                         *
 *                                                                          *
 ****************************************************************************/

  // Macros & Constants 

#define PROGRAM_NAME    "JPEG"
#define LANGUAGE_NAME   "JPEG_*.DLL"


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
   strcat ( LogFile, PROGRAM_NAME ".log" ) ;
 
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
   return ( "JPG" ) ;
}


/****************************************************************************
 *                                                                          *
 *  Query Filename Wildcard                                                 *
 *                                                                          *
 ****************************************************************************/

extern char* APIENTRY QueryWildcard ( void ) {
   return ( "*.JPG,*.JPEG,*.JFI,*.JFIF" ) ;
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

   // Match against *.JPG
   if ( MatchWildcard ( Name, "*.JPG" ) )
      return ( TRUE ) ;

   // Match against *.JPEG
   if ( MatchWildcard ( Name, "*.JPEG" ) )
      return ( TRUE ) ;

   // Match against *.JFI
   if ( MatchWildcard ( Name, "*.JFI" ) )
      return ( TRUE ) ;

   // Match against *.JFIF
   if ( MatchWildcard ( Name, "*.JFIF" ) )
      return ( TRUE ) ;

   // Otherwise, return FALSE.
   return ( FALSE ) ;
}


/****************************************************************************
 *                                                                          *
 *  Verify File by Inspection                                               *
 *                                                                          *
 ****************************************************************************/

typedef struct {
   BYTE SOI [2] ;
   BYTE APP0 [2] ;
   BYTE Length [2] ;
   BYTE Identifier [5] ;
   BYTE Version [2] ;
   BYTE Units ;
   BYTE Xdensity [2] ;
   BYTE Ydentity [2] ;
   BYTE XThumbnail ;
   BYTE YThumbnail ;
} JFIFHEAD ;

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
   if ( Size < sizeof(JFIFHEAD) ) {
      fclose ( File ) ;
      if ( Owner ) {
         ResourceString Title ( Library->QueryHandle(), IDS_TITLE ) ;
         ResourceString Message ( Library->QueryHandle(), IDS_ERROR_FILE_TOO_SMALL ) ;
         Sys_MessageBox ( HWND_DESKTOP, Owner, PSZ(Message), PSZ(Title), 0, MB_ENTER | MB_ICONEXCLAMATION ) ;
      } /* endif */
      return ( FALSE ) ;
   } /* endif */

   // Get the image header.
   JFIFHEAD Header ;
   fseek ( File, 0, SEEK_SET ) ;
   fread ( &Header, 1, sizeof(Header), File ) ;

   // Verify that this is a JPEG/JFIF file.
   if ( ( Header.SOI[0] != 0xFF ) || ( Header.SOI[1] != 0xD8 ) ) {
      fclose ( File ) ;
      if ( Owner ) {
         ResourceString Title ( Library->QueryHandle(), IDS_TITLE ) ;
         ResourceString Message ( Library->QueryHandle(), IDS_ERROR_INVALID_FILE_ID ) ;
         Sys_MessageBox ( HWND_DESKTOP, Owner, PSZ(Message), PSZ(Title), 0, MB_ENTER | MB_ICONEXCLAMATION ) ;
      } /* endif */
      return ( FALSE ) ;
   } /* endif */
   if ( ( Header.APP0[0] != 0xFF ) || ( Header.APP0[1] != 0xE0 ) ) {
      fclose ( File ) ;
      if ( Owner ) {
         ResourceString Title ( Library->QueryHandle(), IDS_TITLE ) ;
         ResourceString Message ( Library->QueryHandle(), IDS_ERROR_INVALID_FILE_ID ) ;
         Sys_MessageBox ( HWND_DESKTOP, Owner, PSZ(Message), PSZ(Title), 0, MB_ENTER | MB_ICONEXCLAMATION ) ;
      } /* endif */
      return ( FALSE ) ;
   } /* endif */
   if ( memcmp ( PCHAR(Header.Identifier), "JFIF", 5 ) ) {
      fclose ( File ) ;
      if ( Owner ) {
         ResourceString Title ( Library->QueryHandle(), IDS_TITLE ) ;
         ResourceString Message ( Library->QueryHandle(), IDS_ERROR_INVALID_FILE_ID ) ;
         Sys_MessageBox ( HWND_DESKTOP, Owner, PSZ(Message), PSZ(Title), 0, MB_ENTER | MB_ICONEXCLAMATION ) ;
      } /* endif */
      return ( FALSE ) ;
   } /* endif */

   // Close up the file.
   fclose ( File ) ;

   // Return success.  It's still possible the file is invalid . . .
   return ( TRUE ) ;
}


/****************************************************************************
 *                                                                          *
 *  Collapse Bitmap Image from 24-bits to minimum bits/pixel.               *
 *                                                                          *
 ****************************************************************************/

static void CollapseBitmap ( 
   unsigned char *Buffer,       // -> Image Buffer
   RGB2 Colors[],               // -> Colors[] (allocated by caller to 256 colors max)
   int Width,                   // -> cx (image width)
   int Height,                  // -> cy (image height)
   int &BitsPerPixel,           // -> Bits per Pixel
   int &PaddedLineWidth         // -> Padded raster line width (for reference)
) {

   #ifdef DEBUG
      Log ( "JPEG::CollapseBitmap: Started.  Buffer size %i bytes.", Height*PaddedLineWidth ) ;
   #endif

   int ColorCount = 0 ;
   RGB ColorList [256] ;

   for ( int y=0; y<Height; y++ ) {
      RGB *pRGB = (RGB*) ( Buffer + y * PaddedLineWidth ) ;
      for ( int x=0; x<Width; x++, pRGB++ ) {
         if ( ColorCount ) {
            for ( int i=0; i<ColorCount && i<256; i++ ) {
               if ( !memcmp ( pRGB, &ColorList[i], sizeof(RGB) ) )
                  break ;
            } /* endfor */
            if ( i >= ColorCount ) {
               if ( ColorCount < 256 ) {
                  ColorList[ColorCount++] = *pRGB ;
               } else {
                  ColorCount ++ ;
               } /* endif */
            } /* endif */
         } else {
            if ( ColorCount < 256 ) {
               ColorList[ColorCount++] = *pRGB ;
            } else {
               ColorCount ++ ;
            } /* endif */
         } /* endif */
      } /* endfor */
   } /* endfor */

   #ifdef DEBUG
      Log ( "JPEG::CollapseBitmap: Image had %i distinct colors.", ColorCount ) ;
   #endif

   if ( ColorCount > 256 )              // If too many colors, just give up.
      return ;

   if ( ColorCount > 16 )               // Determine what final bits/pixel will be.
      BitsPerPixel = 8 ;
   else if ( ColorCount > 2 )
      BitsPerPixel = 4 ;
   else
      BitsPerPixel = 1 ;
   #ifdef DEBUG
      Log ( "JPEG::CollapseBitmap: Image will have %i bits per pixel.", BitsPerPixel ) ;
   #endif

   // Determine the bitmap buffer line length.
   int OldPaddedLineWidth = PaddedLineWidth ;
   PaddedLineWidth = ( Width * BitsPerPixel ) / 8 ;
   if ( ( Width * BitsPerPixel ) % 8 ) PaddedLineWidth++ ;
   while ( PaddedLineWidth % 4 ) PaddedLineWidth++ ;
   #ifdef DEBUG
      Log ( "JPEG::CollapseBitmap: Padded line width went from %i to %i bytes.", OldPaddedLineWidth, PaddedLineWidth ) ;
   #endif

   // Collapse the image data.
   int DataShift0, MaskShift ; UCHAR Mask0 ;
   switch ( BitsPerPixel ) {
      case 8:  Mask0 = (UCHAR)0xFF ;  MaskShift = 0 ;  DataShift0 = 0 ;  break ;
      case 4:  Mask0 = (UCHAR)0xF0 ;  MaskShift = 4 ;  DataShift0 = 4 ;  break ;
      case 1:  Mask0 = (UCHAR)0x80 ;  MaskShift = 1 ;  DataShift0 = 7 ;  break ;
   } /* endswitch */
   for ( y=0; y<Height; y++ ) {
      RGB *pRGB = (RGB*) ( Buffer + y * OldPaddedLineWidth ) ;
      BYTE *p = (BYTE*) ( Buffer + y * PaddedLineWidth ) ;
      UCHAR Mask = Mask0 ;
      int DataShift = DataShift0 ;
      for ( int x=0; x<Width; x++, pRGB++ ) {
         for ( int i=0; i<ColorCount; i++ ) 
            if ( !memcmp ( pRGB, &ColorList[i], sizeof(RGB) ) ) 
               break ;
         if ( DataShift == DataShift0 ) 
            *p = (BYTE) ( ( i << DataShift ) & Mask ) ;
         else
            *p |= (BYTE) ( ( i << DataShift ) & Mask ) ;
         if ( MaskShift ) {
            Mask >>= MaskShift ;
            DataShift -= MaskShift ;
            if ( DataShift < 0 ) {
               p ++ ;
               Mask = Mask0 ;
               DataShift = DataShift0 ;
            } /* endif */
         } else {
            p ++ ;
         } /* endif */
      } /* endfor */
   } /* endfor */

   // Save the color list.
   for ( int i=0; i<ColorCount; i++ ) {
      Colors[i].bRed   = ColorList[i].bRed ;
      Colors[i].bGreen = ColorList[i].bGreen ;
      Colors[i].bBlue  = ColorList[i].bBlue ;
   } /* endfor */

   // If only two colors, swap the two.
   if ( ColorCount == 2 ) {
      RGB2 Temp = Colors [0] ;
      Colors [0] = Colors [1] ;
      Colors [1] = Temp ;
   } /* endif */

   // Done.
   #ifdef DEBUG
      Log ( "JPEG::CollapseBitmap: Done.  Buffer size %i bytes.", Height*PaddedLineWidth ) ;
   #endif
}

/****************************************************************************
 *                                                                          *
 *  Import File                                                             *
 *                                                                          *
 ****************************************************************************/

extern "C" int read_JPEG_file ( char * filename, void **Buffer, int *Width, int *Height ) ;

extern HBITMAP APIENTRY GraphicImport ( HWND /* Owner */, char *Name ) {

   // Read in the image.
   unsigned char *Buffer ; int Width, Height ;
   if ( read_JPEG_file ( Name, (void**)&Buffer, &Width, &Height ) == 0 ) {
      Log ( "GraphicImport: Image failed to load." ) ;
      return ( 0 ) ;
   } /* endif */

   // Move buffer pointer to the actual data.
   Buffer += sizeof(long) ;

   // Optimize the bitmap, make it palettized if possible.
   RGB2 Colors[256] ; 
   int BitsPerPixel = 24 ;
   int PaddedLineWidth = ( Width * 24 ) / 8 ;
   if ( ( Width * 24 ) % 8 ) PaddedLineWidth++ ;
   while ( PaddedLineWidth % 4 ) PaddedLineWidth++ ;
   CollapseBitmap ( Buffer, Colors, Width, Height, BitsPerPixel, PaddedLineWidth ) ;

   // Determine palette size.
   int PaletteSize = 1 << BitsPerPixel ;
   if ( BitsPerPixel > 8 )
      PaletteSize = 0 ;

   // Create the bitmap header.
   int BitmapInfoSize = sizeof(BITMAPINFOHEADER2) + PaletteSize * sizeof(RGB2) ;
   PBITMAPINFO2 BitmapInfo = (PBITMAPINFO2) malloc ( BitmapInfoSize ) ;
   if ( BitmapInfo == 0 ) {
      ResourceString Message ( Library->QueryHandle(), IDS_ERROR_MEMORY ) ;
      strcpy ( ErrorMessage, PCHAR(Message) ) ;
      free ( Buffer ) ;
      return ( 0 ) ;
   } /* endif */

   // Initialize the bitmap header (except the palette).
   memset ( BitmapInfo, 0, BitmapInfoSize ) ;
   BitmapInfo->cbFix = 16 ;
   BitmapInfo->cPlanes = 1 ;
   BitmapInfo->cBitCount = USHORT ( BitsPerPixel ) ;
   BitmapInfo->cx = Width ;
   BitmapInfo->cy = Height ;
   if ( PaletteSize ) 
      memcpy ( (PUCHAR)BitmapInfo+BitmapInfo->cbFix, Colors, PaletteSize*sizeof(RGB2) ) ;

   // If we need to make or update a palette . . .
   HPAL Palette = 0 ;
   if ( PaletteSize ) {

      // Determine color counts.
      long ColorCount [256] ;
      memset ( ColorCount, 0, sizeof(ColorCount) ) ;
      int DataShift0, MaskShift ; UCHAR Mask0 ;
      switch ( BitsPerPixel ) {
         case 8:  Mask0 = (UCHAR)0xFF ;  MaskShift = 0 ;  DataShift0 = 0 ;  break ;
         case 4:  Mask0 = (UCHAR)0xF0 ;  MaskShift = 4 ;  DataShift0 = 4 ;  break ;
         case 1:  Mask0 = (UCHAR)0x80 ;  MaskShift = 1 ;  DataShift0 = 7 ;  break ;
      } /* endswitch */
      for ( int y=0; y<Height; y++ ) {
         unsigned char *pBuffer = Buffer + PaddedLineWidth * y ;
         UCHAR Mask = Mask0 ;
         int DataShift = DataShift0 ;
         for ( int x=0; x<Width; x++ ) {
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
      for ( int i=0; i<PaletteSize-1; i++ ) {
         for ( int j=i+1; j<PaletteSize; j++ ) {
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

      // Create the palette.
      Palette = GpiCreatePalette ( 0, 0, LCOLF_CONSECRGB, PaletteSize, (PULONG)Colors ) ;
 
   } /* endif creating or updating a palette */

   // Open a memory device context compatible with the video display.
   PSZ pszData [] = { 0, (PSZ)"Display" } ;
   HDC MemoryDC = DevOpenDC ( 0, OD_MEMORY, (PSZ)"*", 2, (PDEVOPENDATA)pszData, 0 ) ;
   if ( MemoryDC == 0 ) {
      ERRORID Error = WinGetLastError ( 0 ) ;
      ResourceString Format ( Library->QueryHandle(), IDS_ERROR_DC_OPEN ) ;
      sprintf ( ErrorMessage, PCHAR(Format), Error ) ;
      free ( BitmapInfo ) ;
      free ( Buffer ) ;
      return ( 0 ) ;
   } /* endif */

   // Create a memory presentation space associated with the memory device context.
   SIZEL PageSize = { 0, 0 } ;
   HPS MemoryPS = GpiCreatePS ( 0, MemoryDC, &PageSize, PU_PELS | GPIA_ASSOC ) ;
   if ( MemoryPS == 0 ) {
      ERRORID Error = WinGetLastError ( 0 ) ;
      ResourceString Format ( Library->QueryHandle(), IDS_ERROR_PS_CREATE ) ;
      sprintf ( ErrorMessage, PCHAR(Format), Error ) ;
      DevCloseDC ( MemoryDC ) ;
      free ( BitmapInfo ) ;
      free ( Buffer ) ;
      return ( 0 ) ;
   } /* endif */

   // Set the color mapping, if necessary.
   if ( Palette ) 
      GpiSelectPalette ( MemoryPS, Palette ) ;

   // Create and load the bitmap.
   HBITMAP Bitmap = GpiCreateBitmap ( MemoryPS, (PBITMAPINFOHEADER2)BitmapInfo, CBM_INIT, (PBYTE)Buffer, BitmapInfo ) ;
   if ( Bitmap == 0 ) {
      ERRORID Error = WinGetLastError ( 0 ) ;
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


