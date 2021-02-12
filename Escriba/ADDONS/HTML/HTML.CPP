/******************************************************************* HTML.CPP
 *                                                                          *
 *              Plain Text Import/Export Add-on for Escriba                 *
 *                                                                          *
 ****************************************************************************/

#include "System.h"

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

#include "HTML.h"

// #define DEBUG

/****************************************************************************
 *                                                                          *
 *                       Definitions & Declarations                         *
 *                                                                          *
 ****************************************************************************/

  // Macros & Constants

#define PROGRAM_NAME            "HTML"
#define LANGUAGE_NAME           "HTML_*.DLL"

#define DEFAULT_FONT            "Helv"
#define DEFAULT_TTFONT          "Courier"
#define DEFAULT_SIZE            (100)
#define DEFAULT_NETSCAPE        (TRUE)

#define DIM(x) (sizeof(x)/sizeof(x[0]))


  // Function Prototypes

extern "C" {
   extern int _CRT_init ( void ) ;
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
   #ifdef DEBUG
      Log ( "ProductVersion started.  Revision %i.", REVISION_NO ) ;
   #endif
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

static char *Bold = " Bold" ;
static char *Italic = " Italic" ;
static USHORT Sizes [] = { 80, 100, 120 } ;

typedef struct {
   HWND ErrorField ;
   char ImportFont [FACESIZE] ;
   char ImportTTFont [FACESIZE] ;
   long ImportSize ;
   BOOL ExportNetScape ;
   HelpWindow *Help ;
} CONFIGUREPARMS, *PCONFIGUREPARMS ;

static int _Optlink Compare_FONTMETRICS ( const void *Item1, const void *Item2 ) {
  PFONTMETRICS Font1 = PFONTMETRICS ( Item1 ) ;
  PFONTMETRICS Font2 = PFONTMETRICS ( Item2 ) ;
  return ( strcmp ( Font1->szFacename, Font2->szFacename ) ) ;
}

static MRESULT EXPENTRY ImportPageProcessor ( HWND Window, ULONG msg, MPARAM mp1, MPARAM mp2 ) {

   switch ( msg ) {

      case WM_INITDLG: {
         PHWND phwndErrorField = PHWND ( PVOIDFROMMP ( mp2 ) ) ;
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

         // Set up the current state.
         Parms->ErrorField = *phwndErrorField ;
         strcpy ( Parms->ImportFont, DEFAULT_FONT ) ;
         strcpy ( Parms->ImportTTFont, DEFAULT_TTFONT ) ;
         Parms->ImportSize = DEFAULT_SIZE ;
         Profile SetupData ( PROGRAM_NAME, WinQueryAnchorBlock(Window), HomePath ) ;
         SetupData.GetString ( "ImportFont", Parms->ImportFont, sizeof(Parms->ImportFont) ) ;
         SetupData.GetString ( "ImportTTFont", Parms->ImportTTFont, sizeof(Parms->ImportTTFont) ) ;
         SetupData.GetItem ( "ImportSize", &Parms->ImportSize, sizeof(Parms->ImportSize) ) ;

         // Build the font list.
         Canvas MyCanvas ( "PTEXT::ImportPageProcessor::MyCanvas", Window ) ;
         int FontCount = MyCanvas.QueryFontCount ( ) ;
         PFONTMETRICS pfm = PFONTMETRICS ( malloc ( size_t ( FontCount * sizeof(FONTMETRICS) ) ) ) ;
         MyCanvas.QueryFonts ( FontCount, pfm ) ;
         qsort ( pfm, (size_t)FontCount, sizeof(FONTMETRICS), Compare_FONTMETRICS ) ;
         for ( int i=0, j=0; i<FontCount; i++ ) {
            if ( pfm[i].fsDefn & FM_DEFN_OUTLINE ) {
               if ( i != j )
                  pfm[j] = pfm[i] ;
               j ++ ;
            } /* endif */
         } /* endfor */
         FontCount = j ;
         for ( i=0, j=0; i<(USHORT)FontCount; i++ ) {
            char Facename [FACESIZE+1] ;
            strcpy ( Facename, PCHAR(pfm[i].szFacename) ) ;
            char *p = strstr ( Facename, PCHAR(Bold) ) ;
            if ( p ) {
               char *p2 = p + strlen(PCHAR(Bold)) ;
               while ( *p2 ) *p++ = *p2++ ;
               *p = 0 ;
            } /* endif */
            p = strstr ( Facename, PCHAR(Italic) ) ;
            if ( p ) {
               char *p2 = p + strlen(PCHAR(Italic)) ;
               while ( *p2 ) *p++ = *p2++ ;
               *p = 0 ;
            } /* endif */
            if ( ( j == 0 ) || strcmpi ( Facename, pfm[j-1].szFacename ) ) {
               if ( i != j )
                  pfm[j] = pfm[i] ;
               j ++ ;
            } /* endif */
         } /* endfor */
         FontCount = j ;

         // Load the regular font list.
         for ( i=0; i<FontCount; i++ ) {
            Sys_SendMessage ( CHILD(Window,IDD_CONFIGURE_FONT), LM_INSERTITEM,
               MPFROMSHORT ( LIT_SORTASCENDING ), MPFROMP ( pfm[i].szFacename ) ) ;
         } /* endfor */
         Sys_SetWindowText ( CHILD(Window,IDD_CONFIGURE_FONT), PSZ(Parms->ImportFont) ) ;
         for ( i=0; i<FontCount; i++ ) {
            UCHAR Font [FACESIZE] ;
            Sys_SendMessage ( CHILD(Window,IDD_CONFIGURE_FONT), LM_QUERYITEMTEXT,
               MPFROM2SHORT ( i, sizeof(Font) ), MPFROMP ( Font ) ) ;
            if ( !strcmpi ( PCHAR(Parms->ImportFont), PCHAR(Font) ) ) {
               Sys_SendMessage ( CHILD(Window,IDD_CONFIGURE_FONT), LM_SELECTITEM,
                  MPFROMSHORT(i), MPFROMSHORT(TRUE) ) ;
               break ;
            } /* endif */
         } /* endfor */

         // Load the typewriter font list.
         for ( i=0; i<FontCount; i++ ) {
            if ( pfm[i].fsType & FM_TYPE_FIXED ) {
               Sys_SendMessage ( CHILD(Window,IDD_CONFIGURE_TTFONT), LM_INSERTITEM,
                  MPFROMSHORT ( LIT_SORTASCENDING ), MPFROMP ( pfm[i].szFacename ) ) ;
            } /* endif */
         } /* endfor */
         Sys_SetWindowText ( CHILD(Window,IDD_CONFIGURE_TTFONT), PSZ(Parms->ImportTTFont) ) ;
         for ( i=0; i<FontCount; i++ ) {
            UCHAR Font [FACESIZE] ;
            Sys_SendMessage ( CHILD(Window,IDD_CONFIGURE_TTFONT), LM_QUERYITEMTEXT,
               MPFROM2SHORT ( i, sizeof(Font) ), MPFROMP ( Font ) ) ;
            if ( !strcmpi ( PCHAR(Parms->ImportTTFont), PCHAR(Font) ) ) {
               Sys_SendMessage ( CHILD(Window,IDD_CONFIGURE_TTFONT), LM_SELECTITEM,
                  MPFROMSHORT(i), MPFROMSHORT(TRUE) ) ;
               break ;
            } /* endif */
         } /* endfor */

         // Release the font list.
         free ( pfm ) ;

         // Build the size list.
         for ( i=0; i<sizeof(Sizes)/sizeof(Sizes[0]); i++ ) {
            ResourceString Pts ( Library->QueryHandle(), IDS_PTS ) ;
            char Text [10] ;
            sprintf ( Text, "%.1lf%s", (double)Sizes[i]/10.0, PSZ(Pts) ) ;
            Sys_SendMessage ( CHILD(Window,IDD_CONFIGURE_SIZE), LM_INSERTITEM,
               MPFROMSHORT ( LIT_END ), MPFROMP ( Text ) ) ;
            if ( Sizes[i] == Parms->ImportSize ) {
               Sys_SetWindowText ( CHILD(Window,IDD_CONFIGURE_SIZE), PSZ(Text) ) ;
               Sys_SendMessage ( CHILD(Window,IDD_CONFIGURE_SIZE), LM_SELECTITEM, MPFROMSHORT(i), MPFROMSHORT(TRUE) ) ;
            } /* endif */
         } /* endfor */

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

               // Get the new font.
               char ImportFont [FACESIZE] ;
               Sys_GetWindowText ( CHILD(Window,IDD_CONFIGURE_FONT), ImportFont, sizeof(ImportFont) ) ;
               memcpy ( Parms->ImportFont, ImportFont, sizeof(Parms->ImportFont) ) ;

               // Get the new typewriter font.
               char ImportTTFont [FACESIZE] ;
               Sys_GetWindowText ( CHILD(Window,IDD_CONFIGURE_TTFONT), ImportTTFont, sizeof(ImportTTFont) ) ;
               memcpy ( Parms->ImportTTFont, ImportTTFont, sizeof(Parms->ImportTTFont) ) ;

               // Get the new size.
               SHORT Index = SHORT1FROMMR ( Sys_SendMessage ( CHILD(Window,IDD_CONFIGURE_SIZE),
                  LM_QUERYSELECTION, MPFROMSHORT(LIT_FIRST), 0 ) ) ;
               Parms->ImportSize = ( Index == LIT_NONE ) ? DEFAULT_SIZE : Sizes[Index] ;

               // Save the new information in PTEXT.INI.
               Profile SetupData ( PROGRAM_NAME, WinQueryAnchorBlock(Window), HomePath ) ;
               SetupData.PutString ( "ImportFont", Parms->ImportFont ) ;
               SetupData.PutString ( "ImportTTFont", Parms->ImportTTFont ) ;
               SetupData.PutItem ( "ImportSize", &Parms->ImportSize, sizeof(Parms->ImportSize) ) ;

               break ; } /* endcase */

            case BKN_PAGESELECTED: {
//             PAGESELECTNOTIFY *pInfo = (PAGESELECTNOTIFY*) PVOIDFROMMP(mp2) ;
               Sys_SetFocus ( CHILD(Window,IDD_CONFIGURE_FONT) ) ;
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
         PHWND phwndErrorField = PHWND ( PVOIDFROMMP ( mp2 ) ) ;
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

         // Set up the current state.
         Parms->ErrorField = *phwndErrorField ;
         Parms->ExportNetScape = DEFAULT_NETSCAPE ;
         Profile SetupData ( PROGRAM_NAME, WinQueryAnchorBlock(Window), HomePath ) ;
         SetupData.GetItem ( "ExportNetScape", &Parms->ExportNetScape, sizeof(Parms->ExportNetScape) ) ;

         // Load the checkboxes.
         Sys_SendMessage ( CHILD(Window,IDD_CONFIGURE_NETSCAPE),  BM_SETCHECK, MPFROMSHORT(Parms->ExportNetScape), 0 ) ;

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

               // Get checkbox values.
               Parms->ExportNetScape = SHORT1FROMMR ( Sys_SendMessage ( CHILD(Window,IDD_CONFIGURE_NETSCAPE), BM_QUERYCHECK, 0, 0 ) ) ;

               // Save the new information in PTEXT.INI.
               Profile SetupData ( PROGRAM_NAME, WinQueryAnchorBlock(Window), HomePath ) ;
               SetupData.PutItem ( "ExportNetScape", &Parms->ExportNetScape, sizeof(Parms->ExportNetScape) ) ;

               break ; } /* endcase */

            case BKN_PAGESELECTED: {
//             PAGESELECTNOTIFY *pInfo = (PAGESELECTNOTIFY*) PVOIDFROMMP(mp2) ;
               Sys_SetFocus ( CHILD(Window,IDD_CONFIGURE_FONT) ) ;
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
   return ( "HTML" ) ;
}


/****************************************************************************
 *                                                                          *
 *  Query Filename Wildcard                                                 *
 *                                                                          *
 ****************************************************************************/

extern char* APIENTRY QueryWildcard ( void ) {
   return ( "*.HTML,*.HTM" ) ;
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

   // Match against *.HTML.
   if ( MatchWildcard ( Name, "*.HTML" ) )
      return ( TRUE ) ;

   // Match against *.HTM.
   if ( MatchWildcard ( Name, "*.HTM" ) )
      return ( TRUE ) ;

   // Otherwise, return FALSE.
   return ( FALSE ) ;
}


/****************************************************************************
 *                                                                          *
 *  HTML Parser, adapted to C++, based on Netscape's parser.                *
 *                                                                          *
 ****************************************************************************/

static struct { char *Tag; int Token; } TagTable [] = {
  { PT_TITLE              , P_TITLE             },
  { PT_INDEX              , P_INDEX             },
  { PT_BASE               , P_BASE              },
  { PT_LINK               , P_LINK              },
  { PT_HEADER_1           , P_HEADER_1          },
  { PT_HEADER_2           , P_HEADER_2          },
  { PT_HEADER_3           , P_HEADER_3          },
  { PT_HEADER_4           , P_HEADER_4          },
  { PT_HEADER_5           , P_HEADER_5          },
  { PT_HEADER_6           , P_HEADER_6          },
  { PT_ANCHOR             , P_ANCHOR            },
  { PT_PARAGRAPH          , P_PARAGRAPH         },
  { PT_ADDRESS            , P_ADDRESS           },
  { PT_IMAGE              , P_IMAGE             },
  { PT_PLAIN_TEXT         , P_PLAIN_TEXT        },
  { PT_PLAIN_PIECE        , P_PLAIN_PIECE       },
  { PT_PREFORMAT          , P_PREFORMAT         },
  { PT_LISTING_TEXT       , P_LISTING_TEXT      },
  { PT_UNUM_LIST          , P_UNUM_LIST         },
  { PT_NUM_LIST           , P_NUM_LIST          },
  { PT_MENU               , P_MENU              },
  { PT_DIRECTORY          , P_DIRECTORY         },
  { PT_LIST_ITEM          , P_LIST_ITEM         },
  { PT_DESC_LIST          , P_DESC_LIST         },
  { PT_DESC_TITLE         , P_DESC_TITLE        },
  { PT_DESC_TEXT          , P_DESC_TEXT         },
  { PT_STRIKEOUT          , P_STRIKEOUT         },
  { PT_FIXED              , P_FIXED             },
  { PT_BOLD               , P_BOLD              },
  { PT_ITALIC             , P_ITALIC            },
  { PT_EMPHASIZED         , P_EMPHASIZED        },
  { PT_STRONG             , P_STRONG            },
  { PT_CODE               , P_CODE              },
  { PT_SAMPLE             , P_SAMPLE            },
  { PT_KEYBOARD           , P_KEYBOARD          },
  { PT_VARIABLE           , P_VARIABLE          },
  { PT_CITATION           , P_CITATION          },
  { PT_BLOCKQUOTE         , P_BLOCKQUOTE        },
  { PT_FORM               , P_FORM              },
  { PT_INPUT              , P_INPUT             },
  { PT_SELECT             , P_SELECT            },
  { PT_OPTION             , P_OPTION            },
  { PT_TEXTAREA           , P_TEXTAREA          },
  { PT_HRULE              , P_HRULE             },
  { PT_LINEBREAK          , P_LINEBREAK         },
  { PT_WORDBREAK          , P_WORDBREAK         },
  { PT_NOBREAK            , P_NOBREAK           },
  { PT_BASEFONT           , P_BASEFONT          },
  { PT_FONT               , P_FONT              },
  { PT_BLINK              , P_BLINK             },
  { PT_NEW_IMAGE          , P_NEW_IMAGE         },
  { PT_CENTER             , P_CENTER            },
  { PT_SUBDOC             , P_SUBDOC            },
  { PT_CELL               , P_CELL              },
  { PT_TABLE              , P_TABLE             },
  { PT_CAPTION            , P_CAPTION           },
  { PT_TABLE_ROW          , P_TABLE_ROW         },
  { PT_TABLE_HEADER       , P_TABLE_HEADER      },
  { PT_TABLE_DATA         , P_TABLE_DATA        },
  { PT_EMBED              , P_EMBED             },
  { PT_BODY               , P_BODY              },
  { PT_META               , P_META              },
  { PT_COLORMAP           , P_COLORMAP          },
  { PT_HYPE               , P_HYPE              },
  { PT_BIG                , P_BIG               },
  { PT_SMALL              , P_SMALL             },
  { PT_SUPER              , P_SUPER             },
  { PT_SUB                , P_SUB               },
  { PT_GRID               , P_GRID              },
  { PT_GRID_CELL          , P_GRID_CELL         },
  { PT_NOGRIDS            , P_NOGRIDS           },
  { PT_JAVA_APPLET        , P_JAVA_APPLET       },
  { PT_PARAM              , P_PARAM             },
  { PT_MAP                , P_MAP               },
  { PT_AREA               , P_AREA              },
  { PT_DIVISION           , P_DIVISION          },
  { PT_KEYGEN             , P_KEYGEN            },
  { PT_SCRIPT             , P_SCRIPT            },
  { PT_NOSCRIPT           , P_NOSCRIPT          },
  { PT_NOEMBED            , P_NOEMBED           },
  { PT_HEAD               , P_HEAD              },
  { PT_HTML               , P_HTML              },
  { PT_SERVER             , P_SERVER            },
  { PT_CERTIFICATE        , P_CERTIFICATE       },
  { PT_STRIKE             , P_STRIKE            },
  { PT_UNDERLINE          , P_UNDERLINE         },
  { PT_SPACER             , P_SPACER            },
  { PT_MULTICOLUMN        , P_MULTICOLUMN       },
  { PT_NSCP_CLOSE         , P_NSCP_CLOSE        },
  { PT_NSCP_OPEN          , P_NSCP_OPEN         },
  { PT_ILAYER             , P_ILAYER            },
  { PT_LAYER              , P_LAYER             },
  { PT_NOLAYER            , P_NOLAYER           },
  { PT_STYLE              , P_STYLE             },
  { PT_MQUOTE             , P_MQUOTE            },
  { PT_OBJECT             , P_OBJECT            },
  { PT_SPAN               , P_SPAN              },
  { PT_SPELL              , P_SPELL             },
  { PT_INLINEINPUT        , P_INLINEINPUT       },
  { PT_NSCP_REBLOCK       , P_NSCP_REBLOCK      },
  { PT_NSDT               , P_NSDT              },
  { PT_INLINEINPUTTHICK   , P_INLINEINPUTTHICK  },
  { PT_INLINEINPUTDOTTED  , P_INLINEINPUTDOTTED },
  { "doctype"             , 1000 },
} ;

static int pa_tokenize_tag ( char *str ) {
   for ( int i=0; i<DIM(TagTable); i++ ) 
      if ( !strcmpi ( str, TagTable[i].Tag ) ) 
         return ( TagTable[i].Token ) ;
   return ( P_UNKNOWN ) ;
}

static struct SymbolTable { char *Symbol ; unsigned char Code ; } Symbols[] = {
  { "&quot;",    34 },
  { "&amp;",     38 },
  { "&gt;",      60 },
  { "&lt;",      62 },
  { "&copy;",   169 },
  { "&Agrave;", 192 },
  { "&Aacute;", 193 },
  { "&Acirc;",  194 },
  { "&Atilde;", 195 },
  { "&Auml;",   196 },
  { "&Aring;",  197 },
  { "&Aelig;",  198 },
  { "&Ccedil;", 199 },
  { "&Egrave;", 200 },
  { "&Eacute;", 201 },
  { "&Ecirc;",  202 },
  { "&Euml;",   203 },
  { "&Igrave;", 204 },
  { "&Iacute;", 205 },
  { "&Icirc;",  206 },
  { "&Iuml;",   207 },
  { "&ETH;",    208 },
  { "&Ntilde;", 209 },
  { "&Ograve;", 210 },
  { "&Oacute;", 211 },
  { "&Ocirc;",  212 },
  { "&Otilde;", 213 },
  { "&Ouml;",   214 },
  { "&Oslash;", 216 },
  { "&Ugrave;", 217 },
  { "&Uacute;", 218 },
  { "&Ucirc;",  219 },
  { "&Uuml;",   220 },
  { "&Yacute;", 221 },
  { "&THORN;",  222 },
  { "&szlig;",  223 },
  { "&agrave;", 224 },
  { "&aacute;", 225 },
  { "&acirc;",  226 },
  { "&atilde;", 227 },
  { "&auml;",   228 },
  { "&aring;",  229 },
  { "&aelig;",  230 },
  { "&ccedil;", 231 },
  { "&egrave;", 232 },
  { "&eacute;", 233 },
  { "&ecirc;",  234 },
  { "&euml;",   235 },
  { "&igrave;", 236 },
  { "&iacute;", 237 },
  { "&icirc;",  238 },
  { "&iuml;",   239 },
  { "&eth;",    240 },
  { "&ntilde;", 241 },
  { "&ograve;", 242 },
  { "&oacute;", 243 },
  { "&ocirc;",  244 },
  { "&otilde;", 245 },
  { "&ouml;",   246 },
  { "&oslash;", 248 },
  { "&ugrave;", 249 },
  { "&uacute;", 250 },
  { "&ucirc;",  251 },
  { "&uuml;",   252 },
  { "&yacute;", 253 },
  { "&thorn;",  254 },
  { "&yuml;",   255 },
} ;

static char *ConvertSymbol ( char *pText, unsigned char &Symbol ) {

   // Return at once if the first character is not a '&'.
   if ( pText[0] != '&' ) 
      return ( 0 ) ;

   // Check for "&#nnn;".
   if ( ( pText[1] == '#' ) && strchr ( pText+2, ';' ) ) {
      int Value ; sscanf ( pText, "&#%d;", &Value ) ;
      Symbol = (unsigned char) Value ;
      return ( strchr ( pText+2, ';' ) + 1 ) ;
   } /* endif */

   // Otherwise scan the symbol table.
   for ( int i=0; i<DIM(Symbols); i++ ) {
      if ( !strncmp ( pText, Symbols[i].Symbol, strlen(Symbols[i].Symbol) ) ) {
         Symbol = Symbols[i].Code ;
         return ( pText + strlen(Symbols[i].Symbol) ) ;
      } /* endif */
   } /* endfor */

   // Return NULL if total failure.
   return ( 0 ) ;
}

#define STRING_EXPANSION_INCREMENT (16)

static int appendString ( char **str, unsigned short *len, unsigned short *size, char c ) {
   if ( *len == *size ) {
      char *newName ( new char [*size+STRING_EXPANSION_INCREMENT] ) ;
      char *old ( *str ) ;
      if ( newName == NULL ) return ( -1 ) ;
      memcpy ( newName, *str, *size ) ;
      *str = newName ;
      if ( old != NULL ) delete [] old ;
      *size += STRING_EXPANSION_INCREMENT ;
   } /* endif */
   *( *str + *len ) = c ;
   ++(*len) ;
   return ( 0 ) ;
}

#define STRINGLIST_EXPANSION_INCREMENT (8)

static int appendStringList ( char ***list_p, unsigned short *len, unsigned short *size, char *str ) {
   char **list = *list_p ;
   if ( *len == *size ) {
      char **newList = new char* [*size+STRINGLIST_EXPANSION_INCREMENT] ;
      char **old = list ;
      if ( newList == NULL ) return ( -1 ) ;
      memcpy ( newList, list, sizeof(char*)*(*size) ) ;
      list = newList ;
      if ( old != NULL ) delete [] old ;
      *size += STRINGLIST_EXPANSION_INCREMENT ;
   } /* endif */
   *( list + *len ) = str ;
   ++(*len);
   *list_p = list ;
   return ( 0 ) ;
}

class Tag {

   private: public:

      char *name ;
      int token ;
      char **attributeNames ;
      char **attributeValues ;
      unsigned short sizeNames ;
      unsigned short numNames ;
      unsigned short sizeValues ;
      unsigned short numValues ;
      int emptyTagp ;
      int endTagp ;

   public:

      Tag ( ) : name(0), token(0), sizeNames(4), sizeValues(4), numNames(0), numValues(0), emptyTagp(FALSE), endTagp(FALSE) {
         attributeNames = new char* [sizeNames] ;
         attributeValues = new char* [sizeValues] ;
      } /* endmethod */

      ~Tag ( ) {
         Clear ( ) ;
         delete [] attributeValues ;
         delete [] attributeNames ;
      } /* endmethod */

      void Clear ( ) {
         if ( name )
            delete [] name, name = 0 ;
         for ( int i=0; i<numNames; i++ )
            delete [] attributeNames[i] ;
         for ( i=0; i<numValues; i++ )
            delete [] attributeValues[i] ;
         numNames = 0 ;
         numValues = 0 ;
         emptyTagp = FALSE ;
         endTagp = FALSE ;
      } /* endmethod */

      char *GetTagName ( ) { return ( name ) ; }

      int GetTagToken ( ) { return ( token ) ; }

      int IsEmptyTag ( ) { return ( emptyTagp ) ; }

      int IsEndTag ( ) { return ( endTagp ) ; }

      unsigned short GetNumberOfAttributes ( ) { return ( numNames ) ; }

      char *GetNthAttributeName ( unsigned short n ) { return ( attributeNames[n] ) ; }

      char *GetNthAttributeValue ( unsigned short n ) { return ( attributeValues[n] ) ; }

      char *GetAttributeValue ( char *attributeName ) {
         int count ( 0 ) ;
         while ( count < numNames ) {
            if ( !strcmp ( attributeName, attributeNames[count] ) )
               return ( attributeValues[count] ) ;
            count ++ ;
         } /* endwhile */
         return ( 0 ) ;
      } /* endmethod */
} ;

typedef int (*ParseFunction) ( class Parser *obj, int isTag, void *data ) ;

/* return values from CRAWL_ParserPut */
enum { CRAWL_PARSE_NO_ERROR, CRAWL_PARSE_ERROR, CRAWL_PARSE_TERMINATE, CRAWL_PARSE_OUT_OF_MEMORY } ;

/* return values for CRAWL_ParseFunc callback function */
enum { PARSE_GET_NEXT_TOKEN, PARSE_STOP, PARSE_OUT_OF_MEMORY } ;

class Parser {

   private:

      enum { PS_START, PS_BETWEEN_TAGS, PS_TAG_NAME, PS_EMPTY_TAGS,
         PS_CLOSE_BRACKET, PS_ATTRIBUTE, PS_EQUALS, PS_VALUE,
         PS_START_COMMENT, PS_END_COMMENT } state ;
      Tag *tag ;
      char *data ;
      unsigned short dataLen ;
      unsigned short dataSize ;
      char *str ;
      unsigned short strLen ;
      unsigned short strSize ;
      char prev1 ;
      char prev2 ;
      char inQuote ;
      int inComment ;
      int inScript ;
      int skipWhitespace ;
      int isRDF ;

   public:

      Parser ( ) : state(PS_START), data(0), dataLen(0), dataSize(0), str(0), strLen(0), strSize(0), 
         prev1(0), prev2(0), inQuote(FALSE), inComment(FALSE), inScript(FALSE), skipWhitespace(FALSE), isRDF(FALSE) {
         tag = new Tag ;
      } /* endmethod */

      ~Parser ( ) {
         delete tag ;
         if ( data )
            delete [] data, data = 0 ;
         if ( str )
            delete [] str, str = 0 ;
      } /* endmethod */

      void Clear ( ) {
         tag->Clear ( ) ;
         if ( data )
            delete [] data ;
         data = 0 ;
         dataLen = dataSize = 0 ;
      } /* endmethod */

      Tag *GetTagParsed ( ) { return ( data ? 0 : tag ) ; }

      char *GetDataParsed ( ) { return ( data ? data : 0 ) ; }

      int Put ( char *string, unsigned long len, ParseFunction func, void *userdata ) {
         unsigned long n ( 0 ) ;             /* where we are in the buffer */
         unsigned long lastn ( 0 ) ;         /* position the last time in the loop */
         char c ( 0 ) ;
         while ( n < len ) {
            if ( lastn < n ) {        /* we advanced a character */
               prev1 = prev2;
               prev2 = c;
            } /* endif */
            lastn = n;
            c = *( string + n );
            if ( inComment ) {
               /* if we're in a comment, ignore everything until we detect end of comment */
               if ( ( prev1 == '-' ) && ( prev2 == '-' ) && ( c == '>' ) )
                  inComment = FALSE;
               n++;
            } else if ( skipWhitespace ) {
               if ( ( c == ' ' ) || ( c == '\n' ) || ( c == '\r' ) ) {
                  n++;
               } else
                  skipWhitespace = FALSE;
            } else {
               int endOfString = FALSE;
               switch ( state ) {
                  case PS_START: {
                     /* PS_START - expecting open bracket or character data */
                     if ( c == '<' ) {
                        state = PS_TAG_NAME;
                        n++;
                     } else {
                        state = PS_BETWEEN_TAGS;
                     }
                     break; } /* endcase PS_START */
                  case PS_BETWEEN_TAGS: {
                     /* PS_BETWEEN_TAGS - expecting open bracket (terminating character data) or more character data */
                     if ( inQuote == c ) {
                        inQuote = 0;  /* close quote */
                     } else if ( ( c == '"' ) || ( inScript && ( c == '\'' ) ) ) {       /* start a quote, only double quotes significant in between tags */
                        inQuote = c;
                     } /* endif */
                     /* open bracket not in quoted section indicates end of data */
                     if ( ( inQuote == 0 ) && ( c == '<' ) ) {
                        state = PS_START;
                        if ( appendString ( (char**)&data, &dataLen, &dataSize, 0 ) != 0 )    /* null terminate string */
                           return CRAWL_PARSE_OUT_OF_MEMORY;
                        if ( func ( this, FALSE, userdata ) == PARSE_STOP )
                           return CRAWL_PARSE_TERMINATE;
                        Clear ( ) ;
                     } else {
                        if ( appendString ( (char**)&data, &dataLen, &dataSize, c ) != 0 )
                           return CRAWL_PARSE_OUT_OF_MEMORY;
                        n++;
                     } /* endif */
                     break; } /* endcase PS_BETWEEN_TAGS */
                  case PS_TAG_NAME: {
                     /* PS_TAG_NAME - terminated by space, \r, \n, >, / */
                     if ( ( c == '"' ) || ( c == '\'' ) )
                        return CRAWL_PARSE_ERROR;     /* error - these are not allowed in tagname */
                     else if ( c == ' ' ) {
                        /* Note: Both mozilla and XML don't allow any spaces between < and tagname. Need to check for zero-length tagname. */
                        if ( str == NULL )
                           return CRAWL_PARSE_ERROR;  /* str is the buffer we're working on */
                        endOfString = TRUE;
                        state = PS_ATTRIBUTE;
                        skipWhitespace = TRUE;
                        n++;
                     } else if ( c == '/' ) {
                        if ( tag->GetTagName() == NULL )
                           tag->endTagp = TRUE;       /* indicates end tag if no tag name read yet */
                        else if ( isRDF ) {      /* otherwise its an empty tag (RDF only) */
                           endOfString = TRUE;
                           tag->emptyTagp = TRUE;
                           state = PS_CLOSE_BRACKET;
                        } else
                           return CRAWL_PARSE_ERROR;
                        n++;
                     } else if ( c == '>' ) {
                        endOfString = TRUE;
                        state = PS_CLOSE_BRACKET;
                     } else if ( ( c != '\r' ) && ( c != '\n' ) ) {
                        if ( appendString ( &str, &strLen, &strSize, c ) != 0 )
                           return CRAWL_PARSE_OUT_OF_MEMORY;
                        n++;
                     } else {
                        endOfString = TRUE;
                        state = PS_ATTRIBUTE;    /* note - mozilla allows newline after tag name */
                        skipWhitespace = TRUE;
                        n++;
                     }
                     if ( endOfString ) {
                        if ( appendString ( &str, &strLen, &strSize, 0 ) != 0 )       /* null terminate string */
                           return CRAWL_PARSE_OUT_OF_MEMORY;
                        if ( strcmp ( str, "!--" ) == 0 ) {      /* html comment */
                           inComment = TRUE;
                           state = PS_START;
                        } else {
                           tag->name = str;
                           tag->token = pa_tokenize_tag ( str );
                        } /* endif */
                        str = NULL;
                        strLen = strSize = 0;
                        endOfString = FALSE;
                     } /* endif */
                     break; } /* endcase PS_TAG_NAME */
                  case PS_CLOSE_BRACKET: {
                     /* PS_CLOSE_BRACKET - expecting a close bracket, anything else is an error */
                     if ( c == '>' ) {
                        if ( !isRDF && ( tag->token == P_SCRIPT ) ) {
                           /* we're inside a script tag (not RDF) */
                           if ( tag->endTagp )
                              inScript = FALSE;
                           else
                              inScript = TRUE;
                        }
                        if ( func ( this, TRUE, userdata ) == PARSE_STOP )
                           return CRAWL_PARSE_TERMINATE;
                        Clear ( ) ;
                        state = PS_START;
                        n++;
                     } else
                        return CRAWL_PARSE_ERROR;     /* error */
                     break; } /* endcase PS_CLOSE_BRACKET */
                  case PS_ATTRIBUTE: {
                     /* PS_ATTRIBUTE - expecting an attribute name, or / (RDF only) or > indicating no more attributes */
                     /* accept attributes without values, such as <tag attr1 attr2=val2> or <tag attr2=val2 attr1> */
                     if ( inQuote == c ) {
                        inQuote = 0;  /* close quote */
                     } else if ( ( ( c == '"' ) || ( c == '\'' ) ) && ( inQuote == 0 ) ) {
                        /* start a quote if none is already in effect */
                        inQuote = c;
                     }
                     if ( inQuote == 0 ) {
                        if ( ( ( ( c == '/' ) && isRDF ) || ( c == '>' ) ) && ( str == NULL ) ) {
                           state = PS_CLOSE_BRACKET;
                        } else if ( ( c == ' ' ) || ( c == '=' ) || ( c == '\n' ) || ( c == '\r' ) || ( ( c == '/' ) && isRDF ) || ( c == '>' ) ) {
                           if ( appendString ( &str, &strLen, &strSize, 0 ) != 0 )    /* null terminate string */
                              return CRAWL_PARSE_OUT_OF_MEMORY;
                           if ( appendStringList ( &tag->attributeNames, &tag->numNames, &tag->sizeNames, str ) != 0 )
                              return CRAWL_PARSE_OUT_OF_MEMORY;
                           str = NULL;
                           strLen = strSize = 0;
                           state = PS_EQUALS;    /* if non-null attribute name */
                        } else {
                           if ( appendString ( &str, &strLen, &strSize, c ) != 0 )
                              return CRAWL_PARSE_OUT_OF_MEMORY;
                           n++;
                        }
                     } else {
                        if ( appendString ( &str, &strLen, &strSize, c ) != 0 )
                           return CRAWL_PARSE_OUT_OF_MEMORY;
                        n++;
                     }
                     break; } /* endcase PS_ATTRIBUTE */
                  case PS_EQUALS: {
                     if ( ( c == ' ' ) || ( c == '\n' ) || ( c == '\r' ) ) {
                        skipWhitespace = TRUE;
                        n++;
                     } else if ( c == '=' ) {
                        skipWhitespace = TRUE;
                        state = PS_VALUE;
                        n++;
                     } else {         /* no value for the attribute - error in RDF? */
                        if ( appendString ( &str, &strLen, &strSize, 0 ) != 0 )       /* null terminate string */
                           return CRAWL_PARSE_OUT_OF_MEMORY;
                        if ( appendStringList ( &tag->attributeValues, &tag->numValues, &tag->sizeValues, str ) != 0 )
                           return CRAWL_PARSE_OUT_OF_MEMORY;
                        str = NULL;
                        strLen = strSize = 0;
                        state = PS_ATTRIBUTE;
                     }
                     break; } /* endcase PS_EQUALS */
                  case PS_VALUE: {
                     /* expecting a value, or space, / (RDF only), or > indicating end of value. */
                     int include = TRUE;     /* whether the current character should be included in value */
                     if ( inQuote == c ) {
                        inQuote = 0;       /* close quote */
                        include = FALSE;
                     } else if ( ( ( c == '"' ) || ( c == '\'' ) ) && ( inQuote == 0 ) ) {
                        /* start a quote if none is already in effect */
                        inQuote = c;
                        include = FALSE;
                     } /* endif */
                     if ( inQuote == 0 ) {
                        if ( ( c == '/' ) && isRDF ) {
                           endOfString = TRUE;
                           state = PS_CLOSE_BRACKET;
                           n++;
                        } else if ( c == '>' ) {
                           endOfString = TRUE;
                           state = PS_CLOSE_BRACKET;
                        } else if ( ( c == ' ' ) || ( c == '\r' ) || ( c == '\n' ) ) {
                           endOfString = TRUE;
                           skipWhitespace = TRUE;
                           state = PS_ATTRIBUTE;      /* if non-null value name */
                           n++;
                        } else if ( include ) {
                           if ( appendString ( &str, &strLen, &strSize, c ) != 0 )
                              return CRAWL_PARSE_OUT_OF_MEMORY;
                           n++;
                        } else
                           n++;
                     } else if ( include ) {
                        if ( appendString ( &str, &strLen, &strSize, c ) != 0 )
                           return CRAWL_PARSE_OUT_OF_MEMORY;
                        n++;
                     } else
                        n++;
                     if ( endOfString ) {
                        if ( appendString ( &str, &strLen, &strSize, 0 ) != 0 )    /* null terminate string */
                           return CRAWL_PARSE_OUT_OF_MEMORY;
                        if ( appendStringList ( &tag->attributeValues, &tag->numValues, &tag->sizeValues, str ) != 0 )
                           return CRAWL_PARSE_OUT_OF_MEMORY;
                        str = NULL;
                        strLen = strSize = 0;
                        endOfString = FALSE;
                     } /* endif */
                     break; } /* endcase VALUE */
               } /* endswitch for state */
            } /* endif comment, whitespace, token */
         } /* endwhile more data in buffer */
         return CRAWL_PARSE_NO_ERROR;
      } /* endmethod */
} ;

/****************************************************************************
 *                                                                          *
 *  Verify File by Inspection                                               *
 *                                                                          *
 ****************************************************************************/

struct VerifyParserInfo {

   // Data
   int Valid ;

   // Default Constructor
   VerifyParserInfo ( ) : Valid(FALSE) {
   } /* endmethod */

   // Destructor
   ~VerifyParserInfo ( ) {
   } /* endmethod */
} ;

static int ParseVerify ( Parser *parser, int isTag, void *userdata ) {

   VerifyParserInfo *pInfo = (VerifyParserInfo*) userdata ;

   if ( isTag ) {
      Tag *tag = parser->GetTagParsed ( ) ;
      switch ( tag->GetTagToken() ) {
         case P_HTML:
            pInfo->Valid = TRUE ;
            break;
      } /* endswitch */

   } /* endif */

   return ( PARSE_GET_NEXT_TOKEN ) ;
}

extern BOOL APIENTRY Verify ( HWND Owner, char *Name ) {

   FILE *File = fopen ( Name, "rb" ) ;
   if ( File == 0 ) {
      if ( Owner ) {
         ResourceString Message ( Library->QueryHandle(), IDS_ERROR_OPEN_VERIFY ) ;
         Sys_MessageBox ( HWND_DESKTOP, Owner, PCHAR(Message), ProductName(), 0, MB_ENTER | MB_ICONEXCLAMATION ) ;
      } /* endif */
      return ( FALSE ) ;
   } /* endif */

   fseek ( File, 0, SEEK_END ) ;
   size_t Size = (size_t) ftell ( File ) ;
   if ( Size == 0 ) {
      fclose ( File ) ;
      return ( TRUE ) ;
   } /* endif */

   fseek ( File, 0, SEEK_SET ) ;
   char *Text = new char [Size] ;
   if ( Text == 0 ) {
      fclose ( File ) ;
      if ( Owner ) {
         ResourceString Message ( Library->QueryHandle(), IDS_ERROR_MEMORY_VERIFY ) ;
         Sys_MessageBox ( HWND_DESKTOP, Owner, PCHAR(Message), ProductName(), 0, MB_ENTER | MB_ICONEXCLAMATION ) ;
      } /* endif */
      return ( FALSE ) ;
   } /* endif */

   // Read the text into memory.
   Size = fread ( Text, 1, Size, File ) ;
   if ( Size == 0 ) {
      delete [] Text ;
      fclose ( File ) ;
      return ( TRUE ) ;
   } /* endif */

   // Make sure the text contains no nulls.
   if ( strlen(Text) < Size ) {
      delete [] Text ;
      fclose ( File ) ;
      if ( Owner ) {
         ResourceString Message ( Library->QueryHandle(), IDS_ERROR_INVALID ) ;
         Sys_MessageBox ( HWND_DESKTOP, Owner, PSZ(Message), ProductName(), 0, MB_ENTER | MB_ICONEXCLAMATION ) ;
      } /* endif */
      return ( FALSE ) ;
   } /* endif */

   // Validate the text.
   Parser parser ; VerifyParserInfo VerifyInfo ;
   if ( parser.Put ( Text, Size, ParseVerify, &VerifyInfo ) || !VerifyInfo.Valid ) {
      delete [] Text ;
      fclose ( File ) ;
      if ( Owner ) {
         ResourceString Message ( Library->QueryHandle(), IDS_ERROR_INVALID ) ;
         Sys_MessageBox ( HWND_DESKTOP, Owner, PSZ(Message), ProductName(), 0, MB_ENTER | MB_ICONEXCLAMATION ) ;
      } /* endif */
      return ( FALSE ) ;
   } /* endif */

   // Clean up and return.
   delete [] Text ;
   fclose ( File ) ;
   return ( TRUE ) ;
}


/****************************************************************************
 *                                                                          *
 *  Import File                                                             *
 *                                                                          *
 ****************************************************************************/

struct SizeParserInfo {

   // Data
   unsigned long Size ;
   char Title [200] ;
   int InTitle ;
   int InBody ;
   char *ImportFont ;
   char *ImportTTFont ;

   // Default Constructor
   SizeParserInfo ( char *Font, char *TTFont ) : Size(0), InTitle(FALSE), InBody(FALSE) {
      memset ( Title, 0, sizeof(Title) ) ;
      ImportFont = new char [strlen(Font)+1] ; strcpy ( ImportFont, Font ) ;
      ImportTTFont = new char [strlen(TTFont)+1] ; strcpy ( ImportTTFont, TTFont ) ;
   } /* endmethod */

   // Destructor
   ~SizeParserInfo ( ) {
      delete [] ImportFont ;
      delete [] ImportTTFont ;
   } /* endmethod */
} ;

static int SizeParser ( Parser *parser, int isTag, void *userdata ) {

   SizeParserInfo *pInfo = (SizeParserInfo*) userdata ;

   if ( isTag ) {
      Tag *tag = parser->GetTagParsed ( ) ;
      switch ( tag->GetTagToken() ) {
         case P_TITLE:
            pInfo->InTitle = tag->IsEndTag() ? FALSE : TRUE ;
            break;
         case P_BODY:
            pInfo->InBody = tag->IsEndTag() ? FALSE : TRUE ;
            break;
         case P_PARAGRAPH:
            pInfo->Size += 2 ;
            break;
         case P_LINEBREAK:
            pInfo->Size ++ ;
            break;
         case P_HRULE:
            pInfo->Size += 2 + strlen(FunctionNames[FN__SEPARATOR]) ;
            break;
         case P_BOLD:
            pInfo->Size += 2 + strlen(FunctionNames[FN__BOLD]) + 1 ;
            break;
         case P_ITALIC:
            pInfo->Size += 2 + strlen(FunctionNames[FN__ITALIC]) + 1 ;
            break;
         case P_UNDERLINE:
            pInfo->Size += 2 + strlen(FunctionNames[FN__UNDERSCORE]) + 1 ;
            break;
         case P_STRIKE: case P_STRIKEOUT:
            pInfo->Size += 2 + strlen(FunctionNames[FN__STRIKEOUT]) + 1 ;
            break;
         case P_CENTER:
            if ( tag->IsEndTag() ) {
               pInfo->Size += 2 + strlen(FunctionNames[FN__LEFTJUSTIFY]) ;
            } else {
               pInfo->Size += 2 + strlen(FunctionNames[FN__CENTERJUSTIFY]) ;
            } /* endif */
            break;
         case P_FIXED:
            if ( tag->IsEndTag() ) {
               pInfo->Size += 2 + strlen(FunctionNames[FN__FONT]) + strlen(pInfo->ImportFont) ;
            } else {
               pInfo->Size += 2 + strlen(FunctionNames[FN__FONT]) + strlen(pInfo->ImportTTFont) ;
            } /* endif */
            break;
         case P_HEADER_1:
            if ( tag->IsEndTag() ) {
               pInfo->Size += 2 + strlen(FunctionNames[FN__LEFTJUSTIFY]) ;
            } else {
               pInfo->Size += 2 + strlen(FunctionNames[FN__CENTERJUSTIFY]) ;
            } /* endif */
            pInfo->Size += 2 ;
            pInfo->Size += 4 + strlen(FunctionNames[FN__SIZE]) + 6 ;
            break;
      } /* endswitch */

   } else if ( pInfo->InTitle ) {
      char *p = parser->GetDataParsed() ;
      for ( int i=0; i<strlen(p); i++ ) {
         unsigned char Code ; char *p1 ( ConvertSymbol ( p+i, Code ) ) ;
         if ( p1 ) {
            pInfo->Title[strlen(pInfo->Title)] = (char) Code ;
            i += p1 - (p+i) - 1 ;
         } else {
            pInfo->Title[strlen(pInfo->Title)] = p[i] ;
         } /* endif */
      } /* endfor */

   } else if ( pInfo->InBody ) {
      pInfo->Size += strlen(parser->GetDataParsed()) ;

   } /* endif */

   return ( PARSE_GET_NEXT_TOKEN ) ;
}

struct ImportParserInfo {

   // Data
   char *Text ;
   unsigned long Size ;
   int InBody ;
   char *ImportFont ;
   char *ImportTTFont ;
   int ImportSize ;

   // Default Constructor
   ImportParserInfo ( char *text, unsigned long size, char *Font, char *TTFont, int importsize ) : 
      Text(text), Size(size), ImportSize(importsize), InBody(FALSE) {
      ImportFont = new char [strlen(Font)+1] ; strcpy ( ImportFont, Font ) ;
      ImportTTFont = new char [strlen(TTFont)+1] ; strcpy ( ImportTTFont, TTFont ) ;
   } /* endmethod */

   // Destructor
   ~ImportParserInfo ( ) {
      delete [] ImportFont ;
      delete [] ImportTTFont ;
   } /* endmethod */
} ;

static int ImportParser ( Parser *parser, int isTag, void *userdata ) {

   ImportParserInfo *pInfo = (ImportParserInfo*) userdata ;

   if ( isTag ) {
      Tag *tag = parser->GetTagParsed ( ) ;
      switch ( tag->GetTagToken() ) {
         case P_TITLE:
            break;
         case P_BODY:
            pInfo->InBody = tag->IsEndTag() ? FALSE : TRUE ;
            break;
         case P_PARAGRAPH:
            pInfo->Text[pInfo->Size++] = '\n' ;
            pInfo->Text[pInfo->Size++] = '\n' ;
            break;
         case P_LINEBREAK:
            pInfo->Text[pInfo->Size++] = '\n' ;
            break;
         case P_HRULE:
            pInfo->Size += sprintf ( pInfo->Text+pInfo->Size, "%c%s%c", FUNCTION_START, FunctionNames[FN__SEPARATOR], FUNCTION_END ) ;
            break;
         case P_BOLD:
            pInfo->Size += sprintf ( pInfo->Text+pInfo->Size, "%c%s%i%c", FUNCTION_START, FunctionNames[FN__BOLD], tag->IsEndTag()?0:1, FUNCTION_END ) ;
            break;
         case P_ITALIC:
            pInfo->Size += sprintf ( pInfo->Text+pInfo->Size, "%c%s%i%c", FUNCTION_START, FunctionNames[FN__ITALIC], tag->IsEndTag()?0:1, FUNCTION_END ) ;
            break;
         case P_UNDERLINE:
            pInfo->Size += sprintf ( pInfo->Text+pInfo->Size, "%c%s%i%c", FUNCTION_START, FunctionNames[FN__UNDERSCORE], tag->IsEndTag()?0:1, FUNCTION_END ) ;
            break;
         case P_STRIKE: case P_STRIKEOUT:
            pInfo->Size += sprintf ( pInfo->Text+pInfo->Size, "%c%s%i%c", FUNCTION_START, FunctionNames[FN__STRIKEOUT], tag->IsEndTag()?0:1, FUNCTION_END ) ;
            break;
         case P_CENTER:
            if ( tag->IsEndTag() ) {
               pInfo->Size += sprintf ( pInfo->Text+pInfo->Size, "%c%s%c", FUNCTION_START, FunctionNames[FN__LEFTJUSTIFY], FUNCTION_END ) ;
            } else {
               pInfo->Size += sprintf ( pInfo->Text+pInfo->Size, "%c%s%c", FUNCTION_START, FunctionNames[FN__CENTERJUSTIFY], FUNCTION_END ) ;
            } /* endif */
            break;
         case P_FIXED:
            if ( tag->IsEndTag() ) {
               pInfo->Size += sprintf ( pInfo->Text+pInfo->Size, "%c%s%s%c", FUNCTION_START, FunctionNames[FN__FONT], pInfo->ImportFont, FUNCTION_END ) ;
            } else {
               pInfo->Size += sprintf ( pInfo->Text+pInfo->Size, "%c%s%s%c", FUNCTION_START, FunctionNames[FN__FONT], pInfo->ImportTTFont, FUNCTION_END ) ;
            } /* endif */
            break;
         case P_HEADER_1:
            if ( tag->IsEndTag() ) {
               pInfo->Size += sprintf ( pInfo->Text+pInfo->Size, "%c%s%.1lf%c", FUNCTION_START, FunctionNames[FN__SIZE], double(pInfo->ImportSize)/10, FUNCTION_END ) ;
               pInfo->Size += sprintf ( pInfo->Text+pInfo->Size, "%c%s%c", FUNCTION_START, FunctionNames[FN__LEFTJUSTIFY], FUNCTION_END ) ;
               pInfo->Text[pInfo->Size++] = '\n' ;
               pInfo->Text[pInfo->Size++] = '\n' ;
            } else {
               pInfo->Text[pInfo->Size++] = '\n' ;
               pInfo->Text[pInfo->Size++] = '\n' ;
               pInfo->Size += sprintf ( pInfo->Text+pInfo->Size, "%c%s%c", FUNCTION_START, FunctionNames[FN__CENTERJUSTIFY], FUNCTION_END ) ;
               pInfo->Size += sprintf ( pInfo->Text+pInfo->Size, "%c%s%.1lf%c", FUNCTION_START, FunctionNames[FN__SIZE], double(pInfo->ImportSize)/5, FUNCTION_END ) ;
            } /* endif */
            break;
      } /* endswitch */

   } else if ( pInfo->InBody ) {
      char *p = parser->GetDataParsed() ;
      for ( int i=0; i<strlen(p); i++ ) {
         if ( p[i] == '\n' ) {
            if ( pInfo->Size && ( pInfo->Text[pInfo->Size-1] != ' ' ) ) 
               pInfo->Text[pInfo->Size++] = ' ' ;
         } else if ( p[i] < ' ' ) {
            pInfo->Size += sprintf ( pInfo->Text+pInfo->Size, "%c%s%i%c", FUNCTION_START, FunctionNames[FN__GLYPH], p[i], FUNCTION_END ) ;
         } else {
            unsigned char Code ; char *p1 ( ConvertSymbol ( p+i, Code ) ) ;
            if ( p1 ) {
               pInfo->Text[pInfo->Size++] = (char) Code ;
               i += p1 - (p+i) - 1 ;
            } else {
               pInfo->Text[pInfo->Size++] = p[i] ;
            } /* endif */
         } /* endif */
      } /* endfor */

   } /* endif */

   return ( PARSE_GET_NEXT_TOKEN ) ;
}

extern void* APIENTRY Import ( HWND Owner, char *Name, long *Size ) {

   // Get profile data.
   Profile SetupData ( PROGRAM_NAME, WinQueryAnchorBlock(Owner), HomePath ) ;
   char ImportFont [FACESIZE] ;
   strcpy ( ImportFont, DEFAULT_FONT ) ;
   SetupData.GetString ( "ImportFont", ImportFont, sizeof(ImportFont) ) ;
   char ImportTTFont [FACESIZE] ;
   strcpy ( ImportTTFont, DEFAULT_TTFONT ) ;
   SetupData.GetString ( "ImportTTFont", ImportTTFont, sizeof(ImportTTFont) ) ;
   long ImportSize ( DEFAULT_SIZE ) ;
   SetupData.GetItem ( "ImportSize", &ImportSize, sizeof(ImportSize) ) ;

   // Try to open file file for reading.
   FILE *File = fopen ( Name, "r" ) ;
   if ( File == 0 ) {
      if ( Owner ) {
         ResourceString Message ( Library->QueryHandle(), IDS_ERROR_OPEN_LOAD ) ;
         Sys_MessageBox ( HWND_DESKTOP, Owner, PSZ(Message), ProductName(), 0, MB_ENTER | MB_ICONEXCLAMATION ) ;
      } /* endif */
      return ( 0 ) ;
   } /* endif */

   // Try to determine the file size.
   fseek ( File, 0, SEEK_END ) ;
   *Size = ftell ( File ) ;
   if ( *Size == 0 ) {
      fclose ( File ) ;
      return ( 0 ) ;
   } /* endif */

   // Allocate memory to load the entire unconverted file.
   fseek ( File, 0, SEEK_SET ) ;
   char *Text = new char [*Size] ;
   if ( Text == 0 ) {
      fclose ( File ) ;
      if ( Owner ) {
         ResourceString Message ( Library->QueryHandle(), IDS_ERROR_MEMORY_OPEN ) ;
         Sys_MessageBox ( HWND_DESKTOP, Owner, PSZ(Message), ProductName(), 0, MB_ENTER | MB_ICONEXCLAMATION ) ;
      } /* endif */
      return ( 0 ) ;
   } /* endif */

   // Read the entire unconverted file into memory.
   *Size = (long) fread ( Text, 1, (size_t)*Size, File ) ;
   fclose ( File ) ;

   // If file read failed, return an error.
   if ( *Size == 0 ) {
      delete [] Text ;
      return ( 0 ) ;
   } /* endif */

   // Scan the document to gather information about it, like its title.
   Parser parser1 ; SizeParserInfo SizeInfo ( ImportFont, ImportTTFont ) ;
   if ( parser1.Put ( Text, *Size, SizeParser, &SizeInfo ) ) {
      delete [] Text ;
      if ( Owner ) {
         ResourceString Message ( Library->QueryHandle(), IDS_ERROR_INVALID ) ;
         Sys_MessageBox ( HWND_DESKTOP, Owner, PSZ(Message), ProductName(), 0, MB_ENTER | MB_ICONEXCLAMATION ) ;
      } /* endif */
      return ( FALSE ) ;
   } /* endif */

   // Build the document preface.
   char Header [0x1000] = { 0 } ;
   sprintf ( Header+strlen(Header), "%c%s", FUNCTION_START, FunctionNames[FN__PREFACE] ) ;
   sprintf ( Header+strlen(Header), "%c%s%s%c", FUNCTION_START, FunctionNames[FN__TITLE], SizeInfo.Title, FUNCTION_END ) ;
   sprintf ( Header+strlen(Header), "%c%s%i%c", FUNCTION_START, FunctionNames[FN__CODEPAGE], 819, FUNCTION_END ) ;
   sprintf ( Header+strlen(Header), "%c%s%s%c", FUNCTION_START, FunctionNames[FN__FONT], ImportFont, FUNCTION_END ) ;
   sprintf ( Header+strlen(Header), "%c%s%.1lf%c", FUNCTION_START, FunctionNames[FN__SIZE], double(ImportSize)/10, FUNCTION_END ) ;
   sprintf ( Header+strlen(Header), "%c", FUNCTION_END ) ;

   // Allocate memory for the converted document.
   size_t EstimatedSize = strlen(Header) + SizeInfo.Size + 100 ;
   char *ConvertedText = (char*) malloc ( (size_t)EstimatedSize ) ;

   // Copy the header to the converted document.
   strcpy ( ConvertedText, Header ) ;
   long ConvertedSize = (long) strlen(ConvertedText) ;

   // Convert the body of the document.
   Parser parser2 ; ImportParserInfo ImportInfo ( ConvertedText, ConvertedSize, ImportFont, ImportTTFont, ImportSize ) ;
   if ( parser1.Put ( Text, *Size, ImportParser, &ImportInfo ) ) {
      delete [] Text ;
      if ( Owner ) {
         ResourceString Message ( Library->QueryHandle(), IDS_ERROR_INVALID ) ;
         Sys_MessageBox ( HWND_DESKTOP, Owner, PSZ(Message), ProductName(), 0, MB_ENTER | MB_ICONEXCLAMATION ) ;
      } /* endif */
      return ( FALSE ) ;
   } /* endif */

   // Release the memory holding the raw text.
   delete [] Text ;

   // Save the converted document size.
   *Size = ImportInfo.Size ;

   // Return the address of the converted text.
   return ( ConvertedText ) ;
}


/****************************************************************************
 *                                                                          *
 *  Export File                                                             *
 *                                                                          *
 ****************************************************************************/

static BOOL IsDBCSHeader ( UCHAR DBCSVector[][2], UCHAR Byte ) {
   // If no DBCS vector, return FALSE at once.
   if ( DBCSVector[0][0] == 0 )
      return ( FALSE ) ;
   // Scan the DBCS vector to see if this byte is a header.  Return TRUE if so.
   int Block = 0 ;
   while ( DBCSVector[Block][0] ) {
      if ( Byte >= DBCSVector[Block][0] )
         if ( Byte <= DBCSVector[Block][1] )
            return ( TRUE ) ;
      Block ++ ;
   } /* endwhile */
   // Return FALSE.
   return ( FALSE ) ;
}

static BOOL FileWrite ( HWND Owner, FILE *File, char *Text, int Length ) {
   if ( fwrite ( Text, 1, Length, File ) != Length ) {
      fclose ( File ) ;
      if ( Owner ) {
         ResourceString Message ( Library->QueryHandle(), IDS_ERROR_WRITE ) ;
         Sys_MessageBox ( HWND_DESKTOP, Owner, PSZ(Message), ProductName(), 0, MB_ENTER | MB_ICONEXCLAMATION ) ;
      } /* endif */
      return ( FALSE ) ;
   } /* endif */
   return ( TRUE ) ;
}

static BOOL FileWrite ( HWND Owner, FILE *File, char *Text ) {
   return ( FileWrite ( Owner, File, Text, strlen(Text) ) ) ;
}

static BOOL FlushWord ( HWND Owner, FILE *File, char Word[], int &Column ) {
   if ( Word[0] && !FileWrite ( Owner, File, Word ) )
      return ( FALSE ) ;
   Column += strlen(Word) ;
   Word[0] = 0 ;
   return ( TRUE ) ;
}

static BOOL NewLine ( HWND Owner, FILE *File, int &Column ) {
   if ( !FileWrite ( Owner, File, "\n" ) )
      return ( FALSE ) ;
   Column = 0 ;
   return ( TRUE ) ;
}

static BOOL WriteTag ( HWND Owner, FILE *File, char Word[], int &Column, char *Tag ) {
   if ( !FlushWord ( Owner, File, Word, Column ) )
      return ( FALSE ) ;
   if ( Column+strlen(Tag) > 80 )
      if ( !NewLine ( Owner, File, Column ) )
         return ( FALSE ) ;
   if ( !FileWrite ( Owner, File, Tag ) )
      return ( FALSE ) ;
   Column += strlen(Tag) ;
   return ( TRUE ) ;
}

extern BOOL APIENTRY Export ( HWND Owner, char *Name, void *Text, long Size ) {

   // Get the initial value of the DBCS vector.
   COUNTRYCODE Country = { 0, 0 } ;
   UCHAR DBCSVector [6] [2] = { { 0 } } ;
   DosQueryDBCSEnv ( sizeof(DBCSVector), &Country, PCHAR(DBCSVector) ) ;

   // Try to open the file for writing.  Return an error if not successful.
   FILE *File = fopen ( Name, "w" ) ;
   if ( File == 0 ) {
      if ( Owner )
         Sys_MessageBox ( HWND_DESKTOP, Owner, "ERROR: Unable to create || open file for saving.", ProductName(), 0, MB_ENTER | MB_ICONEXCLAMATION ) ;
      return ( FALSE ) ;
   } /* endif */

   // Write the document header here.
   if ( !FileWrite ( Owner, File, "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 3.2//EN\">\n" ) )
      return ( FALSE ) ;
   if ( !FileWrite ( Owner, File, "<HTML>\n" ) )
      return ( FALSE ) ;
   if ( !FileWrite ( Owner, File, "<HEAD>\n" ) )
      return ( FALSE ) ;

   // Write the document itself here.
   int Nesting(0), Column(0), InPreface(0), Bold(0), Italic(0), Underscore(0), Strikeout(0) ;
   char Word [200] = { 0 } ;
   for ( int i=0; i<Size; i++ ) {

      // If function escape, skip onwards.  This should never happen at nesting level 0.
      if ( ( PCHAR(Text)[i] == FUNCTION_ESCAPE ) && ( i < Size-2 ) ) {
         if ( ( PCHAR(Text)[i+1] == FUNCTION_ESCAPE ) || ( PCHAR(Text)[i+1] == FUNCTION_START ) || ( PCHAR(Text)[i+1] == FUNCTION_END ) ) {
            i ++ ;
         } /* endif */

      // Else, if entering a function, note the change in nesting.  Interpret the function always.
      } else if ( PCHAR(Text)[i] == FUNCTION_START ) {
         Nesting ++ ;
         for ( int Type=0; Type<MAX_FUNCTIONS; Type++ )
            if ( !strnicmp ( PCHAR(Text)+i+1, FunctionNames[Type], strlen(FunctionNames[Type]) ) )
               break ;
         char *p ( PCHAR(Text)+i+1+strlen(FunctionNames[Type]) ) ;
         switch ( Type ) {
            case FN__PREFACE: {
               InPreface = TRUE ;
               break; }
            case FN__SEPARATOR:
               if ( !FlushWord ( Owner, File, Word, Column ) )
                  return ( FALSE ) ;
               if ( !FileWrite ( Owner, File, "<HR>\n" ) )
                  return ( FALSE ) ;
               Column = 0 ;
               break;
            case FN__TITLE: {
               char Title [80] = { 0 } ; int Len ( 0 ) ;
               while ( ( Len < sizeof(Title)-1 ) && ( *p != FUNCTION_END ) )
                  Title[Len++] = *p++ ;
               if ( !FileWrite ( Owner, File, "<TITLE>" ) )
                  return ( FALSE ) ;
               if ( !FileWrite ( Owner, File, Title ) )
                  return ( FALSE ) ;
               if ( !FileWrite ( Owner, File, "</TITLE>\n" ) )
                  return ( FALSE ) ;
               break; }
            case FN__COMMENT: {
               char Comment [80] = { 0 } ; int Len ( 0 ) ;
               while ( ( Len < sizeof(Comment)-1 ) && ( *p != FUNCTION_END ) )
                  Comment[Len++] = *p++ ;
               if ( !FileWrite ( Owner, File, "\n<!--" ) )
                  return ( FALSE ) ;
               if ( !FileWrite ( Owner, File, Comment ) )
                  return ( FALSE ) ;
               if ( !FileWrite ( Owner, File, "-->\n" ) )
                  return ( FALSE ) ;
               break; }
            case FN__BOLD: {
               if ( !FlushWord ( Owner, File, Word, Column ) )
                  return ( FALSE ) ;
               if ( *p == '1' ) {
                  if ( !Bold && !FileWrite ( Owner, File, "<B>" ) )
                     return ( FALSE ) ;
                  Bold = TRUE ;
               } else {
                  if ( Bold && !FileWrite ( Owner, File, "</B>" ) )
                     return ( FALSE ) ;
                  Bold = FALSE ;
               } /* endif */
               break; }
            case FN__ITALIC: {
               if ( !FlushWord ( Owner, File, Word, Column ) )
                  return ( FALSE ) ;
               if ( *p == '1' ) {
                  if ( !Italic && !FileWrite ( Owner, File, "<I>" ) )
                     return ( FALSE ) ;
                  Italic = TRUE ;
               } else {
                  if ( Italic && !FileWrite ( Owner, File, "</I>" ) )
                     return ( FALSE ) ;
                  Italic = FALSE ;
               } /* endif */
               break; }
            case FN__UNDERSCORE: {
               if ( !FlushWord ( Owner, File, Word, Column ) )
                  return ( FALSE ) ;
               if ( *p == '1' ) {
                  if ( !Underscore && !FileWrite ( Owner, File, "<U>" ) )
                     return ( FALSE ) ;
                  Underscore = TRUE ;
               } else {
                  if ( Underscore && !FileWrite ( Owner, File, "</U>" ) )
                     return ( FALSE ) ;
                  Underscore = FALSE ;
               } /* endif */
               break; }
            case FN__STRIKEOUT: {
               if ( !FlushWord ( Owner, File, Word, Column ) )
                  return ( FALSE ) ;
               if ( *p == '1' ) {
                  if ( !Strikeout && !FileWrite ( Owner, File, "<S>" ) )
                     return ( FALSE ) ;
                  Strikeout = TRUE ;
               } else {
                  if ( Strikeout && !FileWrite ( Owner, File, "</S>" ) )
                     return ( FALSE ) ;
                  Strikeout = FALSE ;
               } /* endif */
               break; }
            case FN__GLYPH: {
               unsigned char Character = UCHAR(atoi(PCHAR(Text)+i+1+strlen(FunctionNames[Type]))) ;
               if ( Character > 0x7F ) {
                  char Tag [10] ;
                  sprintf ( Tag, "&#%02i;", Character ) ;
                  if ( !WriteTag ( Owner, File, Word, Column, Tag ) )
                     return ( FALSE ) ;
               } else {
                  int Offset = strlen(Word) ;
                  Word[Offset+0] = char ( Character & 0xFF ) ;
                  Word[Offset+1] = 0 ;
               } /* endif */
               break; }
            default: {
               // Ignore all other codes, for the moment.
               break; }
         } /* endswitch */

      // Else, if exiting a function, note the change in nesting.
      } else if ( PCHAR(Text)[i] == FUNCTION_END ) {
         Nesting -- ;
         if ( ( Nesting == 0 ) && InPreface ) {
            InPreface = FALSE ;
            if ( !FileWrite ( Owner, File, "</HEAD>\n" ) )
               return ( FALSE ) ;
            if ( !FileWrite ( Owner, File, "<BODY>\n" ) )
               return ( FALSE ) ;
         } /* endif */

      // Else, if at nesting level zero, we have literal text to output.
      } else if ( Nesting == 0 ) {

         USHORT Character = PUCHAR(Text) [i] ;
         int CharSize = 1 ;

         // If end-of-document . . .
         if ( Character == ENDFILE ) {
            if ( !FlushWord ( Owner, File, Word, Column ) )
               return ( FALSE ) ;
            Column = 0 ;

         // Else, if end-of-page . . .
         } else if ( Character == '\f' ) {
            if ( !FlushWord ( Owner, File, Word, Column ) )
               return ( FALSE ) ;
            if ( !FileWrite ( Owner, File, "<HR>\n" ) )
               return ( FALSE ) ;
            Column = 0 ;

         // Else, if end-of-line . . .
         } else if ( Character == '\n' ) {
            if ( !FlushWord ( Owner, File, Word, Column ) )
               return ( FALSE ) ;
            if ( !FileWrite ( Owner, File, "<BR>\n" ) )
               return ( FALSE ) ;
            Column = 0 ;

         // Else, if space . . .
         } else if ( Character == ' ' ) {
            if ( !FlushWord ( Owner, File, Word, Column ) )
               return ( FALSE ) ;
            if ( Column+1 > 80 ) {
               if ( !NewLine ( Owner, File, Column ) )
                  return ( FALSE ) ;
            } else {
               if ( !FileWrite ( Owner, File, " " ) )
                  return ( FALSE ) ;
               Column ++ ;
            } /* endif */

         // Else, if less-than character . . .
         } else if ( Character == '<' ) {
            if ( !WriteTag ( Owner, File, Word, Column, "&lt;" ) )
               return ( FALSE ) ;

         // Else, if greater-than character . . .
         } else if ( Character == '>' ) {
            if ( !WriteTag ( Owner, File, Word, Column, "&gt;" ) )
               return ( FALSE ) ;

         // Else, if ampersand . . .
         } else if ( Character == '&' ) {
            if ( !WriteTag ( Owner, File, Word, Column, "&amp;" ) )
               return ( FALSE ) ;

         // Else, if double quotes . . .
         } else if ( Character == '\"' ) {
            if ( !WriteTag ( Owner, File, Word, Column, "&quot;" ) )
               return ( FALSE ) ;

         // Else, if eight-bit character . . .
         } else if ( Character > 0x7F ) {
            char Tag [10] ;
            sprintf ( Tag, "&#%02i;", Character ) ;
            if ( !WriteTag ( Owner, File, Word, Column, Tag ) )
               return ( FALSE ) ;

         // Otherwise normal character.
         } else {
            int Offset = strlen(Word) ;
            if ( IsDBCSHeader ( DBCSVector, UCHAR(Character) ) ) {
               Character = USHORT ( Character | USHORT ( PCHAR(Text)[++i] << 8 ) ) ;
               CharSize ++ ;
               Word[Offset+0] = char ( Character & 0xFF ) ;
               Word[Offset+1] = char ( Character >> 8 ) ;
               Word[Offset+2] = 0 ;
            } else {
               Word[Offset+0] = char ( Character & 0xFF ) ;
               Word[Offset+1] = 0 ;
            } /* endif */
            if ( Column && ( Column + strlen(Word) > 80 ) )
               if ( !NewLine ( Owner, File, Column ) )
                  return ( FALSE ) ;

         } /* endif */

      } /* endif */

   } /* endfor */

   // If a word remains to be flushed, do so now.
   if ( !FlushWord ( Owner, File, Word, Column ) )
      return ( FALSE ) ;

   // Output the trailer now.
   if ( !NewLine ( Owner, File, Column ) )
      return ( FALSE ) ;
   if ( !FileWrite ( Owner, File, "</BODY>\n" ) )
      return ( FALSE ) ;
   if ( !FileWrite ( Owner, File, "</HTML>\n" ) )
      return ( FALSE ) ;

   // Build the file's extended attributes.
   static MVMT_VALUE Types [] = {
      { EAT_ASCII,  4, "HTML"       },
      { EAT_ASCII, 10, "Plain Text" },
   } ;
   static EADATA Table [] = {
      { PSZ(".TYPE"),     EAT_MVMT,  0, 0 },     // Type length & value need filling in.
   } ;
   Table[0].Length = USHORT ( BuildMVMTValue ( 0, sizeof(Types)/sizeof(Types[0]), Types ) ) ;
   Table[0].Value = malloc ( Table[0].Length ) ;
   BuildMVMTValue ( Table[0].Value, sizeof(Types)/sizeof(Types[0]), Types ) ;
   PEAOP2 pExtendedAttributes = BuildExtendedAttributes ( sizeof(Table)/sizeof(Table[0]), Table ) ;

   // If the extended attributes got built successfully, then attach them to the file.
   if ( pExtendedAttributes ) {
      DosSetFileInfo ( ULONG(fileno(File)), 2, PUCHAR(pExtendedAttributes), sizeof(*pExtendedAttributes) ) ;
      free ( pExtendedAttributes->fpFEA2List ) ;
      free ( pExtendedAttributes ) ;
   } /* endif */

   // Free the memory allocated for the MVMT value.
   free ( Table[0].Value ) ;

   // Close the file.
   fclose ( File ) ;

   // Return no error.
   return ( TRUE ) ;
}


