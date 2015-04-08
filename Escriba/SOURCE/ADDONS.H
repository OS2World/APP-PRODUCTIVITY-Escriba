/******************************************************************* ADDONS.H
 *                                                                          *
 *                       Add-on Extension Registry                          *
 *                                                                          *
 ****************************************************************************/

#ifndef ADDONS_H
#define ADDONS_H

#include "System.h"

#include "Event.h"
#include "Module.h"
#include "Mutex.h"

extern class Addon *Addon_Create ( char *Path ) ;

/****************************************************************************
 *                                                                          *
 *                     Add-on Extension Registry Entry                      *
 *                                                                          *
 ****************************************************************************/

class Addon {

   private:
      char *Type ;

      char* (APIENTRY * Identify ) ( void ) ;
      BOOL  (APIENTRY * SetLanguage ) ( char *Language ) ;
      char* (APIENTRY * ProductName ) ( void ) ;
      int   (APIENTRY * ProductVersion ) ( void ) ;
      void  (APIENTRY * ProductInformation ) ( HWND Owner ) ;
      int   (APIENTRY * ProductConfigure )
         ( int MaxPages, char *Names, char *MajorTabs, char *MinorTabs, PWINDOWPROC Processors[], HMODULE Libraries[], int DialogIDs[] ) ;
      int Id ;
      Addon *Prev, *Next ;

   protected:
      Module *Library ;

   public:

      Addon ( Module *library ) ;       // For creating addon extension without reloading the DLL.
      Addon ( char *Path ) ;            // For creating addon extension where DLL hasn't yet been loaded.
      Addon ( void *Type ) ;            // For creating addon extension implemented internally (no DLL).
      Addon ( const Addon & object ) ;  // Copy constructor.

      virtual ~Addon ( ) ;              // Destructor.

      Module *QueryLibrary        ( ) { return ( Library ) ; }
      char   *QueryType           ( ) { return ( Identify ? Identify ( ) : Type ) ; }
      char   *QueryName           ( ) { return ( ProductName ? ProductName ( ) : "" ) ; }
      int     QueryVersion        ( ) { return ( ProductVersion ? ProductVersion ( ) : 0 ) ; }
      Addon  *QueryPrev           ( ) { return ( Prev ) ; }
      Addon  *QueryNext           ( ) { return ( Next ) ; }
      int     QueryId             ( ) { return ( Id ) ; }

      void SetPrev ( Addon *prev ) { Prev = prev ; }
      void SetNext ( Addon *next ) { Next = next ; }
      void SetId ( int id ) { Id = id ; }

      void AddToMenu ( HWND Menu ) ;
      void ShowProductInformation ( HWND Owner ) ;

      int  QueryConfigureInfo ( int MaxPages, char *Names, char *MajorTabs, char *MinorTabs, PWINDOWPROC Processors[], HMODULE Libraries[], int DialogIDs[] ) {
         return ( ProductConfigure ? ProductConfigure ( MaxPages, Names, MajorTabs, MinorTabs, Processors, Libraries, DialogIDs ) : 0 ) ;
      } /* endmethod */

      BOOL SelectLanguage ( char *Language ) { return ( SetLanguage ? SetLanguage(Language) : FALSE ) ; }

      void Dump ( int indent = 0 ) ;
} ;

/****************************************************************************
 *                                                                          *
 *                        Add-on Extension Registry                         *
 *                                                                          *
 ****************************************************************************/

class AddonRegistry {

   private:

      Mutex Lock ;
      Event Building, Built ;
      Addon *First ;
      Addon *Last ;
      int NextID ;
      char DefaultLanguage [80] ;

   public:

      AddonRegistry ( int FirstID ) ;
      ~AddonRegistry ( ) ;

      void Clear ( ) ;

      void Add ( Addon *Entry ) ;

      void AddToMenu ( HWND Menu ) ;

      void ShowProductInformation ( int Id, HWND Owner ) ;

      void SelectLanguage ( char *Language ) ;

      Addon *FindFirstOfType ( char *Type ) ;
      Addon *FindNextOfType ( Addon *Entry ) ;

      int  QueryConfigureCount ( ) ;
      void Configure ( HWND Owner ) ;

      void Dump ( char *Title ) ;

      void Build ( char *Language ) ;
      friend void AddonRegistry_BuildThread ( void *Parameter ) ;
} ;

extern void AddonRegistry_BuildThread ( void *Parameter ) ;

extern AddonRegistry AddonList ;

#endif
