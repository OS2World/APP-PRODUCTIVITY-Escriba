@Echo Off
Verify On
If *%TMP%==* TMP=e:

REM *** Remove Extended Attributes ***
eautil /s English.cmd
eautil /s Chinese.cmd
eautil /s Deutsch.cmd
eautil /s Espanol.cmd
eautil /s Francais.cmd
eautil /s Japanese.cmd
eautil /s Ndrlands.cmd
zap eas

If *%1==*ALL Goto All

:UPDATE
REM *** Update Distribution Diskette ***
Echo Insert Distribution diskette.
Pause
Echo Updating ESCRIBA.ZIP . . .
Zip -b %TMP%\ -9fo a:ESCRIBA
Cd ..\RunTime
Echo Updating RUNTIME.ZIP
Zip -b %TMP%\ -9fo a:RUNTIME
Cd ..\Escriba
Echo Updating ADDONS.ZIP
Zip -b %TMP%\ -9fo a:ADDONS
Cd ..\Escriba

REM *** Update Languages Diskette ***
Echo Insert Languages diskette.
Pause
Echo Updating ESCR_DEU.ZIP (German) . . .
Zip -b %TMP%\ -9fo a:ESCR_DEU
Echo Updating ESCR_ESP.ZIP (Spanish) . . .
Zip -b %TMP%\ -9fo a:ESCR_ESP
Echo Updating ESCR_FRA.ZIP (French) . . .
Zip -b %TMP%\ -9fo a:ESCR_FRA
Echo Updating ESCR_NLD.ZIP (Dutch) . . .
Zip -b %TMP%\ -9fo a:ESCR_NLD
Echo Updating ESCR_CHT.ZIP (Chinese) . . .
Zip -b %TMP%\ -9fo a:ESCR_CHT
Echo Updating ESCR_JPN.ZIP (Japanese) . . .
Zip -b %TMP%\ -9fo a:ESCR_JPN

REM *** Update Source Diskette ***
Echo Insert Source diskette.
Pause
Zip -b %TMP%\ -9fo a:ESCRIBAS

REM *** Back up ISpeller stuff ***
CD ..\ISpeller
Call BACKUP

CD ..\Escriba
Goto EXIT

:ALL
REM *** Generate Distribution Diskette ***
Erase *.zip 1> NUL 2> NUL
Echo Building ESCRIBA.ZIP . . .
Zip -9o \ESCRIBA -@ < ESCRIBA.PKG
Echo Building ADDONS.ZIP . . .
Zip -9o \ADDONS -@ < ADDONS.PKG
Echo Building RUNTIME.ZIP . . .
Cd ..\RunTime
Zip -9o \RUNTIME -@ < RUNTIME.PKG
Cd ..\Escriba
Dir \*.zip
Echo Insert Distribution diskette.
Pause
Copy \*.zip a: /v 1>NUL
Erase \*.zip 1>NUL 2>NUL

REM *** Generate Languages Diskette ***
Echo Building ESCR_DEU.ZIP (German) . . .
Zip -9o \ESCR_DEU -@ < ESCR_DEU.PKG
Echo Building ESCR_ESP.ZIP (Spanish) . . .
Zip -9o \ESCR_ESP -@ < ESCR_ESP.PKG
Echo Building ESCR_FRA.ZIP (French) . . .
Zip -9o \ESCR_FRA -@ < ESCR_FRA.PKG
Echo Building ESCR_NLD.ZIP (Dutch) . . .
Zip -9o \ESCR_NLD -@ < ESCR_NLD.PKG
Echo Building ESCR_CHT.ZIP (Chinese) . . .
Zip -9o \ESCR_CHT -@ < ESCR_CHT.PKG
Echo Building ESCR_JPN.ZIP (Japanese) . . .
Zip -9o \ESCR_JPN -@ < ESCR_JPN.PKG
Dir \*.zip
Echo Insert Languages diskette.
Pause
Copy \*.zip a: /v 1>NUL
Erase \*.zip 1>NUL 2>NUL

REM *** Generate Source Diskette ***
PKZip -ex -o -r -p -x@exclude.txt -x@addons.pkg \Escribas.zip
Dir \*.zip
Echo Insert Source diskette.
Pause
Copy \*.zip a: /v 1>NUL
Erase \*.zip 1>NUL 2>NUL

REM *** Back up ISpeller stuff ***
CD ..\ISpeller
Call BACKUP ALL

CD ..\Escriba
Goto EXIT

:ERROR
Echo ERROR: Something went wrong!
Echo 

:EXIT
