/***************************************************************** ADDONS.CPP
 *                                                                          *
 *                       Add-on Extension Registry                          *
 *                                                                          *
 ****************************************************************************/

#include "System.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Addons.h"
#include "Debug.h"
#include "Dialog2.h"
#include "Escriba.h"
#include "External.h"
#include "Imp_Exp.h"
#include "ReString.h"
#include "Thread.h"

// #define DEBUG
// #define DEBUG_DESTRUCTOR
// #define DEBUG_LOCKS

/****************************************************************************
 *                                                                          *
 *  Utility: Given a module name, create addon extension object.            *
 *                                                                          *
 ****************************************************************************/

extern class Addon *Addon_Create ( char *Path ) {

   // First, try to load the module specified.
   Module *Library = new Module ( Path, FALSE ) ;
   if ( Library->QueryHandle() == 0 ) {
      delete Library ;
      return ( 0 ) ;
   } /* endif */

   // Get module type function.
   char*(APIENTRY*Identify)() = (char*(APIENTRY*)()) Library->QueryAddress ( "Identify" ) ;
   if ( Identify == 0 ) {
      delete Library ;
      return ( 0 ) ;
   } /* endif */

   // Check for document import/export module.
   if ( !strcmpi ( Identify(), TYPE_IMPORTEXPORT ) ) {
      ImportExport *Entry = new ImportExport ( Library ) ;
      if ( Entry->QueryLibrary() == 0 ) {
         delete Entry ;
         return ( 0 ) ;
      } /* endif */
      return ( Entry ) ;
   } /* endif */

   // Check for graphic import module.
   if ( !strcmpi ( Identify(), TYPE_GRAPHICIMPORT ) ) {
      GraphicImporter *Entry = new GraphicImporter ( Library ) ;
      if ( Entry->QueryLibrary() == 0 ) {
         delete Entry ;
         return ( 0 ) ;
      } /* endif */
      return ( Entry ) ;
   } /* endif */

   delete Library ;
   return ( 0 ) ;
}

/****************************************************************************
 *                                                                          *
 *  Add-on Extension Entry: Constructor for external module (DLL loaded).   *
 *                                                                          *
 ****************************************************************************/

Addon::Addon ( Module *library ) : 
   Type(0), Library(library), Identify(0), SetLanguage(0), ProductName(0), ProductVersion(0), 
   ProductInformation(0), ProductConfigure(0), Id(0), Prev(0), Next(0) {

   #ifdef DEBUG
      Log ( "Addon(%08X)::Addon(Module=%08X): Started.", this, library ) ;
   #endif

   // Get module type function.
   Identify = (char*(APIENTRY*)()) Library->QueryAddress ( "Identify" ) ;
   if ( Identify == 0 ) {
      #ifdef DEBUG
         Log ( "Addon(%08X)::Addon: Unable to find module type function for module %08X.", this, library ) ;
      #endif
      delete Library ;
      Library = 0 ;
      return ;
   } /* endif */

   // Get module language selection function.
   SetLanguage = (BOOL(APIENTRY*)(char*)) Library->QueryAddress ( "SetLanguage" ) ;
   if ( SetLanguage == 0 ) {
      #ifdef DEBUG
         Log ( "Addon(%08X)::Addon: Unable to find language select function for module %08X.", this, library ) ;
      #endif
      delete Library ;
      Library = 0 ;
      return ;
   } /* endif */

   // Get module name function.
   ProductName = (char*(APIENTRY*)()) Library->QueryAddress ( "ProductName" ) ;
   if ( ProductName == 0 ) {
      #ifdef DEBUG
         Log ( "Addon(%08X)::Addon: Unable to find product name function for module %08X.", this, library ) ;
      #endif
      delete Library ;
      Library = 0 ;
      return ;
   } /* endif */

   // Get module version function.
   ProductVersion = (int(APIENTRY*)()) Library->QueryAddress ( "ProductVersion" ) ;
   if ( ProductVersion == 0 ) {
      #ifdef DEBUG
         Log ( "Addon(%08X)::Addon: Unable to find product version function for module %08X.", this, library ) ;
      #endif
      delete Library ;
      Library = 0 ;
      return ;
   } /* endif */

   // Get module product information dialog function.
   ProductInformation = (void(APIENTRY*)(HWND)) Library->QueryAddress ( "ProductInformation" ) ;
   if ( ProductInformation == 0 ) {
      #ifdef DEBUG
         Log ( "Addon(%08X)::Addon: Unable to find product information function for module %08X.", this, library ) ;
      #endif
      delete Library ;
      Library = 0 ;
      return ;
   } /* endif */

   // Get the optional module product information dialog function.
   ProductConfigure = (int(APIENTRY*)(int,char*,char*,char*,PWINDOWPROC*,HMODULE*,int*)) Library->QueryAddress ( "ProductConfigure" ) ;

   #ifdef DEBUG
      Log ( "Addon(%08X)::Addon: Loaded extender module from library %08X.", this, library ) ;
   #endif
} /* endmethod */

