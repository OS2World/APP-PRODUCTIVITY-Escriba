/****************************************************************** PROFILE.H
 *                                                                          *
 *                      Profile Object definitions                          *
 *                                                                          *
 ****************************************************************************/

#ifndef PROFILE_H
#define PROFILE_H

#include "System.h"

#include "HelpWin.h"

class Profile {

   private:

      HINI   Handle ;               // INI file handle.
      char  *Name ;                 // Application Name.

   public:

      Profile ( char *Name, HAB Anchor, HMODULE Library, int DialogID, HelpWindow *Help, BOOL ResetFlag ) ;
      Profile ( char *Name, HAB Anchor, char *HomePath ) ;
      ~Profile ( ) ;

      HINI QueryHandle ( ) { return ( Handle ) ; }
      char *QueryName ( ) { return ( Name ) ; }

      BOOL GetItem ( char *ItemName, void *Item, int Size ) ;
      BOOL PutItem ( char *ItemName, void *Item, int Size ) ;

      BOOL GetString ( char *ItemName, char *Item, int MaxSize ) ;
      BOOL GetString ( char *ItemName, char* &Item ) ;
      BOOL PutString ( char *ItemName, char *Item ) ;

      void Reset ( char *ItemName ) { PrfWriteProfileData ( Handle, PSZ(Name), PSZ(ItemName), 0, 0 ) ; }
      void Reset ( ) { PrfWriteProfileData ( Handle, PSZ(Name), 0, 0, 0 ) ; }
} ;

#endif
