/*************************************************************** BARCODES.CPP
 *                                                                          *
 *                       Barcode Definition Dialog                          *
 *                                                                          *
 ****************************************************************************/

#include "System.h"

#include <string.h>

#include "Barcodes.h"
#include "Dialog2.h"
#include "Escriba.h"
#include "ReString.h"
#include "Support.h"

//#define DEBUG
 
/****************************************************************************
 *                                                                          *
 *                     Definitions & Declarations                           *
 *                                                                          *
 ****************************************************************************/

static WINDOWPROC InitDlg ;
static WINDOWPROC Control ;
static WINDOWPROC Command ;
static WINDOWPROC OK ;
static WINDOWPROC Cancel ;

static struct {
   char *Type ;
   char *Name ;
   char *Valid ;
} Types [] = {
   { "UP", "UPC-A",              "0123456789" },
   { "UE", "UPC-E",              "0123456789" },
   { "E8", "EAN-8",              "0123456789" },
   { "EA", "EAN-13",             "0123456789" },
   { "PN", "POSTNET",            "0123456789" },
   { "39", "Code-39 (3 from 9)", "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ+-*/$%. " },
   { "27", "Codabar (2 of 7)",   "0123456789-$:/.+ABCD" },
   { "25", "Code 2 of 5",        "0123456789" },
   { "28", "Code 128",           "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz !\"#$%&'()*+,-./:;<=>?@[\\]^_`~{|}" },
} ;

/****************************************************************************
 *                                                                          *
 *      "Barcodes" Dialog Processor                                         *
 *                                                                          *
 ****************************************************************************/

