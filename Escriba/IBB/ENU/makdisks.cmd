/* Make Diskettes from CD */

'@Echo Off'

Call RxFuncAdd SysLoadFuncs, RexxUtil, SysLoadFuncs
Call SysLoadFuncs

Say "MAKDISKS: Build installation diskettes."

do DisketteNumber=1 to 20
   rc = SysFileTree( 'DISKET'DisketteNumber'.DSK', File, 'F' )
   If ( File.0 = 0 ) Then Leave
   Say ""
   Say "Insert diskette number" DisketteNumber "and press <ENTER>."
   Parse Pull Dummy
   'LOADDSKF DISKET'DisketteNumber'.DSK A: /F/Y/Q'
end /* do */

Say ""
Say "Remove the last diskette."
Say "Done."
