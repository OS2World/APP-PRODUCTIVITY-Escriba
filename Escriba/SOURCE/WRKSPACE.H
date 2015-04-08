/***************************************************************** WRKSPACE.H
 *                                                                          *
 *                        Graphic Workspace Class                           *
 *                                                                          *
 ****************************************************************************/

#ifndef WRKSPACE_H
#define WRKSPACE_H

#define GPIMEASURE  // Use GpiQueryTextBox instead of font width table.

#define MAX_KERNINGPAIRS (512)

#include <string.h>

#include "System.h"

#include "Device.h"
#include "Font.h"
#include "Mutex.h"
#include "State.h"

enum WorkspaceType { WRKSPC_GETPS, WRKSPC_PAINT, WRKSPC_GPI } ;

class WorkSpace {
   private:
      WorkspaceType Type ; HPS hPS ;
      char *Name ; int CodePage ; HAB Anchor ; HWND Window ; DeviceContext *pDevice ; 
      SIZEL PageSize ; int Scaled : 1 ; int Metric : 1 ; int Micro : 1 ;

      UCHAR DBCSVector [6] [2] ;
      FontCache Fonts ;
      #ifdef GPIMEASURE
         BOOL FontWidthsValid ;
         long FontWidths [256] ;
         long KerningPairCount ;
         PKERNINGPAIRS KerningPairs ;
      #endif
      char FontName [FACESIZE+1] ;
      int FontSize ;
      COLOR LineColor ;
      int LineType ;
      int LineWidth ;
      int LineJoin ;
      int LineEnd ;
      COLOR FillColor ;
      int FillType ;
      int Bold : 1 ;
      int Italic : 1 ;
      int Underscore : 1 ;
      int Strikeout : 1 ;
      int Outline : 1 ;
      int Caps : 1 ;
      int Expand ;
      int Mix ;
      int Ascent ;
      int Descent ;
      int AvgWidth ;
      int StrikeoutInfo [2] ; // [0] = Position below baseline, [1] = Height.
      int UnderscoreInfo [2] ; // [0] = Position above baseline, [1] = Height.
      HRGN ClipRegion ;
      RECTL ClippingRectangle ;
      FIXED Zoom ;
      int DeltaX ;
      int DeltaY ;
      Mutex Lock ;

   public:
      // Constructor 1: For a non-scaled micro-cached PS associated with a window.
      WorkSpace ( char *Name, int CodePage, HWND Window, DeviceContext *pDevice ) ;

      // Constructor 2: For a scaled micro-cached PS associated with a window.
      WorkSpace ( char *Name, int CodePage, HWND Window, DeviceContext *pDevice, int metric ) ;

      // Constructor 3: For a non-scaled PS for use in processing WM_PAINT.
      WorkSpace ( char *Name, int CodePage, HWND Window, DeviceContext *pDevice, RECTL &Clip ) ;

      // Constructor 4: For a scaled PS for use in processing WM_PAINT.
      WorkSpace ( char *Name, int CodePage, HWND Window, DeviceContext *pDevice, int metric, RECTL &Clip ) ;

      // Constructor 5: For a non-scaled PS associated with a device.
      WorkSpace ( char *Name, int CodePage, DeviceContext *pDevice, HAB Anchor, int Micro ) ;

      // Constructor 6: For a scaled PS associated with a device.
      WorkSpace ( char *Name, int CodePage, DeviceContext *pDevice, HAB Anchor, int metric, int Micro ) ;

      // Constructor 7: For a scaled PS not associated with a device.
      WorkSpace ( char *Name, int CodePage, HAB Anchor, int metric, int Width, int Height ) ;

      // Constructor 8: For a non-scaled PS associated with another task's window.
      WorkSpace ( char *Name, HWND Window ) ;

      // Constructor 9: For the Desktop PS.
      WorkSpace ( char *Name ) ;

      // Destructor.
      ~WorkSpace ( ) ;

      // Methods: Create/Destroy PS.
      void CreatePS ( BOOL Completely ) ;
      void DestroyPS ( ) ;

      // Methods: Query Ascent / Descent.
      int QueryAscent ( ) { return ( Ascent ) ; }
      int QueryDescent ( ) { return ( Descent ) ; }
      int QueryAvgWidth ( ) { return ( AvgWidth ) ; }

      // Method: Reset workspace.
      void Reset ( int CodePage, int Metric, int Width, int Height ) ;
      void Reset ( BOOL Completely = FALSE ) ;

      // Methods: Associate/Deassociate workspace.
      BOOL Associate ( DeviceContext *pdevice ) { pDevice = pdevice ; return ( pDevice->Associate ( hPS ) ) ; }
      BOOL Deassociate ( ) { pDevice = 0 ; return ( GpiAssociate ( hPS, 0 ) ) ; }

