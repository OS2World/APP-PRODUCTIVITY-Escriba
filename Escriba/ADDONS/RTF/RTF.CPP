/******************************************************************** RTF.CPP
 *                                                                          *
 *              Plain Text Import/Export Add-on for Escriba                 *
 *                                                                          *
 ****************************************************************************/

#include "System.h"

#include <ctype.h>
#include <direct.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define DECLARE_FUNCTION_NAMES

#include "External.h"

#include "Canvas.h"
#include "Controls.h"
#include "Debug.h"
#include "Dialog.h"
#include "EAttr.h"
#include "HelpWin.h"
#include "LModule.h"
#include "Module.h"
#include "Profile.h"
#include "ReString.h"

#include "RTF.h"

// #define DEBUG

/****************************************************************************
 *                                                                          *
 *                       Definitions & Declarations                         *
 *                                                                          *
 ****************************************************************************/

  // Macros & Constants

#define PROGRAM_NAME            "RTF"
#define LANGUAGE_NAME           "RTF_*.DLL"

#define DIM(x) (sizeof(x)/sizeof(x[0]))


  // Function Prototypes

extern "C" {
   int _CRT_init ( void ) ;
   void __ctordtorInit ( void ) ;
   void __ctordtorTerm ( void ) ;
}


  // Globals (private to the library, created by library instance)

extern char HomePath [_MAX_PATH+1] = { 0 } ;
static class LanguageModule *Library = 0 ;
static HMODULE LibraryHandle = 0 ;


/****************************************************************************
 *                                                                          *
 *                    Log Message (Debugging Function)                      *
 *                                                                          *
 ****************************************************************************/

extern void Log ( char *Message, ... ) {

  /**************************************************************************
   * Open the log file.                                                     *
   **************************************************************************/

   char LogFile [_MAX_PATH] ;
   strcpy ( LogFile, HomePath ) ;
   strcat ( LogFile, "\\" PROGRAM_NAME ".log" ) ;

   FILE *File = fopen ( LogFile, "a" ) ;

  /**************************************************************************
   * If the file got opened, write the message to the log file and close it.*
   **************************************************************************/

   if ( File ) {
      char Time [9], Date [9] ;
      fprintf ( File, "%s %s " REVISION " %02i ", _strtime(Time), _strdate(Date), *_threadid ) ;
      va_list Marker ;
      va_start ( Marker, Message ) ;
      vfprintf ( File, Message, Marker ) ;
      va_end ( Marker ) ;
      fprintf ( File, "\n" ) ;
      fclose ( File ) ;
   } /* endif */
}


/****************************************************************************
 *                                                                          *
 *                       DLL Initialization Function                        *
 *                                                                          *
 ****************************************************************************/

extern unsigned long _System _DLL_InitTerm ( unsigned long modhandle, unsigned long flag ) {

  /**************************************************************************
   * If starting up (since cleanup will not be called this way) . . .       *
   **************************************************************************/

   if ( flag == 0 ) {

     /***********************************************************************
      * Call C RunTime initialization.  Abort if error.                     *
      ***********************************************************************/

      if ( _CRT_init() == -1 )
         return ( 0 ) ;

     /***********************************************************************
      * Call static constructors.                                           *
      ***********************************************************************/

      __ctordtorInit ( ) ;

     /***********************************************************************
      * Save the library handle.                                            *
      ***********************************************************************/

      LibraryHandle = modhandle ;

     /***********************************************************************
      * Find out where this module was loaded from.                         *
      ***********************************************************************/

      DosQueryModuleName ( LibraryHandle, sizeof(HomePath), HomePath ) ;
      char Drive[_MAX_DRIVE], Dir[_MAX_DIR], Fname[_MAX_FNAME], Ext[_MAX_EXT] ;
      _splitpath ( HomePath, Drive, Dir, Fname, Ext ) ;
      if ( Dir[strlen(Dir)-1] == '\\' )
         Dir[strlen(Dir)-1] = 0 ;
      strcpy ( HomePath, Drive ) ;
      strcat ( HomePath, Dir ) ;

  /**************************************************************************
   * Else, if shutting down . . .                                           *
   **************************************************************************/

   } else {

     /***********************************************************************
      * Delete the language module, if one's been created.                  *
      ***********************************************************************/

      if ( Library )
         delete Library, Library = 0 ;

     /***********************************************************************
      * Call static destructors.                                            *
      ***********************************************************************/

      __ctordtorTerm ( ) ;

   } /* endif */

  /**************************************************************************
   * Return NZ, indicating success.                                         *
   **************************************************************************/

   return ( 1 ) ;
}


/****************************************************************************
 *                                                                          *
 *  Identify DLL type.                                                      *
 *                                                                          *
 ****************************************************************************/

extern char* APIENTRY Identify ( void ) {
   return ( TYPE_IMPORTEXPORT );
}


/****************************************************************************
 *                                                                          *
 *  Set Language (must be called before anything using Library)             *
 *                                                                          *
 ****************************************************************************/

extern BOOL APIENTRY SetLanguage ( char *Language ) {
   return ( _SetLanguage ( PROGRAM_NAME, REVISION, IDS_TITLE1, Language, LANGUAGE_NAME, HomePath, Library ) ) ;
}


/****************************************************************************
 *                                                                          *
 *  Give name/title.                                                        *
 *                                                                          *
 ****************************************************************************/

extern char* APIENTRY ProductName ( void ) {

   if ( Library == 0 ) {
      Log ( "ProductName aborted because SetLanguage not yet called." ) ;
      return ( "" ) ;
   } /* endif */

   ResourceString Name ( Library->QueryHandle(), IDS_TITLE ) ;

   static char Buffer [200] ;
   strcpy ( Buffer, PCHAR(Name) ) ;

   return ( Buffer ) ;
}


/****************************************************************************
 *                                                                          *
 *  Identify DLL version.  Returned in x100 form.                           *
 *                                                                          *
 ****************************************************************************/

extern int APIENTRY ProductVersion ( void ) {
   return ( REVISION_NO );
}


/****************************************************************************
 *                                                                          *
 *  Product Information Dialog                                              *
 *                                                                          *
 ****************************************************************************/

class AboutDialog : public Dialog {

   private:
      HelpWindow *Help ;

   public:

      AboutDialog ( HWND Parent, HWND Owner, HMODULE Resource, int Id ) : Dialog ( Parent, Owner, Resource, Id ), Help(0) {
         if ( !Load ( ) )
            return ;
         char Drive [_MAX_DRIVE+1], Dir[_MAX_DIR+1], Fname[_MAX_FNAME+1], Ext[_MAX_EXT+1] ;
         _splitpath ( Library->QueryName(), Drive, Dir, Fname, Ext ) ;
         char HelpFileName [CCHMAXPATH] ;
         sprintf ( HelpFileName, "%s\\%s.hlp", HomePath, Fname ) ;
         ResourceString HelpTitle ( Library->QueryHandle(), IDS_HELPTITLE ) ;
         Help = new HelpWindow ( 0, LibraryHandle, ID_HELPTABLE, PSZ(HelpFileName), PSZ(HelpTitle) ) ;
         Help->Associate ( Handle ) ;
      } /* endmethod */

      ~AboutDialog ( ) {
         if ( Help )
            delete Help ;
      } /* endmethod */
} ;

extern void APIENTRY ProductInformation ( HWND Owner ) {
   AboutDialog About ( HWND_DESKTOP, Owner, Library->QueryHandle(), IDD_PRODUCTINFO ) ;
   About.Process ( ) ;
}


/****************************************************************************
 *                                                                          *
 *  Product Configuration Info                                              *
 *                                                                          *
 ****************************************************************************/

typedef struct {
   HWND ErrorField ;
   HelpWindow *Help ;
} CONFIGUREPARMS, *PCONFIGUREPARMS ;

static MRESULT EXPENTRY ImportPageProcessor ( HWND Window, ULONG msg, MPARAM mp1, MPARAM mp2 ) {

   switch ( msg ) {

      case WM_INITDLG: {
//       PHWND phwndErrorField = PHWND ( PVOIDFROMMP ( mp2 ) ) ;
         PCONFIGUREPARMS Parms = new CONFIGUREPARMS ;
         Sys_SetWindowData ( Window, Parms ) ;

         // Associate help instance with the dialog.
         char Drive [_MAX_DRIVE+1], Dir[_MAX_DIR+1], Fname[_MAX_FNAME+1], Ext[_MAX_EXT+1] ;
         _splitpath ( Library->QueryName(), Drive, Dir, Fname, Ext ) ;
         char HelpFileName [CCHMAXPATH] ;
         sprintf ( HelpFileName, "%s\\%s.hlp", HomePath, Fname ) ;
         ResourceString HelpTitle ( Library->QueryHandle(), IDS_HELPTITLE ) ;
         Parms->Help = new HelpWindow ( 0, LibraryHandle, ID_HELPTABLE, PSZ(HelpFileName), PSZ(HelpTitle) ) ;
         Parms->Help->Associate ( Window ) ;

         return ( 0 ) ; }

      case WM_DESTROY: {
         PCONFIGUREPARMS Parms = PCONFIGUREPARMS ( Sys_GetWindowData ( Window ) ) ;
         delete Parms->Help ;
         delete Parms ;
         break ; }

      case WM_CONTROL: {
         PCONFIGUREPARMS Parms = PCONFIGUREPARMS ( Sys_GetWindowData ( Window ) ) ;
//       SHORT Id = SHORT1FROMMP ( mp1 ) ;
         SHORT Message = SHORT2FROMMP ( mp1 ) ;
         switch ( Message ) {

            case BKN_PAGESELECTEDPENDING: {
//             PAGESELECTNOTIFY *pInfo = (PAGESELECTNOTIFY*) PVOIDFROMMP(mp2) ;

               // Clear the error field.
               Sys_SetWindowText ( Parms->ErrorField, "" ) ;

               break ; } /* endcase */

            case BKN_PAGESELECTED: {
//             PAGESELECTNOTIFY *pInfo = (PAGESELECTNOTIFY*) PVOIDFROMMP(mp2) ;
               Sys_SendMessage ( Parms->Help->QueryHandle(), HM_SET_ACTIVE_WINDOW, MPFROMHWND(Window), MPFROMHWND(Window) ) ;
               break ; } /* endcase */

         } /* endswitch */
         break; }

      case WM_COMMAND: {
         return ( Sys_SendMessage ( PARENT(Window), msg, mp1, mp2 ) ) ; }

      case WM_HELP: {
         PCONFIGUREPARMS Parms = PCONFIGUREPARMS ( Sys_GetWindowData ( Window ) ) ;
         Sys_SendMessage ( Parms->Help->QueryHandle(), HM_EXT_HELP, 0, 0 ) ;
         return ( 0 ) ; }

   } /* endswitch */

   return ( Sys_DefDialogProc ( Window, msg, mp1, mp2 ) ) ;
}

static MRESULT EXPENTRY ExportPageProcessor ( HWND Window, ULONG msg, MPARAM mp1, MPARAM mp2 ) {

   switch ( msg ) {

      case WM_INITDLG: {
//       PHWND phwndErrorField = PHWND ( PVOIDFROMMP ( mp2 ) ) ;
         PCONFIGUREPARMS Parms = new CONFIGUREPARMS ;
         Sys_SetWindowData ( Window, Parms ) ;

         // Associate help instance with the dialog.
         char Drive [_MAX_DRIVE+1], Dir[_MAX_DIR+1], Fname[_MAX_FNAME+1], Ext[_MAX_EXT+1] ;
         _splitpath ( Library->QueryName(), Drive, Dir, Fname, Ext ) ;
         char HelpFileName [CCHMAXPATH] ;
         sprintf ( HelpFileName, "%s\\%s.hlp", HomePath, Fname ) ;
         ResourceString HelpTitle ( Library->QueryHandle(), IDS_HELPTITLE ) ;
         Parms->Help = new HelpWindow ( 0, LibraryHandle, ID_HELPTABLE, PSZ(HelpFileName), PSZ(HelpTitle) ) ;
         Parms->Help->Associate ( Window ) ;

         return ( 0 ) ; }

      case WM_DESTROY: {
         PCONFIGUREPARMS Parms = PCONFIGUREPARMS ( Sys_GetWindowData ( Window ) ) ;
         delete Parms->Help ;
         delete Parms ;
         break ; }

      case WM_CONTROL: {
         PCONFIGUREPARMS Parms = PCONFIGUREPARMS ( Sys_GetWindowData ( Window ) ) ;
//       SHORT Id = SHORT1FROMMP ( mp1 ) ;
         SHORT Message = SHORT2FROMMP ( mp1 ) ;
         switch ( Message ) {

            case BKN_PAGESELECTEDPENDING: {
//             PAGESELECTNOTIFY *pInfo = (PAGESELECTNOTIFY*) PVOIDFROMMP(mp2) ;

               // Clear the error field.
               Sys_SetWindowText ( Parms->ErrorField, "" ) ;

               break ; } /* endcase */

            case BKN_PAGESELECTED: {
//             PAGESELECTNOTIFY *pInfo = (PAGESELECTNOTIFY*) PVOIDFROMMP(mp2) ;
               Sys_SendMessage ( Parms->Help->QueryHandle(), HM_SET_ACTIVE_WINDOW, MPFROMHWND(Window), MPFROMHWND(Window) ) ;
               break ; } /* endcase */

         } /* endswitch */
         break; }

      case WM_COMMAND: {
         return ( Sys_SendMessage ( PARENT(Window), msg, mp1, mp2 ) ) ; }

      case WM_HELP: {
         PCONFIGUREPARMS Parms = PCONFIGUREPARMS ( Sys_GetWindowData ( Window ) ) ;
         Sys_SendMessage ( Parms->Help->QueryHandle(), HM_EXT_HELP, 0, 0 ) ;
         return ( 0 ) ; }

   } /* endswitch */

   return ( Sys_DefDialogProc ( Window, msg, mp1, mp2 ) ) ;
}

extern int APIENTRY ProductConfigure ( int /* MaxPages */, char Names[][64], char MajorTabs[][32], char MinorTabs[][32], PWINDOWPROC Processors[], HMODULE Libraries[], int DialogIDs[] ) {

   ResourceString Title ( Library->QueryHandle(), IDS_TITLE ) ;
   ResourceString MajorTab ( Library->QueryHandle(), IDS_MAJORTAB ) ;

   strcpy ( Names[0], PCHAR(Title) ) ;
   strcpy ( MajorTabs[0], PCHAR(MajorTab) ) ;
   strcpy ( MinorTabs[0], "" ) ;
   Processors[0] = ImportPageProcessor ;
   Libraries[0] = Library->QueryHandle() ;
   DialogIDs[0] = IDD_CONFIGURE_IMPORT ;

   strcpy ( Names[1], "" ) ;
   strcpy ( MajorTabs[1], "" ) ;
   strcpy ( MinorTabs[1], "" ) ;
   Processors[1] = ExportPageProcessor ;
   Libraries[1] = Library->QueryHandle() ;
   DialogIDs[1] = IDD_CONFIGURE_EXPORT ;

   return ( 2 ) ;
}


