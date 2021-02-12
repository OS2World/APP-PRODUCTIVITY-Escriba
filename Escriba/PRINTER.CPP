/**************************************************************** PRINTER.CPP
 *                                                                          *
 *                           PM Printer Support                             *
 *                                                                          *
 ****************************************************************************/

#include "System.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Debug.h"
#include "Device.h"
#include "Printer.h"
#include "Support.h"

// #define DEBUG

/****************************************************************************
 *                                                                          *
 *               Open Printer Device Context for Information                *
 *                                                                          *
 *  Parameters:                                                             *
 *    HAB Anchor         - Anchor Block Handle                              *
 *    char *QueueName    - Printer Queue Name                               *
 *    PDRIVDATA Settings - Printer Job Settings (or NULL to use default)    *
 *                                                                          *
 *  Returns:                                                                *
 *    HDC Device Context Handle or NULLHANDLE.                              *
 *                                                                          *
 ****************************************************************************/

extern HDC OpenPrinterDC ( HAB Anchor, char *QueueName, char *FormName, PDRIVDATA Settings ) {

 /***************************************************************************
  * Find the specified queue's information, or the default.                 *
  ***************************************************************************/

  #ifdef DEBUG
     Log ( "OpenPrinterDC(Anchor=%08X,Queue='%s',Settings) Started.", Anchor, QueueName ) ;
  #endif // DEBUG

  QueueInfo Info ( "OpenPrinterDC-Info", Anchor, QueueName, FormName, Settings ) ;

 /***************************************************************************
  * Trim the printer driver sub-type from the driver name.                  *
  ***************************************************************************/

  #ifdef DEBUG
     Log ( "OpenPrinterDC(Anchor=%08X,Queue='%s',Settings) Allocating memory for driver name '%s'.", Anchor, QueueName, Info.QueryDriverName() ) ;
  #endif // DEBUG

  char *DriverName = new char [ strlen(PCHAR(Info.QueryDriverName()))+1 ] ;
  strcpy ( DriverName, PCHAR(Info.QueryDriverName()) ) ;

  char *p = strchr ( DriverName, '.' ) ;
  if ( p )
     *p = 0 ;

  #ifdef DEBUG
     Log ( "OpenPrinterDC(Anchor=%08X,Queue='%s',Settings) Driver name trimmed to '%s'.", Anchor, QueueName, DriverName ) ;
  #endif // DEBUG

 /***************************************************************************
  * Build the device open parameter list.                                   *
  ***************************************************************************/

  DEVOPENSTRUC DeviceOpenParms ;
  DeviceOpenParms.pszDriverName      = PSZ(DriverName) ;
  DeviceOpenParms.pszLogAddress      = Info.QueryName() ;
  DeviceOpenParms.pdriv              = Info.QuerySettings() ;
  DeviceOpenParms.pszDataType        = PSZ("PM_Q_STD") ;

 /***************************************************************************
  * Open printer device context.                                            *
  ***************************************************************************/

  #ifdef DEBUG
     Log ( "OpenPrinterDC(Anchor=%08X,Queue='%s',Settings) Opening device context.", Anchor, QueueName ) ;
  #endif // DEBUG

  HDC hDC = DevOpenDC ( Anchor, OD_INFO, PSZ("*"), 4, PDEVOPENDATA(&DeviceOpenParms), 0 ) ;

 /***************************************************************************
  * Return the DC handle.                                                   *
  ***************************************************************************/

  #ifdef DEBUG
     Log ( "OpenPrinterDC(Anchor=%08X,Queue='%s',Settings) Releasing memory for driver name.", Anchor, QueueName ) ;
  #endif // DEBUG

  delete [] DriverName ;

  #ifdef DEBUG
     Log ( "OpenPrinterDC(Anchor=%08X,Queue='%s',Settings) Done.  hDC=%08X.", Anchor, QueueName, hDC ) ;
  #endif // DEBUG

  return ( hDC ) ;
}
 
/****************************************************************************
 *                                                                          *
 *                Open Printer Device Context for Printing                  *
 *                                                                          *
 *  Parameters:                                                             *
 *    HAB Anchor         - Anchor Block Handle                              *
 *    char *QueueName    - Printer Queue Name                               *
 *    char *FormName     - Printer Form Name                                *
 *    PDRIVDATA Settings - Printer Job Settings (or NULL to use default)    *
 *    int Copies         - The number of copies to be printed.              *
 *                                                                          *
 *  Returns:                                                                *
 *    HDC Device Context Handle or NULLHANDLE.                              *
 *                                                                          *
 ****************************************************************************/

