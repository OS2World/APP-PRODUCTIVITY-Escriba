/***************************************************************** MODULE.CPP
 *                                                                          *
 *  Class: Resource Module                                                  *
 *                                                                          *
 ****************************************************************************/

#include "System.h"

#include <stdlib.h>
#include <string.h>

#include "Module.h"

Module::Module ( char *name ) : Handle(0) {
   if ( name && *name ) {
      Name = (char*) malloc ( strlen(name) + 1 ) ;
      strcpy ( Name, name ) ;
      char Drive[_MAX_DRIVE+1], Dir[_MAX_DIR+1], Fname[_MAX_FNAME+1], Ext[_MAX_EXT+1] ;
      _splitpath ( Name, Drive, Dir, Fname, Ext ) ;
      char ModuleName [CCHMAXPATH] ;
      strcpy ( ModuleName, Drive ) ;
      strcat ( ModuleName, Dir ) ;
      strcat ( ModuleName, Fname ) ;
      if ( *Drive || *Dir )
         strcat ( ModuleName, Ext ) ;
      strupr ( ModuleName ) ;
      char MissingModule [CCHMAXPATH] ;
      DosLoadModule ( PSZ(MissingModule), sizeof(MissingModule), PSZ(ModuleName), &Handle ) ;
   } else {
      Name = 0 ;
   } /* endif */
} /* endmethod */

Module::Module ( const Module &object ) : Name(0), Handle(0) {
   Name = (char*) malloc ( strlen(object.Name) + 1 ) ;
   strcpy ( Name, object.Name ) ;
   if ( object.Handle ) {
      char Drive[_MAX_DRIVE], Dir[_MAX_DIR], Fname[_MAX_FNAME], Ext[_MAX_EXT] ;
      _splitpath ( Name, Drive, Dir, Fname, Ext ) ;
      char ModuleName [CCHMAXPATH] ;
      strcpy ( ModuleName, Drive ) ;
      strcat ( ModuleName, Dir ) ;
      strcat ( ModuleName, Fname ) ;
      if ( *Drive || *Dir )
         strcat ( ModuleName, Ext ) ;
      strupr ( ModuleName ) ;
      char MissingModule [CCHMAXPATH] ;
      DosLoadModule ( PSZ(MissingModule), sizeof(MissingModule), PSZ(ModuleName), &Handle ) ;
   } /* endif */
} /* endmethod */

Module::~Module ( ) {
   if ( Handle )
      DosFreeModule ( Handle ) ;
   if ( Name ) 
      free ( Name ) ;
} /* endmethod */

PFN Module::QueryAddress ( char *Name ) {
   PFN Address ;
   if ( DosQueryProcAddr ( Handle, 0, PSZ(Name), (PFN*)&Address ) )
      return ( 0 ) ;
   return ( Address ) ;
} /* endmethod */

PFN Module::QueryAddress ( int Ordinal ) {
   PFN Address ;
   if ( DosQueryProcAddr ( Handle, Ordinal, 0, (PFN*)&Address ) )
      return ( 0 ) ;
   return ( Address ) ;
} /* endmethod */