/****************************************************************************
 *                                                                          *
 *  Add-on Extension Entry: Constructor for external module (load DLL).     *
 *                                                                          *
 ****************************************************************************/

Addon::Addon ( char *Path ) :
   Type(0), Library(0), Identify(0), SetLanguage(0), ProductName(0), ProductVersion(0), 
   ProductInformation(0), ProductConfigure(0), Id(0), Prev(0), Next(0) {

   #ifdef DEBUG
      Log ( "Addon(%08X)::Addon(Path='%s'): Started.", this, Path ) ;
   #endif

   // First, try to load the module specified.
   Library = new Module ( Path, FALSE ) ;
   if ( Library->QueryHandle() == 0 ) {
      #ifdef DEBUG
         Log ( "Addon(%08X)::Addon: Unable to load module '%s'.", this, Path ) ;
      #endif
      delete Library ;
      Library = 0 ;
      return ;
   } /* endif */

   // Get module type function.
   Identify = (char*(APIENTRY*)()) Library->QueryAddress ( "Identify" ) ;
   if ( Identify == 0 ) {
      #ifdef DEBUG
         Log ( "Addon(%08X)::Addon: Unable to find module type function for '%s'.", this, Path ) ;
      #endif
      delete Library ;
      Library = 0 ;
      return ;
   } /* endif */

   // Get module language selection function.
   SetLanguage = (BOOL(APIENTRY*)(char*)) Library->QueryAddress ( "SetLanguage" ) ;
   if ( SetLanguage == 0 ) {
      #ifdef DEBUG
         Log ( "Addon(%08X)::Addon: Unable to find language select function for '%s'.", this, Path ) ;
      #endif
      delete Library ;
      Library = 0 ;
      return ;
   } /* endif */

   // Get module name function.
   ProductName = (char*(APIENTRY*)()) Library->QueryAddress ( "ProductName" ) ;
   if ( ProductName == 0 ) {
      #ifdef DEBUG
         Log ( "Addon(%08X)::Addon: Unable to find product name function for '%s'.", this, Path ) ;
      #endif
      delete Library ;
      Library = 0 ;
      return ;
   } /* endif */

   // Get module version function.
   ProductVersion = (int(APIENTRY*)()) Library->QueryAddress ( "ProductVersion" ) ;
   if ( ProductVersion == 0 ) {
      #ifdef DEBUG
         Log ( "Addon(%08X)::Addon: Unable to find product version function for '%s'.", this, Path ) ;
      #endif
      delete Library ;
      Library = 0 ;
      return ;
   } /* endif */

   // Get module product information dialog function.
   ProductInformation = (void(APIENTRY*)(HWND)) Library->QueryAddress ( "ProductInformation" ) ;
   if ( ProductInformation == 0 ) {
      #ifdef DEBUG
         Log ( "Addon(%08X)::Addon: Unable to find product information function for '%s'.", this, Path ) ;
      #endif
      delete Library ;
      Library = 0 ;
      return ;
   } /* endif */

   // Get the optional module product information dialog function.
   ProductConfigure = (int(APIENTRY*)(int,char*,char*,char*,PWINDOWPROC*,HMODULE*,int*)) Library->QueryAddress ( "ProductConfigure" ) ;

   #ifdef DEBUG
      Log ( "Addon(%08X)::Addon: Loaded extender module '%s'.", this, Path ) ;
   #endif
} /* endmethod */

