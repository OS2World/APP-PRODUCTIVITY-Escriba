/* RTF User API functions */

#ifndef RTF_H
#define RTF_H

#define REVISION     "RTF 0.97"
#define REVISION_NO  (97)

#define ID_HELPTABLE             1
                                 
#define IDD_PRODUCTINFO          1
#define IDD_CONFIGURE_IMPORT     2
#define IDD_CONFIGURE_EXPORT     3

#define IDS_TITLE                1
#define IDS_TITLE1               2
#define IDS_HELPTITLE            3
#define IDS_ERROR_CREATEHELP     4

#define IDS_MAJORTAB             1000
#define IDS_ERROR_OPEN_VERIFY    1001
#define IDS_ERROR_MEMORY_VERIFY  1002
#define IDS_ERROR_INVALID        1003
#define IDS_ERROR_OPEN_LOAD      1004
#define IDS_ERROR_MEMORY_OPEN    1005
#define IDS_ERROR_WRITE          1006

/* The following definitions were taken from 
   Microsoft's RTF 1.5 specification document
   and from its example code. */

typedef char bool;
#define fTrue 1
#define fFalse 0

typedef struct char_prop {
   int DefaultFont ;
   int Size ;
   unsigned long Color ;
   char fBold ;
   char fItalic ;
   char fUnderscore ;
   char fCaps ;
   char fStrikeout ;
   char fOutline ;
   char_prop ( ) : Size(120), Color(0), fBold(0), fItalic(0), fUnderscore(0), fCaps(0), fStrikeout(0), fOutline(0) { } 
} CHP;                  // CHaracter Properties

typedef enum { justL, justR, justC, justF } JUST ;
typedef struct para_prop {
   int xaLeft;                 // left indent in twips
   int xaRight;                // right indent in twips
   int xaFirst;                // first line indent in twips
   JUST just;                  // justification
   int yaLineSpacing ;         // line spacing in twips
   para_prop ( ) : xaLeft(0), xaRight(0), xaFirst(0), just(justL), yaLineSpacing(1000) { } 
} PAP;                  // PAragraph Properties

typedef enum { sbkNon, sbkCol, sbkEvn, sbkOdd, sbkPg } SBK ;
typedef enum { pgDec, pgURom, pgLRom, pgULtr, pgLLtr } PGN ;
typedef struct sect_prop {
   int cCols;                  // number of columns
   sect_prop ( ) : cCols(0) { } 
} SEP;                  // SEction Properties

typedef struct doc_prop {
   int DefaultTab ;            // Default Tab (in twips)
   int xaPage;                 // page width in twips
   int yaPage;                 // page height in twips
   int xaLeft;                 // left margin in twips
   int yaTop;                  // top margin in twips
   int xaRight;                // right margin in twips
   int yaBottom;               // bottom margin in twips
   doc_prop ( ) : DefaultTab(720), xaPage(12240), yaPage(15480), xaLeft(1800), yaTop(1440), xaRight(1800), yaBottom(1440) { } 
} DOP;                  // DOcument Properties

typedef enum { ftNil, ftTrueType } FT ;

typedef struct fnt_prop {
   int Charset ;                // Character Set / Codepage
   int Pitch ;                  // Pitch: 0 = Default, 1 = Fixed, 2 = Variable
   int Bias ;                   // Bias: 0 = Normal, 1 = Oriental.
   FT Type ;                    // Type: 0 = Nil, 1 = TrueType.
   int CodePage ;               // Character Mapping
   fnt_prop ( ) : Charset(0), Pitch(0), Bias(0), Type(ftNil), CodePage(0) { } 
} FNT ;                 // Font Properties

typedef struct clr_prop {
   unsigned char Red ;
   unsigned char Green ;
   unsigned char Blue ;
   clr_prop ( ) : Red(0), Green(0), Blue(0) { } 
   void Reset ( ) { Red = Green = Blue = 0 ; }
} CLR ;                 

struct FontTable {
   struct FontTable *pNext ;
   int Number ;
   FNT Fnt ;
   char Panose [10] ;
   char Family [40] ;
   char FontName [40] ;
   char AltName [40] ;
   FontTable ( int number ) : pNext(0), Number(number) {
      memset ( Panose, 0, sizeof(Panose) ) ;
      memset ( Family, 0, sizeof(Family) ) ;
      memset ( FontName, 0, sizeof(FontName) ) ;
      memset ( AltName, 0, sizeof(AltName) ) ;
   } /* endmethod */
} ;

typedef struct pict_prop {
   long picw ;
   long pich ;
   long picwgoal ;
   long pichgoal ;
   long picscalex ;
   long picscaley ;            
   int Box ; int BorderDouble ; int BorderWidth ;int BorderColor ;
   pict_prop ( ) : picw(0), pich(0), picwgoal(0), pichgoal(0), picscalex(0), picscaley(0), 
      Box(FALSE), BorderDouble(FALSE), BorderWidth(0), BorderColor(0) { } 
   void Reset ( ) { 
      picw = pich = picwgoal = pichgoal = picscalex = picscaley = 0 ; 
      Box = FALSE ; BorderDouble = FALSE ; BorderWidth = 0 ; BorderColor = 0 ;
   } /* endmethod */
} PICT ;

