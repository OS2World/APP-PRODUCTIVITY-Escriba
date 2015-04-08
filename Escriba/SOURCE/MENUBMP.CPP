/**************************************************************** MENUBMP.CPP
 *                                                                          *
 *                        Menu Bitmap Functions                             *
 *                                                                          *
 ****************************************************************************/

#include "System.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Debug.h"
#include "Document.h"
#include "Escriba.h"
#include "Event.h"
#include "HourGlas.h"
#include "MenuBmp.h"
#include "Mutex.h"
#include "Process.h"
#include "ReString.h"
#include "Support.h"
#include "Thread.h"
#include "WrkSpace.h"


// #define DEBUG
// #define DEBUG_QUERYFONTS
   #define THREADED
// #define MANAGERS

 
/****************************************************************************
 *                                                                          *
 *                   General Definitions & Declarations                     *
 *                                                                          *
 ****************************************************************************/

  // Class Definitions

class MenuBitmapBuilder {

   private:
      char    *Name ;
      PHBITMAP BitmapArray ;
      PSIZEL   BitmapSizes ;
      USHORT   Count ;
      Mutex    Lock ;

   public:
      MenuBitmapBuilder ( char *name ) : Name(name), BitmapArray(0), BitmapSizes(0), Count(0), Lock(name) {
      } /* endmethod */

      ~MenuBitmapBuilder ( ) {
         Reset ( ) ;
      } /* endmethod */

      void Reset ( ) {
         Lock.Request ( ) ;
         for ( int i=0; i<Count; i++ )
            if ( BitmapArray[i] )
               GpiDeleteBitmap ( BitmapArray[i] ) ;
         Count = 0 ;
         free ( BitmapArray ) ; BitmapArray = 0 ;
         free ( BitmapSizes ) ; BitmapSizes = 0 ;
         Lock.Release ( ) ;
      } /* endmethod */
} ;


  // Function Prototypes

static HBITMAP CreateMenuBitmap (
   WorkSpace &PS,
   PSZ MenuText,
   PSZ Font,
   USHORT PointSize,
   BOOL Bold,
   BOOL Italic,
   BOOL Underscore,
   BOOL Strikeout,
   BOOL Outline,
   BOOL Caps,
   ULONG Expand
) ;

static int _Optlink CompareFonts ( const void *Item1, const void *Item2 ) ;
static int _Optlink CompareUSHORTs ( const void *Item1, const void *Item2 ) ;
static int _Optlink CompareCOLORs ( const void *Item1, const void *Item2 ) ;
static int _Optlink CompareULONGs ( const void *Item1, const void *Item2 ) ;


  // Global Data (private to this module)

static BOOL Active = TRUE ;

 
/****************************************************************************
 ****************************************************************************
 ***                                                                      ***
 ***                Font Menu Management Functions & Data                 ***
 ***                                                                      ***
 ****************************************************************************
 ****************************************************************************/

typedef struct {
   BOOL          Threaded ;
   HMODULE       Library ;
   HWND          WindowToNotify ;
} FONTINIT_PARMS, *PFONTINIT_PARMS ;

static VOID FontInitThread ( PVOID Parameter ) ;

static BOOL UpdateFontBitmaps (
   HAB Anchor,
   USHORT OldCount,
   FONTMETRICS OldFonts[],
   USHORT NewCount,
   FONTMETRICS NewFonts[],
   HBITMAP Bitmaps[],
   SIZEL BitmapSizes[]
) ;

static PHBITMAP     FontBitmaps = 0 ;
static PSIZEL       FontBitmapSizes = 0 ;
static USHORT       FontCount = 0 ;
static Event        FontDone ( "MenuBmp-FontDone" ) ;
static PFONTMETRICS FontMetrics = 0 ;
static Mutex        FontSemaphore ( "MenuBmp-FontSemaphore" ) ;
static TID          FontThread = 0 ;

 
/****************************************************************************
 *                                                                          *
 *  Start thread to initialize font bitmaps.                                *
 *                                                                          *
 ****************************************************************************/

extern void InitFontBitmaps (
   HMODULE      Library,        // -> Resource Library
   HWND         WindowToNotify  // -> Window to notify upon completion.
) {

  /**************************************************************************
   * Wait for previous initialization to complete before starting it anew.  *
   **************************************************************************/

   if ( long(FontThread) > 0 ) {
      DosSetPriority ( PRTYS_THREAD, PRTYC_FOREGROUNDSERVER, +20, FontThread ) ;
      FontDone.Wait() ;
   } /* endif */

  /**************************************************************************
   * Allocate and initialize the thread parameters.                         *
   **************************************************************************/

   PFONTINIT_PARMS Parms = PFONTINIT_PARMS ( malloc ( sizeof(FONTINIT_PARMS) ) ) ;
   Parms->Threaded = TRUE ;
   Parms->Library = Library ;
   Parms->WindowToNotify = WindowToNotify ;

  /**************************************************************************
   * Start the initialization thread.                                       *
   **************************************************************************/

   #ifdef THREADED
      Sys_SendMessage ( WindowToNotify, WM_THREAD_STARTED, 0, 0 ) ;
      FontDone.Reset ( ) ;
      FontThread = StartThread ( "FontInitThread", FontInitThread, 0x10000, Parms, PRTYC_IDLETIME, +20 ) ;
   #else
      Parms->Threaded = FALSE ;
      Parms->WindowToNotify = 0 ;
      FontThread = 0 ;
      FontInitThread ( Parms ) ;
   #endif
}
 
/****************************************************************************
 *                                                                          *
 *  Initialize Font Menu                                                    *
 *                                                                          *
 ****************************************************************************/

extern void InitFontMenu (
   HWND          Menu,           // -> Menu
   Estado       &State,          // -> Current State
   HMODULE       Library,        // -> Resource Library
   int           FirstEntry      // Offset of first entry to display.
) {

  /**************************************************************************
   * Set the hourglass for a bit.                                           *
   **************************************************************************/

   HourGlass Hold ( Menu ) ;

  /**************************************************************************
   * Wait for any font list update in progress to complete.                 *
   **************************************************************************/

   if ( long(FontThread) > 0 ) {
      DosSetPriority ( PRTYS_THREAD, PRTYC_FOREGROUNDSERVER, +20, FontThread ) ;
      FontDone.Wait() ;
   } /* endif */

  /**************************************************************************
   * Update it again, since we don't know how long ago the update was.      *
   **************************************************************************/

   PFONTINIT_PARMS Parms = PFONTINIT_PARMS ( malloc ( sizeof(FONTINIT_PARMS) ) ) ;
   Parms->Threaded = FALSE ;
   Parms->Library = Library ;
   Parms->WindowToNotify = 0 ;
   FontInitThread ( Parms ) ;

  /**************************************************************************
   * Clear out the old menu.                                                *
   **************************************************************************/

   LONG OldCount = LONGFROMMR ( Sys_SendMessage ( Menu, MM_QUERYITEMCOUNT, 0, 0 ) ) ;
   for ( int i=0; i<OldCount; i++ ) {
      LONG Id = LONGFROMMR ( Sys_SendMessage ( Menu, MM_ITEMIDFROMPOSITION, MPFROMSHORT(0), 0 ) ) ;
      Sys_SendMessage ( Menu, MM_REMOVEITEM, MPFROM2SHORT(Id,TRUE), 0 ) ;
   } /* endfor */

  /**************************************************************************
   * Initialize the menu item structure.                                    *
   **************************************************************************/

   MENUITEM MenuItem ;
   MenuItem.iPosition = MIT_END ;
   MenuItem.afAttribute = 0 ;
   MenuItem.hwndSubMenu = 0 ;
   MenuItem.id = USHORT ( IDM_FONTS + FirstEntry ) ;

  /**************************************************************************
   * Determine the maximum menu height & width.                             *
   **************************************************************************/

   RECTL Desktop ;
   WinQueryWindowRect ( HWND_DESKTOP, &Desktop ) ;

   SWP Position ;
   WinQueryWindowPos ( OWNER(Menu), &Position ) ;

   POINTL Points[2] ;
   Points[0].x = Points[1].x = Position.x ;
   Points[0].y = Position.y ;  Points[1].y = Position.y + Position.cy ;
   WinMapWindowPoints ( PARENT(OWNER(Menu)), HWND_DESKTOP, PPOINTL(&Points), 2 ) ;

   SIZEL MaxSize ;
   MaxSize.cx = Desktop.xRight - Desktop.xLeft ;
   MaxSize.cy = max ( Desktop.yTop-Points[1].y, Points[0].y-Desktop.yBottom ) ;

  /**************************************************************************
   * Determine all the bitmap sizes.                                        *
   **************************************************************************/

   int ColumnStart = FirstEntry ;
   LONG Left = 0 ;
   LONG Width = 0 ;
   LONG Bottom = MaxSize.cy - ( FirstEntry ? 50 : 0 ) ;

   for ( i=FirstEntry; i<FontCount; i++ ) {
      Bottom -= FontBitmapSizes[i].cy + 6 ;
      if ( Bottom < 0 ) {
         ColumnStart = i ;
         Left += Width ;
         Width = 0 ;
         Bottom = MaxSize.cy - ( FontBitmapSizes[i].cy + 6 ) ;
      } /* endif */
      Width = max ( Width, FontBitmapSizes[i].cx + 50 ) ;
      if ( Width > MaxSize.cx-Left ) {
         i = ColumnStart ;
         break ;
      } /* endif */
   } /* endfor */

   int LastEntry = i - 1 ;

  /**************************************************************************
   * If we aren't showing the start of the list, prepend a page-up entry.   *
   **************************************************************************/

   if ( FirstEntry ) {
      ResourceString More ( Library, IDS_MORE_PGUP ) ;
      MENUITEM MenuItem ;
      MenuItem.iPosition = MIT_END ;
      MenuItem.afAttribute = 0 ;
      MenuItem.hwndSubMenu = 0 ;
      MenuItem.id = IDM_FONTS_PGUP ;
      MenuItem.afStyle = MIS_TEXT ;
      Sys_SendMessage ( Menu, MM_INSERTITEM, MPFROMP(&MenuItem), MPFROMP(PSZ(More)) ) ;
   } /* endif */

  /**************************************************************************
   * Build the menu entries.                                                *
   **************************************************************************/

   Left = 0 ;
   Width = 0 ;
   Bottom = MaxSize.cy - ( FirstEntry ? 50 : 0 ) ;

   for ( i=FirstEntry; i<=LastEntry; i++ ) {

     /***********************************************************************
      * Set the item to break the menu into columns if necessary.           *
      ***********************************************************************/

      MenuItem.afStyle = MIS_BITMAP ;
      Bottom -= FontBitmapSizes[i].cy + 6 ;
      if ( Bottom < 0 ) {
         MenuItem.afStyle |= MIS_BREAKSEPARATOR ;
         Bottom = MaxSize.cy - ( FontBitmapSizes[i].cy + 6 ) ;
      } /* endif */

     /***********************************************************************
      * Set the normal bitmap.                                              *
      ***********************************************************************/

      MenuItem.hItem = ULONG ( FontBitmaps[i] ) ;

     /***********************************************************************
      * Add the menu entry.                                                 *
      ***********************************************************************/

      Sys_SendMessage ( Menu, MM_INSERTITEM, MPFROMP(&MenuItem), MPFROMP("") ) ;

     /***********************************************************************
      * If this is the current font of the active window, check it.         *
      ***********************************************************************/

      if ( !strcmp ( PCHAR(State.Font), PCHAR(FontMetrics[i].szFacename) ) ) {
         Sys_SendMessage ( Menu, MM_SETITEMATTR, MPFROM2SHORT(MenuItem.id,TRUE), MPFROM2SHORT(MIA_CHECKED,MIA_CHECKED) ) ;
      } /* endif */

     /***********************************************************************
      * Get the next menu item ID.                                          *
      ***********************************************************************/

      MenuItem.id ++ ;

   } /* endfor */

  /**************************************************************************
   * If we didn't show the end of the list, append a page-down entry.       *
   **************************************************************************/

   if ( LastEntry < FontCount-1 ) {
      ResourceString More ( Library, IDS_MORE_PGDN ) ;
      MENUITEM MenuItem ;
      MenuItem.iPosition = MIT_END ;
      MenuItem.afAttribute = 0 ;
      MenuItem.hwndSubMenu = 0 ;
      MenuItem.id = IDM_FONTS_PGDN ;
      MenuItem.afStyle = MIS_TEXT ;
      Sys_SendMessage ( Menu, MM_INSERTITEM, MPFROMP(&MenuItem), MPFROMP(PSZ(More)) ) ;
   } /* endif */

  /**************************************************************************
   * In any case, add on a separator and the menu manager.                  *
   **************************************************************************/

#if 0
   #ifdef MANAGERS
   MenuItem.iPosition = MIT_END ;
   MenuItem.afAttribute = 0 ;
   MenuItem.hwndSubMenu = 0 ;
   MenuItem.id = 0 ;
   MenuItem.afStyle = MIS_SEPARATOR ;
   MenuItem.hItem = 0 ;
   Sys_SendMessage ( Menu, MM_INSERTITEM, MPFROMP(&MenuItem), MPFROMP("") ) ;

   ResourceString Manager ( Library, IDS_MANAGER ) ;
   MenuItem.iPosition = MIT_END ;
   MenuItem.afAttribute = 0 ;
   MenuItem.hwndSubMenu = 0 ;
   MenuItem.id = IDM_FONTMGR ;
   MenuItem.afStyle = MIS_TEXT ;
   MenuItem.hItem = 0 ;
   Sys_SendMessage ( Menu, MM_INSERTITEM, MPFROMP(&MenuItem), MPFROMP(PSZ(Manager)) ) ;
   #endif
#endif
}
 
/****************************************************************************
 *                                                                          *
 *      Font Menu Initialization Thread                                     *
 *                                                                          *
 ****************************************************************************/

static VOID FontInitThread ( PVOID Parameter ) {

   #ifdef DEBUG
      Log ( "Font Init thread started." ) ;
   #endif

  /**************************************************************************
   * Get parameters.                                                        *
   **************************************************************************/

   PFONTINIT_PARMS Parms = PFONTINIT_PARMS ( Parameter ) ;

  /**************************************************************************
   * Set up for GPI calls.                                                  *
   **************************************************************************/

   Process *Proc = 0 ;
   if ( Parms->Threaded ) 
      Proc = new Process ( "FontInitThread", 0, LibraryHandle ) ;

   WorkSpace *PS = new WorkSpace ( "FontInitThread", 0, Proc ? Proc->QueryAnchor() : 0, FALSE, 8500, 11000 ) ;

  /**************************************************************************
   * Get the new font list.                                                 *
   **************************************************************************/

   LONG Count = PS->QueryFontCount ( ) ;

   PFONTMETRICS Metrics = 0 ;
   if ( Count ) {
      Metrics = PFONTMETRICS ( malloc ( sizeof(FONTMETRICS) * Count ) ) ;

      PS->QueryFonts ( Count, Metrics ) ;

      qsort ( Metrics, (size_t)Count, sizeof(FONTMETRICS), CompareFonts ) ;

      for ( int i=0, j=0; i<Count; i++ ) {
         if ( Metrics[i].fsDefn & FM_DEFN_OUTLINE ) {
            if ( i != j ) {
               Metrics[j] = Metrics[i] ;
            } /* endif */
            j ++ ;
         } else {
            #ifdef DEBUG_QUERYFONTS
               Log ( "FontInitThread: Discarding font '%s' because not an outline font.", Metrics[i].szFacename ) ;
            #endif // DEBUG_QUERYFONTS
         } /* endif */
      } /* endfor */
      Count = j ;

      ResourceString Bold ( Parms->Library, IDS_BOLD ) ;
      ResourceString Italic ( Parms->Library, IDS_ITALIC ) ;
      for ( i=0, j=0; i<Count; i++ ) {
         char Facename [FACESIZE+1] ;
         strcpy ( Facename, PCHAR(Metrics[i].szFacename) ) ;

         char *p = strstr ( Facename, PCHAR(Bold) ) ;
         if ( p ) {
            char *p2 = p + strlen(PCHAR(Bold)) ;
            while ( *p2 ) {
               *p++ = *p2++ ;
            } /* endwhile */
            *p = 0 ;
         } /* endif */

         p = strstr ( Facename, PCHAR(Italic) ) ;
         if ( p ) {
            char *p2 = p + strlen(PCHAR(Italic)) ;
            while ( *p2 ) {
               *p++ = *p2++ ;
            } /* endwhile */
            *p = 0 ;
         } /* endif */

         if ( ( j == 0 ) OR strcmp ( Facename, Metrics[j-1].szFacename ) ) {
            if ( i != j ) {
               Metrics[j] = Metrics[i] ;
            } /* endif */
            j ++ ;
         } else {
            #ifdef DEBUG_QUERYFONTS
               Log ( "FontInitThread: Discarding font '%s' because not a base font.", Metrics[i].szFacename ) ;
            #endif // DEBUG_QUERYFONTS
         } /* endif */
      } /* endfor */
      Count = j ;
   } /* endif */

   delete PS ;

  /**************************************************************************
   * Expand the bitmap handle array, if necessary.                          *
   **************************************************************************/

   if ( Count > FontCount ) {
      PHBITMAP Bitmaps = PHBITMAP ( malloc ( Count * sizeof(HBITMAP) ) ) ;
      memset ( Bitmaps, 0, size_t(Count*sizeof(HBITMAP)) ) ;
      if ( FontBitmaps ) {
         memcpy ( Bitmaps, FontBitmaps, FontCount * sizeof(HBITMAP) ) ;
         free ( FontBitmaps ) ;
      } /* endif */
      FontBitmaps = Bitmaps ;
      PSIZEL BitmapSizes = PSIZEL ( malloc ( Count * sizeof(SIZEL) ) ) ;
      memset ( BitmapSizes, 0, size_t(Count*sizeof(SIZEL)) ) ;
      if ( FontBitmapSizes ) {
         memcpy ( BitmapSizes, FontBitmapSizes, FontCount * sizeof(SIZEL) ) ;
         free ( FontBitmapSizes ) ;
      } /* endif */
      FontBitmapSizes = BitmapSizes ;
   } /* endif */

  /**************************************************************************
   * Update the bitmaps.                                                    *
   **************************************************************************/

   UpdateFontBitmaps ( Proc ? Proc->QueryAnchor() : 0, FontCount, FontMetrics, USHORT(Count), Metrics, FontBitmaps, FontBitmapSizes ) ;

  /**************************************************************************
   * Save the new font list and count.                                      *
   **************************************************************************/

   if ( FontMetrics )
      free ( FontMetrics ) ;

   FontMetrics = Metrics ;
   FontCount = USHORT ( Count ) ;

  /**************************************************************************
   * Notify the window that wanted to know when the thread was finished.    *
   **************************************************************************/

   if ( Parms->WindowToNotify )
      if ( !Sys_PostMessage ( Parms->WindowToNotify, WM_THREAD_DONE, MPFROMLONG(Sys_ThreadNumber(*_threadid)), 0 ) ) {
         ERRORID Code = Sys_GetLastError ( Proc ? Proc->QueryAnchor() : 0 ) ;
         Log ( "WARNING: Attempt to notify window of font init thread completion failed.  Error %lX.", Code ) ;
      } /* endif */

  /**************************************************************************
   * Release PM resources.                                                  *
   **************************************************************************/

   if ( Parms->Threaded ) 
      delete Proc ;

  /**************************************************************************
   * Release the parameter memory.                                          *
   **************************************************************************/

   free ( Parms ) ;

  /**************************************************************************
   * We're done.                                                            *
   **************************************************************************/

   FontDone.Post() ;
   FontThread = 0 ;

   #ifdef DEBUG
      Log ( "Font Init thread done." ) ;
   #endif
}
 
/****************************************************************************
 *                                                                          *
 *  Update Font Bitmaps                                                     *
 *                                                                          *
 ****************************************************************************/

static BOOL UpdateFontBitmaps (
   HAB Anchor,
   USHORT OldCount,
   FONTMETRICS OldFonts[],
   USHORT NewCount,
   FONTMETRICS NewFonts[],
   HBITMAP Bitmaps[],
   SIZEL BitmapSizes[]
) {

  /**************************************************************************
   * Reconcile the new fonts against the old, deleting obsolete bitmaps.    *
   **************************************************************************/

   qsort ( NewFonts, (size_t)NewCount, sizeof(NewFonts[0]), CompareFonts ) ;

   PHBITMAP OldBitmaps = 0 ;
   if ( OldCount ) {
      OldBitmaps = PHBITMAP ( malloc ( sizeof(Bitmaps[0]) * OldCount ) ) ;
      memcpy ( OldBitmaps, Bitmaps, sizeof(Bitmaps[0])*OldCount ) ;
   } /* endif */

   memset ( Bitmaps, 0, sizeof(Bitmaps[0])*NewCount ) ;

   BOOL AlteredList = FALSE ;
   BOOL NeedNewBitmaps = FALSE ;

   int i=0, j=0;
   while ( i<OldCount OR j<NewCount ) {
      if ( i>=NewCount ) {
         if ( OldBitmaps[i] )
            GpiDeleteBitmap ( OldBitmaps[i] ) ;
         AlteredList = TRUE ;
         i ++ ;
      } else if ( j>=OldCount ) {
         AlteredList = TRUE ;
         NeedNewBitmaps = TRUE ;
         j ++ ;
      } else {
         int Compare = CompareFonts ( &OldFonts[i], &NewFonts[j] ) ;
         if ( Compare == 0 ) {
            if ( OldBitmaps[i] )
               Bitmaps[j] = OldBitmaps[i] ;
            else
               NeedNewBitmaps = TRUE ;
            i++, j++;
         } else if ( Compare < 0 ) {
            if ( OldBitmaps[i] )
               GpiDeleteBitmap ( OldBitmaps[i] ) ;
            AlteredList = TRUE ;
            i ++ ;
         } else {
            AlteredList = TRUE ;
            NeedNewBitmaps = TRUE ;
            j ++ ;
         } /* endif */
      } /* endif */
   } /* endwhile */

   if ( OldBitmaps )
      free ( OldBitmaps ) ;

   if ( NOT NeedNewBitmaps )
      return ( AlteredList ) ;

  /**************************************************************************
   * Open a memory device context.                                          *
   **************************************************************************/

   DeviceContext Memory = DeviceContext ( "UpdateFontBitmaps", long(Anchor) ) ;

  /**************************************************************************
   * Create a memory presentation space for painting bitmaps.               *
   **************************************************************************/

   WorkSpace PS = WorkSpace ( "UpdateFontBitmaps", 0, &Memory, Anchor, FALSE, TRUE ) ;

  /**************************************************************************
   * Redraw the bitmaps.                                                    *
   **************************************************************************/

   for ( i=0; i<NewCount && Active; i++ ) {

     /***********************************************************************
      * If new bitmap not needed, skip this one.                            *
      ***********************************************************************/

      if ( Bitmaps[i] )
         continue ;

     /***********************************************************************
      * Create the bitmap.                                                  *
      ***********************************************************************/

      Bitmaps[i] = CreateMenuBitmap ( PS, PSZ(NewFonts[i].szFacename),
         PSZ(NewFonts[i].szFacename), 180,
         FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, 100 ) ;

     /***********************************************************************
      * Measure the bitmap.                                                 *
      ***********************************************************************/

      BITMAPINFOHEADER2 BitmapInfoHeader ;
      BitmapInfoHeader.cbFix = sizeof(BitmapInfoHeader) ;
      GpiQueryBitmapInfoHeader ( Bitmaps[i], &BitmapInfoHeader ) ;
      BitmapSizes[i].cx = BitmapInfoHeader.cx ;
      BitmapSizes[i].cy = BitmapInfoHeader.cy ;

   } /* endfor */

   return ( TRUE ) ;
}
 
/****************************************************************************
 *                                                                          *
 *  Return address of Font Metrics array.                                   *
 *                                                                          *
 ****************************************************************************/

extern PFONTMETRICS QueryFontMetrics ( void ) {
   return ( FontMetrics ) ;
}



 
/****************************************************************************
 ****************************************************************************
 ***                                                                      ***
 ***                Size Menu Management Functions & Data                 ***
 ***                                                                      ***
 ****************************************************************************
 ****************************************************************************/

typedef struct {
   BOOL      Threaded ;
   HMODULE   Library ;
   HWND      WindowToNotify ;
   PSZ       NewFont ;
   USHORT    NewCount ;
   PUSHORT   NewSizes ;
} SIZEINIT_PARMS, *PSIZEINIT_PARMS ;

static VOID SizeInitThread ( PVOID Parameter ) ;

static BOOL UpdateSizeBitmaps (
   HAB Anchor,
   HMODULE Library,
   PSZ OldFont,
   PSZ NewFont,
   USHORT OldCount,
   USHORT OldSizes[],
   USHORT NewCount,
   USHORT NewSizes[],
   HBITMAP Bitmaps[],
   SIZEL BitmapSizes[]
) ;

static PHBITMAP     SizeBitmaps = 0 ;
static PSIZEL       SizeBitmapSizes = 0 ;
static USHORT       SizeCount = 0 ;
static Event        SizeDone ( "MenuBmp-SizeDone" ) ;;
static PSZ          SizeFont = 0 ;
static PUSHORT      Sizes = 0 ;
static Mutex        SizeSemaphore ( "MenuBmp-SizeSemaphore" ) ;
static TID          SizeThread = 0 ;

 
/****************************************************************************
 *                                                                          *
 *  Start thread to initialize size bitmaps.                                *
 *                                                                          *
 ****************************************************************************/

extern void InitSizeBitmaps (
   HMODULE      Library,        // -> Resource Library
   HWND         WindowToNotify, // -> Window to notify upon completion.
   PSZ          NewFont,        // -> New Font
   USHORT       NewCount,       // -> New Size Count
   PUSHORT      NewSizes        // -> New Sizes
) {

  /**************************************************************************
   * Wait for previous initialization to complete before starting it anew.  *
   **************************************************************************/

   if ( long(SizeThread) > 0 ) {
      DosSetPriority ( PRTYS_THREAD, PRTYC_FOREGROUNDSERVER, +20, SizeThread ) ;
      SizeDone.Wait() ;
   } /* endif */

  /**************************************************************************
   * Allocate and initialize the thread parameters.                         *
   **************************************************************************/

   PSIZEINIT_PARMS Parms = PSIZEINIT_PARMS ( malloc ( sizeof(SIZEINIT_PARMS) ) ) ;
   Parms->Threaded = TRUE ;
   Parms->Library = Library ;
   Parms->WindowToNotify = WindowToNotify ;
   Parms->NewFont = NewFont ;
   Parms->NewCount = NewCount ;
   Parms->NewSizes = NewSizes ;

  /**************************************************************************
   * Start the initialization thread.                                       *
   **************************************************************************/

   #ifdef THREADED
      Sys_SendMessage ( WindowToNotify, WM_THREAD_STARTED, 0, 0 ) ;
      SizeDone.Reset ( ) ;
      SizeThread = StartThread ( "SizeInitThread", SizeInitThread, 0x10000, Parms, PRTYC_IDLETIME, +20 ) ;
   #else
      Parms->Threaded = FALSE ;
      Parms->WindowToNotify = 0 ;
      SizeThread = 0 ;
      SizeInitThread ( Parms ) ;
   #endif
}
 
/****************************************************************************
 *                                                                          *
 *  Initialize the Size Menu                                                *
 *                                                                          *
 ****************************************************************************/