/****************************************************************************
 *                                                                          *
 *  Add-on Extension Entry: Constructor for internal module.                *
 *                                                                          *
 ****************************************************************************/

Addon::Addon ( void *type ) :
   Type(0), Library(0), Identify(0), SetLanguage(0), ProductName(0), ProductVersion(0), 
   ProductInformation(0), Id(0), Prev(0), Next(0) {

   #ifdef DEBUG
      Log ( "Addon(%08X)::Addon(Type='%s'): Started.", this, type ) ;
   #endif

   // Save the type name.
   Type = (char*) malloc ( strlen((char*)type) + 1 ) ;
   strcpy ( Type, (char*)type ) ;

   #ifdef DEBUG
      Log ( "Addon(%08X)::Addon(Type='%s'): Done.", this, Type ) ;
   #endif

} /* endmethod */

/****************************************************************************
 *                                                                          *
 *  Add-on Extension Entry: Copy Constructor                                *
 *                                                                          *
 ****************************************************************************/

Addon::Addon ( const Addon & object ) :
   Identify(object.Identify), SetLanguage(object.SetLanguage), ProductName(object.ProductName),
   ProductVersion(object.ProductVersion), ProductInformation(object.ProductInformation),
   ProductConfigure(object.ProductConfigure), Id(object.Id), Prev(0), Next(0), Library(0) {

   #ifdef DEBUG
      Log ( "Addon(%08X)::Addon(Addon=%08X): Started.", this, &object ) ;
   #endif

   Library = object.Library ;

   Type = (char*) malloc ( strlen(object.Type) + 1 ) ;
   strcpy ( Type, object.Type ) ;

} /* endmethod */

/****************************************************************************
 *                                                                          *
 *  Add-on Extension Entry: Destructor                                      *
 *                                                                          *
 ****************************************************************************/

Addon::~Addon ( ) {

   #ifdef DEBUG_DESTRUCTOR
      Log ( "Addon(%08X)::~Addon Started,  Type '%s'", this, QueryType() ) ;
   #endif

   if ( Library ) 
      delete Library ;

   if ( Type ) 
      free ( Type ) ;

   #ifdef DEBUG_DESTRUCTOR
      Log ( "Addon(%08X)::~Addon Done.", this ) ;
   #endif

} /* endmethod */

/****************************************************************************
 *                                                                          *
 *  Add-on Extension Entry: Add to Menu                                     *
 *                                                                          *
 ****************************************************************************/

void Addon::AddToMenu ( HWND Menu ) {

   #ifdef DEBUG
      Log ( "Addon(%08X)::AddToMenu(Menu=%08X) Started.", this, Menu ) ;
   #endif

   MENUITEM MenuItem ;
   MenuItem.iPosition = MIT_END ;
   MenuItem.afAttribute = 0 ;
   MenuItem.hwndSubMenu = 0 ;
   MenuItem.id = Id ;
   MenuItem.afStyle = MIS_TEXT ;
   MenuItem.hItem = 0 ;

   #ifdef DEBUG
      Log ( "Addon(%08X)::AddToMenu(Menu=%08X) ProductName()=%p, ProductVersion()=%p.", this, Menu, ProductName, ProductVersion ) ;
   #endif

   char Label [80] ;
   sprintf ( Label, "%s %.2lf", ProductName ? ProductName() : "??", double(ProductVersion?ProductVersion():0)/100.0 ) ;

   #ifdef DEBUG
      Log ( "Addon(%08X)::AddToMenu(Menu=%08X) Label='%s'.", this, Menu, Label ) ;
   #endif

   Sys_SendMessage ( Menu, MM_INSERTITEM, MPFROMP(&MenuItem), MPFROMP(Label) ) ;

   #ifdef DEBUG
      Log ( "Addon(%08X)::AddToMenu(Menu=%08X) Done.", this, Menu ) ;
   #endif
} /* endmethod */

/****************************************************************************
 *                                                                          *
 *  Add-on Extension Entry: Show Product Information                        *
 *                                                                          *
 ****************************************************************************/

void Addon::ShowProductInformation ( HWND Owner ) {
   if ( ProductInformation ) 
      ProductInformation ( Owner ) ;
} /* endmethod */

