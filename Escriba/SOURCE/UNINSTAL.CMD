/* UNINSTAL.CMD: Uninstall ESCRIBA. */

'@Echo Off'
'CHCP 850'


/* Load REXXUTIL */

Call RxFuncAdd 'SysLoadFuncs', 'REXXUTIL', 'SysLoadFuncs'
Call SysLoadFuncs


/* Initialize */

Signal On Failure Name FAILURE
Signal On Halt Name HALT
Signal On Syntax Name SYNTAX

/* Confirm the user's intentions. */

Call SysCls
Say 'Do you wish to uninstall Escriba? (Y/N)'
Pull YesNo
If \ ( YesNo = "Y" ) Then Signal Done


/* Announce the start of the destruction . . . */

Say 'Removing ESCRIBA...'
Say ''

/* Determine Escriba directory. */

Default = SysIni('USER', 'ESCRIBA', 'INIPATH' )
If Default = 'ERROR:' Then Do
  Say 'ERROR: ESCRIBA does not appear to be installed on this system.'
  Signal DONE
  End

If SUBSTR(Default,LENGTH(Default),1) <= ' '
  Then Default = SUBSTR(Default,1,LENGTH(Default)-1)


/* Remove every Escriba file from the install directory. */

Result = SysFileTree( Default"\Escriba.EXE", 'Files', 'F' )
If Files.0 > 0 Then Do
  Say "Removing Escriba.exe."
  'Erase' Default'\escriba.exe'
  End

Result = SysFileTree( Default"\Escriba.INI", 'Files', 'F' )
If Files.0 > 0 Then Do
  Say "Removing Escriba.INI."
  'Erase' Default'\escriba.INI'
  End

Result = SysFileTree( Default"\Escriba.DOC", 'Files', 'F' )
If Files.0 > 0 Then Do
  Say "Removing Escriba.doc."
  'Erase' Default'\escriba.doc'
  End

Result = SysFileTree( Default"\Escriba.INF", 'Files', 'F' )
If Files.0 > 0 Then Do
  Say "Removing Escriba.inf."
  'Erase' Default'\escriba.inf'
  End

Result = SysFileTree( Default"\Escriba.ICO", 'Files', 'F' )
If Files.0 > 0 Then Do
  Say "Removing Escriba.ico."
  'Erase' Default'\escriba.ico'
  End

Result = SysFileTree( Default"\Escriba.LOG", 'Files', 'F' )
If Files.0 > 0 Then Do
   Say "Removing Escriba.log."
   'Erase' Default'\escriba.log'
   End

Result = SysFileTree( Default"\RUNTIME.DLL", 'Files', 'F' )
If Files.0 > 0 Then Do
   Say "Removing RunTime.dll."
   'Erase' Default'\RunTime.dll'
   End

Result = SysFileTree( Default"\ISpeller.DLL", 'Files', 'F' )
If Files.0 > 0 Then Do
   Say "Removing ISpeller.dll."
   'Erase' Default'\ISpeller.dll'
   End

Result = SysFileTree( Default"\ISpeller.INI", 'Files', 'F' )
If Files.0 > 0 Then Do
   Say "Removing ISpeller.INI."
   'Erase' Default'\ISpeller.INI'
   End

Result = SysFileTree( Default"\BuildHas.EXE", 'Files', 'F' )
If Files.0 > 0 Then Do
   Say "Removing BuildHas.exe."
   'Erase' Default'\BuildHas.exe'
   End

Result = SysFileTree( Default"\*.DIC", 'Files', 'F' )
If Files.0 > 0 Then Do
   Say "Removing *.DIC."
   'Erase' Default'\*.DIC'
   End

Result = SysFileTree( Default"\*.AFF", 'Files', 'F' )
If Files.0 > 0 Then Do
   Say "Removing *.AFF."
   'Erase' Default'\*.AFF'
   End

