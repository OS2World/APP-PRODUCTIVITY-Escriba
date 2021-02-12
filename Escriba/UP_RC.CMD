@Echo Off
chcp 850
If *%1==*ALL Goto All

epm deutsch\*.rc english\*.rc espanol\*.rc francais\*.rc ndrlands\*.rc chinese\*.rc japanese\*.rc 
Goto Done

:All
epm deutsch\*.rc english\*.rc espanol\*.rc francais\*.rc ndrlands\*.rc chinese\*.rc japanese\*.rc 
epm addons\bmp\*.rc addons\gif\*.rc addons\html\*.rc addons\jpeg\*.rc addons\pcx\*.rc addons\ptext\*.rc addons\tiff\*.rc
epm ..\ispeller\*.rc 

:Done