/****************************************************************************
 *                                                                          *
 *  Add-on Extension Entry: Dump                                            *
 *                                                                          *
 ****************************************************************************/

void Addon::Dump ( int indent ) {
   Log ( "%*sDump of Addon(%08X)",  indent, "", this ) ;
   Log ( "%*s  Type:           '%s'",  indent, "", Type ) ;
   Log ( "%*s  ID:             %i",    indent, "", Id ) ;
   Log ( "%*s  Prev,Next:      %p,%p", indent, "", Prev, Next ) ;
   Log ( "%*s  Module Handle:  %i",    indent, "", Library->QueryHandle() ) ;
   Log ( "%*s  Identify:       '%s'",  indent, "", Identify() ) ;
   Log ( "%*s  ProductName:    '%s'",  indent, "", ProductName() ) ;
   Log ( "%*s  ProductVersion: %.2lf", indent, "", double(ProductVersion())/100.0 ) ;
}

/****************************************************************************
 *                                                                          *
 *  Add-on Extension Registry: Constructor                                  *
 *                                                                          *
 ****************************************************************************/

AddonRegistry::AddonRegistry ( int FirstID ) : 
   Lock("AddonRegistry::Lock"), Building("AddonRegistry::Building"), Built("AddonRegistry::Built"), 
   First(0), Last(0), NextID(FirstID) {

   #ifdef DEBUG_LOCKS
      Lock.SetDebug ( TRUE ) ;
      Building.SetDebug ( TRUE ) ;
      Built.SetDebug ( TRUE ) ;
   #endif // DEBUG_LOCKS

   strcpy ( DefaultLanguage, "" ) ;  // Clear the language (no default constructor).

   Built.Post ( ) ;  // Set the event just in case the Build command never arrives.

} /* endmethod */

/****************************************************************************
 *                                                                          *
 *  Add-on Extension Registry: Destructor                                   *
 *                                                                          *
 ****************************************************************************/

AddonRegistry::~AddonRegistry ( ) {

   Built.Wait ( ) ;  // Wait for the builder to complete, if it ever started.

   Clear ( ) ;  // Clear the addon list, invoking everyone's destructors.

} /* endmethod */

/****************************************************************************
 *                                                                          *
 *  Add-on Extension Registry: Clear the List                               *
 *                                                                          *
 ****************************************************************************/

void AddonRegistry::Clear ( ) {
   Lock.Request ( ) ;
   while ( First ) {
      Addon *Next = First->QueryNext() ;
      #ifdef DEBUG_DESTRUCTOR
         Log ( "AddonRegistry(%08X)::Clear() Deleting addon %08X.", this, First ) ;
      #endif // DEBUG_DESTRUCTOR
      delete First ;
      #ifdef DEBUG_DESTRUCTOR
         Log ( "AddonRegistry(%08X)::Clear() Delete completed.", this ) ;
      #endif // DEBUG_DESTRUCTOR
      First = Next ;
   } /* endwhile */
   First = Last = 0 ;
   Lock.Release ( ) ;
} /* endmethod */

/****************************************************************************
 *                                                                          *
 *  Add-on Extension Registry: Add Entry                                    *
 *                                                                          *
 ****************************************************************************/

void AddonRegistry::Add ( Addon *Entry ) {
   Lock.Request ( ) ;
   Entry->SetId ( NextID++ ) ;
   Entry->SetPrev ( Last ) ;
   Entry->SetNext ( 0 ) ;
   if ( Last ) {
      Last->SetNext ( Entry ) ;
   } else {
      First = Entry ;
   } /* endif */
   Last = Entry ;
   Lock.Release ( ) ;
} /* endmethod */

/****************************************************************************
 *                                                                          *
 *  Add-on Extension Registry: Add all products to menu                     *
 *                                                                          *
 ****************************************************************************/

void AddonRegistry::AddToMenu ( HWND Menu ) {

   Lock.Request ( ) ;

   Addon *Entry = First ;
   while ( Entry ) {

      Entry->AddToMenu ( Menu ) ;

      Entry = Entry->QueryNext() ;

   } /* endwhile */

   Lock.Release ( ) ;
} /* endmethod */

