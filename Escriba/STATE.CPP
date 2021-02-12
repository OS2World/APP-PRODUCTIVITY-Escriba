/****************************************************************** STATE.CPP
 *                                                                          *
 *  STATE Functions                                                         *
 *                                                                          *
 ****************************************************************************/

#include "System.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Crc.h"
#include "DateFmt.h"
#include "Debug.h"
#include "Document.h"
#include "DocSup.h"
#include "External.h"
#include "Printer.h"
#include "State.h"

// #define DEBUG

// #define BEEP 
#define BEEP DosBeep(800,10)

/****************************************************************************
 *                                                                          *
 *  STATE: Reset from Document Parameters                                   *
 *                                                                          *
 ****************************************************************************/

void _Estado::Reset ( WorkSpace*, DocumentParms &DocParms, int, int, time_t ) {

   QueueInfo Info ( "Estado::Reset", 0, DocParms.Printer, DocParms.FormName, PDRIVDATA(DocParms.JobSettings) ) ;
   Info.QueryForm ( 0, DocParms.Metric, FormInfo ) ;

   Margins.xLeft   = max ( FormInfo.xLeftClip,   DocParms.LeftMargin ) ;
   Margins.yBottom = max ( FormInfo.yBottomClip, DocParms.BottomMargin ) ;
   Margins.xRight  = min ( FormInfo.xRightClip,  FormInfo.cx - DocParms.RightMargin ) ;
   Margins.yTop    = min ( FormInfo.yTopClip,    FormInfo.cy - DocParms.TopMargin ) ;

   TabCount = 0 ;
   if ( DocParms.TabSpacing )
      for ( int i=int(Margins.xLeft+DocParms.TabSpacing); i<Margins.xRight && TabCount<MAX_TABS; i+=DocParms.TabSpacing )
         Tabs[TabCount++] = i ;

   strcpy ( Font, PCHAR(DocParms.Font) ) ;
   strcpy ( DateFormat, DocParms.DateFormat ) ;

   Indent      = DocParms.Indent ;
   LineSpacing = DocParms.LineSpacing ;
   Size        = DocParms.Size ;
   Color       = DocParms.Color ;
   FillColor   = DocParms.FillColor ;
   Bold        = USHORT ( DocParms.Bold ) ;
   Italic      = USHORT ( DocParms.Italic ) ;
   Underscore  = USHORT ( DocParms.Underscore ) ;
   Strikeout   = USHORT ( DocParms.Strikeout ) ;
   Outline     = USHORT ( DocParms.Outline ) ;
   Caps        = USHORT ( DocParms.Caps ) ;
   Expand      = DocParms.Expand ;
   Justify     = DocParms.Justify ;
   CenterPage  = USHORT ( DocParms.CenterPage ) ;
   Metric      = USHORT ( DocParms.Metric ) ;
   LineWidth   = DocParms.LineWidth ;
   LineJoin    = DocParms.LineJoin ;
   LineEnd     = DocParms.LineEnd ;
   FillType    = DocParms.FillType ;
}

void Estado::Reset ( WorkSpace *PS, DocumentParms &DocParms, int PageNumber, int NumberOfPages, time_t FileDate ) {

   _Estado::Reset ( PS, DocParms ) ;

   if ( PS ) {

      memcpy ( Header, DocParms.Header, DocParms.Header[0]+1 ) ;
      HeaderState = *(_Estado*)this ;
      ComputeHeaderHeight ( PS, PageNumber, NumberOfPages, FileDate ) ;

      memcpy ( Footer, DocParms.Footer, DocParms.Footer[0]+1 ) ;
      FooterState = *(_Estado*)this ;
      ComputeFooterHeight ( PS, PageNumber, NumberOfPages, FileDate ) ;

   } /* endif */
}

/****************************************************************************
 *                                                                          *
 *  STATE: Determine if specified tab is currently set.                     *
 *                                                                          *
 ****************************************************************************/

BOOL _Estado::TabSet ( LONG TabStop ) {
   for ( int i=0; i<TabCount; i++ )
      if ( Tabs[i] == TabStop )
         return ( TRUE ) ;
   return ( FALSE ) ;
}

/****************************************************************************
 *                                                                          *
 *  STATE: Compute CRC-16 for format review purposes                        *
 *                                                                          *
 ****************************************************************************/

USHORT _Estado::ComputeCRC ( ) {
   return ( UpdateCRC16 ( 0, this, sizeof(*this) ) ) ;
}

USHORT Estado::ComputeCRC ( ) {
   return ( UpdateCRC16 ( 0, this, sizeof(*this) ) ) ;
}

/****************************************************************************
 *                                                                          *
 *  STATE: Dump                                                             *
 *                                                                          *
 ****************************************************************************/

void _Estado::Dump ( int indent ) {
   Log ( "%*sFormInfo:", indent, "" ) ;
   Log ( "%*s  Formname: '%s'", indent, "", FormInfo.szFormname ) ;
   Log ( "%*s  Formsize: %ix%i", indent, "", FormInfo.cx, FormInfo.cy ) ;
   Log ( "%*s  Clipping: %i,%i-%i,%i", indent, "", FormInfo.xLeftClip, FormInfo.yBottomClip, FormInfo.xRightClip, FormInfo.yTopClip ) ;
   Log ( "%*s  Pels:     %ix%i", indent, "", FormInfo.xPels, FormInfo.yPels ) ;
   Log ( "%*s  Attrs:    %s%s", indent, "",
      ( FormInfo.flAttributes & HCAPS_CURRENT ? "Current " : "" ),
      ( FormInfo.flAttributes & HCAPS_SELECTABLE ? "Selectable " : "" ) ) ;
   Log ( "%*sLeft:     %i", indent, "", Margins.xLeft ) ;
   Log ( "%*sRight:    %i", indent, "", Margins.xRight ) ;
   Log ( "%*sTop:      %i", indent, "", Margins.yTop ) ;
   Log ( "%*sBottom:   %i", indent, "", Margins.yBottom ) ;
   Log ( "%*sIndent:   %i", indent, "", Indent ) ;
   Log ( "%*sLineSpc:  %i", indent, "", LineSpacing ) ;
   Log ( "%*sExpand:   %i%%", indent, "", Expand ) ;
   char Text [500] ;
   strcpy ( Text, "" ) ;
   for ( int i=0; i<TabCount; i++ ) {
      sprintf ( Text+strlen(Text), "%i ", Tabs[i] ) ;
   } /* endfor */
   Log ( "%*sTabs:     %s", indent, "", Text ) ;
   Log ( "%*sColor:    %06X", indent, "", Color ) ;
   Log ( "%*sFillColor:%06X", indent, "", FillColor ) ;
   Log ( "%*sFont:     '%s'", indent, "", Font ) ;
   Log ( "%*sSize:     %i", indent, "", Size ) ;
   Log ( "%*sLineWidth:%i", indent, "", LineWidth ) ;
   Log ( "%*sLineJoin: %i", indent, "", LineJoin ) ;
   Log ( "%*sLineEnd:  %i", indent, "", LineEnd ) ;
   Log ( "%*sFillType: %i", indent, "", FillType ) ;
   Log ( "%*sDateFmt:  '%s'", indent, "", DateFormat ) ;
   Log ( "%*sUnits:    %s", indent, "", Metric?"Metric":"English" ) ;
   Log ( "%*sAttr:     %s%s%s%s%s%s", indent, "",
      Bold?"Bold ":"",
      Italic?"Italic ":"",
      Underscore?"Underscore ":"",
      Strikeout?"Strikeout ":"",
      Outline?"Outline ":"",
      Caps?"Caps ":"" ) ;
   Log ( "%*sCtrPage:  %s", indent, "", CenterPage?"TRUE":"FALSE" ) ;
   Log ( "%*sJustify:  %s", indent, "",
      (Justify==JUSTIFY_LEFT     ) ? "Left" :
      (Justify==JUSTIFY_CENTER   ) ? "Center" :
      (Justify==JUSTIFY_RIGHT    ) ? "Right" :
      "Left" ) ;
}

