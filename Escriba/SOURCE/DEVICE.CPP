/***************************************************************** DEVICE.CPP
 *                                                                          *
 *                      Graphic Device Context Class                        *
 *                                                                          *
 ****************************************************************************/

#include "System.h"

#include "Debug.h"
#include "Device.h"

// #define DEBUG

DeviceContext::DeviceContext ( char *name, HWND Window ) : Name(name), Type(DEVICE_WINDOW), hDC(0), Created(FALSE), LockSemaphore("DeviceContext::LockSemaphore") {
   hDC = WinQueryWindowDC ( Window ) ;
   if ( hDC ) {
      #ifdef DEBUG
         Log ( "DeviceContext(%s)::DeviceContext: Created for window DC %08X.", Name, hDC ) ;
      #endif
   } else {
      char Message [512] ;
      Log ( "DeviceContext(%s)::DeviceContext: Unable to query device context for window %08X: %s", Name, Window, InterpretWinError ( 0, Message ) ) ;
   } /* endif */
} /* endmethod */

DeviceContext::DeviceContext ( char *name, HAB Anchor, char *QueueName, char *FormName, PDRIVDATA Settings, int Copies ) : Name(name), Type(DEVICE_PRINTER), hDC(0), Created(FALSE), LockSemaphore("DeviceContext::LockSemaphore") {
   hDC = OpenPrinterDC ( Anchor, QueueName, FormName, Settings, Copies ) ;
   if ( hDC ) {
      Created = TRUE ;
      #ifdef DEBUG
         Log ( "DeviceContext(%s)::DeviceContext: Created context %08X for printer queue '%s'.", Name, hDC, QueueName ) ;
      #endif
   } else {
      char Message [512] ;
      Log ( "DeviceContext(%s)::DeviceContext: Unable to create device context for print queue '%s': %s", Name, QueueName, InterpretWinError ( 0, Message ) ) ;
   } /* endif */
} /* endmethod */

DeviceContext::DeviceContext ( char *name, HAB Anchor, char *QueueName, char *FormName, PDRIVDATA Settings ) : Name(name), Type(DEVICE_PRINTER_INFO), hDC(0), Created(FALSE), LockSemaphore("DeviceContext::LockSemaphore") {
   hDC = OpenPrinterDC ( Anchor, QueueName, FormName, Settings ) ;
   if ( hDC ) {
      Created = TRUE ;
      #ifdef DEBUG
         Log ( "DeviceContext(%s)::DeviceContext: Created informational device context %08X for printer queue '%s'.", Name, hDC, QueueName ) ;
      #endif
   } else {
      char Message [512] ;
      Log ( "DeviceContext(%s)::DeviceContext: Unable to create informational device context for print queue '%s': %s", Name, QueueName, InterpretWinError ( 0, Message ) ) ;
   } /* endif */
} /* endmethod */

DeviceContext::DeviceContext ( char *name, long Anchor ) : Name(name), Type(DEVICE_MEMORY), hDC(0), Created(FALSE), LockSemaphore("DeviceContext::LockSemaphore") {
   PSZ pszData[] = { 0, PSZ("Display") } ;
   hDC = DevOpenDC ( HAB(Anchor), OD_MEMORY, PSZ("*"), 2, pszData, 0 ) ;
   if ( hDC ) {
      Created = TRUE ;
      #ifdef DEBUG
         Log ( "DeviceContext(%s)::DeviceContext: Created memory DC %08X.", Name, hDC ) ;
      #endif
   } else {
      char Message [512] ;
      Log ( "DeviceContext(%s)::DeviceContext: Unable to create memory device context: %s", Name, InterpretWinError ( 0, Message ) ) ;
   } /* endif */
} /* endmethod */

DeviceContext::~DeviceContext ( ) {
   #ifdef DEBUG
      Log ( "DeviceContext(%s)::~DeviceContext Started.", Name ) ;
   #endif
   if ( Created ) {
      HMF Result = DevCloseDC ( hDC ) ;
      switch ( Result ) {
         case DEV_ERROR: {
            char Message [512] ;
            Log ( "DeviceContext(%s)::~DeviceContext: Unable to close device context: %s", Name, InterpretWinError ( 0, Message ) ) ;
            break; }
         case DEV_OK: {
            #ifdef DEBUG
               Log ( "DeviceContext(%s)::~DeviceContext Device context closed normally.", Name ) ;
            #endif
            break; }
         default: {
            Log ( "DeviceContext(%s)::~DeviceContext: Metafile created.  Handle %08X.  Deleting it now.", Name, Result ) ;
            if ( !GpiDeleteMetaFile ( Result ) ) {
               char Message [512] ;
               Log ( "DeviceContext(%s)::~DeviceContext: Unable to delete metafile: %s", Name, InterpretWinError ( 0, Message ) ) ;
            } /* endif */
            break; }
      } /* endswitch */
   } /* endif */
   #ifdef DEBUG
      Log ( "DeviceContext(%s)::~DeviceContext Done.", Name ) ;
   #endif
} /* endmethod */

void DeviceContext::Dump ( int indent ) {
   Log ( "%*sDump of device context(%08p):", indent, "", this ) ;
   Log ( "%*s  Name '%s'", indent, "", Name ) ;
   static char *Types [] = { "Window", "Printer", "PrinterInfo", "Memory" } ;
   Log ( "%*s  Type '%s'", indent, "", Types[Type] ) ;
   Log ( "%*s  hDC %08X", indent, "", hDC ) ;
   Log ( "%*s  Created: %s", indent, "", Created ? "TRUE" : "FALSE" ) ;
} /* endmethod */

