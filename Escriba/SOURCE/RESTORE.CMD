@Echo Off
Verify On

Echo Insert Distribution diskette . . .
Pause
UNZIP -Cuo a:ESCRIBA
UNZIP -Cuo a:ADDONS
md ..\RunTime >NUL 2>NUL
cd ..\RunTime
UNZIP -Cuo a:RUNTIME
cd ..\Escriba

Echo Insert Languages diskette . . .
Pause
UNZIP -Cuo a:ESCR_DEU
UNZIP -Cuo a:ESCR_ESP
UNZIP -Cuo a:ESCR_FRA
UNZIP -Cuo a:ESCR_NLD

Echo Insert Source diskette . . .
Pause
UNZIP -Cou a:ESCRIBAS

Echo Insert ISpeller Source diskette . . .
Pause
md ..\ISpeller >NUL 2>NUL
cd ..\ISpeller
UNZIP -Cou a:ISpeller
cd ..\Escriba

