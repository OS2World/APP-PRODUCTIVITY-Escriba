/*************************************************************** DOCPARMS.CPP
 *                                                                          *
 *  DocumentParms Definitions & Functions                                   *
 *                                                                          *
 ****************************************************************************/

#include "System.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Debug.h"
#include "Escriba.h"
#include "External.h"
#include "Printer.h"
#include "Register.h"
#include "ReString.h"
#include "State.h"
#include "Support.h"

#include "DateFmt.h"
#include "DocParms.h"

// #define DEBUG
 
/****************************************************************************
 *                                                                          *
 *  DocumentParms: Constructor                                              *
 *                                                                          *
 ****************************************************************************/

DocumentParms::DocumentParms ( HMODULE RegistrationLibrary ) : 
   Version(0), Size(120), Color(RGB_BLACK), FillColor(RGB_WHITE), 
   Bold(FALSE), Italic(FALSE), Underscore(FALSE), Strikeout(FALSE), Outline(FALSE), Caps(FALSE), 
   Expand(100), Justify(JUSTIFY_LEFT), CenterPage(FALSE), Indent(0), LineSpacing(100), 
   LineType(LINETYPE_DEFAULT), LineWidth(1), LineJoin(LINEJOIN_DEFAULT), 
   LineEnd(LINEEND_DEFAULT), FillType(PATSYM_DEFAULT) {

   // Get country information.
   #ifdef DEBUG
      Log ( "DocumentParms::DocumentParms(RegistrationLibrary=%08X) Started.  Getting country information.", RegistrationLibrary ) ;
   #endif // DEBUG
   COUNTRYCODE CountryCode ;
   COUNTRYINFO CountryInfo ;
   ULONG Count ;
   CountryCode.country = 0 ;
   CountryCode.codepage = 0 ;
   if ( DosGetCtryInfo ( sizeof(CountryInfo), &CountryCode, &CountryInfo, &Count ) ) {
      CountryInfo.fsDateFmt = DATEFMT_MM_DD_YY ;
      CountryInfo.szDateSeparator[0] = '/' ;
      CountryInfo.szDateSeparator[1] = 0 ;
   } /* endif */
   char Text [2] ;
   ULONG ItemSize = 2 ;
   if ( PrfQueryProfileData ( HINI_USERPROFILE, PSZ("PM_National"), PSZ("iDate"), Text, &ItemSize ) ) {
      CountryInfo.fsDateFmt = atoi ( Text ) ;
   } /* endif */
   ItemSize = 2 ;
   PrfQueryProfileData ( HINI_USERPROFILE, PSZ("PM_National"), PSZ("sDate"), CountryInfo.szDateSeparator, &ItemSize ) ;
   strcpy ( Text, "1" ) ;
   ItemSize = 2 ;
   PrfQueryProfileData ( HINI_USERPROFILE, PSZ("PM_National"), PSZ("iMeasurement"), Text, &ItemSize ) ;

   // Set default Document Parameters.
   #ifdef DEBUG
      Log ( "DocumentParms::DocumentParms(RegistrationLibrary=%08X) Getting job settings.", RegistrationLibrary ) ;
   #endif // DEBUG
   strcpy ( PCHAR(Printer), "" ) ;
   QueueInfo Info ( "DocumentParms::DocumentParms", 0, Printer, 0, 0 ) ;
   memset ( JobSettings, 0, sizeof(JobSettings) ) ;
   memcpy ( JobSettings, Info.QuerySettings(), size_t(min(sizeof(JobSettings),Info.QuerySettings()->cb)) ) ;

   // Try out codepages until a usable one is found.  They should be in preference order.
   #ifdef DEBUG
      Log ( "DocumentParms::DocumentParms(RegistrationLibrary=%08X) Started.  Determining codepage to use.", RegistrationLibrary ) ;
   #endif // DEBUG
   PVOID Offset ;
   DosGetResource ( LibraryHandle, RT_RCDATA, 1, &Offset ) ;
   PCHAR DllType = PCHAR ( Offset ) ;                   
   PCHAR AppName = DllType + strlen(DllType) + 1 ;      
   PCHAR Language = AppName + strlen(AppName) + 1 ;     
   PUSHORT pCodePage = PUSHORT ( Language + strlen(Language) + 1 ) ;
   while ( *pCodePage ) {
      if ( !DosSetProcessCp ( *pCodePage ) )
         break ;
      pCodePage ++ ;
   } /* endwhile */

   // Set default values.
   #ifdef DEBUG
      Log ( "DocumentParms::DocumentParms(RegistrationLibrary=%08X) Setting misc defaults.", RegistrationLibrary ) ;
   #endif // DEBUG
   strcpy ( Title, "" ) ;
   strcpy ( Author, "" ) ;
   strcpy ( Header, "" ) ;
   strcpy ( Footer, "" ) ;
   CreateDate = time ( 0 ) ;  
   CodePage = *pCodePage ? *pCodePage : *PUSHORT(Language+strlen(Language)+1) ;
   strcpy ( PCHAR(Font), "Courier" ) ;
   Metric = ( Text[0] == '1' ) ? FALSE : TRUE ;
   TabSpacing = Metric ? 1000 : 500 ;

   #ifdef DEBUG
      Log ( "DocumentParms::DocumentParms(RegistrationLibrary=%08X) Getting form name & size.", RegistrationLibrary ) ;
   #endif // DEBUG
   HCINFO FormInfo ;
   Info.QueryForm ( 0, Metric, FormInfo ) ;
   memset ( FormName, 0, sizeof(FormName) ) ;
   memcpy ( FormName, FormInfo.szFormname, min(sizeof(FormName)-1,strlen(FormInfo.szFormname)) ) ;

   #ifdef DEBUG
      Log ( "DocumentParms::DocumentParms(RegistrationLibrary=%08X) Setting margins.", RegistrationLibrary ) ;
   #endif // DEBUG
   int Margin = Metric ? 2000 : 1000 ;
   LeftMargin   = max ( Margin, FormInfo.xLeftClip+1 ) ;
   RightMargin  = max ( Margin, FormInfo.cx-FormInfo.xRightClip-1 ) ;
   TopMargin    = max ( Margin, FormInfo.cy-FormInfo.yTopClip-1 ) ;
   BottomMargin = max ( Margin, FormInfo.yBottomClip+1 ) ;

   #ifdef DEBUG
      Log ( "DocumentParms::DocumentParms(RegistrationLibrary=%08X) Setting date format.", RegistrationLibrary ) ;
   #endif // DEBUG
   switch ( CountryInfo.fsDateFmt ) {
      case DATEFMT_DD_MM_YY:
         sprintf ( DateFormat, "%%d%s%%m%s%%y", CountryInfo.szDateSeparator, CountryInfo.szDateSeparator ) ;
         break ;
      case DATEFMT_YY_MM_DD:
         sprintf ( DateFormat, "%%y%s%%m%s%%d", CountryInfo.szDateSeparator, CountryInfo.szDateSeparator ) ;
         break ;
      case DATEFMT_MM_DD_YY:
      default:
         sprintf ( DateFormat, "%%m%s%%d%s%%y", CountryInfo.szDateSeparator, CountryInfo.szDateSeparator ) ;
         break ;
   } /* endswitch */

   // Get registration information, if any.
   if ( RegistrationLibrary ) {
      #ifdef DEBUG
         Log ( "DocumentParms::DocumentParms(RegistrationLibrary=%08X) Getting registration info.", RegistrationLibrary ) ;
      #endif // DEBUG
      Registration Info ( RegistrationLibrary ) ;
      if ( Info.IsValid() ) 
         strcpy ( Author, Info.Query_Name() ) ;
   } /* endif */

   #ifdef DEBUG
      Log ( "DocumentParms::DocumentParms(RegistrationLibrary=%08X) Done.", RegistrationLibrary ) ;
   #endif // DEBUG

}
 
