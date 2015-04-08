/****************************************************************** LMODULE.H
 *                                                                          *
 *  Class: Language Module                                                  *
 *                                                                          *
 ****************************************************************************/

#ifndef LMODULE_H
#define LMODULE_H

#include "Module.h"

class LanguageModule : public Module {

   private:
      char *Language ;
      USHORT *Codepages ;

   public:
      LanguageModule ( char *AppName, char *Name = "" ) ;
      ~LanguageModule ( ) ;
      char   *QueryLanguage ( ) { return ( Language ) ; }
      USHORT *QueryCodepages ( ) { return ( Codepages ) ; }

} ;

extern LanguageModule *Language_Create ( char *Application, char *Revision, int RevisionID, char *Language, char *Wildcard ) ;

extern BOOL _SetLanguage ( char *Application, char *Revision, int RevisionID, char *Language, char *Wildcard, char *HomePath, LanguageModule* &Library ) ;

#endif