/****************************************************************************
 *                                                                          *
 *  Query Filename EA Type                                                  *
 *                                                                          *
 ****************************************************************************/

extern char* APIENTRY QueryFileType ( void ) {
   return ( "Rich Text Format (RTF)" ) ;
}


/****************************************************************************
 *                                                                          *
 *  Query Filename Wildcard                                                 *
 *                                                                          *
 ****************************************************************************/

extern char* APIENTRY QueryWildcard ( void ) {
   return ( "*.RTF" ) ;
}


/****************************************************************************
 *                                                                          *
 *  Filter Filename & Type                                                  *
 *                                                                          *
 ****************************************************************************/

static BOOL MatchWildcard ( char *Name, char *Wildcard ) {
   char Target [80] = { 0 } ;
   if ( !DosEditName ( 1, Name, Wildcard, Target, sizeof(Target) ) )
      if ( !stricmp ( Name, Target ) )
         return ( TRUE ) ;
   return ( FALSE ) ;
}

extern BOOL APIENTRY Filter ( char *Name, char *Type ) {

   // If extended type is set properly, return TRUE at once.
   if ( !stricmp ( Type, QueryFileType() ) )
      return ( TRUE ) ;

   // Match against *.RTF.
   if ( MatchWildcard ( Name, "*.RTF" ) )
      return ( TRUE ) ;

   // Otherwise, return FALSE.
   return ( FALSE ) ;
}


/****************************************************************************
 *                                                                          *
 *  Class: RTF Parser                                                       *
 *                                                                          *
 ****************************************************************************/

// RTF parser tables

// Property descriptions
static PROP rgprop [ipropMax] = {
   propmod ( actnByte,   propChp,    offsetof(CHP, fBold)       ), // ipropBold
   propmod ( actnByte,   propChp,    offsetof(CHP, fItalic)     ), // ipropItalic
   propmod ( actnByte,   propChp,    offsetof(CHP, fUnderscore) ), // ipropUnderscore
   propmod ( actnByte,   propChp,    offsetof(CHP, fCaps)       ), // ipropCaps
   propmod ( actnByte,   propChp,    offsetof(CHP, fStrikeout)  ), // ipropStrikeout
   propmod ( actnByte,   propChp,    offsetof(CHP, fOutline)    ), // ipropOutline
   propmod ( actnWord,   propChp,    offsetof(CHP, DefaultFont) ), // ipropDefFont
   propmod ( actnWord,   propPap,    offsetof(PAP, xaLeft)      ), // ipropLeftInd
   propmod ( actnWord,   propPap,    offsetof(PAP, xaRight)     ), // ipropRightInd
   propmod ( actnWord,   propPap,    offsetof(PAP, xaFirst)     ), // ipropFirstInd
   propmod ( actnWord,   propSep,    offsetof(SEP, cCols)       ), // ipropCols
   propmod ( actnWord,   propDop,    offsetof(DOP, DefaultTab)  ), // ipropDefTab
   propmod ( actnWord,   propDop,    offsetof(DOP, xaPage)      ), // ipropXaPage
   propmod ( actnWord,   propDop,    offsetof(DOP, yaPage)      ), // ipropYaPage
   propmod ( actnWord,   propDop,    offsetof(DOP, xaLeft)      ), // ipropXaLeft
   propmod ( actnWord,   propDop,    offsetof(DOP, xaRight)     ), // ipropXaRight
   propmod ( actnWord,   propDop,    offsetof(DOP, yaTop)       ), // ipropYaTop
   propmod ( actnWord,   propDop,    offsetof(DOP, yaBottom)    ), // ipropYaBottom
   propmod ( actnByte,   propPap,    offsetof(PAP, just)        ), // ipropJust
   propmod ( actnWord,   propPap,    offsetof(PAP, yaLineSpacing)),// ipropLineSpacing
   propmod ( actnSpec,   propPap,    0                          ), // ipropPard
   propmod ( actnSpec,   propChp,    0                          ), // ipropPlain
   propmod ( actnSpec,   propSep,    0                          ), // ipropSectd
   propmod ( actnWord,   propFnt,    offsetof(FNT, Charset)     ), // ipropFontSet
   propmod ( actnWord,   propFnt,    offsetof(FNT, Pitch)       ), // ipropFontPitch
   propmod ( actnWord,   propFnt,    offsetof(FNT, Bias)        ), // ipropFontBias
   propmod ( actnWord,   propFnt,    offsetof(FNT, CodePage)    ), // ipropFontCP
   propmod ( actnByte,   propColor,  offsetof(CLR, Red)         ), // ipropRed
   propmod ( actnByte,   propColor,  offsetof(CLR, Green)       ), // ipropGreen
   propmod ( actnByte,   propColor,  offsetof(CLR, Blue)        ), // ipropBlue
   propmod ( actnWord,   propPict,   offsetof(PICT,picw)        ), // ipropPicw
   propmod ( actnWord,   propPict,   offsetof(PICT,pich)        ), // ipropPich
   propmod ( actnWord,   propPict,   offsetof(PICT,picwgoal)    ), // ipropPicwGoal
   propmod ( actnWord,   propPict,   offsetof(PICT,pichgoal)    ), // ipropPichGoal
   propmod ( actnWord,   propPict,   offsetof(PICT,picscalex)   ), // ipropPicScaleX
   propmod ( actnWord,   propPict,   offsetof(PICT,picscaley)   ), // ipropPicScaleY
};

// Keyword descriptions
static SYM rgsymRtf[] = {

//          keyword       dflt    fPassDflt   kwd         idx

                // Simple Characters
   symbol ( "page",       0,      fFalse,     kwdChar,    0x0c            ),
   symbol ( "par",        0,      fFalse,     kwdChar,    0x0a            ),
   symbol ( "\0x0a",      0,      fFalse,     kwdChar,    0x0a            ),
   symbol ( "\0x0d",      0,      fFalse,     kwdChar,    0x0a            ),
   symbol ( "tab",        0,      fFalse,     kwdChar,    0x09            ),
   symbol ( "{",          0,      fFalse,     kwdChar,    '{'             ),
   symbol ( "}",          0,      fFalse,     kwdChar,    '}'             ),
   symbol ( "\\",         0,      fFalse,     kwdChar,    '\\'            ),

                // Simple Properties
   symbol ( "b",          1,      fFalse,     kwdProp,    ipropBold       ),
   symbol ( "i",          1,      fFalse,     kwdProp,    ipropItalic     ),
   symbol ( "ul",         1,      fFalse,     kwdProp,    ipropUnderscore ),
   symbol ( "caps",       1,      fFalse,     kwdProp,    ipropCaps       ),
   symbol ( "strike",     1,      fFalse,     kwdProp,    ipropStrikeout  ),
   symbol ( "outl",       1,      fFalse,     kwdProp,    ipropOutline    ),
   symbol ( "deff",       1,      fFalse,     kwdProp,    ipropDefFont    ),
   symbol ( "li",         0,      fFalse,     kwdProp,    ipropLeftInd    ),    // ELABORATE
   symbol ( "ri",         0,      fFalse,     kwdProp,    ipropRightInd   ),    // ELABORATE
   symbol ( "fi",         0,      fFalse,     kwdProp,    ipropFirstInd   ),
   symbol ( "cols",       1,      fFalse,     kwdProp,    ipropCols       ),    // ELABORATE
   symbol ( "deftab",     720,    fFalse,     kwdProp,    ipropDefTab     ),
   symbol ( "paperw",     12240,  fFalse,     kwdProp,    ipropXaPage     ),    // ELABORATE
   symbol ( "paperh",     15480,  fFalse,     kwdProp,    ipropYaPage     ),    // ELABORATE
   symbol ( "margl",      1800,   fFalse,     kwdProp,    ipropXaLeft     ),
   symbol ( "margr",      1800,   fFalse,     kwdProp,    ipropXaRight    ),
   symbol ( "margt",      1440,   fFalse,     kwdProp,    ipropYaTop      ),
   symbol ( "margb",      1440,   fFalse,     kwdProp,    ipropYaBottom   ),
   symbol ( "qc",         justC,  fTrue,      kwdProp,    ipropJust       ),
   symbol ( "ql",         justL,  fTrue,      kwdProp,    ipropJust       ),
   symbol ( "qr",         justR,  fTrue,      kwdProp,    ipropJust       ),
   symbol ( "qj",         justF,  fTrue,      kwdProp,    ipropJust       ),
   symbol ( "sl",         1000,   fFalse,     kwdProp,    ipropLineSpacing),
   symbol ( "pard",       1,      fFalse,     kwdProp,    ipropPard       ),
   symbol ( "plain",      1,      fFalse,     kwdProp,    ipropPlain      ),
   symbol ( "sectd",      1,      fFalse,     kwdProp,    ipropSectd      ),
   symbol ( "fcharset",   0,      fFalse,     kwdProp,    ipropFontSet    ),
   symbol ( "fprq",       0,      fFalse,     kwdProp,    ipropFontPitch  ),
   symbol ( "fbias",      0,      fFalse,     kwdProp,    ipropFontBias   ),
   symbol ( "cpg",        0,      fFalse,     kwdProp,    ipropFontCP     ),
   symbol ( "red",        0,      fFalse,     kwdProp,    ipropRed        ),
   symbol ( "green",      0,      fFalse,     kwdProp,    ipropGreen      ),
   symbol ( "blue",       0,      fFalse,     kwdProp,    ipropBlue       ),
   symbol ( "picw",       0,      fFalse,     kwdProp,    ipropPicw       ),
   symbol ( "pich",       0,      fFalse,     kwdProp,    ipropPich       ),
   symbol ( "picwgoal",   0,      fFalse,     kwdProp,    ipropPicwGoal   ),
   symbol ( "pichgoal",   0,      fFalse,     kwdProp,    ipropPichGoal   ),
   symbol ( "picscalx", 100,      fFalse,     kwdProp,    ipropPicScaleX  ),
   symbol ( "picscaly", 100,      fFalse,     kwdProp,    ipropPicScaleY  ),

                // Implemented Destinations
   symbol ( "creatim",    0,      fFalse,     kwdDest,    idestCreateTime ),
   symbol ( "header",     0,      fFalse,     kwdDest,    idestHeader     ),
   symbol ( "footer",     0,      fFalse,     kwdDest,    idestFooter     ),
   symbol ( "title",      0,      fFalse,     kwdDest,    idestTitle      ),
   symbol ( "author",     0,      fFalse,     kwdDest,    idestAuthor     ),
   symbol ( "fonttbl",    0,      fFalse,     kwdDest,    idestFontTable  ),
   symbol ( "colortbl",   0,      fFalse,     kwdDest,    idestColorTable ),
   symbol ( "pict",       0,      fFalse,     kwdDest,    idestPict       ),

                // Destinations not yet implemented.
   symbol ( "aftncn",     0,      fFalse,     kwdDest,    idestSkip       ),
   symbol ( "aftsep",     0,      fFalse,     kwdDest,    idestSkip       ),
   symbol ( "aftsepc",    0,      fFalse,     kwdDest,    idestSkip       ),
   symbol ( "annotation", 0,      fFalse,     kwdDest,    idestSkip       ),
   symbol ( "atnauthor",  0,      fFalse,     kwdDest,    idestSkip       ),
   symbol ( "atnicn",     0,      fFalse,     kwdDest,    idestSkip       ),
   symbol ( "atnid",      0,      fFalse,     kwdDest,    idestSkip       ),
   symbol ( "atnref",     0,      fFalse,     kwdDest,    idestSkip       ),
   symbol ( "atntime",    0,      fFalse,     kwdDest,    idestSkip       ),
   symbol ( "atnfend",    0,      fFalse,     kwdDest,    idestSkip       ),
   symbol ( "atnfstart",  0,      fFalse,     kwdDest,    idestSkip       ),
   symbol ( "background", 0,      fFalse,     kwdDest,    idestSkip       ),
   symbol ( "bkmend",     0,      fFalse,     kwdDest,    idestSkip       ),
   symbol ( "bkmstart",   0,      fFalse,     kwdDest,    idestSkip       ),
   symbol ( "buptim",     0,      fFalse,     kwdDest,    idestSkip       ),
   symbol ( "category",   0,      fFalse,     kwdDest,    idestSkip       ),
   symbol ( "comment",    0,      fFalse,     kwdDest,    idestSkip       ),
   symbol ( "company",    0,      fFalse,     kwdDest,    idestSkip       ),
   symbol ( "datafield",  0,      fFalse,     kwdDest,    idestSkip       ),
   symbol ( "do",         0,      fFalse,     kwdDest,    idestSkip       ),
   symbol ( "doccomm",    0,      fFalse,     kwdDest,    idestSkip       ),
   symbol ( "docvar",     0,      fFalse,     kwdDest,    idestSkip       ),
   symbol ( "dptxbxtext", 0,      fFalse,     kwdDest,    idestSkip       ),
   symbol ( "falt",       0,      fFalse,     kwdDest,    idestSkip       ),
   symbol ( "ffdeftext",  0,      fFalse,     kwdDest,    idestSkip       ),
   symbol ( "ffentrymcr", 0,      fFalse,     kwdDest,    idestSkip       ),
   symbol ( "ffexitmcr",  0,      fFalse,     kwdDest,    idestSkip       ),
   symbol ( "ffformat",   0,      fFalse,     kwdDest,    idestSkip       ),
   symbol ( "ffhelptext", 0,      fFalse,     kwdDest,    idestSkip       ),
   symbol ( "ffl",        0,      fFalse,     kwdDest,    idestSkip       ),
   symbol ( "ffname",     0,      fFalse,     kwdDest,    idestSkip       ),
   symbol ( "ffstattext", 0,      fFalse,     kwdDest,    idestSkip       ),
   symbol ( "field",      0,      fFalse,     kwdDest,    idestSkip       ),
   symbol ( "file",       0,      fFalse,     kwdDest,    idestSkip       ),
   symbol ( "filetbl",    0,      fFalse,     kwdDest,    idestSkip       ),
   symbol ( "fldinst",    0,      fFalse,     kwdDest,    idestSkip       ),
   symbol ( "fldrslt",    0,      fFalse,     kwdDest,    idestSkip       ),
   symbol ( "fldtype",    0,      fFalse,     kwdDest,    idestSkip       ),
   symbol ( "fontemb",    0,      fFalse,     kwdDest,    idestSkip       ),
   symbol ( "fontfile",   0,      fFalse,     kwdDest,    idestSkip       ),
   symbol ( "footerf",    0,      fFalse,     kwdDest,    idestSkip       ),
   symbol ( "footerl",    0,      fFalse,     kwdDest,    idestSkip       ),
   symbol ( "footerr",    0,      fFalse,     kwdDest,    idestSkip       ),
   symbol ( "footnote",   0,      fFalse,     kwdDest,    idestSkip       ),
   symbol ( "formfield",  0,      fFalse,     kwdDest,    idestSkip       ),
   symbol ( "ftncn",      0,      fFalse,     kwdDest,    idestSkip       ),
   symbol ( "ftnsep",     0,      fFalse,     kwdDest,    idestSkip       ),
   symbol ( "ftnsepc",    0,      fFalse,     kwdDest,    idestSkip       ),
   symbol ( "g",          0,      fFalse,     kwdDest,    idestSkip       ),
   symbol ( "gridtbl",    0,      fFalse,     kwdDest,    idestSkip       ),
   symbol ( "headerf",    0,      fFalse,     kwdDest,    idestSkip       ),
   symbol ( "headerl",    0,      fFalse,     kwdDest,    idestSkip       ),
   symbol ( "headerr",    0,      fFalse,     kwdDest,    idestSkip       ),
   symbol ( "info",       0,      fFalse,     kwdDest,    idestSkip       ),
   symbol ( "keycode",    0,      fFalse,     kwdDest,    idestSkip       ),
   symbol ( "keywords",   0,      fFalse,     kwdDest,    idestSkip       ),
   symbol ( "levelnumbers",0,     fFalse,     kwdDest,    idestSkip       ),
   symbol ( "listname",   0,      fFalse,     kwdDest,    idestSkip       ),
   symbol ( "listtext",   0,      fFalse,     kwdDest,    idestSkip       ),
   symbol ( "manager",    0,      fFalse,     kwdDest,    idestSkip       ),
   symbol ( "nextfile",   0,      fFalse,     kwdDest,    idestSkip       ),
   symbol ( "objalias",   0,      fFalse,     kwdDest,    idestSkip       ),
   symbol ( "objclass",   0,      fFalse,     kwdDest,    idestSkip       ),
   symbol ( "objdata",    0,      fFalse,     kwdDest,    idestSkip       ),
   symbol ( "object",     0,      fFalse,     kwdDest,    idestSkip       ),
   symbol ( "objname",    0,      fFalse,     kwdDest,    idestSkip       ),
   symbol ( "objsect",    0,      fFalse,     kwdDest,    idestSkip       ),
   symbol ( "objtime",    0,      fFalse,     kwdDest,    idestSkip       ),
   symbol ( "operator",   0,      fFalse,     kwdDest,    idestSkip       ),
   symbol ( "panose",     0,      fFalse,     kwdDest,    idestSkip       ),
   symbol ( "picprop",    0,      fFalse,     kwdDest,    idestSkip       ),
   symbol ( "pn",         0,      fFalse,     kwdDest,    idestSkip       ),
   symbol ( "pnseclvl",   0,      fFalse,     kwdDest,    idestSkip       ),
   symbol ( "pntext",     0,      fFalse,     kwdDest,    idestSkip       ),
   symbol ( "pntxta",     0,      fFalse,     kwdDest,    idestSkip       ),
   symbol ( "pntxtb",     0,      fFalse,     kwdDest,    idestSkip       ),
   symbol ( "printim",    0,      fFalse,     kwdDest,    idestSkip       ),
   symbol ( "private",    0,      fFalse,     kwdDest,    idestSkip       ),
   symbol ( "result",     0,      fFalse,     kwdDest,    idestSkip       ),
   symbol ( "revtbl",     0,      fFalse,     kwdDest,    idestSkip       ),
   symbol ( "revtim",     0,      fFalse,     kwdDest,    idestSkip       ),
   symbol ( "rxe",        0,      fFalse,     kwdDest,    idestSkip       ),
   symbol ( "shppict",    0,      fFalse,     kwdDest,    idestSkip       ),
   symbol ( "stylesheet", 0,      fFalse,     kwdDest,    idestSkip       ),
   symbol ( "subject",    0,      fFalse,     kwdDest,    idestSkip       ),
   symbol ( "tc",         0,      fFalse,     kwdDest,    idestSkip       ),
   symbol ( "template",   0,      fFalse,     kwdDest,    idestSkip       ),
   symbol ( "txe",        0,      fFalse,     kwdDest,    idestSkip       ),
   symbol ( "ud",         0,      fFalse,     kwdDest,    idestSkip       ),
   symbol ( "upr",        0,      fFalse,     kwdDest,    idestSkip       ),
   symbol ( "userprops",  0,      fFalse,     kwdDest,    idestSkip       ),
   symbol ( "xe",         0,      fFalse,     kwdDest,    idestSkip       ),

                // Special Keywords
   symbol ( "bin",        0,      fFalse,     kwdSpec,    ipfnBin         ),
   symbol ( "'",          0,      fFalse,     kwdSpec,    ipfnHex         ),
   symbol ( "*",          0,      fFalse,     kwdSpec,    ipfnSkipDest    ),
   symbol ( "rtf",        1,      fFalse,     kwdSpec,    ipfnRtf         ),
   symbol ( "ansi",       1,      fFalse,     kwdSpec,    ipfnAnsi        ),
   symbol ( "mac",        1,      fFalse,     kwdSpec,    ipfnMac         ),
   symbol ( "pc",         1,      fFalse,     kwdSpec,    ipfnPc          ),
   symbol ( "pca",        1,      fFalse,     kwdSpec,    ipfnPca         ),
   symbol ( "yr",         0,      fFalse,     kwdSpec,    ipfnYear        ),
   symbol ( "mo",         0,      fFalse,     kwdSpec,    ipfnMonth       ),
   symbol ( "dy",         0,      fFalse,     kwdSpec,    ipfnDay         ),
   symbol ( "hr",         0,      fFalse,     kwdSpec,    ipfnHour        ),
   symbol ( "min",        0,      fFalse,     kwdSpec,    ipfnMinute      ),
   symbol ( "sec",        0,      fFalse,     kwdSpec,    ipfnSecond      ),
   symbol ( "fnil",       0,      fFalse,     kwdSpec,    ipfnFontNil     ),
   symbol ( "froman",     0,      fFalse,     kwdSpec,    ipfnFontRoman   ),
   symbol ( "fswiss",     0,      fFalse,     kwdSpec,    ipfnFontSwiss   ),
   symbol ( "fmodern",    0,      fFalse,     kwdSpec,    ipfnFontModern  ),
   symbol ( "fscript",    0,      fFalse,     kwdSpec,    ipfnFontScript  ),
   symbol ( "fdecor",     0,      fFalse,     kwdSpec,    ipfnFontDecor   ),
   symbol ( "ftech",      0,      fFalse,     kwdSpec,    ipfnFontTech    ),
   symbol ( "fbidi",      0,      fFalse,     kwdSpec,    ipfnFontBidi    ),
   symbol ( "f",          0,      fFalse,     kwdSpec,    ipfnFontNumber  ),
   symbol ( "fs",         0,      fFalse,     kwdSpec,    ipfnFontSize    ),
   symbol ( "ftnil",      0,      fFalse,     kwdSpec,    ipfnFTNil       ),
   symbol ( "fttruetype", 0,      fFalse,     kwdSpec,    ipfnFTTrueType  ),
   symbol ( "bullet",     0,      fFalse,     kwdSpec,    ipfnBullet      ),
   symbol ( "endash",     0,      fFalse,     kwdSpec,    ipfnEnDash      ),
   symbol ( "emdash",     0,      fFalse,     kwdSpec,    ipfnEmDash      ),
   symbol ( "lquote",     0,      fFalse,     kwdSpec,    ipfnLQuote      ),
   symbol ( "rquote",     0,      fFalse,     kwdSpec,    ipfnRQuote      ),
   symbol ( "ldblquote",  0,      fFalse,     kwdSpec,    ipfnDLQuote     ),
   symbol ( "rdblquote",  0,      fFalse,     kwdSpec,    ipfnDRQuote     ),
   symbol ( "-",          0,      fFalse,     kwdSpec,    ipfnOptHyphen   ),
   symbol ( "_",          0,      fFalse,     kwdSpec,    ipfnNBKHyphen   ),
   symbol ( "~",          0,      fFalse,     kwdSpec,    ipfnNBKSpace    ),
   symbol ( "cf",         0,      fFalse,     kwdSpec,    ipfnFgColor     ),
   symbol ( "box",        0,      fFalse,     kwdSpec,    ipfnBox         ),
   symbol ( "brdrs",      0,      fFalse,     kwdSpec,    ipfnBrdrs       ),
   symbol ( "brdrw",      0,      fFalse,     kwdSpec,    ipfnBrdrw       ),
   symbol ( "brdrcf",     0,      fFalse,     kwdSpec,    ipfnBrdrcf      ),
};

