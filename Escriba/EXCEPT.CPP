/**************************************************************** EXCEPT.CPP
 *                                                                         *
 *                            Exception Handler                            *
 *                                                                         *
 *  This rather simple exception handler includes a just-as-simple stack   *
 *  walker, which comes handy sometimes when debugging exceptions.  A far  *
 *  better one can be constructed with the use of Watcom's prologue and    *
 *  epilogue function calls (/ee /ep /en).                                 *
 *                                                                         *
 *  NOTE: For this to work reasonably reliably with the Watcom compiler,   *
 *    the compiler option /of+ should be specified, so as to enforce the   *
 *    usage of stack frames.                                               *
 *                                                                         *
 ***************************************************************************/

#include "System.h"

#include <builtin.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Debug.h"
#include "Escriba.h"
#include "Except.h"
#include "ReString.h"
#include "Support.h"
 
#define USEMESSAGEBOX

/***************************************************************************
 *                                                                         *
 *  Exception Handler for OS/2                                             *
 *                                                                         *
 ***************************************************************************/

#ifdef __OS2__

extern "C" APIRET APIENTRY DosQueryModFromEIP ( HMODULE *phMod, ULONG *pObjNum,
   ULONG BuffLen, PCHAR pBuff, ULONG *pOffset, PVOID Address );

static APIRET GetModuleOffset ( PVOID Address, char *Name, int NameSize, ULONG &Object, ULONG &Offset ) {
   HMODULE hModule ( 0 ) ;
   return ( DosQueryModFromEIP ( &hModule, &Object, NameSize, Name, &Offset, Address ) ) ;
}

extern BOOL BreakKey = FALSE;

static void RegisterDump ( int ThreadNumber, PCONTEXTRECORD pContextRecord ) {

   // Get failing module name.
   ULONG ObjectNumber ( 0 ), Offset ( 0 ) ;
   char ModuleName [ CCHMAXPATH + 1 ] = { 0 } ;
   GetModuleOffset ( (PVOID)pContextRecord->ctx_RegEip, ModuleName, sizeof(ModuleName), ObjectNumber, Offset ) ;

   // Log what was found.
   Log ( "  Module '%s', Object %i, Offset %08lX.", ModuleName, ObjectNumber+1, Offset ) ;
   Log ( "  Thread %i.", ThreadNumber ) ;
   Log ( "  The registers were as follows:" ) ;
   Log ( "    AX:%08lX  BX:%08lX  CX:%08lX  DX:%08lX",
      pContextRecord->ctx_RegEax, pContextRecord->ctx_RegEbx,
      pContextRecord->ctx_RegEcx, pContextRecord->ctx_RegEdx ) ;
   Log ( "    FL:%08lX  DI:%08lX  SI:%08lX  BP:%08lX",
      pContextRecord->ctx_EFlags, pContextRecord->ctx_RegEdi,
      pContextRecord->ctx_RegEsi, pContextRecord->ctx_RegEbp ) ;
   Log ( "    CS:%08lX  IP:%08lX  SS:%08lX  SP:%08lX",
      pContextRecord->ctx_SegCs,  pContextRecord->ctx_RegEip,
      pContextRecord->ctx_SegSs,  pContextRecord->ctx_RegEsp ) ;
   Log ( "    DS:%08lX  ES:%08lX  FS:%08lX  GS:%08lX",
      pContextRecord->ctx_SegDs,  pContextRecord->ctx_SegEs,
      pContextRecord->ctx_SegFs,  pContextRecord->ctx_SegGs ) ;
}

static void StackTrace ( PCONTEXTRECORD pContextRecord ) {
   if ( pContextRecord->ctx_RegEbp >= pContextRecord->ctx_RegEsp) {
      PLONG Base = (PLONG)pContextRecord->ctx_RegEbp ;
      while ( Base && ( *Base > (LONG)Base ) ) {
         ULONG ObjectNumber ( 0 ), Offset ( 0 ) ;
         char ModuleName [ CCHMAXPATH + 1 ] = { 0 } ;
         if ( !GetModuleOffset ( (PVOID)(*(Base+1)), ModuleName, sizeof(ModuleName), ObjectNumber, Offset ) )
            Log ( "  Calling function was at %08lX (%s,%i:%08X).", *(Base+1), ModuleName, ObjectNumber+1, Offset ) ;
         else 
            break ;
         Base = (PLONG)*Base ;
      } /* endwhile */
   } /* endif */
}