/****************************************************************************
 *                                                                          *
 *  DocumentParms: Dump                                                     *
 *                                                                          *
 ****************************************************************************/

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

void DocumentParms::Dump ( int indent ) {

   Log ( "%*sVersion   %04i", indent, "", Version ) ;
   Log ( "%*sTitle     '%s'", indent, "", Title ) ;
   Log ( "%*sAuthor    '%s'", indent, "", Author ) ;
   char Date [80] ;
   FormatDate ( CreateDate, DateFormat, Date, sizeof(Date) ) ;
   Log ( "%*sCreateDate '%s'", indent, "", Date ) ;
   Log ( "%*sPrinter   '%s'", indent, "", Printer ) ;
   Log ( "%*sFormName  '%s'", indent, "", FormName ) ;
   Log ( "%*sCodePage  %i", indent, "", CodePage ) ;
   Log ( "%*sFont      '%s'", indent, "", Font ) ;
   Log ( "%*sSize      %i", indent, "", Size ) ;
   Log ( "%*sLineType  %i", indent, "", LineType ) ;
   Log ( "%*sLineWidth %i", indent, "", LineWidth ) ;
   Log ( "%*sLineJoin  %i", indent, "", LineJoin ) ;
   Log ( "%*sLineEnd   %i", indent, "", LineEnd ) ;
   Log ( "%*sFillType  %i", indent, "", FillType ) ;
   Log ( "%*sColor     %06X", indent, "", Color ) ;
   Log ( "%*sFillColor %06X", indent, "", FillColor ) ;
   Log ( "%*sAttr:     %s%s%s%s%s%s", indent, "",
      Bold?"Bold ":"",
      Italic?"Italic ":"",
      Underscore?"Underscore ":"",
      Strikeout?"Strikeout ":"",
      Outline?"Outline ":"",
      Caps?"Caps ":"" ) ;
   Log ( "%*sExpand:   %i%%", indent, "", Expand ) ;
   Log ( "%*sJustify:  %s", indent, "",
      Justify==JUSTIFY_LEFT?"Left":(
      Justify==JUSTIFY_CENTER?"Center":(
      Justify==JUSTIFY_RIGHT?"Right":"Full" ) ) ) ;
   Log ( "%*sCtrPage:  %s", indent, "", CenterPage?"TRUE":"FALSE" ) ;
   Log ( "%*sUnits:    %s", indent, "", Metric?"Metric":"English" ) ;
   Log ( "%*sLeft:     %i", indent, "", LeftMargin ) ;
   Log ( "%*sRight:    %i", indent, "", RightMargin ) ;
   Log ( "%*sTop:      %i", indent, "", TopMargin ) ;
   Log ( "%*sBottom:   %i", indent, "", BottomMargin ) ;
   Log ( "%*sTabSpc:   %i", indent, "", TabSpacing ) ;
   Log ( "%*sIndent:   %i", indent, "", Indent ) ;
   Log ( "%*sLineSpc:  %i", indent, "", LineSpacing ) ;
   Log ( "%*sDateFmt:  '%s'", indent, "", DateFormat ) ;
   Log ( "%*sHeader:", indent, "" ) ;  ::Dump ( indent+2, (unsigned char*)(Header+1), Header[0] ) ;
   Log ( "%*sFooter:", indent, "" ) ;  ::Dump ( indent+2, (unsigned char*)(Footer+1), Footer[0] ) ;

}

