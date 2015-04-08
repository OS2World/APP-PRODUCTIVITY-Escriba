/*************************************************************** CONTROLS.CPP
 *                                                                          *
 *                          Subclassed Controls                             *
 *                                                                          *
 ****************************************************************************/

#include "System.h"

#include "Debug.h"
#include "Controls.h"

static WINDOWPROC  ComboEntry_Processor1 ;
static PWINDOWPROC OldComboEntry_Processor1 ;

static WINDOWPROC  ComboEntry_Processor2 ;
static PWINDOWPROC OldComboEntry_Processor2 ;

 
/****************************************************************************
 *                                                                          *
 *                     Register Subclassed Controls                         *
 *                                                                          *
 ****************************************************************************/

extern void RegisterControls ( HAB Anchor ) {

   CLASSINFO ClassInfo ;
   WinQueryClassInfo ( Anchor, WC_BUTTON, &ClassInfo ) ;
   ClassInfo.flClassStyle = (unsigned long) ( ClassInfo.flClassStyle & (unsigned long)(~CS_PUBLIC) ) ;

   if ( ! WinRegisterClass ( Anchor, PSZ(BUTTON_CLASS), PWINDOWPROC(Button_Processor),
      ClassInfo.flClassStyle, ClassInfo.cbWindowData ) )
      Log ( "WARNING: Unable to register class '%s'", BUTTON_CLASS ) ;

   WinQueryClassInfo ( Anchor, WC_COMBOBOX, &ClassInfo ) ;
   ClassInfo.flClassStyle = (unsigned long) ( ClassInfo.flClassStyle & (unsigned long)(~CS_PUBLIC) ) ;

   if ( ! WinRegisterClass ( Anchor, PSZ(COMBOBOX_CLASS), PWINDOWPROC(ComboBox_Processor),
      ClassInfo.flClassStyle, ClassInfo.cbWindowData ) )
      Log ( "WARNING: Unable to register class '%s'", COMBOBOX_CLASS ) ;

   WinQueryClassInfo ( Anchor, WC_ENTRYFIELD, &ClassInfo ) ;
   ClassInfo.flClassStyle = (unsigned long) ( ClassInfo.flClassStyle & (unsigned long)(~CS_PUBLIC) ) ;

   if ( ! WinRegisterClass ( Anchor, PSZ(ENTRYFIELD_CLASS), PWINDOWPROC(EntryField_Processor),
      ClassInfo.flClassStyle, ClassInfo.cbWindowData ) )
      Log ( "WARNING: Unable to register class '%s'", ENTRYFIELD_CLASS ) ;
}

 
/****************************************************************************
 *                                                                          *
 *                       Button Subclass Procedure                          *
 *                                                                          *
 *    This button subclass adds the following features to the default       *
 *  system button control, for use with the pushbutton style only:          *
 *                                                                          *
 *  (1) When WM_SETFOCUS message is received, a WM_CONTROL(BN_SETFOCUS)     *
 *      message is sent to the control's owner upon receiving the focus,    *
 *      and a WM_CONTROL(BN_KILLFOCUS) message is sent to the control's     *
 *      owner upon losing the focus.  The SETFOCUS and KILLFOCUS control    *
 *      messages are inconsistently implemented across the different        *
 *      system-defined controls.                                            *
 *                                                                          *
 *  (2) Upon receiving the focus, the pushbutton becomes a default button.  *
 *      Upon losing the focus, it becomes a regular button once more.       *
 *                                                                          *
 ****************************************************************************/

