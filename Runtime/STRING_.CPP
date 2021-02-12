/***************************************************************** STRING.CPP
 *                                                                          *
 *                       Generic String Object Class                        *
 *                                                                          *
 ****************************************************************************/

#include "System.h"

#include <stdlib.h>
#include <string.h>

#include "Debug.h"
#include "String_.h"

//#define DEBUG

String::String ( unsigned char *Value ) {
   #ifdef DEBUG
   Log ( "String(%p)::String(\"%s\") started.", this, Value ) ;
   #endif

   Data = NULL ;
   Len = 0 ;

   if ( Value && *Value ) {
      Len = strlen ( (char *)Value ) ;
      Data = new char [ Len + 1 ] ;
      strcpy ( Data, (char *)Value ) ;
   } /* endif */

   #ifdef DEBUG
   Log ( "String(%p)::String(\"%s\") done.", this, Value ) ;
   #endif
} /* endmethod */

String::String ( char *Value ) {
   #ifdef DEBUG
   Log ( "String(%p)::String(\"%s\") started.", this, Value ) ;
   #endif

   Data = NULL ;
   Len = 0 ;

   if ( Value && *Value ) {
      Len = strlen ( Value ) ;
      Data = new char [ Len + 1 ] ;
      strcpy ( Data, Value ) ;
   } /* endif */

   #ifdef DEBUG
   Log ( "String(%p)::String(\"%s\") done.", this, Value ) ;
   #endif
} /* endmethod */

String::String ( ) {
   #ifdef DEBUG
   Log ( "String(%p)::String() started.", this ) ;
   #endif

   Data = NULL ;
   Len = 0 ;

   #ifdef DEBUG
   Log ( "String(%p)::String() done.", this ) ;
   #endif
} /* endmethod */

String::~String ( ) {
   #ifdef DEBUG
   Log ( "String(%p)::~String() started.", this ) ;
   #endif

   if ( Data ) {
      #ifdef DEBUG
      Log ( "String(%p)::~String(): Deleting data string \"%s\".", this, Data ) ;
      #endif

      delete [] Data ;
      Data = 0 ;
   } /* endif */

   #ifdef DEBUG
   Log ( "String(%p)::~String() done.", this ) ;
   #endif
} /* endmethod */

String::String ( const String & Object ) {
   #ifdef DEBUG
   Log ( "String(%p)::String(%p) started.", this, &Object ) ;
   #endif

   Data = NULL ;
   Len = 0 ;

   if ( Object.Data && *Object.Data ) {
      Len = strlen ( Object.Data ) ;
      Data = new char [ Len + 1 ] ;
      strcpy ( Data, Object.Data ) ;
   } /* endif */

   #ifdef DEBUG
   Log ( "String(%p)::String(%p) done.", this, &Object ) ;
   #endif
} /* endmethod */

String & String::operator= ( const String & Object ) {
   #ifdef DEBUG
   Log ( "String(%p)::operator=(%p) started.", this, &Object ) ;
   #endif

   if ( Data ) {
      #ifdef DEBUG
      Log ( "String(%p)::operator=(): Deleting data string \"%s\".", this, Data ) ;
      #endif
      delete [] Data ;
      Data = 0 ;
   } /* endif */

   Data = NULL ;
   Len = 0 ;

   if ( Object.Data && *Object.Data ) {
      Len = strlen ( Object.Data ) ;
      Data = new char [ Len + 1 ] ;
      strcpy ( Data, Object.Data ) ;
   } /* endif */

   #ifdef DEBUG
   Log ( "String(%p)::operator=(%p) done.", this, &Object ) ;
   #endif

   return ( *this ) ;
} /* endmethod */

// Assignment Operator (String = unsigned char *)
String & String::operator= ( const unsigned char *Value ) {
   #ifdef DEBUG
   Log ( "String(%p)::operator=(\"%s\") started.", this, Value ) ;
   #endif

   if ( Data ) {
      #ifdef DEBUG
      Log ( "String(%p)::operator=(): Deleting data string \"%s\".", this, Data ) ;
      #endif
      delete [] Data ;
      Data = 0 ;
   } /* endif */

   Data = NULL ;
   Len = 0 ;

   if ( Value && *Value ) {
      Len = strlen ( (const char *)Value ) ;
      Data = new char [ Len + 1 ] ;
      strcpy ( Data, (const char *)Value ) ;
   } /* endif */

   #ifdef DEBUG
   Log ( "String(%p)::operator=(\"%s\") done.", this, Value ) ;
   #endif

   return ( *this ) ;
} /* endmethod */

// Assignment Operator (String = char *)
String & String::operator= ( const char *Value ) {
   #ifdef DEBUG
   Log ( "String(%p)::operator=(\"%s\") started.", this, Value ) ;
   #endif

   if ( Data ) {
      #ifdef DEBUG
      Log ( "String(%p)::operator=(): Deleting data string \"%s\".", this, Data ) ;
      #endif
      delete [] Data ;
      Data = 0 ;
   } /* endif */

   Data = NULL ;
   Len = 0 ;

   if ( Value && *Value ) {
      Len = strlen ( Value ) ;
      Data = new char [ Len + 1 ] ;
      strcpy ( Data, Value ) ;
   } /* endif */

   #ifdef DEBUG
   Log ( "String(%p)::operator=(\"%s\") done.", this, Value ) ;
   #endif

   return ( *this ) ;
} /* endmethod */