extern HDC OpenPrinterDC ( HAB Anchor, char *QueueName, char *FormName, PDRIVDATA Settings, int Copies ) {

 /***************************************************************************
  * Find the specified queue's information, or the default.                 *
  ***************************************************************************/

  QueueInfo Info ( "OpenPrinterDC-Print", Anchor, QueueName, FormName, Settings ) ;

 /***************************************************************************
  * Trim the printer driver sub-type from the driver name.                  *
  ***************************************************************************/

  char *DriverName = new char [ strlen(PCHAR(Info.QueryDriverName()))+1 ] ;
  strcpy ( DriverName, PCHAR(Info.QueryDriverName()) ) ;

  char *p = strchr ( DriverName, '.' ) ;
  if ( p )
     *p = 0 ;

 /***************************************************************************
  * Build the queue processor parameter text.                               *
  ***************************************************************************/

  char QueueParms [80] ;
  sprintf ( QueueParms, "COP=%i", Copies ) ;

 /***************************************************************************
  * Build the spooler parameter text.                                       *
  ***************************************************************************/

  char SpoolerParms [80] ;
  sprintf ( SpoolerParms, "FORM=%s", FormName ) ;

 /***************************************************************************
  * Build the device open parameter list.                                   *
  ***************************************************************************/

  DEVOPENSTRUC DeviceOpenParms ;
  DeviceOpenParms.pszDriverName      = PSZ(DriverName) ;
  DeviceOpenParms.pszLogAddress      = Info.QueryName() ;
  DeviceOpenParms.pdriv              = Info.QuerySettings() ;
  DeviceOpenParms.pszDataType        = PSZ("PM_Q_STD") ;
  DeviceOpenParms.pszComment         = PSZ("Escriba") ;
  DeviceOpenParms.pszQueueProcName   = PSZ("") ;
  DeviceOpenParms.pszQueueProcParams = PSZ(QueueParms) ;
  DeviceOpenParms.pszSpoolerParams   = PSZ("") /* PSZ(SpoolerParms) */ ;

 /***************************************************************************
  * Open printer device context.                                            *
  ***************************************************************************/

  HDC hDC = DevOpenDC ( Anchor, OD_QUEUED, PSZ("*"), 8, PDEVOPENDATA(&DeviceOpenParms), 0 ) ;

 /***************************************************************************
  * Return the DC handle.                                                   *
  ***************************************************************************/

  delete [] DriverName ;

  return ( hDC ) ;
}
 
/****************************************************************************
 *                                                                          *
 *  QueueInfo: Constructor                                                  *
 *                                                                          *
 ****************************************************************************/