static void Dump ( int Indent, unsigned char *Memory, int Size ) {
   char Text [90] ;
   for ( int i=0; i<Size; i+=16 ) {
      sprintf ( Text, "%04X:", i ) ;
      for ( int j=i; j<i+16 && j<Size; j++ ) {
         sprintf ( Text+strlen(Text), " %02X", Memory[j] ) ;
      } /* endfor */
      while ( j<i+16 ) {
         strcat ( Text, "   " ) ;
         j ++ ;
      } /* endfor */
      strcat ( Text, " |" ) ;
      for ( j=i; j<i+16 && j<Size; j++ ) {
         sprintf ( Text+strlen(Text), "%c", isprint(Memory[j])?Memory[j]:'.' ) ;
      } /* endfor */
      while ( j<i+16 ) {
         strcat ( Text, " " ) ;
         j ++ ;
      } /* endfor */
      strcat ( Text, "|" ) ;
      Log ( "%*s%s", Indent, "", Text ) ;
   } /* endfor */
}

void Estado::Dump ( int indent ) {
   _Estado::Dump ( indent ) ;
   Log ( "%*sHeader:", indent, "" ) ;  ::Dump ( indent+2, (unsigned char*)(Header+1), Header[0] ) ;
   Log ( "%*s  HeaderHeight: %i", indent, "", HeaderHeight ) ;
   Log ( "%*s  HeaderState:", indent, "" ) ;  HeaderState.Dump ( indent + 4 ) ;
   Log ( "%*sFooter:", indent, "" ) ;  ::Dump ( indent+2, (unsigned char*)(Footer+1), Footer[0] ) ;
   Log ( "%*s  FooterHeight: %i", indent, "", FooterHeight ) ;
   Log ( "%*s  FooterState:", indent, "" ) ;  FooterState.Dump ( indent + 4 ) ;
}

/****************************************************************************
 *                                                                          *
 *  STATE: Process Function                                                 *
 *                                                                          *
 ****************************************************************************/

void Estado::ProcessFunction ( PUCHAR pToken, WorkSpace *PS, int PageNumber, int NumberOfPages, time_t FileDate ) {

   // Position to the token type.
   char *p = PCHAR(pToken) + 1 ;

   // Update the state according to what token is found.
   switch ( *p++ ) {

      case FN__HEADER:
         memcpy ( Header, p, *p+1 ) ;
         HeaderState = *this ;
         ComputeHeaderHeight ( PS, PageNumber, NumberOfPages, FileDate ) ;
         break;

      case FN__FOOTER:
         memcpy ( Footer, p, *p+1 ) ;
         FooterState = *this ;
         ComputeFooterHeight ( PS, PageNumber, NumberOfPages, FileDate ) ;
         break;

      default:
         _Estado::ProcessFunction ( pToken, PS ) ;
         return ;

   } /* endswitch */

} /* endmethod */