static void DisplayMessage ( ) {
   #ifdef USEMESSAGEBOX
   ResourceString Title ( LibraryHandle, IDS_TITLE ) ;
   ResourceString Exception ( LibraryHandle, IDS_ERROR_EXCEPTION ) ;
   char LogFile [CCHMAXPATH] ;
   strcpy ( LogFile, HomePath ) ;
   strcat ( LogFile, "\\" LOGFILE ) ;
   static char Message [500] ;
   sprintf ( Message, PCHAR(Exception), LogFile ) ;
   Sys_MessageBox ( HWND_DESKTOP, HWND_DESKTOP, PSZ(Message), PSZ(Title),
      0, MB_ENTER | MB_ICONEXCLAMATION ) ;
   #endif // USEMESSAGEBOX
}

extern ULONG APIENTRY ExceptionHandler (
   PEXCEPTIONREPORTRECORD pExceptionReportRecord,
   PEXCEPTIONREGISTRATIONRECORD,
   PCONTEXTRECORD pContextRecord,
   PVOID 
) {
   // Local Declarations
   int ThreadNumber = Sys_ThreadNumber(*_threadid) ;
   static BOOL Active = FALSE ;

   // Signal Exceptions
   if ( pExceptionReportRecord->ExceptionNum == XCPT_SIGNAL ) {
      if ( pExceptionReportRecord->ExceptionInfo[0] == XCPT_SIGNAL_INTR ) {
         Sys_Beep ( 800, 100 );
         BreakKey = TRUE;
         Log ( "Received interrupt signal." );
         return ( XCPT_CONTINUE_EXECUTION );
      } /* endif */
      if ( pExceptionReportRecord->ExceptionInfo[0] == XCPT_SIGNAL_BREAK ) {
         Sys_Beep ( 800, 100 );
         BreakKey = TRUE;
         Log ( "Received break signal." );
         return ( XCPT_CONTINUE_EXECUTION );
      } /* endif */
      if ( pExceptionReportRecord->ExceptionInfo[0] == XCPT_SIGNAL_KILLPROC ) {
         Log ( "Received KILLPROC signal." );
         return ( XCPT_CONTINUE_SEARCH );
      } /* endif */
   } /* endif */

   // Memory access violation.
   if ( pExceptionReportRecord->ExceptionNum == XCPT_ACCESS_VIOLATION ) {
      if ( Active ) {
         Log ( "ABORT: Recursion within exception handler." ) ;
         DosExit ( EXIT_PROCESS, 0 ) ;
      } /* endif */
      Active = TRUE ;
      Log (
        "ABORT: %s%s%s%s%s%sviolation trying to access address/selector %08lX.",
        ( pExceptionReportRecord->ExceptionInfo[0] & XCPT_READ_ACCESS ) ? "Read " : "",
        ( pExceptionReportRecord->ExceptionInfo[0] & XCPT_WRITE_ACCESS ) ? "Write " : "",
        ( pExceptionReportRecord->ExceptionInfo[0] & XCPT_EXECUTE_ACCESS ) ? "Execute " : "",
        ( pExceptionReportRecord->ExceptionInfo[0] & XCPT_SPACE_ACCESS ) ? "Space " : "",
        ( pExceptionReportRecord->ExceptionInfo[0] & XCPT_LIMIT_ACCESS ) ? "Limit " : "",
        ( pExceptionReportRecord->ExceptionInfo[0] & XCPT_UNKNOWN_ACCESS ) ? "Unknown " : "",
        pExceptionReportRecord->ExceptionInfo[1]
      ) ;

      RegisterDump ( ThreadNumber, pContextRecord ) ;
      StackTrace ( pContextRecord ) ;
      DisplayMessage ( ) ;
      DosExit ( EXIT_PROCESS, 0 ) ;
   } /* endif */

   // Privileged Instruction violation.
   if ( pExceptionReportRecord->ExceptionNum == XCPT_PRIVILEGED_INSTRUCTION ) {
      if ( Active ) {
         Log ( "ABORT: Recursion within exception handler." ) ;
         DosExit ( EXIT_PROCESS, 0 ) ;
      } /* endif */
      Active = TRUE ;
      Log ( "ABORT: A privileged instruction was encountered." ) ;
      RegisterDump ( ThreadNumber, pContextRecord ) ;
      StackTrace ( pContextRecord ) ;
      DisplayMessage ( ) ;
      DosExit ( EXIT_PROCESS, 0 ) ;
   } /* endif */

   // Illegal instruction violation.
   if ( pExceptionReportRecord->ExceptionNum == XCPT_ILLEGAL_INSTRUCTION ) {
      if ( Active ) {
         Log ( "ABORT: Recursion within exception handler." ) ;
         DosExit ( EXIT_PROCESS, 0 ) ;
      } /* endif */
      Active = TRUE ;
      Log ( "ABORT: An illegal instruction was encountered." ) ;
      RegisterDump ( ThreadNumber, pContextRecord ) ;
      StackTrace ( pContextRecord ) ;
      DisplayMessage ( ) ;
      DosExit ( EXIT_PROCESS, 0 ) ;
   } /* endif */

   // Integer Divide by Zero violation.
   if ( pExceptionReportRecord->ExceptionNum == XCPT_INTEGER_DIVIDE_BY_ZERO ) {
      if ( Active ) {
         Log ( "ABORT: Recursion within exception handler." ) ;
         DosExit ( EXIT_PROCESS, 0 ) ;
      } /* endif */
      Active = TRUE ;
      Log ( "ABORT: An integer divide-by-zero error has occurred." ) ;
      RegisterDump ( ThreadNumber, pContextRecord ) ;
      StackTrace ( pContextRecord ) ;
      DisplayMessage ( ) ;
      DosExit ( EXIT_PROCESS, 0 ) ;
   } /* endif */

   // Integer Overflow violation.
   if ( pExceptionReportRecord->ExceptionNum == XCPT_INTEGER_OVERFLOW ) {
      if ( Active ) {
         Log ( "ABORT: Recursion within exception handler." ) ;
         DosExit ( EXIT_PROCESS, 0 ) ;
      } /* endif */
      Active = TRUE ;
      Log ( "ABORT: An integer overflow has occurred." ) ;
      RegisterDump ( ThreadNumber, pContextRecord ) ;
      StackTrace ( pContextRecord ) ;
      DisplayMessage ( ) ;
      DosExit ( EXIT_PROCESS, 0 ) ;
   } /* endif */

   // Floating Point Divide by Zero violation.
   if ( pExceptionReportRecord->ExceptionNum == XCPT_FLOAT_DIVIDE_BY_ZERO ) {
      if ( Active ) {
         Log ( "ABORT: Recursion within exception handler." ) ;
         DosExit ( EXIT_PROCESS, 0 ) ;
      } /* endif */
      Active = TRUE ;
      Log ( "ABORT: A floating point divide by zero error has occurred." ) ;
      RegisterDump ( ThreadNumber, pContextRecord ) ;
      StackTrace ( pContextRecord ) ;
      DisplayMessage ( ) ;
      DosExit ( EXIT_PROCESS, 0 ) ;
   } /* endif */

   // Floating Point Overflow violation.
   if ( pExceptionReportRecord->ExceptionNum == XCPT_FLOAT_OVERFLOW ) {
      if ( Active ) {
         Log ( "ABORT: Recursion within exception handler." ) ;
         DosExit ( EXIT_PROCESS, 0 ) ;
      } /* endif */
      Active = TRUE ;
      Log ( "ABORT: A floating point overflow error has occurred." ) ;
      RegisterDump ( ThreadNumber, pContextRecord ) ;
      StackTrace ( pContextRecord ) ;
      DisplayMessage ( ) ;
      DosExit ( EXIT_PROCESS, 0 ) ;
   } /* endif */

   // Floating Point Underflow violation.
   if ( pExceptionReportRecord->ExceptionNum == XCPT_FLOAT_UNDERFLOW ) {
      if ( Active ) {
         Log ( "ABORT: Recursion within exception handler." ) ;
         DosExit ( EXIT_PROCESS, 0 ) ;
      } /* endif */
      Active = TRUE ;
      Log ( "ABORT: A floating point underflow error has occurred." ) ;
      RegisterDump ( ThreadNumber, pContextRecord ) ;
      StackTrace ( pContextRecord ) ;
      DisplayMessage ( ) ;
      DosExit ( EXIT_PROCESS, 0 ) ;
   } /* endif */

   switch ( pExceptionReportRecord->ExceptionNum ) {
      case XCPT_GUARD_PAGE_VIOLATION:
//       Log ( "ExceptionHandler: Guard page violation passed on to system." ) ;
         break;
      case XCPT_UNABLE_TO_GROW_STACK:
         Log ( "ExceptionHandler: Unable to grow stack violation passed on to system." ) ;
         break;
      case XCPT_DATATYPE_MISALIGNMENT:
         Log ( "ExceptionHandler: Datatype misalignment violation passed on to system." ) ;
         break;
      case XCPT_BREAKPOINT:
         Log ( "ExceptionHandler: Breakpoint violation passed on to system." ) ;
         break;
      case XCPT_SINGLE_STEP:
         Log ( "ExceptionHandler: Singlestep violation passed on to system." ) ;
         break;
      case XCPT_FLOAT_DENORMAL_OPERAND:
         Log ( "ExceptionHandler: Floating point denormal operand violation passed on to system." ) ;
         break;
      case XCPT_FLOAT_INEXACT_RESULT:
         Log ( "ExceptionHandler: Floating point inexact result violation passed on to system." ) ;
         break;
      case XCPT_FLOAT_INVALID_OPERATION:
         Log ( "ExceptionHandler: Floating point invalid operation violation passed on to system." ) ;
         break;
      case XCPT_FLOAT_STACK_CHECK:
         Log ( "ExceptionHandler: Floating point stack check violation passed on to system." ) ;
         break;
      case XCPT_IN_PAGE_ERROR:
         Log ( "ExceptionHandler: In-Page violation passed on to system." ) ;
         break;
      case XCPT_PROCESS_TERMINATE:
//       Log ( "ExceptionHandler: Process Terminate violation passed on to system." ) ;
         break;
      case XCPT_ASYNC_PROCESS_TERMINATE:
//       Log ( "ExceptionHandler: Async Process Terminate violation passed on to system.  Terminator TID %i.", pExceptionReportRecord->ExceptionInfo[0] ) ;
         break;
      case XCPT_NONCONTINUABLE_EXCEPTION:
         Log ( "ExceptionHandler: Non-continuable violation passed on to system." ) ;
         break;
      case XCPT_INVALID_DISPOSITION:
         Log ( "ExceptionHandler: Invalid Disposition violation passed on to system." ) ;
         break;
      case XCPT_INVALID_LOCK_SEQUENCE:
         Log ( "ExceptionHandler: Invalid Lock Sequence violation passed on to system." ) ;
         break;
      case XCPT_ARRAY_BOUNDS_EXCEEDED:
         Log ( "ExceptionHandler: Array Bound Exceeded violation passed on to system." ) ;
         break;
      case XCPT_B1NPX_ERRATA_02:
         Log ( "ExceptionHandler: B1NPX_ERRATA_02 violation passed on to system." ) ;
         break;
      case XCPT_UNWIND:
//       Log ( "ExceptionHandler: Unwind violation passed on to system." ) ;
         break;
      case XCPT_BAD_STACK:
         Log ( "ExceptionHandler: Bad Stack violation passed on to system." ) ;
         break;
      case XCPT_INVALID_UNWIND_TARGET:
         Log ( "ExceptionHandler: Invalid Unwind Target violation passed on to system." ) ;
         break;
      default:
         Log ( "Exception %08lX detected and passed on for system handling.", pExceptionReportRecord->ExceptionNum ) ;
         break;
   } /* endswitch */

   return ( XCPT_CONTINUE_SEARCH ) ;
}