extern void InitSizeMenu (
   HWND         Menu,           // -> Menu
   Estado      &State,          // -> Current State
   HMODULE      Library,        // -> Resource Library
   int          FirstEntry,     // Offset of first entry to display.
   USHORT       NewCount,       // -> New Size Count
   PUSHORT      NewSizes        // -> New Sizes (or NULL to use old ones)
) {

  /**************************************************************************
   * Set the hourglass for a bit.                                           *
   **************************************************************************/

   HourGlass Hold ( Menu ) ;

  /**************************************************************************
   * Wait for any size list update in progress to complete.                 *
   **************************************************************************/

   if ( long(SizeThread) > 0 ) {
      DosSetPriority ( PRTYS_THREAD, PRTYC_FOREGROUNDSERVER, +20, SizeThread ) ;
      SizeDone.Wait() ;
   } /* endif */

  /**************************************************************************
   * Update it again, since we don't know how long ago the update was.      *
   **************************************************************************/

   PSIZEINIT_PARMS Parms = PSIZEINIT_PARMS ( malloc ( sizeof(SIZEINIT_PARMS) ) ) ;
   Parms->Threaded = FALSE ;
   Parms->Library = Library ;
   Parms->WindowToNotify = 0 ;
   Parms->NewFont = PSZ(State.Font) ;
   Parms->NewCount = NewSizes ? NewCount : SizeCount ;
   Parms->NewSizes = NewSizes ? NewSizes : Sizes ;
   SizeInitThread ( Parms ) ;

  /**************************************************************************
   * Clear out the old menu.                                                *
   **************************************************************************/

   LONG OldCount = LONGFROMMR ( Sys_SendMessage ( Menu, MM_QUERYITEMCOUNT, 0, 0 ) ) ;
   for ( int i=0; i<OldCount; i++ ) {
      LONG Id = LONGFROMMR ( Sys_SendMessage ( Menu, MM_ITEMIDFROMPOSITION, MPFROMSHORT(0), 0 ) ) ;
      Sys_SendMessage ( Menu, MM_REMOVEITEM, MPFROM2SHORT(Id,TRUE), 0 ) ;
   } /* endfor */

  /**************************************************************************
   * Initialize the menu item structure.                                    *
   **************************************************************************/

   MENUITEM MenuItem ;
   MenuItem.iPosition = MIT_END ;
   MenuItem.afAttribute = 0 ;
   MenuItem.hwndSubMenu = 0 ;
   MenuItem.id = USHORT ( IDM_SIZES + FirstEntry ) ;

  /**************************************************************************
   * Determine the maximum menu height & width.                             *
   **************************************************************************/

   RECTL Desktop ;
   WinQueryWindowRect ( HWND_DESKTOP, &Desktop ) ;

   SWP Position ;
   WinQueryWindowPos ( OWNER(Menu), &Position ) ;

   POINTL Points[2] ;
   Points[0].x = Points[1].x = Position.x ;
   Points[0].y = Position.y ;  Points[1].y = Position.y + Position.cy ;
   WinMapWindowPoints ( PARENT(OWNER(Menu)), HWND_DESKTOP, PPOINTL(&Points), 2 ) ;

   SIZEL MaxSize ;
   MaxSize.cx = Desktop.xRight - Desktop.xLeft ;
   MaxSize.cy = max ( Desktop.yTop-Points[1].y, Points[0].y-Desktop.yBottom ) ;

  /**************************************************************************
   * Determine all the bitmap sizes.                                        *
   **************************************************************************/

   int ColumnStart = FirstEntry ;
   LONG Left = 0 ;
   LONG Width = 0 ;
   LONG Bottom = MaxSize.cy - ( FirstEntry ? 50 : 0 ) ;

   for ( i=FirstEntry; i<SizeCount; i++ ) {
      Bottom -= SizeBitmapSizes[i].cy + 6 ;
      if ( Bottom < 0 ) {
         ColumnStart = i ;
         Left += Width ;
         Width = 0 ;
         Bottom = MaxSize.cy - ( SizeBitmapSizes[i].cy + 6 ) ;
      } /* endif */
      Width = max ( Width, SizeBitmapSizes[i].cx + 50 ) ;
      if ( Width > MaxSize.cx-Left ) {
         i = ColumnStart ;
         break ;
      } /* endif */
   } /* endfor */

   int LastEntry = i - 1 ;

  /**************************************************************************
   * If we aren't showing the start of the list, prepend a page-up entry.   *
   **************************************************************************/

   if ( FirstEntry ) {
      ResourceString More ( Library, IDS_MORE_PGUP ) ;
      MENUITEM MenuItem ;
      MenuItem.iPosition = MIT_END ;
      MenuItem.afAttribute = 0 ;
      MenuItem.hwndSubMenu = 0 ;
      MenuItem.id = IDM_SIZES_PGUP ;
      MenuItem.afStyle = MIS_TEXT ;
      Sys_SendMessage ( Menu, MM_INSERTITEM, MPFROMP(&MenuItem), MPFROMP(PSZ(More)) ) ;
   } /* endif */

  /**************************************************************************
   * Build the menu entries.                                                *
   **************************************************************************/

   Left = 0 ;
   Width = 0 ;
   Bottom = MaxSize.cy - ( FirstEntry ? 50 : 0 ) ;

   for ( i=FirstEntry; i<=LastEntry; i++ ) {

     /***********************************************************************
      * Set the item to break the menu into columns if necessary.           *
      ***********************************************************************/

      MenuItem.afStyle = MIS_BITMAP ;
      Bottom -= SizeBitmapSizes[i].cy + 6 ;
      if ( Bottom < 0 ) {
         MenuItem.afStyle |= MIS_BREAKSEPARATOR ;
         Bottom = MaxSize.cy - ( SizeBitmapSizes[i].cy + 6 ) ;
      } /* endif */

     /***********************************************************************
      * Set the normal bitmap.                                              *
      ***********************************************************************/

      MenuItem.hItem = ULONG ( SizeBitmaps[i] ) ;

     /***********************************************************************
      * Add the menu entry.                                                 *
      ***********************************************************************/

      Sys_SendMessage ( Menu, MM_INSERTITEM, MPFROMP(&MenuItem), MPFROMP("") ) ;

     /***********************************************************************
      * If this is the current size of the active window, check it.         *
      ***********************************************************************/

      if ( Sizes[i] == State.Size ) {
         Sys_SendMessage ( Menu, MM_SETITEMATTR, MPFROM2SHORT(MenuItem.id,TRUE), MPFROM2SHORT(MIA_CHECKED,MIA_CHECKED) ) ;
      }

     /***********************************************************************
      * Get the next menu item ID.                                          *
      ***********************************************************************/

      MenuItem.id ++ ;
   }

  /**************************************************************************
   * If we didn't show the end of the list, append a page-down entry.       *
   **************************************************************************/

   if ( LastEntry < SizeCount-1 ) {
      ResourceString More ( Library, IDS_MORE_PGDN ) ;
      MENUITEM MenuItem ;
      MenuItem.iPosition = MIT_END ;
      MenuItem.afAttribute = 0 ;
      MenuItem.hwndSubMenu = 0 ;
      MenuItem.id = IDM_SIZES_PGDN ;
      MenuItem.afStyle = MIS_TEXT ;
      Sys_SendMessage ( Menu, MM_INSERTITEM, MPFROMP(&MenuItem), MPFROMP(PSZ(More)) ) ;
   } /* endif */

  /**************************************************************************
   * In any case, add on a separator and the menu manager.                  *
   **************************************************************************/

#ifdef MANAGERS
   MenuItem.iPosition = MIT_END ;
   MenuItem.afAttribute = 0 ;
   MenuItem.hwndSubMenu = 0 ;
   MenuItem.id = 0 ;
   MenuItem.afStyle = MIS_SEPARATOR ;
   MenuItem.hItem = 0 ;
   Sys_SendMessage ( Menu, MM_INSERTITEM, MPFROMP(&MenuItem), MPFROMP("") ) ;

   ResourceString Manager ( Library, IDS_MANAGER ) ;
   MenuItem.iPosition = MIT_END ;
   MenuItem.afAttribute = 0 ;
   MenuItem.hwndSubMenu = 0 ;
   MenuItem.id = IDM_SIZEMGR ;
   MenuItem.afStyle = MIS_TEXT ;
   MenuItem.hItem = 0 ;
   Sys_SendMessage ( Menu, MM_INSERTITEM, MPFROMP(&MenuItem), MPFROMP(PSZ(Manager)) ) ;
#endif
}
 
/****************************************************************************
 *                                                                          *
 *      Size Menu Initialization Thread                                     *
 *                                                                          *
 ****************************************************************************/

static VOID SizeInitThread ( PVOID Parameter ) {

   #ifdef DEBUG
      Log ( "Size Init thread started." ) ;
   #endif

  /**************************************************************************
   * Get parameters.                                                        *
   **************************************************************************/

   PSIZEINIT_PARMS Parms = PSIZEINIT_PARMS ( Parameter ) ;

  /**************************************************************************
   * Set up for GPI calls.                                             h    *
   **************************************************************************/

   Process *Proc = 0 ;
   if ( Parms->Threaded ) 
      Proc = new Process ( "SizeInitThread", 0, LibraryHandle ) ;

  /**************************************************************************
   * Expand the bitmap handle array, if necessary.                          *
   **************************************************************************/

   if ( Parms->NewCount > SizeCount ) {
      PHBITMAP Bitmaps = PHBITMAP ( malloc ( Parms->NewCount * sizeof(HBITMAP) ) ) ;
      memset ( Bitmaps, 0, Parms->NewCount * sizeof(HBITMAP) ) ;
      if ( SizeBitmaps ) {
         memcpy ( Bitmaps, SizeBitmaps, SizeCount * sizeof(HBITMAP) ) ;
         free ( SizeBitmaps ) ;
      } /* endif */
      SizeBitmaps = Bitmaps ;
      PSIZEL BitmapSizes = PSIZEL ( malloc ( Parms->NewCount * sizeof(SIZEL) ) ) ;
      memset ( BitmapSizes, 0, Parms->NewCount * sizeof(SIZEL) ) ;
      if ( SizeBitmapSizes ) {
         memcpy ( BitmapSizes, SizeBitmapSizes, SizeCount * sizeof(SIZEL) ) ;
         free ( SizeBitmapSizes ) ;
      } /* endif */
      SizeBitmapSizes = BitmapSizes ;
   } /* endif */

  /**************************************************************************
   * Update the bitmaps.                                                    *
   **************************************************************************/

   UpdateSizeBitmaps ( Proc ? Proc->QueryAnchor() : 0, Parms->Library,
      SizeFont ? SizeFont : PSZ(""), Parms->NewFont, SizeCount, Sizes,
      Parms->NewCount, Parms->NewSizes, SizeBitmaps, SizeBitmapSizes ) ;

  /**************************************************************************
   * Save the last font for which the size menu was built.                  *
   **************************************************************************/

   if ( SizeFont )
      free ( SizeFont ) ;

   SizeFont = PSZ ( malloc ( strlen(PCHAR(Parms->NewFont))+1 ) ) ;
   strcpy ( PCHAR(SizeFont), PCHAR(Parms->NewFont) ) ;

   SizeCount = Parms->NewCount ;

   if ( Sizes != Parms->NewSizes ) {
      if ( Sizes )
         free ( Sizes ) ;
      Sizes = PUSHORT ( malloc ( SizeCount * sizeof(USHORT) ) ) ;
      memcpy ( Sizes, Parms->NewSizes, SizeCount * sizeof(USHORT) ) ;
   } /* endif */

  /**************************************************************************
   * Notify the window that wanted to know when the thread was finished.    *
   **************************************************************************/

   if ( Parms->WindowToNotify )
      if ( !Sys_PostMessage ( Parms->WindowToNotify, WM_THREAD_DONE, MPFROMLONG(Sys_ThreadNumber(*_threadid)), 0 ) ) {
         ERRORID Code = Sys_GetLastError ( Proc ? Proc->QueryAnchor() : 0 ) ;
         Log ( "WARNING: Attempt to notify window of size init thread completion failed.  Error %lX.", Code ) ;
      } /* endif */

  /**************************************************************************
   * Release PM resources.                                                  *
   **************************************************************************/

   if ( Parms->Threaded ) 
      delete Proc ;

  /**************************************************************************
   * Release the parameter memory.                                          *
   **************************************************************************/

   free ( Parms ) ;

  /**************************************************************************
   * We're done.                                                            *
   **************************************************************************/

   SizeDone.Post() ;
   SizeThread = 0 ;

   #ifdef DEBUG
      Log ( "Size Init thread done." ) ;
   #endif
}
 
/****************************************************************************
 *                                                                          *
 *  Update Size Bitmaps                                                     *
 *                                                                          *
 ****************************************************************************/

static BOOL UpdateSizeBitmaps (
   HAB Anchor,
   HMODULE Library,
   PSZ OldFont,
   PSZ NewFont,
   USHORT OldCount,
   USHORT OldSizes[],
   USHORT NewCount,
   USHORT NewSizes[],
   HBITMAP Bitmaps[],
   SIZEL BitmapSizes[]
) {

  /**************************************************************************
   * Reconcile the new sizes against the old, deleting obsolete bitmaps.    *
   **************************************************************************/

   qsort ( NewSizes, (size_t)NewCount, sizeof(NewSizes[0]), CompareUSHORTs ) ;

   PHBITMAP OldBitmaps = 0 ;
   if ( OldCount ) {
      OldBitmaps = PHBITMAP ( malloc ( sizeof(Bitmaps[0]) * OldCount ) ) ;
      memcpy ( OldBitmaps, Bitmaps, sizeof(Bitmaps[0])*OldCount ) ;
   } /* endif */

   memset ( Bitmaps, 0, sizeof(Bitmaps[0])*NewCount ) ;

   BOOL AlteredList = FALSE ;
   BOOL NeedNewBitmaps = FALSE ;

   int i=0, j=0;
   while ( i<OldCount OR j<NewCount ) {
      if ( i>=NewCount ) {
         if ( OldBitmaps[i] )
            GpiDeleteBitmap ( OldBitmaps[i] ) ;
         AlteredList = TRUE ;
         i ++ ;
      } else if ( j>=OldCount ) {
         AlteredList = TRUE ;
         NeedNewBitmaps = TRUE ;
         j ++ ;
      } else {
         int Compare = CompareUSHORTs ( &OldSizes[i], &NewSizes[j] ) ;
         if ( Compare == 0 ) {
            if ( OldBitmaps[i] )
               if ( strcmp ( (char*)OldFont, (char*)NewFont ) ) {
                  GpiDeleteBitmap ( OldBitmaps[i] ) ;
                  AlteredList = TRUE ;
                  NeedNewBitmaps = TRUE ;
               } else
                  Bitmaps[j] = OldBitmaps[i] ;
            else
               NeedNewBitmaps = TRUE ;
            i++, j++;
         } else if ( Compare < 0 ) {
            if ( OldBitmaps[i] )
               GpiDeleteBitmap ( OldBitmaps[i] ) ;
            AlteredList = TRUE ;
            i ++ ;
         } else {
            AlteredList = TRUE ;
            NeedNewBitmaps = TRUE ;
            j ++ ;
         } /* endif */
      } /* endif */
   } /* endwhile */

   if ( OldBitmaps )
      free ( OldBitmaps ) ;

   if ( NOT NeedNewBitmaps )
      return ( AlteredList ) ;

  /**************************************************************************
   * Open a memory device context.                                          *
   **************************************************************************/

   DeviceContext Memory = DeviceContext ( "UpdateSizeBitmaps", long(Anchor) ) ;

  /**************************************************************************
   * Create a memory presentation space for painting bitmaps.               *
   **************************************************************************/

   WorkSpace PS = WorkSpace ( "UpdateSizeBitmaps", 0, &Memory, Anchor, FALSE, TRUE ) ;

  /**************************************************************************
   * Redraw the bitmaps.                                                    *
   **************************************************************************/

   ResourceString Point ( Library, IDS_POINT ) ;
   for ( i=0; i<NewCount && Active; i++ ) {

     /***********************************************************************
      * If new bitmap not needed, skip this one.                            *
      ***********************************************************************/

      if ( Bitmaps[i] )
         continue ;

     /***********************************************************************
      * Create the menu item text.                                          *
      ***********************************************************************/

      UCHAR Text [80] ;
      sprintf ( PCHAR(Text), "%.1lf%s", (double)NewSizes[i]/10.0, PSZ(Point) ) ;

     /***********************************************************************
      * Create the bitmap.                                                  *
      ***********************************************************************/

      Bitmaps[i] = CreateMenuBitmap ( PS, PSZ(Text), NewFont, NewSizes[i],
         FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, 100 ) ;

     /***********************************************************************
      * Measure the bitmap.                                                 *
      ***********************************************************************/

      BITMAPINFOHEADER2 BitmapInfoHeader ;
      BitmapInfoHeader.cbFix = sizeof(BitmapInfoHeader) ;
      GpiQueryBitmapInfoHeader ( Bitmaps[i], &BitmapInfoHeader ) ;
      BitmapSizes[i].cx = BitmapInfoHeader.cx ;
      BitmapSizes[i].cy = BitmapInfoHeader.cy ;

   } /* endfor */

   return ( TRUE ) ;
}



 
/****************************************************************************
 ****************************************************************************
 ***                                                                      ***
 ***              Attribute Menu Management Functions & Data              ***
 ***                                                                      ***
 ****************************************************************************
 ****************************************************************************/

typedef struct {
   BOOL      Threaded ;
   HMODULE   Library ;
   HWND      WindowToNotify ;
   PSZ       NewFont ;
} ATTRINIT_PARMS, *PATTRINIT_PARMS ;

static VOID AttrInitThread ( PVOID Parameter ) ;

static BOOL UpdateAttrBitmaps (
   HAB Anchor,
   HMODULE Library,
   PSZ OldFont,
   PSZ NewFont,
   HBITMAP Bitmaps[]
) ;

#define ATTR_BITMAP_COUNT 8

static HBITMAP      AttrBitmaps [ATTR_BITMAP_COUNT] ;
static USHORT       AttrCount = ATTR_BITMAP_COUNT ;
static Event        AttrDone ( "MenuBmp-AttrDone" ) ;;
static PSZ          AttrFont = 0 ;
static Mutex        AttrSemaphore ( "MenuBmp-AttrSemaphore" ) ;
static TID          AttrThread = 0 ;

 
/****************************************************************************
 *                                                                          *
 *  Start thread to initialize attribute bitmaps.                           *
 *                                                                          *
 ****************************************************************************/

extern void InitAttrBitmaps (
   HMODULE      Library,        // -> Resource Library
   HWND         WindowToNotify, // -> Window to notify upon completion.
   PSZ          NewFont         // -> New Font
) {

  /**************************************************************************
   * Wait for previous initialization to complete before starting it anew.  *
   **************************************************************************/

   if ( long(AttrThread) > 0 ) {
      DosSetPriority ( PRTYS_THREAD, PRTYC_FOREGROUNDSERVER, +20, AttrThread ) ;
      AttrDone.Wait() ;
   } /* endif */

  /**************************************************************************
   * Allocate and initialize the thread parameters.                         *
   **************************************************************************/

   PATTRINIT_PARMS Parms = PATTRINIT_PARMS ( malloc ( sizeof(ATTRINIT_PARMS) ) ) ;
   Parms->Threaded = TRUE ;
   Parms->Library = Library ;
   Parms->WindowToNotify = WindowToNotify ;
   Parms->NewFont = NewFont ;

  /**************************************************************************
   * Start the initialization thread.                                       *
   **************************************************************************/

   #ifdef THREADED
      Sys_SendMessage ( WindowToNotify, WM_THREAD_STARTED, 0, 0 ) ;
      AttrDone.Reset ( ) ;
      AttrThread = StartThread ( "AttrInitThread", AttrInitThread, 0x10000, Parms, PRTYC_IDLETIME, +20 ) ;
   #else
      Parms->Threaded = FALSE ;
      Parms->WindowToNotify = 0 ;
      AttrThread = 0 ;
      AttrInitThread ( Parms ) ;
   #endif
}
 
/****************************************************************************
 *                                                                          *
 *  Initialize the Attribute Menu                                           *
 *                                                                          *
 ****************************************************************************/

extern void InitAttrMenu (
   HWND         Menu,           // -> Menu
   Estado      &State,          // -> Current State
   DocumentParms &DocParms,     // -> Document Parameters (Default State Info)
   HMODULE      Library         // -> Resource Library
) {

  /**************************************************************************
   * Set the hourglass for a bit.                                           *
   **************************************************************************/

   #ifdef DEBUG
      Log ( "InitAttrMenu: Started.  Setting hourglass pointer." ) ;
   #endif

   HourGlass Hold ( Menu ) ;

  /**************************************************************************
   * Wait for any attribute list update in progress to complete.            *
   **************************************************************************/

   if ( long(AttrThread) > 0 ) {
      #ifdef DEBUG
         Log ( "InitAttrMenu: Boosting priority of AttrInitThread." ) ;
      #endif
      DosSetPriority ( PRTYS_THREAD, PRTYC_FOREGROUNDSERVER, +20, AttrThread ) ;
      #ifdef DEBUG
         Log ( "InitAttrMenu: Waiting for AttrInitThread to complete." ) ;
      #endif
      AttrDone.Wait() ;
      #ifdef DEBUG
         Log ( "InitAttrMenu: AttrInitThread has completed." ) ;
      #endif
   } /* endif */

  /**************************************************************************
   * Update it again, since we don't know how long ago the update was.      *
   **************************************************************************/

   #ifdef DEBUG
      Log ( "InitAttrMenu: Executing AttrInitThread directly for good measure." ) ;
   #endif

   PATTRINIT_PARMS Parms = PATTRINIT_PARMS ( malloc ( sizeof(ATTRINIT_PARMS) ) ) ;
   Parms->Threaded = FALSE ;
   Parms->Library = Library ;
   Parms->WindowToNotify = 0 ;
   Parms->NewFont = PSZ(State.Font) ;
   AttrInitThread ( Parms ) ;

  /**************************************************************************
   * Update the menu.                                                       *
   **************************************************************************/

   #ifdef DEBUG
      Log ( "InitAttrMenu: Updating menu entry (Normal)." ) ;
   #endif

   Sys_SendMessage ( Menu, MM_SETITEMHANDLE,
      MPFROMSHORT(IDM_NORMAL), MPFROMLONG(AttrBitmaps[0]) ) ;
   Sys_SendMessage ( Menu, MM_SETITEMATTR, MPFROM2SHORT(IDM_NORMAL,TRUE),
      MPFROM2SHORT(MIA_CHECKED,( NOT State.Bold AND NOT State.Italic
      AND NOT State.Underscore AND NOT State.Strikeout
      AND NOT State.Outline )?MIA_CHECKED:0) ) ;

   #ifdef DEBUG
      Log ( "InitAttrMenu: Updating menu entry (Bold)." ) ;
   #endif

   Sys_SendMessage ( Menu, MM_SETITEMHANDLE,
      MPFROMSHORT(IDM_BOLD), MPFROMLONG(AttrBitmaps[1]) ) ;
   Sys_SendMessage ( Menu, MM_SETITEMATTR, MPFROM2SHORT(IDM_BOLD,TRUE),
      MPFROM2SHORT(MIA_CHECKED,State.Bold?MIA_CHECKED:0) ) ;

   #ifdef DEBUG
      Log ( "InitAttrMenu: Updating menu entry (Italic)." ) ;
   #endif

   Sys_SendMessage ( Menu, MM_SETITEMHANDLE,
      MPFROMSHORT(IDM_ITALIC), MPFROMLONG(AttrBitmaps[2]) ) ;
   Sys_SendMessage ( Menu, MM_SETITEMATTR, MPFROM2SHORT(IDM_ITALIC,TRUE),
      MPFROM2SHORT(MIA_CHECKED,State.Italic?MIA_CHECKED:0) ) ;

   #ifdef DEBUG
      Log ( "InitAttrMenu: Updating menu entry (Underscore)." ) ;
   #endif

   Sys_SendMessage ( Menu, MM_SETITEMHANDLE,
      MPFROMSHORT(IDM_UNDERSCORE), MPFROMLONG(AttrBitmaps[3]) ) ;
   Sys_SendMessage ( Menu, MM_SETITEMATTR, MPFROM2SHORT(IDM_UNDERSCORE,TRUE),
      MPFROM2SHORT(MIA_CHECKED,State.Underscore?MIA_CHECKED:0) ) ;

   #ifdef DEBUG
      Log ( "InitAttrMenu: Updating menu entry (Strikeout)." ) ;
   #endif

   Sys_SendMessage ( Menu, MM_SETITEMHANDLE,
      MPFROMSHORT(IDM_STRIKEOUT), MPFROMLONG(AttrBitmaps[4]) ) ;
   Sys_SendMessage ( Menu, MM_SETITEMATTR, MPFROM2SHORT(IDM_STRIKEOUT,TRUE),
      MPFROM2SHORT(MIA_CHECKED,State.Strikeout?MIA_CHECKED:0) ) ;

   #ifdef DEBUG
      Log ( "InitAttrMenu: Updating menu entry (Outline)." ) ;
   #endif

   Sys_SendMessage ( Menu, MM_SETITEMHANDLE,
      MPFROMSHORT(IDM_OUTLINE), MPFROMLONG(AttrBitmaps[5]) ) ;
   Sys_SendMessage ( Menu, MM_SETITEMATTR, MPFROM2SHORT(IDM_OUTLINE,TRUE),
      MPFROM2SHORT(MIA_CHECKED,State.Outline?MIA_CHECKED:0) ) ;

   #ifdef DEBUG
      Log ( "InitAttrMenu: Updating menu entry (Caps)." ) ;
   #endif

   Sys_SendMessage ( Menu, MM_SETITEMHANDLE,
      MPFROMSHORT(IDM_CAPS), MPFROMLONG(AttrBitmaps[6]) ) ;
   Sys_SendMessage ( Menu, MM_SETITEMATTR, MPFROM2SHORT(IDM_CAPS,TRUE),
      MPFROM2SHORT(MIA_CHECKED,State.Caps?MIA_CHECKED:0) ) ;

   #ifdef DEBUG
      Log ( "InitAttrMenu: Updating menu entry (Expand Menu)." ) ;
   #endif

   Sys_SendMessage ( Menu, MM_SETITEMHANDLE,
      MPFROMSHORT(IDM_EXPAND_MENU), MPFROMLONG(AttrBitmaps[7]) ) ;
   Sys_SendMessage ( Menu, MM_SETITEMATTR, MPFROM2SHORT(IDM_EXPAND_MENU,TRUE),
      MPFROM2SHORT(MIA_CHECKED,State.Expand!=DocParms.Expand?MIA_CHECKED:0) ) ;

   #ifdef DEBUG
      Log ( "InitAttrMenu: Updating menu entries (misc menus)." ) ;
   #endif

   Sys_SendMessage ( Menu, MM_SETITEMATTR, MPFROM2SHORT(IDM_LINECOLOR_MENU,TRUE),
      MPFROM2SHORT(MIA_CHECKED,State.Color!=DocParms.Color?MIA_CHECKED:0) ) ;
   Sys_SendMessage ( Menu, MM_SETITEMATTR, MPFROM2SHORT(IDM_LINEWIDTH_MENU,TRUE),
      MPFROM2SHORT(MIA_CHECKED,State.LineWidth!=DocParms.LineWidth?MIA_CHECKED:0) ) ;
   Sys_SendMessage ( Menu, MM_SETITEMATTR, MPFROM2SHORT(IDM_FILLCOLOR_MENU,TRUE),
      MPFROM2SHORT(MIA_CHECKED,State.FillColor!=DocParms.FillColor?MIA_CHECKED:0) ) ;
   Sys_SendMessage ( Menu, MM_SETITEMATTR, MPFROM2SHORT(IDM_FILLTYPE_MENU,TRUE),
      MPFROM2SHORT(MIA_CHECKED,State.FillType!=DocParms.FillType?MIA_CHECKED:0) ) ;
   Sys_SendMessage ( Menu, MM_SETITEMATTR, MPFROM2SHORT(IDM_LINEEND_MENU,TRUE),
      MPFROM2SHORT(MIA_CHECKED,State.LineEnd!=DocParms.LineEnd?MIA_CHECKED:0) ) ;
   Sys_SendMessage ( Menu, MM_SETITEMATTR, MPFROM2SHORT(IDM_LINEJOIN_MENU,TRUE),
      MPFROM2SHORT(MIA_CHECKED,State.LineJoin!=DocParms.LineJoin?MIA_CHECKED:0) ) ;

   #ifdef DEBUG
      Log ( "InitAttrMenu: Done." ) ;
   #endif

}
 
/****************************************************************************
 *                                                                          *
 *      Attribute Menu Initialization Thread                                *
 *                                                                          *
 ****************************************************************************/