static char *RTFErrors [] = {
   "OK",
   "Stack Underflow",
   "Stack Overflow",
   "Unmatched Brace",
   "Invalid Hex",
   "Bad Table",
   "Assertion",
   "EndOfFile"
} ;

class RTFParser_OpenError {
   public:
      RTFParser_OpenError ( ) { } ;
} ;

class RTFParser {

   private:
      char *Name ;
      FILE *File ;
      int OutSize ;
      char *OutBuffer ;
      bool fFirstToken ;
      bool fEmitting ;
      int cGroup ;
      RDS rds ;
      RIS ris ;
      CHP chp ;
      PAP pap ;
      SEP sep ;
      DOP dop ;
      PICT pict ;
      SAVE *psave ;
      long cbBin ;
      long lParam ;
      bool fSkipDestIfUnk ;
      char Title [256] ;
      char Author [256] ;
      int CodePage ;
      int Year, Month, Day, Hour, Minute, Second ;
      time_t CreateTime ;
      int ColorIndex ; CLR Colors[256] ;
      struct DestinationState {
         char FontName [40] ;
         int Size ;
         unsigned long Color ;
         int Bold : 1 ;
         int Italic : 1 ;
         int Underscore : 1 ;
         int Caps : 1 ;
         int Strikeout : 1 ;
         int Outline : 1 ;
         int LeftMargin, RightMargin, TopMargin, BottomMargin, Indent ;
         JUST Justify ;
         int LineSpacing ;
         DestinationState ( ) : Size(0), Color(0), Bold(0), Italic(0),
            Underscore(0), Caps(0), Strikeout(0), Outline(0),
            LeftMargin(1800), RightMargin(1800), TopMargin(1440), BottomMargin(1440),
            Indent(0), Justify(justL), LineSpacing(1000) {
            memset ( FontName, 0, sizeof(FontName) ) ;
         } /* endmethod */
         void Reset ( ) {
            Size = 0 ; Color = 0 ;
            Bold = 0 ; Italic = 0 ; Underscore = 0 ; Caps = 0 ; Strikeout = 0 ; Outline = 0 ;
            LeftMargin = RightMargin = 1800 ;
            TopMargin = BottomMargin = 1440 ;
            Indent = 0 ; Justify = justL ;
            LineSpacing = 1000 ;
            memset ( FontName, 0, sizeof(FontName) ) ;
         } /* endmethod */
      } NormalState, HeaderState, FooterState ;
      struct FontTable *pFontTable, *pCurrentFont ;

      void Reset ( char *outbuffer = 0 ) {

         // Reset parser state entirely.
         fseek ( File, 0, SEEK_SET ) ;
         OutSize = 0 ;
         OutBuffer = outbuffer ;
         fFirstToken = fTrue ;
         fEmitting = fFalse ;
         cGroup = 0 ;
         rds = rdsNorm ;
         ris = risNorm ;
         chp = char_prop ( ) ;
         pap = para_prop ( ) ;
         sep = sect_prop ( ) ;
         dop = doc_prop ( ) ;
         pict = pict_prop ( ) ;
         psave = 0 ;
         cbBin = 0 ;
         lParam = 0 ;
         fSkipDestIfUnk = 0 ;
         CodePage = 1252 ;
         memset ( Title, 0, sizeof(Title) ) ;
         memset ( Author, 0, sizeof(Author) ) ;
         Year = Month = Day = Hour = Minute = Second = 0 ;
         CreateTime = 0 ;
         NormalState.Reset ( ) ;
         HeaderState.Reset ( ) ;
         FooterState.Reset ( ) ;

         ColorIndex = 0 ;
         for ( int i=0; i<DIM(Colors); i++ )
            Colors[i].Reset ( ) ;

         while ( pFontTable ) {
            struct FontTable *pNext ( pFontTable->pNext ) ;
            delete pFontTable ;
            pFontTable = pNext ;
         } /* endwhile */
         pCurrentFont = 0 ;

      } /* endmethod */

      int ParseSpecialProperty ( IPROP iprop, int val ) {
         #ifdef DEBUG
            Log ( "RTFParser::ParseSpecialProperty(iprop=%i,val=%i)", iprop, val ) ;
         #else
            val ++ ;
         #endif // DEBUG
         switch ( iprop ) {
            case ipropPard:
               pap = para_prop ( ) ;
               #ifdef DEBUG
                  Log ( "RTFParser::ParseSpecialProperty() Paragraph default.  Cleared paragraph properties." ) ;
               #endif // DEBUG
               return ( ecOK ) ;
            case ipropPlain:
               chp = char_prop ( ) ;
               #ifdef DEBUG
                  Log ( "RTFParser::ParseSpecialProperty() Plain text.  Cleared character properties." ) ;
               #endif // DEBUG
               return ( ecOK ) ;
            case ipropSectd:
               sep = sect_prop ( ) ;
               #ifdef DEBUG
                  Log ( "RTFParser::ParseSpecialProperty() Section default.  Cleared section properties." ) ;
               #endif // DEBUG
               return ( ecOK ) ;
            default:
               #ifdef DEBUG
                  Log ( "RTFParser::ParseSpecialProperty() ERROR: Unknown property type." ) ;
               #endif // DEBUG
               return ( ecBadTable ) ;
         } /* endswitch */
      } /* endmethod */

