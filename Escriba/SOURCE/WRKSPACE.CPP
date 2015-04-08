/*************************************************************** WRKSPACE.CPP
 *                                                                          *
 *                        Graphic Workspace Class                           *
 *                                                                          *
 ****************************************************************************/

#include "System.h"

#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>

#include "WrkSpace.h"

// #define DEBUG
// #define DEBUG_CONSTRUCTOR
// #define DEBUG_INVERTCELL
// #define DEBUG_TRANSFORM
// #define DEBUG_BITMAP
// #define DEBUG_METAFILE
// #define DEBUG_DRAWTEXT
// #define DEBUG_POSTNET
// #define DEBUG_EAN8
// #define DEBUG_QUERYFONTS

#ifdef DEBUG
   #define DEBUG_CONSTRUCTOR
   #define DEBUG_INVERTCELL
   #define DEBUG_TRANSFORM
   #define DEBUG_BITMAP
   #define DEBUG_METAFILE
   #define DEBUG_DRAWTEXT
   #define DEBUG_POSTNET
   #define DEBUG_EAN8
   #define DEBUG_QUERYFONTS
#endif

/****************************************************************************
 *                                                                          *
 *  Constructor: Nonscaled cached PS associated with a window.              *
 *                                                                          *
 ****************************************************************************/

WorkSpace::WorkSpace ( char *name, int cp, HWND window, DeviceContext *pdevice ) :
   Type(WRKSPC_GETPS), hPS(0), 
   Name(name), CodePage(cp), Anchor(0), Window(window), pDevice(pdevice), 
   Scaled(FALSE), Metric(FALSE), Micro(FALSE), 
   #ifndef GPIMEASURE
      FontWidthsValid(FALSE), KerningPairCount(0), KerningPairs(0),
   #endif // Not GPIMEASURE.
   FontSize(120), LineColor(RGB_BLACK), LineType(LINETYPE_DEFAULT), 
   LineWidth(1), LineJoin(LINEJOIN_DEFAULT), LineEnd(LINEEND_DEFAULT), FillColor(RGB_WHITE), 
   FillType(PATSYM_DEFAULT), Bold(FALSE), Italic(FALSE), 
   Underscore(FALSE), Strikeout(FALSE), Outline(FALSE), Caps(FALSE),
   Expand(100), Mix(FM_DEFAULT), Ascent(0), Descent(0), AvgWidth(0), ClipRegion(0),
   Lock("WorkSpace::Lock") {

   #ifdef DEBUG_CONSTRUCTOR
      Log ( "WorkSpace(%08X:'%s')::Workspace() Creating nonscaled cached PS associated with window %08X (DC %08X).", this, Name, Window, pDevice->QueryHandle() ) ;
   #endif

   Anchor = WinQueryAnchorBlock ( Window ) ;
   memset ( FontName, 0, sizeof(FontName) ) ;
   #ifndef GPIMEASURE
      memset ( FontWidths, 0, sizeof(FontWidths) ) ;
      DosAllocMem ( PPVOID(&KerningPairs), MAX_KERNINGPAIRS*sizeof(KERNINGPAIRS), PAG_COMMIT | PAG_READ | PAG_WRITE ) ;
   #endif // Not GPIMEASURE.
   if ( pDevice ) pDevice->Lock ( ) ;  Lock.Request ( ) ;
   PageSize.cx = 0 ;
   PageSize.cy = 0 ;
   Reset ( TRUE ) ;
   Lock.Release ( ) ;  if ( pDevice ) pDevice->Unlock ( ) ;

   #ifdef DEBUG_CONSTRUCTOR
      Log ( "WorkSpace(%08X:'%s')::WorkSpace() Done.", this, Name ) ;
   #endif
}
 
/****************************************************************************
 *                                                                          *
 *  Constructor: Scaled cached PS associated with a window.                 *
 *                                                                          *
 ****************************************************************************/

WorkSpace::WorkSpace ( char *name, int cp, HWND window, DeviceContext *pdevice, int metric ) :
   Type(WRKSPC_GETPS), hPS(0), 
   Name(name), CodePage(cp), Anchor(0), Window(window), pDevice(pdevice), 
   Scaled(TRUE), Metric(metric), Micro(FALSE), 
   #ifndef GPIMEASURE
      FontWidthsValid(FALSE), KerningPairCount(0), KerningPairs(0),
   #endif // Not GPIMEASURE.
   FontSize(120), LineColor(RGB_BLACK), LineType(LINETYPE_DEFAULT), 
   LineWidth(1), LineJoin(LINEJOIN_DEFAULT), LineEnd(LINEEND_DEFAULT), FillColor(RGB_WHITE), 
   FillType(PATSYM_DEFAULT), Bold(FALSE), Italic(FALSE),
   Underscore(FALSE), Strikeout(FALSE), Outline(FALSE), Caps(FALSE),
   Expand(100), Mix(FM_DEFAULT), Ascent(0), Descent(0), AvgWidth(0), ClipRegion(0),
   Lock("WorkSpace::Lock") {

   #ifdef DEBUG_CONSTRUCTOR
      Log ( "WorkSpace(%08X:'%s')::WorkSpace() Creating scaled cached PS associated with window %08X (DC %08X).", this, Name, Window, pDevice->QueryHandle() ) ;
   #endif

   Anchor = WinQueryAnchorBlock ( Window ) ;
   memset ( FontName, 0, sizeof(FontName) ) ;
   #ifndef GPIMEASURE
      memset ( FontWidths, 0, sizeof(FontWidths) ) ;
      DosAllocMem ( PPVOID(&KerningPairs), MAX_KERNINGPAIRS*sizeof(KERNINGPAIRS), PAG_COMMIT | PAG_READ | PAG_WRITE ) ;
   #endif // Not GPIMEASURE.
   if ( pDevice ) pDevice->Lock ( ) ;  Lock.Request ( ) ;
   PageSize.cx = 0 ;
   PageSize.cy = 0 ;
   Reset ( TRUE ) ;
   SetTransform ( 1.0, 0, 0 ) ;
   Lock.Release ( ) ;  if ( pDevice ) pDevice->Unlock ( ) ;

   #ifdef DEBUG_CONSTRUCTOR
      Log ( "WorkSpace(%08X:'%s')::WorkSpace() Done.", this, Name ) ;
   #endif
}
 
/****************************************************************************
 *                                                                          *
 *  Constructor: Non-scaled cached PS for use in processing WM_PAINT.       *
 *                                                                          *
 ****************************************************************************/

WorkSpace::WorkSpace ( char *name, int cp, HWND window, DeviceContext *pdevice, RECTL &Clip ) :
   Type(WRKSPC_PAINT), hPS(0),
   Name(name), CodePage(cp), Anchor(0), Window(window), pDevice(pdevice), 
   Scaled(FALSE), Metric(FALSE), Micro(FALSE), 
   #ifndef GPIMEASURE
      FontWidthsValid(FALSE), KerningPairCount(0), KerningPairs(0),
   #endif // Not GPIMEASURE.
   FontSize(120), LineColor(RGB_BLACK), LineType(LINETYPE_DEFAULT), 
   LineWidth(1), LineJoin(LINEJOIN_DEFAULT), LineEnd(LINEEND_DEFAULT), FillColor(RGB_WHITE), 
   FillType(PATSYM_DEFAULT), Bold(FALSE), Italic(FALSE), 
   Underscore(FALSE), Strikeout(FALSE), Outline(FALSE), Caps(FALSE),
   Expand(100), Mix(FM_DEFAULT), Ascent(0), Descent(0), AvgWidth(0), ClipRegion(0),
   Lock("WorkSpace::Lock") {

   #ifdef DEBUG_CONSTRUCTOR
      Log ( "WorkSpace(%08X:'%s')::WorkSpace() Creating nonscaled cached PS for painting on window %08X (DC %08X).", this, Name, Window, pDevice->QueryHandle() ) ;
   #endif

   Anchor = WinQueryAnchorBlock ( Window ) ;
   memset ( FontName, 0, sizeof(FontName) ) ;
   #ifndef GPIMEASURE
      memset ( FontWidths, 0, sizeof(FontWidths) ) ;
      DosAllocMem ( PPVOID(&KerningPairs), MAX_KERNINGPAIRS*sizeof(KERNINGPAIRS), PAG_COMMIT | PAG_READ | PAG_WRITE ) ;
   #endif // Not GPIMEASURE.
   if ( pDevice ) pDevice->Lock ( ) ;  Lock.Request ( ) ;
   PageSize.cx = 0 ;
   PageSize.cy = 0 ;
   Reset ( TRUE ) ;
   Clip = ClippingRectangle ;
   Lock.Release ( ) ;  if ( pDevice ) pDevice->Unlock ( ) ;

   #ifdef DEBUG_CONSTRUCTOR
      Log ( "WorkSpace(%08X:'%s')::WorkSpace() Done.", this, Name ) ;
   #endif
}
 
/****************************************************************************
 *                                                                          *
 *  Constructor: Scaled cached PS for use in processing WM_PAINT.           *
 *                                                                          *
 ****************************************************************************/

WorkSpace::WorkSpace ( char *name, int cp, HWND window, DeviceContext *pdevice, int metric, RECTL &Clip ) :
   Type(WRKSPC_PAINT), hPS(0),
   Name(name), CodePage(cp), Anchor(0), Window(window), pDevice(pdevice), 
   Scaled(TRUE), Metric(metric), Micro(FALSE), 
   #ifndef GPIMEASURE
      FontWidthsValid(FALSE), KerningPairCount(0), KerningPairs(0),
   #endif // Not GPIMEASURE.
   FontSize(120), LineColor(RGB_BLACK), LineType(LINETYPE_DEFAULT), 
   LineWidth(1), LineJoin(LINEJOIN_DEFAULT), LineEnd(LINEEND_DEFAULT), FillColor(RGB_WHITE), 
   FillType(PATSYM_DEFAULT), Bold(FALSE), Italic(FALSE),
   Underscore(FALSE), Strikeout(FALSE), Outline(FALSE), Caps(FALSE),
   Expand(100), Mix(FM_DEFAULT), Ascent(0), Descent(0), AvgWidth(0), ClipRegion(0),
   Lock("WorkSpace::Lock") {

   #ifdef DEBUG_CONSTRUCTOR
      Log ( "WorkSpace(%08X:'%s')::WorkSpace() Creating scaled cached PS for painting on window %08X (DC %08X).", this, Name, Window, pDevice->QueryHandle() ) ;
   #endif

   Anchor = WinQueryAnchorBlock ( Window ) ;
   memset ( FontName, 0, sizeof(FontName) ) ;
   #ifndef GPIMEASURE
      memset ( FontWidths, 0, sizeof(FontWidths) ) ;
      DosAllocMem ( PPVOID(&KerningPairs), MAX_KERNINGPAIRS*sizeof(KERNINGPAIRS), PAG_COMMIT | PAG_READ | PAG_WRITE ) ;
   #endif // Not GPIMEASURE.
   if ( pDevice ) pDevice->Lock ( ) ;  Lock.Request ( ) ;
   PageSize.cx = 0 ;
   PageSize.cy = 0 ;
   Reset ( TRUE ) ;
   Clip = ClippingRectangle ;
   SetTransform ( 1.0, 0, 0 ) ;
   Lock.Release ( ) ;  if ( pDevice ) pDevice->Unlock ( ) ;

   #ifdef DEBUG_CONSTRUCTOR
      Log ( "WorkSpace(%08X:'%s')::WorkSpace() Done.", this, Name ) ;
   #endif
}
 
/****************************************************************************
 *                                                                          *
 *  Constructor: Non-scaled PS associated with a generic device.            *
 *                                                                          *
 ****************************************************************************/

WorkSpace::WorkSpace ( char *name, int cp, DeviceContext *pdevice, HAB anchor, int micro ) :
   Type(WRKSPC_GPI), hPS(0),
   Name(name), CodePage(cp), Anchor(anchor), Window(0), pDevice(pdevice), 
   Scaled(FALSE), Metric(FALSE), Micro(micro), 
   #ifndef GPIMEASURE
      FontWidthsValid(FALSE), KerningPairCount(0), KerningPairs(0),
   #endif // Not GPIMEASURE.
   FontSize(120), LineColor(RGB_BLACK), LineType(LINETYPE_DEFAULT), 
   LineWidth(1), LineJoin(LINEJOIN_DEFAULT), LineEnd(LINEEND_DEFAULT), FillColor(RGB_WHITE), 
   FillType(PATSYM_DEFAULT), Bold(FALSE), Italic(FALSE), 
   Underscore(FALSE), Strikeout(FALSE), Outline(FALSE), Caps(FALSE),
   Expand(100), Mix(FM_DEFAULT), Ascent(0), Descent(0), AvgWidth(0), ClipRegion(0),
   Lock("WorkSpace::Lock") {

   #ifdef DEBUG_CONSTRUCTOR
      Log ( "WorkSpace(%08X:'%s')::WorkSpace() Creating nonscaled PS associated with DC %08X.", this, Name, pDevice->QueryHandle() ) ;
   #endif

   memset ( FontName, 0, sizeof(FontName) ) ;
   #ifndef GPIMEASURE
      memset ( FontWidths, 0, sizeof(FontWidths) ) ;
      DosAllocMem ( PPVOID(&KerningPairs), MAX_KERNINGPAIRS*sizeof(KERNINGPAIRS), PAG_COMMIT | PAG_READ | PAG_WRITE ) ;
   #endif // Not GPIMEASURE.
   if ( pDevice ) pDevice->Lock ( ) ;  Lock.Request ( ) ;
   PageSize.cx = 0 ;
   PageSize.cy = 0 ;
   Reset ( TRUE ) ;
   Lock.Release ( ) ;  if ( pDevice ) pDevice->Unlock ( ) ;

   #ifdef DEBUG_CONSTRUCTOR
      Log ( "WorkSpace(%08X:'%s')::WorkSpace() Done.", this, Name ) ;
   #endif
}
 
/****************************************************************************
 *                                                                          *
 *  Constructor: Scaled PS associated with a generic device.                *
 *                                                                          *
 ****************************************************************************/

WorkSpace::WorkSpace ( char *name, int cp, DeviceContext *pdevice, HAB anchor, int metric, int micro ) :
   Type(WRKSPC_GPI), hPS(0),
   Name(name), CodePage(cp), Anchor(anchor), Window(0), pDevice(pdevice), 
   Scaled(TRUE), Metric(metric), Micro(micro), 
   #ifndef GPIMEASURE
      FontWidthsValid(FALSE), KerningPairCount(0), KerningPairs(0),
   #endif // Not GPIMEASURE.
   FontSize(120), LineColor(RGB_BLACK), LineType(LINETYPE_DEFAULT), 
   LineWidth(1), LineJoin(LINEJOIN_DEFAULT), LineEnd(LINEEND_DEFAULT), FillColor(RGB_WHITE), 
   FillType(PATSYM_DEFAULT), Bold(FALSE), Italic(FALSE),
   Underscore(FALSE), Strikeout(FALSE), Outline(FALSE), Caps(FALSE),
   Expand(100), Mix(FM_DEFAULT), Ascent(0), Descent(0), AvgWidth(0), ClipRegion(0),
   Lock("WorkSpace::Lock") {

   #ifdef DEBUG_CONSTRUCTOR
      Log ( "WorkSpace(%08X:'%s')::WorkSpace() Creating scaled PS associated with DC %08X (device %p).", this, Name, pDevice->QueryHandle(), pDevice ) ;
   #endif

   memset ( FontName, 0, sizeof(FontName) ) ;
   #ifndef GPIMEASURE
      memset ( FontWidths, 0, sizeof(FontWidths) ) ;
      DosAllocMem ( PPVOID(&KerningPairs), MAX_KERNINGPAIRS*sizeof(KERNINGPAIRS), PAG_COMMIT | PAG_READ | PAG_WRITE ) ;
   #endif // Not GPIMEASURE.
   if ( pDevice ) pDevice->Lock ( ) ;  Lock.Request ( ) ;
   PageSize.cx = 0 ;
   PageSize.cy = 0 ;
   Reset ( TRUE ) ;
   SetTransform ( 1.0, 0, 0 ) ;
   Lock.Release ( ) ;  if ( pDevice ) pDevice->Unlock ( ) ;

   #ifdef DEBUG_CONSTRUCTOR
      Log ( "WorkSpace(%08X:'%s')::WorkSpace() Done.", this, Name ) ;
   #endif
}
 
/****************************************************************************
 *                                                                          *
 *  Constructor: Scaled PS not associated with a device.                    *
 *                                                                          *
 ****************************************************************************/

WorkSpace::WorkSpace ( char *name, int cp, HAB anchor, int metric, int Width, int Height ) :
   Type(WRKSPC_GPI), hPS(0),
   Name(name), CodePage(cp), Anchor(anchor), Window(0), pDevice(0), 
   Scaled(TRUE), Metric(metric), Micro(FALSE), 
   #ifndef GPIMEASURE
      FontWidthsValid(FALSE), KerningPairCount(0), KerningPairs(0),
   #endif // Not GPIMEASURE.
   FontSize(120), LineColor(RGB_BLACK), LineType(LINETYPE_DEFAULT), 
   LineWidth(1), LineJoin(LINEJOIN_DEFAULT), LineEnd(LINEEND_DEFAULT), FillColor(RGB_WHITE), 
   FillType(PATSYM_DEFAULT), Bold(FALSE), Italic(FALSE),
   Underscore(FALSE), Strikeout(FALSE), Outline(FALSE), Caps(FALSE),
   Expand(100), Mix(FM_DEFAULT), Ascent(0), Descent(0), AvgWidth(0), ClipRegion(0),
   Lock("WorkSpace::Lock") {

   #ifdef DEBUG_CONSTRUCTOR
      Log ( "WorkSpace(%08X:'%s')::WorkSpace() Creating scaled PS not associated with any device.", this, Name ) ;
   #endif

   memset ( FontName, 0, sizeof(FontName) ) ;
   #ifndef GPIMEASURE
      memset ( FontWidths, 0, sizeof(FontWidths) ) ;
      DosAllocMem ( PPVOID(&KerningPairs), MAX_KERNINGPAIRS*sizeof(KERNINGPAIRS), PAG_COMMIT | PAG_READ | PAG_WRITE ) ;
   #endif // Not GPIMEASURE.
   if ( pDevice ) pDevice->Lock ( ) ;  Lock.Request ( ) ;  
   PageSize.cx = Width ;
   PageSize.cy = Height ;
   Reset ( TRUE ) ;
   SetTransform ( 1.0, 0, 0 ) ;
   Lock.Release ( ) ;  if ( pDevice ) pDevice->Unlock ( ) ;

   #ifdef DEBUG_CONSTRUCTOR
      Log ( "WorkSpace(%08X:'%s')::WorkSpace() Done.", this, Name ) ;
   #endif
}
 
/****************************************************************************
 *                                                                          *
 *  Constructor: Nonscaled PS associated with another task's window.        *
 *                                                                          *
 ****************************************************************************/

WorkSpace::WorkSpace ( char *name, HWND window ) :
   Type(WRKSPC_GETPS), hPS(0), 
   Name(name), CodePage(0), Anchor(0), Window(window), pDevice(0), 
   Scaled(FALSE), Metric(FALSE), Micro(FALSE), 
   #ifndef GPIMEASURE
      FontWidthsValid(FALSE), KerningPairCount(0), KerningPairs(0),
   #endif // Not GPIMEASURE.
   FontSize(120), LineColor(RGB_BLACK), LineType(LINETYPE_DEFAULT), 
   LineWidth(1), LineJoin(LINEJOIN_DEFAULT), LineEnd(LINEEND_DEFAULT), FillColor(RGB_WHITE), 
   FillType(PATSYM_DEFAULT), Bold(FALSE), Italic(FALSE), 
   Underscore(FALSE), Strikeout(FALSE), Outline(FALSE), Caps(FALSE),
   Expand(100), Mix(FM_DEFAULT), Ascent(0), Descent(0), AvgWidth(0), ClipRegion(0),
   Lock("WorkSpace::Lock") {

   #ifdef DEBUG_CONSTRUCTOR
      Log ( "WorkSpace(%08X:'%s')::WorkSpace() Creating nonscaled cached PS associated with window %08X (DC %08X).", this, Name, Window, pDevice->QueryHandle() ) ;
   #endif

   Anchor = WinQueryAnchorBlock ( Window ) ;
   memset ( FontName, 0, sizeof(FontName) ) ;
   #ifndef GPIMEASURE
      memset ( FontWidths, 0, sizeof(FontWidths) ) ;
      DosAllocMem ( PPVOID(&KerningPairs), MAX_KERNINGPAIRS*sizeof(KERNINGPAIRS), PAG_COMMIT | PAG_READ | PAG_WRITE ) ;
   #endif // Not GPIMEASURE.
   if ( pDevice ) pDevice->Lock ( ) ;  Lock.Request ( ) ;
   PageSize.cx = 0 ;
   PageSize.cy = 0 ;
   Reset ( TRUE ) ;
   Lock.Release ( ) ;  if ( pDevice ) pDevice->Unlock ( ) ;

   #ifdef DEBUG_CONSTRUCTOR
      Log ( "WorkSpace(%08X:'%s')::WorkSpace() Done.", this, Name ) ;
   #endif
}
 
/****************************************************************************
 *                                                                          *
 *  Constructor: Desktop Presentation Space                                 *
 *                                                                          *
 ****************************************************************************/

WorkSpace::WorkSpace ( char *name ) :
   Type(WRKSPC_GETPS), hPS(0), 
   Name(name), CodePage(0), Anchor(0), Window(HWND_DESKTOP), pDevice(0), 
   Scaled(FALSE), Metric(FALSE), Micro(FALSE), 
   #ifndef GPIMEASURE
      FontWidthsValid(FALSE), KerningPairCount(0), KerningPairs(0),
   #endif // Not GPIMEASURE.
   FontSize(120), LineColor(RGB_BLACK), LineType(LINETYPE_DEFAULT), 
   LineWidth(1), LineJoin(LINEJOIN_DEFAULT), LineEnd(LINEEND_DEFAULT), FillColor(RGB_WHITE), 
   FillType(PATSYM_DEFAULT), Bold(FALSE), Italic(FALSE), 
   Underscore(FALSE), Strikeout(FALSE), Outline(FALSE), Caps(FALSE),
   Expand(100), Mix(FM_DEFAULT), Ascent(0), Descent(0), AvgWidth(0), ClipRegion(0),
   Lock("WorkSpace::Lock") {

   #ifdef DEBUG_CONSTRUCTOR
      Log ( "WorkSpace(%08X:'%s')::WorkSpace() Creating nonscaled cached PS associated with the desktop window.", this, Name ) ;
   #endif

   memset ( FontName, 0, sizeof(FontName) ) ;
   #ifndef GPIMEASURE
      memset ( FontWidths, 0, sizeof(FontWidths) ) ;
      DosAllocMem ( PPVOID(&KerningPairs), MAX_KERNINGPAIRS*sizeof(KERNINGPAIRS), PAG_COMMIT | PAG_READ | PAG_WRITE ) ;
   #endif // Not GPIMEASURE.
   if ( pDevice ) pDevice->Lock ( ) ;  Lock.Request ( ) ;
   PageSize.cx = 0 ;
   PageSize.cy = 0 ;
   Reset ( TRUE ) ;
   Lock.Release ( ) ;  if ( pDevice ) pDevice->Unlock ( ) ;

   #ifdef DEBUG_CONSTRUCTOR
      Log ( "WorkSpace(%08X:'%s')::WorkSpace() Done.", this, Name ) ;
   #endif
}
 