void _Estado::ProcessFunction ( PUCHAR pToken, WorkSpace *PS, int, int, time_t ) {

   // Position to the token type.
   char *p = PCHAR(pToken) + 1 ;

   // Update the state according to what token is found.
   switch ( *p++ ) {

      case FN__FONT:
         memset ( Font, 0, sizeof(Font) ) ;
         strcpy ( Font, p ) ;
         if ( PS ) PS->SetFontName ( Font ) ;
         break;

      case FN__SIZE:
         Size = int(*PULONG(p)) ;
         if ( PS ) PS->SetFontSize ( Size ) ;
         break;

      case FN__LINECOLOR:
         Color = *PCOLOR(p) ;
         if ( PS ) PS->SetColor ( Color ) ;
         break;

      case FN__FILLCOLOR:
         FillColor = *PCOLOR(p) ;
         if ( PS ) PS->SetFillColor ( FillColor ) ;
         break;

      case FN__BOLD:
         Bold = *PUCHAR(p) ;
         if ( PS ) PS->SetBold ( Bold ) ;
         break;

      case FN__ITALIC:
         Italic = *PUCHAR(p) ;
         if ( PS ) PS->SetItalic ( Italic ) ;
         break;

      case FN__UNDERSCORE:
         Underscore = *PUCHAR(p) ;
         if ( PS ) PS->SetUnderscore ( Underscore ) ;
         break;

      case FN__STRIKEOUT:
         Strikeout = *PUCHAR(p) ;
         if ( PS ) PS->SetStrikeout ( Strikeout ) ;
         break;

      case FN__OUTLINE:
         Outline = *PUCHAR(p) ;
         if ( PS ) PS->SetOutline ( Outline ) ;
         break;

      case FN__CAPS:
         Caps = *PUCHAR(p) ;
         if ( PS ) PS->SetCaps ( Caps ) ;
         break;

      case FN__EXPAND:
         Expand = int(*PULONG(p)) ;
         if ( PS ) PS->SetExpand ( Expand ) ;
         break;

      case FN__LEFTJUSTIFY:
         Justify = JUSTIFY_LEFT ;
         break;

      case FN__CENTERJUSTIFY:
         Justify = JUSTIFY_CENTER ;
         break;

      case FN__RIGHTJUSTIFY:
         Justify = JUSTIFY_RIGHT ;
         break;

      case FN__FULLJUSTIFY:
         Justify = JUSTIFY_FULL ;
         break;

      case FN__CENTERPAGE:
         CenterPage = *PUCHAR(p) ;
         break;

      case FN__TOPMARGIN:
         Margins.yTop = min ( *PLONG(p), FormInfo.yTopClip ) ;
         break;

      case FN__BOTTOMMARGIN:
         Margins.yBottom = max ( FormInfo.yBottomClip, *PLONG(p) ) ;
         break;

      case FN__LEFTMARGIN:
         Margins.xLeft = max ( FormInfo.xLeftClip, *PLONG(p) ) ;
         break;

      case FN__RIGHTMARGIN:
         Margins.xRight = min ( *PLONG(p), FormInfo.xRightClip ) ;
         break;

      case FN__INDENT:
         Indent = int(*PLONG(p)) ;
         break;

      case FN__DATEFORMAT:
         memset ( DateFormat, 0, sizeof(DateFormat) ) ;
         memcpy ( DateFormat, p, min(strlen(p),sizeof(DateFormat)-1) ) ;
         break;

      case FN__LINESPACING:
         LineSpacing = int(*PLONG(p)) ;
         break;

      case FN__TABSET: {
         for ( int i=0; i<TabCount; i++ )
            if ( Tabs[i] >= *PLONG(p) )
               break ;
         if ( i < TabCount ) {
            if ( Tabs[i] != *PLONG(p) ) {
               if ( TabCount < MAX_TABS ) {
                  for ( int j=TabCount; j>i; j-- ) {
                     Tabs[j] = Tabs[j-1] ;
                  } /* endfor */
                  Tabs[i] = int(*PLONG(p)) ;
                  TabCount ++ ;
               } /* endif */
            } /* endif */
         } else if ( i < MAX_TABS ) {
            Tabs[TabCount++] = int(*PLONG(p)) ;
         } /* endif */
         break; }

      case FN__TABCLEAR: {
         for ( int i=0; i<TabCount; i++ )
            if ( Tabs[i] == *PLONG(p) )
               break ;
         if ( i < TabCount ) {
            while ( i < TabCount-1 ) {
               Tabs[i] = Tabs[i+1] ;
               i ++ ;
            } /* endfor */
            TabCount -- ;
         } /* endif */
         break; }

      case FN__LINEWIDTH:
         LineWidth = int(*PLONG(p)) ;
         if ( PS ) PS->SetLineWidth ( LineWidth ) ;
         break;

      case FN__LINEJOIN:
         LineJoin = *PSHORT(p) ;
         if ( PS ) PS->SetLineJoin ( LineJoin ) ;
         break;

      case FN__LINEEND:
         LineEnd = *PSHORT(p) ;
         if ( PS ) PS->SetLineEnd ( LineEnd ) ;
         break;

      case FN__FILLTYPE:
         FillType = *PSHORT(p) ;
         if ( PS ) PS->SetFillType ( FillType ) ;
         break;

      case FN__DATEFIXED:
      case FN__DATECURRENT:
      case FN__DATECHANGED:
      case FN__SEPARATOR:
      case FN__PAGENUMBER:
      case FN__PAGECOUNT:
      case FN__GLYPH:
      case FN__GRAPHIC:
      case FN__COMMENT:
         break;

      default:
         Log ( "_Estado::ProcessFunction: WARNING: Invalid function code '%02X'.", *(p-1) ) ;
         BEEP ;
         break;

   } /* endswitch */

} /* endmethod */

/****************************************************************************
 *                                                                          *
 *  STATE: Assignment operators                                             *
 *                                                                          *
 ****************************************************************************/

_Estado& _Estado::operator= ( const _Estado& obj ) {
   memcpy ( this, &obj, sizeof(*this) ) ;
   return ( *this ) ;
}

_Estado& _Estado::operator= ( const Estado& obj ) {
   memcpy ( this, &obj, sizeof(*this) ) ;
   return ( *this ) ;
}

Estado& Estado::operator= ( const Estado& obj ) {
   memcpy ( this, &obj, sizeof(obj) ) ;
   return ( *this ) ;
}

Estado& Estado::operator= ( const _Estado& obj ) {
   memset ( this, 0, sizeof(*this) ) ;
   memcpy ( this, &obj, sizeof(obj) ) ;
   return ( *this ) ;
}

/****************************************************************************
 *                                                                          *
 *  STATE: Compute Header/Footer Height                                     *
 *                                                                          *
 ****************************************************************************/

int Estado::Compute_HF_Height ( WorkSpace *PS, unsigned char *Text, _Estado &InitialState, int PageNumber, int NumberOfPages, time_t FileDate ) {

   // Initialize.
   _Estado WorkingState = InitialState ;
   PS->SetState ( WorkingState ) ;
   int Height ( 0 ) ;

   // Until we run out of text . . .
   int LineStartIndex(0), NextLineStartIndex(0) ;
   while ( LineStartIndex < (unsigned char)Text[0] ) {

      // Compute the line's effective state.
      _Estado LineState = WorkingState ; PS->SetState ( LineState ) ; _Estado EffectiveState ;
      ComputeEffectiveState ( PS, (unsigned char*)&Text[1], Text[0], PS->Query_DBCS_Vector(), LineStartIndex, LineState, EffectiveState ) ;

      // Build up the current line.
      int MaxAscent(0), MaxDescent(0), Width(0), Spaces(0), LineEndIndex(0) ;
      BuildLine ( PS, PageNumber, NumberOfPages, FileDate, (unsigned char*)&Text[1], Text[0], WorkingState, EffectiveState, 
         LineStartIndex, LineEndIndex, NextLineStartIndex, MaxAscent, MaxDescent, Width, Spaces ) ;

      // Move to the next line.
      LineStartIndex = NextLineStartIndex ;
      Height += MaxAscent + MaxDescent ;

   } /* endwhile */

   return ( Height ) ;

} /* endif */

/****************************************************************************
 *                                                                          *
 *  STATE: Render Header                                                    *
 *                                                                          *
 ****************************************************************************/

void Estado::ComputeHeaderHeight ( WorkSpace *PS, int PageNumber, int NumberOfPages, time_t FileDate ) {
   if ( PS ) {
      HeaderHeight = Compute_HF_Height ( PS, (unsigned char*)Header, HeaderState, PageNumber, NumberOfPages, FileDate ) ;
      PS->SetState ( *this ) ;
   } else {
      HeaderHeight = 0 ;
   } /* endif */
} /* endmethod */

/****************************************************************************
 *                                                                          *
 *  STATE: Render Footer                                                    *
 *                                                                          *
 ****************************************************************************/

void Estado::ComputeFooterHeight ( WorkSpace *PS, int PageNumber, int NumberOfPages, time_t FileDate ) {
   if ( PS ) {
      FooterHeight = Compute_HF_Height ( PS, (unsigned char*)Footer, FooterState, PageNumber, NumberOfPages, FileDate ) ;
      PS->SetState ( *this ) ;
   } else {
      FooterHeight = 0 ;
   } /* endif */
} /* endmethod */

/****************************************************************************
 *                                                                          *
 *  STATE: Methods to help in rendering.                                    *
 *                                                                          *
 ****************************************************************************/