extern MRESULT EXPENTRY Barcodes_Processor ( HWND Window, MESG msg, MPARAM1 mp1, MPARAM2 mp2 ) {

  /**************************************************************************
   * Dispatch the message according to the method table and return the      *
   *   result.  Any messages not defined above get handled by the system    *
   *   default dialog processor.                                            *
   **************************************************************************/

   static METHOD Methods [] = {
      { WM_INITDLG, InitDlg },
      { WM_CONTROL, Control },
      { WM_COMMAND, Command }
   } ;

   #ifdef DEBUG
      static int Indent = 0 ;
      Log ( "%*sBARCODES: Message %08X received.  Parm1=%08X, Parm2=%08X.", Indent, "", msg, mp1, mp2 ) ;
      Indent += 2 ;
   #endif

   MRESULT Result = DispatchMessage ( Window, msg, mp1, mp2, Methods, sizeof(Methods)/sizeof(Methods[0]), Dialog_Processor ) ;

   #ifdef DEBUG
      Indent -= 2 ;
      Log ( "%*sBARCODES: Message %08X done.  Result %08X.", Indent, "", msg, Result ) ;
   #endif

   return ( Result ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Initialize Dialog                                                   *
 *                                                                          *
 ****************************************************************************/

static MRESULT APIENTRY InitDlg ( HWND Window, MESG msg, MPARAM1 mp1, MPARAM2 mp2 ) {

  /**************************************************************************
   * Get the dialog parameters.                                             *
   **************************************************************************/

   PBARCODES_PARMS Parms = PBARCODES_PARMS ( ( PVOIDFROMMP ( mp2 ) ) ) ;

   Sys_SetWindowData ( Window, Parms ) ;

  /**************************************************************************
   * Initialize the type list.                                              *
   **************************************************************************/

   for ( int i=0; i<sizeof(Types)/sizeof(Types[0]); i++ ) {
      Sys_SendMessage ( CHILD(Window,IDD_BARCODE_TYPE), LM_INSERTITEM,
         MPFROMSHORT ( LIT_SORTASCENDING ), MPFROMP ( Types[i].Name ) ) ;
   } /* endfor */

   Sys_SendMessage ( CHILD(Window,IDD_BARCODE_TYPE), LM_SELECTITEM, 0, MPFROMSHORT(TRUE) ) ;

   char Type [80] ;
   Sys_SendMessage ( CHILD(Window,IDD_BARCODE_TYPE), LM_QUERYITEMTEXT,
      MPFROM2SHORT ( 0, sizeof(Type) ), MPFROMP ( Type ) ) ;

   Sys_SetWindowText ( CHILD(Window,IDD_BARCODE_TYPE), PSZ(Type) ) ;

  /**************************************************************************
   * Initialize the entry field.                                            *
   **************************************************************************/

   Sys_SetWindowText ( CHILD(Window,IDD_BARCODE_ENTRY), PSZ(Parms->Text) ) ;
   Sys_SendMessage ( CHILD(Window,IDD_BARCODE_ENTRY), EM_SETTEXTLIMIT, MPFROMSHORT(sizeof(Parms->Text)-1), 0 ) ;

  /**************************************************************************
   * Return all's well.  No focus change.                                   *
   **************************************************************************/

   return ( Dialog_Processor ( Window, msg, mp1, mp2 ) ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process Control Messages                                            *
 *                                                                          *
 ****************************************************************************/

static MRESULT APIENTRY Control ( HWND Window, MESG, MPARAM1 mp1, MPARAM2 ) {

  /**************************************************************************
   * Decode the message.  Find out what control sent it, and what the       *
   *   control had to say.                                                  *
   **************************************************************************/
 
   SHORT Id = SHORT1FROMMP ( mp1 ) ;
   SHORT Message = SHORT2FROMMP ( mp1 ) ;

  /**************************************************************************
   * Process notification according to which control sent it.               *
   **************************************************************************/

   switch ( Id ) {

      case IDD_BARCODE_TYPE: {
         switch ( Message ) {
            case CBN_ENTER:
            case CBN_EFSCROLL:
            case CBN_LBSCROLL:
            case CBN_LBSELECT: {
               short Index = SHORT1FROMMR ( Sys_SendMessage ( CHILD(Window,IDD_BARCODE_TYPE), LM_QUERYSELECTION, MPFROMSHORT(LIT_FIRST), 0 ) ) ;
               if ( Index == LIT_NONE )
                  break ;
               char Type [80] ;
               Sys_SendMessage ( CHILD(Window,IDD_BARCODE_TYPE), LM_QUERYITEMTEXT,
                  MPFROM2SHORT ( Index, sizeof(Type) ), MPFROMP ( Type ) ) ;
               for ( int i=0; i<sizeof(Types)/sizeof(Types[0]); i++ ) 
                  if ( !strcmp ( Type, Types[i].Name ) ) 
                     break ;
               Sys_SetWindowText ( CHILD(Window,IDD_BARCODE_CHARS), PSZ(Types[i].Valid) ) ;
               break ; } /* endcase */
         } /* endswitch */
         break ; } /* endcase */

   } /* endswitch */

  /**************************************************************************
   * Return no error.                                                       *
   **************************************************************************/

   return ( MRFROMSHORT ( FALSE ) ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process commands received by the Dialog                             *
 *                                                                          *
 ****************************************************************************/

static MRESULT APIENTRY Command ( HWND Window, MESG, MPARAM1 mp1, MPARAM2 mp2 ) {

  /**************************************************************************
   * Dispatch the message without a default message processor.              *
   **************************************************************************/

   static METHOD Methods [] = {
      { DID_OK,     OK     },
      { DID_CANCEL, Cancel },
   } ;

   return ( DispatchMessage ( Window, SHORT1FROMMP(mp1), mp1, mp2, Methods, sizeof(Methods)/sizeof(Methods[0]), 0 ) ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process the Dialog's OK button being pressed.                       *
 *                                                                          *
 ****************************************************************************/

static MRESULT APIENTRY OK ( HWND Window, MESG, MPARAM1, MPARAM2 ) {

  /**************************************************************************
   * Find the instance data.                                                *
   **************************************************************************/

   PBARCODES_PARMS Parms = PBARCODES_PARMS ( Sys_GetWindowData ( Window ) ) ;

  /**************************************************************************
   * Get the selected barcode type.                                         *
   **************************************************************************/

   short Index = SHORT1FROMMR ( Sys_SendMessage ( CHILD(Window,IDD_BARCODE_TYPE), LM_QUERYSELECTION, MPFROMSHORT(LIT_FIRST), 0 ) ) ;
   if ( Index == LIT_NONE ) {
      Sys_BeepError ( ) ;
      Sys_SetFocus ( CHILD ( Window, IDD_BARCODE_TYPE ) ) ;
      return ( 0 ) ;
   } /* endif */

   char Type [80] ;
   Sys_SendMessage ( CHILD(Window,IDD_BARCODE_TYPE), LM_QUERYITEMTEXT,
      MPFROM2SHORT ( Index, sizeof(Type) ), MPFROMP ( Type ) ) ;

   for ( int i=0; i<sizeof(Types)/sizeof(Types[0]); i++ ) 
      if ( !strcmp ( Type, Types[i].Name ) ) 
         break ;

  /**************************************************************************
   * Get the text.                                                          *
   **************************************************************************/

   char Text [sizeof(Parms->Text)] ;
   Sys_GetWindowText ( CHILD(Window,IDD_BARCODE_ENTRY), Text, sizeof(Text) ) ;

  /**************************************************************************
   * The text cannot be blank.                                              *
   **************************************************************************/

   if ( strlen(Text) == 0 ) {
      ResourceString Message ( Parms->Library, IDS_BARCODE_ERROR1 ) ;
      Sys_SetWindowText ( CHILD(Window,IDD_BARCODE_ERR), PSZ(Message) ) ;
      Sys_BeepError ( ) ;
      Sys_SetFocus ( CHILD ( Window, IDD_BARCODE_ENTRY ) ) ;
      return ( 0 ) ;
   } /* endif */

  /**************************************************************************
   * Get rid of the white space to either side.                             *
   **************************************************************************/

   char *p = Text + strlen(Text) - 1 ;
   while ( (p-Text) AND ( ( *p == ' ' ) OR ( *p == '\t' ) ) ) {
      *p = 0 ;
      p -- ;
   } /* endwhile */

   p = Text ;
   while ( *p AND ( ( *p == ' ' ) OR ( *p == '\t' ) ) ) p ++ ;

  /**************************************************************************
   * Make sure only valid characters are used.                              *
   **************************************************************************/

   for ( int j=0; j<strlen(p); j++ ) {
      for ( int k=0; k<strlen(Types[i].Valid); k++ ) 
         if ( p[j] == Types[i].Valid[k] ) 
            break ;
      if ( k >= strlen(Types[i].Valid) ) {
         ResourceString Message ( Parms->Library, IDS_BARCODE_ERROR2 ) ;
         Sys_SetWindowText ( CHILD(Window,IDD_BARCODE_ERR), PSZ(Message) ) ;
         Sys_BeepError ( ) ;
         Sys_SetFocus ( CHILD ( Window, IDD_BARCODE_ENTRY ) ) ;
         return ( 0 ) ;
      } /* endif */
   } /* endfor */

  /**************************************************************************
   * If CODABAR, verify the start/end characters.                           *
   **************************************************************************/

   if ( !strcmp ( Types[i].Type, "27" ) ) {
      if ( p[0] != p[strlen(p)-1] ) {
         ResourceString Message ( Parms->Library, IDS_BARCODE_ERROR3 ) ;
         Sys_SetWindowText ( CHILD(Window,IDD_BARCODE_ERR), PSZ(Message) ) ;
         Sys_BeepError ( ) ;
         Sys_SetFocus ( CHILD ( Window, IDD_BARCODE_ENTRY ) ) ;
         return ( 0 ) ;
      } /* endif */
      if ( ( p[0] < 'A' ) || ( p[0] > 'D' ) ) {
         ResourceString Message ( Parms->Library, IDS_BARCODE_ERROR4 ) ;
         Sys_SetWindowText ( CHILD(Window,IDD_BARCODE_ERR), PSZ(Message) ) ;
         Sys_BeepError ( ) ;
         Sys_SetFocus ( CHILD ( Window, IDD_BARCODE_ENTRY ) ) ;
         return ( 0 ) ;
      } /* endif */
      if ( ( p[strlen(p)-1] < 'A' ) || ( p[strlen(p)-1] > 'D' ) ) {
         ResourceString Message ( Parms->Library, IDS_BARCODE_ERROR4 ) ;
         Sys_SetWindowText ( CHILD(Window,IDD_BARCODE_ERR), PSZ(Message) ) ;
         Sys_BeepError ( ) ;
         Sys_SetFocus ( CHILD ( Window, IDD_BARCODE_ENTRY ) ) ;
         return ( 0 ) ;
      } /* endif */
      for ( int i=1; i<strlen(p)-1; i++ ) {
         if ( ( p[i] >= 'A' ) && ( p[i] <= 'D' ) ) {
            ResourceString Message ( Parms->Library, IDS_BARCODE_ERROR2 ) ;
            Sys_SetWindowText ( CHILD(Window,IDD_BARCODE_ERR), PSZ(Message) ) ;
            Sys_BeepError ( ) ;
            Sys_SetFocus ( CHILD ( Window, IDD_BARCODE_ENTRY ) ) ;
            return ( 0 ) ;
         } /* endif */
      } /* endfor */
   } /* endif */

  /**************************************************************************
   * Save the validated information.                                        *
   **************************************************************************/

   strcpy ( Parms->Type, Types[i].Type ) ;
   strcpy ( Parms->Text, p ) ;

  /**************************************************************************
   * Dismiss the dialog with a TRUE status.                                 *
   **************************************************************************/

   Sys_EndDialog ( Window, TRUE ) ;

   return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process the Dialog's being cancelled.                               *
 *                                                                          *
 ****************************************************************************/

static MRESULT APIENTRY Cancel ( HWND Window, MESG, MPARAM1, MPARAM2 ) {

  /**************************************************************************
   * Dismiss the dialog with a TRUE status.                                 *
   **************************************************************************/

   Sys_EndDialog ( Window, FALSE ) ;

   return ( 0 ) ;
}