/****************************************************************************
 *                                                                          *
 *  Destructor.                                                             *
 *                                                                          *
 ****************************************************************************/

WorkSpace::~WorkSpace ( ) {

   #ifdef DEBUG_CONSTRUCTOR
      Log ( "WorkSpace(%08X:'%s')::~WorkSpace() Started.", this, Name ) ;
   #endif

   DestroyPS ( ) ;

   #ifndef GPIMEASURE
      if ( KerningPairs )
         DosFreeMem ( KerningPairs ) ;
   #endif // Not GPIMEASURE.

   #ifdef DEBUG_CONSTRUCTOR
      Log ( "WorkSpace(%08X:'%s')::~WorkSpace() Done.", this, Name ) ;
   #endif
}
 
/****************************************************************************
 *                                                                          *
 *  Method: Create Presentation Space                                       *
 *                                                                          *
 ****************************************************************************/

void WorkSpace::CreatePS ( BOOL /* Completely */ ) {

   #ifdef DEBUG_CONSTRUCTOR
      Log ( "WorkSpace(%08X:'%s')::CreatePS() Started.", this, Name ) ;
   #endif

   // Lock the device against other threads.
   if ( pDevice ) pDevice->Lock ( ) ;  Lock.Request ( ) ;

   // Create the PS according to its type.
   switch ( Type ) {

      case WRKSPC_GETPS: { 
         if ( Window == HWND_DESKTOP ) {
            #ifdef DEBUG_CONSTRUCTOR
               Log ( "WorkSpace(%08X:'%s')::DestroyPS() Getting desktop PS.", this, Name ) ;
            #endif
            hPS = WinGetScreenPS ( HWND_DESKTOP ) ;
            if ( hPS == 0 ) {
               char Message [512] ;
               Log ( "WorkSpace(%08X:'%s')::CreatePS() Unable to obtain desktop PS: %s", this, Name, InterpretWinError ( Anchor, Message ) ) ;
            } else {
               if ( !GpiSetPS ( hPS, &PageSize, Scaled ? ( Metric ? PU_HIMETRIC : PU_HIENGLISH ) : PU_PELS ) ) {
                  char Message [512] ;
                  Log ( "WorkSpace(%08X:'%s')::CreatePS() Unable to set PS: %s", this, Name, InterpretWinError ( Anchor, Message ) ) ;
               } /* endif */
            } /* endif */
         } else {
            #ifdef DEBUG_CONSTRUCTOR
               Log ( "WorkSpace(%08X:'%s')::DestroyPS() Getting window PS.", this, Name ) ;
            #endif
            hPS = WinGetPS ( Window ) ;
            if ( hPS == 0 ) {
               char Message [512] ;
               Log ( "WorkSpace(%08X:'%s')::CreatePS() Unable to obtain window PS: %s", this, Name, InterpretWinError ( Anchor, Message ) ) ;
            } else {
               if ( !GpiSetPS ( hPS, &PageSize, Scaled ? ( Metric ? PU_HIMETRIC : PU_HIENGLISH ) : PU_PELS ) ) {
                  char Message [512] ;
                  Log ( "WorkSpace(%08X:'%s')::CreatePS() Unable to set PS: %s", this, Name, InterpretWinError ( Anchor, Message ) ) ;
               } /* endif */
            } /* endif */
         } /* endif */
         break; }

      case WRKSPC_PAINT: { 
         #ifdef DEBUG_CONSTRUCTOR
            Log ( "WorkSpace(%08X:'%s')::DestroyPS() Beginning paint.", this, Name ) ;
         #endif
         hPS = WinBeginPaint ( Window, 0, &ClippingRectangle ) ;
         if ( hPS == 0 ) {
            char Message [512] ;
            Log ( "WorkSpace(%08X:'%s')::CreatePS() Unable to obtain window PS: %s", this, Name, InterpretWinError ( Anchor, Message ) ) ;
         } else {
            if ( !GpiSetPS ( hPS, &PageSize, Scaled ? ( Metric ? PU_HIMETRIC : PU_HIENGLISH ) : PU_PELS ) ) {
               char Message [512] ;
               Log ( "WorkSpace(%08X:'%s')::CreatePS() Unable to set PS: %s", this, Name, InterpretWinError ( Anchor, Message ) ) ;
            } /* endif */
         } /* endif */
         break; }

      case WRKSPC_GPI: { 
         #ifdef DEBUG_CONSTRUCTOR
            Log ( "WorkSpace(%08X:'%s')::DestroyPS() Creating PS.", this, Name ) ;
         #endif
         hPS = GpiCreatePS ( Anchor, pDevice?pDevice->QueryHandle():0, &PageSize, 
            ( Scaled ? ( Metric ? PU_HIMETRIC : PU_HIENGLISH ) : PU_PELS ) | 
            ( pDevice ? GPIA_ASSOC : 0 ) | 
            ( Micro ? GPIT_MICRO : 0 ) ) ;
         if ( hPS == 0 ) {
            ERRORID Code = Sys_GetLastError ( Anchor ) ;
            if ( ( Code & 0xFFFF ) != PMERR_INV_HDC ) {
               char Message [512] ;
               Log ( "WorkSpace(%08X:'%s')::CreatePS() Unable to create PS: %s", this, Name, InterpretWinError ( Anchor, Message ) ) ;
            } else {
               Log ( "WorkSpace(%08X:'%s')::CreatePS() Unable to create PS with invalid DC %08X (device %p).", this, Name, pDevice->QueryHandle(), pDevice ) ;
            } /* endif */
         } /* endif */
         break; }

      default: {
         Log ( "WorkSpace(%08X:'%s')::CreatePS() Undefined PS type!", this, Name ) ;
         break; }

   } /* endswitch */

   // Release the device so other threads can get at it.
   Lock.Release ( ) ;  if ( pDevice ) pDevice->Unlock ( ) ;

   #ifdef DEBUG_CONSTRUCTOR
      Log ( "WorkSpace(%08X:'%s')::CreatePS() Done.", this, Name ) ;
   #endif

} /* endmethod */

/****************************************************************************
 *                                                                          *
 *  Method: Destroy Presentation Space                                      *
 *                                                                          *
 ****************************************************************************/

void WorkSpace::DestroyPS ( ) {

   #ifdef DEBUG_CONSTRUCTOR
      Log ( "WorkSpace(%08X:'%s')::DestroyPS() Started.", this, Name ) ;
   #endif

   // Lock the device against other threads.
   if ( pDevice ) pDevice->Lock ( ) ;  Lock.Request ( ) ;

   // Reset the clipping region, if any.
   if ( ClipRegion )
      ResetClipping ( ) ;

   // Reset the font cache.
   Fonts.Reset() ;

   // Destroy the PS according to its type.
   switch ( Type ) {

      case WRKSPC_GETPS:
         if ( hPS ) { 
            #ifdef DEBUG_CONSTRUCTOR
               Log ( "WorkSpace(%08X:'%s')::DestroyPS() Releasing PS.", this, Name ) ;
            #endif
            if ( !WinReleasePS ( hPS ) ) {
               char Message [512] ;
               Log ( "WorkSpace(%08X:'%s')::DestroyPS() Unable to release PS: %s", this, Name, InterpretWinError ( 0, Message ) ) ;
            } /* endif */
            hPS = 0 ;
         } /* endif */
         break;

      case WRKSPC_PAINT:
         if ( hPS ) { 
            #ifdef DEBUG_CONSTRUCTOR
               Log ( "WorkSpace(%08X:'%s')::DestroyPS() Ending paint.", this, Name ) ;
            #endif
            if ( !WinEndPaint ( hPS ) ) {
               char Message [512] ;
               Log ( "WorkSpace(%08X:'%s')::DestroyPS() Unable to end paint: %s", this, Name, InterpretWinError ( 0, Message ) ) ;
            } /* endif */
            hPS = 0 ;
         } /* endif */
         break;

      case WRKSPC_GPI:
         if ( hPS ) { 
            #ifdef DEBUG_CONSTRUCTOR
               Log ( "WorkSpace(%08X:'%s')::DestroyPS() Destroying PS.", this, Name ) ;
            #endif
            if ( !GpiDestroyPS ( hPS ) ) {
               char Message [512] ;
               Log ( "WorkSpace(%08X:'%s')::DestroyPS() Unable to destroy PS: %s", this, Name, InterpretWinError ( 0, Message ) ) ;
            } /* endif */
            hPS = 0 ;
         } /* endif */
         break;

      default: {
         Log ( "WorkSpace(%08X:'%s')::DestroyPS() Undefined PS type!", this, Name ) ;
         break; }

   } /* endswitch */

   // Release the device so other threads can get at it.
   Lock.Release ( ) ;  if ( pDevice ) pDevice->Unlock ( ) ;

   #ifdef DEBUG_CONSTRUCTOR
      Log ( "WorkSpace(%08X:'%s')::DestroyPS() Done.", this, Name ) ;
   #endif

} /* endmethod */

/****************************************************************************
 *                                                                          *
 *  Method: Reset Workspace                                                 *
 *                                                                          *
 ****************************************************************************/

void WorkSpace::Reset ( int codepage, int metric, int width, int height ) {
   CodePage = codepage ;
   Metric = metric ;
   PageSize.cx = width ;
   PageSize.cy = height ;
   Reset ( ) ;
} /* endmethod */

void WorkSpace::Reset ( BOOL Completely ) {

   #ifdef DEBUG
      Log ( "WorkSpace(%08X:'%s')::Reset()", this, Name ) ;
   #endif

   // Lock the device context and this object.
   if ( pDevice ) pDevice->Lock ( ) ;  Lock.Request ( ) ;

   // Destroy the existing PS, if any.
   DestroyPS ( ) ;

   // Create a new POS, if necessary or required.
   if ( ( hPS == 0 ) || ( Completely && ( ( pDevice == 0 ) || ( pDevice->QueryType() != DEVICE_PRINTER ) ) ) ) 
      CreatePS ( Completely ) ;

   // Finish up with the presentation space initialization.
   Fonts.Set_CodePage ( CodePage ) ;
   memset ( FontName, 0, sizeof(FontName) ) ;
   #ifndef GPIMEASURE
      memset ( FontWidths, 0, sizeof(FontWidths) ) ;
   #endif // Not GPIMEASURE.
   SetCodepage ( ) ;
   if ( !GpiCreateLogColorTable ( hPS, LCOL_RESET, LCOLF_RGB, 0, 0, 0 ) ) {
      char Message [512] ;
      Log ( "WorkSpace(%08X:'%s')::Reset: Unable to create logical color table: %s", this, Name, InterpretWinError ( Anchor, Message ) ) ;
   } /* endif */
   GpiOpenSegment ( hPS, 0 ) ;
   Comment ( "WorkSpace::Reset()." ) ;
   SetColor ( LineColor ) ;
   SetLineType ( LineType ) ;
   SetLineWidth ( LineWidth ) ;
   SetLineJoin ( LineJoin ) ;
   SetLineEnd ( LineEnd ) ;
   SetFillType ( FillType ) ;
   SetMix ( Mix ) ;
   GpiCloseSegment ( hPS ) ;

   // Unlock everything.
   Lock.Release ( ) ;  if ( pDevice ) pDevice->Unlock ( ) ;
}
 
/****************************************************************************
 *                                                                          *
 *  Method: Move To Point (Set Position)                                    *
 *                                                                          *
 ****************************************************************************/

BOOL WorkSpace::Move ( int x, int y ) {
   POINTL Point = { x, y } ;
   return ( Move ( Point ) ) ;
}

BOOL WorkSpace::Move ( POINTL &Point ) {

   #ifdef DEBUG
   Log ( "WorkSpace(%08X,'%s')::Move(%i,%i)", this, Name, Point.x, Point.y ) ;
   #endif

   if ( pDevice ) pDevice->Lock ( ) ;  Lock.Request ( ) ;

   BOOL Result = GpiMove ( hPS, &Point ) ;
   if ( Result == FALSE ) {
      char Message [512] ;
      Log ( "WorkSpace(%08X:'%s')::Move: Unable to set graphic position: %s", this, Name, InterpretWinError ( Anchor, Message ) ) ;
   } /* endif */

   Lock.Release ( ) ;  if ( pDevice ) pDevice->Unlock ( ) ;

   return ( Result ) ;
}
 
/****************************************************************************
 *                                                                          *
 *  Method: Draw Line To Point (Simple)                                     *
 *                                                                          *
 ****************************************************************************/

long WorkSpace::DrawLine ( int x, int y ) {
   POINTL Point = { x, y } ;
   return ( DrawLine ( Point ) ) ;
}

long WorkSpace::DrawLine ( POINTL &Point ) {

   #ifdef DEBUG
   Log ( "WorkSpace(%08X,'%s')::DrawLine(%i,%i)", this, Name, Point.x, Point.y ) ;
   #endif

   if ( pDevice ) pDevice->Lock ( ) ;  Lock.Request ( ) ;
   long Result = GpiLine ( hPS, &Point ) ;
   if ( Result == FALSE ) {
      char Message [512] ;
      Log ( "WorkSpace(%08X:'%s')::DrawLine: Unable to draw line: %s", this, Name, InterpretWinError ( Anchor, Message ) ) ;
   } /* endif */
   Lock.Release ( ) ;  if ( pDevice ) pDevice->Unlock ( ) ;
   return ( Result ) ;
}
 
/****************************************************************************
 *                                                                          *
 *  Method: Draw Line To Point (Geometric)                                  *
 *                                                                          *
 ****************************************************************************/

long WorkSpace::DrawLineGeom ( int x, int y ) {
   POINTL Point = { x, y } ;
   return ( DrawLineGeom ( Point ) ) ;
}

long WorkSpace::DrawLineGeom ( POINTL &Point ) {

   #ifdef DEBUG
   Log ( "WorkSpace(%08X,'%s')::DrawLineGeom(%i,%i)", this, Name, Point.x, Point.y ) ;
   #endif

   if ( pDevice ) pDevice->Lock ( ) ;  Lock.Request ( ) ;

   if ( !GpiBeginPath ( hPS, 1 ) ) {
      char Message [512] ;
      Log ( "WorkSpace(%08X:'%s')::DrawLineGeom: Unable to begin path: %s", this, Name, InterpretWinError ( Anchor, Message ) ) ;
   } /* endif */

   long Result = GpiLine ( hPS, &Point ) ;
   if ( Result == FALSE ) {
      char Message [512] ;
      Log ( "WorkSpace(%08X:'%s')::DrawLineGeom: Unable to draw line: %s", this, Name, InterpretWinError ( Anchor, Message ) ) ;
   } /* endif */

   if ( !GpiEndPath ( hPS ) ) {
      char Message [512] ;
      Log ( "WorkSpace(%08X:'%s')::DrawLineGeom: Unable to end path: %s", this, Name, InterpretWinError ( Anchor, Message ) ) ;
   } /* endif */

   if ( !GpiSetPattern ( hPS, PATSYM_SOLID ) ) {
      char Message [512] ;
      Log ( "WorkSpace(%08X:'%s')::DrawLineGeom: Unable to set fill pattern: %s", this, Name, InterpretWinError ( Anchor, Message ) ) ;
   } /* endif */

   if ( !GpiSetLineType ( hPS, LineType ) ) {
      char Message [512] ;
      Log ( "WorkSpace(%08X:'%s')::DrawLineGeom: Unable to set line type: %s", this, Name, InterpretWinError ( Anchor, Message ) ) ;
   } /* endif */

   if ( !GpiSetLineWidthGeom ( hPS, LineWidth ) ) {
      char Message [512] ;
      Log ( "WorkSpace(%08X:'%s')::DrawLineGeom: Unable to set geometric line width: %s", this, Name, InterpretWinError ( Anchor, Message ) ) ;
   } /* endif */

   if ( !GpiSetLineJoin ( hPS, LineJoin ) ) {
      char Message [512] ;
      Log ( "WorkSpace(%08X:'%s')::DrawLineGeom: Unable to set line join type: %s", this, Name, InterpretWinError ( Anchor, Message ) ) ;
   } /* endif */

   if ( !GpiSetLineEnd ( hPS, LineEnd ) ) {
      char Message [512] ;
      Log ( "WorkSpace(%08X:'%s')::DrawLineGeom: Unable to set line end type: %s", this, Name, InterpretWinError ( Anchor, Message ) ) ;
   } /* endif */

   if ( !GpiSetColor ( hPS, LineColor ) ) {
      char Message [512] ;
      Log ( "WorkSpace(%08X:'%s')::DrawLineGeom: Unable to set line color: %s", this, Name, InterpretWinError ( Anchor, Message ) ) ;
   } /* endif */

   if ( GpiStrokePath ( hPS, 1, 0 ) == GPI_ERROR ) {
      char Message [512] ;
      Log ( "WorkSpace(%08X:'%s')::DrawLineGeom: Unable to stroke path: %s", this, Name, InterpretWinError ( Anchor, Message ) ) ;
   } /* endif */

   Lock.Release ( ) ;  if ( pDevice ) pDevice->Unlock ( ) ;

   return ( Result ) ;
}
 
/****************************************************************************
 *                                                                          *
 *  Method: Draw Poly Line                                                  *
 *                                                                          *
 ****************************************************************************/

long WorkSpace::DrawPolyLine ( int Count, POINTL Points[] ) {

   if ( pDevice ) pDevice->Lock ( ) ;  Lock.Request ( ) ;

   if ( !GpiBeginPath ( hPS, 1 ) ) {
      char Message [512] ;
      Log ( "WorkSpace(%08X:'%s')::DrawPolyLine: Unable to begin path: %s", this, Name, InterpretWinError ( Anchor, Message ) ) ;
   } /* endif */

   if ( !GpiMove ( hPS, Points ) ) {
      char Message [512] ;
      Log ( "WorkSpace(%08X:'%s')::DrawPolyLine: Unable to move to first point: %s", this, Name, InterpretWinError ( Anchor, Message ) ) ;
   } /* endif */

   long Result = GpiPolyLine ( hPS, Count, Points ) ;
   if ( !Result ) {
      char Message [512] ;
      Log ( "WorkSpace(%08X:'%s')::DrawPolyLine: Unable to draw polyline: %s", this, Name, InterpretWinError ( Anchor, Message ) ) ;
   } /* endif */

   if ( !GpiEndPath ( hPS ) ) {
      char Message [512] ;
      Log ( "WorkSpace(%08X:'%s')::DrawPolyLine: Unable to end path: %s", this, Name, InterpretWinError ( Anchor, Message ) ) ;
   } /* endif */

   if ( !GpiSetPattern ( hPS, PATSYM_SOLID ) ) {
      char Message [512] ;
      Log ( "WorkSpace(%08X:'%s')::DrawPolyLine: Unable to set fill pattern: %s", this, Name, InterpretWinError ( Anchor, Message ) ) ;
   } /* endif */

   if ( !GpiSetLineType ( hPS, LineType ) ) {
      char Message [512] ;
      Log ( "WorkSpace(%08X:'%s')::DrawPolyLine: Unable to set line type: %s", this, Name, InterpretWinError ( Anchor, Message ) ) ;
   } /* endif */

   if ( !GpiSetLineWidthGeom ( hPS, LineWidth ) ) {
      char Message [512] ;
      Log ( "WorkSpace(%08X:'%s')::DrawPolyLine: Unable to set geometric line width: %s", this, Name, InterpretWinError ( Anchor, Message ) ) ;
   } /* endif */

   if ( !GpiSetLineJoin ( hPS, LineJoin ) ) {
      char Message [512] ;
      Log ( "WorkSpace(%08X:'%s')::DrawPolyLine: Unable to set line join type: %s", this, Name, InterpretWinError ( Anchor, Message ) ) ;
   } /* endif */

   if ( !GpiSetLineEnd ( hPS, LineEnd ) ) {
      char Message [512] ;
      Log ( "WorkSpace(%08X:'%s')::DrawPolyLine: Unable to set line end type: %s", this, Name, InterpretWinError ( Anchor, Message ) ) ;
   } /* endif */

   if ( !GpiSetColor ( hPS, LineColor ) ) {
      char Message [512] ;
      Log ( "WorkSpace(%08X:'%s')::DrawPolyLine: Unable to set line color: %s", this, Name, InterpretWinError ( Anchor, Message ) ) ;
   } /* endif */

   if ( GpiStrokePath ( hPS, 1, 0 ) == GPI_ERROR ) {
      char Message [512] ;
      Log ( "WorkSpace(%08X:'%s')::DrawPolyLine: Unable to stroke path: %s", this, Name, InterpretWinError ( Anchor, Message ) ) ;
   } /* endif */

   Lock.Release ( ) ;  if ( pDevice ) pDevice->Unlock ( ) ;

   return ( Result ) ;
}
 
/****************************************************************************
 *                                                                          *
 *  Method: Draw Polygon and fill it                                        *
 *                                                                          *
 ****************************************************************************/