int Estado::MeasureFlush ( WorkSpace *PS, char *String, int &StringLength, int MaxWidth, int &Width, int &MaxAscent, int &MaxDescent ) {
   if ( StringLength ) {
      int ThisWidth = PS->MeasureText ( String, StringLength ) ;
      if ( Width && ( Width + ThisWidth > MaxWidth ) )
         return ( TRUE ) ;
      Width += ThisWidth ;
      MaxAscent = max ( MaxAscent, PS->QueryAscent() ) ;
      MaxDescent = max ( MaxDescent, PS->QueryDescent() ) ;
      StringLength = 0 ;
   } /* endif */
   return ( FALSE ) ;
} /* endmethod */

void Estado::RenderFlush ( WorkSpace *PS, char *String, int &StringLength, POINTL &Position ) {
   if ( StringLength ) {
      PS->Move ( Position ) ;
      Position.x += PS->DrawText ( String, StringLength ) ;
      StringLength = 0 ;
   } /* endif */
} /* endmethod */

/****************************************************************************
 *                                                                          *
 *  STATE: Build Header/Footer Line                                         *
 *                                                                          *
 ****************************************************************************/

void Estado::BuildLine ( WorkSpace *PS, int PageNumber, int NumberOfPages, time_t FileDate, 
   unsigned char *Data, int DataSize, _Estado &WorkingState, _Estado &EffectiveState, 
   int &LineStartIndex, int &LineEndIndex, int &NextLineStartIndex, 
   int &MaxAscent, int &MaxDescent, int &Width, int &Spaces ) {

   #ifdef DEBUG
      Log ( "Estado::BuildLine() Trying to build a line from offset %i (max %i).", LineStartIndex, DataSize ) ;
   #endif // DEBUG

   // Gather information about what will fit on this line.
   MaxAscent = MaxDescent = Width = Spaces = 0 ;
   char String[MAX_STRING] ; int StringLength(0) ;
   int Index(LineStartIndex), NextIndex(0) ;  int EndOfLine(FALSE), LineFull(FALSE) ;
   while ( ( Index < DataSize ) && !EndOfLine && !LineFull ) {
      unsigned char *p = Data + Index ;
      if ( IsEndOfLine(p) && !IsSeparator(p) ) {
         NextIndex = Index + 1 ;
         LineFull |= MeasureFlush ( PS, String, StringLength, EffectiveState.Margins.xRight-EffectiveState.Margins.xLeft, Width, MaxAscent, MaxDescent ) ;
         if ( !MaxAscent && !MaxDescent ) {
            MaxAscent = PS->QueryAscent() ;
            MaxDescent = PS->QueryDescent() ;
         } /* endif */
         if ( !LineFull )  {
            NextLineStartIndex = NextIndex ;
            EndOfLine = TRUE ;
         } /* endif */
      } else if ( IsSpace ( *p ) ) {
         NextIndex = Index + 1 ;
         if ( EffectiveState.Justify != JUSTIFY_FULL ) {
            if ( StringLength+2 > sizeof(String) ) {
               LineFull |= MeasureFlush ( PS, String, StringLength, EffectiveState.Margins.xRight-EffectiveState.Margins.xLeft, Width, MaxAscent, MaxDescent ) ;
               if ( !LineFull )
                  NextLineStartIndex = NextIndex ;
            } /* endif */
            strcpy ( String+StringLength, " " ) ;
            StringLength ++ ;
         } /* endif */
         LineFull |= MeasureFlush ( PS, String, StringLength, EffectiveState.Margins.xRight-EffectiveState.Margins.xLeft, Width, MaxAscent, MaxDescent ) ;
         if ( !LineFull )
            NextLineStartIndex = NextIndex ;
         Spaces ++ ;
         Index ++ ;
      } else if ( IsFunction ( *p ) ) {
         long LengthIn ; ComputeFunctionSize ( p, LengthIn ) ;
         NextIndex = Index + LengthIn ;
         if ( IsGlyphFunction ( p ) ) {
            char Text [100] ;
            switch ( *(p+1) ) {
               case FN__DATEFIXED: {
                  FormatDate ( *PULONG(p+2), WorkingState.DateFormat, Text, sizeof(Text) ) ;
                  if ( StringLength+strlen(Text)+1 > sizeof(String) ) {
                     LineFull |= MeasureFlush ( PS, String, StringLength, EffectiveState.Margins.xRight-EffectiveState.Margins.xLeft, Width, MaxAscent, MaxDescent ) ;
                     if ( !LineFull )
                        NextLineStartIndex = NextIndex ;
                  } /* endif */
                  strcpy ( String+StringLength, Text ) ;
                  StringLength += strlen(Text) ;
                  break; }
               case FN__DATECURRENT: {
                  FormatDate ( time(0), WorkingState.DateFormat, Text, sizeof(Text) ) ;
                  if ( StringLength+strlen(Text)+1 > sizeof(String) ) {
                     LineFull |= MeasureFlush ( PS, String, StringLength, EffectiveState.Margins.xRight-EffectiveState.Margins.xLeft, Width, MaxAscent, MaxDescent ) ;
                     if ( !LineFull )
                        NextLineStartIndex = NextIndex ;
                  } /* endif */
                  strcpy ( String+StringLength, Text ) ;
                  StringLength += strlen(Text) ;
                  break; }
               case FN__DATECHANGED: {
                  FormatDate ( FileDate, WorkingState.DateFormat, Text, sizeof(Text) ) ;
                  if ( StringLength+strlen(Text)+1 > sizeof(String) ) {
                     LineFull |= MeasureFlush ( PS, String, StringLength, EffectiveState.Margins.xRight-EffectiveState.Margins.xLeft, Width, MaxAscent, MaxDescent ) ;
                     if ( !LineFull )
                        NextLineStartIndex = NextIndex ;
                  } /* endif */
                  strcpy ( String+StringLength, Text ) ;
                  StringLength += strlen(Text) ;
                  break; }
               case FN__PAGENUMBER: {
                  sprintf ( Text, "%i", PageNumber+1 ) ;
                  if ( StringLength+strlen(Text)+1 > sizeof(String) ) {
                     LineFull |= MeasureFlush ( PS, String, StringLength, EffectiveState.Margins.xRight-EffectiveState.Margins.xLeft, Width, MaxAscent, MaxDescent ) ;
                     if ( !LineFull )
                        NextLineStartIndex = NextIndex ;
                  } /* endif */
                  strcpy ( String+StringLength, Text ) ;
                  StringLength += strlen(Text) ;
                  break; }
               case FN__PAGECOUNT: {
                  sprintf ( Text, "%i", NumberOfPages ) ;
                  if ( StringLength+strlen(Text)+1 > sizeof(String) ) {
                     LineFull |= MeasureFlush ( PS, String, StringLength, EffectiveState.Margins.xRight-EffectiveState.Margins.xLeft, Width, MaxAscent, MaxDescent ) ;
                     if ( !LineFull )
                        NextLineStartIndex = NextIndex ;
                  } /* endif */
                  strcpy ( String+StringLength, Text ) ;
                  StringLength += strlen(Text) ;
                  break; }
               case FN__GLYPH: {
                  sprintf ( Text, "%c", *PUCHAR(p+2) ) ;
                  if ( StringLength+strlen(Text)+1 > sizeof(String) ) {
                     LineFull |= MeasureFlush ( PS, String, StringLength, EffectiveState.Margins.xRight-EffectiveState.Margins.xLeft, Width, MaxAscent, MaxDescent ) ;
                     if ( !LineFull )
                        NextLineStartIndex = NextIndex ;
                  } /* endif */
                  strcpy ( String+StringLength, Text ) ;
                  StringLength += strlen(Text) ;
                  break; }
               case FN__SEPARATOR: {
                  LineFull |= MeasureFlush ( PS, String, StringLength, EffectiveState.Margins.xRight-EffectiveState.Margins.xLeft, Width, MaxAscent, MaxDescent ) ;
                  if ( !LineFull )  {
                     if ( MaxAscent || MaxDescent )
                        NextLineStartIndex = Index ;
                     else {
                        MaxAscent = PS->QueryAscent() ;
                        MaxDescent = PS->QueryDescent() ;
                        Width = EffectiveState.Margins.xRight - EffectiveState.Margins.xLeft ;
                        NextLineStartIndex = NextIndex ;
                     } /* endif */
                     EndOfLine = TRUE ;
                     LineFull |= TRUE ;
                  } /* endif */
                  break; }
            } /* endswitch */
         } else {
            LineFull |= MeasureFlush ( PS, String, StringLength, EffectiveState.Margins.xRight-EffectiveState.Margins.xLeft, Width, MaxAscent, MaxDescent ) ;
            if ( !LineFull ) {
               long LengthIn ; ComputeFunctionSize ( p, LengthIn ) ;
               NextLineStartIndex = Index + LengthIn ;
            } /* endif */
         } /* endif */
         Index = NextIndex ;
         WorkingState.ProcessFunction ( p, PS ) ;
      } else {
         NextIndex = Index + 1 ;
         if ( StringLength >= sizeof(String) ) {
            LineFull |= MeasureFlush ( PS, String, StringLength, EffectiveState.Margins.xRight-EffectiveState.Margins.xLeft, Width, MaxAscent, MaxDescent ) ;
            if ( !LineFull )
               NextLineStartIndex = NextIndex ;
         } /* endif */
         String[StringLength++] = *p ;
         if ( PS->IsDBCSHeader ( *p ) ) {
            NextIndex = Index + 2 ;
            String[StringLength++] = *(p+1) ;
            Index ++ ;
         } /* endif */
         Index ++ ;
      } /* endif */
   } /* endwhile */
   LineFull |= MeasureFlush ( PS, String, StringLength, EffectiveState.Margins.xRight-EffectiveState.Margins.xLeft, Width, MaxAscent, MaxDescent ) ;
   if ( !LineFull )
      NextLineStartIndex = NextIndex ;

   // Adjust Line Start Index to eliminate leading blanks if left, center or full justified.
   int SpaceWidth = PS->MeasureText ( " ", 1 ) ;
   if ( EffectiveState.Justify != JUSTIFY_RIGHT ) {
      while ( ( Data[LineStartIndex] == ' ' ) && ( LineStartIndex < NextLineStartIndex ) ) {
         LineStartIndex ++ ;
         Spaces -- ;
         Width -= SpaceWidth ;
      } /* endwhile */
   } /* endif */

   // Adjust Line End Index to eliminate trailing blanks if center, right or full justified.
   LineEndIndex = NextLineStartIndex ;
   if ( EffectiveState.Justify != JUSTIFY_LEFT ) {
      while ( ( Data[LineEndIndex-1] == ' ' ) && ( LineEndIndex > LineStartIndex ) ) {
         LineEndIndex -- ;
         Spaces -- ;
         Width -= SpaceWidth ;
      } /* endwhile */
   } /* endif */

   // ELABORATE: Tabs are not dealt with here.

} /* endmethod */

