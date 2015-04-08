// Class RESTRING: Encapsulates the load/discard
//   logic for a resource String Table entry.

#include "System.h"

#include "Debug.h"
#include "ReString.h"


  // Constructor

ResourceString::ResourceString ( HMODULE Module, ULONG Id, const char *File, size_t Line ) : 
   SavedModule(Module), SavedId(Id), BlockPointer(0), StringPointer(0) {

   APIRET Status = DosGetResource ( Module, RT_STRING, Id/16+1, &BlockPointer ) ;
   if ( Status ) {
      Log ( "ERROR: Unable to get string resource.  Module %lu, id %lu, code %08lX.  Requested by module %s, line %i.",
         SavedModule, SavedId, Status, File, Line ) ;
      return ;
   } /* endif */

   StringPointer = PSZ(BlockPointer) + sizeof(USHORT) ;

   USHORT Index = (USHORT) ( Id % 16 ) ;
   while ( Index-- ) {
      StringPointer += * PUCHAR ( StringPointer ) ;
      StringPointer ++ ;
   } /* endwhile */

   StringPointer ++ ;

} /* endmethod */


  // Destructor

ResourceString::~ResourceString ( ) {

   if ( BlockPointer ) 
      DosFreeResource ( BlockPointer ) ;

} /* endmethod */

