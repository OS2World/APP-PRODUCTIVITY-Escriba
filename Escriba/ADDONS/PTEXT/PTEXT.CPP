/****************************************************************** PTEXT.CPP
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

#include "PText.h"

// #define DEBUG

/****************************************************************************
 *                                                                          *
 *                       Definitions & Declarations                         *
 *                                                                          *
 ****************************************************************************/

  // Macros & Constants 

#define PROGRAM_NAME            "PTEXT"
#define LANGUAGE_NAME           "PTXT_*.DLL"

#define OUTPUT_FONT             "Courier"
#define OUTPUT_PITCH            (100)
#define OUTPUT_HEIGHT           (55)
#define OUTPUT_HEIGHT_MIN       (40)
#define OUTPUT_HEIGHT_MAX       (80)
#define OUTPUT_WIDTH            (80)
#define OUTPUT_WIDTH_MIN        (40)
#define OUTPUT_WIDTH_MAX        (160)


  // Function Prototypes 

extern "C" {
   extern int _CRT_init ( void ) ;
}


  // Globals (private to the library, created by library instance) 

extern char HomePath [_MAX_PATH+1] = { 0 } ;
static class LanguageModule *Library ( 0 ) ;
static HMODULE LibraryHandle ( 0 ) ;


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
   char Font [FACESIZE] ;
   long Size ;
   long Height ;
   long Width ;
   BOOL ImportMetric ;
   BOOL ImportSoftbreaks ;
   BOOL ExportFormat ;
   BOOL ExportBold ;
   BOOL ExportUnderscore ;
   BOOL ExportStrikeout ;
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
         PCONFIGUREPARMS Parms = PCONFIGUREPARMS ( malloc ( sizeof(CONFIGUREPARMS) ) ) ;
         Sys_SetWindowData ( Window, Parms ) ;

         // Associate help instance with the dialog.
         char Drive [_MAX_DRIVE+1], Dir[_MAX_DIR+1], Fname[_MAX_FNAME+1], Ext[_MAX_EXT+1] ;
         _splitpath ( Library->QueryName(), Drive, Dir, Fname, Ext ) ;
         char HelpFileName [CCHMAXPATH] ;
         sprintf ( HelpFileName, "%s\\%s.hlp", HomePath, Fname ) ;
         ResourceString HelpTitle ( Library->QueryHandle(), IDS_HELPTITLE ) ;
         Parms->Help = new HelpWindow ( 0, LibraryHandle, ID_HELPTABLE, PSZ(HelpFileName), PSZ(HelpTitle) ) ;
         Parms->Help->Associate ( Window ) ;

         char MetricFlag [2] = { '1', 0 } ;
         ULONG ItemSize = sizeof(MetricFlag) ;
         PrfQueryProfileData ( HINI_USERPROFILE, PSZ("PM_National"), PSZ("iMeasurement"), MetricFlag, &ItemSize ) ;

         // Set up the current state.
         Parms->ErrorField = *phwndErrorField ;
         strcpy ( Parms->Font, OUTPUT_FONT ) ;
         Parms->Size = OUTPUT_PITCH ;
         Parms->ImportMetric = ( MetricFlag [0] == '1' ) ? FALSE : TRUE ;
         Parms->ImportSoftbreaks = FALSE ;
         Profile SetupData ( PROGRAM_NAME, WinQueryAnchorBlock(Window), HomePath ) ;
         SetupData.GetString ( "Font", Parms->Font, sizeof(Parms->Font) ) ;
         SetupData.GetItem ( "Size", &Parms->Size, sizeof(Parms->Size) ) ;
         SetupData.GetItem ( "ImportMetric", &Parms->ImportMetric, sizeof(Parms->ImportMetric) ) ;
         SetupData.GetItem ( "ImportSoftbreaks", &Parms->ImportSoftbreaks, sizeof(Parms->ImportSoftbreaks) ) ;

         // Build the font list.
         Canvas MyCanvas ( "PTEXT::ImportPageProcessor::MyCanvas", Window ) ;
         int FontCount = MyCanvas.QueryFontCount ( ) ;
         PFONTMETRICS pfm = PFONTMETRICS ( malloc ( size_t ( FontCount * sizeof(FONTMETRICS) ) ) ) ;
         MyCanvas.QueryFonts ( FontCount, pfm ) ;
         qsort ( pfm, (size_t)FontCount, sizeof(FONTMETRICS), Compare_FONTMETRICS ) ;
         for ( int i=0, j=0; i<FontCount; i++ ) {
            if ( ( pfm[i].fsDefn & FM_DEFN_OUTLINE ) && ( pfm[i].fsType & FM_TYPE_FIXED ) ) {
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
         for ( i=0; i<FontCount; i++ ) {
            Sys_SendMessage ( CHILD(Window,IDD_CONFIGURE_FONT), LM_INSERTITEM,
               MPFROMSHORT ( LIT_SORTASCENDING ), MPFROMP ( pfm[i].szFacename ) ) ;
         } /* endfor */
         Sys_SetWindowText ( CHILD(Window,IDD_CONFIGURE_FONT), PSZ(Parms->Font) ) ;
         for ( i=0; i<FontCount; i++ ) {
            UCHAR Font [FACESIZE] ;
            Sys_SendMessage ( CHILD(Window,IDD_CONFIGURE_FONT), LM_QUERYITEMTEXT,
               MPFROM2SHORT ( i, sizeof(Font) ), MPFROMP ( Font ) ) ;
            if ( !strcmpi ( PCHAR(Parms->Font), PCHAR(Font) ) ) {
               Sys_SendMessage ( CHILD(Window,IDD_CONFIGURE_FONT), LM_SELECTITEM,
                  MPFROMSHORT(i), MPFROMSHORT(TRUE) ) ;
               break ;
            } /* endif */
         } /* endfor */
         free ( pfm ) ;

         // Build the size list.
         for ( i=0; i<sizeof(Sizes)/sizeof(Sizes[0]); i++ ) {
            ResourceString Pts ( Library->QueryHandle(), IDS_PTS ) ;
            char Text [10] ;
            sprintf ( Text, "%.1lf%s", (double)Sizes[i]/10.0, PSZ(Pts) ) ;
            Sys_SendMessage ( CHILD(Window,IDD_CONFIGURE_SIZE), LM_INSERTITEM,
               MPFROMSHORT ( LIT_END ), MPFROMP ( Text ) ) ;
            if ( Sizes[i] == Parms->Size ) {
               Sys_SetWindowText ( CHILD(Window,IDD_CONFIGURE_SIZE), PSZ(Text) ) ;
               Sys_SendMessage ( CHILD(Window,IDD_CONFIGURE_SIZE), LM_SELECTITEM, MPFROMSHORT(i), MPFROMSHORT(TRUE) ) ;
            } /* endif */
         } /* endfor */

         // Load the checkboxes.
         Sys_SendMessage ( CHILD(Window,IDD_CONFIGURE_METRIC),     BM_SETCHECK, MPFROMSHORT(Parms->ImportMetric),     0 ) ;
         Sys_SendMessage ( CHILD(Window,IDD_CONFIGURE_SOFTBREAKS), BM_SETCHECK, MPFROMSHORT(Parms->ImportSoftbreaks), 0 ) ;

         return ( 0 ) ; }

      case WM_DESTROY: {
         PCONFIGUREPARMS Parms = PCONFIGUREPARMS ( Sys_GetWindowData(Window) ) ;
         delete Parms->Help ;
         free ( Parms ) ;
         break ; }

      case WM_CONTROL: {
         PCONFIGUREPARMS Parms = PCONFIGUREPARMS ( Sys_GetWindowData(Window) ) ;
//       SHORT Id = SHORT1FROMMP ( mp1 ) ;
         SHORT Message = SHORT2FROMMP ( mp1 ) ;
         switch ( Message ) {

            case BKN_PAGESELECTEDPENDING: {
//             PAGESELECTNOTIFY *pInfo = (PAGESELECTNOTIFY*) PVOIDFROMMP(mp2) ;

               // Clear the error field.
               Sys_SetWindowText ( Parms->ErrorField, "" ) ;

               // Get the new font.
               char Face [FACESIZE] ;
               Sys_GetWindowText ( CHILD(Window,IDD_CONFIGURE_FONT), Face, sizeof(Face) ) ;
               memcpy ( Parms->Font, Face, sizeof(Parms->Font) ) ;

               // Get the new size.
               SHORT Index = SHORT1FROMMR ( Sys_SendMessage ( CHILD(Window,IDD_CONFIGURE_SIZE), 
                  LM_QUERYSELECTION, MPFROMSHORT(LIT_FIRST), 0 ) ) ;
               Parms->Size = ( Index == LIT_NONE ) ? 120 : Sizes[Index] ;

               // Get checkbox values.
               Parms->ImportMetric = SHORT1FROMMR ( Sys_SendMessage ( CHILD(Window,IDD_CONFIGURE_METRIC), BM_QUERYCHECK, 0, 0 ) ) ;
               Parms->ImportSoftbreaks = SHORT1FROMMR ( Sys_SendMessage ( CHILD(Window,IDD_CONFIGURE_SOFTBREAKS), BM_QUERYCHECK, 0, 0 ) ) ;

               // Save the new information in PTEXT.INI.
               Profile SetupData ( PROGRAM_NAME, WinQueryAnchorBlock(Window), HomePath ) ;
               SetupData.PutString ( "Font", Parms->Font ) ;
               SetupData.PutItem ( "Size", &Parms->Size, sizeof(Parms->Size) ) ;
               SetupData.PutItem ( "ImportMetric", &Parms->ImportMetric, sizeof(Parms->ImportMetric) ) ;
               SetupData.PutItem ( "ImportSoftbreaks", &Parms->ImportSoftbreaks, sizeof(Parms->ImportSoftbreaks) ) ;

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
         PCONFIGUREPARMS Parms = PCONFIGUREPARMS ( Sys_GetWindowData(Window) ) ;
         Sys_SendMessage ( Parms->Help->QueryHandle(), HM_EXT_HELP, 0, 0 ) ;
         return ( 0 ) ; }

   } /* endswitch */

   return ( Sys_DefDialogProc ( Window, msg, mp1, mp2 ) ) ;
}

static MRESULT EXPENTRY ExportPageProcessor ( HWND Window, ULONG msg, MPARAM mp1, MPARAM mp2 ) {

   switch ( msg ) {

      case WM_INITDLG: {
         PHWND phwndErrorField = PHWND ( PVOIDFROMMP ( mp2 ) ) ;
         PCONFIGUREPARMS Parms = PCONFIGUREPARMS ( malloc ( sizeof(CONFIGUREPARMS) ) ) ;
         Sys_SetWindowData ( Window, Parms ) ;

         // Associate help instance with the dialog.
         char Drive [_MAX_DRIVE+1], Dir[_MAX_DIR+1], Fname[_MAX_FNAME+1], Ext[_MAX_EXT+1] ;
         _splitpath ( Library->QueryName(), Drive, Dir, Fname, Ext ) ;
         char HelpFileName [CCHMAXPATH] ;
         sprintf ( HelpFileName, "%s\\%s.hlp", HomePath, Fname ) ;
         ResourceString HelpTitle ( Library->QueryHandle(), IDS_HELPTITLE ) ;
         Parms->Help = new HelpWindow ( 0, LibraryHandle, ID_HELPTABLE, PSZ(HelpFileName), PSZ(HelpTitle) ) ;
         Parms->Help->Associate ( Window ) ;

         char MetricFlag [2] = { '1', 0 } ;
         ULONG ItemSize = sizeof(MetricFlag) ;
         PrfQueryProfileData ( HINI_USERPROFILE, PSZ("PM_National"), PSZ("iMeasurement"), MetricFlag, &ItemSize ) ;

         // Set up the current state.
         Parms->ErrorField = *phwndErrorField ;
         Parms->Height = OUTPUT_HEIGHT ;
         Parms->Width = OUTPUT_WIDTH ;
         Parms->ExportFormat = TRUE ;
         Parms->ExportBold = FALSE ;
         Parms->ExportUnderscore = FALSE ;
         Parms->ExportStrikeout = FALSE ;
         Profile SetupData ( PROGRAM_NAME, WinQueryAnchorBlock(Window), HomePath ) ;
         SetupData.GetItem ( "Height", &Parms->Height, sizeof(Parms->Height) ) ;
         SetupData.GetItem ( "Width", &Parms->Width, sizeof(Parms->Width) ) ;
         SetupData.GetItem ( "ExportFormat", &Parms->ExportFormat, sizeof(Parms->ExportFormat) ) ;
         SetupData.GetItem ( "ExportBold", &Parms->ExportBold, sizeof(Parms->ExportBold) ) ;
         SetupData.GetItem ( "ExportUnderscore", &Parms->ExportUnderscore, sizeof(Parms->ExportUnderscore) ) ;
         SetupData.GetItem ( "ExportStrikeout", &Parms->ExportStrikeout, sizeof(Parms->ExportStrikeout) ) ;

         // Set the height.
         Sys_SendMessage ( CHILD(Window,IDD_CONFIGURE_HEIGHT),
            SPBM_SETLIMITS, (MPARAM)OUTPUT_HEIGHT_MAX, (MPARAM)OUTPUT_HEIGHT_MIN ) ;
         Sys_SendMessage ( CHILD(Window,IDD_CONFIGURE_HEIGHT),
            SPBM_SETCURRENTVALUE, (MPARAM)(Parms->Height), 0 ) ;

         // Set the width.
         Sys_SendMessage ( CHILD(Window,IDD_CONFIGURE_WIDTH),
            SPBM_SETLIMITS, (MPARAM)OUTPUT_WIDTH_MAX, (MPARAM)OUTPUT_WIDTH_MIN ) ;
         Sys_SendMessage ( CHILD(Window,IDD_CONFIGURE_WIDTH),
            SPBM_SETCURRENTVALUE, (MPARAM)(Parms->Width), 0 ) ;

         // Load the checkboxes.
         Sys_SendMessage ( CHILD(Window,IDD_CONFIGURE_FORMAT),     BM_SETCHECK, MPFROMSHORT(Parms->ExportFormat),     0 ) ;
         Sys_SendMessage ( CHILD(Window,IDD_CONFIGURE_BOLD),       BM_SETCHECK, MPFROMSHORT(Parms->ExportBold),       0 ) ;
         Sys_SendMessage ( CHILD(Window,IDD_CONFIGURE_UNDERSCORE), BM_SETCHECK, MPFROMSHORT(Parms->ExportUnderscore), 0 ) ;
         Sys_SendMessage ( CHILD(Window,IDD_CONFIGURE_STRIKEOUT),  BM_SETCHECK, MPFROMSHORT(Parms->ExportStrikeout),  0 ) ;

         return ( 0 ) ; }

      case WM_DESTROY: {
         PCONFIGUREPARMS Parms = PCONFIGUREPARMS ( Sys_GetWindowData(Window) ) ;
         delete Parms->Help ;
         free ( Parms ) ;
         break ; }

      case WM_CONTROL: {
         PCONFIGUREPARMS Parms = PCONFIGUREPARMS ( Sys_GetWindowData(Window) ) ;
//       SHORT Id = SHORT1FROMMP ( mp1 ) ;
         SHORT Message = SHORT2FROMMP ( mp1 ) ;
         switch ( Message ) {

            case BKN_PAGESELECTEDPENDING: {
               PAGESELECTNOTIFY *pInfo = (PAGESELECTNOTIFY*) PVOIDFROMMP(mp2) ;

               // Clear the error field.
               Sys_SetWindowText ( Parms->ErrorField, "" ) ;

               // Get the new height.
               Sys_SendMessage ( CHILD(Window,IDD_CONFIGURE_HEIGHT), SPBM_QUERYVALUE,
                  &Parms->Height, MPFROM2SHORT(0,SPBQ_UPDATEIFVALID) ) ;
               if ( ( Parms->Height < OUTPUT_HEIGHT_MIN ) || ( Parms->Height > OUTPUT_HEIGHT_MAX ) ) {
                  ResourceString Error_InvalidHeight ( Library->QueryHandle(), IDS_ERROR_INVALIDHEIGHT ) ;
                  Sys_SetWindowText ( Parms->ErrorField, PSZ(Error_InvalidHeight) ) ;
                  Sys_BeepWarning ( ) ;
                  Sys_SetFocus ( CHILD(Window,IDD_CONFIGURE_HEIGHT) ) ;
                  pInfo->ulPageIdNew = 0 ;
                  break ;
               } /* endif */

               // Get the new width.
               Sys_SendMessage ( CHILD(Window,IDD_CONFIGURE_WIDTH), SPBM_QUERYVALUE,
                  &Parms->Width, MPFROM2SHORT(0,SPBQ_UPDATEIFVALID) ) ;
               if ( ( Parms->Width < OUTPUT_WIDTH_MIN ) || ( Parms->Width > OUTPUT_WIDTH_MAX ) ) {
                  ResourceString Error_InvalidWidth ( Library->QueryHandle(), IDS_ERROR_INVALIDWIDTH ) ;
                  Sys_SetWindowText ( Parms->ErrorField, PSZ(Error_InvalidWidth) ) ;
                  Sys_BeepWarning ( ) ;
                  Sys_SetFocus ( CHILD(Window,IDD_CONFIGURE_WIDTH) ) ;
                  pInfo->ulPageIdNew = 0 ;
                  break ;
               } /* endif */

               // Get checkbox values.
               Parms->ExportFormat = SHORT1FROMMR ( Sys_SendMessage ( CHILD(Window,IDD_CONFIGURE_FORMAT), BM_QUERYCHECK, 0, 0 ) ) ;
               Parms->ExportBold = SHORT1FROMMR ( Sys_SendMessage ( CHILD(Window,IDD_CONFIGURE_BOLD), BM_QUERYCHECK, 0, 0 ) ) ;
               Parms->ExportUnderscore = SHORT1FROMMR ( Sys_SendMessage ( CHILD(Window,IDD_CONFIGURE_UNDERSCORE), BM_QUERYCHECK, 0, 0 ) ) ;
               Parms->ExportStrikeout = SHORT1FROMMR ( Sys_SendMessage ( CHILD(Window,IDD_CONFIGURE_STRIKEOUT), BM_QUERYCHECK, 0, 0 ) ) ;

               // Save the new information in PTEXT.INI.
               Profile SetupData ( PROGRAM_NAME, WinQueryAnchorBlock(Window), HomePath ) ;
               SetupData.PutItem ( "Height", &Parms->Height, sizeof(Parms->Height) ) ;
               SetupData.PutItem ( "Width", &Parms->Width, sizeof(Parms->Width) ) ;
               SetupData.PutItem ( "ExportFormat", &Parms->ExportFormat, sizeof(Parms->ExportFormat) ) ;
               SetupData.PutItem ( "ExportBold", &Parms->ExportBold, sizeof(Parms->ExportBold) ) ;
               SetupData.PutItem ( "ExportUnderscore", &Parms->ExportUnderscore, sizeof(Parms->ExportUnderscore) ) ;
               SetupData.PutItem ( "ExportStrikeout", &Parms->ExportStrikeout, sizeof(Parms->ExportStrikeout) ) ;

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
         PCONFIGUREPARMS Parms = PCONFIGUREPARMS ( Sys_GetWindowData(Window) ) ;
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
   return ( "Plain Text" ) ;
}


/****************************************************************************
 *                                                                          *
 *  Query Filename Wildcard                                                 *
 *                                                                          *
 ****************************************************************************/

extern char* APIENTRY QueryWildcard ( void ) {
   return ( "*.TXT,*.DOC,*.ME,*.BAT,*.CMD,*.MOI,*.MAN" ) ;
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

   // Match against *.TXT.
   if ( MatchWildcard ( Name, "*.TXT" ) ) 
      return ( TRUE ) ;

   // Match against *.DOC.
   if ( MatchWildcard ( Name, "*.DOC" ) )
      return ( TRUE ) ;

   // Match against *.ME.
   if ( MatchWildcard ( Name, "*.ME" ) )
      return ( TRUE ) ;

   // Match against *.BAT.
   if ( MatchWildcard ( Name, "*.BAT" ) )
      return ( TRUE ) ;

   // Match against *.CMD.
   if ( MatchWildcard ( Name, "*.CMD" ) )
      return ( TRUE ) ;

   // Match against *.MOI.
   if ( MatchWildcard ( Name, "*.MOI" ) )
      return ( TRUE ) ;

   // Match against *.MAN.
   if ( MatchWildcard ( Name, "*.MAN" ) )
      return ( TRUE ) ;

   // Otherwise, return FALSE.
   return ( FALSE ) ;
}


/****************************************************************************
 *                                                                          *
 *  Verify File by Inspection                                               *
 *                                                                          *
 *  Files of the 'Plain Text' type cannot contain imbedded nulls.  Other    *
 *  than that, anything goes.                                               *
 *                                                                          *
 ****************************************************************************/

extern BOOL APIENTRY Verify ( HWND Owner, char *Name ) {

   FILE *File = fopen ( Name, "rb" ) ;
   if ( File == 0 ) {
      if ( Owner ) {
         ResourceString Message ( Library->QueryHandle(), IDS_ERROR_OPEN_VERIFY ) ;
         Sys_MessageBox ( HWND_DESKTOP, Owner, PSZ(Message), ProductName(), 0, MB_ENTER | MB_ICONEXCLAMATION ) ;
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
   char *Text = (char*) malloc ( Size ) ;
   if ( Text == 0 ) {
      fclose ( File ) ;
      if ( Owner ) {
         ResourceString Message ( Library->QueryHandle(), IDS_ERROR_MEMORY_VERIFY ) ;
         Sys_MessageBox ( HWND_DESKTOP, Owner, PSZ(Message), ProductName(), 0, MB_ENTER | MB_ICONEXCLAMATION ) ;
      } /* endif */
      return ( FALSE ) ;
   } /* endif */

   Size = fread ( Text, 1, Size, File ) ;
   if ( Size == 0 ) {
      free ( Text ) ;
      fclose ( File ) ;
      return ( TRUE ) ;
   } /* endif */

   int HasNulls = FALSE ;
   for ( int i=0; i<Size; i++ ) {
      if ( Text[i] == 0 ) 
         HasNulls = TRUE ;
      else if ( HasNulls ) {
         free ( Text ) ;
         fclose ( File ) ;
         if ( Owner ) {
            ResourceString Message ( Library->QueryHandle(), IDS_ERROR_HAS_NULLS ) ;
            int rc = Sys_MessageBox ( HWND_DESKTOP, Owner, PSZ(Message), ProductName(), 0, MB_YESNO | MB_QUERY | MB_DEFBUTTON2 ) ;
            if ( rc == MBID_YES ) 
               return ( TRUE ) ;
         } /* endif */
         return ( FALSE ) ;
      } /* endif */
   } /* endfor */

   free ( Text ) ;
   fclose ( File ) ;
   return ( TRUE ) ;
}


/****************************************************************************
 *                                                                          *
 *  Import File                                                             *
 *                                                                          *
 ****************************************************************************/

extern void* APIENTRY Import ( HWND Owner, char *Name, long *Size ) {

   FILE *File = fopen ( Name, "rb" ) ;
   if ( File == 0 ) {
      if ( Owner ) {
         ResourceString Message ( Library->QueryHandle(), IDS_ERROR_OPEN_LOAD ) ;
         Sys_MessageBox ( HWND_DESKTOP, Owner, PSZ(Message), ProductName(), 0, MB_ENTER | MB_ICONEXCLAMATION ) ;
      } /* endif */
      return ( 0 ) ;
   } /* endif */

   fseek ( File, 0, SEEK_END ) ;
   *Size = ftell ( File ) ;
   if ( *Size == 0 ) {
      fclose ( File ) ;
      return ( 0 ) ;
   } /* endif */

   fseek ( File, 0, SEEK_SET ) ;
   char *Text = (char*) malloc ( (size_t)*Size ) ;
   if ( Text == 0 ) {
      fclose ( File ) ;
      if ( Owner ) {
         ResourceString Message ( Library->QueryHandle(), IDS_ERROR_MEMORY_OPEN ) ;
         Sys_MessageBox ( HWND_DESKTOP, Owner, PSZ(Message), ProductName(), 0, MB_ENTER | MB_ICONEXCLAMATION ) ;
      } /* endif */
      return ( 0 ) ;
   } /* endif */

   *Size = (long) fread ( Text, 1, (size_t)*Size, File ) ;
   fclose ( File ) ;

   if ( *Size == 0 ) {
      free ( Text ) ;
      return ( 0 ) ;
   } /* endif */

   BOOL WasBold = FALSE, WasUnderscored = FALSE, WasStruckout = FALSE ;
   int GlyphCount = 0 ;
   int Underscores = 0, Bolds = 0, Strikeouts = 0 ;
   for ( int i=0; i<*Size; i++ ) {
      switch ( Text[i] ) {
         case FUNCTION_ESCAPE:
         case FUNCTION_END:
         case FUNCTION_START:
         case ENDFILE:
            GlyphCount ++ ;
            break;
         case 0:
            break;
         default:
            if ( ( i<(*Size-2) ) && ( Text[i+1] == '\b' ) ) {
               if ( ( Text[i] == '_' ) && ( Text[i+2] != '_' ) ) {
                  if ( WasBold ) Bolds ++ ;
                  if ( WasStruckout ) Strikeouts ++ ;
                  if ( !WasUnderscored ) {
                     WasUnderscored = TRUE ;
                     Underscores ++ ;
                  } /* endif */
               } else if ( ( Text[i] == '-' ) && ( Text[i+2] != '-' ) ) {
                  if ( WasBold ) Bolds ++ ;
                  if ( WasUnderscored ) Underscores ++ ;
                  if ( !WasStruckout ) {
                     WasStruckout = TRUE ;
                     Strikeouts ++ ;
                  } /* endif */
               } else {
                  if ( WasUnderscored ) Underscores ++ ;
                  if ( WasStruckout ) Strikeouts ++ ;
                  if ( !WasBold ) {
                     WasBold = TRUE ;
                     Bolds ++ ;
                  } /* endif */
               } /* endif */
               i += 2 ;
            } else {
               if ( WasBold ) {
                  WasBold = FALSE ;
                  Bolds ++ ;
               } /* endif */
               if ( WasUnderscored ) {
                  WasUnderscored = FALSE ;
                  Underscores ++ ;
               } /* endif */
               if ( WasStruckout ) {
                  WasStruckout = FALSE ;
                  Strikeouts ++ ;
               } /* endif */
            } /* endif */
            break;
      } /* endswitch */
   } /* endfor */

   Profile SetupData ( PROGRAM_NAME, WinQueryAnchorBlock(Owner), HomePath ) ;

   char Font [FACESIZE] ;
   strcpy ( Font, OUTPUT_FONT ) ;
   SetupData.GetString ( "Font", Font, sizeof(Font) ) ;

   long FontSize = OUTPUT_PITCH ;
   SetupData.GetItem ( "Size", &FontSize, sizeof(FontSize) ) ;

   char MetricFlag [2] = { '1', 0 } ;
   ULONG ItemSize = sizeof(MetricFlag) ;
   PrfQueryProfileData ( HINI_USERPROFILE, PSZ("PM_National"), PSZ("iMeasurement"), MetricFlag, &ItemSize ) ;
   BOOL ImportMetric = ( MetricFlag [0] == '1' ) ? FALSE : TRUE ;
   SetupData.GetItem ( "ImportMetric", &ImportMetric, sizeof(ImportMetric) ) ;
   BOOL ImportSoftbreaks = FALSE ;
   SetupData.GetItem ( "ImportSoftbreaks", &ImportSoftbreaks, sizeof(ImportSoftbreaks) ) ;

   char Header [0x1000] = { 0 } ;
   sprintf ( Header+strlen(Header), "%c%s", FUNCTION_START, FunctionNames[FN__PREFACE] ) ;
   sprintf ( Header+strlen(Header), "%c%s%s%c", FUNCTION_START, FunctionNames[FN__FONT], Font, FUNCTION_END ) ;
   sprintf ( Header+strlen(Header), "%c%s%.1lf%c", FUNCTION_START, FunctionNames[FN__SIZE], double(FontSize)/10, FUNCTION_END ) ;
   sprintf ( Header+strlen(Header), "%c%s%i%c", FUNCTION_START, FunctionNames[FN__METRIC], ImportMetric, FUNCTION_END ) ;
   sprintf ( Header+strlen(Header), "%c%s%i%c", FUNCTION_START, FunctionNames[FN__TABSPACING], long(double(8*FontSize)*(ImportMetric?2.54:1.00)), FUNCTION_END ) ;
   sprintf ( Header+strlen(Header), "%c", FUNCTION_END ) ;

   long ConvertedSize = strlen(Header) + *Size + 100 ;
   ConvertedSize += GlyphCount * ( strlen(FunctionNames[FN__GLYPH]) + 2 + 2 ) ;
   ConvertedSize += Underscores * ( strlen(FunctionNames[FN__UNDERSCORE]) + 2 + 1 ) ;
   ConvertedSize += Strikeouts * ( strlen(FunctionNames[FN__STRIKEOUT]) + 2 + 1 ) ;
   ConvertedSize += Bolds * ( strlen(FunctionNames[FN__BOLD]) + 2 + 1 ) ;

   char *ConvertedText = (char*) malloc ( (size_t) ConvertedSize ) ;

   strcpy ( ConvertedText, Header ) ;
   ConvertedSize = (long) strlen(ConvertedText) ;

   BOOL NewLine = TRUE ;
   WasBold = FALSE, WasUnderscored = FALSE, WasStruckout = FALSE ;
   for ( i=0; i<*Size; i++ ) {
      switch ( Text[i] ) {
         case FUNCTION_ESCAPE:
         case FUNCTION_END:
         case FUNCTION_START:
            ConvertedSize += sprintf ( ConvertedText+ConvertedSize, "%c%s%i%c", FUNCTION_START, FunctionNames[FN__GLYPH], Text[i], FUNCTION_END ) ;
            NewLine = FALSE ;
            break;
         case ENDFILE:
            if ( i < (*Size)-1 ) {
               ConvertedSize += sprintf ( ConvertedText+ConvertedSize, "%c%s%i%c", FUNCTION_START, FunctionNames[FN__GLYPH], Text[i], FUNCTION_END ) ;
            } else {
               ConvertedText[ConvertedSize++] = Text[i] ;
            } /* endif */
            break;
         case '\r':
            if ( ( i<(*Size-2) ) && ( Text[i+1] == '\r' ) && ( Text[i+2] == '\n' ) ) {
               i += 2 ; // Skip the entire soft return (CR/CR/LF).
            } else if ( ( i<(*Size-1) ) && ( Text[i+1] == '\f' ) ) {
               i ++ ;   // Skip the soft form feed (CR/FF).
            } else if ( ( i<(*Size-1) ) && ( Text[i+1] == '\n' ) ) {
               // We've found a CR/LF pair.
               // If followed by another pair, we absolutely accept it, converting it into a single newline.
               if ( ( i<(*Size-2) ) && ( ( Text[i+2] == '\r' ) || ( Text[i+2] == '\n' ) ) ) {
                  i ++ ;   
                  ConvertedText[ConvertedSize++] = Text[i] ;
                  NewLine = TRUE ;
               // If ignoring single CR/LF pairs, skip the pair if we're not on an empty line.
               } else if ( ImportSoftbreaks && !NewLine ) {
                  i ++ ;   
                  ConvertedText[ConvertedSize++] = ' ' ;
               // Else, convert it into a single newline character.
               } else {
                  i ++ ;   
                  ConvertedText[ConvertedSize++] = Text[i] ;
                  NewLine = TRUE ;
               } /* endif */
            } else {
               ConvertedText[ConvertedSize++] = '\n' ;
               NewLine = TRUE ;
            } /* endif */
            break;
         case 0:
            break;
         default: 
            NewLine = FALSE ;
            // Perform special traps . . .
            if ( ( i<(*Size-2) ) && ( Text[i+1] == '\b' ) ) {
               if ( ( Text[i] == '_' ) && ( Text[i+2] != '_' ) ) {
                  if ( WasBold ) {
                     ConvertedSize += sprintf ( ConvertedText+ConvertedSize, "%c%s0%c", FUNCTION_START, FunctionNames[FN__BOLD], FUNCTION_END ) ;
                     WasBold = FALSE ;
                  } /* endif */
                  if ( WasStruckout ) {
                     ConvertedSize += sprintf ( ConvertedText+ConvertedSize, "%c%s0%c", FUNCTION_START, FunctionNames[FN__STRIKEOUT], FUNCTION_END ) ;
                     WasStruckout = FALSE ;
                  } /* endif */
                  if ( !WasUnderscored ) {
                     ConvertedSize += sprintf ( ConvertedText+ConvertedSize, "%c%s1%c", FUNCTION_START, FunctionNames[FN__UNDERSCORE], FUNCTION_END ) ;
                     WasUnderscored = TRUE ;
                  } /* endif */
                  ConvertedText[ConvertedSize++] = Text[i+2] ;
               } else if ( ( Text[i] == '-' ) && ( Text[i+2] != '-' ) ) {
                  if ( WasBold ) {
                     ConvertedSize += sprintf ( ConvertedText+ConvertedSize, "%c%s0%c", FUNCTION_START, FunctionNames[FN__BOLD], FUNCTION_END ) ;
                     WasBold = FALSE ;
                  } /* endif */
                  if ( WasUnderscored ) {
                     ConvertedSize += sprintf ( ConvertedText+ConvertedSize, "%c%s0%c", FUNCTION_START, FunctionNames[FN__UNDERSCORE], FUNCTION_END ) ;
                     WasUnderscored = FALSE ;
                  } /* endif */
                  if ( !WasStruckout ) {
                     ConvertedSize += sprintf ( ConvertedText+ConvertedSize, "%c%s1%c", FUNCTION_START, FunctionNames[FN__STRIKEOUT], FUNCTION_END ) ;
                     WasStruckout = TRUE ;
                  } /* endif */
                  ConvertedText[ConvertedSize++] = Text[i+2] ;
               } else {
                  if ( WasStruckout ) {
                     ConvertedSize += sprintf ( ConvertedText+ConvertedSize, "%c%s0%c", FUNCTION_START, FunctionNames[FN__STRIKEOUT], FUNCTION_END ) ;
                     WasStruckout = FALSE ;
                  } /* endif */
                  if ( WasUnderscored ) {
                     ConvertedSize += sprintf ( ConvertedText+ConvertedSize, "%c%s0%c", FUNCTION_START, FunctionNames[FN__UNDERSCORE], FUNCTION_END ) ;
                     WasUnderscored = FALSE ;
                  } /* endif */
                  if ( !WasBold ) {
                     ConvertedSize += sprintf ( ConvertedText+ConvertedSize, "%c%s1%c", FUNCTION_START, FunctionNames[FN__BOLD], FUNCTION_END ) ;
                     WasBold = TRUE ;
                  } /* endif */
                  ConvertedText[ConvertedSize++] = Text[i] ;
               } /* endif */
               i += 2 ;
            } else {
               if ( WasBold ) {
                  ConvertedSize += sprintf ( ConvertedText+ConvertedSize, "%c%s0%c", FUNCTION_START, FunctionNames[FN__BOLD], FUNCTION_END ) ;
                  WasBold = FALSE ;
               } /* endif */
               if ( WasStruckout ) {
                  ConvertedSize += sprintf ( ConvertedText+ConvertedSize, "%c%s0%c", FUNCTION_START, FunctionNames[FN__STRIKEOUT], FUNCTION_END ) ;
                  WasStruckout = FALSE ;
               } /* endif */
               if ( WasUnderscored ) {
                  ConvertedSize += sprintf ( ConvertedText+ConvertedSize, "%c%s0%c", FUNCTION_START, FunctionNames[FN__UNDERSCORE], FUNCTION_END ) ;
                  WasUnderscored = FALSE ;
               } /* endif */
               ConvertedText[ConvertedSize++] = Text[i] ;
            } /* endif */
            break;
      } /* endswitch */
   } /* endfor */

   free ( Text ) ;

   *Size = ConvertedSize ;

   return ( ConvertedText ) ;
}


/****************************************************************************
 *                                                                          *
 *  Is byte a DBCS Header?                                                  *
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


/****************************************************************************
 *                                                                          *
 *  Export File                                                             *
 *                                                                          *
 ****************************************************************************/

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

static int StringLength ( char *Text ) {
   int Length = 0 ;
   char *p = Text ;
   while ( *p ) {
      if ( *p == '\b' ) {
         Length -- ;
      } else {
         Length ++ ;
      } /* endif */
      p ++ ;
   } /* endwhile */
   return ( Length ) ;
}

static BOOL FlushWord ( HWND Owner, FILE *File, char Word[], int &Column ) {

   if ( Word[0] && !FileWrite ( Owner, File, Word ) )
      return ( FALSE ) ;

   Column += StringLength(Word) ;
   Word[0] = 0 ;

   return ( TRUE ) ;
}

static BOOL NewPage ( HWND Owner, FILE *File, int &Page, int &Row ) {

   if ( !FileWrite ( Owner, File, "\r\f" ) ) 
      return ( FALSE ) ;

   Page ++ ;  Row = 0 ;

   return ( TRUE ) ;
}

static BOOL NewLine ( HWND Owner, FILE *File, long Height, int &Page, int &Row, int &Column ) {

   if ( !FileWrite ( Owner, File, "\r\n" ) ) 
      return ( FALSE ) ;

   Row ++ ; Column = 0 ;

   if ( Row >= Height ) 
      if ( !NewPage ( Owner, File, Page, Row ) )
         return ( FALSE ) ;

   return ( TRUE ) ;
}


extern BOOL APIENTRY Export ( HWND Owner, char *Name, void *Text, long Size ) {

   // Get the user's output width preference.
   Profile SetupData ( PROGRAM_NAME, WinQueryAnchorBlock(Owner), HomePath ) ;
   BOOL ExportFormat = TRUE ;
   SetupData.GetItem ( "ExportFormat", &ExportFormat, sizeof(ExportFormat) ) ;
   long Height = 0x7FFFFFFF, Width = 0x7FFFFFFF ;
   if ( ExportFormat ) {
      Height = OUTPUT_HEIGHT ; SetupData.GetItem ( "Height", &Height, sizeof(Height) ) ;
      Width = OUTPUT_WIDTH ; SetupData.GetItem ( "Width", &Width, sizeof(Width) ) ;
   } /* endif */
   BOOL ExportBold = FALSE ;
   SetupData.GetItem ( "ExportBold", &ExportBold, sizeof(ExportBold) ) ;
   BOOL ExportUnderscore = FALSE ;
   SetupData.GetItem ( "ExportUnderscore", &ExportUnderscore, sizeof(ExportUnderscore) ) ;
   BOOL ExportStrikeout = FALSE ;
   SetupData.GetItem ( "ExportStrikeout", &ExportStrikeout, sizeof(ExportStrikeout) ) ;

   // Get the initial value of the DBCS vector.
   COUNTRYCODE Country = { 0, 0 } ;
   UCHAR DBCSVector [6] [2] = { { 0 } } ;
   DosQueryDBCSEnv ( sizeof(DBCSVector), &Country, PCHAR(DBCSVector) ) ;

   // Try to open the file for writing.  Return an error if not successful. 
   FILE *File = fopen ( Name, "wb" ) ;
   if ( File == 0 ) {
      if ( Owner ) 
         Sys_MessageBox ( HWND_DESKTOP, Owner, "ERROR: Unable to create || open file for saving.", ProductName(), 0, MB_ENTER | MB_ICONEXCLAMATION ) ;
      return ( FALSE ) ;
   } /* endif */

   // Write out the file, byte by byte. 
   BOOL Metric = FALSE, Caps = FALSE, Bold = FALSE, Underscore = FALSE, Strikeout = FALSE ;
   #define MAX_TABS (100)
   long TabSpacing = 500, Tabs[MAX_TABS], CharWidth = 100 ;
   int Page = 1, Row = 0, Column = 0, TabCount = 0 ;
   int Nesting = 0 ;
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
         if ( Type == FN__METRIC ) {
            Metric = BOOL ( atoi ( PCHAR(Text)+i+1+strlen(FunctionNames[Type]) ) ) ;
            CharWidth = Metric ? 254 : 100 ;
            TabCount = 0 ;
            if ( TabSpacing ) 
               for ( long i=TabSpacing; i<Width*CharWidth && TabCount<MAX_TABS; i+=TabSpacing ) 
                  Tabs[TabCount++] = i ;
         } else if ( Type == FN__CAPS ) {
            Caps = BOOL ( atoi ( PCHAR(Text)+i+1+strlen(FunctionNames[Type]) ) ) ;
         } else if ( Type == FN__BOLD ) {
            Bold = BOOL ( atoi ( PCHAR(Text)+i+1+strlen(FunctionNames[Type]) ) ) ;
         } else if ( Type == FN__UNDERSCORE ) {
            Underscore = BOOL ( atoi ( PCHAR(Text)+i+1+strlen(FunctionNames[Type]) ) ) ;
         } else if ( Type == FN__STRIKEOUT ) {
            Strikeout = BOOL ( atoi ( PCHAR(Text)+i+1+strlen(FunctionNames[Type]) ) ) ;
         } else if ( Type == FN__CODEPAGE ) {
            Country.codepage = ULONG ( atoi ( PCHAR(Text)+i+1+strlen(FunctionNames[Type]) ) ) ;
            DosQueryDBCSEnv ( sizeof(DBCSVector), &Country, PCHAR(DBCSVector) ) ;
         } else if ( Type == FN__TABSPACING ) {
            TabSpacing = atol ( PCHAR(Text)+i+1+strlen(FunctionNames[Type]) ) ;
            TabCount = 0 ;
            if ( TabSpacing ) 
               for ( long i=TabSpacing; i<Width*CharWidth && TabCount<MAX_TABS; i+=TabSpacing ) 
                  Tabs[TabCount++] = i ;
         } else if ( Type == FN__TABSET ) {
            long Tab = atol ( PCHAR(Text)+i+1+strlen(FunctionNames[Type]) ) ;
            for ( i=0; i<TabCount; i++ )
               if ( Tabs[i] >= Tab )
                  break ;
            if ( i < TabCount ) {
               if ( Tabs[i] != Tab ) {
                  if ( TabCount < MAX_TABS ) {
                     for ( int j=TabCount; j>i; j-- ) {
                        Tabs[j] = Tabs[j-1] ;
                     } /* endfor */
                     Tabs[i] = Tab ;
                     TabCount ++ ;
                  } /* endif */
               } /* endif */
            } else if ( i < MAX_TABS ) {
               Tabs[TabCount++] = Tab ;
            } /* endif */
         } else if ( Type == FN__TABCLEAR ) {
            long Tab = atol ( PCHAR(Text)+i+1+strlen(FunctionNames[Type]) ) ;
            for ( i=0; i<TabCount; i++ )
               if ( Tabs[i] == Tab )
                  break ;
            if ( i < TabCount ) {
               while ( i < TabCount-1 ) {
                  Tabs[i] = Tabs[i+1] ;
                  i ++ ;
               } /* endfor */
               TabCount -- ;
            } /* endif */
         } else if ( Nesting == 1 ) {
            int Offset = strlen(Word) ;
            if ( Type == FN__GLYPH ) {
               char Character = CHAR(atoi(PCHAR(Text)+i+1+strlen(FunctionNames[Type]))) ;
               if ( ExportBold && Bold ) {
                  Word[Offset+0] = char ( Character & 0xFF ) ;
                  Word[Offset+1] = '\b' ;
                  Word[Offset+2] = char ( Character & 0xFF ) ;
                  Word[Offset+3] = 0 ;
               } else if ( ExportUnderscore && Underscore ) {
                  Word[Offset+0] = '_' ;
                  Word[Offset+1] = '\b' ;
                  Word[Offset+2] = char ( Character & 0xFF ) ;
                  Word[Offset+3] = 0 ;
               } else if ( ExportStrikeout && Strikeout ) {
                  Word[Offset+0] = '-' ;
                  Word[Offset+1] = '\b' ;
                  Word[Offset+2] = char ( Character & 0xFF ) ;
                  Word[Offset+3] = 0 ;
               } else {
                  Word[Offset+0] = char ( Character & 0xFF ) ;
                  Word[Offset+1] = 0 ;
               } /* endif */
            } else if ( Type == FN__SEPARATOR ) {
               if ( !FlushWord ( Owner, File, Word, Column ) ) 
                  return ( FALSE ) ;
               if ( Column && !NewLine ( Owner, File, Height, Page, Row, Column ) ) 
                  return ( FALSE ) ;
               for ( int i=0; i<Width; i++ ) {
                  Word[Column+0] = '-' ;
                  Word[Column+1] = 0 ;
                  Column ++ ;
               } /* endfor */
               if ( !FlushWord ( Owner, File, Word, Column ) ) 
                  return ( FALSE ) ;
               if ( !NewLine ( Owner, File, Height, Page, Row, Column ) ) 
                  return ( FALSE ) ;
            } else if ( Type == FN__PAGENUMBER ) {
               sprintf ( Word+Offset, "%i", Page ) ;
            } else if ( Type == FN__PAGECOUNT ) {
               // ELABORATE 
            } else if ( Type == FN__DATEFIXED ) { 
               // ELABORATE 
            } else if ( Type == FN__DATECURRENT ) {
               // ELABORATE 
            } else if ( Type == FN__DATECHANGED ) {
               // ELABORATE 
            } /* endif */
            if ( Column && ( Column + StringLength(Word) > Width ) ) {
               if ( !NewLine ( Owner, File, Height, Page, Row, Column ) ) 
                  return ( FALSE ) ;
            } /* endif */
         } /* endif */

      // Else, if exiting a function, note the change in nesting. 
      } else if ( PCHAR(Text)[i] == FUNCTION_END ) {
         Nesting -- ;

      // Else, if at nesting level zero, we have literal text to output. 
      } else if ( Nesting == 0 ) {

         USHORT Character = PCHAR(Text) [i] ;
         int CharSize = 1 ;

         // If end-of-document . . .
         if ( Character == ENDFILE ) {
            if ( !FlushWord ( Owner, File, Word, Column ) ) 
               return ( FALSE ) ;
            if ( !FileWrite ( Owner, File, PCHAR(&Character) ) )
               return ( FALSE ) ;
            Row ++ ; Column = 0 ;
            if ( Row >= Height ) 
               if ( !NewPage ( Owner, File, Page, Row ) )
                  return ( FALSE ) ;

         // Else, if end-of-page . . .
         } else if ( Character == '\f' ) {
            if ( !FlushWord ( Owner, File, Word, Column ) ) 
               return ( FALSE ) ;
            if ( !FileWrite ( Owner, File, PCHAR(&Character) ) )
               return ( FALSE ) ;
            Page ++ ; Row = 0 ; Column = 0 ;

         // Else, if end-of-line . . .
         } else if ( Character == '\n' ) {
            if ( !FlushWord ( Owner, File, Word, Column ) ) 
               return ( FALSE ) ;
            if ( !FileWrite ( Owner, File, PCHAR(&Character) ) )
               return ( FALSE ) ;
            Row ++ ; Column = 0 ;
            if ( Row >= Height ) 
               if ( !NewPage ( Owner, File, Page, Row ) )
                  return ( FALSE ) ;

         // Else, if tab . . .
         } else if ( Character == '\t' ) {
            if ( !FlushWord ( Owner, File, Word, Column ) ) 
               return ( FALSE ) ;
            int NumberOfSpaces = 8 - Column % 8 ;
            char *Spaces = "          " ;
            if ( !FileWrite ( Owner, File, Spaces, NumberOfSpaces ) ) 
               return ( FALSE ) ;
            Column += NumberOfSpaces ;

         // Else, if space . . .
         } else if ( Character == ' ' ) {
            if ( !FlushWord ( Owner, File, Word, Column ) ) 
               return ( FALSE ) ;
            if ( Column+1 > Width ) {
               if ( !NewLine ( Owner, File, Height, Page, Row, Column ) ) 
                  return ( FALSE ) ;
            } else {
               if ( ExportUnderscore && Underscore ) {
                  if ( !FileWrite ( Owner, File, "_\b " ) )
                     return ( FALSE ) ;
               } else if ( ExportStrikeout && Strikeout ) {
                  if ( !FileWrite ( Owner, File, "-\b " ) )
                     return ( FALSE ) ;
               } else {
                  if ( !FileWrite ( Owner, File, " " ) )
                     return ( FALSE ) ;
               } /* endif */
               Column ++ ;
            } /* endif */

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
               if ( Caps ) 
                  DosMapCase ( 1, &Country, PCHAR(&Character) ) ;
               if ( ExportBold && Bold ) {
                  Word[Offset+0] = char ( Character & 0xFF ) ;
                  Word[Offset+1] = '\b' ;
                  Word[Offset+2] = char ( Character & 0xFF ) ;
                  Word[Offset+3] = 0 ;
               } else if ( ExportUnderscore && Underscore ) {
                  Word[Offset+0] = '_' ;
                  Word[Offset+1] = '\b' ;
                  Word[Offset+2] = char ( Character & 0xFF ) ;
                  Word[Offset+3] = 0 ;
               } else if ( ExportStrikeout && Strikeout ) {
                  Word[Offset+0] = '-' ;
                  Word[Offset+1] = '\b' ;
                  Word[Offset+2] = char ( Character & 0xFF ) ;
                  Word[Offset+3] = 0 ;
               } else {
                  Word[Offset+0] = char ( Character & 0xFF ) ;
                  Word[Offset+1] = 0 ;
               } /* endif */
            } /* endif */
            if ( Column && ( Column + StringLength(Word) > Width ) ) 
               if ( !NewLine ( Owner, File, Height, Page, Row, Column ) ) 
                  return ( FALSE ) ;

         } /* endif */

      } /* endif */

   } /* endfor */

   // If a word remains to be flushed, do so now.
   if ( !FlushWord ( Owner, File, Word, Column ) ) 
      return ( FALSE ) ;

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
      { EAT_ASCII, 10, "Plain Text"    },
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

   // Close the file. 
   fclose ( File ) ;

   // Return no error.
   return ( TRUE ) ;
}