QueueInfo::QueueInfo ( char *objectname, HAB, char *QueueName, char *FormName, PDRIVDATA Settings ) : ObjectName(objectname) {

   #ifdef DEBUG
      Log ( "QueueInfo::QueueInfo(Name='%s',Queue='%s',Form='%s') Started.", objectname, QueueName?QueueName:"", FormName?FormName:"" ) ;
   #endif

   // Initialize
   memset ( &Data, 0, sizeof(Data) ) ;

   // Save the queue name.
   Name = new char [ strlen(PCHAR(QueueName?QueueName:""))+1 ] ;
   strcpy ( PCHAR(Name), PCHAR(QueueName?QueueName:"") ) ;

   // Save the form name.
   Form = new char [ strlen(PCHAR(FormName?FormName:""))+1 ] ;
   strcpy ( PCHAR(Form), PCHAR(FormName?FormName:"") ) ;

   // Get the entire list of queues and their information.
   ULONG cTotal;
   ULONG cReturned ;
   ULONG cbNeeded ;
   PVOID pBuf = 0 ;
   PPRQINFO3 pQueueInfo = 0 ;
   SplEnumQueue( 0, 3, 0, 0, &cReturned, &cTotal, &cbNeeded, 0 ) ;
   if ( cbNeeded ) {

      #ifdef DEBUG
         Log ( "QueueInfo::QueueInfo(Name='%s',Queue='%s') Allocating %i bytes for buffer.", objectname, QueueName?QueueName:"", cbNeeded ) ;
      #endif

      DosAllocMem ( &pBuf, cbNeeded, PAG_READ|PAG_WRITE|PAG_COMMIT ) ;

      #ifdef DEBUG
         Log ( "QueueInfo::QueueInfo(Name='%s',Queue='%s') Enumerating queues.", objectname, QueueName?QueueName:"" ) ;
      #endif

      ULONG cbBuf = cbNeeded ;
      SplEnumQueue ( 0, 3, pBuf, cbBuf, &cReturned, &cTotal, &cbNeeded, 0 ) ;

      // Select the named queue from the list.
      if ( QueueName ) {
         PPRQINFO3 prq = PPRQINFO3(pBuf) ;
         for ( int i=0; i<cReturned; i++, prq++ ) {
            #ifdef DEBUG
               Log ( "QueueInfo::QueueInfo(Name='%s',Queue='%s') Looking at queue '%s'.", objectname, QueueName, prq->pszComment ) ;
            #endif
            if ( !stricmp ( PCHAR(QueueName), PCHAR(prq->pszComment) ) ) {
               #ifdef DEBUG
                  Log ( "QueueInfo::QueueInfo(Name='%s',Queue='%s') Chose queue '%s'.", objectname, QueueName, prq->pszComment ) ;
               #endif
               pQueueInfo = prq ;
            } /* endif */
         } /* endfor i */
      } /* endif */

      // If no queue got selected, use the application default queue.
      if ( pQueueInfo == 0 ) {
         #ifdef DEBUG
            Log ( "QueueInfo::QueueInfo(Name='%s',Queue='%s') No queue was selected.  Picking default.", objectname, QueueName?QueueName:"" ) ;
         #endif
         PPRQINFO3 prq = PPRQINFO3(pBuf) ;
         for ( int i=0; i<cReturned; i++, prq++ ) {
            #ifdef DEBUG
               Log ( "QueueInfo::QueueInfo(Name='%s',Queue='%s') Looking at queue '%s'.", objectname, QueueName?QueueName:"", prq->pszComment ) ;
            #endif
            if ( prq->fsType & PRQ3_TYPE_APPDEFAULT ) {
               #ifdef DEBUG
                  Log ( "QueueInfo::QueueInfo(Name='%s',Queue='%s') Queue '%s' chosen because default.", objectname, QueueName?QueueName:"", prq->pszComment ) ;
               #endif
               pQueueInfo = prq ;
            } /* endif */
         } /* endfor */
      } /* endif */

   } /* endif */

   // If there was no application default queue, use the first queue in the list.
   if ( pQueueInfo == 0 ) {
      pQueueInfo = PPRQINFO3(pBuf) ;
      #ifdef DEBUG
         PPRQINFO3 prq = PPRQINFO3(pBuf) ;
         Log ( "QueueInfo::QueueInfo(Name='%s',Queue='%s') Chose queue '%s' because first in list.", objectname, QueueName?QueueName:"", prq->pszComment ) ;
      #endif
   } /* endif */

   // Copy the queue information into the object's private area.
   if ( pQueueInfo ) {
      #ifdef DEBUG
         Log ( "QueueInfo::QueueInfo(Name='%s',Queue='%s') Setting up data.", objectname, QueueName?QueueName:"" ) ;
      #endif
      Data = *pQueueInfo ;
      Data.pszName = (PSZ) new char [ strlen(PCHAR(pQueueInfo->pszName))+1 ] ;
      strcpy ( PCHAR(Data.pszName), PCHAR(pQueueInfo->pszName) ) ;
      Data.pszPrProc = (PSZ) new char [ strlen(PCHAR(pQueueInfo->pszPrProc))+1 ] ;
      strcpy ( PCHAR(Data.pszPrProc), PCHAR(pQueueInfo->pszPrProc) ) ;
      Data.pszSepFile = (PSZ) new char [ strlen(PCHAR(pQueueInfo->pszSepFile))+1 ] ;
      strcpy ( PCHAR(Data.pszSepFile), PCHAR(pQueueInfo->pszSepFile) ) ;
      Data.pszParms = (PSZ) new char [ strlen(PCHAR(pQueueInfo->pszParms))+1 ] ;
      strcpy ( PCHAR(Data.pszParms), PCHAR(pQueueInfo->pszParms) ) ;
      Data.pszComment = (PSZ) new char [ strlen(PCHAR(pQueueInfo->pszComment))+1 ] ;
      strcpy ( PCHAR(Data.pszComment), PCHAR(pQueueInfo->pszComment) ) ;
      Data.pszPrinters = (PSZ) new char [ strlen(PCHAR(pQueueInfo->pszPrinters))+1 ] ;
      strcpy ( PCHAR(Data.pszPrinters), PCHAR(pQueueInfo->pszPrinters) ) ;
      Data.pszDriverName = (PSZ) new char [ strlen(PCHAR(pQueueInfo->pszDriverName))+1 ] ;
      strcpy ( PCHAR(Data.pszDriverName), PCHAR(pQueueInfo->pszDriverName) ) ;
      if ( Settings 
         AND Settings->cb 
         AND ( Settings->lVersion == pQueueInfo->pDriverData->lVersion ) 
         AND !strcmp ( Settings->szDeviceName, pQueueInfo->pDriverData->szDeviceName ) ) {
         Data.pDriverData = (PDRIVDATA) new char [ Settings->cb ] ;
         memcpy ( Data.pDriverData, Settings, size_t(Settings->cb) ) ;
      } else {
         Data.pDriverData = (PDRIVDATA) new char [ pQueueInfo->pDriverData->cb ] ;
         memcpy ( Data.pDriverData, pQueueInfo->pDriverData, size_t(pQueueInfo->pDriverData->cb) ) ;
      } /* endif */
   } else {
      #ifdef DEBUG
         Log ( "QueueInfo::QueueInfo(Name='%s',Queue='%s') Setting up default data.", objectname, QueueName?QueueName:"" ) ;
      #endif
      memset ( &Data, 0, sizeof(Data) ) ;
      Data.pszName = (PSZ) new char [1] ;
      strcpy ( PCHAR(Data.pszName), "" ) ;
      Data.pszPrProc = (PSZ) new char [1] ;
      strcpy ( PCHAR(Data.pszPrProc), "" ) ;
      Data.pszSepFile = (PSZ) new char [1] ;
      strcpy ( PCHAR(Data.pszSepFile), "" ) ;
      Data.pszParms = (PSZ) new char [1] ;
      strcpy ( PCHAR(Data.pszParms), "" ) ;
      Data.pszComment = (PSZ) new char [1] ;
      strcpy ( PCHAR(Data.pszComment), "" ) ;
      Data.pszPrinters = (PSZ) new char [1] ;
      strcpy ( PCHAR(Data.pszPrinters), "" ) ;
      Data.pszDriverName = (PSZ) new char [1] ;
      strcpy ( PCHAR(Data.pszDriverName), "" ) ;
      if ( Settings AND Settings->cb ) {
         Data.pDriverData = (PDRIVDATA) new char [ Settings->cb ] ;
         memcpy ( Data.pDriverData, Settings, size_t(Settings->cb) ) ;
      } else {
         Data.pDriverData = (PDRIVDATA) new char [ sizeof(pQueueInfo->pDriverData->cb) ] ;
         Data.pDriverData->cb = sizeof(Data.pDriverData->cb) ;
      } /* endif */
   } /* endif */

   #ifdef DEBUG
      Log ( "QueueInfo::QueueInfo(Name='%s',Queue='%s') Releasing buffer.", objectname, QueueName?QueueName:"" ) ;
   #endif

   if ( pBuf ) 
      DosFreeMem ( pBuf ) ;
}
 
