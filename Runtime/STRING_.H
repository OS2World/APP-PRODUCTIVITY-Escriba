/******************************************************************* STRING.H
 *                                                                          *
 *                       Generic String Object Class                        *
 *                                                                          *
 ****************************************************************************/

#ifndef STRING_H
#define STRING_H

#include <string.h>

#define DEBUG_STRING

class String {

   private:

      char *Data ;
      int Len ;

   public:

      String ( unsigned char *Value ) ;
      String ( char *Value ) ;
      String ( ) ;

      ~String ( ) ;

      String ( const String & Object ) ;

      operator unsigned char * () { return ( (unsigned char *)Data ) ; }
      operator char * () { return ( Data ) ; }

      String & operator= ( const String & Object ) ;
      String & operator= ( const unsigned char *Value ) ;
      String & operator= ( const char *Value ) ;
} ;

#endif