static VOID AttrInitThread ( PVOID Parameter ) {

   #ifdef DEBUG
      Log ( "Attr Init thread started." ) ;
   #endif

  /**************************************************************************
   * Get parameters.                                                        *
   **************************************************************************/

   PATTRINIT_PARMS Parms = PATTRINIT_PARMS ( Parameter ) ;

  /**************************************************************************
   * Set up for GPI calls.                                                  *
   **************************************************************************/

   Process *Proc = 0 ;
   if ( Parms->Threaded ) 
      Proc = new Process ( "AttrInitThread", 0, LibraryHandle ) ;

  /**************************************************************************
   * Update the bitmaps.                                                    *
   **************************************************************************/

   UpdateAttrBitmaps ( Proc ? Proc->QueryAnchor() : 0, Parms->Library,
      AttrFont ? AttrFont : PSZ(""), Parms->NewFont, AttrBitmaps ) ;

  /**************************************************************************
   * Save the last font for which the attribute menu was built.             *
   **************************************************************************/

   if ( AttrFont )
      free ( AttrFont ) ;

   AttrFont = PSZ ( malloc ( strlen(PCHAR(Parms->NewFont))+1 ) ) ;
   strcpy ( PCHAR(AttrFont), PCHAR(Parms->NewFont) ) ;

  /**************************************************************************
   * Notify the window that wanted to know when the thread was finished.    *
   **************************************************************************/

   if ( Parms->WindowToNotify )
      if ( !Sys_PostMessage ( Parms->WindowToNotify, WM_THREAD_DONE, MPFROMLONG(Sys_ThreadNumber(*_threadid)), 0 ) ) {
         ERRORID Code = Sys_GetLastError ( Proc ? Proc->QueryAnchor() : 0 ) ;
         Log ( "WARNING: Attempt to notify window of attr init thread completion failed.  Error %lX.", Code ) ;
      } /* endif */

  /**************************************************************************
   * Release PM resources.                                                  *
   **************************************************************************/

   if ( Parms->Threaded ) 
      delete Proc ;

  /**************************************************************************
   * Release the parameter memory.                                          *
   **************************************************************************/

   free ( Parms ) ;

  /**************************************************************************
   * We're done.                                                            *
   **************************************************************************/

   AttrDone.Post() ;
   AttrThread = 0 ;

   #ifdef DEBUG
      Log ( "Attr Init thread done." ) ;
   #endif
}
 
/****************************************************************************
 *                                                                          *
 *  Update Attribute Bitmaps                                                *
 *                                                                          *
 ****************************************************************************/

static BOOL UpdateAttrBitmaps (
   HAB Anchor,
   HMODULE Library,
   PSZ OldFont,
   PSZ NewFont,
   HBITMAP Bitmaps[]
) {

  /**************************************************************************
   * Check to see if any bitmaps need updating.                             *
   **************************************************************************/

   BOOL NeedNewBitmaps = FALSE ;

   for ( int i=0; i<ATTR_BITMAP_COUNT; i++ ) {
      if ( Bitmaps[i] ) {
         if ( strcmp ( (char*)OldFont, (char*)NewFont ) ) {
            GpiDeleteBitmap ( Bitmaps[i] ) ;
            Bitmaps[i] = 0 ;
            NeedNewBitmaps = TRUE ;
         }
      } else {
         NeedNewBitmaps = TRUE ;
      }
   }

   if ( NOT NeedNewBitmaps )
      return ( FALSE ) ;

  /**************************************************************************
   * Open a memory device context.                                          *
   **************************************************************************/

   DeviceContext Memory = DeviceContext ( "UpdateAttrBitmaps", long(Anchor) ) ;

  /**************************************************************************
   * Create a memory presentation space for painting bitmaps.               *
   **************************************************************************/

   WorkSpace PS = WorkSpace ( "UpdateAttrBitmaps", 0, &Memory, Anchor, FALSE, TRUE ) ;

  /**************************************************************************
   * Redraw the bitmaps.                                                    *
   **************************************************************************/

   ResourceString Normal ( Library, IDS_ATTR_NORMAL ) ;
   Bitmaps[0] = CreateMenuBitmap ( PS, PSZ(Normal), NewFont, 180,
     FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, 100 ) ;

   ResourceString Bold ( Library, IDS_ATTR_BOLD ) ;
   Bitmaps[1] = CreateMenuBitmap ( PS, PSZ(Bold), NewFont, 180,
     TRUE, FALSE, FALSE, FALSE, FALSE, FALSE, 100 ) ;

   ResourceString Italic ( Library, IDS_ATTR_ITALIC ) ;
   Bitmaps[2] = CreateMenuBitmap ( PS, PSZ(Italic), NewFont, 180,
     FALSE, TRUE, FALSE, FALSE, FALSE, FALSE, 100 ) ;

   ResourceString Underscore ( Library, IDS_ATTR_UNDERSCORE ) ;
   Bitmaps[3] = CreateMenuBitmap ( PS, PSZ(Underscore), NewFont, 180,
     FALSE, FALSE, TRUE, FALSE, FALSE, FALSE, 100 ) ;

   ResourceString Strikeout ( Library, IDS_ATTR_STRIKEOUT ) ;
   Bitmaps[4] = CreateMenuBitmap ( PS, PSZ(Strikeout), NewFont, 180,
     FALSE, FALSE, FALSE, TRUE, FALSE, FALSE, 100 ) ;

   ResourceString Outline ( Library, IDS_ATTR_OUTLINE ) ;
   Bitmaps[5] = CreateMenuBitmap ( PS, PSZ(Outline), NewFont, 180,
     FALSE, FALSE, FALSE, FALSE, TRUE, FALSE, 100 ) ;

   ResourceString Caps ( Library, IDS_ATTR_CAPS ) ;
   Bitmaps[6] = CreateMenuBitmap ( PS, PSZ(Caps), NewFont, 180,
     FALSE, FALSE, FALSE, FALSE, FALSE, TRUE, 100 ) ;

   ResourceString Expand ( Library, IDS_EXPAND ) ;
   char Text [80] ;
   strcpy ( Text, PCHAR(Expand) ) ;
   Text [ strcspn ( Text, " " ) ] = 0 ;
   Bitmaps[7] = CreateMenuBitmap ( PS, PSZ(Text), NewFont, 180,
     FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, 200 ) ;

   return ( TRUE ) ;
}



 
/****************************************************************************
 ****************************************************************************
 ***                                                                      ***
 ***              Expansion Menu Management Functions & Data              ***
 ***                                                                      ***
 ****************************************************************************
 ****************************************************************************/

typedef struct {
   BOOL      Threaded ;
   HMODULE   Library ;
   HWND      WindowToNotify ;
   PSZ       NewFont ;
   USHORT    NewCount ;
   PUSHORT   NewExpansions ;
} EXPANDINIT_PARMS, *PEXPANDINIT_PARMS ;

static VOID ExpandInitThread ( PVOID Parameter ) ;

static BOOL UpdateExpandBitmaps (
   HAB Anchor,
   HMODULE Library,
   PSZ OldFont,
   PSZ NewFont,
   USHORT OldCount,
   USHORT OldExpansions[],
   USHORT NewCount,
   USHORT NewExpansions[],
   HBITMAP Bitmaps[],
   SIZEL BitmapSizes[]
) ;

static PHBITMAP     ExpandBitmaps = 0 ;
static PSIZEL       ExpandBitmapSizes = 0 ;
static USHORT       ExpandCount = 0 ;
static Event        ExpandDone ( "MenuBmp-ExpandDone" ) ;
static PSZ          ExpandFont = 0 ;
static PUSHORT      Expansions = 0 ;
static Mutex        ExpandSemaphore ( "MenuBmp-ExpandSemaphore" ) ;
static TID          ExpandThread = 0 ;

 
/****************************************************************************
 *                                                                          *
 *  Start thread to initialize expand bitmaps.                              *
 *                                                                          *
 ****************************************************************************/

extern void InitExpandBitmaps (
   HMODULE      Library,        // -> Resource Library
   HWND         WindowToNotify, // -> Window to notify upon completion.
   PSZ          NewFont,        // -> New Font
   USHORT       NewCount,       // -> New Size Count
   PUSHORT      NewExpansions   // -> New Expansions
) {

  /**************************************************************************
   * Wait for previous initialization to complete before starting it anew.  *
   **************************************************************************/

   if ( long(ExpandThread) > 0 ) {
      DosSetPriority ( PRTYS_THREAD, PRTYC_FOREGROUNDSERVER, +20, ExpandThread ) ;
      ExpandDone.Wait() ;
   } /* endif */

  /**************************************************************************
   * Allocate and initialize the thread parameters.                         *
   **************************************************************************/

   PEXPANDINIT_PARMS Parms = PEXPANDINIT_PARMS ( malloc ( sizeof(EXPANDINIT_PARMS) ) ) ;
   Parms->Threaded = TRUE ;
   Parms->Library = Library ;
   Parms->WindowToNotify = WindowToNotify ;
   Parms->NewFont = NewFont ;
   Parms->NewCount = NewCount ;
   Parms->NewExpansions = NewExpansions ;

  /**************************************************************************
   * Start the initialization thread.                                       *
   **************************************************************************/

   #ifdef THREADED
      Sys_SendMessage ( WindowToNotify, WM_THREAD_STARTED, 0, 0 ) ;
      ExpandDone.Reset ( ) ;
      ExpandThread = StartThread ( "ExpandInitThread", ExpandInitThread, 0x10000, Parms, PRTYC_IDLETIME, +20 ) ;
   #else
      Parms->Threaded = FALSE ;
      Parms->WindowToNotify = 0 ;
      ExpandThread = 0 ;
      ExpandInitThread ( Parms ) ;
   #endif
}
 
/****************************************************************************
 *                                                                          *
 *  Initialize the Expand Menu                                              *
 *                                                                          *
 ****************************************************************************/

extern void InitExpandMenu (
   HWND         Menu,           // -> Menu
   Estado      &State,          // -> Current State
   HMODULE      Library,        // -> Resource Library
   int          FirstEntry,     // Offset of first entry to display.
   USHORT       NewCount,       // -> New Expansion Count
   PUSHORT      NewExpansions   // -> New Expansions (or NULL to use old ones)
) {

  /**************************************************************************
   * Set the hourglass for a bit.                                           *
   **************************************************************************/

   HourGlass Hold ( Menu ) ;

  /**************************************************************************
   * Wait for any expand list update in progress to complete.               *
   **************************************************************************/

   if ( long(ExpandThread) > 0 ) {
      DosSetPriority ( PRTYS_THREAD, PRTYC_FOREGROUNDSERVER, +20, ExpandThread ) ;
      ExpandDone.Wait() ;
   } /* endif */

  /**************************************************************************
   * Update it again, since we don't know how long ago the update was.      *
   **************************************************************************/

   PEXPANDINIT_PARMS Parms = PEXPANDINIT_PARMS ( malloc ( sizeof(EXPANDINIT_PARMS) ) ) ;
   Parms->Threaded = FALSE ;
   Parms->Library = Library ;
   Parms->WindowToNotify = 0 ;
   Parms->NewFont = PSZ(State.Font) ;
   Parms->NewCount = NewExpansions ? NewCount : ExpandCount ;
   Parms->NewExpansions = NewExpansions ? NewExpansions : Expansions ;
   ExpandInitThread ( Parms ) ;

  /**************************************************************************
   * Clear out the old menu.                                                *
   **************************************************************************/

   LONG OldCount = LONGFROMMR ( Sys_SendMessage ( Menu, MM_QUERYITEMCOUNT, 0, 0 ) ) ;
   for ( int i=0; i<OldCount; i++ ) {
      LONG Id = LONGFROMMR ( Sys_SendMessage ( Menu, MM_ITEMIDFROMPOSITION, MPFROMSHORT(0), 0 ) ) ;
      Sys_SendMessage ( Menu, MM_REMOVEITEM, MPFROM2SHORT(Id,TRUE), 0 ) ;
   } /* endfor */

  /**************************************************************************
   * Initialize the menu item structure.                                    *
   **************************************************************************/

   MENUITEM MenuItem ;
   MenuItem.iPosition = MIT_END ;
   MenuItem.afAttribute = 0 ;
   MenuItem.hwndSubMenu = 0 ;
   MenuItem.id = USHORT ( IDM_EXPANDS + FirstEntry ) ;

  /**************************************************************************
   * Determine the maximum menu height & width.                             *
   **************************************************************************/

   RECTL Desktop ;
   WinQueryWindowRect ( HWND_DESKTOP, &Desktop ) ;

   SWP Position ;
   WinQueryWindowPos ( OWNER(Menu), &Position ) ;

   POINTL Points[2] ;
   Points[0].x = Points[1].x = Position.x ;
   Points[0].y = Position.y ;  Points[1].y = Position.y + Position.cy ;
   WinMapWindowPoints ( PARENT(OWNER(Menu)), HWND_DESKTOP, PPOINTL(&Points), 2 ) ;

   SIZEL MaxSize ;
   MaxSize.cx = Desktop.xRight - Desktop.xLeft ;
   MaxSize.cy = max ( Desktop.yTop-Points[1].y, Points[0].y-Desktop.yBottom ) ;

  /**************************************************************************
   * Determine all the bitmap sizes.                                        *
   **************************************************************************/

   int ColumnStart = FirstEntry ;
   LONG Left = 0 ;
   LONG Width = 0 ;
   LONG Bottom = MaxSize.cy - ( FirstEntry ? 50 : 0 ) ;

   for ( i=FirstEntry; i<ExpandCount; i++ ) {
      Bottom -= ExpandBitmapSizes[i].cy + 6 ;
      if ( Bottom < 0 ) {
         ColumnStart = i ;
         Left += Width ;
         Width = 0 ;
         Bottom = MaxSize.cy - ( ExpandBitmapSizes[i].cy + 6 ) ;
      } /* endif */
      Width = max ( Width, ExpandBitmapSizes[i].cx + 50 ) ;
      if ( Width > MaxSize.cx-Left ) {
         i = ColumnStart ;
         break ;
      } /* endif */
   } /* endfor */

   int LastEntry = i - 1 ;

  /**************************************************************************
   * If we aren't showing the start of the list, prepend a page-up entry.   *
   **************************************************************************/

   if ( FirstEntry ) {
      ResourceString More ( Library, IDS_MORE_PGUP ) ;
      MENUITEM MenuItem ;
      MenuItem.iPosition = MIT_END ;
      MenuItem.afAttribute = 0 ;
      MenuItem.hwndSubMenu = 0 ;
      MenuItem.id = IDM_EXPANDS_PGUP ;
      MenuItem.afStyle = MIS_TEXT ;
      Sys_SendMessage ( Menu, MM_INSERTITEM, MPFROMP(&MenuItem), MPFROMP(PSZ(More)) ) ;
   } /* endif */

  /**************************************************************************
   * Build the menu entries.                                                *
   **************************************************************************/

   Left = 0 ;
   Width = 0 ;
   Bottom = MaxSize.cy - ( FirstEntry ? 50 : 0 ) ;

   for ( i=FirstEntry; i<=LastEntry; i++ ) {

     /***********************************************************************
      * Set the item to break the menu into columns if necessary.           *
      ***********************************************************************/

      MenuItem.afStyle = MIS_BITMAP ;
      Bottom -= ExpandBitmapSizes[i].cy + 6 ;
      if ( Bottom < 0 ) {
         MenuItem.afStyle |= MIS_BREAKSEPARATOR ;
         Bottom = MaxSize.cy - ( ExpandBitmapSizes[i].cy + 6 ) ;
      } /* endif */

     /***********************************************************************
      * Set the normal bitmap.                                              *
      ***********************************************************************/

      MenuItem.hItem = ULONG ( ExpandBitmaps[i] ) ;

     /***********************************************************************
      * Add the menu entry.                                                 *
      ***********************************************************************/

      Sys_SendMessage ( Menu, MM_INSERTITEM, MPFROMP(&MenuItem), MPFROMP("") ) ;

     /***********************************************************************
      * If this is the current size of the active window, check it.         *
      ***********************************************************************/

      if ( Expansions[i] == State.Expand ) {
         Sys_SendMessage ( Menu, MM_SETITEMATTR, MPFROM2SHORT(MenuItem.id,TRUE), MPFROM2SHORT(MIA_CHECKED,MIA_CHECKED) ) ;
      } /* endif */

     /***********************************************************************
      * Get the next menu item ID.                                          *
      ***********************************************************************/

      MenuItem.id ++ ;

   } /* endfor */

  /**************************************************************************
   * If we didn't show the end of the list, append a page-down entry.       *
   **************************************************************************/

   if ( LastEntry < ExpandCount-1 ) {
      ResourceString More ( Library, IDS_MORE_PGDN ) ;
      MENUITEM MenuItem ;
      MenuItem.iPosition = MIT_END ;
      MenuItem.afAttribute = 0 ;
      MenuItem.hwndSubMenu = 0 ;
      MenuItem.id = IDM_EXPANDS_PGDN ;
      MenuItem.afStyle = MIS_TEXT ;
      Sys_SendMessage ( Menu, MM_INSERTITEM, MPFROMP(&MenuItem), MPFROMP(PSZ(More)) ) ;
   } /* endif */

  /**************************************************************************
   * In any case, add on a separator and the menu manager.                  *
   **************************************************************************/

   #ifdef MANAGERS
   MenuItem.iPosition = MIT_END ;
   MenuItem.afAttribute = 0 ;
   MenuItem.hwndSubMenu = 0 ;
   MenuItem.id = 0 ;
   MenuItem.afStyle = MIS_SEPARATOR ;
   MenuItem.hItem = 0 ;
   Sys_SendMessage ( Menu, MM_INSERTITEM, MPFROMP(&MenuItem), MPFROMP("") ) ;

   ResourceString Manager ( Library, IDS_MANAGER ) ;
   MenuItem.iPosition = MIT_END ;
   MenuItem.afAttribute = 0 ;
   MenuItem.hwndSubMenu = 0 ;
   MenuItem.id = IDM_EXPANDMGR ;
   MenuItem.afStyle = MIS_TEXT ;
   MenuItem.hItem = 0 ;
   Sys_SendMessage ( Menu, MM_INSERTITEM, MPFROMP(&MenuItem), MPFROMP(PSZ(Manager)) ) ;
   #endif
}
 
/****************************************************************************
 *                                                                          *
 *      Expand Menu Initialization Thread                                   *
 *                                                                          *
 ****************************************************************************/

static VOID ExpandInitThread ( PVOID Parameter ) {

   #ifdef DEBUG
      Log ( "Expand Init thread started." ) ;
   #endif

  /**************************************************************************
   * Get parameters.                                                        *
   **************************************************************************/

   PEXPANDINIT_PARMS Parms = PEXPANDINIT_PARMS ( Parameter ) ;

  /**************************************************************************
   * Set up for GPI calls.                                             h    *
   **************************************************************************/

   Process *Proc = 0 ;
   if ( Parms->Threaded ) 
      Proc = new Process ( "ExpandInitThread", 0, LibraryHandle ) ;

  /**************************************************************************
   * Expand the bitmap handle array, if necessary.                          *
   **************************************************************************/

   if ( Parms->NewCount > ExpandCount ) {
      PHBITMAP Bitmaps = PHBITMAP ( malloc ( Parms->NewCount * sizeof(HBITMAP) ) ) ;
      memset ( Bitmaps, 0, Parms->NewCount * sizeof(HBITMAP) ) ;
      if ( ExpandBitmaps ) {
         memcpy ( Bitmaps, ExpandBitmaps, ExpandCount * sizeof(HBITMAP) ) ;
         free ( ExpandBitmaps ) ;
      } /* endif */
      ExpandBitmaps = Bitmaps ;
      PSIZEL BitmapSizes = PSIZEL ( malloc ( Parms->NewCount * sizeof(SIZEL) ) ) ;
      memset ( BitmapSizes, 0, Parms->NewCount * sizeof(SIZEL) ) ;
      if ( ExpandBitmapSizes ) {
         memcpy ( BitmapSizes, ExpandBitmapSizes, ExpandCount * sizeof(SIZEL) ) ;
         free ( ExpandBitmapSizes ) ;
      } /* endif */
      ExpandBitmapSizes = BitmapSizes ;
   } /* endif */

  /**************************************************************************
   * Update the bitmaps.                                                    *
   **************************************************************************/

   UpdateExpandBitmaps ( Proc ? Proc->QueryAnchor() : 0, Parms->Library,
      ExpandFont ? ExpandFont : PSZ(""), Parms->NewFont, ExpandCount, Expansions,
      Parms->NewCount, Parms->NewExpansions, ExpandBitmaps, ExpandBitmapSizes ) ;

  /**************************************************************************
   * Save the last font for which the size menu was built.                  *
   **************************************************************************/

   if ( ExpandFont )
      free ( ExpandFont ) ;

   ExpandFont = PSZ ( malloc ( strlen(PCHAR(Parms->NewFont))+1 ) ) ;
   strcpy ( PCHAR(ExpandFont), PCHAR(Parms->NewFont) ) ;

   ExpandCount = Parms->NewCount ;

   if ( Expansions != Parms->NewExpansions ) {
      if ( Expansions )
         free ( Expansions ) ;
      Expansions = PUSHORT ( malloc ( ExpandCount * sizeof(USHORT) ) ) ;
      memcpy ( Expansions, Parms->NewExpansions, ExpandCount * sizeof(USHORT) ) ;
   } /* endif */

  /**************************************************************************
   * Notify the window that wanted to know when the thread was finished.    *
   **************************************************************************/

   if ( Parms->WindowToNotify )
      if ( !Sys_PostMessage ( Parms->WindowToNotify, WM_THREAD_DONE, MPFROMLONG(Sys_ThreadNumber(*_threadid)), 0 ) ) {
         ERRORID Code = Sys_GetLastError ( Proc ? Proc->QueryAnchor() : 0 ) ;
         Log ( "WARNING: Attempt to notify window of expand init thread completion failed.  Error %lX.", Code ) ;
      } /* endif */

  /**************************************************************************
   * Release PM resources.                                                  *
   **************************************************************************/

   if ( Parms->Threaded ) 
      delete Proc ;

  /**************************************************************************
   * Release the parameter memory.                                          *
   **************************************************************************/

   free ( Parms ) ;

  /**************************************************************************
   * We're done.                                                            *
   **************************************************************************/

   ExpandDone.Post() ;
   ExpandThread = 0 ;

   #ifdef DEBUG
      Log ( "Expand Init thread done." ) ;
   #endif
}
 
/****************************************************************************
 *                                                                          *
 *  Update Expand Bitmaps                                                   *
 *                                                                          *
 ****************************************************************************/

static BOOL UpdateExpandBitmaps (
   HAB Anchor,
   HMODULE Library,
   PSZ OldFont,
   PSZ NewFont,
   USHORT OldCount,
   USHORT OldExpansions[],
   USHORT NewCount,
   USHORT NewExpansions[],
   HBITMAP Bitmaps[],
   SIZEL BitmapSizes[]
) {

  /**************************************************************************
   * Reconcile the new expansions against the old, deleting obsolete bitmaps*
   **************************************************************************/

   qsort ( NewExpansions, (size_t)NewCount, sizeof(NewExpansions[0]), CompareUSHORTs ) ;

   PHBITMAP OldBitmaps = 0 ;
   if ( OldCount ) {
      OldBitmaps = PHBITMAP ( malloc ( sizeof(Bitmaps[0]) * OldCount ) ) ;
      memcpy ( OldBitmaps, Bitmaps, sizeof(Bitmaps[0])*OldCount ) ;
   } /* endif */

   memset ( Bitmaps, 0, sizeof(Bitmaps[0])*NewCount ) ;

   BOOL AlteredList = FALSE ;
   BOOL NeedNewBitmaps = FALSE ;

   int i=0, j=0;
   while ( i<OldCount OR j<NewCount ) {
      if ( i>=NewCount ) {
         if ( OldBitmaps[i] )
            GpiDeleteBitmap ( OldBitmaps[i] ) ;
         AlteredList = TRUE ;
         i ++ ;
      } else if ( j>=OldCount ) {
         AlteredList = TRUE ;
         NeedNewBitmaps = TRUE ;
         j ++ ;
      } else {
         int Compare = CompareUSHORTs ( &OldExpansions[i], &NewExpansions[j] ) ;
         if ( Compare == 0 ) {
            if ( OldBitmaps[i] )
               if ( strcmp ( (char*)OldFont, (char*)NewFont ) ) {
                  GpiDeleteBitmap ( OldBitmaps[i] ) ;
                  AlteredList = TRUE ;
                  NeedNewBitmaps = TRUE ;
               } else
                  Bitmaps[j] = OldBitmaps[i] ;
            else
               NeedNewBitmaps = TRUE ;
            i++, j++;
         } else if ( Compare < 0 ) {
           if ( OldBitmaps[i] )
             GpiDeleteBitmap ( OldBitmaps[i] ) ;
           AlteredList = TRUE ;
           i ++ ;
         } else {
           AlteredList = TRUE ;
           NeedNewBitmaps = TRUE ;
           j ++ ;
         } /* endif */
      } /* endif */
   } /* endwhile */

   if ( OldBitmaps )
      free ( OldBitmaps ) ;

   if ( NOT NeedNewBitmaps )
      return ( AlteredList ) ;

  /**************************************************************************
   * Open a memory device context.                                          *
   **************************************************************************/

   DeviceContext Memory = DeviceContext ( "UpdateExpandBitmaps", long(Anchor) ) ;

  /**************************************************************************
   * Create a memory presentation space for painting bitmaps.               *
   **************************************************************************/

   WorkSpace PS = WorkSpace ( "UpdateExpandBitmaps", 0, &Memory, Anchor, FALSE, TRUE ) ;

  /**************************************************************************
   * Redraw the bitmaps.                                                    *
   **************************************************************************/

   ResourceString Expand ( Library, IDS_EXPAND ) ;
   for ( i=0; i<NewCount && Active; i++ ) {

     /***********************************************************************
      * If new bitmap not needed, skip this one.                            *
      ***********************************************************************/

      if ( Bitmaps[i] )
         continue ;

     /***********************************************************************
      * Create the menu item text.                                          *
      ***********************************************************************/

      UCHAR Text [80] ;
      sprintf ( PCHAR(Text), PCHAR(Expand), NewExpansions[i] ) ;

     /***********************************************************************
      * Create the bitmap.                                                  *
      ***********************************************************************/

      Bitmaps[i] = CreateMenuBitmap ( PS, PSZ(Text), NewFont, 180,
         FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, NewExpansions[i] ) ;

     /***********************************************************************
      * Measure the bitmap.                                                 *
      ***********************************************************************/

      BITMAPINFOHEADER2 BitmapInfoHeader ;
      BitmapInfoHeader.cbFix = sizeof(BitmapInfoHeader) ;
      GpiQueryBitmapInfoHeader ( Bitmaps[i], &BitmapInfoHeader ) ;
      BitmapSizes[i].cx = BitmapInfoHeader.cx ;
      BitmapSizes[i].cy = BitmapInfoHeader.cy ;

   } /* endfor */

   return ( TRUE ) ;
}



 
/****************************************************************************
 ****************************************************************************
 ***                                                                      ***
 ***             Line Color Menu Management Functions & Data              ***
 ***                                                                      ***
 ****************************************************************************
 ****************************************************************************/

typedef struct {
   BOOL      Threaded ;
   HMODULE   Library ;
   HWND      WindowToNotify ;
   USHORT    NewCount ;
   PCOLOR    NewColors ;
} LINECOLORINIT_PARMS, *PLINECOLORINIT_PARMS ;

static VOID LineColorInitThread ( PVOID Parameter ) ;

static BOOL UpdateLineColorBitmaps (
   HAB Anchor,
   int OldCount,
   COLOR OldColors[],
   int NewCount,
   COLOR NewColors[],
   HBITMAP Bitmaps[],
   SIZEL BitmapSizes[]
) ;

static PHBITMAP     LineColorBitmaps = 0 ;
static PSIZEL       LineColorBitmapSizes = 0 ;
static USHORT       LineColorCount = 0 ;
static Event        LineColorDone ( "MenuBmp-LineColorDone" ) ;
static PCOLOR       LineColors = 0 ;
static Mutex        LineColorSemaphore ( "MenuBmp-LineColorSemaphore" ) ;
static TID          LineColorThread = 0 ;

 
/****************************************************************************
 *                                                                          *
 *  Start thread to initialize line color bitmaps.                          *
 *                                                                          *
 ****************************************************************************/

