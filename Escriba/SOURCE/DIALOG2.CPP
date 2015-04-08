/**************************************************************** DIALOG2.CPP
 *                                                                          *
 *                         Subclassed Basic Dialog                          *
 *                                                                          *
 ****************************************************************************/

#include "System.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Addons.h"
#include "Debug.h"
#include "Dialog2.h"
#include "Document.h"
#include "EAttr.h"
#include "Escriba.h"
#include "External.h"
#include "Imp_Exp.h"
#include "ReString.h"
#include "Support.h"

// #define DEBUG
// #define DEBUG_FILEDIALOG

#define STATIC

 
/****************************************************************************
 *                                                                          *
 *      Basic Dialog Processor                                              *
 *                                                                          *
 ****************************************************************************/

extern MRESULT EXPENTRY Dialog_Processor ( HWND Window, ULONG msg, MPARAM mp1, MPARAM mp2 ) {

   switch ( msg ) {

      case WM_INITDLG: {

         // Associate dialog with help instance of owner window.
         WinAssociateHelpInstance ( WinQueryHelpInstance(OWNER(Window)), Window ) ;

         // Center the dialog over its owner.
         RECTL OwnerRectangle, DialogRectangle, DesktopRectangle ;
         WinQueryWindowRect ( OWNER(Window), &OwnerRectangle ) ;
         WinQueryWindowRect ( Window, &DialogRectangle ) ;
         WinQueryWindowRect ( HWND_DESKTOP, &DesktopRectangle ) ;
         POINTL Position ;
         Position.x = ( ( OwnerRectangle.xRight - OwnerRectangle.xLeft ) - ( DialogRectangle.xRight - DialogRectangle.xLeft ) ) / 2 + OwnerRectangle.xLeft ;
         Position.y = ( ( OwnerRectangle.yTop - OwnerRectangle.yBottom ) - ( DialogRectangle.yTop - DialogRectangle.yBottom ) ) / 2 + OwnerRectangle.yBottom ;
         WinMapWindowPoints ( OWNER(Window), HWND_DESKTOP, &Position, 1 ) ;
         if ( Position.x < DesktopRectangle.xLeft ) 
            Position.x = DesktopRectangle.xLeft ;
         if ( Position.y < DesktopRectangle.yBottom ) 
            Position.y = DesktopRectangle.yBottom ;
         if ( Position.x+(DialogRectangle.xRight-DialogRectangle.xLeft) > DesktopRectangle.xRight ) 
            Position.x = DesktopRectangle.xRight - (DialogRectangle.xRight-DialogRectangle.xLeft) ;
         if ( Position.y+(DialogRectangle.yTop-DialogRectangle.yBottom) > DesktopRectangle.yTop ) 
            Position.y = DesktopRectangle.yTop - (DialogRectangle.yTop-DialogRectangle.yBottom) ;
         WinSetWindowPos ( Window, 0, Position.x, Position.y, 0, 0, SWP_MOVE ) ;

         return ( 0 ) ; } /* endcase WM_INITDLG */

   } /* endswitch */

   #ifdef DEBUG
      static int Indent = 0 ;
      LogMsgRcvd ( Indent, "Dialog", msg, mp1, mp2 ) ;
      Indent += 2 ;
   #endif

   MRESULT Result = Sys_DefDialogProc ( Window, msg, mp1, mp2 ) ;

   #ifdef DEBUG
      Indent -= 2 ;
      LogMsgDone ( Indent, "Dialog", msg, Result ) ;
   #endif

   return ( Result ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Verify files of native Escriba/Pluma format.                        *
 *                                                                          *
 ****************************************************************************/

STATIC BOOL APIENTRY Escriba_Verify ( HWND Owner, char *Name ) {

   ResourceString Title ( LibraryHandle, IDS_TITLE ) ;

   FILE *File = 0 ;
   int i = 0 ;
   size_t Size = 0 ;
   char *Text = 0 ;

   File = fopen ( Name, "rb" ) ;
   if ( File == 0 ) {
      if ( Owner ) {
         ResourceString Format ( LibraryHandle, IDS_VERIFY_ERROR1 ) ;
         char Message [512] ;
         sprintf ( Message, PCHAR(Format), Name ) ;
         MessageBox ( HWND_DESKTOP, Owner, PSZ(Message), PSZ(Title), IDD_ERROR_VERIFY1, MB_ENTER | MB_ICONEXCLAMATION | MB_HELP, LibraryHandle, IDS_MESSAGE_BOX_STRINGS ) ;
      } /* endif */
      return ( FALSE ) ;
   } /* endif */

   fseek ( File, 0, SEEK_END ) ;
   Size = (size_t) ftell ( File ) ;
   if ( Size == 0 ) {
      fclose ( File ) ;
      return ( TRUE ) ;
   } /* endif */

   fseek ( File, 0, SEEK_SET ) ;
   Text = (char*) malloc ( Size ) ;
   if ( Text == 0 ) {
      fclose ( File ) ;
      if ( Owner ) {
         ResourceString Format ( LibraryHandle, IDS_VERIFY_ERROR2 ) ;
         char Message [512] ;
         sprintf ( Message, PCHAR(Format), Name ) ;
         MessageBox ( HWND_DESKTOP, Owner, PSZ(Message), PSZ(Title), IDD_ERROR_VERIFY2, MB_ENTER | MB_ICONEXCLAMATION | MB_HELP, LibraryHandle, IDS_MESSAGE_BOX_STRINGS ) ;
      } /* endif */
      return ( FALSE ) ;
   } /* endif */

   Size = fread ( Text, 1, Size, File ) ;
   if ( Size == 0 ) {
      free ( Text ) ;
      fclose ( File ) ;
      return ( TRUE ) ;
   } /* endif */

   if ( ( Text[i] != FUNCTION_START ) OR strnicmp ( Text+1, FunctionNames[FN__PREFACE], strlen(FunctionNames[FN__PREFACE]) ) ) {
      free ( Text ) ;
      fclose ( File ) ;
      if ( Owner ) {
         ResourceString Format ( LibraryHandle, IDS_VERIFY_ERROR7 ) ;
         char Message [512] ;
         sprintf ( Message, PCHAR(Format), Name ) ;
         MessageBox ( HWND_DESKTOP, Owner, PSZ(Message), PSZ(Title), IDD_ERROR_VERIFY7, MB_ENTER | MB_ICONEXCLAMATION | MB_HELP, LibraryHandle, IDS_MESSAGE_BOX_STRINGS ) ;
      } /* endif */
      return ( FALSE ) ;
   } /* endif */

   int Nesting = 0 ;
   for ( i=0; i<Size; i++ ) {
      if ( ( Text[i] == FUNCTION_ESCAPE ) AND ( i < Size-2 ) ) {
         if ( ( Text[i+1] == FUNCTION_ESCAPE ) OR ( Text[i+1] == FUNCTION_START ) OR ( Text[i+1] == FUNCTION_END ) ) {
            i ++ ;
         } else {
            free ( Text ) ;
            fclose ( File ) ;
            if ( Owner ) {
               ResourceString Format ( LibraryHandle, IDS_VERIFY_ERROR3 ) ;
               char Message [512] ;
               sprintf ( Message, PCHAR(Format), Name ) ;
               MessageBox ( HWND_DESKTOP, Owner, PSZ(Message), PSZ(Title), IDD_ERROR_VERIFY3, MB_ENTER | MB_ICONEXCLAMATION | MB_HELP, LibraryHandle, IDS_MESSAGE_BOX_STRINGS ) ;
            } /* endif */
            return ( FALSE ) ;
         } /* endif */
      } else if ( Text[i] == FUNCTION_START ) {
         Nesting ++ ;
         for ( int Type=0; Type<MAX_FUNCTIONS; Type++ )
            if ( !strnicmp ( PCHAR(Text+i+1), FunctionNames[Type], strlen(FunctionNames[Type]) ) )
               break ;
         if ( Type >= MAX_FUNCTIONS ) {
            free ( Text ) ;
            fclose ( File ) ;
            if ( Owner ) {
               ResourceString Format ( LibraryHandle, IDS_VERIFY_ERROR4 ) ;
               char Message [512] ;
               sprintf ( Message, PCHAR(Format), Name ) ;
               MessageBox ( HWND_DESKTOP, Owner, PSZ(Message), PSZ(Title), IDD_ERROR_VERIFY4, MB_ENTER | MB_ICONEXCLAMATION | MB_HELP, LibraryHandle, IDS_MESSAGE_BOX_STRINGS ) ;
            } /* endif */
            return ( FALSE ) ;
         } /* endif */
       } else if ( Text[i] == FUNCTION_END ) {
         Nesting -- ;
      } else if ( ( Text[i] == 0 ) AND ( Nesting == 0 ) ) {
         free ( Text ) ;
         fclose ( File ) ;
         if ( Owner ) {
            ResourceString Format ( LibraryHandle, IDS_VERIFY_ERROR5 ) ;
            char Message [512] ;
            sprintf ( Message, PCHAR(Format), Name ) ;
            MessageBox ( HWND_DESKTOP, Owner, PSZ(Message), PSZ(Title), IDD_ERROR_VERIFY5, MB_ENTER | MB_ICONEXCLAMATION | MB_HELP, LibraryHandle, IDS_MESSAGE_BOX_STRINGS ) ;
         } /* endif */
         return ( FALSE ) ;
      } /* endif */
   } /* endfor */

   free ( Text ) ;
   fclose ( File ) ;

   if ( Nesting ) {
      if ( Owner ) {
         ResourceString Format ( LibraryHandle, IDS_VERIFY_ERROR6 ) ;
         char Message [512] ;
         sprintf ( Message, PCHAR(Format), Name ) ;
         MessageBox ( HWND_DESKTOP, Owner, PSZ(Message), PSZ(Title), IDD_ERROR_VERIFY6, MB_ENTER | MB_ICONEXCLAMATION | MB_HELP, LibraryHandle, IDS_MESSAGE_BOX_STRINGS ) ;
      } /* endif */
      return ( FALSE ) ;
   } /* endif */

   return ( TRUE ) ;
}


/****************************************************************************
 *                                                                          *
 *      Import files of native Escriba/Pluma format.                        *
 *                                                                          *
 ****************************************************************************/

STATIC void * APIENTRY Escriba_Import ( HWND Owner, char *Name, long *Size ) {

   ResourceString Title ( LibraryHandle, IDS_TITLE ) ;

   FILE *File = 0 ;
   char *Text = 0 ;

   File = fopen ( Name, "rb" ) ;
   if ( File == 0 ) {
      if ( Owner ) {
         ResourceString Format ( LibraryHandle, IDS_IMPORT_ERROR1 ) ;
         char Message [512] ;
         sprintf ( Message, PCHAR(Format), Name ) ;
         MessageBox ( HWND_DESKTOP, Owner, PSZ(Message), PSZ(Title), IDD_ERROR_IMPORT1, MB_ENTER | MB_ICONEXCLAMATION | MB_HELP, LibraryHandle, IDS_MESSAGE_BOX_STRINGS ) ;
      } /* endif */
      return ( 0 ) ;
   } /* endif */

   fseek ( File, 0, SEEK_END ) ;
   *Size = ftell ( File ) ;
   if ( *Size == 0 ) {
      fclose ( File ) ;
      return ( 0 ) ;
   } /* endif */

   fseek ( File, 0, SEEK_SET ) ;
   Text = (char*) malloc ( (size_t)*Size ) ;
   if ( Text == 0 ) {
      fclose ( File ) ;
      if ( Owner ) {
         ResourceString Format ( LibraryHandle, IDS_IMPORT_ERROR2 ) ;
         char Message [512] ;
         sprintf ( Message, PCHAR(Format), Name ) ;
         MessageBox ( HWND_DESKTOP, Owner, PSZ(Message), PSZ(Title), IDD_ERROR_IMPORT2, MB_ENTER | MB_ICONEXCLAMATION | MB_HELP, LibraryHandle, IDS_MESSAGE_BOX_STRINGS ) ;
      } /* endif */
      return ( 0 ) ;
   } /* endif */

   *Size = (long) fread ( Text, 1, (size_t)*Size, File ) ;
   if ( *Size == 0 ) {
      free ( Text ) ;
      Text = 0 ;
   } /* endif */

   fclose ( File ) ;
   return ( Text ) ;
}

/****************************************************************************
 *                                                                          *
 *      Export files of native Escriba/Pluma format.                        *
 *                                                                          *
 ****************************************************************************/

STATIC BOOL APIENTRY Escriba_Export ( HWND Owner, char *Name, void *Text, long Size ) {

   ResourceString Title ( LibraryHandle, IDS_TITLE ) ;

   // Try to open the file.  Complain if not able to and return an error.
   FILE *File = fopen ( PCHAR(Name), "wb" ) ;
   if ( File == NULL ) {
      if ( Owner ) {
         ResourceString Format ( LibraryHandle, IDS_EXPORT_ERROR1 ) ;
         char Message [512] ;
         sprintf ( Message, PCHAR(Format), Name ) ;
         MessageBox ( HWND_DESKTOP, Owner, PSZ(Message), PSZ(Title), IDD_ERROR_EXPORT1, MB_ENTER | MB_ICONEXCLAMATION | MB_HELP, LibraryHandle, IDS_MESSAGE_BOX_STRINGS ) ;
      } /* endif */
      return ( FALSE ) ;
   } /* endif */

   // Write the file data to disk.
   if ( fwrite ( Text, 1, (size_t)Size, File ) != Size ) {
      fclose ( File ) ;
      remove ( Name ) ;
      if ( Owner ) {
         ResourceString Format ( LibraryHandle, IDS_EXPORT_ERROR2 ) ;
         char Message [512] ;
         sprintf ( Message, PCHAR(Format), Name ) ;
         MessageBox ( HWND_DESKTOP, Owner, PSZ(Message), PSZ(Title), IDD_ERROR_EXPORT2, MB_ENTER | MB_ICONEXCLAMATION | MB_HELP, LibraryHandle, IDS_MESSAGE_BOX_STRINGS ) ;
      } /* endif */
      return ( FALSE ) ;
   } /* endif */

   // Get Escriba's icon data.
   PVOID IconData ;
   DosGetResource ( 0, RT_POINTER, ID_MAIN, &IconData ) ;

   // Get the icon size.
   ULONG IconSize ;
   DosQueryResourceSize ( 0, RT_POINTER, ID_MAIN, &IconSize ) ;

   // Pull off the file's full name (name+ext) from the full path.
   char FullName [ CCHMAXPATH + 1 ] ;
   char Drive [_MAX_DRIVE+1] ;
   char Dir   [_MAX_DIR+1] ;
   char Fname [_MAX_FNAME+1] ;
   char Ext   [_MAX_EXT+1] ;
   _splitpath ( Name, Drive, Dir, Fname, Ext ) ;
   strcpy ( FullName, Fname ) ;
   strcat ( FullName, Ext ) ;

   // Build the file's extended attributes.
   static MVMT_VALUE Types [] = {
      { EAT_ASCII,  7, "Escriba"    },
   } ;

   static EADATA Table [] = {
      { PSZ(".TYPE"),     EAT_MVMT,  0, NULL     },     // Type length & value need filling in.
      { PSZ(".VERSION"),  EAT_ASCII, 3, REVISION },
      { PSZ(".LONGNAME"), EAT_ASCII, 0, NULL     },     // Name length & value need filling in.
      { PSZ(".ICON"),     EAT_ICON,  0, NULL     },     // Icon size & value need filling in.
   } ;

   Table[0].Length = USHORT ( BuildMVMTValue ( 0, sizeof(Types)/sizeof(Types[0]), Types ) ) ;
   Table[0].Value = malloc ( Table[0].Length ) ;
   BuildMVMTValue ( Table[0].Value, sizeof(Types)/sizeof(Types[0]), Types ) ;

   Table[2].Length = USHORT ( strlen ( FullName ) ) ;
   Table[2].Value = FullName ;

   Table[3].Length = USHORT(IconSize) ;
   Table[3].Value = IconData ;

   PEAOP2 pExtendedAttributes = BuildExtendedAttributes ( sizeof(Table)/sizeof(Table[0]), Table ) ;

   // If the extended attributes got built successfully, then attach them to the file.
   if ( pExtendedAttributes ) {
      DosSetFileInfo ( fileno(File), 2, PUCHAR(pExtendedAttributes), sizeof(*pExtendedAttributes) ) ;
      free ( pExtendedAttributes->fpFEA2List ) ;
      free ( pExtendedAttributes ) ;
   } /* endif */

   // Free the memory allocated for the MVMT value.
   free ( Table[0].Value ) ;

   // Release the icon data.
   DosFreeResource ( IconData ) ;

   // Close the file.
   fclose ( File ) ;

   // Return no error.
   return ( TRUE ) ;
}

/****************************************************************************
 *                                                                          *
 *      Prepare Document File Type List                                     *
 *                                                                          *
 ****************************************************************************/

#define MAX_FILE_TYPES (100)

extern PSZ      FileTypeList      [MAX_FILE_TYPES] = { 0 } ;
extern char    *FileWildcardList  [MAX_FILE_TYPES] = { 0 } ;
extern Addon   *FileExtenderList  [MAX_FILE_TYPES] = { 0 } ;
static BOOL     FileExtenderAdded [MAX_FILE_TYPES] = { 0 } ;

extern void PrepareFileTypeList ( BOOL ForExport ) {

   // Clear the file type list.
   ClearFileTypeList ( ) ;
   int EntryCount = 0 ;
   memset ( FileExtenderList, 0, sizeof(FileExtenderList) ) ;

   // Add the 'all files' entry to the list.
   ResourceString AllDocs ( LibraryHandle, IDS_ALL_DOCUMENTS ) ;
   FileTypeList     [EntryCount] = PSZ(AllDocs) ;
   FileWildcardList [EntryCount] = "*.*" ;
   FileExtenderList [EntryCount] = 0 ;
   FileExtenderAdded[EntryCount] = FALSE ;
   EntryCount++ ;

   // Add the 'Escriba' entry to the list.
   Addon *Entry = new ImportExport ( "Escriba", "*.*", Escriba_Verify, Escriba_Import, Escriba_Export ) ;
   FileTypeList     [EntryCount] = ((ImportExport*)Entry)->QueryFileType() ;
   FileWildcardList [EntryCount] = ((ImportExport*)Entry)->QueryWildcard() ;
   FileExtenderList [EntryCount] = Entry ;
   FileExtenderAdded[EntryCount] = TRUE ;
   EntryCount ++ ;

   // Scan the add-on list for import/export modules.
   Entry = AddonList.FindFirstOfType ( TYPE_IMPORTEXPORT ) ;
   while ( Entry ) {
      FileTypeList     [EntryCount] = ((ImportExport*)Entry)->QueryFileType() ;
      FileWildcardList [EntryCount] = ((ImportExport*)Entry)->QueryWildcard() ;
      FileExtenderList [EntryCount] = Entry ;
      FileExtenderAdded[EntryCount] = FALSE ;
      EntryCount ++ ;
      Entry = AddonList.FindNextOfType ( Entry ) ;
   } /* endwhile */

   // If not building list for export, scan the add-on list for graphic import modules.
   if ( NOT ForExport ) {

      Entry = AddonList.FindFirstOfType ( TYPE_GRAPHICIMPORT ) ;
      while ( Entry ) {
         FileTypeList     [EntryCount] = ((GraphicImporter*)Entry)->QueryFileType() ;
         FileWildcardList [EntryCount] = ((GraphicImporter*)Entry)->QueryWildcard() ;
         FileExtenderList [EntryCount] = Entry ;
         FileExtenderAdded[EntryCount] = FALSE ;
         EntryCount ++ ;
         Entry = AddonList.FindNextOfType ( Entry ) ;
      } /* endwhile */

      // Add the 'Metafile' entry to the list.
      Entry = new MetafileImporter ( ) ;
      FileTypeList     [EntryCount] = ((MetafileImporter*)Entry)->QueryFileType() ;
      FileWildcardList [EntryCount] = ((MetafileImporter*)Entry)->QueryWildcard() ;
      FileExtenderList [EntryCount] = Entry ;
      FileExtenderAdded[EntryCount] = TRUE ;
      EntryCount ++ ;

   } /* endif */

   // Mark the new end of list.
   FileTypeList     [EntryCount] = 0 ;
   FileWildcardList [EntryCount] = 0 ;
   FileExtenderList [EntryCount] = 0 ;
   FileExtenderAdded[EntryCount] = 0 ;
}

extern void ClearFileTypeList ( void ) {
   int Index = 0 ;
   while ( FileTypeList[Index] ) {
      if ( FileExtenderList[Index] && FileExtenderAdded[Index] ) 
         delete FileExtenderList[Index] ;
      Index ++ ;
   } /* endwhile */
   memset ( FileTypeList,      0, sizeof(FileTypeList     ) ) ;
   memset ( FileWildcardList,  0, sizeof(FileWildcardList ) ) ;
   memset ( FileExtenderList,  0, sizeof(FileExtenderList ) ) ;
   memset ( FileExtenderAdded, 0, sizeof(FileExtenderAdded) ) ;
} 

/****************************************************************************
 *                                                                          *
 *      Prepare Graphic File Type List                                      *
 *                                                                          *
 ****************************************************************************/

extern PSZ      GraphicTypeList      [MAX_FILE_TYPES] = { 0 } ;
extern char    *GraphicWildcardList  [MAX_FILE_TYPES] = { 0 } ;
extern Addon   *GraphicExtenderList  [MAX_FILE_TYPES] = { 0 } ;
static BOOL     GraphicExtenderAdded [MAX_FILE_TYPES] = { 0 } ;

extern void PrepareGraphicTypeList ( void ) {

   // Clear the file type list.
   ClearGraphicTypeList ( ) ;
   int EntryCount = 0 ;
   memset ( GraphicExtenderList, 0, sizeof(GraphicExtenderList) ) ;

   // Add the 'all files' entry to the list.
   ResourceString AllGraphics ( LibraryHandle, IDS_ALL_GRAPHICS ) ;
   GraphicTypeList     [EntryCount] = PSZ(AllGraphics) ;
   GraphicWildcardList [EntryCount] = "*.*" ;
   GraphicExtenderList [EntryCount] = 0 ;
   GraphicExtenderAdded[EntryCount] = FALSE ;
   EntryCount++ ;

   // Scan the add-on list for import/export modules.
   Addon *Entry = AddonList.FindFirstOfType ( TYPE_GRAPHICIMPORT ) ;
   while ( Entry ) {
      GraphicTypeList     [EntryCount] = ((GraphicImporter*)Entry)->QueryFileType() ;
      GraphicWildcardList [EntryCount] = ((GraphicImporter*)Entry)->QueryWildcard() ;
      GraphicExtenderList [EntryCount] = Entry ;
      GraphicExtenderAdded[EntryCount] = FALSE ;
      EntryCount ++ ;
      Entry = AddonList.FindNextOfType ( Entry ) ;
   } /* endwhile */

   // Add the 'Metafile' entry to the list.
   Entry = new MetafileImporter ( ) ;
   GraphicTypeList     [EntryCount] = ((MetafileImporter*)Entry)->QueryFileType() ;
   GraphicWildcardList [EntryCount] = ((MetafileImporter*)Entry)->QueryWildcard() ;
   GraphicExtenderList [EntryCount] = Entry ;
   GraphicExtenderAdded[EntryCount] = TRUE ;
   EntryCount ++ ;

   // Mark the new end of list.
   GraphicTypeList     [EntryCount] = 0 ;
   GraphicWildcardList [EntryCount] = 0 ;
   GraphicExtenderList [EntryCount] = 0 ;
   GraphicExtenderAdded[EntryCount] = 0 ;
}

extern void ClearGraphicTypeList ( void ) {
   int Index = 0 ;
   while ( GraphicTypeList[Index] ) {
      if ( GraphicExtenderList[Index] && GraphicExtenderAdded[Index] ) 
         delete GraphicExtenderList[Index] ;
      Index ++ ;
   } /* endwhile */
   memset ( GraphicTypeList,      0, sizeof(GraphicTypeList     ) ) ;
   memset ( GraphicWildcardList,  0, sizeof(GraphicWildcardList ) ) ;
   memset ( GraphicExtenderList,  0, sizeof(GraphicExtenderList ) ) ;
   memset ( GraphicExtenderAdded, 0, sizeof(GraphicExtenderAdded) ) ;
} 

/****************************************************************************
 *                                                                          *
 *      Set File-Dialog's File Filter                                       *
 *                                                                          *
 ****************************************************************************/

STATIC void SetFilter ( HWND Window ) {
   PFILEDLG DialogData = PFILEDLG ( Sys_GetWindowData ( Window ) ) ;
   PFILEDIALOG_DATA Data = PFILEDIALOG_DATA ( DialogData->ulUser ) ;
   SHORT Index = SHORT1FROMMR ( Sys_SendMessage ( CHILD(Window,DID_FILTER_CB), LM_QUERYSELECTION, MPFROMSHORT(LIT_FIRST), 0 ) ) ;
   if ( Index == LIT_NONE ) {
      Data->Index = Data->FilterIndex = -1 ;
      Sys_SetWindowText ( CHILD(Window,DID_FILENAME_ED), PSZ("*.*") ) ;
   } else {
      char Type [80] ;
      Sys_SendMessage ( CHILD(Window,DID_FILTER_CB), LM_QUERYITEMTEXT,
         MPFROM2SHORT ( Index, sizeof(Type) ), MPFROMP ( Type ) ) ;
      for ( int i=0; Data->TypeList[i]; i++ ) {
         if ( !strcmpi ( Type, Data->TypeList[i] ) ) 
            break ;
      } /* endfor every file type */
      if ( Data->TypeList[i] ) {
         Data->Index = Data->FilterIndex = i ;
         Sys_SetWindowText ( CHILD(Window,DID_FILENAME_ED), PSZ(Data->WildcardList[Data->Index]) ) ;
      } else {
         Data->Index = Data->FilterIndex = -1 ;
         Sys_SetWindowText ( CHILD(Window,DID_FILENAME_ED), PSZ("*.*") ) ;
      } /* endif */
   } /* endif */
}

/****************************************************************************
 *                                                                          *
 *      File Type Dialog Processor                                          *
 *                                                                          *
 ****************************************************************************/

typedef struct {
   char *Filename ;
   char **TypeList ;
   BOOL *Passed ;
   int   Index ;
} FILETYPEDIALOG_DATA, *PFILETYPEDIALOG_DATA ;

STATIC MRESULT EXPENTRY FileTypeDialog_Processor ( HWND Window, ULONG msg, MPARAM mp1, MPARAM mp2 ) {

   switch ( msg ) {

      case WM_INITDLG: {        // WM_INITDLG: Load the file name and type list controls.
         PFILETYPEDIALOG_DATA Parms = PFILETYPEDIALOG_DATA ( PVOIDFROMMP ( mp2 ) ) ;
         Sys_SetWindowData ( Window, Parms ) ;
         Sys_SetWindowText ( CHILD(Window,IDD_FILETYPE_NAME), PSZ(Parms->Filename) ) ;
         for ( int i=0; Parms->TypeList[i]; i++ ) {
            if ( Parms->Passed[i] ) {
               Sys_SendMessage ( CHILD(Window,IDD_FILETYPE_LIST), LM_INSERTITEM,
                  MPFROMSHORT ( LIT_SORTASCENDING ), MPFROMP ( Parms->TypeList[i] ) ) ;
            } /* endif */
         } /* endfor */
         break; } /* endcase WM_INITDLG */

      case WM_CONTROL: {        // WM_CONTROL: Check for double-click on list entry.
         PFILETYPEDIALOG_DATA Parms = PFILETYPEDIALOG_DATA ( Sys_GetWindowData ( Window ) ) ;
         SHORT Id = SHORT1FROMMP ( mp1 ) ;
         SHORT Message = SHORT2FROMMP ( mp1 ) ;
         if ( Id == IDD_FILETYPE_LIST ) {
            if ( Message == LN_ENTER ) {
               SHORT Index = SHORT1FROMMR ( Sys_SendMessage ( CHILD(Window,IDD_FILETYPE_LIST), LM_QUERYSELECTION, MPFROMSHORT(LIT_FIRST), 0 ) ) ;
               if ( Index == LIT_NONE )
                  break ;
               UCHAR FileType [100] ;
               Sys_SendMessage ( CHILD(Window,IDD_FILETYPE_LIST), LM_QUERYITEMTEXT,
                  MPFROM2SHORT ( Index, sizeof(FileType) ), MPFROMP ( FileType ) ) ;
               for ( int i=1; Parms->TypeList[i]; i++ ) {
                  if ( !stricmp ( Parms->TypeList[i], PCHAR(FileType) ) ) {
                     Parms->Index = i ;
                     Sys_EndDialog ( Window, TRUE ) ;
                  } /* endif */
               } /* endfor */
            } /* endif */
         } /* endif */
         break; } /* endcase WM_CONTROL */

      case WM_COMMAND: {        // WM_COMMAND: Check the click on OK or Cancel.
         PFILETYPEDIALOG_DATA Parms = PFILETYPEDIALOG_DATA ( Sys_GetWindowData ( Window ) ) ;
         if ( SHORT1FROMMP(mp1) == DID_OK ) {
            SHORT Index = SHORT1FROMMR ( Sys_SendMessage ( CHILD(Window,IDD_FILETYPE_LIST), LM_QUERYSELECTION, MPFROMSHORT(LIT_FIRST), 0 ) ) ;
            if ( Index == LIT_NONE )
               break ;
            UCHAR FileType [100] ;
            Sys_SendMessage ( CHILD(Window,IDD_FILETYPE_LIST), LM_QUERYITEMTEXT,
               MPFROM2SHORT ( Index, sizeof(FileType) ), MPFROMP ( FileType ) ) ;
            for ( int i=1; Parms->TypeList[i]; i++ ) {
               if ( !stricmp ( Parms->TypeList[i], PCHAR(FileType) ) ) {
                  Parms->Index = i ;
                  Sys_EndDialog ( Window, TRUE ) ;
               } /* endif */
            } /* endfor */
         } else if ( SHORT1FROMMP(mp1) == DID_CANCEL ) {
            Sys_EndDialog ( Window, FALSE ) ;
         } /* endif */
         return ( 0 ) ; } /* endcase WM_COMMAND */

   } /* endswitch */

   MRESULT Result = Dialog_Processor ( Window, msg, mp1, mp2 ) ;

   return ( Result ) ;
}
/****************************************************************************
 *                                                                          *
 *      Determine File Type                                                 *
 *                                                                          *
 ****************************************************************************/

extern int GetFileTypeIndex ( HWND Window, char *Name, PSZ *TypeList, Addon *ExtenderList[], int Index ) {

   // If a specific file type is already selected, then only check the file that way.
   if ( Index > 0 ) {
      if ( !stricmp ( ((Addon*)ExtenderList[Index])->QueryType(), TYPE_IMPORTEXPORT ) ) {
         if ( ((ImportExport*)ExtenderList[Index])->Verify ( Window, Name ) )
            return ( Index ) ;
      } else if ( !stricmp ( ((Addon*)ExtenderList[Index])->QueryType(), TYPE_GRAPHICIMPORT ) ) {
         if ( ((GraphicImporter*)ExtenderList[Index])->Verify ( Window, Name ) )
            return ( Index ) ;
      } else if ( !stricmp ( ((Addon*)ExtenderList[Index])->QueryType(), TYPE_METAFILEIMPORT ) ) {
         if ( ((MetafileImporter*)ExtenderList[Index])->Verify ( Window, Name ) )
            return ( Index ) ;
      } /* endif */
      return ( 0 ) ;

   } /* endif */

   // Check the file against all the specific file types.
   int FirstPassed = 0 ;
   BOOL AnyPassed = FALSE, ManyPassed = FALSE ;
   BOOL Passed [MAX_FILE_TYPES] ;
   memset ( Passed, 0, sizeof(Passed) ) ;
   for ( int i=1; ExtenderList[i]; i++ ) {
      if ( !stricmp ( ((Addon*)ExtenderList[i])->QueryType(), TYPE_IMPORTEXPORT ) ) {
         Passed[i] = ((ImportExport*)ExtenderList[i])->Verify ( 0, Name ) ;
      } else if ( !stricmp ( ((Addon*)ExtenderList[i])->QueryType(), TYPE_GRAPHICIMPORT ) ) {
         Passed[i] = ((GraphicImporter*)ExtenderList[i])->Verify ( 0, Name ) ;
      } else if ( !stricmp ( ((Addon*)ExtenderList[i])->QueryType(), TYPE_METAFILEIMPORT ) ) {
         Passed[i] = ((MetafileImporter*)ExtenderList[i])->Verify ( 0, Name ) ;
      } /* endif */
      if ( Passed[i] ) {
         if ( AnyPassed ) {
            ManyPassed = TRUE ;
         } else {
            AnyPassed = TRUE ;
            FirstPassed = i ;
         } /* endif */
      } /* endif */
   } /* endfor i */

   // If none passed, return an error.
   if ( NOT AnyPassed ) {
      ResourceString Title ( LibraryHandle, IDS_TITLE ) ;
      ResourceString Format ( LibraryHandle, IDS_VERIFY_ERROR8 ) ;
      char Message [500] ;
      sprintf ( Message, PCHAR(Format), Name ) ;
      MessageBox ( HWND_DESKTOP, Window, PSZ(Message), PSZ(Title), IDD_ERROR_VERIFY8, MB_ENTER | MB_ICONEXCLAMATION | MB_HELP, LibraryHandle, IDS_MESSAGE_BOX_STRINGS ) ;
      return ( 0 ) ;
   } /* endif */

   // If many types passed . . .
   if ( ManyPassed ) {
      // If a parent window has been specified, then summon the file types dialog and let the user choose.
      if ( Window ) {
         FILETYPEDIALOG_DATA Parms = { Name, TypeList, Passed, 0 } ;
         if ( WinDlgBox ( HWND_DESKTOP, Window, FileTypeDialog_Processor, LibraryHandle, IDD_FILETYPE, &Parms ) ) 
            return ( Parms.Index ) ;
      } /* endif */
      // Otherwise, return an error.
      return ( 0 ) ;
   } /* endif */

   // Return the index of the type that passed inspection.
   return ( FirstPassed ) ;
}

/****************************************************************************
 *                                                                          *
 *      File Dialog Processor                                               *
 *                                                                          *
 ****************************************************************************/

extern MRESULT EXPENTRY FileDialog_Processor ( HWND Window, ULONG msg, MPARAM mp1, MPARAM mp2 ) {

   #ifdef DEBUG_FILEDIALOG
      static int Indent = 0 ;
      LogMsgRcvd ( Indent, "FileDialog", Window, msg, mp1, mp2 ) ;
      Indent += 2 ;
   #endif

   MRESULT Result ;

   switch ( msg ) {

      case WM_INITDLG: {        // WM_INITDLG: Go read the filter type field.
         PFILEDLG DialogData = PFILEDLG ( Sys_GetWindowData ( Window ) ) ;
         PFILEDIALOG_DATA Data = PFILEDIALOG_DATA ( DialogData->ulUser ) ;
         SetFilter ( Window ) ;
         ResourceString FilenameText ( LibraryHandle, Data->SelectingForLoad?IDS_FDLG_FILENAME_TXT1:IDS_FDLG_FILENAME_TXT2) ;
         Sys_SetWindowText ( CHILD(Window,DID_FILENAME_TXT), PSZ(FilenameText) ) ;
         ResourceString DriveText ( LibraryHandle, IDS_FDLG_DRIVE_TXT ) ;
         Sys_SetWindowText ( CHILD(Window,DID_DRIVE_TXT), PSZ(DriveText) ) ;
         ResourceString FilterText ( LibraryHandle, IDS_FDLG_FILTER_TXT ) ;
         Sys_SetWindowText ( CHILD(Window,DID_FILTER_TXT), PSZ(FilterText) ) ;
         ResourceString DirectoryText ( LibraryHandle, IDS_FDLG_DIRECTORY_TXT ) ;
         Sys_SetWindowText ( CHILD(Window,DID_DIRECTORY_TXT), PSZ(DirectoryText) ) ;
         ResourceString FilesText ( LibraryHandle, IDS_FDLG_FILES_TXT ) ;
         Sys_SetWindowText ( CHILD(Window,DID_FILES_TXT), PSZ(FilesText) ) ;
         ResourceString HelpText ( LibraryHandle, IDS_FDLG_HELP_PB ) ;
         Sys_SetWindowText ( CHILD(Window,DID_HELP_PB), PSZ(HelpText) ) ;
         ResourceString ApplyText ( LibraryHandle, IDS_FDLG_APPLY_PB ) ;
         Sys_SetWindowText ( CHILD(Window,DID_APPLY_PB), PSZ(ApplyText) ) ;
         ResourceString OKText ( LibraryHandle, Data->SelectingForLoad?IDS_FDLG_OK_PB1:IDS_FDLG_OK_PB2 ) ;
         Sys_SetWindowText ( CHILD(Window,DID_OK_PB), PSZ(OKText) ) ;
         ResourceString CancelText ( LibraryHandle, IDS_FDLG_CANCEL_PB ) ;
         Sys_SetWindowText ( CHILD(Window,DID_CANCEL_PB), PSZ(CancelText) ) ;

         DeviceContext Memory = DeviceContext ( "FileDialog_Processor::INITDLG", long(WinQueryAnchorBlock(Window)) ) ;
         WorkSpace MemoryPS = WorkSpace ( "FileDialog_Processor::INITDLG", 0, &Memory, WinQueryAnchorBlock(Window), TRUE ) ;

         SWP Position ;
         WinQueryWindowPos ( CHILD(Window,DID_OK_PB), &Position ) ;
         POINTL Point = { Position.x, Position.y } ;
         long Spacing = Position.x ;
         long Padding = MemoryPS.MeasureText ( "XXX" ) ;
         long Width = MemoryPS.MeasureText ( PCHAR(OKText) ) + Padding ;
         long Height = Position.cy ;
         WinSetWindowPos ( CHILD(Window,DID_OK_PB), 0, Point.x, Point.y, Width, Height, SWP_MOVE | SWP_SIZE ) ;
         Point.x += Width + Spacing ;

         if ( WinQueryWindowULong ( CHILD(Window,DID_APPLY_PB), QWL_STYLE ) & WS_VISIBLE ) {
            WinQueryWindowPos ( CHILD(Window,DID_APPLY_PB), &Position ) ;
            long Width = MemoryPS.MeasureText ( PCHAR(ApplyText) ) + Padding ;
            WinSetWindowPos ( CHILD(Window,DID_APPLY_PB), 0, Point.x, Point.y, Width, Height, SWP_MOVE | SWP_SIZE ) ;
            Point.x += Width + Spacing ;
         } /* endif */

         WinQueryWindowPos ( CHILD(Window,DID_CANCEL_PB), &Position ) ;
         Width = MemoryPS.MeasureText ( PCHAR(CancelText) ) + Padding ;
         WinSetWindowPos ( CHILD(Window,DID_CANCEL_PB), 0, Point.x, Point.y, Width, Height, SWP_MOVE | SWP_SIZE ) ;
         Point.x += Width + Spacing ;

         if ( WinQueryWindowULong ( CHILD(Window,DID_HELP_PB), QWL_STYLE ) & WS_VISIBLE ) {
            WinQueryWindowPos ( CHILD(Window,DID_HELP_PB), &Position ) ;
            Width = MemoryPS.MeasureText ( PCHAR(HelpText) ) + Padding ;
            WinSetWindowPos ( CHILD(Window,DID_HELP_PB), 0, Point.x, Point.y, Width, Height, SWP_MOVE | SWP_SIZE ) ;
            Point.x += Width + Spacing ;
         } /* endif */

         Result = WinDefFileDlgProc ( Window, msg, mp1, mp2 ) ;
         break; } /* endcase WM_INITDLG */

      case WM_CONTROL: {        // WM_CONTROL: Check if the filter type field has changed.
         SHORT Id = SHORT1FROMMP ( mp1 ) ;
         SHORT Message = SHORT2FROMMP ( mp1 ) ;
         if ( Id == DID_FILTER_CB ) 
            if ( ( Message == CBN_ENTER ) OR ( Message == CBN_LBSELECT ) )
               SetFilter ( Window ) ;
         Result = WinDefFileDlgProc ( Window, msg, mp1, mp2 ) ;
         break; } /* endcase WM_CONTROL */

      case FDM_FILTER: {        // FDM_FILTER: Validate file for inclusion in list.
         PFILEDLG DialogData = PFILEDLG ( Sys_GetWindowData ( Window ) ) ;
         PFILEDIALOG_DATA Data = PFILEDIALOG_DATA ( DialogData->ulUser ) ;
         char *Name = (char*) PVOIDFROMMP ( mp1 ) ;
         char *Type = (char*) PVOIDFROMMP ( mp2 ) ;
         Result = MRFROMLONG ( FALSE ) ;

         // Check against wildcard(s) first.
         char Wildcard [CCHMAXPATH], EditedText[CCHMAXPATH] ;
         Sys_GetWindowText ( CHILD(Window,DID_FILENAME_ED), Wildcard, sizeof(Wildcard) ) ;
         BOOL Matched = FALSE ; char *p = Wildcard, *p1 = 0 ;
         do {
            p1 = strstr ( p, "," ) ; if ( p1 ) *p1 = 0 ;
            APIRET rc = DosEditName ( 1, Name, p, EditedText, sizeof(EditedText) ) ;
            if ( EditedText[strlen(EditedText)-1] == '.' ) 
               EditedText[strlen(EditedText)-1] = 0 ;
            if ( !rc && !strcmpi ( Name, EditedText ) ) 
               Matched = TRUE ;
            p = p1 ? p1 + 1 : 0 ;
         } while ( p && !Matched ); /* enddo */
         if ( !Matched ) {
            Result = MRFROMLONG ( FALSE ) ;

         // If matching all file types, return TRUE unconditionally.
         } else if ( Data->Index == -1  ) {
            Result = MRFROMLONG ( TRUE ) ;

         // If showing all files matched by any filter . . .
         } else if ( Data->Index == 0  ) {
            Result = MRFROMLONG ( FALSE ) ;
            for ( int i=1; Data->WildcardList[i]; i++ ) {
               if ( !stricmp ( ((Addon*)Data->ExtenderList[i])->QueryType(), TYPE_IMPORTEXPORT ) ) {
                  if ( ((ImportExport*)Data->ExtenderList[i])->Filter ( Name, Type ) ) {
                     Result = MRFROMLONG ( TRUE ) ;
                     break ;
                  } /* endif */
               } else if ( !stricmp ( ((Addon*)Data->ExtenderList[i])->QueryType(), TYPE_GRAPHICIMPORT ) ) {
                  if ( ((GraphicImporter*)Data->ExtenderList[i])->Filter ( Name, Type ) ) {
                     Result = MRFROMLONG ( TRUE ) ;
                     break ;
                  } /* endif */
               } else if ( !stricmp ( ((Addon*)Data->ExtenderList[i])->QueryType(), TYPE_METAFILEIMPORT ) ) {
                  if ( ((MetafileImporter*)Data->ExtenderList[i])->Filter ( Name, Type ) ) {
                     Result = MRFROMLONG ( TRUE ) ;
                     break ;
                  } /* endif */
               } /* endif */
            } /* endfor every specific file type */

         // For all other file types, rely on the extender's filter function.
         } else if ( !stricmp ( ((Addon*)Data->ExtenderList[Data->Index])->QueryType(), TYPE_IMPORTEXPORT ) ) {
            Result = MRFROMLONG ( ((ImportExport*)Data->ExtenderList[Data->Index])->Filter ( Name, Type ) ) ;
         } else if ( !stricmp ( ((Addon*)Data->ExtenderList[Data->Index])->QueryType(), TYPE_GRAPHICIMPORT ) ) {
            Result = MRFROMLONG ( ((GraphicImporter*)Data->ExtenderList[Data->Index])->Filter ( Name, Type ) ) ;
         } else if ( !stricmp ( ((Addon*)Data->ExtenderList[Data->Index])->QueryType(), TYPE_METAFILEIMPORT ) ) {
            Result = MRFROMLONG ( ((MetafileImporter*)Data->ExtenderList[Data->Index])->Filter ( Name, Type ) ) ;
         } /* endif */

         break ;
      } /* endcase FDM_FILTER */

      case FDM_VALIDATE: {      // FDM_VALIDATE: Validate file selected for use.
         PFILEDLG DialogData = PFILEDLG ( Sys_GetWindowData ( Window ) ) ;
         PFILEDIALOG_DATA Data = PFILEDIALOG_DATA ( DialogData->ulUser ) ;
         if ( Data->SelectingForLoad ) {
            char *Name = (char*) PVOIDFROMMP ( mp1 ) ;
            Data->Index = GetFileTypeIndex ( Window, Name, Data->TypeList, Data->ExtenderList, Data->Index ) ;
            if ( Data->Index > 0 )
               return ( MRFROMLONG ( TRUE ) ) ;
            SetFilter ( Window ) ;
            Result = MRFROMLONG ( FALSE ) ;
         } else {
            Result = MRFROMLONG ( TRUE ) ;
         } /* endif */
         break ;
      } /* endcase FDM_VALIDATE */

      default: 
         Result = WinDefFileDlgProc ( Window, msg, mp1, mp2 ) ;

   } /* endswitch */

   #ifdef DEBUG_FILEDIALOG
      Indent -= 2 ;
      LogMsgDone ( Indent, "FileDialog", msg, Result ) ;
   #endif

   return ( Result ) ;
}

