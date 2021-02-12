/******************************************************************** GIF.CPP
 *                                                                          *
 *                GIF Bitmap Graphic Import Add-on for Escriba              *
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

#include "Gif.h"

// #define DEBUG

/****************************************************************************
 *                                                                          *
 *                       Definitions & Declarations                         *
 *                                                                          *
 ****************************************************************************/

  // Macros & Constants 

#define PROGRAM_NAME    "GIF"
#define LANGUAGE_NAME   "GIF__*.DLL"

#define WORD unsigned short


  // Type Definitions

/*
**  The GIF header format.
**
**  This structure actually contains the header, logical screen
**  descriptor, and the global color table for the GIF image.
*/
typedef struct _GifHeader       /* Offset   Description            */
{
    UCHAR       Signature[3];   /*  00h     ID Signature           */
    UCHAR       Version[3];     /*  03h     Version Number         */
    WORD        ScreenWidth;    /*  06h     Logical Screen Width   */
    WORD        ScreenHeight;   /*  08h     Logical Screen Height  */ 
    UCHAR       PackedField;    /*  0Ah     Color Information      */
    UCHAR       ColorIndex;     /*  0Bh     Background Color Index */
    UCHAR       AspectRatio;    /*  0Ch     Pixel Aspect Ratio     */
} GIFHEAD;                      

/*
**  The GIF Image Descriptor.
*/
typedef struct _GifImageDescriptor
{
    UCHAR       ImageSeparator;     /* Image Descriptor identifier            */
    WORD        ImageLeft;          /* X position of image on the display     */
    WORD        ImageTop;           /* Y position of image on the display     */
    WORD        ImageWidth;         /* Width of the image in pixels           */
    WORD        ImageHeight;        /* Height of the image in pixels          */
    UCHAR       PackedField;        /* Image and Color Table Data Information */
} GIFIMAGEDESC;


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

      #ifdef DEBUG
      Log ( "DLLINIT: HomePath '%s'.", HomePath ) ;
      #endif

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
   #ifdef DEBUG
   Log ( "ProductVersion started.  Revision %i.", REVISION_NO ) ;
   #endif
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
   return ( "GIF" ) ;
}


/****************************************************************************
 *                                                                          *
 *  Query Filename Wildcard                                                 *
 *                                                                          *
 ****************************************************************************/