extern void InitLineColorBitmaps (
   HMODULE      Library,        // -> Resource Library
   HWND         WindowToNotify, // -> Window to notify upon completion.
   USHORT       NewCount,       // -> New Color Count
   PCOLOR       NewColors       // -> New Colors
) {

 /***************************************************************************
  * Wait for previous initialization to complete before starting it anew.   *
  ***************************************************************************/

  if ( long(LineColorThread) > 0 ) {
     DosSetPriority ( PRTYS_THREAD, PRTYC_FOREGROUNDSERVER, +20, LineColorThread ) ;
     LineColorDone.Wait() ;
  } /* endif */

 /***************************************************************************
  * Allocate and initialize the thread parameters.                          *
  ***************************************************************************/

  PLINECOLORINIT_PARMS Parms = PLINECOLORINIT_PARMS ( malloc ( sizeof(LINECOLORINIT_PARMS) ) ) ;
  Parms->Threaded = TRUE ;
  Parms->Library = Library ;
  Parms->WindowToNotify = WindowToNotify ;
  Parms->NewCount = NewCount ;
  Parms->NewColors = NewColors ;

 /***************************************************************************
  * Start the initialization thread.                                        *
  ***************************************************************************/

  #ifdef THREADED
    Sys_SendMessage ( WindowToNotify, WM_THREAD_STARTED, 0, 0 ) ;
    LineColorDone.Reset() ;
    LineColorThread = StartThread ( "LineColorInitThread", LineColorInitThread, 0x10000, Parms, PRTYC_IDLETIME, +20 ) ;
  #else
    Parms->Threaded = FALSE ;
    Parms->WindowToNotify = 0 ;
    LineColorThread = 0 ;
    LineColorInitThread ( Parms ) ;
  #endif
}
 
/****************************************************************************
 *                                                                          *
 *  Initialize the Line Color Menu                                          *
 *                                                                          *
 ****************************************************************************/

extern void InitLineColorMenu (
   HWND         Menu,           // -> Menu
   Estado      &State,          // -> Current State
   HMODULE      Library,        // -> Resource Library
   int          FirstEntry,     // Offset of first entry to display.
   USHORT       NewCount,       // -> New Color Count
   PCOLOR       NewColors       // -> New Colors (or NULL to use old ones)
) {

  /**************************************************************************
   * Set the hourglass for a bit.                                           *
   **************************************************************************/

   HourGlass Hold ( Menu ) ;

  /**************************************************************************
   * Wait for any color menu update in progress to complete.                *
   **************************************************************************/

   if ( long(LineColorThread) > 0 ) {
      DosSetPriority ( PRTYS_THREAD, PRTYC_FOREGROUNDSERVER, +20, LineColorThread ) ;
      LineColorDone.Wait() ;
   } /* endif */

  /**************************************************************************
   * Update it again, since we don't know how long ago the update was.      *
   **************************************************************************/

   PLINECOLORINIT_PARMS Parms = PLINECOLORINIT_PARMS ( malloc ( sizeof(LINECOLORINIT_PARMS) ) ) ;
   Parms->Threaded = FALSE ;
   Parms->Library = Library ;
   Parms->WindowToNotify = 0 ;
   Parms->NewCount = NewColors ? NewCount : LineColorCount ;
   Parms->NewColors = NewColors ? NewColors : LineColors ;
   LineColorInitThread ( Parms ) ;

  /**************************************************************************
   * Clear out the old menu.                                                *
   **************************************************************************/

   LONG OldCount = LONGFROMMR ( Sys_SendMessage ( Menu, MM_QUERYITEMCOUNT, 0, 0 ) ) ;
   for ( int i=0; i<OldCount; i++ ) {
      LONG Id = LONGFROMMR ( Sys_SendMessage ( Menu, MM_ITEMIDFROMPOSITION, MPFROMSHORT(0), 0 ) ) ;
      Sys_SendMessage ( Menu, MM_REMOVEITEM, MPFROM2SHORT(Id,TRUE), 0 ) ;
   } /* endfor */

  /**************************************************************************
   * Initialize the menu item structure.                                    *
   **************************************************************************/

   MENUITEM MenuItem ;
   MenuItem.iPosition = MIT_END ;
   MenuItem.afAttribute = 0 ;
   MenuItem.hwndSubMenu = 0 ;
   MenuItem.id = USHORT ( IDM_LINECOLORS + FirstEntry ) ;

  /**************************************************************************
   * Determine the maximum menu height & width.                             *
   **************************************************************************/

   RECTL Desktop ;
   WinQueryWindowRect ( HWND_DESKTOP, &Desktop ) ;

   SWP Position ;
   WinQueryWindowPos ( OWNER(Menu), &Position ) ;

   POINTL Points[2] ;
   Points[0].x = Points[1].x = Position.x ;
   Points[0].y = Position.y ;  Points[1].y = Position.y + Position.cy ;
   WinMapWindowPoints ( PARENT(OWNER(Menu)), HWND_DESKTOP, PPOINTL(&Points), 2 ) ;

   SIZEL MaxSize ;
   MaxSize.cx = Desktop.xRight - Desktop.xLeft ;
   MaxSize.cy = max ( Desktop.yTop-Points[1].y, Points[0].y-Desktop.yBottom ) ;

  /**************************************************************************
   * Determine all the bitmap sizes.                                        *
   **************************************************************************/

   int ColumnStart = FirstEntry ;
   LONG Left = 0 ;
   LONG Width = 0 ;
   LONG Bottom = MaxSize.cy - ( FirstEntry ? 50 : 0 ) ;

   for ( i=FirstEntry; i<LineColorCount; i++ ) {
      Bottom -= LineColorBitmapSizes[i].cy + 6 ;
      if ( Bottom < 0 ) {
         ColumnStart = i ;
         Left += Width ;
         Width = 0 ;
         Bottom = MaxSize.cy - ( LineColorBitmapSizes[i].cy + 6 ) ;
      } /* endif */
      Width = max ( Width, LineColorBitmapSizes[i].cx + 50 ) ;
      if ( Width > MaxSize.cx-Left ) {
         i = ColumnStart ;
         break ;
      } /* endif */
   } /* endfor */

   int LastEntry = i - 1 ;

  /**************************************************************************
   * If we aren't showing the start of the list, prepend a page-up entry.   *
   **************************************************************************/

   if ( FirstEntry ) {
      ResourceString More ( Library, IDS_MORE_PGUP ) ;
      MENUITEM MenuItem ;
      MenuItem.iPosition = MIT_END ;
      MenuItem.afAttribute = 0 ;
      MenuItem.hwndSubMenu = 0 ;
      MenuItem.id = IDM_LINECOLORS_PGUP ;
      MenuItem.afStyle = MIS_TEXT ;
      Sys_SendMessage ( Menu, MM_INSERTITEM, MPFROMP(&MenuItem), MPFROMP(PSZ(More)) ) ;
   } /* endif */

  /**************************************************************************
   * Build the menu entries.                                                *
   **************************************************************************/

   Left = 0 ;
   Width = 0 ;
   Bottom = MaxSize.cy - ( FirstEntry ? 50 : 0 ) ;

   for ( i=FirstEntry; i<=LastEntry; i++ ) {

     /***********************************************************************
      * Set the item to break the menu into columns if necessary.           *
      ***********************************************************************/

      MenuItem.afStyle = MIS_BITMAP ;
      Bottom -= LineColorBitmapSizes[i].cy + 6 ;
      if ( Bottom < 0 ) {
         MenuItem.afStyle |= MIS_BREAKSEPARATOR ;
         Bottom = MaxSize.cy - ( LineColorBitmapSizes[i].cy + 6 ) ;
      } /* endif */

     /***********************************************************************
      * Set the normal bitmap.                                              *
      ***********************************************************************/

      MenuItem.hItem = ULONG ( LineColorBitmaps[i] ) ;

     /***********************************************************************
      * Add the menu entry.                                                 *
      ***********************************************************************/

      Sys_SendMessage ( Menu, MM_INSERTITEM, MPFROMP(&MenuItem), MPFROMP("") ) ;

     /***********************************************************************
      * Check-mark the current color.                                       *
      ***********************************************************************/

      if ( State.Color == LineColors[i] ) {
         Sys_SendMessage ( Menu, MM_SETITEMATTR, MPFROM2SHORT(IDM_LINECOLORS+i,TRUE), MPFROM2SHORT(MIA_CHECKED,MIA_CHECKED) ) ;
      } else {
         Sys_SendMessage ( Menu, MM_SETITEMATTR, MPFROM2SHORT(IDM_LINECOLORS+i,TRUE), MPFROM2SHORT(MIA_CHECKED,0) ) ;
      } /* endif */

     /***********************************************************************
      * Get the next menu item ID.                                          *
      ***********************************************************************/

      MenuItem.id ++ ;
   }

  /**************************************************************************
   * If we didn't show the end of the list, append a page-down entry.       *
   **************************************************************************/

   if ( LastEntry < LineColorCount-1 ) {
      ResourceString More ( Library, IDS_MORE_PGDN ) ;
      MENUITEM MenuItem ;
      MenuItem.iPosition = MIT_END ;
      MenuItem.afAttribute = 0 ;
      MenuItem.hwndSubMenu = 0 ;
      MenuItem.id = IDM_LINECOLORS_PGDN ;
      MenuItem.afStyle = MIS_TEXT ;
      Sys_SendMessage ( Menu, MM_INSERTITEM, MPFROMP(&MenuItem), MPFROMP(PSZ(More)) ) ;
   } /* endif */

  /**************************************************************************
   * In any case, add on a separator and the menu manager.                  *
   **************************************************************************/

   #ifdef MANAGERS
   MenuItem.iPosition = MIT_END ;
   MenuItem.afAttribute = 0 ;
   MenuItem.hwndSubMenu = 0 ;
   MenuItem.id = 0 ;
   MenuItem.afStyle = MIS_SEPARATOR ;
   MenuItem.hItem = 0 ;
   Sys_SendMessage ( Menu, MM_INSERTITEM, MPFROMP(&MenuItem), MPFROMP("") ) ;

   ResourceString Manager ( Library, IDS_MANAGER ) ;
   MenuItem.iPosition = MIT_END ;
   MenuItem.afAttribute = 0 ;
   MenuItem.hwndSubMenu = 0 ;
   MenuItem.id = IDM_LINECOLORMGR ;
   MenuItem.afStyle = MIS_TEXT ;
   MenuItem.hItem = 0 ;
   Sys_SendMessage ( Menu, MM_INSERTITEM, MPFROMP(&MenuItem), MPFROMP(PSZ(Manager)) ) ;
   #endif
}
 
/****************************************************************************
 *                                                                          *
 *      Line Color Menu Initialization Thread                               *
 *                                                                          *
 ****************************************************************************/

static VOID LineColorInitThread ( PVOID Parameter ) {

  #ifdef DEBUG
     Log ( "LineColor Init thread started." ) ;
  #endif

 /***************************************************************************
  * Get parameters.                                                         *
  ***************************************************************************/

  PLINECOLORINIT_PARMS Parms = PLINECOLORINIT_PARMS ( Parameter ) ;

 /***************************************************************************
  * Set up for GPI calls.                                                   *
  ***************************************************************************/

   Process *Proc = 0 ;
   if ( Parms->Threaded ) 
      Proc = new Process ( "LineColorInitThread", 0, LibraryHandle ) ;

 /***************************************************************************
  * Expand the bitmap handle array, if necessary.                           *
  ***************************************************************************/

  if ( Parms->NewCount > LineColorCount ) {
     PHBITMAP Bitmaps = PHBITMAP ( malloc ( Parms->NewCount * sizeof(HBITMAP) ) ) ;
     memset ( Bitmaps, 0, Parms->NewCount * sizeof(HBITMAP) ) ;
     if ( LineColorBitmaps ) {
        memcpy ( Bitmaps, LineColorBitmaps, LineColorCount * sizeof(HBITMAP) ) ;
        free ( LineColorBitmaps ) ;
     } /* endif */
     LineColorBitmaps = Bitmaps ;
     PSIZEL BitmapSizes = PSIZEL ( malloc ( Parms->NewCount * sizeof(SIZEL) ) ) ;
     memset ( BitmapSizes, 0, Parms->NewCount * sizeof(SIZEL) ) ;
     if ( LineColorBitmapSizes ) {
        memcpy ( BitmapSizes, LineColorBitmapSizes, LineColorCount * sizeof(SIZEL) ) ;
        free ( LineColorBitmapSizes ) ;
     } /* endif */
     LineColorBitmapSizes = BitmapSizes ;
  } /* endif */

 /***************************************************************************
  * Update the bitmaps.                                                     *
  ***************************************************************************/

  UpdateLineColorBitmaps ( Proc ? Proc->QueryAnchor() : 0, LineColorCount, LineColors, Parms->NewCount, Parms->NewColors, LineColorBitmaps, LineColorBitmapSizes ) ;

 /***************************************************************************
  * Save the new color list and count.                                      *
  ***************************************************************************/

  LineColorCount = Parms->NewCount ;

  if ( LineColors != Parms->NewColors ) {
     if ( LineColors )
        free ( LineColors ) ;
     LineColors = PCOLOR ( malloc ( LineColorCount * sizeof(COLOR) ) ) ;
     memcpy ( LineColors, Parms->NewColors, LineColorCount * sizeof(COLOR) ) ;
  } /* endif */

 /***************************************************************************
  * Notify the window that wanted to know when the thread was finished.     *
  ***************************************************************************/

  if ( Parms->WindowToNotify )
     if ( !Sys_PostMessage ( Parms->WindowToNotify, WM_THREAD_DONE, MPFROMLONG(Sys_ThreadNumber(*_threadid)), 0 ) ) {
        ERRORID Code = Sys_GetLastError ( Proc ? Proc->QueryAnchor() : 0 ) ;
        Log ( "WARNING: Attempt to notify window of line color init thread completion failed.  Error %lX.", Code ) ;
     } /* endif */

 /***************************************************************************
  * Release PM resources.                                                   *
  ***************************************************************************/

   if ( Parms->Threaded ) 
      delete Proc ;

  /**************************************************************************
   * Release the parameter memory.                                          *
   **************************************************************************/

   free ( Parms ) ;

 /***************************************************************************
  * We're done.                                                             *
  ***************************************************************************/

  LineColorDone.Post() ;
  LineColorThread = 0 ;

  #ifdef DEBUG
     Log ( "LineColor Init thread done." ) ;
  #endif
}
 
/****************************************************************************
 *                                                                          *
 *  Update Line Color Bitmaps                                               *
 *                                                                          *
 ****************************************************************************/

static BOOL UpdateLineColorBitmaps (
  HAB Anchor,
  int OldCount,
  COLOR OldColors[],
  int NewCount,
  COLOR NewColors[],
  HBITMAP Bitmaps[],
  SIZEL BitmapSizes[]
) {

 /***************************************************************************
  * Reconcile the new colors against the old, deleting obsolete bitmaps.    *
  ***************************************************************************/

  qsort ( NewColors, (size_t)NewCount, sizeof(NewColors[0]), CompareCOLORs ) ;

  PHBITMAP OldBitmaps = 0 ;
  if ( OldCount ) {
    OldBitmaps = PHBITMAP ( malloc ( sizeof(Bitmaps[0]) * OldCount ) ) ;
    memcpy ( OldBitmaps, Bitmaps, sizeof(Bitmaps[0])*OldCount ) ;
  }

  memset ( Bitmaps, 0, sizeof(Bitmaps[0])*NewCount) ;

  BOOL AlteredList = FALSE ;
  BOOL NeedNewBitmaps = FALSE ;

  int i=0, j=0;
  while ( i<OldCount OR j<NewCount ) {
     if ( i>=NewCount ) {
        if ( OldBitmaps[i] )
           GpiDeleteBitmap ( OldBitmaps[i] ) ;
        AlteredList = TRUE ;
        i ++ ;
     } else if ( j>=OldCount ) {
        AlteredList = TRUE ;
        NeedNewBitmaps = TRUE ;
        j ++ ;
     } else {
        int Compare = CompareCOLORs ( &OldColors[i], &NewColors[j] ) ;
        if ( Compare == 0 ) {
           if ( OldBitmaps[i] )
              Bitmaps[j] = OldBitmaps[i] ;
           else
              NeedNewBitmaps = TRUE ;
           i++, j++;
        } else if ( Compare < 0 ) {
          if ( OldBitmaps[i] )
            GpiDeleteBitmap ( OldBitmaps[i] ) ;
          AlteredList = TRUE ;
          i ++ ;
        } else {
          AlteredList = TRUE ;
          NeedNewBitmaps = TRUE ;
          j ++ ;
        } /* endif */
     } /* endif */
  } /* endwhile */

  if ( OldBitmaps )
    free ( OldBitmaps ) ;

  if ( NOT NeedNewBitmaps )
     return ( AlteredList ) ;

  /**************************************************************************
   * Open a memory device context.                                          *
   **************************************************************************/

   DeviceContext Memory = DeviceContext ( "UpdateLineColorBitmaps", long(Anchor) ) ;

  /**************************************************************************
   * Create a memory presentation space for painting bitmaps.               *
   **************************************************************************/

   WorkSpace PS = WorkSpace ( "UpdateLineColorBitmaps", 0, &Memory, Anchor, TRUE ) ;

 /***************************************************************************
  * Find out what kind of bitmaps the memory PS supports.                   *
  ***************************************************************************/

  int Planes, BitCount ;
  PS.QueryBitmapFormat ( Planes, BitCount ) ;

 /***************************************************************************
  * Initialize the bitmap header.                                           *
  ***************************************************************************/

  BITMAPINFOHEADER2 BitmapHeader ;
  memset ( &BitmapHeader, 0, sizeof(BitmapHeader) ) ;
  BitmapHeader.cbFix = 16 ;
  BitmapHeader.cx = 64 ;
  BitmapHeader.cy = 20 ;
  BitmapHeader.cPlanes = USHORT ( Planes ) ;
  BitmapHeader.cBitCount = USHORT ( BitCount ) ;

 /***************************************************************************
  * For each color to be shown in the menu . . .                            *
  ***************************************************************************/

  for ( i=0; i<NewCount && Active; i++ ) {

   /*************************************************************************
    * If new bitmap not needed, skip this one.                              *
    *************************************************************************/

    if ( Bitmaps[i] )
       continue ;

   /*************************************************************************
    * Create a bitmap for the menu item.                                    *
    *************************************************************************/

    Bitmaps[i] = PS.CreateBitmap ( BitmapHeader ) ;
    PS.SetBitmap ( Bitmaps[i] ) ;

   /*************************************************************************
    * Paint the bitmap with the current color.                              *
    *************************************************************************/

    PS.SetColor ( NewColors[i] ) ;
    RECTL Box = { 1, 1, BitmapHeader.cx-2, BitmapHeader.cy-2 } ;
    PS.FillBox ( Box ) ;

    PS.SetColor ( RGB_BLACK ) ;
    Box.xLeft = Box.yBottom = 0 ;
    Box.xRight = BitmapHeader.cx - 1 ;
    Box.yTop = BitmapHeader.cy - 1 ;
    PS.DrawBox ( Box ) ;

   /*************************************************************************
    * Measure the bitmap.                                                   *
    *************************************************************************/

    BITMAPINFOHEADER2 BitmapInfoHeader ;
    BitmapInfoHeader.cbFix = sizeof(BitmapInfoHeader) ;
    GpiQueryBitmapInfoHeader ( Bitmaps[i], &BitmapInfoHeader ) ;
    BitmapSizes[i].cx = BitmapInfoHeader.cx ;
    BitmapSizes[i].cy = BitmapInfoHeader.cy ;

  } /* endfor */

  return ( TRUE ) ;
}



 
/****************************************************************************
 ****************************************************************************
 ***                                                                      ***
 ***             Line Width Menu Management Functions & Data              ***
 ***                                                                      ***
 ****************************************************************************
 ****************************************************************************/

typedef struct {
   BOOL      Threaded ;
   HMODULE   Library ;
   HWND      WindowToNotify ;
   USHORT    NewCount ;
   PULONG    NewWidths ;
} LINEWIDTHINIT_PARMS, *PLINEWIDTHINIT_PARMS ;

static VOID LineWidthInitThread ( PVOID Parameter ) ;

static BOOL UpdateLineWidthBitmaps (
  HAB Anchor,
  int OldCount,
  ULONG OldWidths[],
  int NewCount,
  ULONG NewWidths[],
  HBITMAP Bitmaps[],
  SIZEL BitmapSizes[]
) ;

static PHBITMAP     LineWidthBitmaps = 0 ;
static PSIZEL       LineWidthBitmapSizes = 0 ;
static USHORT       LineWidthCount = 0 ;
static Event        LineWidthDone ( "MenuBmp-LineWidthDone" ) ;
static PULONG       LineWidths = 0 ;
static Mutex        LineWidthSemaphore ( "MenuBmp-LineWidthSemaphore" ) ;
static TID          LineWidthThread = 0 ;

 
/****************************************************************************
 *                                                                          *
 *  Start thread to initialize line width bitmaps.                          *
 *                                                                          *
 ****************************************************************************/

extern void InitLineWidthBitmaps (
   HMODULE      Library,        // -> Resource Library
   HWND         WindowToNotify, // -> Window to notify upon completion.
   USHORT       NewCount,       // -> New Width Count
   PULONG       NewWidths       // -> New Widths
) {

 /***************************************************************************
  * Wait for previous initialization to complete before starting it anew.   *
  ***************************************************************************/

  if ( long(LineWidthThread) > 0 ) {
     DosSetPriority ( PRTYS_THREAD, PRTYC_FOREGROUNDSERVER, +20, LineWidthThread ) ;
     LineWidthDone.Wait() ;
  } /* endif */

 /***************************************************************************
  * Allocate and initialize the thread parameters.                          *
  ***************************************************************************/

  PLINEWIDTHINIT_PARMS Parms = PLINEWIDTHINIT_PARMS ( malloc ( sizeof(LINEWIDTHINIT_PARMS) ) ) ;
  Parms->Threaded = TRUE ;
  Parms->Library = Library ;
  Parms->WindowToNotify = WindowToNotify ;
  Parms->NewCount = NewCount ;
  Parms->NewWidths = NewWidths ;

 /***************************************************************************
  * Start the initialization thread.                                        *
  ***************************************************************************/

  #ifdef THREADED
    Sys_SendMessage ( WindowToNotify, WM_THREAD_STARTED, 0, 0 ) ;
    LineWidthDone.Reset() ;
    LineWidthThread = StartThread ( "LineWidthInitThread", LineWidthInitThread, 0x10000, Parms, PRTYC_IDLETIME, +20 ) ;
  #else
    Parms->Threaded = FALSE ;
    Parms->WindowToNotify = 0 ;
    LineWidthThread = 0 ;
    LineWidthInitThread ( Parms ) ;
  #endif
}
 
/****************************************************************************
 *                                                                          *
 *  Initialize the Line Width Menu                                          *
 *                                                                          *
 ****************************************************************************/

extern void InitLineWidthMenu (
   HWND         Menu,           // -> Menu
   Estado      &State,          // -> Current State
   HMODULE      Library,        // -> Resource Library
   int          FirstEntry,     // Offset of first entry to display.
   USHORT       NewCount,       // -> New Width Count
   PULONG       NewWidths       // -> New Widths (or NULL to use old ones)
) {

  /**************************************************************************
   * Set the hourglass for a bit.                                           *
   **************************************************************************/

   HourGlass Hold ( Menu ) ;

  /**************************************************************************
   * Wait for any width menu update in progress to complete.                *
   **************************************************************************/

   if ( long(LineWidthThread) > 0 ) {
      DosSetPriority ( PRTYS_THREAD, PRTYC_FOREGROUNDSERVER, +20, LineWidthThread ) ;
      LineWidthDone.Wait() ;
   } /* endif */

  /**************************************************************************
   * Update it again, since we don't know how long ago the update was.      *
   **************************************************************************/

   PLINEWIDTHINIT_PARMS Parms = PLINEWIDTHINIT_PARMS ( malloc ( sizeof(LINEWIDTHINIT_PARMS) ) ) ;
   Parms->Threaded = FALSE ;
   Parms->Library = Library ;
   Parms->WindowToNotify = 0 ;
   Parms->NewCount = NewWidths ? NewCount : LineWidthCount ;
   Parms->NewWidths = NewWidths ? NewWidths : LineWidths ;
   LineWidthInitThread ( Parms ) ;

  /**************************************************************************
   * Clear out the old menu.                                                *
   **************************************************************************/

   LONG OldCount = LONGFROMMR ( Sys_SendMessage ( Menu, MM_QUERYITEMCOUNT, 0, 0 ) ) ;
   for ( int i=0; i<OldCount; i++ ) {
      LONG Id = LONGFROMMR ( Sys_SendMessage ( Menu, MM_ITEMIDFROMPOSITION, MPFROMSHORT(0), 0 ) ) ;
      Sys_SendMessage ( Menu, MM_REMOVEITEM, MPFROM2SHORT(Id,TRUE), 0 ) ;
   } /* endfor */

  /**************************************************************************
   * Initialize the menu item structure.                                    *
   **************************************************************************/

   MENUITEM MenuItem ;
   MenuItem.iPosition = MIT_END ;
   MenuItem.afAttribute = 0 ;
   MenuItem.hwndSubMenu = 0 ;
   MenuItem.id = USHORT ( IDM_LINEWIDTHS + FirstEntry ) ;

  /**************************************************************************
   * Determine the maximum menu height & width.                             *
   **************************************************************************/

   RECTL Desktop ;
   WinQueryWindowRect ( HWND_DESKTOP, &Desktop ) ;

   SWP Position ;
   WinQueryWindowPos ( OWNER(Menu), &Position ) ;

   POINTL Points[2] ;
   Points[0].x = Points[1].x = Position.x ;
   Points[0].y = Position.y ;  Points[1].y = Position.y + Position.cy ;
   WinMapWindowPoints ( PARENT(OWNER(Menu)), HWND_DESKTOP, PPOINTL(&Points), 2 ) ;

   SIZEL MaxSize ;
   MaxSize.cx = Desktop.xRight - Desktop.xLeft ;
   MaxSize.cy = max ( Desktop.yTop-Points[1].y, Points[0].y-Desktop.yBottom ) ;

  /**************************************************************************
   * Determine all the bitmap sizes.                                        *
   **************************************************************************/

   int ColumnStart = FirstEntry ;
   LONG Left = 0 ;
   LONG Width = 0 ;
   LONG Bottom = MaxSize.cy - ( FirstEntry ? 50 : 0 ) ;

   for ( i=FirstEntry; i<LineWidthCount; i++ ) {
      Bottom -= LineWidthBitmapSizes[i].cy + 6 ;
      if ( Bottom < 0 ) {
         ColumnStart = i ;
         Left += Width ;
         Width = 0 ;
         Bottom = MaxSize.cy - ( LineWidthBitmapSizes[i].cy + 6 ) ;
      } /* endif */
      Width = max ( Width, LineWidthBitmapSizes[i].cx + 50 ) ;
      if ( Width > MaxSize.cx-Left ) {
         i = ColumnStart ;
         break ;
      } /* endif */
   } /* endfor */

   int LastEntry = i - 1 ;

  /**************************************************************************
   * If we aren't showing the start of the list, prepend a page-up entry.   *
   **************************************************************************/

   if ( FirstEntry ) {
      ResourceString More ( Library, IDS_MORE_PGUP ) ;
      MENUITEM MenuItem ;
      MenuItem.iPosition = MIT_END ;
      MenuItem.afAttribute = 0 ;
      MenuItem.hwndSubMenu = 0 ;
      MenuItem.id = IDM_LINEWIDTHS_PGUP ;
      MenuItem.afStyle = MIS_TEXT ;
      Sys_SendMessage ( Menu, MM_INSERTITEM, MPFROMP(&MenuItem), MPFROMP(PSZ(More)) ) ;
   } /* endif */

  /**************************************************************************
   * Build the menu entries.                                                *
   **************************************************************************/

   Left = 0 ;
   Width = 0 ;
   Bottom = MaxSize.cy - ( FirstEntry ? 50 : 0 ) ;

   for ( i=FirstEntry; i<=LastEntry; i++ ) {

     /***********************************************************************
      * Set the item to break the menu into columns if necessary.           *
      ***********************************************************************/

      MenuItem.afStyle = MIS_BITMAP ;
      Bottom -= LineWidthBitmapSizes[i].cy + 6 ;
      if ( Bottom < 0 ) {
         MenuItem.afStyle |= MIS_BREAKSEPARATOR ;
         Bottom = MaxSize.cy - ( LineWidthBitmapSizes[i].cy + 6 ) ;
      } /* endif */

     /***********************************************************************
      * Set the normal bitmap.                                              *
      ***********************************************************************/

      MenuItem.hItem = ULONG ( LineWidthBitmaps[i] ) ;

     /***********************************************************************
      * Add the menu entry.                                                 *
      ***********************************************************************/

      Sys_SendMessage ( Menu, MM_INSERTITEM, MPFROMP(&MenuItem), MPFROMP("") ) ;

     /***********************************************************************
      * Check-mark the current width.                                       *
      ***********************************************************************/

      if ( State.LineWidth == LineWidths[i] ) {
         Sys_SendMessage ( Menu, MM_SETITEMATTR, MPFROM2SHORT(IDM_LINEWIDTHS+i,TRUE), MPFROM2SHORT(MIA_CHECKED,MIA_CHECKED) ) ;
      } else {
         Sys_SendMessage ( Menu, MM_SETITEMATTR, MPFROM2SHORT(IDM_LINEWIDTHS+i,TRUE), MPFROM2SHORT(MIA_CHECKED,0) ) ;
      } /* endif */

     /***********************************************************************
      * Get the next menu item ID.                                          *
      ***********************************************************************/

      MenuItem.id ++ ;
   }

  /**************************************************************************
   * If we didn't show the end of the list, append a page-down entry.       *
   **************************************************************************/

   if ( LastEntry < LineWidthCount-1 ) {
      ResourceString More ( Library, IDS_MORE_PGDN ) ;
      MENUITEM MenuItem ;
      MenuItem.iPosition = MIT_END ;
      MenuItem.afAttribute = 0 ;
      MenuItem.hwndSubMenu = 0 ;
      MenuItem.id = IDM_LINEWIDTHS_PGDN ;
      MenuItem.afStyle = MIS_TEXT ;
      Sys_SendMessage ( Menu, MM_INSERTITEM, MPFROMP(&MenuItem), MPFROMP(PSZ(More)) ) ;
   } /* endif */

  /**************************************************************************
   * In any case, add on a separator and the menu manager.                  *
   **************************************************************************/

   #ifdef MANAGERS
   MenuItem.iPosition = MIT_END ;
   MenuItem.afAttribute = 0 ;
   MenuItem.hwndSubMenu = 0 ;
   MenuItem.id = 0 ;
   MenuItem.afStyle = MIS_SEPARATOR ;
   MenuItem.hItem = 0 ;
   Sys_SendMessage ( Menu, MM_INSERTITEM, MPFROMP(&MenuItem), MPFROMP("") ) ;

   ResourceString Manager ( Library, IDS_MANAGER ) ;
   MenuItem.iPosition = MIT_END ;
   MenuItem.afAttribute = 0 ;
   MenuItem.hwndSubMenu = 0 ;
   MenuItem.id = IDM_LINEWIDTHMGR ;
   MenuItem.afStyle = MIS_TEXT ;
   MenuItem.hItem = 0 ;
   Sys_SendMessage ( Menu, MM_INSERTITEM, MPFROMP(&MenuItem), MPFROMP(PSZ(Manager)) ) ;
   #endif
}
 