/****************************************************************************
 *                                                                          *
 *  Add-on Extension Registry: Show Product Information                     *
 *                                                                          *
 ****************************************************************************/

void AddonRegistry::ShowProductInformation ( int Id, HWND Owner ) {

   Lock.Request ( ) ;

   Addon *Entry = First ;
   while ( Entry ) {

      if ( Id == Entry->QueryId() ) {
         Entry->ShowProductInformation ( Owner ) ;
         break ;
      } /* endif */

      Entry = Entry->QueryNext() ;

   } /* endwhile */

   Lock.Release ( ) ;
} /* endmethod */

/****************************************************************************
 *                                                                          *
 *  Add-on Extension Registry: Select Language                              *
 *                                                                          *
 ****************************************************************************/

void AddonRegistry::SelectLanguage ( char *Language ) {
   Lock.Request ( ) ;
   Addon *Entry = First ;
   while ( Entry ) {
      Entry->SelectLanguage ( Language ) ;
      Entry = Entry->QueryNext() ;
   } /* endwhile */
   Lock.Release ( ) ;
} /* endmethod */

/****************************************************************************
 *                                                                          *
 *  Add-on Extension Registry: Find First Entry of given Type               *
 *                                                                          *
 ****************************************************************************/

Addon *AddonRegistry::FindFirstOfType ( char *Type ) {

   Lock.Request ( ) ;

   Addon *Entry = First ;
   while ( Entry ) {

      if ( !stricmp ( Type, Entry->QueryType() ) ) 
         break ;

      Entry = Entry->QueryNext() ;

   } /* endwhile */

   Lock.Release ( ) ;

   return ( Entry ) ;

} /* endmethod */

/****************************************************************************
 *                                                                          *
 *  Add-on Extension Registry: Find First Entry of given Type               *
 *                                                                          *
 ****************************************************************************/

Addon *AddonRegistry::FindNextOfType ( Addon *Entry ) {

   Lock.Request ( ) ;

   char *Type = Entry->QueryType() ;

   Entry = Entry->QueryNext() ;

   while ( Entry ) {

      if ( !stricmp ( Type, Entry->QueryType() ) ) 
         break ;

      Entry = Entry->QueryNext() ;

   } /* endwhile */

   Lock.Release ( ) ;

   return ( Entry ) ;

} /* endmethod */

/****************************************************************************
 *                                                                          *
 *  Add-on Extension Registry: Count Configurable Extensions                *
 *                                                                          *
 ****************************************************************************/

int AddonRegistry::QueryConfigureCount ( ) {

   int PageCount ;                      // Number of notebook pages defined.
   char Name            [20][64] ;      // Notebook page names.
   char TabMajor        [20][32] ;      // Notebook page tabs (major).
   char TabMinor        [20][32] ;      // Notebook page tabs (minor).
   PWINDOWPROC Processor[20] ;          // Notebook page dialog processors.
   HMODULE Library      [20] ;          // Notebook page definition library handles.
   int DialogID         [20] ;          // Notebook page definition IDs.

   Lock.Request ( ) ;

   PageCount = 0 ;
   Addon *Entry = First ;
   while ( Entry ) {
      PageCount += Entry->QueryConfigureInfo ( 20, Name[0], TabMajor[0], TabMinor[0], &Processor[0], &Library[0], &DialogID[0] ) ;
      Entry = Entry->QueryNext() ;
   } /* endwhile */

   Lock.Release ( ) ;

   return ( PageCount ) ;

} /* endmethod */

/****************************************************************************
 *                                                                          *
 *  Add-on Extension Registry: Configure                                    *
 *                                                                          *
 ****************************************************************************/

#define MAX_PAGES 100

typedef struct {
   int PageCount ;                              // Number of notebook pages defined.
   char Name            [MAX_PAGES] [64] ;      // Notebook page names.
   char TabMajor        [MAX_PAGES] [32] ;      // Notebook page tabs (major).
   char TabMinor        [MAX_PAGES] [32] ;      // Notebook page tabs (minor).
   PWINDOWPROC Processor[MAX_PAGES] ;           // Notebook page dialog processors.
   HMODULE Library      [MAX_PAGES] ;           // Notebook page definition library handles.
   int DialogID         [MAX_PAGES] ;           // Notebook page definition IDs.
   HWND ErrorField ;                            // Dialog error field handle (internal use only).
} CONFIGURE_PARMS, *PCONFIGURE_PARMS ;

