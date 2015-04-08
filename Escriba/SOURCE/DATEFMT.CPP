/**************************************************************** DATEFMT.CPP
 *                                                                          *
 *                           Date Format Dialog                             *
 *                                                                          *
 ****************************************************************************/

#include "System.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "DateFmt.h"
#include "Debug.h"
#include "Dialog2.h"
#include "Escriba.h"
#include "ReString.h"
#include "Support.h"

// #define DEBUG

 
/****************************************************************************
 *                                                                          *
 *                     Definitions & Declarations                           *
 *                                                                          *
 ****************************************************************************/

  // Function Prototypes

static WINDOWPROC InitDlg ;
static WINDOWPROC Control ;
static WINDOWPROC Command ;
static WINDOWPROC OK ;
static WINDOWPROC Cancel ;
static WINDOWPROC Apply ;

 
/****************************************************************************
 *                                                                          *
 *      Dialog Message Processor                                            *
 *                                                                          *
 ****************************************************************************/

extern MRESULT EXPENTRY DateFormatProcessor ( HWND Window, ULONG msg, MPARAM mp1, MPARAM mp2 ) {

  /**************************************************************************
   * Dispatch the message according to the method table and return the      *
   *   result.  Any messages not defined above get handled by the system    *
   *   default dialog processor.                                            *
   **************************************************************************/

   static METHOD Methods [] = {
     { WM_INITDLG, InitDlg },
     { WM_CONTROL, Control },
     { WM_COMMAND, Command },
   } ;

   #ifdef DEBUG
      static int Indent = 0 ;
      Log ( "%*sDATEFMT: Message %08X received.  Parm1=%08X, Parm2=%08X.", Indent, "", msg, mp1, mp2 ) ;
      Indent += 2 ;
   #endif

   MRESULT Result = DispatchMessage ( Window, msg, mp1, mp2, Methods, sizeof(Methods)/sizeof(Methods[0]), Dialog_Processor ) ;

   #ifdef DEBUG
      Indent -= 2 ;
      Log ( "%*sDATEFMT: Message %08X done.  Result %08X.", Indent, "", msg, Result ) ;
   #endif

   return ( Result ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Initialize Dialog                                                   *
 *                                                                          *
 ****************************************************************************/

static MRESULT APIENTRY InitDlg ( HWND Window, ULONG msg, MPARAM mp1, MPARAM mp2 ) {

  /**************************************************************************
   * Get parameters from initialization message.                            *
   **************************************************************************/

   PDATEFMT_PARMS Parms = PDATEFMT_PARMS ( PVOIDFROMMP ( mp2 ) ) ;

   Sys_SetWindowData ( Window, Parms ) ;

  /**************************************************************************
   * Fill the entry field.                                                  *
   **************************************************************************/

   Sys_SendMessage ( CHILD(Window,IDD_DATEFMT_ENTRY), EM_SETTEXTLIMIT, MPFROMLONG(sizeof(Parms->Format)-1), 0 ) ;

   char Local [sizeof(Parms->Format)] ;
   LocalDateFormat ( Local, Parms->Format ) ;
   Sys_SetWindowText ( CHILD(Window,IDD_DATEFMT_ENTRY), Local ) ;

  /**************************************************************************
   * Return no error.                                                       *
   **************************************************************************/

   return ( Dialog_Processor ( Window, msg, mp1, mp2 ) ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process control notifications.                                      *
 *                                                                          *
 ****************************************************************************/

static MRESULT APIENTRY Control ( HWND Window, ULONG, MPARAM mp1, MPARAM ) {

  /**************************************************************************
   * Find the instance data.                                                *
   **************************************************************************/

   PDATEFMT_PARMS Parms = PDATEFMT_PARMS ( Sys_GetWindowData ( Window ) ) ;

  /**************************************************************************
   * Decode the message.  Find out what control sent it, and what the       *
   *   control had to say.                                                  *
   **************************************************************************/

   USHORT id = SHORT1FROMMP ( mp1 ) ;
   USHORT Message = SHORT2FROMMP ( mp1 ) ;

  /**************************************************************************
   * Handle changes according to which control reported them.               *
   **************************************************************************/

   switch ( id ) {

      case IDD_DATEFMT_ENTRY: {
         if ( Message == EN_CHANGE ) {
            char Local [sizeof(Parms->Format)] ;
            Sys_GetWindowText ( CHILD(Window,id), Local, sizeof(Local) ) ;
            GeneralDateFormat ( Parms->Format, Local ) ;
            char Result [100] ;
            if ( FormatDate ( time(0), Parms->Format, Result, sizeof(Result) ) ) {
               Sys_SetWindowText ( CHILD(Window,IDD_DATEFMT_SAMPLE), Result ) ;
               Sys_SetWindowText ( CHILD(Window,IDD_DATEFMT_ERR), "" ) ;
            } else {
               ResourceString Message ( Parms->Library, IDS_DATEFMT_ERROR1 ) ;
               Sys_SetWindowText ( CHILD(Window,IDD_DATEFMT_ERR), PSZ(Message) ) ;
            } /* endif */
         } /* endif */
         return ( 0 ) ;
      } /* endcase */
   } /* endswitch */

   return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process commands received by the dialog window.                     *
 *                                                                          *
 ****************************************************************************/

static MRESULT APIENTRY Command ( HWND Window, ULONG, MPARAM mp1, MPARAM mp2 ) {

  /**************************************************************************
   * Dispatch the message without a default message processor.              *
   **************************************************************************/

   static METHOD Methods [] = {
      { DID_OK,            OK       },
      { DID_CANCEL,        Cancel   },
      { IDD_DATEFMT_APPLY, Apply    },
   } ;

   return ( DispatchMessage ( Window, SHORT1FROMMP(mp1), mp1, mp2, Methods,
      sizeof(Methods)/sizeof(Methods[0]), 0 ) ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process the dialog's OK button being pressed.                       *
 *                                                                          *
 ****************************************************************************/

static MRESULT APIENTRY OK ( HWND Window, ULONG, MPARAM, MPARAM ) {

  /**************************************************************************
   * Find the instance data.                                                *
   **************************************************************************/

   PDATEFMT_PARMS Parms = PDATEFMT_PARMS ( Sys_GetWindowData ( Window ) ) ;

  /**************************************************************************
   * Verify the date format.                                                *
   **************************************************************************/

   if ( !VerifyDateFormat ( CHILD(Window,IDD_DATEFMT_ENTRY), CHILD(Window,IDD_DATEFMT_ERR), CHILD(Window,IDD_DATEFMT_SAMPLE), Parms->Format ) )
      return ( 0 ) ;

  /**************************************************************************
   * Dismiss the dialog with a TRUE status.                                 *
   **************************************************************************/

   Sys_EndDialog ( Window, TRUE ) ;

   return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process the dialog's cancel button being pressed.                   *
 *                                                                          *
 ****************************************************************************/

static MRESULT APIENTRY Cancel ( HWND Window, ULONG, MPARAM, MPARAM ) {

  /**************************************************************************
   * Dismiss the dialog with a TRUE status.                                 *
   **************************************************************************/

   Sys_EndDialog ( Window, FALSE ) ;

   return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Apply the new date format.                                          *
 *                                                                          *
 ****************************************************************************/

static MRESULT APIENTRY Apply ( HWND Window, ULONG, MPARAM, MPARAM ) {

  /**************************************************************************
   * Find the instance data.                                                *
   **************************************************************************/

   PDATEFMT_PARMS Parms = PDATEFMT_PARMS ( Sys_GetWindowData ( Window ) ) ;

  /**************************************************************************
   * Verify the date format.                                                *
   **************************************************************************/

   if ( !VerifyDateFormat ( CHILD(Window,IDD_DATEFMT_ENTRY), CHILD(Window,IDD_DATEFMT_ERR), CHILD(Window,IDD_DATEFMT_SAMPLE), Parms->Format ) )
      return ( 0 ) ;

  /**************************************************************************
   * Send the new text back to the parent window.                           *
   **************************************************************************/

   Sys_SendMessage ( OWNER(Window), WM_SET_DATEFMT, MPFROMP(Parms->Format), 0 ) ;

   return ( 0 ) ;
}


 
/****************************************************************************
 *                                                                          *
 *  Verify Date Format                                                      *
 *                                                                          *
 ****************************************************************************/

extern BOOL VerifyDateFormat ( HWND hwndField, HWND hwndError, HWND hwndSample, char *DateFormat ) {

  /**************************************************************************
   * Get the new value.                                                     *
   **************************************************************************/

   char Local [100] ;
   Sys_GetWindowText ( hwndField, Local, sizeof(Local) ) ;
   GeneralDateFormat ( DateFormat, Local ) ;

  /**************************************************************************
   * Validate it.                                                           *
   **************************************************************************/

   char Result [100] ;
   if ( FormatDate ( time(0), DateFormat, Result, sizeof(Result) ) ) {
      Sys_SetWindowText ( hwndSample, Result ) ;
   } else {
      ResourceString Message ( LibraryHandle, IDS_DATEFMT_ERROR1 ) ;
      Sys_SetWindowText ( hwndError, PCHAR(Message) ) ;
      Sys_BeepError ( ) ;
      Sys_SetFocus ( hwndField ) ;
      return ( FALSE ) ;
   } /* endif */

  /**************************************************************************
   * Return no error.                                                       *
   **************************************************************************/

   return ( TRUE ) ;
}

 
/****************************************************************************
 *                                                                          *
 *      Load/Unload Date Formatting Resource Strings                        *
 *                                                                          *
 ****************************************************************************/

enum {
   DATEFN_HEADER,
   DATEFN_YEAR4,
   DATEFN_YEAR2,
   DATEFN_MON,
   DATEFN_MONTH,
   DATEFN_MONTHS,
   DATEFN_DAY,
   DATEFN_WEEKDAY,
   DATEFN_WKDAY,
   DATEFN_MAX
} ;

static ResourceString *FormatHeader ;
static ResourceString *FormatYear4 ;
static ResourceString *FormatYear2 ;
static ResourceString *FormatMon ;
static ResourceString *FormatMonth ;
static ResourceString *FormatMonths ;
static ResourceString *FormatDay ;
static ResourceString *FormatWeekday ;
static ResourceString *FormatWkDay ;
static ResourceString *Weekdays ;
static ResourceString *Sunday ;
static ResourceString *Monday ;
static ResourceString *Tuesday ;
static ResourceString *Wednesday ;
static ResourceString *Thursday ;
static ResourceString *Friday ;
static ResourceString *Saturday ;
static ResourceString *Months ;
static ResourceString *January ;
static ResourceString *February ;
static ResourceString *March ;
static ResourceString *April ;
static ResourceString *May ;
static ResourceString *June ;
static ResourceString *July ;
static ResourceString *August ;
static ResourceString *September ;
static ResourceString *October ;
static ResourceString *November ;
static ResourceString *December ;

static char *WeekdayNames [7] ;
static char *MonthNames [12] ;
static char Functions [DATEFN_MAX] [2] ;

extern void LoadDateFormattingResources ( void ) {

   FormatHeader = new ResourceString ( LibraryHandle, IDS_DATEFMT_HEADER  ) ;   // Token header.
   FormatYear4  = new ResourceString ( LibraryHandle, IDS_DATEFMT_YEAR4   ) ;   // Token for 4-digit year.
   FormatYear2  = new ResourceString ( LibraryHandle, IDS_DATEFMT_YEAR2   ) ;   // Token for 2-digit year.
   FormatMon    = new ResourceString ( LibraryHandle, IDS_DATEFMT_MON     ) ;   // Token for abbreviated string month.
   FormatMonth  = new ResourceString ( LibraryHandle, IDS_DATEFMT_MONTH   ) ;   // Token for numeric month.
   FormatMonths = new ResourceString ( LibraryHandle, IDS_DATEFMT_MONTHS  ) ;   // Token for string month.
   FormatDay    = new ResourceString ( LibraryHandle, IDS_DATEFMT_DAY     ) ;   // Token for numeric day in month.
   FormatWeekday= new ResourceString ( LibraryHandle, IDS_DATEFMT_WEEKDAY ) ;   // Token for string day-of-week.
   FormatWkDay  = new ResourceString ( LibraryHandle, IDS_DATEFMT_WKDAY   ) ;   // Token for abbreviated string weekday.
   Weekdays     = new ResourceString ( LibraryHandle, IDS_DATEFMT_WEEKDAYS) ;
   Sunday       = new ResourceString ( LibraryHandle, IDS_DATEFMT_DAY0    ) ;
   Monday       = new ResourceString ( LibraryHandle, IDS_DATEFMT_DAY1    ) ;
   Tuesday      = new ResourceString ( LibraryHandle, IDS_DATEFMT_DAY2    ) ;
   Wednesday    = new ResourceString ( LibraryHandle, IDS_DATEFMT_DAY3    ) ;
   Thursday     = new ResourceString ( LibraryHandle, IDS_DATEFMT_DAY4    ) ;
   Friday       = new ResourceString ( LibraryHandle, IDS_DATEFMT_DAY5    ) ;
   Saturday     = new ResourceString ( LibraryHandle, IDS_DATEFMT_DAY6    ) ;
   Months       = new ResourceString ( LibraryHandle, IDS_DATEFMT_MONS    ) ;
   January      = new ResourceString ( LibraryHandle, IDS_DATEFMT_MONTH1  ) ;
   February     = new ResourceString ( LibraryHandle, IDS_DATEFMT_MONTH2  ) ;
   March        = new ResourceString ( LibraryHandle, IDS_DATEFMT_MONTH3  ) ;
   April        = new ResourceString ( LibraryHandle, IDS_DATEFMT_MONTH4  ) ;
   May          = new ResourceString ( LibraryHandle, IDS_DATEFMT_MONTH5  ) ;
   June         = new ResourceString ( LibraryHandle, IDS_DATEFMT_MONTH6  ) ;
   July         = new ResourceString ( LibraryHandle, IDS_DATEFMT_MONTH7  ) ;
   August       = new ResourceString ( LibraryHandle, IDS_DATEFMT_MONTH8  ) ;
   September    = new ResourceString ( LibraryHandle, IDS_DATEFMT_MONTH9  ) ;
   October      = new ResourceString ( LibraryHandle, IDS_DATEFMT_MONTH10 ) ;
   November     = new ResourceString ( LibraryHandle, IDS_DATEFMT_MONTH11 ) ;
   December     = new ResourceString ( LibraryHandle, IDS_DATEFMT_MONTH12 ) ;

   WeekdayNames[0] = PCHAR(*Sunday) ;
   WeekdayNames[1] = PCHAR(*Monday) ;
   WeekdayNames[2] = PCHAR(*Tuesday) ;
   WeekdayNames[3] = PCHAR(*Wednesday) ;
   WeekdayNames[4] = PCHAR(*Thursday) ;
   WeekdayNames[5] = PCHAR(*Friday) ;
   WeekdayNames[6] = PCHAR(*Saturday) ;

   MonthNames[ 0] = PCHAR(*January) ;
   MonthNames[ 1] = PCHAR(*February) ;
   MonthNames[ 2] = PCHAR(*March) ;
   MonthNames[ 3] = PCHAR(*April) ;
   MonthNames[ 4] = PCHAR(*May) ;
   MonthNames[ 5] = PCHAR(*June) ;
   MonthNames[ 6] = PCHAR(*July) ;
   MonthNames[ 7] = PCHAR(*August) ;
   MonthNames[ 8] = PCHAR(*September) ;
   MonthNames[ 9] = PCHAR(*October) ;
   MonthNames[10] = PCHAR(*November) ;
   MonthNames[11] = PCHAR(*December) ;

   Functions [DATEFN_HEADER ] [0] = '%' ;  Functions [DATEFN_HEADER ] [1] = *PCHAR(*FormatHeader) ;
   Functions [DATEFN_YEAR4  ] [0] = 'y' ;  Functions [DATEFN_YEAR4  ] [1] = *PCHAR(*FormatYear4) ;
   Functions [DATEFN_YEAR2  ] [0] = 'z' ;  Functions [DATEFN_YEAR2  ] [1] = *PCHAR(*FormatYear2) ;
   Functions [DATEFN_MON    ] [0] = 'n' ;  Functions [DATEFN_MON    ] [1] = *PCHAR(*FormatMon) ;
   Functions [DATEFN_MONTH  ] [0] = 'm' ;  Functions [DATEFN_MONTH  ] [1] = *PCHAR(*FormatMonth) ;
   Functions [DATEFN_MONTHS ] [0] = 'M' ;  Functions [DATEFN_MONTHS ] [1] = *PCHAR(*FormatMonths) ;
   Functions [DATEFN_DAY    ] [0] = 'd' ;  Functions [DATEFN_DAY    ] [1] = *PCHAR(*FormatDay) ;
   Functions [DATEFN_WEEKDAY] [0] = 'w' ;  Functions [DATEFN_WEEKDAY] [1] = *PCHAR(*FormatWeekday) ;
   Functions [DATEFN_WKDAY  ] [0] = 'x' ;  Functions [DATEFN_WKDAY  ] [1] = *PCHAR(*FormatWkDay) ;
}

extern void UnloadDateFormattingResources ( void ) {
   delete FormatHeader ;
   delete FormatYear4 ;
   delete FormatYear2 ;
   delete FormatMon ;
   delete FormatMonth ;
   delete FormatMonths ;
   delete FormatDay ;
   delete FormatWeekday ;
   delete FormatWkDay ;
   delete Weekdays ;
   delete Sunday ;
   delete Monday ;
   delete Tuesday ;
   delete Wednesday ;
   delete Thursday ;
   delete Friday ;
   delete Saturday ;
   delete Months ;
   delete January ;
   delete February ;
   delete March ;
   delete April ;
   delete May ;
   delete June ;
   delete July ;
   delete August ;
   delete September ;
   delete October ;
   delete November ;
   delete December ;
}
 
/****************************************************************************
 *                                                                          *
 *      Format Date using general format.                                   *
 *                                                                          *
 ****************************************************************************/

extern char *FormatDate ( time_t Time, char *Format, char *Buffer, int BufferSize ) {

   // Get the local time and date.
   struct tm Tm ;
   memcpy ( &Tm, localtime(&Time), sizeof(Tm) ) ;

   // Process the format string from start to finish.
   char *Source = Format ;
   char *Destination = Buffer ;

   while ( *Source AND ( Destination-Buffer < BufferSize ) ) {
      if ( *Source == Functions[DATEFN_HEADER][0] ) {
         Source ++ ;
         if ( *Source == Functions[DATEFN_YEAR4][0] ) {
            sprintf ( Destination, "%04i", Tm.tm_year+1900 ) ;
         } else if ( *Source == Functions[DATEFN_YEAR2][0] ) {
            sprintf ( Destination, "%02i", Tm.tm_year % 100  ) ;
         } else if ( *Source == Functions[DATEFN_MON][0] ) {
            sprintf ( Destination, "%0.*s", strlen(PCHAR(*Months))/12, PSZ(*Months) + Tm.tm_mon * (strlen(PCHAR(*Months))/12) ) ;
         } else if ( *Source == Functions[DATEFN_MONTH][0] ) {
            sprintf ( Destination, "%i", Tm.tm_mon+1 ) ;
         } else if ( *Source == Functions[DATEFN_MONTHS][0] ) {
            strcpy ( Destination, MonthNames[Tm.tm_mon] ) ;
         } else if ( *Source == Functions[DATEFN_DAY][0] ) {
            sprintf ( Destination, "%i", Tm.tm_mday ) ;
         } else if ( *Source == Functions[DATEFN_WEEKDAY][0] ) {
            strcpy ( Destination, WeekdayNames[Tm.tm_wday] ) ;
         } else if ( *Source == Functions[DATEFN_WKDAY][0] ) {
            sprintf ( Destination, "%0.*s", strlen(PCHAR(*Weekdays))/7, PSZ(*Weekdays) + Tm.tm_wday * (strlen(PCHAR(*Weekdays))/7) ) ;
         } else {
            *Destination++ = Functions[DATEFN_HEADER][0] ;
            *Destination++ = *Source ;
            *Destination = 0 ;
         } /* endif */
         Destination += strlen(Destination) ;
      } else {
         *Destination++ = *Source ;
      } /* endif */
      Source ++ ;
   } /* endwhile */

   // Null terminate the result.
   *Destination = 0 ;

   // Return the address of the formatted date.
   return ( Buffer ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Date Format Translate (from general to local format)                *
 *                                                                          *
 ****************************************************************************/

extern char *LocalDateFormat ( char *Local, char *General ) {

   // Process the format string from start to finish.
   char *Source = General ;
   char *Destination = Local ;

   while ( *Source ) {
      if ( *Source == Functions[DATEFN_HEADER][0] ) {
         Source ++ ;
         for ( int i=1; i<DATEFN_MAX; i++ ) {
            if ( *Source == Functions[i][0] ) {
               *Destination++ = Functions[DATEFN_HEADER][1] ;
               *Destination++ = Functions[i][1] ;
               break ;
            } /* endif */
         } /* endfor */
         if ( i >= DATEFN_MAX ) {
            *Destination++ = Functions[DATEFN_HEADER][0] ;
            *Destination++ = *Source ;
         } /* endif */
      } else {
         *Destination++ = *Source ;
      } /* endif */
      Source ++ ;
   } /* endwhile */

   // Null terminate the result.
   *Destination = 0 ;

   // Return the address of the formatted date.
   return ( Local ) ;
}

extern char *GeneralDateFormat ( char *General, char *Local ) {

   // Process the format string from start to finish.
   char *Source = Local ;
   char *Destination = General ;

   while ( *Source ) {
      if ( *Source == Functions[DATEFN_HEADER][1] ) {
         Source ++ ;
         for ( int i=1; i<DATEFN_MAX; i++ ) {
            if ( *Source == Functions[i][1] ) {
               *Destination++ = Functions[DATEFN_HEADER][0] ;
               *Destination++ = Functions[i][0] ;
               break ;
            } /* endif */
         } /* endfor */
         if ( i >= DATEFN_MAX ) {
            *Destination++ = Functions[DATEFN_HEADER][1] ;
            *Destination++ = *Source ;
         } /* endif */
      } else {
         *Destination++ = *Source ;
      } /* endif */
      Source ++ ;
   } /* endwhile */

   // Null terminate the result.
   *Destination = 0 ;

   // Return the address of the formatted date.
   return ( General ) ;
}

