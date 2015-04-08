/***************************************************************** WORKPAGE.H
 *                                                                          *
 *                      Working Page Class Definition                       *
 *                                                                          *
 ****************************************************************************/

#ifndef WORKPAGE_H
#define WORKPAGE_H

#include <ctype.h>
#include <stdio.h>
#include <string.h>

#include "System.h"

#include "Pool.h"
#include "State.h"
#include "Support.h"
#include "Text.h"
#include "WrkSpace.h"


        // Class: Graphic - A rectangular picture object.

class Borradora ;
class Linea ;
class Palabra ;

class Grafico {

   public:
      long   Offset ;                   // Offset into document text.
      long   Length ;                   // Size in document text.
      BOOL   NeedsPainting ;            // Flag: This word needs to be repainted.
      long   Handle ;                   // Handle to Graphic.
      Estado State ;                    // State at graphic's location in the document.
      POINTL Position ;                 // Position on page (x,y).
      SIZEL  Size ;                     // Size on page (cx,cy).
      SIZEL  ImageSize ;                // Original image size (bitmaps & metafiles)
      int    Aspect ;                   // Aspect preservation type (None, FixX, FixY).
      USHORT CRC ;                      // State CRC.
      POINTL OldPosition ;              // -> Old Position on page (x,y).
      SIZEL  OldSize ;                  // -> Old Size on page (cx,cy).
      int    OldAspect ;                // Old aspect preservation type.
      BOOL   OldPrinterOptimize ;       // Old printer optimize flag.
      USHORT OldCRC ;                   // -> Old State CRC.
      char   Type [2] ;                 // Graphic Type (BM,MF,UP,39,PN)
      char   BarCode [80] ;             // UPC Code, if of type UP, or Number, if of type 39, or ZIP+4, if of type PN.
      long   BorderWidth ;              // Border thickness.
      COLOR  BorderColor ;              // Border color.
      char   BorderCaption [80] ;       // Border caption text.
      BOOL   Background ;               // Flag: Use as background and allow printing on top?
      BOOL   NoTextAside ;              // Flag: Do not allow text to either side?
      BOOL   Halftone ;                 // Flag: Paint the graphic halftone?
      BOOL   Reverse ;                  // Flag: Paint the graphic reverse?
      BOOL   PrinterOptimize ;          // Flag: Optimize size for printer?

      Grafico *PrevGraphic ( ) ;
      Grafico *NextGraphic ( ) ;

      void ComputeImageSize ( WorkSpace *PS ) ;

      void ComputeBox    ( Palabra *pWord, RECTL &Box, POINTL &PrinterResolution, BOOL IncludeBorders=FALSE ) ;
      void ComputeOldBox ( Palabra *pWord, RECTL &Box, POINTL &PrinterResolution, BOOL IncludeBorders=FALSE ) ;
      void ComputeBox    ( Palabra *pWord, POINTL &Position, SIZEL &Size, int Aspect, int Optimize, RECTL &Box, POINTL &PrinterResolution, BOOL IncludeBorders=FALSE ) ;

      void Split ( long Offset ) ;

      void Dump ( int indent ) ;
} ;


        // Class: Word - A rectangular text object.

class Palabra {

   public:
      long Offset ;             // Offset into document text.
      long Length ;             // Size in document text.
      BOOL NeedsMeasuring ;     // Flag: This word needs to be measured yet.
      BOOL NeedsPainting ;      // Flag: This word needs to be repainted.
      BOOL WasGraphic ;         // Flag: This word was a graphic.
      POINTL Position ;         // -> Position on page (x,y).
      SIZEL Size ;              // -> Size on page (cx,cy).
      USHORT CRC ;              // State CRC.
      POINTL OldPosition ;      // -> Old Position on page (x,y).
      SIZEL  OldSize ;          // -> Old Size on page (cx,cy).
      USHORT OldCRC ;           // -> Old State CRC.
      LONG MaxAscent ;          // Max Ascent over baseline (dy).
      LONG MaxDescent ;         // Max Descent below baseline (dy).
      LONG OldMaxDescent ;      // Old Max Descent below baseline (dy).
      UCHAR Dormant ;           // Flag: Word dormant?
      BOOL SegmentStart ;       // Flag: Segment start word?