      int ApplyPropChange ( IPROP iprop, int val ) {
         #ifdef DEBUG
            Log ( "RTFParser::ApplyPropChange(iprop=%i,val=%i)", iprop, val ) ;
         #endif // DEBUG
         if ( rds == rdsSkip ) {            // If we're skipping text,
            #ifdef DEBUG
               Log ( "RTFParser::ApplyPropChange() Skipping." ) ;
            #endif // DEBUG
            return ( ecOK ) ;               // don't do anything.
         } /*endif */
         char *pb;
         switch ( rgprop[iprop].prop ) {
            case propDop:
               #ifdef DEBUG
                  Log ( "RTFParser::ApplyPropChange() Setting pointer to document properties." ) ;
               #endif // DEBUG
               pb = (char*)&dop;
               break;
            case propSep:
               #ifdef DEBUG
                  Log ( "RTFParser::ApplyPropChange() Setting pointer to section properties." ) ;
               #endif // DEBUG
               pb = (char*)&sep;
               break;
            case propPap:
               #ifdef DEBUG
                  Log ( "RTFParser::ApplyPropChange() Setting pointer to page properties." ) ;
               #endif // DEBUG
               pb = (char*)&pap;
               break;
            case propChp:
               #ifdef DEBUG
                  Log ( "RTFParser::ApplyPropChange() Setting pointer to character properties." ) ;
               #endif // DEBUG
               pb = (char*)&chp;
               break;
            case propFnt:
               if ( !pCurrentFont ) {
                  #ifdef DEBUG
                     Log ( "RTFParser::ApplyPropChange() ERROR: No current font table entry defined." ) ;
                  #endif // DEBUG
                  return ( ecInvalidFormat ) ;
               } /* endif */
               #ifdef DEBUG
                  Log ( "RTFParser::ApplyPropChange() Setting pointer to font properties." ) ;
               #endif // DEBUG
               pb = (char*)&pCurrentFont->Fnt;
               break;
            case propColor:
               pb = (char*)&Colors[ColorIndex] ;
               break;
            case propPict:
               pb = (char*)&pict ;
               break;
            default:
               if ( rgprop[iprop].actn != actnSpec ) {
                  #ifdef DEBUG
                     Log ( "RTFParser::ApplyPropChange() ERROR: Unknown property class." ) ;
                  #endif // DEBUG
                  return ( ecBadTable ) ;
               } /* endif */
               break;
         } /* endswitch */
         switch ( rgprop[iprop].actn ) {
            case actnByte:
               #ifdef DEBUG
                  Log ( "RTFParser::ApplyPropChange() Updating byte property." ) ;
               #endif // DEBUG
               pb[rgprop[iprop].offset] = (unsigned char) val;
               break;
            case actnWord:
               #ifdef DEBUG
                  Log ( "RTFParser::ApplyPropChange() Updating word property." ) ;
               #endif // DEBUG
               (*(int *) (pb+rgprop[iprop].offset)) = val;
               break;
            case actnSpec:
               #ifdef DEBUG
                  Log ( "RTFParser::ApplyPropChange() Updating special property." ) ;
               #endif // DEBUG
               return ( ParseSpecialProperty ( iprop, val ) ) ;
            default:
               #ifdef DEBUG
                  Log ( "RTFParser::ApplyPropChange() ERROR: Invalid property action type." ) ;
               #endif // DEBUG
               return ( ecBadTable ) ;
         } /* endswitch */
         #ifdef DEBUG
            Log ( "RTFParser::ApplyPropChange() Done OK." ) ;
         #endif // DEBUG
         return ( ecOK ) ;
      } /* endmethod */

      void EmitPreamble ( ) {

         // If we've already started, we need not start over.
         if ( fEmitting )
            return ;

         #ifdef DEBUG
            Log ( "RTFParser::EmitPreamble() Emitting document preface." ) ;
         #endif // DEBUG

         // Emit the document preface.
         if ( OutBuffer ) {
            OutSize += sprintf ( OutBuffer+OutSize, "%c%s", FUNCTION_START, FunctionNames[FN__PREFACE] ) ;
            OutSize += sprintf ( OutBuffer+OutSize, "%c%s%i%c", FUNCTION_START, FunctionNames[FN__TABSPACING], long((dop.DefaultTab*1000.0)/1440.0), FUNCTION_END ) ;
            OutSize += sprintf ( OutBuffer+OutSize, "%c%s%i%c", FUNCTION_START, FunctionNames[FN__LEFTMARGIN], long((dop.xaLeft*1000.0)/1440.0), FUNCTION_END ) ;
            OutSize += sprintf ( OutBuffer+OutSize, "%c%s%i%c", FUNCTION_START, FunctionNames[FN__RIGHTMARGIN], long((dop.xaRight*1000.0)/1440.0), FUNCTION_END ) ;
            OutSize += sprintf ( OutBuffer+OutSize, "%c%s%i%c", FUNCTION_START, FunctionNames[FN__TOPMARGIN], long((dop.yaTop*1000.0)/1440.0), FUNCTION_END ) ;
            OutSize += sprintf ( OutBuffer+OutSize, "%c%s%i%c", FUNCTION_START, FunctionNames[FN__BOTTOMMARGIN], long((dop.yaBottom*1000.0)/1440.0), FUNCTION_END ) ;
            OutSize += sprintf ( OutBuffer+OutSize, "%c%s%i%c", FUNCTION_START, FunctionNames[FN__INDENT], long((pap.xaFirst*1000.0)/1440.0), FUNCTION_END ) ;
            OutSize += sprintf ( OutBuffer+OutSize, "%c%s%i%c", FUNCTION_START, FunctionNames[FN__CODEPAGE], CodePage, FUNCTION_END ) ;
            if ( Title[0] )
               OutSize += sprintf ( OutBuffer+OutSize, "%c%s%s%c", FUNCTION_START, FunctionNames[FN__TITLE], Title, FUNCTION_END ) ;
            if ( Author[0] )
               OutSize += sprintf ( OutBuffer+OutSize, "%c%s%s%c", FUNCTION_START, FunctionNames[FN__AUTHOR], Author, FUNCTION_END ) ;
            if ( CreateTime )
               OutSize += sprintf ( OutBuffer+OutSize, "%c%s%i%c", FUNCTION_START, FunctionNames[FN__CREATEDATE], CreateTime, FUNCTION_END ) ;
            OutSize += sprintf ( OutBuffer+OutSize, "%c", FUNCTION_END ) ;
         } else {
            OutSize += strlen(FunctionNames[FN__PREFACE])+strlen(FunctionNames[FN__CODEPAGE]) + 8 ;
            OutSize += strlen(FunctionNames[FN__TABSPACING]) + 10 ;
            OutSize += strlen(FunctionNames[FN__LEFTMARGIN]) + 10 ;
            OutSize += strlen(FunctionNames[FN__RIGHTMARGIN]) + 10 ;
            OutSize += strlen(FunctionNames[FN__TOPMARGIN]) + 10 ;
            OutSize += strlen(FunctionNames[FN__BOTTOMMARGIN]) + 10 ;
            OutSize += strlen(FunctionNames[FN__INDENT]) + 10 ;
            if ( Title[0] )
               OutSize += strlen(FunctionNames[FN__TITLE]) + strlen(Title) + 2 ;
            if ( Author[0] )
               OutSize += strlen(FunctionNames[FN__AUTHOR]) + strlen(Author) + 2 ;
            if ( CreateTime )
               OutSize += strlen(FunctionNames[FN__CREATEDATE]) + 12 ;
            OutSize ++ ;
         } /* endif */

         // We've now started emitting.
         fEmitting = fTrue ;
      } /* endmethod */

      int EmitChar ( int ch, DestinationState &State ) {
         EmitPreamble ( ) ;
         #ifdef DEBUG
            Log ( "RTFParser::EmitChar() Emitting character %02X '%c'.", ch, isprint(ch)?ch:' ' ) ;
         #endif // DEBUG
         int NewFont ( FALSE ) ;
         if ( pCurrentFont || FindFont ( chp.DefaultFont ) )
            NewFont = strcmp ( pCurrentFont->FontName, State.FontName ) ;
         int NewSize ( chp.Size != State.Size ) ;
         int NewColor ( chp.Color != State.Color ) ;
         int BoldOn ( chp.fBold && !State.Bold ) ;
         int BoldOff ( !chp.fBold && State.Bold ) ;
         int ItalicOn ( chp.fItalic && !State.Italic ) ;
         int ItalicOff ( !chp.fItalic && State.Italic ) ;
         int UnderscoreOn ( chp.fUnderscore && !State.Underscore ) ;
         int UnderscoreOff ( !chp.fUnderscore && State.Underscore ) ;
         int CapsOn ( chp.fCaps && !State.Caps ) ;
         int CapsOff ( !chp.fCaps && State.Caps ) ;
         int StrikeoutOn ( chp.fStrikeout && !State.Strikeout ) ;
         int StrikeoutOff ( !chp.fStrikeout && State.Strikeout ) ;
         int OutlineOn ( chp.fOutline && !State.Outline ) ;
         int OutlineOff ( !chp.fOutline && State.Outline ) ;
         int NewLeftMargin ( dop.xaLeft != State.LeftMargin ) ;
         int NewRightMargin ( dop.xaRight != State.RightMargin ) ;
         int NewTopMargin ( dop.yaTop != State.TopMargin ) ;
         int NewBottomMargin ( dop.yaBottom != State.BottomMargin ) ;
         int NewIndent ( pap.xaFirst != State.Indent ) ;
         int NewJustification ( pap.just != State.Justify ) ;
         int NewLineSpacing ( pap.yaLineSpacing != State.LineSpacing ) ;
         if ( OutBuffer ) {
            if ( NewFont )
               OutSize += sprintf ( OutBuffer+OutSize, "%c%s%s%c", FUNCTION_START, FunctionNames[FN__FONT], pCurrentFont->FontName, FUNCTION_END ) ;
            if ( NewSize )
               OutSize += sprintf ( OutBuffer+OutSize, "%c%s%.1lf%c", FUNCTION_START, FunctionNames[FN__SIZE], double(chp.Size)/10.0, FUNCTION_END ) ;
            if ( NewColor )
               OutSize += sprintf ( OutBuffer+OutSize, "%c%s%06X%c", FUNCTION_START, FunctionNames[FN__LINECOLOR], chp.Color, FUNCTION_END ) ;
            if ( BoldOn )
               OutSize += sprintf ( OutBuffer+OutSize, "%c%s%i%c", FUNCTION_START, FunctionNames[FN__BOLD], 1, FUNCTION_END ) ;
            else if ( BoldOff )
               OutSize += sprintf ( OutBuffer+OutSize, "%c%s%i%c", FUNCTION_START, FunctionNames[FN__BOLD], 0, FUNCTION_END ) ;
            if ( ItalicOn )
               OutSize += sprintf ( OutBuffer+OutSize, "%c%s%i%c", FUNCTION_START, FunctionNames[FN__ITALIC], 1, FUNCTION_END ) ;
            else if ( ItalicOff )
               OutSize += sprintf ( OutBuffer+OutSize, "%c%s%i%c", FUNCTION_START, FunctionNames[FN__ITALIC], 0, FUNCTION_END ) ;
            if ( UnderscoreOn )
               OutSize += sprintf ( OutBuffer+OutSize, "%c%s%i%c", FUNCTION_START, FunctionNames[FN__UNDERSCORE], 1, FUNCTION_END ) ;
            else if ( UnderscoreOff )
               OutSize += sprintf ( OutBuffer+OutSize, "%c%s%i%c", FUNCTION_START, FunctionNames[FN__UNDERSCORE], 0, FUNCTION_END ) ;
            if ( CapsOn )
               OutSize += sprintf ( OutBuffer+OutSize, "%c%s%i%c", FUNCTION_START, FunctionNames[FN__CAPS], 1, FUNCTION_END ) ;
            else if ( CapsOff )
               OutSize += sprintf ( OutBuffer+OutSize, "%c%s%i%c", FUNCTION_START, FunctionNames[FN__CAPS], 0, FUNCTION_END ) ;
            if ( StrikeoutOn )
               OutSize += sprintf ( OutBuffer+OutSize, "%c%s%i%c", FUNCTION_START, FunctionNames[FN__STRIKEOUT], 1, FUNCTION_END ) ;
            else if ( StrikeoutOff )
               OutSize += sprintf ( OutBuffer+OutSize, "%c%s%i%c", FUNCTION_START, FunctionNames[FN__STRIKEOUT], 0, FUNCTION_END ) ;
            if ( OutlineOn )
               OutSize += sprintf ( OutBuffer+OutSize, "%c%s%i%c", FUNCTION_START, FunctionNames[FN__OUTLINE], 1, FUNCTION_END ) ;
            else if ( OutlineOff )
               OutSize += sprintf ( OutBuffer+OutSize, "%c%s%i%c", FUNCTION_START, FunctionNames[FN__OUTLINE], 0, FUNCTION_END ) ;
            if ( NewLeftMargin )
               OutSize += sprintf ( OutBuffer+OutSize, "%c%s%i%c", FUNCTION_START, FunctionNames[FN__LEFTMARGIN], long((dop.xaLeft*1000.0)/1440.0), FUNCTION_END ) ;
            if ( NewRightMargin )
               OutSize += sprintf ( OutBuffer+OutSize, "%c%s%i%c", FUNCTION_START, FunctionNames[FN__RIGHTMARGIN], long((dop.xaRight*1000.0)/1440.0), FUNCTION_END ) ;
            if ( NewTopMargin )
               OutSize += sprintf ( OutBuffer+OutSize, "%c%s%i%c", FUNCTION_START, FunctionNames[FN__TOPMARGIN], long((dop.yaTop*1000.0)/1440.0), FUNCTION_END ) ;
            if ( NewBottomMargin )
               OutSize += sprintf ( OutBuffer+OutSize, "%c%s%i%c", FUNCTION_START, FunctionNames[FN__BOTTOMMARGIN], long((dop.yaBottom*1000.0)/1440.0), FUNCTION_END ) ;
            if ( NewIndent )
               OutSize += sprintf ( OutBuffer+OutSize, "%c%s%i%c", FUNCTION_START, FunctionNames[FN__INDENT], long((pap.xaFirst*1000.0)/1440.0), FUNCTION_END ) ;
            if ( NewJustification ) {
               switch ( pap.just ) {
                  case justL:
                     OutSize += sprintf ( OutBuffer+OutSize, "%c%s%c", FUNCTION_START, FunctionNames[FN__LEFTJUSTIFY], FUNCTION_END ) ;
                     break;
                  case justR:
                     OutSize += sprintf ( OutBuffer+OutSize, "%c%s%c", FUNCTION_START, FunctionNames[FN__RIGHTJUSTIFY], FUNCTION_END ) ;
                     break;
                  case justC:
                     OutSize += sprintf ( OutBuffer+OutSize, "%c%s%c", FUNCTION_START, FunctionNames[FN__CENTERJUSTIFY], FUNCTION_END ) ;
                     break;
                  case justF:
                     OutSize += sprintf ( OutBuffer+OutSize, "%c%s%c", FUNCTION_START, FunctionNames[FN__FULLJUSTIFY], FUNCTION_END ) ;
                     break;
               } /* endswitch */
            } /* endif */
            if ( NewLineSpacing ) {
               if ( pap.yaLineSpacing != 1000 )
                  OutSize += sprintf ( OutBuffer+OutSize, "%c%s%i%c", FUNCTION_START, FunctionNames[FN__LINESPACING], long(100.0*(pap.yaLineSpacing/(chp.Size*2))), FUNCTION_END ) ;
               else
                  OutSize += sprintf ( OutBuffer+OutSize, "%c%s%i%c", FUNCTION_START, FunctionNames[FN__LINESPACING], 100, FUNCTION_END ) ;
            } /* endif */
            OutBuffer[OutSize] = (char) ch ;
            OutSize ++ ;
         } else {
            if ( NewFont )
               OutSize += strlen(FunctionNames[FN__FONT]) + strlen(pCurrentFont->FontName) + 2 ;
            if ( NewSize )
               OutSize += strlen(FunctionNames[FN__SIZE]) + 10 ;
            if ( NewColor )
               OutSize += strlen(FunctionNames[FN__LINECOLOR]) + 8 ;
            if ( BoldOn )
               OutSize += strlen(FunctionNames[FN__BOLD]) + 3 ;
            else if ( BoldOff )
               OutSize += strlen(FunctionNames[FN__BOLD]) + 3 ;
            if ( ItalicOn )
               OutSize += strlen(FunctionNames[FN__ITALIC]) + 3 ;
            else if ( ItalicOff )
               OutSize += strlen(FunctionNames[FN__ITALIC]) + 3 ;
            if ( UnderscoreOn )
               OutSize += strlen(FunctionNames[FN__UNDERSCORE]) + 3 ;
            else if ( UnderscoreOff )
               OutSize += strlen(FunctionNames[FN__UNDERSCORE]) + 3 ;
            if ( CapsOn )
               OutSize += strlen(FunctionNames[FN__CAPS]) + 3 ;
            else if ( CapsOff )
               OutSize += strlen(FunctionNames[FN__CAPS]) + 3 ;
            if ( StrikeoutOn )
               OutSize += strlen(FunctionNames[FN__STRIKEOUT]) + 3 ;
            else if ( StrikeoutOff )
               OutSize += strlen(FunctionNames[FN__STRIKEOUT]) + 3 ;
            if ( OutlineOn )
               OutSize += strlen(FunctionNames[FN__OUTLINE]) + 3 ;
            else if ( OutlineOff )
               OutSize += strlen(FunctionNames[FN__OUTLINE]) + 3 ;
            if ( NewLeftMargin )
               OutSize += strlen(FunctionNames[FN__LEFTMARGIN] ) + 10 ;
            if ( NewRightMargin )
               OutSize += strlen(FunctionNames[FN__RIGHTMARGIN] ) + 10 ;
            if ( NewTopMargin )
               OutSize += strlen(FunctionNames[FN__TOPMARGIN] ) + 10 ;
            if ( NewBottomMargin )
               OutSize += strlen(FunctionNames[FN__BOTTOMMARGIN] ) + 10 ;
            if ( NewIndent )
               OutSize += strlen(FunctionNames[FN__INDENT] ) + 10 ;
            if ( NewJustification ) {
               switch ( pap.just ) {
                  case justL:
                     OutSize += strlen(FunctionNames[FN__LEFTJUSTIFY]) + 2 ;
                     break;
                  case justR:
                     OutSize += strlen(FunctionNames[FN__RIGHTJUSTIFY]) + 2 ;
                     break;
                  case justC:
                     OutSize += strlen(FunctionNames[FN__CENTERJUSTIFY]) + 2 ;
                     break;
                  case justF:
                     OutSize += strlen(FunctionNames[FN__FULLJUSTIFY]) + 2 ;
                     break;
               } /* endswitch */
            } /* endif */
            if ( NewLineSpacing )
               OutSize += strlen(FunctionNames[FN__LINESPACING]) + 10 ;
            OutSize ++ ;
         } /* endif */
         if ( pCurrentFont )
            strcpy ( State.FontName, pCurrentFont->FontName ) ;
         State.Size = chp.Size ;
         State.Color = chp.Color ;
         State.Bold = ( chp.fBold != 0 ) ;
         State.Italic = ( chp.fItalic != 0 ) ;
         State.Underscore = ( chp.fUnderscore != 0 ) ;
         State.Caps = ( chp.fCaps != 0 ) ;
         State.Strikeout = ( chp.fStrikeout != 0 ) ;
         State.Outline = ( chp.fOutline != 0 ) ;
         State.LeftMargin = dop.xaLeft ;
         State.RightMargin = dop.xaRight ;
         State.TopMargin = dop.yaTop ;
         State.BottomMargin = dop.yaBottom ;
         State.Indent = pap.xaFirst ;
         State.Justify = pap.just ;
         State.LineSpacing = pap.yaLineSpacing ;
         return ( ecOK ) ;
      } /* endmethod */