/****************************************************************************
 *                                                                          *
 *      Line Width Menu Initialization Thread                               *
 *                                                                          *
 ****************************************************************************/

static VOID LineWidthInitThread ( PVOID Parameter ) {

  #ifdef DEBUG
     Log ( "LineWidth Init thread started." ) ;
  #endif

 /***************************************************************************
  * Get parameters.                                                         *
  ***************************************************************************/

  PLINEWIDTHINIT_PARMS Parms = PLINEWIDTHINIT_PARMS ( Parameter ) ;

 /***************************************************************************
  * Set up for GPI calls.                                                   *
  ***************************************************************************/

   Process *Proc = 0 ;
   if ( Parms->Threaded ) 
      Proc = new Process ( "LineWidthInitThread", 0, LibraryHandle ) ;

 /***************************************************************************
  * Expand the bitmap handle array, if necessary.                           *
  ***************************************************************************/

  if ( Parms->NewCount > LineWidthCount ) {
     PHBITMAP Bitmaps = PHBITMAP ( malloc ( Parms->NewCount * sizeof(HBITMAP) ) ) ;
     memset ( Bitmaps, 0, Parms->NewCount * sizeof(HBITMAP) ) ;
     if ( LineWidthBitmaps ) {
        memcpy ( Bitmaps, LineWidthBitmaps, LineWidthCount * sizeof(HBITMAP) ) ;
        free ( LineWidthBitmaps ) ;
     } /* endif */
     LineWidthBitmaps = Bitmaps ;
     PSIZEL BitmapSizes = PSIZEL ( malloc ( Parms->NewCount * sizeof(SIZEL) ) ) ;
     memset ( BitmapSizes, 0, Parms->NewCount * sizeof(SIZEL) ) ;
     if ( LineWidthBitmapSizes ) {
        memcpy ( BitmapSizes, LineWidthBitmapSizes, LineWidthCount * sizeof(SIZEL) ) ;
        free ( LineWidthBitmapSizes ) ;
     } /* endif */
     LineWidthBitmapSizes = BitmapSizes ;
  } /* endif */

 /***************************************************************************
  * Update the bitmaps.                                                     *
  ***************************************************************************/

  UpdateLineWidthBitmaps ( Proc ? Proc->QueryAnchor() : 0, LineWidthCount, LineWidths, Parms->NewCount, Parms->NewWidths, LineWidthBitmaps, LineWidthBitmapSizes ) ;

 /***************************************************************************
  * Save the new line width list and count.                                 *
  ***************************************************************************/

  LineWidthCount = Parms->NewCount ;

  if ( LineWidths != Parms->NewWidths ) {
     if ( LineWidths )
        free ( LineWidths ) ;
     LineWidths = PULONG ( malloc ( LineWidthCount * sizeof(ULONG) ) ) ;
     memcpy ( LineWidths, Parms->NewWidths, LineWidthCount * sizeof(ULONG) ) ;
  } /* endif */

 /***************************************************************************
  * Notify the window that wanted to know when the thread was finished.     *
  ***************************************************************************/

  if ( Parms->WindowToNotify )
     if ( !Sys_PostMessage ( Parms->WindowToNotify, WM_THREAD_DONE, MPFROMLONG(Sys_ThreadNumber(*_threadid)), 0 ) ) {
        ERRORID Code = Sys_GetLastError ( Proc ? Proc->QueryAnchor() : 0 ) ;
        Log ( "WARNING: Attempt to notify window of line width init thread completion failed.  Error %lX.", Code ) ;
     } /* endif */

 /***************************************************************************
  * Release PM resources.                                                   *
  ***************************************************************************/

   if ( Parms->Threaded ) 
      delete Proc ;

  /**************************************************************************
   * Release the parameter memory.                                          *
   **************************************************************************/

   free ( Parms ) ;

 /***************************************************************************
  * We're done.                                                             *
  ***************************************************************************/

  LineWidthDone.Post() ;
  LineWidthThread = 0 ;

  #ifdef DEBUG
     Log ( "LineWidth Init thread done." ) ;
  #endif
}
 
/****************************************************************************
 *                                                                          *
 *  Update Line Width Bitmaps                                               *
 *                                                                          *
 ****************************************************************************/

static BOOL UpdateLineWidthBitmaps (
  HAB Anchor,
  int OldCount,
  ULONG OldWidths[],
  int NewCount,
  ULONG NewWidths[],
  HBITMAP Bitmaps[],
  SIZEL BitmapSizes[]
) {

 /***************************************************************************
  * Reconcile the new line widths against the old, deleting obsolete bitmaps*
  ***************************************************************************/

  qsort ( NewWidths, (size_t)NewCount, sizeof(NewWidths[0]), CompareULONGs ) ;

  PHBITMAP OldBitmaps = 0 ;
  if ( OldCount ) {
     OldBitmaps = PHBITMAP ( malloc ( sizeof(Bitmaps[0]) * OldCount ) ) ;
     memcpy ( OldBitmaps, Bitmaps, sizeof(Bitmaps[0])*OldCount ) ;
  } /* endif */

  memset ( Bitmaps, 0, sizeof(Bitmaps[0])*NewCount) ;

  BOOL AlteredList = FALSE ;
  BOOL NeedNewBitmaps = FALSE ;

  int i=0, j=0;
  while ( i<OldCount OR j<NewCount ) {
     if ( i>=NewCount ) {
        if ( OldBitmaps[i] )
           GpiDeleteBitmap ( OldBitmaps[i] ) ;
        AlteredList = TRUE ;
        i ++ ;
     } else if ( j>=OldCount ) {
        AlteredList = TRUE ;
        NeedNewBitmaps = TRUE ;
        j ++ ;
     } else {
        int Compare = CompareULONGs ( &OldWidths[i], &NewWidths[j] ) ;
        if ( Compare == 0 ) {
           if ( OldBitmaps[i] )
              Bitmaps[j] = OldBitmaps[i] ;
           else
              NeedNewBitmaps = TRUE ;
           i++, j++;
        } else if ( Compare < 0 ) {
          if ( OldBitmaps[i] )
            GpiDeleteBitmap ( OldBitmaps[i] ) ;
          AlteredList = TRUE ;
          i ++ ;
        } else {
          AlteredList = TRUE ;
          NeedNewBitmaps = TRUE ;
          j ++ ;
        } /* endif */
     } /* endif */
  } /* endwhile */

  if ( OldBitmaps )
    free ( OldBitmaps ) ;

  if ( NOT NeedNewBitmaps )
     return ( AlteredList ) ;

  /**************************************************************************
   * Open a memory device context.                                          *
   **************************************************************************/

   DeviceContext Memory = DeviceContext ( "UpdateLineWidthBitmaps", long(Anchor) ) ;

  /**************************************************************************
   * Create a memory presentation space for painting bitmaps.               *
   **************************************************************************/

   WorkSpace PS = WorkSpace ( "UpdateLineWidthBitmaps", 0, &Memory, Anchor, FALSE, TRUE ) ;

 /***************************************************************************
  * Initialize the bitmap header.                                           *
  ***************************************************************************/

  BITMAPINFOHEADER2 BitmapHeader ;
  memset ( &BitmapHeader, 0, sizeof(BitmapHeader) ) ;
  BitmapHeader.cbFix = 16 ;
  BitmapHeader.cx = 64 ;
  BitmapHeader.cy = 20 ;
  BitmapHeader.cPlanes = 1 ;
  BitmapHeader.cBitCount = 1 ;

  POINTL BitmapSize = { 64, 20 } ;
  PS.Transform ( CVTC_DEVICE, CVTC_DEFAULTPAGE, BitmapSize ) ;

 /***************************************************************************
  * For each line width to be shown in the menu . . .                       *
  ***************************************************************************/

  for ( i=0; i<NewCount && Active; i++ ) {

   /*************************************************************************
    * If new bitmap not needed, skip this one.                              *
    *************************************************************************/

    if ( Bitmaps[i] )
       continue ;

   /*************************************************************************
    * Create a bitmap for the menu item.                                    *
    *************************************************************************/

    Bitmaps[i] = PS.CreateBitmap ( BitmapHeader ) ;
    PS.SetBitmap ( Bitmaps[i] ) ;

   /*************************************************************************
    * Draw an example of the current line width on the bitmap.              *
    *************************************************************************/

    PS.SetColor ( CLR_BLACK ) ;
    RECTL Box = { 0, 0, BitmapSize.x-1, BitmapSize.y-1 } ;
    PS.FillBox ( Box ) ;

    PS.SetColor ( CLR_WHITE ) ;
    PS.SetLineWidth ( int(NewWidths[i]) ) ;
    PS.Move ( 0, int(BitmapSize.y/2) ) ;
    PS.DrawLineGeom ( int(BitmapSize.x-1), int(BitmapSize.y/2) ) ;

   /*************************************************************************
    * Measure the bitmap.                                                   *
    *************************************************************************/

    BITMAPINFOHEADER2 BitmapInfoHeader ;
    BitmapInfoHeader.cbFix = sizeof(BitmapInfoHeader) ;
    GpiQueryBitmapInfoHeader ( Bitmaps[i], &BitmapInfoHeader ) ;
    BitmapSizes[i].cx = BitmapInfoHeader.cx ;
    BitmapSizes[i].cy = BitmapInfoHeader.cy ;

  } /* endfor */

  return ( TRUE ) ;
}



 
/****************************************************************************
 ****************************************************************************
 ***                                                                      ***
 ***             Fill Color Menu Management Functions & Data              ***
 ***                                                                      ***
 ****************************************************************************
 ****************************************************************************/

typedef struct {
   BOOL      Threaded ;
   HMODULE   Library ;
   HWND      WindowToNotify ;
   USHORT    NewCount ;
   PCOLOR    NewColors ;
} FILLCOLORINIT_PARMS, *PFILLCOLORINIT_PARMS ;

static VOID FillColorInitThread ( PVOID Parameter ) ;

static BOOL UpdateFillColorBitmaps (
  HAB Anchor,
  int OldCount,
  COLOR OldColors[],
  int NewCount,
  COLOR NewColors[],
  HBITMAP Bitmaps[],
  SIZEL BitmapSizes[]
) ;

static int _Optlink CompareCOLORs ( const void *Item1, const void *Item2 ) ;

static PHBITMAP     FillColorBitmaps = 0 ;
static PSIZEL       FillColorBitmapSizes = 0 ;
static USHORT       FillColorCount = 0 ;
static Event        FillColorDone ( "MenuBmp-FillColorDone" ) ;
static PCOLOR       FillColors = 0 ;
static Mutex        FillColorSemaphore ( "MenuBmp-FillColorSemaphore" ) ;
static TID          FillColorThread = 0 ;

 
/****************************************************************************
 *                                                                          *
 *  Start thread to initialize fill color bitmaps.                          *
 *                                                                          *
 ****************************************************************************/

extern void InitFillColorBitmaps (
   HMODULE      Library,        // -> Resource Library
   HWND         WindowToNotify, // -> Window to notify upon completion.
   USHORT       NewCount,       // -> New Color Count
   PCOLOR       NewColors       // -> New Colors
) {

 /***************************************************************************
  * Wait for previous initialization to complete before starting it anew.   *
  ***************************************************************************/

  if ( long(FillColorThread) > 0 ) {
     DosSetPriority ( PRTYS_THREAD, PRTYC_FOREGROUNDSERVER, +20, FillColorThread ) ;
     FillColorDone.Wait() ;
  } /* endif */

 /***************************************************************************
  * Allocate and initialize the thread parameters.                          *
  ***************************************************************************/

  PFILLCOLORINIT_PARMS Parms = PFILLCOLORINIT_PARMS ( malloc ( sizeof(FILLCOLORINIT_PARMS) ) ) ;
  Parms->Threaded = TRUE ;
  Parms->Library = Library ;
  Parms->WindowToNotify = WindowToNotify ;
  Parms->NewCount = NewCount ;
  Parms->NewColors = NewColors ;

 /***************************************************************************
  * Start the initialization thread.                                        *
  ***************************************************************************/

  #ifdef THREADED
    Sys_SendMessage ( WindowToNotify, WM_THREAD_STARTED, 0, 0 ) ;
    FillColorDone.Reset() ;
    FillColorThread = StartThread ( "FillColorInitThread", FillColorInitThread, 0x10000, Parms, PRTYC_IDLETIME, +20 ) ;
  #else
    Parms->Threaded = FALSE ;
    Parms->WindowToNotify = 0 ;
    FillColorThread = 0 ;
    FillColorInitThread ( Parms ) ;
  #endif
}
 
/****************************************************************************
 *                                                                          *
 *  Initialize the Fill Color Menu                                          *
 *                                                                          *
 ****************************************************************************/

extern void InitFillColorMenu (
   HWND         Menu,           // -> Menu
   Estado      &State,          // -> Current State
   HMODULE      Library,        // -> Resource Library
   int          FirstEntry,     // Offset of first entry to display.
   USHORT       NewCount,       // -> New Color Count
   PCOLOR       NewColors       // -> New Colors (or NULL to use old ones)
) {

  /**************************************************************************
   * Set the hourglass for a bit.                                           *
   **************************************************************************/

   HourGlass Hold ( Menu ) ;

  /**************************************************************************
   * Wait for any color menu update in progress to complete.                *
   **************************************************************************/

   if ( long(FillColorThread) > 0 ) {
      DosSetPriority ( PRTYS_THREAD, PRTYC_FOREGROUNDSERVER, +20, FillColorThread ) ;
      FillColorDone.Wait() ;
   } /* endif */

  /**************************************************************************
   * Update it again, since we don't know how long ago the update was.      *
   **************************************************************************/

   PFILLCOLORINIT_PARMS Parms = PFILLCOLORINIT_PARMS ( malloc ( sizeof(FILLCOLORINIT_PARMS) ) ) ;
   Parms->Threaded = FALSE ;
   Parms->Library = Library ;
   Parms->WindowToNotify = 0 ;
   Parms->NewCount = NewColors ? NewCount : FillColorCount ;
   Parms->NewColors = NewColors ? NewColors : FillColors ;
   FillColorInitThread ( Parms ) ;

  /**************************************************************************
   * Clear out the old menu.                                                *
   **************************************************************************/

   LONG OldCount = LONGFROMMR ( Sys_SendMessage ( Menu, MM_QUERYITEMCOUNT, 0, 0 ) ) ;
   for ( int i=0; i<OldCount; i++ ) {
      LONG Id = LONGFROMMR ( Sys_SendMessage ( Menu, MM_ITEMIDFROMPOSITION, MPFROMSHORT(0), 0 ) ) ;
      Sys_SendMessage ( Menu, MM_REMOVEITEM, MPFROM2SHORT(Id,TRUE), 0 ) ;
   } /* endfor */

  /**************************************************************************
   * Initialize the menu item structure.                                    *
   **************************************************************************/

   MENUITEM MenuItem ;
   MenuItem.iPosition = MIT_END ;
   MenuItem.afAttribute = 0 ;
   MenuItem.hwndSubMenu = 0 ;
   MenuItem.id = USHORT ( IDM_FILLCOLORS + FirstEntry ) ;

  /**************************************************************************
   * Determine the maximum menu height & width.                             *
   **************************************************************************/

   RECTL Desktop ;
   WinQueryWindowRect ( HWND_DESKTOP, &Desktop ) ;

   SWP Position ;
   WinQueryWindowPos ( OWNER(Menu), &Position ) ;

   POINTL Points[2] ;
   Points[0].x = Points[1].x = Position.x ;
   Points[0].y = Position.y ;  Points[1].y = Position.y + Position.cy ;
   WinMapWindowPoints ( PARENT(OWNER(Menu)), HWND_DESKTOP, PPOINTL(&Points), 2 ) ;

   SIZEL MaxSize ;
   MaxSize.cx = Desktop.xRight - Desktop.xLeft ;
   MaxSize.cy = max ( Desktop.yTop-Points[1].y, Points[0].y-Desktop.yBottom ) ;

  /**************************************************************************
   * Determine all the bitmap sizes.                                        *
   **************************************************************************/

   int ColumnStart = FirstEntry ;
   LONG Left = 0 ;
   LONG Width = 0 ;
   LONG Bottom = MaxSize.cy - ( FirstEntry ? 50 : 0 ) ;

   for ( i=FirstEntry; i<FillColorCount; i++ ) {
      Bottom -= FillColorBitmapSizes[i].cy + 6 ;
      if ( Bottom < 0 ) {
         ColumnStart = i ;
         Left += Width ;
         Width = 0 ;
         Bottom = MaxSize.cy - ( FillColorBitmapSizes[i].cy + 6 ) ;
      } /* endif */
      Width = max ( Width, FillColorBitmapSizes[i].cx + 50 ) ;
      if ( Width > MaxSize.cx-Left ) {
         i = ColumnStart ;
         break ;
      } /* endif */
   } /* endfor */

   int LastEntry = i - 1 ;

  /**************************************************************************
   * If we aren't showing the start of the list, prepend a page-up entry.   *
   **************************************************************************/

   if ( FirstEntry ) {
      ResourceString More ( Library, IDS_MORE_PGUP ) ;
      MENUITEM MenuItem ;
      MenuItem.iPosition = MIT_END ;
      MenuItem.afAttribute = 0 ;
      MenuItem.hwndSubMenu = 0 ;
      MenuItem.id = IDM_FILLCOLORS_PGUP ;
      MenuItem.afStyle = MIS_TEXT ;
      Sys_SendMessage ( Menu, MM_INSERTITEM, MPFROMP(&MenuItem), MPFROMP(PSZ(More)) ) ;
   } /* endif */

  /**************************************************************************
   * Build the menu entries.                                                *
   **************************************************************************/

   Left = 0 ;
   Width = 0 ;
   Bottom = MaxSize.cy - ( FirstEntry ? 50 : 0 ) ;

   for ( i=FirstEntry; i<=LastEntry; i++ ) {

     /***********************************************************************
      * Set the item to break the menu into columns if necessary.           *
      ***********************************************************************/

      MenuItem.afStyle = MIS_BITMAP ;
      Bottom -= FillColorBitmapSizes[i].cy + 6 ;
      if ( Bottom < 0 ) {
         MenuItem.afStyle |= MIS_BREAKSEPARATOR ;
         Bottom = MaxSize.cy - ( FillColorBitmapSizes[i].cy + 6 ) ;
      } /* endif */

     /***********************************************************************
      * Set the normal bitmap.                                              *
      ***********************************************************************/

      MenuItem.hItem = ULONG ( FillColorBitmaps[i] ) ;

     /***********************************************************************
      * Add the menu entry.                                                 *
      ***********************************************************************/

      Sys_SendMessage ( Menu, MM_INSERTITEM, MPFROMP(&MenuItem), MPFROMP("") ) ;

     /***********************************************************************
      * Check-mark the current color.                                       *
      ***********************************************************************/

      if ( State.FillColor == FillColors[i] ) {
         Sys_SendMessage ( Menu, MM_SETITEMATTR, MPFROM2SHORT(IDM_FILLCOLORS+i,TRUE), MPFROM2SHORT(MIA_CHECKED,MIA_CHECKED) ) ;
      } else {
         Sys_SendMessage ( Menu, MM_SETITEMATTR, MPFROM2SHORT(IDM_FILLCOLORS+i,TRUE), MPFROM2SHORT(MIA_CHECKED,0) ) ;
      } /* endif */

     /***********************************************************************
      * Get the next menu item ID.                                          *
      ***********************************************************************/

      MenuItem.id ++ ;
   }

  /**************************************************************************
   * If we didn't show the end of the list, append a page-down entry.       *
   **************************************************************************/

   if ( LastEntry < FillColorCount-1 ) {
      ResourceString More ( Library, IDS_MORE_PGDN ) ;
      MENUITEM MenuItem ;
      MenuItem.iPosition = MIT_END ;
      MenuItem.afAttribute = 0 ;
      MenuItem.hwndSubMenu = 0 ;
      MenuItem.id = IDM_FILLCOLORS_PGDN ;
      MenuItem.afStyle = MIS_TEXT ;
      Sys_SendMessage ( Menu, MM_INSERTITEM, MPFROMP(&MenuItem), MPFROMP(PSZ(More)) ) ;
   } /* endif */

  /**************************************************************************
   * In any case, add on a separator and the menu manager.                  *
   **************************************************************************/

   #ifdef MANAGERS
   MenuItem.iPosition = MIT_END ;
   MenuItem.afAttribute = 0 ;
   MenuItem.hwndSubMenu = 0 ;
   MenuItem.id = 0 ;
   MenuItem.afStyle = MIS_SEPARATOR ;
   MenuItem.hItem = 0 ;
   Sys_SendMessage ( Menu, MM_INSERTITEM, MPFROMP(&MenuItem), MPFROMP("") ) ;

   ResourceString Manager ( Library, IDS_MANAGER ) ;
   MenuItem.iPosition = MIT_END ;
   MenuItem.afAttribute = 0 ;
   MenuItem.hwndSubMenu = 0 ;
   MenuItem.id = IDM_FILLCOLORMGR ;
   MenuItem.afStyle = MIS_TEXT ;
   MenuItem.hItem = 0 ;
   Sys_SendMessage ( Menu, MM_INSERTITEM, MPFROMP(&MenuItem), MPFROMP(PSZ(Manager)) ) ;
   #endif
}
 
/****************************************************************************
 *                                                                          *
 *      Fill Color Menu Initialization Thread                               *
 *                                                                          *
 ****************************************************************************/

static VOID FillColorInitThread ( PVOID Parameter ) {

   #ifdef DEBUG
      Log ( "FillColor Init thread started." ) ;
   #endif

  /**************************************************************************
   * Get parameters.                                                        *
   **************************************************************************/

   PFILLCOLORINIT_PARMS Parms = PFILLCOLORINIT_PARMS ( Parameter ) ;

  /**************************************************************************
   * Set up for GPI calls.                                                  *
   **************************************************************************/

   Process *Proc = 0 ;
   if ( Parms->Threaded ) 
      Proc = new Process ( "FillColorInitThread", 0, LibraryHandle ) ;

  /**************************************************************************
   * Expand the bitmap handle array, if necessary.                          *
   **************************************************************************/

   if ( Parms->NewCount > FillColorCount ) {
      PHBITMAP Bitmaps = PHBITMAP ( malloc ( Parms->NewCount * sizeof(HBITMAP) ) ) ;
      memset ( Bitmaps, 0, Parms->NewCount * sizeof(HBITMAP) ) ;
      if ( FillColorBitmaps ) {
         memcpy ( Bitmaps, FillColorBitmaps, FillColorCount * sizeof(HBITMAP) ) ;
         free ( FillColorBitmaps ) ;
      } /* endif */
      FillColorBitmaps = Bitmaps ;
      PSIZEL BitmapSizes = PSIZEL ( malloc ( Parms->NewCount * sizeof(SIZEL) ) ) ;
      memset ( BitmapSizes, 0, Parms->NewCount * sizeof(SIZEL) ) ;
      if ( FillColorBitmapSizes ) {
         memcpy ( BitmapSizes, FillColorBitmapSizes, FillColorCount * sizeof(SIZEL) ) ;
         free ( FillColorBitmapSizes ) ;
      } /* endif */
      FillColorBitmapSizes = BitmapSizes ;
   } /* endif */

  /**************************************************************************
   * Update the bitmaps.                                                    *
   **************************************************************************/

   UpdateFillColorBitmaps ( Proc ? Proc->QueryAnchor() : 0, FillColorCount, FillColors, Parms->NewCount, Parms->NewColors, FillColorBitmaps, FillColorBitmapSizes ) ;

  /**************************************************************************
   * Save the new color list and count.                                     *
   **************************************************************************/

   FillColorCount = Parms->NewCount ;

   if ( FillColors != Parms->NewColors ) {
      if ( FillColors )
         free ( FillColors ) ;
      FillColors = PCOLOR ( malloc ( FillColorCount * sizeof(COLOR) ) ) ;
      memcpy ( FillColors, Parms->NewColors, FillColorCount * sizeof(COLOR) ) ;
   } /* endif */

  /**************************************************************************
   * Notify the window that wanted to know when the thread was finished.    *
   **************************************************************************/

   if ( Parms->WindowToNotify )
      if ( !Sys_PostMessage ( Parms->WindowToNotify, WM_THREAD_DONE, MPFROMLONG(Sys_ThreadNumber(*_threadid)), 0 ) ) {
         ERRORID Code = Sys_GetLastError ( Proc ? Proc->QueryAnchor() : 0 ) ;
         Log ( "WARNING: Attempt to notify window of fill color init thread completion failed.  Error %lX.", Code ) ;
      } /* endif */

  /**************************************************************************
   * Release PM resources.                                                  *
   **************************************************************************/

   if ( Parms->Threaded ) 
      delete Proc ;

  /**************************************************************************
   * Release the parameter memory.                                          *
   **************************************************************************/

   free ( Parms ) ;

  /**************************************************************************
   * We're done.                                                            *
   **************************************************************************/

   FillColorDone.Post() ;
   FillColorThread = 0 ;

   #ifdef DEBUG
      Log ( "FillColor Init thread done." ) ;
   #endif
}
 
/****************************************************************************
 *                                                                          *
 *  Update Fill Color Bitmaps                                               *
 *                                                                          *
 ****************************************************************************/