/****************************************************************************
 *                                                                          *
 *  STATE: Render Header                                                    *
 *                                                                          *
 ****************************************************************************/

void Estado::RenderHeader ( WorkSpace *PS, int PageNumber, int NumberOfPages, time_t FileDate ) {

   #ifdef DEBUG
      Log ( "Estado::RenderHeader() Started." ) ;
   #endif // DEBUG

   // Initialize.
   _Estado WorkingState = HeaderState ;
   PS->SetState ( WorkingState ) ;
   int Height ( 0 ) ;

   // Until we run out of text . . .
   int LineStartIndex(0), NextLineStartIndex(0) ;
   while ( LineStartIndex < (unsigned char)Header[0] ) {

      // Compute the line's effective state.
      _Estado LineState = WorkingState ; PS->SetState ( LineState ) ; _Estado EffectiveState ;
      ComputeEffectiveState ( PS, (unsigned char*)&Header[1], Header[0], PS->Query_DBCS_Vector(), LineStartIndex, LineState, EffectiveState ) ;

      // Build up the current line.
      int MaxAscent(0), MaxDescent(0), Width(0), Spaces(0), LineEndIndex(0) ;
      BuildLine ( PS, PageNumber, NumberOfPages, FileDate, (unsigned char*)&Header[1], Header[0], WorkingState, EffectiveState, 
         LineStartIndex, LineEndIndex, NextLineStartIndex, MaxAscent, MaxDescent, Width, Spaces ) ;

      #ifdef DEBUG
         Log ( "Estado::RenderHeader() Rendering line from offset %i to %i.", LineStartIndex, LineEndIndex-1 ) ;
      #endif // DEBUG

      // Now render the line we just measured.
      POINTL Position = { EffectiveState.Margins.xLeft, EffectiveState.Margins.yTop-Height-PS->QueryAscent() } ;
      int SpaceWidth ( 0 ) ;
      switch ( EffectiveState.Justify ) {
         case JUSTIFY_CENTER:
            Position.x += ( EffectiveState.Margins.xRight - EffectiveState.Margins.xLeft - Width ) / 2 ;
            break ;
         case JUSTIFY_RIGHT:
            Position.x += EffectiveState.Margins.xRight - EffectiveState.Margins.xLeft - Width ;
            break ;
         case JUSTIFY_FULL:
            SpaceWidth = ( EffectiveState.Margins.xRight - EffectiveState.Margins.xLeft - Width ) / Spaces ;
            break ;
         case JUSTIFY_LEFT:
         default:
            break;
      } /* endswitch */
      WorkingState = LineState ;
      PS->SetState ( WorkingState ) ;
      char String[MAX_STRING] ; int StringLength(0) ;
      int Index ( LineStartIndex ) ;
      while ( Index < LineEndIndex ) {
         unsigned char *p = (unsigned char*) &Header[Index+1] ;
         if ( IsEndOfLine(p) && !IsSeparator(p) ) {
            RenderFlush ( PS, String, StringLength, Position ) ;
            Index ++ ;
         } else if ( IsSpace ( *p ) && ( EffectiveState.Justify == JUSTIFY_FULL ) ) {
            RenderFlush ( PS, String, StringLength, Position ) ;
            SIZEL Size = { SpaceWidth, 0 } ;
            Position.x += PS->DrawSpace ( Size ) ;
            Index ++ ;
         } else if ( IsFunction ( *p ) ) {
            if ( IsGlyphFunction ( p ) ) {
               char Text [100] ;
               switch ( *(p+1) ) {
                  case FN__DATEFIXED: {
                     FormatDate ( *PULONG(p+2), WorkingState.DateFormat, Text, sizeof(Text) ) ;
                     if ( StringLength+strlen(Text)+1 > sizeof(String) )
                        RenderFlush ( PS, String, StringLength, Position ) ;
                     strcpy ( String+StringLength, Text ) ;
                     StringLength += strlen(Text) ;
                     break; }
                  case FN__DATECURRENT: {
                     FormatDate ( time(0), WorkingState.DateFormat, Text, sizeof(Text) ) ;
                     if ( StringLength+strlen(Text)+1 > sizeof(String) )
                        RenderFlush ( PS, String, StringLength, Position ) ;
                     strcpy ( String+StringLength, Text ) ;
                     StringLength += strlen(Text) ;
                     break; }
                  case FN__DATECHANGED: {
                     FormatDate ( FileDate, WorkingState.DateFormat, Text, sizeof(Text) ) ;
                     if ( StringLength+strlen(Text)+1 > sizeof(String) )
                        RenderFlush ( PS, String, StringLength, Position ) ;
                     strcpy ( String+StringLength, Text ) ;
                     StringLength += strlen(Text) ;
                     break; }
                  case FN__PAGENUMBER: {
                     sprintf ( Text, "%i", PageNumber+1 ) ;
                     if ( StringLength+strlen(Text)+1 > sizeof(String) )
                        RenderFlush ( PS, String, StringLength, Position ) ;
                     strcpy ( String+StringLength, Text ) ;
                     StringLength += strlen(Text) ;
                     break; }
                  case FN__PAGECOUNT: {
                     sprintf ( Text, "%i", NumberOfPages ) ;
                     if ( StringLength+strlen(Text)+1 > sizeof(String) )
                        RenderFlush ( PS, String, StringLength, Position ) ;
                     strcpy ( String+StringLength, Text ) ;
                     StringLength += strlen(Text) ;
                     break; }
                  case FN__GLYPH: {
                     sprintf ( Text, "%c", *PUCHAR(p+2) ) ;
                     if ( StringLength+strlen(Text)+1 > sizeof(String) )
                        RenderFlush ( PS, String, StringLength, Position ) ;
                     strcpy ( String+StringLength, Text ) ;
                     StringLength += strlen(Text) ;
                     break; }
                  case FN__SEPARATOR: {
                     if ( Index > LineStartIndex )
                        ;
                     else {
                        SIZEL Size = { EffectiveState.Margins.xRight-EffectiveState.Margins.xLeft, PS->QueryAscent()+PS->QueryDescent() } ;
                        PS->DrawSeparator ( Position, Size ) ;
                     } /* endif */
                     break; }
               } /* endswitch */
            } else {
               RenderFlush ( PS, String, StringLength, Position ) ;
            } /* endif */
            long LengthIn ; ComputeFunctionSize ( p, LengthIn ) ; Index += LengthIn ;
            WorkingState.ProcessFunction ( p, PS ) ;
         } else {
            if ( StringLength >= sizeof(String) )
               RenderFlush ( PS, String, StringLength, Position ) ;
            String[StringLength++] = *p ;
            if ( PS->IsDBCSHeader ( *p ) ) {
               String[StringLength++] = *(p+1) ;
               Index ++ ;
            } /* endif */
            Index ++ ;
         } /* endif */
      } /* endwhile */
      RenderFlush ( PS, String, StringLength, Position ) ;

      // Move to the next line.
      LineStartIndex = NextLineStartIndex ;
      Height += MaxAscent + MaxDescent ;

   } /* endwhile */

   #ifdef DEBUG
      Log ( "Estado::RenderHeader() Done." ) ;
   #endif // DEBUG

} /* endif */

