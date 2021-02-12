/****************************************************************** EXCEPT.H
 *                                                                         *
 *                            Exception Handler                            *
 *                                                                         *
 ***************************************************************************/

#ifndef EXCEPT_H
#define EXCEPT_H

#include "System.h"

#ifdef __OS2__

   extern BOOL BreakKey ;
   extern ULONG APIENTRY ExceptionHandler (
       PEXCEPTIONREPORTRECORD pExceptionReportRecord,
       PEXCEPTIONREGISTRATIONRECORD pExceptionRegistrationRecord,
       PCONTEXTRECORD pContextRecord,
       PVOID pDispatcherContext
   ) ;

        // *** IMPORTANT *** IMPORTANT *** IMPORTANT *** IMPORTANT *** IMPORTANT ***
        //
        //     These two macros MUST be invoked within the same function body,
        //           preferably at the highest level function possible.
        //
        // *** IMPORTANT *** IMPORTANT *** IMPORTANT *** IMPORTANT *** IMPORTANT ***

   #define REGISTER_EXCEPTION_HANDLER(x)                                        \
      EXCEPTIONREGISTRATIONRECORD ExceptionRecord = {0, ExceptionHandler};      \
      DosError ( FERR_DISABLEEXCEPTION | FERR_DISABLEHARDERR );                 \
      DosSetExceptionHandler ( &ExceptionRecord );                              

   #define UNREGISTER_EXCEPTION_HANDLER(x)                                      \
      DosUnsetExceptionHandler ( &ExceptionRecord ) ;


#else   // __NT__

   extern LONG __stdcall ExceptionFilter ( LPEXCEPTION_POINTERS lpexpExceptionInfo ) ;

   #define REGISTER_EXCEPTION_HANDLER(x)                                        \
        SetUnhandledExceptionFilter ( ExceptionFilter ) ;

   #define UNREGISTER_EXCEPTION_HANDLER(x)                                      \
        SetUnhandledExceptionFilter ( 0 ) ;

#endif // If OS/2 or NT.

#endif // If not already loaded.