      // Method: Move to point.
      BOOL Move ( int x, int y ) ;
      BOOL Move ( POINTL &Point ) ;

      // Method: Draw line to point (simple).
      long DrawLine ( int x, int y ) ;
      long DrawLine ( POINTL &Point ) ;

      // Method: Draw line to point (geometric).
      long DrawLineGeom ( int x, int y ) ;
      long DrawLineGeom ( POINTL &Point ) ;

      // Method: Draw multiple connected lines.
      long DrawPolyLine ( int Count, POINTL Points[] ) ;

      // Method: Draw multiple connected lines and fill them.
      long DrawPolygon ( int Count, POINTL Points[] ) ;

      // Method: Draw Box.
      long DrawBox ( RECTL &Rectangle ) ;

      // Method: Draw Filled Box.
      long FillBox ( RECTL &Rectangle ) ;

      // Method: Invert Box
      BOOL InvertBox ( RECTL &Rectangle ) ;

      // Method: Draw/Undraw Tracking Box (Inverted Alternating)
      void DrawTrackingBox ( RECTL &Rectangle ) ;

      // Method: Query Text Box
      BOOL QueryTextBox ( char *Text, int Length, POINTL TextBox[] ) ;
      BOOL QueryTextBox ( char *Text, POINTL TextBox[] ) { return ( QueryTextBox ( Text, strlen(Text), TextBox ) ) ; }

      // Method: Measure Text
      int MeasureText ( char *Text, int Length, SIZEL &Size ) ;
      int MeasureText ( char *Text, SIZEL &Size ) ;
      int MeasureText ( char *Text, int Length ) ;
      int MeasureText ( char *Text ) ;

      // Method: Invert Cell
      int InvertCell ( SIZEL Size ) ;

      // Method: Invert Text already painted.
      int InvertText ( char *Text, int Length ) ;
      int InvertText ( char *Text ) { return ( InvertText ( Text, strlen(Text) ) ) ; }

      // Method: Draw Text (the easy way)
      long DrawText ( char *Text, int Length, RECTL &Rectangle, COLOR TextColor, COLOR Background, long Flags ) ;

      // Method: Draw Text (the hard way)
      long DrawText ( char *Text, int Length=0, int Select=FALSE ) ;

      // Method: Draw struckout or underscored blank space.
      long DrawSpace ( SIZEL &Size, int Select=FALSE ) ;

      // Method: Draw Pointer
      BOOL DrawPointer ( HPOINTER Pointer, POINTL &Point, int Flags ) ;

      // Method: Select Font (when Name, Bold, Italic, Underscore, Strikeout or Outline change)
      void SelectFont ( ) {
         Fonts.Select ( hPS, FontName, Bold, Italic, Underscore, Strikeout, Outline ) ;
      }

      // Method: Scale Font (when Size or Expand change)
      void ScaleFont ( ) ;

      // Method: Get Font Metrics (when Name, Size, Bold, Italic, or Expand change)
      void GetFontMetrics ( ) ;

      // Method: Set Font Name
      void SetFontName ( char *Name ) ;

      // Method: Set Font Size
      void SetFontSize ( int Size ) ;

      // Method: Set Bold
      void SetBold ( int Flag ) ;

      // Method: Set Italic
      void SetItalic ( int Flag ) ;

      // Method: Set Underscore
      void SetUnderscore ( int Flag ) ;

      // Method: Set Strikeout
      void SetStrikeout ( int Flag ) ;

      // Method: Set Outline
      void SetOutline ( int Flag ) ;

      // Method: Set Caps
      void SetCaps ( int Flag ) { Caps = Flag ; }

      // Method: Set Line/Text Color
      BOOL SetColor ( COLOR color ) ;

      // Method: Set Line Type
      BOOL SetLineType ( int Type ) ;

      // Method: Set Line Geometric Width
      BOOL SetLineWidth ( int Width ) ;

      // Method: Set Line Join
      BOOL SetLineJoin ( int Join ) ;

      // Method: Set Line End
      BOOL SetLineEnd ( int End ) ;

      // Method: Set Fill Color
      void SetFillColor ( COLOR color ) { FillColor = color ; }

      // Method: Set Fill Type
      BOOL SetFillType ( int Type ) ;

      // Method: Set Mix
      BOOL SetMix ( int mix ) ;

      // Method: Set Expansion Factor
      void SetExpand ( int expand ) ;

      // Method: Set magnification and X,Y translation.
      void SetTransform ( FIXED fxZoom, int DeltaX, int DeltaY ) ;
      void SetTransform ( double Zoom, int DeltaX, int DeltaY ) ;

      // Method: Perform coordinate transformations on list of points.
      BOOL Transform ( int From, int To, int Count, POINTL Points[] ) ;

      // Method: Perform coordinate transformations on a single point.
      BOOL Transform ( int From, int To, POINTL &Point ) {
         return ( Transform ( From, To, 1, &Point ) ) ;
      }