static MRESULT EXPENTRY Configure_Processor ( HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2 ) {

   switch ( msg ) {

      case WM_INITDLG: {
         PCONFIGURE_PARMS Parms = PCONFIGURE_PARMS ( PVOIDFROMMP ( mp2 ) ) ;
         Sys_SetWindowData ( hwnd, Parms ) ;
         Parms->ErrorField = CHILD ( hwnd, IDD_CONFIGURE_ERR ) ;

         HWND hwndNotebook = CHILD ( hwnd, IDD_CONFIGURE_NOTEBOOK ) ;
         Sys_SendMessage ( hwndNotebook, BKM_SETNOTEBOOKCOLORS,
            MPFROMLONG(SYSCLR_FIELDBACKGROUND), MPFROMSHORT(BKA_BACKGROUNDPAGECOLORINDEX) ) ;

         long MajorTabWidth = 0 ;
         HPS hPS = WinGetPS ( hwnd ) ;
         FONTMETRICS FontMetrics ;
         GpiQueryFontMetrics ( hPS, sizeof(FontMetrics), &FontMetrics ) ;
         HWND Dialog0 ;
         for ( int i=0; i<Parms->PageCount; i++ ) {
            int PageNumber(1), PageCount(1) ;  char *Name(0);
            for ( int j=i; j>=0; j-- ) 
               if ( Parms->Name[j][0] ) 
                  break ;
            Name = Parms->Name[j] ;
            for ( j=j+1; j<Parms->PageCount && !Parms->Name[j][0]; j++ ) {
               PageCount ++ ;
               if ( j <= i ) 
                  PageNumber ++ ;
            } /* endfor */
            ULONG PageHandle = ULONG ( Sys_SendMessage ( hwndNotebook, BKM_INSERTPAGE, MPFROMLONG(0), 
               MPFROM2SHORT( ( Parms->TabMajor[i][0] ? BKA_MAJOR : 0 ) | BKA_STATUSTEXTON, BKA_LAST ) ) ) ;
            char StatusText [80] ;
            if ( PageCount == 1 ) {
               strcpy ( StatusText, Parms->Name[i] ) ;
            } else {
               ResourceString Page ( LibraryHandle, IDS_PAGE ) ;
               ResourceString Of ( LibraryHandle, IDS_OF ) ;
               sprintf ( StatusText, "%s, %s %i %s %i", Name, PCHAR(Page), PageNumber, PCHAR(Of), PageCount ) ;
            } /* endif */
            Sys_SendMessage ( hwndNotebook, BKM_SETSTATUSLINETEXT, MPFROMLONG(PageHandle), MPFROMP(StatusText) ) ;
            if ( Parms->TabMajor[i][0] ) {
               Sys_SendMessage ( hwndNotebook, BKM_SETTABTEXT, MPFROMLONG(PageHandle), MPFROMP(Parms->TabMajor[i][0]?Parms->TabMajor[i]:"") ) ;
               POINTL TextBox [TXTBOX_COUNT] ;
               GpiQueryTextBox ( hPS, strlen(Parms->TabMajor[i]), PCH(Parms->TabMajor[i]), TXTBOX_COUNT, TextBox ) ;
               MajorTabWidth = max ( MajorTabWidth, TextBox[TXTBOX_CONCAT].x ) ;
            } /* endif */
            HWND Dialog = WinLoadDlg ( hwndNotebook, hwnd, Parms->Processor[i], Parms->Library[i], Parms->DialogID[i], &Parms->ErrorField );
            Sys_SendMessage ( hwndNotebook, BKM_SETPAGEWINDOWHWND, MPFROMP(PageHandle), MPFROMLONG(Dialog) ) ;
            if ( i == 0 ) { 
               PAGESELECTNOTIFY Info = { hwndNotebook, PageHandle, PageHandle } ;
               Sys_SendMessage ( Dialog, WM_CONTROL, MPFROM2SHORT(IDD_CONFIGURE_NOTEBOOK,BKN_PAGESELECTED), MPFROMP(&Info) ) ;
               Dialog0 = Dialog ;
            } /* endif */
         } /* endfor */
         Sys_SendMessage ( hwndNotebook, BKM_SETDIMENSIONS,
            MPFROM2SHORT( SHORT(MajorTabWidth+FontMetrics.lAveCharWidth*2), SHORT(((FontMetrics.lMaxAscender+FontMetrics.lMaxDescender)*3)/2) ),
            MPFROMSHORT( BKA_MAJORTAB ) ) ;
         WinReleasePS ( hPS ) ;
         Sys_SetFocus ( Dialog0 ) ;
         Dialog_Processor ( hwnd, msg, mp1, mp2 ) ;
         return ( MRFROMSHORT ( TRUE ) ) ; } /* endcase */

      case WM_CONTROL: {
//       PCONFIGURE_PARMS Parms = PCONFIGURE_PARMS ( Sys_GetWindowData ( hwnd ) ) ;
         SHORT Id = SHORT1FROMMP ( mp1 ) ;
         SHORT Message = SHORT2FROMMP ( mp1 ) ;
         switch ( Id ) {
            case IDD_CONFIGURE_NOTEBOOK: {
               switch ( Message ) {
                  case BKN_PAGESELECTEDPENDING: {
                     PAGESELECTNOTIFY *pInfo = (PAGESELECTNOTIFY*) PVOIDFROMMP(mp2) ;
                     HWND Dialog = HWND ( Sys_SendMessage ( pInfo->hwndBook, BKM_QUERYPAGEWINDOWHWND, MPFROMLONG(pInfo->ulPageIdCur), 0 ) ) ;
                     return ( Sys_SendMessage ( Dialog, msg, mp1, mp2 ) ) ; } /* endcase */
                  case BKN_PAGESELECTED: {
                     PAGESELECTNOTIFY *pInfo = (PAGESELECTNOTIFY*) PVOIDFROMMP(mp2) ;
                     HWND Dialog = HWND ( Sys_SendMessage ( pInfo->hwndBook, BKM_QUERYPAGEWINDOWHWND, MPFROMLONG(pInfo->ulPageIdNew), 0 ) ) ;
                     return ( Sys_SendMessage ( Dialog, msg, mp1, mp2 ) ) ; } /* endcase */
               } /* endswitch */
               break ; } /* endcase */
         } /* endswitch */
         return ( 0 ) ; } /* endcase */

      case WM_COMMAND: {
//       PCONFIGURE_PARMS Parms = PCONFIGURE_PARMS ( Sys_GetWindowData ( hwnd ) ) ;
         switch ( SHORT1FROMMP ( mp1 ) ) {
            case DID_OK: {
               HWND hwndNotebook = CHILD(hwnd,IDD_CONFIGURE_NOTEBOOK) ;
               ULONG Page = ULONG ( Sys_SendMessage ( hwndNotebook, BKM_QUERYPAGEID, 0, MPFROM2SHORT(BKA_FIRST,0) ) ) ;
               while ( Page ) {
                  PAGESELECTNOTIFY Info = { hwndNotebook, Page, Page } ;
                  HWND hwndPage = HWND ( Sys_SendMessage ( hwndNotebook, BKM_QUERYPAGEWINDOWHWND, MPFROMLONG(Page), 0 ) ) ;
                  Sys_SendMessage ( hwndPage, WM_CONTROL, MPFROM2SHORT(IDD_CONFIGURE_NOTEBOOK,BKN_PAGESELECTEDPENDING), MPFROMP(&Info) ) ;
                  if ( Info.ulPageIdNew == 0 ) {
                     Sys_SendMessage ( hwndNotebook, BKM_TURNTOPAGE, MPFROMLONG(Page), 0 ) ;
                     Sys_SetFocus ( hwndPage ) ;
                     return ( FALSE ) ;
                  } /* endif */
                  Page = ULONG ( Sys_SendMessage ( hwndNotebook, BKM_QUERYPAGEID, MPFROMLONG(Page), MPFROM2SHORT(BKA_NEXT,0) ) ) ;
               } /* endwhile */
               Sys_EndDialog ( hwnd, TRUE ) ;
               return ( 0 ) ; } /* endcase */
            case DID_CANCEL: {
               Sys_EndDialog ( hwnd, FALSE ) ;
               return ( 0 ) ; } /* endcase */
         } /* endswitch */
         return ( 0 ) ; } /* endcase */

   } /* endswitch */

   return ( Dialog_Processor ( hwnd, msg, mp1, mp2 ) ) ;

} /* endmethod */