/****************************************************************************
 *                                                                          *
 *  STATE: Render Footer                                                    *
 *                                                                          *
 ****************************************************************************/

void Estado::RenderFooter ( WorkSpace *PS, int PageNumber, int NumberOfPages, time_t FileDate ) {

   #ifdef DEBUG
      Log ( "Estado::RenderFooter() Started." ) ;
   #endif // DEBUG

   // Initialize.
   _Estado WorkingState = FooterState ;
   PS->SetState ( WorkingState ) ;
   int Height ( 0 ) ;

   // Until we run out of text . . .
   int LineStartIndex(0), NextLineStartIndex(0) ;
   while ( LineStartIndex < (unsigned char)Footer[0] ) {

      #ifdef DEBUG
         Log ( "Estado::RenderFooter() Trying to build a line from offset %i (max %i).", LineStartIndex, (unsigned char)Footer[0] ) ;
      #endif // DEBUG

      // Compute the line's effective state.
      _Estado LineState = WorkingState ; PS->SetState ( LineState ) ; _Estado EffectiveState ;
      ComputeEffectiveState ( PS, (unsigned char *)&Footer[1], Footer[0], PS->Query_DBCS_Vector(), LineStartIndex, LineState, EffectiveState ) ;

      // Gather information about what will fit on this line.
      int MaxAscent(0), MaxDescent(0), Width(0), Spaces(0) ;
      char String[MAX_STRING] ; int StringLength(0) ;
      int Index(LineStartIndex), NextIndex(0) ;  int EndOfLine(FALSE), LineFull(FALSE) ;
      while ( ( Index < (unsigned char)Footer[0] ) && !EndOfLine && !LineFull ) {
         unsigned char *p = (unsigned char*) &Footer[Index+1] ;
         if ( IsEndOfLine(p) && !IsSeparator(p) ) {
            NextIndex = Index + 1 ;
            LineFull |= MeasureFlush ( PS, String, StringLength, EffectiveState.Margins.xRight-EffectiveState.Margins.xLeft, Width, MaxAscent, MaxDescent ) ;
            if ( !MaxAscent && !MaxDescent ) {
               MaxAscent = PS->QueryAscent() ;
               MaxDescent = PS->QueryDescent() ;
            } /* endif */
            if ( !LineFull ) {
               NextLineStartIndex = NextIndex ;
               EndOfLine = TRUE ;
            } /* endif */
         } else if ( IsSpace ( *p ) ) {
            NextIndex = Index + 1 ;
            if ( EffectiveState.Justify != JUSTIFY_FULL ) {
               if ( StringLength+2 > sizeof(String) ) {
                  LineFull |= MeasureFlush ( PS, String, StringLength, EffectiveState.Margins.xRight-EffectiveState.Margins.xLeft, Width, MaxAscent, MaxDescent ) ;
                  if ( !LineFull )
                     NextLineStartIndex = NextIndex ;
               } /* endif */
               strcpy ( String+StringLength, " " ) ;
               StringLength ++ ;
            } /* endif */
            LineFull |= MeasureFlush ( PS, String, StringLength, EffectiveState.Margins.xRight-EffectiveState.Margins.xLeft, Width, MaxAscent, MaxDescent ) ;
            if ( !LineFull )
               NextLineStartIndex = NextIndex ;
            Spaces ++ ;
            Index ++ ;
         } else if ( IsFunction ( *p ) ) {
            long LengthIn ; ComputeFunctionSize ( p, LengthIn ) ;
            NextIndex = Index + LengthIn ;
            if ( IsGlyphFunction ( p ) ) {
               char Text [100] ;
               switch ( *(p+1) ) {
                  case FN__DATEFIXED: {
                     FormatDate ( *PULONG(p+2), WorkingState.DateFormat, Text, sizeof(Text) ) ;
                     if ( StringLength+strlen(Text)+1 > sizeof(String) ) {
                        LineFull |= MeasureFlush ( PS, String, StringLength, EffectiveState.Margins.xRight-EffectiveState.Margins.xLeft, Width, MaxAscent, MaxDescent ) ;
                        if ( !LineFull )
                           NextLineStartIndex = NextIndex ;
                     } /* endif */
                     strcpy ( String+StringLength, Text ) ;
                     StringLength += strlen(Text) ;
                     break; }
                  case FN__DATECURRENT: {
                     FormatDate ( time(0), WorkingState.DateFormat, Text, sizeof(Text) ) ;
                     if ( StringLength+strlen(Text)+1 > sizeof(String) ) {
                        LineFull |= MeasureFlush ( PS, String, StringLength, EffectiveState.Margins.xRight-EffectiveState.Margins.xLeft, Width, MaxAscent, MaxDescent ) ;
                        if ( !LineFull )
                           NextLineStartIndex = NextIndex ;
                     } /* endif */
                     strcpy ( String+StringLength, Text ) ;
                     StringLength += strlen(Text) ;
                     break; }
                  case FN__DATECHANGED: {
                     FormatDate ( FileDate, WorkingState.DateFormat, Text, sizeof(Text) ) ;
                     if ( StringLength+strlen(Text)+1 > sizeof(String) ) {
                        LineFull |= MeasureFlush ( PS, String, StringLength, EffectiveState.Margins.xRight-EffectiveState.Margins.xLeft, Width, MaxAscent, MaxDescent ) ;
                        if ( !LineFull )
                           NextLineStartIndex = NextIndex ;
                     } /* endif */
                     strcpy ( String+StringLength, Text ) ;
                     StringLength += strlen(Text) ;
                     break; }
                  case FN__PAGENUMBER: {
                     sprintf ( Text, "%i", PageNumber+1 ) ;
                     if ( StringLength+strlen(Text)+1 > sizeof(String) ) {
                        LineFull |= MeasureFlush ( PS, String, StringLength, EffectiveState.Margins.xRight-EffectiveState.Margins.xLeft, Width, MaxAscent, MaxDescent ) ;
                        if ( !LineFull )
                           NextLineStartIndex = NextIndex ;
                     } /* endif */
                     strcpy ( String+StringLength, Text ) ;
                     StringLength += strlen(Text) ;
                     break; }
                  case FN__PAGECOUNT: {
                     sprintf ( Text, "%i", NumberOfPages ) ;
                     if ( StringLength+strlen(Text)+1 > sizeof(String) ) {
                        LineFull |= MeasureFlush ( PS, String, StringLength, EffectiveState.Margins.xRight-EffectiveState.Margins.xLeft, Width, MaxAscent, MaxDescent ) ;
                        if ( !LineFull )
                           NextLineStartIndex = NextIndex ;
                     } /* endif */
                     strcpy ( String+StringLength, Text ) ;
                     StringLength += strlen(Text) ;
                     break; }
                  case FN__GLYPH: {
                     sprintf ( Text, "%c", *PUCHAR(p+2) ) ;
                     if ( StringLength+strlen(Text)+1 > sizeof(String) ) {
                        LineFull |= MeasureFlush ( PS, String, StringLength, EffectiveState.Margins.xRight-EffectiveState.Margins.xLeft, Width, MaxAscent, MaxDescent ) ;
                        if ( !LineFull )
                           NextLineStartIndex = NextIndex ;
                     } /* endif */
                     strcpy ( String+StringLength, Text ) ;
                     StringLength += strlen(Text) ;
                     break; }
                  case FN__SEPARATOR: {
                     LineFull |= MeasureFlush ( PS, String, StringLength, EffectiveState.Margins.xRight-EffectiveState.Margins.xLeft, Width, MaxAscent, MaxDescent ) ;
                     if ( !LineFull )  {
                        if ( MaxAscent || MaxDescent )
                           NextLineStartIndex = Index ;
                        else {
                           MaxAscent = PS->QueryAscent() ;
                           MaxDescent = PS->QueryDescent() ;
                           Width = EffectiveState.Margins.xRight - EffectiveState.Margins.xLeft ;
                           NextLineStartIndex = NextIndex ;
                        } /* endif */
                        EndOfLine = TRUE ;
                        LineFull |= TRUE ;
                     } /* endif */
                     break; }
               } /* endswitch */
            } else {
               LineFull |= MeasureFlush ( PS, String, StringLength, EffectiveState.Margins.xRight-EffectiveState.Margins.xLeft, Width, MaxAscent, MaxDescent ) ;
               if ( !LineFull )
                  NextLineStartIndex = NextIndex ;
            } /* endif */
            Index = NextIndex ;
            WorkingState.ProcessFunction ( p, PS ) ;
         } else {
            NextIndex = Index + 1 ;
            if ( StringLength >= sizeof(String) ) {
               LineFull |= MeasureFlush ( PS, String, StringLength, EffectiveState.Margins.xRight-EffectiveState.Margins.xLeft, Width, MaxAscent, MaxDescent ) ;
               if ( !LineFull )
                  NextLineStartIndex = NextIndex ;
            } /* endif */
            String[StringLength++] = *p ;
            if ( PS->IsDBCSHeader ( *p ) ) {
               NextIndex = Index + 2 ;
               String[StringLength++] = *(p+1) ;
               Index ++ ;
            } /* endif */
            Index ++ ;
         } /* endif */
      } /* endwhile */
      LineFull |= MeasureFlush ( PS, String, StringLength, EffectiveState.Margins.xRight-EffectiveState.Margins.xLeft, Width, MaxAscent, MaxDescent ) ;
      if ( !LineFull )
         NextLineStartIndex = NextIndex ;

      // Adjust Line Start Index to eliminate leading blanks if left, center or full justified.
      int SpaceWidth = PS->MeasureText ( " ", 1 ) ;
      if ( EffectiveState.Justify != JUSTIFY_RIGHT ) {
         while ( ( (unsigned char)Footer[LineStartIndex+1] == ' ' ) && ( LineStartIndex < NextLineStartIndex ) ) {
            LineStartIndex ++ ;
            Spaces -- ;
            Width -= SpaceWidth ;
         } /* endwhile */
      } /* endif */

      // Adjust Line End Index to eliminate trailing blanks if center, right or full justified.
      int LineEndIndex ( NextLineStartIndex ) ;
      if ( EffectiveState.Justify != JUSTIFY_LEFT ) {
         while ( ( Footer[LineEndIndex] == ' ' ) && ( LineEndIndex > LineStartIndex ) ) {
            LineEndIndex -- ;
            Spaces -- ;
            Width -= SpaceWidth ;
         } /* endwhile */
      } /* endif */

      // ELABORATE: Tabs are not dealt with here.

      #ifdef DEBUG
         Log ( "Estado::RenderFooter() Rendering line from offset %i to %i.", LineStartIndex, LineEndIndex-1 ) ;
      #endif // DEBUG

      // Now render the line we just measured.
      POINTL Position = { EffectiveState.Margins.xLeft, EffectiveState.Margins.yBottom+FooterHeight-Height-PS->QueryAscent() } ;
      SpaceWidth = 0 ;
      switch ( EffectiveState.Justify ) {
         case JUSTIFY_CENTER:
            Position.x += ( EffectiveState.Margins.xRight - EffectiveState.Margins.xLeft - Width ) / 2 ;
            break ;
         case JUSTIFY_RIGHT:
            Position.x += EffectiveState.Margins.xRight - EffectiveState.Margins.xLeft - Width ;
            break ;
         case JUSTIFY_FULL:
            SpaceWidth = ( EffectiveState.Margins.xRight - EffectiveState.Margins.xLeft - Width ) / Spaces ;
            break ;
         case JUSTIFY_LEFT:
         default:
            break;
      } /* endswitch */
      WorkingState = LineState ;
      PS->SetState ( WorkingState ) ;
      Index = LineStartIndex ; StringLength = 0 ;
      while ( Index < LineEndIndex ) {
         unsigned char *p = (unsigned char*) &Footer[Index+1] ;
         if ( IsEndOfLine(p) && !IsSeparator(p) ) {
            RenderFlush ( PS, String, StringLength, Position ) ;
            Index ++ ;
         } else if ( IsSpace ( *p ) && ( EffectiveState.Justify == JUSTIFY_FULL ) ) {
            RenderFlush ( PS, String, StringLength, Position ) ;
            SIZEL Size = { SpaceWidth, 0 } ;
            Position.x += PS->DrawSpace ( Size ) ;
            Index ++ ;
         } else if ( IsFunction ( *p ) ) {
            if ( IsGlyphFunction ( p ) ) {
               char Text [100] ;
               switch ( *(p+1) ) {
                  case FN__DATEFIXED: {
                     FormatDate ( *PULONG(p+2), WorkingState.DateFormat, Text, sizeof(Text) ) ;
                     if ( StringLength+strlen(Text)+1 > sizeof(String) )
                        RenderFlush ( PS, String, StringLength, Position ) ;
                     strcpy ( String+StringLength, Text ) ;
                     StringLength += strlen(Text) ;
                     break; }
                  case FN__DATECURRENT: {
                     FormatDate ( time(0), WorkingState.DateFormat, Text, sizeof(Text) ) ;
                     if ( StringLength+strlen(Text)+1 > sizeof(String) )
                        RenderFlush ( PS, String, StringLength, Position ) ;
                     strcpy ( String+StringLength, Text ) ;
                     StringLength += strlen(Text) ;
                     break; }
                  case FN__DATECHANGED: {
                     FormatDate ( FileDate, WorkingState.DateFormat, Text, sizeof(Text) ) ;
                     if ( StringLength+strlen(Text)+1 > sizeof(String) )
                        RenderFlush ( PS, String, StringLength, Position ) ;
                     strcpy ( String+StringLength, Text ) ;
                     StringLength += strlen(Text) ;
                     break; }
                  case FN__PAGENUMBER: {
                     sprintf ( Text, "%i", PageNumber+1 ) ;
                     if ( StringLength+strlen(Text)+1 > sizeof(String) )
                        RenderFlush ( PS, String, StringLength, Position ) ;
                     strcpy ( String+StringLength, Text ) ;
                     StringLength += strlen(Text) ;
                     break; }
                  case FN__PAGECOUNT: {
                     sprintf ( Text, "%i", NumberOfPages ) ;
                     if ( StringLength+strlen(Text)+1 > sizeof(String) )
                        RenderFlush ( PS, String, StringLength, Position ) ;
                     strcpy ( String+StringLength, Text ) ;
                     StringLength += strlen(Text) ;
                     break; }
                  case FN__GLYPH: {
                     sprintf ( Text, "%c", *PUCHAR(p+2) ) ;
                     if ( StringLength+strlen(Text)+1 > sizeof(String) )
                        RenderFlush ( PS, String, StringLength, Position ) ;
                     strcpy ( String+StringLength, Text ) ;
                     StringLength += strlen(Text) ;
                     break; }
                  case FN__SEPARATOR: {
                     if ( Index > LineStartIndex )
                        ;
                     else {
                        SIZEL Size = { EffectiveState.Margins.xRight-EffectiveState.Margins.xLeft, PS->QueryAscent()+PS->QueryDescent() } ;
                        PS->DrawSeparator ( Position, Size ) ;
                     } /* endif */
                     break; }
               } /* endswitch */
            } else {
               RenderFlush ( PS, String, StringLength, Position ) ;
            } /* endif */
            long LengthIn ; ComputeFunctionSize ( p, LengthIn ) ; Index += LengthIn ;
            WorkingState.ProcessFunction ( p, PS ) ;
         } else {
            if ( StringLength >= sizeof(String) )
               RenderFlush ( PS, String, StringLength, Position ) ;
            String[StringLength++] = *p ;
            if ( PS->IsDBCSHeader ( *p ) ) {
               String[StringLength++] = *(p+1) ;
               Index ++ ;
            } /* endif */
            Index ++ ;
         } /* endif */
      } /* endwhile */
      RenderFlush ( PS, String, StringLength, Position ) ;

      // Move to the next line.
      LineStartIndex = NextLineStartIndex ;
      Height += MaxAscent + MaxDescent ;

   } /* endwhile */

   #ifdef DEBUG
      Log ( "Estado::RenderFooter() Done." ) ;
   #endif // DEBUG

} /* endif */