long WorkSpace::DrawPolygon ( int Count, POINTL Points[] ) {

   if ( pDevice ) pDevice->Lock ( ) ;  Lock.Request ( ) ;

   // Fill the polygon first.
   if ( !GpiBeginPath ( hPS, 1 ) ) {
      char Message [512] ;
      Log ( "WorkSpace(%08X:'%s')::DrawPolygon: Unable to begin path: %s", this, Name, InterpretWinError ( Anchor, Message ) ) ;
   } /* endif */

   if ( !GpiMove ( hPS, Points ) ) {
      char Message [512] ;
      Log ( "WorkSpace(%08X:'%s')::DrawPolygon: Unable to move to first point: %s", this, Name, InterpretWinError ( Anchor, Message ) ) ;
   } /* endif */

   if ( !GpiPolyLine ( hPS, Count, Points ) ) {
      char Message [512] ;
      Log ( "WorkSpace(%08X:'%s')::DrawPolygon: Unable to draw polyline: %s", this, Name, InterpretWinError ( Anchor, Message ) ) ;
   } /* endif */

   if ( !GpiLine ( hPS, Points ) ) {
      char Message [512] ;
      Log ( "WorkSpace(%08X:'%s')::DrawPolygon: Unable to draw line from last point to start: %s", this, Name, InterpretWinError ( Anchor, Message ) ) ;
   } /* endif */

   if ( !GpiEndPath ( hPS ) ) {
      char Message [512] ;
      Log ( "WorkSpace(%08X:'%s')::DrawPolygon: Unable to end path: %s", this, Name, InterpretWinError ( Anchor, Message ) ) ;
   } /* endif */

   if ( !GpiSetPattern ( hPS, FillType ) ) {
      char Message [512] ;
      Log ( "WorkSpace(%08X:'%s')::DrawPolygon: Unable to set fill pattern: %s", this, Name, InterpretWinError ( Anchor, Message ) ) ;
   } /* endif */

   if ( !GpiSetColor ( hPS, FillColor ) ) {
      char Message [512] ;
      Log ( "WorkSpace(%08X:'%s')::DrawPolygon: Unable to set fill color: %s", this, Name, InterpretWinError ( Anchor, Message ) ) ;
   } /* endif */

   if ( GpiFillPath ( hPS, 1, FPATH_ALTERNATE ) == GPI_ERROR ) {
      char Message [512] ;
      Log ( "WorkSpace(%08X:'%s')::DrawPolygon: Unable to fill path: %s", this, Name, InterpretWinError ( Anchor, Message ) ) ;
   } /* endif */

   // Then draw it's borders.
   if ( !GpiBeginPath ( hPS, 1 ) ) {
      char Message [512] ;
      Log ( "WorkSpace(%08X:'%s')::DrawPolygon: Unable to begin path: %s", this, Name, InterpretWinError ( Anchor, Message ) ) ;
   } /* endif */

   if ( !GpiMove ( hPS, Points ) ) {
      char Message [512] ;
      Log ( "WorkSpace(%08X:'%s')::DrawPolygon: Unable to move to first point: %s", this, Name, InterpretWinError ( Anchor, Message ) ) ;
   } /* endif */

   long Result = GpiPolyLine ( hPS, Count, Points ) ;
   if ( !Result ) {
      char Message [512] ;
      Log ( "WorkSpace(%08X:'%s')::DrawPolygon: Unable to draw polyline: %s", this, Name, InterpretWinError ( Anchor, Message ) ) ;
   } /* endif */

   if ( !GpiLine ( hPS, Points ) ) {
      char Message [512] ;
      Log ( "WorkSpace(%08X:'%s')::DrawPolygon: Unable to draw line from last point to start: %s", this, Name, InterpretWinError ( Anchor, Message ) ) ;
   } /* endif */

   if ( !GpiEndPath ( hPS ) ) {
      char Message [512] ;
      Log ( "WorkSpace(%08X:'%s')::DrawPolygon: Unable to end path: %s", this, Name, InterpretWinError ( Anchor, Message ) ) ;
   } /* endif */

   if ( !GpiSetPattern ( hPS, PATSYM_SOLID ) ) {
      char Message [512] ;
      Log ( "WorkSpace(%08X:'%s')::DrawPolygon: Unable to set fill pattern: %s", this, Name, InterpretWinError ( Anchor, Message ) ) ;
   } /* endif */

   if ( !GpiSetLineType ( hPS, LineType ) ) {
      char Message [512] ;
      Log ( "WorkSpace(%08X:'%s')::DrawPolygon: Unable to set line type: %s", this, Name, InterpretWinError ( Anchor, Message ) ) ;
   } /* endif */

   if ( !GpiSetLineWidthGeom ( hPS, LineWidth ) ) {
      char Message [512] ;
      Log ( "WorkSpace(%08X:'%s')::DrawPolygon: Unable to set geometric line width: %s", this, Name, InterpretWinError ( Anchor, Message ) ) ;
   } /* endif */

   if ( !GpiSetLineJoin ( hPS, LineJoin ) ) {
      char Message [512] ;
      Log ( "WorkSpace(%08X:'%s')::DrawPolygon: Unable to set line join type: %s", this, Name, InterpretWinError ( Anchor, Message ) ) ;
   } /* endif */

   if ( !GpiSetLineEnd ( hPS, LineEnd ) ) {
      char Message [512] ;
      Log ( "WorkSpace(%08X:'%s')::DrawPolygon: Unable to set line end type: %s", this, Name, InterpretWinError ( Anchor, Message ) ) ;
   } /* endif */

   if ( !GpiSetColor ( hPS, LineColor ) ) {
      char Message [512] ;
      Log ( "WorkSpace(%08X:'%s')::DrawPolygon: Unable to set line color: %s", this, Name, InterpretWinError ( Anchor, Message ) ) ;
   } /* endif */

   if ( GpiStrokePath ( hPS, 1, 0 ) == GPI_ERROR ) {
      char Message [512] ;
      Log ( "WorkSpace(%08X:'%s')::DrawPolygon: Unable to stroke path: %s", this, Name, InterpretWinError ( Anchor, Message ) ) ;
   } /* endif */

   Lock.Release ( ) ;  if ( pDevice ) pDevice->Unlock ( ) ;

   return ( Result ) ;
}
 
/****************************************************************************
 *                                                                          *
 *  Method: Draw Box                                                        *
 *                                                                          *
 ****************************************************************************/

long WorkSpace::DrawBox ( RECTL &Rectangle ) {

   #ifdef DEBUG
   Log ( "WorkSpace(%08X,'%s')::DrawBox(%i,%i)", this, Name, Rectangle.xRight, Rectangle.yTop ) ;
   #endif

   if ( pDevice ) pDevice->Lock ( ) ;  Lock.Request ( ) ;

   Move ( *PPOINTL(&Rectangle.xLeft) ) ;

   long Result = GpiBox ( hPS, DRO_OUTLINE, PPOINTL(&Rectangle.xRight), 0, 0 ) ;
   if ( Result == GPI_ERROR ) {
      char Message [512] ;
      Log ( "WorkSpace(%08X:'%s')::DrawBox: Unable to draw box: %s", this, Name, InterpretWinError ( Anchor, Message ) ) ;
   } /* endif */

   Lock.Release ( ) ;  if ( pDevice ) pDevice->Unlock ( ) ;

   return ( Result ) ;
}
 
/****************************************************************************
 *                                                                          *
 *  Method: Draw Filled Box                                                 *
 *                                                                          *
 ****************************************************************************/

long WorkSpace::FillBox ( RECTL &Rectangle ) {

   #ifdef DEBUG
   Log ( "Workspace(%08X,'%s')::FillBox(%i,%i)", this, Name, Rectangle.xRight, Rectangle.yTop ) ;
   #endif

   if ( pDevice ) pDevice->Lock ( ) ;  Lock.Request ( ) ;

   Move ( *PPOINTL(&Rectangle.xLeft) ) ;

   long Result = GpiBox ( hPS, DRO_FILL, PPOINTL(&Rectangle.xRight), 0, 0 ) ;
   if ( Result == GPI_ERROR ) {
      char Message [512] ;
      Log ( "WorkSpace(%08X:'%s')::FillBox: Unable to draw box: %s", this, Name, InterpretWinError ( Anchor, Message ) ) ;
   } /* endif */

   Lock.Release ( ) ;  if ( pDevice ) pDevice->Unlock ( ) ;

   return ( Result ) ;
}
 
/****************************************************************************
 *                                                                          *
 *  Method: Invert Box                                                      *
 *                                                                          *
 ****************************************************************************/

BOOL WorkSpace::InvertBox ( RECTL &Rectangle ) {

   #ifdef DEBUG
   Log ( "WorkSpace(%08X:'%s')::InvertBox(%i,%i-%i,%i)", this, Name,
      Rectangle.xLeft, Rectangle.yBottom, Rectangle.xRight, Rectangle.yTop ) ;
   #endif

   if ( pDevice ) pDevice->Lock ( ) ;  Lock.Request ( ) ;

   RECTL Box = Rectangle ;
   Transform ( CVTC_DEFAULTPAGE, CVTC_DEVICE, Box ) ;

   BOOL Result = WinInvertRect ( hPS, &Box ) ;

   Lock.Release ( ) ;  if ( pDevice ) pDevice->Unlock ( ) ;

   return ( Result ) ;
}
 
/****************************************************************************
 *                                                                          *
 *  Method: Draw Tracking Box                                               *
 *                                                                          *
 ****************************************************************************/

void WorkSpace::DrawTrackingBox ( RECTL &Rectangle ) {

   #ifdef DEBUG
   Log ( "WorkSpace(%08X:'%s')::DrawTrackingBox(%i,%i-%i,%i)", this, Name,
      Rectangle.xLeft, Rectangle.yBottom, Rectangle.xRight, Rectangle.yTop ) ;
   #endif

   if ( pDevice ) pDevice->Lock ( ) ;  Lock.Request ( ) ;

   if ( !GpiSetMix ( hPS, FM_INVERT ) ) {
      char Message [512] ;
      Log ( "WorkSpace(%08X:'%s')::DrawTrackingBox: Unable to set mix: %s", this, Name, InterpretWinError ( Anchor, Message ) ) ;
   } /* endif */

   if ( !GpiSetLineType ( hPS, LINETYPE_ALTERNATE ) ) {
      char Message [512] ;
      Log ( "WorkSpace(%08X:'%s')::DrawTrackingBox: Unable to set line type: %s", this, Name, InterpretWinError ( Anchor, Message ) ) ;
   } /* endif */

   DrawBox ( Rectangle ) ;

   if ( !GpiSetLineType ( hPS, LineType ) ) {
      char Message [512] ;
      Log ( "WorkSpace(%08X:'%s')::DrawTrackingBox: Unable to restore line type: %s", this, Name, InterpretWinError ( Anchor, Message ) ) ;
   } /* endif */

   if ( !GpiSetMix ( hPS, Mix ) ) {
      char Message [512] ;
      Log ( "WorkSpace(%08X:'%s')::DrawTrackingBox: Unable to restore mix: %s", this, Name, InterpretWinError ( Anchor, Message ) ) ;
   } /* endif */

   Lock.Release ( ) ;  if ( pDevice ) pDevice->Unlock ( ) ;
}
 
/****************************************************************************
 *                                                                          *
 *  Method: Query Text Box                                                  *
 *                                                                          *
 ****************************************************************************/

BOOL WorkSpace::QueryTextBox ( char *Text, int Length, POINTL TextBox[TXTBOX_COUNT] ) {

   #ifdef DEBUG
   Log ( "WorkSpace(%08X:'%s')::QueryTextBox(\"%0.*s\")", this, Name, Length, Text ) ;
   #endif

#ifdef GPIMEASURE

   if ( pDevice ) pDevice->Lock ( ) ;  Lock.Request ( ) ;

   if ( Caps ) {
      COUNTRYCODE Country = { 0, CodePage } ;
      DosMapCase ( Length, &Country, PCHAR(Text) ) ;
   } /* endif */

   BOOL Result = GpiQueryTextBox ( hPS, Length, PCH(Text), TXTBOX_COUNT, TextBox ) ;
   if ( !Result ) {
      char Message [512] ;
      Log ( "WorkSpace(%08X:'%s')::QueryTextBox: Unable to query text box: %s", this, Name, InterpretWinError ( Anchor, Message ) ) ;
   } /* endif */

   Lock.Release ( ) ;  if ( pDevice ) pDevice->Unlock ( ) ;

   return ( Result ) ;

#else

   int Width = MeasureText ( Text, Length ) ;

   TextBox[TXTBOX_BOTTOMLEFT ].x = 0 ;
   TextBox[TXTBOX_BOTTOMLEFT ].y = -Descent + 1 ;

   TextBox[TXTBOX_TOPLEFT    ].x = 0 ;
   TextBox[TXTBOX_TOPLEFT    ].y = Ascent ;

   TextBox[TXTBOX_BOTTOMRIGHT].x = Width ;
   TextBox[TXTBOX_BOTTOMRIGHT].y = -Descent + 1 ;

   TextBox[TXTBOX_TOPRIGHT   ].x = Width ;
   TextBox[TXTBOX_TOPRIGHT   ].y = Ascent ;

   TextBox[TXTBOX_CONCAT     ].x = Width ;
   TextBox[TXTBOX_CONCAT     ].y = 0 ;

   return ( TRUE ) ;

#endif

}
 
/****************************************************************************
 *                                                                          *
 *  Method: Measure Text                                                    *
 *                                                                          *
 ****************************************************************************/

int WorkSpace::MeasureText ( char *Text ) {
   return ( MeasureText ( Text, int(strlen(Text)) ) ) ;
}

int WorkSpace::MeasureText ( char *Text, int Length ) {
   SIZEL Size ;
   return ( MeasureText ( Text, Length, Size ) ) ;
}

int WorkSpace::MeasureText ( char *Text, SIZEL &Size ) {
   return ( MeasureText ( Text, int(strlen(Text)), Size ) ) ;
}

int WorkSpace::MeasureText ( char *Text, int Length, SIZEL &Size ) {

   #ifdef DEBUG
      Log ( "WorkSpace(%08X:'%s')::MeasureText(\"%0.*s\")", this, Name, Length, Text ) ;
   #endif

   if ( pDevice ) pDevice->Lock ( ) ;  Lock.Request ( ) ;

   // Capitalize if necessary.
   if ( Caps ) {
      COUNTRYCODE Country = { 0, CodePage } ;
      DosMapCase ( Length, &Country, PCHAR(Text) ) ;
   } /* endif */

#ifndef GPIMEASURE
   if ( ! FontWidthsValid ) {
#endif // Not GPIMEASURE.

      // Find out the text box size.
      POINTL TextBox [TXTBOX_COUNT] ;
      if ( !GpiQueryTextBox ( hPS, Length, PCH(Text), TXTBOX_COUNT, TextBox ) ) {
         char Message [512] ;
         Log ( "WorkSpace(%08X:'%s')::MeasureText: Unable to query text box: %s", this, Name, InterpretWinError ( Anchor, Message ) ) ;
      } /* endif */

      // Extract the text box width and height.
      Size.cx = TextBox[TXTBOX_CONCAT].x ;
      Size.cy = TextBox[TXTBOX_TOPLEFT].y - TextBox[TXTBOX_BOTTOMLEFT].y ;

#ifndef GPIMEASURE

   } else {

      // Compute the text box height.
      Size.cy = Ascent + Descent ;

      // Compute the text box width.
      Size.cx = 0 ;
      for ( int i=0; i<Length; i++ ) {
         Size.cx += FontWidths [ UCHAR( Text[i] ) ] ;
         if ( KerningPairs && ( i < Length-1 ) ) {
            for ( int j=0; j<KerningPairCount; j++ ) {
               if ( KerningPairs[j].sFirstChar == Text[i] ) {
                  if ( KerningPairs[j].sSecondChar == Text[i+1] ) {
                     Size.cx += KerningPairs[j].lKerningAmount ;
                     break ;
                  } /* endif */
               } /* endif */
            } /* endfor */
         } /* endif */
      } /* endfor */

   } /* endif */

#endif // Not GPIMEASURE.

   Lock.Release ( ) ;  if ( pDevice ) pDevice->Unlock ( ) ;

   // Return the box width as the function return value.
   return ( int(Size.cx) ) ;
}
 
/****************************************************************************
 *                                                                          *
 *  Method: Invert Cell                                                     *
 *                                                                          *
 ****************************************************************************/

int WorkSpace::InvertCell ( SIZEL Size ) {

   if ( Size.cx == 0 )
      return ( 0 ) ;

   if ( pDevice ) pDevice->Lock ( ) ;  Lock.Request ( ) ;

   POINTL Cursor ;
   GpiQueryCurrentPosition ( hPS, &Cursor ) ;

   RECTL Cell ;
   Cell.xLeft   = Cursor.x ;
   Cell.yBottom = Cursor.y - Descent + 1 ;
   Cell.xRight  = Cell.xLeft + Size.cx ;
   Cell.yTop    = Cell.yBottom + Size.cy ;

   RECTL WindowCell = Cell ;
   Transform ( CVTC_DEFAULTPAGE, CVTC_DEVICE, WindowCell ) ;

   #ifdef DEBUG_INVERTCELL
   Log ( "WorkSpace(%08X:'%s')::InvertCell %i,%i (%ix%i) WindowCell %i,%i (%i,%i).", this, Name, 
      Cursor.x, Cursor.y-Descent+1, Size.cx, Size.cy, WindowCell.xLeft, WindowCell.yBottom, 
      WindowCell.xRight-WindowCell.xLeft, WindowCell.yTop-WindowCell.yBottom ) ;
   #endif

   WinInvertRect ( hPS, &WindowCell ) ;

   Lock.Release ( ) ;  if ( pDevice ) pDevice->Unlock ( ) ;

   return ( int(Size.cx) ) ;
}
 
/****************************************************************************
 *                                                                          *
 *  Method: Invert Text Cell                                                *
 *                                                                          *
 ****************************************************************************/

int WorkSpace::InvertText ( char *Text, int Length ) {

   #ifdef DEBUG
   Log ( "WorkSpace(%08X:'%s')::InvertText(\"%0.*s\")", this, Name, Length, Text ) ;
   #endif

   SIZEL Size ;
   MeasureText ( Text, Length, Size ) ;

   InvertCell ( Size ) ;

   return ( int(Size.cx) ) ;
}
 
/****************************************************************************
 *                                                                          *
 *  Method: Draw Text (the easy way)                                        *
 *                                                                          *
 ****************************************************************************/

long WorkSpace::DrawText ( char *Text, int Length, RECTL &Rectangle, COLOR TextColor, COLOR Background, long Flags ) {

   #ifdef DEBUG_DRAWTEXT
   Log ( "WorkSpace(%08X:'%s')::DrawText(\"%0.*s\") CodePage %i.", this, Name, Length, Text, QueryCodepage() ) ;
   #endif

   if ( pDevice ) pDevice->Lock ( ) ;  Lock.Request ( ) ;
   long Result = WinDrawText ( hPS, Length, PCH(Text), &Rectangle, TextColor, Background, Flags ) ;
   Lock.Release ( ) ;  if ( pDevice ) pDevice->Unlock ( ) ;
   return ( Result ) ;
}
 
/****************************************************************************
 *                                                                          *
 *  Method: Draw Text (the hard but complete way)                           *
 *                                                                          *
 ****************************************************************************/

long WorkSpace::DrawText ( char *Text, int Length, int Select ) {

   #ifdef DEBUG_DRAWTEXT
   Log ( "WorkSpace(%08X:'%s')::DrawText(\"%0.*s\") CodePage %i.", this, Name, Length, Text, QueryCodepage() ) ;
   #endif

   if ( pDevice ) pDevice->Lock ( ) ;  Lock.Request ( ) ;

   // If length zero, use 'strlen' to determine real length.
   if ( Length == 0 )
      Length = strlen(Text) ;

   // Capitalize if necessary.
   if ( Caps ) {
      COUNTRYCODE Country = { 0, CodePage } ;
      DosMapCase ( Length, &Country, PCHAR(Text) ) ;
   } /* endif */

   // Get the current cursor location.
   POINTL Cursor0 ;
   GpiQueryCurrentPosition ( hPS, &Cursor0 ) ;

   // If hollow characters . . .
   if ( Outline ) {

      // Draw the filling.
      GpiBeginPath   ( hPS, 1 ) ;
      #ifdef DEBUG
      Log ( "WorkSpace(%08X:'%s')::DrawText: Calling GpiCharString ( %08X, %i, (%08X)\"%0.*s\" ) ;", this, Name, hPS, Length, Text, Length, Text ) ;
      #endif
      if ( GpiCharString ( hPS, Length, PCH(Text) ) == GPI_ERROR ) {
         char Message [512] ;
         Log ( "WorkSpace(%08X:'%s')::DrawText: Unable to draw text: %s", this, Name, InterpretWinError ( Anchor, Message ) ) ;
      } /* endif */
      #ifdef DEBUG
      Log ( "WorkSpace(%08X:'%s')::DrawText: GpiCharString done.", this, Name ) ;
      #endif
      GpiEndPath     ( hPS ) ;
      GpiSetPattern  ( hPS, FillType ) ;
      GpiSetColor    ( hPS, FillColor ) ;
      GpiFillPath    ( hPS, 1, FPATH_ALTERNATE ) ;

      // Move back.
      Move ( Cursor0 ) ;

      // Draw the outline.
      GpiBeginPath        ( hPS, 1 ) ;
      #ifdef DEBUG
      Log ( "WorkSpace(%08X:'%s')::DrawText: Calling GpiCharString ( %08X, %i, (%08X)\"%0.*s\" ) ;", this, Name, hPS, Length, Text, Length, Text ) ;
      #endif
      if ( GpiCharString ( hPS, Length, PCH(Text) ) == GPI_ERROR ) {
         char Message [512] ;
         Log ( "WorkSpace(%08X:'%s')::DrawText: Unable to draw text: %s", this, Name, InterpretWinError ( Anchor, Message ) ) ;
      } /* endif */
      #ifdef DEBUG
      Log ( "WorkSpace(%08X:'%s')::DrawText: GpiCharString done.", this, Name ) ;
      #endif
      GpiEndPath          ( hPS ) ;
      GpiSetPattern       ( hPS, PATSYM_SOLID ) ;
      GpiSetLineType      ( hPS, LineType  ) ;
      GpiSetLineWidthGeom ( hPS, LineWidth ) ;
      GpiSetLineJoin      ( hPS, LineJoin  ) ;
      GpiSetLineEnd       ( hPS, LineEnd   ) ;
      GpiSetColor         ( hPS, LineColor ) ;
      GpiStrokePath       ( hPS, 1, 0 ) ;

   // Else if not hollow, let the system draw them solid.
   } else {
      #ifdef DEBUG
      Log ( "WorkSpace(%08X:'%s')::DrawText: Calling GpiCharString ( %08X, %i, (%08X)\"%0.*s\" ) ;", this, Name, hPS, Length, Text, Length, Text ) ;
      #endif
      if ( GpiCharString ( hPS, Length, PCH(Text) ) == GPI_ERROR ) {
         char Message [512] ;
         Log ( "WorkSpace(%08X:'%s')::DrawText: Unable to draw text: %s", this, Name, InterpretWinError ( Anchor, Message ) ) ;
      } /* endif */
      #ifdef DEBUG
      Log ( "WorkSpace(%08X:'%s')::DrawText: GpiCharString done.", this, Name ) ;
      #endif

   } /* endif */

   // If the text was to be selected, invert it now.
   if ( Select ) {
      Move ( Cursor0 ) ;
      InvertText ( Text, Length ) ;
   } /* endif */

   // Determine the displayed size.
   SIZEL Size ;
   MeasureText ( Text, Length, Size ) ;

   // Set the new cursor location.
   POINTL Cursor1 = { Cursor0.x+Size.cx, Cursor0.y } ;
   Move ( Cursor1 ) ;

   Lock.Release ( ) ;  if ( pDevice ) pDevice->Unlock ( ) ;

   #ifdef DEBUG
   Log ( "WorkSpace(%08X:'%s')::DrawText: Done.", this, Name ) ;
   #endif

   // Return the display width.
   return ( Size.cx ) ;
}

