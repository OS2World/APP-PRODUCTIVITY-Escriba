/* Build CONFIG.H from CONFIG.X and LOCAL.H */

'@Echo Off'

'Copy config.x config.h'

Call LINEOUT 'config.h', ''
Call LINEOUT 'config.h', '/* AUTOMATICALLY-GENERATED SYMBOLS */'
Call LINEOUT 'config.h', ''
Call LINEOUT 'config.h'

Call LINEOUT 'editfile', 's/TYPE[ ]*/TYPE_STRING \"/'
Call LINEOUT 'editfile', 's/$/\"/'
Call LINEOUT 'editfile', '1q'
Call LINEOUT 'editfile'

'Type local.h  | egrep "^#define[ ]*SIGNAL_TYPE" | sed -f editfile >> config.h'
'Type config.X | egrep "^#define[ ]*SIGNAL_TYPE" | sed -f editfile >> config.h'
'Type local.h  | egrep "^#define[ ]*MASKTYPE"    | sed -f editfile >> config.h'
'Type config.X | egrep "^#define[ ]*MASKTYPE"    | sed -f editfile >> config.h'

'Erase editfile'

Call LINEOUT 'config.h', ''
Call LINEOUT 'config.h', '/* END AUTOMATICALLY-GENERATED SYMBOLS */'
Call LINEOUT 'config.h', ''
Call LINEOUT 'config.h'

