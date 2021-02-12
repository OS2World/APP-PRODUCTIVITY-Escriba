/**************************************************************** SPELLER.CPP
 *                                                                          *
 *                         Spell Checker Support                            *
 *                                                                          *
 ****************************************************************************/

#include "System.h"

#include <direct.h>
#include <io.h>
#include <string.h>

#include "Debug.h"
#include "Dialog2.h"
#include "Escriba.h"
#include "ReString.h"
#include "Speller.h"
#include "Support.h"
#include "Thread.h"

// #define DEBUG
// #define DEBUG_MESSAGES

 
/****************************************************************************
 *                                                                          *
 *  Dictionary: Constructor                                                 *
 *                                                                          *
 ****************************************************************************/

Dictionary::Dictionary ( char *ModulePath, char *DefaultLanguage ) :  Addon ( ModulePath ),

   Handle(-1), 
   DictionaryReady("DictionaryReady"),
   Speller_SetHome(0), 
   Speller_DefaultDictionary(0), 
   Speller_OpenDictionary(0), 
   Speller_CloseDictionary(0),
   Speller_Ready(0),
   Speller_BuildWord(0), 
   Speller_FindWord(0),
   Speller_FindReplacements(0), 
   Speller_Accept(0), 
   Speller_Insert(0),
   Speller_UnCaps(0) {

   #ifdef DEBUG
      Log ( "Dictionary::Dictionary() Started." ) ;
   #endif // DEBUG

   // Verify that the module was loaded.  Exit if not.
   if ( !Available() ) {
      #ifdef DEBUG
         Log ( "Dictionary::Dictionary() Aborted because module could not be loaded." ) ;
      #endif // DEBUG
      return ;
   } /* endif */

   // Validate the language selection.  Exit if invalid.
   #ifdef DEBUG
      Log ( "Dictionary::Dictionary() Selecting language '%s'.", DefaultLanguage ) ;
   #endif // DEBUG
   SelectLanguage ( DefaultLanguage ) ;
   #ifdef DEBUG
      Log ( "Dictionary::Dictionary() Checking to see if language was indeed selected.  QueryName() %08X.", QueryName() ) ;
   #endif // DEBUG
   if ( !QueryName() || ( *QueryName() == 0 ) ) {
      #ifdef DEBUG
         Log ( "Dictionary::Dictionary() Language selection was invalid." ) ;
      #endif // DEBUG
      delete Library ;
      Library = 0 ;
      #ifdef DEBUG
         Log ( "Dictionary::Dictionary() Aborted because module name could not be obtained." ) ;
      #endif // DEBUG
      return ;
   } /* endif */

   // Get pointers to the spell checker functions.
   #ifdef DEBUG
      Log ( "Dictionary::Dictionary() Getting pointers to speller functions." ) ;
   #endif // DEBUG
   Speller_SetHome           = (void(APIENTRY*)(char*))                                             QueryLibrary()->QueryAddress ( "Speller_SetHome"           ) ;
   Speller_DefaultDictionary = (char*(APIENTRY*)())                                                 QueryLibrary()->QueryAddress ( "Speller_DefaultDictionary" ) ;
   Speller_OpenDictionary    = (int(APIENTRY*)(char*))                                              QueryLibrary()->QueryAddress ( "Speller_OpenDictionary"    ) ;
   Speller_CloseDictionary   = (void(APIENTRY*)())                                                  QueryLibrary()->QueryAddress ( "Speller_CloseDictionary"   ) ;
   Speller_Ready             = (BOOL(APIENTRY*)())                                                  QueryLibrary()->QueryAddress ( "Speller_Ready"             ) ;
   Speller_BuildWord         = (int(APIENTRY*)(char*(APIENTRY*)(void*,long*,long),void*,char*,long*,long*)) QueryLibrary()->QueryAddress ( "Speller_BuildWord" ) ;
   Speller_FindWord          = (int(APIENTRY*)(char*))                                              QueryLibrary()->QueryAddress ( "Speller_FindWord"          ) ;
   Speller_FindReplacements  = (int(APIENTRY*)(char*,int,int,void*,int*))                           QueryLibrary()->QueryAddress ( "Speller_FindReplacements"  ) ;
   Speller_Accept            = (int(APIENTRY*)(char*))                                              QueryLibrary()->QueryAddress ( "Speller_Accept"            ) ;
   Speller_Insert            = (int(APIENTRY*)(char*))                                              QueryLibrary()->QueryAddress ( "Speller_Insert"            ) ;
   Speller_UnCaps            = (int(APIENTRY*)(char*))                                              QueryLibrary()->QueryAddress ( "Speller_UnCaps"            ) ;

   // If the ISPELL environment variable wasn't set already, set it now.
   if ( Speller_SetHome ) {
      #ifdef DEBUG
         Log ( "Dictionary::Dictionary() Calling speller SetHome function." ) ;
      #endif // DEBUG
      Speller_SetHome ( getenv ( "ISPELL" ) ) ;
   } /* endif */

   // Try to open the default dictionary.
   #ifdef DEBUG
      Log ( "Dictionary::Dictionary() Posting DictionaryReady." ) ;
   #endif // DEBUG
   DictionaryReady.Post() ;

   #ifdef DEBUG
      Log ( "Dictionary::Dictionary() Opening dictionary." ) ;
   #endif // DEBUG
   OpenDictionary ( ) ;

   #ifdef DEBUG
      Log ( "Dictionary::Dictionary() Done OK." ) ;
   #endif // DEBUG

} /* endmethod */

 
/****************************************************************************
 *                                                                          *
 *  Dictionary: Destructor                                                  *
 *                                                                          *
 ****************************************************************************/

