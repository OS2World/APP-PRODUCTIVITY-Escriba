/******************************************************************* FORMAT.H
 *                                                                          *
 *                     Document Formatting Functions                        *
 *                                                                          *
 ****************************************************************************/

#ifndef FORMAT_H
#define FORMAT_H

#include <stdlib.h>

#include "System.h"

#include "CirQueue.h"
#include "Event.h"
#include "State.h"

enum {
   DOCCMD_NOP = 0,              // Dummy function.                              No parameters.
   DOCCMD_INSERT_TOGGLE,        // Toggle insert mode.                          No parameters.
   DOCCMD_INSERT_CHAR,          // Insert character before cursor.              Parm: Ext char code.
   DOCCMD_INSERT_STRING,        // Insert string before cursor.                 Parms: String & Length.
   DOCCMD_CLEAR,                // Delete selected text.                        No parameters.
   DOCCMD_PASTE,                // Delete selected text & paste from clipboard. No parameters.
   DOCCMD_UNDO,                 // Undo one change level.                       No parameters.
   DOCCMD_REDO,                 // Redo one change level.                       No parameters.
   DOCCMD_MAX
} ;

struct CommandQueueEntry {
   int Command ;
   void *Parameter ;
   int Length ;
} ;

class Documento ;

class Supervisor {
   private:
      BOOL      Active ;
      BOOL      BlockRequest ;          // Set TRUE to request format block.
      Event     BlockPermit ;           // Posted when block request granted.
      Event     BlockDone ;             // Posted when block to be released.
      CircularQueue Commands ;          // Command Queue
      Event     Done ;                  // Event posted when Request complete.
      BOOL      EditRequest ;           // Flag: Set TRUE to request edit permission.
      Event     EditPermit ;            // Event posted when Edit permission granted.
      Event     EditDone ;              // Event posted when Edit complete.
      BOOL      FlushRequest ;
      BOOL      PageLocked ;
      Documento *pDocument ;
      Event     Request ;
      TID       Thread ;
      Event     ThreadDone ;

   public:

      // The following functions are called by the C++ compiler.

      Supervisor ( Documento *pdocument ) ;
      ~Supervisor ( ) ;

      // The following functions are called by the Window/Document thread.

      void       EnqueueInsertToggle ( ) ;
      void       EnqueueKey ( int KeyCode ) ;
      void       EnqueueString ( char *String, int Length ) ;
      void       EnqueueClear ( ) ;
      void       EnqueuePaste ( ) ;
      void       EnqueueUndo ( ) ;
      void       EnqueueRedo ( ) ;
      void       FlushCommandQueue ( ) ;

      void       Block ( ) ;
      void       FormatPage ( BOOL Wait=FALSE ) ;
      void       Unblock ( ) ;

      void       PageLockRelease ( ) ;
      void       PageLockRequest ( ) ;

      void       CommenceEdit ( ) ;
      void       CompleteEdit ( ) ;

        // The following functions are only called by the formatter thread.

      BOOL       AllowBlock ( ) ;
      BOOL       AllowEdit ( ) ;
      BOOL       CommandsQueued ( ) ;
      BOOL       ExecuteCommands ( ) ;
      BOOL       FlushCommands ( ) ;
      void       FormatFirstDirtyPage ( BOOL SetColumn=FALSE ) ;
      BOOL       Query_Active ( ) { return ( Active ) ; }
      Event     *Query_ThreadDone ( ) { return ( &ThreadDone ) ; }
      void       WaitForRequest ( ) ;

      friend void Supervisor_Thread ( void *Parameter ) ;
} ;

#endif
