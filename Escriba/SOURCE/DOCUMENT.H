/***************************************************************** DOCUMENT.H
 *                                                                          *
 *                             Document Classes                             *
 *                                                                          *
 ****************************************************************************/

#ifndef DOCUMENT_H
#define DOCUMENT_H

#include <ctype.h>
#include <stdio.h>
#include <time.h>

#include "System.h"

#include "DocParms.h"
#include "Event.h"
#include "External.h"
#include "Format.h"
#include "Imp_Exp.h"
#include "Painter.h"
#include "Profile2.h"
#include "Readers.h"
#include "Speller.h"
#include "State.h"
#include "String_.h"
#include "Support.h"
#include "Text.h"
#include "WorkPage.h"
#include "WrkSpace.h"


        // Constants

#define ALL             0xFFFF
#define GRAPHIC_BORDER  100
#define MAX_STRING      200

#define TOKEN_BACKUP_TYPE long

#define GRAPHIC_EDGE_LEFT               (-1)
#define GRAPHIC_EDGE_RIGHT              (-2)
#define GRAPHIC_EDGE_TOP                (-3)
#define GRAPHIC_EDGE_BOTTOM             (-4)
#define GRAPHIC_MARGIN_LEFT             (-5)
#define GRAPHIC_MARGIN_RIGHT            (-6)
#define GRAPHIC_MARGIN_TOP              (-7)
#define GRAPHIC_MARGIN_BOTTOM           (-8)
#define GRAPHIC_CENTERED                (-9)
#define GRAPHIC_EDGE_TO_EDGE            (-10)
#define GRAPHIC_MARGIN_TO_MARGIN        (-11)
#define GRAPHIC_NEXT_CHAR               (-12)
#define GRAPHIC_NEXT_LINE               (-13)

enum AspectType { GRAPHIC_ASPECT_UNFIXED, GRAPHIC_ASPECT_FIX_X, GRAPHIC_ASPECT_FIX_Y } ;

enum TestResult { TEST_TEXT, TEST_GRAPHIC, TEST_GRAPHICBORDER } ;

#define ADJUSTSTATE_LEFTMARGIN          0x00000001
#define ADJUSTSTATE_RIGHTMARGIN         0x00000002
#define ADJUSTSTATE_BOTTOMMARGIN        0x00000004
#define ADJUSTSTATE_TOPMARGIN           0x00000008
#define ADJUSTSTATE_MARGINS             0x0000000F
#define ADJUSTSTATE_TAB                 0x00000010
#define ADJUSTSTATE_TABS                0x00000020
#define ADJUSTSTATE_INDENT              0x00000040
#define ADJUSTSTATE_LINESPACING         0x00000080
#define ADJUSTSTATE_FONT                0x00000100
#define ADJUSTSTATE_SIZE                0x00000200
#define ADJUSTSTATE_LINECOLOR           0x00000400
#define ADJUSTSTATE_BOLD                0x00000800
#define ADJUSTSTATE_ITALIC              0x00001000
#define ADJUSTSTATE_UNDERSCORE          0x00002000
#define ADJUSTSTATE_STRIKEOUT           0x00004000
#define ADJUSTSTATE_OUTLINE             0x00008000
#define ADJUSTSTATE_CAPS                0x00010000
#define ADJUSTSTATE_ATTRIBUTES          0x0001F800
#define ADJUSTSTATE_EXPAND              0x00020000
#define ADJUSTSTATE_CENTERPAGE          0x00040000
#define ADJUSTSTATE_JUSTIFY             0x00080000
#define ADJUSTSTATE_DATEFORMAT          0x00100000
#define ADJUSTSTATE_FILLCOLOR           0x00200000
#define ADJUSTSTATE_LINEWIDTH           0x00400000
#define ADJUSTSTATE_LINEJOIN            0x00800000
#define ADJUSTSTATE_LINEEND             0x01000000
#define ADJUSTSTATE_FILLTYPE            0x02000000
#define ADJUSTSTATE_HEADER              0x04000000
#define ADJUSTSTATE_FOOTER              0x08000000


        // Class: Page - An abbreviated page description.

class Pagina {                  // Class: Minimum Page Status
   public:
      BOOL NeedsFormat ;        // Flag: Needs Formatting.
      Estado State ;            // Initial State
      long Offset ;             // Offset into document text.
      long Length ;             // Size in document text.

      void Dump ( int indent ) {
         Log ( "%*sOffset, Length: %li, %li", indent, "", Offset, Length ) ;
         Log ( "%*sNeedsFormat: %s", indent, "", NeedsFormat?"TRUE":"FALSE" ) ;
         Log ( "%*sState:", indent, "" ) ;  State.Dump ( indent+2 ) ;
      } /* endmethod */
} ;


        // Class: Documento - A complete document.