Dictionary::~Dictionary ( ) {

   CloseDictionary ( ) ;

} /* endmethod */

 
/****************************************************************************
 *                                                                          *
 *  Dictionary: Open a dictionary                                           *
 *                                                                          *
 ****************************************************************************/

BOOL Dictionary::OpenDictionary ( ) {

   if ( !Available() ) 
      return ( FALSE ) ;

   if ( !Speller_OpenDictionary )
      return ( FALSE ) ;

   if ( !Speller_CloseDictionary )
      return ( FALSE ) ;

   if ( !Speller_DefaultDictionary )
      return ( FALSE ) ;

   if ( !DictionaryReady.Wait ( 10000 ) )
      return ( FALSE ) ;

   if ( Handle >= 0 )
      Speller_CloseDictionary ( ) ;

   DictionaryReady.Reset ( ) ;
   StartThread ( "DictionaryOpenThread", DictionaryOpenThread, 0x8000, this ) ;

   return ( TRUE ) ;

} /* endmethod */

void DictionaryOpenThread ( void *Parameter ) {
   Dictionary *Dictionary = (Dictionary*) Parameter ;
   Dictionary->Handle = Dictionary->Speller_OpenDictionary ( Dictionary->Speller_DefaultDictionary() ) ;
   Dictionary->DictionaryReady.Post ( ) ;
}

/****************************************************************************
 *                                                                          *
 *  Dictionary: Close the current dictionary.                               *
 *                                                                          *
 ****************************************************************************/

void Dictionary::CloseDictionary ( ) {

   if ( !Available() ) 
      return ;

   DictionaryReady.Wait ( ) ;

   if ( Handle < 0 ) 
      return ;

   if ( Speller_CloseDictionary )
      Speller_CloseDictionary ( ) ;

   Handle = -1 ;

} /* endmethod */

 
/****************************************************************************
 *                                                                          *
 *  Dictionary: Is Dictionary Ready for use?                                *
 *                                                                          *
 ****************************************************************************/

BOOL Dictionary::Ready ( ) { 

   // If the DLL is not linked, return FALSE.
   if ( !Available() ) 
      return ( FALSE ) ;

   // If the dictionary load times out, return FALSE.
   if ( !DictionaryReady.Wait ( 10000 ) )
      return ( FALSE ) ;

   // Otherwise, return TRUE if the checker itself says it is initialized OK.
   return ( Speller_Ready && Speller_Ready() ) ;

} /* endmethod */

 
/****************************************************************************
 *                                                                          *
 *  Dictionary: Build a word for checking                                   *
 *                                                                          *
 ****************************************************************************/

BOOL Dictionary::BuildWord ( char* (APIENTRY*GetPtr)(void *Object,long *Offset,long Delta), void *Object, char Word[], long &StartOffset, long &EndOffset ) {

   if ( !Ready() )
      return ( FALSE ) ;

   if ( Speller_BuildWord && ( Handle >= 0 ) )
      return ( Speller_BuildWord ( GetPtr, Object, Word, &StartOffset, &EndOffset ) ) ;

   return ( FALSE ) ;

} /* endmethod */

 
/****************************************************************************
 *                                                                          *
 *  Dictionary: Find a word in the dictionary                               *
 *                                                                          *
 ****************************************************************************/

BOOL Dictionary::FindWord ( char *Word ) {

   if ( !Ready() )
      return ( FALSE ) ;

   if ( Speller_FindWord && ( Handle >= 0 ) )
      return ( Speller_FindWord ( Word ) ) ;

   return ( FALSE ) ;

} /* endmethod */

 
/****************************************************************************
 *                                                                          *
 *  Dictionary: Find possible replacements for a word not found             *
 *                                                                          *
 ****************************************************************************/

int Dictionary::FindReplacements ( char *Word, int ListSize, int EntrySize, void *List, int *EasyCount ) {

   if ( !Ready() )
      return ( FALSE ) ;

   if ( Speller_FindReplacements && ( Handle >= 0 ) )
      return ( Speller_FindReplacements ( Word, ListSize, EntrySize, List, EasyCount ) ) ;

   return ( 0 ) ;

} /* endmethod */

 
/****************************************************************************
 *                                                                          *
 *  Dictionary: Accept a word temporarily into the personal dictionary      *
 *                                                                          *
 ****************************************************************************/

BOOL Dictionary::Accept ( char *Word ) {

   if ( !Ready() )
      return ( FALSE ) ;

   if ( Speller_Accept && ( Handle >= 0 ) )
      return ( Speller_Accept ( Word ) ) ;

   return ( FALSE ) ;

} /* endmethod */

 
/****************************************************************************
 *                                                                          *
 *  Dictionary: Insert a word permanently into the personal dictionary      *
 *                                                                          *
 ****************************************************************************/

