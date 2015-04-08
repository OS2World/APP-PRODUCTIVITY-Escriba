// Class PROCESS: Encapsulates the startup/shutdown logic for a OS/2-PM process.

#include "System.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Debug.h"
#include "Escriba.h"
#include "Process.h"
#include "ReString.h"
#include "Support.h"

// #define DEBUG


// Constructor

Process::Process ( char *name, HWND Owner, HMODULE, long QueueSize, unsigned short CodePage ) : Name(0), Anchor(0), Queue(0) {

   #ifdef DEBUG
      Log ( "Process::Process(%s) Saving name.", name ) ;
   #endif

   // Save the process name, if given.
   if ( name ) {
      Name = PCHAR ( malloc ( strlen(name)+1 ) ) ;
      strcpy ( Name, name ) ;
   } /* endif */

   #ifdef DEBUG
      Log ( "Process::Process(%s) Creating anchor block.", name ) ;
   #endif

   // Create thread anchor block.
   Anchor = WinInitialize ( 0 ) ;
   if ( Anchor == 0 ) {
      // This error message cannot be displayed, only logged, because we have no queue yet.
      Log ( "ERROR: Unable to initialize for windowing." ) ;
      return ;
   } /* endif */

   #ifdef DEBUG
      Log ( "Process::Process(%s) Creating message queue.", name ) ;
   #endif

   // Create thread message queue.
   Queue = WinCreateMsgQueue ( Anchor, QueueSize ) ;
   if ( Queue == 0 ) {
      // This error message cannot be displayed, only logged, because we have no queue yet.
      Log ( "ERROR: Unable to create process message queue." ) ;
      WinTerminate ( Anchor ) ;  Anchor = 0 ;
      return ;
   } /* endif */

   // Establish message queue codepage.  Give warning if unable to do so.
   if ( CodePage ) {
      #ifdef DEBUG
         Log ( "Process::Process(%s) Setting codepage %i.", name, CodePage ) ;
      #endif
      SetQueueCodepage ( Owner, Queue, CodePage ) ;
   } /* endif */

   #ifdef DEBUG
      Log ( "Process::Process(%s) Done.  Anchor %08p, Queue %08p.", name, Anchor, Queue ) ;
   #endif

} /* endmethod */


// Destructor

Process::~Process ( ) {

   #ifdef DEBUG
      Log ( "Process::~Process(%s) Started.  Destroying message queue %08p.", Name, Queue ) ;
   #endif

   if ( Queue ) { 
      if ( !WinDestroyMsgQueue ( Queue ) ) {
         char Message [512] ;
         Log ( "Process::~Process(%s) WinDestroyMsgQueue failed.  %s", Name, InterpretWinError(0,Message) ) ;
      } /* endif */
   } /* endif */

   #ifdef DEBUG
      Log ( "Process::~Process(%s) Destroying anchor block %08p.", Name, Anchor ) ;
   #endif

   if ( Anchor ) { 
      if ( !WinTerminate ( Anchor ) ) {
         char Message [512] ;
         Log ( "Process::~Process(%s) WinTerminate failed.  %s", Name, InterpretWinError(0,Message) ) ;
      } /* endif */
   } /* endif */

   #ifdef DEBUG
      Log ( "Process::~Process(%s) Releasing name.", Name ) ;
   #endif

   if ( Name ) free ( Name ) ;

   #ifdef DEBUG
      Log ( "Process::~Process() Done." ) ;
   #endif

} /* endmethod */


// SetQueueCodepage: Utility function

extern BOOL SetQueueCodepage ( HWND Owner, HMQ Queue, unsigned short CodePage ) {

   // Determine which codepages have been prepared.
   ULONG CpList [8], ListSize ;
   APIRET Result = DosQueryCp ( sizeof(CpList), CpList, &ListSize ) ;
   if ( Result ) {
      ResourceString Title ( LibraryHandle, IDS_TITLE ) ;
      ResourceString Format ( LibraryHandle, IDS_WARNING_DOSQUERYCP ) ;
      char Message [200] ;
      sprintf ( Message, PCHAR(Format), CodePage, Result ) ;
      MessageBox ( HWND_DESKTOP, Owner, PSZ(Message), PSZ(Title), 0, MB_ENTER | MB_WARNING, LibraryHandle, IDS_MESSAGE_BOX_STRINGS ) ;
      return ( FALSE ) ;
   } /* endif */

   // Is the selected codepage in the list?
   for ( int i=0; i<ListSize/sizeof(CpList[0]); i++ ) 
      if ( CpList[i] == CodePage ) 
         break ;

   // If not, return an error.
   if ( i >= ListSize/sizeof(CpList[0]) ) {
      ResourceString Title ( LibraryHandle, IDS_TITLE ) ;
      ResourceString RequestedPage ( LibraryHandle, IDS_CODEPAGES + CodePage ) ;
      ResourceString DefaultPage ( LibraryHandle, IDS_CODEPAGES + CpList[0] ) ;
      ResourceString Format ( LibraryHandle, IDS_WARNING_CODEPAGENOTACTIVE ) ;
      char Message [200] ;
      sprintf ( Message, PCHAR(Format), CodePage, PCHAR(RequestedPage), CpList[0], PCHAR(DefaultPage) ) ;
      MessageBox ( HWND_DESKTOP, Owner, PSZ(Message), PSZ(Title), 0, MB_ENTER | MB_WARNING, LibraryHandle, IDS_MESSAGE_BOX_STRINGS ) ;
      return ( FALSE ) ;
   } /* endif */

   // Try to set the codepage.  Abort if error.
   if ( !WinSetCp ( Queue, CodePage ) ) {
      ResourceString Title ( LibraryHandle, IDS_TITLE ) ;
      ResourceString Format ( LibraryHandle, IDS_WARNING_BADCODEPAGE ) ;
      char Message [200] ;
      sprintf ( Message, PCHAR(Format), CodePage ) ;
      MessageBox ( HWND_DESKTOP, Owner, PSZ(Message), PSZ(Title), 0, MB_ENTER | MB_WARNING, LibraryHandle, IDS_MESSAGE_BOX_STRINGS ) ;
      return ( FALSE ) ;
   } /* endif */

   return ( TRUE ) ;
}
