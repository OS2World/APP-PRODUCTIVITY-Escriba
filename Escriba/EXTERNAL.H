/***************************************************************** EXTERNAL.H
 *                                                                          *
 *        Escriba Word Processor for OS/2-PM - External Interfaces          *
 *                                                                          *
 ****************************************************************************/

#ifndef EXTERNAL_H
#define EXTERNAL_H

        /* Various Constants used by Escriba */

#define FUNCTION_ESCAPE (15)
#define FUNCTION_START  (17)
#define FUNCTION_END    (16)
#define ENDFILE         (0x1A)

enum {
   FN__VERSION,
   FN__PREFACE,
   FN__PRINTER,
   FN__FORM,
   FN__CODEPAGE,
   FN__FONT,
   FN__SIZE,
   FN__LINECOLOR,
   FN__LINEWIDTH,
   FN__LINEEND,
   FN__LINEJOIN,
   FN__FILLCOLOR,
   FN__FILLTYPE,
   FN__BOLD,
   FN__ITALIC,
   FN__UNDERSCORE,
   FN__STRIKEOUT,
   FN__OUTLINE,
   FN__CAPS,
   FN__EXPAND,
   FN__TOPMARGIN,
   FN__BOTTOMMARGIN,
   FN__LEFTMARGIN,
   FN__RIGHTMARGIN,
   FN__LEFTJUSTIFY,
   FN__CENTERJUSTIFY,
   FN__RIGHTJUSTIFY,
   FN__FULLJUSTIFY,
   FN__CENTERPAGE,
   FN__TABSET,
   FN__TABCLEAR,
   FN__TABSPACING,
   FN__INDENT,
   FN__METRIC,
   FN__GRAPHIC,
   FN__DATEFIXED,
   FN__DATECURRENT,
   FN__DATECHANGED,
   FN__DATEFORMAT,
   FN__LINESPACING,
   FN__JOBSETTINGS,
   FN__PAGENUMBER,
   FN__PAGECOUNT,
   FN__GLYPH,
   FN__AUTHOR,
   FN__CREATEDATE,
   FN__COMMENT,
   FN__SEPARATOR,
   FN__TITLE,
   FN__HEADER,
   FN__FOOTER,
   MAX_FUNCTIONS
} ;

#ifdef DECLARE_FUNCTION_NAMES

extern char *FunctionNames [MAX_FUNCTIONS] = {
   "Version:",
   "Preface:",
   "Printer:",
   "Form:",
   "CodePage:",
   "Font:",
   "Size:",
   "Color:",
   "LineWidth:",
   "LineEnd:",
   "LineJoin:",
   "FillColor:",
   "FillType:",
   "Bold:",
   "Italic:",
   "Underscore:",
   "Strikeout:",
   "Outline:",
   "Caps:",
   "Expand:",
   "TopMargin:",
   "BottomMargin:",
   "LeftMargin:",
   "RightMargin:",
   "LeftJustify",
   "CenterJustify",
   "RightJustify",
   "FullJustify",
   "CenterPage:",
   "TabSet:",
   "TabClear:",
   "TabSpacing:",
   "Indent:",
   "Metric:",
   "Graphic:",
   "Date:Fixed:",
   "Date:Current",
   "Date:Changed",
   "DateFormat:",
   "LineSpacing:",
   "JobSettings:",
   "PageNumber",
   "PageCount",
   "Glyph:",
   "Author:",
   "CreateDate:",
   "Comment:",
   "Separator",
   "Title:",
   "Header:",
   "Footer:"
} ;

#else

extern char *FunctionNames [MAX_FUNCTIONS] ;

#endif

#ifdef __cplusplus
extern "C" {
#endif

        /* Add-on extension type identifiers. */

#define TYPE_ISPELLER       "ISPELLER"
#define TYPE_IMPORTEXPORT   "IMPORT/EXPORT"
#define TYPE_GRAPHICIMPORT  "GRAPHICIMPORT"
#define TYPE_METAFILEIMPORT "METAFILEIMPORT"

        /* Standard functions for all add-on extensions. */

extern char* APIENTRY Identify ( void ) ;                       /* Identify extension type.                     */
extern BOOL  APIENTRY SetLanguage ( char *Language ) ;          /* Set language for menus/dialogs/texts.        */
extern char* APIENTRY ProductName ( void ) ;                    /* Give module name/title.                      */
extern int   APIENTRY ProductVersion ( void ) ;                 /* Give version number x 100.                   */
extern void  APIENTRY ProductInformation ( HWND Owner ) ;       /* Display product information/copyright dialog.*/
extern int   APIENTRY ProductConfigure                          /* Provide configuration notebook page info.    */
   ( int MaxPages, char Names[][64], char MajorTabs[][32], char MinorTabs[][32], PWINDOWPROC Processors[], HMODULE Libraries[], int DialogIDs[] ) ;

        /* Import/Export functions. */

extern char* APIENTRY QueryFileType ( void ) ;                  /* Specify EA filter type.                      */
extern char* APIENTRY QueryWildcard ( void ) ;                  /* Specify filename wildcard filter.            */
extern BOOL  APIENTRY Filter ( char *Name, char *Type ) ;       /* Filter file name & type.                     */
extern BOOL  APIENTRY Verify ( HWND Owner, char *Name ) ;       /* Verify file type by inspection.              */
extern void* APIENTRY Import ( HWND Owner, char *Name, long *Size ) ;           /* Import file from external to Escriba format. */
extern BOOL  APIENTRY Export ( HWND Owner, char *Name, void *Text, long Size ) ;/* Export file from Escriba to external format. */

        /* Graphic Import functions. */

extern char*   APIENTRY QueryFileType ( void ) ;                /* Specify EA filter type.                      */
extern char*   APIENTRY QueryWildcard ( void ) ;                /* Specify filename wildcard filter.            */
extern BOOL    APIENTRY Filter ( char *Name, char *Type ) ;     /* Filter file name & type.                     */
extern BOOL    APIENTRY Verify ( HWND Owner, char *Name ) ;     /* Verify file type by inspection.              */
extern HBITMAP APIENTRY GraphicImport ( HWND Owner, char *Name ) ; /* Import graphic file as system bitmap.     */

#ifdef __cplusplus
}
#endif

#endif
