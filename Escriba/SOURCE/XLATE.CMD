@Echo Off
If *%TMP%==* TMP=e:

Echo *** Building Developer's Package ***
Erase XLATEDEV.zip 2>NUL
Zip -9oj XLATEDEV -@ < XLATEDEV.PKG

Echo *** Building English Package ***
Erase %TMP%\XLATE.zip 2>NUL
Zip -9oj %TMP%\XLATE -@ < XLATE.PKG

Echo *** Building German Package ***
Erase %TMP%\XLAT_DEU.zip 2>NUL
Zip -9oj %TMP%\XLAT_DEU -@ < XLAT_DEU.PKG

Echo *** Building Spanish Package ***
Erase %TMP%\XLAT_ESP.zip 2>NUL
Zip -9oj %TMP%\XLAT_ESP -@ < XLAT_ESP.PKG

Echo *** Building French Package ***
Erase %TMP%\XLAT_FRA.zip 2>NUL
Zip -9oj %TMP%\XLAT_FRA -@ < XLAT_FRA.PKG

Echo *** Building Dutch Package ***
Erase %TMP%\XLAT_NLD.zip 2>NUL
Zip -9oj %TMP%\XLAT_NLD -@ < XLAT_NLD.PKG

Erase XLATEDEV.zip 2>NUL
Dir %TMP%\XLAT*.zip