      // Method: Perform coordinate transformation on rectangle.
      BOOL Transform ( int From, int To, RECTL &Rectangle ) {
         return ( Transform ( From, To, 2, PPOINTL(&Rectangle) ) ) ;
      }

      // Method: Determine the handle of the Device associated with this PS.
      HDC QueryDevice ( ) ;

      // Method: Determine how many fonts are available to this PS.
      LONG QueryFontCount ( ) ;

      // Method: Obtain a list of the first 'Count' fonts available to this PS.
      LONG QueryFonts ( LONG Count, PFONTMETRICS pfm ) ;

      // Method: Determine the bitmap format for this PS.
      void QueryBitmapFormat ( int &Planes, int &BitCount ) ;

      // Method: Create a bitmap of a specific size, compatible with this PS.
      HBITMAP CreateBitmap ( RECTL &Rectangle ) ;

      // Method: Create a bitmap of a specific size, compatible with this PS, and load it from another PS.
      HBITMAP CreateBitmap ( RECTL &Rectangle, WorkSpace &SourcePS ) ;

      // Method: Create a bitmap and load it from another.
      HBITMAP CreateBitmap ( HBITMAP Original ) ;

      // Method: Create a bitmap (though this really shouldn't need the PS to do)
      HBITMAP CreateBitmap ( BITMAPINFOHEADER2 &BitmapHeader ) ;

      // Method: Create a bitmap and load it (though this really shouldn't need the PS to do)
      HBITMAP CreateBitmap ( BITMAPINFOHEADER2 &BitmapHeader, PVOID pBuffer, PBITMAPINFO2 pbmi ) ;

      // Method: Load a bitmap from resource library.
      HBITMAP CreateBitmap ( HMODULE Library, int Id ) ;

      // Method: Unload bits from bitmap.
      LONG QueryBitmapBits ( int FirstLine, int LineCount, PVOID pBuffer, PBITMAPINFO2 pbmi ) ;

      // Method: Associate a bitmap with the PS.
      HBITMAP SetBitmap ( HBITMAP Bitmap ) ;

      // Method: Copy bitmap from another PS into this one.
      LONG Blit ( WorkSpace &Source, LONG lCount, PPOINTL aptlPoints, LONG lRop, ULONG flOptions ) ;

      // Establish clipping.
      void SetClipping ( RECTL &Clip ) ;

      // Stop clipping.
      void ResetClipping ( ) ;

      // Method: Draw Horizontal Separator
      int DrawSeparator ( POINTL &Position, SIZEL &Size ) ;

      // Method: Draw a bitmap on the PS.
      int MeasureBitmap ( HBITMAP Bitmap, SIZEL &Size ) ;
      int DrawBitmap ( HBITMAP Bitmap, POINTL &Position, SIZEL &Size, BOOL Halftone=FALSE, BOOL Reverse=FALSE ) ;

      // Method: Draw a metafile on the PS.
      int MeasureMetafile ( HMF Bitmap, RECTL &Rectangle ) ;
      int DrawMetafile ( HMF Bitmap, POINTL &Position, SIZEL &Size ) ;

      // Method: Draw a UPC-A Code on the PS.
      int DrawUpcCodeA ( char *BarCode, POINTL &Position, SIZEL &Size ) ;

      // Method: Draw a UPC-E Code on the PS.
      int DrawUpcCodeE ( char *BarCode, POINTL &Position, SIZEL &Size ) ;

      // Method: Draw a Code-39 on the PS.
      int DrawCode39 ( char *BarCode, POINTL &Position, SIZEL &Size ) ;

      // Method: Draw a POSTNET on the PS.
      int DrawPOSTNET ( char *BarCode, POINTL &Position, SIZEL &Size ) ;

      // Method: Draw a EAN-8 on the PS.
      int DrawEAN8 ( char *BarCode, POINTL &Position, SIZEL &Size ) ;

      // Method: Draw a EAN-13 on the PS.
      int DrawEAN13 ( char *BarCode, POINTL &Position, SIZEL &Size ) ;

      // Method: Set Graphic State from logical State.
      void SetState ( _Estado &State ) ;

      // Method: Issue a New Frame command to the device context.
      void NewFrame ( ) ;

      // Method: Query code page.
      int QueryCodepage ( ) ;

      // Method: Set code page.
      void SetCodepage ( int Codepage ) ;
      void SetCodepage ( ) ;

      // Method: Determine if byte is a DBCS header.
      BOOL IsDBCSHeader ( UCHAR Byte ) ;

      // Method: Report the DBCS Vector.
      PUCHAR Query_DBCS_Vector ( ) { return ( DBCSVector[0] ) ; }

      // Method: Insert comment in metafile.
      void Comment ( char *Message, ... ) ;

      // Method: Dump
      void Dump ( int indent=0 ) ;
} ;

#endif