/****************************************************************************
 *                                                                          *
 *  QueueInfo: Destructor                                                   *
 *                                                                          *
 ****************************************************************************/

QueueInfo::~QueueInfo ( ) {
   if ( Data.pDriverData   ) delete [] Data.pDriverData ;
   if ( Data.pszDriverName ) delete [] Data.pszDriverName ;
   if ( Data.pszPrinters   ) delete [] Data.pszPrinters ;
   if ( Data.pszComment    ) delete [] Data.pszComment ;
   if ( Data.pszParms      ) delete [] Data.pszParms ;
   if ( Data.pszSepFile    ) delete [] Data.pszSepFile ;
   if ( Data.pszPrProc     ) delete [] Data.pszPrProc ;
   if ( Data.pszName       ) delete [] Data.pszName ;
   if ( Form               ) delete [] Form ;
   if ( Name               ) delete [] Name ;
}
 
/****************************************************************************
 *                                                                          *
 *  QueueInfo: Query Selected Form Information                              *
 *                                                                          *
 ****************************************************************************/

void QueueInfo::QueryForm ( HAB Anchor, BOOL Metric, HCINFO &FormInfo ) {

   #ifdef DEBUG
      if ( Name && *Name ) {
         if ( Form && *Form ) {
            Log ( "QueueInfo(%s)::QueryForm: Looking for printer '%s', form '%s'.", ObjectName, Name, Form ) ;
         } else {
            Log ( "QueueInfo(%s)::QueryForm: Looking for printer '%s', default form.", ObjectName, Name ) ;
         } /* endif */
      } else {
         if ( Form && *Form ) {
            Log ( "QueueInfo(%s)::QueryForm: Looking for default printer, form '%s'.", ObjectName, Form ) ;
         } else {
            Log ( "QueueInfo(%s)::QueryForm: Looking for default printer, default form.", ObjectName ) ;
         } /* endif */
      } /* endif */
   #endif

   // Set defaults.
   LONG HorizontalResolution = 11808 ;          // 300 dpi horizontal dot pitch
   LONG VerticalResolution = 11808 ;            // 300 dpi vertical dot pitch
   strcpy ( FormInfo.szFormname, Metric ? "A4" : "Letter" ) ;       
   FormInfo.cx = Metric ? 210 : 216 ;           // PageWidth 8.5" / 21.0cm
   FormInfo.cy = Metric ? 297 : 279 ;           // PageHeight 11" / 29.7cm
   FormInfo.xLeftClip = 0 ;                     // Left Clipping 0"
   FormInfo.xRightClip = 203 ;                  // Right Clipping 8"
   FormInfo.yBottomClip = 38 ;                  // Bottom Clipping 1.5"
   FormInfo.yTopClip = 279 ;                    // Top Clipping 11.0"
   FormInfo.xPels = 2400 ;                      // PrintableWidth 8.0"
   FormInfo.yPels = 2850 ;                      // PrintableHeight 9.5"

   // Try to get form info from the printer driver.
   DeviceContext Printer = DeviceContext ( "QueueInfo::QueryForm", Anchor, Name, Form, Data.pDriverData ) ;
   if ( Printer.QueryHandle() ) {
      DevQueryCaps ( Printer.QueryHandle(), CAPS_VERTICAL_RESOLUTION,   1, &VerticalResolution ) ;
      DevQueryCaps ( Printer.QueryHandle(), CAPS_HORIZONTAL_RESOLUTION, 1, &HorizontalResolution ) ;
      LONG FormsCount = DevQueryHardcopyCaps ( Printer.QueryHandle(), 0, 0, 0 ) ;
      PHCINFO FormsInfo = new HCINFO [ FormsCount ] ;
      DevQueryHardcopyCaps ( Printer.QueryHandle(), 0, FormsCount, FormsInfo ) ;
      for ( int i=0; i<FormsCount; i++ ) {
         #ifdef DEBUG
            Log ( "QueueInfo(%s)::QueryForm: Checking form '%s' %08X.", ObjectName, FormsInfo[i].szFormname, FormsInfo[i].flAttributes ) ;
         #endif
         if ( Form && *Form ) {
            if ( !stricmp ( FormsInfo[i].szFormname, Form ) ) {
               FormInfo = FormsInfo[i] ;
               #ifdef DEBUG
                  Log ( "QueueInfo(%s)::QueryForm: Found exact match.", ObjectName ) ;
               #endif
               break ;
            } /* endif */
         } else {
            if ( FormsInfo[i].flAttributes & HCAPS_CURRENT ) {
               FormInfo = FormsInfo[i] ;
               #ifdef DEBUG
                  Log ( "QueueInfo(%s)::QueryForm: Found current form.", ObjectName ) ;
               #endif
               break ;
            } /* endif */
         } /* endif */
      } /* endfor */
      if ( i >= FormsCount ) {
         for ( i=0; i<FormsCount; i++ ) {
            if ( FormsInfo[i].flAttributes & HCAPS_CURRENT ) {
               FormInfo = FormsInfo[i] ;
               #ifdef DEBUG
                  Log ( "QueueInfo(%s)::QueryForm: Selected current form.", ObjectName ) ;
               #endif
               break ;
            } /* endif */
         } /* endfor */
      } /* endif */
      if ( i >= FormsCount ) 
         FormInfo = FormsInfo[0] ;
      delete [] FormsInfo ;
   } /* endif */

   // Convert all measurements to the currently selected system.

   #define METRIC(x) LONG(x*100)
   #define ENGLISH(x)  LONG((double(x)*1000.0)/25.4)
 
   SIZEL PrintSize ;
   if ( Metric ) {
      FormInfo.cx          = METRIC ( FormInfo.cx ) ;
      FormInfo.cy          = METRIC ( FormInfo.cy ) ;
      FormInfo.xLeftClip   = METRIC ( FormInfo.xLeftClip ) ;
      FormInfo.xRightClip  = METRIC ( FormInfo.xRightClip ) ;
      FormInfo.yBottomClip = METRIC ( FormInfo.yBottomClip ) ;
      FormInfo.yTopClip    = METRIC ( FormInfo.yTopClip ) ;
      PrintSize.cx = LONG ( ( double(FormInfo.xPels) * double(100000.0) ) / double(HorizontalResolution) ) ;
      PrintSize.cy = LONG ( ( double(FormInfo.yPels) * double(100000.0) ) / double(VerticalResolution) ) ;
   } else {
      FormInfo.cx          = ENGLISH ( FormInfo.cx ) ;
      FormInfo.cy          = ENGLISH ( FormInfo.cy ) ;
      FormInfo.xLeftClip   = ENGLISH ( FormInfo.xLeftClip ) ;
      FormInfo.xRightClip  = ENGLISH ( FormInfo.xRightClip ) ;
      FormInfo.yBottomClip = ENGLISH ( FormInfo.yBottomClip ) ;
      FormInfo.yTopClip    = ENGLISH ( FormInfo.yTopClip ) ;
      PrintSize.cx = LONG ( ( double(FormInfo.xPels) * double(39380.0) ) / double(HorizontalResolution) ) ;
      PrintSize.cy = LONG ( ( double(FormInfo.yPels) * double(39380.0) ) / double(VerticalResolution) ) ;
   } /* endif */

   // Adjust right clipping.
   FormInfo.xRightClip = min ( FormInfo.xRightClip, min ( FormInfo.cx, FormInfo.xLeftClip+PrintSize.cx ) ) ;

   // Adjust vertical clipping.
   if ( FormInfo.yBottomClip == 0 ) {
      FormInfo.yBottomClip = FormInfo.cy - PrintSize.cy ;
      FormInfo.yTopClip = FormInfo.cy ;
   } /* endif */
   FormInfo.yTopClip = min ( FormInfo.yTopClip, min ( FormInfo.cy, FormInfo.yBottomClip+PrintSize.cy ) ) ;

   #ifdef DEBUG
      Log ( "QueueInfo(%s)::QueryForm: PaperSize=%.3lfx%.3lf PrintSize=%.3lfx%.3lf Clipping=%.3lf,%.3lf-%.3lf,%.3lf Resolution=%lux%ludpi",
         ObjectName,
         double(FormInfo.cx)/1000.0, double(FormInfo.cy)/1000.0, 
         double(PrintSize.cx)/1000.0, double(PrintSize.cy)/1000.0,
         double(FormInfo.xLeftClip)/1000.0, double(FormInfo.yBottomClip)/1000.0,
         double(FormInfo.xRightClip)/1000.0, double(FormInfo.yTopClip)/1000.0, 
         long(double(HorizontalResolution)/39.38), long(double(VerticalResolution)/39.38) ) ;
   #endif
}