#define MAX_PAGES (200)
#define MAX_HANDLES (200)

class Documento {

                        // Private Data

   private:
      HAB Anchor ;                      // Thread Anchor Block handle.
      COLOR Colors [2] ;                // Foreground/Background colors.
      long Column ;                     // Cursor column (preferred).
      POINTL CursorBox [TXTBOX_COUNT] ; // Cursor box.
      DocumentParms DocParms ;          // Document Parameters (used to build initial state).
      BOOL fDocParms ;                  // Flag: Document Parameters altered.
      time_t FileDate ;                 // Document Change Date
      String *Filename ;                // -> File Name string.
      String *FileType ;                // -> File Type string.
      BOOL FullNames ;                  // Flag: Show full file name in title-bar?
      BOOL InsertMode ;                 // Flag: Insert mode if true, else Replace.
      long LevelOffset ;                // Offset at start of edit level.
      LanguageModule *Library ;         // -> Language text resource library.
      BOOL Minimized ;                  // Flag: Is window minimized right now?
      long Offset ;                     // Offset within document to cursor location.
      long OffsetLast ;                 // Offset of last selection update.
      long OffsetOrigin ;               // Offset of selection start.
      int PageNumber ;                  // Current page number.
      Painter *PainterObject ;          // -> Painter Thread object.
      POINTL Position ;                 // Cursor position on page.
      POINTL PrinterResolution ;        // Printer resolution (x,y) in pixels per unit (centimeter or inch).
      WorkSpace *PS ;                   // -> Graphics workspace.
      Text *pText ;                     // -> Document Text
      HWND Rulers [2] ;                 // Horizontal & Vertical Ruler window handles.
      int SavedLevel ;                  // Change level at last save.
      HWND ScrollBars [2] ;             // Horizontal & Vertical Scroll Bar window handles.
      POINTL ScrollMax ;                // Scrolling Maxima
      POINTL ScrollPos ;                // Scrolling Position
      SIZEL Size ;                      // Cursor size on page.
      Estado State ;                    // Document state at cursor location.
      Estado State0 ;                   // Initial state of document.
      HWND StatusWindow ;               // -> Status line window
      Supervisor *SupervisorObject ;    // -> Format Supervisor Thread object.
      String *Title ;                   // -> Window Title string
      HWND TitleWindow ;                // -> Title-bar window
      POINTL TopLeft ;                  // Coordinates of displayed page's upper-left corner.
      HWND Window ;                     // Document display window handle.
      Event WindowThreadDone ;          // Event indicating that window thread has initialized.
      TID WindowThreadId ;              // Window thread ID.
      Event WindowThreadReady ;         // Event indicating that window thread has terminated.
      DeviceContext *pWindowDevice ;    // Document display window context object.
      FIXED Zoom ;                      // Display magnification factor.

      Mutex CurrentPageLock ;           // Current page access semaphore.
      Borradora CurrentPage ;           // Current page description.
      BOOL CurrentPageValid ;           // Flag: Current page valid?
      Borradora WorkingPage ;           // Page under construction in background, if any.

      volatile short NumberOfPages ;    // Abbreviated list of Pages in the document.
      Pagina Pages [ MAX_PAGES ] ;

      int HandleCount ;                 // Count of Resource Handles in use
      LONG Handles [ MAX_HANDLES ] ;    // Resource Handles
      int HandleTypes [ MAX_HANDLES ] ; // Resource Handle Types


                        // Private Methods