static BOOL UpdateFillColorBitmaps (
   HAB Anchor,
   int OldCount,
   COLOR OldColors[],
   int NewCount,
   COLOR NewColors[],
   HBITMAP Bitmaps[],
   SIZEL BitmapSizes[]
) {

  /**************************************************************************
   * Reconcile the new colors against the old, deleting obsolete bitmaps.   *
   **************************************************************************/

   qsort ( NewColors, (size_t)NewCount, sizeof(NewColors[0]), CompareCOLORs ) ;

   PHBITMAP OldBitmaps = 0 ;
   if ( OldCount ) {
      OldBitmaps = PHBITMAP ( malloc ( sizeof(Bitmaps[0]) * OldCount ) ) ;
      memcpy ( OldBitmaps, Bitmaps, sizeof(Bitmaps[0])*OldCount ) ;
   } /* endif */

   memset ( Bitmaps, 0, sizeof(Bitmaps[0])*NewCount) ;

   BOOL AlteredList = FALSE ;
   BOOL NeedNewBitmaps = FALSE ;

   int i=0, j=0;
   while ( i<OldCount OR j<NewCount ) {
      if ( i>=NewCount ) {
         if ( OldBitmaps[i] )
            GpiDeleteBitmap ( OldBitmaps[i] ) ;
         AlteredList = TRUE ;
         i ++ ;
      } else if ( j>=OldCount ) {
         AlteredList = TRUE ;
         NeedNewBitmaps = TRUE ;
         j ++ ;
      } else {
         int Compare = CompareCOLORs ( &OldColors[i], &NewColors[j] ) ;
         if ( Compare == 0 ) {
            if ( OldBitmaps[i] )
               Bitmaps[j] = OldBitmaps[i] ;
            else
               NeedNewBitmaps = TRUE ;
            i++, j++;
         } else if ( Compare < 0 ) {
            if ( OldBitmaps[i] )
               GpiDeleteBitmap ( OldBitmaps[i] ) ;
            AlteredList = TRUE ;
            i ++ ;
         } else {
            AlteredList = TRUE ;
            NeedNewBitmaps = TRUE ;
            j ++ ;
         } /* endif */
      } /* endif */
   } /* endwhile */
 
   if ( OldBitmaps )
      free ( OldBitmaps ) ;

   if ( NOT NeedNewBitmaps )
      return ( AlteredList ) ;

  /**************************************************************************
   * Open a memory device context.                                          *
   **************************************************************************/

   DeviceContext Memory = DeviceContext ( "UpdateFillColorBitmaps", long(Anchor) ) ;

  /**************************************************************************
   * Create a memory presentation space for painting bitmaps.               *
   **************************************************************************/

   WorkSpace PS = WorkSpace ( "UpdateFillColorBitmaps", 0, &Memory, Anchor, TRUE ) ;

  /**************************************************************************
   * Find out what kind of bitmaps the memory PS supports.                  *
   **************************************************************************/

   int Planes, BitCount ;
   PS.QueryBitmapFormat ( Planes, BitCount ) ;

  /**************************************************************************
   * Initialize the bitmap header.                                          *
   **************************************************************************/

   BITMAPINFOHEADER2 BitmapHeader ;
   memset ( &BitmapHeader, 0, sizeof(BitmapHeader) ) ;
   BitmapHeader.cbFix = 16 ;
   BitmapHeader.cx = 64 ;
   BitmapHeader.cy = 20 ;
   BitmapHeader.cPlanes = USHORT ( Planes ) ;
   BitmapHeader.cBitCount = USHORT ( BitCount ) ;

  /**************************************************************************
   * For each color to be shown in the menu . . .                           *
   **************************************************************************/

   for ( i=0; i<NewCount && Active; i++ ) {

     /***********************************************************************
      * If new bitmap not needed, skip this one.                            *
      ***********************************************************************/

      if ( Bitmaps[i] )
         continue ;

     /***********************************************************************
      * Create a bitmap for the menu item.                                  *
      ***********************************************************************/

      Bitmaps[i] = PS.CreateBitmap ( BitmapHeader ) ;
      PS.SetBitmap ( Bitmaps[i] ) ;

     /***********************************************************************
      * Paint the bitmap with the current color.                            *
      ***********************************************************************/

      PS.SetColor ( NewColors[i] ) ;
      RECTL Box = { 1, 1, BitmapHeader.cx-2, BitmapHeader.cy-2 } ;
      PS.FillBox ( Box ) ;

      PS.SetColor ( RGB_BLACK ) ;
      Box.xLeft = Box.yBottom = 0 ;
      Box.xRight = BitmapHeader.cx - 1 ;
      Box.yTop = BitmapHeader.cy - 1 ;
      PS.DrawBox ( Box ) ;

     /***********************************************************************
      * Measure the bitmap.                                                 *
      ***********************************************************************/

      BITMAPINFOHEADER2 BitmapInfoHeader ;
      BitmapInfoHeader.cbFix = sizeof(BitmapInfoHeader) ;
      GpiQueryBitmapInfoHeader ( Bitmaps[i], &BitmapInfoHeader ) ;
      BitmapSizes[i].cx = BitmapInfoHeader.cx ;
      BitmapSizes[i].cy = BitmapInfoHeader.cy ;

   } /* endfor */

   return ( TRUE ) ;
}



 
/****************************************************************************
 ****************************************************************************
 ***                                                                      ***
 ***             Fill Type Menu Management Functions & Data               ***
 ***                                                                      ***
 ****************************************************************************
 ****************************************************************************/

typedef struct {
   BOOL      Threaded ;
   HMODULE   Library ;
   HWND      WindowToNotify ;
   USHORT    NewCount ;
   PULONG    NewTypes ;
} FILLTYPEINIT_PARMS, *PFILLTYPEINIT_PARMS ;

static VOID FillTypeInitThread ( PVOID Parameter ) ;

static BOOL UpdateFillTypeBitmaps (
  HAB Anchor,
  int OldCount,
  ULONG OldTypes[],
  int NewCount,
  ULONG NewTypes[],
  HBITMAP Bitmaps[],
  SIZEL BitmapSizes[]
) ;

static PHBITMAP     FillTypeBitmaps = 0 ;
static PSIZEL       FillTypeBitmapSizes = 0 ;
static USHORT       FillTypeCount = 0 ;
static Event        FillTypeDone ( "MenuBmp-FillTypeDone" ) ;
static PULONG       FillTypes = 0 ;
static Mutex        FillTypeSemaphore ( "MenuBmp-FillTypeSemaphore" ) ;
static TID          FillTypeThread = 0 ;

 
/****************************************************************************
 *                                                                          *
 *  Start thread to initialize fill type bitmaps.                           *
 *                                                                          *
 ****************************************************************************/

extern void InitFillTypeBitmaps (
   HMODULE      Library,        // -> Resource Library
   HWND         WindowToNotify, // -> Window to notify upon completion.
   USHORT       NewCount,       // -> New Type Count
   PULONG       NewTypes        // -> New Types
) {

 /***************************************************************************
  * Wait for previous initialization to complete before starting it anew.   *
  ***************************************************************************/

  if ( long(FillTypeThread) > 0 ) {
     DosSetPriority ( PRTYS_THREAD, PRTYC_FOREGROUNDSERVER, +20, FillTypeThread ) ;
     FillTypeDone.Wait() ;
  } /* endif */

 /***************************************************************************
  * Allocate and initialize the thread parameters.                          *
  ***************************************************************************/

  PFILLTYPEINIT_PARMS Parms = PFILLTYPEINIT_PARMS ( malloc ( sizeof(FILLTYPEINIT_PARMS) ) ) ;
  Parms->Threaded = TRUE ;
  Parms->Library = Library ;
  Parms->WindowToNotify = WindowToNotify ;
  Parms->NewCount = NewCount ;
  Parms->NewTypes = NewTypes ;

 /***************************************************************************
  * Start the initialization thread.                                        *
  ***************************************************************************/

  #ifdef THREADED
    Sys_SendMessage ( WindowToNotify, WM_THREAD_STARTED, 0, 0 ) ;
    FillTypeDone.Reset() ;
    FillTypeThread = StartThread ( "FillTypeInitThread", FillTypeInitThread, 0x10000, Parms, PRTYC_IDLETIME, +20 ) ;
  #else
    Parms->Threaded = FALSE ;
    Parms->WindowToNotify = 0 ;
    FillTypeThread = 0 ;
    FillTypeInitThread ( Parms ) ;
  #endif
}
 
/****************************************************************************
 *                                                                          *
 *  Initialize the Fill Type Menu                                           *
 *                                                                          *
 ****************************************************************************/

extern void InitFillTypeMenu (
   HWND         Menu,           // -> Menu
   Estado      &State,          // -> Current State
   HMODULE      Library,        // -> Resource Library
   int          FirstEntry,     // Offset of first entry to display.
   USHORT       NewCount,       // -> New Type Count
   PULONG       NewTypes        // -> New Types (or NULL to use old ones)
) {

  /**************************************************************************
   * Set the hourglass for a bit.                                           *
   **************************************************************************/

   HourGlass Hold ( Menu ) ;

  /**************************************************************************
   * Wait for any type menu update in progress to complete.                 *
   **************************************************************************/

   if ( long(FillTypeThread) > 0 ) {
      DosSetPriority ( PRTYS_THREAD, PRTYC_FOREGROUNDSERVER, +20, FillTypeThread ) ;
      FillTypeDone.Wait() ;
   } /* endif */

  /**************************************************************************
   * Update it again, since we don't know how long ago the update was.      *
   **************************************************************************/

   PFILLTYPEINIT_PARMS Parms = PFILLTYPEINIT_PARMS ( malloc ( sizeof(FILLTYPEINIT_PARMS) ) ) ;
   Parms->Threaded = FALSE ;
   Parms->Library = Library ;
   Parms->WindowToNotify = 0 ;
   Parms->NewCount = NewTypes ? NewCount : FillTypeCount ;
   Parms->NewTypes = NewTypes ? NewTypes : FillTypes ;
   FillTypeInitThread ( Parms ) ;

  /**************************************************************************
   * Clear out the old menu.                                                *
   **************************************************************************/

   LONG OldCount = LONGFROMMR ( Sys_SendMessage ( Menu, MM_QUERYITEMCOUNT, 0, 0 ) ) ;
   for ( int i=0; i<OldCount; i++ ) {
      LONG Id = LONGFROMMR ( Sys_SendMessage ( Menu, MM_ITEMIDFROMPOSITION, MPFROMSHORT(0), 0 ) ) ;
      Sys_SendMessage ( Menu, MM_REMOVEITEM, MPFROM2SHORT(Id,TRUE), 0 ) ;
   } /* endfor */

  /**************************************************************************
   * Initialize the menu item structure.                                    *
   **************************************************************************/

   MENUITEM MenuItem ;
   MenuItem.iPosition = MIT_END ;
   MenuItem.afAttribute = 0 ;
   MenuItem.hwndSubMenu = 0 ;
   MenuItem.id = USHORT ( IDM_FILLTYPES + FirstEntry ) ;

  /**************************************************************************
   * Determine the maximum menu height & width.                             *
   **************************************************************************/

   RECTL Desktop ;
   WinQueryWindowRect ( HWND_DESKTOP, &Desktop ) ;

   SWP Position ;
   WinQueryWindowPos ( OWNER(Menu), &Position ) ;

   POINTL Points[2] ;
   Points[0].x = Points[1].x = Position.x ;
   Points[0].y = Position.y ;  Points[1].y = Position.y + Position.cy ;
   WinMapWindowPoints ( PARENT(OWNER(Menu)), HWND_DESKTOP, PPOINTL(&Points), 2 ) ;

   SIZEL MaxSize ;
   MaxSize.cx = Desktop.xRight - Desktop.xLeft ;
   MaxSize.cy = max ( Desktop.yTop-Points[1].y, Points[0].y-Desktop.yBottom ) ;

  /**************************************************************************
   * Determine all the bitmap sizes.                                        *
   **************************************************************************/

   int ColumnStart = FirstEntry ;
   LONG Left = 0 ;
   LONG Width = 0 ;
   LONG Bottom = MaxSize.cy - ( FirstEntry ? 50 : 0 ) ;

   for ( i=FirstEntry; i<FillTypeCount; i++ ) {
      Bottom -= FillTypeBitmapSizes[i].cy + 6 ;
      if ( Bottom < 0 ) {
         ColumnStart = i ;
         Left += Width ;
         Width = 0 ;
         Bottom = MaxSize.cy - ( FillTypeBitmapSizes[i].cy + 6 ) ;
      } /* endif */
      Width = max ( Width, FillTypeBitmapSizes[i].cx + 50 ) ;
      if ( Width > MaxSize.cx-Left ) {
         i = ColumnStart ;
         break ;
      } /* endif */
   } /* endfor */

   int LastEntry = i - 1 ;

  /**************************************************************************
   * If we aren't showing the start of the list, prepend a page-up entry.   *
   **************************************************************************/

   if ( FirstEntry ) {
      ResourceString More ( Library, IDS_MORE_PGUP ) ;
      MENUITEM MenuItem ;
      MenuItem.iPosition = MIT_END ;
      MenuItem.afAttribute = 0 ;
      MenuItem.hwndSubMenu = 0 ;
      MenuItem.id = IDM_FILLTYPES_PGUP ;
      MenuItem.afStyle = MIS_TEXT ;
      Sys_SendMessage ( Menu, MM_INSERTITEM, MPFROMP(&MenuItem), MPFROMP(PSZ(More)) ) ;
   } /* endif */

  /**************************************************************************
   * Build the menu entries.                                                *
   **************************************************************************/

   Left = 0 ;
   Width = 0 ;
   Bottom = MaxSize.cy - ( FirstEntry ? 50 : 0 ) ;

   for ( i=FirstEntry; i<=LastEntry; i++ ) {

     /***********************************************************************
      * Set the item to break the menu into columns if necessary.           *
      ***********************************************************************/

      MenuItem.afStyle = MIS_BITMAP ;
      Bottom -= FillTypeBitmapSizes[i].cy + 6 ;
      if ( Bottom < 0 ) {
         MenuItem.afStyle |= MIS_BREAKSEPARATOR ;
         Bottom = MaxSize.cy - ( FillTypeBitmapSizes[i].cy + 6 ) ;
      } /* endif */

     /***********************************************************************
      * Set the normal bitmap.                                              *
      ***********************************************************************/

      MenuItem.hItem = ULONG ( FillTypeBitmaps[i] ) ;

     /***********************************************************************
      * Add the menu entry.                                                 *
      ***********************************************************************/

      Sys_SendMessage ( Menu, MM_INSERTITEM, MPFROMP(&MenuItem), MPFROMP("") ) ;

     /***********************************************************************
      * Check-mark the current type.                                        *
      ***********************************************************************/

      if ( State.FillType == FillTypes[i] ) {
         Sys_SendMessage ( Menu, MM_SETITEMATTR, MPFROM2SHORT(IDM_FILLTYPES+i,TRUE), MPFROM2SHORT(MIA_CHECKED,MIA_CHECKED) ) ;
      } else {
         Sys_SendMessage ( Menu, MM_SETITEMATTR, MPFROM2SHORT(IDM_FILLTYPES+i,TRUE), MPFROM2SHORT(MIA_CHECKED,0) ) ;
      } /* endif */

     /***********************************************************************
      * Get the next menu item ID.                                          *
      ***********************************************************************/

      MenuItem.id ++ ;
   }

  /**************************************************************************
   * If we didn't show the end of the list, append a page-down entry.       *
   **************************************************************************/

   if ( LastEntry < FillTypeCount-1 ) {
      ResourceString More ( Library, IDS_MORE_PGDN ) ;
      MENUITEM MenuItem ;
      MenuItem.iPosition = MIT_END ;
      MenuItem.afAttribute = 0 ;
      MenuItem.hwndSubMenu = 0 ;
      MenuItem.id = IDM_FILLTYPES_PGDN ;
      MenuItem.afStyle = MIS_TEXT ;
      Sys_SendMessage ( Menu, MM_INSERTITEM, MPFROMP(&MenuItem), MPFROMP(PSZ(More)) ) ;
   } /* endif */

  /**************************************************************************
   * In any case, add on a separator and the menu manager.                  *
   **************************************************************************/

#if 0
   #ifdef MANAGERS
   MenuItem.iPosition = MIT_END ;
   MenuItem.afAttribute = 0 ;
   MenuItem.hwndSubMenu = 0 ;
   MenuItem.id = 0 ;
   MenuItem.afStyle = MIS_SEPARATOR ;
   MenuItem.hItem = 0 ;
   Sys_SendMessage ( Menu, MM_INSERTITEM, MPFROMP(&MenuItem), MPFROMP("") ) ;

   ResourceString Manager ( Library, IDS_MANAGER ) ;
   MenuItem.iPosition = MIT_END ;
   MenuItem.afAttribute = 0 ;
   MenuItem.hwndSubMenu = 0 ;
   MenuItem.id = IDM_FILLTYPEMGR ;
   MenuItem.afStyle = MIS_TEXT ;
   MenuItem.hItem = 0 ;
   Sys_SendMessage ( Menu, MM_INSERTITEM, MPFROMP(&MenuItem), MPFROMP(PSZ(Manager)) ) ;
   #endif
#endif
}
 
/****************************************************************************
 *                                                                          *
 *      Fill Type Menu Initialization Thread                                *
 *                                                                          *
 ****************************************************************************/

static VOID FillTypeInitThread ( PVOID Parameter ) {

  #ifdef DEBUG
     Log ( "FillType Init thread started." ) ;
  #endif

 /***************************************************************************
  * Get parameters.                                                         *
  ***************************************************************************/

  PFILLTYPEINIT_PARMS Parms = PFILLTYPEINIT_PARMS ( Parameter ) ;

 /***************************************************************************
  * Set up for GPI calls.                                                   *
  ***************************************************************************/

   Process *Proc = 0 ;
   if ( Parms->Threaded ) 
      Proc = new Process ( "FillTypeInitThread", 0, LibraryHandle ) ;

 /***************************************************************************
  * Expand the bitmap handle array, if necessary.                           *
  ***************************************************************************/

  if ( Parms->NewCount > FillTypeCount ) {
     PHBITMAP Bitmaps = PHBITMAP ( malloc ( Parms->NewCount * sizeof(HBITMAP) ) ) ;
     memset ( Bitmaps, 0, Parms->NewCount * sizeof(HBITMAP) ) ;
     if ( FillTypeBitmaps ) {
        memcpy ( Bitmaps, FillTypeBitmaps, FillTypeCount * sizeof(HBITMAP) ) ;
        free ( FillTypeBitmaps ) ;
     } /* endif */
     FillTypeBitmaps = Bitmaps ;
     PSIZEL BitmapSizes = PSIZEL ( malloc ( Parms->NewCount * sizeof(SIZEL) ) ) ;
     memset ( BitmapSizes, 0, Parms->NewCount * sizeof(SIZEL) ) ;
     if ( FillTypeBitmapSizes ) {
        memcpy ( BitmapSizes, FillTypeBitmapSizes, FillTypeCount * sizeof(SIZEL) ) ;
        free ( FillTypeBitmapSizes ) ;
     } /* endif */
     FillTypeBitmapSizes = BitmapSizes ;
  } /* endif */

 /***************************************************************************
  * Update the bitmaps.                                                     *
  ***************************************************************************/

  UpdateFillTypeBitmaps ( Proc ? Proc->QueryAnchor() : 0, FillTypeCount, FillTypes, Parms->NewCount, Parms->NewTypes, FillTypeBitmaps, FillTypeBitmapSizes ) ;

 /***************************************************************************
  * Save the new fill type list and count.                                  *
  ***************************************************************************/

  FillTypeCount = Parms->NewCount ;

  if ( FillTypes != Parms->NewTypes ) {
     if ( FillTypes )
        free ( FillTypes ) ;
     FillTypes = PULONG ( malloc ( FillTypeCount * sizeof(ULONG) ) ) ;
     memcpy ( FillTypes, Parms->NewTypes, FillTypeCount * sizeof(ULONG) ) ;
  } /* endif */

 /***************************************************************************
  * Notify the window that wanted to know when the thread was finished.     *
  ***************************************************************************/

  if ( Parms->WindowToNotify )
     if ( !Sys_PostMessage ( Parms->WindowToNotify, WM_THREAD_DONE, MPFROMLONG(Sys_ThreadNumber(*_threadid)), 0 ) ) {
        ERRORID Code = Sys_GetLastError ( Proc ? Proc->QueryAnchor() : 0 ) ;
        Log ( "WARNING: Attempt to notify window of fill type init thread completion failed.  Error %lX.", Code ) ;
     } /* endif */

 /***************************************************************************
  * Release PM resources.                                                   *
  ***************************************************************************/

   if ( Parms->Threaded ) 
      delete Proc ;

  /**************************************************************************
   * Release the parameter memory.                                          *
   **************************************************************************/

   free ( Parms ) ;

 /***************************************************************************
  * We're done.                                                             *
  ***************************************************************************/

  FillTypeDone.Post() ;
  FillTypeThread = 0 ;

  #ifdef DEBUG
     Log ( "FillType Init thread done." ) ;
  #endif
}
 
/****************************************************************************
 *                                                                          *
 *  Update Fill Type Bitmaps                                                *
 *                                                                          *
 ****************************************************************************/

static BOOL UpdateFillTypeBitmaps (
   HAB Anchor,
   int OldCount,
   ULONG OldTypes[],
   int NewCount,
   ULONG NewTypes[],
   HBITMAP Bitmaps[],
   SIZEL BitmapSizes[]
) {

  /**************************************************************************
   * Reconcile the new fill types against the old, deleting obsolete bitmaps*
   **************************************************************************/

   qsort ( NewTypes, (size_t)NewCount, sizeof(NewTypes[0]), CompareULONGs ) ;

   PHBITMAP OldBitmaps = 0 ;
   if ( OldCount ) {
      OldBitmaps = PHBITMAP ( malloc ( sizeof(Bitmaps[0]) * OldCount ) ) ;
      memcpy ( OldBitmaps, Bitmaps, sizeof(Bitmaps[0])*OldCount ) ;
   } /* endif */

   memset ( Bitmaps, 0, sizeof(Bitmaps[0])*NewCount) ;

   BOOL AlteredList = FALSE ;
   BOOL NeedNewBitmaps = FALSE ;

   int i=0, j=0;
   while ( i<OldCount OR j<NewCount ) {
      if ( i>=NewCount ) {
         if ( OldBitmaps[i] )
            GpiDeleteBitmap ( OldBitmaps[i] ) ;
         AlteredList = TRUE ;
         i ++ ;
      } else if ( j>=OldCount ) {
         AlteredList = TRUE ;
         NeedNewBitmaps = TRUE ;
          j ++ ;
      } else {
         int Compare = CompareULONGs ( &OldTypes[i], &NewTypes[j] ) ;
         if ( Compare == 0 ) {
            if ( OldBitmaps[i] )
               Bitmaps[j] = OldBitmaps[i] ;
            else
               NeedNewBitmaps = TRUE ;
            i++, j++;
         } else if ( Compare < 0 ) {
           if ( OldBitmaps[i] )
             GpiDeleteBitmap ( OldBitmaps[i] ) ;
           AlteredList = TRUE ;
           i ++ ;
         } else {
           AlteredList = TRUE ;
           NeedNewBitmaps = TRUE ;
           j ++ ;
         } /* endif */
      } /* endif */
   } /* endwhile */

   if ( OldBitmaps )
      free ( OldBitmaps ) ;

   if ( NOT NeedNewBitmaps )
      return ( AlteredList ) ;

  /**************************************************************************
   * Open a memory device context.                                          *
   **************************************************************************/

   DeviceContext Memory = DeviceContext ( "UpdateFillTypeBitmaps", long(Anchor) ) ;

  /**************************************************************************
   * Create a memory presentation space for painting bitmaps.               *
   **************************************************************************/

   WorkSpace PS = WorkSpace ( "UpdateFillTypeBitmaps", 0, &Memory, Anchor, TRUE ) ;

  /**************************************************************************
   * Initialize the bitmap header.                                          *
   **************************************************************************/

   BITMAPINFOHEADER2 BitmapHeader ;
   memset ( &BitmapHeader, 0, sizeof(BitmapHeader) ) ;
   BitmapHeader.cbFix = 16 ;
   BitmapHeader.cx = 64 ;
   BitmapHeader.cy = 20 ;
   BitmapHeader.cPlanes = 1 ;
   BitmapHeader.cBitCount = 1 ;

  /**************************************************************************
   * For each fill type to be shown in the menu . . .                       *
   **************************************************************************/

   for ( i=0; i<NewCount && Active; i++ ) {

     /***********************************************************************
      * If new bitmap not needed, skip this one.                            *
      ***********************************************************************/

      if ( Bitmaps[i] )
         continue ;

     /***********************************************************************
      * Create a bitmap for the menu item.                                  *
      ***********************************************************************/

      Bitmaps[i] = PS.CreateBitmap ( BitmapHeader ) ;
      PS.SetBitmap ( Bitmaps[i] ) ;

     /***********************************************************************
      * Draw an example of the current fill type on the bitmap.             *
      ***********************************************************************/

      PS.SetColor ( CLR_BLACK ) ;
      RECTL Box = { 1, 1, BitmapHeader.cx-2, BitmapHeader.cy-2 } ;
      PS.FillBox ( Box ) ;

      PS.SetColor ( CLR_WHITE ) ;
      PS.SetFillType ( int(NewTypes[i]) ) ;
      Box.xLeft = Box.yBottom = 0 ;
      Box.xRight = BitmapHeader.cx - 1 ;
      Box.yTop = BitmapHeader.cy - 1 ;
      PS.FillBox ( Box ) ;

      PS.DrawBox ( Box ) ;

     /***********************************************************************
      * Measure the bitmap.                                                 *
      ***********************************************************************/

      BITMAPINFOHEADER2 BitmapInfoHeader ;
      BitmapInfoHeader.cbFix = sizeof(BitmapInfoHeader) ;
      GpiQueryBitmapInfoHeader ( Bitmaps[i], &BitmapInfoHeader ) ;
      BitmapSizes[i].cx = BitmapInfoHeader.cx ;
      BitmapSizes[i].cy = BitmapInfoHeader.cy ;

   } /* endfor */

   return ( TRUE ) ;
}


 
/****************************************************************************
 ****************************************************************************
 ***                                                                      ***
 ***             Line End Menu Management Functions & Data                ***
 ***                                                                      ***
 ****************************************************************************
 ****************************************************************************/

typedef struct {
   BOOL      Threaded ;
   HMODULE   Library ;
   HWND      WindowToNotify ;
   USHORT    NewCount ;
   PULONG    NewTypes ;
} LINEENDINIT_PARMS, *PLINEENDINIT_PARMS ;

static VOID LineEndInitThread ( PVOID Parameter ) ;

static BOOL UpdateLineEndBitmaps (
  HAB Anchor,
  int OldCount,
  ULONG OldTypes[],
  int NewCount,
  ULONG NewTypes[],
  HBITMAP Bitmaps[],
  SIZEL BitmapSizes[]
) ;

static PHBITMAP     LineEndBitmaps = 0 ;
static PSIZEL       LineEndBitmapSizes = 0 ;
static USHORT       LineEndCount = 0 ;
static Event        LineEndDone ( "MenuBmp-LineEndDone" ) ;
static PULONG       LineEnds = 0 ;
static Mutex        LineEndSemaphore ( "MenuBmp-LineEndSemaphore" ) ;
static TID          LineEndThread = 0 ;

 
/****************************************************************************
 *                                                                          *
 *  Start thread to initialize line end bitmaps.                            *
 *                                                                          *
 ****************************************************************************/

extern void InitLineEndBitmaps (
   HMODULE      Library,        // -> Resource Library
   HWND         WindowToNotify, // -> Window to notify upon completion.
   USHORT       NewCount,       // -> New Count
   PULONG       NewTypes        // -> New Line End Types
) {

  /**************************************************************************
   * Wait for previous initialization to complete before starting it anew.  *
   **************************************************************************/

   if ( long(LineEndThread) > 0 ) {
      DosSetPriority ( PRTYS_THREAD, PRTYC_FOREGROUNDSERVER, +20, LineEndThread ) ;
      LineEndDone.Wait() ;
   } /* endif */

  /**************************************************************************
   * Allocate and initialize the thread parameters.                         *
   **************************************************************************/

   PLINEENDINIT_PARMS Parms = PLINEENDINIT_PARMS ( malloc ( sizeof(LINEENDINIT_PARMS) ) ) ;
   Parms->Threaded = TRUE ;
   Parms->Library = Library ;
   Parms->WindowToNotify = WindowToNotify ;
   Parms->NewCount = NewCount ;
   Parms->NewTypes = NewTypes ;

  /**************************************************************************
   * Start the initialization thread.                                       *
   **************************************************************************/

   #ifdef THREADED
      Sys_SendMessage ( WindowToNotify, WM_THREAD_STARTED, 0, 0 ) ;
      LineEndDone.Reset() ;
      LineEndThread = StartThread ( "LineEndInitThread", LineEndInitThread, 0x10000, Parms, PRTYC_IDLETIME, +20 ) ;
   #else
      Parms->Threaded = FALSE ;
      Parms->WindowToNotify = 0 ;
      LineEndThread = 0 ;
      LineEndInitThread ( Parms ) ;
   #endif
}
 
