/****************************************************************** MENUBMP.H
 *                                                                          *
 *                        Menu Bitmap Functions                             *
 *                                                                          *
 ****************************************************************************/

#ifndef MENUBMP_H
#define MENUBMP_H

#include "System.h"

#include "DocParms.h"
#include "State.h"


  // General Menu Support

extern void CancelBitmapThreads ( void ) ;
extern void DestroyBitmaps ( void ) ;


  // Font Menu Support

extern void InitFontBitmaps (
   HMODULE      Library,        // -> Resource Library
   HWND         WindowToNotify  // -> Window to notify upon completion.
) ;
extern void InitFontMenu (
   HWND          Menu,           // -> Menu
   Estado       &State,          // -> Current State
   HMODULE       Library,        // -> Resource Library
   int           FirstEntry      // Index of first entry to display.
) ;
extern PFONTMETRICS QueryFontMetrics ( void ) ;


  // Size Menu Support

extern void InitSizeBitmaps (
   HMODULE      Library,        // -> Resource Library
   HWND         WindowToNotify, // -> Window to notify upon completion.
   PSZ          NewFont,        // -> New Font
   USHORT       NewCount,       // -> New Size Count
   PUSHORT      NewSizes        // -> New Sizes
) ;
extern void InitSizeMenu (
   HWND         Menu,           // -> Menu
   Estado      &State,          // -> Current State
   HMODULE      Library,        // -> Resource Library
   int          FirstEntry,     // Index of first entry to display.
   USHORT       NewCount,       // -> New Size Count
   PUSHORT      NewSizes        // -> New Sizes
) ;


  // Attribute Menu Support

extern void InitAttrBitmaps (
   HMODULE      Library,        // -> Resource Library
   HWND         WindowToNotify, // -> Window to notify upon completion.
   PSZ          NewFont         // -> New Font
) ;
extern void InitAttrMenu (
   HWND         Menu,           // -> Menu
   Estado      &State,          // -> Current State
   DocumentParms &DocParms,     // -> Document Parameters (Default State Info)
   HMODULE      Library         // -> Resource Library
) ;


  // Expand Menu Support

extern void InitExpandBitmaps (
   HMODULE      Library,        // -> Resource Library
   HWND         WindowToNotify, // -> Window to notify upon completion.
   PSZ          NewFont,        // -> New Font
   USHORT       NewCount,       // -> New Expand Count
   PUSHORT      NewExpansions   // -> New Expansions
) ;
extern void InitExpandMenu (
   HWND         Menu,           // -> Menu
   Estado      &State,          // -> Current State
   HMODULE      Library,        // -> Resource Library
   int          FirstEntry,     // Index of first entry to display.
   USHORT       NewCount,       // -> New Expand Count
   PUSHORT      NewExpansions   // -> New Expansions
) ;


  // Line Color Menu Support

extern void InitLineColorBitmaps (  // Start line color bitmap list initialization thread.
   HMODULE      Library,        // -> Resource Library
   HWND         WindowToNotify, // -> Window to notify upon completion.
   USHORT       NewCount,       // -> New Color Count
   PCOLOR       NewColors       // -> New Colors
) ;
extern void InitLineColorMenu (
   HWND         Menu,           // -> Menu
   Estado      &State,          // -> Current State
   HMODULE      Library,        // -> Resource Library
   int          FirstEntry,     // Index of first entry to display.
   USHORT       NewCount,       // -> New Color Count
   PCOLOR       NewColors       // -> New Colors
) ;

  // Line Width Menu Support

extern void InitLineWidthBitmaps (  // Start line width bitmap list initialization thread.
   HMODULE      Library,        // -> Resource Library
   HWND         WindowToNotify, // -> Window to notify upon completion.
   USHORT       NewCount,       // -> New Width Count
   PULONG       NewWidths       // -> New Widths
) ;
extern void InitLineWidthMenu (
   HWND         Menu,           // -> Menu
   Estado      &State,          // -> Current State
   HMODULE      Library,        // -> Resource Library
   int          FirstEntry,     // Index of first entry to display.
   USHORT       NewCount,       // -> New Color Count
   PULONG       NewWidths       // -> New Widths
) ;

  // Fill Color Menu Support

extern void InitFillColorBitmaps (  // Start fill color bitmap list initialization thread.
   HMODULE      Library,        // -> Resource Library
   HWND         WindowToNotify, // -> Window to notify upon completion.
   USHORT       NewCount,       // -> New Color Count
   PCOLOR       NewColors       // -> New Colors
) ;
extern void InitFillColorMenu (
   HWND         Menu,           // -> Menu
   Estado      &State,          // -> Current State
   HMODULE      Library,        // -> Resource Library
   int          FirstEntry,     // Index of first entry to display.
   USHORT       NewCount,       // -> New Color Count
   PCOLOR       NewColors       // -> New Colors
) ;

  // Fill Type Menu Support

extern void InitFillTypeBitmaps (  // Start fill type bitmap list initialization thread.
   HMODULE      Library,        // -> Resource Library
   HWND         WindowToNotify, // -> Window to notify upon completion.
   USHORT       NewCount,       // -> New Type Count
   PULONG       NewTypes        // -> New Types
) ;
extern void InitFillTypeMenu (
   HWND         Menu,           // -> Menu
   Estado      &State,          // -> Current State
   HMODULE      Library,        // -> Resource Library
   int          FirstEntry,     // Index of first entry to display.
   USHORT       NewCount,       // -> New Type Count
   PULONG       NewTypes        // -> New Types
) ;

  // Line End Menu Support

extern void InitLineEndBitmaps (// Start line end bitmap list initialization thread.
   HMODULE      Library,        // -> Resource Library
   HWND         WindowToNotify, // -> Window to notify upon completion.
   USHORT       NewCount,       // -> New Count
   PULONG       NewTypes        // -> New Line End Types
) ;
extern void InitLineEndMenu (
   HWND         Menu,           // -> Menu
   Estado      &State,          // -> Current State
   HMODULE      Library,        // -> Resource Library
   int          FirstEntry,     // Index of first entry to display.
   USHORT       NewCount,       // -> New Count
   PULONG       NewTypes        // -> New Line End Types
) ;

  // Line Join Menu Support

extern void InitLineJoinBitmaps (// Start line join bitmap list initialization thread.
   HMODULE      Library,        // -> Resource Library
   HWND         WindowToNotify, // -> Window to notify upon completion.
   USHORT       NewCount,       // -> New Count
   PULONG       NewTypes        // -> New Line End Types
) ;
extern void InitLineJoinMenu (
   HWND         Menu,           // -> Menu
   Estado      &State,          // -> Current State
   HMODULE      Library,        // -> Resource Library
   int          FirstEntry,     // Index of first entry to display.
   USHORT       NewCount,       // -> New Count
   PULONG       NewTypes        // -> New Line Join Types
) ;


#endif