      Palabra *PrevWord ( ) ;
      Palabra *NextWord ( ) ;

      BOOL IsSeparator ( Text *pText, PUCHAR DBCSVector ) ;
      BOOL IsSeparator ( Text *pText ) ;
      BOOL IsGraphicInline ( Text *pText ) ;
      BOOL IsEndOfDocument ( Text *pText ) ;
      BOOL IsEndOfPage ( Text *pText ) ;
      BOOL IsEndOfLine ( Text *pText ) ;
      BOOL IsTab ( Text *pText ) ;
      BOOL IsSpace ( Text *pText ) ;
      BOOL StartsParagraph ( Text *pText, WorkSpace *PS ) ;
      void Measure ( Text *pText, WorkSpace *PS, Estado &State, int PageNumber, int NumberOfPages, time_t FileDate ) ;
      BOOL LooksLike ( Palabra &OtherWord ) ;
      void Dump ( int indent, Text *pText ) ;
} ;


        // Class: Line - A collection of Words laid along the same vertical line.

class Linea {

   public:
      Estado State ;            // Initial State
      USHORT CRC ;              // State CRC.
      long Offset ;             // Offset into document text.
      long Length ;             // Size in document text.
      int WordNumber ;          // Initial word's number in page
      int WordCount ;           // Number of words in line.
      ULONG Top ;               // Position of line top.
      ULONG Height ;            // Line height.
      LONG MaxAscent ;          // Max Ascent over baseline (dy).
      LONG MaxDescent ;         // Max Descent below baseline (dy).

      Linea *PrevLine ( ) ;
      Linea *NextLine ( ) ;

      void Reset ( WorkSpace *PS, Borradora &Page, Estado &state, Linea *pPrevLine, long LineTop, int PageNumber, int NumberOfPages, time_t FileDate ) ;
      void MarkPainted ( Borradora &Page ) ;
      void EstimateHeight ( WorkSpace *PS, Text *pText, Borradora &Page, Palabra *pFirstWord, 
         Estado &EffectiveState, int Paragraph, int NumberOfPages, time_t Date, int Force, int &MaxAscent, int &MaxDescent ) ;
      void Dump ( int indent ) ;
} ;


        // Class: WorkingPage - A full page description.

class Borradora {               // Class: Working Page

   private:
      long Offset ;             // Offset into document text.
      long Length ;             // Size in document text.
      short PageNumber ;        // Page Number
      Estado State ;            // Initial State
      USHORT CRC ;              // State CRC.
      Estado OldEffState ;      // Effective State the last time we reviewed for screen refresh.
      USHORT OldEffCRC ;        // Old Effective State CRC.
      Estado LastState ;        // State after last word constructed.
      Text *pText ;             // -> Text object

      long RetainedLength ;     // Last successful formatted page length.
      long CurrentLength ;      // Current formatted page length.

      Pool Words ;              // Word pool.
      Pool Lines ;              // Line pool.
      Pool Graphics ;           // Graphic pool.

      RECTL InvalidRectangle ;  // Portion of page which needs repainting due to deletions.

   public:

      Borradora ( ) ;
      virtual ~Borradora ( ) ;

      Borradora & operator= ( const Borradora & Original ) ;

      void     Set_LastState          ( Estado &state ) { LastState = state ; }
      void     Set_LastState          ( Estado *state ) { LastState = *state ; }
      void     Set_Length             ( long length   ) { Length = length ; }
      void     Set_Offset             ( long offset   ) { Offset = offset ; }
      void     Set_PageNumber         ( short number  ) { PageNumber = number ; }
      void     Set_State              ( Estado &state ) { State = state ; }
      void     Set_State              ( Estado *state ) { State = *state ; }
      void     Set_Text               ( Text *ptext   ) { pText = ptext ; }

      long     Query_RetainedLength   ( ) { return ( RetainedLength   ) ; }
      void     Set_RetainedLength     ( long Length ) { RetainedLength = Length ; }

      long     Query_CurrentLength    ( ) { return ( CurrentLength   ) ; }
      void     Set_CurrentLength      ( long Length ) { CurrentLength = Length ; }

