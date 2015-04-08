// Class RESTRING: Encapsulates the load/discard
//   logic for a resource String Table entry.

#ifndef RESTRING_H
#define RESTRING_H

#include "System.h"

class ResourceString {

   private:
      HMODULE SavedModule ;
      ULONG SavedId ;

      PVOID BlockPointer ;
      PSZ StringPointer ;

   public:
      // Constructor
      ResourceString ( HMODULE Module, ULONG Id, const char *File=__FILE__, size_t Line=__LINE__ ) ;

      // Destructor
      ~ResourceString ( ) ;

      // Typecast Operators
      operator unsigned char * () {
         return ( (unsigned char *) (StringPointer?StringPointer:"") ) ;
      } /* endmethod */

      operator char * () {
         return ( (char *) (StringPointer?StringPointer:"") ) ;
      } /* endmethod */

} ;

#endif