/****************************************************************************
 *                                                                          *
 *  Method: Draw Space (Underscored, Struckout, Selected)                   *
 *                                                                          *
 ****************************************************************************/

long WorkSpace::DrawSpace ( SIZEL &Size, int Select ) {

   // Lock the device context.
   if ( pDevice ) pDevice->Lock ( ) ;  Lock.Request ( ) ;

   // Get the current cursor location.
   POINTL Cursor0 ;
   GpiQueryCurrentPosition ( hPS, &Cursor0 ) ;

   // Draw the filling.
   GpiBeginPath   ( hPS, 1 ) ;
   if ( Strikeout ) {
      RECTL Rectangle = { Cursor0.x, Cursor0.y+StrikeoutInfo[0]+1, 
         Cursor0.x+Size.cx, Cursor0.y+StrikeoutInfo[0]+StrikeoutInfo[1] } ;
      DrawBox ( Rectangle ) ;
   } /* endif */
   if ( Underscore ) {
      RECTL Rectangle = { Cursor0.x, Cursor0.y-UnderscoreInfo[0]+1, 
         Cursor0.x+Size.cx, Cursor0.y-UnderscoreInfo[0]+UnderscoreInfo[1] } ;
      DrawBox ( Rectangle ) ;
   } /* endif */
   GpiEndPath     ( hPS ) ;
   GpiSetPattern  ( hPS, Outline ? FillType : PATSYM_SOLID ) ;
   GpiSetColor    ( hPS, Outline ? FillColor : LineColor ) ;
   GpiFillPath    ( hPS, 1, FPATH_ALTERNATE ) ;

   // Move back.
   Move ( Cursor0 ) ;

   // Draw the outline, if in outline mode.
   if ( Outline ) {
      GpiBeginPath        ( hPS, 1 ) ;
      if ( Strikeout ) {
         RECTL Rectangle = { Cursor0.x, Cursor0.y+StrikeoutInfo[0]+1, 
            Cursor0.x+Size.cx, Cursor0.y+StrikeoutInfo[0]+StrikeoutInfo[1] } ;
         DrawBox ( Rectangle ) ;
      } /* endif */
      if ( Underscore ) {
         RECTL Rectangle = { Cursor0.x, Cursor0.y-UnderscoreInfo[0]+1, 
            Cursor0.x+Size.cx, Cursor0.y-UnderscoreInfo[0]+UnderscoreInfo[1] } ;
         DrawBox ( Rectangle ) ;
      } /* endif */
      GpiEndPath          ( hPS ) ;
      GpiSetPattern       ( hPS, PATSYM_SOLID ) ;
      GpiSetLineType      ( hPS, LineType  ) ;
      GpiSetLineWidthGeom ( hPS, LineWidth ) ;
      GpiSetLineJoin      ( hPS, LineJoin  ) ;
      GpiSetLineEnd       ( hPS, LineEnd   ) ;
      GpiSetColor         ( hPS, LineColor ) ;
      GpiStrokePath       ( hPS, 1, 0 ) ;
   } /* endif */

   // If the text was to be selected, invert it now.
   if ( Select ) {
      Move ( Cursor0 ) ;
      InvertCell ( Size ) ;
   } /* endif */

   // Set the new cursor location.
   POINTL Cursor1 = { Cursor0.x+Size.cx, Cursor0.y } ;
   Move ( Cursor1 ) ;

   // Release the device context.
   Lock.Release ( ) ;  if ( pDevice ) pDevice->Unlock ( ) ;

   // Return the width of the updated area.
   return ( Size.cx ) ;
}
 
/****************************************************************************
 *                                                                          *
 *  Method: Draw Pointer                                                    *
 *                                                                          *
 ****************************************************************************/

BOOL WorkSpace::DrawPointer ( HPOINTER Pointer, POINTL &Point, int Flags ) {

   #ifdef DEBUG
      Log ( "WorkSpace(%08X:'%s')::DrawPointer()", this, Name ) ;
   #endif

   if ( pDevice ) pDevice->Lock ( ) ;  Lock.Request ( ) ;

   BOOL Result = WinDrawPointer ( hPS, Point.x, Point.y, Pointer, Flags ) ;

   Lock.Release ( ) ;  if ( pDevice ) pDevice->Unlock ( ) ;

   return ( Result ) ;
}
 
/****************************************************************************
 *                                                                          *
 *  Method: Set Font Size                                                   *
 *                                                                          *
 ****************************************************************************/

void WorkSpace::ScaleFont ( ) {

   #ifdef DEBUG
      Log ( "WorkSpace(%08X:'%s')::ScaleFont()", this, Name ) ;
   #endif

   if ( pDevice ) pDevice->Lock ( ) ;  Lock.Request ( ) ;

   if ( !GpiSetCharMode ( hPS, CM_MODE3 ) ) {
      char Message [512] ;
      Log ( "WorkSpace(%08X:'%s')::ScaleFont: Unable to set character mode 3: %s", this, Name, InterpretWinError ( Anchor, Message ) ) ;
   } /* endif */

   double SizeY = double ( 1000.0 * FontSize ) / double ( Metric ? 283 : 720 ) ;
   double SizeX = ( SizeY * Expand ) / 100.0 ;

   #ifdef DEBUG
      Log ( "WorkSpace(%08X:'%s')::ScaleFont(): Sizes X:%lf, Y:%lf", this, Name, SizeX, SizeY ) ;
   #endif

   SIZEF CellSize ;
   CellSize.cx = MAKEFIXED ( short(SizeX), short((SizeX-int(SizeX))*0x10000) ) ;
   CellSize.cy = MAKEFIXED ( short(SizeY), short((SizeY-int(SizeY))*0x10000) ) ;

   #ifdef DEBUG
      Log ( "WorkSpace(%08X:'%s')::ScaleFont(): Char Box %08lX, %08lX", this, Name, CellSize.cx, CellSize.cy ) ;
   #endif

   if ( !GpiSetCharBox ( hPS, &CellSize ) ) {
      char Message [512] ;
      Log ( "WorkSpace(%08X:'%s')::ScaleFont: Unable to set character box: %s", this, Name, InterpretWinError ( Anchor, Message ) ) ;
   } /* endif */

   Lock.Release ( ) ;  if ( pDevice ) pDevice->Unlock ( ) ;
}
 
/****************************************************************************
 *                                                                          *
 *  Method: Get Font Metrics                                                *
 *                                                                          *
 ****************************************************************************/

void WorkSpace::GetFontMetrics ( ) {

   #ifdef DEBUG
      Log ( "WorkSpace(%08X:'%s')::GetFontMetrics()", this, Name ) ;
   #endif

   if ( pDevice ) pDevice->Lock ( ) ;  Lock.Request ( ) ;

   FONTMETRICS FontMetrics ;
   if ( !GpiQueryFontMetrics ( hPS, sizeof(FontMetrics), &FontMetrics ) ) {
      char Message [512] ;
      Log ( "WorkSpace(%08X:'%s')::GetFontMetrics: Unable to query font metrics: %s", this, Name, InterpretWinError ( Anchor, Message ) ) ;
   } /* endif */

   Ascent = int ( FontMetrics.lMaxAscender ) ;
   Descent = int ( FontMetrics.lMaxDescender ) ;
   AvgWidth = int ( FontMetrics.lAveCharWidth ) ;
   StrikeoutInfo[0] = FontMetrics.lStrikeoutPosition ;
   StrikeoutInfo[1] = FontMetrics.lStrikeoutSize ;
   UnderscoreInfo[0] = FontMetrics.lUnderscorePosition ;
   UnderscoreInfo[1] = FontMetrics.lUnderscoreSize ;

#ifndef GPIMEASURE

   if ( !GpiQueryWidthTable ( hPS, 0, 256, FontWidths ) ) {
      char Message [512] ;
      Log ( "WorkSpace(%08X:'%s')::GetFontMetrics: Unable to query font width table: %s", this, Name, InterpretWinError ( Anchor, Message ) ) ;
      FontWidthsValid = FALSE ;
   } else {
      FontWidthsValid = TRUE ;
   } /* endif */

   if ( KerningPairs && FontMetrics.sKerningPairs ) {
      KerningPairCount = GpiQueryKerningPairs ( hPS, min(FontMetrics.sKerningPairs,MAX_KERNINGPAIRS), KerningPairs ) ;
      if ( KerningPairCount == GPI_ALTERROR ) {
         char Message [512] ;
         Log ( "WorkSpace(%08X:'%s')::GetFontMetrics: Unable to query font kerning pairs list: %s", this, Name, InterpretWinError ( Anchor, Message ) ) ;
         KerningPairCount = 0 ;
      } /* endif */
   } else {
      KerningPairCount = 0 ;
   } /* endif */

#endif // Not GPIMEASURE

   Lock.Release ( ) ;  if ( pDevice ) pDevice->Unlock ( ) ;
}
 
/****************************************************************************
 *                                                                          *
 *  Method: Set Font Name                                                   *
 *                                                                          *
 ****************************************************************************/

void WorkSpace::SetFontName ( char *name ) {

   #ifdef DEBUG
   Log ( "WorkSpace(%08X:'%s')::SetFontName(\"%s\")", this, Name, name ) ;
   #endif

   if ( strcmp ( FontName, name ) ) {        // If name has changed,
      strcpy ( FontName, name ) ;
      SelectFont ( ) ;
      GetFontMetrics ( ) ;

   } /* endif */
}
 
/****************************************************************************
 *                                                                          *
 *  Method: Set Font Size                                                   *
 *                                                                          *
 ****************************************************************************/

void WorkSpace::SetFontSize ( int Size ) {

   #ifdef DEBUG
   Log ( "WorkSpace(%08X:'%s')::SetFontSize(%.1lf)", this, Name, double(Size)/10.0 ) ;
   #endif

   if ( FontSize != Size ) {
      FontSize = Size ;
      ScaleFont ( ) ;
      GetFontMetrics ( ) ;

   } /* endif */
}
 
/****************************************************************************
 *                                                                          *
 *  Method: Set Bold                                                        *
 *                                                                          *
 ****************************************************************************/

void WorkSpace::SetBold ( int Flag ) {

   #ifdef DEBUG
   Log ( "WorkSpace(%08X:'%s')::SetBold(%s)", this, Name, Flag?"TRUE":"FALSE" ) ;
   #endif

   if ( Bold != Flag ) {
      Bold = Flag ;
      SelectFont ( ) ;
      GetFontMetrics ( ) ;

   } /* endif */
}
 
/****************************************************************************
 *                                                                          *
 *  Method: Set Italic                                                      *
 *                                                                          *
 ****************************************************************************/

void WorkSpace::SetItalic ( int Flag ) {

   #ifdef DEBUG
   Log ( "WorkSpace(%08X:'%s')::SetItalic(%s)", this, Name, Flag?"TRUE":"FALSE" ) ;
   #endif

   if ( Italic != Flag ) {
      Italic = Flag ;
      SelectFont ( ) ;
      GetFontMetrics ( ) ;

   } /* endif */
}
 
/****************************************************************************
 *                                                                          *
 *  Method: Set Underscore                                                  *
 *                                                                          *
 ****************************************************************************/

void WorkSpace::SetUnderscore ( int Flag ) {

   #ifdef DEBUG
   Log ( "WorkSpace(%08X:'%s')::SetUnderscore(%s)", this, Name, Flag?"TRUE":"FALSE" ) ;
   #endif

   if ( Underscore != Flag ) {
      Underscore = Flag ;
      SelectFont ( ) ;

   } /* endif */
}
 
/****************************************************************************
 *                                                                          *
 *  Method: Set Strikeout                                                   *
 *                                                                          *
 ****************************************************************************/

void WorkSpace::SetStrikeout ( int Flag ) {

   #ifdef DEBUG
   Log ( "WorkSpace(%08X:'%s')::SetStrikeout(%s)", this, Name, Flag?"TRUE":"FALSE" ) ;
   #endif

   if ( Strikeout != Flag ) {
      Strikeout = Flag ;
      SelectFont ( ) ;

   } /* endif */
}
 
/****************************************************************************
 *                                                                          *
 *  Method: Set Outline                                                     *
 *                                                                          *
 ****************************************************************************/

void WorkSpace::SetOutline ( int Flag ) {

   #ifdef DEBUG
   Log ( "WorkSpace(%08X:'%s')::SetOutline(%s)", this, Name, Flag?"TRUE":"FALSE" ) ;
   #endif

   if ( Outline != Flag ) {
      Outline = Flag ;
      SelectFont ( ) ;

   } /* endif */
}
 
/****************************************************************************
 *                                                                          *
 *  Method: Set Color                                                       *
 *                                                                          *
 ****************************************************************************/

BOOL WorkSpace::SetColor ( COLOR color ) {

   #ifdef DEBUG
   Log ( "WorkSpace(%08X:'%s')::SetColor(%06X)", this, Name, color ) ;
   #endif

   LineColor = color ;

   if ( pDevice ) pDevice->Lock ( ) ;  Lock.Request ( ) ;

   BOOL Result = GpiSetColor ( hPS, LineColor ) ;
   if ( !Result ) {
      char Message [512] ;
      Log ( "WorkSpace(%08X:'%s')::SetColor: Unable to set color %06X: %s", this, Name, LineColor, InterpretWinError ( Anchor, Message ) ) ;
   } /* endif */

   Lock.Release ( ) ;  if ( pDevice ) pDevice->Unlock ( ) ;

   return ( Result ) ;
}
 
/****************************************************************************
 *                                                                          *
 *  Method: Set Line Type                                                   *
 *                                                                          *
 ****************************************************************************/

BOOL WorkSpace::SetLineType ( int Type ) {

   #ifdef DEBUG
   Log ( "WorkSpace(%08X:'%s')::SetLineType(%i)", this, Name, Type ) ;
   #endif

   LineType = Type ;

   if ( pDevice ) pDevice->Lock ( ) ;  Lock.Request ( ) ;

   BOOL Result = GpiSetLineType ( hPS, Type ) ;
   if ( !Result ) {
      char Message [512] ;
      Log ( "WorkSpace(%08X:'%s')::SetLineType: Unable to set line type %i: %s", this, Name, Type, InterpretWinError ( Anchor, Message ) ) ;
   } /* endif */

   Lock.Release ( ) ;  if ( pDevice ) pDevice->Unlock ( ) ;

   return ( Result ) ;
}
 
/****************************************************************************
 *                                                                          *
 *  Method: Set Line Width                                                  *
 *                                                                          *
 ****************************************************************************/

BOOL WorkSpace::SetLineWidth ( int Width ) {

   #ifdef DEBUG
   Log ( "WorkSpace(%08X:'%s')::SetLineWidth(%i)", this, Name, Width ) ;
   #endif

   LineWidth = Width ;

   if ( pDevice ) pDevice->Lock ( ) ;  Lock.Request ( ) ;

   BOOL Result = GpiSetLineWidthGeom ( hPS, LineWidth ) ;
   if ( !Result ) {
      char Message [512] ;
      Log ( "WorkSpace(%08X:'%s')::SetLineWidth: Unable to set line width %i: %s", this, Name, Width, InterpretWinError ( Anchor, Message ) ) ;
   } /* endif */

   Lock.Release ( ) ;  if ( pDevice ) pDevice->Unlock ( ) ;

   return ( Result ) ;
}
 
/****************************************************************************
 *                                                                          *
 *  Method: Set Line Join                                                   *
 *                                                                          *
 ****************************************************************************/

BOOL WorkSpace::SetLineJoin ( int Join ) {

   #ifdef DEBUG
   Log ( "WorkSpace(%08X:'%s')::SetLineJoin(%i)", this, Name, Join ) ;
   #endif

   LineJoin = Join ;

   if ( pDevice ) pDevice->Lock ( ) ;  Lock.Request ( ) ;

   BOOL Result = GpiSetLineJoin ( hPS, Join ) ;
   if ( !Result ) {
      char Message [512] ;
      Log ( "WorkSpace(%08X:'%s')::SetLineJoin: Unable to set line join %i: %s", this, Name, Join, InterpretWinError ( Anchor, Message ) ) ;
   } /* endif */

   Lock.Release ( ) ;  if ( pDevice ) pDevice->Unlock ( ) ;

   return ( Result ) ;
}
 
/****************************************************************************
 *                                                                          *
 *  Method: Set Line End                                                    *
 *                                                                          *
 ****************************************************************************/

BOOL WorkSpace::SetLineEnd ( int End ) {

   #ifdef DEBUG
   Log ( "WorkSpace(%08X:'%s')::SetLineEnd(%i)", this, Name, End ) ;
   #endif

   LineEnd = End ;

   if ( pDevice ) pDevice->Lock ( ) ;  Lock.Request ( ) ;

   BOOL Result = GpiSetLineEnd ( hPS, End ) ;
   if ( !Result ) {
      char Message [512] ;
      Log ( "WorkSpace(%08X:'%s')::SetLineEnd: Unable to set line end %i: %s", this, Name, End, InterpretWinError ( Anchor, Message ) ) ;
   } /* endif */

   Lock.Release ( ) ;  if ( pDevice ) pDevice->Unlock ( ) ;

   return ( Result ) ;
}
 
/****************************************************************************
 *                                                                          *
 *  Method: Set Fill Type                                                   *
 *                                                                          *
 ****************************************************************************/

BOOL WorkSpace::SetFillType ( int Type ) {

   #ifdef DEBUG
   Log ( "WorkSpace(%08X:'%s')::SetFillType(%i)", this, Name, Type ) ;
   #endif

   FillType = Type ;

   if ( pDevice ) pDevice->Lock ( ) ;  Lock.Request ( ) ;

   BOOL Result = GpiSetPattern ( hPS, Type ) ;
   if ( !Result ) {
      char Message [512] ;
      Log ( "WorkSpace(%08X:'%s')::SetFillType: Unable to set fill type %i: %s", this, Name, Type, InterpretWinError ( Anchor, Message ) ) ;
   } /* endif */

   Lock.Release ( ) ;  if ( pDevice ) pDevice->Unlock ( ) ;

   return ( Result ) ;
}
 
/****************************************************************************
 *                                                                          *
 *  Method: Set Mix                                                         *
 *                                                                          *
 ****************************************************************************/

BOOL WorkSpace::SetMix ( int mix ) {

   #ifdef DEBUG
   Log ( "WorkSpace(%08X:'%s')::SetMix(%i)", this, Name, mix ) ;
   #endif

   Mix = mix ;

   if ( pDevice ) pDevice->Lock ( ) ;  Lock.Request ( ) ;

   BOOL Result = GpiSetMix ( hPS, Mix ) ;
   if ( !Result ) {
      char Message [512] ;
      Log ( "WorkSpace(%08X:'%s')::SetMix: Unable to set mix %i: %s", this, Name, Mix, InterpretWinError ( Anchor, Message ) ) ;
   } /* endif */

   Lock.Release ( ) ;  if ( pDevice ) pDevice->Unlock ( ) ;

   return ( Result ) ;
}
 
/****************************************************************************
 *                                                                          *
 *  Method: Set Expansion                                                   *
 *                                                                          *
 ****************************************************************************/

void WorkSpace::SetExpand ( int expand ) {

   #ifdef DEBUG
   Log ( "WorkSpace(%08X:'%s')::SetExpand(%i)", this, Name, expand ) ;
   #endif

   if ( Expand != expand ) {
      Expand = expand ;
      ScaleFont ( ) ;
      GetFontMetrics ( ) ;

   } /* endif */
}
 
/****************************************************************************
 *                                                                          *
 *  Method: Set Transformation Matrix                                       *
 *                                                                          *
 ****************************************************************************/

static LONG SetTranslation ( double &Zoom, int Delta ) {
   return ( LONG ( - Delta * Zoom ) ) ;
}

void WorkSpace::SetTransform ( FIXED fxZoom, int DeltaX, int DeltaY ) {
   #ifdef DEBUG_TRANSFORM
   Log ( "WorkSpace(%08X:'%s')::SetTransform(%i+%i/0x10000,%05i,%05i)",
      this, Name, FIXEDINT(fxZoom), FIXEDFRAC(fxZoom), DeltaX, DeltaY ) ;
   #endif
   double Zoom = (double) FIXEDINT ( fxZoom ) ;
   Zoom += (double) FIXEDFRAC ( fxZoom ) / 0x10000L ;
   SetTransform ( Zoom, DeltaX, DeltaY ) ;
}

void WorkSpace::SetTransform ( double zoom, int deltax, int deltay ) {

   #ifdef DEBUG_TRANSFORM
   Log ( "WorkSpace(%08X:'%s')::SetTransform(%7.3lf,%05i,%05i)",
      this, Name, zoom, deltax, deltay ) ;
   #endif

   USHORT Upper = USHORT ( zoom ) ;
   USHORT Lower = USHORT ( ( zoom - double(Upper) ) * 0x10000L ) ;
   Zoom = MAKEFIXED ( Upper, Lower ) ;
   DeltaX = deltax ;
   DeltaY = deltay ;

   MATRIXLF Matrix ;
   memset ( &Matrix, 0, sizeof(Matrix) ) ;
   Matrix.fxM11 = Zoom ;
   Matrix.fxM22 = Zoom ;
   Matrix.lM31 = SetTranslation ( zoom, DeltaX ) ;
   Matrix.lM32 = SetTranslation ( zoom, DeltaY ) ;
   Matrix.lM33 = 1 ;

   if ( pDevice ) pDevice->Lock ( ) ;  Lock.Request ( ) ;

   if ( !GpiSetDefaultViewMatrix ( hPS, 9, &Matrix, TRANSFORM_REPLACE ) ) {
      char Message [512] ;
      Log ( "WorkSpace(%08X:'%s')::SetTransform: Unable to set default viewing matrix: %s", this, Name, InterpretWinError ( Anchor, Message ) ) ;
   } /* endif */

   Lock.Release ( ) ;  if ( pDevice ) pDevice->Unlock ( ) ;
}
 
/****************************************************************************
 *                                                                          *
 *  Method: Set State                                                       *
 *                                                                          *
 ****************************************************************************/

