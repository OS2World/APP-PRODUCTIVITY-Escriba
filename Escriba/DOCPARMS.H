/***************************************************************** DOCPARMS.H
 *                                                                          *
 *  DocumentParms Definitions & Functions                                   *
 *                                                                          *
 ****************************************************************************/

#ifndef DOCPARMS_H
#define DOCPARMS_H

  // OS/2 Definitions, if not already loaded.

#include "System.h"

  // We will need C time declarations here.

#include <time.h>

  // Document Parameters Structure

class DocumentParms {
   public:
      int    Version ;
      char   Title [80] ;
      char   Author [80] ;
      time_t CreateDate ;
      char   Printer [CCHMAXPATH] ;
      char   JobSettings [4096] ;           // Actually, this is a DRIVDATA structure.
      char   FormName [100] ;
      int    CodePage ;
      char   Font [FACESIZE] ;
      int    Size ;
      COLOR  Color ;
      COLOR  FillColor ;
      int    Bold ;
      int    Italic ;
      int    Underscore ;
      int    Strikeout ;
      int    Outline ;
      int    Caps ;
      int    Expand ;
      int    Justify ;
      int    CenterPage ;
      int    Metric ;
      long   LeftMargin ;
      long   RightMargin ;
      long   TopMargin ;
      long   BottomMargin ;
      int    TabSpacing ;
      int    Indent ;
      int    LineSpacing ;
      char   DateFormat [80] ;
      char   Header [256] ;
      char   Footer [256] ;
      int    LineType ;
      int    LineWidth ;
      int    LineJoin ;
      int    LineEnd ;
      int    FillType ;

      DocumentParms ( HMODULE RegistrationLibrary = 0 ) ;
      void Dump ( int indent ) ;
      int ProcessPrefaceFunction ( PUCHAR &pToken ) ;
} ;

#endif