Result = SysFileTree( Default"\*.HSH", 'Files', 'F' )
If Files.0 > 0 Then Do
   Say "Removing *.HSH."
   'Erase' Default'\*.HSH'
   End

Result = SysFileTree( Default"\BMP.DLL", 'Files', 'F' )
If Files.0 > 0 Then Do
   Say "Removing BMP.DLL."
   'Erase' Default'\BMP.DLL'
   End

Result = SysFileTree( Default"\BMP.INI", 'Files', 'F' )
If Files.0 > 0 Then Do
   Say "Removing BMP.INI."
   'Erase' Default'\BMP.INI'
   End

Result = SysFileTree( Default"\BMP__*.*", 'Files', 'F' )
If Files.0 > 0 Then Do
   Say "Removing BMP__*.*."
   'Erase' Default'\BMP__*.*'
   End

Result = SysFileTree( Default"\GIF.DLL", 'Files', 'F' )
If Files.0 > 0 Then Do
   Say "Removing GIF.DLL."
   'Erase' Default'\GIF.DLL'
   End

Result = SysFileTree( Default"\GIF.INI", 'Files', 'F' )
If Files.0 > 0 Then Do
   Say "Removing GIF.INI."
   'Erase' Default'\GIF.INI'
   End

Result = SysFileTree( Default"\GIF__*.*", 'Files', 'F' )
If Files.0 > 0 Then Do
   Say "Removing GIF__*.*."
   'Erase' Default'\GIF__*.*'
   End

Result = SysFileTree( Default"\HTML.DLL", 'Files', 'F' )
If Files.0 > 0 Then Do
   Say "Removing HTML.DLL."
   'Erase' Default'\HTML.DLL'
   End

Result = SysFileTree( Default"\HTML.INI", 'Files', 'F' )
If Files.0 > 0 Then Do
   Say "Removing HTML.INI."
   'Erase' Default'\HTML.INI'
   End

Result = SysFileTree( Default"\HTML_*.*", 'Files', 'F' )
If Files.0 > 0 Then Do
   Say "Removing HTML_*.*."
   'Erase' Default'\HTML_*.*'
   End

Result = SysFileTree( Default"\JPEG.DLL", 'Files', 'F' )
If Files.0 > 0 Then Do
   Say "Removing JPEG.DLL."
   'Erase' Default'\JPEG.DLL'
   End

Result = SysFileTree( Default"\JPEG.INI", 'Files', 'F' )
If Files.0 > 0 Then Do
   Say "Removing JPEG.INI."
   'Erase' Default'\JPEG.INI'
   End

Result = SysFileTree( Default"\JPEG_*.*", 'Files', 'F' )
If Files.0 > 0 Then Do
   Say "Removing JPEG_*.*."
   'Erase' Default'\JPEG_*.*'
   End

Result = SysFileTree( Default"\PCX.DLL", 'Files', 'F' )
If Files.0 > 0 Then Do
   Say "Removing PCX.DLL."
   'Erase' Default'\PCX.DLL'
   End

Result = SysFileTree( Default"\PCX.INI", 'Files', 'F' )
If Files.0 > 0 Then Do
   Say "Removing PCX.INI."
   'Erase' Default'\PCX.INI'
   End

Result = SysFileTree( Default"\PCX__*.*", 'Files', 'F' )
If Files.0 > 0 Then Do
   Say "Removing PCX__*.*."
   'Erase' Default'\PCX__*.*'
   End

Result = SysFileTree( Default"\PTEXT.DLL", 'Files', 'F' )
If Files.0 > 0 Then Do
   Say "Removing PTEXT.DLL."
   'Erase' Default'\PTEXT.DLL'
   End

Result = SysFileTree( Default"\PTEXT.INI", 'Files', 'F' )
If Files.0 > 0 Then Do
   Say "Removing PTEXT.INI."
   'Erase' Default'\PTEXT.INI'
   End