      int ParseChar ( int ch ) {
         #ifdef DEBUG
            Log ( "RTFParser::ParseChar() Parsing character %02X '%c'.", ch, isprint(ch)?ch:' ' ) ;
         #endif // DEBUG
         if ( ( ris == risBin ) && ( --cbBin <= 0 ) ) {
            #ifdef DEBUG
               Log ( "RTFParser::ParseChar() Reverting from binary to normal mode." ) ;
            #endif // DEBUG
            ris = risNorm ;
         } /* endif */
         switch ( rds ) {
            case rdsSkip:
               // Toss this character.
               #ifdef DEBUG
                  Log ( "RTFParser::ParseChar() In skip mode.  Character discarded." ) ;
               #endif // DEBUG
               return ( ecOK ) ;
            case rdsNorm:
               // Output a character. Properties are valid at this point.
               #ifdef DEBUG
                  Log ( "RTFParser::ParseChar() In normal mode.  Emitting character." ) ;
               #endif // DEBUG
               return ( EmitChar(ch,NormalState) ) ;
            case rdsHeader:
               // Add this character to the current header.
               #ifdef DEBUG
                  Log ( "RTFParser::ParseChar() In header mode.  Emitting character." ) ;
               #endif // DEBUG
               return ( EmitChar(ch,HeaderState) ) ;
            case rdsFooter:
               // Add this character to the current footer.
               #ifdef DEBUG
                  Log ( "RTFParser::ParseChar() In footer mode.  Emitting character." ) ;
               #endif // DEBUG
               return ( EmitChar(ch,FooterState) ) ;
            case rdsTitle:
               // Add the character to the title string.
               #ifdef DEBUG
                  Log ( "RTFParser::ParseChar() In title mode.  Adding to title string." ) ;
               #endif // DEBUG
               if ( strlen(Title) < sizeof(Title)-1 )
                  Title[strlen(Title)] = (char) ch ;
               return ( ecOK ) ;
            case rdsAuthor:
               // Add the character to the author string.
               #ifdef DEBUG
                  Log ( "RTFParser::ParseChar() In author mode.  Adding to author string." ) ;
               #endif // DEBUG
               if ( strlen(Author) < sizeof(Author)-1 )
                  Author[strlen(Author)] = (char) ch ;
               return ( ecOK ) ;
            case rdsFontTable:
               #ifdef DEBUG
                  Log ( "RTFParser::ParseChar() In font table mode.  Adding to font name string." ) ;
               #endif // DEBUG
               if ( pCurrentFont && ( ch != ';' ) ) {
                  if ( strlen(pCurrentFont->FontName) < sizeof(pCurrentFont->FontName)-1 )
                     pCurrentFont->FontName[strlen(pCurrentFont->FontName)] = (char) ch ;
               } /* endif */
               return ( ecOK ) ;
            case rdsColorTable:
               if ( ( ch == ';' ) && ( ColorIndex < DIM(Colors) ) )
                  ColorIndex ++ ;
               return ( ecOK ) ;
            case rdsPicture:
               return ( ecOK ) ;
            default:
               // handle other destinations....
               #ifdef DEBUG
                  Log ( "RTFParser::ParseChar() Unknown mode.  Discarding character." ) ;
               #endif // DEBUG
               return ( ecOK ) ;
         } /* endswitch */
      } /* endmethod */

      int FindFont ( int Number ) {
         struct FontTable *pCurrent ( pFontTable ) ;
         while ( pCurrent ) {
            if ( pCurrent->Number == Number )
               break ;
            pCurrent = pCurrent->pNext ;
         } /* endwhile */
         pCurrentFont = pCurrent ;
         return ( pCurrentFont != 0 ) ;
      } /* endmethod */

