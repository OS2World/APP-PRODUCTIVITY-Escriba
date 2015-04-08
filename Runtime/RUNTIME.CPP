/****************************************************************************
 *                                                                          *
 *                       DLL Initialization Function                        *
 *                                                                          *
 ****************************************************************************/

#include "Debug.h"

extern unsigned long _System _DLL_InitTerm ( unsigned long modhandle, unsigned long flag ) {

  /**************************************************************************
   * If starting up (since cleanup will not be called this way) . . .       *
   **************************************************************************/

   if ( flag == 0 ) {

     /***********************************************************************
      * Call C RunTime initialization.  Abort if error.                     *
      ***********************************************************************/

      if ( _CRT_init() == -1 )
         return ( 0 ) ;

     /***********************************************************************
      * Save the library handle.                                            *
      ***********************************************************************/

      LibraryHandle = modhandle ;

     /***********************************************************************
      * Find out where this module was loaded from.                         *
      ***********************************************************************/

      DosQueryModuleName ( LibraryHandle, sizeof(HomePath), HomePath ) ;
      char Drive[_MAX_DRIVE], Dir[_MAX_DIR], Fname[_MAX_FNAME], Ext[_MAX_EXT] ;
      _splitpath ( HomePath, Drive, Dir, Fname, Ext ) ;
      strcpy ( HomePath, Drive ) ;
      strcat ( HomePath, Dir ) ;

  /**************************************************************************
   * Else, if shutting down . . .                                           *
   **************************************************************************/

   } else {
      // Nothing to be done here.

   } /* endif */

  /**************************************************************************
   * Return NZ, indicating success.                                         *
   **************************************************************************/

   return ( 1 ) ;
}