   private:
      void               AddHandle          ( LONG Handle, int Type ) ;
      void               AdjustState        ( Estado &OldState, Estado &NewState, ULONG Flags, char Text[], LONG TabStop=0 ) ;
      void               CenterPage         ( Borradora &Page, Estado &EffectiveState, long Height ) ;
      void               CreatePreface      ( char *Buffer, int &Size ) ;
      void               CommenceEdit       ( ) ;
      void               CompleteEdit       ( ) ;
      void               CommenceLevel      ( ) ;
      void               CompleteLevel      ( ) ;
      void               ComputeState       ( long Offset, Estado &State, POINTL &Position, SIZEL &Size ) ;
      void               ComputeState       ( long Offset, Estado &State ) ;
      void               ComputeState       ( ) ;
      void               Delete             ( long Start, long Offset ) ;
      void               Delete             ( ) ;
      void               Deselect           ( WorkSpace *DisplayPS=0 ) ;
      long               Extract            ( long BufferSize, char *Buffer, Estado &State1, Estado &State2 ) ;
      long               Extract            ( long BufferSize, char *Buffer, BOOL All=TRUE ) ;
      long               Extract            ( BOOL All=TRUE ) ;
      long               Find               ( char *Text, long &EndingOffset, BOOL CaseSensitive=TRUE, BOOL Regular=FALSE ) ;
      long               FindToken          ( Linea *pLine, Palabra *pWord, long Column ) ;
      long               FindXY             ( POINTL &Point ) ;
      void               FinishMove         ( BOOL Selecting ) ;
      void               FinishMove         ( ) ;
      BOOL               FormatLine         ( Borradora &Page, Linea &Line, int MaxLength, BOOL Force=FALSE ) ;
      BOOL               FormatPage         ( int Page ) ;
      long               FormatPage         ( int PageNumber, Estado &FinalState, Borradora &Page ) ;
      BOOL               FormatPage         ( Borradora &Page, Estado &EffectiveState, int MaxLength, long &Height, BOOL &Done ) ;
      int                GetPageFromOffset  ( long Offset ) ;
      long               ImportText         ( char* RawText, int Length, char* &CookedData ) ;
      long               ImportText         ( char* RawText, char* &CookedData ) ;
      long               Insert             ( int Offset, char *Text, int Length ) ;
      long               Insert             ( int Offset, char *Text ) ;
      long               Insert             ( char *Text ) ;
      long               Insert             ( char *Text, int Length ) ;
      void               InsertBefore       ( char *Text ) ;
      void               InsertBefore       ( char *Text, int Length ) ;
      int                InvertFlush        ( WorkSpace *DisplayPS, char *String, int &StringLength, long Start, Estado &State, POINTL &Position, SIZEL &Size ) ;
      int                InvertFlushLine    ( WorkSpace *DisplayPS, char *String, int &StringLength, long Start, Estado &State, POINTL &Position, SIZEL &Size ) ;
      void               InvertText         ( WorkSpace *DisplayPS, long Offset1, long Offset2 ) ;
      BOOL               IsSelected         ( long TokenOffset ) ;
      void               MoveDirect         ( long offset, BOOL Select ) ;
      void               MoveDirect         ( long offset ) ;
      long               MoveTokenDown      ( long Offset ) ;
      void               MoveTokenDown      ( ) ;
      long               MoveTokenUp        ( long Offset ) ;
      void               MoveTokenUp        ( ) ;
      int                Paste              ( ) ;
      void               QueryTextRectangle ( RECTL &Rectangle, long Offset1, long Offset2 ) ;
      long               ReadPreface        ( int Handle, DocumentParms &DocParms, int &Version ) ;
      long               ReadPreface        ( char *Text, int Size, DocumentParms &DocParms, int &Version ) ;
      long               ReadPreface        ( PREADER Reader, PREADER Peeker, PTELLER Teller, PVOID ReaderData, DocumentParms &DocParms, int &Version ) ;
      void               Select             ( WorkSpace *DisplayPS=0 ) ;
      void               SetBlockState      ( Estado &State, ULONG Flags, LONG TabStop=0 ) ;
      void               SetDefaults        ( ) ;
      void               SetPagesDirty      ( long Start, long End ) ;
      void               UpdateMouse        ( ) ;
      BOOL               UpdatePosition     ( ) ;
      void               UpdateRulers       ( ) ;
      void               UpdateScrollBars   ( ) ;
      void               UpdateStatus       ( ) ;
      BOOL               UpdateWindow       ( int Page, BOOL SetColumn, BOOL UsedCurrentPage ) ;


                        // Public Methods called by Window / Print / Save