void AddonRegistry::Configure ( HWND Owner ) {

   CONFIGURE_PARMS Parms = { 0 } ;
   memset ( &Parms, 0, sizeof(Parms) ) ;

   Lock.Request ( ) ;
   Addon *Entry = First ;
   while ( Entry ) {
      Parms.PageCount += Entry->QueryConfigureInfo ( 
         MAX_PAGES - Parms.PageCount,
         Parms.Name[Parms.PageCount], 
         Parms.TabMajor[Parms.PageCount],
         Parms.TabMinor[Parms.PageCount],
         &Parms.Processor[Parms.PageCount], 
         &Parms.Library[Parms.PageCount],
         &Parms.DialogID[Parms.PageCount] ) ;
      Entry = Entry->QueryNext() ;
   } /* endwhile */
   Lock.Release ( ) ;

   WinDlgBox ( HWND_DESKTOP, Owner, Configure_Processor, LibraryHandle, IDD_CONFIGURE_EXTENSIONS, &Parms ) ;

} /* endmethod */

/****************************************************************************
 *                                                                          *
 *  Add-on Extension Registry: Dump                                         *
 *                                                                          *
 ****************************************************************************/

void AddonRegistry::Dump ( char *Title ) {
   Log ( "AddonRegistry(%08X) Dump '%s'", this, Title ) ;
   Log ( "  NextID: %i", NextID ) ;
   Log ( "  First, Last: %p, %p", First, Last ) ;
   Lock.Request ( ) ;
   Addon *Entry = First ;
   while ( Entry ) {
      Entry->Dump ( 2 ) ;
      Entry = Entry->QueryNext() ;
   } /* endwhile */
   Lock.Release ( ) ;
} /* endmethod */
 