extern char* APIENTRY QueryWildcard ( void ) {
   return ( "*.GIF" ) ;
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

   // Match against *.GIF.
   if ( MatchWildcard ( Name, "*.GIF" ) )
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
   if ( Size < sizeof(GIFHEAD) ) {
      fclose ( File ) ;
      if ( Owner ) {
         ResourceString Title ( Library->QueryHandle(), IDS_TITLE ) ;
         ResourceString Message ( Library->QueryHandle(), IDS_ERROR_FILE_TOO_SMALL ) ;
         Sys_MessageBox ( HWND_DESKTOP, Owner, PSZ(Message), PSZ(Title), 0, MB_ENTER | MB_ICONEXCLAMATION ) ;
      } /* endif */
      return ( FALSE ) ;
   } /* endif */

   // Get the image header.
   GIFHEAD Header ;
   fseek ( File, 0, SEEK_SET ) ;
   fread ( &Header, 1, sizeof(Header), File ) ;

   // Verify that this is a GIF file.
   if ( strnicmp ( PCHAR(Header.Signature), "GIF", 3 ) ) {
      fclose ( File ) ;
      if ( Owner ) {
         ResourceString Title ( Library->QueryHandle(), IDS_TITLE ) ;
         ResourceString Message ( Library->QueryHandle(), IDS_ERROR_INVALID_FILE_ID ) ;
         Sys_MessageBox ( HWND_DESKTOP, Owner, PSZ(Message), PSZ(Title), 0, MB_ENTER | MB_ICONEXCLAMATION ) ;
      } /* endif */
      return ( FALSE ) ;
   } /* endif */
   if ( strnicmp ( PCHAR(Header.Version), "87a", 3 ) && strnicmp ( PCHAR(Header.Version), "89a", 3 ) ) {
      fclose ( File ) ;
      if ( Owner ) {
         ResourceString Title ( Library->QueryHandle(), IDS_TITLE ) ;
         ResourceString Message ( Library->QueryHandle(), IDS_ERROR_INVALID_FILE_VERSION ) ;
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
 *  Build OS/2 Bitmap Data Buffer and Palette from GIF data.                *
 *                                                                          *
 *  Returns NZ if error.                                                    *
 *                                                                          *
 ****************************************************************************/

#define MAX_ROWS 2048

static int   bad_code_count;
static int   BitmapBitsPerPixel ;
static LONG  ColorCount [256] ;
static int   LineCount ;
static char *p ;
static int   PaddedLineWidth ;
static int   PageHeight ;
static short RowTable [MAX_ROWS] ;

static WORD  decoder ( char *Buffer, WORD linewidth ) ;

static int GIF_BuildBitmapData (
   PVOID Data,                  // -> Raw GIF data.
   ULONG /* Size */,            // GIF Data size.
   PCHAR *Buffer,               // -> pBuffer (allocated by this function)
   RGB2 Colors[],               // -> Colors[] (allocated by caller to 256 colors max)
   int *cx,                     // -> cx (image width)
   int *cy,                     // -> cy (image height)
   int *BitmapBitsPerPixel,     // -> Bits per Pixel (in result bitmap)
   int *PaddedLineWidth         // -> Padded raster line width (for reference)
) {

   // Local Declarations
   unsigned char Flags ;
   unsigned PaletteSize ;
   unsigned BitsPerPixel ;
   int i, j, rc ;

   // Verify that this is a GIF file.
   p = (char*) Data ;
   if ( strnicmp ( (char*)((GIFHEAD*)p)->Signature, "GIF", 3 ) ) {
      ResourceString Message ( Library->QueryHandle(), IDS_ERROR_INVALID_FILE_ID ) ;
      strcpy ( ErrorMessage, PCHAR(Message) ) ;
      return ( 1 ) ;
   } /* endif */
   if ( strnicmp ( (char*)((GIFHEAD*)p)->Version, "87a", 3 ) && strnicmp ( (char*)((GIFHEAD*)p)->Version, "89a", 3 ) ) {
      ResourceString Message ( Library->QueryHandle(), IDS_ERROR_INVALID_FILE_VERSION ) ;
      strcpy ( ErrorMessage, PCHAR(Message) ) ;
      return ( 1 ) ;
   } /* endif */

   // Get header information.
   Flags = ((GIFHEAD*)p)->PackedField ;
   BitsPerPixel = ( Flags & 0x07 ) + 1 ;
   PaletteSize = ( Flags & 0x80 ) ? 1 << BitsPerPixel : 0 ;

   p += sizeof(GIFHEAD) ;       

   // If global color table given, load it now.
   memset ( Colors, 0, sizeof(Colors[0])*256 ) ;
   if ( PaletteSize ) {
      for ( i=0; i<PaletteSize; i++ ) {
         Colors[i].fcOptions = 0 ;
         Colors[i].bRed   = *p++ ;
         Colors[i].bGreen = *p++ ;
         Colors[i].bBlue  = *p++ ;
      } /* endfor */
   } /* endif */

   // Scan until the local image descriptor is found . . .
   while ( *p != 0x2C ) {
      switch ( *(UCHAR*)p ) {
         case 0x21:
            switch ( *(UCHAR*)(p+1) ) {
               case 0x01:       // Plain Text Extension
                  p += 15 ;
                  while ( *p ) {
                     p += *(UCHAR*)p ;
                     p ++ ;
                  } /* endwhile */
                  p ++ ;
                  break;
               case 0xF9:       // Graphics Control Extension
                  p += 8 ;
                  break;
               case 0xFE:       // Comment Extension
                  p += 2 ;
                  while ( *p ) {
                     p += *(UCHAR*)p ;
                     p ++ ;
                  } /* endwhile */
                  p ++ ;
                  break;
               case 0xFF:       // Application Extension
                  p += 14 ;
                  while ( *p ) {
                     p += *(UCHAR*)p ;
                     p ++ ;
                  } /* endwhile */
                  p ++ ;
                  break;
               default: 
                  ResourceString Format ( Library->QueryHandle(), IDS_ERROR_INVALID_CONTROL_LABEL ) ;
                  sprintf ( ErrorMessage, PCHAR(Format), *(UCHAR*)(p+1) ) ;
                  return ( 1 ) ;
            } /* endswitch */
            break;
         default: 
            ResourceString Format ( Library->QueryHandle(), IDS_ERROR_INVALID_CONTROL_BLOCK_TYPE ) ;
            sprintf ( ErrorMessage, PCHAR(Format), *(UCHAR*)p ) ;
            return ( 1 ) ;
      } /* endswitch */
   } /* endwhile */

   // Get the image size & attributes.
   *cx = ((GIFIMAGEDESC*)p)->ImageWidth ;
   *cy = ((GIFIMAGEDESC*)p)->ImageHeight ;
   Flags = ((GIFIMAGEDESC*)p)->PackedField ;

   p += sizeof(GIFIMAGEDESC) ;

   // If local color table present, get it.
   if ( Flags & 0x80 ) {
      PaletteSize = 1 << ( ( Flags & 0x07 ) + 1 ) ;
      for ( i=0; i<PaletteSize; i++ ) {
         Colors[i].fcOptions = 0 ;
         Colors[i].bRed   = *p++ ;
         Colors[i].bGreen = *p++ ;
         Colors[i].bBlue  = *p++ ;
      } /* endfor */
   } /* endif */

   // Check for too many rows.
   if ( *cy > MAX_ROWS ) {
      ResourceString Format ( Library->QueryHandle(), IDS_ERROR_TOO_MANY_ROWS ) ;
      sprintf ( ErrorMessage, PCHAR(Format), cy, MAX_ROWS ) ;
      return ( 1 ) ;
   } /* endif */

   // Prepare the row table.
   if ( Flags & 0x40 ) {
      for ( i=0, j=0; i<*cy; i+=8, j++ )
         RowTable[j] = short(i) ;
      for ( i=4; i<*cy; i+=8, j++ )
         RowTable[j] = short(i) ;
      for ( i=2; i<*cy; i+=4, j++ )
         RowTable[j] = short(i) ;
      for ( i=1; i<*cy; i+=2, j++ )
         RowTable[j] = short(i) ;
   } else {
      for ( i=0; i<*cy; i++ )
         RowTable[i] = short(i) ;
   } /* endif */

   // Adjust bits/pixel and palette size.
   *BitmapBitsPerPixel = BitsPerPixel ;
   if ( *BitmapBitsPerPixel == 3 ) {
      *BitmapBitsPerPixel = 4 ;
   } else if ( *BitmapBitsPerPixel > 4 ) {
      *BitmapBitsPerPixel = 8 ;
   } /* endif */

   // Prepare the data buffer.
   *PaddedLineWidth = ( *cx * *BitmapBitsPerPixel ) / 8 ;
   if ( ( *cx * *BitmapBitsPerPixel ) % 8 ) (*PaddedLineWidth)++ ;
   while ( *PaddedLineWidth % 4 ) (*PaddedLineWidth)++ ;
   PageHeight = *cy ;
   *Buffer = (char*) malloc ( *PaddedLineWidth * PageHeight ) ;
   if ( *Buffer == 0 ) {
      ResourceString Message ( Library->QueryHandle(), IDS_ERROR_MEMORY1 ) ;
      strcpy ( ErrorMessage, PCHAR(Message) ) ;
      return ( 1 ) ;
   } /* endif */

   // Decode the image data blocks.
   memset ( ColorCount, 0, sizeof(ColorCount) ) ;
   LineCount = 0 ;
   bad_code_count = 0 ;
   rc = decoder ( *Buffer, (WORD)*cx ) ;
   if ( rc || bad_code_count ) {
      ResourceString Format ( Library->QueryHandle(), IDS_ERROR_DECODER ) ;
      sprintf ( ErrorMessage, PCHAR(Format), rc ) ;
      free ( *Buffer ) ;
      return ( 1 ) ;
   } /* endif */

   // Return no error if we got this far.
   return ( 0 ) ;
}


/****************************************************************************
 *                                                                          *
 *                     Build OS/2 Bitmap from GIF data                      *
 *                                                                          *
 *  NOTE: If *Palette is set to zero before the function call, the palette  *
 *    in the GIF file will be used.  If not, the palette provided by the    *
 *    caller will be used.                                                  *
 *                                                                          *
 ****************************************************************************/

static HBITMAP CreateBitmapFromGIF ( HAB Anchor, PVOID Data, unsigned long Size, PHPAL Palette, BOOL FixedPalette ) {

   // Local Declarations
   HBITMAP      Bitmap = 0 ;
   PBITMAPINFO2 BitmapInfo ;
   int          BitmapInfoSize ;
   char        *Buffer ;
   RGB2         Colors [256] ;
   int          cx, cy ;
   int          i, j ;
   HDC          MemoryDC ;
   HPS          MemoryPS ;
   unsigned     PaletteSize ;
   SIZEL        PageSize = { 0, 0 } ;
   PSZ          pszData [] = { 0, (PSZ)"Display" } ;

   // Obtain the image data.
   if ( GIF_BuildBitmapData ( Data, Size, &Buffer, Colors, &cx, &cy, &BitmapBitsPerPixel, &PaddedLineWidth ) )
      return ( 0 ) ;
 
   // Determine palette size.
   PaletteSize = 1 << BitmapBitsPerPixel ;
   if ( BitmapBitsPerPixel > 8 )
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
   BitmapInfo->cbFix = 16 ;
   BitmapInfo->cPlanes = 1 ;
   BitmapInfo->cBitCount = USHORT ( BitmapBitsPerPixel ) ;
   BitmapInfo->cx = cx ;
   BitmapInfo->cy = cy ;
   if ( PaletteSize ) 
      memcpy ( (PUCHAR)BitmapInfo+BitmapInfo->cbFix, Colors, PaletteSize*sizeof(RGB2) ) ;

   // If we need to make or update a palette . . .
   if ( !FixedPalette && PaletteSize ) {

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

/* Various error codes used by decoder
 * and my own routines...   It's okay
 * for you to define whatever you want,
 * as long as it's negative...  It will be
 * returned intact up the various subroutine
 * levels...
 */
#define OUT_OF_MEMORY -10
#define BAD_CODE_SIZE -20
#define READ_ERROR -1
#define WRITE_ERROR -2
#define OPEN_ERROR -3
#define CREATE_ERROR -4

/* extern int get_byte()
 * 
 * - This external (machine specific) function is expected to return either
 * the next byte from the GIF file, or a negative number, as defined in
 * ERRS.H. */
static int get_byte ( void ) {
   return ( *(UCHAR*)p++ ) ;
}


/* extern int out_line(pixels, linelen) UUCHAR pixels[]; int linelen;
 * 
 * - This function takes a full line of pixels (one byte per pixel) and
 * displays them (or does whatever your program wants with them...).  It
 * should return zero, or negative if an error or some other event occurs
 * which would require aborting the decode process...  Note that the
 * length passed will almost always be equal to the line length passed to
 * the decoder function, with the sole exception occurring when an ending
 * code occurs in an odd place in the GIF file...  In any case, linelen
 * will be equal to the number of pixels passed... */
static int out_line ( char *Buffer, unsigned char *Line, int LineWidth ) {

   UCHAR Bits, Mask, Mask0 ;
   int x, MaskShift, DataShift0, DataShift ;

   char *pBuffer = Buffer + PaddedLineWidth * ( PageHeight - 1 - RowTable[LineCount] ) ;
   memset ( pBuffer, 0, PaddedLineWidth ) ;

   switch ( BitmapBitsPerPixel ) {
      case 8:  Mask0 = (UCHAR)0xFF ;  MaskShift = 0 ;  DataShift0 = 0 ;  break;
      case 4:  Mask0 = (UCHAR)0xF0 ;  MaskShift = 4 ;  DataShift0 = 4 ;  break;
      case 2:  Mask0 = (UCHAR)0xC0 ;  MaskShift = 2 ;  DataShift0 = 6 ;  break;
      case 1:  Mask0 = (UCHAR)0x80 ;  MaskShift = 1 ;  DataShift0 = 7 ;  break;
   } /* endswitch */

   Mask = Mask0 ;                               // Reset the pixel mask.
   DataShift = DataShift0 ;                     // Reset the data shift count.
   for ( x=0; x<LineWidth; x++ ) {              // FOR each byte in the line . . .
      ColorCount [ Line [x] ] ++ ;              //   Update the color count array for palette.
      Bits = (UCHAR) ( Line[x] << DataShift ) ;  //   Shift the color index the appropriate amount left.
      if ( BitmapBitsPerPixel == 1 )            //   If b/w bitmap,
         Bits = (UCHAR) ~Bits ;                  //     Invert the bit.
      *pBuffer |= ( Bits & Mask ) ;             //   Merge the new pixel into the bitmap buffer.
      if ( MaskShift ) {                        //   If dealing with partial byte pixels,
         Mask >>= MaskShift ;                   //     Shift the mask over for the next pixel.
         DataShift -= MaskShift ;               //     Adjust the amount by which data if shifted left.
         if ( DataShift < 0 ) {                 //     If we've moved past the end of the byte,
            pBuffer ++ ;                        //       Advance bitmap buffer pointer to next byte.
            Mask = Mask0 ;                      //       Reset the mask.
            DataShift = DataShift0 ;            //       Reset the data shift count.
         } /* endif */                          //     End if
      } else {                                  //   Else, if whole byte pixels,
         pBuffer ++ ;                           //     Just advance the bitmap buffer pointer.
      } /* endif */                             //   End if/else
   } /* endfor */                               // End FOR

   LineCount ++ ;

   return ( 0 ) ;
}

/* extern int bad_code_count;
 * 
 * This value is the only other global required by the using program, and is
 * incremented each time an out of range code is read by the decoder. When
 * this value is non-zero after a decode, your GIF file is probably
 * corrupt in some way... */
//static int bad_code_count;

#define MAX_CODES   4095

/* Static variables */
static WORD curr_size;           /* The current code size */
static WORD clear;               /* Value for a clear code */
static WORD ending;              /* Value for a ending code */
static WORD newcodes;            /* First available code */
static WORD top_slot;            /* Highest code for current size */
static WORD slot;                /* Last read code */
/* The following static variables are used for separating out codes */
static WORD navail_bytes = 0;    /* # bytes left in block */
static WORD nbits_left = 0;      /* # bits left in current byte */
static unsigned char b1;                 /* Current byte */
static unsigned char byte_buff[257];     /* Current block */
static unsigned char *pbytes;            /* Pointer to next byte in block */

static long code_mask[13] = {
   0,
   0x0001, 0x0003,
   0x0007, 0x000F,
   0x001F, 0x003F,
   0x007F, 0x00FF,
   0x01FF, 0x03FF,
   0x07FF, 0x0FFF
};


/* This function initializes the decoder for reading a new image. */
static WORD init_exp ( WORD size ) {
   curr_size = (WORD) ( size + 1 ) ;
   top_slot = (WORD) ( 1 << curr_size ) ;
   clear = (WORD) ( 1 << size ) ;
   ending = (WORD) ( clear + 1 ) ;
   slot = (WORD) ( newcodes = (WORD) ( ending + 1 ) ) ;
   navail_bytes = nbits_left = 0;
   return ( 0 );
}

/* get_next_code() - gets the next code from the GIF file.  Returns the
 * code, or else a negative number in case of file errors... */
static WORD get_next_code (  ) {
   WORD i,
    x;
   unsigned long ret;
   if ( nbits_left == 0 ) {
      if ( int(navail_bytes) <= 0 ) {

         /* Out of bytes in current block, so read next block */
         pbytes = byte_buff;
         if ( int ( navail_bytes = (WORD) get_byte ( ) ) < 0 )
            return ( navail_bytes );
         else if ( navail_bytes ) {
            for ( i = 0; i < navail_bytes; ++i ) {
               if ( int ( x = (WORD) get_byte (  ) ) < 0 )
                  return ( x );
               byte_buff[i] = (UCHAR) x ;
            }
         }
      }
      b1 = *pbytes++;
      nbits_left = 8;
      --navail_bytes;
   }

   ret = b1 >> ( 8 - nbits_left );
   while ( curr_size > nbits_left ) {
      if ( int(navail_bytes) <= 0 ) {

         /* Out of bytes in current block, so read next block */
         pbytes = byte_buff;
         if ( int ( navail_bytes = (WORD) get_byte (  ) ) < 0 )
            return ( navail_bytes );
         else if ( navail_bytes ) {
            for ( i = 0; i < navail_bytes; ++i ) {
               if ( int ( x = (WORD) get_byte (  ) ) < 0 )
                  return ( x );
               byte_buff[i] = (UCHAR) x ;
            }
         }
      }
      b1 = *pbytes++;
      ret |= b1 << nbits_left;
      nbits_left += 8;
      --navail_bytes;
   }
   nbits_left -= curr_size;
   ret &= code_mask[curr_size];
   return ( ( WORD ) ( ret ) );
}


/* The reason we have these separated like this instead of using a
 * structure like the original Wilhite code did, is because this stuff
 * generally produces significantly faster code when compiled... This code
 * is full of similar speedups...  (For a good book on writing C for speed
 * or for space optomisation, see Efficient C by Tom Plum, published by
 * Plum-Hall Associates...) */
static unsigned char stack[MAX_CODES + 1];       /* Stack for storing pixels */
static unsigned char suffix[MAX_CODES + 1];      /* Suffix table */
static unsigned short prefix[MAX_CODES + 1];      /* Prefix linked list */

/* WORD decoder(linewidth) WORD linewidth;               * Pixels per line
 * of image *
 * 
 * - This function decodes an LZW image, according to the method used in the
 * GIF spec.  Every *linewidth* "characters" (ie. pixels) decoded will
 * generate a call to out_line(), which is a user specific function to
 * display a line of pixels.  The function gets it's codes from
 * get_next_code() which is responsible for reading blocks of data and
 * separating them into the proper size codes.  Finally, get_byte() is the
 * global routine to read the next byte from the GIF file.
 * 
 * It is generally a good idea to have linewidth correspond to the actual
 * width of a line (as specified in the Image header) to make your own
 * code a bit simpler, but it isn't absolutely necessary.
 * 
 * Returns: 0 if successful, else negative.  (See ERRS.H)
 * 
 */

static WORD decoder ( char *Buffer, WORD linewidth ) {

   register unsigned char *sp,
   *bufptr;
   unsigned char *buf;
   register WORD code,
    fc,
    oc,
    bufcnt;
   WORD c,
    size,
    ret;

   /* Initialize for decoding a new image... */
   if ( int ( size = (WORD) get_byte (  ) ) < 0 ) {
      ResourceString Message ( Library->QueryHandle(), IDS_ERROR_GET_CODE_SIZE ) ;
      strcpy ( ErrorMessage, PCHAR(Message) ) ;
      return ( size );
   }

   if ( size < 2 || 9 < size ) {
      ResourceString Message ( Library->QueryHandle(), IDS_ERROR_INVALID_CODE_SIZE ) ;
      strcpy ( ErrorMessage, PCHAR(Message) ) ;
      return ( (WORD) BAD_CODE_SIZE );
   }

   init_exp ( size );

   /* Initialize in case they forgot to put in a clear code. (This
    * shouldn't happen, but we'll try and decode it anyway...) */
   oc = fc = 0;

   /* Allocate space for the decode buffer */
   if ( ( buf = ( unsigned char * ) malloc ( linewidth + 1 ) ) == NULL ) {
      ResourceString Message ( Library->QueryHandle(), IDS_ERROR_MEMORY3 ) ;
      strcpy ( ErrorMessage, PCHAR(Message) ) ;
      return ( (WORD) OUT_OF_MEMORY ) ;
   }

   /* Set up the stack pointer and decode buffer pointer */
   sp = stack;
   bufptr = buf;
   bufcnt = linewidth;

   /* This is the main loop.  For each code we get we pass through the
    * linked list of prefix codes, pushing the corresponding "character"
    * for each code onto the stack.  When the list reaches a single
    * "character" we push that on the stack too, and then start unstacking
    * each character for output in the correct order.  Special handling is
    * included for the clear code, and the whole thing ends when we get an
    * ending code. */
   while ( ( c = get_next_code (  ) ) != ending ) {

      /* If we had a file error, return without completing the decode */
      if ( int(c) < 0 ) {
         ResourceString Message ( Library->QueryHandle(), IDS_ERROR_FILE_ERROR ) ;
         strcpy ( ErrorMessage, PCHAR(Message) ) ;
         free ( buf );
         return ( 0 );
      }

      /* If the code is a clear code, reinitialize all necessary items. */
      if ( c == clear ) {
         curr_size = (WORD) ( size + 1 ) ;
         slot = newcodes;
         top_slot = (WORD) ( 1 << curr_size ) ;

         /* Continue reading codes until we get a non-clear code (Another
          * unlikely, but possible case...) */
         while ( int ( c = (WORD) get_next_code (  ) ) == clear ) { } 

         /* If we get an ending code immediately after a clear code (Yet
          * another unlikely case), then break out of the loop. */
         if ( c == ending ) {
            break;
         }

         /* Finally, if the code is beyond the range of already set codes,
          * (This one had better NOT happen...  I have no idea what will
          * result from this, but I doubt it will look good...) then set
          * it to color zero. */
         if ( c >= slot )
            c = 0;

         oc = fc = c;

         /* And let us not forget to put the char into the buffer... And
          * if, on the off chance, we were exactly one pixel from the end
          * of the line, we have to send the buffer to the out_line()
          * routine... */
         *bufptr++ = (UCHAR) c ;
         if ( --bufcnt == 0 ) {
            if ( int ( ret = (WORD) ( out_line ( Buffer, buf, linewidth ) ) ) < 0 ) {
               free ( buf );
               return ( ret );
            }
            bufptr = buf;
            bufcnt = linewidth;
         }
      }
      else {

         /* In this case, it's not a clear code or an ending code, so it
          * must be a code code...  So we can now decode the code into a
          * stack of character codes. (Clear as mud, right?) */
         code = c;

         /* Here we go again with one of those off chances...  If, on the
          * off chance, the code we got is beyond the range of those
          * already set up (Another thing which had better NOT happen...)
          * we trick the decoder into thinking it actually got the last
          * code read. (Hmmn... I'm not sure why this works...  But it
          * does...) */
         if ( code >= slot ) {
            if ( code > slot )
               ++bad_code_count;
            code = oc;
            *sp++ = (UCHAR) fc ;
         }

         /* Here we scan back along the linked list of prefixes, pushing
          * helpless characters (ie. suffixes) onto the stack as we do so. */
         while ( code >= newcodes ) {
            *sp++ = suffix[code];
            code = prefix[code];
         }

         /* Push the last character on the stack, and set up the new
          * prefix and suffix, and if the required slot number is greater
          * than that allowed by the current bit size, increase the bit
          * size.  (NOTE - If we are all full, we *don't* save the new
          * suffix and prefix...  I'm not certain if this is correct... it
          * might be more proper to overwrite the last code... */
         *sp++ = (UCHAR) code ;
         if ( slot < top_slot ) {
            suffix[slot] = (UCHAR) ( fc = (WORD) code ) ;
            prefix[slot++] = oc;
            oc = c;
         }
         if ( slot >= top_slot )
            if ( curr_size < 12 ) {
               top_slot <<= 1;
               ++curr_size;
            }

         /* Now that we've pushed the decoded string (in reverse order)
          * onto the stack, lets pop it off and put it into our decode
          * buffer...  And when the decode buffer is full, write another
          * line... */
         while ( sp > stack ) {
            *bufptr++ = *( --sp );
            if ( --bufcnt == 0 ) {
               if ( int ( ret = (WORD) ( out_line ( Buffer, buf, linewidth ) ) ) < 0 ) {
                  free ( buf );
                  return ( ret );
               }
               bufptr = buf;
               bufcnt = linewidth;
            }
         }
      }
   }
   ret = 0;
   if ( bufcnt != linewidth )
      ret = (WORD) out_line ( Buffer, buf, ( linewidth - bufcnt ) ) ;
   free ( buf );
   return ( ret );
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
      ResourceString Format ( Library->QueryHandle(), IDS_ERROR_OPEN_FOR_LOAD ) ;
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
      ResourceString Format ( Library->QueryHandle(), IDS_ERROR_MEMORY4 ) ;
      char Message [500] ;
      sprintf ( Message, PCHAR(Format), Size ) ;
      Sys_MessageBox ( HWND_DESKTOP, Owner, PSZ(Message), PSZ(Title), 0, MB_ENTER | MB_ICONEXCLAMATION ) ;
      return ( 0 ) ;
   } /* endif */

   fseek ( File, 0, SEEK_SET ) ;
   fread ( pData, 1, size_t(Size), File ) ;
   fclose ( File ) ;

   HBITMAP Bitmap = CreateBitmapFromGIF ( WinQueryAnchorBlock(Owner), pData, Size, 0, TRUE ) ;

   free ( pData ) ;

   if ( Bitmap == 0 ) {
      ResourceString Title ( Library->QueryHandle(), IDS_TITLE ) ;
      Sys_MessageBox ( HWND_DESKTOP, Owner, PSZ(ErrorMessage), PSZ(Title), 0, MB_ENTER | MB_ICONEXCLAMATION ) ;
   } /* endif */

   return ( Bitmap ) ;
}

