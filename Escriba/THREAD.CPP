/***************************************************************** THREAD.CPP
 *                                                                          *
 *                       Thread Starter Function                            *
 *                                                                          *
 ****************************************************************************/

#include "System.h"

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "Debug.h"
#include "Except.h"
#include "Thread.h"

// #define DEBUG

 
/****************************************************************************
 *                                                                          *
 *                         Definitions & Declarations                       *
 *                                                                          *
 ****************************************************************************/

  // Type Definitions

typedef struct {
   char *Name ;
   void (*Function)(void*) ;
   void *Parameter ;
   int PriorityClass ;
   int PriorityDelta ;
} THREAD_PARMS, *PTHREAD_PARMS ;


  // Function Prototypes

static void _Optlink Thread ( void *Parameter ) ;

 
/****************************************************************************
 *                                                                          *
 *                         Generic Thread Starter                           *
 *                                                                          *
 ****************************************************************************/

extern TID StartThread ( char *Name, void(*Function)(void*), int StackSize, void *Parameter, int Class, int Delta ) {

   PTHREAD_PARMS Parms = new THREAD_PARMS ;
   Parms->Name = new char [ strlen(Name) + 1 ] ;
   strcpy ( Parms->Name, Name ) ;
   Parms->Function = Function ;
   Parms->Parameter = Parameter ;
   Parms->PriorityClass = Class ;
   Parms->PriorityDelta = Delta ;

   TID ThreadID = _beginthread ( Thread, 0, StackSize, Parms ) ;

   if ( ThreadID < 1 )
      Log ( "ERROR: Unable to start thread '%s'.", Name ) ;

   return ( ThreadID ) ;
}
 
/****************************************************************************
 *                                                                          *
 *                            Generic Thread                                *
 *                                                                          *
 ****************************************************************************/

static void _Optlink Thread ( void *Parameter ) {

  /**************************************************************************
   * Get parameters.                                                        *
   **************************************************************************/

   PTHREAD_PARMS Parms = PTHREAD_PARMS ( Parameter ) ;

  /**************************************************************************
   * Set a default exception filter.                                        *
   **************************************************************************/

   REGISTER_EXCEPTION_HANDLER(0)

  /**************************************************************************
   * Register self.                                                         *
   **************************************************************************/

   Sys_RegisterThread ( ) ;

  /**************************************************************************
   * Adjust the thread priority.                                            *
   **************************************************************************/

   DosSetPriority ( PRTYS_THREAD, Parms->PriorityClass, Parms->PriorityDelta, 0 ) ;

  /**************************************************************************
   * Execute the function requested.                                        *
   **************************************************************************/

   #ifdef DEBUG
      Log ( "Thread '%s' starting.", Parms->Name ) ;
   #endif

   Parms->Function ( Parms->Parameter ) ;

   #ifdef DEBUG
      Log ( "Thread '%s' ending.", Parms->Name ) ;
   #endif

  /**************************************************************************
   * Release the memory allocated for the thread parameters.                *
   **************************************************************************/

   delete [] Parms->Name ;
   delete Parms ;

  /**************************************************************************
   * De-register self.                                                      *
   **************************************************************************/

   Sys_DeregisterThread ( ) ;

  /**************************************************************************
   * Unhook the exception handler.                                          *
   **************************************************************************/

   UNREGISTER_EXCEPTION_HANDLER(0)
}