/****************************************************************************
 *                                                                          *
 *  Add-on Extension Registry: Start Build Thread                           *
 *                                                                          *
 ****************************************************************************/

void AddonRegistry::Build ( char *Language ) {
   strcpy ( DefaultLanguage, Language ) ;
   StartThread ( "AddonRegistry_BuildThread", AddonRegistry_BuildThread, 0x20000, this ) ;
   Building.Wait ( ) ; 
} /* endmethod */

/****************************************************************************
 *                                                                          *
 *  Add-on Extension Registry: Build Thread                                 *
 *                                                                          *
 ****************************************************************************/

extern void AddonRegistry_BuildThread ( void *Parameter ) {

   AddonRegistry *Registry = (AddonRegistry*) Parameter ;
   Registry->Lock.Request ( ) ;
   Registry->Built.Reset ( ) ;
   Registry->Building.Post ( ) ;

   char Wildcard [CCHMAXPATH] ;
   strcpy ( Wildcard, HomePath ) ;
   strcat ( Wildcard, "\\*.DLL" ) ;

   FILEFINDBUF3 Found ;
   HDIR hDir = (HDIR) HDIR_CREATE ;
   ULONG Count = 1 ;
   if ( !DosFindFirst2 ( Wildcard, &hDir, FILE_NORMAL, &Found, sizeof(Found), &Count, FIL_STANDARD ) ) {
      do {
         char Filename [CCHMAXPATH] ;
         strcpy ( Filename, HomePath ) ;
         strcat ( Filename, "\\" ) ;
         strcat ( Filename, Found.achName ) ;
         Addon *Entry = Addon_Create ( Filename ) ;
         if ( Entry ) {
            Entry->SelectLanguage ( Registry->DefaultLanguage ) ;
            if ( Entry->QueryName() && *Entry->QueryName() ) {
               Registry->Add ( Entry ) ;
            } else {
               delete Entry ;
            } /* endif */
         } /* endif */
      } while ( !DosFindNext ( hDir, &Found, sizeof(Found), &Count ) ) ;
      DosFindClose ( hDir ) ;
   } /* endif */

   Registry->Lock.Release ( ) ;
   Registry->Built.Post ( ) ;
}