/***************************************************************************
 *                                                                         *
 *  Exception Handler for Win32                                            *
 *                                                                         *
 ***************************************************************************/

#else // __NT__

#include "Excpt.h"

extern LONG __stdcall ExceptionFilter ( LPEXCEPTION_POINTERS lpexpExceptionInfo ) {

   Log ( 
      "WARNING: An unhandled exception has occurred at %08X.\n"
      "  The registers were as follows:\n"
      "    AX:%08lX  BX:%08lX  CX:%08lX  DX:%08lX\n"
      "    FL:%08lX  DI:%08lX  SI:%08lX  BP:%08lX\n"
      "    CS:%08lX  IP:%08lX  SS:%08lX  SP:%08lX\n"
      "    DS:%08lX  ES:%08lX  FS:%08lX  GS:%08lX",
      lpexpExceptionInfo->ContextRecord->Eip,
      lpexpExceptionInfo->ContextRecord->Eax,
      lpexpExceptionInfo->ContextRecord->Ebx,
      lpexpExceptionInfo->ContextRecord->Ecx,
      lpexpExceptionInfo->ContextRecord->Edx,
      lpexpExceptionInfo->ContextRecord->EFlags,
      lpexpExceptionInfo->ContextRecord->Edi,
      lpexpExceptionInfo->ContextRecord->Esi,
      lpexpExceptionInfo->ContextRecord->Ebp,
      lpexpExceptionInfo->ContextRecord->SegCs,
      lpexpExceptionInfo->ContextRecord->Eip,
      lpexpExceptionInfo->ContextRecord->SegSs,
      lpexpExceptionInfo->ContextRecord->Esp,
      lpexpExceptionInfo->ContextRecord->SegDs,
      lpexpExceptionInfo->ContextRecord->SegEs,
      lpexpExceptionInfo->ContextRecord->SegFs,
      lpexpExceptionInfo->ContextRecord->SegGs ) ;

   PLONG Base = ( PLONG ) lpexpExceptionInfo->ContextRecord->Ebp;
   while ( Base && ( *Base > ( LONG ) Base ) ) {
      Log ( "  Calling function was at %08lX.", *( Base + 1 ) );
      Base = ( PLONG ) * Base;
   } /* endwhile */

   return ( EXCEPTION_CONTINUE_SEARCH ) ;
}

#endif
