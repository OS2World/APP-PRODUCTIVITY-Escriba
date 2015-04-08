/******************************************************************** STATE.H
 *                                                                          *
 *  STATE Definitions & Functions                                           *
 *                                                                          *
 ****************************************************************************/

#ifndef STATE_H
#define STATE_H

  // OS/2 Definitions, if not already loaded.

#include "System.h"

  // Other Definitions

#include "DocParms.h"

  // Constants

#define MAX_TABS 20

enum { JUSTIFY_LEFT, JUSTIFY_CENTER, JUSTIFY_RIGHT, JUSTIFY_FULL } ;


  // Data Types

class Estado ;
class WorkSpace ;

class _Estado {
   public:
      HCINFO FormInfo ;
      RECTL  Margins ;
      int    Indent ;
      int    LineSpacing ;
      int    Expand ;
      int    TabCount ;
      int    Tabs [MAX_TABS] ;
      COLOR  Color ;
      COLOR  FillColor ;
      char   Font [FACESIZE+1] ;
      int    Size ;
      char   DateFormat [80] ;
      int    LineWidth ;
      int    LineJoin ;
      int    LineEnd ;
      int    FillType ;
      int    Metric ;
      int    Bold ;
      int    Italic ;
      int    Underscore ;
      int    Strikeout ;
      int    Outline ;
      int    Caps ;
      int    CenterPage ;
      int    Justify ;
      BOOL TabSet ( LONG TabStop ) ;
      virtual void Reset ( WorkSpace*, DocumentParms &DocParms, int PageNumber=0, int NumberOfPages=0, time_t FileDate=0 ) ;
      virtual USHORT ComputeCRC ( ) ;
      virtual void Dump ( int indent=0 ) ;
      virtual void ProcessFunction ( PUCHAR pToken, WorkSpace *PS, int PageNumber=0, int NumberOfPages=0, time_t FileDate=0 ) ;
      _Estado& operator= ( const _Estado& ) ;
      _Estado& operator= ( const Estado& ) ;
} ;

class Estado : public _Estado {
   private:
      int Compute_HF_Height ( WorkSpace *PS, unsigned char *Text, _Estado &InitialState, int PageNumber, int NumberOfPages, time_t FileDate ) ;
      int MeasureFlush ( WorkSpace *PS, char *String, int &StringLength, int MaxWidth, int &Width, int &MaxAscent, int &MaxDescent ) ;
      void RenderFlush ( WorkSpace *PS, char *String, int &StringLength, POINTL &Position ) ;
      void BuildLine ( WorkSpace *PS, int PageNumber, int NumberOfPages, time_t FileDate, 
         unsigned char *Data, int DataSize, _Estado &WorkingState, _Estado &EffectiveState, 
         int &LineStartIndex, int &LineEndIndex, int &NextLineStartIndex, 
         int &MaxAscent, int &MaxDescent, int &Width, int &Spaces ) ;
   public:
      char Header [256] ; _Estado HeaderState ; int HeaderHeight ; 
      char Footer [256] ; _Estado FooterState ; int FooterHeight ; 
      virtual void Reset ( WorkSpace *PS, DocumentParms &DocParms, int PageNumber, int NumberOfPages, time_t FileDate ) ;
      virtual USHORT ComputeCRC ( ) ;
      virtual void Dump ( int indent=0 ) ;
      virtual void ProcessFunction ( PUCHAR pToken, WorkSpace *PS, int PageNumber, int NumberOfPages, time_t FileDate ) ;
      Estado& operator= ( const _Estado& ) ;
      Estado& operator= ( const Estado& ) ;
      void ComputeHeaderHeight ( WorkSpace *PS, int PageNumber, int NumberOfPages, time_t FileDate ) ;
      void ComputeFooterHeight ( WorkSpace *PS, int PageNumber, int NumberOfPages, time_t FileDate ) ;
      void RenderHeader ( WorkSpace *PS, int PageNumber, int NumberOfPages, time_t Date ) ;
      void RenderFooter ( WorkSpace *PS, int PageNumber, int NumberOfPages, time_t Date ) ;
} ;

#endif