      int      Query_NumberOfGraphics ( long Offset ) ;
      int      Query_NumberOfGraphics ( ) { return ( Graphics.Query_Count() ) ; }

      int      Query_NumberOfLines    ( ) { return ( Lines.Query_Count() ) ; }

      int      Query_NumberOfWords    ( ) { return ( Words.Query_Count() ) ; }

      Estado  *Query_LastState        ( ) { return ( &LastState       ) ; }
      long     Query_Length           ( ) { return ( Length           ) ; }
      long     Query_Offset           ( ) { return ( Offset           ) ; }
      long     Query_PageNumber       ( ) { return ( PageNumber       ) ; }
      Estado  *Query_State            ( ) { return ( &State           ) ; }
      Text    *Query_Text             ( ) { return ( pText            ) ; }

      void     Reset ( Estado &State ) ;

      Linea   *FindLine ( int Index ) ;
      Linea   *FindLine ( long Offset, int &Line ) ;
      Linea   *FindLine ( POINTL &Position, int &Line ) ;
      void     ClearLines ( ) ;
      Linea   *AddLine ( Linea &Line ) ;

      BOOL     BuildWord ( int NumberOfPages, time_t FileDate, int WordNumber, WorkSpace *PS ) ;
      Palabra *FindWord ( int Index ) const ;
      Palabra *FindWord ( long Offset ) ;
      Palabra *FindWord ( Palabra *pWord, int Offset ) ;
      Palabra *FindWord ( int Line, int Column ) ;

      BOOL     BuildGraphic ( WorkSpace *PS, Linea *pLine, Palabra *pWord, PUCHAR pByte, int NumberOfPages, time_t FileDate ) ;
      Grafico *FindGraphic ( int Index ) ;
      Grafico *FindGraphic ( long Offset ) ;

      void     Insert ( WorkSpace *PS, long Offset, long Length ) ;
      void     Delete ( WorkSpace *PS, POINTL &PrinterResolution, long Offset, long Length ) ;

      void     Remove_Word ( Palabra *pWord, POINTL &PrinterResolution ) ;
      void     Join_Words ( Palabra *pWord ) ;
      void     Split_Word ( Palabra *pWord, long Offset ) ;

      void     AdjustOffsets ( long Origin, long Delta ) ;

      void     UpdateWordList ( WorkSpace *PS, int NumberOfPages, time_t Date, BOOL CurrentPageValid, long offset, int number, Estado &state, time_t FileDate ) ;

      RECTL   *ComputeInvalidRectangle ( WorkSpace *PS, POINTL &PrinterResolution, int NumberOfPages, time_t FileDate ) ;

      void     Dump ( int indent ) ;
} ;


        // Support Functions

extern void AdjustOffset ( long Origin, long Delta, long &Offset, long &Length, BOOL FixedLength ) ;

extern BOOL IsGlyph         ( PUCHAR pToken, PUCHAR DBCSVector ) ;
inline BOOL IsSpace         (  UCHAR  Token ) { return ( Token == ' ' ) ; }
inline BOOL IsTab           (  UCHAR  Token ) { return ( Token == '\t' ) ; }
inline BOOL IsHyphen        (  UCHAR  Token ) { return ( Token == '-' ) ; }
inline BOOL IsFunction      (  UCHAR  Token ) { return ( Token == 0 ) ; }
extern BOOL IsGlyphFunction ( PUCHAR pToken ) ;
extern BOOL IsGraphic       ( PUCHAR pToken ) ;
extern BOOL IsGraphicInline ( PUCHAR pToken ) ;
extern BOOL IsSeparator     ( PUCHAR pToken ) ;
inline BOOL IsEndOfDocument (  UCHAR  Token ) { return ( Token == '\x1A' ) ; }
inline BOOL IsEndOfPage     (  UCHAR  Token ) { return ( IsEndOfDocument(Token) OR ( Token == '\f' ) ) ; }
extern BOOL IsEndOfLine     ( PUCHAR pToken ) ;
extern BOOL IsEndOfWord     ( PUCHAR pToken, PUCHAR DBCSVector ) ;
extern BOOL IsDBCS          (  UCHAR  Token, PUCHAR DBCSVector ) ;

#endif