/****************************************************************************
 *                                                                          *
 *  DocumentParms: Process Preface Function                                 *
 *                                                                          *
 ****************************************************************************/

int DocumentParms::ProcessPrefaceFunction ( PUCHAR &pToken ) {

  // Position to the token type.
  char *p = PCHAR(pToken) + 1 ;

  // Update the document parameters according to what token is found.
  switch ( *p++ ) {

     case FN__VERSION:
        Version = (int) *PLONG(p) ;
        p += sizeof(ULONG) + 2 ;
        break;

     case FN__PRINTER:
        memset ( Printer, 0, sizeof(Printer) ) ;
        strncpy ( PCHAR(Printer), p, sizeof(Printer) ) ;
        p += strlen(p) + 3 ;
        break;

     case FN__JOBSETTINGS:
        memcpy ( JobSettings, PDRIVDATA(p), size_t(min(sizeof(JobSettings),PDRIVDATA(p)->cb)) ) ;
        p += PDRIVDATA(p)->cb + 2 ;
        break;

     case FN__FORM:
        memset ( FormName, 0, sizeof(FormName) ) ;
        strncpy ( PCHAR(FormName), p, sizeof(FormName) ) ;
        p += strlen(p) + 3 ;
        break;

     case FN__CODEPAGE:
        CodePage = *PUSHORT(p) ;
        p += sizeof(USHORT) + 2 ;
        break;

     case FN__FONT:
        memset ( Font, 0, sizeof(Font) ) ;
        strncpy ( PCHAR(Font), p, sizeof(Font) ) ;
        p += strlen(p) + 3 ;
        break;

     case FN__SIZE:
        Size = int ( *PULONG(p) ) ;
        p += sizeof(ULONG) + 2 ;
        break;

     case FN__LINECOLOR:
        Color = *PCOLOR(p) ;
        p += sizeof(COLOR) + 2 ;
        break;

     case FN__FILLCOLOR:
        FillColor = *PCOLOR(p) ;
        p += sizeof(COLOR) + 2 ;
        break;

     case FN__BOLD:
        Bold = *PUCHAR(p) ;
        p += sizeof(UCHAR) + 2 ;
        break;

     case FN__ITALIC:
        Italic = *PUCHAR(p) ;
        p += sizeof(UCHAR) + 2 ;
        break;

     case FN__UNDERSCORE:
        Underscore = *PUCHAR(p) ;
        p += sizeof(UCHAR) + 2 ;
        break;

     case FN__STRIKEOUT:
        Strikeout = *PUCHAR(p) ;
        p += sizeof(UCHAR) + 2 ;
        break;

     case FN__OUTLINE:
        Outline = *PUCHAR(p) ;
        p += sizeof(UCHAR) + 2 ;
        break;

     case FN__CAPS:
        Caps = *PUCHAR(p) ;
        p += sizeof(UCHAR) + 2 ;
        break;

     case FN__EXPAND:
        Expand = int ( *PULONG(p) ) ;
        p += sizeof(ULONG) + 2 ;
        break;

     case FN__LEFTJUSTIFY:
        Justify = JUSTIFY_LEFT ;
        p += 2 ;
        break;

     case FN__CENTERJUSTIFY:
        Justify = JUSTIFY_CENTER ;
        p += 2 ;
        break;

     case FN__RIGHTJUSTIFY:
        Justify = JUSTIFY_RIGHT ;
        p += 2 ;
        break;

     case FN__FULLJUSTIFY:
        Justify = JUSTIFY_FULL ;
        p += 2 ;
        break;

     case FN__CENTERPAGE:
        CenterPage = *PUCHAR(p) ;
        p += sizeof(UCHAR) + 2 ;
        break;

     case FN__TOPMARGIN:
        TopMargin = *PLONG(p) ;
        p += sizeof(LONG) + 2 ;
        break;

     case FN__BOTTOMMARGIN:
        BottomMargin = *PLONG(p) ;
        p += sizeof(LONG) + 2 ;
        break;

     case FN__LEFTMARGIN:
        LeftMargin = *PLONG(p) ;
        p += sizeof(LONG) + 2 ;
        break;

     case FN__RIGHTMARGIN:
        RightMargin = *PLONG(p) ;
        p += sizeof(LONG) + 2 ;
        break;

     case FN__TABSPACING:
        TabSpacing = int ( *PLONG(p) ) ;
        p += sizeof(LONG) + 2 ;
        break;

     case FN__INDENT:
        Indent = int ( *PLONG(p) ) ;
        p += sizeof(LONG) + 2 ;
        break;

     case FN__DATEFORMAT:
        memset ( DateFormat, 0, sizeof(DateFormat) ) ;
        memcpy ( DateFormat, p, min(strlen(p),sizeof(DateFormat)-1) ) ;
        p += strlen(p) + 3 ;
        break;

     case FN__METRIC:
        Metric = *PUCHAR(p) ;
        p += sizeof(UCHAR) + 2 ;
        break;

     case FN__LINESPACING:
        LineSpacing = int ( *PLONG(p) ) ;
        p += sizeof(LONG) + 2 ;
        break;

     case FN__LINEWIDTH:
        LineWidth = int ( *PLONG(p) ) ;
        p += sizeof(LONG) + 2 ;
        break;

     case FN__LINEJOIN:
        LineJoin = *PSHORT(p) ;
        p += sizeof(SHORT) + 2 ;
        break;

     case FN__LINEEND:
        LineEnd = *PSHORT(p) ;
        p += sizeof(SHORT) + 2 ;
        break;

     case FN__FILLTYPE:
        FillType = *PSHORT(p) ;
        p += sizeof(SHORT) + 2 ;
        break;

     case FN__AUTHOR:
        strncpy ( Author, p, sizeof(Author) ) ;
        p += strlen(p) + 3 ;
        break;

     case FN__CREATEDATE:
        CreateDate = time_t ( *PLONG(p) ) ;
        p += sizeof(LONG) + 2 ;
        break;

     case FN__COMMENT:
        p += strlen(p) + 3 ;
        break;

     case FN__TITLE:
        strncpy ( Title, p, sizeof(Title) ) ;
        p += strlen(p) + 3 ;
        break;

     case FN__HEADER:
        memcpy ( Header, p, *p+1 ) ;
        p += *p + 3 ;
        break;

     case FN__FOOTER:
        memcpy ( Footer, p, *p+1 ) ;
        p += *p + 3 ;
        break;

     default:
        Log ( "DocumentParms::ProcessPrefaceFunction: WARNING: Invalid function code '%02X'.", *(p-1) ) ;
        p += strlen(p) + 3 ;
        break;

  } /* endswitch */

  // Return the updated token pointer and the token size.
  int Size = p - PCHAR(pToken) ;
  pToken = PUCHAR(p) ;
  return ( Size ) ;
}

