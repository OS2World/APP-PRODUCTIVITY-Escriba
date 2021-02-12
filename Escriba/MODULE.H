/******************************************************************* MODULE.H
 *                                                                          *
 *  Classes 'Module' & 'LanguageModule': DLL wrappers                       *
 *                                                                          *
 ****************************************************************************/

#ifndef MODULE_H
#define MODULE_H

#include "System.h"

class Module {

   protected:
      char *Name ;
      HMODULE Handle ;

   public:
      Module ( char *Name = "", BOOL Required = TRUE ) ;
      Module ( const Module &object ) ;
      virtual ~Module ( ) ;
      char *QueryName () { return ( Name ) ; }
      HMODULE QueryHandle () { return ( Handle ) ; }
      PFN QueryAddress ( char *Name ) ;
      PFN QueryAddress ( int Ordinal ) ;
} ;

class LanguageModule : public Module {

   private:
      char *Language ;
      USHORT *Codepages ;
      int NumberOfCodepages ;

   public:
      LanguageModule ( char *AppName, char *Name = "", BOOL Required = TRUE ) ;
      LanguageModule ( const LanguageModule &object ) ;
      ~LanguageModule ( ) ;
      char   *QueryLanguage ( ) { return ( Language ) ; }
      USHORT *QueryCodepages ( ) { return ( Codepages ) ; }

} ;

extern LanguageModule *Language_Create ( char *Application, char *Revision, int RevisionID, char *DefaultLanguage ) ;

#endif