Result = SysFileTree( Default"\PTXT_*.*", 'Files', 'F' )
If Files.0 > 0 Then Do
   Say "Removing PTXT_*.*."
   'Erase' Default'\PTXT_*.*'
   End

Result = SysFileTree( Default"\TIFF.DLL", 'Files', 'F' )
If Files.0 > 0 Then Do
   Say "Removing TIFF.DLL."
   'Erase' Default'\TIFF.DLL'
   End

Result = SysFileTree( Default"\TIFF.INI", 'Files', 'F' )
If Files.0 > 0 Then Do
   Say "Removing TIFF.INI."
   'Erase' Default'\TIFF.INI'
   End

Result = SysFileTree( Default"\TIFF_*.*", 'Files', 'F' )
If Files.0 > 0 Then Do
   Say "Removing TIFF_*.*."
   'Erase' Default'\TIFF_*.*'
   End

Result = SysFileTree( Default"\DEUTSCH.*", 'Files', 'F' )
If Files.0 > 0 Then Do
   Say "Removing DEUTSCH.*."
   'Erase' Default'\DEUTSCH.*'
   End

Result = SysFileTree( Default"\ENGLISH.*", 'Files', 'F' )
If Files.0 > 0 Then Do
   Say "Removing ENGLISH.*."
   'Erase' Default'\ENGLISH.*'
   End

Result = SysFileTree( Default"\ESPANOL.*", 'Files', 'F' )
If Files.0 > 0 Then Do
   Say "Removing ESPANOL.*."
   'Erase' Default'\ESPANOL.*'
   End

Result = SysFileTree( Default"\FRANCAIS.*", 'Files', 'F' )
If Files.0 > 0 Then Do
   Say "Removing FRANCAIS.*."
   'Erase' Default'\FRANCAIS.*'
   End

Result = SysFileTree( Default"\NDRLANDS.*", 'Files', 'F' )
If Files.0 > 0 Then Do
   Say "Removing NDRLANDS.*."
   'Erase' Default'\NDRLANDS.*'
   End

Result = SysFileTree( Default"\PLUMAR.DLL", 'Files', 'F' )
If Files.0 > 0 Then Do
  Say "Removing PlumaR.dll."
  'Erase' Default'\PlumaR.dll'
  End

Result = SysFileTree( Default"\ESCRIBAR.DLL", 'Files', 'F' )
If Files.0 > 0 Then Do
  Say "Removing EscribaR.dll."
  'Erase' Default'\EscribaR.dll'
  End

Result = SysFileTree( Default"\UNINSTAL.CMD", 'Files', 'F' )
If Files.0 > 0 Then Do
  Say "Removing Uninstal.cmd."
  'Erase' Default'\Uninstal.cmd'
  End

Result = SysFileTree( Default"\*.*", 'Files', 'F' )
If Files.0 == 0 Then Do
   Say "Removing directory "Default"."
   'rmdir' Default
   End


/* Destroy the Escriba WPS objects. */

Say "Removing WPS objects."
Result = SysDestroyObject( "<ESCRIBA>" )
Result = SysDestroyObject( "<ESCRIBAI>" )
Result = SysDestroyObject( "<ESCRIBAD>" )
Result = SysDestroyObject( "<ESCRIBAF>" )


/* Remove the USER/ESCRIBA/INIPATH entry. */

Say "Removing forwarding entry in OS2.INI."
Result = SysIni('USER', 'ESCRIBA', 'DELETE:' )


/* Terminate procedure. */

If Files.0 == 0 Then Do
   Say "Done.  You will still need to manually"
   Say "  remove UNINSTAL.CMD."
   End /* do */
Else Do
   Say "Done.  You will still need to manually"
   Say "  remove UNINSTAL.CMD and the directory"
   Say "  '"Default"'."
   End /* do */
Signal DONE

FAILURE:
Say 'REXX failure.'
Signal DONE

HALT:
Say 'REXX halt.'
Signal DONE

SYNTAX:
Say 'REXX syntax error.'
Signal DONE

DONE:
Exit