/****************************************************************************
 *                                                                          *
 *  Initialize the Line End Menu                                            *
 *                                                                          *
 ****************************************************************************/

extern void InitLineEndMenu (
   HWND         Menu,           // -> Menu
   Estado      &State,          // -> Current State
   HMODULE      Library,        // -> Resource Library
   int          FirstEntry,     // Offset of first entry to display.
   USHORT       NewCount,       // -> New Count
   PULONG       NewTypes        // -> New Line End Types (or NULL to use old ones)
) {

  /**************************************************************************
   * Set the hourglass for a bit.                                           *
   **************************************************************************/

   HourGlass Hold ( Menu ) ;

  /**************************************************************************
   * Wait for any type menu update in progress to complete.                 *
   **************************************************************************/

   if ( long(LineEndThread) > 0 ) {
      DosSetPriority ( PRTYS_THREAD, PRTYC_FOREGROUNDSERVER, +20, LineEndThread ) ;
      LineEndDone.Wait() ;
   } /* endif */

  /**************************************************************************
   * Update it again, since we don't know how long ago the update was.      *
   **************************************************************************/

   PLINEENDINIT_PARMS Parms = PLINEENDINIT_PARMS ( malloc ( sizeof(LINEENDINIT_PARMS) ) ) ;
   Parms->Threaded = FALSE ;
   Parms->Library = Library ;
   Parms->WindowToNotify = 0 ;
   Parms->NewCount = NewTypes ? NewCount : LineEndCount ;
   Parms->NewTypes = NewTypes ? NewTypes : LineEnds ;
   LineEndInitThread ( Parms ) ;

  /**************************************************************************
   * Clear out the old menu.                                                *
   **************************************************************************/

   LONG OldCount = LONGFROMMR ( Sys_SendMessage ( Menu, MM_QUERYITEMCOUNT, 0, 0 ) ) ;
   for ( int i=0; i<OldCount; i++ ) {
      LONG Id = LONGFROMMR ( Sys_SendMessage ( Menu, MM_ITEMIDFROMPOSITION, MPFROMSHORT(0), 0 ) ) ;
      Sys_SendMessage ( Menu, MM_REMOVEITEM, MPFROM2SHORT(Id,TRUE), 0 ) ;
   } /* endfor */

  /**************************************************************************
   * Initialize the menu item structure.                                    *
   **************************************************************************/

   MENUITEM MenuItem ;
   MenuItem.iPosition = MIT_END ;
   MenuItem.afAttribute = 0 ;
   MenuItem.hwndSubMenu = 0 ;
   MenuItem.id = USHORT ( IDM_LINEENDS + FirstEntry ) ;

  /**************************************************************************
   * Determine the maximum menu height & width.                             *
   **************************************************************************/

   RECTL Desktop ;
   WinQueryWindowRect ( HWND_DESKTOP, &Desktop ) ;

   SWP Position ;
   WinQueryWindowPos ( OWNER(Menu), &Position ) ;

   POINTL Points[2] ;
   Points[0].x = Points[1].x = Position.x ;
   Points[0].y = Position.y ;  Points[1].y = Position.y + Position.cy ;
   WinMapWindowPoints ( PARENT(OWNER(Menu)), HWND_DESKTOP, PPOINTL(&Points), 2 ) ;

   SIZEL MaxSize ;
   MaxSize.cx = Desktop.xRight - Desktop.xLeft ;
   MaxSize.cy = max ( Desktop.yTop-Points[1].y, Points[0].y-Desktop.yBottom ) ;

  /**************************************************************************
   * Determine all the bitmap sizes.                                        *
   **************************************************************************/

   int ColumnStart = FirstEntry ;
   LONG Left = 0 ;
   LONG Width = 0 ;
   LONG Bottom = MaxSize.cy - ( FirstEntry ? 50 : 0 ) ;

   for ( i=FirstEntry; i<LineEndCount; i++ ) {
      Bottom -= LineEndBitmapSizes[i].cy + 6 ;
      if ( Bottom < 0 ) {
         ColumnStart = i ;
         Left += Width ;
         Width = 0 ;
         Bottom = MaxSize.cy - ( LineEndBitmapSizes[i].cy + 6 ) ;
      } /* endif */
      Width = max ( Width, LineEndBitmapSizes[i].cx + 50 ) ;
      if ( Width > MaxSize.cx-Left ) {
         i = ColumnStart ;
         break ;
      } /* endif */
   } /* endfor */

   int LastEntry = i - 1 ;

  /**************************************************************************
   * If we aren't showing the start of the list, prepend a page-up entry.   *
   **************************************************************************/

   if ( FirstEntry ) {
      ResourceString More ( Library, IDS_MORE_PGUP ) ;
      MENUITEM MenuItem ;
      MenuItem.iPosition = MIT_END ;
      MenuItem.afAttribute = 0 ;
      MenuItem.hwndSubMenu = 0 ;
      MenuItem.id = IDM_LINEENDS_PGUP ;
      MenuItem.afStyle = MIS_TEXT ;
      Sys_SendMessage ( Menu, MM_INSERTITEM, MPFROMP(&MenuItem), MPFROMP(PSZ(More)) ) ;
   } /* endif */

  /**************************************************************************
   * Build the menu entries.                                                *
   **************************************************************************/

   Left = 0 ;
   Width = 0 ;
   Bottom = MaxSize.cy - ( FirstEntry ? 50 : 0 ) ;

   for ( i=FirstEntry; i<=LastEntry; i++ ) {

     /***********************************************************************
      * Set the item to break the menu into columns if necessary.           *
      ***********************************************************************/

      MenuItem.afStyle = MIS_BITMAP ;
      Bottom -= LineEndBitmapSizes[i].cy + 6 ;
      if ( Bottom < 0 ) {
         MenuItem.afStyle |= MIS_BREAKSEPARATOR ;
         Bottom = MaxSize.cy - ( LineEndBitmapSizes[i].cy + 6 ) ;
      } /* endif */

     /***********************************************************************
      * Set the normal bitmap.                                              *
      ***********************************************************************/

      MenuItem.hItem = ULONG ( LineEndBitmaps[i] ) ;

     /***********************************************************************
      * Add the menu entry.                                                 *
      ***********************************************************************/

      Sys_SendMessage ( Menu, MM_INSERTITEM, MPFROMP(&MenuItem), MPFROMP("") ) ;

     /***********************************************************************
      * Check-mark the current type.                                        *
      ***********************************************************************/

      if ( ( State.LineEnd == LineEnds[i] ) OR ( ( State.LineEnd == LINEEND_DEFAULT ) AND ( LineEnds[i] == LINEEND_FLAT ) ) ) {
         Sys_SendMessage ( Menu, MM_SETITEMATTR, MPFROM2SHORT(IDM_LINEENDS+i,TRUE), MPFROM2SHORT(MIA_CHECKED,MIA_CHECKED) ) ;
      } else {
         Sys_SendMessage ( Menu, MM_SETITEMATTR, MPFROM2SHORT(IDM_LINEENDS+i,TRUE), MPFROM2SHORT(MIA_CHECKED,0) ) ;
      } /* endif */

     /***********************************************************************
      * Get the next menu item ID.                                          *
      ***********************************************************************/

      MenuItem.id ++ ;
   }

  /**************************************************************************
   * If we didn't show the end of the list, append a page-down entry.       *
   **************************************************************************/

   if ( LastEntry < LineEndCount-1 ) {
      ResourceString More ( Library, IDS_MORE_PGDN ) ;
      MENUITEM MenuItem ;
      MenuItem.iPosition = MIT_END ;
      MenuItem.afAttribute = 0 ;
      MenuItem.hwndSubMenu = 0 ;
      MenuItem.id = IDM_LINEENDS_PGDN ;
      MenuItem.afStyle = MIS_TEXT ;
      Sys_SendMessage ( Menu, MM_INSERTITEM, MPFROMP(&MenuItem), MPFROMP(PSZ(More)) ) ;
   } /* endif */

  /**************************************************************************
   * In any case, add on a separator and the menu manager.                  *
   **************************************************************************/

#if 0
   #ifdef MANAGERS
   MenuItem.iPosition = MIT_END ;
   MenuItem.afAttribute = 0 ;
   MenuItem.hwndSubMenu = 0 ;
   MenuItem.id = 0 ;
   MenuItem.afStyle = MIS_SEPARATOR ;
   MenuItem.hItem = 0 ;
   Sys_SendMessage ( Menu, MM_INSERTITEM, MPFROMP(&MenuItem), MPFROMP("") ) ;

   ResourceString Manager ( Library, IDS_MANAGER ) ;
   MenuItem.iPosition = MIT_END ;
   MenuItem.afAttribute = 0 ;
   MenuItem.hwndSubMenu = 0 ;
   MenuItem.id = IDM_LINEENDMGR ;
   MenuItem.afStyle = MIS_TEXT ;
   MenuItem.hItem = 0 ;
   Sys_SendMessage ( Menu, MM_INSERTITEM, MPFROMP(&MenuItem), MPFROMP(PSZ(Manager)) ) ;
   #endif
#endif
}
 
/****************************************************************************
 *                                                                          *
 *      Line End Menu Initialization Thread                                *
 *                                                                          *
 ****************************************************************************/

static VOID LineEndInitThread ( PVOID Parameter ) {

   #ifdef DEBUG
      Log ( "LineEnd Init thread started." ) ;
   #endif

  /**************************************************************************
   * Get parameters.                                                        *
   **************************************************************************/

   PLINEENDINIT_PARMS Parms = PLINEENDINIT_PARMS ( Parameter ) ;

  /**************************************************************************
   * Set up for GPI calls.                                                  *
   **************************************************************************/

   Process *Proc = 0 ;
   if ( Parms->Threaded ) 
      Proc = new Process ( "LineEndInitThread", 0, LibraryHandle ) ;

  /**************************************************************************
   * Expand the bitmap handle array, if necessary.                          *
   **************************************************************************/

   if ( Parms->NewCount > LineEndCount ) {
      PHBITMAP Bitmaps = PHBITMAP ( malloc ( Parms->NewCount * sizeof(HBITMAP) ) ) ;
      memset ( Bitmaps, 0, Parms->NewCount * sizeof(HBITMAP) ) ;
      if ( LineEndBitmaps ) {
         memcpy ( Bitmaps, LineEndBitmaps, LineEndCount * sizeof(HBITMAP) ) ;
         free ( LineEndBitmaps ) ;
      } /* endif */
      LineEndBitmaps = Bitmaps ;
      PSIZEL BitmapSizes = PSIZEL ( malloc ( Parms->NewCount * sizeof(SIZEL) ) ) ;
      memset ( BitmapSizes, 0, Parms->NewCount * sizeof(SIZEL) ) ;
      if ( LineEndBitmapSizes ) {
         memcpy ( BitmapSizes, LineEndBitmapSizes, LineEndCount * sizeof(SIZEL) ) ;
         free ( LineEndBitmapSizes ) ;
      } /* endif */
      LineEndBitmapSizes = BitmapSizes ;
   } /* endif */

  /**************************************************************************
   * Update the bitmaps.                                                    *
   **************************************************************************/

   UpdateLineEndBitmaps ( Proc ? Proc->QueryAnchor() : 0, LineEndCount, LineEnds, Parms->NewCount, Parms->NewTypes, LineEndBitmaps, LineEndBitmapSizes ) ;

  /**************************************************************************
   * Save the new line end list and count.                                  *
   **************************************************************************/

   LineEndCount = Parms->NewCount ;

   if ( LineEnds != Parms->NewTypes ) {
      if ( LineEnds )
         free ( LineEnds ) ;
      LineEnds = PULONG ( malloc ( LineEndCount * sizeof(ULONG) ) ) ;
      memcpy ( LineEnds, Parms->NewTypes, LineEndCount * sizeof(ULONG) ) ;
   } /* endif */
 
  /**************************************************************************
   * Notify the window that wanted to know when the thread was finished.    *
   **************************************************************************/

   if ( Parms->WindowToNotify )
      if ( !Sys_PostMessage ( Parms->WindowToNotify, WM_THREAD_DONE, MPFROMLONG(Sys_ThreadNumber(*_threadid)), 0 ) ) {
         ERRORID Code = Sys_GetLastError ( Proc ? Proc->QueryAnchor() : 0 ) ;
         Log ( "WARNING: Attempt to notify window of line end init thread completion failed.  Error %lX.", Code ) ;
      } /* endif */

  /**************************************************************************
   * Release PM resources.                                                  *
   **************************************************************************/

   if ( Parms->Threaded ) 
      delete Proc ;

  /**************************************************************************
   * Release the parameter memory.                                          *
   **************************************************************************/

   free ( Parms ) ;

  /**************************************************************************
   * We're done.                                                            *
   **************************************************************************/

   LineEndDone.Post() ;
   LineEndThread = 0 ;

   #ifdef DEBUG
      Log ( "LineEnd Init thread done." ) ;
   #endif
}
 
/****************************************************************************
 *                                                                          *
 *  Update Line End Bitmaps                                                *
 *                                                                          *
 ****************************************************************************/

static BOOL UpdateLineEndBitmaps (
  HAB Anchor,
  int OldCount,
  ULONG OldTypes[],
  int NewCount,
  ULONG NewTypes[],
  HBITMAP Bitmaps[],
  SIZEL BitmapSizes[]
) {

 /***************************************************************************
  * Reconcile the new line ends against the old, deleting obsolete bitmaps.*
  ***************************************************************************/

  qsort ( NewTypes, (size_t)NewCount, sizeof(NewTypes[0]), CompareULONGs ) ;

  PHBITMAP OldBitmaps = 0 ;
  if ( OldCount ) {
     OldBitmaps = PHBITMAP ( malloc ( sizeof(Bitmaps[0]) * OldCount ) ) ;
     memcpy ( OldBitmaps, Bitmaps, sizeof(Bitmaps[0])*OldCount ) ;
  } /* endif */

  memset ( Bitmaps, 0, sizeof(Bitmaps[0])*NewCount) ;

  BOOL AlteredList = FALSE ;
  BOOL NeedNewBitmaps = FALSE ;

  int i=0, j=0;
  while ( i<OldCount OR j<NewCount ) {
     if ( i>=NewCount ) {
        if ( OldBitmaps[i] )
           GpiDeleteBitmap ( OldBitmaps[i] ) ;
        AlteredList = TRUE ;
        i ++ ;
     } else if ( j>=OldCount ) {
        AlteredList = TRUE ;
        NeedNewBitmaps = TRUE ;
        j ++ ;
     } else {
        int Compare = CompareULONGs ( &OldTypes[i], &NewTypes[j] ) ;
        if ( Compare == 0 ) {
           if ( OldBitmaps[i] )
              Bitmaps[j] = OldBitmaps[i] ;
           else
              NeedNewBitmaps = TRUE ;
           i++, j++;
        } else if ( Compare < 0 ) {
          if ( OldBitmaps[i] )
            GpiDeleteBitmap ( OldBitmaps[i] ) ;
          AlteredList = TRUE ;
          i ++ ;
        } else {
          AlteredList = TRUE ;
          NeedNewBitmaps = TRUE ;
          j ++ ;
        } /* endif */
     } /* endif */
  } /* endwhile */

  if ( OldBitmaps )
    free ( OldBitmaps ) ;

  if ( NOT NeedNewBitmaps )
     return ( AlteredList ) ;

  /**************************************************************************
   * Open a memory device context.                                          *
   **************************************************************************/

   DeviceContext Memory = DeviceContext ( "UpdateLineEndBitmaps", long(Anchor) ) ;

  /**************************************************************************
   * Create a memory presentation space for painting bitmaps.               *
   **************************************************************************/

   WorkSpace PS = WorkSpace ( "UpdateLineEndBitmaps", 0, &Memory, Anchor, TRUE ) ;

  /**************************************************************************
   * Initialize the bitmap header.                                          *
   **************************************************************************/

   BITMAPINFOHEADER2 BitmapHeader ;
   memset ( &BitmapHeader, 0, sizeof(BitmapHeader) ) ;
   BitmapHeader.cbFix = 16 ;
   BitmapHeader.cx = 64 ;
   BitmapHeader.cy = 32 ;
   BitmapHeader.cPlanes = 1 ;
   BitmapHeader.cBitCount = 1 ;

  /**************************************************************************
   * For each line end to be shown in the menu . . .                        *
   **************************************************************************/

   for ( i=0; i<NewCount && Active; i++ ) {

     /***********************************************************************
      * If new bitmap not needed, skip this one.                            *
      ***********************************************************************/

      if ( Bitmaps[i] )
         continue ;

     /***********************************************************************
      * Create a bitmap for the menu item.                                  *
      ***********************************************************************/

      Bitmaps[i] = PS.CreateBitmap ( BitmapHeader ) ;
      PS.SetBitmap ( Bitmaps[i] ) ;

     /***********************************************************************
      * Draw an example of the current line end on the bitmap.              *
      ***********************************************************************/

      PS.SetColor ( CLR_BLACK ) ;
      RECTL Box = { 1, 1, BitmapHeader.cx-2, BitmapHeader.cy-2 } ;
      PS.FillBox ( Box ) ;

      PS.SetColor ( CLR_WHITE ) ;
      PS.SetLineWidth ( 20 ) ;
      PS.SetLineEnd ( int(NewTypes[i]) ) ;
      PS.Move ( 0, int(BitmapHeader.cy/2) ) ;
      PS.DrawLineGeom ( int((BitmapHeader.cx*2)/3), int(BitmapHeader.cy/2) ) ;

     /***********************************************************************
      * Measure the bitmap.                                                 *
      ***********************************************************************/

      BITMAPINFOHEADER2 BitmapInfoHeader ;
      BitmapInfoHeader.cbFix = sizeof(BitmapInfoHeader) ;
      GpiQueryBitmapInfoHeader ( Bitmaps[i], &BitmapInfoHeader ) ;
      BitmapSizes[i].cx = BitmapInfoHeader.cx ;
      BitmapSizes[i].cy = BitmapInfoHeader.cy ;

   } /* endfor */

   return ( TRUE ) ;
}


 
/****************************************************************************
 ****************************************************************************
 ***                                                                      ***
 ***             Line Join Menu Management Functions & Data               ***
 ***                                                                      ***
 ****************************************************************************
 ****************************************************************************/

typedef struct {
   BOOL      Threaded ;
   HMODULE   Library ;
   HWND      WindowToNotify ;
   USHORT    NewCount ;
   PULONG    NewTypes ;
} LINEJOININIT_PARMS, *PLINEJOININIT_PARMS ;

static VOID LineJoinInitThread ( PVOID Parameter ) ;

static BOOL UpdateLineJoinBitmaps (
  HAB Anchor,
  int OldCount,
  ULONG OldTypes[],
  int NewCount,
  ULONG NewTypes[],
  HBITMAP Bitmaps[],
  SIZEL BitmapSizes[]
) ;

static PHBITMAP     LineJoinBitmaps = 0 ;
static PSIZEL       LineJoinBitmapSizes = 0 ;
static USHORT       LineJoinCount = 0 ;
static Event        LineJoinDone ( "MenuBmp-LineJoinDone" ) ;
static PULONG       LineJoins = 0 ;
static Mutex        LineJoinSemaphore ( "MenuBmp-LineJoinSemaphore" ) ;
static TID          LineJoinThread = 0 ;

 
/****************************************************************************
 *                                                                          *
 *  Start thread to initialize line end bitmaps.                            *
 *                                                                          *
 ****************************************************************************/

extern void InitLineJoinBitmaps (
   HMODULE      Library,        // -> Resource Library
   HWND         WindowToNotify, // -> Window to notify upon completion.
   USHORT       NewCount,       // -> New Count
   PULONG       NewTypes        // -> New Line Join Types
) {

 /***************************************************************************
  * Wait for previous initialization to complete before starting it anew.   *
  ***************************************************************************/

  if ( long(LineJoinThread) > 0 ) {
     DosSetPriority ( PRTYS_THREAD, PRTYC_FOREGROUNDSERVER, +20, LineJoinThread ) ;
     LineJoinDone.Wait() ;
  } /* endif */

 /***************************************************************************
  * Allocate and initialize the thread parameters.                          *
  ***************************************************************************/

  PLINEJOININIT_PARMS Parms = PLINEJOININIT_PARMS ( malloc ( sizeof(LINEJOININIT_PARMS) ) ) ;
  Parms->Threaded = TRUE ;
  Parms->Library = Library ;
  Parms->WindowToNotify = WindowToNotify ;
  Parms->NewCount = NewCount ;
  Parms->NewTypes = NewTypes ;

 /***************************************************************************
  * Start the initialization thread.                                        *
  ***************************************************************************/

  #ifdef THREADED
    Sys_SendMessage ( WindowToNotify, WM_THREAD_STARTED, 0, 0 ) ;
    LineJoinDone.Reset() ;
    LineJoinThread = StartThread ( "LineJoinInitThread", LineJoinInitThread, 0x10000, Parms, PRTYC_IDLETIME, +20 ) ;
  #else
    Parms->Threaded = FALSE ;
    Parms->WindowToNotify = 0 ;
    LineJoinThread = 0 ;
    LineJoinInitThread ( Parms ) ;
  #endif
}
 
/****************************************************************************
 *                                                                          *
 *  Initialize the Line Join Menu                                           *
 *                                                                          *
 ****************************************************************************/

extern void InitLineJoinMenu (
   HWND         Menu,           // -> Menu
   Estado      &State,          // -> Current State
   HMODULE      Library,        // -> Resource Library
   int          FirstEntry,     // Offset of first entry to display.
   USHORT       NewCount,       // -> New Count
   PULONG       NewTypes        // -> New Line Join Types (or NULL to use old ones)
) {

  /**************************************************************************
   * Set the hourglass for a bit.                                           *
   **************************************************************************/

   HourGlass Hold ( Menu ) ;

  /**************************************************************************
   * Wait for any type menu update in progress to complete.                 *
   **************************************************************************/

   if ( long(LineJoinThread) > 0 ) {
      DosSetPriority ( PRTYS_THREAD, PRTYC_FOREGROUNDSERVER, +20, LineJoinThread ) ;
      LineJoinDone.Wait() ;
   } /* endif */

  /**************************************************************************
   * Update it again, since we don't know how long ago the update was.      *
   **************************************************************************/

   PLINEJOININIT_PARMS Parms = PLINEJOININIT_PARMS ( malloc ( sizeof(LINEJOININIT_PARMS) ) ) ;
   Parms->Threaded = FALSE ;
   Parms->Library = Library ;
   Parms->WindowToNotify = 0 ;
   Parms->NewCount = NewTypes ? NewCount : LineJoinCount ;
   Parms->NewTypes = NewTypes ? NewTypes : LineJoins ;
   LineJoinInitThread ( Parms ) ;

  /**************************************************************************
   * Clear out the old menu.                                                *
   **************************************************************************/

   LONG OldCount = LONGFROMMR ( Sys_SendMessage ( Menu, MM_QUERYITEMCOUNT, 0, 0 ) ) ;
   for ( int i=0; i<OldCount; i++ ) {
      LONG Id = LONGFROMMR ( Sys_SendMessage ( Menu, MM_ITEMIDFROMPOSITION, MPFROMSHORT(0), 0 ) ) ;
      Sys_SendMessage ( Menu, MM_REMOVEITEM, MPFROM2SHORT(Id,TRUE), 0 ) ;
   } /* endfor */

  /**************************************************************************
   * Initialize the menu item structure.                                    *
   **************************************************************************/

   MENUITEM MenuItem ;
   MenuItem.iPosition = MIT_END ;
   MenuItem.afAttribute = 0 ;
   MenuItem.hwndSubMenu = 0 ;
   MenuItem.id = USHORT ( IDM_LINEJOINS + FirstEntry ) ;

  /**************************************************************************
   * Determine the maximum menu height & width.                             *
   **************************************************************************/

   RECTL Desktop ;
   WinQueryWindowRect ( HWND_DESKTOP, &Desktop ) ;

   SWP Position ;
   WinQueryWindowPos ( OWNER(Menu), &Position ) ;

   POINTL Points[2] ;
   Points[0].x = Points[1].x = Position.x ;
   Points[0].y = Position.y ;  Points[1].y = Position.y + Position.cy ;
   WinMapWindowPoints ( PARENT(OWNER(Menu)), HWND_DESKTOP, PPOINTL(&Points), 2 ) ;

   SIZEL MaxSize ;
   MaxSize.cx = Desktop.xRight - Desktop.xLeft ;
   MaxSize.cy = max ( Desktop.yTop-Points[1].y, Points[0].y-Desktop.yBottom ) ;

  /**************************************************************************
   * Determine all the bitmap sizes.                                        *
   **************************************************************************/

   int ColumnStart = FirstEntry ;
   LONG Left = 0 ;
   LONG Width = 0 ;
   LONG Bottom = MaxSize.cy - ( FirstEntry ? 50 : 0 ) ;

   for ( i=FirstEntry; i<LineJoinCount; i++ ) {
      Bottom -= LineJoinBitmapSizes[i].cy + 6 ;
      if ( Bottom < 0 ) {
         ColumnStart = i ;
         Left += Width ;
         Width = 0 ;
         Bottom = MaxSize.cy - ( LineJoinBitmapSizes[i].cy + 6 ) ;
      } /* endif */
      Width = max ( Width, LineJoinBitmapSizes[i].cx + 50 ) ;
      if ( Width > MaxSize.cx-Left ) {
         i = ColumnStart ;
         break ;
      } /* endif */
   } /* endfor */

   int LastEntry = i - 1 ;

  /**************************************************************************
   * If we aren't showing the start of the list, prepend a page-up entry.   *
   **************************************************************************/

   if ( FirstEntry ) {
      ResourceString More ( Library, IDS_MORE_PGUP ) ;
      MENUITEM MenuItem ;
      MenuItem.iPosition = MIT_END ;
      MenuItem.afAttribute = 0 ;
      MenuItem.hwndSubMenu = 0 ;
      MenuItem.id = IDM_LINEJOINS_PGUP ;
      MenuItem.afStyle = MIS_TEXT ;
      Sys_SendMessage ( Menu, MM_INSERTITEM, MPFROMP(&MenuItem), MPFROMP(PSZ(More)) ) ;
   } /* endif */

  /**************************************************************************
   * Build the menu entries.                                                *
   **************************************************************************/

   Left = 0 ;
   Width = 0 ;
   Bottom = MaxSize.cy - ( FirstEntry ? 50 : 0 ) ;

   for ( i=FirstEntry; i<=LastEntry; i++ ) {

     /***********************************************************************
      * Set the item to break the menu into columns if necessary.           *
      ***********************************************************************/

      MenuItem.afStyle = MIS_BITMAP ;
      Bottom -= LineJoinBitmapSizes[i].cy + 6 ;
      if ( Bottom < 0 ) {
         MenuItem.afStyle |= MIS_BREAKSEPARATOR ;
         Bottom = MaxSize.cy - ( LineJoinBitmapSizes[i].cy + 6 ) ;
      } /* endif */

     /***********************************************************************
      * Set the normal bitmap.                                              *
      ***********************************************************************/

      MenuItem.hItem = ULONG ( LineJoinBitmaps[i] ) ;

     /***********************************************************************
      * Add the menu entry.                                                 *
      ***********************************************************************/

      Sys_SendMessage ( Menu, MM_INSERTITEM, MPFROMP(&MenuItem), MPFROMP("") ) ;

     /***********************************************************************
      * Check-mark the current type.                                        *
      ***********************************************************************/

      if ( ( State.LineJoin == LineJoins[i] ) OR ( ( State.LineJoin == LINEJOIN_DEFAULT ) AND ( LineJoins[i] == LINEJOIN_BEVEL ) ) ) {
         Sys_SendMessage ( Menu, MM_SETITEMATTR, MPFROM2SHORT(IDM_LINEJOINS+i,TRUE), MPFROM2SHORT(MIA_CHECKED,MIA_CHECKED) ) ;
      } else {
         Sys_SendMessage ( Menu, MM_SETITEMATTR, MPFROM2SHORT(IDM_LINEJOINS+i,TRUE), MPFROM2SHORT(MIA_CHECKED,0) ) ;
      } /* endif */

     /***********************************************************************
      * Get the next menu item ID.                                          *
      ***********************************************************************/

      MenuItem.id ++ ;
   }

  /**************************************************************************
   * If we didn't show the end of the list, append a page-down entry.       *
   **************************************************************************/

   if ( LastEntry < LineJoinCount-1 ) {
      ResourceString More ( Library, IDS_MORE_PGDN ) ;
      MENUITEM MenuItem ;
      MenuItem.iPosition = MIT_END ;
      MenuItem.afAttribute = 0 ;
      MenuItem.hwndSubMenu = 0 ;
      MenuItem.id = IDM_LINEJOINS_PGDN ;
      MenuItem.afStyle = MIS_TEXT ;
      Sys_SendMessage ( Menu, MM_INSERTITEM, MPFROMP(&MenuItem), MPFROMP(PSZ(More)) ) ;
   } /* endif */

  /**************************************************************************
   * In any case, add on a separator and the menu manager.                  *
   **************************************************************************/

#if 0
   #ifdef MANAGERS
   MenuItem.iPosition = MIT_END ;
   MenuItem.afAttribute = 0 ;
   MenuItem.hwndSubMenu = 0 ;
   MenuItem.id = 0 ;
   MenuItem.afStyle = MIS_SEPARATOR ;
   MenuItem.hItem = 0 ;
   Sys_SendMessage ( Menu, MM_INSERTITEM, MPFROMP(&MenuItem), MPFROMP("") ) ;

   ResourceString Manager ( Library, IDS_MANAGER ) ;
   MenuItem.iPosition = MIT_END ;
   MenuItem.afAttribute = 0 ;
   MenuItem.hwndSubMenu = 0 ;
   MenuItem.id = IDM_LINEJOINMGR ;
   MenuItem.afStyle = MIS_TEXT ;
   MenuItem.hItem = 0 ;
   Sys_SendMessage ( Menu, MM_INSERTITEM, MPFROMP(&MenuItem), MPFROMP(PSZ(Manager)) ) ;
   #endif
#endif
}
 