/****************************************************************************
 *                                                                          *
 *  QueueInfo: Query Printer Resolution (pels/inch or centimeter)           *
 *                                                                          *
 ****************************************************************************/

BOOL QueueInfo::QueryResolution ( HAB Anchor, BOOL Metric, long &Horizontal, long &Vertical ) {

   // Set defaults in pels per meter.
   Horizontal = 11808 ;                         // 300 dpi horizontal dot pitch
   Vertical = 11808 ;                           // 300 dpi vertical dot pitch

   // Try to get form info from the printer driver.
   DeviceContext Printer = DeviceContext ( "QueueInfo::QueryResolution", Anchor, Name, Form, Data.pDriverData ) ;
   if ( Printer.QueryHandle() ) {
      DevQueryCaps ( Printer.QueryHandle(), CAPS_VERTICAL_RESOLUTION,   1, &Vertical ) ;
      DevQueryCaps ( Printer.QueryHandle(), CAPS_HORIZONTAL_RESOLUTION, 1, &Horizontal ) ;
   } /* endif */

   // Convert to target measuring system.
   if ( Metric ) {
      Horizontal = long(double(Horizontal)/100.0) ;
      Vertical = long(double(Vertical)/100.0) ;
   } else {
      Horizontal = long(double(Horizontal)/39.38) ;
      Vertical = long(double(Vertical)/39.38) ;
   } /* endif */

   // Return TRUE if accurate, FALSE if default.
   return ( Printer.QueryHandle() != 0 ) ;
}
