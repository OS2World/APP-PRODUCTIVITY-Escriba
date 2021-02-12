/***************************************************************** HOURGLAS.H
 *                                                                          *
 *                        Hourglass Wait object                             *
 *                                                                          *
 ****************************************************************************/

#ifndef HOURGLAS_H
#define HOURGLAS_H

class HourGlass {

   private:
      int Active ;
      HPOINTER OldPointer ;
      HPOINTER Pointer ;

   public:
      HourGlass ( HWND Window ) : Active ( Window != 0 ) {
         if ( Active ) {
            OldPointer = WinQueryPointer ( HWND_DESKTOP ) ;
            Pointer = WinQuerySysPointer ( HWND_DESKTOP, SPTR_WAIT, FALSE ) ;
            WinSetPointer ( HWND_DESKTOP, Pointer ) ;
         } /* endif */
      } /* endmethod */

      ~HourGlass ( ) {
         if ( Active ) {
            WinSetPointer ( HWND_DESKTOP, OldPointer ) ;
         } /* endif */
      } /* endmethod */
} ;

#endif // If not already included.