   public:
      Documento ( HAB Anchor, LanguageModule *Library, DocumentParms &docparms, PSZ filename=0, Addon *Importer=0 ) ;
      Documento ( HAB Anchor, Documento &Original, BOOL ForSaveOnly=FALSE ) ;
      ~Documento ( ) ;                                                  
      BOOL               ChangeAll          ( char *OldString, char *NewString, BOOL CaseSensitive=TRUE, BOOL Regular=FALSE ) ;
      BOOL               ChangeString       ( char *OldString, char *NewString, BOOL CaseSensitive=TRUE, BOOL Regular=FALSE ) ;
      void       Enqueue_Clear              ( ) ;
      void       Dequeue_Clear              ( ) ;
      void               ClearTab           ( int TabStop ) ;
      HWND               CreateWindow       ( HWND Owner, INIDATA *IniData, Dictionary *Speller, int WindowID, int Trap ) ;
      void               Copy               ( ) ;
      void               CopyGraphic        ( Grafico &Graphic ) ;
      BOOL               DestroyWindow      ( HWND Owner, BOOL Mandatory = FALSE ) ;
      void               FlushCommandQueue  ( ) ;
      BOOL               FindString         ( char *String, BOOL CaseSensitive=TRUE, BOOL Regular=FALSE ) ;
      BOOL               FindStringNext     ( char *String, BOOL CaseSensitive=TRUE, BOOL Regular=FALSE ) ;
      void               FormatPage         ( ) ;
      BOOL               HasSelection       ( ) { return ( OffsetLast != OffsetOrigin ) ; }
      void               HorizontalScroll   ( SHORT ScrollType, SHORT ScrollPosition ) ;
      void       Enqueue_InsertKey          ( USHORT Key ) ;
      void       Dequeue_InsertKey          ( USHORT Key ) ;
      void       Enqueue_InsertModeToggle   ( ) ;
      void       Dequeue_InsertModeToggle   ( ) ;
      void               InsertStateChange  ( Estado &OldState, Estado &NewState, ULONG Flags, LONG TabStop=0 ) ;
      void       Enqueue_InsertText         ( char *Text, int Length ) ;
      void       Enqueue_InsertText         ( char *Text ) ;
      void       Dequeue_InsertText         ( char *Text, int Length ) ;
      BOOL               IsChanged          ( ) { return ( fDocParms OR ( SavedLevel != pText->QueryCurrentLevel() ) ) ; }
      void               MergeFile          ( char *Filename, DocumentParms &DocParms, Addon *Importer=0 ) ;
      BOOL               MoveBottom         ( BOOL Selecting=FALSE ) ;  
      BOOL               MoveDown           ( BOOL Selecting=FALSE ) ;  
      BOOL               MoveLineBottom     ( BOOL Selecting=FALSE ) ;  
      BOOL               MoveLineDown       ( BOOL Selecting=FALSE ) ;  
      BOOL               MoveLineTop        ( BOOL Selecting=FALSE ) ;  
      BOOL               MoveLineUp         ( BOOL Selecting=FALSE ) ;  
      BOOL               MovePageBottom     ( BOOL Selecting=FALSE ) ;
      BOOL               MovePageDown       ( BOOL Selecting=FALSE ) ;  
      BOOL               MovePageNumber     ( int Number, BOOL Selecting=FALSE ) ;
      BOOL               MovePageTop        ( BOOL Selecting=FALSE ) ;
      BOOL               MovePageUp         ( BOOL Selecting=FALSE ) ;  
      void               MoveTab            ( int OldTabStop, int NewTabStop ) ;
      void               MoveText           ( int Copying ) ;
      BOOL               MoveTop            ( BOOL Selecting=FALSE ) ;  
      BOOL               MoveUp             ( BOOL Selecting=FALSE ) ;  
      BOOL               MoveWordDown       ( BOOL Selecting=FALSE ) ;  
      BOOL               MoveWordUp         ( BOOL Selecting=FALSE ) ;  
      void               MoveXY             ( POINTL &Point, BOOL Selecting ) ;
      void               MoveXY             ( POINTL &Point ) ;
      void               Paint              ( ) ;
      void       Enqueue_Paste              ( ) ;
      void       Dequeue_Paste              ( ) ;
      BOOL               QueryGraphic       ( POINTL &Point, Grafico &Graphic, RECTL &Box, BOOL IncludeBorders=FALSE ) ;
      void               QueryTextRectangle ( RECTL &Rectangle ) ;
      int                Query_CurrentLevel ( ) { return ( pText->QueryCurrentLevel() ) ; }
      PSZ                Query_Filename     ( ) { return ( PSZ(*Filename) ) ; }
      time_t             Query_FileDate     ( ) { return ( FileDate ) ; }
      PSZ                Query_FileType     ( ) { return ( PSZ(*FileType) ) ; }
      BOOL               Query_InsertMode   ( ) { return ( InsertMode ) ; }
      int                Query_MaxLevel     ( ) { return ( pText->QueryMaxLevel() ) ; }
      int                Query_PageCount    ( ) { return ( NumberOfPages ) ; }
      int                Query_PageNumber   ( ) { return ( PageNumber ) ; }
      DocumentParms     *Query_Parms        ( ) { return ( &DocParms ) ; }
      POINTL            *Query_Resolution   ( ) { return ( &PrinterResolution ) ; }
      int                Query_SavedLevel   ( ) { return ( SavedLevel ) ; }
      Estado            *Query_State        ( ) { return ( &State ) ; }
      Estado            *Query_State0       ( ) { return ( &State0 ) ; }
      Text              *Query_Text         ( ) { return ( pText ) ; }
      POINTL            &Query_TopLeft      ( ) { return ( TopLeft ) ; }
      HWND               Query_Window       ( ) { return ( Window ) ; }
      void               Query_WindowSize   ( RECTL &Rectangle, SIZEL &WindowSize ) ;
      void               Query_WindowSize   ( SIZEL &WindowSize ) ;
      FIXED              Query_Zoom         ( ) { return ( Zoom ) ; }
      void       Enqueue_Redo               ( ) ;
      void       Dequeue_Redo               ( ) ;
      void               Render             ( WorkSpace *DisplayPS, PRECTL ClippingRectangle=0 ) ;
      void               ReplaceSelectedText ( char *String ) ;
      void               Rezoom             ( ) ;
      BOOL               Save               ( HWND Owner, BOOL Rename ) ;
      BOOL               Save               ( HWND Owner, PSZ filename, ImportExport *Exporter ) ;
      void               SelectAll          ( ) ;
      void               SelectWord         ( POINTL &Point ) ;
      void               SetDefaults        ( DocumentParms &docparms ) ;
      void               SetTab             ( int TabStop ) ;
      void               Set_Colors         ( COLOR Colors[] ) ;
      void               Set_FullNames      ( BOOL flag ) { FullNames = flag ; }
      void               Set_Language       ( LanguageModule *NewLibrary ) ;
      void               Set_Minimized      ( BOOL flag ) { Minimized = flag ; }
      void               Set_Name           ( PSZ filename ) ;
      void               Set_RulerWindows   ( HWND HRuler, HWND VRuler ) { Rulers[0] = HRuler ; Rulers[1] = VRuler ; }
      void               Set_SavedLevel     ( int Level ) { SavedLevel = Level ; fDocParms = FALSE ; }
      void               Set_ScrollBars     ( HWND HBar, HWND VBar ) { ScrollBars[0] = HBar ; ScrollBars[1] = VBar ; }
      void               Set_StatusWindow   ( HWND window ) { StatusWindow = window ; }
      void               Set_TitleWindow    ( HWND window ) { TitleWindow = window ; }
      void               Set_Type           ( PSZ filetype ) ;
      void               Set_Window         ( HWND window, DeviceContext *pdevice ) ;
      void               Set_Zoom           ( FIXED zoom ) ;
      BOOL               SpellCheck         ( Dictionary *Speller, int WordSize, char Word[], int ListSize, int EntrySize, char List[], int &ListCount, int &EasyCount, BOOL First=FALSE ) ;
      int                TestXY             ( POINTL &Point, BOOL &Selected ) ;
      BOOL               Transform          ( int From, int To, POINTL &Point ) {
                            return ( PS->Transform ( From, To, 1, &Point ) ) ; }
      BOOL               Transform          ( int From, int To, RECTL &Rectangle ) {
                            return ( PS->Transform ( From, To, 2, PPOINTL(&Rectangle) ) ) ; }
      void       Enqueue_Undo               ( ) ;
      void       Dequeue_Undo               ( ) ;
      void               UpdateCursor       ( ) ;
      void               UpdateGraphic      ( Grafico &Graphic ) ;
      void               UpdateTitle        ( BOOL UpdateNow = FALSE ) ;
      void               VerticalScroll     ( SHORT ScrollType, SHORT ScrollPosition ) ;


                        // Public Methods called by Supervisor/Formatter & Painter

   public:
      RECTL             *ComputeInvalidRectangle ( ) { return ( CurrentPage.ComputeInvalidRectangle(PS,PrinterResolution,NumberOfPages,Query_CurrentLevel()?time(0):FileDate) ) ; }
      void               FormatFirstDirtyPage ( Event &RequestAnotherFormat, Event &CurrentPageDone, BOOL SetColumn=FALSE ) ;
      void               LockCurrentPage    ( ) { CurrentPageLock.Request() ; }
      void               Paint              ( WorkSpace *WindowPS, RECTL &ClippingRectangle ) ;
      void               SetPageDirty       ( ) ;
      void               UnlockCurrentPage  ( ) { CurrentPageLock.Release() ; }


                        // Public Debugging Methods

   public:
      void               Dump ( PSZ Title ) ;


                        // Spell-Checker Support

   friend char* APIENTRY GetPtr ( void *Object, long *Offset, long Delta ) ;

} ;


        // Miscellaneous Declarations

extern char *FunctionNames [MAX_FUNCTIONS] ;


#endif