BOOL Dictionary::Insert ( char *Word ) {

   if ( !Ready() )
      return ( FALSE ) ;

   if ( Speller_Insert && ( Handle >= 0 ) )
      return ( Speller_Insert ( Word ) ) ;

   return ( FALSE ) ;

} /* endmethod */

 
/****************************************************************************
 *                                                                          *
 *  Dictionary: Insert a word permanently into the personal dictionary,     *
 *    but in such a way as to be case insensitive.                          *
 *                                                                          *
 ****************************************************************************/

BOOL Dictionary::UnCaps ( char *Word ) {

   if ( !Ready() )
      return ( FALSE ) ;

   if ( Speller_UnCaps && ( Handle >= 0 ) )
      return ( Speller_UnCaps ( Word ) ) ;

   return ( FALSE ) ;

} /* endmethod */

 
/****************************************************************************
 *                                                                          *
 *                     Definitions & Declarations                           *
 *                                                                          *
 ****************************************************************************/

#define ENTRYSIZE 80
#define ENTRYCOUNT 30

enum { SPELL_SKIP, SPELL_ACCEPT, SPELL_INSERT, SPELL_UNCAPS, SPELL_REPLACE, SPELL_ESCAPE } ;

typedef struct {
   HWND  Owner ;                // Owner window.
   Documento *pDocument ;       // -> Document object
   Dictionary *pDictionary ;    // -> Dictionary object
   char *Word ;                 // -> Original Word
   char *Replacement ;          // -> Replacement Word (at least EntrySize in width)
   int   EntrySize ;            // Replacement word list entry size.
   int   ListSize ;             // Replacement word list entry max count.
   int   EasyCount ;            // Replacement word list entry count (easy).
   int   ListCount ;            // Replacement word list entry count (total).
   char *List ;                 // -> Replacement word list
} SPELLCHECK_PARMS, *PSPELLCHECK_PARMS ;

static WINDOWPROC Check_Processor ;
static WINDOWPROC Check_InitDlg ;
static WINDOWPROC Check_Control ;
static WINDOWPROC Check_Command ;
static WINDOWPROC Check_Skip ;
static WINDOWPROC Check_Accept ;
static WINDOWPROC Check_Insert ;
static WINDOWPROC Check_UnCaps ;
static WINDOWPROC Check_Replace ;
static WINDOWPROC Check_Escape ;
static WINDOWPROC Check_MeasureItem ;
static WINDOWPROC Check_DrawItem ;
static BOOL Check_Next ( HWND Window ) ;
static void Check_Reposition ( HWND Window ) ;

 
/****************************************************************************
 *                                                                          *
 *      Spell Checker                                                       *
 *                                                                          *
 ****************************************************************************/

extern void SpellChecker ( HWND Owner, Documento *pDocument, Dictionary *pDictionary ) {

  /**************************************************************************
   * Local Declarations                                                     *
   **************************************************************************/

   char Word [ENTRYSIZE] ;
   char Replacement [ENTRYSIZE] ;
   char List [ENTRYCOUNT] [ENTRYSIZE] ;

   SPELLCHECK_PARMS Parms = { Owner, pDocument, pDictionary, Word, Replacement, ENTRYSIZE, ENTRYCOUNT, 0, 0, (char*)List } ;

  /**************************************************************************
   * If there are misspelled words, summon the dialog to handle them.       *
   **************************************************************************/

   if ( Parms.pDocument->SpellCheck ( Parms.pDictionary, Parms.EntrySize, Parms.Word,
      Parms.ListSize, Parms.EntrySize, Parms.List, Parms.ListCount, Parms.EasyCount, TRUE ) )

      if ( WinDlgBox ( HWND_DESKTOP, Owner, Check_Processor, LibraryHandle, IDD_SPELL, &Parms ) == FALSE )

         return ;

  /**************************************************************************
   * Announce successful completion.                                        *
   **************************************************************************/

   ResourceString Message ( LibraryHandle, IDS_MESSAGE_SPELLDONE ) ;
   ResourceString Title1 ( LibraryHandle, IDS_TITLE1 ) ;
   MessageBox ( HWND_DESKTOP, Owner, PSZ(Message), PSZ(Title1), 
      IDD_MESSAGE_SPELLDONE, MB_ENTER | MB_HELP, LibraryHandle, IDS_MESSAGE_BOX_STRINGS ) ;
}

/****************************************************************************
 *                                                                          *
 *      "Spell Check" Dialog Processor                                      *
 *                                                                          *
 ****************************************************************************/

