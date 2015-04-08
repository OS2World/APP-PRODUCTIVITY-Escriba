/******************************************************************* MODULE.H
 *                                                                          *
 *  Class: Resource Module                                                  *
 *                                                                          *
 ****************************************************************************/

#ifndef MODULE_H
#define MODULE_H

class Module {

   protected:
      char *Name ;
      HMODULE Handle ;

   public:
      Module ( char *Name = "" ) ;
      Module ( const Module &object ) ;
      virtual ~Module ( ) ;
      char *QueryName () { return ( Name ) ; }
      HMODULE QueryHandle () { return ( Handle ) ; }
      PFN QueryAddress ( char *Name ) ;
      PFN QueryAddress ( int Ordinal ) ;
} ;

#endif