      int ParseSpecialKeyword ( IPFN ipfn ) {
         #ifdef DEBUG
            Log ( "RTFParser::ParseSpecialKeyword(ipfn=%i)", ipfn ) ;
         #endif // DEBUG
         if ( ( rds == rdsSkip ) && ( ipfn != ipfnBin ) ) {
            #ifdef DEBUG
               Log ( "RTFParser::ParseSpecialKeyword() In skip mode, not binary.  Discarding function request." ) ;
            #endif // DEBUG
            return ( ecOK ) ;
         } /* endif */
         switch ( ipfn ) {
            case ipfnBin:
               #ifdef DEBUG
                  Log ( "RTFParser::ParseSpecialKeyword() Switching to binary input mode for %i bytes.", lParam ) ;
               #endif // DEBUG
               ris = risBin ;
               cbBin = lParam ;
               break;
            case ipfnSkipDest:
               #ifdef DEBUG
                  Log ( "RTFParser::ParseSpecialKeyword() Setting skip destination if unknown flag." ) ;
               #endif // DEBUG
               fSkipDestIfUnk = fTrue ;
               break;
            case ipfnHex:
               #ifdef DEBUG
                  Log ( "RTFParser::ParseSpecialKeyword() Setting input mode to hex." ) ;
               #endif // DEBUG
               ris = risHex ;
               break;
            case ipfnRtf:
               #ifdef DEBUG
                  Log ( "RTFParser::ParseSpecialKeyword() Saw RTF token.  Resetting first token flag." ) ;
               #endif // DEBUG
               fFirstToken = fFalse ;
               break;
            case ipfnAnsi:
               #ifdef DEBUG
                  Log ( "RTFParser::ParseSpecialKeyword() Setting codepage to 1252." ) ;
               #endif // DEBUG
               CodePage = 1252 ;
               break;
            case ipfnMac:
               #ifdef DEBUG
                  Log ( "RTFParser::ParseSpecialKeyword() Setting codepage to 1252." ) ;
               #endif // DEBUG
               CodePage = 1252 ; // ELABORATE: This is -not- the right codepage for Macintosh documents . . .
               break;
            case ipfnPc:
               #ifdef DEBUG
                  Log ( "RTFParser::ParseSpecialKeyword() Setting codepage to 437." ) ;
               #endif // DEBUG
               CodePage = 437 ;
               break;
            case ipfnPca:
               #ifdef DEBUG
                  Log ( "RTFParser::ParseSpecialKeyword() Setting codepage to 850." ) ;
               #endif // DEBUG
               CodePage = 850 ;
               break;
            case ipfnYear:
               #ifdef DEBUG
                  Log ( "RTFParser::ParseSpecialKeyword() Setting year to %i.", lParam ) ;
               #endif // DEBUG
               Year = lParam ;
               break;
            case ipfnMonth:
               #ifdef DEBUG
                  Log ( "RTFParser::ParseSpecialKeyword() Setting month to %i.", lParam ) ;
               #endif // DEBUG
               Month = lParam ;
               break;
            case ipfnDay:
               #ifdef DEBUG
                  Log ( "RTFParser::ParseSpecialKeyword() Setting day to %i.", lParam ) ;
               #endif // DEBUG
               Day = lParam ;
               break;
            case ipfnHour:
               #ifdef DEBUG
                  Log ( "RTFParser::ParseSpecialKeyword() Setting hour to %i.", lParam ) ;
               #endif // DEBUG
               Hour = lParam ;
               break;
            case ipfnMinute:
               #ifdef DEBUG
                  Log ( "RTFParser::ParseSpecialKeyword() Setting minute to %i.", lParam ) ;
               #endif // DEBUG
               Minute = lParam ;
               break;
            case ipfnSecond:
               #ifdef DEBUG
                  Log ( "RTFParser::ParseSpecialKeyword() Setting second to %i.", lParam ) ;
               #endif // DEBUG
               Second = lParam ;
               break;
            case ipfnFontNil:
               #ifdef DEBUG
                  Log ( "RTFParser::ParseSpecialKeyword() Setting font family to nil." ) ;
               #endif // DEBUG
               if ( pCurrentFont )
                  strcpy ( pCurrentFont->Family, "" ) ;
               break;
            case ipfnFontRoman:
               #ifdef DEBUG
                  Log ( "RTFParser::ParseSpecialKeyword() Setting font family to roman." ) ;
               #endif // DEBUG
               if ( pCurrentFont )
                  strcpy ( pCurrentFont->Family, "Roman" ) ;
               break;
            case ipfnFontSwiss:
               #ifdef DEBUG
                  Log ( "RTFParser::ParseSpecialKeyword() Setting font family to swiss." ) ;
               #endif // DEBUG
               if ( pCurrentFont )
                  strcpy ( pCurrentFont->Family, "Swiss" ) ;
               break;
            case ipfnFontModern:
               #ifdef DEBUG
                  Log ( "RTFParser::ParseSpecialKeyword() Setting font family to modern." ) ;
               #endif // DEBUG
               if ( pCurrentFont )
                  strcpy ( pCurrentFont->Family, "Modern" ) ;
               break;
            case ipfnFontScript:
               #ifdef DEBUG
                  Log ( "RTFParser::ParseSpecialKeyword() Setting font family to script." ) ;
               #endif // DEBUG
               if ( pCurrentFont )
                  strcpy ( pCurrentFont->Family, "Script" ) ;
               break;
            case ipfnFontDecor:
               #ifdef DEBUG
                  Log ( "RTFParser::ParseSpecialKeyword() Setting font family to decorative." ) ;
               #endif // DEBUG
               if ( pCurrentFont )
                  strcpy ( pCurrentFont->Family, "Decor" ) ;
               break;
            case ipfnFontTech:
               #ifdef DEBUG
                  Log ( "RTFParser::ParseSpecialKeyword() Setting font family to technical." ) ;
               #endif // DEBUG
               if ( pCurrentFont )
                  strcpy ( pCurrentFont->Family, "Tech" ) ;
               break;
            case ipfnFontBidi:
               #ifdef DEBUG
                  Log ( "RTFParser::ParseSpecialKeyword() Setting font family to BIDI." ) ;
               #endif // DEBUG
               if ( pCurrentFont )
                  strcpy ( pCurrentFont->Family, "Bidi" ) ;
               break;
            case ipfnFontNumber: {
               #ifdef DEBUG
                  Log ( "RTFParser::ParseSpecialKeyword() Setting font number to %i.", lParam ) ;
               #endif // DEBUG
               struct FontTable *pPrev ( 0 ) ;
               struct FontTable *pCurrent ( pFontTable ) ;
               while ( pCurrent ) {
                  struct FontTable *pNext ( pCurrent->pNext ) ;
                  if ( pCurrent->Number == lParam ) {
                     #ifdef DEBUG
                        Log ( "RTFParser::ParseSpecialKeyword() Found font table entry." ) ;
                     #endif // DEBUG
                     if ( rds == rdsFontTable ) {
                        #ifdef DEBUG
                           Log ( "RTFParser::ParseSpecialKeyword() Replacing font table entry." ) ;
                        #endif // DEBUG
                        delete pCurrent ;
                        pCurrent = new FontTable ( lParam ) ;
                        if ( pPrev )
                           pPrev->pNext = pCurrent ;
                        else
                           pFontTable = pCurrent ;
                        pCurrent->pNext = pNext ;
                     } /* endif */
                     break ;
                  } /* endif */
                  pPrev = pCurrent ;
                  pCurrent = pCurrent->pNext ;
               } /* endwhile */
               if ( pCurrent == 0 ) {
                  #ifdef DEBUG
                     Log ( "RTFParser::ParseSpecialKeyword() Creating font table entry." ) ;
                  #endif // DEBUG
                  if ( pPrev ) {
                     pCurrent = new FontTable ( lParam ) ;
                     pPrev->pNext = pCurrent ;
                  } else {
                     pCurrent = new FontTable ( lParam ) ;
                     pFontTable = pCurrent ;
                  } /* endif */
               } /* endif */
               pCurrentFont = pCurrent ;
               break; }
            case ipfnFontSize:
               #ifdef DEBUG
                  Log ( "RTFParser::ParseSpecialKeyword() Setting font size to %.1lf points.", double(lParam*5)/10.0 ) ;
               #endif // DEBUG
               chp.Size = lParam * 5 ;
               break;
            case ipfnFTNil:
               #ifdef DEBUG
                  Log ( "RTFParser::ParseSpecialKeyword() Setting font type to nil." ) ;
               #endif // DEBUG
               if ( pCurrentFont )
                  pCurrentFont->Fnt.Type = ftNil ;
               break;
            case ipfnFTTrueType:
               #ifdef DEBUG
                  Log ( "RTFParser::ParseSpecialKeyword() Setting font type to TrueType." ) ;
               #endif // DEBUG
               if ( pCurrentFont )
                  pCurrentFont->Fnt.Type = ftTrueType ;
               break;
            case ipfnBullet:
               #ifdef DEBUG
                  Log ( "RTFParser::ParseSpecialKeyword() Emitting bullet." ) ;
               #endif // DEBUG
               switch ( CodePage ) {
                  case 1252:
                     ParseChar ( 149 ) ;
                     break;
                  case 437:
                     ParseChar ( 249 ) ;
                     break;
                  case 850:
                     ParseChar ( 250 ) ;
                     break;
               } /* endswitch */
               break;
            case ipfnEnDash:
               #ifdef DEBUG
                  Log ( "RTFParser::ParseSpecialKeyword() Emitting En dash." ) ;
               #endif // DEBUG
               switch ( CodePage ) {
                  case 1252:
                     ParseChar ( 150 ) ;
                     break;
                  case 437:
                  case 850:
                     ParseChar ( '-' ) ;
                     break;
               } /* endswitch */
               break;
            case ipfnEmDash:
               #ifdef DEBUG
                  Log ( "RTFParser::ParseSpecialKeyword() Emitting Em dash." ) ;
               #endif // DEBUG
               switch ( CodePage ) {
                  case 1252:
                     ParseChar ( 151 ) ;
                     break;
                  case 437:
                  case 850:
                     ParseChar ( '-' ) ;
                     break;
               } /* endswitch */
               break;
            case ipfnLQuote:
               #ifdef DEBUG
                  Log ( "RTFParser::ParseSpecialKeyword() Emitting left single quote." ) ;
               #endif // DEBUG
               switch ( CodePage ) {
                  case 1252:
                     ParseChar ( 145 ) ;
                     break;
                  case 437:
                  case 850:
                     ParseChar ( '\'' ) ;
                     break;
               } /* endswitch */
               break;
            case ipfnRQuote:
               #ifdef DEBUG
                  Log ( "RTFParser::ParseSpecialKeyword() Emitting right single quote." ) ;
               #endif // DEBUG
               switch ( CodePage ) {
                  case 1252:
                     ParseChar ( 146 ) ;
                     break;
                  case 437:
                  case 850:
                     ParseChar ( '\'' ) ;
                     break;
               } /* endswitch */
               break;
            case ipfnDLQuote:
               #ifdef DEBUG
                  Log ( "RTFParser::ParseSpecialKeyword() Emitting double left single quote." ) ;
               #endif // DEBUG
               switch ( CodePage ) {
                  case 1252:
                     ParseChar ( 147 ) ;
                     break;
                  case 437:
                  case 850:
                     ParseChar ( '\"' ) ;
                     break;
               } /* endswitch */
               break;
            case ipfnDRQuote:
               #ifdef DEBUG
                  Log ( "RTFParser::ParseSpecialKeyword() Emitting double right single quote." ) ;
               #endif // DEBUG
               switch ( CodePage ) {
                  case 1252:
                     ParseChar ( 148 ) ;
                     break;
                  case 437:
                  case 850:
                     ParseChar ( '\"' ) ;
                     break;
               } /* endswitch */
               break;
            case ipfnOptHyphen:
               #ifdef DEBUG
                  Log ( "RTFParser::ParseSpecialKeyword() Swallowing optional hyphen." ) ;
               #endif // DEBUG
               break;
            case ipfnNBKHyphen:
               #ifdef DEBUG
                  Log ( "RTFParser::ParseSpecialKeyword() Emitting non-breaking hyphen." ) ;
               #endif // DEBUG
               ParseChar ( '-' ) ;
               break;
            case ipfnNBKSpace:
               #ifdef DEBUG
                  Log ( "RTFParser::ParseSpecialKeyword() Emitting non-breaking space." ) ;
               #endif // DEBUG
               ParseChar ( ' ' ) ;
               break;
            case ipfnFgColor:
               chp.Color = ( Colors[lParam].Red << 16 ) & 0xFF0000 ;
               chp.Color |= ( Colors[lParam].Green << 8 ) & 0xFF00 ;
               chp.Color |= Colors[lParam].Blue & 0xFF ;
               break;
            case ipfnBox:
               if ( rds == rdsPicture )
                  pict.Box = TRUE ;
               break;
            case ipfnBrdrs:
               if ( rds == rdsPicture )
                  pict.BorderDouble = FALSE ;
               break;
            case ipfnBrdrth:
               if ( rds == rdsPicture )
                  pict.BorderDouble = TRUE ;
               break;
            case ipfnBrdrw:
               if ( rds == rdsPicture )
                  pict.BorderWidth = lParam ;
               break;
            case ipfnBrdrcf:
               if ( rds == rdsPicture )
                  pict.BorderColor = lParam ;
               break;
            default:
               #ifdef DEBUG
                  Log ( "RTFParser::ParseSpecialKeyword() ERROR: Unknown special function." ) ;
               #endif // DEBUG
               return ( ecBadTable ) ;
         } /* endswitch */
         #ifdef DEBUG
            Log ( "RTFParser::ParseSpecialKeyword() Done OK." ) ;
         #endif // DEBUG
         return ( ecOK ) ;
      } /* endmethod */

      int ChangeDest ( IDEST idest ) {

         #ifdef DEBUG
            Log ( "RTFParser::ChangeDest(idest=%i)", idest ) ;
         #endif // DEBUG

         switch ( idest ) {
            case idestCreateTime:
               #ifdef DEBUG
                  Log ( "RTFParser::ChangeDest() Setting destination to CreateTime." ) ;
               #endif // DEBUG
               rds = rdsCreateTime ;
               Year = Month = Day = Hour = Minute = Second = 0 ;
               break;
            case idestHeader:
               #ifdef DEBUG
                  Log ( "RTFParser::ChangeDest() Setting destination to Header." ) ;
               #endif // DEBUG
               EmitPreamble ( ) ;
               rds = rdsHeader ;
               if ( OutBuffer ) {
                  OutSize += sprintf ( OutBuffer+OutSize, "%c%s", FUNCTION_START, FunctionNames[FN__HEADER] ) ;
               } else {
                  OutSize += strlen(FunctionNames[FN__HEADER]) + 1  ;
               } /* endif */
               HeaderState.Reset ( ) ;
               break;
            case idestFooter:
               #ifdef DEBUG
                  Log ( "RTFParser::ChangeDest() Setting destination to Footer." ) ;
               #endif // DEBUG
               EmitPreamble ( ) ;
               rds = rdsFooter ;
               if ( OutBuffer ) {
                  OutSize += sprintf ( OutBuffer+OutSize, "%c%s", FUNCTION_START, FunctionNames[FN__FOOTER] ) ;
               } else {
                  OutSize += strlen(FunctionNames[FN__FOOTER]) + 1  ;
               } /* endif */
               FooterState.Reset ( ) ;
               break;
            case idestTitle:
               #ifdef DEBUG
                  Log ( "RTFParser::ChangeDest() Setting destination to Title." ) ;
               #endif // DEBUG
               rds = rdsTitle ;
               memset ( Title, 0, sizeof(Title) ) ;
               break;
            case idestAuthor:
               #ifdef DEBUG
                  Log ( "RTFParser::ChangeDest() Setting destination to Author." ) ;
               #endif // DEBUG
               rds = rdsAuthor ;
               memset ( Author, 0, sizeof(Author) ) ;
               break;
            case idestFontTable:
               #ifdef DEBUG
                  Log ( "RTFParser::ChangeDest() Setting destination to FontTable." ) ;
               #endif // DEBUG
               rds = rdsFontTable ;
               break;
            case idestColorTable: {
               #ifdef DEBUG
                  Log ( "RTFParser::ChangeDest() Setting destination to ColorTable." ) ;
               #endif // DEBUG
               rds = rdsColorTable ;
               ColorIndex = 0 ;
               for ( int i=0; i<DIM(Colors); i++ )
                  Colors[i].Reset ( ) ;
               break; }
            case idestPict: {
               #ifdef DEBUG
                  Log ( "RTFParser::ChangeDest() Setting destination to Picture." ) ;
               #endif // DEBUG
               rds = rdsPicture ;
               pict.Reset ( ) ;
               break; }
            default:
               #ifdef DEBUG
                  Log ( "RTFParser::ChangeDest() Destination not recognized.  Setting to Skip." ) ;
               #endif // DEBUG
               rds = rdsSkip ;
               break;
         } /* endswitch */

         #ifdef DEBUG
            Log ( "RTFParser::ChangeDest() Done OK." ) ;
         #endif // DEBUG
         return ( ecOK ) ;
      } /* endmethod */

      int TranslateKeyword ( char *szKeyword, int param, bool fParam ) {

         #ifdef DEBUG
            Log ( "RTFParser::TranslateKeyword(szKeyword='%s',param=%i,fParam=%i)", szKeyword, param, fParam ) ;
         #endif // DEBUG

         // search for szKeyword in rgsymRtf

         for ( int isym=0; isym<DIM(rgsymRtf); isym++ ) {
            if ( strcmp(szKeyword, rgsymRtf[isym].szKeyword) == 0 ) {
               #ifdef DEBUG
                  Log ( "RTFParser::TranslateKeyword() Keyword found in table." ) ;
               #endif // DEBUG
               break;
            } /* endif */
         } /* endfor */
         if ( isym == DIM(rgsymRtf) ) {  // control word not found
            #ifdef DEBUG
               Log ( "RTFParser::TranslateKeyword() Keyword NOT found in table." ) ;
            #endif // DEBUG
            if ( fSkipDestIfUnk ) {      // if this is a new destination
               #ifdef DEBUG
                  Log ( "RTFParser::TranslateKeyword() Setting destination to Skip because flag for skipping unknown destinations was set." ) ;
               #endif // DEBUG
               rds = rdsSkip;            // skip the destination
            }                            // else just discard it
            #ifdef DEBUG
               Log ( "RTFParser::TranslateKeyword() Resetting skip destination is unknown flag." ) ;
            #endif // DEBUG
            fSkipDestIfUnk = fFalse;
            #ifdef DEBUG
               Log ( "RTFParser::TranslateKeyword() Done OK." ) ;
            #endif // DEBUG
            return ( ecOK ) ;
         } /* endif */

         // found it!  use kwd and idx to determine what to do with it.

         #ifdef DEBUG
            Log ( "RTFParser::TranslateKeyword() Resetting skip destination is unknown flag." ) ;
         #endif // DEBUG
         fSkipDestIfUnk = fFalse;
         switch ( rgsymRtf[isym].kwd ) {
            case kwdProp:
               if (rgsymRtf[isym].fPassDflt || !fParam)
                  param = rgsymRtf[isym].dflt;
               return ( ApplyPropChange(IPROP(rgsymRtf[isym].idx), param) ) ;
            case kwdChar:
               return ( ParseChar(rgsymRtf[isym].idx) ) ;
            case kwdDest:
               return ( ChangeDest ( IDEST(rgsymRtf[isym].idx) ) ) ;
            case kwdSpec:
               return ( ParseSpecialKeyword ( IPFN(rgsymRtf[isym].idx) ) ) ;
            default:
               #ifdef DEBUG
                  Log ( "RTFParser::TranslateKeyword() ERROR: Keyword type unknown." ) ;
               #endif // DEBUG
               return ( ecBadTable ) ;
         } /* endswitch */
      } /* endmethod */

      int EndGroupAction ( RDS rds ) {
         #ifdef DEBUG
            Log ( "RTFParser::EndGroupAction(rds=%i)", rds ) ;
         #endif // DEBUG
         switch ( rds ) {
            case rdsHeader:
            case rdsFooter:
               #ifdef DEBUG
                  Log ( "RTFParser::EndGroupAction() Adding terminator to header/footer." ) ;
               #endif // DEBUG
               if ( OutBuffer ) {
                  OutSize += sprintf ( OutBuffer+OutSize, "%c", FUNCTION_END ) ;
               } else {
                  OutSize ++ ;
               } /* endif */
               break;
            case rdsCreateTime: {
               #ifdef DEBUG
                  Log ( "RTFParser::EndGroupAction() Computing create time." ) ;
               #endif // DEBUG
               struct tm Tm = { 0 } ;
               Tm.tm_year = Year - 1900 ;
               Tm.tm_mon = Month - 1 ;
               Tm.tm_mday = Day ;
               Tm.tm_hour = Hour ;
               Tm.tm_min = Minute ;
               Tm.tm_sec = Second ;
               CreateTime = mktime ( &Tm ) ;
               break; }
            case rdsPicture:
               #ifdef DEBUG
                  Log ( "RTFParser::EndGroupAction() Creating graphic object." ) ;
               #endif // DEBUG
               if ( OutBuffer ) {
                  char Options [300] = { 0 } ;
                  if ( pict.Box )
                     sprintf ( Options, "BorderColor=%02X%02X%02X,BorderWidth=%i",
                        Colors[pict.BorderColor].Red, Colors[pict.BorderColor].Green, Colors[pict.BorderColor].Blue,
                        int(double(pict.BorderWidth*(pict.BorderDouble?2:1))/1.44) ) ;
                  OutSize += sprintf ( OutBuffer+OutSize, "%c%s", FUNCTION_START, FunctionNames[FN__GRAPHIC] ) ;
                  OutSize += sprintf ( OutBuffer+OutSize, "INLINE,0,%i,%i%s%s:BM\0" "0,0", int(double(pict.picwgoal)/1.44), int(double(pict.pichgoal)/1.44), Options[0]?",":"", Options ) ;
                  OutSize += sprintf ( OutBuffer+OutSize, "%c", FUNCTION_END ) ;
               } else {
                  OutSize += strlen(FunctionNames[FN__GRAPHIC]) + 100 ;
               } /* endif */
               break;
         } /* endswitch */
         #ifdef DEBUG
            Log ( "RTFParser::EndGroupAction() Done OK." ) ;
         #endif // DEBUG
         return ( ecOK ) ;
      } /* endmethod */

