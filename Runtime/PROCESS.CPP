// Class PROCESS: Encapsulates the startup/shutdown logic for a OS/2-PM process.

#include "System.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Debug.h"
#include "Process.h"

// #define DEBUG


// Constructor

Process::Process ( char *name, long QueueSize ) : Name(0), Anchor(0), Queue(0) {

   #ifdef DEBUG
      Log ( "Process::Process(%s) Saving name.", name ) ;
   #endif

   // Save the process name, if given.
   if ( name ) {
      Name = new char [ strlen(name)+1 ] ;
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

   if ( Name ) 
      delete [] Name, Name = 0 ;

   #ifdef DEBUG
      Log ( "Process::~Process() Done." ) ;
   #endif

} /* endmethod */