typedef enum { rdsNorm, rdsSkip, rdsCreateTime, rdsHeader, rdsFooter, rdsTitle, rdsAuthor, rdsFontTable, rdsColorTable, rdsPicture } RDS ; // Rtf Destination State

typedef enum { risNorm, risBin, risHex } RIS ; // Rtf Internal State

typedef struct save {          // property save structure
   struct save *pNext;         // next save
   CHP chp;
   PAP pap;
   SEP sep;
   DOP dop;
   RDS rds;
   RIS ris;
   FontTable *pCurrentFont ;
   save ( ) : pNext(0), rds(rdsNorm), ris(risNorm) { } 
} SAVE;

// What types of properties are there?
typedef enum { ipropBold, ipropItalic, ipropUnderscore, ipropCaps, ipropStrikeout, ipropOutline, ipropDefFont,
   ipropLeftInd, ipropRightInd, ipropFirstInd, ipropCols, 
   ipropDefTab, ipropXaPage, ipropYaPage, ipropXaLeft,
   ipropXaRight, ipropYaTop, ipropYaBottom, 
   ipropJust, ipropLineSpacing, ipropPard, ipropPlain, ipropSectd,
   ipropFontSet, ipropFontPitch, ipropFontBias, ipropFontCP,
   ipropRed, ipropGreen, ipropBlue,
   ipropPicw, ipropPich, ipropPicwGoal, ipropPichGoal, ipropPicScaleX, ipropPicScaleY,
   ipropMax } IPROP ;

typedef enum { actnSpec, actnByte, actnWord } ACTN ;
typedef enum { propChp, propPap, propSep, propDop, propFnt, propColor, propPict } PROPTYPE ;

typedef struct propmod {
   ACTN actn;              // size of value
   PROPTYPE prop;          // structure containing value
   int  offset;            // offset of value from base of structure
   propmod ( ACTN a, PROPTYPE p, int o ) : actn(a), prop(p), offset(o) { } 
   propmod ( ) : actn(actnSpec), prop(propChp), offset(0) { } 
} PROP;

typedef enum { ipfnBin, ipfnHex, ipfnSkipDest, ipfnRtf, ipfnAnsi, ipfnMac, ipfnPc, ipfnPca, 
   ipfnYear, ipfnMonth, ipfnDay, ipfnHour, ipfnMinute, ipfnSecond, 
   ipfnFontNil, ipfnFontRoman, ipfnFontSwiss, ipfnFontModern, ipfnFontScript, ipfnFontDecor, ipfnFontTech, ipfnFontBidi,
   ipfnFontNumber, ipfnFontSize, ipfnFTNil, ipfnFTTrueType, 
   ipfnBullet, ipfnEnDash, ipfnEmDash, ipfnLQuote, ipfnRQuote, ipfnDLQuote, ipfnDRQuote, 
   ipfnOptHyphen, ipfnNBKHyphen, ipfnNBKSpace, ipfnFgColor,
   ipfnBox, ipfnBrdrs, ipfnBrdrth, ipfnBrdrw, ipfnBrdrcf,
   MAX_IPFN
} IPFN ;

typedef enum { idestSkip, idestCreateTime, idestHeader, idestFooter, idestTitle, idestAuthor, idestFontTable, idestColorTable, idestPict } IDEST ;

typedef enum { kwdChar, kwdDest, kwdProp, kwdSpec } KWD ;

typedef struct symbol {
   char *szKeyword;        // RTF keyword
   int  dflt;              // default value to use
   bool fPassDflt;         // true to use default value from this table
   KWD  kwd;               // base action to take
   int  idx;               // index into property table if kwd == kwdProp
                           // index into destination table if kwd == kwdDest
                           // character to print if kwd == kwdChar
   symbol ( char *k1, int d, bool f, KWD k2, int i ) : szKeyword(k1), dflt(d), fPassDflt(f), kwd(k2), idx(i) { } 
   symbol ( ) : szKeyword(0), dflt(0), fPassDflt(0), kwd(kwdChar), idx(0) { } 
   ~symbol ( ) { }
} SYM;

// RTF parser error codes

#define ecOK 0                      // Everything's fine!
#define ecStackUnderflow    1       // Unmatched '}'
#define ecStackOverflow     2       // Too many '{' -- memory exhausted
#define ecUnmatchedBrace    3       // RTF ended during an open group.
#define ecInvalidHex        4       // invalid hex character found in data
#define ecBadTable          5       // RTF table (sym or prop) invalid
#define ecAssertion         6       // Assertion failure
#define ecEndOfFile         7       // End of file reached while reading RTF
#define ecInvalidFormat     8       // RTF file has invalid format

#endif /* RTF_H */