void WorkSpace::SetState ( _Estado &State ) {

   #ifdef DEBUG
      Log ( "WorkSpace(%08X:'%s')::SetState()", this, Name ) ;
   #endif

   BOOL Select = FALSE ;
   BOOL Scale = FALSE ;
   BOOL GetMetrics = FALSE ;

   if ( strncmp ( FontName, State.Font, min(sizeof(FontName),sizeof(State.Font)) ) ) {
      strncpy ( FontName, State.Font, min(sizeof(FontName),sizeof(State.Font)) ) ;
      Select = TRUE ;
      Scale = TRUE ;
      GetMetrics = TRUE ;
   } /* endif */

   if ( FontSize != State.Size ) {
      FontSize = int ( State.Size ) ;
      Scale = TRUE ;
      GetMetrics = TRUE ;
   } /* endif */

   if ( Bold != State.Bold ) {
      Bold = State.Bold ;
      Select = TRUE ;
      GetMetrics = TRUE ;
   } /* endif */

   if ( Italic != State.Italic ) {
      Italic = State.Italic ;
      Select = TRUE ;
      GetMetrics = TRUE ;
   } /* endif */

   if ( Underscore != State.Underscore ) {
      Underscore = State.Underscore ;
      Select = TRUE ;
   } /* endif */

   if ( Strikeout != State.Strikeout ) {
      Strikeout = State.Strikeout ;
      Select = TRUE ;
   } /* endif */

   if ( Outline != State.Outline ) {
      Outline = State.Outline ;
      Select = TRUE ;
   } /* endif */

   if ( Expand != State.Expand ) {
      Expand = int ( State.Expand ) ;
      Scale = TRUE ;
      GetMetrics = TRUE ;
   } /* endif */

   if ( pDevice ) pDevice->Lock ( ) ;  Lock.Request ( ) ;

   GpiOpenSegment ( hPS, 0 ) ;

   Comment ( "WorkSpace::SetState()." ) ;

   if ( Select )
      SelectFont ( ) ;

   if ( Scale )
      ScaleFont ( ) ;

   if ( GetMetrics )
      GetFontMetrics ( ) ;

   SetCaps       ( State.Caps ) ;
   SetColor      ( State.Color ) ;
   SetFillColor  ( State.FillColor ) ;
   SetLineWidth  ( int ( State.LineWidth ) ) ;
   SetLineJoin   ( State.LineJoin ) ;
   SetLineEnd    ( State.LineEnd ) ;
   SetFillType   ( State.FillType ) ;

   GpiCloseSegment ( hPS ) ;

   Lock.Release ( ) ;  if ( pDevice ) pDevice->Unlock ( ) ;
}
 
/****************************************************************************
 *                                                                          *
 *  Method: Transform Coordinates                                           *
 *                                                                          *
 ****************************************************************************/

BOOL WorkSpace::Transform ( int From, int To, int Count, POINTL Points[] ) {

   #ifdef DEBUG_TRANSFORM
   Log ( "WorkSpace(%08X:'%s')::Transform(%i,%i,%i,Points) Started.", this, Name, From, To, Count ) ;
   for ( int i=0; i<Count; i++ ) {
      Log ( "WorkSpace(%08X:'%s')::Transform  Points[%i]: %i,%i", this, Name, i+1, Points[i].x, Points[i].y ) ;
   } /* endfor */
   #endif

   if ( pDevice ) pDevice->Lock ( ) ;  Lock.Request ( ) ;

   BOOL Result = GpiConvert ( hPS, From, To, Count, Points  ) ;
   if ( !Result ) {
      char Message [512] ;
      Log ( "WorkSpace(%08X:'%s')::Transform: Unable to convert coordinates: %s", this, Name, InterpretWinError ( Anchor, Message ) ) ;
   } /* endif */

   Lock.Release ( ) ;  if ( pDevice ) pDevice->Unlock ( ) ;

   #ifdef DEBUG_TRANSFORM
   Log ( "WorkSpace(%08X:'%s')::Transform(%i,%i,%i,Points) Done.", this, Name, From, To, Count ) ;
   for ( i=0; i<Count; i++ ) {
      Log ( "WorkSpace(%08X:'%s')::Transform  Points[%i]: %i,%i", this, Name, i+1, Points[i].x, Points[i].y ) ;
   } /* endfor */
   #endif

   return ( Result ) ;
}
 
/****************************************************************************
 *                                                                          *
 *  Method: Query Presentation Space's Device Context                       *
 *                                                                          *
 ****************************************************************************/

HDC WorkSpace::QueryDevice ( ) {

   #ifdef DEBUG
   Log ( "WorkSpace(%08X:'%s')::QueryDevice()", this, Name ) ;
   #endif

   if ( pDevice ) pDevice->Lock ( ) ;  Lock.Request ( ) ;

   HDC Device = GpiQueryDevice ( hPS ) ;

   Lock.Release ( ) ;  if ( pDevice ) pDevice->Unlock ( ) ;

   return ( Device ) ;
}
 
/****************************************************************************
 *                                                                          *
 *  Method: Query Font Count                                                *
 *                                                                          *
 ****************************************************************************/

LONG WorkSpace::QueryFontCount ( ) {

   #if defined(DEBUG) || defined(DEBUG_QUERYFONTS)
      Log ( "WorkSpace(%08X:'%s')::QueryFontCount() Started.", this, Name ) ;
   #endif

   if ( pDevice ) pDevice->Lock ( ) ;  Lock.Request ( ) ;

   LONG Count = 0 ;
   Count = GpiQueryFonts ( hPS, QF_PUBLIC | QF_PRIVATE, 0, &Count, 0, 0 ) ;
   if ( Count == GPI_ALTERROR ) {
      char Message [512] ;
      Log ( "WorkSpace(%08X:'%s')::QueryFontCount: Unable to query font count: %s", this, Name, InterpretWinError ( Anchor, Message ) ) ;
   } /* endif */

   Lock.Release ( ) ;  if ( pDevice ) pDevice->Unlock ( ) ;

   #if defined(DEBUG) || defined(DEBUG_QUERYFONTS)
      Log ( "WorkSpace(%08X:'%s')::QueryFontCount() Returning %i fonts.", this, Name, Count ) ;
   #endif
   return ( Count ) ;
}
 
/****************************************************************************
 *                                                                          *
 *  Method: Query Font Information                                          *
 *                                                                          *
 ****************************************************************************/

LONG WorkSpace::QueryFonts ( LONG Count, PFONTMETRICS pfm ) {

   #if defined(DEBUG) || defined(DEBUG_QUERYFONTS)
      Log ( "WorkSpace(%08X:'%s')::QueryFonts() Started.", this, Name ) ;
   #endif

   if ( pDevice ) pDevice->Lock ( ) ;  Lock.Request ( ) ;

   LONG Result = GpiQueryFonts ( hPS, QF_PUBLIC | QF_PRIVATE, 0, &Count, sizeof(FONTMETRICS), pfm ) ;
   if ( Result == GPI_ALTERROR ) {
      char Message [512] ;
      Log ( "WorkSpace(%08X:'%s')::QueryFonts: Unable to query font information: %s", this, Name, InterpretWinError ( Anchor, Message ) ) ;
   } /* endif */

   Lock.Release ( ) ;  if ( pDevice ) pDevice->Unlock ( ) ;

   #if defined(DEBUG) || defined(DEBUG_QUERYFONTS)
      Log ( "WorkSpace(%08X:'%s')::QueryFonts() Returning %i fonts.", this, Name, Count ) ;
   #endif
   return ( Result ) ;
}
 
/****************************************************************************
 *                                                                          *
 *  Method: Determine Native Bitmap Format                                  *
 *                                                                          *
 ****************************************************************************/

void WorkSpace::QueryBitmapFormat ( int &Planes, int &BitCount ) {

   #ifdef DEBUG_BITMAP
   Log ( "WorkSpace(%08X:'%s')::QueryBitmapFormat() Started.", this, Name ) ;
   #endif

   if ( pDevice ) pDevice->Lock ( ) ;  Lock.Request ( ) ;

   LONG alData [2] ;
   if ( !GpiQueryDeviceBitmapFormats ( hPS, 2, alData ) ) {
      char Message [512] ;
      Log ( "WorkSpace(%08X:'%s')::QueryBitmapFormat: Unable to query device bitmap formats: %s", this, Name, InterpretWinError ( Anchor, Message ) ) ;
   } /* endif */

   Lock.Release ( ) ;  if ( pDevice ) pDevice->Unlock ( ) ;

   Planes = int ( alData[0] ) ;
   BitCount = int ( alData [1] ) ;

   #ifdef DEBUG_BITMAP
   Log ( "WorkSpace(%08X:'%s')::QueryBitmapFormat() Done.  Planes %i, Bits %i.", this, Name, Planes, BitCount ) ;
   #endif
}
 
/****************************************************************************
 *                                                                          *
 *  Method: Create Bitmap                                                   *
 *                                                                          *
 ****************************************************************************/

HBITMAP WorkSpace::CreateBitmap ( RECTL &Rectangle ) {

   #ifdef DEBUG_BITMAP
   Log ( "WorkSpace(%08X:'%s')::CreateBitmap(Rectangle) Started.",
      this, Name ) ;
   #endif

   int Planes, BitCount ;
   QueryBitmapFormat ( Planes, BitCount ) ;

   BITMAPINFOHEADER2 Header ;
   memset ( &Header, 0, sizeof(Header) ) ;
   Header.cbFix = 16 ;
   Header.cPlanes = USHORT ( Planes ) ;
   Header.cBitCount = USHORT ( BitCount ) ;
   Header.cx = Rectangle.xRight - Rectangle.xLeft ;
   Header.cy = Rectangle.yTop - Rectangle.yBottom ;
   HBITMAP Handle = CreateBitmap ( Header ) ;

   #ifdef DEBUG_BITMAP
   Log ( "WorkSpace(%08X:'%s')::CreateBitmap(Rectangle) Done.  Handle %08X.",
      this, Name, Handle ) ;
   #endif

   return ( Handle ) ;
}
 
/****************************************************************************
 *                                                                          *
 *  Method: Create & Load Bitmap from another PS.                           *
 *                                                                          *
 ****************************************************************************/