/****************************************************************************
 *                                                                          *
 *      Line Join Menu Initialization Thread                                *
 *                                                                          *
 ****************************************************************************/

static VOID LineJoinInitThread ( PVOID Parameter ) {

   #ifdef DEBUG
      Log ( "LineJoin Init thread started." ) ;
   #endif

  /**************************************************************************
   * Get parameters.                                                        *
   **************************************************************************/

   PLINEJOININIT_PARMS Parms = PLINEJOININIT_PARMS ( Parameter ) ;

  /**************************************************************************
   * Set up for GPI calls.                                                  *
   **************************************************************************/

   Process *Proc = 0 ;
   if ( Parms->Threaded ) 
      Proc = new Process ( "LineJoinInitThread", 0, LibraryHandle ) ;

  /**************************************************************************
   * Expand the bitmap handle array, if necessary.                          *
   **************************************************************************/

   if ( Parms->NewCount > LineJoinCount ) {
      PHBITMAP Bitmaps = PHBITMAP ( malloc ( Parms->NewCount * sizeof(HBITMAP) ) ) ;
      memset ( Bitmaps, 0, Parms->NewCount * sizeof(HBITMAP) ) ;
      if ( LineJoinBitmaps ) {
         memcpy ( Bitmaps, LineJoinBitmaps, LineJoinCount * sizeof(HBITMAP) ) ;
         free ( LineJoinBitmaps ) ;
      } /* endif */
      LineJoinBitmaps = Bitmaps ;
      PSIZEL BitmapSizes = PSIZEL ( malloc ( Parms->NewCount * sizeof(SIZEL) ) ) ;
      memset ( BitmapSizes, 0, Parms->NewCount * sizeof(SIZEL) ) ;
      if ( LineJoinBitmapSizes ) {
         memcpy ( BitmapSizes, LineJoinBitmapSizes, LineJoinCount * sizeof(SIZEL) ) ;
         free ( LineJoinBitmapSizes ) ;
      } /* endif */
      LineJoinBitmapSizes = BitmapSizes ;
   } /* endif */

  /**************************************************************************
   * Update the bitmaps.                                                    *
   **************************************************************************/

   UpdateLineJoinBitmaps ( Proc ? Proc->QueryAnchor() : 0, LineJoinCount, LineJoins, Parms->NewCount, Parms->NewTypes, LineJoinBitmaps, LineJoinBitmapSizes ) ;

  /**************************************************************************
   * Save the new line end list and count.                                  *
   **************************************************************************/

   LineJoinCount = Parms->NewCount ;

   if ( LineJoins != Parms->NewTypes ) {
      if ( LineJoins )
         free ( LineJoins ) ;
      LineJoins = PULONG ( malloc ( LineJoinCount * sizeof(ULONG) ) ) ;
      memcpy ( LineJoins, Parms->NewTypes, LineJoinCount * sizeof(ULONG) ) ;
   } /* endif */

  /**************************************************************************
   * Notify the window that wanted to know when the thread was finished.    *
   **************************************************************************/

   if ( Parms->WindowToNotify )
      if ( !Sys_PostMessage ( Parms->WindowToNotify, WM_THREAD_DONE, MPFROMLONG(Sys_ThreadNumber(*_threadid)), 0 ) ) {
         ERRORID Code = Sys_GetLastError ( Proc ? Proc->QueryAnchor() : 0 ) ;
         Log ( "WARNING: Attempt to notify window of line end init thread completion failed.  Error %lX.", Code ) ;
      } /* endif */

  /**************************************************************************
   * Release PM resources.                                                  *
   **************************************************************************/

   if ( Parms->Threaded ) 
      delete Proc ;

  /**************************************************************************
   * Release the parameter memory.                                          *
   **************************************************************************/

   free ( Parms ) ;

  /**************************************************************************
   * We're done.                                                            *
   **************************************************************************/

   LineJoinDone.Post() ;
   LineJoinThread = 0 ;

   #ifdef DEBUG
      Log ( "LineJoin Init thread done." ) ;
   #endif
}
 
/****************************************************************************
 *                                                                          *
 *  Update Line Join Bitmaps                                                *
 *                                                                          *
 ****************************************************************************/

static BOOL UpdateLineJoinBitmaps (
   HAB Anchor,
   int OldCount,
   ULONG OldTypes[],
   int NewCount,
   ULONG NewTypes[],
   HBITMAP Bitmaps[],
   SIZEL BitmapSizes[]
) {

  /**************************************************************************
   * Reconcile the new line ends against the old, deleting obsolete bitmaps.*
   **************************************************************************/

   qsort ( NewTypes, (size_t)NewCount, sizeof(NewTypes[0]), CompareULONGs ) ;

   PHBITMAP OldBitmaps = 0 ;
   if ( OldCount ) {
      OldBitmaps = PHBITMAP ( malloc ( sizeof(Bitmaps[0]) * OldCount ) ) ;
      memcpy ( OldBitmaps, Bitmaps, sizeof(Bitmaps[0])*OldCount ) ;
   } /* endif */

   memset ( Bitmaps, 0, sizeof(Bitmaps[0])*NewCount) ;

   BOOL AlteredList = FALSE ;
   BOOL NeedNewBitmaps = FALSE ;

   int i=0, j=0;
   while ( i<OldCount OR j<NewCount ) {
      if ( i>=NewCount ) {
         if ( OldBitmaps[i] )
            GpiDeleteBitmap ( OldBitmaps[i] ) ;
         AlteredList = TRUE ;
         i ++ ;
      } else if ( j>=OldCount ) {
         AlteredList = TRUE ;
         NeedNewBitmaps = TRUE ;
         j ++ ;
      } else {
         int Compare = CompareULONGs ( &OldTypes[i], &NewTypes[j] ) ;
         if ( Compare == 0 ) {
            if ( OldBitmaps[i] )
               Bitmaps[j] = OldBitmaps[i] ;
            else
               NeedNewBitmaps = TRUE ;
            i++, j++;
         } else if ( Compare < 0 ) {
           if ( OldBitmaps[i] )
              GpiDeleteBitmap ( OldBitmaps[i] ) ;
           AlteredList = TRUE ;
           i ++ ;
         } else {
           AlteredList = TRUE ;
           NeedNewBitmaps = TRUE ;
           j ++ ;
         } /* endif */
      } /* endif */
   } /* endwhile */

   if ( OldBitmaps )
      free ( OldBitmaps ) ;

   if ( NOT NeedNewBitmaps )
      return ( AlteredList ) ;

   /*************************************************************************
    * Open a memory device context.                                         *
    *************************************************************************/

    DeviceContext Memory = DeviceContext ( "UpdateLineJoinBitmaps", long(Anchor) ) ;

  /**************************************************************************
   * Create a memory presentation space for painting bitmaps.               *
   **************************************************************************/

   WorkSpace PS = WorkSpace ( "UpdateLineJoinBitmaps", 0, &Memory, Anchor, TRUE ) ;

  /**************************************************************************
   * Initialize the bitmap header.                                          *
   **************************************************************************/

   BITMAPINFOHEADER2 BitmapHeader ;
    memset ( &BitmapHeader, 0, sizeof(BitmapHeader) ) ;
   BitmapHeader.cbFix = 16 ;
   BitmapHeader.cx = 64 ;
   BitmapHeader.cy = 64 ;
   BitmapHeader.cPlanes = 1 ;
   BitmapHeader.cBitCount = 1 ;

  /**************************************************************************
   * For each line end to be shown in the menu . . .                        *
   **************************************************************************/

   for ( i=0; i<NewCount && Active; i++ ) {

     /***********************************************************************
      * If new bitmap not needed, skip this one.                            *
      ***********************************************************************/
 
      if ( Bitmaps[i] )
         continue ;

     /***********************************************************************
      * Create a bitmap for the menu item.                                  *
      ***********************************************************************/

      Bitmaps[i] = PS.CreateBitmap ( BitmapHeader ) ;
      PS.SetBitmap ( Bitmaps[i] ) ;

     /***********************************************************************
      * Draw an example of the current line end on the bitmap.              *
      ***********************************************************************/

      PS.SetColor ( CLR_BLACK ) ;
      RECTL Box = { 1, 1, BitmapHeader.cx-2, BitmapHeader.cy-2 } ;
      PS.FillBox ( Box ) ;

      PS.SetColor ( CLR_WHITE ) ;
      PS.SetLineWidth ( 20 ) ;
      PS.SetLineJoin ( int(NewTypes[i]) ) ;
      POINTL Points[] = {
         { 0, (BitmapHeader.cy*2)/3 },
         { (BitmapHeader.cx*2)/3, (BitmapHeader.cy*2)/3 },
         { (BitmapHeader.cx*2)/3, 0 }
      } ;
      PS.DrawPolyLine ( sizeof(Points)/sizeof(Points[0]), Points ) ;

     /***********************************************************************
      * Measure the bitmap.                                                 *
      ***********************************************************************/

      BITMAPINFOHEADER2 BitmapInfoHeader ;
      BitmapInfoHeader.cbFix = sizeof(BitmapInfoHeader) ;
      GpiQueryBitmapInfoHeader ( Bitmaps[i], &BitmapInfoHeader ) ;
      BitmapSizes[i].cx = BitmapInfoHeader.cx ;
      BitmapSizes[i].cy = BitmapInfoHeader.cy ;

   } /* endfor */

   return ( TRUE ) ;
}



 
/****************************************************************************
 ****************************************************************************
 ***                                                                      ***
 ***                     General Support Functions                        ***
 ***                                                                      ***
 ****************************************************************************
 ****************************************************************************/

/****************************************************************************
 *                                                                          *
 *  Cancel Bitmap Threads, if any are active.                               *
 *                                                                          *
 ****************************************************************************/

extern void CancelBitmapThreads ( void ) {

   #ifdef DEBUG
      Log ( "CancelBitmapThreads: Started." ) ;
   #endif

   // Tell everybody to die.
   Active = FALSE ;

   // Jack up everybody's priority.
   if ( long(FontThread) > 0 ) 
      DosSetPriority ( PRTYS_THREAD, PRTYC_FOREGROUNDSERVER, +20, FontThread ) ;
   if ( long(SizeThread) > 0 ) 
      DosSetPriority ( PRTYS_THREAD, PRTYC_FOREGROUNDSERVER, +20, SizeThread ) ;
   if ( long(AttrThread) > 0 )
      DosSetPriority ( PRTYS_THREAD, PRTYC_FOREGROUNDSERVER, +20, AttrThread ) ;
   if ( long(ExpandThread) > 0 ) 
      DosSetPriority ( PRTYS_THREAD, PRTYC_FOREGROUNDSERVER, +20, ExpandThread ) ;
   if ( long(LineColorThread) > 0 ) 
      DosSetPriority ( PRTYS_THREAD, PRTYC_FOREGROUNDSERVER, +20, LineColorThread ) ;
   if ( long(LineWidthThread) > 0 ) 
      DosSetPriority ( PRTYS_THREAD, PRTYC_FOREGROUNDSERVER, +20, LineWidthThread ) ;
   if ( long(FillColorThread) > 0 ) 
      DosSetPriority ( PRTYS_THREAD, PRTYC_FOREGROUNDSERVER, +20, FillColorThread ) ;
   if ( long(FillTypeThread) > 0 ) 
      DosSetPriority ( PRTYS_THREAD, PRTYC_FOREGROUNDSERVER, +20, FillTypeThread ) ;
   if ( long(LineEndThread) > 0 ) 
      DosSetPriority ( PRTYS_THREAD, PRTYC_FOREGROUNDSERVER, +20, LineEndThread ) ;
   if ( long(LineJoinThread) > 0 ) 
      DosSetPriority ( PRTYS_THREAD, PRTYC_FOREGROUNDSERVER, +20, LineJoinThread ) ;

   // Wait for everybody to complete.
   if ( long(FontThread) > 0 ) {
      FontDone.Wait() ;
      FontThread = 0 ;
   } /* endif */
   if ( long(SizeThread) > 0 ) {
      SizeDone.Wait() ;
      SizeThread = 0 ;
   } /* endif */
   if ( long(AttrThread) > 0 ){
      AttrDone.Wait() ;
      AttrThread = 0 ;
   } /* endif */
   if ( long(ExpandThread) > 0 ) {
      ExpandDone.Wait() ;
      ExpandThread = 0 ;
   } /* endif */
   if ( long(LineColorThread) > 0 ) {
      LineColorDone.Wait() ;
      LineColorThread = 0 ;
   } /* endif */
   if ( long(LineWidthThread) > 0 ) {
      LineWidthDone.Wait() ;
      LineWidthThread = 0 ;
   } /* endif */
   if ( long(FillColorThread) > 0 ) {
      FillColorDone.Wait() ;
      FillColorThread = 0 ;
   } /* endif */
   if ( long(FillTypeThread) > 0 ) {
      FillTypeDone.Wait() ;
      FillTypeThread = 0 ;
   } /* endif */
   if ( long(LineEndThread) > 0 ) {
      LineEndDone.Wait() ;
      LineEndThread = 0 ;
   } /* endif */
   if ( long(LineJoinThread) > 0 ) {
      LineJoinDone.Wait() ;
      LineJoinThread = 0 ;
   } /* endif */

   // Restore Active flag for later.
   Active = TRUE ;

   #ifdef DEBUG
      Log ( "CancelBitmapThreads: Done." ) ;
   #endif
}
 
/****************************************************************************
 *                                                                          *
 *  Destroy Bitmaps, if any currently exist.                                *
 *                                                                          *
 ****************************************************************************/

extern void DestroyBitmaps ( void ) {

   #ifdef DEBUG
      Log ( "DestroyBitmaps: Started." ) ;
   #endif

   // Release font resources.
   for ( int i=0; i<FontCount; i++ )
      if ( FontBitmaps[i] ) 
         GpiDeleteBitmap ( FontBitmaps[i] ) ;
   FontCount = 0 ;
   if ( FontBitmaps     ) free ( FontBitmaps     ), FontBitmaps = 0 ;
   if ( FontBitmapSizes ) free ( FontBitmapSizes ), FontBitmapSizes = 0 ;
   if ( FontMetrics     ) free ( FontMetrics     ), FontMetrics = 0 ;

   // Release size resources.
   for ( i=0; i<SizeCount; i++ )
      if ( SizeBitmaps[i] )
         GpiDeleteBitmap ( SizeBitmaps[i] ) ;
   SizeCount = 0 ;
   if ( Sizes           ) free ( Sizes           ), Sizes = 0 ;
   if ( SizeBitmaps     ) free ( SizeBitmaps     ), SizeBitmaps = 0 ;
   if ( SizeBitmapSizes ) free ( SizeBitmapSizes ), SizeBitmapSizes = 0 ;
   if ( SizeFont        ) free ( SizeFont        ), SizeFont = 0 ;

   // Release attribute resources.
   for ( i=0; i<AttrCount; i++ ) {
      if ( AttrBitmaps[i] ) {
         GpiDeleteBitmap ( AttrBitmaps[i] ) ;
         AttrBitmaps [i] = 0 ;
      } /* endif */
   } /* endfor i */
   if ( AttrFont ) free ( AttrFont ), AttrFont = 0 ;

   // Release expansion resources.
   for ( i=0; i<ExpandCount; i++ )
      if ( ExpandBitmaps[i] )
         GpiDeleteBitmap ( ExpandBitmaps[i] ) ;
   ExpandCount = 0 ;
   if ( Expansions        ) free ( Expansions        ), Expansions = 0 ;
   if ( ExpandBitmaps     ) free ( ExpandBitmaps     ), ExpandBitmaps = 0 ;
   if ( ExpandBitmapSizes ) free ( ExpandBitmapSizes ), ExpandBitmapSizes = 0 ;
   if ( ExpandFont        ) free ( ExpandFont        ), ExpandFont = 0 ;

   // Release line color resources.
   for ( i=0; i<LineColorCount; i++ )
      if ( LineColorBitmaps[i] )
         GpiDeleteBitmap ( LineColorBitmaps[i] ) ;
   LineColorCount = 0 ;
   if ( LineColors           ) free ( LineColors           ), LineColors = 0 ;
   if ( LineColorBitmaps     ) free ( LineColorBitmaps     ), LineColorBitmaps = 0 ;
   if ( LineColorBitmapSizes ) free ( LineColorBitmapSizes ), LineColorBitmapSizes = 0 ;

   // Release line width resources.
   for ( i=0; i<LineWidthCount; i++ )
      if ( LineWidthBitmaps[i] )
         GpiDeleteBitmap ( LineWidthBitmaps[i] ) ;
   LineWidthCount = 0 ;
   if ( LineWidths           ) free ( LineWidths           ), LineWidths = 0 ;
   if ( LineWidthBitmaps     ) free ( LineWidthBitmaps     ), LineWidthBitmaps = 0 ;
   if ( LineWidthBitmapSizes ) free ( LineWidthBitmapSizes ), LineWidthBitmapSizes = 0 ;

   // Release fill color resources.
   for ( i=0; i<FillColorCount; i++ )
      if ( FillColorBitmaps[i] )
         GpiDeleteBitmap ( FillColorBitmaps[i] ) ;
   FillColorCount = 0 ;
   if ( FillColors           ) free ( FillColors           ), FillColors = 0 ;
   if ( FillColorBitmaps     ) free ( FillColorBitmaps     ), FillColorBitmaps = 0 ;
   if ( FillColorBitmapSizes ) free ( FillColorBitmapSizes ), FillColorBitmapSizes = 0 ;

   // Release fill type resources.
   for ( i=0; i<FillTypeCount; i++ )
      if ( FillTypeBitmaps[i] )
         GpiDeleteBitmap ( FillTypeBitmaps[i] ) ;
   FillTypeCount = 0 ;
   if ( FillTypes           ) free ( FillTypes           ), FillTypes = 0 ;
   if ( FillTypeBitmaps     ) free ( FillTypeBitmaps     ), FillTypeBitmaps = 0 ;
   if ( FillTypeBitmapSizes ) free ( FillTypeBitmapSizes ), FillTypeBitmapSizes = 0 ;

   // Release line end resources.
   for ( i=0; i<LineEndCount; i++ )
      if ( LineEndBitmaps[i] )
         GpiDeleteBitmap ( LineEndBitmaps[i] ) ;
   LineEndCount = 0 ;
   if ( LineEnds           ) free ( LineEnds           ), LineEnds = 0 ;
   if ( LineEndBitmaps     ) free ( LineEndBitmaps     ), LineEndBitmaps = 0 ;
   if ( LineEndBitmapSizes ) free ( LineEndBitmapSizes ), LineEndBitmapSizes = 0 ;

   // Release line join resources.
   for ( i=0; i<LineJoinCount; i++ )
      if ( LineJoinBitmaps[i] )
         GpiDeleteBitmap ( LineJoinBitmaps[i] ) ;
   LineJoinCount = 0 ;
   if ( LineJoins           ) free ( LineJoins           ), LineJoins = 0 ;
   if ( LineJoinBitmaps     ) free ( LineJoinBitmaps     ), LineJoinBitmaps = 0 ;
   if ( LineJoinBitmapSizes ) free ( LineJoinBitmapSizes ), LineJoinBitmapSizes = 0 ;

   #ifdef DEBUG
      Log ( "DestroyBitmaps: Done." ) ;
   #endif

}
 
/****************************************************************************
 *                                                                          *
 *      Create Menu Bitmap                                                  *
 *                                                                          *
 ****************************************************************************/

static HBITMAP CreateMenuBitmap (
   WorkSpace &PS,
   PSZ MenuText,
   PSZ Font,
   USHORT PointSize,
   BOOL Bold,
   BOOL Italic,
   BOOL Underscore,
   BOOL Strikeout,
   BOOL Outline,
   BOOL Caps,
   ULONG Expand
) {

   #ifdef DEBUG
      Log ( "CreateMenuBitmap(PS=%08p,Text='%s',Font='%s',Size=%i,Bold=%s,Italic=%s,Underscore=%s,Strikeout=%s,Outline=%s,Caps=%s,Expand=%i) Started.",
         &PS, MenuText, Font, PointSize, Bold?"TRUE":"FALSE", Italic?"TRUE":"FALSE",
         Underscore?"TRUE":"FALSE", Strikeout?"TRUE":"FALSE", Outline?"TRUE":"FALSE",
         Caps?"TRUE":"FALSE", Expand ) ;
   #endif

  /**************************************************************************
   * Build the graphic state.                                               *
   **************************************************************************/

   PS.SetFontName ( PCHAR(Font) ) ;
   PS.SetFontSize ( PointSize ) ;
   PS.SetBold ( int(Bold) ) ;
   PS.SetItalic ( int(Italic) ) ;
   PS.SetUnderscore ( int(Underscore) ) ;
   PS.SetStrikeout ( int(Strikeout) ) ;
   PS.SetOutline ( int(Outline) ) ;
   PS.SetCaps ( int(Caps) ) ;
   PS.SetExpand ( int(Expand) ) ;

  /**************************************************************************
   * If the font name includes " Caps", then force capitals.                *
   **************************************************************************/

   char FontName [33] ;
   strcpy ( FontName, Font ) ;
   strupr ( FontName ) ;
   if ( strstr ( FontName, "CAPS" ) ) 
      PS.SetCaps ( TRUE ) ;

  /**************************************************************************
   * Copy the text to be rendered.                                          *
   **************************************************************************/

   char Text [80] ;

   if ( strlen(PCHAR(MenuText)) > sizeof(Text)-1 ) {
      Log ( "WARNING: Menu Text too large for working buffer." ) ;
      DosBeep ( 400, 10 ) ;
   } /* endif */

   strcpy ( Text, PCHAR(MenuText) ) ;

  /**************************************************************************
   * Measure the text.                                                      *
   **************************************************************************/

   SIZEL Size ;
   PS.MeasureText ( Text, Size ) ;

  /**************************************************************************
   * Convert the size to pels for the bitmap.                               *
   **************************************************************************/

   POINTL Point = { Size.cx, Size.cy }  ;
   PS.Transform ( CVTC_DEFAULTPAGE, CVTC_DEVICE, 1, &Point ) ;

  /**************************************************************************
   * Initialize the bitmap header.                                          *
   **************************************************************************/

   BITMAPINFOHEADER2 BitmapHeader ;
   memset ( &BitmapHeader, 0, sizeof(BitmapHeader) ) ;
   BitmapHeader.cbFix = 16 ;
   BitmapHeader.cPlanes = 1 ;
   BitmapHeader.cBitCount = 1 ;
   BitmapHeader.cx = Point.x + 1 ;
   BitmapHeader.cy = Point.y + 1 ;

  /**************************************************************************
   * Create the bitmap.                                                     *
   **************************************************************************/

   HBITMAP Bitmap = PS.CreateBitmap ( BitmapHeader ) ;
   if ( Bitmap == 0 )
      return ( 0 ) ;

  /**************************************************************************
   * Associate the bitmap with the memory presentation space.               *
   **************************************************************************/

   PS.SetBitmap ( Bitmap ) ;

  /**************************************************************************
   * Fill the bitmap with black.  Black & white bitmaps must have black     *
   *   for the background.                                                  *
   **************************************************************************/

   PS.SetColor ( CLR_BLACK ) ;
   RECTL Box = { 0, 0, Size.cx, Size.cy } ;
   PS.FillBox ( Box ) ;

  /**************************************************************************
   * Paint the text on the bitmap.                                          *
   **************************************************************************/

   PS.SetColor ( CLR_WHITE ) ;

   Point.x = 0 ;
   Point.y = PS.QueryDescent() ;
   PS.Move ( Point ) ;

   PS.DrawText ( Text ) ;

  /**************************************************************************
   * Unset the bitmap from the presentation space.                          *
   **************************************************************************/

   PS.SetBitmap ( 0 ) ;

  /**************************************************************************
   * Return the bitmap handle.                                              *
   **************************************************************************/

   #ifdef DEBUG
      Log ( "CreateMenuBitmap(PS=%08p,Text='%s',Font='%s',Size=%i,Bold=%s,Italic=%s,Underscore=%s,Strikeout=%s,Outline=%s,Caps=%s,Expand=%i) Done.  Bitmap %08p.",
         &PS, MenuText, Font, PointSize, Bold?"TRUE":"FALSE", Italic?"TRUE":"FALSE",
         Underscore?"TRUE":"FALSE", Strikeout?"TRUE":"FALSE", Outline?"TRUE":"FALSE",
         Caps?"TRUE":"FALSE", Expand, Bitmap ) ;
   #endif

   return ( Bitmap ) ;
}
 
/****************************************************************************
 *                                                                          *
 *  Compare FONTMETRICS structures                                          *
 *                                                                          *
 ****************************************************************************/

static int _Optlink CompareFonts ( const void *Item1, const void *Item2 ) {
  PFONTMETRICS Font1 = PFONTMETRICS ( Item1 ) ;
  PFONTMETRICS Font2 = PFONTMETRICS ( Item2 ) ;
  return ( strcmp ( Font1->szFacename, Font2->szFacename ) ) ;
}
 
/****************************************************************************
 *                                                                          *
 *  Compare Font Sizes                                                      *
 *                                                                          *
 ****************************************************************************/

static int _Optlink CompareUSHORTs ( const void *Item1, const void *Item2 ) {
  PUSHORT Size1 = PUSHORT ( Item1 ) ;
  PUSHORT Size2 = PUSHORT ( Item2 ) ;
  return ( *Size1 - *Size2 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *  Compare COLORs                                                          *
 *                                                                          *
 ****************************************************************************/

static int _Optlink CompareCOLORs ( const void *Item1, const void *Item2 ) {
  PCOLOR Color1 = PCOLOR ( Item1 ) ;
  PCOLOR Color2 = PCOLOR ( Item2 ) ;
  return ( int ( *Color1 - *Color2 ) ) ;
}
 
/****************************************************************************
 *                                                                          *
 *  Compare ULONGs                                                          *
 *                                                                          *
 ****************************************************************************/

static int _Optlink CompareULONGs ( const void *Item1, const void *Item2 ) {
  PULONG Type1 = PULONG ( Item1 ) ;
  PULONG Type2 = PULONG ( Item2 ) ;
  return ( int ( *Type1 - *Type2 ) ) ;
}