extern MRESULT EXPENTRY Button_Processor ( HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2 ) {

   static PWINDOWPROC DefaultProcessor = 0 ;

   switch ( msg ) {

      case WM_CREATE: {
         CLASSINFO ClassInfo ;
         WinQueryClassInfo ( WinQueryAnchorBlock(hwnd), WC_BUTTON, &ClassInfo ) ;
         DefaultProcessor = ClassInfo.pfnWindowProc ;
         break; }

      case WM_SETFOCUS: {
         USHORT Id = WinQueryWindowUShort ( hwnd, QWS_ID ) ;
         if ( mp2 ) {
            Sys_SendMessage( hwnd, BM_SETDEFAULT, MPFROMSHORT(TRUE), 0 ) ;
            Sys_SendMessage( OWNER(hwnd), WM_CONTROL, MPFROM2SHORT(Id,BN_SETFOCUS), 0 ) ;
         } else {
            Sys_SendMessage( hwnd, BM_SETDEFAULT, MPFROMSHORT(FALSE), 0 ) ;
            Sys_SendMessage( OWNER(hwnd), WM_CONTROL, MPFROM2SHORT(Id,BN_KILLFOCUS), 0 ) ;
         } /* endif */
         break; }

   } /* endswitch */

   return ( DefaultProcessor ? DefaultProcessor ( hwnd, msg, mp1, mp2 ) : 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *                      Combo-Box Subclass Procedure                        *
 *                                                                          *
 *    This combobox subclass adds the following features to the default     *
 *  system combobox control:                                                *
 *                                                                          *
 *  (1) When WM_SETFOCUS message is received, a WM_CONTROL(CBN_SETFOCUS)    *
 *      message is sent to the control's owner upon receiving the focus,    *
 *      and a WM_CONTROL(CBN_KILLFOCUS) message is sent to the control's    *
 *      owner upon losing the focus.  The SETFOCUS and KILLFOCUS control    *
 *      messages are inconsistently implemented across the different        *
 *      system-defined controls.                                            *
 *                                                                          *
 ****************************************************************************/

extern MRESULT EXPENTRY ComboBox_Processor ( HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2 ) {

   static PWINDOWPROC DefaultProcessor = 0 ;

   switch ( msg ) {

      case WM_CREATE: {
         CLASSINFO ClassInfo ;
         WinQueryClassInfo ( WinQueryAnchorBlock(hwnd), WC_COMBOBOX, &ClassInfo ) ;
         DefaultProcessor = ClassInfo.pfnWindowProc ;
         MRESULT Result = DefaultProcessor ( hwnd, msg, mp1, mp2 ) ;
         HWND Entryfield = WinWindowFromID ( hwnd, CBID_EDIT ) ;
         ULONG Style = WinQueryWindowULong ( hwnd, QWL_STYLE ) ;
         if ( Style & CBS_DROPDOWNLIST ) {
            OldComboEntry_Processor1 = WinSubclassWindow ( Entryfield, ComboEntry_Processor1 ) ;
         } else {
            OldComboEntry_Processor2 = WinSubclassWindow ( Entryfield, ComboEntry_Processor2 ) ;
         } /* endif */
         return ( Result ) ; }

      case WM_CONTROL: {
         USHORT Id = WinQueryWindowUShort ( hwnd, QWS_ID ) ;
         SHORT Control = SHORT1FROMMP ( mp1 ) ;
         SHORT Message = SHORT2FROMMP ( mp1 ) ;
         switch ( Control ) {
            case CBID_EDIT: {
               switch ( Message ) {
                  case EN_SETFOCUS: {
                     Sys_SendMessage( OWNER(hwnd), WM_CONTROL, MPFROM2SHORT(Id,CBN_SETFOCUS), 0 ) ;
                     break; }
                  case EN_KILLFOCUS: {
                     Sys_SendMessage( OWNER(hwnd), WM_CONTROL, MPFROM2SHORT(Id,CBN_KILLFOCUS), 0 ) ;
                     break; }
               } /* endswitch */
               break; }
            case CBID_LIST: {
               switch ( Message ) {
                  case LN_SETFOCUS: {
                     Sys_SendMessage( OWNER(hwnd), WM_CONTROL, MPFROM2SHORT(Id,CBN_SETFOCUS), 0 ) ;
                     break; }
                  case LN_KILLFOCUS: {
                     Sys_SendMessage( OWNER(hwnd), WM_CONTROL, MPFROM2SHORT(Id,CBN_KILLFOCUS), 0 ) ;
                     break; }
               } /* endswitch */
               break; }
         } /* endswitch */
         break; }

      case WM_MEASUREITEM:
         return ( Sys_SendMessage( OWNER(hwnd), msg, mp1, mp2 ) ) ;

      case WM_DRAWITEM: 
         return ( Sys_SendMessage( OWNER(hwnd), msg, mp1, mp2 ) ) ;

   } /* endswitch */

   return ( DefaultProcessor ? DefaultProcessor ( hwnd, msg, mp1, mp2 ) : 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *               Combo-Box Entryfield Subclass Procedure (1)                *
 *                                                                          *
 *  This subclass procedure is needed for the Entry Field created by a      *
 *  ComboBox control with the CBS_DROPDOWNLIST style set.  It intercepts    *
 *  the up and down arrows and passes them to the owner of ComboBox, and    *
 *  also generates the EN_SETFOCUS and EN_KILLFOCUS messages, which are     *
 *  not normally generated by these entry fields.                           *
 *                                                                          *
 ****************************************************************************/

static MRESULT EXPENTRY ComboEntry_Processor1 ( HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2 ) {

   switch ( msg ) {

      case WM_SETFOCUS: {
         USHORT Id = WinQueryWindowUShort ( hwnd, QWS_ID ) ;
         if ( mp2 ) {
            Sys_SendMessage( OWNER(hwnd), WM_CONTROL, MPFROM2SHORT(Id,EN_SETFOCUS), 0 ) ;
         } else {
            Sys_SendMessage( OWNER(hwnd), WM_CONTROL, MPFROM2SHORT(Id,EN_KILLFOCUS), 0 ) ;
         } /* endif */
         break; }

   } /* endswitch */

   return ( OldComboEntry_Processor1 ( hwnd, msg, mp1, mp2 ) ) ;
}
 
/****************************************************************************
 *                                                                          *
 *               Combo-Box Entryfield Subclass Procedure (2)                *
 *                                                                          *
 *  This subclass procedure is needed for the Entry Field created by a      *
 *  ComboBox control with the CBS_SIMPLE || CBS_DROPDOWN styles set.  It    *
 *  intercepts the up and down arrows and passes them to the owner of       *
 *  ComboBox.                                                               *
 *                                                                          *
 ****************************************************************************/

static MRESULT EXPENTRY ComboEntry_Processor2 ( HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2 ) {

   return ( OldComboEntry_Processor2 ( hwnd, msg, mp1, mp2 ) ) ;
}
 
/****************************************************************************
 *                                                                          *
 *                     EntryField Subclass Procedure                        *
 *                                                                          *
 *    This entryfield subclass adds the following features to the default   *
 *  system entryfield control:                                              *
 *                                                                          *
 *  (1) Upon receiving the focus, the entire entryfield text gets           *
 *      selected.                                                           *
 *                                                                          *
 ****************************************************************************/

extern MRESULT EXPENTRY EntryField_Processor ( HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2 ) {

   static PWINDOWPROC DefaultProcessor = 0 ;

   switch ( msg ) {

      case WM_CREATE: {
         CLASSINFO ClassInfo ;
         WinQueryClassInfo ( WinQueryAnchorBlock(hwnd), WC_ENTRYFIELD, &ClassInfo ) ;
         DefaultProcessor = ClassInfo.pfnWindowProc ;
         break; }

      case WM_SETFOCUS: {
         Sys_SendMessage( hwnd, EM_SETSEL, MPFROM2SHORT(0,9999), 0 ) ;
         break; }

   } /* endswitch */

   return ( DefaultProcessor ? DefaultProcessor ( hwnd, msg, mp1, mp2 ) : 0 ) ;
}