      int PushState ( ) {

         #ifdef DEBUG
            Log ( "RTFParser::PushState()" ) ;
         #endif // DEBUG

         SAVE *psaveNew ( (SAVE*)malloc(sizeof(SAVE)) ) ;
         if ( !psaveNew )
            return ( ecStackOverflow ) ;

         psaveNew -> pNext = psave ;
         psaveNew -> chp = chp ;
         psaveNew -> pap = pap ;
         psaveNew -> sep = sep ;
         psaveNew -> dop = dop ;
         psaveNew -> rds = rds ;
         psaveNew -> ris = ris ;

         psaveNew -> pCurrentFont = pCurrentFont ;

         ris = risNorm ;
         psave = psaveNew ;
         cGroup ++ ;
         return ( ecOK ) ;
      } /* endmethod */

      int PopState ( ) {

         #ifdef DEBUG
            Log ( "RTFParser::PopState()" ) ;
         #endif // DEBUG

         if ( !psave ) {
            #ifdef DEBUG
               Log ( "RTFParser::PopState() ERROR: Stack underflow." ) ;
            #endif // DEBUG
            return ( ecStackUnderflow ) ;
         } /* endif */

         if ( rds != psave->rds ) {
            int ec ( EndGroupAction ( rds ) ) ;
            if ( ec != ecOK ) {
               #ifdef DEBUG
                  Log ( "RTFParser::PopState() ERROR: EndGroupAction error." ) ;
               #endif // DEBUG
               return ( ec ) ;
            } /* endif */
         } /* endif */

         chp = psave->chp ;
         pap = psave->pap ;
         sep = psave->sep ;
         dop = psave->dop ;
         rds = psave->rds ;
         ris = psave->ris ;

         pCurrentFont = psave->pCurrentFont ;

         SAVE *psaveOld ( psave ) ;
         psave = psave->pNext ;
         cGroup-- ;
         free ( psaveOld ) ;
         #ifdef DEBUG
            Log ( "RTFParser::PopState() Done OK.  Destination now %i.", rds ) ;
         #endif // DEBUG
         return ( ecOK ) ;
      } /* endmethod */

      int ParseKeyword ( ) {

         char fParam ( fFalse ) ;
         char fNeg ( fFalse ) ;
         int param ( 0 ) ;
         char szKeyword [30] = { 0 } ;
         char szParameter [20] = { 0 } ;

         int ch ( getc(File) ) ;
         if ( ch == EOF )
            return ( ecEndOfFile ) ;

         if ( !isalpha(ch) ) { // a control symbol; no delimiter.
            szKeyword[0] = (char) ch ;
            szKeyword[1] = 0 ;
            return ( TranslateKeyword ( szKeyword, 0, fParam ) ) ;
         } /* endif */

         for ( char* pch=szKeyword; isalpha(ch); ch=getc(File) )
            *pch++ = (char) ch;
         *pch = 0 ;

         if ( ch == '-' ) {
            fNeg  = fTrue;
            ch = getc ( File ) ;
            if ( ch == EOF )
               return ( ecEndOfFile ) ;
         } /* endif */

         if ( isdigit ( ch ) ) {
            fParam = fTrue ; // a digit after the control means we have a parameter
            for ( pch=szParameter; isdigit(ch); ch=getc(File) )
               *pch++ = (char) ch;
            *pch = 0;
            param = atoi(szParameter);
            if ( fNeg )
               param = -param;
            lParam = atol(szParameter);
            if ( fNeg )
               lParam = -lParam;
         }

         if ( ch != ' ' )
            ungetc ( ch, File ) ;

         return ( TranslateKeyword ( szKeyword, param, fParam ) ) ;
      } /* endmethod */

   public:
      RTFParser ( char *name ) throw(RTFParser_OpenError) : Name(0), File(0), CodePage(1004), pFontTable(0) {
         Name = new char [ strlen(name) + 1 ] ;
         strcpy ( Name, name ) ;
         File = fopen ( Name, "r" ) ;
         if ( !File )
            throw ( RTFParser_OpenError() ) ;
         Reset ( ) ;
      } /* endmethod */

      ~RTFParser ( ) {
         if ( File )
            fclose ( File ), File = 0 ;
         if ( Name )
            delete [] Name, Name = 0 ;
      } /* endmethod */

      int QuerySize ( ) { return ( OutSize ) ; }

      int Parse ( char *outbuffer = 0 ) {
         Reset ( outbuffer ) ;
         int cNibble ( 2 ) ;
         int b ( 0 ) ;
         int ch ( getc ( File ) ) ;
         while ( ch != EOF ) {
            if ( cGroup < 0 )
               return ( ecStackUnderflow ) ;
            if ( ris == risBin ) {
               int ec ( ParseChar ( ch ) ) ;
               if ( ec != ecOK )
                  return ( ec ) ;
            } else {
               switch ( ch ) {
                  case '{': {
                     int ec ( PushState() ) ;
                     if ( ec != ecOK )
                        return ( ec ) ;
                     break; } /* endcase */
                  case '}': {
                     int ec ( PopState() ) ;
                     if ( ec != ecOK )
                        return ( ec ) ;
                     break; } /* endcase */
                  case '\\': {
                     int ec ( ParseKeyword ( ) ) ;
                     if ( ec != ecOK )
                        return ( ec ) ;
                     if ( fFirstToken )
                        return ( ecInvalidFormat ) ;
                     break; } /* endcase */
                  case 0x0d:
                  case 0x0a:
                     break;
                  default: {
                     if ( fFirstToken )
                        return ( ecInvalidFormat ) ;
                     if ( ris == risNorm ) {
                        int ec ( ParseChar ( ch ) ) ;
                        if ( ec != ecOK )
                           return ( ec ) ;
                     } else {
                        if ( ris != risHex )
                           return ( ecAssertion ) ;
                        b <<= 4 ;
                        if ( isdigit ( ch ) )
                           b += (char) ch - '0';
                        else {
                           if ( islower ( ch ) ) {
                              if ( ch < 'a' || ch > 'f' )
                                 return ( ecInvalidHex ) ;
                              b += (char) ch - 'a' ;
                           } else {
                              if ( ch < 'A' || ch > 'F' )
                                 return ( ecInvalidHex ) ;
                              b += (char) ch - 'A';
                           } /* endif */
                        } /* endif */
                        cNibble--;
                        if ( !cNibble ) {
                           int ec ( ParseChar ( b ) ) ;
                           if ( ec != ecOK )
                              return ( ec ) ;
                           cNibble = 2 ;
                           b = 0 ;
                           ris = risNorm ;
                        } /* endif */
                     } /* endif */
                  } /* endcase */
               } /* endswitch */
            } /* endif */
            ch = getc ( File ) ;
         } /* endwhile */
         if ( cGroup < 0 )
            return ( ecStackUnderflow ) ;
         if ( cGroup > 0 )
            return ( ecUnmatchedBrace ) ;
         return ( ecOK ) ;
      } /* endmethod */
} ;

/****************************************************************************
 *                                                                          *
 *  Verify File by Inspection                                               *
 *                                                                          *
 ****************************************************************************/

extern BOOL APIENTRY Verify ( HWND Owner, char *Name ) {

   // Try to parse the file.
   try {
      RTFParser Parser ( Name ) ;
      if ( Parser.Parse ( ) ) {
         if ( Owner ) {
            ResourceString Message ( Library->QueryHandle(), IDS_ERROR_INVALID ) ;
            Sys_MessageBox ( HWND_DESKTOP, Owner, PSZ(Message), ProductName(), 0, MB_ENTER | MB_ICONEXCLAMATION ) ;
         } /* endif */
         return ( FALSE ) ;
      } /* endif */
   } /* endtry */
   catch ( RTFParser_OpenError ) {
      if ( Owner ) {
         ResourceString Message ( Library->QueryHandle(), IDS_ERROR_OPEN_VERIFY ) ;
         Sys_MessageBox ( HWND_DESKTOP, Owner, PCHAR(Message), ProductName(), 0, MB_ENTER | MB_ICONEXCLAMATION ) ;
      } /* endif */
      return ( FALSE ) ;
   } /* endcatch */

   // Verified OK.
   return ( TRUE ) ;
}


/****************************************************************************
 *                                                                          *
 *  Import File                                                             *
 *                                                                          *
 ****************************************************************************/

extern void* APIENTRY Import ( HWND Owner, char* Name, long* Size ) {

   // Initialize the result.
   char *OutBuffer ( 0 ) ;

   // Try to parse the file.
   try {
      RTFParser Parser ( Name ) ;
      if ( Parser.Parse ( ) ) {
         if ( Owner ) {
            ResourceString Message ( Library->QueryHandle(), IDS_ERROR_INVALID ) ;
            Sys_MessageBox ( HWND_DESKTOP, Owner, PSZ(Message), ProductName(), 0, MB_ENTER | MB_ICONEXCLAMATION ) ;
         } /* endif */
         return ( 0 ) ;
      } /* endif */
      *Size = Parser.QuerySize ( ) ;
      if ( *Size ) {
         OutBuffer = new char [ *Size ] ;
         Parser.Parse ( OutBuffer ) ;
         *Size = Parser.QuerySize ( ) ;
      } /* endif */
   } /* endtry */
   catch ( RTFParser_OpenError ) {
      if ( Owner ) {
         ResourceString Message ( Library->QueryHandle(), IDS_ERROR_OPEN_VERIFY ) ;
         Sys_MessageBox ( HWND_DESKTOP, Owner, PCHAR(Message), ProductName(), 0, MB_ENTER | MB_ICONEXCLAMATION ) ;
         return ( 0 ) ;
      } /* endif */
   } /* endcatch */

   // Return the address of the memory allocated.
   return ( OutBuffer ) ;
}


/****************************************************************************
 *                                                                          *
 *  Export File                                                             *
 *                                                                          *
 ****************************************************************************/

#define MAX_FONTS (0x200)
#define MAX_COLORS (0x2000)

class ExportFile {

   private:
      char *Name ;
      FILE *File ;
      int NumberOfFonts ;
      char **Fonts ;
      int NumberOfColors ;
      long *Colors ;
      int CodePage ;
      double FontSize ;
      int ForegroundColorIndex ;
      int CurrentLineWidth ;

      int FontIndex ( unsigned char *p ) {
         char FontName [100] ;
         strncpy ( FontName, (char*)p, sizeof(FontName) ) ;
         char *p1 ( strchr ( FontName, FUNCTION_END ) ) ;
         if ( p1 )
            *p1 = 0 ;
         for ( int i=0; i<NumberOfFonts; i++ )
            if ( !strcmpi ( Fonts[i], FontName ) )
               return ( i ) ;
         if ( i >= MAX_FONTS )
            return ( 0 ) ;
         Fonts[i] = new char [strlen(FontName)+1] ;
         strcpy ( Fonts[i], FontName ) ;
         NumberOfFonts ++ ;
         return ( i ) ;
      } /* endmethod */

      int ColorIndex ( unsigned char *p ) {
         COLOR Color ( 0 ) ;
         for ( int i=0; i<6; i++, p++ ) {
            Color <<= 4 ;
            Color += ( (*p>'9') ? (*p-'A'+10) : (*p-'0') ) ;
         } /* endfor */
         for ( i=0; i<NumberOfColors; i++ )
            if ( Colors[i] == Color )
               return ( i ) ;
         if ( i >= MAX_COLORS )
            return ( 0 ) ;
         Colors[i] = Color ;
         NumberOfColors ++ ;
         return ( i ) ;
      } /* endmethod */

      void FileWrite ( char *Text, int Length ) throw(char*) {
         int Written ( fwrite ( Text, 1, Length, File ) ) ;
         if ( Written != Length ) {
            Log ( "ERROR: Attempt to write %i bytes resulted in %i bytes written.", Length, Written ) ;
            ResourceString Message ( Library->QueryHandle(), IDS_ERROR_WRITE ) ;
            throw ( (char*)Message ) ;
         } /* endif */
      } /* endmethod */

      void FileWrite ( char *Text ) throw(char*) {
         FileWrite ( Text, strlen(Text) ) ;
      } /* endmethod */

   public:

      ExportFile ( char *name ) throw(char*) : Name(0), File(0), Fonts(0), Colors(0), FontSize(12.0), ForegroundColorIndex(0), CurrentLineWidth(10) {

         // Try to open the file for writing.  Throw an error if not successful.
         File = fopen ( name, "w" ) ;
         if ( File == 0 )
            throw ( "ERROR: Unable to create or open file for saving." ) ;              // TRANSLATE!

         // Save the file name.
         Name = new char [strlen(name)+1] ;
         strcpy ( Name, name ) ;

      } /* endmethod */

      ~ExportFile ( ) {
         if ( Name ) {
            delete [] Name ;
            Name = 0 ;
         } /* endif */
         if ( File ) {
            fclose ( File ) ;
            File = 0 ;
         } /* endif */
         if ( Fonts ) {
            for ( int i=0; i<NumberOfFonts; i++ )
               delete [] Fonts[i] ;
            delete [] Fonts ;
            Fonts = 0 ;
         } /* endif */
         if ( Colors ) {
            delete [] Colors ;
            Colors = 0 ;
         } /* endif */
      } /* endmethod */

