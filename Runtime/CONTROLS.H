/***************************************************************** CONTROLS.H
 *                                                                          *
 *                          Subclassed Controls                             *
 *                                                                          *
 ****************************************************************************/

#ifndef CONTROLS_H
#define CONTROLS_H

#include "System.h"

enum {   // Additional control notifications for Buttons.
   BN_SETFOCUS = BN_PAINT + 1,
   BN_KILLFOCUS
} ;

enum {   // Additional control notifications for Combo Boxes.
   CBN_SETFOCUS = CBN_ENTER + 1,
   CBN_KILLFOCUS
} ;

extern void RegisterControls ( HAB Anchor ) ;

#define BUTTON_CLASS "Button"
extern FNWP Button_Processor ;

#define COMBOBOX_CLASS "ComboBox"
extern FNWP ComboBox_Processor ;

#define ENTRYFIELD_CLASS "EntryField"
extern FNWP EntryField_Processor ;

#endif
