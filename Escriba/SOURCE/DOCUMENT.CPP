/*************************************************************** DOCUMENT.CPP                              
 *                                                                          *
 *                             Document Classes                             *
 *                                                                          *
 ****************************************************************************/

#define DECLARE_FUNCTION_NAMES
#include "System.h"

#include <ctype.h>
#include <io.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <sys\types.h>
#include <sys\stat.h>

#include "DateFmt.h"
#include "Debug.h"
#include "Dialog2.h"
#include "DocFrame.h"
#include "DocSup.h"
#include "Document.h"
#include "Escriba.h"
#include "Except.h"
#include "External.h"
#include "Event.h"
#include "GClass.h"
#include "HelpWin.h"
#include "HourGlas.h"
#include "HRuler.h"
#include "Imp_Exp.h"
#include "Mutex.h"
#include "Printer.h"
#include "Process.h"
#include "Readers.h"
#include "ReString.h"
#include "State.h"
#include "String_.h"
#include "Support.h"
#include "Text.h"
#include "Thread.h"
#include "VRuler.h"
#include "Window.h"

// #define SMOOTH_SCROLLING

// #define DEBUG
// #define DEBUG_FIND
// #define DEBUG_CHANGEALL
// #define DEBUG_LOCK
// #define DEBUG_MOTION
// #define DEBUG_FINISHMOVE
// #define DEBUG_FINISHMOVE_DETAILS
// #define DEBUG_INVERTTEXT
// #define DEBUG_COMPUTESTATE
// #define DEBUG_TRANSFORM
// #define DEBUG_HANDLES
// #define DEBUG_RENDER
// #define DEBUG_SETDEFAULTS
// #define DEBUG_SCROLLING
// #define DEBUG_UPDATESCROLLBARS
// #define DEBUG_EXTRACT
// #define DEBUG_REPLACESELECTEDTEXT
// #define DEBUG_SPELLCHECK
// #define DEBUG_CONSTRUCTOR
// #define DEBUG_CREATE_DESTROY
// #define DEBUG_SETWINDOW
// #define DEBUG_WINDOWTHREAD
// #define DEBUG_UPDATEPOSITION
// #define DEBUG_NEEDSPAINTING
// #define DEBUG_MERGE
// #define DEBUG_INSERT
// #define DEBUG_SETBLOCKSTATE

#ifdef DEBUG
   #define DEBUG_FIND
   #define DEBUG_CHANGEALL
   #define DEBUG_LOCK
   #define DEBUG_MOTION
   #define DEBUG_FINISHMOVE
   #define DEBUG_INVERTTEXT
   #define DEBUG_COMPUTESTATE
   #define DEBUG_TRANSFORM
   #define DEBUG_HANDLES
   #define DEBUG_RENDER
   #define DEBUG_SETDEFAULTS
   #define DEBUG_SCROLLING
   #define DEBUG_UPDATESCROLLBARS
   #define DEBUG_EXTRACT
   #define DEBUG_REPLACESELECTEDTEXT
   #define DEBUG_SPELLCHECK
   #define DEBUG_CONSTRUCTOR
   #define DEBUG_CREATE_DESTROY
   #define DEBUG_SETWINDOW
   #define DEBUG_WINDOWTHREAD
   #define DEBUG_UPDATEPOSITION
   #define DEBUG_NEEDSPAINTING
   #define DEBUG_MERGE
   #define DEBUG_INSERT
   #define DEBUG_SETBLOCKSTATE
#endif

 
/****************************************************************************
 *                                                                          *
 *  Document: Constructor                                                   *
 *                                                                          *
 ****************************************************************************/

Documento::Documento ( HAB anchor, LanguageModule *library, DocumentParms &docparms, PSZ filename, Addon *Importer ) :
   Anchor(anchor), Column(0), CurrentPageValid(FALSE),
   DocParms(docparms), fDocParms(FALSE), FileDate(0), Filename(0), FullNames(FALSE),
   InsertMode(TRUE), Library(library), Minimized(FALSE), NumberOfPages(0), Offset(0), OffsetLast(0),
   OffsetOrigin(0), PageNumber(0), PainterObject(0), PS(0), pText(0), SavedLevel(0),
   StatusWindow(0), SupervisorObject(0), Title(0), TitleWindow(0), Window(0),
   WindowThreadDone("WindowThreadDone"), WindowThreadReady("WindowThreadReady"),
   HandleCount(0), CurrentPageLock("Documento::CurrentPageLock") {

   #ifdef DEBUG_CONSTRUCTOR
      Log ( "Documento(%08X)::Documento() Started.  Getting file type.", this ) ;
   #endif // DEBUG_CONSTRUCTOR

   #ifdef DEBUG_LOCK
      CurrentPageLock.SetDebug(TRUE) ;
   #endif // DEBUG_LOCK

   if ( Importer ) {
      if ( ! strcmpi ( Importer->QueryType(), TYPE_IMPORTEXPORT ) ) {
         FileType = new String ( ((ImportExport*)Importer)->QueryFileType() ) ;
      } else if ( ! strcmpi ( Importer->QueryType(), TYPE_GRAPHICIMPORT ) ) {
         FileType = new String ( ((GraphicImporter*)Importer)->QueryFileType() ) ;
      } else {
         FileType = new String ( ((MetafileImporter*)Importer)->QueryFileType() ) ;
      } /* endif */
   } else {
      FileType = new String ( "Escriba" ) ;
   } /* endif */

   #ifdef DEBUG_CONSTRUCTOR
      Log ( "Documento(%08X)::Documento() File type '%s'.  Initializing various arrays.", this, PCHAR(*FileType) ) ;
   #endif // DEBUG_CONSTRUCTOR

   Rulers[0] = Rulers[1] = 0 ;
   ScrollBars[0] = ScrollBars[1] = 0 ;
   ScrollMax.x = ScrollMax.y = 0 ;
   ScrollPos.x = ScrollPos.y = 0 ;
   TopLeft.x = TopLeft.y = 0 ;
   Zoom = MAKEFIXED ( 1, 0 ) ;

   Colors[0] = RGB_WHITE ;
   Colors[1] = RGB_BLUE ;

   Filename = new String ( filename ) ;
   if ( filename AND *filename ) {

      #ifdef DEBUG_CONSTRUCTOR
         Log ( "Documento(%08X)::Documento() Getting document date.", this ) ;
      #endif // DEBUG_CONSTRUCTOR

      // Get the document change date.  Use it as the default creation date.
      struct stat FileStatus ;
      if ( _stat ( filename, &FileStatus ) ) {
         FileDate = time(0) ;
      } else {
         FileDate = FileStatus.st_mtime ;
      } /* endif */
      DocParms.CreateDate = FileDate ;

      if ( Importer ) {

         #ifdef DEBUG_CONSTRUCTOR
            Log ( "Documento(%08X)::Documento() Importing document.", this ) ;
         #endif // DEBUG_CONSTRUCTOR

         long Size ;
         char *RawData = 0 ;
         if ( ! strcmpi ( Importer->QueryType(), TYPE_IMPORTEXPORT ) ) {
            RawData = PCHAR ( ((ImportExport*)Importer)->Import ( 0, PCHAR(filename), &Size ) ) ;
         } else if ( ! strcmpi ( Importer->QueryType(), TYPE_GRAPHICIMPORT ) ) {
            RawData = PCHAR ( ((GraphicImporter*)Importer)->Import ( 0, PCHAR(filename), &Size ) ) ;
         } else {
            RawData = PCHAR ( ((MetafileImporter*)Importer)->Import ( 0, PCHAR(filename), &Size ) ) ;
         } /* endif */

         if ( RawData AND Size ) {

            #ifdef DEBUG_CONSTRUCTOR
               Log ( "Documento(%08X)::Documento() Processing document preface, if there is one.", this ) ;
            #endif // DEBUG_CONSTRUCTOR

            // Process the document preface, if there is one.
            int Version ( 0 ) ;
            long Start ( ReadPreface ( RawData, (int)Size, DocParms, Version ) ) ;
            Size -= Start ;

            // If there remains any data to process, do so now.
            if ( Size > 0 ) {

               #ifdef DEBUG_CONSTRUCTOR
                  Log ( "Documento(%08X)::Documento() Processing document text.", this ) ;
               #endif // DEBUG_CONSTRUCTOR

               // Allocate space for the cooked data.  It has no size yet.
               PCHAR CookedData = PCHAR ( malloc ( size_t ( Size ) ) ) ;
               long CookedSize = 0 ;

               // Prepare to process the raw data into cooked data.
               TEXTDATA TextData ;
               TextData.Buffer = PUCHAR(RawData+Start) ;
               TextData.Index = 0 ;
               TextData.Size = Size ;

               UCHAR Bytes [2] ;
               PUCHAR pToken = Bytes ;
               while ( GetTokenData ( pToken, Size, PREADER(ReadText), PREADER(PeekText), &TextData ) ) {

                  // If not a function, simply store it into the cooked data.
                  if ( Size <= 2 ) {
                     memcpy ( CookedData+CookedSize, Bytes, size_t(Size) ) ;
                     CookedSize += Size ;
                     continue ;
                  } /* endif */

                  // Convert the function to internal format.
                  PUCHAR ConvertedToken = PUCHAR ( malloc ( size_t ( Size ) ) ) ;
                  long ConvertedSize ;  LONG Handle = 0 ;  int HandleType ;
                  if ( ImportToken ( Anchor, ConvertedToken, ConvertedSize, pToken, Size, Handle, HandleType, Version ) == FALSE ) {
                     free ( ConvertedToken ) ;
                     free ( pToken ) ;
                     break ;
                  } /* endif */
                  free ( pToken ) ;

                  // Store the cooked function.
                  memcpy ( CookedData+CookedSize, ConvertedToken, size_t(ConvertedSize) ) ;
                  CookedSize += ConvertedSize ;
                  free ( ConvertedToken ) ;

                  // If there was a handle involved, store it too.
                  if ( Handle )
                     AddHandle ( Handle, HandleType ) ;

                  // Prepare to get the next token.
                  pToken = Bytes ;
               } /* endwhile */

               // Add on the EOF character.
               CookedData[CookedSize++] = 0x1A ;

               free ( RawData ) ;
               pText = new Text ( CookedData, CookedSize ) ;
               free ( CookedData ) ;

            } else {
               #ifdef DEBUG_CONSTRUCTOR
                  Log ( "Documento(%08X)::Documento() Creating empty document.", this ) ;
               #endif // DEBUG_CONSTRUCTOR

               if ( RawData )
                  free ( RawData ) ;
               pText = new Text ( "\x1A", 1 ) ;

            } /* endif */

         } else {
            #ifdef DEBUG_CONSTRUCTOR
               Log ( "Documento(%08X)::Documento() Creating empty document.", this ) ;
            #endif // DEBUG_CONSTRUCTOR
            if ( RawData ) 
               free ( RawData ) ;
            pText = new Text ( "\x1A", 1 ) ;

         } /* endif */

      } else {

         #ifdef DEBUG_CONSTRUCTOR
            Log ( "Documento(%08X)::Documento() Loading document in native format.", this ) ;
         #endif // DEBUG_CONSTRUCTOR
         FILE *File = fopen ( PCHAR(filename), "rb" ) ;
         if ( File ) {

            // Process the document preface, if there is one.
            int Version ( 0 ) ;
            long Start ( ReadPreface ( fileno(File), DocParms, Version ) ) ;

            // Find how much data is left to read in.
            fseek ( File, 0, SEEK_END ) ;
            long Size = ftell ( File ) - Start ;

            // If there remains any data to process, do so now.
            if ( Size > 0 ) {

               // Read all the remaining data in the file.
               fseek ( File, Start, SEEK_SET ) ;
               PCHAR RawData = PCHAR ( malloc ( size_t ( Size ) ) ) ;
               fread ( RawData, 1, size_t(Size), File ) ;

               // Allocate space for the cooked data.  It has no size yet.
               PCHAR CookedData = PCHAR ( malloc ( size_t ( Size ) ) ) ;
               long CookedSize = 0 ;

               // Prepare to process the raw data into cooked data.
               TEXTDATA TextData ;
               TextData.Buffer = PUCHAR(RawData) ;
               TextData.Index = 0 ;
               TextData.Size = Size ;

               UCHAR Bytes [2] ;
               PUCHAR pToken = Bytes ;
               while ( GetTokenData ( pToken, Size, PREADER(ReadText), PREADER(PeekText), &TextData ) ) {

                  // If not a function, simply store it into the cooked data.
                  if ( Size <= 2 ) {
                     memcpy ( CookedData+CookedSize, Bytes, size_t(Size) ) ;
                     CookedSize += Size ;
                     continue ;
                  } /* endif */

                  // Convert the function to internal format.
                  PUCHAR ConvertedToken = PUCHAR ( malloc ( size_t ( Size ) ) ) ;
                  long ConvertedSize ;  LONG Handle = 0 ;  int HandleType ;
                  if ( ImportToken ( Anchor, ConvertedToken, ConvertedSize, pToken, Size, Handle, HandleType, Version ) == FALSE ) {
                     free ( ConvertedToken ) ;
                     free ( pToken ) ;
                     break ;
                  } /* endif */
                  free ( pToken ) ;

                  // Store the cooked function.
                  memcpy ( CookedData+CookedSize, ConvertedToken, size_t(ConvertedSize) ) ;
                  CookedSize += ConvertedSize ;
                  free ( ConvertedToken ) ;

                  // If there was a handle involved, store it too.
                  if ( Handle )
                     AddHandle ( Handle, HandleType ) ;

                  // Prepare to get the next token.
                  pToken = Bytes ;
               } /* endwhile */

               // Add on the EOF character.
               CookedData[CookedSize++] = 0x1A ;

               free ( RawData ) ;
               pText = new Text ( CookedData, CookedSize ) ;
               free ( CookedData ) ;

            } else {
               #ifdef DEBUG_CONSTRUCTOR
                  Log ( "Documento(%08X)::Documento() Creating empty document text.", this ) ;
               #endif // DEBUG_CONSTRUCTOR
               pText = new Text ( "\x1A", 1 ) ;

            } /* endif */
            fclose ( File ) ;

         } else {
            #ifdef DEBUG_CONSTRUCTOR
               Log ( "Documento(%08X)::Documento() Creating empty document text.", this ) ;
            #endif // DEBUG_CONSTRUCTOR
            pText = new Text ( "\x1A", 1 ) ;

         } /* endif */

      } /* endif */

   } else {

      DocParms.CreateDate = time ( 0 ) ;

      #ifdef DEBUG_CONSTRUCTOR
         Log ( "Documento(%08X)::Documento() Creating empty document text.", this ) ;
      #endif // DEBUG_CONSTRUCTOR
      pText = new Text ( "\x1A", 1 ) ;

   } /* endif */

   // Pass the text pointer around.
   #ifdef DEBUG_CONSTRUCTOR
      Log ( "Documento(%08X)::Documento() Passing text pointer to current and working pages.", this ) ;
   #endif // DEBUG_CONSTRUCTOR
   CurrentPage.Set_Text ( pText ) ;
   WorkingPage.Set_Text ( pText ) ;

   // Update the document defaults.
   #ifdef DEBUG_CONSTRUCTOR
      Log ( "Documento(%08X)::Documento() Updating document defaults.", this ) ;
   #endif // DEBUG_CONSTRUCTOR
   SetDefaults ( ) ;

   // Reset scrolling position.
   #ifdef DEBUG_CONSTRUCTOR
      Log ( "Documento(%08X)::Documento() Resetting scroll position.", this ) ;
   #endif // DEBUG_CONSTRUCTOR
   TopLeft.x = 0 ;
   TopLeft.y = State0.FormInfo.cy ;

   // Create the working presentation space and initialize its state.
   #ifdef DEBUG_CONSTRUCTOR
      Log ( "Documento(%08X)::Documento() Creating working presentation space.", this ) ;
   #endif // DEBUG_CONSTRUCTOR
   PS = new WorkSpace ( "Document", DocParms.CodePage, Anchor, int(State0.Metric), int(State0.FormInfo.cx), int(State0.FormInfo.cy) ) ;
   #ifdef DEBUG_CONSTRUCTOR
      Log ( "Documento(%08X)::Documento() Setting working presentation space state.", this ) ;
   #endif // DEBUG_CONSTRUCTOR
   PS->SetState ( State0 ) ;

   // Update the document defaults (again, to get header dimensions).
   #ifdef DEBUG_CONSTRUCTOR
      Log ( "Documento(%08X)::Documento() Updating document defaults.", this ) ;
   #endif // DEBUG_CONSTRUCTOR
   SetDefaults ( ) ;

   // Get the current printer resolution.
   { /* startblock */
      #ifdef DEBUG_CONSTRUCTOR
         Log ( "Documento(%08X)::Documento() Getting printer information.", this ) ;
      #endif // DEBUG_CONSTRUCTOR
      QueueInfo Info ( "Documento::Documento", 0, DocParms.Printer, DocParms.FormName, PDRIVDATA(DocParms.JobSettings) ) ;
      #ifdef DEBUG_CONSTRUCTOR
         Log ( "Documento(%08X)::Documento() Getting printer resolution.", this ) ;
      #endif // DEBUG_CONSTRUCTOR
      Info.QueryResolution ( Anchor, DocParms.Metric, PrinterResolution.x, PrinterResolution.y ) ;
   } /* endblock */

   // Create the format supervisor object.
   #ifdef DEBUG_CONSTRUCTOR
      Log ( "Documento(%08X)::Documento() Creating format supervisor.", this ) ;
   #endif // DEBUG_CONSTRUCTOR
   SupervisorObject = new Supervisor ( this ) ;

   // Format the document.
   #ifdef DEBUG_CONSTRUCTOR
      Log ( "Documento(%08X)::Documento() Starting document formatting.", this ) ;
   #endif // DEBUG_CONSTRUCTOR
   SupervisorObject->FormatPage ( TRUE ) ;

   #ifdef DEBUG_CONSTRUCTOR
      Log ( "Documento(%08X)::Documento() Done.", this ) ;
   #endif // DEBUG_CONSTRUCTOR
}
 
/****************************************************************************
 *                                                                          *
 *  Document: Copy Constructor                                              *
 *                                                                          *
 ****************************************************************************/

Documento::Documento ( HAB anchor, Documento &Original, BOOL ForSaveOnly ) :
   Anchor(anchor), PainterObject(0), PS(0),
   StatusWindow(0), SupervisorObject(0), TitleWindow(0), Window(0), 
   WindowThreadDone("WindowThreadDone"), WindowThreadReady("WindowThreadReady"),
   CurrentPageLock("Documento::CurrentPageLock") {

   #ifdef DEBUG_CONSTRUCTOR
      Log ( "Documento(%08X)::Documento() Copy constructor started.", this ) ;
   #endif // DEBUG_CONSTRUCTOR

   #ifdef DEBUG_LOCK
      CurrentPageLock.SetDebug(TRUE) ;
   #endif // DEBUG_LOCK

   // Clear the working and current pages.
   CurrentPageValid = FALSE ;

   Rulers[0] = Rulers[1] = 0 ;
   ScrollBars[0] = ScrollBars[1] = 0 ;

   // Copy all the original document's attributes.
   Colors[0]        = Original.Colors[0] ;
   Colors[1]        = Original.Colors[1] ;
   Column           = Original.Column ;
   DocParms         = Original.DocParms ;
   fDocParms        = Original.fDocParms ;
   FileDate         = Original.Query_CurrentLevel() ? time(0) : Original.FileDate ;
   Filename         = new String ( *(Original.Filename) ) ;
   FileType         = new String ( *(Original.FileType) ) ;
   FullNames        = Original.FullNames ;
   HandleCount      = Original.HandleCount ;
   InsertMode       = Original.InsertMode ;
   Library          = Original.Library ;
   Minimized        = Original.Minimized ;
   NumberOfPages    = Original.NumberOfPages ;
   Offset           = 0 ;
   OffsetLast       = 0 ;
   OffsetOrigin     = 0 ;
   PageNumber       = Original.PageNumber ;
   Position         = Original.Position ;
   PrinterResolution= Original.PrinterResolution ;
   pText            = new Text ( *(Original.pText) ) ;
   SavedLevel       = Original.SavedLevel ;
   ScrollMax        = Original.ScrollMax ;
   ScrollPos        = Original.ScrollPos ;
   Size             = Original.Size ;
   State            = Original.State ;
   State0           = Original.State0 ;
   Title            = new String ( *(Original.Title) ) ;
   TopLeft          = Original.TopLeft ;
   Zoom             = Original.Zoom ;

   memcpy ( CursorBox, Original.CursorBox, sizeof(CursorBox) ) ;
   memcpy ( Pages, Original.Pages, sizeof(Pages) ) ;

   // Pass the text pointer around.
   CurrentPage.Set_Text ( pText ) ;
   WorkingPage.Set_Text ( pText ) ;

   // Duplicate the resources.
   if ( HandleCount ) {
      DeviceContext Memory = DeviceContext ( "Documento::Documento", long(Anchor) ) ;
      WorkSpace MemoryPS ( "Documento::Documento", 0, &Memory, Anchor, TRUE ) ;
      memcpy ( HandleTypes, Original.HandleTypes, sizeof(HandleTypes) ) ;
      for ( int i=0; i<HandleCount; i++ ) {
         if ( HandleTypes[i] ) {
            Handles[i] = LONG ( GpiCopyMetaFile ( HMF(Original.Handles[i]) ) ) ;
            #ifdef DEBUG_HANDLES
               Log ( "Documento(%08X)::Documento(Anchor,Original) Created metafile %08p from original %08p.",
                  this, Handles[i], Original.Handles[i] ) ;
            #endif
         } else {
            Handles[i] = MemoryPS.CreateBitmap ( HBITMAP(Original.Handles[i]) ) ;
            #ifdef DEBUG_HANDLES
               Log ( "Documento(%08X)::Documento(Anchor,Original) Created bitmap %08p from original %08p.",
                  this, Handles[i], Original.Handles[i] ) ;
            #endif
         } /* endif */
      } /* endfor */
   } /* endif */

   // Fix the resource handles within the document text itself.
   pText->Set_Scanning ( TRUE ) ;
   Offset = 0 ;
   PUCHAR pByte ;
   while ( pText->QueryBytePtr ( Offset, pByte ) ) {
      if ( IsFunction ( *pByte ) ) {
         if ( IsGraphic ( pByte ) ) {
            LONG Handle = * PLONG ( pByte + 2 + sizeof(POINTL) + sizeof(SIZEL) + 5 + 2 ) ;
            for ( int i=0; i<HandleCount; i++ ) {
               if ( Handle == Original.Handles[i] ) {
                  #ifdef DEBUG_HANDLES
                     Log ( "Documento(%08X)::Documento(Anchor,Original) Replacing image handle %08p with %08p at offset %i.",
                        this, Original.Handles[i], Handles[i], Offset ) ;
                  #endif
                  *PLONG ( pByte + 2 + sizeof(POINTL) + sizeof(SIZEL) + 5 + 2 ) = Handles[i] ;
                  break ;
               } /* endif */
            } /* endfor */
         } /* endif */
         long LengthIn ;
         ComputeFunctionSize ( pByte, LengthIn ) ;
         Offset += LengthIn ;
      } else {
         Offset ++ ;
      } /* endif */
   } /* endwhile */
   Offset = 0 ;
   pText->QueryBytePtr ( 0, pByte ) ;
   pText->Set_Scanning ( FALSE ) ;

   // Create the working presentation space and initialize its state.
   PS = new WorkSpace ( "Document", DocParms.CodePage, Anchor, int(State0.Metric), int(State0.FormInfo.cx), int(State0.FormInfo.cy) ) ;
   PS->SetState ( State0 ) ;

   // If not duplicating for save purposes only . . .
   if ( NOT ForSaveOnly ) {

      // Create the format supervisor object.
      SupervisorObject = new Supervisor ( this ) ;

      // Format the document.
      SupervisorObject->FormatPage ( TRUE ) ;

   } /* endif */

   #ifdef DEBUG_CONSTRUCTOR
      Log ( "Documento(%08X)::Documento() Copy constructor done.", this ) ;
   #endif // DEBUG_CONSTRUCTOR
}
 
/****************************************************************************
 *                                                                          *
 *  Document: Destructor                                                    *
 *                                                                          *
 ****************************************************************************/

Documento::~Documento ( ) {
   #ifdef DEBUG_CREATE_DESTROY
      Log ( "Documento(%08X)::~Documento: Started.  Destroying supervisor object.", this ) ;
   #endif
   if ( SupervisorObject ) delete SupervisorObject, SupervisorObject = 0 ;
   #ifdef DEBUG_CREATE_DESTROY
      Log ( "Documento(%08X)::~Documento: Destroying painter object.", this ) ;
   #endif
   if ( PainterObject ) delete PainterObject, PainterObject = 0 ;
   #ifdef DEBUG_CREATE_DESTROY
      Log ( "Documento(%08X)::~Documento: Destroying window.", this ) ;
   #endif
   if ( Window ) DestroyWindow ( 0, TRUE ), Window = 0 ;
   #ifdef DEBUG_CREATE_DESTROY
      Log ( "Documento(%08X)::~Documento: Destroying title.", this ) ;
   #endif
   if ( Title ) delete Title, Title = 0 ;
   #ifdef DEBUG_CREATE_DESTROY
      Log ( "Documento(%08X)::~Documento: Destroying PS.", this ) ;
   #endif
   if ( PS ) delete PS, PS = 0 ;
   #ifdef DEBUG_CREATE_DESTROY
      Log ( "Documento(%08X)::~Documento: Destroying any bitmaps/metafiles.", this ) ;
   #endif
   for ( int i=0; i<HandleCount; i++ ) {
      if ( HandleTypes[i] ) {
         #ifdef DEBUG_HANDLES
            Log ( "Documento(%08X)::~Documento() Deleting metafile %08p.", this, Handles[i] ) ;
         #endif
         GpiDeleteMetaFile ( HMF(Handles[i]) ) ;
      } else {
         #ifdef DEBUG_HANDLES
            Log ( "Documento(%08X)::~Documento() Deleting bitmap %08p.", this, Handles[i] ) ;
         #endif
         GpiDeleteBitmap ( HBITMAP(Handles[i]) ) ;
      } /* endif */
   } /* endfor */
   #ifdef DEBUG_CREATE_DESTROY
      Log ( "Documento(%08X)::~Documento: Destroying filename string.", this ) ;
   #endif
   delete Filename ;
   #ifdef DEBUG_CREATE_DESTROY
      Log ( "Documento(%08X)::~Documento: Destroying filetype string.", this ) ;
   #endif
   delete FileType ;
   #ifdef DEBUG_CREATE_DESTROY
      Log ( "Documento(%08X)::~Documento: Destroying text object.", this ) ;
   #endif
   delete pText ;
   #ifdef DEBUG_CREATE_DESTROY
      Log ( "Documento(%08X)::~Documento: Done.", this ) ;
   #endif
}
 
/****************************************************************************
 *                                                                          *
 *  Document: Create Window and Window Thread                               *
 *                                                                          *
 ****************************************************************************/

typedef struct {

        // Input

   Event *Ready ;               // Event to signal window thread ready.
   Event *Done ;                // Event to signal window thread done.
   HWND Owner ;                 // Window owner.
   Documento *pDocument ;       // -> Document
   LanguageModule *Library ;    // -> Language Library
   INIDATA *IniData ;           // -> Profile
   Dictionary *Speller ;        // -> Spell Checker
   int WindowID ;               // Window ID.
   int Trap ;                   // Flag: Trap Ctrl+Esc, etc.

        // Results

   HWND Window ;                // Document window handle.

} WINDOW_PARMS ;

static void Window_Thread ( PVOID Parameter ) {

   #ifdef DEBUG_WINDOWTHREAD
      Log ( "Window_Thread: Started." ) ;
   #endif

   WINDOW_PARMS *Parms = (WINDOW_PARMS*) Parameter ;
   Event *Done = Parms->Done ;

   #ifdef DEBUG_WINDOWTHREAD
      Log ( "Window_Thread: Creating process object." ) ;
   #endif

   {
      Process Proc ( Parms->pDocument->Query_Filename(), Parms->Owner, Parms->Library->QueryHandle(),
         100, USHORT(Parms->pDocument->Query_Parms()->CodePage) ) ;
      if ( Proc.QueryAnchor() == 0 ) {
         Parms->Window = 0 ;
         Parms->Ready->Post ( ) ;
         return ;
      } /* endif */

      #ifdef DEBUG_WINDOWTHREAD
         Log ( "Window_Thread: Creating help object." ) ;
      #endif

      char Drive [_MAX_DRIVE+1], Dir[_MAX_DIR+1], Fname[_MAX_FNAME+1], Ext[_MAX_EXT+1] ;
      _splitpath ( Parms->Library->QueryName(), Drive, Dir, Fname, Ext ) ;
      char HelpFileName [CCHMAXPATH] ;
      sprintf ( HelpFileName, "%s\\%s.hlp", HomePath, Fname ) ;
      ResourceString HelpTitle ( Parms->Library->QueryHandle(), IDS_HELPTITLE ) ;
      HelpWindow Help ( Proc.QueryAnchor(), 0, ID_MAIN, PSZ(HelpFileName), PSZ(HelpTitle) ) ;

      #ifdef DEBUG_WINDOWTHREAD
         Log ( "Window_Thread: Creating document frame window." ) ;
      #endif

      DOCFRAME_PARMS FrameParms ;
      FrameParms.Help = & Help ;
      FrameParms.IniData = Parms->IniData ;
      FrameParms.Library = Parms->Library ;
      FrameParms.MainWindow = Parms->Owner ;
      FrameParms.pDocument = Parms->pDocument ;
      FrameParms.pSpeller = Parms->Speller ;
      FrameParms.WindowID = USHORT ( Parms->WindowID ) ;
      FrameParms.Trap = Parms->Trap ;

      char Title [_MAX_PATH+10] ;
      sprintf ( Title, "%s, Page 1", Parms->pDocument->Query_Filename() ) ;

      Window Doc ( Parms->Owner, PSZ(DOCFRAME_CLASS_NAME), PSZ(Title),
         0, 0, 0, 0, 0, Parms->Owner, HWND_BOTTOM, Parms->WindowID, &FrameParms, 0 ) ;

      #ifdef DEBUG_WINDOWTHREAD
         Log ( "Window_Thread: Notifying CreateWindow of thread initialization completed." ) ;
      #endif

      Parms->Window = Doc.QueryHandle() ;
      Parms->Ready->Post ( ) ;

      #ifdef DEBUG_WINDOWTHREAD
         Log ( "Window_Thread: Processing messages." ) ;
      #endif

      Sys_ProcessMessages ( Proc.QueryAnchor() ) ;

   }

   #ifdef DEBUG_WINDOWTHREAD
      Log ( "Window_Thread: Signaling completion." ) ;
   #endif

   Done->Post ( ) ;

   #ifdef DEBUG_WINDOWTHREAD
      Log ( "Window_Thread: Done." ) ;
   #endif

}

HWND Documento::CreateWindow ( HWND Owner, INIDATA *IniData, Dictionary *Speller, int WindowID, int Trap ) {

   #ifdef DEBUG_WINDOWTHREAD
      Log ( "Documento::CreateWindow: Started." ) ;
   #endif

   if ( Window ) {
      Log ( "Documento::CreateWindow: WARNING: Window already created!" ) ;
      return ( Window ) ;
   } /* endif */

   WindowThreadReady.Reset ( ) ;
   WindowThreadDone.Reset ( ) ;

   WINDOW_PARMS Parms ;
   Parms.Ready = & WindowThreadReady ;
   Parms.Done = & WindowThreadDone ;
   Parms.Owner = Owner ;
   Parms.pDocument = this ;
   Parms.Library = Library ;
   Parms.IniData = IniData ;
   Parms.Speller = Speller ;
   Parms.WindowID = WindowID ;
   Parms.Trap = Trap ;

   WindowThreadId = StartThread ( "WindowThread", Window_Thread, 0x8000, &Parms ) ;
   WindowThreadReady.Wait ( 60000 ) ;

   #ifdef DEBUG_WINDOWTHREAD
      Log ( "Documento::CreateWindow: Window open for business.  Handle %08p.", Parms.Window ) ;
   #endif

   return ( Parms.Window ) ;
}

BOOL Documento::DestroyWindow ( HWND Owner, BOOL Mandatory ) {

   #ifdef DEBUG_WINDOWTHREAD
      Log ( "Documento::DestroyWindow: Started.  Flushing the command queue." ) ;
   #endif
   FlushCommandQueue ( ) ;

   // If the owner window is specified, and destruction not mandatory, and changes are present, ask the user.
   if ( Owner AND NOT Mandatory AND IsChanged ( ) ) {

      #ifdef DEBUG_WINDOWTHREAD
         Log ( "Documento::DestroyWindow: Asking user if he wants to save or discard changes." ) ;
      #endif

      // Determine the file name, if any has been set yet.
      ResourceString NewDocument ( Library->QueryHandle(), IDS_NEW_DOCUMENT ) ;
      PSZ Filename = Query_Filename ( ) ;
      if ( Filename == NULL )
         Filename = PSZ(NewDocument) ;

      // Build save prompt.
      ResourceString Format ( Library->QueryHandle(), IDS_PROMPT_SAVE ) ;
      char Message [500] ;
      sprintf ( Message, PCHAR(Format), Filename ) ;

      // Ask user's opinion.
      #ifdef DEBUG_WINDOWTHREAD
         Log ( "Document::DestroyWindow: Asking if user wants to save changed file." ) ;
      #endif
      ResourceString Title1 ( Library->QueryHandle(), IDS_TITLE1 ) ;
      APIRET rc = MessageBox ( HWND_DESKTOP, Owner, Message, PCHAR(Title1),
         IDD_PROMPT_SAVE, MB_YESNOCANCEL | MB_ICONQUESTION | MB_DEFBUTTON3 | MB_HELP, 
         Library->QueryHandle(), IDS_MESSAGE_BOX_STRINGS ) ; 
      #ifdef DEBUG_WINDOWTHREAD
         Log ( "Document::DestroyWindow: Done asking if changes should be saved." ) ;
      #endif

      // If user cancelled, exit w/o having done anything.
      if ( rc == MBID_CANCEL ) {
         #ifdef DEBUG_WINDOWTHREAD
            Log ( "Document::DestroyWindow: File save cancelled." ) ;
         #endif
         return ( FALSE ) ;
      } /* endif */

      // If user said Yes, then we must save the file.  Exit error if unable to do so or aborted.
      if ( rc == MBID_YES ) {
         #ifdef DEBUG_WINDOWTHREAD
            Log ( "Document::DestroyWindow: Trying to save file." ) ;
         #endif
         PSZ Filename = Query_Filename ( ) ;
         BOOL Prompt = ( Filename == 0 ) OR ( Filename[0] == 0 ) ;
         if ( !Save ( Owner, Prompt ) ) {
            #ifdef DEBUG_WINDOWTHREAD
               Log ( "Document::DestroyWindow: File save aborted." ) ;
            #endif
            return ( FALSE ) ;
         } /* endif */
         #ifdef DEBUG_WINDOWTHREAD
            Log ( "Document::DestroyWindow: File saved." ) ;
         #endif
      } /* endif */

   } /* endif */

   if ( Window ) {
      #ifdef DEBUG_WINDOWTHREAD
         Log ( "Documento::DestroyWindow: Increasing the priority of the window thread %i.", WindowThreadId ) ;
      #endif
      DosSetPriority ( PRTYS_THREAD, PRTYC_TIMECRITICAL, PRTYD_MAXIMUM, WindowThreadId ) ;
      #ifdef DEBUG_WINDOWTHREAD
         Log ( "Documento::DestroyWindow: Disassociating window from document." ) ;
      #endif
      HWND WindowHandle = Window ;
      Set_Window ( 0, 0 ) ;
      #ifdef DEBUG_WINDOWTHREAD
         Log ( "Documento::DestroyWindow: Obtaining the window queue handle." ) ;
      #endif
      HMQ Queue = HMQ ( WinQueryWindowULong ( WindowHandle, QWL_HMQ ) ) ;
      #ifdef DEBUG_WINDOWTHREAD
         Log ( "Documento::DestroyWindow: Posting WM_QUIT to window queue %08p.", Queue ) ;
      #endif
      WinPostQueueMsg ( Queue, WM_QUIT, 0, 0 ) ;
      #ifdef DEBUG_WINDOWTHREAD
         Log ( "Documento::DestroyWindow: Waiting for window thread to complete." ) ;
      #endif
      WindowThreadDone.Wait ( 60000 ) ;
      #ifdef DEBUG_WINDOWTHREAD
         Log ( "Documento::DestroyWindow: Window thread has terminated." ) ;
      #endif
      Window = 0 ;

   } /* endif */

   #ifdef DEBUG_WINDOWTHREAD
      Log ( "Documento::DestroyWindow: Done." ) ;
   #endif
   return ( TRUE ) ;
}

/****************************************************************************
 *                                                                          *
 *  Document: Set Window                                                    *
 *                                                                          *
 ****************************************************************************/

void Documento::Set_Window ( HWND window, DeviceContext *pdevice ) {

   #ifdef DEBUG_SETWINDOW
      Log ( "Documento::Set_Window() starting." ) ;
   #endif

   if ( Window != window ) {

      // Delete previously existing painter.
      if ( Window )
         delete PainterObject, PainterObject = 0 ;

      // Save window handle.
      Window = window ;
      pWindowDevice = pdevice ;

      // If we are associating the document with a window . . .
      if ( Window ) {

         // Determine window metrics.
         #ifdef DEBUG_SETWINDOW
            Log ( "Documento::Set_Window() Querying window size." ) ;
         #endif // DEBUG_SETWINDOW
         SIZEL WindowSize ;
         Query_WindowSize ( WindowSize ) ;

         // Set window viewing transform.
         #ifdef DEBUG_SETWINDOW
            Log ( "Documento::Set_Window() Setting transform." ) ;
         #endif // DEBUG_SETWINDOW
         PS->SetTransform ( Zoom, int(TopLeft.x), int(TopLeft.y-WindowSize.cy) ) ;

         // Create new painter.
         #ifdef DEBUG_SETWINDOW
            Log ( "Documento::Set_Window() Creating painter." ) ;
         #endif // DEBUG_SETWINDOW
         PainterObject = new Painter ( Window, pWindowDevice, this ) ;

         // Update the title-bar.
         #ifdef DEBUG_SETWINDOW
            Log ( "Documento::Set_Window() Updating title." ) ;
         #endif // DEBUG_SETWINDOW
         UpdateTitle ( ) ;

         // Compute the cursor state, position and size.
         #ifdef DEBUG_SETWINDOW
            Log ( "Documento::Set_Window() Computing initial state." ) ;
         #endif // DEBUG_SETWINDOW
         ComputeState ( ) ;

         // Update the cursor and status.
         #ifdef DEBUG_SETWINDOW
            Log ( "Documento::Set_Window() Updating scroll bars, etc." ) ;
         #endif // DEBUG_SETWINDOW
         UpdateScrollBars ( ) ;
         UpdatePosition ( ) ;
         UpdateRulers ( ) ;
         UpdateStatus ( ) ;
         UpdateCursor ( ) ;
         UpdateMouse ( ) ;

      } /* endif */

   } /* endif */

   #ifdef DEBUG_SETWINDOW
      Log ( "Documento::Set_Window() done." ) ;
   #endif
}
 
/****************************************************************************
 *                                                                          *
 *  Document: Set Language                                                  *
 *                                                                          *
 ****************************************************************************/

void Documento::Set_Language ( LanguageModule *NewLibrary ) {

   // Flush the command queue first.
   FlushCommandQueue ( ) ;

   // Set new library module pointer.
   Library = NewLibrary ;

   // Tell the window.  The window must reload its context menus and reset its status windows.
   Sys_SendMessage ( Window, WM_SET_LANGUAGE, MPFROMP(Library), 0 ) ;

   // Reload the accelerator table.
   HWND Frame = PARENT ( Window ) ;
   HACCEL Accelerator = WinQueryAccelTable ( WinQueryAnchorBlock(Window), Frame ) ;
   if ( Accelerator ) {
      if ( !WinDestroyAccelTable ( Accelerator ) ) {
         char Message [512] ;
         Log ( "Document::Set_Language: Unable to destroy old accelerator table.", InterpretWinError(0,Message) ) ;
      } /* endif */
   } else {
      char Message [512] ;
      Log ( "Document::Set_Language: Unable to query old accelerator table handle.", InterpretWinError(0,Message) ) ;
   } /* endif */
   Accelerator = WinLoadAccelTable ( WinQueryAnchorBlock(Window), Library->QueryHandle(), ID_DATAWINDOW ) ;
   if ( Accelerator ) {
      if ( !WinSetAccelTable ( WinQueryAnchorBlock(Window), Accelerator, Frame ) ) {
         char Message [512] ;
         Log ( "Document::Set_Language: Unable to set new accelerator table.", InterpretWinError(0,Message) ) ;
      } /* endif */
   } else {
      char Message [512] ;
      Log ( "Document::Set_Language: Unable to load new accelerator table.", InterpretWinError(0,Message) ) ;
   } /* endif */

   // Create new help instance.
   char Drive [_MAX_DRIVE+1], Dir[_MAX_DIR+1], Fname[_MAX_FNAME+1], Ext[_MAX_EXT+1] ;
   _splitpath ( Library->QueryName(), Drive, Dir, Fname, Ext ) ;
   char HelpFileName [CCHMAXPATH] ;
   sprintf ( HelpFileName, "%s\\%s.hlp", HomePath, Fname ) ;
   ResourceString HelpTitle ( Library->QueryHandle(), IDS_HELPTITLE ) ;
   Help = new HelpWindow ( WinQueryAnchorBlock(Window), 0, ID_MAIN, PSZ(HelpFileName), PSZ(HelpTitle) ) ;
   Help->Associate ( Window ) ;

   // Update title.
   UpdateTitle ( ) ;

   // Update Status.
   UpdateStatus ( ) ;

   // Mark the first page Dirty.
   Pages[0].NeedsFormat = TRUE ;
   Pages[0].State = State0 ;
   Pages[0].Offset = 0 ;

   // Mark current page invalid, so that word list does not get reused.
   CurrentPageValid = FALSE ;

   // Set all pages dirty.
   SetPagesDirty ( 0, pText->QueryCurrentSize()-1 ) ;

   // Reformat the current page.
   FormatPage ( ) ;
}
 
/****************************************************************************
 *                                                                          *
 *  Document: Set Zoom                                                      *
 *                                                                          *
 ****************************************************************************/

void Documento::Set_Zoom ( FIXED zoom ) {

  /**************************************************************************
   * Set the new scaling/translation transformation.                        *
   **************************************************************************/

   Zoom = zoom ;

   #ifdef DEBUG
      double dZoom = double ( FIXEDINT ( Zoom ) ) + double ( FIXEDFRAC ( Zoom ) ) / 0x10000 ;
      Log ( "Documento::Set_Zoom(%7.3lf)", dZoom ) ;
   #endif

  /**************************************************************************
   * Update the window, if it's defined.                                    *
   **************************************************************************/

   Rezoom ( ) ;

}
 
/****************************************************************************
 *                                                                          *
 *  Document: Rezoom                                                        *
 *                                                                          *
 ****************************************************************************/

void Documento::Rezoom ( ) {

   #ifdef DEBUG
   Log ( "Documento::Rezoom() starting." ) ;
   #endif

  /**************************************************************************
   * Determine window metrics.                                              *
   **************************************************************************/

   SIZEL WindowSize ;
   Query_WindowSize ( WindowSize ) ;

  /**************************************************************************
   * Set the window viewing transform.                                      *
   **************************************************************************/

   PS->SetTransform ( Zoom, int(TopLeft.x), int(TopLeft.y-WindowSize.cy) ) ;

  /**************************************************************************
   * Update the window, if it's defined.                                    *
   **************************************************************************/

   if ( Window ) {
      Sys_InvalidateWindow ( Window ) ;
      UpdateScrollBars ( ) ;
      UpdatePosition ( ) ;
      UpdateRulers ( ) ;
      UpdateCursor ( ) ;
      UpdateMouse ( ) ;

   } /* endif */

   #ifdef DEBUG
   Log ( "Documento::Rezoom() done." ) ;
   #endif
}
 
/****************************************************************************
 *                                                                          *
 *  Document: Set Colors                                                    *
 *                                                                          *
 ****************************************************************************/

void Documento::Set_Colors ( COLOR colors[] ) {
   Colors[0] = colors[0] ;
   Colors[1] = colors[1] ;
}
 
/****************************************************************************
 *                                                                          *
 *  Document: Set Filename (replacing old value)                            *
 *                                                                          *
 ****************************************************************************/

void Documento::Set_Name ( PSZ filename ) {
   if ( ( PSZ(*Filename) == NULL ) OR ( strcmp ( PCHAR(*Filename), PCHAR(filename) ) ) ) {
      if ( Filename ) 
         delete Filename ;
      Filename = new String ( filename ) ;
   } /* endif */
}
 
/****************************************************************************
 *                                                                          *
 *  Document: Set FileType (replacing old value)                            *
 *                                                                          *
 ****************************************************************************/

void Documento::Set_Type ( PSZ filetype ) {
   if ( ( PSZ(*FileType) == NULL ) OR ( strcmp ( PCHAR(*FileType), PCHAR(filetype) ) ) ) {
      if ( FileType ) 
         delete FileType ;
      FileType = new String ( filetype ) ;
   } /* endif */
}
 
/****************************************************************************
 *                                                                          *
 *  Document: Save to File (High-Level)                                     *
 *                                                                          *
 ****************************************************************************/

BOOL Documento::Save ( HWND Owner, BOOL Rename ) {

   // Flush any queued edits.
   FlushCommandQueue ( ) ;

   // Prepare for the file name dialog.
   PSZ Filename = Query_Filename() ;
   FILEDLG DialogData ;
   memset ( &DialogData, 0, sizeof(DialogData) ) ;
   PrepareFileTypeList ( TRUE ) ;
   FILEDIALOG_DATA FileDialogData = { FALSE, FileTypeList, FileWildcardList, FileExtenderList, 0 } ;
   for ( int i=1; FileTypeList[i]; i++ ) {
      if ( !stricmp ( FileTypeList[i], Query_FileType() ) ) {
         FileDialogData.Index = i ;
         break ;
      } /* endif */
   } /* endfor */

   // Ask for the file's new name if it doesn't have one or we are renaming.
   if ( ( Filename == NULL ) OR Rename ) {
      ResourceString SaveDocument ( Library->QueryHandle(), IDS_SAVE_DOCUMENT ) ;
      DialogData.cbSize = sizeof(DialogData) ;
      DialogData.fl = FDS_SAVEAS_DIALOG | FDS_CENTER | FDS_HELPBUTTON | FDS_FILTERUNION ;
      DialogData.pszTitle = PSZ(SaveDocument) ;
      if ( Filename ) {
         strcpy ( DialogData.szFullFile, Filename ) ;
      } else {
         strcpy ( DialogData.szFullFile, FileWildcardList[FileDialogData.Index>0?FileDialogData.Index:1] ) ;
      } /* endif */
      DialogData.pszIType = FileTypeList[FileDialogData.Index>0?FileDialogData.Index:1] ;
      DialogData.papszITypeList = PAPSZ(FileTypeList) ;
      DialogData.pfnDlgProc = FileDialog_Processor ;
      DialogData.ulUser = ULONG ( &FileDialogData ) ;
      #ifdef DEBUG_CLOSE
         Log ( "Document::Save: Invoking File Dialog to get new file name." ) ;
      #endif
      WinFileDlg ( HWND_DESKTOP, Owner, &DialogData ) ;
      #ifdef DEBUG_CLOSE
         Log ( "Document::Save: Done invoking File Dialog." ) ;
      #endif
      if ( DialogData.lReturn != DID_OK )
         return ( FALSE ) ;
      Filename = PSZ(DialogData.szFullFile) ;
      DocParms.CreateDate = time ( 0 ) ;
   } /* endif */

   // Handle the case where the file name is changing.
   if ( ( Query_Filename()==NULL ) OR strcmp ( PCHAR(Filename), PCHAR(Query_Filename()) ) ) {
      if ( access ( PCHAR(Filename), 0 ) == 0 ) {
         ResourceString Title ( Library->QueryHandle(), IDS_TITLE1 ) ;
         ResourceString Format ( Library->QueryHandle(), IDS_PROMPT_SAVECONFIRM1 ) ;
         char Message [500] ;
         sprintf ( Message, PCHAR(Format), Filename ) ;
         #ifdef DEBUG_CLOSE
         Log ( "Document::Save: Asking if user wants to save over file of same name." ) ;
         #endif
         APIRET Result = MessageBox ( HWND_DESKTOP, Owner, PSZ(Message), PSZ(Title),
            IDD_PROMPT_SAVECONFIRM1, MB_YESNOCANCEL | MB_ICONEXCLAMATION | MB_DEFBUTTON2 | MB_HELP, LibraryHandle, IDS_MESSAGE_BOX_STRINGS ) ;
         #ifdef DEBUG_CLOSE
         Log ( "Document::Save: Done asking." ) ;
         #endif
         switch ( Result ) {
            case MBID_YES:
               break;
            case MBID_NO:
               return ( FALSE ) ;
            case MBID_CANCEL:
               return ( FALSE ) ;
         } /* endswitch */
      } /* endif */
      Set_Name ( Filename ) ;
      DocParms.CreateDate = time ( 0 ) ;

   // Handle the case where the file type is changing.
   } else if ( strcmp ( Query_FileType(), FileTypeList[FileDialogData.Index>0?FileDialogData.Index:1] ) ) {
      ResourceString Title ( Library->QueryHandle(), IDS_TITLE1 ) ;
      ResourceString Format ( Library->QueryHandle(), IDS_PROMPT_SAVECONFIRM2 ) ;
      char Message [500] ;
      sprintf ( Message, PCHAR(Format), Query_FileType(), FileTypeList[FileDialogData.Index>0?FileDialogData.Index:1] ) ;
      #ifdef DEBUG_CLOSE
      Log ( "Document::Save: Asking if user wants to save file with new type." ) ;
      #endif
      APIRET Result = MessageBox ( HWND_DESKTOP, Owner, PSZ(Message), PSZ(Title),
         IDD_PROMPT_SAVECONFIRM2, MB_YESNOCANCEL | MB_ICONEXCLAMATION | MB_DEFBUTTON2 | MB_HELP, LibraryHandle, IDS_MESSAGE_BOX_STRINGS ) ;
      #ifdef DEBUG_CLOSE
      Log ( "Document::Save: Done asking." ) ;
      #endif
      switch ( Result ) {
         case MBID_YES:
            break;
         case MBID_NO:
            return ( FALSE ) ;
         case MBID_CANCEL:
            return ( FALSE ) ;
      } /* endswitch */
   } /* endif */

   // Update the document state and title.
   int OldSavedLevel = Query_SavedLevel() ;
   Set_SavedLevel ( Query_CurrentLevel() ) ;
   Set_Type ( FileTypeList [ FileDialogData.Index > 0 ? FileDialogData.Index : 1 ] ) ;
   UpdateTitle ( ) ;

   // Try to save the document.
   ImportExport *Exporter = (ImportExport*) FileExtenderList [ FileDialogData.Index > 0 ? FileDialogData.Index : 1 ] ;
   if ( Save ( Owner, PSZ(Filename), Exporter ) ) 
      // Return TRUE if successful.
      return ( TRUE ) ;

   // Restore state and title if not successful.
   Set_SavedLevel ( OldSavedLevel ) ;
   UpdateTitle ( ) ;
   return ( FALSE ) ;
}

/****************************************************************************
 *                                                                          *
 *  Document: Save to File                                                  *
 *                                                                          *
 ****************************************************************************/

BOOL Documento::Save ( HWND Owner, PSZ filename, ImportExport *Exporter ) {

   // If no file name given, use the current file name.
   if ( ( filename == NULL ) OR ( *filename == 0 ) )
      filename = PSZ ( *Filename ) ;

   // If no file name given, nor preexisting, then return an error.
   if ( filename == NULL ) {
      ResourceString Message ( Library->QueryHandle(), IDS_ERROR_DOCUMENT_NONAME ) ;
      ResourceString Title1 ( Library->QueryHandle(), IDS_TITLE1 ) ;
      MessageBox ( HWND_DESKTOP, Owner, PSZ(Message), PSZ(Title1), 
         0, MB_ENTER | MB_ICONEXCLAMATION, Library->QueryHandle(), IDS_MESSAGE_BOX_STRINGS ) ;
      return ( FALSE ) ;
   } /* endif */

   // If the file has changed, update the file change date.
   if ( Query_CurrentLevel() )
      FileDate = time(0) ;

   // Estimate the exported document text size.
   OffsetOrigin = 0 ;
   OffsetLast = pText->QueryCurrentSize ( ) - 1 ;
   long TextSize = Extract ( ) ;

   // Allocate memory to hold it all.
   char *ExportText = (char*) malloc ( size_t ( 0x10000 + TextSize ) ) ;

   // Load the memory . . . first with the preface function . . .
   int PrefaceSize = 0 ;
   CreatePreface ( ExportText, PrefaceSize ) ;

   // Then with the document text itself . . .
   TextSize = Extract ( 0x10000+TextSize, ExportText+PrefaceSize, TRUE ) ;

   // Finally with an end-of-document marker.
   ExportText[PrefaceSize+TextSize] = 0x1A ;

   // Call the exporter to write the file.
   Exporter->Export ( Owner, filename, ExportText, PrefaceSize+TextSize+1 ) ;

   // Release the allocated memory.
   free ( ExportText ) ;

   // Update the document's file name & type.
   Set_Name ( filename ) ;
   Set_Type ( Exporter->QueryFileType() ) ;

   // Save the document's saved change level.
   SavedLevel = pText->QueryCurrentLevel ( ) ;

   // Reset Document Parms change flag.
   fDocParms = FALSE ;

   // Return no error.
   return ( TRUE ) ;
}
 
/****************************************************************************
 *                                                                          *
 *  Document: Read Preface from File, if it is present.                     *
 *                                                                          *
 ****************************************************************************/

long Documento::ReadPreface ( int Handle, DocumentParms &DocParms, int &Version ) {
   UCHAR Buffer [1024] ;
   FILEDATA FileData ;
   FileData.Handle = Handle ;
   FileData.Capacity = sizeof(Buffer) ;
   FileData.Buffer = Buffer ;
   FileData.Index = 0 ;
   FileData.Size = 0 ;
   return ( ReadPreface ( PREADER(ReadFile), PREADER(PeekFile), PTELLER(TellFile), &FileData, DocParms, Version ) ) ;
}

long Documento::ReadPreface ( char *Text, int Size, DocumentParms &DocParms, int &Version ) {
   TEXTDATA TextData ;
   TextData.Buffer = PUCHAR(Text) ;
   TextData.Index = 0 ;
   TextData.Size = Size ;
   return ( ReadPreface ( PREADER(ReadText), PREADER(PeekText), PTELLER(TellText), &TextData, DocParms, Version ) ) ;
}

long Documento::ReadPreface ( PREADER Reader, PREADER Peeker, PTELLER Teller, PVOID ReaderData, DocumentParms &DocParms, int &Version ) {

   #ifdef DEBUG_READPREFACE
   Log ( "Documento::ReadPreface: Starting." ) ;
   #endif

   // Get the first token from the document file.  If unable to do so, reset file and return.
   UCHAR Bytes [2] ;
   PUCHAR OriginalToken = Bytes ;
   long OriginalSize ;
   if ( GetTokenData ( OriginalToken, OriginalSize, Reader, Peeker, ReaderData ) == FALSE ) {
      #ifdef DEBUG_READPREFACE
      Log ( "Documento::ReadPreface: Aborting because unable to fetch first token." ) ;
      #endif
      return ( 0 ) ;
   } /* endif */

   // If the token is not a function, then reset the file to its start and return.
   if ( *OriginalToken != FUNCTION_START ) {
      #ifdef DEBUG_READPREFACE
      Log ( "Documento::ReadPreface: Aborting because first token was not a function." ) ;
      #endif
      return ( 0 ) ;
   } /* endif */

   // Convert the token to the internal storage format.
   PUCHAR pToken = PUCHAR ( malloc ( size_t ( OriginalSize ) ) ) ;
   long Size ;  LONG ResourceHandle ;  int ResourceHandleType ;  
   if ( ImportToken ( Anchor, pToken, Size, OriginalToken, OriginalSize, ResourceHandle, ResourceHandleType, Version ) == FALSE ) {
      free ( pToken ) ;
      free ( OriginalToken ) ;
      #ifdef DEBUG_READPREFACE
      Log ( "Documento::ReadPreface: Aborting because unable to import first token." ) ;
      #endif
      return ( 0 ) ;
   } /* endif */
   free ( OriginalToken ) ;

   // If the token is not a preface, then reset the file to its start and return.
   if ( *(pToken+1) != FN__PREFACE ) {
      #ifdef DEBUG_READPREFACE
      Log ( "Documento::ReadPreface: Aborting because first token not 'Preface'." ) ;
      #endif
      free ( pToken ) ;
      return ( 0 ) ;
   } /* endif */

   // Prepare to process the preface data, which should be all function codes.
   pToken += 2 ;
   Size -= 4 ;
   TEXTDATA TextData ;
   TextData.Buffer = pToken ;
   TextData.Index = 0 ;
   TextData.Size = Size ;

   // Process each function in the preface data.
   pToken = Bytes ;
   while ( GetTokenData ( pToken, Size, PREADER(ReadText), PREADER(PeekText), &TextData ) ) {

      // If not a function, exit.
      if ( *pToken != FUNCTION_START ) 
         break ;

      #ifdef DEBUG_READPREFACE
      Log ( "Documento::ReadPreface: Processing function '%s'.", pToken+1 ) ;
      #endif

      // Convert the function to internal format.
      PUCHAR ConvertedToken = PUCHAR ( malloc ( size_t ( Size ) ) ) ;
      long ConvertedSize ;
      if ( ImportToken ( Anchor, ConvertedToken, ConvertedSize, pToken, Size, ResourceHandle, ResourceHandleType, Version ) == FALSE ) {
         free ( ConvertedToken ) ;
         free ( pToken ) ;
         return ( Teller(ReaderData) ) ;
      } /* endif */
      free ( pToken ) ;
      pToken = ConvertedToken ;

      // If the function requires processing, do so now, updating the document parameters.
      if ( ConvertedSize ) 
         DocParms.ProcessPrefaceFunction ( pToken ) ;

      // Release the memory allocated for the converted token.
      free ( ConvertedToken ) ;

      // Prepare to get the next token.
      pToken = Bytes ;

   } /* endwhile */

   // Release the text memory.
   free ( TextData.Buffer - 2 ) ;

   // Return the preface size.
   #ifdef DEBUG_READPREFACE
   Log ( "Documento::ReadPreface: Returning.  %i bytes processed.", Teller(ReaderData) ) ;
   #endif
   return ( Teller(ReaderData) ) ;
}
 
/****************************************************************************
 *                                                                          *
 *  Document: Create Preface Function                                       *
 *                                                                          *
 ****************************************************************************/

void Documento::CreatePreface ( char *Buffer, int &Size ) {

   Size = 0 ;

   Size += sprintf ( Buffer, "%c%s", FUNCTION_START, FunctionNames[FN__PREFACE] ) ;

   Size += sprintf ( Buffer+Size, "%c%s%04i%c",
      FUNCTION_START, FunctionNames[FN__VERSION], REVISION_NO, FUNCTION_END ) ;

   Size += sprintf ( Buffer+Size, "%c%s%s%c",
      FUNCTION_START, FunctionNames[FN__TITLE], DocParms.Title, FUNCTION_END ) ;

   Size += sprintf ( Buffer+Size, "%c%s%s%c",
      FUNCTION_START, FunctionNames[FN__AUTHOR], DocParms.Author, FUNCTION_END ) ;

   Size += sprintf ( Buffer+Size, "%c%s%u%c",
      FUNCTION_START, FunctionNames[FN__CREATEDATE], DocParms.CreateDate, FUNCTION_END ) ;

   Size += sprintf ( Buffer+Size, "%c%s%s%c",
      FUNCTION_START, FunctionNames[FN__PRINTER], DocParms.Printer, FUNCTION_END ) ;

   Size += sprintf ( Buffer+Size, "%c%s%s%c",
      FUNCTION_START, FunctionNames[FN__FORM], DocParms.FormName, FUNCTION_END ) ;

   Size += sprintf ( Buffer+Size, "%c%s%i%c",
      FUNCTION_START, FunctionNames[FN__CODEPAGE], DocParms.CodePage, FUNCTION_END ) ;

   Size += sprintf ( Buffer+Size, "%c%s%s%c",
      FUNCTION_START, FunctionNames[FN__FONT], DocParms.Font, FUNCTION_END ) ;

   Size += sprintf ( Buffer+Size, "%c%s%.1lf%c",
      FUNCTION_START, FunctionNames[FN__SIZE], (double)DocParms.Size/10.0, FUNCTION_END ) ;

   Size += sprintf ( Buffer+Size, "%c%s%06lX%c",
      FUNCTION_START, FunctionNames[FN__LINECOLOR], DocParms.Color, FUNCTION_END ) ;

   Size += sprintf ( Buffer+Size, "%c%s%06lX%c",
      FUNCTION_START, FunctionNames[FN__FILLCOLOR], DocParms.FillColor, FUNCTION_END ) ;

   Size += sprintf ( Buffer+Size, "%c%s%i%c",
      FUNCTION_START, FunctionNames[FN__BOLD], DocParms.Bold, FUNCTION_END ) ;

   Size += sprintf ( Buffer+Size, "%c%s%i%c",
      FUNCTION_START, FunctionNames[FN__ITALIC], DocParms.Italic, FUNCTION_END ) ;

   Size += sprintf ( Buffer+Size, "%c%s%i%c",
      FUNCTION_START, FunctionNames[FN__UNDERSCORE], DocParms.Underscore, FUNCTION_END ) ;

   Size += sprintf ( Buffer+Size, "%c%s%i%c",
      FUNCTION_START, FunctionNames[FN__STRIKEOUT], DocParms.Strikeout, FUNCTION_END ) ;

   Size += sprintf ( Buffer+Size, "%c%s%i%c",
      FUNCTION_START, FunctionNames[FN__OUTLINE], DocParms.Outline, FUNCTION_END ) ;

   Size += sprintf ( Buffer+Size, "%c%s%i%c",
      FUNCTION_START, FunctionNames[FN__CAPS], DocParms.Caps, FUNCTION_END ) ;

   Size += sprintf ( Buffer+Size, "%c%s%i%c",
      FUNCTION_START, FunctionNames[FN__EXPAND], DocParms.Expand, FUNCTION_END ) ;

   if ( DocParms.Justify == JUSTIFY_LEFT )
      Size += sprintf ( Buffer+Size, "%c%s%c", FUNCTION_START, FunctionNames[FN__LEFTJUSTIFY], FUNCTION_END ) ;
   else if ( DocParms.Justify == JUSTIFY_CENTER )
      Size += sprintf ( Buffer+Size, "%c%s%c", FUNCTION_START, FunctionNames[FN__CENTERJUSTIFY], FUNCTION_END ) ;
   else if ( DocParms.Justify == JUSTIFY_RIGHT )
      Size += sprintf ( Buffer+Size, "%c%s%c", FUNCTION_START, FunctionNames[FN__RIGHTJUSTIFY], FUNCTION_END ) ;
   else if ( DocParms.Justify == JUSTIFY_FULL )
      Size += sprintf ( Buffer+Size, "%c%s%c", FUNCTION_START, FunctionNames[FN__FULLJUSTIFY], FUNCTION_END ) ;

   Size += sprintf ( Buffer+Size, "%c%s%i%c",
      FUNCTION_START, FunctionNames[FN__CENTERPAGE], DocParms.CenterPage, FUNCTION_END ) ;

   Size += sprintf ( Buffer+Size, "%c%s%i%c",
      FUNCTION_START, FunctionNames[FN__METRIC], DocParms.Metric, FUNCTION_END ) ;

   Size += sprintf ( Buffer+Size, "%c%s%i%c",
      FUNCTION_START, FunctionNames[FN__LEFTMARGIN], DocParms.LeftMargin, FUNCTION_END ) ;
   Size += sprintf ( Buffer+Size, "%c%s%i%c",
      FUNCTION_START, FunctionNames[FN__RIGHTMARGIN], DocParms.RightMargin, FUNCTION_END ) ;
   Size += sprintf ( Buffer+Size, "%c%s%i%c",
      FUNCTION_START, FunctionNames[FN__BOTTOMMARGIN], DocParms.BottomMargin, FUNCTION_END ) ;
   Size += sprintf ( Buffer+Size, "%c%s%i%c",
      FUNCTION_START, FunctionNames[FN__TOPMARGIN], DocParms.TopMargin, FUNCTION_END ) ;

   Size += sprintf ( Buffer+Size, "%c%s%i%c",
      FUNCTION_START, FunctionNames[FN__TABSPACING], DocParms.TabSpacing, FUNCTION_END ) ;

   Size += sprintf ( Buffer+Size, "%c%s%i%c",
      FUNCTION_START, FunctionNames[FN__INDENT], DocParms.Indent, FUNCTION_END ) ;

   Size += sprintf ( Buffer+Size, "%c%s%i%c",
      FUNCTION_START, FunctionNames[FN__LINESPACING], DocParms.LineSpacing, FUNCTION_END ) ;

   Size += sprintf ( Buffer+Size, "%c%s%s%c",
      FUNCTION_START, FunctionNames[FN__DATEFORMAT], DocParms.DateFormat, FUNCTION_END ) ;

   Size += sprintf ( Buffer+Size, "%c%s%i%c",
      FUNCTION_START, FunctionNames[FN__LINEWIDTH], DocParms.LineWidth, FUNCTION_END ) ;

   Size += sprintf ( Buffer+Size, "%c%s%i%c",
      FUNCTION_START, FunctionNames[FN__LINEJOIN], DocParms.LineJoin, FUNCTION_END ) ;

   Size += sprintf ( Buffer+Size, "%c%s%i%c",
      FUNCTION_START, FunctionNames[FN__LINEEND], DocParms.LineEnd, FUNCTION_END ) ;

   Size += sprintf ( Buffer+Size, "%c%s%i%c",
      FUNCTION_START, FunctionNames[FN__FILLTYPE], DocParms.FillType, FUNCTION_END ) ;

   unsigned char *Start = new unsigned char [0x1000] ;
   unsigned char *End ;

   Size += sprintf ( Buffer+Size, "%c%s", FUNCTION_START, FunctionNames[FN__HEADER] ) ;
   End = ExportText ( Anchor, PS, Start, (unsigned char*)DocParms.Header+1, (unsigned char)DocParms.Header[0] ) ;
   memcpy ( Buffer+Size, Start, End-Start ) ;  Size += End-Start ;
   Size += sprintf ( Buffer+Size, "%c", FUNCTION_END ) ;

   Size += sprintf ( Buffer+Size, "%c%s", FUNCTION_START, FunctionNames[FN__FOOTER] ) ;
   End = ExportText ( Anchor, PS, Start, (unsigned char*)DocParms.Footer+1, (unsigned char)DocParms.Footer[0] ) ;
   memcpy ( Buffer+Size, Start, End-Start ) ;  Size += End-Start ;
   Size += sprintf ( Buffer+Size, "%c", FUNCTION_END ) ;

   delete Start ;

   Size += sprintf ( Buffer+Size, "%c%s", FUNCTION_START, FunctionNames[FN__JOBSETTINGS] ) ;
   char Temp [0x1000] ;
   ULONG Written = WriteBinary ( Temp, DocParms.JobSettings, PDRIVDATA(DocParms.JobSettings)->cb ) ;
   Size += WriteBinary ( Buffer+Size, Temp, Written ) ;
   Size += sprintf ( Buffer+Size, "%c", FUNCTION_END ) ;

   Size += sprintf ( Buffer+Size, "%c", FUNCTION_END ) ;
}
 
/****************************************************************************
 *                                                                          *
 *  Document: Dump                                                          *
 *                                                                          *
 ****************************************************************************/

void Documento::Dump ( PSZ title ) {

   Log ( "Dump of Document: %s", title ) ;
   Log ( "  Colors:      %06X,%06X", Colors[0], Colors[1] ) ;
   Log ( "  Column:      %i",    Column ) ;
   Log ( "  Cursor Box:  BL:%i,%i TL:%i,%i BR:%i,%i TR:%i,%i CAT:%i,%i",
      CursorBox[TXTBOX_BOTTOMLEFT ].x, CursorBox[TXTBOX_BOTTOMLEFT ].y,
      CursorBox[TXTBOX_TOPLEFT    ].x, CursorBox[TXTBOX_TOPLEFT    ].y,
      CursorBox[TXTBOX_BOTTOMRIGHT].x, CursorBox[TXTBOX_BOTTOMRIGHT].y,
      CursorBox[TXTBOX_TOPRIGHT   ].x, CursorBox[TXTBOX_TOPRIGHT   ].y,
      CursorBox[TXTBOX_CONCAT     ].x, CursorBox[TXTBOX_CONCAT     ].y ) ;
   Log ( "  DocParms%s:", fDocParms?" (altered)":"" ) ;  DocParms.Dump ( 4 ) ;
   Log ( "  Filename:    '%s'",  PSZ(*Filename) ) ;
   Log ( "  FileType:    '%s'",  PSZ(*FileType) ) ;
   Log ( "  FullNames:   %s",    FullNames?"TRUE":"FALSE" ) ;
   if ( HandleCount ) {
      Log ( "  Handles:" ) ;
      for ( int i=0; i<HandleCount; i++ ) {
         Log ( "    %s %08p", HandleTypes[i]?"MetaFile":"BitMap", Handles[i] ) ;
      } /* endfor */
   } /* endif */
   Log ( "  InsertMode:  %s",    InsertMode?"TRUE":"FALSE" ) ;
   Log ( "  LevelOffset: %i",    LevelOffset ) ;
   Log ( "  Minimized:   %s",    Minimized?"TRUE":"FALSE" ) ;
   Log ( "  Offset:      %i",    Offset ) ;
   Log ( "  OffsetLast:  %i",    OffsetLast ) ;
   Log ( "  OffsetOrigin:%i",    OffsetOrigin ) ;
   Log ( "  PageNumber:  %i",    PageNumber ) ;
   Log ( "  Position:    %i,%i", Position.x, Position.y ) ;
   Log ( "  PS:" ) ;             PS->Dump ( 4 ) ;
   Log ( "  Rulers:      %08p,%08p", Rulers[0], Rulers[1] ) ;
   Log ( "  SavedLevel:  %i",    SavedLevel ) ;
   Log ( "  ScrollMax:   %ix%i", ScrollMax.x, ScrollMax.y ) ;
   Log ( "  ScrollPos:   %ix%i", ScrollPos.x, ScrollPos.y ) ;
   Log ( "  Size:        %ix%i", Size.cx, Size.cy ) ;
   Log ( "  State:" ) ;          State.Dump ( 4 ) ;
   Log ( "  State0:" ) ;         State0.Dump ( 4 ) ;
   Log ( "  StatusWindow:%08p",  StatusWindow ) ;
   Log ( "  Title:       '%s'",  PSZ(*Title) ) ;
   Log ( "  TitleWindow: %08p",  TitleWindow ) ;
   Log ( "  TopLeft:     %ix%i", TopLeft.x, TopLeft.y ) ;
   Log ( "  Window:      %08p",  Window ) ;

   double dZoom = double ( FIXEDINT ( Zoom ) ) + double ( FIXEDFRAC ( Zoom ) ) / 0x10000 ;
   Log ( "  Zoom:        %.1lf", double ( USHORT ( ( dZoom * 100.0 ) + 0.4 ) ) ) ;

   Log ( "  CurrentPage: " ) ;  CurrentPage.Dump ( 4 ) ;
   Log ( "  WorkingPage: " ) ;  WorkingPage.Dump ( 4 ) ;
   Log ( "  Pages:" ) ;
   for ( int i=0; i<NumberOfPages; i++ ) {
      Log ( "    Page %i:", i ) ;
      Pages[i].Dump ( 6 ) ;
   } /* endfor */
   pText->Dump ( 2 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *  Document: Add Handle                                                    *
 *                                                                          *
 ****************************************************************************/

void Documento::AddHandle ( LONG Handle, int Type ) {

   for ( int i=0; i<HandleCount; i++ )
      if ( Handle == Handles[i] )
         return ;

   Handles[HandleCount] = Handle ;
   HandleTypes[HandleCount++] = Type ;
}
 
/****************************************************************************
 *                                                                          *
 *  Documento: Paint (Receive request for window update)                    *
 *                                                                          *
 ****************************************************************************/

void Documento::Paint ( ) {

   #ifdef DEBUG
      Log ( "Documento::Paint() starting." ) ;
   #endif

   RECTL Rectangle ;

   {  // Block
      SIZEL WindowSize ;
      Query_WindowSize ( WindowSize ) ;
      WorkSpace PS ( "Documento::Paint", DocParms.CodePage, Window, pWindowDevice, int(State0.Metric), Rectangle ) ;
      PS.SetTransform ( Zoom, int(TopLeft.x), int(TopLeft.y-WindowSize.cy) ) ;
      PS.Transform ( CVTC_DEVICE, CVTC_DEFAULTPAGE, Rectangle ) ;
   }  // Endblock (deleting the PS just created)

   PainterObject->InvalidateRectangle ( Rectangle ) ;

   #ifdef DEBUG
      Log ( "Documento::Paint() done." ) ;
   #endif
}
 
/****************************************************************************
 *                                                                          *
 *  Documento: Paint (Render on a Window)                                   *
 *                                                                          *
 *  (Called by Painter Thread)                                              *
 *                                                                          *
 ****************************************************************************/

void Documento::Paint ( WorkSpace *PS, RECTL &ClippingRectangle ) {

   // Lock the current page.
   #ifdef DEBUG_LOCK
      Log ( "Documento::Paint: Locking current page." ) ;
   #endif
   LockCurrentPage ( ) ;
   #ifdef DEBUG_LOCK
      Log ( "Documento::Paint:   Current page locked." ) ;
   #endif

   // Hide the cursor.
   PainterObject->HideCursor ( ) ;

   // Get the window size.
   SIZEL WindowSize ;
   Query_WindowSize ( WindowSize ) ;

   // Set the viewing transform.
   PS->SetTransform ( Zoom, int(TopLeft.x), int(TopLeft.y-WindowSize.cy) ) ;

   // Get the window rectangle in page units.
   RECTL WindowRectangle ;
   WinQueryWindowRect ( Window, &WindowRectangle ) ;
   PS->Transform ( CVTC_DEVICE, CVTC_DEFAULTPAGE, WindowRectangle ) ;

   // Establish clipping.
   RECTL ClipBox = ClippingRectangle ;
   PS->Transform ( CVTC_DEFAULTPAGE, CVTC_DEVICE, ClipBox ) ;
   #ifdef DEBUG_RENDER
      Log ( "Documento::Paint: Setting clipping to %i,%i-%i,%i.",
         ClippingRectangle.xLeft, ClippingRectangle.yBottom, ClippingRectangle.xRight, ClippingRectangle.yTop ) ;
   #endif
   PS->SetClipping ( ClipBox ) ;

   // Clear to background color.
   PS->SetColor ( Colors[0] ) ;
   PS->SetFillType ( PATSYM_SOLID ) ;
   PS->FillBox ( ClippingRectangle ) ;

   // Color in the off-page areas.
   PS->SetFillType ( PATSYM_HALFTONE ) ;
   PS->SetColor ( Colors[1] ) ;
   if ( WindowRectangle.xLeft < 0 ) {
      RECTL Box = { WindowRectangle.xLeft, WindowRectangle.yBottom, 0, WindowRectangle.yTop } ;
      if (   ( Box.xLeft   <= ClippingRectangle.xRight  )
         AND ( Box.xRight  >= ClippingRectangle.xLeft   )
         AND ( Box.yBottom <= ClippingRectangle.yTop    )
         AND ( Box.yTop    >= ClippingRectangle.yBottom ) )
         PS->FillBox ( Box ) ;
   } /* endif */
   if ( WindowRectangle.xRight > State0.FormInfo.cx ) {
      RECTL Box = { State0.FormInfo.cx, WindowRectangle.yBottom,
         WindowRectangle.xRight, WindowRectangle.yTop } ;
      if (   ( Box.xLeft   <= ClippingRectangle.xRight  )
         AND ( Box.xRight  >= ClippingRectangle.xLeft   )
         AND ( Box.yBottom <= ClippingRectangle.yTop    )
         AND ( Box.yTop    >= ClippingRectangle.yBottom ) )
         PS->FillBox ( Box ) ;
   } /* endif */
   if ( WindowRectangle.yBottom < 0 ) {
      RECTL Box = { 0, WindowRectangle.yBottom, State0.FormInfo.cx, 0 } ;
      if (   ( Box.xLeft   <= ClippingRectangle.xRight  )
         AND ( Box.xRight  >= ClippingRectangle.xLeft   )
         AND ( Box.yBottom <= ClippingRectangle.yTop    )
         AND ( Box.yTop    >= ClippingRectangle.yBottom ) )
         PS->FillBox ( Box ) ;
   } /* endif */
   if ( WindowRectangle.yTop > State0.FormInfo.cy ) {
      RECTL Box = { 0, State0.FormInfo.cy, State0.FormInfo.cx, WindowRectangle.yTop } ;
      if (   ( Box.xLeft   <= ClippingRectangle.xRight  )
         AND ( Box.xRight  >= ClippingRectangle.xLeft   )
         AND ( Box.yBottom <= ClippingRectangle.yTop    )
         AND ( Box.yTop    >= ClippingRectangle.yBottom ) )
         PS->FillBox ( Box ) ;
   } /* endif */
   PS->SetFillType ( PATSYM_DEFAULT ) ;

   // Stop clipping.
   PS->ResetClipping ( ) ;

   // If the current page is valid, render it.
   if ( CurrentPageValid )
      Render ( PS, &ClippingRectangle ) ;

   // Reveal the cursor once more.
   PainterObject->ShowCursor ( ) ;

   // Unlock the current page.
   #ifdef DEBUG_LOCK
      Log ( "Documento::Paint: Unlocking current page." ) ;
   #endif
   UnlockCurrentPage ( ) ;
}
 
/****************************************************************************
 *                                                                          *
 *  Documento: Render                                                       *
 *                                                                          *
 *  (Called by Painter or Printer Thread)                                   *
 *                                                                          *
 ****************************************************************************/

static void RenderFlush ( WorkSpace *PS, char *String, int &StringLength, BOOL Selected) {
   if ( StringLength ) {
      #ifdef DEBUG_RENDER
         Log ( "Documento::Render: Render flush '%0.*s'.", StringLength, String ) ;
      #endif
      PS->Comment ( "RenderFlush('%0.*s')", StringLength, String ) ;
      PS->DrawText ( String, StringLength, int(Selected) ) ;
      StringLength = 0 ;
   } /* endif */
}

void Documento::Render ( WorkSpace *DisplayPS, PRECTL ClippingRectangle ) {

   #ifdef DEBUG_RENDER
      Log ( "Documento::Render() Start." ) ;
   #endif

   DisplayPS->Comment ( "Documento::Render() Starting." ) ;

   // Get pointer to the page.
   Borradora *Page = & CurrentPage ;

   // Render all graphics on the page.
   Grafico *pGraphic = Page->FindGraphic ( (int)0 ) ;
   int iGraphic = 0 ;
   while ( pGraphic && ( pGraphic->Offset < Page->Query_Offset()+Page->Query_CurrentLength() ) ) {

      #ifdef DEBUG_RENDER
         Log ( "Documento::Render() Examining graphic %i.", iGraphic ) ;
      #endif

      // If graphic is inline, skip it.
      if ( pGraphic->Position.x == GRAPHIC_NEXT_CHAR ) {
         #ifdef DEBUG_RENDER
            Log ( "Documento::Render() Graphic %i not painted because inline.", iGraphic ) ;
         #endif
         pGraphic = pGraphic->NextGraphic ( ) ;  iGraphic ++ ;
         continue ;
      } /* endif */

      // If graphic not in clipping region, skip it.
      RECTL Box ;
      pGraphic->ComputeBox ( Page->FindWord(pGraphic->Offset), Box, PrinterResolution, TRUE ) ;
      if ( ClippingRectangle ) {
         if ( ( Box.xLeft > ClippingRectangle->xRight ) OR ( Box.xRight < ClippingRectangle->xLeft ) ) {
            pGraphic->NeedsPainting  = FALSE ;
            pGraphic->OldPosition = pGraphic->Position ;
            pGraphic->OldSize = pGraphic->Size ;
            pGraphic->OldCRC = pGraphic->CRC ;
            #ifdef DEBUG_RENDER
               Log ( "Documento::Render() Graphic %i not painted because not in horizontal range.", iGraphic ) ;
            #endif
            pGraphic = pGraphic->NextGraphic ( ) ;  iGraphic ++ ;
            continue ;
         } /* endif */
         if ( ( Box.yBottom > ClippingRectangle->yTop ) OR ( Box.yTop < ClippingRectangle->yBottom ) ) {
            pGraphic->NeedsPainting  = FALSE ;
            pGraphic->OldPosition = pGraphic->Position ;
            pGraphic->OldSize = pGraphic->Size ;
            pGraphic->OldCRC = pGraphic->CRC ;
            #ifdef DEBUG_RENDER
               Log ( "Documento::Render() Graphic %i not painted because not in vertical range.", iGraphic ) ;
            #endif
            pGraphic = pGraphic->NextGraphic ( ) ;  iGraphic ++ ;
            continue ;
         } /* endif */
      } /* endif */

      #ifdef DEBUG_RENDER
         Log ( "Documento::Render() Painting graphic %i.", iGraphic ) ;
      #endif

      // Set state.
      DisplayPS->SetState ( pGraphic->State ) ;

      // Clear bitmap's area, plus its border space.
      DisplayPS->SetColor ( pGraphic->BorderColor ) ;
      DisplayPS->FillBox ( Box ) ;
      DisplayPS->SetColor ( pGraphic->State.Color ) ;

      // Draw the graphic.
      pGraphic->ComputeBox ( Page->FindWord(pGraphic->Offset), Box, PrinterResolution ) ;
      POINTL BoxPosition = { Box.xLeft, Box.yBottom } ;
      SIZEL BoxSize = { Box.xRight-Box.xLeft, Box.yTop-Box.yBottom } ;
      if ( !strnicmp ( pGraphic->Type, "BM", 2 ) ) {
         DisplayPS->DrawBitmap   ( pGraphic->Handle, BoxPosition, BoxSize, pGraphic->Halftone, pGraphic->Reverse ) ;
      } else if ( !strnicmp ( pGraphic->Type, "MF", 2 ) ) {
         DisplayPS->DrawMetafile ( pGraphic->Handle, BoxPosition, BoxSize ) ;
      } else if ( !strnicmp ( pGraphic->Type, "UP", 2 ) ) {
         DisplayPS->DrawUpcCodeA  ( pGraphic->BarCode, BoxPosition, BoxSize ) ;
      } else if ( !strnicmp ( pGraphic->Type, "UE", 2 ) ) {
         DisplayPS->DrawUpcCodeE  ( pGraphic->BarCode, BoxPosition, BoxSize ) ;
      } else if ( !strnicmp ( pGraphic->Type, "39", 2 ) ) {
         DisplayPS->DrawCode39  ( pGraphic->BarCode, BoxPosition, BoxSize ) ;
      } else if ( !strnicmp ( pGraphic->Type, "PN", 2 ) ) {
         DisplayPS->DrawPOSTNET ( pGraphic->BarCode, BoxPosition, BoxSize ) ;
      } else if ( !strnicmp ( pGraphic->Type, "E8", 2 ) ) {
         DisplayPS->DrawEAN8 ( pGraphic->BarCode, BoxPosition, BoxSize ) ;
      } else if ( !strnicmp ( pGraphic->Type, "EA", 2 ) ) {
         DisplayPS->DrawEAN13 ( pGraphic->BarCode, BoxPosition, BoxSize ) ;
      } /* endif */

      // Select the graphic if necessary.
      if ( IsSelected ( pGraphic->Offset ) ) {
         RECTL Rectangle ;
         pGraphic->ComputeBox ( Page->FindWord(pGraphic->Offset), Rectangle, PrinterResolution ) ;
         DisplayPS->InvertBox ( Rectangle ) ;
      } /* endif */

      // Mark the graphic painted.
      pGraphic->NeedsPainting  = FALSE ;
      pGraphic->OldPosition = pGraphic->Position ;
      pGraphic->OldSize = pGraphic->Size ;
      pGraphic->OldCRC = pGraphic->CRC ;

      // Move to the next graphic.
      pGraphic = pGraphic->NextGraphic ( ) ;  iGraphic ++ ;

   } /* endwhile */

   // Establish clipping.
   if ( ClippingRectangle ) {
      #ifdef DEBUG_RENDER
         Log ( "Documento::Render() Setting clipping region to %i,%i-%i,%i.", 
            ClippingRectangle->xLeft, ClippingRectangle->yBottom,
            ClippingRectangle->xRight, ClippingRectangle->yTop ) ;
      #endif
      RECTL ClipBox = *ClippingRectangle ;
      DisplayPS->Transform ( CVTC_DEFAULTPAGE, CVTC_DEVICE, ClipBox ) ;
      DisplayPS->SetClipping ( ClipBox ) ;
   } /* endif */

   // Compute the initial state for the page.
   DisplayPS->SetState ( *Page->Query_State() ) ; Estado EffectiveState ;
   ComputeEffectiveState ( DisplayPS, pText, DisplayPS->Query_DBCS_Vector(), Page->Query_Offset(), *Page->Query_State(), EffectiveState, PageNumber, NumberOfPages, Query_CurrentLevel()?time(0):FileDate ) ;

   // Render the page header.
   DisplayPS->SetState ( EffectiveState.HeaderState ) ;
   EffectiveState.RenderHeader ( DisplayPS, PageNumber, NumberOfPages, Query_CurrentLevel()?time(0):FileDate ) ;

   // Render the page footer.
   DisplayPS->SetState ( EffectiveState.FooterState ) ;
   EffectiveState.RenderFooter ( DisplayPS, PageNumber, NumberOfPages, Query_CurrentLevel()?time(0):FileDate ) ;

   // Over each line . . .
   Linea *pLine = Page->FindLine ( (int) 0 ) ;
   for ( int Line=0; Line<Page->Query_NumberOfLines(); Line++, pLine=pLine->NextLine() ) {

      #ifdef DEBUG_RENDER
         Log ( "Documento::Render() Examining line %i.", Line ) ;
      #endif

      // If this line is outside the clipping region, skip it.
      if ( ClippingRectangle ) {
         RECTL Box = { pLine->State.Margins.xLeft, pLine->Top-pLine->Height,
            pLine->State.Margins.xRight, pLine->Top } ;
         if ( Box.xLeft   > ClippingRectangle->xRight  ) {
            pLine->MarkPainted ( *Page ) ;
            #ifdef DEBUG_RENDER
               Log ( "Documento::Render() Not painting line %i because to right of area.", Line ) ;
            #endif
            continue ;
         } /* endif */
         if ( Box.xRight  < ClippingRectangle->xLeft   ) {
            pLine->MarkPainted ( *Page ) ;
            #ifdef DEBUG_RENDER
               Log ( "Documento::Render() Not painting line %i because to left of area.", Line ) ;
            #endif
            continue ;
         } /* endif */
         if ( Box.yBottom > ClippingRectangle->yTop    ) {
            pLine->MarkPainted ( *Page ) ;
            #ifdef DEBUG_RENDER
               Log ( "Documento::Render() Not painting line %i because above area.", Line ) ;
            #endif
            continue ;
         } /* endif */
         if ( Box.yTop    < ClippingRectangle->yBottom ) {
            pLine->MarkPainted ( *Page ) ;
            #ifdef DEBUG_RENDER
               Log ( "Documento::Render() Not painting line %i because below area.", Line ) ;
            #endif
            continue ;
         } /* endif */
      } /* endif */

      #ifdef DEBUG_RENDER
         Log ( "Documento::Render() Painting line %i.", Line ) ;
      #endif

      // Get the state at the start of the line.
      Estado WorkingState = pLine->State ;
      DisplayPS->SetState ( WorkingState ) ;

      // Reset the current string.
      BOOL Selected = FALSE ;
      char String [MAX_STRING] ;
      int StringLength = 0 ;

      // Over each word in the lines . . .
      Palabra *pWord = Page->FindWord ( (int) pLine->WordNumber ) ;
      for ( int Word=0; Word<pLine->WordCount; Word++, pWord=pWord->NextWord() ) {

         // If this word starts a segment, then flush the current string.
         PUCHAR pByte ;
         pText->QueryBytePtr ( pWord->Offset, pByte ) ;
         if ( pWord->SegmentStart OR pWord->Dormant OR IsTab(*pByte) OR IsSeparator(pByte) OR ( IsSpace(*pByte) AND ( WorkingState.Justify == JUSTIFY_FULL ) ) )
            RenderFlush ( DisplayPS, String, StringLength, Selected ) ;

         // If the word is dormant, skip it.
         if ( pWord->Dormant ) {
            #if defined(DEBUG_NEEDSPAINTING) || defined(DEBUG_RENDER)
               Log ( "Word %i: Resetting NeedsPainting because dormant.", pLine->WordNumber + Word ) ;
            #endif
            pWord->NeedsPainting  = FALSE ;
            pWord->OldPosition = pWord->Position ;
            pWord->OldSize = pWord->Size ;
            pWord->OldCRC = pWord->CRC ;
            pWord->OldMaxDescent = pWord->MaxDescent ;
            continue ;
         } /* endif */

         // If string length zero, set graphic position.
         if ( StringLength == 0 ) {
            #ifdef DEBUG_RENDER
            Log ( "Documento::Render: Moving to %i,%i.", pWord->Position.x, pWord->Position.y ) ;
            #endif
            DisplayPS->Move ( pWord->Position ) ;
         } /* endif */

         // Draw the text, optimized for string output within the single word.
         int BytesLeft = int ( pWord->Length ) ;
         #ifdef DEBUG_RENDER
            Log ( "Documento::Render() Line bytes to render: %i.", BytesLeft ) ;
         #endif
         while ( BytesLeft ) {

            // Abort if BytesLeft goes negative.
            if ( BytesLeft < 0 ) {
               Log ( "Documento::Render: BytesLeft went negative on word %08p.", pWord ) ;
               Dump ( "After Render error" ) ;
               break ;
            } /* endif */

            // Get pointer to raw token data.
            PUCHAR pByte ;
            pText->QueryBytePtr ( pWord->Offset + pWord->Length - BytesLeft, pByte ) ;

            // If selection is changing, output any accumulated text now.
            if ( IsSelected ( pWord->Offset + pWord->Length - BytesLeft ) != Selected ) {
               RenderFlush ( DisplayPS, String, StringLength, Selected ) ;
               Selected = IsSelected ( pWord->Offset + pWord->Length - BytesLeft ) ;
            } /* endif */

            // If the current token generates output text . . .
            if ( IsGlyph ( pByte, PS->Query_DBCS_Vector() ) OR ( IsSpace ( *pByte ) AND ( WorkingState.Justify != JUSTIFY_FULL ) ) ) {

               // Process glyph functions.
               if ( IsGlyphFunction ( pByte ) ) {
                  long LengthIn ;
                  ComputeFunctionSize ( pByte, LengthIn ) ;
                  BytesLeft -= LengthIn ;
                  #ifdef DEBUG_RENDER
                     Log ( "Documento::Render() Glyph function of %i bytes.", LengthIn ) ;
                  #endif
                  char Text [100] ;
                  switch ( *(pByte+1) ) {
                     case FN__DATEFIXED: {
                        FormatDate ( *PULONG(pByte+2), WorkingState.DateFormat, Text, sizeof(Text) ) ;
                        if ( StringLength+strlen(Text)+1 > sizeof(String) ) 
                           RenderFlush ( DisplayPS, String, StringLength, Selected ) ;
                        strcpy ( String+StringLength, Text ) ;
                        StringLength += strlen(Text) ;
                        break; }
                     case FN__DATECURRENT: {
                        FormatDate ( time(0), WorkingState.DateFormat, Text, sizeof(Text) ) ;
                        if ( StringLength+strlen(Text)+1 > sizeof(String) ) 
                           RenderFlush ( DisplayPS, String, StringLength, Selected ) ;
                        strcpy ( String+StringLength, Text ) ;
                        StringLength += strlen(Text) ;
                        break; }
                     case FN__DATECHANGED: {
                        FormatDate ( Query_CurrentLevel()?time(0):FileDate, WorkingState.DateFormat, Text, sizeof(Text) ) ;
                        if ( StringLength+strlen(Text)+1 > sizeof(String) ) 
                           RenderFlush ( DisplayPS, String, StringLength, Selected ) ;
                        strcpy ( String+StringLength, Text ) ;
                        StringLength += strlen(Text) ;
                        break; }
                     case FN__PAGENUMBER: {
                        sprintf ( Text, "%i", PageNumber+1 ) ;
                        if ( StringLength+strlen(Text)+1 > sizeof(String) ) 
                           RenderFlush ( DisplayPS, String, StringLength, Selected ) ;
                        strcpy ( String+StringLength, Text ) ;
                        StringLength += strlen(Text) ;
                        break; }
                     case FN__PAGECOUNT: {
                        sprintf ( Text, "%i", NumberOfPages ) ;
                        if ( StringLength+strlen(Text)+1 > sizeof(String) ) 
                           RenderFlush ( DisplayPS, String, StringLength, Selected ) ;
                        strcpy ( String+StringLength, Text ) ;
                        StringLength += strlen(Text) ;
                        break; }
                     case FN__GLYPH: {
                        sprintf ( Text, "%c", *PUCHAR(pByte+2) ) ;
                        if ( StringLength+strlen(Text)+1 > sizeof(String) ) 
                           RenderFlush ( DisplayPS, String, StringLength, Selected ) ;
                        strcpy ( String+StringLength, Text ) ;
                        StringLength += strlen(Text) ;
                        break; }
                     case FN__SEPARATOR: {
                        DisplayPS->DrawSeparator ( pWord->Position, pWord->Size ) ;
                        break; }
                  } /* endswitch */

               // Process normal characters.
               } else {
                  String[StringLength++] = *pByte ;
                  BytesLeft -- ;
                  if ( DisplayPS->IsDBCSHeader ( *pByte ) ) {
                     #ifdef DEBUG_RENDER
                        Log ( "Documento::Render() Double-byte glyph." ) ;
                     #endif
                     String[StringLength++] = *(pByte+1) ;
                     BytesLeft -- ;
                  } else {
                     #ifdef DEBUG_RENDER
                        Log ( "Documento::Render() Single-byte glyph." ) ;
                     #endif
                  } /* endif */
                  if ( StringLength >= sizeof(String) )
                     RenderFlush ( DisplayPS, String, StringLength, Selected ) ;
               } /* endif */

            // Else if inline graphic . . .
            } else if ( IsGraphic(pByte) ) {
               Grafico *pGraphic = CurrentPage.FindGraphic ( pWord->Offset ) ;
               if ( pGraphic && ( pGraphic->Position.x == GRAPHIC_NEXT_CHAR ) ) {
                  RenderFlush ( DisplayPS, String, StringLength, Selected ) ;
                  if ( !strnicmp ( pGraphic->Type, "BM", 2 ) ) {
                     DisplayPS->DrawBitmap   ( pGraphic->Handle, pWord->Position, pWord->Size, pGraphic->Halftone, pGraphic->Reverse ) ;
                  } else if ( !strnicmp ( pGraphic->Type, "MF", 2 ) ) {
                     DisplayPS->DrawMetafile ( pGraphic->Handle, pWord->Position, pWord->Size ) ;
                  } else if ( !strnicmp ( pGraphic->Type, "UP", 2 ) ) {
                     DisplayPS->DrawUpcCodeA  ( pGraphic->BarCode, pWord->Position, pWord->Size ) ;
                  } else if ( !strnicmp ( pGraphic->Type, "UE", 2 ) ) {
                     DisplayPS->DrawUpcCodeE  ( pGraphic->BarCode, pWord->Position, pWord->Size ) ;
                  } else if ( !strnicmp ( pGraphic->Type, "39", 2 ) ) {
                     DisplayPS->DrawCode39  ( pGraphic->BarCode, pWord->Position, pWord->Size ) ;
                  } else if ( !strnicmp ( pGraphic->Type, "PN", 2 ) ) {
                     DisplayPS->DrawPOSTNET ( pGraphic->BarCode, pWord->Position, pWord->Size ) ;
                  } else if ( !strnicmp ( pGraphic->Type, "E8", 2 ) ) {
                     DisplayPS->DrawEAN8 ( pGraphic->BarCode, pWord->Position, pWord->Size ) ;
                  } else if ( !strnicmp ( pGraphic->Type, "EA", 2 ) ) {
                     DisplayPS->DrawEAN13 ( pGraphic->BarCode, pWord->Position, pWord->Size ) ;
                  } /* endif */
                  long LengthIn ;
                  ComputeFunctionSize ( pByte, LengthIn ) ;
                  #ifdef DEBUG_RENDER
                     Log ( "Documento::Render() In-line graphic, %i bytes long.", LengthIn ) ;
                  #endif
                  BytesLeft -= LengthIn ;
               } else {
                  RenderFlush ( DisplayPS, String, StringLength, Selected ) ;
                  long LengthIn ;
                  ComputeFunctionSize ( pByte, LengthIn ) ;
                  #ifdef DEBUG_RENDER
                     Log ( "Documento::Render() Graphic, %i bytes long.", LengthIn ) ;
                  #endif
                  BytesLeft -= LengthIn ;
                  WorkingState.ProcessFunction ( pByte, DisplayPS, PageNumber, NumberOfPages, Query_CurrentLevel()?time(0):FileDate ) ;
               } /* endif */

            // Else if tab or space fully justified, and selected, invert it.
            } else if ( IsTab(*pByte) OR ( IsSpace(*pByte) AND ( WorkingState.Justify == JUSTIFY_FULL ) ) ) {
               #ifdef DEBUG_RENDER
                  Log ( "Documento::Render() Tab or space fully justified." ) ;
               #endif
               RenderFlush ( DisplayPS, String, StringLength, Selected ) ;
               DisplayPS->DrawSpace ( pWord->Size, Selected ) ;
               BytesLeft -- ;

            // Else if not a glyph and not a function code, just skip it.
            } else if ( *pByte ) {
               #ifdef DEBUG_RENDER
                  Log ( "Documento::Render() Non-glyph, non-function." ) ;
               #endif
               BytesLeft -- ;

            // But if it is a function code, update the state.
            } else {
               RenderFlush ( DisplayPS, String, StringLength, Selected ) ;
               long LengthIn ;
               ComputeFunctionSize ( pByte, LengthIn ) ;
               #ifdef DEBUG_RENDER
                  Log ( "Documento::Render() Non-glyph function code, %i bytes long.", LengthIn ) ;
               #endif
               BytesLeft -= LengthIn ;
               WorkingState.ProcessFunction ( pByte, DisplayPS, PageNumber, NumberOfPages, Query_CurrentLevel()?time(0):FileDate ) ;

            } /* endif */

         } /* endwhile bytes left in word */

         #ifdef DEBUG_RENDER
            Log ( "Documento::Render() Line render complete." ) ;
         #endif

         // Mark the word painted.
         #ifdef DEBUG_NEEDSPAINTING
            Log ( "Word %i: Resetting NeedsPainting because painted.", pLine->WordNumber+Word ) ;
         #endif
         pWord->NeedsPainting  = FALSE ;
         pWord->OldPosition = pWord->Position ;
         pWord->OldSize = pWord->Size ;
         pWord->OldCRC = pWord->CRC ;
         pWord->OldMaxDescent = pWord->MaxDescent ;

      } /* endfor each word on the line */

      RenderFlush ( DisplayPS, String, StringLength, Selected ) ;

   } /* endfor each line on the page */

   // Stop clipping.
   if ( ClippingRectangle ) {
      #ifdef DEBUG_RENDER
         Log ( "Documento::Render() Resetting clipping region." ) ;
      #endif
      DisplayPS->ResetClipping ( ) ;
   } /* endif */

   #ifdef DEBUG_RENDER
      Log ( "Documento::Render() Done." ) ;
   #endif

   DisplayPS->Comment ( "Documento::Render() Done." ) ;
}
 
/****************************************************************************
 *                                                                          *
 *  Documento: Compute State at given offset.  This function may be used    *
 *    for any offset within the document.                                   *
 *                                                                          *
 ****************************************************************************/

void Documento::ComputeState ( long Offset, Estado &State ) {

   // Set to the current page.
   Borradora *Page = & CurrentPage ;

   // If the offset is on this page . . .
   if ( ( Offset >= Page->Query_Offset() ) AND ( Offset < Page->Query_Offset()+Page->Query_Length() ) ) {

      #ifdef DEBUG_CHANGEALL
         Log ( "Documento::ComputeState(Offset=%i) Getting state from current page (%i->%i).", Offset, Page->Query_Offset(), Page->Query_Offset()+Page->Query_Length() ) ;
      #endif // DEBUG_CHANGEALL

      // Scan the lines on the page.
      Linea *pLine = Page->FindLine ( (int) 0 ) ;
      for ( int i=0; i<Page->Query_NumberOfLines(); i++, pLine=pLine->NextLine() ) {

         // If the cursor is on this line . . .
         if ( Offset < pLine->Offset + pLine->Length ) {

            #ifdef DEBUG_CHANGEALL
               Log ( "Documento::ComputeState(Offset=%i) Getting state from line %i (%i->%i).", Offset, i, pLine->Offset, pLine->Offset+pLine->Length ) ;
            #endif // DEBUG_CHANGEALL

            // Compute the state at the start of the word.
            ComputeNextState ( PS, pText, pLine->Offset, Offset-pLine->Offset, pLine->State, State, PageNumber, NumberOfPages, Query_CurrentLevel()?time(0):FileDate ) ;

            break ;

         } /* endif */

      } /* endfor */

   // Else, if the offset is on another page . . .
   } else {

      #ifdef DEBUG_CHANGEALL
         Log ( "Documento::ComputeState(Offset=%i) Getting state the hard way.", Offset ) ;
      #endif // DEBUG_CHANGEALL

      // Scan the pages for the offset.
      Pagina *Page = Pages ;
      for ( int i=0; i<=NumberOfPages; i++, Page++ ) {
         if ( Offset >= Page->Offset + Page->Length )
            continue ;
         break ;
      } /* endfor */

      // Compute the state relative to the state at the beginning of the page found.
      ComputeNextState ( PS, pText, Page->Offset, Offset-Page->Offset, Page->State, State, PageNumber, NumberOfPages, Query_CurrentLevel()?time(0):FileDate ) ;

   } /* endif */
}
 
/****************************************************************************
 *                                                                          *
 *  Helper Functions for ComputeState.                                    . *
 *                                                                          *
 ****************************************************************************/

  /************************************************************************
   * Flush Current String                                                 *
   ************************************************************************/

static int ComputeStateFlush (  // Returns the flushed string's width in page units.
   WorkSpace *PS,               // -> Graphic workspace.
   char *String,                // String to be flushed out.
   int &StringLength,           // String length (will be cleared when done).
   int &MaxDescent,             // Max descent below baseline (result).
   int &MaxAscent               // Max ascent above baseline (result).
) {
   int Width = 0 ;
   if ( StringLength ) {
      #ifdef DEBUG_COMPUTESTATE
         Log ( "ComputeStateFlush: Flushing '%0.*s'", StringLength, String ) ;
      #endif
      MaxDescent = max ( MaxDescent, PS->QueryDescent() ) ;
      MaxAscent = max ( MaxAscent, PS->QueryAscent() ) ;
      Width = PS->MeasureText ( String, StringLength ) ;
      StringLength = 0 ;
      #ifdef DEBUG_COMPUTESTATE
         Log ( "ComputeStateFlush:   Width %i, MaxDescent %i, MaxAscent %i.", Width, MaxDescent, MaxAscent ) ;
      #endif
   } /* endif */
   return ( Width ) ;
}

  /************************************************************************
   * Measure Segment                                                      *
   *                                                                      *
   * A segment, in the context of this function, is a block of text at    *
   * the start of which a GpiMove call must be made.  Therefore, when     *
   * left, center or right justifying, the word marked as segment start   *
   * and any word following a tab qualifies.  For this reason, text with  *
   * tabs should never be passed to this function.  When fully justifying,*
   * every word is a segment, and neither spaces nor tabs should ever be  *
   * passed to this function.                                             *
   *                                                                      *
   ************************************************************************/

static void ComputeStateMeasure ( 
   WorkSpace *PS,               // -> Graphic workspace
   Text *pText,                 // -> Text object
   time_t Date,                 // Date in standard DOS format (seconds since 1970).
   int PageNumber,              // Current page number (numbering from 0).
   int NumberOfPages,           // Current number of pages.
   long FromOffset,             // Segment start offset
   long ToOffset,               // Offset to measure to
   Estado &EffectiveState,      // Effective State at start of full segment (for segment-wide attributes)
   Estado &State,               // Segment start state (will be updated).
   int &Width,                  // Segment width (result).
   int &MaxDescent,             // Max descent below baseline (result).
   int &MaxAscent               // Max ascent above baseline (result).
) {

   #ifdef DEBUG_COMPUTESTATE
      Log ( "ComputeStateMeasure: Started.  From %i to %i.", FromOffset, ToOffset ) ;
   #endif

   // Clear the results.
   Width = MaxDescent = MaxAscent = 0 ;

   // If From > To, abort with a beep.
   if ( FromOffset > ToOffset ) {
      Log ( "ComputeStateMeasure: Aborted because From(%i) > To(%i).", FromOffset, ToOffset ) ;
      return ;
   } /* endif */

   // Declare and clear the working string.
   char String [MAX_STRING] ;
   int StringLength = 0 ;

   // Position to the first byte of the segment.
   PUCHAR pByte ;
   pText->QueryBytePtr ( FromOffset, pByte ) ;

   // Loop over the bytes left to measure.
   long BytesLeft = ToOffset - FromOffset ;
   while ( BytesLeft ) {

      // Abort if BytesLeft goes negative.
      if ( BytesLeft < 0 ) {
         Log ( "Documento::ComputeStateMeasure: BytesLeft went negative!  From %i to %i.", FromOffset, ToOffset ) ;
         pText->Dump ( 0 ) ;
         break ;
      } /* endif */

      if ( IsGlyph ( pByte, PS->Query_DBCS_Vector() ) OR ( IsSpace ( *pByte ) AND ( EffectiveState.Justify != JUSTIFY_FULL ) ) ) {
         if ( IsGlyphFunction ( pByte ) ) {
            #ifdef DEBUG_COMPUTESTATE
               Log ( "ComputeStateMeasure: Item %i was a glyph function.", ToOffset-BytesLeft ) ;
            #endif
            long LengthIn ;
            ComputeFunctionSize ( pByte, LengthIn ) ;
            BytesLeft -= LengthIn ;
            char Text [100] ;
            switch ( *(pByte+1) ) {
               case FN__DATEFIXED: {
                  FormatDate ( *PULONG(pByte+2), State.DateFormat, Text, sizeof(Text) ) ;
                  if ( StringLength+strlen(Text)+1 > sizeof(String) ) 
                     Width += ComputeStateFlush ( PS, String, StringLength, MaxDescent, MaxAscent ) ;
                  strcpy ( String+StringLength, Text ) ;
                  StringLength += strlen(Text) ;
                  break; }
               case FN__DATECURRENT: {
                  FormatDate ( time(0), State.DateFormat, Text, sizeof(Text) ) ;
                  if ( StringLength+strlen(Text)+1 > sizeof(String) ) 
                     Width += ComputeStateFlush ( PS, String, StringLength, MaxDescent, MaxAscent ) ;
                  strcpy ( String+StringLength, Text ) ;
                  StringLength += strlen(Text) ;
                  break; }
               case FN__DATECHANGED: {
                  FormatDate ( Date, State.DateFormat, Text, sizeof(Text) ) ;
                  if ( StringLength+strlen(Text)+1 > sizeof(String) ) 
                     Width += ComputeStateFlush ( PS, String, StringLength, MaxDescent, MaxAscent ) ;
                  strcpy ( String+StringLength, Text ) ;
                  StringLength += strlen(Text) ;
                  break; }
               case FN__PAGENUMBER: {
                  sprintf ( Text, "%i", PageNumber+1 ) ;
                  if ( StringLength+strlen(Text)+1 > sizeof(String) ) 
                     Width += ComputeStateFlush ( PS, String, StringLength, MaxDescent, MaxAscent ) ;
                  strcpy ( String+StringLength, Text ) ;
                  StringLength += strlen(Text) ;
                  break; }
               case FN__PAGECOUNT: {
                  sprintf ( Text, "%i", NumberOfPages ) ;
                  if ( StringLength+strlen(Text)+1 > sizeof(String) ) 
                     Width += ComputeStateFlush ( PS, String, StringLength, MaxDescent, MaxAscent ) ;
                  strcpy ( String+StringLength, Text ) ;
                  StringLength += strlen(Text) ;
                  break; }
               case FN__GLYPH: {
                  sprintf ( Text, "%c", *PUCHAR(pByte+2) ) ;
                  if ( StringLength+strlen(Text)+1 > sizeof(String) ) 
                     Width += ComputeStateFlush ( PS, String, StringLength, MaxDescent, MaxAscent ) ;
                  strcpy ( String+StringLength, Text ) ;
                  StringLength += strlen(Text) ;
                  break; }
               case FN__SEPARATOR: {
                  // Needs no work, as we will never get here.
                  break; }
            } /* endswitch */
         } else {
            #ifdef DEBUG_COMPUTESTATE
               Log ( "ComputeStateMeasure: Item %i is glyph or non-justified space.", ToOffset-BytesLeft ) ;
            #endif
            String[StringLength++] = *pByte ;
            BytesLeft -- ;
            if ( PS->IsDBCSHeader ( *pByte ) ) {
               String[StringLength++] = *(pByte+1) ;
               BytesLeft -- ;
            } /* endif */
            if ( StringLength >= sizeof(String) ) 
               Width += ComputeStateFlush ( PS, String, StringLength, MaxDescent, MaxAscent ) ;
         } /* endif */
      } else if ( *pByte ) {
         #ifdef DEBUG_COMPUTESTATE
            Log ( "ComputeStateMeasure: Item %i was not a glyph, or was a justified space, or a tab.", ToOffset-BytesLeft ) ;
         #endif
         BytesLeft -- ;
      } else {
         #ifdef DEBUG_COMPUTESTATE
            Log ( "ComputeStateMeasure: Item %i was a function code.", ToOffset-BytesLeft ) ;
         #endif
         Width += ComputeStateFlush ( PS, String, StringLength, MaxDescent, MaxAscent ) ;
         long LengthIn ;
         ComputeFunctionSize ( pByte, LengthIn ) ;
         BytesLeft -= LengthIn ;
         State.ProcessFunction ( pByte, PS, PageNumber, NumberOfPages, Date ) ;
      } /* endif */
      pText->QueryBytePtr ( ToOffset-BytesLeft, pByte ) ;
   } /* endwhile */

   // Flush whatever's left in the string buffer.
   Width += ComputeStateFlush ( PS, String, StringLength, MaxDescent, MaxAscent ) ;

   #ifdef DEBUG_COMPUTESTATE
      Log ( "ComputeStateMeasure: Done.  Width %i, MaxDescent %i, MaxAscent %i.", Width, MaxDescent, MaxAscent ) ;
   #endif
}
 
/****************************************************************************
 *                                                                          *
 *  Documento: Compute State, Position and Size at a specified offset       *
 *    within the current page, which must be valid.                         *
 *                                                                          *
 ****************************************************************************/

void Documento::ComputeState ( long Offset, Estado &State, POINTL &Position, SIZEL &Size ) {

   #ifdef DEBUG_COMPUTESTATE
      Log ( "ComputeState(Offset=%i,State,Position,Size): Started.", Offset ) ;
   #endif

   // Get pointer to the current page.
   Borradora *Page = & CurrentPage ;

   // If not on the current page, return.
   if ( Offset < Page->Query_Offset() ) {
      #ifdef DEBUG_COMPUTESTATE
         Log ( "ComputeState(Offset=%i,State,Position,Size): Aborted.  Offset before page.", Offset ) ;
      #endif
      return ;
   } /* endif */
   if ( Offset >= Page->Query_Offset() + Page->Query_Length() ) {
      #ifdef DEBUG_COMPUTESTATE
         Log ( "ComputeState(Offset=%i,State,Position,Size): Aborted.  Offset after page.", Offset ) ;
      #endif
      return ;
   } /* endif */

   // Find the right line.
   #ifdef DEBUG_COMPUTESTATE
      Log ( "ComputeState(Offset=%i,State,Position,Size): Searching for line.", Offset ) ;
   #endif
   Linea *pLine = Page->FindLine ( (int) 0 ) ;
   for ( int i=0; i<Page->Query_NumberOfLines(); i++, pLine=pLine->NextLine() ) 
      if ( Offset < pLine->Offset + pLine->Length ) 
         break ;
   #ifdef DEBUG_COMPUTESTATE
      Log ( "ComputeState(Offset=%i,State,Position,Size): Found line %i of %i.", Offset, i, Page->Query_NumberOfLines() ) ;
   #endif

   // Set segment to line start.
   Palabra *pSegmentStart = Page->FindWord ( (int) pLine->WordNumber ) ;
   Estado SegmentState = pLine->State ;

   // Compute the segment's effective state.
   Estado EffectiveState ;
   ComputeEffectiveState ( PS, pText, PS->Query_DBCS_Vector(), pLine->Offset, pLine->State, EffectiveState, PageNumber, NumberOfPages, Query_CurrentLevel()?time(0):FileDate ) ;

   // Scan the line for the word.  Note the segment start and update the segment state as we go.
   #ifdef DEBUG_COMPUTESTATE
      Log ( "ComputeState(Offset=%i,State,Position,Size): Scanning line for segment start.", Offset ) ;
   #endif
   Palabra *pWord = pSegmentStart ;
   Palabra *pLast = pWord ;
   for ( i=0; i<pLine->WordCount; i++, pWord=pWord->NextWord() ) {
      if ( !pWord ) {
         Log ( "ComputeState(Offset=%i,State,Position,Size): Word %i of %i not available for inspection!",
            Offset, i, pLine->WordCount ) ;
         exit ( 0 ) ;
      } /* endif */
      #ifdef DEBUG_COMPUTESTATE
         Log ( "ComputeState(Offset=%i,State,Position,Size): Inspecting word %i of %i.  SegmentStart:%s, LastWasTab:%s, FullJustify:%s, Offset %i, Length %i.",
            Offset, i, pLine->WordCount, pWord->SegmentStart?"TRUE":"FALSE", 
            pLast->IsTab(pText)?"TRUE":"FALSE", 
            EffectiveState.Justify==JUSTIFY_FULL?"TRUE":"FALSE", 
            pWord->Offset, pWord->Length ) ;
      #endif
      if ( pWord->SegmentStart ) {
         ComputeNextState ( PS, pText, pLine->Offset, pSegmentStart->Offset-pLine->Offset, pLine->State, SegmentState, PageNumber, NumberOfPages, Query_CurrentLevel()?time(0):FileDate ) ;
         pSegmentStart = pWord ;
         ComputeEffectiveState ( PS, pText, PS->Query_DBCS_Vector(), pSegmentStart->Offset, SegmentState, EffectiveState, PageNumber, NumberOfPages, Query_CurrentLevel()?time(0):FileDate ) ;
      } /* endif */
      if ( pWord->SegmentStart ) 
         pSegmentStart = pWord ;
      if ( pLast->Dormant ) 
         pSegmentStart = pWord ;
      if ( pLast->IsTab(pText) OR pWord->IsTab(pText) )
         pSegmentStart = pWord ;
      if ( pLast->IsSeparator(pText) OR pWord->IsSeparator(pText) )
         pSegmentStart = pWord ;
      if ( pLast->IsGraphicInline(pText) OR pWord->IsGraphicInline(pText) )
         pSegmentStart = pWord ;
      if ( EffectiveState.Justify == JUSTIFY_FULL ) 
         pSegmentStart = pWord ;
      if ( Offset < pWord->Offset + pWord->Length )
         break ;
      pLast = pWord ;
   } /* endfor */
   #ifdef DEBUG_COMPUTESTATE
      Log ( "ComputeState(Offset=%i,State,Position,Size): Segment starts at %i.", Offset, pSegmentStart->Offset ) ;
   #endif

   // Recompute the segment's state.
   ComputeNextState ( PS, pText, pLine->Offset, pSegmentStart->Offset-pLine->Offset, pLine->State, SegmentState, PageNumber, NumberOfPages, Query_CurrentLevel()?time(0):FileDate ) ;

   // If Tab, Separator or Space when fully justifying, we have to use the formatter's results.
   if ( pWord->IsTab(pText) OR pWord->IsSeparator(pText) OR ( pWord->IsSpace(pText) AND ( EffectiveState.Justify == JUSTIFY_FULL ) ) ) {
      State = SegmentState ;
      Position = pWord->Position ;
      Size = pWord->Size ;

   // Else if inline graphic, use the formatter's results again.
   } else if ( pWord->IsGraphicInline(pText) ) {
      State = SegmentState ;
      Position = pWord->Position ;
      Size = pWord->Size ;

   // Else, we have to do things the hard way.
   } else {

      // Measure from the start of the segment to the current offset.
      PS->SetState ( SegmentState ) ;
      State = SegmentState ;
      int SegmentWidth, SegmentDescent, SegmentAscent ;
      #ifdef DEBUG_COMPUTESTATE
         Log ( "Measuring from the start of the segment to the current offset." ) ;
      #endif
      ComputeStateMeasure ( PS, pText, Query_CurrentLevel()?time(0):FileDate, PageNumber, NumberOfPages,
         pSegmentStart->Offset, Offset, EffectiveState, State, SegmentWidth, SegmentDescent, SegmentAscent ) ;

      // Measure from the start of the segment to the current offset, plus one token if it is a glyph.
      PS->SetState ( SegmentState ) ;
      Estado FinalState = SegmentState ;
      int FinalWidth, FinalDescent, FinalAscent ;
      #ifdef DEBUG_COMPUTESTATE
         Log ( "Measuring from the start of the segment to the current offset, plus one token if it is a glyph." ) ;
      #endif
      ComputeStateMeasure ( PS, pText, Query_CurrentLevel()?time(0):FileDate, PageNumber, NumberOfPages,
         pSegmentStart->Offset, MoveTokenDown(Offset), EffectiveState, FinalState, FinalWidth, FinalDescent, FinalAscent ) ;

      // Measure the result.
      PS->SetState ( State ) ;
      Position.x = pSegmentStart->Position.x + SegmentWidth ;
      Position.y = pSegmentStart->Position.y ;
      Size.cx = pWord->Dormant ? 0 : FinalWidth - SegmentWidth ;
//    Size.cy = SegmentAscent + SegmentDescent ? SegmentAscent + SegmentDescent : PS->QueryAscent() + PS->QueryDescent() ;
      Size.cy = PS->QueryAscent() + PS->QueryDescent() ;

   } /* endif */

   #ifdef DEBUG_COMPUTESTATE
      Log ( "ComputeState(Offset=%i,State,Position,Size): Done.  Position %i,%i.  Size %i,%i.", 
         Offset, Position.x, Position.y, Size.cx, Size.cy ) ;
   #endif
}
 
/****************************************************************************
 *                                                                          *
 *  Documento: Compute State, Position, Size and CursorBox at the current   *
 *    offset.  The current page must be valid.                              *
 *                                                                          *
 ****************************************************************************/

void Documento::ComputeState ( ) {

   #ifdef DEBUG_COMPUTESTATE
      Log ( "ComputeState(): Started.  Offset %i.", Offset ) ;
   #endif

   // Compute the state, storing the results in the document object's areas.
   ComputeState ( Offset, State, Position, Size ) ;

   // Compute cursor box.
   CursorBox[TXTBOX_BOTTOMLEFT ].x = Position.x ;
   CursorBox[TXTBOX_TOPLEFT    ].x = CursorBox[TXTBOX_BOTTOMLEFT].x ;
   CursorBox[TXTBOX_BOTTOMRIGHT].x = CursorBox[TXTBOX_BOTTOMLEFT].x + Size.cx ;
   CursorBox[TXTBOX_TOPRIGHT   ].x = CursorBox[TXTBOX_BOTTOMRIGHT].x ;
   CursorBox[TXTBOX_CONCAT     ].x = CursorBox[TXTBOX_BOTTOMRIGHT].x ;
   CursorBox[TXTBOX_BOTTOMLEFT ].y = Position.y - PS->QueryDescent() + 1 ;
   CursorBox[TXTBOX_TOPLEFT    ].y = CursorBox[TXTBOX_BOTTOMLEFT].y + Size.cy ;
   CursorBox[TXTBOX_BOTTOMRIGHT].y = CursorBox[TXTBOX_BOTTOMLEFT].y ;
   CursorBox[TXTBOX_TOPRIGHT   ].y = CursorBox[TXTBOX_TOPLEFT].y ;
   CursorBox[TXTBOX_CONCAT     ].y = Position.y ;

   #ifdef DEBUG_COMPUTESTATE
      Log ( "ComputeState(): Done.  CursorBox: %i,%i-%i,%i.", 
         CursorBox[TXTBOX_BOTTOMLEFT].x, CursorBox[TXTBOX_BOTTOMLEFT].y, 
         CursorBox[TXTBOX_TOPRIGHT].x, CursorBox[TXTBOX_TOPRIGHT].y ) ;
   #endif
}
 
/****************************************************************************
 *                                                                          *
 *  Document: Update Cursor                                                 *
 *                                                                          *
 ****************************************************************************/

void Documento::UpdateCursor ( ) {

   #ifdef DEBUG
   Log ( "Documento::UpdateCursor() starting." ) ;
   #endif

   // Get the window size.
   SIZEL WindowSize ;
   Query_WindowSize ( WindowSize ) ;

   // Update the window transformation parameters.
   PS->SetTransform ( Zoom, int(TopLeft.x), int(TopLeft.y-WindowSize.cy) ) ;

   // Determine the cursor's rectangle in device coordinates.
   RECTL Box = {
      CursorBox[TXTBOX_BOTTOMLEFT].x,
      CursorBox[TXTBOX_BOTTOMLEFT].y,
      CursorBox[TXTBOX_TOPRIGHT].x,
      CursorBox[TXTBOX_TOPRIGHT].y
   } ;
   PS->Transform ( CVTC_DEFAULTPAGE, CVTC_DEVICE, Box ) ;

   // Create the cursor, replacing whatever was out there already.
   if ( !WinCreateCursor ( Window, Box.xLeft, Box.yBottom,
      InsertMode ? 0 : Box.xRight-Box.xLeft+1, Box.yTop-Box.yBottom,
      CURSOR_SOLID | CURSOR_FLASH, 0 ) ) {
      char Message [512] ;
      Log ( "Documento::UpdateCursor(): Unable to create cursor.", InterpretWinError ( Anchor, Message ) ) ;
   } /* endif */

   // If the window has the focus, show the cursor.
   if ( Window == WinQueryFocus ( HWND_DESKTOP ) )
      if ( !WinShowCursor ( Window, TRUE ) ) {
         char Message [512] ;
         Log ( "Documento::UpdateCursor(): Unable to show cursor.", InterpretWinError ( Anchor, Message ) ) ;
      } /* endif */

   #ifdef DEBUG
   Log ( "Documento::UpdateCursor() Done." ) ;
   #endif
}
 
/****************************************************************************
 *                                                                          *
 *  Document: Update Mouse Pointer                                          *
 *                                                                          *
 ****************************************************************************/

void Documento::UpdateMouse ( ) {

   #ifdef DEBUG
   Log ( "Documento::UpdateMouse() starting." ) ;
   #endif

   Sys_PostMessage ( Window, WM_UPDATE_MOUSE_POINTER, 0, 0 ) ;

   #ifdef DEBUG
   Log ( "Documento::UpdateMouse() Done." ) ;
   #endif
}
 
/****************************************************************************
 *                                                                          *
 *      Adjust Window's Top-Left position to keep the cursor in view        *
 *                                                                          *
 ****************************************************************************/

BOOL Documento::UpdatePosition ( ) {

   #ifdef DEBUG_UPDATEPOSITION
      Log ( "Documento::UpdatePosition() Starting." ) ;
   #endif

  /**************************************************************************
   * Determine window metrics.                                              *
   **************************************************************************/

   SIZEL WindowSize ;
   Query_WindowSize ( WindowSize ) ;

  /**************************************************************************
   * Compute the cursor box's location and size.                            *
   **************************************************************************/

   POINTL Cursor = Position ;

   PS->SetState ( State ) ;

   Cursor.y -= PS->QueryDescent() ;

   SIZEL CursorSize = Size ;

   if ( InsertMode )
      CursorSize.cx = 0 ;

   #ifdef DEBUG_UPDATEPOSITION
      Log ( "Documento::UpdatePosition() Cursor %i,%i (%ix%i).", Cursor.x, Cursor.y, CursorSize.cx, CursorSize.cy ) ;
   #endif

  /**************************************************************************
   * We haven't had to scroll . . . yet.                                    *
   **************************************************************************/

   BOOL HadToScroll = FALSE ;

  /**************************************************************************
   * If entire page should be visible horizontally, make sure it is.        *
   **************************************************************************/

   while ( ScrollPos.x > ScrollMax.x ) {
      #ifdef DEBUG_UPDATEPOSITION
         Log ( "Documento::UpdatePosition() Scrolling right because scrolled left too far." ) ;
      #endif
      HorizontalScroll ( SB_LINELEFT, 0 ) ;
      HadToScroll = TRUE ;
   } /* endwhile */

  /**************************************************************************
   * If cursor off to the left, scroll left until it's visible.             *
   **************************************************************************/

   while ( ( Cursor.x < ScrollPos.x ) AND ( ScrollPos.x > 0 ) ) {
      #ifdef DEBUG_UPDATEPOSITION
         Log ( "Documento::UpdatePosition() Scrolling left." ) ;
      #endif
      HorizontalScroll ( SB_LINELEFT, 0 ) ;
      HadToScroll = TRUE ;
   } /* endwhile */

  /**************************************************************************
   * If cursor off to the right, scroll right until it's visible.           *
   **************************************************************************/

   while ( ( Cursor.x+CursorSize.cx-1 > ScrollPos.x+WindowSize.cx ) AND ( ScrollPos.x < ScrollMax.x ) ) {
      #ifdef DEBUG_UPDATEPOSITION
         Log ( "Documento::UpdatePosition() Scrolling right." ) ;
      #endif
      HorizontalScroll ( SB_LINERIGHT, 0 ) ;
      HadToScroll = TRUE ;
   } /* endwhile */

  /**************************************************************************
   * If the cursor still not horizontally within the window, do fine adjust.*
   **************************************************************************/

   if ( ( Cursor.x < ScrollPos.x ) OR ( Cursor.x+CursorSize.cx-1 > ScrollPos.x+WindowSize.cx ) ) {
      #ifdef DEBUG_UPDATEPOSITION
         Log ( "Documento::UpdatePosition() Adjusting horizontally." ) ;
      #endif
      HorizontalScroll ( SB_SLIDERPOSITION, SHORT(Cursor.x) ) ;
      HadToScroll = TRUE ;
   } /* endif */

  /**************************************************************************
   * If entire page should be visible vertically, make sure it is.          *
   **************************************************************************/

   while ( ScrollPos.y > ScrollMax.y ) {
      #ifdef DEBUG_UPDATEPOSITION
         Log ( "Documento::UpdatePosition() Scrolling down because scrolled up too far." ) ;
      #endif
      VerticalScroll ( SB_LINEUP, 0 ) ;
      HadToScroll = TRUE ;
   } /* endwhile */

  /**************************************************************************
   * If cursor below the window, scroll down until it's visible.            *
   **************************************************************************/

   while ( ( Cursor.y < ScrollMax.y-ScrollPos.y ) AND ( ScrollPos.y < ScrollMax.y ) ) {
      #ifdef DEBUG_UPDATEPOSITION
         Log ( "Documento::UpdatePosition() Scrolling down." ) ;
      #endif
      VerticalScroll ( SB_LINEDOWN, 0 ) ;
      HadToScroll = TRUE ;
   } /* endwhile */

  /**************************************************************************
   * If cursor above the window, scroll up until it's visible.              *
   **************************************************************************/

   while ( ( Cursor.y+CursorSize.cy-1 > ScrollMax.y-ScrollPos.y+WindowSize.cy ) AND ( ScrollPos.y > 0 ) ) {
      #ifdef DEBUG_UPDATEPOSITION
         Log ( "Documento::UpdatePosition() Scrolling up." ) ;
      #endif
      VerticalScroll ( SB_LINEUP, 0 ) ;
      HadToScroll = TRUE ;
   } /* endwhile */

  /**************************************************************************
   * If the cursor still not vertically within the window, do a fine adjust.*
   **************************************************************************/

   if ( ( Cursor.y < ScrollMax.y-ScrollPos.y ) OR ( Cursor.y+CursorSize.cy-1 > ScrollMax.y-ScrollPos.y+WindowSize.cy ) ) {
      #ifdef DEBUG_UPDATEPOSITION
         Log ( "Documento::UpdatePosition() Adjusting vertically." ) ;
      #endif
      VerticalScroll ( SB_SLIDERPOSITION, SHORT(State0.FormInfo.cy-(Cursor.y+CursorSize.cy)) ) ;
      HadToScroll = TRUE ;
   } /* endif */

  /**************************************************************************
   * Return TRUE if we had to scroll.                                       *
   **************************************************************************/

   #ifdef DEBUG_UPDATEPOSITION
      Log ( "Documento::UpdatePosition() Done.  %s to scroll.", HadToScroll ? "Had" : "Didn't have" ) ;
   #endif

   return ( HadToScroll ) ;
}
 
/****************************************************************************
 *                                                                          *
 *  Document: Update Rulers                                                 *
 *                                                                          *
 ****************************************************************************/

void Documento::UpdateRulers ( ) {

   #ifdef DEBUG
   Log ( "Documento::UpdateRulers() starting." ) ;
   #endif

  /**************************************************************************
   * Determine window metrics.                                              *
   **************************************************************************/

   SIZEL WindowSize ;
   Query_WindowSize ( WindowSize ) ;

  /**************************************************************************
   * Adjust the horizontal ruler.                                           *
   **************************************************************************/

   static HORZRULER_SETPARMS HorzRulerParms ;
   HorzRulerParms.Metric      = State.Metric ;
   HorzRulerParms.fxZoom      = Zoom ;
   HorzRulerParms.PageWidth   = State.FormInfo.cx ;
   HorzRulerParms.TopLeft     = TopLeft.x ;
   HorzRulerParms.LeftMargin  = State.Margins.xLeft ;
   HorzRulerParms.RightMargin = State.Margins.xRight ;
   HorzRulerParms.TabCount    = State.TabCount ;
   memcpy ( HorzRulerParms.Tabs, State.Tabs, sizeof(HorzRulerParms.Tabs) ) ;

   Sys_PostMessage ( Rulers[0], WM_SET_RULER, MPFROMP(&HorzRulerParms), 0 ) ;

  /**************************************************************************
   * Adjust the vertical ruler.                                             *
   **************************************************************************/

   static VERTRULER_SETPARMS VertRulerParms ;
   VertRulerParms.Metric       = State.Metric ;
   VertRulerParms.fxZoom       = Zoom ;
   VertRulerParms.PageHeight   = State.FormInfo.cy ;
   VertRulerParms.WindowHeight = WindowSize.cy ;
   VertRulerParms.TopLeft      = TopLeft.y ;
   VertRulerParms.BottomMargin = State.Margins.yBottom ;
   VertRulerParms.TopMargin    = State.Margins.yTop ;

   Sys_PostMessage ( Rulers[1], WM_SET_RULER, MPFROMP(&VertRulerParms), 0 ) ;

   #ifdef DEBUG
   Log ( "Documento::UpdateRulers() done." ) ;
   #endif
}
 
/****************************************************************************
 *                                                                          *
 *  Documento: Update Scroll Bars                                           *
 *                                                                          *
 ****************************************************************************/

void Documento::UpdateScrollBars ( ) {

   #ifdef DEBUG_UPDATESCROLLBARS
      Log ( "Documento::UpdateScrollBars() Starting." ) ;
   #endif

  /**************************************************************************
   * Determine window metrics.                                              *
   **************************************************************************/

   SIZEL WindowSize ;
   Query_WindowSize ( WindowSize ) ;

  /**************************************************************************
   * Adjust the horizontal scroll bar.                                      *
   **************************************************************************/

   #ifdef DEBUG_UPDATESCROLLBARS
      Log ( "Documento::UpdateScrollBars() Computing horizontal scroll bar parameters.  ScrollMax %i, ScrollPos %i, Formwidth %i, Windowwidth %i.", ScrollMax.x, ScrollPos.x, State0.FormInfo.cx, WindowSize.cx ) ;
   #endif

   ScrollMax.x = max ( 0, SHORT(State0.FormInfo.cx-WindowSize.cx) ) ;
   ScrollPos.x = max ( 0, min ( ScrollPos.x, ScrollMax.x ) ) ;

   #ifdef DEBUG_UPDATESCROLLBARS
      Log ( "Documento::UpdateScrollBars() Setting horizontal scroll bar position.  Range 0-%i, Position %i.", ScrollMax.x, ScrollPos.x ) ;
   #endif

   Sys_PostMessage ( ScrollBars[0], SBM_SETSCROLLBAR, MPFROMSHORT ( ScrollPos.x ), MPFROM2SHORT ( 0, ScrollMax.x ) ) ;

   #ifdef DEBUG_UPDATESCROLLBARS
      Log ( "Documento::UpdateScrollBars() Setting horizontal scroll bar slider size to %i.", WindowSize.cx ) ;
   #endif

   Sys_PostMessage ( ScrollBars[0], SBM_SETTHUMBSIZE, MPFROM2SHORT ( SHORT(WindowSize.cx), SHORT(State0.FormInfo.cx) ), 0L ) ;

   TopLeft.x = ScrollPos.x ;

  /**************************************************************************
   * Adjust the vertical scroll bar.                                        *
   **************************************************************************/

   ScrollMax.y = max ( 0, SHORT(State0.FormInfo.cy-WindowSize.cy) ) ;
   ScrollPos.y = max ( 0, min ( ScrollPos.y, ScrollMax.y ) ) ;

   #ifdef DEBUG_UPDATESCROLLBARS
      Log ( "Documento::UpdateScrollBars() Setting vertical scroll bar position.  Range 0-%i, Position %i.", ScrollMax.y, ScrollPos.y ) ;
   #endif

   Sys_PostMessage ( ScrollBars[1], SBM_SETSCROLLBAR, MPFROMSHORT ( ScrollPos.y ), MPFROM2SHORT ( 0, ScrollMax.y ) ) ;

   #ifdef DEBUG_UPDATESCROLLBARS
      Log ( "Documento::UpdateScrollBars() Setting vertical scroll bar slider size to %i.", WindowSize.cy ) ;
   #endif

   Sys_PostMessage ( ScrollBars[1], SBM_SETTHUMBSIZE, MPFROM2SHORT ( USHORT(WindowSize.cy), USHORT(State0.FormInfo.cy) ), 0 ) ;

   TopLeft.y = State0.FormInfo.cy - ScrollPos.y ;

  /**************************************************************************
   * Done.                                                                  *
   **************************************************************************/

   #ifdef DEBUG_UPDATESCROLLBARS
      Log ( "Documento::UpdateScrollBars() Done." ) ;
   #endif
}
 
/****************************************************************************
 *                                                                          *
 *  Documento: Show Status, Position and Size in status window.             *
 *                                                                          *
 ****************************************************************************/

void Documento::UpdateStatus ( ) {

   // Get the cursor position.
   POINTL TokenPosition = Position ;
   PS->SetState ( State ) ;
   TokenPosition.y = State.FormInfo.cy - Position.y - PS->QueryAscent() ;

   // Get the cursor size.
   SIZEL TokenSize = Size ;
   if ( TokenSize.cy == 0 )
      TokenSize.cy = PS->QueryAscent() + PS->QueryDescent() ;

   // Format the cursor position and size into the status text.
   ResourceString Cursor ( Library->QueryHandle(), IDS_CURSOR ) ;
   static char Text [200] ;
   char String1 [20] ;  FormatDecimal ( String1, TokenPosition.x, 1000, 3 ) ;
   char String2 [20] ;  FormatDecimal ( String2, TokenPosition.y, 1000, 3 ) ;
   char String3 [20] ;  FormatDecimal ( String3, TokenSize.cx,    1000, 3 ) ;
   char String4 [20] ;  FormatDecimal ( String4, TokenSize.cy,    1000, 3 ) ;
   sprintf ( Text, "%s x=%s%sy=%s (%sx%s)   ",
      PCHAR(Cursor), String1, CountryInfo.szDataSeparator, String2, String3, String4 ) ;

   // Determine what the cursor is sitting on.
   PUCHAR pByte ;
   pText->QueryBytePtr ( Offset, pByte ) ;

   if ( IsFunction ( *pByte ) ) {
      switch ( *(pByte+1) ) {
         case FN__VERSION: {
            ResourceString Version ( Library->QueryHandle(), IDS_FN_VERSION ) ;
            char String [20] ;  FormatDecimal ( String, *PLONG(pByte+2), 100, 2 ) ;
            sprintf ( Text+strlen(Text), PCHAR(Version), String ) ;
            break; }
         case FN__PREFACE: {
            ResourceString Preface ( Library->QueryHandle(), IDS_FN_PREFACE ) ;
            strcat ( Text, PCHAR(Preface) ) ;
            break; }
         case FN__PRINTER: {
            ResourceString Printer ( Library->QueryHandle(), IDS_FN_PRINTER ) ;
            sprintf ( Text+strlen(Text), PCHAR(Printer), pByte+2 ) ;
            break; }
         case FN__FORM: {
            ResourceString Form ( Library->QueryHandle(), IDS_FN_FORM ) ;
            sprintf ( Text+strlen(Text), PCHAR(Form), pByte+2 ) ;
            break; }
         case FN__CODEPAGE: {
            ResourceString CodePage ( Library->QueryHandle(), IDS_FN_CODEPAGE ) ;
            sprintf ( Text+strlen(Text), PCHAR(CodePage), pByte+2 ) ;
            break; }
         case FN__FONT: {
            ResourceString Font ( Library->QueryHandle(), IDS_FN_FONT ) ;
            sprintf ( Text+strlen(Text), PCHAR(Font), pByte+2 ) ;
            break; }
         case FN__SIZE: {
            ResourceString Size ( Library->QueryHandle(), IDS_FN_SIZE ) ;
            char String [20] ;  FormatDecimal ( String, *PLONG(pByte+2), 10, 1 ) ;
            sprintf ( Text+strlen(Text), PCHAR(Size), String ) ;
            break; }
         case FN__LINECOLOR: {
            ResourceString Color ( Library->QueryHandle(), IDS_FN_COLOR ) ;
            sprintf ( Text+strlen(Text), PCHAR(Color), *PCOLOR(pByte+2) ) ;
            break; }
         case FN__FILLCOLOR: {
            ResourceString Color ( Library->QueryHandle(), IDS_FN_FILLCOLOR ) ;
            sprintf ( Text+strlen(Text), PCHAR(Color), *PCOLOR(pByte+2) ) ;
            break; }
         case FN__BOLD: {
            ResourceString Bold ( Library->QueryHandle(), IDS_FN_BOLD ) ;
            ResourceString On ( Library->QueryHandle(), IDS_ON ) ;
            ResourceString Off ( Library->QueryHandle(), IDS_OFF ) ;
            sprintf ( Text+strlen(Text), PCHAR(Bold), *(pByte+2) ? PCHAR(On) : PCHAR(Off) ) ;
            break; }
         case FN__ITALIC: {
            ResourceString Italic ( Library->QueryHandle(), IDS_FN_ITALIC ) ;
            ResourceString On ( Library->QueryHandle(), IDS_ON ) ;
            ResourceString Off ( Library->QueryHandle(), IDS_OFF ) ;
            sprintf ( Text+strlen(Text), PCHAR(Italic), *(pByte+2) ? PCHAR(On) : PCHAR(Off) ) ;
            break; }
         case FN__UNDERSCORE: {
            ResourceString Underscore ( Library->QueryHandle(), IDS_FN_UNDERSCORE ) ;
            ResourceString On ( Library->QueryHandle(), IDS_ON ) ;
            ResourceString Off ( Library->QueryHandle(), IDS_OFF ) ;
            sprintf ( Text+strlen(Text), PCHAR(Underscore), *(pByte+2) ? PCHAR(On) : PCHAR(Off) ) ;
            break; }
         case FN__STRIKEOUT: {
            ResourceString Strikeout ( Library->QueryHandle(), IDS_FN_STRIKEOUT ) ;
            ResourceString On ( Library->QueryHandle(), IDS_ON ) ;
            ResourceString Off ( Library->QueryHandle(), IDS_OFF ) ;
            sprintf ( Text+strlen(Text), PCHAR(Strikeout), *(pByte+2) ? PCHAR(On) : PCHAR(Off) ) ;
            break; }
         case FN__OUTLINE: {
            ResourceString Outline ( Library->QueryHandle(), IDS_FN_OUTLINE ) ;
            ResourceString On ( Library->QueryHandle(), IDS_ON ) ;
            ResourceString Off ( Library->QueryHandle(), IDS_OFF ) ;
            sprintf ( Text+strlen(Text), PCHAR(Outline), *(pByte+2) ? PCHAR(On) : PCHAR(Off) ) ;
            break; }
         case FN__CAPS: {
            ResourceString Caps ( Library->QueryHandle(), IDS_FN_CAPS ) ;
            ResourceString On ( Library->QueryHandle(), IDS_ON ) ;
            ResourceString Off ( Library->QueryHandle(), IDS_OFF ) ;
            sprintf ( Text+strlen(Text), PCHAR(Caps), *(pByte+2) ? PCHAR(On) : PCHAR(Off) ) ;
            break; }
         case FN__EXPAND: {
            ResourceString Expand ( Library->QueryHandle(), IDS_FN_EXPAND ) ;
            sprintf ( Text+strlen(Text), PCHAR(Expand), *PULONG(pByte+2) ) ;
            break; }
         case FN__TOPMARGIN: {
            ResourceString TopMargin ( Library->QueryHandle(), IDS_FN_TOPMARGIN ) ;
            char String [20] ;  FormatDecimal ( String, State.FormInfo.cy - *PLONG(pByte+2), 1000, 3 ) ;
            sprintf ( Text+strlen(Text), PCHAR(TopMargin), String ) ;
            break; }
         case FN__BOTTOMMARGIN: {
            ResourceString BottomMargin ( Library->QueryHandle(), IDS_FN_BOTTOMMARGIN ) ;
            char String [20] ;  FormatDecimal ( String, *PLONG(pByte+2), 1000, 3 ) ;
            sprintf ( Text+strlen(Text), PCHAR(BottomMargin), String ) ;
            break; }
         case FN__LEFTMARGIN: {
            ResourceString LeftMargin ( Library->QueryHandle(), IDS_FN_LEFTMARGIN ) ;
            char String [20] ;  FormatDecimal ( String, *PLONG(pByte+2), 1000, 3 ) ;
            sprintf ( Text+strlen(Text), PCHAR(LeftMargin), String ) ;
            break; }
         case FN__RIGHTMARGIN: {
            ResourceString RightMargin ( Library->QueryHandle(), IDS_FN_RIGHTMARGIN ) ;
            char String [20] ;  FormatDecimal ( String, State.FormInfo.cx - *PLONG(pByte+2), 1000, 3 ) ;
            sprintf ( Text+strlen(Text), PCHAR(RightMargin), String ) ;
            break; }
         case FN__LEFTJUSTIFY: {
            ResourceString LeftJustify ( Library->QueryHandle(), IDS_FN_LEFTJUSTIFY ) ;
            strcat ( Text, PCHAR(LeftJustify) ) ;
            break; }
         case FN__CENTERJUSTIFY: {
            ResourceString CenterJustify ( Library->QueryHandle(), IDS_FN_CENTERJUSTIFY ) ;
            strcat ( Text, PCHAR(CenterJustify) ) ;
            break; }
         case FN__RIGHTJUSTIFY: {
            ResourceString RightJustify ( Library->QueryHandle(), IDS_FN_RIGHTJUSTIFY ) ;
            strcat ( Text, PCHAR(RightJustify) ) ;
            break; }
         case FN__FULLJUSTIFY: {
            ResourceString FullJustify ( Library->QueryHandle(), IDS_FN_FULLJUSTIFY ) ;
            strcat ( Text, PCHAR(FullJustify) ) ;
            break; }
         case FN__CENTERPAGE: {
            ResourceString CenterPage ( Library->QueryHandle(), IDS_FN_CENTERPAGE ) ;
            ResourceString On ( Library->QueryHandle(), IDS_ON ) ;
            ResourceString Off ( Library->QueryHandle(), IDS_OFF ) ;
            sprintf ( Text+strlen(Text), PCHAR(CenterPage), *(pByte+2) ? PCHAR(On) : PCHAR(Off) ) ;
            break; }
         case FN__TABSET: {
            ResourceString TabSet ( Library->QueryHandle(), IDS_FN_TABSET ) ;
            char String [20] ;  FormatDecimal ( String, *PLONG(pByte+2), 1000, 3 ) ;
            sprintf ( Text+strlen(Text), PCHAR(TabSet), String ) ;
            break; }
         case FN__TABCLEAR: {
            ResourceString TabClear ( Library->QueryHandle(), IDS_FN_TABCLEAR ) ;
            char String [20] ;  FormatDecimal ( String, *PLONG(pByte+2), 1000, 3 ) ;
            sprintf ( Text+strlen(Text), PCHAR(TabClear), String ) ;
            break; }
         case FN__TABSPACING: {
            ResourceString TabSpacing ( Library->QueryHandle(), IDS_FN_TABSPACING ) ;
            char String [20] ;  FormatDecimal ( String, *PLONG(pByte+2), 1000, 3 ) ;
            sprintf ( Text+strlen(Text), PCHAR(TabSpacing), String ) ;
            break; }
         case FN__INDENT: {
            ResourceString Indent ( Library->QueryHandle(), IDS_FN_INDENT ) ;
            char String [20] ;  FormatDecimal ( String, *PLONG(pByte+2), 1000, 3 ) ;
            sprintf ( Text+strlen(Text), PCHAR(Indent), String ) ;
            break; }
         case FN__METRIC: {
            ResourceString Metric ( Library->QueryHandle(), IDS_FN_METRIC ) ;
            ResourceString On ( Library->QueryHandle(), IDS_ON ) ;
            ResourceString Off ( Library->QueryHandle(), IDS_OFF ) ;
            sprintf ( Text+strlen(Text), PCHAR(Metric), *(pByte+2) ? PCHAR(On) : PCHAR(Off) ) ;
            break; }
         case FN__GRAPHIC: {
            ResourceString Graphic ( Library->QueryHandle(), IDS_FN_GRAPHIC ) ;
            ResourceString Bitmap ( Library->QueryHandle(), IDS_BITMAP ) ;
            ResourceString Metafile ( Library->QueryHandle(), IDS_METAFILE ) ;
            ResourceString UpcCodeA ( Library->QueryHandle(), IDS_UPCA ) ;
            ResourceString UpcCodeE ( Library->QueryHandle(), IDS_UPCE ) ;
            ResourceString Code39 ( Library->QueryHandle(), IDS_CODE39 ) ;
            ResourceString POSTNET ( Library->QueryHandle(), IDS_POSTNET ) ;
            ResourceString EAN8 ( Library->QueryHandle(), IDS_EAN8 ) ;
            ResourceString EAN13 ( Library->QueryHandle(), IDS_EAN13 ) ;
            PUCHAR DataIn = pByte + 2 ;
            POINTL Position = *PPOINTL(DataIn) ;                        DataIn += sizeof(POINTL) ;
            SIZEL Size = *PSIZEL(DataIn) ;                              DataIn += sizeof(SIZEL) ;
            /* BOOL Background = *DataIn ? TRUE : FALSE ;  */           DataIn ++ ;
            /* BOOL NoTextAside = *DataIn ? TRUE : FALSE ; */           DataIn ++ ;
            /* BOOL Halftone = *DataIn ? TRUE : FALSE ;    */           DataIn ++ ;
            /* BOOL Reverse = *DataIn ? TRUE : FALSE ;     */           DataIn ++ ;
            /* int  Aspect = *DataIn ;                     */           DataIn ++ ;
            /* BOOL PrinterOptimize = *DataIn ? TRUE : FALSE ; */       DataIn ++ ;
            PUCHAR Type = PUCHAR(DataIn) ;                              DataIn += 2 ;
            char Posx [32], Posy [32], Sizx [32], Sizy [32] ;  
            switch ( Position.x ) {
               case GRAPHIC_NEXT_CHAR:           strcpy ( Posx, PCHAR(ResourceString(Library->QueryHandle(),IDS_INLINE)) ) ;         break ;
               case GRAPHIC_EDGE_LEFT:           strcpy ( Posx, PCHAR(ResourceString(Library->QueryHandle(),IDS_LEFTEDGE)) ) ;       break ;
               case GRAPHIC_EDGE_RIGHT:          strcpy ( Posx, PCHAR(ResourceString(Library->QueryHandle(),IDS_RIGHTEDGE)) ) ;      break ;
               case GRAPHIC_MARGIN_LEFT:         strcpy ( Posx, PCHAR(ResourceString(Library->QueryHandle(),IDS_LEFTMARGIN)) ) ;     break ;
               case GRAPHIC_MARGIN_RIGHT:        strcpy ( Posx, PCHAR(ResourceString(Library->QueryHandle(),IDS_RIGHTMARGIN)) ) ;    break ;
               case GRAPHIC_CENTERED:            strcpy ( Posx, PCHAR(ResourceString(Library->QueryHandle(),IDS_CENTERED)) ) ;       break ;
               default:                          FormatDecimal ( Posx, Position.x, 1000, 3 ) ;
            } /* endswitch */
            if ( Position.x == GRAPHIC_NEXT_CHAR ) {
               strcpy ( Posy, PCHAR(ResourceString(Library->QueryHandle(),IDS_INLINE)) ) ;
            } else {
               switch ( Position.y ) {
                  case GRAPHIC_EDGE_TOP:            strcpy ( Posy, PCHAR(ResourceString(Library->QueryHandle(),IDS_TOPEDGE)) ) ;        break ;
                  case GRAPHIC_EDGE_BOTTOM:         strcpy ( Posy, PCHAR(ResourceString(Library->QueryHandle(),IDS_BOTTOMEDGE)) ) ;     break ;
                  case GRAPHIC_MARGIN_TOP:          strcpy ( Posy, PCHAR(ResourceString(Library->QueryHandle(),IDS_TOPMARGIN)) ) ;      break ;
                  case GRAPHIC_MARGIN_BOTTOM:       strcpy ( Posy, PCHAR(ResourceString(Library->QueryHandle(),IDS_BOTTOMMARGIN)) ) ;   break ;
                  case GRAPHIC_CENTERED:            strcpy ( Posy, PCHAR(ResourceString(Library->QueryHandle(),IDS_CENTERED)) ) ;       break ;
                  default:                          FormatDecimal ( Posy, State.FormInfo.cy-Position.y-Size.cy, 1000, 3 ) ;
               } /* endswitch */
            } /* endif */
            switch ( Size.cx ) {
               case GRAPHIC_EDGE_TO_EDGE:        strcpy ( Sizx, PCHAR(ResourceString(Library->QueryHandle(),IDS_EDGETOEDGE)) ) ;     break ;
               case GRAPHIC_MARGIN_TO_MARGIN:    strcpy ( Sizx, PCHAR(ResourceString(Library->QueryHandle(),IDS_MARGINTOMARGIN)) ) ; break ;
               default:                          FormatDecimal ( Sizx, Size.cx, 1000, 3 ) ;
            } /* endswitch */
            switch ( Size.cy ) {
               case GRAPHIC_EDGE_TO_EDGE:        strcpy ( Sizy, PCHAR(ResourceString(Library->QueryHandle(),IDS_EDGETOEDGE)) ) ;     break ;
               case GRAPHIC_MARGIN_TO_MARGIN:    strcpy ( Sizy, PCHAR(ResourceString(Library->QueryHandle(),IDS_MARGINTOMARGIN)) ) ; break ;
               default:                          FormatDecimal ( Sizy, Size.cy, 1000, 3 ) ;
            } /* endswitch */
            sprintf ( Text+strlen(Text), PCHAR(Graphic), Posx, CountryInfo.szDataSeparator, Posy, Sizx, Sizy,
               !memcmp(Type,"BM",2)?PCHAR(Bitmap):(
               !memcmp(Type,"MF",2)?PCHAR(Metafile):(
               !memcmp(Type,"UP",2)?PCHAR(UpcCodeA):(
               !memcmp(Type,"UE",2)?PCHAR(UpcCodeE):(
               !memcmp(Type,"39",2)?PCHAR(Code39):(
               !memcmp(Type,"PN",2)?PCHAR(POSTNET):(
               !memcmp(Type,"E8",2)?PCHAR(EAN8):(
               !memcmp(Type,"EA",2)?PCHAR(EAN13):"??" ) ) ) ) ) ) ) ) ;
            break; }
         case FN__DATEFIXED: {
            ResourceString DateFixed ( Library->QueryHandle(), IDS_FN_DATEFIXED ) ;
            char Date [80] ;
            FormatDate ( *PULONG(pByte+2), State.DateFormat, Date, sizeof(Date) ) ;
            sprintf ( Text+strlen(Text), PCHAR(DateFixed), Date ) ;
            break; }
         case FN__DATECURRENT: {
            ResourceString CurrentDate ( Library->QueryHandle(), IDS_FN_DATECURRENT ) ;
            strcat ( Text, PCHAR(CurrentDate) ) ;
            break; }
         case FN__DATECHANGED: {
            ResourceString DateChanged ( Library->QueryHandle(), IDS_FN_DATECHANGED ) ;
            char Date [80] ;
            FormatDate ( Query_CurrentLevel()?time(0):FileDate, State.DateFormat, Date, sizeof(Date) ) ;
            sprintf ( Text+strlen(Text), PCHAR(DateChanged), Date ) ;
            break; }
         case FN__DATEFORMAT: {
            ResourceString DateFormat ( Library->QueryHandle(), IDS_FN_DATEFORMAT ) ;
            sprintf ( Text+strlen(Text), PCHAR(DateFormat), pByte+2 ) ;
            break; }
         case FN__LINESPACING: {
            ResourceString LineSpacing ( Library->QueryHandle(), IDS_FN_LINESPACING ) ;
            sprintf ( Text+strlen(Text), PCHAR(LineSpacing), *PULONG(pByte+2) ) ;
            break; }
         case FN__JOBSETTINGS: {
            ResourceString JobSettings ( Library->QueryHandle(), IDS_FN_JOBSETTINGS ) ;
            strcat ( Text, PCHAR(JobSettings) ) ;
            break; }
         case FN__PAGENUMBER: {
            ResourceString PageNumber ( Library->QueryHandle(), IDS_FN_PAGENUMBER ) ;
            strcat ( Text, PCHAR(PageNumber) ) ;
            break; }
         case FN__PAGECOUNT: {
            ResourceString PageCount ( Library->QueryHandle(), IDS_FN_PAGECOUNT ) ;
            strcat ( Text, PCHAR(PageCount) ) ;
            break; }
         case FN__LINEWIDTH: {
            ResourceString LineWidth ( Library->QueryHandle(), IDS_FN_LINEWIDTH ) ;
            char String [20] ;  FormatDecimal ( String, *PLONG(pByte+2), 1000, 3 ) ;
            sprintf ( Text+strlen(Text), PCHAR(LineWidth), String ) ;
            break; }
         case FN__LINEEND: {
            ResourceString LineEnd ( Library->QueryHandle(), IDS_FN_LINEEND ) ;
            sprintf ( Text+strlen(Text), PCHAR(LineEnd), *PSHORT(pByte+2) ) ;
            break; }
         case FN__LINEJOIN: {
            ResourceString LineJoin ( Library->QueryHandle(), IDS_FN_LINEJOIN ) ;
            sprintf ( Text+strlen(Text), PCHAR(LineJoin), *PSHORT(pByte+2) ) ;
            break; }
         case FN__FILLTYPE: {
            ResourceString FillType ( Library->QueryHandle(), IDS_FN_FILLTYPE ) ;
            sprintf ( Text+strlen(Text), PCHAR(FillType), *PSHORT(pByte+2) ) ;
            break; }
         case FN__GLYPH: {
            ResourceString Glyph ( Library->QueryHandle(), IDS_FN_GLYPH ) ;
            sprintf ( Text+strlen(Text), PCHAR(Glyph), *PUCHAR(pByte+2), *PUCHAR(pByte+2) ) ;
            break; }
         case FN__AUTHOR: {
            ResourceString Author ( Library->QueryHandle(), IDS_FN_AUTHOR ) ;
            sprintf ( Text+strlen(Text), PCHAR(Author), pByte+2 ) ;
            break; }
         case FN__CREATEDATE: {
            ResourceString CreateDate ( Library->QueryHandle(), IDS_FN_CREATEDATE ) ;
            char Date [80] ;
            FormatDate ( *PULONG(pByte+2), State.DateFormat, Date, sizeof(Date) ) ;
            sprintf ( Text+strlen(Text), PCHAR(CreateDate), Date ) ;
            break; }
         case FN__COMMENT: {
            ResourceString Comment ( Library->QueryHandle(), IDS_FN_COMMENT ) ;
            sprintf ( Text+strlen(Text), PCHAR(Comment), pByte+2, strlen(PCHAR(pByte)+2)>40 ? "..." : "" ) ;
            break; }
         case FN__SEPARATOR: {
            ResourceString Separator ( Library->QueryHandle(), IDS_FN_SEPARATOR ) ;
            strcat ( Text, PCHAR(Separator) ) ;
            break; }
         case FN__TITLE: {
            ResourceString Title ( Library->QueryHandle(), IDS_FN_TITLE ) ;
            sprintf ( Text+strlen(Text), PCHAR(Title), pByte+2, strlen(PCHAR(pByte)+2)>40 ? "..." : "" ) ;
            break; }
         case FN__HEADER: {
            ResourceString Header ( Library->QueryHandle(), IDS_FN_HEADER ) ;
            unsigned char External [500] ;
            unsigned char *End = ExportText ( Anchor, PS, External, ((unsigned char*)(pByte+2))+1, *(unsigned char*)(pByte+2) ) ;
            *End = 0 ;
            sprintf ( Text+strlen(Text), PCHAR(Header), External, strlen(PCHAR(External))>40 ? "..." : "" ) ;
            break; }
         case FN__FOOTER: {
            ResourceString Footer ( Library->QueryHandle(), IDS_FN_FOOTER ) ;
            unsigned char External [500] ;
            unsigned char *End = ExportText ( Anchor, PS, External, ((unsigned char*)(pByte+2))+1, *(unsigned char*)(pByte+2) ) ;
            *End = 0 ;
            sprintf ( Text+strlen(Text), PCHAR(Footer), External, strlen(PCHAR(External))>40 ? "..." : "" ) ;
            break; }
      } /* endswitch */

   } else {
      ResourceString Dormant ( Library->QueryHandle(), IDS_DORMANT ) ;
      Palabra *pWord = CurrentPage.FindWord ( (long)Offset ) ;
      if ( IsEndOfDocument ( *pByte ) ) {
         ResourceString DocumentEnd ( Library->QueryHandle(), IDS_ENDDOCUMENT ) ;
         strcat ( Text, PCHAR(DocumentEnd) ) ;
      } else if ( IsEndOfPage ( *pByte ) ) {
         ResourceString PageEnd ( Library->QueryHandle(), IDS_ENDPAGE ) ;
         strcat ( Text, PCHAR(PageEnd) ) ;
      } else if ( IsEndOfLine(pByte) ) {
         ResourceString LineEnd ( Library->QueryHandle(), IDS_ENDLINE ) ;
         strcat ( Text, PCHAR(LineEnd) ) ;
      } else if ( IsTab ( *pByte ) ) {
         ResourceString TabChar ( Library->QueryHandle(), IDS_TAB ) ;
         strcat ( Text, PCHAR(TabChar) ) ;
         if ( pWord->Dormant )
            sprintf ( Text+strlen(Text), " (%s)", PSZ(Dormant) ) ;
      } else if ( IsSpace ( *pByte ) ) {
         ResourceString Space ( Library->QueryHandle(), IDS_SPACE ) ;
         strcat ( Text, PCHAR(Space) ) ;
         if ( pWord->Dormant )
            sprintf ( Text+strlen(Text), " (%s)", PSZ(Dormant) ) ;
      } else if ( IsHyphen ( *pByte ) ) {
         ResourceString Hyphen ( Library->QueryHandle(), IDS_HYPHEN ) ;
         strcat ( Text, PCHAR(Hyphen) ) ;
      } else if ( PS->IsDBCSHeader ( *pByte ) ) {
         ResourceString Character ( Library->QueryHandle(), IDS_CHAR ) ;
         sprintf ( Text+strlen(Text), "%s: '%0.2s'", PSZ(Character), pByte ) ;
      } else {
         ResourceString Character ( Library->QueryHandle(), IDS_CHAR ) ;
         sprintf ( Text+strlen(Text), "%s: '%c'", PSZ(Character), *pByte ) ;
      } /* endif */

   } /* endif */

   // Set the status text.
   static WNDPARAMS WindowParms ;
   memset ( &WindowParms, 0, sizeof(WindowParms) ) ;
   WindowParms.fsStatus = WPM_TEXT ;
   WindowParms.cchText = strlen(Text) ;
   WindowParms.pszText = PSZ(Text) ;
   Sys_PostMessage ( StatusWindow, WM_SETWINDOWPARAMS, MPFROMP(&WindowParms), 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *  Documento: Update Window Title                                          *
 *                                                                          *
 ****************************************************************************/

void Documento::UpdateTitle ( BOOL UpdateNow ) {

  /**************************************************************************
   * First, get the document file name.                                     *
   **************************************************************************/

   char NewTitle [500] ;
   if ( DocParms.Title[0] ) {
      strcpy ( NewTitle, DocParms.Title ) ;
      if ( NOT Minimized ) {
         strcat ( NewTitle, " (" ) ;
         if ( Filename AND PCHAR(*Filename) AND *PCHAR(*Filename) ) {
            if ( NOT FullNames OR Minimized ) {
               char Drive [_MAX_DRIVE] ;
               char Dir [_MAX_DIR] ;
               char Fname [_MAX_FNAME] ;
               char Ext [_MAX_EXT] ;
               _splitpath ( PCHAR(*Filename), Drive, Dir, Fname, Ext ) ;
               strcat ( NewTitle, Fname ) ;
               strcat ( NewTitle, Ext ) ;
            } else {
               strcat ( NewTitle, PCHAR(*Filename) ) ;
            } /* endif */
         } else {
            ResourceString NewDocument ( Library->QueryHandle(), IDS_NEW_DOCUMENT ) ;
            strcat ( NewTitle, PCHAR(NewDocument) ) ;
         } /* endif */
         strcat ( NewTitle, ")" ) ;
      } /* endif */
   } else {
      if ( Filename AND PCHAR(*Filename) AND *PCHAR(*Filename) ) {
         if ( NOT FullNames OR Minimized ) {
            char Drive [_MAX_DRIVE] ;
            char Dir [_MAX_DIR] ;
            char Fname [_MAX_FNAME] ;
            char Ext [_MAX_EXT] ;
            _splitpath ( PCHAR(*Filename), Drive, Dir, Fname, Ext ) ;
            strcpy ( NewTitle, Fname ) ;
            strcat ( NewTitle, Ext ) ;
         } else {
            strcpy ( NewTitle, PCHAR(*Filename) ) ;
         } /* endif */
      } else {
         ResourceString NewDocument ( Library->QueryHandle(), IDS_NEW_DOCUMENT ) ;
         strcpy ( NewTitle, PCHAR(NewDocument) ) ;
      } /* endif */
   } /* endif */

  /**************************************************************************
   * If not minimized, append the file type.                                *
   **************************************************************************/ 

   if ( NOT Minimized ) {
      ResourceString Type ( Library->QueryHandle(), IDS_TYPE ) ;    
      if ( FileType AND PCHAR(*FileType) AND *PCHAR(*FileType) ) {
         sprintf ( NewTitle+strlen(NewTitle), ", %s '%s'", PCHAR(Type), PCHAR(*FileType) ) ;
      } else {
         sprintf ( NewTitle+strlen(NewTitle), ", %s 'Escriba'", PCHAR(Type) ) ;
      } /* endif */
   } /* endif */

  /**************************************************************************
   * Now, if it has changed, mark it modified.                              *
   **************************************************************************/

   if ( IsChanged() ) {
      ResourceString Modified ( Library->QueryHandle(), IDS_MODIFIED ) ;
      sprintf ( NewTitle+strlen(NewTitle), " (%s)", PSZ(Modified) ) ;
   } /* endif */

  /**************************************************************************
   * Append the current page number.                                        *
   **************************************************************************/

   if ( ! Minimized ) {
      ResourceString Page ( Library->QueryHandle(), IDS_PAGE ) ;
      ResourceString Of ( Library->QueryHandle(), IDS_OF ) ;
      sprintf ( NewTitle+strlen(NewTitle), ", %s %i %s %i",
         PSZ(Page), Query_PageNumber()+1,
         PSZ(Of), Query_PageCount() ) ;
   } /* endif */

  /**************************************************************************
   * If the current title is defined, and has changed, replace it.          *
   **************************************************************************/

   if ( Title ) {
      if ( strcmp ( PCHAR(*Title), NewTitle ) ) {
         delete Title ;
         Title = new String ( NewTitle ) ;
         if ( Title ) {
            static WNDPARAMS WindowParms ;
            memset ( &WindowParms, 0, sizeof(WindowParms) ) ;
            WindowParms.fsStatus = WPM_TEXT ;
            WindowParms.cchText = strlen(PCHAR(*Title)) ;
            WindowParms.pszText = PSZ(*Title) ;
            if ( UpdateNow ) {
               Sys_SendMessage ( TitleWindow, WM_SETWINDOWPARAMS, MPFROMP(&WindowParms), 0 ) ;
               Sys_UpdateWindow ( TitleWindow ) ;
            } else {
               Sys_PostMessage ( TitleWindow, WM_SETWINDOWPARAMS, MPFROMP(&WindowParms), 0 ) ;
            } /* endif */
         } /* endif */
      } /* endif */

  /**************************************************************************
   * Else, if it wasn't defined, define it.                                 *
   **************************************************************************/

   } else {
      Title = new String ( NewTitle ) ;
      if ( Title ) {
         static WNDPARAMS WindowParms ;
         memset ( &WindowParms, 0, sizeof(WindowParms) ) ;
         WindowParms.fsStatus = WPM_TEXT ;
         WindowParms.cchText = strlen(PCHAR(*Title)) ;
         WindowParms.pszText = PSZ(*Title) ;
         if ( UpdateNow ) {
            Sys_SendMessage ( TitleWindow, WM_SETWINDOWPARAMS, MPFROMP(&WindowParms), 0 ) ;
            Sys_UpdateWindow ( TitleWindow ) ;
         } else {
            Sys_PostMessage ( TitleWindow, WM_SETWINDOWPARAMS, MPFROMP(&WindowParms), 0 ) ;
         } /* endif */
      } /* endif */

   } /* endif */
}

 
/****************************************************************************
 *                                                                          *
 *  Documento: Process Horizontal Scrolling Request                         *
 *                                                                          *
 ****************************************************************************/

void Documento::HorizontalScroll ( SHORT ScrollType, SHORT ScrollPosition ) {

   #ifdef DEBUG_SCROLLING
      Log ( "Documento::HorizontalScroll() starting." ) ;
   #endif // DEBUG_SCROLLING

  /**************************************************************************
   * Determine window metrics.                                              *
   **************************************************************************/

   RECTL Rectangle ;  SIZEL WindowSize ;
   Query_WindowSize ( Rectangle, WindowSize ) ;

  /**************************************************************************
   * Determine how much to scroll from scroll command.                      *
   **************************************************************************/

   int Increment = 0 ;
   switch ( ScrollType ) {

      case SB_LINELEFT:
         Increment = State0.Metric ? -2000 : -1000 ;
         break ;

      case SB_LINERIGHT:
         Increment = State0.Metric ? +2000 : +1000 ;
         break ;

      case SB_PAGELEFT:
         Increment = int ( -WindowSize.cx ) ;
         break ;

      case SB_PAGERIGHT:
         Increment = int ( +WindowSize.cx ) ;
         break ;

      case SB_SLIDERTRACK:
      case SB_SLIDERPOSITION:
         Increment = int ( ScrollPosition - ScrollPos.x ) ;
         break ;

   } /* endswitch */

  /**************************************************************************
   * Limit scrolling distance to window bounds.                             *
   **************************************************************************/

   Increment = int ( max ( -ScrollPos.x, min ( Increment, ScrollMax.x-ScrollPos.x ) ) ) ;

  /**************************************************************************
   * If any scrolling to do, do it now.  Set scroll bar slider too.         *
   **************************************************************************/

   if ( Increment ) {

      LockCurrentPage ( ) ;
      PainterObject->LockUpdate() ;

      ScrollPos.x += Increment ;
      #ifdef DEBUG_SCROLLING
         Log ( "Documento::HorizontalScroll: X-scroll position updated to %i.", ScrollPos.x ) ;
      #endif // DEBUG_SCROLLING
      TopLeft.x += Increment ;
      PS->SetTransform ( Zoom, int(TopLeft.x), int(TopLeft.y-WindowSize.cy) ) ;

      #ifdef SMOOTH_SCROLLING

         RECTL NewRectangle ;  SIZEL NewWindowSize ;
         Query_WindowSize ( NewRectangle, NewWindowSize ) ;

         #ifdef DEBUG_SCROLLING
            Log ( "Documento::HorizontalScroll: OldRectangle %i,%i-%i,%i, OldSize %ix%i.", 
               Rectangle.xLeft, Rectangle.yBottom, Rectangle.xRight, Rectangle.yTop, WindowSize.cx, WindowSize.cy ) ;
            Log ( "Documento::HorizontalScroll: NewRectangle %i,%i-%i,%i, NewSize %ix%i.", 
               NewRectangle.xLeft, NewRectangle.yBottom, NewRectangle.xRight, NewRectangle.yTop, NewWindowSize.cx, NewWindowSize.cy ) ;
         #endif // DEBUG_SCROLLING
  
         POINTL Points [2] = { { Rectangle.xLeft, Rectangle.yTop }, { NewRectangle.xLeft, NewRectangle.yTop } } ;
         PS->Transform ( CVTC_DEFAULTPAGE, CVTC_DEVICE, 2, Points ) ;
         int ScrollDistance = Points[0].x - Points[1].x ; 
         #ifdef DEBUG_SCROLLING
            Log ( "Documento::HorizontalScroll: Scrolling %i units (%i pixels).", Rectangle.xLeft-NewRectangle.xLeft, ScrollDistance ) ;
         #endif // DEBUG_SCROLLING 

         if ( Increment > 0 ) {
            if ( NewRectangle.xLeft >= Rectangle.xRight ) {
               #ifdef DEBUG_SCROLLING 
                  Log ( "Documento::HorizontalScroll: Invalidating entire window." ) ;
               #endif // DEBUG_SCROLLING
               Sys_InvalidateWindow ( Window ) ;   // ELABORATE: Only invalidate the document area (not the halftone).
            } else {
               #ifdef DEBUG_SCROLLING 
                  Log ( "Documento::HorizontalScroll: Scrolling right." ) ;
               #endif // DEBUG_SCROLLING
               WinScrollWindow ( Window, ScrollDistance, 0, 0, 0, 0, 0, SW_INVALIDATERGN ) ;
            } /* endif */
         } else {
            if ( NewRectangle.xRight <= Rectangle.xLeft ) {
               Sys_InvalidateWindow ( Window ) ;   // ELABORATE: Only invalidate the document area (not the halftone).
            } else {
               #ifdef DEBUG_SCROLLING 
                  Log ( "Documento::HorizontalScroll: Scrolling left." ) ;
               #endif // DEBUG_SCROLLING
               WinScrollWindow ( Window, ScrollDistance, 0, 0, 0, 0, 0, SW_INVALIDATERGN ) ;
            } /* endif */
         } /* endif */

      #else

         Sys_InvalidateWindow ( Window ) ;

      #endif

      PainterObject->UnlockUpdate() ;
      UnlockCurrentPage ( ) ;

      UpdateRulers ( ) ;

      UpdateCursor ( ) ;

      UpdateMouse ( ) ;

      Sys_PostMessage ( ScrollBars[0], SBM_SETPOS, MPFROMSHORT(ScrollPos.x), 0 ) ;

   } /* endif */

   #ifdef DEBUG_SCROLLING
      Log ( "Documento::HorizontalScroll() done." ) ;
   #endif // DEBUG_SCROLLING
}
 
/****************************************************************************
 *                                                                          *
 *  Documento: Process Vertical Scrolling Request                           *
 *                                                                          *
 ****************************************************************************/

void Documento::VerticalScroll ( SHORT ScrollType, SHORT ScrollPosition ) {

   #ifdef DEBUG_SCROLLING
      Log ( "Documento::VerticalScroll() starting." ) ;
   #endif // DEBUG_SCROLLING

  /**************************************************************************
   * Determine window metrics.                                              *
   **************************************************************************/

   RECTL Rectangle ;  SIZEL WindowSize ;
   Query_WindowSize ( Rectangle, WindowSize ) ;

  /**************************************************************************
   * Determine how much to scroll from scroll command.                      *
   **************************************************************************/

   int Increment = 0 ;
   switch ( ScrollType ) {

      case SB_LINEUP:
         Increment = State0.Metric ? -2000 : -1000 ;
         break ;

      case SB_LINEDOWN:
         Increment = State0.Metric ? +2000 : +1000 ;
         break ;

      case SB_PAGEUP:
         Increment = int ( -WindowSize.cy ) ;
         break ;

      case SB_PAGEDOWN:
         Increment = int ( +WindowSize.cy ) ;
         break ;

      case SB_SLIDERTRACK:
      case SB_SLIDERPOSITION:
         Increment = int ( ScrollPosition - ScrollPos.y ) ;
         break ;

   } /* endswitch */

  /**************************************************************************
   * Limit scrolling distance to window bounds.                             *
   **************************************************************************/

   Increment = int ( max ( -ScrollPos.y, min ( Increment, ScrollMax.y-ScrollPos.y ) ) ) ;

  /**************************************************************************
   * If any scrolling to do, do it now.  Set scroll bar slider too.         *
   **************************************************************************/

   if ( Increment ) {

      LockCurrentPage ( ) ;
      PainterObject->LockUpdate() ;

      ScrollPos.y += Increment ;
      #ifdef DEBUG_SCROLLING
         Log ( "Documento::VerticalScroll: Y-scroll position updated to %i.", ScrollPos.y ) ;
      #endif // DEBUG_SCROLLING
      TopLeft.y -= Increment ;
      PS->SetTransform ( Zoom, int(TopLeft.x), int(TopLeft.y-WindowSize.cy) ) ;

      #ifdef SMOOTH_SCROLLING

         RECTL NewRectangle ;  SIZEL NewWindowSize ;
         Query_WindowSize ( NewRectangle, NewWindowSize ) ;
 
         #ifdef DEBUG_SCROLLING
            Log ( "Documento::VerticalScroll: OldRectangle %i,%i-%i,%i, OldSize %ix%i.", 
               Rectangle.xLeft, Rectangle.yBottom, Rectangle.xRight, Rectangle.yTop, WindowSize.cx, WindowSize.cy ) ;
            Log ( "Documento::VerticalScroll: NewRectangle %i,%i-%i,%i, NewSize %ix%i.", 
               NewRectangle.xLeft, NewRectangle.yBottom, NewRectangle.xRight, NewRectangle.yTop, NewWindowSize.cx, NewWindowSize.cy ) ;
         #endif // DEBUG_SCROLLING
   
         POINTL Points [2] = { { Rectangle.xLeft, Rectangle.yTop }, { NewRectangle.xLeft, NewRectangle.yTop } } ;
         PS->Transform ( CVTC_DEFAULTPAGE, CVTC_DEVICE, 2, Points ) ;
         int ScrollDistance = Points[0].y - Points[1].y ; 
         #ifdef DEBUG_SCROLLING
            Log ( "Documento::VerticalScroll: Scrolling %i units (%i pixels).", Rectangle.yTop-NewRectangle.yTop, ScrollDistance ) ;
         #endif // DEBUG_SCROLLING 
      
         if ( Increment > 0 ) {
            if ( NewRectangle.yTop <= Rectangle.yBottom ) {
               #ifdef DEBUG_SCROLLING 
                  Log ( "Documento::VerticalScroll: Invalidating entire window." ) ;
               #endif // DEBUG_SCROLLING
               Sys_InvalidateWindow ( Window ) ;   // ELABORATE: Only invalidate the document area (not the halftone).
            } else {
               #ifdef DEBUG_SCROLLING 
                  Log ( "Documento::VerticalScroll: Scrolling upwards." ) ;
               #endif // DEBUG_SCROLLING
               WinScrollWindow ( Window, 0, ScrollDistance, 0, 0, 0, 0, SW_INVALIDATERGN ) ;
            } /* endif */
         } else {
            if ( NewRectangle.yBottom >= Rectangle.yTop ) {
               Sys_InvalidateWindow ( Window ) ;   // ELABORATE: Only invalidate the document area (not the halftone).
            } else {
               #ifdef DEBUG_SCROLLING 
                  Log ( "Documento::VerticalScroll: Scrolling downwards." ) ;
               #endif // DEBUG_SCROLLING
               WinScrollWindow ( Window, 0, ScrollDistance, 0, 0, 0, 0, SW_INVALIDATERGN ) ;
            } /* endif */
         } /* endif */

      #else

         Sys_InvalidateWindow ( Window ) ;

      #endif

      PainterObject->UnlockUpdate() ;
      UnlockCurrentPage ( ) ;

      UpdateRulers ( ) ;

      UpdateCursor ( ) ;

      UpdateMouse ( ) ;

      Sys_PostMessage ( ScrollBars[1], SBM_SETPOS, MPFROMSHORT(ScrollPos.y), 0 ) ;

   } /* endif */

   #ifdef DEBUG_SCROLLING
      Log ( "Documento::VerticalScroll() done." ) ;
   #endif // DEBUG_SCROLLING
}
 
/****************************************************************************
 *                                                                          *
 *  Documento: Move Up one Token w/o adjusting state.                       *
 *                                                                          *
 ****************************************************************************/

void Documento::MoveTokenUp ( ) {
   Offset = MoveTokenUp ( Offset ) ;
}

long Documento::MoveTokenUp ( long Offset ) {
   return ( pText->PrevToken ( PS, Offset ) ) ;
} /* endmethod */
 
/****************************************************************************
 *                                                                          *
 *  Documento: Move Down one Token w/o adjusting state.                     *
 *                                                                          *
 ****************************************************************************/

void Documento::MoveTokenDown ( ) {
   Offset = MoveTokenDown ( Offset ) ;
}

long Documento::MoveTokenDown ( long Offset ) {
   return ( pText->NextToken ( PS, Offset ) ) ;
} /* endmethod */
 
/****************************************************************************
 *                                                                          *
 *  Documento: Determine offset of token within word, given column.         *
 *                                                                          *
 ****************************************************************************/

static void FindTokenFlush (
   WorkSpace *PS,
   char *String,
   int &StringLength,
   long &Cursor
) {
   if ( StringLength ) {
      #ifdef DEBUG
         Log ( "FindTokenFlush: Flushing '%0.*s'.", StringLength, String ) ;
      #endif
      Cursor += PS->MeasureText ( String, StringLength ) ;
      StringLength = 0 ;
   } /* endif */
}

long Documento::FindToken ( Linea *pLine, Palabra *pWord, long Column ) {

   #ifdef DEBUG
      Log ( "Documento::FindToken(): Starting.  Setting the initial state." ) ;
   #endif

   // Set the initial state.
   Estado WorkingState ;
   ComputeNextState ( PS, pText, pLine->Offset, pWord->Offset-pLine->Offset, pLine->State, WorkingState, PageNumber, NumberOfPages, Query_CurrentLevel()?time(0):FileDate ) ;
   PS->SetState ( WorkingState ) ;

   // Set up to scan the word for the column position.
   #ifdef DEBUG
      Log ( "Documento::FindToken(): Setting up to scan the current word." ) ;
   #endif
   char String [MAX_STRING] ;
   int StringLength = 0 ;
   long Offset = pWord->Offset ;
   long Cursor = pWord->Position.x ;
   PUCHAR pByte ;
   pText->QueryBytePtr ( Offset, pByte ) ;
   int BytesLeft = int ( pWord->Length ) ;

   // While there is more text in the word . . .
   while ( BytesLeft ) {

      // Abort if BytesLeft goes negative.
      if ( BytesLeft < 0 ) {
         Log ( "Documento::FindToken: BytesLeft went negative!  Word %p.", pWord ) ;
         Dump ( "After FindToken error" ) ;
         break ;
      } /* endif */

      #ifdef DEBUG
         Log ( "Documento::FindToken(): Scanning.  %i bytes left.", BytesLeft ) ;
      #endif

      // If text can be measured . . .
      if ( IsGlyph ( pByte, PS->Query_DBCS_Vector() ) OR ( IsSpace ( *pByte ) AND ( WorkingState.Justify != JUSTIFY_FULL ) ) ) {
         if ( IsGlyphFunction ( pByte ) ) {
            long LengthIn ;
            ComputeFunctionSize ( pByte, LengthIn ) ;
            BytesLeft -= LengthIn ;
            char Text [100] ;
            switch ( *(pByte+1) ) {
               case FN__DATEFIXED: {
                  FormatDate ( *PULONG(pByte+2), WorkingState.DateFormat, Text, sizeof(Text) ) ;
                  if ( StringLength+strlen(Text)+1 > sizeof(String) ) 
                     FindTokenFlush ( PS, String, StringLength, Cursor ) ;
                  strcpy ( String+StringLength, Text ) ;
                  StringLength += strlen(Text) ;
                  break; }
               case FN__DATECURRENT: {
                  FormatDate ( time(0), WorkingState.DateFormat, Text, sizeof(Text) ) ;
                  if ( StringLength+strlen(Text)+1 > sizeof(String) ) 
                     FindTokenFlush ( PS, String, StringLength, Cursor ) ;
                  strcpy ( String+StringLength, Text ) ;
                  StringLength += strlen(Text) ;
                  break; }
               case FN__DATECHANGED: {
                  FormatDate ( Query_CurrentLevel()?time(0):FileDate, WorkingState.DateFormat, Text, sizeof(Text) ) ;
                  if ( StringLength+strlen(Text)+1 > sizeof(String) ) 
                     FindTokenFlush ( PS, String, StringLength, Cursor ) ;
                  strcpy ( String+StringLength, Text ) ;
                  StringLength += strlen(Text) ;
                  break; }
               case FN__PAGENUMBER: {
                  sprintf ( Text, "%i", PageNumber+1 ) ;
                  if ( StringLength+strlen(Text)+1 > sizeof(String) ) 
                     FindTokenFlush ( PS, String, StringLength, Cursor ) ;
                  strcpy ( String+StringLength, Text ) ;
                  StringLength += strlen(Text) ;
                  break; }
               case FN__PAGECOUNT: {
                  sprintf ( Text, "%i", NumberOfPages ) ;
                  if ( StringLength+strlen(Text)+1 > sizeof(String) ) 
                     FindTokenFlush ( PS, String, StringLength, Cursor ) ;
                  strcpy ( String+StringLength, Text ) ;
                  StringLength += strlen(Text) ;
                  break; }
               case FN__GLYPH: {
                  sprintf ( Text, "%c", *PUCHAR(pByte+2) ) ;
                  if ( StringLength+strlen(Text)+1 > sizeof(String) ) 
                     FindTokenFlush ( PS, String, StringLength, Cursor ) ;
                  strcpy ( String+StringLength, Text ) ;
                  StringLength += strlen(Text) ;
                  break; }
               case FN__SEPARATOR: {
                  // ******** NEEDS ELABORATION ??? ********
                  break; }
            } /* endswitch */
         } else {
            String[StringLength++] = *pByte ;
            BytesLeft -- ;
            if ( PS->IsDBCSHeader ( *pByte ) ) {
               String[StringLength++] = *(pByte+1) ;
               BytesLeft -- ;
            } /* endif */
            if ( StringLength >= MAX_STRING ) 
               FindTokenFlush ( PS, String, StringLength, Cursor ) ;
         } /* endif */

      // Else if the text cannot be measured . . .
      } else if ( *pByte ) {
         BytesLeft -- ;

      // Else if the function code . . .
      } else {
         FindTokenFlush ( PS, String, StringLength, Cursor ) ;
         long LengthIn ;
         ComputeFunctionSize ( pByte, LengthIn ) ;
         BytesLeft -= LengthIn ;
         WorkingState.ProcessFunction ( pByte, PS, PageNumber, NumberOfPages, Query_CurrentLevel()?time(0):FileDate ) ;
      } /* endif */

      #ifdef DEBUG
         Log ( "Documento::FindToken(): Token scanned.  %i bytes left.", BytesLeft ) ;
      #endif

      // Measure the text we have so far.
      long Width = 0 ;
      if ( StringLength ) {
         Width += PS->MeasureText ( String, StringLength ) ;
      } /* endif */

      // If the column is within this token's cell, break out.
      if ( Column < Cursor+Width )
         break ;

      // Else, move on to the next token.
      Offset = pWord->Offset + pWord->Length - BytesLeft ;
      pText->QueryBytePtr ( Offset, pByte ) ;

   } /* endwhile */

   // If we went past the end of the word, come back to it.
   if ( Offset >= pWord->Offset + pWord->Length )
      Offset = MoveTokenUp ( Offset ) ;

   // Return the offset of the token found.
   #ifdef DEBUG
      Log ( "Documento::FindToken(): Done.  Offset %i.", Offset ) ;
   #endif
   return ( Offset ) ;
}
 
/****************************************************************************
 *                                                                          *
 *  Documento: Move Direct to an offset within the document.                *
 *                                                                          *
 ****************************************************************************/

void Documento::MoveDirect ( long offset ) {

   // Save the new offset, making sure it isn't too large.
   Offset = min ( offset, pText->QueryCurrentSize()-1 ) ;

}
 
/****************************************************************************
 *                                                                          *
 *  Documento: Complete Move Processing                                     *
 *                                                                          *
 ****************************************************************************/

void Documento::FinishMove ( BOOL Selecting ) {

   #if defined(DEBUG_FINISHMOVE) || defined(DEBUG_FINISHMOVE_DETAILS)
      Log ( "Documento::FinishMove() Starting." ) ;
   #endif

   // Compute the cursor state, position and size.
   ComputeState ( ) ;

   // If no window yet, just exit this routine.
   if ( Window == 0 ) {
      #ifdef DEBUG_FINISHMOVE_DETAILS
         Log ( "Documento::FinishMove() done.  No window to update." ) ;
      #endif
      return ;
   } /* endif */

   // Update the scroll bars.
   #ifdef DEBUG_FINISHMOVE_DETAILS
      Log ( "Documento::FinishMove() Updating scroll bars." ) ;
   #endif
   UpdateScrollBars ( ) ;

   // Lock the painter.
   LockCurrentPage ( ) ;
   PainterObject->LockUpdate() ;

   // Reposition the view if necessary.
   #ifdef DEBUG_FINISHMOVE_DETAILS
      Log ( "Documento::FinishMove() Repositioning view." ) ;
   #endif
   if ( UpdatePosition ( ) )
      PainterObject->UpdateWindow ( TRUE, FALSE ) ;

   // Update selection highlight.
   if ( HasSelection() OR Selecting ) {
      #ifdef DEBUG_FINISHMOVE_DETAILS
         Log ( "Documento::FinishMove() Hiding cursor." ) ;
      #endif
      PainterObject->HideCursor ( ) ;
      #ifdef DEBUG_FINISHMOVE_DETAILS
         Log ( "Documento::FinishMove() Updating window." ) ;
      #endif
      PainterObject->UpdateWindow ( FALSE, TRUE ) ;
      #ifdef DEBUG_FINISHMOVE_DETAILS
         Log ( "Documento::FinishMove() Creating PS." ) ;
      #endif
      WorkSpace PS ( "FinishMove", DocParms.CodePage, Window, pWindowDevice, int(State0.Metric) ) ;
      SIZEL WindowSize ;
      Query_WindowSize ( WindowSize ) ;
      PS.SetTransform ( Zoom, int(TopLeft.x), int(TopLeft.y-WindowSize.cy) ) ;
      #ifdef DEBUG_FINISHMOVE_DETAILS
         Log ( "Documento::FinishMove() Updating selection." ) ;
      #endif
      if ( NOT Selecting ) {
         #ifdef DEBUG_FINISHMOVE_DETAILS
            Log ( "Documento::FinishMove() Deselecting text." ) ;
         #endif
         Deselect ( &PS ) ;
      } else {
         #ifdef DEBUG_FINISHMOVE_DETAILS
            Log ( "Documento::FinishMove() Selecting text." ) ;
         #endif
         Select ( &PS ) ;
      } /* endif */
      #ifdef DEBUG_FINISHMOVE_DETAILS
         Log ( "Documento::FinishMove() Showing cursor." ) ;
      #endif
      PainterObject->ShowCursor ( ) ;
   } else {
      #ifdef DEBUG_FINISHMOVE_DETAILS
         Log ( "Documento::FinishMove() Deselecting text." ) ;
      #endif
      Deselect ( ) ;
   } /* endif */

   // Release the painter.
   PainterObject->UnlockUpdate ( ) ;
   UnlockCurrentPage ( ) ;

   // Update the window title.
   #ifdef DEBUG_FINISHMOVE_DETAILS
      Log ( "Documento::FinishMove() Updating title bar." ) ;
   #endif
   UpdateTitle ( ) ;

   // Update the rulers.
   #ifdef DEBUG_FINISHMOVE_DETAILS
      Log ( "Documento::FinishMove() Updating rulers." ) ;
   #endif
   UpdateRulers ( ) ;

   // Update the current status.
   #ifdef DEBUG_FINISHMOVE_DETAILS
      Log ( "Documento::FinishMove() Updating status bar." ) ;
   #endif
   UpdateStatus ( ) ;

   // Reposition the cursor.
   #ifdef DEBUG_FINISHMOVE_DETAILS
      Log ( "Documento::FinishMove() Updating cursor." ) ;
   #endif
   UpdateCursor ( ) ;

   // Update the mouse pointer.
   #ifdef DEBUG_FINISHMOVE_DETAILS
      Log ( "Documento::FinishMove() Updating pointer." ) ;
   #endif
   UpdateMouse ( ) ;

   #if defined(DEBUG_FINISHMOVE) || defined(DEBUG_FINISHMOVE_DETAILS)
      Log ( "Documento::FinishMove() done." ) ;
   #endif
}

void Documento::FinishMove ( ) {

   // Compute the cursor state, position and size.
   ComputeState ( ) ;

   // If no window yet, just exit this routine.
   if ( Window == 0 )
      return ;

   // Update the scroll bars.
   UpdateScrollBars ( ) ;

   // Reposition the view if necessary.
   UpdatePosition ( ) ;

   // Update the window title.
   UpdateTitle ( ) ;

   // Update the rulers.
   UpdateRulers ( ) ;

   // Update the current status.
   UpdateStatus ( ) ;

   // Reposition the cursor.
   UpdateCursor ( ) ;

   // Update the mouse pointer.
   UpdateMouse ( ) ;
}
 
/****************************************************************************
 *                                                                          *
 *  Documento: Move Up (towards the top) within the document.               *
 *                                                                          *
 ****************************************************************************/

BOOL Documento::MoveUp ( BOOL Selecting ) {

   #ifdef DEBUG_MOTION
      Log ( "Documento::MoveUp: Starting." ) ;
   #endif

   // Flush the command queue.
   FlushCommandQueue ( ) ;

   // Lock current page.
   #ifdef DEBUG_LOCK
      Log ( "Documento::MoveUp: Locking current page." ) ;
   #endif
   LockCurrentPage ( ) ;
   #ifdef DEBUG_LOCK
      Log ( "Documento::MoveUp:   Current page locked." ) ;
   #endif

   // Move towards the top of the document.
   BOOL Moved = FALSE ;
   if ( Offset ) {
      Offset = MoveTokenUp ( Offset ) ;
      Moved = TRUE ;
   } /* endif */

   // If we have left the current page for the previous, format that page.
   if ( Offset < CurrentPage.Query_Offset() ) {
      HourGlass Hold ( Window ) ;
      SupervisorObject->Block ( ) ;
      PageNumber -- ;
      if ( !FormatPage ( PageNumber ) )
         CurrentPage = WorkingPage ;
      CurrentPageValid = TRUE ;
      SupervisorObject->Unblock ( ) ;
      if ( Window ) Sys_InvalidateWindow ( Window ) ;
   } /* endif */

   // Finish up move processing.
   FinishMove ( Selecting ) ;

   // Reset cursor's preferred column.
   Column = Position.x ;

   // Unlock current page.
   #ifdef DEBUG_LOCK
      Log ( "Documento::MoveUp: Unlocking current page." ) ;
   #endif
   UnlockCurrentPage ( ) ;

   // Return good status.
   #ifdef DEBUG_MOTION
      Log ( "Documento::MoveUp: Done.  Offset %i.", Offset ) ;
   #endif
   return ( NOT Moved ) ;
}
 
/****************************************************************************
 *                                                                          *
 *  Documento: Move Down (towards the bottom) within the document.          *
 *                                                                          *
 ****************************************************************************/

BOOL Documento::MoveDown ( BOOL Selecting ) {

   #ifdef DEBUG_MOTION
      Log ( "Documento::MoveDown: Starting." ) ;
   #endif

   // Flush the command queue.
   FlushCommandQueue ( ) ;

   // Lock current page.
   #ifdef DEBUG_LOCK
      Log ( "Documento::MoveDown: Locking current page." ) ;
   #endif
   LockCurrentPage ( ) ;
   #ifdef DEBUG_LOCK
      Log ( "Documento::MoveDown:   Current page locked." ) ;
   #endif

   // Move towards the bottom of the document.
   BOOL Moved = FALSE ;
   if ( Offset < pText->QueryCurrentSize()-1 ) {
      Offset = MoveTokenDown ( Offset ) ;
      Moved = TRUE ;
   } /* endif */

   // If we have left the current page for the next, format that page.
   if ( Offset >= CurrentPage.Query_Offset() + CurrentPage.Query_Length() ) {
      HourGlass Hold ( Window ) ;
      SupervisorObject->Block ( ) ;
      PageNumber ++ ;
      if ( !FormatPage ( PageNumber ) )
         CurrentPage = WorkingPage ;
      CurrentPageValid = TRUE ;
      SupervisorObject->Unblock ( ) ;
      if ( Window ) Sys_InvalidateWindow ( Window ) ;
   } /* endif */

   // Finish up move processing.
   FinishMove ( Selecting ) ;

   // Reset cursor's preferred column.
   Column = Position.x ;

   // Unlock current page.
   #ifdef DEBUG_LOCK
      Log ( "Documento::MoveDown: Unlocking current page." ) ;
   #endif
   UnlockCurrentPage ( ) ;

   // Return good status.
   #ifdef DEBUG_MOTION
      Log ( "Documento::MoveDown: Done.  Offset %i.", Offset ) ;
   #endif
   return ( NOT Moved ) ;
}
 
/****************************************************************************
 *                                                                          *
 *  Documento: Move Up one Word (towards the top) within the document.      *
 *                                                                          *
 ****************************************************************************/

BOOL Documento::MoveWordUp ( BOOL Selecting ) {

   #ifdef DEBUG_MOTION
      Log ( "Documento::MoveWordUp: Starting." ) ;
   #endif

   // Flush the command queue.
   FlushCommandQueue ( ) ;

   // Lock current page.
   #ifdef DEBUG_LOCK
      Log ( "Documento::MoveWordUp: Locking current page." ) ;
   #endif
   LockCurrentPage ( ) ;
   #ifdef DEBUG_LOCK
      Log ( "Documento::MoveWordUp:   Current page locked." ) ;
   #endif

   // If we're going to move . . .
   BOOL Moved = FALSE ;
   if ( Offset ) {
      Moved = TRUE ;

      // Discover which word we're at on the page.
      Palabra *pWord = CurrentPage.FindWord ( (int)0 ) ;
      for ( int Word=0; Word<CurrentPage.Query_NumberOfWords(); Word++, pWord=pWord->NextWord() ) 
         if ( ( Offset >= pWord->Offset ) AND ( Offset < pWord->Offset + pWord->Length ) )
            break ;

      // If we're currently in a word, but not at its start, then move back to the word start.
      if ( Offset != pWord->Offset ) {
         Offset = pWord->Offset ;
     
      // Else, we have to search backwards to the previous graphic word (not space, tab, etc).
      } else {
         while ( Offset ) {
            if ( Word == 0 ) {
               HourGlass Hold ( Window ) ;
               SupervisorObject->Block ( ) ;
               PageNumber -- ;
               Offset = CurrentPage.Query_Offset() - 1 ;
               if ( !FormatPage ( PageNumber ) )
                  CurrentPage = WorkingPage ;
               CurrentPageValid = TRUE ;
               SupervisorObject->Unblock ( ) ;
               if ( Window ) Sys_InvalidateWindow ( Window ) ;
               Word = CurrentPage.Query_NumberOfWords() - 1 ;
            } else {
               Word -- ;
            } /* endif */
            pWord = CurrentPage.FindWord ( Word ) ;
            Offset = pWord->Offset ;
            PUCHAR pByte ;
            pText->QueryBytePtr ( Offset, pByte ) ;
            if ( IsHyphen ( *pByte ) ) 
               continue ;
            if ( IsGlyph ( pByte, PS->Query_DBCS_Vector() ) OR IsFunction ( *pByte ) )
               break ;
         } /* endwhile */
      } /* endif */
   } /* endif */

   // Finish up move processing.
   FinishMove ( Selecting ) ;

   // Reset cursor's preferred column.
   Column = Position.x ;

   // Unlock current page.
   #ifdef DEBUG_LOCK
      Log ( "Documento::MoveWordUp: Unlocking current page." ) ;
   #endif
   UnlockCurrentPage ( ) ;

   // Return good status.
   #ifdef DEBUG_MOTION
      Log ( "Documento::MoveWordUp: Done.  Offset %i.", Offset ) ;
   #endif
   return ( NOT Moved ) ;
}
 
/****************************************************************************
 *                                                                          *
 *  Documento: Move Down one Word (towards the top) within the document.    *
 *                                                                          *
 ****************************************************************************/

BOOL Documento::MoveWordDown ( BOOL Selecting ) {

   #ifdef DEBUG_MOTION
      Log ( "Documento::MoveWordDown: Starting." ) ;
   #endif

   // Flush the command queue.
   FlushCommandQueue ( ) ;

    // Lock current page.
   #ifdef DEBUG_LOCK
      Log ( "Documento::MoveWordDown: Locking current page." ) ;
   #endif
   LockCurrentPage ( ) ;
   #ifdef DEBUG_LOCK
      Log ( "Documento::MoveWordDown:   Current page locked." ) ;
   #endif

   // If we're going to move . . .
   BOOL Moved = FALSE ;
   if ( Offset < pText->QueryCurrentSize()-1 ) {
      Moved = TRUE ;

      // Discover which word we're at on the page.
      Palabra *pWord = CurrentPage.FindWord ( (int)0 ) ;
      for ( int Word=0; Word<CurrentPage.Query_NumberOfWords(); Word++, pWord=pWord->NextWord() ) 
         if ( ( Offset >= pWord->Offset ) AND ( Offset < pWord->Offset + pWord->Length ) )
            break ;

      // Search forwards until we find a word that is not a space, tab, etc.
      while ( Offset < pText->QueryCurrentSize()-1 ) {
         if ( Word == CurrentPage.Query_NumberOfWords() - 1 ) {
            HourGlass Hold ( Window ) ;
            SupervisorObject->Block ( ) ;
            PageNumber ++ ;
            Offset = Pages[PageNumber].Offset ;
            if ( !FormatPage ( PageNumber ) )
               CurrentPage = WorkingPage ;
            CurrentPageValid = TRUE ;
            SupervisorObject->Unblock ( ) ;
            if ( Window ) Sys_InvalidateWindow ( Window ) ;
            Word = 0 ;
         } else {
            Word ++ ;
         } /* endif */
         pWord = CurrentPage.FindWord ( Word ) ;
         Offset = pWord->Offset ;
         PUCHAR pByte ;
         pText->QueryBytePtr ( Offset, pByte ) ;
         if ( IsHyphen ( *pByte ) ) 
            continue ;
         if ( IsGlyph ( pByte, PS->Query_DBCS_Vector() ) OR IsFunction ( *pByte ) )
            break ;
      } /* endwhile */
   } /* endif */

   // Finish up move processing.
   FinishMove ( Selecting ) ;

   // Reset cursor's preferred column.
   Column = Position.x ;

   // Unlock current page.
   #ifdef DEBUG_LOCK
      Log ( "Documento::MoveWordDown: Unlocking current page." ) ;
   #endif
   UnlockCurrentPage ( ) ;

   // Return good status.
   #ifdef DEBUG_MOTION
      Log ( "Documento::MoveWordDown: Done.  Offset %i.", Offset ) ;
   #endif
   return ( NOT Moved ) ;
}
 
/****************************************************************************
 *                                                                          *
 *  Documento: Move Up one Line (towards the top) within the document.      *
 *                                                                          *
 ****************************************************************************/

BOOL Documento::MoveLineUp ( BOOL Selecting ) {

   #ifdef DEBUG_MOTION
      Log ( "Documento::MoveLineUp: Starting." ) ;
   #endif

   // Flush the command queue.
   FlushCommandQueue ( ) ;

   // Lock current page.
   #ifdef DEBUG_LOCK
      Log ( "Documento::MoveLineUp: Locking current page." ) ;
   #endif
   LockCurrentPage ( ) ;
   #ifdef DEBUG_LOCK
      Log ( "Documento::MoveLineUp:   Current page locked." ) ;
   #endif

   // If we're going to move . . .
   BOOL Moved = FALSE ;
   if ( Offset ) {
      Moved = TRUE ;

      // Find out what line the cursor is on.
      int Line ;
      Linea *pLine = CurrentPage.FindLine ( Offset, Line ) ;
    
      // Back up one line.
      if ( Line == 0 ) {
         if ( PageNumber ) {
            HourGlass Hold ( Window ) ;
            SupervisorObject->Block ( ) ;
            PageNumber -- ;
            Offset = CurrentPage.Query_Offset() - 1 ;
            if ( !FormatPage ( PageNumber ) )
               CurrentPage = WorkingPage ;
            CurrentPageValid = TRUE ;
            SupervisorObject->Unblock ( ) ;
            if ( Window ) Sys_InvalidateWindow ( Window ) ;
            Line = CurrentPage.Query_NumberOfLines() - 1 ;
         } else {
            #ifdef DEBUG_LOCK
               Log ( "Documento::MoveLineUp: Unlocking current page." ) ;
            #endif
            FinishMove ( Selecting ) ;
            UnlockCurrentPage ( ) ;
            return ( TRUE ) ;
         } /* endif */
      } else {
         Line -- ;
      } /* endif */
      pLine = CurrentPage.FindLine ( (int) Line ) ;

      // Determine our location within the line.
      Palabra *pWord = CurrentPage.FindWord ( Line, Column ) ;

      // Determine our location within the word.
      Offset = FindToken ( pLine, pWord, Column ) ;

   } /* endif */

   // Finish up move processing.
   FinishMove ( Selecting ) ;

   // Unlock current page.
   #ifdef DEBUG_LOCK
      Log ( "Documento::MoveLineUp: Unlocking current page." ) ;
   #endif
   UnlockCurrentPage ( ) ;

   // Return good status.
   #ifdef DEBUG_MOTION
      Log ( "Documento::MoveLineUp: Done.  Offset %i.", Offset ) ;
   #endif
   return ( NOT Moved ) ;
}
 
/****************************************************************************
 *                                                                          *
 *  Documento: Move Down one Line (towards the bottom) within the document. *
 *                                                                          *
 ****************************************************************************/

BOOL Documento::MoveLineDown ( BOOL Selecting ) {

   #ifdef DEBUG_MOTION
      Log ( "Documento::MoveLineDown: Starting." ) ;
   #endif

   // Flush the command queue.
   FlushCommandQueue ( ) ;

   // Lock current page.
   #ifdef DEBUG_LOCK
      Log ( "Documento::MoveLineDown: Locking current page." ) ;
   #endif
   LockCurrentPage ( ) ;
   #ifdef DEBUG_LOCK
      Log ( "Documento::MoveLineDown:   Current page locked." ) ;
   #endif

   // If we're going to move . . .
   BOOL Moved = FALSE ;
   if ( Offset < pText->QueryCurrentSize()-1 ) {
      Moved = TRUE ;

      // Find out what line the cursor is on.
      int Line ;
      Linea *pLine = CurrentPage.FindLine ( Offset, Line ) ;

      // Move down one line.
      if ( Line == CurrentPage.Query_NumberOfLines() - 1 ) {
         if ( PageNumber < NumberOfPages-1 ) {
            HourGlass Hold ( Window ) ;
            SupervisorObject->Block ( ) ;
            PageNumber ++ ;
            Offset = Pages[PageNumber].Offset ;
            if ( !FormatPage ( PageNumber ) )
               CurrentPage = WorkingPage ;
            CurrentPageValid = TRUE ;
            SupervisorObject->Unblock ( ) ;
            if ( Window ) Sys_InvalidateWindow ( Window ) ;
            Line = 0 ;
         } else {
            #ifdef DEBUG_LOCK
               Log ( "Documento::MoveLineDown: Unlocking current page." ) ;
            #endif
            FinishMove ( Selecting ) ;
            UnlockCurrentPage ( ) ;
            return ( TRUE ) ;
         } /* endif */
      } else {
         Line ++ ;
      } /* endif */
      pLine = CurrentPage.FindLine ( (int)Line ) ;

      // Determine our location within the line.
      Palabra *pWord = CurrentPage.FindWord ( Line, Column ) ;
       
      // Determine our location within the word.
      Offset = FindToken ( pLine, pWord, Column ) ;

   } /* endif */

   // Finish up move processing.
   FinishMove ( Selecting ) ;

   // Unlock current page.
   #ifdef DEBUG_LOCK
      Log ( "Documento::MoveLineDown: Unlocking current page." ) ;
   #endif
   UnlockCurrentPage ( ) ;

   // Return good status.
   #ifdef DEBUG_MOTION
      Log ( "Documento::MoveLineDown: Down." ) ;
   #endif
   return ( NOT Moved) ;
}
 
/****************************************************************************
 *                                                                          *
 *  Documento: Move Up one Page (towards the top) within the document.      *
 *                                                                          *
 ****************************************************************************/

BOOL Documento::MovePageUp ( BOOL Selecting ) {

   #ifdef DEBUG_MOTION
      Log ( "Documento::MovePageUp: Starting." ) ;
   #endif

   // Flush the command queue.
   FlushCommandQueue ( ) ;

   // If not displaying entire page . . .
   if ( ScrollMax.y ) {
      if ( Offset < CurrentPage.FindLine((int)0)->Length )
         return ( MoveLineUp ( Selecting ) ) ;
      SIZEL WindowSize ;
      Query_WindowSize ( WindowSize ) ;
      POINTL Point = { Position.x, Position.y + WindowSize.cy } ;
      Offset = FindXY ( Point ) ;
      if ( ScrollPos.y == 0 ) {
         return ( MoveLineUp ( Selecting ) ) ;
      } else {
         VerticalScroll ( SB_PAGEUP, 0 ) ;
         FinishMove ( Selecting ) ;
         #ifdef DEBUG_MOTION
            Log ( "Documento::MovePageUp: Done.  Scrolled page upwards." ) ;
         #endif
         return ( FALSE ) ;
      } /* endif */
   } /* endif */

   // Lock current page.
   #ifdef DEBUG_LOCK
      Log ( "Documento::MovePageUp: Locking current page." ) ;
   #endif
   LockCurrentPage ( ) ;
   #ifdef DEBUG_LOCK
      Log ( "Documento::MovePageUp:   Current page locked." ) ;
   #endif

   // If we're going to move . . .
   BOOL Moved = FALSE ;
   if ( Offset ) {
      Moved = TRUE ;

      // Back up one page.
      if ( PageNumber ) {
         HourGlass Hold ( Window ) ;
         SupervisorObject->Block ( ) ;
         PageNumber -- ;
         Offset = CurrentPage.Query_Offset() - 1 ;
         if ( !FormatPage ( PageNumber ) )
            CurrentPage = WorkingPage ;
         CurrentPageValid = TRUE ;
         SupervisorObject->Unblock ( ) ;
         if ( Window ) Sys_InvalidateWindow ( Window ) ;
      } else {
         #ifdef DEBUG_LOCK
            Log ( "Documento::MovePageUp: Unlocking current page." ) ;
         #endif
         FinishMove ( Selecting ) ;
         UnlockCurrentPage ( ) ;
         #ifdef DEBUG_MOTION
            Log ( "Documento::MovePageUp: Aborted.  Already on page zero!" ) ;
         #endif
         return ( TRUE ) ;
      } /* endif */

      // Find out which line we're on.
      int Line ;
      Linea *pLine = CurrentPage.FindLine ( Position, Line ) ;

      // Determine our location within the line.
      Palabra *pWord = CurrentPage.FindWord ( Line, Column ) ;

      // Determine our location within the word.
      Offset = FindToken ( pLine, pWord, Column ) ;

   } /* endif */

   // Finish up move processing.
   FinishMove ( Selecting ) ;

   // Unlock current page.
   #ifdef DEBUG_LOCK
      Log ( "Documento::MovePageUp: Unlocking current page." ) ;
   #endif
   UnlockCurrentPage ( ) ;

   // Return good status.
   #ifdef DEBUG_MOTION
      Log ( "Documento::MovePageUp: Done.  Offset %i.", Offset ) ;
   #endif
   return ( NOT Moved ) ;
}
 
/****************************************************************************
 *                                                                          *
 *  Documento: Move Down one Page (towards the bottom) within the document. *
 *                                                                          *
 ****************************************************************************/

BOOL Documento::MovePageDown ( BOOL Selecting ) {

   #ifdef DEBUG_MOTION
      Log ( "Documento::MovePageDown: Starting." ) ;
   #endif

   // Flush the command queue.
   FlushCommandQueue ( ) ;

   // If not displaying entire page . . .
   if ( ScrollMax.y ) {
      if ( Offset >= CurrentPage.FindLine(CurrentPage.Query_NumberOfLines()-1)->Offset )
         return ( MoveLineDown ( Selecting ) ) ;
      SIZEL WindowSize ;
      Query_WindowSize ( WindowSize ) ;
      POINTL Point = { Position.x, max ( 0, Position.y - WindowSize.cy ) } ;
      Offset = FindXY ( Point ) ;
      if ( ScrollPos.y == ScrollMax.y ) {
         return ( MoveLineDown ( Selecting ) ) ;
      } else {
         VerticalScroll ( SB_PAGEDOWN, 0 ) ;
         FinishMove ( Selecting ) ;
         #ifdef DEBUG_MOTION
            Log ( "Documento::MovePageDown: Done.  Scrolled page downwards." ) ;
         #endif
         return ( FALSE ) ;
      } /* endif */
   } /* endif */

   // Lock current page.
   #ifdef DEBUG_LOCK
      Log ( "Documento::MovePageDown: Locking current page." ) ;
   #endif
   LockCurrentPage ( ) ;
   #ifdef DEBUG_LOCK
      Log ( "Documento::MovePageDown:   Current page locked." ) ;
   #endif

   // If we're going to move . . .
   BOOL Moved = FALSE ;
   if ( Offset < pText->QueryCurrentSize()-1 ) {
      Moved = TRUE ;

      // Move down one page.
      if ( PageNumber < NumberOfPages-1 ) {
         HourGlass Hold ( Window ) ;
         SupervisorObject->Block ( ) ;
         PageNumber ++ ;
         Offset = Pages[PageNumber].Offset ;
         if ( !FormatPage ( PageNumber ) )
            CurrentPage = WorkingPage ;
         CurrentPageValid = TRUE ;
         SupervisorObject->Unblock ( ) ;
         if ( Window ) Sys_InvalidateWindow ( Window ) ;
      } else {
         #ifdef DEBUG_LOCK
            Log ( "Documento::MovePageDown: Unlocking current page." ) ;
         #endif
         FinishMove ( Selecting ) ;
         UnlockCurrentPage ( ) ;
         #ifdef DEBUG_MOTION
            Log ( "Documento::MovePageDown: Aborted.  Already on last page!" ) ;
         #endif
         return ( TRUE ) ;
      } /* endif */

      // Find out which line we're on.
      int Line ;
      Linea *pLine = CurrentPage.FindLine ( Position, Line ) ;

      // Determine our location within the line.
      Palabra *pWord = CurrentPage.FindWord ( Line, Column ) ;

      // Determine our location within the word.
      Offset = FindToken ( pLine, pWord, Column ) ;

   } /* endif */

   // Finish up move processing.
   FinishMove ( Selecting ) ;

   // Unlock current page.
   #ifdef DEBUG_LOCK
      Log ( "Documento::MovePageDown: Unlocking current page." ) ;
   #endif
   UnlockCurrentPage ( ) ;

   // Return good status.
   #ifdef DEBUG_MOTION
      Log ( "Documento::MovePageDown: Done.  Offset %i.", Offset ) ;
   #endif
   return ( NOT Moved ) ;
}
 
/****************************************************************************
 *                                                                          *
 *  Documento: Move to top of current page, preserving column.              *
 *                                                                          *
 ****************************************************************************/

BOOL Documento::MovePageTop ( BOOL Selecting ) {

   #ifdef DEBUG_MOTION
      Log ( "Documento::MovePageTop: Starting." ) ;
   #endif

   // Flush the command queue.
   FlushCommandQueue ( ) ;

   // Lock current page.
   #ifdef DEBUG_LOCK
      Log ( "Documento::MovePageTop: Locking current page." ) ;
   #endif
   LockCurrentPage ( ) ;
   #ifdef DEBUG_LOCK
      Log ( "Documento::MovePageTop:   Current page locked." ) ;
   #endif

   // Search for top of page.
   POINTL Point = { Column, 100000 } ;
   Offset = FindXY ( Point ) ;

   // Finish up move processing.
   FinishMove ( Selecting ) ;

   // Unlock current page.
   #ifdef DEBUG_LOCK
      Log ( "Documento::MovePageTop: Unlocking current page." ) ;
   #endif
   UnlockCurrentPage ( ) ;

   // Return good status.
   #ifdef DEBUG_MOTION
      Log ( "Documento::MovePageTop: Done.  Offset %i.", Offset ) ;
   #endif
   return ( FALSE ) ;
}
 
/****************************************************************************
 *                                                                          *
 *  Documento: Move to bottom of current page, preserving column.           *
 *                                                                          *
 ****************************************************************************/

BOOL Documento::MovePageBottom ( BOOL Selecting ) {

   #ifdef DEBUG_MOTION
      Log ( "Documento::MovePageBottom: Starting." ) ;
   #endif

   // Flush the command queue.
   FlushCommandQueue ( ) ;

   // Lock current page.
   #ifdef DEBUG_LOCK
      Log ( "Documento::MovePageBottom: Locking current page." ) ;
   #endif
   LockCurrentPage ( ) ;
   #ifdef DEBUG_LOCK
      Log ( "Documento::MovePageBottom:   Current page locked." ) ;
   #endif

   // Search for bottom of page.
   POINTL Point = { Column, 0 } ;
   Offset = FindXY ( Point ) ;

   // Finish up move processing.
   FinishMove ( Selecting ) ;

   // Unlock current page.
   #ifdef DEBUG_LOCK
      Log ( "Documento::MovePageBottom: Unlocking current page." ) ;
   #endif
   UnlockCurrentPage ( ) ;

   // Return good status.
   #ifdef DEBUG_MOTION
      Log ( "Documento::MovePageBottom: Done.  Offset %i.", Offset ) ;
   #endif
   return ( FALSE ) ;
}
 
/****************************************************************************
 *                                                                          *
 *  Documento: Move to Top (start) of Document                              *
 *                                                                          *
 ****************************************************************************/

BOOL Documento::MoveTop ( BOOL Selecting ) {

   #ifdef DEBUG_MOTION
      Log ( "Documento::MoveTop: Starting." ) ;
   #endif

   // Flush the command queue.
   FlushCommandQueue ( ) ;

   // Lock current page.
   #ifdef DEBUG_LOCK
      Log ( "Documento::MoveTop: Locking current page." ) ;
   #endif
   LockCurrentPage ( ) ;
   #ifdef DEBUG_LOCK
      Log ( "Documento::MoveTop:   Current page locked." ) ;
   #endif

   // If we're going to move . . .
   BOOL Moved = FALSE ;
   if ( Offset ) {
      Moved = TRUE ;

      // Move to the top of the document.
      Offset = 0 ;

      // If we are not on page 1, then move to and format page 1.
      if ( PageNumber ) {
         #ifdef DEBUG_MOTION
            Log ( "Documento::MoveTop: Formatting first page and making it current." ) ;
         #endif
         HourGlass Hold ( Window ) ;
         SupervisorObject->Block ( ) ;
         PageNumber = 0 ;
         if ( !FormatPage ( PageNumber ) )
            CurrentPage = WorkingPage ;
         CurrentPageValid = TRUE ;
         SupervisorObject->Unblock ( ) ;
         if ( Window ) Sys_InvalidateWindow ( Window ) ;
      } /* endif */

   } /* endif */

   // Finish up move processing.
   FinishMove ( Selecting ) ;

   // Reset cursor's preferred column.
   Column = 0 ;

   // Unlock current page.
   #ifdef DEBUG_LOCK
      Log ( "Documento::MoveTop: Unlocking current page." ) ;
   #endif
   UnlockCurrentPage ( ) ;

   // Return good status.
   #ifdef DEBUG_MOTION
      Log ( "Documento::MoveTop: Done.  Offset %i.", Offset ) ;
   #endif
   return ( NOT Moved ) ;
}
 
/****************************************************************************
 *                                                                          *
 *  Documento: Move to Bottom (end) of Document                             *
 *                                                                          *
 ****************************************************************************/

BOOL Documento::MoveBottom ( BOOL Selecting ) {

   #ifdef DEBUG_MOTION
      Log ( "Documento::MoveBottom: Starting." ) ;
   #endif

   // Flush the command queue.
   FlushCommandQueue ( ) ;

   // Lock current page.
   #ifdef DEBUG_LOCK
      Log ( "Documento::MoveBottom: Locking current page." ) ;
   #endif
   LockCurrentPage ( ) ;
   #ifdef DEBUG_LOCK
      Log ( "Documento::MoveBottom:   Current page locked." ) ;
   #endif

   // If we're going to move . . .
   BOOL Moved = FALSE ;
   if ( Offset < pText->QueryCurrentSize()-1 ) {
      Moved = TRUE ;

      // Move to the bottom of the document.
      Offset = pText->QueryCurrentSize() - 1 ;

      // If we are not on the last page, then move to and format the last page.
      SupervisorObject->Block ( ) ;
      if ( Pages[NumberOfPages-1].Offset+Pages[NumberOfPages-1].Length < Offset ) {
         HourGlass Hold ( Window ) ;
         while ( Pages[NumberOfPages-1].Offset+Pages[NumberOfPages-1].Length < Offset ) {
            PageNumber = NumberOfPages ;
            if ( !FormatPage ( PageNumber ) )
               CurrentPage = WorkingPage ;
            CurrentPageValid = TRUE ;
            if ( Window ) {
               Sys_InvalidateWindow ( Window ) ;
               UpdateTitle ( TRUE ) ;
            } /* endif */
         } /* endwhile */
      } else if ( PageNumber < NumberOfPages - 1 ) {
         HourGlass Hold ( Window ) ;
         PageNumber = NumberOfPages - 1 ;
         if ( !FormatPage ( PageNumber ) )
            CurrentPage = WorkingPage ;
         CurrentPageValid = TRUE ;
         if ( Window ) {
            Sys_InvalidateWindow ( Window ) ;
            UpdateTitle ( TRUE ) ;
         } /* endif */
      } /* endif */
      SupervisorObject->Unblock ( ) ;
   } /* endif */

   // Finish up move processing.
   FinishMove ( Selecting ) ;

   // Reset cursor's preferred column.
   Column = 0x7FFFFFFF ;

   // Unlock current page.
   #ifdef DEBUG_LOCK
      Log ( "Documento::MoveBottom: Unlocking current page." ) ;
   #endif
   UnlockCurrentPage ( ) ;

   // Return good status.
   #ifdef DEBUG_MOTION
      Log ( "Documento::MoveBottom: Done.  Offset %i.", Offset ) ;
   #endif
   return ( NOT Moved ) ;
}
 
/****************************************************************************
 *                                                                          *
 *  Documento: Move Up to start of Line, or to start of previous line.      *
 *                                                                          *
 ****************************************************************************/

BOOL Documento::MoveLineTop ( BOOL Selecting ) {

   #ifdef DEBUG_MOTION
      Log ( "Documento::MoveLineTop: Starting." ) ;
   #endif

   // Flush the command queue.
   FlushCommandQueue ( ) ;

   // Lock current page.
   #ifdef DEBUG_LOCK
      Log ( "Documento::MoveLineTop: Locking current page." ) ;
   #endif
   LockCurrentPage ( ) ;
   #ifdef DEBUG_LOCK
      Log ( "Documento::MoveLineTop:   Current page locked." ) ;
   #endif

   // If we're going to move . . .
   BOOL Moved = FALSE ;
   if ( Offset ) {
      Moved = TRUE ;

      // Move towards the top of the document.
      Offset = MoveTokenUp ( Offset ) ;

      // Determine which line of the page we are on.
      int Line ;
      Linea *pLine = CurrentPage.FindLine ( Offset, Line ) ;

      // If we're now on the previous page, format the page.
      if ( !pLine ) {
         HourGlass Hold ( Window ) ;
         SupervisorObject->Block ( ) ;
         PageNumber -- ;
         Offset = CurrentPage.Query_Offset() - 1 ;
         if ( !FormatPage ( PageNumber ) )
            CurrentPage = WorkingPage ;
         CurrentPageValid = TRUE ;
         SupervisorObject->Unblock ( ) ;
         if ( Window ) Sys_InvalidateWindow ( Window ) ;
         pLine = CurrentPage.FindLine ( CurrentPage.Query_NumberOfLines() - 1 ) ;
      } /* endif */
        
      // Move to the top of the current line.
      Offset = pLine->Offset ;

   } /* endif */

   // Finish up move processing.
   FinishMove ( Selecting ) ;

   // Reset cursor's preferred column.
   Column = 0 ;

   // Unlock current page.
   #ifdef DEBUG_LOCK
      Log ( "Documento::MoveLineTop: Unlocking current page." ) ;
   #endif
   UnlockCurrentPage ( ) ;

   // Return good status.
   #ifdef DEBUG_MOTION
      Log ( "Documento::MoveLineTop: Done.  Offset %i.", Offset ) ;
   #endif
   return ( NOT Moved ) ;
}
 
/****************************************************************************
 *                                                                          *
 *  Documento: Move Down to end of Line, or to end of next line.            *
 *                                                                          *
 ****************************************************************************/

BOOL Documento::MoveLineBottom ( BOOL Selecting ) {

   #ifdef DEBUG_MOTION
      Log ( "Documento::MoveLineBottom: Starting." ) ;
   #endif

   // Flush the command queue.
   FlushCommandQueue ( ) ;

   // Lock current page.
   #ifdef DEBUG_LOCK
      Log ( "Documento::MoveLineBottom: Locking current page." ) ;
   #endif
   LockCurrentPage ( ) ;
   #ifdef DEBUG_LOCK
      Log ( "Documento::MoveLineBottom:   Current page locked." ) ;
   #endif

   // If we're going to move . . .
   BOOL Moved = FALSE ;
   if ( Offset < pText->QueryCurrentSize()-1 ) {
      Moved = TRUE ;

      // Move one token towards the bottom.
      Offset = MoveTokenDown ( Offset ) ;

      // Determine which line of the page we are on.
      int Line ;
      Linea *pLine = CurrentPage.FindLine ( Offset, Line ) ;

      // If we're no longer on the current page, move to the next.
      if ( pLine == 0 ) {
         HourGlass Hold ( Window ) ;
         SupervisorObject->Block ( ) ;
         PageNumber ++ ;
         Offset = Pages[PageNumber].Offset ;
         if ( !FormatPage ( PageNumber ) )
            CurrentPage = WorkingPage ;
         CurrentPageValid = TRUE ;
         SupervisorObject->Unblock ( ) ;
         if ( Window ) Sys_InvalidateWindow ( Window ) ;
         pLine = CurrentPage.FindLine ( (int)0 ) ;
      } /* endif */
   
      // Move to the bottom of the current line.
      Offset = pLine->Offset + pLine->Length ;
      Offset = MoveTokenUp ( Offset ) ;

   } /* endif */

   // Finish up move processing.
   FinishMove ( Selecting ) ;

   // Reset cursor's preferred column.
   Column = 0x7FFFFFFF ;

   // Unlock current page.
   #ifdef DEBUG_LOCK
      Log ( "Documento::MoveLineBottom: Unlocking current page." ) ;
   #endif
   UnlockCurrentPage ( ) ;

   // Return good status.
   #ifdef DEBUG_MOTION
      Log ( "Documento::MoveLineBottom: Done.  Offset %i.", Offset ) ;
   #endif
   return ( NOT Moved ) ;
}
 
/****************************************************************************
 *                                                                          *
 *  Documento: Move to page 'n', where 'n' is numbered from zero.           *
 *                                                                          *
 ****************************************************************************/

BOOL Documento::MovePageNumber ( int Number, BOOL Selecting ) {

   #ifdef DEBUG_MOTION
      Log ( "Documento::MovePageNumber: Starting." ) ;
   #endif

   // Flush the command queue.
   FlushCommandQueue ( ) ;

   // Lock current page.
   #ifdef DEBUG_LOCK
      Log ( "Documento::MovePageNumber: Locking current page." ) ;
   #endif
   LockCurrentPage ( ) ;
   #ifdef DEBUG_LOCK
      Log ( "Documento::MovePageNumber:   Current page locked." ) ;
   #endif

   // If we're going to move . . .
   BOOL Moved = FALSE ;
   if ( ( Number >= 0 ) && ( Number < NumberOfPages ) ) {
      Moved = TRUE ;

      // If the page number is changing . . .
      if ( PageNumber != Number ) {

         // Format the selected page.
         HourGlass Hold ( Window ) ;
         SupervisorObject->Block ( ) ;
         PageNumber = Number ;
         Offset = Pages[PageNumber].Offset ;
         if ( !FormatPage ( PageNumber ) )
            CurrentPage = WorkingPage ;
         CurrentPageValid = TRUE ;
         SupervisorObject->Unblock ( ) ;
         if ( Window ) Sys_InvalidateWindow ( Window ) ;
     
         // Find the token we're moving to.
         Offset = FindXY ( Position ) ;
   
      } /* endif */

   } /* endif */

   // Finish up move processing.
   FinishMove ( Selecting ) ;

   // Reset cursor's preferred column.
   Column = Position.x ;

   // Unlock current page.
   #ifdef DEBUG_LOCK
      Log ( "Documento::MovePageNumber: Unlocking current page." ) ;
   #endif
   UnlockCurrentPage ( ) ;

   // Return good status.
   #ifdef DEBUG_MOTION
      Log ( "Documento::MovePageNumber: Done.  Offset %i.", Offset ) ;
   #endif
   return ( NOT Moved ) ;
}
 
/****************************************************************************
 *                                                                          *
 *  Documento: Move to X,Y position on the current page.                    *
 *                                                                          *
 ****************************************************************************/

void Documento::MoveXY ( POINTL &Point, BOOL Selecting ) {

   #ifdef DEBUG_MOTION
      Log ( "Documento::MoveXY(X=%i,Y=%i,%s): Starting.", Point.x, Point.y, Selecting?"Selecting":"Deselecting" ) ;
   #endif

   // Flush the command queue.
   FlushCommandQueue ( ) ;

   // Lock current page.
   #ifdef DEBUG_LOCK
      Log ( "Documento::MoveXY: Locking current page." ) ;
   #endif
   LockCurrentPage ( ) ;
   #ifdef DEBUG_LOCK
      Log ( "Documento::MoveXY:   Current page locked." ) ;
   #endif

   // Find the token we're moving to.
   Offset = FindXY ( Point ) ;

   // Reset cursor's preferred column.
   Column = Point.x ;

   // Finish up move processing.
   FinishMove ( Selecting ) ;

   // Unlock current page.
   #ifdef DEBUG_LOCK
      Log ( "Documento::MoveXY: Unlocking current page." ) ;
   #endif
   UnlockCurrentPage ( ) ;

   #ifdef DEBUG_MOTION
      Log ( "Documento::MoveXY(X=%i,Y=%i,%s): Done.  Offset %i.", Point.x, Point.y, Selecting?"Selecting":"Deselecting", Offset ) ;
   #endif
}

void Documento::MoveXY ( POINTL &Point ) {

   #ifdef DEBUG_MOTION
      Log ( "Documento::MoveXY(X=%i,Y=%i): Starting.", Point.x, Point.y ) ;
   #endif

   // Flush the command queue.
   FlushCommandQueue ( ) ;

   // Lock current page.
   #ifdef DEBUG_LOCK
      Log ( "Documento::MoveXY: Locking current page." ) ;
   #endif
   LockCurrentPage ( ) ;
   #ifdef DEBUG_LOCK
      Log ( "Documento::MoveXY:   Current page locked." ) ;
   #endif

   // Find the token we're moving to.
   Offset = FindXY ( Point ) ;

   // Reset cursor's preferred column.
   Column = Point.x ;

   // Finish up move processing.
   FinishMove ( ) ;

   // Unlock current page.
   #ifdef DEBUG_LOCK
      Log ( "Documento::MoveXY: Unlocking current page." ) ;
   #endif
   UnlockCurrentPage ( ) ;

   #ifdef DEBUG_MOTION
      Log ( "Documento::MoveXY(X=%i,Y=%i): Done.  Offset %i.", Point.x, Point.y, Offset ) ;
   #endif
}
 
/****************************************************************************
 *                                                                          *
 *  Documento: Find token at X,Y position on the current page.              *
 *                                                                          *
 ****************************************************************************/

long Documento::FindXY ( POINTL &Point ) {

   // Reset position.
   long Offset = 0 ;

   // Check to see if we're touching a graphic or graphic border.
   Grafico *pGraphic = CurrentPage.FindGraphic ( (int)0 ) ;
   while ( pGraphic && ( pGraphic->Offset < CurrentPage.Query_Offset()+CurrentPage.Query_CurrentLength() ) ) {

      RECTL Box ;
      pGraphic->ComputeBox ( CurrentPage.FindWord(pGraphic->Offset), Box, PrinterResolution, TRUE ) ;

      if ( ( Point.x < Box.xLeft ) OR ( Point.y < Box.yBottom ) ) {
         pGraphic = pGraphic->NextGraphic ( ) ;
         continue ;
      } /* endif */

      if ( ( Point.x > Box.xRight ) OR ( Point.y > Box.yTop ) ) {
         pGraphic = pGraphic->NextGraphic ( ) ;
         continue ;
      } /* endif */

      Offset = pGraphic->Offset ;
      break ;

   } /* endwhile */

   // If the token was not found as a graphic . . .
   if ( Offset == 0 ) {

      // Find out which line we're on.
      int Line ;
      Linea *pLine = CurrentPage.FindLine ( Point, Line ) ;

      // Determine our location within the line.
      Palabra *pWord = CurrentPage.FindWord ( Line, int(Point.x) ) ;

      // Determine our location within the word.
      Offset = FindToken ( pLine, pWord, Point.x ) ;

   } /* endif */

   // Return the token's offset.
   return ( Offset ) ;
}
 
/****************************************************************************
 *                                                                          *
 *  Documento: Test X,Y position on the current page.                       *
 *                                                                          *
 ****************************************************************************/

int Documento::TestXY ( POINTL &Point, BOOL &Selected ) {

   #ifdef DEBUG
   Log ( "Documento::TestXY(): Started.  Scanning graphic list." ) ;
   #endif

   // Check to see if we're touching a graphic or graphic border.
   Grafico *pGraphic = CurrentPage.FindGraphic ( (int)0 ) ;
   while ( pGraphic && ( pGraphic->Offset < CurrentPage.Query_Offset()+CurrentPage.Query_CurrentLength() ) ) {

      RECTL Box ;
      pGraphic->ComputeBox ( CurrentPage.FindWord(pGraphic->Offset), Box, PrinterResolution, TRUE ) ;

      if ( ( Point.x < Box.xLeft ) OR ( Point.y < Box.yBottom ) ) {
         pGraphic = pGraphic->NextGraphic ( ) ;
         continue ;
      } /* endif */

      if ( ( Point.x > Box.xRight ) OR ( Point.y > Box.yTop ) ) {
         pGraphic = pGraphic->NextGraphic ( ) ;
         continue ;
      } /* endif */

      Selected = IsSelected ( pGraphic->Offset ) ;

      pGraphic->ComputeBox ( CurrentPage.FindWord(pGraphic->Offset), Box, PrinterResolution ) ;

      if ( ( Point.x < Box.xLeft ) OR ( Point.y < Box.yBottom ) ) {
         #ifdef DEBUG
         Log ( "Documento::TestXY(): Found graphic border." ) ;
         #endif
         return ( TEST_GRAPHICBORDER ) ; }

      if ( ( Point.x > Box.xRight ) OR ( Point.y > Box.yTop ) ) {
         #ifdef DEBUG
         Log ( "Documento::TestXY(): Found graphic border." ) ;
         #endif
         return ( TEST_GRAPHICBORDER ) ; }

      #ifdef DEBUG
      Log ( "Documento::TestXY(): Found graphic." ) ;
      #endif

      return ( TEST_GRAPHIC ) ;

   } /* endwhile */

   #ifdef DEBUG
   Log ( "Documento::TestXY(): Scanning line list." ) ;
   #endif

   // Find out which line we're on.
   int Line ;
   Linea *pLine = CurrentPage.FindLine ( Point, Line ) ;

   // Determine our location within the line.
   #ifdef DEBUG
   Log ( "Documento::TestXY(): Finding word in line." ) ;
   #endif
   Palabra *pWord = CurrentPage.FindWord ( Line, int(Point.x) ) ;

   // Determine our location within the word.
   #ifdef DEBUG
   Log ( "Documento::TestXY(): Finding token in word." ) ;
   #endif
   long Offset = FindToken ( pLine, pWord, Point.x ) ;

   // Determine if the point is selected.
   Selected = IsSelected ( Offset ) ;

   #ifdef DEBUG
   Log ( "Documento::TestXY(): Text found." ) ;
   #endif

   return ( TEST_TEXT ) ;
}
 
/****************************************************************************
 *                                                                          *
 *  Documento: Query for Graphic Rectangle at Point.                        *
 *                                                                          *
 ****************************************************************************/

BOOL Documento::QueryGraphic ( POINTL &Point, Grafico &Graphic, RECTL &Box, BOOL IncludeBorders ) {

   // Check to see if we're touching a graphic or graphic border.
   Grafico *pGraphic = CurrentPage.FindGraphic ( (int)0 ) ;
   while ( pGraphic && ( pGraphic->Offset < CurrentPage.Query_Offset()+CurrentPage.Query_CurrentLength() ) ) {

      pGraphic->ComputeBox ( CurrentPage.FindWord(pGraphic->Offset), Box, PrinterResolution, IncludeBorders ) ;

      if ( ( Point.x < Box.xLeft ) OR ( Point.y < Box.yBottom ) ) {
         pGraphic = pGraphic->NextGraphic ( ) ;
         continue ;
      } /* endif */

      if ( ( Point.x > Box.xRight ) OR ( Point.y > Box.yTop ) ) {
         pGraphic = pGraphic->NextGraphic ( ) ;
         continue ;
      } /* endif */

      Graphic = * pGraphic ;
      return ( TRUE ) ;

   } /* endfor */

   return ( FALSE ) ;
}
 
/****************************************************************************
 *                                                                          *
 *  Documento: Select Text from OffsetLast to Offset.                       *
 *                                                                          *
 ****************************************************************************/

void Documento::Select ( WorkSpace *DisplayPS ) {

   // If we are to update the window . . .
   if ( DisplayPS ) {

      // Invert text from OffsetLast to Offset.
      PainterObject->HideCursor ( ) ;
      PainterObject->UpdateWindow ( FALSE, TRUE ) ;
      PainterObject->LockUpdate() ;
      #ifdef DEBUG_INVERTTEXT
         Log ( "Documento::Select() Inverting from %i to %i.", OffsetLast, Offset ) ;
      #endif
      InvertText ( DisplayPS, OffsetLast, Offset ) ;
      PainterObject->UnlockUpdate() ;
      PainterObject->ShowCursor ( ) ;

   } /* endif */

   OffsetLast = Offset ;
}
 
/****************************************************************************
 *                                                                          *
 *  Documento: Deselect all selected text from OffsetOrigin to OffsetLast.  *
 *                                                                          *
 ****************************************************************************/

void Documento::Deselect ( WorkSpace *DisplayPS ) {

   // If we are to update the window . . .
   if ( DisplayPS ) {

      // Invert text from OffsetOrigin to OffsetLast.
      PainterObject->HideCursor ( ) ;
      PainterObject->UpdateWindow ( FALSE, TRUE ) ;
      PainterObject->LockUpdate() ;
      #ifdef DEBUG_INVERTTEXT
         Log ( "Documento::Deselect() Inverting from %i to %i.", OffsetOrigin, OffsetLast ) ;
      #endif
      InvertText ( DisplayPS, OffsetOrigin, OffsetLast ) ;
      PainterObject->UnlockUpdate ( ) ;
      PainterObject->ShowCursor ( ) ;

   } /* endif */

   OffsetOrigin = OffsetLast = Offset ;
}
 
/****************************************************************************
 *                                                                          *
 *  Documento: Select entire document.                                      *
 *                                                                          *
 ****************************************************************************/

void Documento::SelectAll ( ) {

   // Flush the command queue.
   FlushCommandQueue ( ) ;

   // Determine how big the document is.
   long TextSize = pText->QueryCurrentSize() ;

   // If there's a window associated with this document . . .
   if ( Window ) {

      PainterObject->HideCursor ( ) ;
      PainterObject->UpdateWindow ( FALSE, TRUE ) ;
      PainterObject->LockUpdate ( ) ;
      WorkSpace PS ( "SelectAll", DocParms.CodePage, Window, pWindowDevice, int(State0.Metric) ) ;
      SIZEL WindowSize ;
      Query_WindowSize ( WindowSize ) ;
      PS.SetTransform ( Zoom, int(TopLeft.x), int(TopLeft.y-WindowSize.cy) ) ;

      long Start = min ( OffsetOrigin, OffsetLast ) ;
      if ( Start ) {
         #ifdef DEBUG_INVERTTEXT
            Log ( "Documento::SelectAll() Inverting from %i to %i.", 0, Start ) ;
         #endif
         InvertText ( &PS, 0, Start ) ;
      } /* endif */

      long Finish = max ( OffsetOrigin, OffsetLast ) ;
      if ( Finish < TextSize-1 ) {
         #ifdef DEBUG_INVERTTEXT
            Log ( "Documento::SelectAll() Inverting from %i to %i.", Finish, TextSize-1 ) ;
         #endif
         InvertText ( &PS, Finish, TextSize-1 ) ;
      } /* endif */

      PainterObject->UnlockUpdate ( ) ;
      PainterObject->ShowCursor ( ) ;

   } /* endif */

   // Reset the current selection range to include all the document.
   OffsetOrigin = 0 ;
   OffsetLast = TextSize - 1 ;
}
 
/****************************************************************************
 *                                                                          *
 *  Documento: Select Word at point.                                        *
 *                                                                          *
 ****************************************************************************/

void Documento::SelectWord ( POINTL &Point ) {

   // Flush the command queue.
   FlushCommandQueue ( ) ;

   // Find what's at the point.
   long Offset = FindXY ( Point ) ;

   // Find the start of the word.
   PUCHAR pByte ;
   pText->QueryBytePtr ( Offset, pByte ) ;
   if ( NOT IsEndOfWord ( pByte, PS->Query_DBCS_Vector() ) ) {
      while ( Offset AND !IsEndOfWord ( pByte, PS->Query_DBCS_Vector() ) ) {
         Offset = MoveTokenUp ( Offset ) ;
         pText->QueryBytePtr ( Offset, pByte ) ;
      } /* endwhile */
      Offset = MoveTokenDown ( Offset ) ;
   } /* endif */
   long Start = Offset ;

   // Find the end of the word.
   pText->QueryBytePtr ( Offset, pByte ) ;
   if ( NOT IsEndOfWord ( pByte, PS->Query_DBCS_Vector() ) ) {
      while ( Offset AND !IsEndOfWord ( pByte, PS->Query_DBCS_Vector() ) ) {
         Offset = MoveTokenDown ( Offset ) ;
         pText->QueryBytePtr ( Offset, pByte ) ;
      } /* endwhile */
      Offset = MoveTokenUp ( Offset ) ;
   } /* endif */
   Offset = MoveTokenDown ( Offset ) ;
   long End = Offset ;

   // If there's a window associated with this document . . .
   if ( Window ) {

      PainterObject->HideCursor ( ) ;
      PainterObject->UpdateWindow ( FALSE, TRUE ) ;
      PainterObject->LockUpdate ( ) ;
      WorkSpace PS ( "SelectAll", DocParms.CodePage, Window, pWindowDevice, int(State0.Metric) ) ;
      SIZEL WindowSize ;
      Query_WindowSize ( WindowSize ) ;
      PS.SetTransform ( Zoom, int(TopLeft.x), int(TopLeft.y-WindowSize.cy) ) ;

      #ifdef DEBUG_INVERTTEXT
         Log ( "Documento::SelectWord() Inverting from %i to %i.", OffsetOrigin, OffsetLast ) ;
      #endif
      InvertText ( &PS, OffsetOrigin, OffsetLast ) ;
      #ifdef DEBUG_INVERTTEXT
         Log ( "Documento::SelectWord() Inverting from %i to %i.", Start, End ) ;
      #endif
      InvertText ( &PS, Start, End ) ;

      PainterObject->UnlockUpdate ( ) ;
      PainterObject->ShowCursor ( ) ;

   } /* endif */

   // Reset the current selection range to include the entire word.
   OffsetOrigin = Start ;
   OffsetLast = End ;
}
 
/****************************************************************************
 *                                                                          *
 *  Documento: Query Selected Rectangle                                     *
 *                                                                          *
 ****************************************************************************/

static void QueryTextUpdate ( RECTL &Rectangle, RECTL &Box ) {
   Rectangle.xLeft = min ( Rectangle.xLeft, Box.xLeft ) ;
   Rectangle.xRight = max ( Rectangle.xRight, Box.xRight ) ;
   Rectangle.yBottom = min ( Rectangle.yBottom, Box.yBottom ) ;
   Rectangle.yTop = max ( Rectangle.yTop, Box.yTop ) ;
}

static void QueryTextUpdate ( WorkSpace *PS, RECTL &Rectangle, POINTL &Position, SIZEL &Size ) {
   RECTL Box = { Position.x, Position.y-PS->QueryDescent()+1,
      Position.x+Size.cx, Position.y-PS->QueryDescent()+Size.cy } ;
   QueryTextUpdate ( Rectangle, Box ) ;
}

static void QueryTextFlush (
   WorkSpace *PS,
   char *String,
   int &StringLength,
   POINTL &Position,
   RECTL &Rectangle
) {
   if ( StringLength ) {
      #ifdef DEBUG
         Log ( "QueryTextFlush: Flushing '%0.*s'.", StringLength, String ) ;
      #endif
      SIZEL Size ;
      PS->MeasureText ( String, StringLength, Size ) ;
      StringLength = 0 ;
      QueryTextUpdate ( PS, Rectangle, Position, Size ) ;
      Position.x += Size.cx ;
   } /* endif */
}

void Documento::QueryTextRectangle ( RECTL &Rectangle, long Offset1, long Offset2 ) {

   // Reset rectangle.
   Rectangle.xLeft = Rectangle.yBottom = 0x7FFFFFFF ;
   Rectangle.xRight = Rectangle.yTop = 0 ;

    // Correct first offset selected to compensate for paging.
   if ( Offset1 < CurrentPage.Query_Offset() ) {
      Offset1 = CurrentPage.Query_Offset() ;
   } else if ( Offset1 > CurrentPage.Query_Offset() + CurrentPage.Query_Length() ) {
      Offset1 = CurrentPage.Query_Offset() + CurrentPage.Query_Length() ;
   } /* endif */

   // Correct last offset selected to compensate for paging.
   if ( Offset2 < CurrentPage.Query_Offset() ) {
      Offset2 = CurrentPage.Query_Offset() ;
   } else if ( Offset2 > CurrentPage.Query_Offset() + CurrentPage.Query_Length() ) {
      Offset2 = CurrentPage.Query_Offset() + CurrentPage.Query_Length() ;
   } /* endif */

   // Determine the range of text whose selection is to be inverted.
   long Start = min ( Offset1, Offset2 ) ;
   long Finish = max ( Offset1, Offset2 ) ;

   // Find the word to start at.
   Palabra *pWord = CurrentPage.FindWord ( (int)0 ) ;
   for ( int Word=0; Word<CurrentPage.Query_NumberOfWords(); Word++, pWord=pWord->NextWord() ) 
      if ( ( Start >= pWord->Offset ) AND ( Start < pWord->Offset+pWord->Length ) )
         break ;

   // Create a PS to work with.
   WorkSpace PS = WorkSpace ( "QuerySelectedRectangle", DocParms.CodePage, Anchor,
      int(State0.Metric), int(State0.FormInfo.cx), int(State0.FormInfo.cy) ) ;

   // Get starting token information.
   Estado State ;  POINTL Position ;  SIZEL Size ;
   ComputeState ( Start, State, Position, Size ) ;
   PS.SetState ( State ) ;

   // Measure all text from Start to Finish.
   char String [500] ;
   int StringLength = 0 ;
   PUCHAR pByte ;
   pText->QueryBytePtr ( Start, pByte ) ;
   while ( Start < Finish ) {

      // If we've moved on to the next word . . .
      if ( Start >= pWord->Offset + pWord->Length ) {

         // Measure the current rectangle, if any.
         QueryTextFlush ( &PS, String, StringLength, Position, Rectangle ) ;

         // Advance to the next non-dormant word.
         Word ++ ;  pWord = CurrentPage.FindWord ( Word ) ;
         while ( pWord->Dormant ) { Word ++ ;  pWord = CurrentPage.FindWord ( Word ) ; }
         Start = pWord->Offset ;
         pText->QueryBytePtr ( Start, pByte ) ;
         Position = pWord->Position ;
         Size = pWord->Size ;

      } /* endif */

      // Exit if we're skipped to far by skipping dormant words.
      if ( Start >= Finish )
         continue ;

      // Process the current character.
      if ( IsGraphic ( pByte ) ) {                           // Graphic cell tokens.
         long LengthIn ;
         ComputeFunctionSize ( pByte, LengthIn ) ;
         Start += LengthIn ;
         POINTL Position ;
         char *pData = PCHAR(pByte) + 2 ;
         Position.x = *PLONG(pData) ;   pData += sizeof(LONG) ;
         Position.y = *PLONG(pData) ;   pData += sizeof(LONG) ;
         SIZEL Size ;
         Size.cx = *PLONG(pData) ;      pData += sizeof(LONG) ;
         Size.cy = *PLONG(pData) ;      pData += sizeof(LONG) ;
         if ( Position.x == GRAPHIC_NEXT_CHAR ) {
            QueryTextUpdate ( &PS, Rectangle, Position, Size ) ;
         } else {
            RECTL Box ;
            Box.xLeft   = Position.x ;
            Box.yBottom = Position.y ;
            Box.xRight  = Box.xLeft + Size.cx ;
            Box.yTop    = Box.yBottom + Size.cy ;
            QueryTextUpdate ( Rectangle, Box ) ;
         } /* endif */

      } else if ( IsTab ( *pByte ) ) {                       // Tab tokens.
         Start ++ ;
         QueryTextUpdate ( &PS, Rectangle, Position, Size ) ;

      } else if ( IsSpace ( *pByte ) ) {                     // Space tokens.
         Start ++ ;
         QueryTextUpdate ( &PS, Rectangle, Position, Size ) ;

      } else if ( IsSeparator ( pByte ) ) {                  // Separator tokens.
         Start ++ ;
         QueryTextUpdate ( &PS, Rectangle, Position, Size ) ;

      } else if ( IsGlyphFunction ( pByte ) ) {              // Glyph function tokens.
         long LengthIn ;
         ComputeFunctionSize ( pByte, LengthIn ) ;
         Start += LengthIn ;
         char Text [100] ;
         switch ( *(pByte+1) ) {
            case FN__DATEFIXED: {
               FormatDate ( *PULONG(pByte+2), State.DateFormat, Text, sizeof(Text) ) ;
               if ( StringLength+strlen(Text)+1 > sizeof(String) )
                  QueryTextFlush ( &PS, String, StringLength, Position, Rectangle ) ;
               strcpy ( String+StringLength, Text ) ;
               StringLength += strlen(Text) ;
               break; }
            case FN__DATECURRENT: {
               FormatDate ( time(0), State.DateFormat, Text, sizeof(Text) ) ;
               if ( StringLength+strlen(Text)+1 > sizeof(String) )
                  QueryTextFlush ( &PS, String, StringLength, Position, Rectangle ) ;
               strcpy ( String+StringLength, Text ) ;
               StringLength += strlen(Text) ;
               break; }
            case FN__DATECHANGED: {
               FormatDate ( Query_CurrentLevel()?time(0):FileDate, State.DateFormat, Text, sizeof(Text) ) ;
               if ( StringLength+strlen(Text)+1 > sizeof(String) )
                  QueryTextFlush ( &PS, String, StringLength, Position, Rectangle ) ;
               strcpy ( String+StringLength, Text ) ;
               StringLength += strlen(Text) ;
               break; }
            case FN__PAGENUMBER: {
               sprintf ( Text, "%i", PageNumber+1 ) ;
               if ( StringLength+strlen(Text)+1 > sizeof(String) )
                  QueryTextFlush ( &PS, String, StringLength, Position, Rectangle ) ;
               strcpy ( String+StringLength, Text ) ;
               StringLength += strlen(Text) ;
               break; }
            case FN__PAGECOUNT: {
               sprintf ( Text, "%i", NumberOfPages ) ;
               if ( StringLength+strlen(Text)+1 > sizeof(String) )
                  QueryTextFlush ( &PS, String, StringLength, Position, Rectangle ) ;
               strcpy ( String+StringLength, Text ) ;
               StringLength += strlen(Text) ;
               break; }
            case FN__GLYPH: {
               sprintf ( Text, "%c", *PUCHAR(pByte+2) ) ;
               if ( StringLength+strlen(Text)+1 > sizeof(String) )
                  QueryTextFlush ( &PS, String, StringLength, Position, Rectangle ) ;
               strcpy ( String+StringLength, Text ) ;
               StringLength += strlen(Text) ;
               break; }
            case FN__SEPARATOR: {
               // Already handled above.
               break; }
         } /* endswitch */

      } else if ( IsGlyph ( pByte, PS.Query_DBCS_Vector() ) ) { // Normal glyphs.
         String[StringLength++] = *pByte ;
         Start ++ ;
         if ( PS.IsDBCSHeader ( *pByte ) ) {
            String[StringLength++] = *(pByte+1) ;
            Start ++ ;
         } /* endif */
         if ( StringLength >= MAX_STRING ) 
            QueryTextFlush ( &PS, String, StringLength, Position, Rectangle ) ;

      } else if ( IsFunction ( *pByte ) ) {          // Non-glyph function tokens.
         QueryTextFlush ( &PS, String, StringLength, Position, Rectangle ) ;
         long LengthIn ;
         ComputeFunctionSize ( pByte, LengthIn ) ;
         Start += LengthIn ;
         State.ProcessFunction ( pByte, &PS, PageNumber, NumberOfPages, Query_CurrentLevel()?time(0):FileDate ) ;

      } else {                                      // EndLine, EndPage and EndDocument tokens.
         Start ++ ;

      } /* endif */

      // Get the pointer to the next token.
      pText->QueryBytePtr ( Start, pByte ) ;

   } /* endwhile */

   // If anything left, invert it now.
   QueryTextFlush ( &PS, String, StringLength, Position, Rectangle ) ;
}

void Documento::QueryTextRectangle ( RECTL &Rectangle ) {
   QueryTextRectangle ( Rectangle, OffsetOrigin, OffsetLast ) ;
}

 
/****************************************************************************
 *                                                                          *
 *  Documento: Invert text from Offset1 to Offset2                          *
 *                                                                          *
 ****************************************************************************/

   // Flush text to the current offset 'Start' (inclusive).
int Documento::InvertFlush ( WorkSpace *DisplayPS, char *String, int &StringLength, long Start, Estado &State, POINTL &Position, SIZEL &Size ) {
   POINTL NewPosition ;  SIZEL NewSize ;
   ComputeState ( Start, State, NewPosition, NewSize ) ;
   int Result = 0 ;
   if ( StringLength ) {
      #ifdef DEBUG_INVERTTEXT
         Log ( "InvertFlush: Flushing '%0.*s' (cell %i,%i (%ix%i)).", 
            StringLength, String, Position.x, Position.y, NewPosition.x-Position.x+NewSize.cx, NewSize.cy ) ;
         Log ( "InvertFlush: Old position %i,%i (%ix%i), new position %i,%i (%ix%i).", 
            Position.x, Position.y, Size.cx, Size.cy, NewPosition.x, NewPosition.y, NewSize.cx, NewSize.cy ) ;
      #else
         String ++ ;
      #endif
      DisplayPS->SetState ( State ) ;
      DisplayPS->Move ( Position ) ;
      SIZEL Cell = { NewPosition.x-Position.x+NewSize.cx, NewSize.cy } ;
      DisplayPS->InvertCell ( Cell ) ;
      Result = int ( NewPosition.x - Position.x ) ;
      StringLength = 0 ;
   } /* endif */
   Position = NewPosition ;  Size = NewSize ;
   return ( Result ) ;
}

void Documento::InvertText ( WorkSpace *DisplayPS, long Offset1, long Offset2 ) {

   #ifdef DEBUG_INVERTTEXT
      Log ( "Documento::InvertText(%i->%i) Starting.", Offset1, Offset2 ) ;
   #endif

   // Correct first offset selected to compensate for paging.
   if ( Offset1 < CurrentPage.Query_Offset() ) {
      Offset1 = CurrentPage.Query_Offset() ;
   } else if ( Offset1 > CurrentPage.Query_Offset() + CurrentPage.Query_Length() ) {
      Offset1 = CurrentPage.Query_Offset() + CurrentPage.Query_Length() ;
   } /* endif */

   // Correct last offset selected to compensate for paging.
   if ( Offset2 < CurrentPage.Query_Offset() ) {
      Offset2 = CurrentPage.Query_Offset() ;
   } else if ( Offset2 > CurrentPage.Query_Offset() + CurrentPage.Query_Length() ) {
      Offset2 = CurrentPage.Query_Offset() + CurrentPage.Query_Length() ;
   } /* endif */

   // Determine the range of text whose selection is to be inverted.
   long Start = min ( Offset1, Offset2 ) ;
   long Finish = max ( Offset1, Offset2 ) ;

   #ifdef DEBUG_INVERTTEXT
      Log ( "Documento::InvertText(%i->%i) Starting, ending offsets: %i, %i.", Offset1, Offset2, Start, Finish ) ;
   #endif

   // Find the word to start at.
   Palabra *pWord = CurrentPage.FindWord ( (int)0 )  ;
   for ( int Word=0; Word<CurrentPage.Query_NumberOfWords(); Word++, pWord=pWord->NextWord() ) 
      if ( ( Start >= pWord->Offset ) AND ( Start < pWord->Offset+pWord->Length ) )
         break ;

   // Find the line to start at.
   int Line ;
   Linea *pLine = CurrentPage.FindLine ( Start, Line ) ;

   // Get starting token information.
   Estado State ;  POINTL Position ;  SIZEL Size ;
   ComputeState ( Start, State, Position, Size ) ;
   DisplayPS->SetState ( State ) ;

   // Lock the window for update, preventing the painter from interfering.
   PainterObject->HideCursor ( ) ;
   PainterObject->LockUpdate ( ) ;

   // Invert all text from Start to Finish.
   char String [500] ;
   int StringLength = 0 ;
   PUCHAR pByte ;
   pText->QueryBytePtr ( Start, pByte ) ;
   long OldStart = Start ;
   while ( Start < Finish ) {

      // If we've moved on to the next word . . .
      if ( Start >= pWord->Offset + pWord->Length ) {

         // Move to the next word.
         Word ++, pWord = pWord->NextWord ( ) ;
         #ifdef DEBUG_INVERTTEXT
            Log ( "Documento::InvertText(%i->%i) Moved to word %i (%i->%i).", Offset1, Offset2, Word, pWord->Offset, pWord->Offset+pWord->Length-1 ) ;
         #endif

         // If we've started a new line, move to it, flushing any accumulated text.
         if ( ( Start >= pLine->Offset + pLine->Length ) ) {
            InvertFlush ( DisplayPS, String, StringLength, OldStart, State, Position, Size ) ;
            Position = pWord->Position ; 
            Line ++, pLine = pLine->NextLine ( ) ;
            #ifdef DEBUG_INVERTTEXT
               Log ( "Documento::InvertText(%i->%i) Moved to line %i (%i->%i).", Offset1, Offset2, Line, pLine->Offset, pLine->Offset+pLine->Length-1 ) ;
            #endif

         // Else, if we're starting a new segment, flush the old one first.
         } else if ( pWord->SegmentStart OR IsTab ( *pByte ) OR ( IsSpace ( *pByte ) AND ( State.Justify == JUSTIFY_FULL ) ) ) {
            InvertFlush ( DisplayPS, String, StringLength, OldStart, State, Position, Size ) ;
            Position = pWord->Position ; 

         } /* endif */

      } /* endif new word */

      // Save the current start as the old one.
      OldStart = Start ;

      // Process the current character.
      if ( IsGraphic ( pByte ) ) {                           // Graphic cell tokens.
         #ifdef DEBUG_INVERTTEXT
            Log ( "Documento::InvertText(%i->%i) Processing graphic token.", Offset1, Offset2 ) ;
         #endif
         InvertFlush ( DisplayPS, String, StringLength, OldStart, State, Position, Size ) ;
         Grafico *pGraphic = CurrentPage.FindGraphic ( pWord->Offset ) ;
         if ( pGraphic && ( pGraphic->Offset < CurrentPage.Query_Offset()+CurrentPage.Query_CurrentLength() ) ) {
            if ( pGraphic->Position.x == GRAPHIC_NEXT_CHAR ) {
               DisplayPS->Move ( pWord->Position ) ;
               DisplayPS->InvertCell ( pWord->Size ) ;
            } else {
               RECTL Rectangle ;
               pGraphic->ComputeBox ( CurrentPage.FindWord(pGraphic->Offset), Rectangle, PrinterResolution ) ;
               DisplayPS->InvertBox ( Rectangle ) ;
            } /* endif */
         } /* endif */
         Start += pWord->Length ;
         Palabra *pNext = pWord->NextWord ( ) ;
         if ( pNext ) 
            Position = pNext->Position ;

      } else if ( pWord->Dormant ) {                         // Dormant words.
         #ifdef DEBUG_INVERTTEXT
            Log ( "Documento::InvertText(%i->%i) Processing dormant token.", Offset1, Offset2 ) ;
         #endif
         InvertFlush ( DisplayPS, String, StringLength, OldStart, State, Position, Size ) ;
         Start += pWord->Length ;
         Palabra *pNext = pWord->NextWord ( ) ;
         if ( pNext ) 
            Position = pNext->Position ;

      } else if ( IsTab ( *pByte ) ) {                       // Tab tokens.
         #ifdef DEBUG_INVERTTEXT
            Log ( "Documento::InvertText(%i->%i) Processing tab token.", Offset1, Offset2 ) ;
         #endif
         InvertFlush ( DisplayPS, String, StringLength, OldStart, State, Position, Size ) ;
         Start ++ ;
         DisplayPS->Move ( pWord->Position ) ;
         DisplayPS->InvertCell ( pWord->Size ) ;
         Palabra *pNext = pWord->NextWord ( ) ;
         if ( pNext ) 
            Position = pNext->Position ;

      } else if ( IsSpace ( *pByte ) ) {                     // Space tokens.
         #ifdef DEBUG_INVERTTEXT
            Log ( "Documento::InvertText(%i->%i) Processing space token.", Offset1, Offset2 ) ;
         #endif
         Start ++ ;
         if ( State.Justify == JUSTIFY_FULL ) {
            InvertFlush ( DisplayPS, String, StringLength, OldStart, State, Position, Size ) ;
            DisplayPS->Move ( pWord->Position ) ;
            DisplayPS->InvertCell ( pWord->Size ) ;
            Palabra *pNext = pWord->NextWord ( ) ;
            if ( pNext ) 
               Position = pNext->Position ;
         } else {
            if ( StringLength+2 > sizeof(String) ) 
               InvertFlush ( DisplayPS, String, StringLength, OldStart, State, Position, Size ) ;
            String[StringLength++] = *pByte ;
         } /* endif */

      } else if ( IsSeparator ( pByte ) ) {                  // Separator tokens.
         #ifdef DEBUG_INVERTTEXT
            Log ( "Documento::InvertText(%i->%i) Processing separator token.", Offset1, Offset2 ) ;
         #endif
         InvertFlush ( DisplayPS, String, StringLength, OldStart, State, Position, Size ) ;
         long LengthIn ;
         ComputeFunctionSize ( pByte, LengthIn ) ;
         Start += LengthIn ;
         DisplayPS->Move ( pWord->Position ) ;
         DisplayPS->InvertCell ( pWord->Size ) ;
         Palabra *pNext = pWord->NextWord ( ) ;
         if ( pNext ) 
            Position = pNext->Position ;

      } else if ( IsGlyphFunction ( pByte ) ) {              // Glyph function tokens.
         #ifdef DEBUG_INVERTTEXT
            Log ( "Documento::InvertText(%i->%i) Processing function token.", Offset1, Offset2 ) ;
         #endif
         long LengthIn ;
         ComputeFunctionSize ( pByte, LengthIn ) ;
         Start += LengthIn ;
         char Text [100] ;
         switch ( *(pByte+1) ) {
            case FN__DATEFIXED: {
               FormatDate ( *PULONG(pByte+2), State.DateFormat, Text, sizeof(Text) ) ;
               if ( StringLength+strlen(Text)+1 > sizeof(String) ) 
                  InvertFlush ( DisplayPS, String, StringLength, OldStart, State, Position, Size ) ;
               strcpy ( String+StringLength, Text ) ;
               StringLength += strlen(Text) ;
               break; }
            case FN__DATECURRENT: {
               FormatDate ( time(0), State.DateFormat, Text, sizeof(Text) ) ;
               if ( StringLength+strlen(Text)+1 > sizeof(String) ) 
                  InvertFlush ( DisplayPS, String, StringLength, OldStart, State, Position, Size ) ;
               strcpy ( String+StringLength, Text ) ;
               StringLength += strlen(Text) ;
               break; }
            case FN__DATECHANGED: {
               FormatDate ( Query_CurrentLevel()?time(0):FileDate, State.DateFormat, Text, sizeof(Text) ) ;
               if ( StringLength+strlen(Text)+1 > sizeof(String) ) 
                  InvertFlush ( DisplayPS, String, StringLength, OldStart, State, Position, Size ) ;
               strcpy ( String+StringLength, Text ) ;
               StringLength += strlen(Text) ;
               break; }
            case FN__PAGENUMBER: {
               sprintf ( Text, "%i", PageNumber+1 ) ;
               if ( StringLength+strlen(Text)+1 > sizeof(String) ) 
                  InvertFlush ( DisplayPS, String, StringLength, OldStart, State, Position, Size ) ;
               strcpy ( String+StringLength, Text ) ;
               StringLength += strlen(Text) ;
               break; }
            case FN__PAGECOUNT: {
               sprintf ( Text, "%i", NumberOfPages ) ;
               if ( StringLength+strlen(Text)+1 > sizeof(String) ) 
                  InvertFlush ( DisplayPS, String, StringLength, OldStart, State, Position, Size ) ;
               strcpy ( String+StringLength, Text ) ;
               StringLength += strlen(Text) ;
               break; }
            case FN__GLYPH: {
               sprintf ( Text, "%c", *PUCHAR(pByte+2) ) ;
               if ( StringLength+strlen(Text)+1 > sizeof(String) ) 
                  InvertFlush ( DisplayPS, String, StringLength, OldStart, State, Position, Size ) ;
               strcpy ( String+StringLength, Text ) ;
               StringLength += strlen(Text) ;
               break; }
            case FN__SEPARATOR: {
               // Already handled above.
               break; }
         } /* endswitch */

      } else if ( IsGlyph ( pByte, PS->Query_DBCS_Vector() ) ) { // Normal glyphs.
         #ifdef DEBUG_INVERTTEXT
            Log ( "Documento::InvertText(%i->%i) Processing normal glyph.", Offset1, Offset2 ) ;
         #endif
         if ( StringLength+1 >= MAX_STRING ) 
            InvertFlush ( DisplayPS, String, StringLength, OldStart, State, Position, Size ) ;
         String[StringLength++] = *pByte ;
         Start ++ ;
         if ( PS->IsDBCSHeader ( *pByte ) ) {
            String[StringLength++] = *(pByte+1) ;
            Start ++ ;
         } /* endif */

      } else if ( IsFunction ( *pByte ) ) {          // Non-glyph function tokens.
         #ifdef DEBUG_INVERTTEXT
            Log ( "Documento::InvertText(%i->%i) Processing non-glyph function token.", Offset1, Offset2 ) ;
         #endif
         InvertFlush ( DisplayPS, String, StringLength, OldStart, State, Position, Size ) ;
         long LengthIn ;
         ComputeFunctionSize ( pByte, LengthIn ) ;
         Start += LengthIn ;
         State.ProcessFunction ( pByte, DisplayPS, PageNumber, NumberOfPages, Query_CurrentLevel()?time(0):FileDate ) ;
         ComputeState ( Start, State, Position, Size ) ;

      } else {                                      // EndLine, EndPage and EndDocument tokens.
         #ifdef DEBUG_INVERTTEXT
            Log ( "Documento::InvertText(%i->%i) Processing EndLine/EndPage/EndDocument token.", Offset1, Offset2 ) ;
         #endif
         InvertFlush ( DisplayPS, String, StringLength, OldStart, State, Position, Size ) ;
         Start ++ ;
         Palabra *pNext = pWord->NextWord ( ) ;
         if ( pNext ) 
            Position = pNext->Position ;

      } /* endif */

      // Get the pointer to the next token.
      pText->QueryBytePtr ( Start, pByte ) ;

   } /* endwhile */

   // If anything left, invert it now.
   #ifdef DEBUG_INVERTTEXT
      Log ( "Documento::InvertText(%i->%i) Flushing what text is left.  Start %i, Line %i->%i.", Offset1, Offset2, Start, pLine->Offset, pLine->Offset+pLine->Length-1 ) ;
   #endif
   InvertFlush ( DisplayPS, String, StringLength, OldStart, State, Position, Size ) ;

   // Unlock the display window, letting the painter back in.
   PainterObject->UnlockUpdate ( ) ;
   PainterObject->ShowCursor ( ) ;

   #ifdef DEBUG_INVERTTEXT
      Log ( "Documento::InvertText(%i->%i) Done.", Offset1, Offset2 ) ;
   #endif
}
 
/****************************************************************************
 *                                                                          *
 *  Documento: Determine if token is selected (part of Render).             *
 *                                                                          *
 ****************************************************************************/

BOOL Documento::IsSelected ( long TokenOffset ) {

   long Start = min ( OffsetOrigin, OffsetLast ) ;
   long Finish = max ( OffsetOrigin, OffsetLast ) ;

   if ( TokenOffset < Start )
      return ( FALSE ) ;

   if ( TokenOffset >= Finish )
      return ( FALSE ) ;

   return ( TRUE ) ;
}
 
/****************************************************************************
 *                                                                          *
 *  Documento: Extract selected text.                                       *
 *                                                                          *
 ****************************************************************************/

long Documento::Extract ( long BufferSize, char *Buffer, BOOL All ) {

   #ifdef DEBUG_EXTRACT
   Log ( "Documento::Extract(BufferSize=%i,Buffer=%p,All=%s): Started.", BufferSize, Buffer, All?"TRUE":"FALSE" ) ;
   #endif

   // Reset the total size of data extracted.
   long TotalSize = 0 ;

   // Determine start and finish offsets.
   long Start = min ( OffsetOrigin, OffsetLast ) ;
   long Finish = max ( OffsetOrigin, OffsetLast ) ;

   // Loop over the selected region.
   while ( Start < Finish ) {

      // Get a pointer to the current token.
      PUCHAR pByte ;
      pText->QueryBytePtr ( Start, pByte ) ;

      // If token is a function . . .
      if ( IsFunction ( *pByte ) ) {
         long LengthOut, LengthIn ;
         ComputeFunctionSize ( Anchor, PS, pByte, LengthOut, LengthIn ) ;
         if ( All ) {
            if ( Buffer AND ( TotalSize+LengthOut <= BufferSize ) ) {
               UCHAR Bytes [2] ;
               PUCHAR pDataOut = Bytes ;
               ExportToken ( Anchor, PS, pDataOut, LengthOut, pByte, LengthIn ) ;
               memcpy ( Buffer+TotalSize, pDataOut, size_t(LengthOut) ) ;
               TotalSize += LengthOut ;
               if ( LengthOut > 2 )
                  free ( pDataOut ) ;
            } else {
               TotalSize += LengthOut ;
            } /* endif */
         } /* endif */
         Start += LengthIn ;

      // Else if it is not a function . . .
      } else {
         // Copy out the first byte.
         if ( Buffer AND ( TotalSize+1 <= BufferSize ) ) {
            Buffer[TotalSize++] = *pByte ;
         } else {
            TotalSize ++ ;
         } /* endif */
         Start ++ ;
         // Copy out the second byte if DBCS.
         if ( PS->IsDBCSHeader ( *pByte ) ) {
            if ( Buffer AND ( TotalSize+1 <= BufferSize ) ) {
               Buffer[TotalSize++] = *(pByte+1) ;
            } else {
               TotalSize ++ ;
            } /* endif */
            Start ++ ;
         } /* endif */

      } /* endif */

   } /* endwhile */

   #ifdef DEBUG_EXTRACT
   Log ( "Documento::Extract(BufferSize=%i,Buffer=%p,All=%s): Done.  TotalSize %i.", BufferSize, Buffer, All?"TRUE":"FALSE", TotalSize ) ;
   #endif

   return ( TotalSize ) ;
}
 
/****************************************************************************
 *                                                                          *
 *  Documento: Extract selected text and the boundary states for it.        *
 *                                                                          *
 ****************************************************************************/

long Documento::Extract ( long BufferSize, char *Buffer, Estado &State1, Estado &State2 ) {

   // Extract the text itself.
   long TotalSize = Extract ( BufferSize, Buffer ) ;

   // Retrieve the initial state.
   long Start = min ( OffsetOrigin, OffsetLast ) ;
   ComputeState ( Start, State1 ) ;

   // Retrieve the final state.
   long Finish = max ( OffsetOrigin, OffsetLast ) ;
   ComputeState ( Finish, State2 ) ;

   // Return the text size.
   return ( TotalSize ) ;
}
 
/****************************************************************************
 *                                                                          *
 *  Documento: Determine size of extractable text.                          *
 *                                                                          *
 ****************************************************************************/

long Documento::Extract ( BOOL All ) {
   return ( Extract ( 0, 0, All ) ) ;
}

 
/****************************************************************************
 *                                                                          *
 *  Documento: Commence New Change Level                                    *
 *                                                                          *
 ****************************************************************************/

void Documento::CommenceLevel ( ) {
   LevelOffset = Offset ;
   pText->CommenceLevel ( ) ;
}
 
/****************************************************************************
 *                                                                          *
 *  Documento: Complete Change Level                                        *
 *                                                                          *
 ****************************************************************************/

void Documento::CompleteLevel ( ) {
   pText->CompleteLevel ( LevelOffset ) ;
}
 
/****************************************************************************
 *                                                                          *
 *  Documento: Delete selected text                                         *
 *                                                                          *
 *  ******** Public Queued Interface ********                               *
 *                                                                          *
 ****************************************************************************/

void Documento::Enqueue_Clear ( ) {
   SupervisorObject->EnqueueClear ( ) ;
}

  // Request executor.

void Documento::Dequeue_Clear ( ) {
   if ( NOT HasSelection() ) {                  // If nothing is selected,
      long Size = pText->QueryCurrentSize() ;   //   Get current size.
      if ( Offset >= Size-1 )                   //   If we're at EOF,
         return ;                               //     Get out of here.
      MoveTokenDown ( ) ;                       //   Move one token down.
      Select ( ) ;                              //   Select the token just passed.
   } /* endif */                                // End If
   CommenceLevel ( ) ;                          // Start edit level.
   Delete ( ) ;                                 // Delete selected area.
   CompleteLevel ( ) ;                          // Finish edit level.
}
 
/****************************************************************************
 *                                                                          *
 *  Documento: Delete selected text.                                        *
 *                                                                          *
 ****************************************************************************/

void Documento::Delete ( ) {

   // Determine start and finish offsets.
   long Start = min ( OffsetOrigin, OffsetLast ) ;
   long Finish = max ( OffsetOrigin, OffsetLast ) ;

   // Reset selection.
   OffsetOrigin = OffsetLast = Start ;

   // Delete from start to finish.
   Delete ( Start, Finish-Start ) ;
}
 
/****************************************************************************
 *                                                                          *
 *  Documento: Delete Specified Block                                       *
 *                                                                          *
 ****************************************************************************/

void Documento::Delete ( long Start, long Length ) {

   #ifdef DEBUG
   Log ( "Documento::Delete: Deleting text from offset %i for %i bytes.", Start, Length ) ;
   #endif

   // Update the text object and the current page.
   CurrentPage.Delete ( PS, PrinterResolution, Start, Length ) ;

   // Mark pages dirty.
   SetPagesDirty ( Start, Start ) ;

   // Update all offsets except the current page.
   long DummyLength ;
   DummyLength = 0 ; AdjustOffset ( Start, -Length, Offset, DummyLength, FALSE ) ;
   DummyLength = 0 ; AdjustOffset ( Start, -Length, OffsetLast, DummyLength, FALSE ) ;
   DummyLength = 0 ; AdjustOffset ( Start, -Length, OffsetOrigin, DummyLength, FALSE ) ;
   WorkingPage.AdjustOffsets ( Start, -Length ) ;
   for ( int i=0; i<=NumberOfPages; i++ )
      AdjustOffset ( Start, -Length, Pages[i].Offset, Pages[i].Length, FALSE ) ;
}
 
/****************************************************************************
 *                                                                          *
 *  Documento: Convert Text from Raw to Cooked Format                       *
 *                                                                          *
 ****************************************************************************/

long Documento::ImportText ( char *RawText, char* &CookedData ) {
   return ( ImportText ( RawText, strlen(RawText), CookedData ) ) ;
}

long Documento::ImportText ( char* RawText, int Length, char* &CookedData ) {

   // Optimize this a lot: If the raw text is two bytes or less, it cannot be a function token.
   if ( Length <= 2 ) {
      memcpy ( CookedData, RawText, Length ) ;
      return ( Length ) ;
   } /* endif */

   // Allocate space for the cooked data.  It has no size yet.
   CookedData = PCHAR ( malloc ( size_t ( Length ) ) ) ;
   long CookedSize = 0 ;

   // Prepare to process the raw data into cooked data.
   TEXTDATA TextData ;
   TextData.Buffer = PUCHAR(RawText) ;
   TextData.Index = 0 ;
   TextData.Size = Length ;

   UCHAR Bytes [2] ;
   PUCHAR pToken = Bytes ;
   long Size ;
   while ( GetTokenData ( pToken, Size, PREADER(ReadText), PREADER(PeekText), &TextData ) ) {

      // If not a function, simply store it into the cooked data.
      if ( Size <= 2 ) {
         memcpy ( CookedData+CookedSize, Bytes, size_t(Size) ) ;
         CookedSize += Size ;
         continue ;
      } /* endif */

      // Convert the function to internal format.
      PUCHAR ConvertedToken = PUCHAR ( malloc ( size_t ( Size ) ) ) ;
      long ConvertedSize ;  LONG Handle = 0 ;  int HandleType ;  int Version = REVISION_NO ;
      if ( ImportToken ( Anchor, ConvertedToken, ConvertedSize, pToken, Size, Handle, HandleType, Version ) == FALSE ) {
         free ( ConvertedToken ) ;
         free ( pToken ) ;
         break ;
      } /* endif */
      free ( pToken ) ;

      // Store the cooked function.
      memcpy ( CookedData+CookedSize, ConvertedToken, size_t(ConvertedSize) ) ;
      CookedSize += ConvertedSize ;
      free ( ConvertedToken ) ;

      // If there was a handle associated with it, store it now.
      if ( Handle )
         AddHandle ( Handle, HandleType ) ;

      // Prepare to get the next token.
      pToken = Bytes ;

   } /* endwhile */

   // Return the size of the cooked data.
   return ( CookedSize ) ;
}
 
/****************************************************************************
 *                                                                          *
 *  Documento: Insert Exported Text at the current offset.                  *
 *                                                                          *
 ****************************************************************************/

long Documento::Insert ( char *Text ) {
   return ( Insert ( Offset, Text, strlen(Text) ) ) ;
}

long Documento::Insert ( char *Text, int Length ) {
   return ( Insert ( Offset, Text, Length ) ) ;
}

long Documento::Insert ( int Offset, char *Text ) {
   return ( Insert ( Offset, Text, strlen(Text) ) ) ;
}

long Documento::Insert ( int offset, char *Text, int Length ) {

   #ifdef DEBUG_INSERT
      Log ( "Documento::Insert(offset=%i,Text='%0.*s') Started.  Importing text.", offset, Length>100?100:Length, Text ) ;
   #endif

   // Allocate space for the cooked data.  It has no size yet.
   char Bytes [2] ;
   char *CookedData = Bytes ;
   long CookedSize = ImportText ( Text, Length, CookedData ) ;

   // Insert the cooked data into the document.
   #ifdef DEBUG_INSERT
      Log ( "Documento::Insert(offset=%i,Text='%0.*s') Inserting cooked text.  CookedSize %i.", offset, Length>100?100:Length, Text, CookedSize ) ;
   #endif
   pText->Insert ( offset, CookedData, CookedSize ) ;

   // Mark pages dirty.
   #ifdef DEBUG_INSERT
      Log ( "Documento::Insert(offset=%i,Text='%0.*s') Marking dirty pages.", offset, Length>100?100:Length, Text ) ;
   #endif
   SetPagesDirty ( offset, offset ) ;

   // Release the cooked data.
   if ( CookedSize > 2 ) {
      #ifdef DEBUG_INSERT
         Log ( "Documento::Insert(offset=%i,Text='%0.*s') Releasing cooked text.", offset, Length>100?100:Length, Text ) ;
      #endif
      free ( CookedData ) ;
   } /* endif */

   // Mark first page in need of reformatting.
   #ifdef DEBUG_INSERT
      Log ( "Documento::Insert(offset=%i,Text='%0.*s') Marking pages for formatting.", offset, Length>100?100:Length, Text ) ;
   #endif
   for ( int i=0; i<NumberOfPages; i++ ) {
      // If start at or past start of this page, and last page or start before start of next page . . .
      if ( ( offset >= Pages[i].Offset ) AND ( (i>NumberOfPages-1) OR ( offset<Pages[i+1].Offset ) ) ) {
         Pages[i].NeedsFormat = TRUE ;
         break ;
      } /* endif */
   } /* endfor */

   // Update the current page.
   #ifdef DEBUG_INSERT
      Log ( "Documento::Insert(offset=%i,Text='%0.*s') Updating current page.", offset, Length>100?100:Length, Text ) ;
   #endif
   CurrentPage.Insert ( PS, offset, CookedSize ) ;

   // Update all offsets except the current page.
   #ifdef DEBUG_INSERT
      Log ( "Documento::Insert(offset=%i,Text='%0.*s') Adjusting offsets.", offset, Length>100?100:Length, Text ) ;
   #endif
   long DummyLength ;
   DummyLength = 0 ; AdjustOffset ( offset, CookedSize, Offset, DummyLength, FALSE ) ;
   DummyLength = 0 ; AdjustOffset ( offset, CookedSize, OffsetLast, DummyLength, FALSE ) ;
   DummyLength = 0 ; AdjustOffset ( offset, CookedSize, OffsetOrigin, DummyLength, FALSE ) ;
   WorkingPage.AdjustOffsets ( offset, CookedSize ) ;
   for ( i=0; i<=NumberOfPages; i++ )
      AdjustOffset ( offset, CookedSize, Pages[i].Offset, Pages[i].Length, FALSE ) ;

   // Return the count of inserted bytes.
   #ifdef DEBUG_INSERT
      Log ( "Documento::Insert: Done." ) ;
   #endif
   return ( CookedSize ) ;
}
 
/****************************************************************************
 *                                                                          *
 *  Documento: Insert Exported Text at the current offset and then advance  *
 *    the cursor past the inserted text.                                    *
 *                                                                          *
 ****************************************************************************/

void Documento::InsertBefore ( char *Text ) {
   // Insert the text and advance the cursor past it.
   Offset += Insert ( Text, strlen(Text) ) ;
   OffsetOrigin = OffsetLast = Offset ;
}

void Documento::InsertBefore ( char *Text, int Length ) {
   // Insert the text and advance the cursor past it.
   Offset += Insert ( Text, Length ) ;
   OffsetOrigin = OffsetLast = Offset ;
}
 
/****************************************************************************
 *                                                                          *
 *  Documento: Undo one change level, if possible.                          *
 *                                                                          *
 *  ******** Public Queued Interface ********                               *
 *                                                                          *
 ****************************************************************************/

void Documento::Enqueue_Undo ( ) {
   SupervisorObject->EnqueueUndo ( ) ;
}

  // Request executor.

void Documento::Dequeue_Undo ( ) {
   if ( pText->QueryCurrentLevel() == 0 )               // If level zero,
      return ;                                          //   Get out of here.
   pText->Undo ( ) ;                                    // Undo one level.
   long Start, End ;                                    // Mark document pages dirty.
   pText->QueryChangeRange ( pText->QueryCurrentLevel(), Start, End ) ;
   SetPagesDirty ( Start, End ) ;
   MoveDirect ( pText->QueryCurrentPosition() ) ;       // Move to new position.
   Deselect ( ) ;                                       // Reset selection.
   CurrentPageValid = FALSE ;                           // Mark current page invalid.
}

 
/****************************************************************************
 *                                                                          *
 *  Documento: Redo one change level, if possible.                          *
 *                                                                          *
 *  ******** Public Queued Interface ********                               *
 *                                                                          *
 ****************************************************************************/

void Documento::Enqueue_Redo ( ) {
   SupervisorObject->EnqueueRedo ( ) ;
}

  // Request executor.

void Documento::Dequeue_Redo ( ) {
   if ( pText->QueryCurrentLevel() >= pText->QueryMaxLevel() )  // If at or beyond max level,
      return ;                                                  //   Break out of here.
   pText->Redo ( ) ;                                            // Redo one level.
   long Start, End ;                                            // Mark document pages dirty.
   pText->QueryChangeRange ( pText->QueryCurrentLevel()-1, Start, End ) ;
   SetPagesDirty ( Start, End ) ;
   MoveDirect ( pText->QueryCurrentPosition() ) ;               // Move to new position.
   Deselect ( ) ;                                               // Reset selection.
   CurrentPageValid = FALSE ;                                   // Mark current page invalid.
}

 
/****************************************************************************
 *                                                                          *
 *  Documento: Create State Adjustment Text                                 *
 *                                                                          *
 ****************************************************************************/

void Documento::AdjustState ( Estado &OldState, Estado &NewState, ULONG Flags, char Text[], LONG TabStop ) {

   strcpy ( Text, "" ) ;

   if ( Flags & ADJUSTSTATE_LEFTMARGIN ) {
      if ( OldState.Margins.xLeft != NewState.Margins.xLeft ) {
         sprintf ( Text+strlen(Text), "%c%s%li%c", FUNCTION_START, FunctionNames[FN__LEFTMARGIN], NewState.Margins.xLeft, FUNCTION_END ) ;
      } /* endif */
   } /* endif */

   if ( Flags & ADJUSTSTATE_RIGHTMARGIN ) {
      if ( OldState.Margins.xRight != NewState.Margins.xRight ) {
         sprintf ( Text+strlen(Text), "%c%s%li%c", FUNCTION_START, FunctionNames[FN__RIGHTMARGIN], NewState.Margins.xRight, FUNCTION_END ) ;
      } /* endif */
   } /* endif */

   if ( Flags & ADJUSTSTATE_BOTTOMMARGIN ) {
      if ( OldState.Margins.yBottom != NewState.Margins.yBottom ) {
         sprintf ( Text+strlen(Text), "%c%s%li%c", FUNCTION_START, FunctionNames[FN__BOTTOMMARGIN], NewState.Margins.yBottom, FUNCTION_END ) ;
      } /* endif */
   } /* endif */

   if ( Flags & ADJUSTSTATE_TOPMARGIN ) {
      if ( OldState.Margins.yTop != NewState.Margins.yTop ) {
         sprintf ( Text+strlen(Text), "%c%s%li%c", FUNCTION_START, FunctionNames[FN__TOPMARGIN], NewState.Margins.yTop, FUNCTION_END ) ;
      } /* endif */
   } /* endif */

   if ( Flags & ADJUSTSTATE_TAB ) {
      if ( OldState.TabSet(TabStop) != NewState.TabSet(TabStop) ) {
         sprintf ( Text+strlen(Text), "%c%s%li%c", FUNCTION_START, NewState.TabSet(TabStop)?FunctionNames[FN__TABSET]:FunctionNames[FN__TABCLEAR], TabStop, FUNCTION_END ) ;
      } /* endif */
   } /* endif */

   if ( Flags & ADJUSTSTATE_TABS ) {
      int OldIndex = 0 ;
      int NewIndex = 0 ;
      while ( ( OldIndex < OldState.TabCount ) OR ( NewIndex < NewState.TabCount ) ) {
         if ( ( OldIndex < OldState.TabCount ) AND ( NewIndex < NewState.TabCount ) ) {
            if ( NewState.Tabs[NewIndex] == OldState.Tabs[OldIndex] ) {
               OldIndex ++ ;
               NewIndex ++ ;
               continue ;
            } /* endif */
            if ( NewState.Tabs[NewIndex] < OldState.Tabs[OldIndex] ) {
               sprintf ( PCHAR(Text)+strlen(PCHAR(Text)), "%c%s%li%c", FUNCTION_START, FunctionNames[FN__TABSET], NewState.Tabs[NewIndex], FUNCTION_END ) ;
               NewIndex ++ ;
               continue ;
            } /* endif */
            sprintf ( PCHAR(Text)+strlen(PCHAR(Text)), "%c%s%li%c", FUNCTION_START, FunctionNames[FN__TABCLEAR], OldState.Tabs[OldIndex], FUNCTION_END ) ;
            OldIndex ++ ;
            continue ;
         } /* endif */
         if ( OldIndex < OldState.TabCount ) {
            sprintf ( PCHAR(Text)+strlen(PCHAR(Text)), "%c%s%li%c", FUNCTION_START, FunctionNames[FN__TABCLEAR], OldState.Tabs[OldIndex], FUNCTION_END ) ;
            OldIndex ++ ;
            continue ;
         } /* endif */
         sprintf ( PCHAR(Text)+strlen(PCHAR(Text)), "%c%s%li%c", FUNCTION_START, FunctionNames[FN__TABSET], NewState.Tabs[NewIndex], FUNCTION_END ) ;
         NewIndex ++ ;
         continue ;
      } /* endwhile */
   } /* endif */

   if ( Flags & ADJUSTSTATE_INDENT ) {
      if ( OldState.Indent != NewState.Indent ) {
         sprintf ( Text+strlen(Text), "%c%s%i%c", FUNCTION_START, FunctionNames[FN__INDENT], NewState.Indent, FUNCTION_END ) ;
      } /* endif */
   } /* endif */

   if ( Flags & ADJUSTSTATE_LINESPACING ) {
      if ( OldState.LineSpacing != NewState.LineSpacing ) {
         sprintf ( Text+strlen(Text), "%c%s%i%c", FUNCTION_START, FunctionNames[FN__LINESPACING], NewState.LineSpacing, FUNCTION_END ) ;
      } /* endif */
   } /* endif */

   if ( Flags & ADJUSTSTATE_FONT ) {
      if ( strcmp ( OldState.Font, NewState.Font ) ) {
         sprintf ( Text+strlen(Text), "%c%s%s%c", FUNCTION_START, FunctionNames[FN__FONT], NewState.Font, FUNCTION_END ) ;
      } /* endif */
   } /* endif */

   if ( Flags & ADJUSTSTATE_SIZE ) {
      if ( OldState.Size != NewState.Size ) {
         sprintf ( Text+strlen(Text), "%c%s%.1lf%c", FUNCTION_START, FunctionNames[FN__SIZE], (double)NewState.Size/10.0, FUNCTION_END ) ;
      } /* endif */
   } /* endif */

   if ( Flags & ADJUSTSTATE_LINECOLOR ) {
      if ( OldState.Color != NewState.Color ) {
         sprintf ( Text+strlen(Text), "%c%s%06lX%c", FUNCTION_START, FunctionNames[FN__LINECOLOR], NewState.Color, FUNCTION_END ) ;
      } /* endif */
   } /* endif */

   if ( Flags & ADJUSTSTATE_BOLD ) {
      if ( OldState.Bold != NewState.Bold ) {
         sprintf ( Text+strlen(Text), "%c%s%i%c", FUNCTION_START, FunctionNames[FN__BOLD], NewState.Bold, FUNCTION_END ) ;
      } /* endif */
   } /* endif */

   if ( Flags & ADJUSTSTATE_ITALIC ) {
      if ( OldState.Italic != NewState.Italic ) {
         sprintf ( Text+strlen(Text), "%c%s%i%c", FUNCTION_START, FunctionNames[FN__ITALIC], NewState.Italic, FUNCTION_END ) ;
      } /* endif */
   } /* endif */

   if ( Flags & ADJUSTSTATE_UNDERSCORE ) {
      if ( OldState.Underscore != NewState.Underscore ) {
         sprintf ( Text+strlen(Text), "%c%s%i%c", FUNCTION_START, FunctionNames[FN__UNDERSCORE], NewState.Underscore, FUNCTION_END ) ;
      } /* endif */
   } /* endif */

   if ( Flags & ADJUSTSTATE_STRIKEOUT ) {
      if ( OldState.Strikeout != NewState.Strikeout ) {
         sprintf ( Text+strlen(Text), "%c%s%i%c", FUNCTION_START, FunctionNames[FN__STRIKEOUT], NewState.Strikeout, FUNCTION_END ) ;
      } /* endif */
   } /* endif */

   if ( Flags & ADJUSTSTATE_OUTLINE ) {
      if ( OldState.Outline != NewState.Outline ) {
         sprintf ( Text+strlen(Text), "%c%s%i%c", FUNCTION_START, FunctionNames[FN__OUTLINE], NewState.Outline, FUNCTION_END ) ;
      } /* endif */
   } /* endif */

   if ( Flags & ADJUSTSTATE_CAPS ) {
      if ( OldState.Caps != NewState.Caps ) {
         sprintf ( Text+strlen(Text), "%c%s%i%c", FUNCTION_START, FunctionNames[FN__CAPS], NewState.Caps, FUNCTION_END ) ;
      } /* endif */
   } /* endif */

   if ( Flags & ADJUSTSTATE_EXPAND ) {
      if ( OldState.Expand != NewState.Expand ) {
         sprintf ( Text+strlen(Text), "%c%s%i%c", FUNCTION_START, FunctionNames[FN__EXPAND], NewState.Expand, FUNCTION_END ) ;
      } /* endif */
   } /* endif */

   if ( Flags & ADJUSTSTATE_CENTERPAGE ) {
      if ( OldState.CenterPage != NewState.CenterPage ) {
         sprintf ( Text+strlen(Text), "%c%s%i%c", FUNCTION_START, FunctionNames[FN__CENTERPAGE], NewState.CenterPage, FUNCTION_END ) ;
      } /* endif */
   } /* endif */

   if ( Flags & ADJUSTSTATE_JUSTIFY ) {
      if ( OldState.Justify != NewState.Justify ) {
         sprintf ( Text+strlen(Text), "%c%s%c", FUNCTION_START,
            ( NewState.Justify == JUSTIFY_LEFT ) ? FunctionNames[FN__LEFTJUSTIFY] :
            ( NewState.Justify == JUSTIFY_CENTER ) ? FunctionNames[FN__CENTERJUSTIFY] :
            ( NewState.Justify == JUSTIFY_RIGHT ) ? FunctionNames[FN__RIGHTJUSTIFY] : 
            ( NewState.Justify == JUSTIFY_FULL ) ? FunctionNames[FN__FULLJUSTIFY] :
            FunctionNames[FN__LEFTJUSTIFY], FUNCTION_END ) ;
      } /* endif */
   } /* endif */

   if ( Flags & ADJUSTSTATE_DATEFORMAT ) {
      if ( strcmp ( OldState.DateFormat, NewState.DateFormat ) ) {
         sprintf ( Text+strlen(Text), "%c%s%s%c", FUNCTION_START, FunctionNames[FN__DATEFORMAT], NewState.DateFormat, FUNCTION_END ) ;
      } /* endif */
   } /* endif */

   if ( Flags & ADJUSTSTATE_FILLCOLOR ) {
      if ( OldState.FillColor != NewState.FillColor ) {
         sprintf ( Text+strlen(Text), "%c%s%06X%c", FUNCTION_START, FunctionNames[FN__FILLCOLOR], NewState.FillColor, FUNCTION_END ) ;
      } /* endif */
   } /* endif */

   if ( Flags & ADJUSTSTATE_LINEWIDTH ) {
      if ( OldState.LineWidth != NewState.LineWidth ) {
         sprintf ( Text+strlen(Text), "%c%s%i%c", FUNCTION_START, FunctionNames[FN__LINEWIDTH], NewState.LineWidth, FUNCTION_END ) ;
      } /* endif */
   } /* endif */

   if ( Flags & ADJUSTSTATE_LINEJOIN ) {
      if ( OldState.LineJoin != NewState.LineJoin ) {
         sprintf ( Text+strlen(Text), "%c%s%i%c", FUNCTION_START, FunctionNames[FN__LINEJOIN], NewState.LineJoin, FUNCTION_END ) ;
      } /* endif */
   } /* endif */

   if ( Flags & ADJUSTSTATE_LINEEND ) {
      if ( OldState.LineEnd != NewState.LineEnd ) {
         sprintf ( Text+strlen(Text), "%c%s%i%c", FUNCTION_START, FunctionNames[FN__LINEEND], NewState.LineEnd, FUNCTION_END ) ;
      } /* endif */
   } /* endif */

   if ( Flags & ADJUSTSTATE_FILLTYPE ) {
      if ( OldState.FillType != NewState.FillType ) {
         sprintf ( Text+strlen(Text), "%c%s%i%c", FUNCTION_START, FunctionNames[FN__FILLTYPE], NewState.FillType, FUNCTION_END ) ;
      } /* endif */
   } /* endif */

   if ( Flags & ADJUSTSTATE_HEADER ) {
      if ( ( OldState.Header[0] != NewState.Header[0] ) OR memcmp ( OldState.Header+1, NewState.Header+1, NewState.Header[0] ) ) {
         unsigned char *Start = new unsigned char [0x1000] ;
         sprintf ( Text+strlen(Text), "%c%s", FUNCTION_START, FunctionNames[FN__HEADER] ) ;
         unsigned char *End = ExportText ( Anchor, PS, Start, (unsigned char*)NewState.Header+1, (unsigned char)NewState.Header[0] ) ;
         Text[strlen(Text)+End-Start] = 0 ;  memcpy ( Text+strlen(Text), Start, End-Start ) ;
         sprintf ( Text+strlen(Text), "%c", FUNCTION_END ) ;
         delete Start ;
      } /* endif */
   } /* endif */

   if ( Flags & ADJUSTSTATE_FOOTER ) {
      if ( ( OldState.Footer[0] != NewState.Footer[0] ) OR memcmp ( OldState.Footer+1, NewState.Footer+1, NewState.Footer[0] ) ) {
         unsigned char *Start = new unsigned char [0x1000] ;
         sprintf ( Text+strlen(Text), "%c%s", FUNCTION_START, FunctionNames[FN__FOOTER] ) ;
         unsigned char *End = ExportText ( Anchor, PS, Start, (unsigned char*)NewState.Footer+1, (unsigned char)NewState.Footer[0] ) ;
         Text[strlen(Text)+End-Start] = 0 ;  memcpy ( Text+strlen(Text), Start, End-Start ) ;
         sprintf ( Text+strlen(Text), "%c", FUNCTION_END ) ;
         delete Start ;
      } /* endif */
   } /* endif */
}
 
/****************************************************************************
 *                                                                          *
 *  Documento: Set State over the currently selected area.                  *
 *                                                                          *
 ****************************************************************************/

void Documento::SetBlockState ( Estado &State, ULONG Flags, LONG TabStop ) {

   // Determine start and finish offsets.
   #ifdef DEBUG_SETBLOCKSTATE
      Log ( "Documento::SetBlockState() Started.  Determining start/finish offsets." ) ;
   #endif // DEBUG_SETBLOCKSTATE
   long Start = min ( OffsetOrigin, OffsetLast ) ;
   long Finish = max ( OffsetOrigin, OffsetLast ) ;
   long OldFinish = Finish ;
   BOOL OriginFirst = ( Start == OffsetOrigin ) ;

   // Obtain the starting and ending states.
   #ifdef DEBUG_SETBLOCKSTATE
      Log ( "Documento::SetBlockState() Determining start/finish states." ) ;
   #endif // DEBUG_SETBLOCKSTATE
   Estado State1 ;
   ComputeState ( Start, State1 ) ;
   Estado State2;
   ComputeState ( Finish, State2 ) ;

   // Insert the function codes necessary to terminate the block right.
   #ifdef DEBUG_SETBLOCKSTATE
      Log ( "Documento::SetBlockState() Computing finalization codes and inserting them." ) ;
   #endif // DEBUG_SETBLOCKSTATE
   char Text [500] ;
   AdjustState ( State, State2, Flags, Text ) ;
   if ( Text[0] )
      Finish += Insert ( int(Finish), Text ) ;

   // Loop over the selected region, clearing out interfering function codes.
   #ifdef DEBUG_SETBLOCKSTATE
      Log ( "Documento::SetBlockState() Clearing conflicting codes from block." ) ;
   #endif // DEBUG_SETBLOCKSTATE
   long CurrentOffset = OldFinish ;
   while ( CurrentOffset > Start ) {

      // Move back one token.
      CurrentOffset = MoveTokenUp ( CurrentOffset ) ;

      // Get a pointer to the current token.
      PUCHAR pByte ;
      pText->QueryBytePtr ( CurrentOffset, pByte ) ;

      // If token is a function . . .
      if ( IsFunction ( *pByte ) ) {

         long LengthIn ;
         ComputeFunctionSize ( pByte, LengthIn ) ;

         if ( Flags & ADJUSTSTATE_LEFTMARGIN ) {
            if ( *(pByte+1) == FN__LEFTMARGIN ) {
               Delete ( CurrentOffset, LengthIn ) ;
               Finish -= LengthIn ;
               continue ;
            } /* endif */
         } /* endif */

         if ( Flags & ADJUSTSTATE_RIGHTMARGIN ) {
            if ( *(pByte+1) == FN__RIGHTMARGIN ) {
               Delete ( CurrentOffset, LengthIn ) ;
               Finish -= LengthIn ;
               continue ;
            } /* endif */
         } /* endif */

         if ( Flags & ADJUSTSTATE_BOTTOMMARGIN ) {
            if ( *(pByte+1) == FN__BOTTOMMARGIN ) {
               Delete ( CurrentOffset, LengthIn ) ;
               Finish -= LengthIn ;
               continue ;
            } /* endif */
         } /* endif */

         if ( Flags & ADJUSTSTATE_TOPMARGIN ) {
            if ( *(pByte+1) == FN__TOPMARGIN ) {
               Delete ( CurrentOffset, LengthIn ) ;
               Finish -= LengthIn ;
               continue ;
            } /* endif */
         } /* endif */

         if ( Flags & ADJUSTSTATE_TAB ) {
            if ( ( *(pByte+1) == FN__TABSET ) OR ( *(pByte+1) == FN__TABCLEAR ) ) {
               if ( *PLONG(pByte+2) == TabStop ) {
                  Delete ( CurrentOffset, LengthIn ) ;
                  Finish -= LengthIn ;
                  continue ;
               } /* endif */
            } /* endif */
         } /* endif */

         if ( Flags & ADJUSTSTATE_TABS ) {
            if ( ( *(pByte+1) == FN__TABSET ) OR ( *(pByte+1) == FN__TABCLEAR ) ) {
               Delete ( CurrentOffset, LengthIn ) ;
               Finish -= LengthIn ;
               continue ;
            } /* endif */
         } /* endif */

         if ( Flags & ADJUSTSTATE_INDENT ) {
            if ( *(pByte+1) == FN__INDENT ) {
               Delete ( CurrentOffset, LengthIn ) ;
               Finish -= LengthIn ;
               continue ;
            } /* endif */
         } /* endif */

         if ( Flags & ADJUSTSTATE_LINESPACING ) {
            if ( *(pByte+1) == FN__LINESPACING ) {
               Delete ( CurrentOffset, LengthIn ) ;
               Finish -= LengthIn ;
               continue ;
            } /* endif */
         } /* endif */

         if ( Flags & ADJUSTSTATE_FONT ) {
            if ( *(pByte+1) == FN__FONT ) {
               Delete ( CurrentOffset, LengthIn ) ;
               Finish -= LengthIn ;
               continue ;
            } /* endif */
         } /* endif */

         if ( Flags & ADJUSTSTATE_SIZE ) {
            if ( *(pByte+1) == FN__SIZE ) {
               Delete ( CurrentOffset, LengthIn ) ;
               Finish -= LengthIn ;
               continue ;
            } /* endif */
         } /* endif */

         if ( Flags & ADJUSTSTATE_LINECOLOR ) {
            if ( *(pByte+1) == FN__LINECOLOR ) {
               Delete ( CurrentOffset, LengthIn ) ;
               Finish -= LengthIn ;
               continue ;
            } /* endif */
         } /* endif */

         if ( Flags & ADJUSTSTATE_BOLD ) {
            if ( *(pByte+1) == FN__BOLD ) {
               Delete ( CurrentOffset, LengthIn ) ;
               Finish -= LengthIn ;
               continue ;
            } /* endif */
         } /* endif */

         if ( Flags & ADJUSTSTATE_ITALIC ) {
            if ( *(pByte+1) == FN__ITALIC ) {
               Delete ( CurrentOffset, LengthIn ) ;
               Finish -= LengthIn ;
               continue ;
            } /* endif */
         } /* endif */

         if ( Flags & ADJUSTSTATE_UNDERSCORE ) {
            if ( *(pByte+1) == FN__UNDERSCORE ) {
               Delete ( CurrentOffset, LengthIn ) ;
               Finish -= LengthIn ;
               continue ;
            } /* endif */
         } /* endif */

         if ( Flags & ADJUSTSTATE_STRIKEOUT ) {
            if ( *(pByte+1) == FN__STRIKEOUT ) {
               Delete ( CurrentOffset, LengthIn ) ;
               Finish -= LengthIn ;
               continue ;
            } /* endif */
         } /* endif */

         if ( Flags & ADJUSTSTATE_OUTLINE ) {
            if ( *(pByte+1) == FN__OUTLINE ) {
               Delete ( CurrentOffset, LengthIn ) ;
               Finish -= LengthIn ;
               continue ;
            } /* endif */
         } /* endif */

         if ( Flags & ADJUSTSTATE_CAPS ) {
            if ( *(pByte+1) == FN__CAPS ) {
               Delete ( CurrentOffset, LengthIn ) ;
               Finish -= LengthIn ;
               continue ;
            } /* endif */
         } /* endif */

         if ( Flags & ADJUSTSTATE_EXPAND ) {
            if ( *(pByte+1) == FN__EXPAND ) {
               Delete ( CurrentOffset, LengthIn ) ;
               Finish -= LengthIn ;
               continue ;
            } /* endif */
         } /* endif */

         if ( Flags & ADJUSTSTATE_CENTERPAGE ) {
            if ( *(pByte+1) == FN__CENTERPAGE ) {
               Delete ( CurrentOffset, LengthIn ) ;
               Finish -= LengthIn ;
               continue ;
            } /* endif */
         } /* endif */

         if ( Flags & ADJUSTSTATE_JUSTIFY ) {
            if ( ( *(pByte+1) == FN__LEFTJUSTIFY ) OR ( *(pByte+1) == FN__CENTERJUSTIFY )
               OR ( *(pByte+1) == FN__RIGHTJUSTIFY ) OR ( *(pByte+1) == FN__FULLJUSTIFY ) ) {
               Delete ( CurrentOffset, LengthIn ) ;
               Finish -= LengthIn ;
               continue ;
            } /* endif */
         } /* endif */

         if ( Flags & ADJUSTSTATE_DATEFORMAT ) {
            if ( *(pByte+1) == FN__DATEFORMAT ) {
               Delete ( CurrentOffset, LengthIn ) ;
               Finish -= LengthIn ;
               continue ;
            } /* endif */
         } /* endif */

         if ( Flags & ADJUSTSTATE_FILLCOLOR ) {
            if ( *(pByte+1) == FN__FILLCOLOR ) {
               Delete ( CurrentOffset, LengthIn ) ;
               Finish -= LengthIn ;
               continue ;
            } /* endif */
         } /* endif */

         if ( Flags & ADJUSTSTATE_LINEWIDTH ) {
            if ( *(pByte+1) == FN__LINEWIDTH ) {
               Delete ( CurrentOffset, LengthIn ) ;
               Finish -= LengthIn ;
               continue ;
            } /* endif */
         } /* endif */

         if ( Flags & ADJUSTSTATE_LINEJOIN ) {
            if ( *(pByte+1) == FN__LINEJOIN ) {
               Delete ( CurrentOffset, LengthIn ) ;
               Finish -= LengthIn ;
               continue ;
            } /* endif */
         } /* endif */

         if ( Flags & ADJUSTSTATE_LINEEND ) {
            if ( *(pByte+1) == FN__LINEEND ) {
               Delete ( CurrentOffset, LengthIn ) ;
               Finish -= LengthIn ;
               continue ;
            } /* endif */
         } /* endif */

         if ( Flags & ADJUSTSTATE_FILLTYPE ) {
            if ( *(pByte+1) == FN__FILLTYPE ) {
               Delete ( CurrentOffset, LengthIn ) ;
               Finish -= LengthIn ;
               continue ;
            } /* endif */
         } /* endif */

         if ( Flags & ADJUSTSTATE_HEADER ) {
            if ( *(pByte+1) == FN__HEADER ) {
               Delete ( CurrentOffset, LengthIn ) ;
               Finish -= LengthIn ;
               continue ;
            } /* endif */
         } /* endif */

         if ( Flags & ADJUSTSTATE_FOOTER ) {
            if ( *(pByte+1) == FN__FOOTER ) {
               Delete ( CurrentOffset, LengthIn ) ;
               Finish -= LengthIn ;
               continue ;
            } /* endif */
         } /* endif */

      } /* endif */

   } /* endwhile */

   // Insert the function codes necessary to start the block off right.
   #ifdef DEBUG_SETBLOCKSTATE
      Log ( "Documento::SetBlockState() Inserting block start codes." ) ;
   #endif // DEBUG_SETBLOCKSTATE
   AdjustState ( State1, State, Flags, Text ) ;
   if ( Text[0] )
      Finish += Insert ( int(Start), Text ) ;

   // Set new selection range.
   #ifdef DEBUG_SETBLOCKSTATE
      Log ( "Documento::SetBlockState() Adjusting selection range to include new codes." ) ;
   #endif // DEBUG_SETBLOCKSTATE
   OffsetOrigin = OriginFirst ? Start : Finish ;
   OffsetLast = OriginFirst ? Finish : Start ;
   Offset = min ( Offset, OffsetLast ) ;

   #ifdef DEBUG_SETBLOCKSTATE
      Log ( "Documento::SetBlockState() Done." ) ;
   #endif // DEBUG_SETBLOCKSTATE
}
 
/****************************************************************************
 *                                                                          *
 *  Documento: Update Document Parameters                                   *
 *                                                                          *
 *  ******** Public Queued Interface ********                               *
 *                                                                          *
 ****************************************************************************/

void Documento::SetDefaults ( DocumentParms &docparms ) {

   #ifdef DEBUG_SETDEFAULTS
      Log ( "Documento::SetDefaults(Parms) Started." ) ;
   #endif

   FlushCommandQueue ( ) ;

   #ifdef DEBUG_SETDEFAULTS
      Log ( "Documento::SetDefaults(Parms) Command queue flushed.  About to commence edit." ) ;
   #endif

   CommenceEdit ( ) ;

   #ifdef DEBUG_SETDEFAULTS
      Log ( "Documento::SetDefaults(Parms) Edit commenced.  About to set document defaults." ) ;
   #endif

   // Determine if PS will need to be reset.
   BOOL ResetPS = ( DocParms.Metric != docparms.Metric ) || ( DocParms.CodePage != docparms.CodePage ) ;

   // Set the document defaults if they've changed.
   if ( memcmp ( &DocParms, &docparms, sizeof(DocParms) ) ) {
      fDocParms = TRUE ;
      if ( DocParms.Metric != docparms.Metric ) {
         long Offset = pText->QueryCurrentSize() - 1 ;
         while ( 1 ) { 
            PUCHAR pByte ;
            pText->QueryBytePtr ( Offset, pByte ) ;
            if ( IsGraphic ( pByte ) ) {
               PPOINTL pPosition = PPOINTL ( pByte + 2 ) ;
               PSIZEL pSize = PSIZEL ( pPosition + 1 ) ;
               if ( docparms.Metric ) {
                  pPosition->x = long ( double ( pPosition->x ) * 2.54 ) ;
                  pPosition->y = long ( double ( pPosition->y ) * 2.54 ) ;
                  pSize->cx = long ( double ( pSize->cx ) * 2.54 ) ;
                  pSize->cy = long ( double ( pSize->cy ) * 2.54 ) ;
               } else {
                  pPosition->x = long ( double ( pPosition->x ) / 2.54 ) ;
                  pPosition->y = long ( double ( pPosition->y ) / 2.54 ) ;
                  pSize->cx = long ( double ( pSize->cx ) / 2.54 ) ;
                  pSize->cy = long ( double ( pSize->cy ) / 2.54 ) ;
               } /* endif */
            } /* endif */
            if ( Offset ) 
               Offset = MoveTokenUp ( Offset ) ;
            else
               break ;
         } /* endwhile */
      } /* endif */
      DocParms = docparms ;
      SetDefaults ( ) ;
   } /* endif */

   #ifdef DEBUG_SETDEFAULTS
      Log ( "Documento::SetDefaults(Parms) Defaults set.  About to reset PS." ) ;
   #endif

   // If the PS needs resetting, do so now.
   if ( ResetPS ) 
      PS->Reset ( int(docparms.CodePage), int(docparms.Metric), int(State0.FormInfo.cx), int(State0.FormInfo.cy) ) ;

   #ifdef DEBUG_SETDEFAULTS
      Log ( "Documento::SetDefaults(Parms) PS reset.  About to query printer resolution." ) ;
   #endif

   // Get the current printer resolution.
   { /* startblock */
      QueueInfo Info ( "Documento::SetDefaults", 0, DocParms.Printer, DocParms.FormName, PDRIVDATA(DocParms.JobSettings) ) ;
      Info.QueryResolution ( Anchor, DocParms.Metric, PrinterResolution.x, PrinterResolution.y ) ;
   } /* endblock */

   #ifdef DEBUG_SETDEFAULTS
      Log ( "Documento::SetDefaults(Parms) Printer resolution %ix%i.  About to complete edit.", PrinterResolution.x, PrinterResolution.y ) ;
   #endif

   // Release the document from editing.
   CompleteEdit ( ) ;

   #ifdef DEBUG_SETDEFAULTS
      Log ( "Documento::SetDefaults(Parms) Done." ) ;
   #endif
}

void Documento::SetDefaults ( ) {

   // Use already-loaded parameters to set initial state.
   State0.Reset ( PS, DocParms, PageNumber, NumberOfPages, Query_CurrentLevel()?time(0):FileDate ) ;

   // Mark the first page Dirty.
   Pages[0].NeedsFormat = TRUE ;
   Pages[0].State = State0 ;
   Pages[0].Offset = 0 ;

   // Mark current page invalid, so that word list does not get reused.
   CurrentPageValid = FALSE ;

   // Set all pages dirty.
   SetPagesDirty ( 0, pText->QueryCurrentSize()-1 ) ;
}

 
/****************************************************************************
 *                                                                          *
 *  Documento: Toggle Insert Mode                                           *
 *                                                                          *
 *  ******** Public Queued Interface ********                               *
 *                                                                          *
 ****************************************************************************/

void Documento::Enqueue_InsertModeToggle ( ) {
   SupervisorObject->EnqueueInsertToggle ( ) ;
}

  // Request executor.

void Documento::Dequeue_InsertModeToggle ( ) {
   InsertMode = NOT InsertMode ;
   UpdateCursor ( ) ;
}
 
/****************************************************************************
 *                                                                          *
 *  Documento: Insert Key-code into Document                                *
 *                                                                          *
 *  ******** Public Queued Interface ********                               *
 *                                                                          *
 ****************************************************************************/

void Documento::Enqueue_InsertKey ( USHORT Key ) {
   SupervisorObject->EnqueueKey ( int(Key) ) ;
}

  // Request executor.

void Documento::Dequeue_InsertKey ( USHORT KeyCode ) {
   int Char1 = KeyCode & 0xFF ;                         // Get first byte.
   int Char2 = ( KeyCode >> 8 ) & 0xFF ;                // Get second byte.
   CommenceLevel ( ) ;                                  // Start edit level.
   if ( KeyCode == '\b' ) {                             // If backspace,
      MoveTokenUp ( ) ;                                 //   Move back one token.
      Select ( ) ;                                      //   Select that token.
      Delete ( ) ;                                      //   Delete it.
   } else {                                             // Else,
      if ( HasSelection ( ) )                           //   If anything's selected,
         Delete ( ) ;                                   //     Delete it.
      if ( Char2 )                                      //   If DBCS,
         InsertBefore ( PCHAR(&KeyCode), 2 ) ;          //     Insert both bytes before cursor.
      else                                              //   Else if not,
         InsertBefore ( PCHAR(&Char1), 1 ) ;            //     Insert byte before cursor.
      Deselect ( ) ;                                    //   Reset selection.
      if ( NOT Query_InsertMode() ) {                   //   If replace mode,
         MoveTokenDown ( ) ;                            //     Move forward one token.
         Select ( ) ;                                   //     Select that token.
         Delete ( ) ;                                   //     Delete it.
      } /* endif */                                     //   End if.
   } /* endif */                                        // End if.
   CompleteLevel ( ) ;                                  // Finish edit level.
}
 
/****************************************************************************
 *                                                                          *
 *  Documento: Insert Text into Document                                    *
 *                                                                          *
 *  ******** Public Queued Interface ********                               *
 *                                                                          *
 ****************************************************************************/

void Documento::Enqueue_InsertText ( char *Text, int Length ) {
   if ( Length ) {
      SupervisorObject->EnqueueString ( Text, Length ) ;
   } /* endif */
}

void Documento::Enqueue_InsertText ( char *Text ) {
   int Length = strlen(Text) ;
   Enqueue_InsertText ( Text, Length ) ;
}

  // Request executor.

void Documento::Dequeue_InsertText ( char *Text, int Length ) {
   CommenceLevel ( ) ;                                  // Start edit level.
   if ( HasSelection ( ) )                              // If anything's selected,
      Delete ( ) ;                                      //   Delete it.
   InsertBefore ( Text, Length ) ;                      // Insert string before cursor.
   Deselect ( ) ;                                       // Reset selection.
   CompleteLevel ( ) ;                                  // Finish edit level.
   free ( Text ) ;                                      // Release string memory.
}
 
/****************************************************************************
 *                                                                          *
 *  Documento: Insert State Change                                          *
 *                                                                          *
 *  ******** Public Queued Interface ********                               *
 *                                                                          *
 ****************************************************************************/

void Documento::InsertStateChange ( Estado &OldState, Estado &NewState, ULONG Flags, LONG TabStop ) {

   #ifdef DEBUG
      Log ( "Documento::InsertStateChange() starting." ) ;
   #endif

   FlushCommandQueue ( ) ;

   if ( HasSelection() ) {
      CommenceEdit ( ) ;
      CommenceLevel ( ) ;
      SetBlockState ( NewState, Flags, TabStop ) ;
      CompleteLevel ( ) ;
      CompleteEdit ( ) ;

   } else {
      char Text [500] ;
      AdjustState ( OldState, NewState, Flags, Text ) ;
      Enqueue_InsertText ( Text ) ;

   } /* endif */

   #ifdef DEBUG
      Log ( "Documento::InsertStateChange() done." ) ;
   #endif
}
 
/****************************************************************************
 *                                                                          *
 *  Documento: Obtain Window Size                                           *
 *                                                                          *
 ****************************************************************************/

inline void ComputeWindowSize ( HDC hDC, BOOL Metric, FIXED Zoom, double &ScaleX, double &ScaleY, RECTL &Rectangle, SIZEL &Size ) {

   LONG HorizontalResolution = 0, VerticalResolution = 0 ;
   DevQueryCaps ( hDC, CAPS_HORIZONTAL_RESOLUTION, 1, &HorizontalResolution ) ;
   DevQueryCaps ( hDC, CAPS_VERTICAL_RESOLUTION,   1, &VerticalResolution ) ;
   #ifdef DEBUG_TRANSFORM
      Log ( "ComputeWindowSize:: Resolution %i,%i pels/meter.", HorizontalResolution, VerticalResolution ) ;
   #endif

   double dZoom = double ( FIXEDINT ( Zoom ) ) + double ( FIXEDFRAC ( Zoom ) ) / 0x10000 ;
   ScaleX = ( double ( Metric ? 100000 : 39380 ) / HorizontalResolution ) / dZoom ;
   ScaleY = ( double ( Metric ? 100000 : 39380 ) / VerticalResolution ) / dZoom ;

   Size.cx = long ( ( Rectangle.xRight - Rectangle.xLeft ) * ScaleX + 0.4 ) ;
   Size.cy = long ( ( Rectangle.yTop - Rectangle.yBottom ) * ScaleY + 0.4 ) ;
}

void Documento::Query_WindowSize ( RECTL &Rectangle, SIZEL &WindowSize ) {

   #ifdef DEBUG_TRANSFORM
      Log ( "Documento::Query_WindowSize() starting.  TopLeft: %i,%i.", TopLeft.x, TopLeft.y ) ;
   #endif

   WinQueryWindowRect ( Window, &Rectangle ) ;

   #ifdef DEBUG_TRANSFORM
      Log ( "Documento::Query_WindowSize() Window rectangle %i,%i-%i,%i.",
         Rectangle.xLeft, Rectangle.yBottom, Rectangle.xRight, Rectangle.yTop ) ;
   #endif

   if ( ( Rectangle.xLeft == Rectangle.xRight ) OR ( Rectangle.yBottom == Rectangle.yTop ) ) {
      #ifdef DEBUG_TRANSFORM
         Log ( "Documento::Query_WindowSize() Window covers no area.  Returning null size." ) ;
      #endif
      Rectangle.xLeft = Rectangle.xRight = Rectangle.yBottom = Rectangle.yTop = 0 ;
      WindowSize.cx = WindowSize.cy = 0 ;
      return ;
   } /* endif */

   RECTL ScaledRectangle = Rectangle ;
   PS->Transform ( CVTC_DEVICE, CVTC_DEFAULTPAGE, ScaledRectangle ) ;
   #ifdef DEBUG_TRANSFORM
      Log ( "Documento::Query_WindowSize() Scaled window rectangle %i,%i-%i,%i (by default page transform).",
         ScaledRectangle.xLeft, ScaledRectangle.yBottom, ScaledRectangle.xRight, ScaledRectangle.yTop ) ;
   #endif

   HDC hDC ( PS->QueryDevice() ) ;
   HPS hPS ( 0 ) ;
   if ( !hDC ) {
      hPS = WinGetScreenPS ( HWND_DESKTOP ) ;
      hDC = GpiQueryDevice ( hPS ) ;
   } /* endif */

   double DeviceScaleX, DeviceScaleY ;
   ComputeWindowSize ( hDC, State0.Metric, Zoom, DeviceScaleX, DeviceScaleY, Rectangle, WindowSize ) ;

   if ( hPS ) 
      WinReleasePS ( hPS ) ;

   POINTL Offset = { TopLeft.x, TopLeft.y-WindowSize.cy } ;

   Rectangle.xLeft  = long ( Rectangle.xLeft * DeviceScaleX + 0.4 ) + Offset.x ;
   Rectangle.xRight = Rectangle.xLeft + WindowSize.cx ;

   Rectangle.yBottom = long ( Rectangle.yBottom * DeviceScaleY + 0.4 ) + Offset.y ;
   Rectangle.yTop = Rectangle.yBottom + WindowSize.cy ;

   #ifdef DEBUG_TRANSFORM
      Log ( "Documento::Query_WindowSize() Scaled window rectangle %i,%i-%i,%i.",
         Rectangle.xLeft, Rectangle.yBottom, Rectangle.xRight, Rectangle.yTop ) ;
      Log ( "Documento::Query_WindowSize() Window size: %ix%i.", WindowSize.cx, WindowSize.cy ) ;
      Log ( "Documento::Query_WindowSize() Device scale: %.4lf, %.4lf.", DeviceScaleX, DeviceScaleY ) ;
   #endif

   #ifdef DEBUG_TRANSFORM
      Log ( "Documento::Query_WindowSize() done." ) ;
   #endif
}

void Documento::Query_WindowSize ( SIZEL &WindowSize ) {

   #ifdef DEBUG_TRANSFORM
      Log ( "Documento::Query_WindowSize() starting.  TopLeft: %i,%i.", TopLeft.x, TopLeft.y ) ;
   #endif

   RECTL Rectangle ;
   WinQueryWindowRect ( Window, &Rectangle ) ;

   #ifdef DEBUG_TRANSFORM
      Log ( "Documento::Query_WindowSize() Window rectangle %i,%i-%i,%i.",
         Rectangle.xLeft, Rectangle.yBottom, Rectangle.xRight, Rectangle.yTop ) ;
   #endif

   if ( ( Rectangle.xLeft == Rectangle.xRight ) OR ( Rectangle.yBottom == Rectangle.yTop ) ) {
      #ifdef DEBUG_TRANSFORM
         Log ( "Documento::Query_WindowSize() Window covers no area.  Returning null size." ) ;
      #endif
      Rectangle.xLeft = Rectangle.xRight = Rectangle.yBottom = Rectangle.yTop = 0 ;
      WindowSize.cx = WindowSize.cy = 0 ;
      return ;
   } /* endif */

   RECTL ScaledRectangle = Rectangle ;
   PS->Transform ( CVTC_DEVICE, CVTC_DEFAULTPAGE, ScaledRectangle ) ;
   #ifdef DEBUG_TRANSFORM
      Log ( "Documento::Query_WindowSize() Scaled window rectangle %i,%i-%i,%i (by default page transform).",
         ScaledRectangle.xLeft, ScaledRectangle.yBottom, ScaledRectangle.xRight, ScaledRectangle.yTop ) ;
   #endif

   HDC hDC ( PS->QueryDevice() ) ;
   HPS hPS ( 0 ) ;
   if ( !hDC ) {
      hPS = WinGetScreenPS ( HWND_DESKTOP ) ;
      hDC = GpiQueryDevice ( hPS ) ;
   } /* endif */

   double DeviceScaleX, DeviceScaleY ;
   ComputeWindowSize ( hDC, State0.Metric, Zoom, DeviceScaleX, DeviceScaleY, Rectangle, WindowSize ) ;

   if ( hPS ) 
      WinReleasePS ( hPS ) ;

   #ifdef DEBUG_TRANSFORM
      Log ( "Documento::Query_WindowSize() Window size: %ix%i.", WindowSize.cx, WindowSize.cy ) ;
      Log ( "Documento::Query_WindowSize() Device scale: %.4lf, %.4lf.", DeviceScaleX, DeviceScaleY ) ;
   #endif

   #ifdef DEBUG_TRANSFORM
      Log ( "Documento::Query_WindowSize() done." ) ;
   #endif
}
 
/****************************************************************************
 *                                                                          *
 *  Documento: Find Text                                                    *
 *                                                                          *
 ****************************************************************************/

long Documento::Find ( char *String, long &EndingOffset, BOOL CaseSensitive, BOOL Regular ) {

   // Set initial search conditions.
   COUNTRYCODE Country = { 0, DocParms.CodePage } ;
   long StartingOffset = Offset ;
   long TextSize = pText->QueryCurrentSize() ;

   // Set up the GREPper, if required.
   cGREP2 *Grepper ( 0 ) ; 
   if ( Regular ) 
      Grepper = new cGREP2 ( String, pText ) ;

   // Loop until the text is found, or the end of file, whichever comes first.
   PUCHAR pByte ;
   while ( pText->QueryBytePtr ( StartingOffset, pByte ) ) {

      // Function codes are skipped.
      if ( IsFunction ( *pByte ) ) {
         StartingOffset = MoveTokenDown ( StartingOffset ) ;
         continue ;
      } /* endif */

      // Try to match the string.  NOTE: Search does -not- ignore imbedded function codes.
      if ( Regular ) {

         if ( Grepper->is_reg_exp_present ( (char*)StartingOffset ) ) {
            Grepper->matched_string ( StartingOffset, EndingOffset ) ;
            delete Grepper ;
            return ( StartingOffset ) ;
         } /* endif */

      } else {

         // Deal with plain text search.  
         EndingOffset = StartingOffset ;
         for ( int i=0; i<strlen(String); i++ ) {

            PUCHAR pByte ;
            if ( pText->QueryBytePtr ( EndingOffset, pByte ) == 0 ) 
               break ;

            if ( IsFunction ( *pByte ) ) 
               break;

            USHORT DocChar = *PUCHAR(pByte) ;
            if ( PS->IsDBCSHeader ( *pByte ) ) 
               DocChar |= *(pByte+1) << 8 ;
            else if ( NOT CaseSensitive ) 
               DosMapCase ( 1, &Country, PCHAR(&DocChar) ) ;

            USHORT TxtChar = *PUCHAR(String+i) ;
            if ( PS->IsDBCSHeader ( *(String+i) ) )
               TxtChar |= *(String+1) << 8 ;
            else if ( NOT CaseSensitive ) 
               DosMapCase ( 1, &Country, PCHAR(&TxtChar) ) ;

            if ( DocChar != TxtChar ) 
               break ;

            EndingOffset = MoveTokenDown ( EndingOffset ) ;

            if ( EndingOffset >= TextSize - 1 ) 
               break ;

         } /* endfor */

         if ( i >= strlen(String) ) 
            return ( StartingOffset ) ;

      } /* endif */

      StartingOffset = MoveTokenDown ( StartingOffset ) ;

      if ( StartingOffset >= TextSize - 1 ) 
         break ;

   } /* endwhile */

   // Return an error if we get this far.
   if ( Grepper ) delete Grepper ;
   EndingOffset = Offset ;
   return ( -1 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *  Documento: Find String                                                  *
 *                                                                          *
 ****************************************************************************/

BOOL Documento::FindStringNext ( char *String, BOOL CaseSensitive, BOOL Regular ) {

   #ifdef DEBUG_FIND
      Log ( "Documento::FindStringNext(\"%s\") Started.", String ) ;
   #endif // DEBUG_FIND

   if ( Offset >= pText->QueryCurrentSize()-1 ) {
      #ifdef DEBUG_FIND
         Log ( "Documento::FindStringNext: Aborted.  End of document reached." ) ;
      #endif // DEBUG_FIND
      return ( -1 ) ;
   } /* endif */

   #ifdef DEBUG_FIND
      Log ( "Documento::FindStringNext: Moving one token down." ) ;
   #endif // DEBUG_FIND
   Offset = MoveTokenDown ( Offset ) ;

   #ifdef DEBUG_FIND
      Log ( "Documento::FindStringNext: Searching." ) ;
   #endif // DEBUG_FIND
   if ( FindString ( String, CaseSensitive, Regular ) ) {
      #ifdef DEBUG_FIND
         Log ( "Documento::FindStringNext: String not found.  Moving one token up." ) ;
      #endif // DEBUG_FIND
      Offset = MoveTokenUp ( Offset ) ;
      return ( TRUE ) ;
   } /* endif */

   #ifdef DEBUG_FIND
      Log ( "Documento::FindStringNext: Done.  String found at %i.", Offset ) ;
   #endif // DEBUG_FIND
   return ( FALSE ) ;
}

BOOL Documento::FindString ( char *String, BOOL CaseSensitive, BOOL Regular ) {

   #ifdef DEBUG_FIND
      Log ( "Documento::FindString(\"%s\") Started.  Flushing the command queue.", String ) ;
   #endif // DEBUG_FIND

   // Flush the command queue.
   FlushCommandQueue ( ) ;

   // Find the string first.  Return error if not found.
   #ifdef DEBUG_FIND
      Log ( "Documento::FindString: Searching." ) ;
   #endif // DEBUG_FIND
   long EndingOffset ;
   long StartingOffset = Find ( String, EndingOffset, CaseSensitive, Regular ) ;
   if ( StartingOffset == -1 ) {
      #ifdef DEBUG_FIND
         Log ( "Documento::FindString: Done.  Nothing found." ) ;
      #endif // DEBUG_FIND
      return ( TRUE ) ;
   } /* endif */

   // Move to the ending offset.
   #ifdef DEBUG_FIND
      Log ( "Documento::FindString: Moving to string ending offset %i.", EndingOffset ) ;
   #endif // DEBUG_FIND
   MoveDirect ( EndingOffset, FALSE ) ;

   // Move to the starting offset, selecting.
   #ifdef DEBUG_FIND
      Log ( "Documento::FindString: Selecting to string starting offset %i.", StartingOffset ) ;
   #endif // DEBUG_FIND
   MoveDirect ( StartingOffset, TRUE ) ;

   #ifdef DEBUG_FIND
      Log ( "Documento::FindString: Done." ) ;
   #endif // DEBUG_FIND
   return ( FALSE ) ;
}
 
/****************************************************************************
 *                                                                          *
 *  Documento: Change String (first occurence)                              *
 *                                                                          *
 ****************************************************************************/

BOOL Documento::ChangeString ( char *OldString, char *NewString, BOOL CaseSensitive, BOOL Regular ) {

   // Find the string to be replaced.  Return error if not found.
   if ( FindString ( OldString, CaseSensitive, Regular ) )
      return ( TRUE ) ;

   // Replace the old string with the new.
   ReplaceSelectedText ( NewString ) ;

   // Return no error.
   return ( FALSE ) ;
}
 
/****************************************************************************
 *                                                                          *
 *  Documento: Change String (all occurences)                               *
 *                                                                          *
 ****************************************************************************/

BOOL Documento::ChangeAll ( char *OldString, char *NewString, BOOL CaseSensitive, BOOL Regular ) {

   #ifdef DEBUG_CHANGEALL
      Log ( "Documento::ChangeAll(\"%s\"->\"%s\") Started.", OldString, NewString ) ;
   #endif // DEBUG_CHANGEALL

   // Flush the command queue.
   FlushCommandQueue ( ) ;

   // Find the string's first occurence.  Return error if not found.
   #ifdef DEBUG_CHANGEALL
      Log ( "Documento::ChangeAll: Searching for old string." ) ;
   #endif // DEBUG_CHANGEALL
   long EndingOffset ;
   long StartingOffset = Find ( OldString, EndingOffset, CaseSensitive, Regular ) ;
   if ( StartingOffset == -1 ) {
      #ifdef DEBUG_CHANGEALL
         Log ( "Documento::ChangeAll: Done.  Old string not found." ) ;
      #endif // DEBUG_CHANGEALL
      return ( TRUE ) ;
   } /* endif */

   // Start the change level.
   #ifdef DEBUG_CHANGEALL
      Log ( "Documento::ChangeAll: Commencing edit." ) ;
   #endif // DEBUG_CHANGEALL
   CommenceEdit ( ) ;
   CommenceLevel ( ) ;

   // Replace every occurence until there aren't any more.
   do {

      // Get the starting and ending states of the selected area.
      #ifdef DEBUG_CHANGEALL
         Log ( "Documento::ChangeAll: Determining states at offsets %i & %i.", StartingOffset, EndingOffset ) ;
      #endif // DEBUG_CHANGEALL
      Estado State1 ; ComputeState ( StartingOffset, State1 ) ;
      Estado State2 ; ComputeState ( EndingOffset, State2 ) ;

      // If anything's selected, delete it.
      if ( StartingOffset != EndingOffset ) {
         #ifdef DEBUG_CHANGEALL
            Log ( "Documento::ChangeAll: Determining selected text." ) ;
         #endif // DEBUG_CHANGEALL
         Delete ( StartingOffset, EndingOffset - StartingOffset ) ;
      } /* endif */

      // If we have replacement text, insert it.
      if ( NewString AND *NewString ) {
         #ifdef DEBUG_CHANGEALL
            Log ( "Documento::ChangeAll: Inserting replacement text." ) ;
         #endif // DEBUG_CHANGEALL
         StartingOffset += Insert ( int(StartingOffset), NewString ) ;
      } /* endif */

      // Insert state change after replacement text.
      char Text [500] ;
      AdjustState ( State1, State2, 0xFFFFFFFF, Text ) ;
      if ( Text[0] ) {
         #ifdef DEBUG_CHANGEALL
            Log ( "Documento::ChangeAll: Inserting state change at %i.", StartingOffset ) ;
         #endif // DEBUG_CHANGEALL
         StartingOffset += Insert ( int(StartingOffset), Text ) ;
      } /* endif */

      // Find the next occurence
      #ifdef DEBUG_CHANGEALL
         Log ( "Documento::ChangeAll: Moving to new starting offset %i.", StartingOffset ) ;
      #endif // DEBUG_CHANGEALL
      MoveDirect ( StartingOffset ) ;
      #ifdef DEBUG_CHANGEALL
         Log ( "Documento::ChangeAll: Searching for old string." ) ;
      #endif // DEBUG_CHANGEALL
      StartingOffset = Find ( OldString, EndingOffset, CaseSensitive, Regular ) ;

      #ifdef DEBUG_CHANGEALL
         Log ( "Documento::ChangeAll: Search result %i.", StartingOffset ) ;
      #endif // DEBUG_CHANGEALL

   } while ( StartingOffset != -1 ); /* enddo */

   // End the change level.
   #ifdef DEBUG_CHANGEALL
      Log ( "Documento::ChangeAll: Completing edit level." ) ;
   #endif // DEBUG_CHANGEALL
   CompleteLevel ( ) ;
   CompleteEdit ( ) ;

   // Return no error.
   #ifdef DEBUG_CHANGEALL
      Log ( "Documento::ChangeAll: Done." ) ;
   #endif // DEBUG_CHANGEALL
   return ( FALSE ) ;
}
 
/****************************************************************************
 *                                                                          *
 *  Documento: Replace Selected Text                                        *
 *                                                                          *
 ****************************************************************************/

void Documento::ReplaceSelectedText ( char *String ) {

   #ifdef DEBUG_REPLACESELECTEDTEXT
      Log ( "Documento::ReplaceSelectedText(\"%s\") Started.  About to flush command queue.", String ) ;
   #endif

   // Flush the command queue.
   FlushCommandQueue ( ) ;

   // If we have text selected, or new text to insert . . .
   if ( HasSelection() OR ( String AND *String ) ) {

      // Start the change level.
      #ifdef DEBUG_REPLACESELECTEDTEXT
         Log ( "Documento::ReplaceSelectedText: About to commence edit." ) ;
      #endif
      CommenceEdit ( ) ;
      #ifdef DEBUG_REPLACESELECTEDTEXT
         Log ( "Documento::ReplaceSelectedText: About to commence level." ) ;
      #endif
      CommenceLevel ( ) ;

      // Get the starting and ending states of the selected area.
      #ifdef DEBUG_REPLACESELECTEDTEXT
         Log ( "Documento::ReplaceSelectedText: About to compute starting state." ) ;
      #endif
      long Start = min ( OffsetOrigin, OffsetLast ) ;
      Estado State1 ; ComputeState ( Start, State1 ) ;
      #ifdef DEBUG_REPLACESELECTEDTEXT
         Log ( "Documento::ReplaceSelectedText: About to compute ending state." ) ;
      #endif
      long Finish = max ( OffsetOrigin, OffsetLast ) ;
      Estado State2 ; ComputeState ( Finish, State2 ) ;

      // If anything's selected, delete it.
      #ifdef DEBUG_REPLACESELECTEDTEXT
         Log ( "Documento::ReplaceSelectedText: About to delete selected text." ) ;
      #endif
      if ( HasSelection() ) Delete ( ) ;

      // If we have replacement text, insert it.
      #ifdef DEBUG_REPLACESELECTEDTEXT
         Log ( "Documento::ReplaceSelectedText: About to insert replacement text before cursor." ) ;
      #endif
      if ( String AND *String ) InsertBefore ( String ) ;

      // Insert state change after replacement text.
      #ifdef DEBUG_REPLACESELECTEDTEXT
         Log ( "Documento::ReplaceSelectedText: About to adjust state." ) ;
      #endif
      char Text [500] ;
      AdjustState ( State1, State2, 0xFFFFFFFF, Text ) ;
      if ( Text[0] )
         InsertBefore ( Text ) ;

      // End the change level.
      #ifdef DEBUG_REPLACESELECTEDTEXT
         Log ( "Documento::ReplaceSelectedText: About to complete change level." ) ;
      #endif
      CompleteLevel ( ) ;
      #ifdef DEBUG_REPLACESELECTEDTEXT
         Log ( "Documento::ReplaceSelectedText: About to complete edit." ) ;
      #endif
      CompleteEdit ( ) ;

   } /* endif */

   #ifdef DEBUG_REPLACESELECTEDTEXT
      Log ( "Documento::ReplaceSelectedText: Done." ) ;
   #endif
}
 
/****************************************************************************
 *                                                                          *
 *  Documento: Spell Check - Find first misspelled word from current offset.*
 *     Return TRUE if one found.  Select the misspelled word and return     *
 *     the mispelled word and a list of possible replacements.              *
 *                                                                          *
 ****************************************************************************/

char* APIENTRY GetPtr ( void *Object, long *Offset, long Delta ) {

   Documento *pDocument = (Documento*) Object ;

   PUCHAR pByte ;
   if ( !(pDocument->Query_Text())->QueryBytePtr ( *Offset, pByte ) ) 
      return ( 0 ) ;

   while ( Delta ) {
      if ( Delta > 0 ) {
         if ( *Offset >= (pDocument->Query_Text())->QueryCurrentSize() - 1 ) 
            return ( 0 ) ;
         *Offset = pDocument->MoveTokenDown ( *Offset ) ;
         (pDocument->Query_Text())->QueryBytePtr ( *Offset, pByte ) ;
         if ( !IsFunction(*pByte) )
            Delta -- ;
      } else {
         if ( Offset == 0 ) 
            return ( 0 ) ;
         *Offset = pDocument->MoveTokenUp ( *Offset ) ;
         (pDocument->Query_Text())->QueryBytePtr ( *Offset, pByte ) ;
         if ( !IsFunction(*pByte) )
            Delta ++ ;
      } /* endif */
   } /* endwhile */

   return ( (char*) pByte ) ;
}

BOOL Documento::SpellCheck ( Dictionary *Speller, int, char Word[], int ListSize, int EntrySize, char List[], int &ListCount, int &EasyCount, BOOL First ) {

   #ifdef DEBUG_SPELLCHECK
      Log ( "Documento::SpellCheck: Starting.  At offset %i.", Offset ) ;
   #endif

   // If dictionary allocated and ready for use . . .
   if ( Speller AND Speller->Ready() ) {

      // Flush the command queue.
      #ifdef DEBUG_SPELLCHECK
         Log ( "Documento::SpellCheck: About to flush command queue." ) ;
      #endif
      FlushCommandQueue ( ) ;

      long StartingOffset = Offset ;
      long EndingOffset = StartingOffset ;
      long TextSize = pText->QueryCurrentSize() ;

      // If first scan, and cursor is in the middle of a word, back up to start of the current word.
      if ( First ) {
         PUCHAR pByte ;
         pText->QueryBytePtr ( StartingOffset, pByte ) ;
         if ( IsFunction(*pByte) OR NOT IsEndOfWord(pByte,PS->Query_DBCS_Vector()) ) {
            #ifdef DEBUG_SPELLCHECK
               Log ( "Documento::SpellCheck: About to rewind to start of current word." ) ;
            #endif
            while ( StartingOffset AND pText->QueryBytePtr ( StartingOffset, pByte ) ) {
               if ( !IsFunction(*pByte) AND IsEndOfWord(pByte,PS->Query_DBCS_Vector()) ) {
                  StartingOffset = MoveTokenDown ( StartingOffset ) ;
                  break ;
               } /* endif */
               StartingOffset = MoveTokenUp ( StartingOffset ) ;
            } /* endwhile */
         } /* endif */
      } /* endif */

      // Scan to the end of the document, building and checking words.
      #ifdef DEBUG_SPELLCHECK
         Log ( "Documento::SpellCheck: About to scan for misspelled words.  Offset %i.", StartingOffset ) ;
      #endif
      EndingOffset = StartingOffset ;
      while ( EndingOffset < TextSize-1 ) {
         #ifdef DEBUG_SPELLCHECK
            Log ( "Documento::SpellCheck: About to build word.  Offset %i.", StartingOffset ) ;
         #endif
         if ( !Speller->BuildWord ( GetPtr, this, Word, StartingOffset, EndingOffset ) ) 
            break ;
         #ifdef DEBUG_SPELLCHECK
            Log ( "Documento::SpellCheck: About to verify word '%s'.  Offset %i-%i.", Word, StartingOffset, EndingOffset ) ;
         #endif
         if ( !Speller->FindWord ( Word ) ) {
            ListCount = Speller->FindReplacements ( Word, ListSize, EntrySize, List, &EasyCount ) ;
            MoveDirect ( StartingOffset, FALSE ) ;
            #if defined(DEBUG_SPELLCHECK)
               Log ( "Documento::SpellCheck: Moving directly to offset %i, selecting.", EndingOffset ) ;
            #endif
            MoveDirect ( EndingOffset, TRUE ) ;
            #ifdef DEBUG_SPELLCHECK
               Log ( "Documento::SpellCheck: Done.  Possibly misspelled word '%s' found.", Word ) ;
            #endif
            return ( TRUE ) ;
         } /* endif */
         StartingOffset = EndingOffset ;
      } /* endwhile */
 
      #ifdef DEBUG_SPELLCHECK
         Log ( "Documento::SpellCheck: Done.  No more misspelled words found." ) ;
      #endif

      // Return nothing found, as no more mismatches were found.
      return ( FALSE ) ;

   } /* endif */

   // Return nothing found, as the dictionary was not ready for use.
   #ifdef DEBUG_SPELLCHECK
   Log ( "Documento::SpellCheck: Done.  ISpeller not present or not ready." ) ;
   #endif
   return ( FALSE ) ;
}
 
/****************************************************************************
 *                                                                          *
 *  Documento: Move Direct, updating the window.                            *
 *     Current page must be locked and the formatter halted before this     *
 *     function is called.                                                  *
 *                                                                          *
 ****************************************************************************/

void Documento::MoveDirect ( long offset, BOOL Select ) {

   Offset = offset ;

   if ( ( Offset < Pages[PageNumber].Offset ) OR ( Offset >= Pages[PageNumber].Offset+Pages[PageNumber].Length ) ) {

      HourGlass Hold ( Window ) ;

      SupervisorObject->Block ( ) ;

      for ( PageNumber=0; PageNumber<NumberOfPages; PageNumber ++ ) {
         if ( Offset < Pages[PageNumber].Offset )
            continue ;
         if ( Offset >= Pages[PageNumber].Offset+Pages[PageNumber].Length )
            continue ;
         break ;
      } /* endfor */

      if ( !FormatPage ( PageNumber ) )
         CurrentPage = WorkingPage ;
      CurrentPageValid = TRUE ;

      SupervisorObject->Unblock ( ) ;

      if ( Window ) Sys_InvalidateWindow ( Window ) ;

   } /* endif */

   FinishMove ( Select ) ;
}
 
/****************************************************************************
 *                                                                          *
 *  Documento: Format Page, waiting for the current page to complete.       *
 *                                                                          *
 ****************************************************************************/

void Documento::FormatPage ( ) {

   // Reformat the document, waiting for the current page to be ready.
   SupervisorObject->FormatPage ( TRUE ) ;

   // Finish up move processing.
   FinishMove ( FALSE ) ;
}
 
/****************************************************************************
 *                                                                          *
 *  Documento: Copy selected text to the system clipboard.                  *
 *                                                                          *
 ****************************************************************************/

void Documento::Copy ( ) {

   // Flush the command queue.
   FlushCommandQueue ( ) ;

   // If nothing is selected, return error.
   if ( NOT HasSelection() )
      return ;

   // If nothing can be extracted, return error.
   ULONG Size = Extract ( ) ;
   if ( Size == 0 ) 
      return ;

   // Allocate memory for extracted data.
   PCHAR Text = PCHAR ( malloc ( size_t ( Size ) ) ) ;
   if ( Text == 0 ) 
      return ;

   // Get complete copy of text.
   Estado InitialState, FinalState ;
   Size = Extract ( Size, Text, InitialState, FinalState ) ;

   // Allocate shared memory for the text.
   PUCHAR ClipText1 ;                      
   DosAllocSharedMem ( PPVOID(&ClipText1), 0, sizeof(Estado)*2+sizeof(ULONG)+Size+1,
      PAG_WRITE | PAG_COMMIT | OBJ_GIVEABLE ) ;

   // Copy complete text into shared memory.
   memcpy ( ClipText1, &InitialState, sizeof(Estado) ) ;
   memcpy ( ClipText1+sizeof(Estado), &FinalState, sizeof(Estado) ) ;
   memcpy ( ClipText1+sizeof(Estado)*2, &Size, sizeof(Size) ) ;
   memcpy ( ClipText1+sizeof(Estado)*2+sizeof(Size), Text, size_t(Size) ) ;
   ClipText1[sizeof(Estado)*2+sizeof(Size)+Size] = 0 ;

   // Release the local memory.
   free ( Text ) ;

   // Determine the size of text w/o imbedded function codes.
   Size = Extract ( FALSE ) ;
   PUCHAR ClipText2 = 0 ;

   // If there is anything to copy . . .
   if ( Size ) {
      // Allocate memory for the abridged text.
      Text = PCHAR ( malloc ( size_t ( ( Size * 5 ) / 4 ) ) ) ;
      // If successful,
      if ( Text ) {
         // Extract the abridged text.
         Size = Extract ( Size, Text, FALSE ) ;
         // Count the LF codes.
         int LFCount = 0 ;
         for ( int i=0; i<Size; i++ ) {
            if ( Text[i] == '\n' ) 
               LFCount ++ ;
         } /* endfor */
         // Allocate shared memory for it.
         DosAllocSharedMem ( PPVOID(&ClipText2), 0, Size+LFCount+1, PAG_WRITE | PAG_COMMIT | OBJ_GIVEABLE ) ;
         // Place the text there, converting all LFs to CR+LF.
         PUCHAR Source = PUCHAR ( Text ) ;
         PUCHAR Dest = ClipText2 ;
         for ( i=0; i<Size; i++ ) {
            if ( *Source == '\n' )
               *Dest++ = '\r' ;
            *Dest++ = *Source++ ;
         } /* endfor */
         // Null terminate it.
         *Dest = 0 ;
         // Release the local memory.
         free ( Text ) ;
      } /* endif */
   } /* endif */

   // Try to open the clipboard.  If unsuccessful, discard the shared memory and return an error.
   if ( !WinOpenClipbrd ( 0 ) ) {
      DosFreeMem ( ClipText1 ) ;
      DosFreeMem ( ClipText2 ) ;
      return ;
   } /* endif */

   // Clear the clipboard.
   WinEmptyClipbrd ( 0 ) ;

   // Place the shared memory blocks on the clipboard.
   WinSetClipbrdData ( 0, (ULONG)ClipText1, CF_DSPTEXT, CFI_POINTER ) ;
   WinSetClipbrdData ( 0, (ULONG)ClipText2, CF_TEXT, CFI_POINTER ) ;

   // Close the clipboard.
   WinCloseClipbrd ( 0 ) ;

   // Done w/o error.
   return ;
}
 
/****************************************************************************
 *                                                                          *
 *  Documento: Delete selected text, then paste from the system clipboard.  *
 *                                                                          *
 *  ******** Public Queued Interface ********                               *
 *                                                                          *
 ****************************************************************************/

void Documento::Enqueue_Paste ( ) {
   SupervisorObject->EnqueuePaste ( ) ;
}

  // Request executor.

void Documento::Dequeue_Paste ( ) {
   CommenceLevel ( ) ;                          // Start edit level.
   if ( HasSelection() )                        // If something is selected,
      Delete ( ) ;                              //   Delete selected area.
   Paste ( ) ;                                  // Paste from the clipboard.
   CompleteLevel ( ) ;                          // Finish edit level.
}

 
/****************************************************************************
 *                                                                          *
 *  Documento: Paste text from the system clipboard.                        *
 *                                                                          *
 ****************************************************************************/

int Documento::Paste ( ) {

   Estado InitialState, FinalState ;
   BOOL StateValid = FALSE ;
   char *pText = 0 ;
   ULONG Size = 0 ;

   if ( !WinOpenClipbrd ( 0 ) ) 
      return ( 1 ) ;

   char *ClipText = PCHAR ( WinQueryClipbrdData ( 0, CF_DSPTEXT ) ) ;
   if ( ClipText ) {
      StateValid = TRUE ;
      memcpy ( &InitialState, ClipText, sizeof(Estado) ) ;
      memcpy ( &FinalState, ClipText+sizeof(Estado), sizeof(Estado) ) ;
      memcpy ( &Size, ClipText+sizeof(Estado)*2, sizeof(Size) ) ;
      pText = PCHAR ( malloc ( size_t ( Size + 1 ) ) ) ;
      memcpy ( pText, ClipText+sizeof(Estado)*2+sizeof(Size), size_t(Size+1) ) ;
   } else {
      ClipText = PCHAR ( WinQueryClipbrdData ( 0, CF_TEXT ) ) ;
      if ( ClipText ) {
         pText = PCHAR ( malloc ( size_t ( strlen(ClipText)+1 ) ) ) ;
         strcpy ( pText, ClipText ) ;
         Size = strlen(pText) ;
      } /* endif */
   } /* endif */

   WinCloseClipbrd ( 0 ) ;

   if ( pText == 0 ) 
      return ( 2 ) ;

   Estado CurrentState = State ;

   if ( StateValid ) {
      char Text [500] ;
      AdjustState ( FinalState, CurrentState, 0xFFFFFFFF, Text ) ;
      if ( Text[0] ) 
         Insert ( Text, strlen(Text) ) ;
   } /* endif */

   Insert ( pText, int(Size) ) ;

   if ( StateValid ) {
      char Text [500] ;
      AdjustState ( CurrentState, InitialState, 0xFFFFFFFF, Text ) ;
      if ( Text[0] ) 
         Insert ( Text, strlen(Text) ) ;
   } /* endif */

   free ( pText ) ;

   return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *  Documento: Merge File after the cursor.                                 *
 *                                                                          *
 *  ******** Public Queued Interface ********                               *
 *                                                                          *
 ****************************************************************************/

void Documento::MergeFile ( char *filename, DocumentParms &docparms, Addon *Importer ) {

   char *RawData = 0 ;
   long Size = 0 ;
   long Start = 0 ;
   int Version = 0 ;

   if ( Importer ) {

      #ifdef DEBUG_MERGE
         Log ( "Documento::MergeFile(filename='%s',...) Attempting to import text with importer '%s'.", 
            filename, Importer->QueryName() ) ;
      #endif // DEBUG_MERGE

      if ( ! strcmpi ( Importer->QueryType(), TYPE_IMPORTEXPORT ) ) {
         RawData = PCHAR ( ((ImportExport*)Importer)->Import ( 0, PCHAR(filename), &Size ) ) ;
      } else if ( ! strcmpi ( Importer->QueryType(), TYPE_GRAPHICIMPORT ) ) {
         RawData = PCHAR ( ((GraphicImporter*)Importer)->Import ( 0, PCHAR(filename), &Size ) ) ;
      } else {
         RawData = PCHAR ( ((MetafileImporter*)Importer)->Import ( 0, PCHAR(filename), &Size ) ) ;
      } /* endif */

      #ifdef DEBUG_MERGE
         Log ( "Documento::MergeFile(filename='%s',...) %i bytes were imported.", filename, Size ) ;
      #endif // DEBUG_MERGE

      if ( ( RawData == 0 ) OR ( Size == 0 ) ) 
         return ;

      // Process the document preface, if there is one.
      Start = ReadPreface ( RawData, (int)Size, docparms, Version ) ;
      Size -= Start ;

      #ifdef DEBUG_MERGE
         Log ( "Documento::MergeFile(filename='%s',...) The preface had %i bytes.", filename, Start ) ;
      #endif // DEBUG_MERGE

   } else {

      // Open the file.
      FILE *File = fopen ( PCHAR(filename), "rb" ) ;
      if ( File == 0 ) 
         return ;

      // Process the document preface, if there is one.
      Start = ReadPreface ( fileno(File), docparms, Version ) ;

      // Read the remaining data into a raw data buffer.
      fseek ( File, 0, SEEK_END ) ;
      Size = ftell ( File ) - Start ;
      fseek ( File, Start, SEEK_SET ) ;
      PCHAR RawData = PCHAR ( malloc ( size_t ( Size ) ) ) ;
      fread ( RawData, 1, size_t(Size), File ) ;
      fclose ( File ) ;

      // Reset offset from start of raw data to start importing from.
      Start = 0 ;

   } /* endif */

   #ifdef DEBUG_MERGE
      Log ( "Documento::MergeFile(filename='%s',...) %i bytes remain to merge.", filename, Size ) ;
   #endif // DEBUG_MERGE

   // If no data remains to merge, just return.
   if ( Size <= 0 ) 
      return ;

   // Flush the command queue.
   #ifdef DEBUG_MERGE
      Log ( "Documento::MergeFile(filename='%s',...) Flushing command queue.", filename ) ;
   #endif // DEBUG_MERGE
   FlushCommandQueue ( ) ;

   // Request edit mode.
   #ifdef DEBUG_MERGE
      Log ( "Documento::MergeFile(filename='%s',...) Commencing edit.", filename ) ;
   #endif // DEBUG_MERGE
   CommenceEdit ( ) ;

   // Start a new change level.
   #ifdef DEBUG_MERGE
      Log ( "Documento::MergeFile(filename='%s',...) Commencing level.", filename ) ;
   #endif // DEBUG_MERGE
   CommenceLevel ( ) ;

   // Save the state at the insertion point.
   Estado OriginalState = State ;
   long OriginalOffset = Offset ;

   // Reset the current page.
   CurrentPage.Reset ( *CurrentPage.Query_State() ) ;
   CurrentPageValid = FALSE ;

   // Insert the data at the current offset.
   #ifdef DEBUG_MERGE
      Log ( "Documento::MergeFile(filename='%s',...) Inserting file.", filename ) ;
   #endif // DEBUG_MERGE
   int InsertedSize = int ( Insert ( RawData+Start, int(Size) ) ) ;

   // Insert function codes to adjust the state from the original state to the merged's initial state.
   #ifdef DEBUG_MERGE
      Log ( "Documento::MergeFile(filename='%s',...) Inserting initial state adjustors.", filename ) ;
   #endif // DEBUG_MERGE
   Estado InitialState ;  InitialState.Reset ( PS, docparms, PageNumber, NumberOfPages, Query_CurrentLevel()?time(0):FileDate ) ;
   char Text [500] ;
   AdjustState ( OriginalState, InitialState, 0xFFFFFFFF, Text ) ;
   if ( Text[0] ) 
      InsertedSize += int ( Insert ( Text ) ) ;

   // Insert function codes to adjust the state from the merged's final state to the original state.
   #ifdef DEBUG_MERGE
      Log ( "Documento::MergeFile(filename='%s',...) Inserting final state adjustors.", filename ) ;
   #endif // DEBUG_MERGE
   Estado FinalState ;
   ComputeNextState ( PS, pText, OriginalOffset, InsertedSize, OriginalState, FinalState, PageNumber, NumberOfPages, Query_CurrentLevel()?time(0):FileDate ) ;
   AdjustState ( FinalState, OriginalState, 0xFFFFFFFF, Text ) ;
   if ( Text[0] ) 
      Insert ( int(OriginalOffset+InsertedSize), Text ) ;

   // Finish the change level.
   #ifdef DEBUG_MERGE
      Log ( "Documento::MergeFile(filename='%s',...) Completing level.", filename ) ;
   #endif // DEBUG_MERGE
   CompleteLevel ( ) ;

   // Release edit mode.
   CompleteEdit ( ) ;
   #ifdef DEBUG_MERGE
      Log ( "Documento::MergeFile(filename='%s',...) Completing edit.", filename ) ;
   #endif // DEBUG_MERGE

   // Format the document.
   SupervisorObject->FormatPage ( TRUE ) ;
}
 
/****************************************************************************
 *                                                                          *
 *  Documento: Set Tab                                                      *
 *                                                                          *
 *  ******** Public Queued Interface ********                               *
 *                                                                          *
 ****************************************************************************/

void Documento::SetTab ( int TabStop ) {

   // Flush the command queue.
   FlushCommandQueue ( ) ;

   if ( HasSelection() ) {
      Estado NewState ;
      NewState.TabCount = 1 ;
      NewState.Tabs[0] = TabStop ;
      CommenceEdit ( ) ;
      CommenceLevel ( ) ;
      SetBlockState ( NewState, ADJUSTSTATE_TAB, TabStop ) ;
      CompleteLevel ( ) ;
      CompleteEdit ( ) ;

   } else {
      if ( NOT Query_State()->TabSet ( TabStop ) ) {
         char Text [80] ;
         sprintf ( Text, "%c%s%lu%c", FUNCTION_START, FunctionNames[FN__TABSET], TabStop, FUNCTION_END ) ;
         Enqueue_InsertText ( Text ) ;
      } /* endif */

   } /* endif */
}
 
/****************************************************************************
 *                                                                          *
 *  Documento: Clear Tab                                                    *
 *                                                                          *
 *  ******** Public Queued Interface ********                               *
 *                                                                          *
 ****************************************************************************/

void Documento::ClearTab ( int TabStop ) {

   // Flush the command queue.
   FlushCommandQueue ( ) ;

   if ( HasSelection() ) {
      Estado NewState ;
      NewState.TabCount = 0 ;
      CommenceEdit ( ) ;
      CommenceLevel ( ) ;
      SetBlockState ( NewState, ADJUSTSTATE_TAB, TabStop ) ;
      CompleteLevel ( ) ;
      CompleteEdit ( ) ;

   } else {
      if ( Query_State()->TabSet ( TabStop ) ) {
         char Text [80] ;
         sprintf ( Text, "%c%s%lu%c", FUNCTION_START, FunctionNames[FN__TABCLEAR], TabStop, FUNCTION_END ) ;
         Enqueue_InsertText ( Text ) ;
      } /* endif */

   } /* endif */
}
 
/****************************************************************************
 *                                                                          *
 *  Documento: Move Tab                                                     *
 *                                                                          *
 *  ******** Public Queued Interface ********                               *
 *                                                                          *
 ****************************************************************************/

void Documento::MoveTab ( int OldTabStop, int NewTabStop ) {

   // Flush the command queue.
   FlushCommandQueue ( ) ;

   if ( HasSelection() ) {
      Estado NewState ;
      NewState.TabCount = 0 ;
      CommenceEdit ( ) ;
      CommenceLevel ( ) ;
      SetBlockState ( NewState, ADJUSTSTATE_TAB, OldTabStop ) ;
      NewState.TabCount = 1 ;
      NewState.Tabs[0] = NewTabStop ;
      SetBlockState ( NewState, ADJUSTSTATE_TAB, NewTabStop ) ;
      CompleteLevel ( ) ;
      CompleteEdit ( ) ;

   } else {
      char Text [80] ;
      sprintf ( Text, "%c%s%lu%c", FUNCTION_START, FunctionNames[FN__TABCLEAR], OldTabStop, FUNCTION_END ) ;
      sprintf ( Text+strlen(Text), "%c%s%lu%c", FUNCTION_START, FunctionNames[FN__TABSET], NewTabStop, FUNCTION_END ) ;
      Enqueue_InsertText ( Text ) ;

   } /* endif */
}
 
/****************************************************************************
 *                                                                          *
 *  Documento: Move/Copy Text                                               *
 *                                                                          *
 *  Text from OffsetOrigin to OffsetLast is moved to Offset.                *
 *                                                                          *
 *  ******** Public Queued Interface ********                               *
 *                                                                          *
 ****************************************************************************/

void Documento::MoveText ( int Copying ) {

   // Flush the command queue.
   FlushCommandQueue ( ) ;

   // Find out how large the selected area is.
   ULONG Size = Extract ( ) ;
   if ( Size == 0 ) 
      return ;

   // Allocate memory to hold it.
   PCHAR pText = PCHAR ( malloc ( size_t ( Size ) ) ) ;
   if ( pText == 0 ) 
      return ;

   // Clip the selected text and its boundary states.
   Estado InitialState, FinalState ;
   Size = Extract ( Size, pText, InitialState, FinalState ) ;

   // Here we start making changes.
   CommenceEdit ( ) ;
   CommenceLevel ( ) ;

   // If moving, delete the selected area.
   if ( NOT Copying ) 
      Delete ( ) ;

   // Insert final state adjustments.
   char Text [500] ;
   AdjustState ( FinalState, State, 0xFFFFFFFF, Text ) ;
   if ( Text[0] ) 
      Insert ( Text ) ;

   // Insert text.
   Insert ( pText, int(Size) ) ;

   // Insert initial state adjustments.
   AdjustState ( State, InitialState, 0xFFFFFFFF, Text ) ;
   if ( Text[0] ) 
      Insert ( Text ) ;

   // Now we're done making changes.
   CompleteLevel ( ) ;
   CompleteEdit ( ) ;

   // Free the memory allocated to hold the text.
   free ( pText ) ;
}
 
/****************************************************************************
 *                                                                          *
 *  Documento: Update Graphic                                               *
 *                                                                          *
 *  ******** Public Queued Interface ********                               *
 *                                                                          *
 ****************************************************************************/

void Documento::UpdateGraphic ( Grafico &Graphic ) {

   // Flush the command queue.
   FlushCommandQueue ( ) ;

   // Construct the new function token.
   char Text [200] ;
   CreateGraphicToken ( Text, Graphic ) ;

   // Update the document.
   CommenceEdit ( ) ;
   CommenceLevel ( ) ;
   MoveDirect ( Graphic.Offset ) ;
   Deselect ( ) ;
   MoveTokenDown ( ) ;
   Select ( ) ;
   Delete ( ) ;
   Insert ( Text ) ;
   CompleteLevel ( ) ;
   CompleteEdit ( ) ;
}
 
/****************************************************************************
 *                                                                          *
 *  Documento: Copy Graphic                                                 *
 *                                                                          *
 *  ******** Public Queued Interface ********                               *
 *                                                                          *
 ****************************************************************************/

void Documento::CopyGraphic ( Grafico &Graphic ) {

   // Flush the command queue.
   FlushCommandQueue ( ) ;

   // Construct the new function token.
   char Text [200] ;
   CreateGraphicToken ( Text, Graphic ) ;

   // Update the document.
   CommenceEdit ( ) ;
   CommenceLevel ( ) ;
   MoveDirect ( Graphic.Offset ) ;
   MoveTokenDown ( ) ;
   Deselect ( ) ;
   Insert ( Text ) ;
   CompleteLevel ( ) ;
   CompleteEdit ( ) ;
}
 
/****************************************************************************
 *                                                                          *
 *  Documento: Flush Command Queue, completing all pending edits, though    *
 *    not necessarily all pending formatting.                               *
 *                                                                          *
 *  ******** Public Queued Interface ********                               *
 *                                                                          *
 ****************************************************************************/

void Documento::FlushCommandQueue ( ) {

   if ( SupervisorObject ) 
      SupervisorObject->FlushCommandQueue ( ) ;

} /* endmethod */

/****************************************************************************
 *                                                                          *
 *  Documento: Determine page number given document offset.                 *
 *                                                                          *
 ****************************************************************************/

int Documento::GetPageFromOffset ( long Offset ) {

   int PageNumber ( 0 ) ;

   for ( int i=1; i<NumberOfPages; i++, PageNumber++ ) 
      if ( Pages[i].Offset >= Offset ) 
         break ;

   return ( PageNumber ) ;

} /* endmethod */

