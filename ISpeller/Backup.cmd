@Echo Off
Verify On

REM *** Remove Extended Attributes ***
eautil /s Install.cmd
eautil /s Build_Config.cmd
del eas\*.cmd
zap eas

If *%1==*ALL Goto All

:UPDATE
Echo Insert Diskette 'ISpeller for OS/2 PM (source)'
Pause
Zip -b e: -9for a:ISpeller.zip *
If Errorlevel 1 Goto ERROR
Copy BuildHash.exe BuildHas.exe
Zip -b e: -9fo a:ISpell__.zip *
If Errorlevel 1 Goto ERROR
Echo Insert Diskette 'ISpell Dictionaries (distribution)'
Pause
Zip -b e: -9fo a:ispeller.zip *
If Errorlevel 1 Goto ERROR
cd ..\Dictionaries
Zip -b e: -9fo a:English.zip *
If Errorlevel 1 Goto ERROR
Zip -b e: -9fo a:Deutsch.zip *
If Errorlevel 1 Goto ERROR
Zip -b e: -9fo a:Espanol.zip *
If Errorlevel 1 Goto ERROR
Zip -b e: -9fo a:Francais.zip *
If Errorlevel 1 Goto ERROR
Zip -b e: -9fo a:Ndrlands.zip *
If Errorlevel 1 Goto ERROR
Goto EXIT

:ALL
Zip -9or \ISpeller.zip * -x *.obj *.err *.res *.map
If Errorlevel 1 Goto ERROR
Copy BuildHash.exe BuildHas.exe
Zip -9o \ISpell__.zip -@ < ISPELL__.PKG
If Errorlevel 1 Goto ERROR
Dir \ispell*.zip
Echo Insert Diskette 'ISpeller for OS/2 PM (source)'
Pause
Copy \ispell*.zip a: /v
Del \ispell*.zip
Zip -9o \ISpeller.zip -@ < ISPELLER.PKG
If Errorlevel 1 Goto ERROR
Cd ..\Dictionaries
Zip -9o \English.zip English.aff English.dic
If Errorlevel 1 Goto ERROR
Zip -9o \Deutsch.zip Deutsch.aff Deutsch.dic
If Errorlevel 1 Goto ERROR
Zip -9o \Espanol.zip Espanol.aff Espanol.dic
If Errorlevel 1 Goto ERROR
Zip -9o \Francais.zip Francais.aff Francais.dic
If Errorlevel 1 Goto ERROR
Zip -9o \NdrLands.zip NdrLands.aff NdrLands.dic
If Errorlevel 1 Goto ERROR
Dir \*.zip
Echo Insert Diskette 'ISpell Dictionaries (distribution)'
Pause
Copy \*.zip a: /v
Del \*.zip
Goto EXIT

:ERROR
Echo ERROR: Something went wrong!
Echo 

:EXIT
Del BuildHas.exe