      int ProcessBlock ( void *Text, long Size ) {
         int BytesProcessed ( 0 ) ;
         int Nesting ( 0 ) ;
         int Index ( 0 ) ;
         unsigned char *pText ( (unsigned char *) Text ) ;
         while ( Index < Size ) {
            if ( ( pText[0] == FUNCTION_ESCAPE ) && ( Index < Size-2 ) ) {
               if ( ( pText[1] == FUNCTION_ESCAPE ) || ( pText[1] == FUNCTION_START ) || ( pText[1] == FUNCTION_END ) )
                  pText ++ ; Index ++ ; BytesProcessed ++ ;
            } else if ( pText[0] == FUNCTION_START ) {
               Nesting ++ ;
               for ( int Type=0; Type<MAX_FUNCTIONS; Type++ )
                  if ( !strnicmp ( (char*)pText+1, FunctionNames[Type], strlen(FunctionNames[Type]) ) )
                     break ;
               if ( Type == FN__CAPS ) {
                  FileWrite ( atoi ( (char*)pText+strlen(FunctionNames[Type])+1 ) ? "\\caps " : "\\caps0 " ) ;
               } else if ( Type == FN__BOLD ) {
                  FileWrite ( atoi ( (char*)pText+strlen(FunctionNames[Type])+1 ) ? "\\b " : "\\b0 " ) ;
               } else if ( Type == FN__ITALIC ) {
                  FileWrite ( atoi ( (char*)pText+strlen(FunctionNames[Type])+1 ) ? "\\i " : "\\i0 " ) ;
               } else if ( Type == FN__UNDERSCORE ) {
                  FileWrite ( atoi ( (char*)pText+strlen(FunctionNames[Type])+1 ) ? "\\ul " : "\\ul0 " ) ;
               } else if ( Type == FN__STRIKEOUT ) {
                  FileWrite ( atoi ( (char*)pText+strlen(FunctionNames[Type])+1 ) ? "\\strike " : "\\strike0 " ) ;
               } else if ( Type == FN__OUTLINE ) {
                  FileWrite ( atoi ( (char*)pText+strlen(FunctionNames[Type])+1 ) ? "\\outl " : "\\outl0 " ) ;
               } else if ( Type == FN__FONT ) {
                  int Font ( FontIndex ( pText+strlen(FunctionNames[FN__FONT])+1 ) ) ;
                  char Text [20] ; sprintf ( Text, "\\f%i ", Font+1 ) ;
                  FileWrite ( Text ) ;
               } else if ( Type == FN__SIZE ) {
                  FontSize = atof((char*)pText+strlen(FunctionNames[Type])+1) ;
                  char Text [20] ; sprintf ( Text, "\\fs%i ", int ( FontSize * 2.0 + 0.49 ) ) ;
                  FileWrite ( Text ) ;
               } else if ( Type == FN__INDENT ) {
                  int Indent ( int ( ( atoi((char*)pText+strlen(FunctionNames[Type])+1) * 1440.0 ) / 1000.0 ) ) ;
                  char Text [20] ;
                  if ( Indent < 0 ) {
                     sprintf ( Text, "\\fi%i\\li%i ", Indent, -Indent ) ;
                  } else {
                     sprintf ( Text, "\\fi%i\\li0 ", Indent ) ;
                  } /* endif */
                  FileWrite ( Text ) ;
               } else if ( Type == FN__LINECOLOR ) {
                  ForegroundColorIndex = ColorIndex ( pText+strlen(FunctionNames[FN__LINECOLOR])+1 ) ;
                  char Text [20] ; sprintf ( Text, "\\cf%i ", ForegroundColorIndex ) ;
                  FileWrite ( Text ) ;
               } else if ( Type == FN__LEFTJUSTIFY ) {
                  FileWrite ( "\\ql " ) ;
               } else if ( Type == FN__CENTERJUSTIFY ) {
                  FileWrite ( "\\qc " ) ;
               } else if ( Type == FN__RIGHTJUSTIFY ) {
                  FileWrite ( "\\qr " ) ;
               } else if ( Type == FN__FULLJUSTIFY ) {
                  FileWrite ( "\\qj " ) ;
               } else if ( Type == FN__LINESPACING ) {
                  int Factor ( atoi ( (char*)pText+strlen(FunctionNames[FN__LINESPACING])+1 ) ) ;
                  char Text [20] ;
                  if ( Factor == 100 ) {
                     sprintf ( Text, "\\sl%i\\smult0 ", int(FontSize*2.0+0.49) ) ;
                  } else {
                     int Spacing = int ( ( Factor * FontSize ) / 5.0 ) ;
                     sprintf ( Text, "\\sl%i\\smult1 ", Spacing ) ;
                  } /* endif */
                  FileWrite ( Text ) ;
               } else if ( Type == FN__HEADER ) {
                  FileWrite ( "{\\header " ) ;
                  unsigned char *p ( pText + strlen(FunctionNames[Type]) + 1 ) ;
                  int n ( Size - ( BytesProcessed + strlen(FunctionNames[Type]) + 1 ) ) ;
                  int Processed ( ProcessBlock ( p, n ) ) ;
                  pText += Processed + strlen(FunctionNames[Type]) ;
                  Index += Processed + strlen(FunctionNames[Type]) ;
                  BytesProcessed += Processed + strlen(FunctionNames[Type]) ;
                  FileWrite ( "}\n" ) ;
               } else if ( Type == FN__FOOTER ) {
                  FileWrite ( "{\\footer " ) ;
                  unsigned char *p ( pText + strlen(FunctionNames[Type]) + 1 ) ;
                  int n ( Size - ( BytesProcessed + strlen(FunctionNames[Type]) + 1 ) ) ;
                  int Processed ( ProcessBlock ( p, n ) ) ;
                  pText += Processed + strlen(FunctionNames[Type]) ;
                  Index += Processed + strlen(FunctionNames[Type]) ;
                  BytesProcessed += Processed + strlen(FunctionNames[Type]) ;
                  FileWrite ( "}\n" ) ;
               } else if ( Type == FN__GLYPH ) {
                  char Text [2] ;
                  sprintf ( Text, "\\\'%02X ", atoi ( (char*)pText+strlen(FunctionNames[Type])+1 ) ) ;
                  FileWrite ( Text ) ;
               } else if ( Type == FN__LINEWIDTH ) {
                  CurrentLineWidth = atoi ( (char*)pText+strlen(FunctionNames[Type])+1 ) ;
               } else if ( Type == FN__SEPARATOR ) {
                  char Text [80] ;
                  sprintf ( Text, "\\brdrb\\brdrs\\brdrw%i\\brsp%i\\brdrcf%i\\par\\pard\\par ", int((CurrentLineWidth*100.0+0.49)/144.0), int((FontSize*20.0+0.49)/2.0), ForegroundColorIndex ) ;
                  FileWrite ( Text ) ;
               } else if ( Type == FN__PAGENUMBER ) {
                  // ELABORATE
               } else if ( Type == FN__PAGECOUNT ) {
                  // ELABORATE
               } /* endif */
            } else if ( pText[0] == FUNCTION_END ) {
               if ( Nesting == 0 ) 
                  return ( BytesProcessed ) ;
               Nesting -- ;
            } else if ( Nesting == 0 ) {
               if ( ( pText[0] == '\n' ) || ( pText[0] == '\r' ) ) {
                  FileWrite ( "\\par\n" ) ;
               } else if ( pText[0] == '\f' ) {
                  FileWrite ( "\\page\n" ) ;
               } else if ( pText[0] == '\t' ) {
                  FileWrite ( "\\tab " ) ;
               } else if ( pText[0] == 149 ) {
                  if ( CodePage == 1252 ) {
                     FileWrite ( "\\bullet " ) ;
                  } else {
                     FileWrite ( (char*)pText, 1 ) ;
                  } /* endif */
               } else if ( pText[0] == 150 ) {
                  if ( CodePage == 1252 ) {
                     FileWrite ( "\\endash " ) ;
                  } else {
                     FileWrite ( (char*)pText, 1 ) ;
                  } /* endif */
               } else if ( pText[0] == 151 ) {
                  if ( CodePage == 1252 ) {
                     FileWrite ( "\\emdash " ) ;
                  } else {
                     FileWrite ( (char*)pText, 1 ) ;
                  } /* endif */
               } else if ( pText[0] == 145 ) {
                  if ( CodePage == 1252 ) {
                     FileWrite ( "\\lquote " ) ;
                  } else {
                     FileWrite ( (char*)pText, 1 ) ;
                  } /* endif */
               } else if ( pText[0] == 146 ) {
                  if ( CodePage == 1252 ) {
                     FileWrite ( "\\rquote " ) ;
                  } else {
                     FileWrite ( (char*)pText, 1 ) ;
                  } /* endif */
               } else if ( pText[0] == 147 ) {
                  if ( CodePage == 1252 ) {
                     FileWrite ( "\\ldblquote " ) ;
                  } else {
                     FileWrite ( (char*)pText, 1 ) ;
                  } /* endif */
               } else if ( pText[0] == 148 ) {
                  if ( CodePage == 1252 ) {
                     FileWrite ( "\\rdblquote " ) ;
                  } else {
                     FileWrite ( (char*)pText, 1 ) ;
                  } /* endif */
               } else if ( ( pText[0] == ENDFILE ) && ( Index == Size - 1 ) ) {
                  // Discard the final EOF character.
               } else
                  FileWrite ( (char*)pText, 1 ) ;
            } /* endif */
            pText ++ ; Index ++ ; BytesProcessed ++ ;
         } /* endwhile */
         return ( BytesProcessed ) ;
      } /* endmethod */

      void ProcessText ( void *Text, long Size ) {

         // Scan the document, gathering information.  Build the font and color tables.
         NumberOfFonts = 0 ;
         Fonts = new char * [MAX_FONTS] ;
         memset ( Fonts, 0, sizeof(char*) * MAX_FONTS ) ;
         NumberOfColors = 1 ;
         Colors = new long [MAX_COLORS] ;
         memset ( Colors, 0, sizeof(long) * MAX_COLORS ) ;
         CodePage = 1252 ;
         { /* block */
            int Index ( 0 ) ;
            unsigned char *pText ( (unsigned char *) Text ) ;
            while ( Index < Size ) {
               if ( pText[0] == FUNCTION_START ) {
                  if ( !strnicmp ( (char*)pText+1, FunctionNames[FN__FONT], strlen(FunctionNames[FN__FONT]) ) )
                     FontIndex ( pText+strlen(FunctionNames[FN__FONT])+1 ) ;
                  else if ( !strnicmp ( (char*)pText+1, FunctionNames[FN__LINECOLOR], strlen(FunctionNames[FN__LINECOLOR]) ) )
                     ColorIndex ( pText+strlen(FunctionNames[FN__LINECOLOR])+1 ) ;
                  else if ( !strnicmp ( (char*)pText+1, FunctionNames[FN__CODEPAGE], strlen(FunctionNames[FN__CODEPAGE]) ) )
                     CodePage = atoi ( (char*)pText+strlen(FunctionNames[FN__CODEPAGE])+1 ) ;
               } /* endif */
               pText ++ ; Index ++ ;
            } /* endwhile */
         } /* endblock */

         // Write the file header.
         FileWrite ( "{\\rtf1" ) ;

         // Output the character set spec.
         switch ( CodePage ) {
            case 1252:
               FileWrite ( "\\ansi\n" ) ;
               break;
            case 437:
               FileWrite ( "\\pc\n" ) ;
               break;
            case 850:
               FileWrite ( "\\pca\n" ) ;
               break;
            default:
               FileWrite ( "\\ansi\\cpg%i\n", CodePage ) ;
               break;
         } /* endswitch */

         // Write out the font table.
         FileWrite ( "{\\fonttbl" ) ;
         for ( int i=0; i<NumberOfFonts; i++ ) {
            char Text [200] ;
            sprintf ( Text, "{\\f%i %s;}", i+1, Fonts[i] ) ;
            FileWrite ( Text ) ;
         } /* endfor */
         FileWrite ( "}\n" ) ;

         // Write out the color table.
         FileWrite ( "{\\colortbl;" ) ;
         for ( i=1; i<NumberOfColors; i++ ) {
            char Text [40] ;
            sprintf ( Text, "\\red%i\\green%i\\blue%i;", (Colors[i]>>16)&0xFF, (Colors[i]>>8)&0xFF, Colors[i]&0xFF ) ;
            FileWrite ( Text ) ;
         } /* endfor */
         FileWrite ( "}\n" ) ;

         // Process the outer text block.
         ProcessBlock ( Text, Size ) ;

         // Write the file trailer.
         FileWrite ( "\n}" ) ;

         // Pull off the file's full name (name+ext) from the full path.
         char FullName [ CCHMAXPATH + 1 ] = { 0 } ;
         char Drive [_MAX_DRIVE+1] = { 0 } ;
         char Dir   [_MAX_DIR+1] = { 0 } ;
         char Fname [_MAX_FNAME+1] = { 0 } ;
         char Ext   [_MAX_EXT+1] = { 0 } ;
         _splitpath ( Name, Drive, Dir, Fname, Ext ) ;
         strcpy ( FullName, Fname ) ;
         strcat ( FullName, Ext ) ;

         // Build the file's extended attributes.
         static MVMT_VALUE Types [] = {
            { EAT_ASCII, 10, "RTF"    },
         } ;
         static EADATA Table [] = {
            { PSZ(".TYPE"),     EAT_MVMT,  0, 0 },     // Type length & value need filling in.
            { PSZ(".LONGNAME"), EAT_ASCII, 0, 0 },     // Name length & value need filling in.
         } ;
         Table[0].Length = USHORT ( BuildMVMTValue ( 0, sizeof(Types)/sizeof(Types[0]), Types ) ) ;
         Table[0].Value = malloc ( Table[0].Length ) ;
         BuildMVMTValue ( Table[0].Value, sizeof(Types)/sizeof(Types[0]), Types ) ;
         Table[1].Length = USHORT ( strlen ( FullName ) ) ;
         Table[1].Value = FullName ;
         PEAOP2 pExtendedAttributes = BuildExtendedAttributes ( sizeof(Table)/sizeof(Table[0]), Table ) ;

         // If the extended attributes got built successfully, then attach them to the file.
         if ( pExtendedAttributes ) {
            DosSetFileInfo ( ULONG(fileno(File)), 2, PUCHAR(pExtendedAttributes), sizeof(*pExtendedAttributes) ) ;
            free ( pExtendedAttributes->fpFEA2List ) ;
            free ( pExtendedAttributes ) ;
         } /* endif */

         // Free the memory allocated for the MVMT value.
         free ( Table[0].Value ) ;

      } /* endmethod */

} ;

extern BOOL APIENTRY Export ( HWND Owner, char *Name, void *Text, long Size ) {

   // Try to export the file.
   try {
      ExportFile Exporter ( Name ) ;
      Exporter.ProcessText ( Text, Size ) ;
   } /* endtry */

   // If an error message is thrown, display it and return an error status.
   catch ( char *Message ) {
      if ( Owner ) 
         Sys_MessageBox ( HWND_DESKTOP, Owner, PSZ(Message), ProductName(), 0, MB_ENTER | MB_ICONEXCLAMATION ) ;
      return ( FALSE ) ;
   } /* endcatch */

   // Otherwise, return no error.
   return ( TRUE ) ;
}


