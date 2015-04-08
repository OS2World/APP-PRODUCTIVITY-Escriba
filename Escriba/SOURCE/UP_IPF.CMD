@Echo Off
chcp 850
If *%1==*ALL Goto All

epm deutsch\*.ipf english\*.ipf espanol\*.ipf francais\*.ipf ndrlands\*.ipf 
Goto Done

:All
epm deutsch\*.ipf english\*.ipf espanol\*.ipf francais\*.ipf ndrlands\*.ipf 
epm addons\bmp\*.ipf addons\gif\*.ipf addons\html\*.ipf addons\jpeg\*.ipf addons\pcx\*.ipf addons\ptext\*.ipf addons\ptext\*.ipf
epm ..\ispeller\*.ipf

:Done