HBITMAP WorkSpace::CreateBitmap ( RECTL &Rectangle, WorkSpace &SourcePS ) {

   #ifdef DEBUG_BITMAP
   Log ( "WorkSpace(%08X:'%s')::CreateBitmap(Rectangle,SourcePS=%08X) Started.",
      this, Name, SourcePS ) ;
   #endif

   HBITMAP Handle = CreateBitmap ( Rectangle ) ;

   if ( Handle ) {

      POINTL Points [4] ;
      Points[0].x = Points[0].y = Points[2].x = Points[2].y = 0 ;
      Points[1].x = Points[3].x = Rectangle.xRight - Rectangle.xLeft ;
      Points[1].y = Points[3].y = Rectangle.yTop - Rectangle.yBottom ;

      SetBitmap ( Handle ) ;
      Blit ( SourcePS, 4, Points, ROP_SRCCOPY, BBO_IGNORE ) ;
      SetBitmap ( 0 ) ;

   } /* endif */

   #ifdef DEBUG_BITMAP
   Log ( "WorkSpace(%08X:'%s')::CreateBitmap(Rectangle,SourcePS=%08X) Done.  Handle %08X.",
      this, Name, SourcePS, Handle ) ;
   #endif

   return ( Handle ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Create Bitmap from another                                          *
 *                                                                          *
 ****************************************************************************/

HBITMAP WorkSpace::CreateBitmap ( HBITMAP Original ) {

   #ifdef DEBUG_BITMAP
   Log ( "WorkSpace(%08X:'%s')::CreateBitmap(Original=%08X) Started.",
      this, Name, Original ) ;
   #endif

   if ( pDevice ) pDevice->Lock ( ) ;  Lock.Request ( ) ;

   DeviceContext Memory = DeviceContext ( "WorkSpace::CreateBitmap", long(Anchor) ) ;
   WorkSpace MemoryPS = WorkSpace ( "WorkSpace::CreateBitmap", 0, &Memory, Anchor, TRUE ) ;
   MemoryPS.SetBitmap ( Original ) ;

   BITMAPINFOHEADER2 Header ;
   Header.cbFix = sizeof(Header) ;
   GpiQueryBitmapInfoHeader ( Original, &Header ) ;
   RECTL Rectangle = { 0, 0, Header.cx, Header.cy } ;
   HBITMAP Copy = CreateBitmap ( Rectangle, MemoryPS ) ;

   MemoryPS.SetBitmap ( 0 ) ;

   Lock.Release ( ) ;  if ( pDevice ) pDevice->Unlock ( ) ;

   #ifdef DEBUG_BITMAP
   Log ( "WorkSpace(%08X:'%s')::CreateBitmap(Original=%08X) Done.  Copy %08X.",
      this, Name, Original, Copy ) ;
   #endif

   return ( Copy ) ;
}
 
/****************************************************************************
 *                                                                          *
 *  Method: Create Bitmap                                                   *
 *                                                                          *
 ****************************************************************************/

HBITMAP WorkSpace::CreateBitmap ( BITMAPINFOHEADER2 &BitmapHeader ) {

   #ifdef DEBUG_BITMAP
   Log ( "WorkSpace(%08X:'%s')::CreateBitmap(Header) Started.", this, Name ) ;
   #endif

   if ( pDevice ) pDevice->Lock ( ) ;  Lock.Request ( ) ;

   HBITMAP Handle = GpiCreateBitmap ( hPS, &BitmapHeader, 0, 0, 0 ) ;
   if ( Handle == 0 ) {
      char Message [512] ;
      Log ( "WorkSpace(%08X:'%s')::CreateBitmap: Unable to create bitmap: %s", this, Name, InterpretWinError ( Anchor, Message ) ) ;
   } /* endif */

   Lock.Release ( ) ;  if ( pDevice ) pDevice->Unlock ( ) ;

   #ifdef DEBUG_BITMAP
   Log ( "WorkSpace(%08X:'%s')::CreateBitmap(Header) Done.  Handle %08X.",
      this, Name, Handle ) ;
   #endif

   return ( Handle ) ;
}
 
/****************************************************************************
 *                                                                          *
 *  Method: Create and Load Bitmap                                          *
 *                                                                          *
 ****************************************************************************/

HBITMAP WorkSpace::CreateBitmap ( BITMAPINFOHEADER2 &BitmapHeader, PVOID pBuffer, PBITMAPINFO2 pbmi ) {

   #ifdef DEBUG_BITMAP
   Log ( "WorkSpace(%08X:'%s')::CreateBitmap(Header,Data) Started.", this, Name ) ;
   #endif

   if ( pDevice ) pDevice->Lock ( ) ;  Lock.Request ( ) ;

   HBITMAP Handle = GpiCreateBitmap ( hPS, &BitmapHeader, CBM_INIT, PBYTE(pBuffer), pbmi ) ;
   if ( Handle == 0 ) {
      char Message [512] ;
      Log ( "WorkSpace(%08X:'%s')::CreateBitmap: Unable to create and load bitmap: %s", this, Name, InterpretWinError ( Anchor, Message ) ) ;
   } /* endif */

   Lock.Release ( ) ;  if ( pDevice ) pDevice->Unlock ( ) ;

   #ifdef DEBUG_BITMAP
   Log ( "WorkSpace(%08X:'%s')::CreateBitmap(Header,Data) Done.  Handle %08X.",
      this, Name, Handle ) ;
   #endif

   return ( Handle ) ;
}
 
/****************************************************************************
 *                                                                          *
 *  Method: Load a bitmap from resource library.                            *
 *                                                                          *
 ****************************************************************************/

HBITMAP WorkSpace::CreateBitmap ( HMODULE Library, int Id ) {
   return ( GpiLoadBitmap ( hPS, Library, Id, 0, 0 ) ) ;
}

/****************************************************************************
 *                                                                          *
 *  Method: Unload Bitmap                                                   *
 *                                                                          *
 ****************************************************************************/

LONG WorkSpace::QueryBitmapBits ( int FirstLine, int LineCount, PVOID pBuffer, PBITMAPINFO2 pbmi ) {

   #ifdef DEBUG_BITMAP
   Log ( "WorkSpace(%08X:'%s')::QueryBitmapBits() Started.", this, Name ) ;
   #endif

   if ( pDevice ) pDevice->Lock ( ) ;  Lock.Request ( ) ;

   LONG Lines = GpiQueryBitmapBits ( hPS, FirstLine, LineCount, PBYTE(pBuffer), pbmi ) ;
   if ( Lines == GPI_ALTERROR ) {
      char Message [512] ;
      Log ( "WorkSpace(%08X:'%s')::QueryBitmapBits: Unable to unload bitmap: %s", this, Name, InterpretWinError ( Anchor, Message ) ) ;
   } /* endif */

   Lock.Release ( ) ;  if ( pDevice ) pDevice->Unlock ( ) ;

   #ifdef DEBUG_BITMAP
   Log ( "WorkSpace(%08X:'%s')::QueryBitmapBits() Done.", this, Name ) ;
   #endif

   return ( Lines ) ;
}
 
/****************************************************************************
 *                                                                          *
 *  Method: Set Bitmap to Presentation Space                                *
 *                                                                          *
 ****************************************************************************/

HBITMAP WorkSpace::SetBitmap ( HBITMAP Bitmap ) {

   #ifdef DEBUG_BITMAP
   Log ( "WorkSpace(%08X:'%s')::SetBitmap(%08X) Started.", this, Name, Bitmap ) ;
   #endif

   if ( pDevice ) pDevice->Lock ( ) ;  Lock.Request ( ) ;

   HBITMAP OldBitmap = GpiSetBitmap ( hPS, Bitmap ) ;
   if ( OldBitmap == HBM_ERROR ) {
      char Message [512] ;
      Log ( "WorkSpace(%08X:'%s')::SetBitmap: Unable to set bitmap: %s", this, Name, InterpretWinError ( Anchor, Message ) ) ;
   } /* endif */

   Lock.Release ( ) ;  if ( pDevice ) pDevice->Unlock ( ) ;

   #ifdef DEBUG_BITMAP
   Log ( "WorkSpace(%08X:'%s')::SetBitmap(%08X) Done.", this, Name, Bitmap ) ;
   #endif

   return ( OldBitmap ) ;
}
 
/****************************************************************************
 *                                                                          *
 *  Method: Copy bitmap from another PS into this one.                      *
 *                                                                          *
 ****************************************************************************/

LONG WorkSpace::Blit ( WorkSpace &Source, LONG lCount, PPOINTL aptlPoints, LONG lRop, ULONG flOptions ) {

   #ifdef DEBUG_BITMAP
   Log ( "WorkSpace(%08X:'%s')::Blit(SourcePS=%08X) Started.", this, Name, Source ) ;
   #endif

   if ( pDevice ) pDevice->Lock ( ) ;  Lock.Request ( ) ;
   if ( Source.pDevice ) Source.pDevice->Lock ( ) ;  Source.Lock.Request ( ) ;

   LONG Result = GpiBitBlt ( hPS, Source.hPS, lCount, aptlPoints, lRop, flOptions ) ;

   if ( Result != GPI_OK ) {
      char Message [512] ;
      Log ( "BitBlit: Unable to blit between workspaces '%s' & '%s': %s", Source.Name, Name, InterpretWinError ( Anchor, Message ) ) ;
   } /* endif */

   Source.Lock.Release ( ) ;  if ( Source.pDevice ) Source.pDevice->Unlock ( ) ;
   Lock.Release ( ) ;  if ( pDevice ) pDevice->Unlock ( ) ;

   #ifdef DEBUG_BITMAP
   Log ( "WorkSpace(%08X:'%s')::Blit(SourcePS=%08X) Done.", this, Name, Source ) ;
   #endif

   return ( Result ) ;
}
 
/****************************************************************************
 *                                                                          *
 *  Method: Set Clipping                                                    *
 *                                                                          *
 ****************************************************************************/

void WorkSpace::SetClipping ( RECTL &Clip ) {

   #ifdef DEBUG
   Log ( "WorkSpace(%08X:'%s')::SetClipping(%i,%i-%i,%i)", this, Name, Clip.xLeft, Clip.yBottom, Clip.xRight, Clip.yTop ) ;
   #endif

   if ( pDevice ) pDevice->Lock ( ) ;  Lock.Request ( ) ;

   ClippingRectangle = Clip ;

   ClipRegion = GpiCreateRegion ( hPS, 1, &Clip ) ;
   if ( ClipRegion == RGN_ERROR ) {
      char Message [512] ;
      Log ( "WorkSpace(%08X:'%s')::SetClipping: Unable to create clipping region: %s", this, Name, InterpretWinError ( Anchor, Message ) ) ;
      ClipRegion = 0 ;

   } else if ( GpiSetClipRegion ( hPS, ClipRegion, 0 ) == RGN_ERROR ) {
      char Message [512] ;
      Log ( "WorkSpace(%08X:'%s')::SetClipping: Unable to set clipping region: %s", this, Name, InterpretWinError ( Anchor, Message ) ) ;
      GpiDestroyRegion ( hPS, ClipRegion ) ;
      ClipRegion = 0 ;

   } /* endif */

   Lock.Release ( ) ;  if ( pDevice ) pDevice->Unlock ( ) ;
}
 
/****************************************************************************
 *                                                                          *
 *  Method: Reset Clipping                                                  *
 *                                                                          *
 ****************************************************************************/

void WorkSpace::ResetClipping ( ) {

   #ifdef DEBUG
   Log ( "WorkSpace(%08X:'%s')::ResetClipping()", this, Name ) ;
   #endif

   if ( ClipRegion == 0 ) 
      return ;

   if ( pDevice ) pDevice->Lock ( ) ;  Lock.Request ( ) ;

   if ( GpiSetClipRegion ( hPS, 0, 0 ) == RGN_ERROR ) {
      char Message [512] ;
      Log ( "WorkSpace(%08X:'%s')::ResetClipping: Unable to reset clipping region: %s", this, Name, InterpretWinError ( Anchor, Message ) ) ;
   } /* endif */

   if ( !GpiDestroyRegion ( hPS, ClipRegion ) ) {
      char Message [512] ;
      Log ( "WorkSpace(%08X:'%s')::ResetClipping: Unable to destroy clipping region: %s", this, Name, InterpretWinError ( Anchor, Message ) ) ;
   } /* endif */

   ClipRegion = 0 ;

   Lock.Release ( ) ;  if ( pDevice ) pDevice->Unlock ( ) ;
}
 
/****************************************************************************
 *                                                                          *
 *  Method: Draw Horizontal Separator                                       *
 *                                                                          *
 ****************************************************************************/

int WorkSpace::DrawSeparator ( POINTL &Position, SIZEL &Size ) {

   if ( pDevice ) pDevice->Lock ( ) ;  Lock.Request ( ) ;

   if ( !GpiBeginPath ( hPS, 1 ) ) {
      char Message [512] ;
      Log ( "WorkSpace(%08X:'%s')::DrawSeparator: Unable to begin path: %s", this, Name, InterpretWinError ( Anchor, Message ) ) ;
   } /* endif */

   POINTL Point = Position ;
   if ( LineEnd ) 
      Point.x += LineWidth ;
   Point.y -= ( Descent - 1 ) ;
   Point.y += Size.cy / 2 ;
   if ( !GpiMove ( hPS, &Point ) ) {
      char Message [512] ;
      Log ( "WorkSpace(%08X:'%s')::DrawSeparator: Unable to move to first point: %s", this, Name, InterpretWinError ( Anchor, Message ) ) ;
   } /* endif */

   Point.x = Position.x + Size.cx ; 
   if ( LineEnd ) 
      Point.x -= LineWidth ;
   if ( !GpiLine ( hPS, &Point ) ) {
      char Message [512] ;
      Log ( "WorkSpace(%08X:'%s')::DrawSeparator: Unable to move to first point: %s", this, Name, InterpretWinError ( Anchor, Message ) ) ;
   } /* endif */

   if ( !GpiEndPath ( hPS ) ) {
      char Message [512] ;
      Log ( "WorkSpace(%08X:'%s')::DrawSeparator: Unable to end path: %s", this, Name, InterpretWinError ( Anchor, Message ) ) ;
   } /* endif */

   if ( !GpiSetPattern ( hPS, PATSYM_SOLID ) ) {
      char Message [512] ;
      Log ( "WorkSpace(%08X:'%s')::DrawSeparator: Unable to set fill pattern: %s", this, Name, InterpretWinError ( Anchor, Message ) ) ;
   } /* endif */

   if ( !GpiSetLineType ( hPS, LineType ) ) {
      char Message [512] ;
      Log ( "WorkSpace(%08X:'%s')::DrawSeparator: Unable to set line type: %s", this, Name, InterpretWinError ( Anchor, Message ) ) ;
   } /* endif */

   if ( !GpiSetLineWidthGeom ( hPS, LineWidth ) ) {
      char Message [512] ;
      Log ( "WorkSpace(%08X:'%s')::DrawSeparator: Unable to set geometric line width: %s", this, Name, InterpretWinError ( Anchor, Message ) ) ;
   } /* endif */

   if ( !GpiSetLineJoin ( hPS, LineJoin ) ) {
      char Message [512] ;
      Log ( "WorkSpace(%08X:'%s')::DrawSeparator: Unable to set line join type: %s", this, Name, InterpretWinError ( Anchor, Message ) ) ;
   } /* endif */

   if ( !GpiSetLineEnd ( hPS, LineEnd ) ) {
      char Message [512] ;
      Log ( "WorkSpace(%08X:'%s')::DrawSeparator: Unable to set line end type: %s", this, Name, InterpretWinError ( Anchor, Message ) ) ;
   } /* endif */

   if ( !GpiSetColor ( hPS, LineColor ) ) {
      char Message [512] ;
      Log ( "WorkSpace(%08X:'%s')::DrawSeparator: Unable to set line color: %s", this, Name, InterpretWinError ( Anchor, Message ) ) ;
   } /* endif */

   if ( GpiStrokePath ( hPS, 1, 0 ) == GPI_ERROR ) {
      char Message [512] ;
      Log ( "WorkSpace(%08X:'%s')::DrawSeparator: Unable to stroke path: %s", this, Name, InterpretWinError ( Anchor, Message ) ) ;
   } /* endif */

   Lock.Release ( ) ;  if ( pDevice ) pDevice->Unlock ( ) ;

   return ( 0 ) ;

} /* endmethod */

/****************************************************************************
 *                                                                          *
 *  Method: Measure Bitmap                                                  *
 *                                                                          *
 ****************************************************************************/

int WorkSpace::MeasureBitmap ( HBITMAP Bitmap, SIZEL &Size ) {

   if ( pDevice ) pDevice->Lock ( ) ;  Lock.Request ( ) ;

   BITMAPINFOHEADER2 BitmapInfoHeader ;
   BitmapInfoHeader.cbFix = sizeof(BitmapInfoHeader) ;
   if ( GpiQueryBitmapInfoHeader ( Bitmap, &BitmapInfoHeader ) != GPI_OK ) {
      char Message [512] ;
      Log ( "WorkSpace(%08X:'%s')::MeasureBitmap: Unable to query bitmap information header: %s", this, Name, InterpretWinError ( Anchor, Message ) ) ;
      Lock.Release ( ) ;  if ( pDevice ) pDevice->Unlock ( ) ;
      return ( 1 ) ;
   } /* endif */

   Size.cx = BitmapInfoHeader.cx ;
   Size.cy = BitmapInfoHeader.cy ;

   Lock.Release ( ) ;  if ( pDevice ) pDevice->Unlock ( ) ;

   return ( 0 ) ;

} /* endmethod */
 
/****************************************************************************
 *                                                                          *
 *  Method: Draw Bitmap                                                     *
 *                                                                          *
 ****************************************************************************/

int WorkSpace::DrawBitmap ( HBITMAP Bitmap, POINTL &Position, SIZEL &Size, BOOL Halftone, BOOL Reverse ) {

   #ifdef DEBUG_BITMAP
      Log ( "WorkSpace(%08X:'%s')::DrawBitmap(%08X) Started.", this, Name, Bitmap ) ;
   #endif

   if ( pDevice ) pDevice->Lock ( ) ;  Lock.Request ( ) ;

   // Get the bitmap's original size information.
   BITMAPINFOHEADER2 BitmapInfoHeader ;
   BitmapInfoHeader.cbFix = sizeof(BitmapInfoHeader) ;
   if ( GpiQueryBitmapInfoHeader ( Bitmap, &BitmapInfoHeader ) != GPI_OK ) {
      char Message [512] ;
      Log ( "WorkSpace(%08X:'%s')::DrawBitmap Unable to query bitmap information header: %s", this, Name, InterpretWinError ( Anchor, Message ) ) ;
      Lock.Release ( ) ;  if ( pDevice ) pDevice->Unlock ( ) ;
      return ( 1 ) ;
   } /* endif */
   SIZEL BitmapSize ;
   BitmapSize.cx = BitmapInfoHeader.cx ;
   BitmapSize.cy = BitmapInfoHeader.cy ;

   // Create a memory device context.
   HDC hdcMemory = DevOpenDC ( Anchor, OD_MEMORY, PSZ("*"), 0, 0, GpiQueryDevice(hPS) ) ;
   if ( hdcMemory == 0 ) {
      char Message [512] ;
      Log ( "WorkSpace(%08X:'%s')::DrawBitmap: Unable to open memory device context: %s", this, Name, InterpretWinError ( Anchor, Message ) ) ;
      Lock.Release ( ) ;  if ( pDevice ) pDevice->Unlock ( ) ;
      return ( 1 ) ;
   } /* endif */

   // Create a memory presentation space for the bitmap.
   SIZEL PageSize = { 0, 0 } ;
   HPS MemoryPS = GpiCreatePS ( Anchor, hdcMemory, &PageSize, PU_PELS | GPIA_ASSOC | GPIT_MICRO ) ;
   if ( MemoryPS == 0 ) {
      char Message [512] ;
      Log ( "WorkSpace(%08X:'%s')::DrawBitmap: Unable to create memory PS: %s", this, Name, InterpretWinError ( Anchor, Message ) ) ;
      DevCloseDC ( hdcMemory ) ;
      Lock.Release ( ) ;  if ( pDevice ) pDevice->Unlock ( ) ;
      return ( 1 ) ;
   } /* endif */

   // Set the bitmap to the display presentation space.
   if ( GpiSetBitmap ( MemoryPS, Bitmap ) == HBM_ERROR ) {
      char Message [512] ;
      Log ( "WorkSpace(%08X:'%s')::DrawBitmap: Unable to set bitmap %08X to PS: %s", this, Name, Bitmap, InterpretWinError ( Anchor, Message ) ) ;
      GpiDestroyPS ( MemoryPS ) ;
      DevCloseDC ( hdcMemory ) ;
      Lock.Release ( ) ;  if ( pDevice ) pDevice->Unlock ( ) ;
      return ( 1 ) ;
   } /* endif */

#if 0
   // Get the bitmap's palette information.
   ULONG PaletteSize = 1 << BitmapInfoHeader.cBitCount ;
   ULONG cbBitmapInfo = sizeof(BITMAPINFO2) + ( sizeof(RGB2) * PaletteSize ) ;
   PBITMAPINFO2 pBitmapInfo ;
   DosAllocMem ( (PPVOID)&pBitmapInfo, cbBitmapInfo, PAG_COMMIT | PAG_READ | PAG_WRITE ) ;
   memcpy ( pBitmapInfo, &BitmapInfoHeader, sizeof(BitmapInfoHeader) ) ;
   if ( GpiQueryBitmapBits ( MemoryPS, 0, 0, 0, pBitmapInfo ) < 0 ) {
      char Message [512] ;
      Log ( "WorkSpace(%08X:'%s')::DrawBitmap: Unable to query bitmap palette: %s", this, Name, InterpretWinError ( Anchor, Message ) ) ;
      GpiSetBitmap ( MemoryPS, 0 ) ;
      GpiDestroyPS ( MemoryPS ) ;
      DevCloseDC ( hdcMemory ) ;
      Lock.Release ( ) ;  if ( pDevice ) pDevice->Unlock ( ) ;
      return ( 1 ) ;
   } /* endif */

   // Create a palette to use with the memory presentation space.
   for ( int i=0; i<PaletteSize; i++ ) 
      Log ( "Bitmap palette entry %03i: %02X %02X %02X", i, pBitmapInfo->argbColor[i].bRed, pBitmapInfo->argbColor[i].bGreen, pBitmapInfo->argbColor[i].bBlue ) ;
   HPAL Palette = GpiCreatePalette ( Anchor, 0, LCOLF_CONSECRGB, PaletteSize, 
      PULONG(((char*)pBitmapInfo)+pBitmapInfo->cbFix) ) ;
   DosFreeMem ( pBitmapInfo ) ;
   if ( Palette == GPI_ERROR ) {
      char Message [512] ;
      Log ( "WorkSpace(%08X:'%s')::DrawBitmap: Unable to create bitmap palette: %s", this, Name, InterpretWinError ( Anchor, Message ) ) ;
      GpiSetBitmap ( MemoryPS, 0 ) ;
      GpiDestroyPS ( MemoryPS ) ;
      DevCloseDC ( hdcMemory ) ;
      Lock.Release ( ) ;  if ( pDevice ) pDevice->Unlock ( ) ;
      return ( 1 ) ;
   } /* endif */

   // Set the bitmap's palette in the memory presentation space.
   if ( GpiSelectPalette ( MemoryPS, Palette ) == PAL_ERROR ) {
      char Message [512] ;
      Log ( "WorkSpace(%08X:'%s')::DrawBitmap: Unable to select bitmap palette: %s", this, Name, InterpretWinError ( Anchor, Message ) ) ;
      GpiDeletePalette ( Palette ) ;
      GpiSetBitmap ( MemoryPS, 0 ) ;
      GpiDestroyPS ( MemoryPS ) ;
      DevCloseDC ( hdcMemory ) ;
      Lock.Release ( ) ;  if ( pDevice ) pDevice->Unlock ( ) ;
      return ( 1 ) ;
   } /* endif */
#endif

   // Transform the bitmap positions to device coordinates.
   POINTL Points [4] ;
   Points[0] = Position ;
   Points[1].x = Position.x + Size.cx ;
   Points[1].y = Position.y + Size.cy ;
   Points[2].x = 0 ;
   Points[2].y = 0 ;
   Points[3].x = BitmapSize.cx ;
   Points[3].y = BitmapSize.cy ;
   Transform ( CVTC_DEFAULTPAGE, CVTC_DEVICE, 2, PPOINTL(Points) ) ;

   // If monochrome bitmap . . .
// if ( PaletteSize == 2 ) {

      // Set the background color for monochrome bitmaps.
      if ( !GpiSetBackColor ( hPS, RGB_WHITE ) ) {
         char Message [512] ;
         Log ( "WorkSpace(%08X:'%s')::DrawBitmap: Unable to set background color: %s", this, Name, InterpretWinError ( Anchor, Message ) ) ;
      } /* endif */

      // Set the foreground color for monochrome bitmaps.
      if ( !GpiSetColor ( hPS, LineColor ) ) {
         char Message [512] ;
         Log ( "WorkSpace(%08X:'%s')::DrawBitmap: Unable to set line color: %s", this, Name, InterpretWinError ( Anchor, Message ) ) ;
      } /* endif */

// } /* endif */

   // Set the halftone if requested.
   if ( !GpiSetPattern ( hPS, Halftone ? PATSYM_HALFTONE : PATSYM_SOLID ) ) {
      char Message [512] ;
      Log ( "WorkSpace(%08X:'%s')::DrawBitmap: Unable to set pattern: %s", this, Name, InterpretWinError ( Anchor, Message ) ) ;
   } /* endif */

   // Copy the bitmap to the device space.
   //    0xCC = Copy the bitmap verbatim in blitting it to the presentation space.
   //    0x33 = Invert the bitmap in blitting it to the presentation space.
   //    0xCF = Copy only those bitmap bits that match the pattern.  Set all other bits to white.
   //    0x3F = Copy only those bitmap bits that match the pattern.  Invert them.  Set all other bits to white.
   //
   if ( GpiBitBlt ( hPS, MemoryPS, 4, Points, Reverse ? ( Halftone ? 0x3F : 0x33 ) : ( Halftone ? 0xCF : 0xCC ), BBO_IGNORE ) != GPI_OK ) {
      char Message [512] ;
      Log ( "WorkSpace(%08X:'%s')::DrawBitmap: Unable to blit bitmap %08X: %s", this, Name, Bitmap, InterpretWinError ( Anchor, Message ) ) ;
      GpiSetPattern ( hPS, FillType ) ;
//    GpiSelectPalette ( MemoryPS, 0 ) ;
//    GpiDeletePalette ( Palette ) ;
      GpiSetBitmap ( MemoryPS, 0 ) ;
      GpiDestroyPS ( MemoryPS ) ;
      DevCloseDC ( hdcMemory ) ;
      Lock.Release ( ) ;  if ( pDevice ) pDevice->Unlock ( ) ;
      return ( 1 ) ;
   } /* endif */

   // Clean up.
   GpiSetPattern ( hPS, FillType ) ;
// GpiSelectPalette ( MemoryPS, 0 ) ;
// GpiDeletePalette ( Palette ) ;
   GpiSetBitmap ( MemoryPS, 0 ) ;
   GpiDestroyPS ( MemoryPS ) ;
   DevCloseDC ( hdcMemory ) ;
   Lock.Release ( ) ;  if ( pDevice ) pDevice->Unlock ( ) ;

   #ifdef DEBUG_BITMAP
      Log ( "WorkSpace(%08X:'%s')::DrawBitmap(%08X) Done.", this, Name, Bitmap ) ;
   #endif

   return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *  Method: Measure Metafile                                                *
 *                                                                          *
 ****************************************************************************/

int WorkSpace::MeasureMetafile ( HMF Metafile, RECTL &Rectangle ) {

   // Lock the device context and the workspace.
   #ifdef DEBUG_METAFILE
      Log ( "WorkSpace(%08X:'%s')::MeasureMetafile(%08X) Locking device context & workspace.", this, Name, Metafile ) ;
   #endif
   if ( pDevice ) pDevice->Lock ( ) ;  Lock.Request ( ) ;

   // Save the viewport and the default view transform.
   MATRIXLF OriginalMatrix ;
   GpiQueryDefaultViewMatrix ( hPS, 9, &OriginalMatrix ) ;
   RECTL OriginalViewport ;
   GpiQueryPageViewport ( hPS, &OriginalViewport ) ;

   // Measure the graphic.
   #ifdef DEBUG_METAFILE
      Log ( "WorkSpace(%08X:'%s')::MeasureMetafile(%08X) Playing metafile to bit-bucket to get its size.", this, Name, Metafile ) ;
   #endif
   GpiResetBoundaryData ( hPS ) ;
   GpiSetDrawControl ( hPS, DCTL_DISPLAY, DCTL_OFF ) ;
   GpiSetDrawControl ( hPS, DCTL_BOUNDARY, DCTL_ON ) ;
   LONG Options [] = { 0, LT_NOMODIFY, 0, LC_LOADDISC, RES_NORESET,
      SUP_NOSUPPRESS, CTAB_REPLACE, CREA_NOREALIZE, DDEF_LOADDISC } ;
   if ( GpiPlayMetaFile ( hPS, Metafile, sizeof(Options)/sizeof(Options[0]), Options, 0, 0, 0 ) == GPI_ERROR ) {
      char Message [512] ;
      Log ( "WorkSpace(%08X:'%s')::MeasureMetafile: Unable to play metafile %08X: %s", this, Name, Metafile, InterpretWinError ( Anchor, Message ) ) ;
      Reset ( TRUE ) ;
      if ( !GpiSetPageViewport ( hPS, &OriginalViewport ) ) {
         char Message [512] ;
         Log ( "WorkSpace(%08X:'%s')::MeasureMetafile: Unable to restore original page viewport: %s", this, Name, InterpretWinError ( Anchor, Message ) ) ;
      } /* endif */
      if ( !GpiSetDefaultViewMatrix ( hPS, 9, &OriginalMatrix, TRANSFORM_REPLACE ) ) {
         char Message [512] ;
         Log ( "WorkSpace(%08X:'%s')::MeasureMetafile: Unable to restore original default viewing matrix: %s", this, Name, InterpretWinError ( Anchor, Message ) ) ;
      } /* endif */
      Lock.Release ( ) ;  if ( pDevice ) pDevice->Unlock ( ) ;
      return ( 1 ) ;
   } /* endif */
   GpiQueryBoundaryData ( hPS, &Rectangle ) ;
   GpiSetDrawControl ( hPS, DCTL_BOUNDARY, DCTL_OFF ) ;
   GpiSetDrawControl ( hPS, DCTL_DISPLAY, DCTL_ON ) ;

   // Restore the presentation space to its original state.
   #ifdef DEBUG_METAFILE
      Log ( "WorkSpace(%08X:'%s')::MeasureMetafile(%08X) Restoring workspace.", this, Name, Metafile ) ;
   #endif
   Reset ( TRUE ) ;
   if ( !GpiSetPageViewport ( hPS, &OriginalViewport ) ) {
      char Message [512] ;
      Log ( "WorkSpace(%08X:'%s')::MeasureMetafile: Unable to restore original page viewport: %s", this, Name, InterpretWinError ( Anchor, Message ) ) ;
   } /* endif */
   if ( !GpiSetDefaultViewMatrix ( hPS, 9, &OriginalMatrix, TRANSFORM_REPLACE ) ) {
      char Message [512] ;
      Log ( "WorkSpace(%08X:'%s')::MeasureMetafile: Unable to restore original default viewing matrix: %s", this, Name, InterpretWinError ( Anchor, Message ) ) ;
   } /* endif */

   // Release the workspace and the device context.
   #ifdef DEBUG_METAFILE
      Log ( "WorkSpace(%08X:'%s')::MeasureMetafile(%08X) Unlocking workspace & device context.", this, Name, Metafile ) ;
   #endif
   Lock.Release ( ) ;  if ( pDevice ) pDevice->Unlock ( ) ;

   #ifdef DEBUG_METAFILE
      Log ( "WorkSpace(%08X:'%s')::MeasureMetafile(%08X) Done.  Rectangle %i,%i-%i,%i.", 
         this, Name, Metafile, Rectangle.xLeft, Rectangle.yBottom, Rectangle.xRight, Rectangle.yTop ) ;
   #endif
   return ( 0 ) ;

} /* endmethod */
 
/****************************************************************************
 *                                                                          *
 *  Method: Draw Metafile                                                   *
 *                                                                          *
 ****************************************************************************/

int WorkSpace::DrawMetafile ( HMF Metafile, POINTL &Position, SIZEL &Size ) {

   // Lock the device context and the workspace.
   #ifdef DEBUG_METAFILE
      Log ( "WorkSpace(%08X:'%s')::DrawMetafile(%08X) Locking device context & workspace.", this, Name, Metafile ) ;
   #endif
   if ( pDevice ) pDevice->Lock ( ) ;  Lock.Request ( ) ;

   // Save the viewport and the default view transform.
   MATRIXLF Original_ModelTransformMatrix ;
   GpiQueryModelTransformMatrix ( hPS, 9, &Original_ModelTransformMatrix ) ;
   MATRIXLF Original_ViewingTransformMatrix ;
   GpiQueryViewingTransformMatrix ( hPS, 9, &Original_ViewingTransformMatrix ) ;
   MATRIXLF Original_DefaultViewMatrix ;
   GpiQueryDefaultViewMatrix ( hPS, 9, &Original_DefaultViewMatrix ) ;
   RECTL Original_PageViewport ;
   GpiQueryPageViewport ( hPS, &Original_PageViewport ) ;

   // Determine the drawing rectangle in device units.
   RECTL Box = { Position.x, Position.y, Position.x+Size.cx-1, Position.y+Size.cy-1 } ;
   #ifdef DEBUG_METAFILE
      SIZEL CurrentPageSize ; ULONG CurrentOptions = GpiQueryPS ( hPS, &CurrentPageSize ) ;
      Log ( "WorkSpace(%08X:'%s')::DrawMetafile(%08X) Graphic cell (%i,%i-%i,%i) in default page units.  Current pagesize %ix%i, options %08lX.",
         this, Name, Metafile, Box.xLeft, Box.yBottom, Box.xRight, Box.yTop, CurrentPageSize.cx, CurrentPageSize.cy, CurrentOptions ) ;
   #endif
   Transform ( CVTC_DEFAULTPAGE, CVTC_DEVICE, Box ) ;
   #ifdef DEBUG_METAFILE
      Log ( "WorkSpace(%08X:'%s')::DrawMetafile(%08X) Graphic cell in device units %i,%i-%i,%i.",
         this, Name, Metafile, Box.xLeft, Box.yBottom, Box.xRight, Box.yTop ) ;
   #endif

   // Measure the graphic.
   #ifdef DEBUG_METAFILE
      Log ( "WorkSpace(%08X:'%s')::DrawMetafile(%08X) Playing metafile to bit-bucket to get its size.", this, Name, Metafile ) ;
   #endif
   GpiResetBoundaryData ( hPS ) ;
   GpiSetDrawControl ( hPS, DCTL_DISPLAY, DCTL_OFF ) ;
   GpiSetDrawControl ( hPS, DCTL_BOUNDARY, DCTL_ON ) ;
   LONG Options [] = { 0, LT_NOMODIFY, 0, LC_LOADDISC, RES_NORESET,
      SUP_NOSUPPRESS, CTAB_REPLACE, CREA_NOREALIZE, DDEF_LOADDISC } ;
   if ( GpiPlayMetaFile ( hPS, Metafile, sizeof(Options)/sizeof(Options[0]), Options, 0, 0, 0 ) == GPI_ERROR ) {
      char Message [512] ;
      Log ( "WorkSpace(%08X:'%s')::DrawMetafile: Unable to play metafile %08X: %s", this, Name, Metafile, InterpretWinError ( Anchor, Message ) ) ;
      Reset ( TRUE ) ;
      if ( !GpiSetPageViewport ( hPS, &Original_PageViewport ) ) {
         char Message [512] ;
         Log ( "WorkSpace(%08X:'%s')::DrawMetafile: Unable to restore original page viewport: %s", this, Name, InterpretWinError ( Anchor, Message ) ) ;
      } /* endif */
      if ( !GpiSetDefaultViewMatrix ( hPS, 9, &Original_DefaultViewMatrix, TRANSFORM_REPLACE ) ) {
         char Message [512] ;
         Log ( "WorkSpace(%08X:'%s')::DrawMetafile: Unable to restore original default viewing matrix: %s", this, Name, InterpretWinError ( Anchor, Message ) ) ;
      } /* endif */
      if ( !GpiSetViewingTransformMatrix ( hPS, 9, &Original_ViewingTransformMatrix, TRANSFORM_REPLACE ) ) {
         char Message [512] ;
         Log ( "WorkSpace(%08X:'%s')::DrawMetafile: Unable to restore original viewing transform matrix: %s", this, Name, InterpretWinError ( Anchor, Message ) ) ;
      } /* endif */
      if ( !GpiSetModelTransformMatrix ( hPS, 9, &Original_ModelTransformMatrix, TRANSFORM_REPLACE ) ) {
         char Message [512] ;
         Log ( "WorkSpace(%08X:'%s')::DrawMetafile: Unable to restore original model transform matrix: %s", this, Name, InterpretWinError ( Anchor, Message ) ) ;
      } /* endif */
      Lock.Release ( ) ;  if ( pDevice ) pDevice->Unlock ( ) ;
      return ( 1 ) ;
   } /* endif */
   RECTL Rectangle ;
   GpiQueryBoundaryData ( hPS, &Rectangle ) ;
   GpiSetDrawControl ( hPS, DCTL_BOUNDARY, DCTL_OFF ) ;
   GpiSetDrawControl ( hPS, DCTL_DISPLAY, DCTL_ON ) ;

   // Set viewing transformation.
   #ifdef DEBUG_METAFILE
      CurrentOptions = GpiQueryPS ( hPS, &CurrentPageSize ) ;
      Log ( "WorkSpace(%08X:'%s')::DrawMetafile(%08X) Setting viewing transform.  Current pagesize %ix%i, options %08lX.", 
         this, Name, Metafile, CurrentPageSize.cx, CurrentPageSize.cy, CurrentOptions ) ;
   #endif
   if ( !GpiSetPageViewport ( hPS, &Original_PageViewport ) ) {
      char Message [512] ;
      Log ( "WorkSpace(%08X:'%s')::DrawMetafile: Unable to restore original page viewport: %s", this, Name, InterpretWinError ( Anchor, Message ) ) ;
   } /* endif */
   if ( !GpiSetDefaultViewMatrix ( hPS, 9, &Original_DefaultViewMatrix, TRANSFORM_REPLACE ) ) {
      char Message [512] ;
      Log ( "WorkSpace(%08X:'%s')::DrawMetafile: Unable to restore original default viewing matrix: %s", this, Name, InterpretWinError ( Anchor, Message ) ) ;
   } /* endif */
   if ( !GpiSetViewingTransformMatrix ( hPS, 9, &Original_ViewingTransformMatrix, TRANSFORM_REPLACE ) ) {
      char Message [512] ;
      Log ( "WorkSpace(%08X:'%s')::DrawMetafile: Unable to restore original viewing transform matrix: %s", this, Name, InterpretWinError ( Anchor, Message ) ) ;
   } /* endif */
   if ( !GpiSetModelTransformMatrix ( hPS, 9, &Original_ModelTransformMatrix, TRANSFORM_REPLACE ) ) {
      char Message [512] ;
      Log ( "WorkSpace(%08X:'%s')::DrawMetafile: Unable to restore original model transform matrix: %s", this, Name, InterpretWinError ( Anchor, Message ) ) ;
   } /* endif */
   Transform ( CVTC_DEVICE, CVTC_PAGE, Box ) ;
   #ifdef DEBUG_METAFILE
      Log ( "WorkSpace(%08X:'%s')::DrawMetafile(%08X) Graphic cell in page units %i,%i-%i,%i.",
         this, Name, Metafile, Box.xLeft, Box.yBottom, Box.xRight, Box.yTop ) ;
   #endif
   double ScaleX = double(Box.xRight-Box.xLeft) / double(Rectangle.xRight-Rectangle.xLeft) ;
   double ScaleY = double(Box.yTop-Box.yBottom) / double(Rectangle.yTop-Rectangle.yBottom) ;
   #ifdef DEBUG_METAFILE
      Log ( "WorkSpace(%08X:'%s')::DrawMetafile(%08X) Zoom M11:%lf, M22:%lf.", this, Name, Metafile, ScaleX, ScaleY ) ;
   #endif
   USHORT Upper = USHORT ( ScaleX ) ;
   USHORT Lower = USHORT ( ( ScaleX - double(Upper) ) * 0x10000L ) ;
   FIXED ZoomX = MAKEFIXED ( Upper, Lower ) ;
   Upper = USHORT ( ScaleY ) ;
   Lower = USHORT ( ( ScaleY - double(Upper) ) * 0x10000L ) ;
   FIXED ZoomY = MAKEFIXED ( Upper, Lower ) ;
   MATRIXLF Matrix ;
   memset ( &Matrix, 0, sizeof(Matrix) ) ;
   Matrix.fxM11 = ZoomX ;
   Matrix.fxM22 = ZoomY ;
   #ifdef DEBUG_METAFILE
      Log ( "WorkSpace(%08X:'%s')::DrawMetafile(%08X) Box.xLeft %i, Rectangle.xLeft %i.", this, Name, Metafile, Box.xLeft, Rectangle.xLeft ) ;
   #endif
   Matrix.lM31 = Box.xLeft - long ( ScaleX * Rectangle.xLeft ) ;
   #ifdef DEBUG_METAFILE
      Log ( "WorkSpace(%08X:'%s')::DrawMetafile(%08X) Box.yBottom %i, Rectangle.yBottom %i.", this, Name, Metafile, Box.yBottom, Rectangle.yBottom ) ;
   #endif
   Matrix.lM32 = Box.yBottom - long ( ScaleY * Rectangle.yBottom ) ;
   Matrix.lM33 = 1 ;
   #ifdef DEBUG_METAFILE
      Log ( "WorkSpace(%08X:'%s')::DrawMetafile(%08X) Offsets M31:%i, M32:%i.", this, Name, Metafile, Matrix.lM31, Matrix.lM32 ) ;
   #endif
   if ( !GpiSetDefaultViewMatrix ( hPS, 9, &Matrix, TRANSFORM_REPLACE ) ) {
      char Message [512] ;
      Log ( "WorkSpace(%08X:'%s')::DrawMetafile: Unable to add to default viewing matrix: %s", this, Name, InterpretWinError ( Anchor, Message ) ) ;
   } /* endif */

   // Display the metafile, now that we've scaled everything.
   #ifdef DEBUG_METAFILE
      Log ( "WorkSpace(%08X:'%s')::DrawMetafile(%08X) Playing metafile to presentation space.", this, Name, Metafile ) ;
   #endif
   Options[PMF_SUPPRESS] = SUP_NOSUPPRESS ;
   Options[PMF_LCIDS] = LC_NOLOAD ;
   Options[PMF_DEFAULTS] = DDEF_IGNORE ;
   if ( GpiPlayMetaFile ( hPS, Metafile, sizeof(Options)/sizeof(Options[0]), Options, 0, 0, 0 ) == GPI_ERROR ) {
      char Message [512] ;
      Log ( "WorkSpace(%08X:'%s')::DrawMetafile: Unable to play metafile %08X: %s", this, Name, Metafile, InterpretWinError ( Anchor, Message ) ) ;
      Reset ( TRUE ) ;
      if ( !GpiSetPageViewport ( hPS, &Original_PageViewport ) ) {
         char Message [512] ;
         Log ( "WorkSpace(%08X:'%s')::DrawMetafile: Unable to restore original page viewport: %s", this, Name, InterpretWinError ( Anchor, Message ) ) ;
      } /* endif */
      if ( !GpiSetDefaultViewMatrix ( hPS, 9, &Original_DefaultViewMatrix, TRANSFORM_REPLACE ) ) {
         char Message [512] ;
         Log ( "WorkSpace(%08X:'%s')::DrawMetafile: Unable to restore original default viewing matrix: %s", this, Name, InterpretWinError ( Anchor, Message ) ) ;
      } /* endif */
      if ( !GpiSetViewingTransformMatrix ( hPS, 9, &Original_ViewingTransformMatrix, TRANSFORM_REPLACE ) ) {
         char Message [512] ;
         Log ( "WorkSpace(%08X:'%s')::DrawMetafile: Unable to restore original viewing transform matrix: %s", this, Name, InterpretWinError ( Anchor, Message ) ) ;
      } /* endif */
      if ( !GpiSetModelTransformMatrix ( hPS, 9, &Original_ModelTransformMatrix, TRANSFORM_REPLACE ) ) {
         char Message [512] ;
         Log ( "WorkSpace(%08X:'%s')::DrawMetafile: Unable to restore original model transform matrix: %s", this, Name, InterpretWinError ( Anchor, Message ) ) ;
      } /* endif */
      Lock.Release ( ) ;  if ( pDevice ) pDevice->Unlock ( ) ;
      return ( 1 ) ;
   } /* endif */

   // Restore the presentation space to its original state.
   #ifdef DEBUG_METAFILE
      CurrentOptions = GpiQueryPS ( hPS, &CurrentPageSize ) ;
      Log ( "WorkSpace(%08X:'%s')::DrawMetafile(%08X) Restoring saved workspace.  Current pagesize %ix%i, options %08lX.", 
         this, Name, Metafile, CurrentPageSize.cx, CurrentPageSize.cy, CurrentOptions ) ;
   #endif
   Reset ( TRUE ) ;
   if ( !GpiSetPageViewport ( hPS, &Original_PageViewport ) ) {
      char Message [512] ;
      Log ( "WorkSpace(%08X:'%s')::DrawMetafile: Unable to restore original page viewport: %s", this, Name, InterpretWinError ( Anchor, Message ) ) ;
   } /* endif */
   if ( !GpiSetDefaultViewMatrix ( hPS, 9, &Original_DefaultViewMatrix, TRANSFORM_REPLACE ) ) {
      char Message [512] ;
      Log ( "WorkSpace(%08X:'%s')::DrawMetafile: Unable to restore original default viewing matrix: %s", this, Name, InterpretWinError ( Anchor, Message ) ) ;
   } /* endif */
   if ( !GpiSetViewingTransformMatrix ( hPS, 9, &Original_ViewingTransformMatrix, TRANSFORM_REPLACE ) ) {
      char Message [512] ;
      Log ( "WorkSpace(%08X:'%s')::DrawMetafile: Unable to restore original viewing transform matrix: %s", this, Name, InterpretWinError ( Anchor, Message ) ) ;
   } /* endif */
   if ( !GpiSetModelTransformMatrix ( hPS, 9, &Original_ModelTransformMatrix, TRANSFORM_REPLACE ) ) {
      char Message [512] ;
      Log ( "WorkSpace(%08X:'%s')::DrawMetafile: Unable to restore original model transform matrix: %s", this, Name, InterpretWinError ( Anchor, Message ) ) ;
   } /* endif */

   #ifdef DEBUG_METAFILE
      Log ( "WorkSpace(%08X:'%s')::DrawMetafile(%08X) Unlocking workspace & device context.", this, Name, Metafile ) ;
   #endif
   Lock.Release ( ) ;  if ( pDevice ) pDevice->Unlock ( ) ;

   #ifdef DEBUG_METAFILE
      Log ( "WorkSpace(%08X:'%s')::DrawMetafile(%08X) Done.", this, Name, Metafile ) ;
   #endif

   return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *  Method: Draw UPC-A Code                                                 *
 *                                                                          *
 ****************************************************************************/

/* UPC-A Definitions */

#define EAN_END    "101"
#define EAN_END1   "010"   // End inverted.
#define EAN_CENTER "01010"

static char *EanSetA [] = {  // Also known as the set with odd parity.
   "0001101", "0011001", "0010011", "0111101", "0100011",
   "0110001", "0101111", "0111011", "0110111", "0001011"
} ;

static char *EanSetB [] = {  // Also known as the set with even parity.
   "0100111", "0110011", "0011011", "0100001", "0011101",
   "0111001", "0000101", "0010001", "0001001", "0010111"
} ;

static char *EanSetC [] = {  // This set is simply Set A inverted.
   "1110010", "1100110", "1101100", "1000010", "1011100",
   "1001110", "1010000", "1000100", "1001000", "1110100"
} ;

static char *EanCheckPattern [] = {
   "BBBBBB", "BBABAA", "BBAABA", "BBAAAB", "BABBAA",
   "BAABBA", "BAAABB", "BABABA", "BABAAB", "BAABAB"
} ;

static void UPC_A_from_E ( char *UpcE, char *UpcA ) {
   memset ( UpcA, '0', 11 ) ;
   UpcA[1] = UpcE[0] ;    
   UpcA[2] = UpcE[1] ;
   switch ( UpcE[5] ) {
      case '0':
      case '1':
      case '2':
         UpcA[ 8] = UpcE[2] ;
         UpcA[ 9] = UpcE[3] ;
         UpcA[10] = UpcE[4] ;
         break;
      case '3':
         UpcA[ 3] = UpcE[2] ;
         UpcA[ 9] = UpcE[3] ;
         UpcA[10] = UpcE[4] ;
         break;
      case '4':
         UpcA[ 3] = UpcE[2] ;
         UpcA[ 4] = UpcE[3] ;
         UpcA[10] = UpcE[4] ;
         break;
      case '5':
      case '6':
      case '7':
      case '8':
      case '9':
         UpcA[ 3] = UpcE[2] ;
         UpcA[ 4] = UpcE[3] ;
         UpcA[ 5] = UpcE[4] ;
         UpcA[10] = UpcE[5] ;
         break;
   } /* endswitch */
   UpcA[11] = 0 ;         
}

static int EAN_CheckDigit ( char *BarCode ) {
   char Digits [13] ;
   strcpy ( Digits, BarCode ) ;
   if ( strlen(Digits) == 6 ) {
      char Text [8] ;
      strcpy ( Text, Digits ) ;
      UPC_A_from_E ( Text, Digits ) ;
   } /* endif */
   int CheckDigit = 0 ;
   for ( int i=0; i<strlen(Digits); i++ ) {
      CheckDigit += ( ( i % 2 ) ? 1 : 3 ) * ( Digits[i] - '0' ) ;
   } /* endfor */
   CheckDigit %= 10 ;
   CheckDigit = 10 - CheckDigit ;
   CheckDigit %= 10 ;
   return ( CheckDigit ) ;
} 

int WorkSpace::DrawUpcCodeA ( char *UpcCode, POINTL &Position, SIZEL &Size ) {

   #ifdef DEBUG
      Log ( "WorkSpace(%08X:'%s')::DrawUpcCodeA(\"%s\")", this, Name, UpcCode ) ;
   #endif

   // Lock the PS.
   if ( pDevice ) pDevice->Lock ( ) ;  Lock.Request ( ) ;

   // Right-justify the code and left-pad it with zero digits.
   char Digits [12] ;
   sprintf ( Digits, "%11.11s", UpcCode ) ;
   for ( int i=0; i<strlen(Digits); i++ ) 
      if ( Digits[i] == ' ' ) 
         Digits [i] = '0' ;

   // Compute the check-digit.
   int CheckDigit = EAN_CheckDigit ( Digits ) ;

   // Generate the barcode pattern.
   char Pattern [96] ;
   strcpy ( Pattern, EAN_END ) ;
   for ( i=0; i<6; i++ ) {
     strcat ( Pattern, EanSetA[Digits[i]-'0'] ) ;
   } /* endfor */
   strcat ( Pattern, EAN_CENTER ) ;
   for ( i=6; i<11; i++ ) {
     strcat ( Pattern, EanSetC[Digits[i]-'0'] ) ;
   } /* endfor */
   strcat ( Pattern, EanSetC[CheckDigit] ) ;
   strcat ( Pattern, EAN_END ) ;

   // Paint the barcode.
   RECTL Rectangle = { Position.x, Position.y, Position.x, Position.y+Size.cy } ;
   for ( i=0; i<strlen(Pattern); i++ ) {
      if ( ( Pattern[i] == '0' ) && i && ( Pattern[i-1] == '1' ) ) {            // Complete a bar.
         Rectangle.xRight = Position.x + long ( ( i * double(Size.cx) ) / strlen(Pattern) + 0.4 ) ;
         FillBox ( Rectangle ) ;
      } else if ( ( Pattern[i] == '1' ) && i && ( Pattern[i-1] == '0' ) ) {     // Mark start and end of bar.
         Rectangle.xLeft = Position.x + long ( ( i * double(Size.cx) ) / strlen(Pattern) + 0.4 ) ;
         Rectangle.xRight = Position.x + long ( ( (i+1) * double(Size.cx) ) / strlen(Pattern) + 0.4 ) ;
      } else if ( Pattern[i] == '1' ) {                                         // Update end of bar.
         Rectangle.xRight = Position.x + long ( ( (i+1) * double(Size.cx) ) / strlen(Pattern) + 0.4 ) ;
      } /* endif */
   } /* endfor */
   if ( Pattern[i-1] == '1' ) {                                         // Complete final bar.
      Rectangle.xRight = Position.x + long ( ( i * double(Size.cx) ) / strlen(Pattern) + 0.4 ) ;
      FillBox ( Rectangle ) ;
   } /* endif */

   // Release the PS.
   Lock.Release ( ) ;  if ( pDevice ) pDevice->Unlock ( ) ;

   return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *  Method: Draw UPC-E Code                                                 *
 *                                                                          *
 ****************************************************************************/

int WorkSpace::DrawUpcCodeE ( char *UpcCode, POINTL &Position, SIZEL &Size ) {

   #ifdef DEBUG
      Log ( "WorkSpace(%08X:'%s')::DrawUpcCodeE(\"%s\")", this, Name, UpcCode ) ;
   #endif

   // Lock the PS.
   if ( pDevice ) pDevice->Lock ( ) ;  Lock.Request ( ) ;

   // Draw a box around the cell (temporary).
#if 0
   RECTL Box = { Position.x, Position.y, Position.x+Size.cx, Position.y+Size.cy } ;
   DrawBox ( Box ) ;

   // Get the font we'll need.
   int OldFont = Fonts.Query_Font ( ) ;
   Fonts.Select ( hPS, "Helv" ) ;
   int OldFontSize = FontSize ;
   int OldExpand = Expand ;
#endif

   // Right-justify the code and left-pad it with zero digits.
   char Digits [7] ;
   sprintf ( Digits, "%6.6s", UpcCode ) ;
   for ( int i=0; i<strlen(Digits); i++ ) 
      if ( Digits[i] == ' ' ) 
         Digits [i] = '0' ;

   // Compute the check-digit.
   int CheckDigit = EAN_CheckDigit ( Digits ) ;
   char *CheckPattern = EanCheckPattern [CheckDigit] ;

   // Generate the barcode pattern.
   char Pattern [52] ;
   strcpy ( Pattern, EAN_END ) ;
   for ( i=0; i<strlen(Digits); i++ ) {
      strcat ( Pattern, CheckPattern[i]=='A' ? EanSetA[Digits[i]-'0'] : EanSetB[Digits[i]-'0'] ) ;
   } /* endfor */
   strcat ( Pattern, EAN_END1 ) ;
   strcat ( Pattern, EAN_END ) ;

   // Paint the barcode.
   RECTL Rectangle = { Position.x, Position.y, Position.x, Position.y+Size.cy } ;
   for ( i=0; i<strlen(Pattern); i++ ) {
      if ( ( Pattern[i] == '0' ) && i && ( Pattern[i-1] == '1' ) ) {            // Complete a bar.
         Rectangle.xRight = Position.x + long ( ( i * double(Size.cx) ) / strlen(Pattern) + 0.4 ) ;
         FillBox ( Rectangle ) ;
      } else if ( ( Pattern[i] == '1' ) && i && ( Pattern[i-1] == '0' ) ) {     // Mark start and end of bar.
         Rectangle.xLeft = Position.x + long ( ( i * double(Size.cx) ) / strlen(Pattern) + 0.4 ) ;
         Rectangle.xRight = Position.x + long ( ( (i+1) * double(Size.cx) ) / strlen(Pattern) + 0.4 ) ;
      } else if ( Pattern[i] == '1' ) {                                         // Update end of bar.
         Rectangle.xRight = Position.x + long ( ( (i+1) * double(Size.cx) ) / strlen(Pattern) + 0.4 ) ;
      } /* endif */
   } /* endfor */
   if ( Pattern[i-1] == '1' ) {                                         // Complete final bar.
      Rectangle.xRight = Position.x + long ( ( i * double(Size.cx) ) / strlen(Pattern) + 0.4 ) ;
      FillBox ( Rectangle ) ;
   } /* endif */

   // Paint the numeric interpretation.
#if 0
   Move ( Position ) ;
   FontSize = ( Size.cx / 12 ) / ( Metric ? 2.54 : 1 ) ;
   Expand = 100 ;
   ScaleFont ( ) ;
   if ( GpiCharString ( hPS, strlen(Digits), PCH(Digits) ) == GPI_ERROR ) {
      char Message [512] ;
      Log ( "WorkSpace(%08X:'%s')::DrawText: Unable to draw text: %s", this, Name, InterpretWinError ( Anchor, Message ) ) ;
   } /* endif */

   // Restore the old font.
   Fonts.Set_Font ( hPS, OldFont ) ;
   FontSize = OldFontSize ;
   Expand = OldExpand ;
   ScaleFont ( ) ;
#endif

   // Release the PS.
   Lock.Release ( ) ;  if ( pDevice ) pDevice->Unlock ( ) ;

   return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *  Method: Draw Code-39 Barcode                                            *
 *                                                                          *
 ****************************************************************************/

struct {
   char Character ;
   char *Codes ;
} Code39Table[] = {
   { '1', "011100100001001001110" },
   { '2', "010011100001001001110" },
   { '3', "011100111000010010010" },
   { '4', "010010000111001001110" },
   { '5', "011100100001110010010" },
   { '6', "010011100000110010010" },
   { '7', "010010000100111001110" },
   { '8', "011100100001001110010" },
   { '9', "010011100001001110010" },
   { '0', "010010000111001110010" },
   { 'A', "011100100100001001110" },
   { 'B', "010011100100001001110" },
   { 'C', "011100111001000010010" },
   { 'D', "010010011100001001110" },
   { 'E', "011100100111000010010" },
   { 'F', "010011100111000010010" },
   { 'G', "010010010000111001110" },
   { 'H', "011100100100001110010" },
   { 'I', "010011100100001110010" },
   { 'J', "010010011100001110010" },
   { 'K', "011100100100100001110" },
   { 'L', "010011100100100001110" },
   { 'M', "011100111001001000010" },
   { 'N', "010010011100100001110" },
   { 'O', "011100100111001000010" },
   { 'P', "010011100111001000010" },
   { 'Q', "010010010011100001110" },
   { 'R', "011100100100111000010" },
   { 'S', "010011100100111000010" },
   { 'T', "010010011100111000010" },
   { 'U', "011100001001001001110" },
   { 'V', "010000111001001001110" },
   { 'W', "011100001110010010010" },
   { 'X', "010000100111001001110" },
   { 'Y', "011100001001110010010" },
   { 'Z', "010000111001110010010" },
   { '-', "010000100100111001110" },
   { '.', "011100001001001110010" },
   { ' ', "010000111001001110010" },
   { '*', "010000100111001110010" },
   { '$', "010000100001000010010" },
   { '/', "010000100001001000010" },
   { '+', "010000100100001000010" },
   { '%', "010010000100001000010" },
} ;

inline char *GetCode39 ( char Character ) {

   for ( int i=0; i<sizeof(Code39Table)/sizeof(Code39Table[0]); i++ ) {
      if ( Code39Table[i].Character == Character )
         break ;
   } /* endfor */

   if ( i >= sizeof(Code39Table)/sizeof(Code39Table[0]) ) 
      return ( "" ) ;

   return ( Code39Table[i].Codes ) ;
}

int WorkSpace::DrawCode39 ( char *BarCode, POINTL &Position, SIZEL &Size ) {

   #ifdef DEBUG
      Log ( "WorkSpace(%08X:'%s')::DrawCode39(\"%s\")", this, Name, BarCode ) ;
   #endif

   // Lock the PS.
   if ( pDevice ) pDevice->Lock ( ) ;  Lock.Request ( ) ;

   // Allocate text buffer.
   char *Pattern = (char*) malloc ( strlen(Code39Table[0].Codes) * (strlen(BarCode)+2) + 1 ) ;

   // Build up the text.
   strcpy ( Pattern, GetCode39 ( '*' ) ) ;
   for ( int i=0; i<strlen(BarCode); i++ ) {
      strcat ( Pattern, GetCode39 ( BarCode[i] ) ) ;
   } /* endfor */
   strcat ( Pattern, GetCode39 ( '*' ) ) ;

   RECTL Rectangle = { Position.x, Position.y, Position.x, Position.y+Size.cy } ;
   for ( i=0; i<strlen(Pattern); i++ ) {
      if ( ( Pattern[i] == '0' ) && i && ( Pattern[i-1] == '1' ) ) {            // Complete a bar.
         Rectangle.xRight = Position.x + long ( ( i * double(Size.cx) ) / strlen(Pattern) + 0.4 ) ;
         FillBox ( Rectangle ) ;
      } else if ( ( Pattern[i] == '1' ) && i && ( Pattern[i-1] == '0' ) ) {     // Mark start and end of bar.
         Rectangle.xLeft = Position.x + long ( ( i * double(Size.cx) ) / strlen(Pattern) + 0.4 ) ;
         Rectangle.xRight = Position.x + long ( ( (i+1) * double(Size.cx) ) / strlen(Pattern) + 0.4 ) ;
      } else if ( Pattern[i] == '1' ) {                                         // Update end of bar.
         Rectangle.xRight = Position.x + long ( ( (i+1) * double(Size.cx) ) / strlen(Pattern) + 0.4 ) ;
      } /* endif */
   } /* endfor */

   if ( Pattern[i-1] == '1' ) {                                         // Complete final bar.
      Rectangle.xRight = Position.x + long ( ( i * double(Size.cx) ) / strlen(Pattern) + 0.4 ) ;
      FillBox ( Rectangle ) ;
   } /* endif */

   // Release the buffer.
   free ( Pattern ) ;

   // Unlock the PS.
   Lock.Release ( ) ;  if ( pDevice ) pDevice->Unlock ( ) ;

   // Return no error.
   return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *  Method: Draw POSTNET Barcode                                            *
 *                                                                          *
 ****************************************************************************/

static int Ticks [] = { 7, 8, 4, 4 } ;

static char *FrameBar = "31" ;

static char *PostNetCodes [] = {
   "3130212021",        // "0"
   "2120213031",        // "1"
   "2120312031",        // "2"
   "2120313021",        // "3"
   "2130212031",        // "4"
   "2130213021",        // "5"
   "2130312021",        // "6"
   "3120212031",        // "7"
   "3120213021",        // "8"
   "3120312021"         // "9"
} ;

int WorkSpace::DrawPOSTNET ( char *BarCode, POINTL &Position, SIZEL &Size ) {

   #if defined(DEBUG) || defined(DEBUG_POSTNET)
      Log ( "WorkSpace(%08X:'%s')::DrawPOSTNET(\"%s\")", this, Name, BarCode ) ;
   #endif

   // Lock the PS.
   #ifdef DEBUG_POSTNET
      Log ( "WorkSpace(%08X:'%s')::DrawPOSTNET(\"%s\") Started.  Locking device & saving PS.", this, Name, BarCode ) ;
   #endif
   if ( pDevice ) pDevice->Lock ( ) ;  Lock.Request ( ) ;

   // Compute the check-digit.
   #ifdef DEBUG_POSTNET
      Log ( "WorkSpace(%08X:'%s')::DrawPOSTNET(\"%s\") Computing check-digit.", this, Name, BarCode ) ;
   #endif
   int CheckDigit = 0 ;
   for ( int i=0; i<min(9,strlen(BarCode)); i++ ) {
      CheckDigit += BarCode[i] - '0' ;
   } /* endfor */
   CheckDigit %= 10 ;
   CheckDigit = 10 - CheckDigit ;
   CheckDigit %= 10 ;

   // Build up the token pattern.
   #ifdef DEBUG_POSTNET
      Log ( "WorkSpace(%08X:'%s')::DrawPOSTNET(\"%s\") Building token pattern with check-digit %i.", this, Name, BarCode, CheckDigit ) ;
   #endif
   char Pattern [105] ;
   strcpy ( Pattern, FrameBar ) ;
   for ( i=0; i<min(9,strlen(BarCode)); i++ ) {
      strcat ( Pattern, PostNetCodes[BarCode[i]-'0'] ) ;
   } /* endfor */
   strcat ( Pattern, PostNetCodes[CheckDigit] ) ;
   strcat ( Pattern, FrameBar ) ;

   // How many ticks in this code?
   #ifdef DEBUG_POSTNET
      Log ( "WorkSpace(%08X:'%s')::DrawPOSTNET(\"%s\") Counting ticks in pattern '%s'.", this, Name, BarCode, Pattern ) ;
   #endif
   int TickCount = 0 ;
   for ( i=0; i<strlen(Pattern); i++ ) {
      TickCount += Ticks[Pattern[i]-'0'] ;
   } /* endfor */

   // Determine just how wide a tick will be.
   #ifdef DEBUG_POSTNET
      Log ( "WorkSpace(%08X:'%s')::DrawPOSTNET(\"%s\") Determining tick-width for %i ticks.", this, Name, BarCode, TickCount ) ;
   #endif
   double TickWidth = double(Size.cx) / TickCount ;

   // Draw the barcode.
   #ifdef DEBUG_POSTNET
      Log ( "WorkSpace(%08X:'%s')::DrawPOSTNET(\"%s\") Drawing barcode with tick width %.4lf.", this, Name, BarCode, TickWidth ) ;
   #endif
   int TickPosition = 0 ;
   for ( i=0; i<strlen(Pattern); i++ ) {
      switch ( Pattern[i] ) {
         case '0': {
            // Half-Space.  Do nothing.
            break; }
         case '1': {
            // Full-Space.  Do nothing.
            break; }
         case '2': {
            // Half-Bar.
            RECTL Rectangle = { long(Position.x+TickWidth*TickPosition+0.4), Position.y, long(Position.x+TickWidth*(TickPosition+4)+0.4), Position.y+Size.cy/2 } ;
            FillBox ( Rectangle ) ;
            break; }
         case '3': {
            // Full-Bar.
            RECTL Rectangle = { long(Position.x+TickWidth*TickPosition+0.4), Position.y, long(Position.x+TickWidth*(TickPosition+4)+0.4), Position.y+Size.cy } ;
            FillBox ( Rectangle ) ;
            break; }
      } /* endswitch */
      TickPosition += Ticks[Pattern[i]-'0'] ;
   } /* endfor */

   // Unlock the PS.
   #ifdef DEBUG_POSTNET
      Log ( "WorkSpace(%08X:'%s')::DrawPOSTNET(\"%s\") Restoring PS & unlocking device.", this, Name, BarCode ) ;
   #endif
   Lock.Release ( ) ;  if ( pDevice ) pDevice->Unlock ( ) ;

   // Return no error.
   #ifdef DEBUG_POSTNET
      Log ( "WorkSpace(%08X:'%s')::DrawPOSTNET(\"%s\") Done.", this, Name, BarCode ) ;
   #endif
   return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *  Method: Draw EAN-8 Barcode                                              *
 *                                                                          *
 ****************************************************************************/

int WorkSpace::DrawEAN8 ( char *BarCode, POINTL &Position, SIZEL &Size ) {

   #if defined(DEBUG) || defined(DEBUG_EAN8)
      Log ( "WorkSpace(%08X:'%s')::DrawEAN8(\"%s\")", this, Name, BarCode ) ;
   #endif

   // Lock the PS.
   if ( pDevice ) pDevice->Lock ( ) ;  Lock.Request ( ) ;

   // Right-justify the code and left-pad it with zero digits.
   char Digits [8] ;
   sprintf ( Digits, "%7.7s", BarCode ) ;
   for ( int i=0; i<strlen(Digits); i++ ) 
      if ( Digits[i] == ' ' ) 
         Digits [i] = '0' ;

   // Compute the check-digit.
   int CheckDigit = EAN_CheckDigit ( Digits ) ;

   // Build up the pattern to be coded.
   char Pattern [68] ;
   strcpy ( Pattern, EAN_END ) ;
   for ( i=0; i<4; i++ ) {
     strcat ( Pattern, EanSetA[Digits[i]-'0'] ) ;
   } /* endfor */
   strcat ( Pattern, EAN_CENTER ) ;
   for ( i=4; i<7; i++ ) {
     strcat ( Pattern, EanSetC[Digits[i]-'0'] ) ;
   } /* endfor */
   strcat ( Pattern, EanSetC[CheckDigit] ) ;
   strcat ( Pattern, EAN_END ) ;

   // Paint the barcode.
   RECTL Rectangle = { Position.x, Position.y, Position.x, Position.y+Size.cy } ;
   for ( i=0; i<strlen(Pattern); i++ ) {
      if ( ( Pattern[i] == '0' ) && i && ( Pattern[i-1] == '1' ) ) {            // Complete a bar.
         Rectangle.xRight = Position.x + long ( ( i * double(Size.cx) ) / strlen(Pattern) + 0.4 ) ;
         FillBox ( Rectangle ) ;
      } else if ( ( Pattern[i] == '1' ) && i && ( Pattern[i-1] == '0' ) ) {     // Mark start and end of bar.
         Rectangle.xLeft = Position.x + long ( ( i * double(Size.cx) ) / strlen(Pattern) + 0.4 ) ;
         Rectangle.xRight = Position.x + long ( ( (i+1) * double(Size.cx) ) / strlen(Pattern) + 0.4 ) ;
      } else if ( Pattern[i] == '1' ) {                                         // Update end of bar.
         Rectangle.xRight = Position.x + long ( ( (i+1) * double(Size.cx) ) / strlen(Pattern) + 0.4 ) ;
      } /* endif */
   } /* endfor */
   if ( Pattern[i-1] == '1' ) {                                         // Complete final bar.
      Rectangle.xRight = Position.x + long ( ( i * double(Size.cx) ) / strlen(Pattern) + 0.4 ) ;
      FillBox ( Rectangle ) ;
   } /* endif */

   // Unlock the PS.
   Lock.Release ( ) ;  if ( pDevice ) pDevice->Unlock ( ) ;

   // Return no error.
   return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *  Method: Draw EAN-13 Barcode                                             *
 *                                                                          *
 ****************************************************************************/

int WorkSpace::DrawEAN13 ( char *BarCode, POINTL &Position, SIZEL &Size ) {

   #if defined(DEBUG) || defined(DEBUG_EAN13)
      Log ( "WorkSpace(%08X:'%s')::DrawEAN13(\"%s\")", this, Name, BarCode ) ;
   #endif

   // Lock the PS.
   if ( pDevice ) pDevice->Lock ( ) ;  Lock.Request ( ) ;

   // Right-justify the code and left-pad it with zero digits.
   // Stick the first byte at the end because of the strange way it's handled.
   char Digits [13] ;
   sprintf ( Digits, "%11.11s%1.1s", BarCode+1, BarCode ) ;
   for ( int i=0; i<strlen(Digits); i++ ) 
      if ( Digits[i] == ' ' ) 
         Digits [i] = '0' ;

   // Compute the check-digit.
   int CheckDigit = EAN_CheckDigit ( Digits ) ;

   // Build up the pattern to be coded.
   char Pattern [103] ;
   strcpy ( Pattern, EAN_END ) ;
   for ( i=0; i<6; i++ ) {
      char *CheckPattern = EanCheckPattern [ Digits [11] - '0' ] ;
      strcat ( Pattern, (CheckPattern[5-i]=='A') ? EanSetA[Digits[i]-'0'] : EanSetB[Digits[i]-'0'] ) ;
   } /* endfor */
   strcat ( Pattern, EAN_CENTER ) ;
   for ( i=6; i<11; i++ ) {
      strcat ( Pattern, EanSetC[Digits[i]-'0'] ) ;
   } /* endfor */
   strcat ( Pattern, EanSetC[CheckDigit] ) ;
   strcat ( Pattern, EAN_END ) ;

   // Paint the barcode.
   RECTL Rectangle = { Position.x, Position.y, Position.x, Position.y+Size.cy } ;
   for ( i=0; i<strlen(Pattern); i++ ) {
      if ( ( Pattern[i] == '0' ) && i && ( Pattern[i-1] == '1' ) ) {            // Complete a bar.
         Rectangle.xRight = Position.x + long ( ( i * double(Size.cx) ) / strlen(Pattern) + 0.4 ) ;
         FillBox ( Rectangle ) ;
      } else if ( ( Pattern[i] == '1' ) && i && ( Pattern[i-1] == '0' ) ) {     // Mark start and end of bar.
         Rectangle.xLeft = Position.x + long ( ( i * double(Size.cx) ) / strlen(Pattern) + 0.4 ) ;
         Rectangle.xRight = Position.x + long ( ( (i+1) * double(Size.cx) ) / strlen(Pattern) + 0.4 ) ;
      } else if ( Pattern[i] == '1' ) {                                         // Update end of bar.
         Rectangle.xRight = Position.x + long ( ( (i+1) * double(Size.cx) ) / strlen(Pattern) + 0.4 ) ;
      } /* endif */
   } /* endfor */
   if ( Pattern[i-1] == '1' ) {                                         // Complete final bar.
      Rectangle.xRight = Position.x + long ( ( i * double(Size.cx) ) / strlen(Pattern) + 0.4 ) ;
      FillBox ( Rectangle ) ;
   } /* endif */

   // Unlock the PS.
   Lock.Release ( ) ;  if ( pDevice ) pDevice->Unlock ( ) ;

   // Return no error.
   return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *  Method: Issue New Frame command to device                               *
 *                                                                          *
 ****************************************************************************/

void WorkSpace::NewFrame ( ) {
   if ( pDevice ) pDevice->Lock ( ) ;  Lock.Request ( ) ;
   DevEscape ( GpiQueryDevice(hPS), DEVESC_NEWFRAME, 0, 0, 0, 0 ) ;
   Lock.Release ( ) ;  if ( pDevice ) pDevice->Unlock ( ) ;
}
 
/****************************************************************************
 *                                                                          *
 *  Method: Dump                                                            *
 *                                                                          *
 ****************************************************************************/

void WorkSpace::Dump ( int indent ) {

   char *Types[] = { "GetPS", "Paint", "Gpi" } ;

   Log ( "%*sDump of graphic workspace(%08X):", indent, "", this ) ;
   Log ( "%*s  Name:      '%s'",  indent, "", Name ) ;
   Log ( "%*s  CodePage:  %i", indent, "", CodePage ) ;

   if ( DBCSVector[0][0] ) {
      char Text [100] = { 0 } ;
      int i=0 ;
      while ( DBCSVector[i][0] ) {
         if ( i ) 
            strcat ( Text, ", " ) ;
         sprintf ( Text+strlen(Text), "%02X-%02X", DBCSVector[i][0], DBCSVector[i][1] ) ;
         i ++ ;
      } /* endwhile */
      Log ( "%*s    DBCS headers: %s.", indent, "", Text ) ;
   } else {
      Log ( "%*s    Single-byte character set.", indent, "" ) ;
   } /* endif */

   Log ( "%*s  Anchor:    %08lX", indent, "", Anchor ) ;
   Log ( "%*s  hPS:       %08lX", indent, "", hPS ) ;
   Log ( "%*s  Type:      %s%s",indent,"", Types[Type], Scaled?(Metric?" (Metric)":" (English)"):"" ) ;
   Log ( "%*s  PageSize:  %ix%i", indent, "", PageSize.cx, PageSize.cy ) ;
   Log ( "%*s  FontName:  %s",    indent, "", FontName ) ;
   Log ( "%*s  FontSize:  %0.1lf",indent, "", double(FontSize)/10.0 ) ;
   Log ( "%*s  FontAttr:  %s%s%s%s%s%s",indent, "",
      Bold?"Bold ":"", Italic?"Italic ":"", Underscore?"Underscore ":"",
      Strikeout?"Strikeout ":"", Outline?"Outline ":"", Caps?"Caps ":"" ) ;
   Log ( "%*s  LineColor: %06X",  indent, "", LineColor ) ;
   Log ( "%*s  LineType:  %i",    indent, "", LineType ) ;
   Log ( "%*s  LineWidth: %i",    indent, "", LineWidth ) ;
   Log ( "%*s  LineJoin:  %i",    indent, "", LineJoin ) ;
   Log ( "%*s  LineEnd:   %i",    indent, "", LineEnd ) ;
   Log ( "%*s  FillColor: %06X",  indent, "", FillColor ) ;
   Log ( "%*s  FillType:  %i",    indent, "", FillType ) ;
   Log ( "%*s  Ascent:    %i",    indent, "", Ascent ) ;
   Log ( "%*s  Descent:   %i",    indent, "", Descent ) ;
   Log ( "%*s  AvgWidth:  %i",    indent, "", AvgWidth ) ;
   Log ( "%*s  StrikeoutInfo: %i, %i",    indent, "", StrikeoutInfo[0], StrikeoutInfo[1] ) ;
   Log ( "%*s  UnderscoreInfo: %i, %i",    indent, "", UnderscoreInfo[0], UnderscoreInfo[1] ) ;
   double dZoom = double ( FIXEDINT ( Zoom ) ) + double ( FIXEDFRAC ( Zoom ) ) / 0x10000 ;
   Log ( "%*s  Zoom:      %.3lf", indent, "", dZoom ) ;
   Log ( "%*s  DeltaX,Y:  %i,%i", indent, "", DeltaX, DeltaY ) ;
   Fonts.Dump ( indent + 2 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *  Method: Query/Set Codepage                                              *
 *                                                                          *
 ****************************************************************************/

int WorkSpace::QueryCodepage ( ) {
   return ( int ( GpiQueryCp ( hPS ) ) ) ;
}

void WorkSpace::SetCodepage ( int cp ) {
   CodePage = cp ;
   SetCodepage ( ) ;
}

void WorkSpace::SetCodepage ( ) {

   if ( CodePage ) {

      if ( pDevice ) pDevice->Lock ( ) ;  Lock.Request ( ) ;

      if ( !GpiSetCp ( hPS, CodePage ) ) {
         char Message [512] ;
         Log ( "WorkSpace(%08X:'%s')::SetCodepage: Unable to set codepage %i: %s", this, Name, CodePage, InterpretWinError ( Anchor, Message ) ) ;
         memset ( DBCSVector, 0, sizeof(DBCSVector) ) ;
      } else {
         COUNTRYCODE Country = { 0, CodePage } ;
         APIRET Result = DosQueryDBCSEnv ( sizeof(DBCSVector), &Country, PCHAR(DBCSVector) ) ;
         if ( Result && ( Result != ERROR_NLS_NO_CTRY_CODE ) ) {
            Log ( "WorkSpace(%08X:'%s')::SetCodepage: Unable to query DBCS vector.  Status %08X.", this, Name, Result ) ;
            memset ( DBCSVector, 0, sizeof(DBCSVector) ) ;
         } /* endif */
      } /* endif */

      Lock.Release ( ) ;  if ( pDevice ) pDevice->Unlock ( ) ;

   } /* endif */
}
 
/****************************************************************************
 *                                                                          *
 *  Method: Determine if byte is DBCS Header.                               *
 *                                                                          *
 ****************************************************************************/

BOOL WorkSpace::IsDBCSHeader ( UCHAR Byte ) {

   // If no DBCS vector, return FALSE at once.
   if ( DBCSVector[0][0] == 0 ) 
      return ( FALSE ) ;

   // Scan the DBCS vector to see if this byte is a header.  Return TRUE if so.
   int Block = 0 ;
   while ( DBCSVector[Block][0] ) {
      if ( Byte >= DBCSVector[Block][0] )
         if ( Byte <= DBCSVector[Block][1] ) 
            return ( TRUE ) ;
      Block ++ ;
   } /* endwhile */

   // Return FALSE.
   return ( FALSE ) ;
}

/****************************************************************************
 *                                                                          *
 *  Method: Comment the Metafile                                            *
 *                                                                          *
 ****************************************************************************/

void WorkSpace::Comment ( char *Message, ... ) {
   if ( pDevice ) pDevice->Lock ( ) ;  Lock.Request ( ) ;
   va_list Marker ;
   char Text [500] ;
   va_start ( Marker, Message ) ;
   vsprintf ( Text, Message, Marker ) ;
   va_end ( Marker ) ;
   GpiComment ( hPS, strlen(Text), Text ) ;
   Lock.Release ( ) ;  if ( pDevice ) pDevice->Unlock ( ) ;
}