static MRESULT EXPENTRY Check_Processor ( HWND Window, ULONG msg, MPARAM mp1, MPARAM mp2 ) {

  /**************************************************************************
   * Dispatch the message according to the method table and return the      *
   *   result.  Any messages not defined above get handled by the system    *
   *   default dialog processor.                                            *
   **************************************************************************/

   static METHOD Methods [] = {
      { WM_INITDLG,     Check_InitDlg     },
      { WM_CONTROL,     Check_Control     },
      { WM_COMMAND,     Check_Command     },
      { WM_MEASUREITEM, Check_MeasureItem },
      { WM_DRAWITEM,    Check_DrawItem    },
   } ;

   #ifdef DEBUG_MESSAGES
      static int Indent = 0 ;
      LogMsgRcvd ( Indent, "SPELLCHECK", Window, msg, mp1, mp2 ) ;
      Indent += 2 ;
   #endif

   MRESULT Result = DispatchMessage ( Window, msg, mp1, mp2, Methods, sizeof(Methods)/sizeof(Methods[0]), Dialog_Processor ) ;

   #ifdef DEBUG_MESSAGES
      Indent -= 2 ;
      LogMsgDone ( Indent, "SPELLCHECK", msg, Result ) ;
   #endif

   return ( Result ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Initialize Dialog                                                   *
 *                                                                          *
 ****************************************************************************/

static void BuildAlternates ( char *Word, int &AltCount, char AltWords[][ENTRYSIZE] ) {
   char *p1 = strstr ( Word, "+" ) ;
   char *p2 = strstr ( Word, "-" ) ;
   char *p ( 0 ) ;
   if ( p1 && p2 && ( p1 < p2 ) ) {
      p = p1 ;
   } else if ( p1 && p2 && ( p1 > p2 ) ) {
      p = p2 ;
   } else if ( p1 ) {
      p = p1 ;
   } else if ( p2 ) { 
      p = p2 ;
   } /* endif */
   if ( p ) {
      char NewWord [ENTRYSIZE] ;
      if ( *p == '+' ) {
         memcpy ( NewWord, Word, p-Word ) ;
         NewWord[p-Word] = 0 ;
         strcat ( NewWord, p+1 ) ;
         BuildAlternates ( NewWord, AltCount, AltWords ) ;
      } else {
         memcpy ( AltWords[AltCount], Word, p-Word ) ;
         AltWords[AltCount++][p-Word] = 0 ;
         char *p3 = strstr ( p, "+" ) ;
         if ( p3 ) {
            if ( !memcmp ( p-(p3-p-1), p+1, (p3-p-1) ) ) {
               memcpy ( NewWord, Word, p-Word-(p3-p-1) ) ;
               NewWord[p-Word-(p3-p-1)] = 0 ;
               strcpy ( &NewWord[p-Word-(p3-p-1)], p3+1 ) ;
               BuildAlternates ( NewWord, AltCount, AltWords ) ;
            } else {
               Log ( "- extension rejected due to mismatch between '%0.*s' and '%0.*s'.",
                  (p3-p-1), p-(p3-p-1), (p3-p-1), p+1 ) ;
               strcpy ( AltWords[AltCount++], Word ) ;
            } /* endif */
         } else {
            Log ( "- extension rejected because no + clause found." ) ;
            strcpy ( AltWords[AltCount++], Word ) ;
         } /* endif */
      } /* endif */
   } else {
      strcpy ( AltWords[AltCount++], Word ) ;
   } /* endif */
}

static MRESULT APIENTRY Check_InitDlg ( HWND Window, ULONG msg, MPARAM mp1, MPARAM mp2 ) {

  /**************************************************************************
   * Get the dialog parameters.                                             *
   **************************************************************************/

   PSPELLCHECK_PARMS Parms = PSPELLCHECK_PARMS ( mp2 ) ;

   Sys_SetWindowData ( Window, Parms ) ;

  /**************************************************************************
   * Perform normal default actions.                                        *
   **************************************************************************/

   Dialog_Processor ( Window, msg, mp1, mp2 ) ;

  /**************************************************************************
   * Position the dialog to the upper right.                                *
   **************************************************************************/

   RECTL OwnerRectangle, DialogRectangle, DesktopRectangle ;
   WinQueryWindowRect ( Parms->Owner, &OwnerRectangle ) ;
   WinQueryWindowRect ( Window, &DialogRectangle ) ;
   WinQueryWindowRect ( HWND_DESKTOP, &DesktopRectangle ) ;
   POINTL Position ;
   Position.x = OwnerRectangle.xRight - ( DialogRectangle.xRight - DialogRectangle.xLeft ) - 10 ;
   Position.y = OwnerRectangle.yTop - ( DialogRectangle.yTop - DialogRectangle.yBottom ) - 10 ;
   WinMapWindowPoints ( Parms->Owner, HWND_DESKTOP, &Position, 1 ) ;
   if ( Position.x < DesktopRectangle.xLeft ) 
      Position.x = DesktopRectangle.xLeft ;
   if ( Position.y < DesktopRectangle.yBottom ) 
      Position.y = DesktopRectangle.yBottom ;
   if ( Position.x+(DialogRectangle.xRight-DialogRectangle.xLeft) > DesktopRectangle.xRight ) 
      Position.x = DesktopRectangle.xRight - (DialogRectangle.xRight-DialogRectangle.xLeft) ;
   if ( Position.y+(DialogRectangle.yTop-DialogRectangle.yBottom) > DesktopRectangle.yTop ) 
      Position.y = DesktopRectangle.yTop - (DialogRectangle.yTop-DialogRectangle.yBottom) ;
   WinSetWindowPos ( Window, 0, Position.x, Position.y, 0, 0, SWP_MOVE ) ;

  /**************************************************************************
   * Adjust for position of selected text in the document.                  *
   **************************************************************************/

   Check_Reposition ( Window ) ;

  /**************************************************************************
   * Modify the main list-box's style to make it ownerdraw.                 *
   **************************************************************************/

   HWND ComboBox = CHILD(Window,IDD_SPELL_NEWWORD) ;
   HWND ListBox = CHILD(ComboBox,CBID_LIST) ;
   ULONG Style = WinQueryWindowULong ( ListBox, QWL_STYLE ) ;
   Style |= LS_OWNERDRAW ;
   WinSetWindowULong ( ListBox, QWL_STYLE, Style ) ;

  /**************************************************************************
   * Expand the 'try harder' words in the replacement list.                 *
   **************************************************************************/

   char AltWords [ENTRYCOUNT] [ENTRYSIZE] ;
   int AltCount = 0 ;
   memset ( AltWords, 0, sizeof(AltWords) ) ;

   for ( int i=Parms->EasyCount; i<Parms->ListCount; i++ ) {
      char Word [ENTRYSIZE] ;
      strcpy ( Word, Parms->List + Parms->EntrySize * i ) ;
      BuildAlternates ( Word, AltCount, AltWords ) ;
   } /* endfor */

   qsort ( AltWords, AltCount, ENTRYSIZE, (int(_Optlink*)(const void*,const void*))strcmp ) ;

   for ( i=0; i<AltCount; i++ ) {
      if ( ( i<AltCount-1 ) && !strcmp ( AltWords[i], AltWords[i+1] ) ) {
         memcpy ( AltWords[i], AltWords[i+1], ENTRYSIZE*(ENTRYCOUNT-i-1) ) ;
         AltCount -- ; i -- ;
      } else {
         for ( int j=0; j<Parms->EasyCount; j++ ) {
            if ( !strcmp ( AltWords[i], Parms->List+Parms->EntrySize*j ) ) {
               memcpy ( AltWords[i], AltWords[i+1], ENTRYSIZE*(ENTRYCOUNT-i-1) ) ;
               AltCount -- ; i -- ;
               break ;
            } /* endif */
         } /* endfor */
      } /* endif */
   } /* endfor */

   for ( i=0; i<AltCount; i++ ) 
      strcpy ( Parms->List+Parms->EntrySize*(i+Parms->EasyCount), AltWords[i] ) ;

   Parms->ListCount = Parms->EasyCount + AltCount ;

  /**************************************************************************
   * Fill in the fields.                                                    *
   **************************************************************************/

   Sys_SetWindowText ( CHILD(Window,IDD_SPELL_OLDWORD), Parms->Word ) ;
   Sys_SetWindowText ( CHILD(Window,IDD_SPELL_NEWWORD), "" ) ;
   Sys_SetWindowText ( CHILD(Window,IDD_SPELL_ERR), "" ) ;

   Sys_SendMessage ( ComboBox, LM_DELETEALL, 0, 0 ) ;
   for ( i=0; i<Parms->ListCount; i++ ) {
      Sys_SendMessage ( ComboBox, LM_INSERTITEM,
         MPFROMSHORT ( LIT_END ), MPFROMP ( Parms->List + Parms->EntrySize * i ) ) ;
   } /* endfor */

   Sys_EnableWindow ( CHILD(Window,IDD_SPELL_REPLACE), FALSE ) ;

  /**************************************************************************
   * Return all's well.  No focus change.                                   *
   **************************************************************************/

   return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process Control Messages                                            *
 *                                                                          *
 ****************************************************************************/

static MRESULT APIENTRY Check_Control ( HWND Window, MESG, MPARAM1 mp1, MPARAM2 ) {

  /**************************************************************************
   * Find the instance data.                                                *
   **************************************************************************/

   PSPELLCHECK_PARMS Parms = PSPELLCHECK_PARMS ( Sys_GetWindowData ( Window ) ) ;

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

      case IDD_SPELL_NEWWORD: {

         switch ( Message ) {

            case CBN_ENTER: {

               SHORT Index = SHORT1FROMMR ( Sys_SendMessage ( CHILD(Window,IDD_SPELL_NEWWORD), LM_QUERYSELECTION, MPFROMSHORT(LIT_FIRST), 0 ) ) ;

               if ( Index == LIT_NONE )
                  break ;

               Sys_SendMessage ( CHILD(Window,IDD_SPELL_NEWWORD), LM_QUERYITEMTEXT,
                  MPFROM2SHORT ( Index, Parms->EntrySize ), MPFROMP ( Parms->Replacement ) ) ;

               Check_Replace ( Window, 0, 0, 0 ) ;

               break ; } /* endcase */

            case CBN_EFCHANGE: {
               Sys_GetWindowText ( CHILD(Window,IDD_SPELL_NEWWORD), Parms->Replacement, Parms->EntrySize ) ;
               Sys_EnableWindow ( CHILD(Window,IDD_SPELL_REPLACE), Parms->Replacement[0] != 0 ) ;
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
 *      Process commands received by the dialog.                            *
 *                                                                          *
 ****************************************************************************/

static MRESULT APIENTRY Check_Command ( HWND Window, MESG, MPARAM1 mp1, MPARAM2 mp2 ) {

  /**************************************************************************
   * Dispatch the message without a default message processor.              *
   **************************************************************************/

   static METHOD Methods [] = {
      { IDD_SPELL_SKIP,       Check_Skip       },
      { IDD_SPELL_ACCEPT,     Check_Accept     },
      { IDD_SPELL_INSERT,     Check_Insert     },
      { IDD_SPELL_UNCAPS,     Check_UnCaps     },
      { IDD_SPELL_REPLACE,    Check_Replace    },
      { DID_CANCEL,           Check_Escape     },
   } ;

   return ( DispatchMessage ( Window, SHORT1FROMMP(mp1), mp1, mp2, Methods, sizeof(Methods)/sizeof(Methods[0]), 0 ) ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process the Skip button.                                            *
 *                                                                          *
 ****************************************************************************/

static MRESULT APIENTRY Check_Skip ( HWND Window, MESG, MPARAM1, MPARAM2 ) {

  /**************************************************************************
   * Look for the next misspelled word.  Return if one was found.           *
   **************************************************************************/

   if ( Check_Next ( Window ) ) 
      return ( 0 ) ;

  /**************************************************************************
   * Dismiss the dialog with the good status.                               *
   **************************************************************************/

   Sys_EndDialog ( Window, TRUE ) ;

   return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process the Accept button.                                          *
 *                                                                          *
 ****************************************************************************/

static MRESULT APIENTRY Check_Accept ( HWND Window, MESG, MPARAM1, MPARAM2 ) {

  /**************************************************************************
   * Find the instance data.                                                *
   **************************************************************************/

   PSPELLCHECK_PARMS Parms = PSPELLCHECK_PARMS ( Sys_GetWindowData ( Window ) ) ;

  /**************************************************************************
   * Accept the current word into the temporary dictionary.                 *
   **************************************************************************/

   Parms->pDictionary->Accept ( Parms->Word ) ;

  /**************************************************************************
   * Look for the next misspelled word.  Return if one was found.           *
   **************************************************************************/

   if ( Check_Next ( Window ) ) 
      return ( 0 ) ;

  /**************************************************************************
   * Dismiss the dialog with the good status.                               *
   **************************************************************************/

   Sys_EndDialog ( Window, TRUE ) ;

   return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process the Insert button.                                          *
 *                                                                          *
 ****************************************************************************/

static MRESULT APIENTRY Check_Insert ( HWND Window, MESG, MPARAM1, MPARAM2 ) {

  /**************************************************************************
   * Find the instance data.                                                *
   **************************************************************************/

   PSPELLCHECK_PARMS Parms = PSPELLCHECK_PARMS ( Sys_GetWindowData ( Window ) ) ;

  /**************************************************************************
   * Insert the current word into the user's personal dictionary.           *
   **************************************************************************/

   Parms->pDictionary->Insert ( Parms->Word ) ;

  /**************************************************************************
   * Look for the next misspelled word.  Return if one was found.           *
   **************************************************************************/

   if ( Check_Next ( Window ) ) 
      return ( 0 ) ;

  /**************************************************************************
   * Dismiss the dialog with the good status.                               *
   **************************************************************************/

   Sys_EndDialog ( Window, TRUE ) ;

   return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process the UnCaps button.                                          *
 *                                                                          *
 ****************************************************************************/

static MRESULT APIENTRY Check_UnCaps ( HWND Window, MESG, MPARAM1, MPARAM2 ) {

  /**************************************************************************
   * Find the instance data.                                                *
   **************************************************************************/

   PSPELLCHECK_PARMS Parms = PSPELLCHECK_PARMS ( Sys_GetWindowData ( Window ) ) ;

  /**************************************************************************
   * Insert the current word into the user's personal dictionary.           *
   **************************************************************************/

   Parms->pDictionary->UnCaps ( Parms->Word ) ;

  /**************************************************************************
   * Look for the next misspelled word.  Return if one was found.           *
   **************************************************************************/

   if ( Check_Next ( Window ) ) 
      return ( 0 ) ;

  /**************************************************************************
   * Dismiss the dialog with the good status.                               *
   **************************************************************************/

   Sys_EndDialog ( Window, TRUE ) ;

   return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process the Replace button.                                         *
 *                                                                          *
 ****************************************************************************/

static MRESULT APIENTRY Check_Replace ( HWND Window, MESG, MPARAM1, MPARAM2 ) {

  /**************************************************************************
   * Find the instance data.                                                *
   **************************************************************************/

   PSPELLCHECK_PARMS Parms = PSPELLCHECK_PARMS ( Sys_GetWindowData ( Window ) ) ;

  /**************************************************************************
   * Get the replacement text.                                              *
   **************************************************************************/

   Sys_GetWindowText ( CHILD(Window,IDD_SPELL_NEWWORD), Parms->Replacement, Parms->EntrySize ) ;

   if ( Parms->Replacement[0] == 0 ) {
      ResourceString Message ( LibraryHandle, IDS_ERROR_SPELL_REPLACE ) ;
      Sys_SetWindowText ( CHILD(Window,IDD_SPELL_ERR), PSZ(Message) ) ;
      Sys_BeepNote ( ) ;
      Sys_SetFocus ( CHILD ( Window, IDD_SPELL_NEWWORD ) ) ;
      return ( 0 ) ;
   } /* endif */

  /**************************************************************************
   * Replace the selected text in the document.                             *
   **************************************************************************/

   Parms->pDocument->ReplaceSelectedText ( Parms->Replacement ) ;

  /**************************************************************************
   * Look for the next misspelled word.  Return if one was found.           *
   **************************************************************************/

   if ( Check_Next ( Window ) ) 
      return ( 0 ) ;

  /**************************************************************************
   * Dismiss the dialog with the good status.                               *
   **************************************************************************/

   Sys_EndDialog ( Window, TRUE ) ;

   return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process the Escape button.                                          *
 *                                                                          *
 ****************************************************************************/

static MRESULT APIENTRY Check_Escape ( HWND Window, MESG, MPARAM1, MPARAM2 ) {

  /**************************************************************************
   * Dismiss the dialog with the bad status.                                *
   **************************************************************************/

   Sys_EndDialog ( Window, FALSE ) ;

   return ( 0 ) ;
}

/****************************************************************************
 *                                                                          *
 *      Process the Measure Item message                                    *
 *                                                                          *
 ****************************************************************************/

static MRESULT APIENTRY Check_MeasureItem ( HWND, MESG, MPARAM1, MPARAM2 ) {
   return ( FALSE ) ;
}

/****************************************************************************
 *                                                                          *
 *      Process the Draw Item message                                       *
 *                                                                          *
 ****************************************************************************/

static MRESULT APIENTRY Check_DrawItem ( HWND Window, MESG, MPARAM1, MPARAM2 mp2 ) {

   PSPELLCHECK_PARMS Parms = PSPELLCHECK_PARMS ( Sys_GetWindowData ( Window ) ) ;
   POWNERITEM pOwnerItem = (POWNERITEM) mp2 ;

   char Text [100] ;
   Sys_SendMessage ( CHILD(Window,IDD_SPELL_NEWWORD), LM_QUERYITEMTEXT,
      MPFROM2SHORT ( pOwnerItem->idItem, sizeof(Text) ), MPFROMP ( Text ) ) ;

   long Background = GpiQueryColorIndex ( pOwnerItem->hps, 0, RGB_WHITE ) ;
   long Foreground = GpiQueryColorIndex ( pOwnerItem->hps, 0, ( pOwnerItem->idItem < Parms->EasyCount ) ? RGB_BLACK : 0x808080 ) ;
   WinDrawText ( pOwnerItem->hps, strlen(Text), Text, &pOwnerItem->rclItem, Foreground, Background, DT_LEFT | DT_TOP | DT_ERASERECT ) ;

   return ( MRFROMSHORT ( TRUE ) ) ;
}

/****************************************************************************
 *                                                                          *
 *      Look for the next misspelled word.                                  *
 *                                                                          *
 ****************************************************************************/

static BOOL Check_Next ( HWND Window ) {

   PSPELLCHECK_PARMS Parms = PSPELLCHECK_PARMS ( Sys_GetWindowData ( Window ) ) ;

   if ( Parms->pDocument->SpellCheck ( Parms->pDictionary, Parms->EntrySize, Parms->Word, 
      Parms->ListSize, Parms->EntrySize, Parms->List, Parms->ListCount, Parms->EasyCount, FALSE ) ) {

      Check_Reposition ( Window ) ;

      char AltWords [ENTRYCOUNT] [ENTRYSIZE] ;
      int AltCount = 0 ;
      memset ( AltWords, 0, sizeof(AltWords) ) ;

      for ( int i=Parms->EasyCount; i<Parms->ListCount; i++ ) {
         char Word [ENTRYSIZE] ;
         strcpy ( Word, Parms->List + Parms->EntrySize * i ) ;
         BuildAlternates ( Word, AltCount, AltWords ) ;
      } /* endfor */

      qsort ( AltWords, AltCount, ENTRYSIZE, (int(_Optlink*)(const void*,const void*))strcmp ) ;

      for ( i=0; i<AltCount; i++ ) {
         if ( ( i<AltCount-1 ) && !strcmp ( AltWords[i], AltWords[i+1] ) ) {
            memcpy ( AltWords[i], AltWords[i+1], ENTRYSIZE*(ENTRYCOUNT-i-1) ) ;
            AltCount -- ; i -- ;
         } else {
            for ( int j=0; j<Parms->EasyCount; j++ ) {
               if ( !strcmp ( AltWords[i], Parms->List+Parms->EntrySize*j ) ) {
                  memcpy ( AltWords[i], AltWords[i+1], ENTRYSIZE*(ENTRYCOUNT-i-1) ) ;
                  AltCount -- ; i -- ;
                  break ;
               } /* endif */
            } /* endfor */
         } /* endif */
      } /* endfor */

      for ( i=0; i<AltCount; i++ ) 
         strcpy ( Parms->List+Parms->EntrySize*(i+Parms->EasyCount), AltWords[i] ) ;

      Parms->ListCount = Parms->EasyCount + AltCount ;

      Sys_SetWindowText ( CHILD(Window,IDD_SPELL_OLDWORD), Parms->Word ) ;
      Sys_SetWindowText ( CHILD(Window,IDD_SPELL_NEWWORD), "" ) ;
      Sys_SetWindowText ( CHILD(Window,IDD_SPELL_ERR), "" ) ;

      Sys_SendMessage ( CHILD(Window,IDD_SPELL_NEWWORD), LM_DELETEALL, 0, 0 ) ;
      for ( i=0; i<Parms->ListCount; i++ ) {
         Sys_SendMessage ( CHILD(Window,IDD_SPELL_NEWWORD), LM_INSERTITEM,
            MPFROMSHORT ( LIT_END ), MPFROMP ( Parms->List + Parms->EntrySize * i ) ) ;
      } /* endfor */

      Sys_EnableWindow ( CHILD(Window,IDD_SPELL_REPLACE), FALSE ) ;

      return ( TRUE ) ;

   } /* endif */

   return ( FALSE ) ;
}

/****************************************************************************
 *                                                                          *
 *      Reposition the Dialog if necessary to uncover selected text.        *
 *                                                                          *
 ****************************************************************************/

static void Check_Reposition ( HWND Window ) {

   PSPELLCHECK_PARMS Parms = PSPELLCHECK_PARMS ( Sys_GetWindowData ( Window ) ) ;

   RECTL Text ;
   Sys_SendMessage ( Parms->Owner, WM_QUERY_TEXT_RECTANGLE, MPFROMP(&Text), 0 ) ;
   WinMapWindowPoints ( Parms->Owner, HWND_DESKTOP, PPOINTL(&Text), 2 ) ;

   RECTL Desktop ;
   WinQueryWindowRect ( HWND_DESKTOP, &Desktop ) ;

   RECTL Dialog ;
   WinQueryWindowRect ( Window, &Dialog ) ;
   WinMapWindowPoints ( Window, HWND_DESKTOP, PPOINTL(&Dialog), 2 ) ;

   // If dialog covers the highlighted text at any point . . .
   if (  ( Dialog.xLeft   <= Text.xRight )
      && ( Dialog.xRight  >= Text.xLeft )
      && ( Dialog.yBottom <= Text.yTop )
      && ( Dialog.yTop    >= Text.yBottom ) ) {

      // Set default position first.
      RECTL OwnerRectangle, DialogRectangle ;
      WinQueryWindowRect ( Parms->Owner, &OwnerRectangle ) ;
      WinQueryWindowRect ( Window, &DialogRectangle ) ;
      POINTL Position ;
      Position.x = OwnerRectangle.xRight - ( DialogRectangle.xRight - DialogRectangle.xLeft ) - 10 ;
      Position.y = OwnerRectangle.yTop - ( DialogRectangle.yTop - DialogRectangle.yBottom ) - 10 ;
      WinMapWindowPoints ( Parms->Owner, HWND_DESKTOP, &Position, 1 ) ;
      WinSetWindowPos ( Window, 0, Position.x, Position.y, 0, 0, SWP_MOVE ) ;

      // If there is room below, relocate the dialog there.
      if ( Text.yBottom - Desktop.yBottom > Dialog.yTop - Dialog.yBottom ) {
         Position.y = ( ( Text.yBottom - Desktop.yBottom ) - ( Dialog.yTop - Dialog.yBottom ) ) / 2 + Desktop.yBottom ;
         WinSetWindowPos ( Window, 0, Position.x, Position.y, 0, 0, SWP_MOVE ) ;

      // Else, if there is room above, relocate the dialog there.
      } else if ( Desktop.yTop - Text.yTop > Dialog.yTop - Dialog.yBottom ) {
         Position.y = ( ( Desktop.yTop - Text.yTop ) - ( Dialog.yTop - Dialog.yBottom ) ) / 2 + Text.yTop ;
         WinSetWindowPos ( Window, 0, Position.x, Position.y, 0, 0, SWP_MOVE ) ;

      // Else, if there is room to the right, relocate there.
      } else if ( Desktop.xRight - Text.xRight > Dialog.xRight - Dialog.xLeft ) {
         Position.x = ( ( Desktop.xRight - Text.xRight ) - ( Dialog.xRight - Dialog.xLeft ) ) / 2 + Text.xRight ;
         WinSetWindowPos ( Window, 0, Position.x, Position.y, 0, 0, SWP_MOVE ) ;

      // Else, if there is room to the left, relocate there.
      } else if ( Text.xLeft - Desktop.xLeft > Dialog.xRight - Dialog.xLeft ) {
         Position.x = ( ( Text.xLeft - Desktop.xLeft ) - ( Dialog.xRight - Dialog.xLeft ) ) / 2 + Desktop.xLeft ;
         WinSetWindowPos ( Window, 0, Position.x, Position.y, 0, 0, SWP_MOVE ) ;

      // Else, don't relocate at all.  Just suffer.
      } else {

      } /* endif */

   } /* endif */
}
