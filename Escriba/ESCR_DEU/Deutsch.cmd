/* DEUTSCH.CMD: Installation von ESCRIBA in Deutsch. */

'@Echo Off'
'CHCP 850'

/* Objekt-Titel festlegen. */

Title = 'Escriba 0.96d'
Language = 'Deutsch'
FileName = 'Deutsch'


/* REXXUTIL laden */

Call RxFuncAdd 'SysLoadFuncs', 'REXXUTIL', 'SysLoadFuncs'
Call SysLoadFuncs


/* Initialisierung */

Signal On Failure Name FAILURE
Signal On Halt Name HALT
Signal On Syntax Name SYNTAX

Call SysCls
Say 'Installiere ESCRIBA...'
Say ''


/* PrÅfe, ob verschiedene Komponenten (Dateien) vorhanden sind. */

Result = SysFileTree( 'ESCRIBA.EXE', 'Files', 'F' )
If Files.0 = 0 Then
   Do
   Say 'FEHLER: ESCRIBA.EXE nicht gefunden!'
   Signal DONE
   End

Result = SysFileTree( FileName".DLL", 'Files', 'F' )
If Files.0 = 0 Then
   Do
   Say 'FEHLER: 'FileName'.DLL nicht gefunden!'
   Signal DONE
   End

Result = SysFileTree( FileName".HLP", 'Files', 'F' )
If Files.0 = 0 Then
   Do
   Say 'FEHLER: 'FileName'.HLP nicht gefunden!'
   Signal DONE
   End


/* Zielverzeichnis erfragen. */

BootDrive = Filespec('Drive',Value('SYSTEM_INI',,'OS2ENVIRONMENT'))
Default = SysIni('USER', 'ESCRIBA', 'INIPATH' )
If Default = 'ERROR:'
  Then Default = BootDrive'\OS2\Apps'
If SUBSTR(Default,LENGTH(Default),1) <= ' '
  Then Default = SUBSTR(Default,1,LENGTH(Default)-1)

Say 'Bitte geben Sie den vollen Verzeichnisnamen ein, wohin'
Say '  ESCRIBA installiert werden soll (Standard: 'Default'): '
Parse Pull Directory
If Directory = "" Then Directory = Default


/* Falls notwendig, Zielverzeichnis anlegen. */

Result = SysFileTree( Directory, 'Dirs', 'D' )
If Dirs.0 = 0 Then
   Do
   Result = SysMkDir( Directory )
   if Result == 0 Then
      Do
      End
   Else
      Do
      Say 'FEHLER: Zielverzeichnis kann nicht angelegt werden.'
      Signal DONE
      End
   End
Say ''


/* Save the target directory. */

SysIni( "USER", "ESCRIBA", "INIPATH", Directory )


/* Installation durchfÅhren. */

Say 'Kopiere ESCRIBA nach 'Directory' ...'
'Copy ESCRIBA.EXE' Directory                   '1>NUL'
'Copy ESCRIBA.ICO' Directory                   '1>NUL'
'Copy UNINSTAL.CMD' Directory                  '1>NUL'
'Copy *.DLL' Directory                         '1>NUL'
'Copy *.HLP' Directory                         '1>NUL'
'Copy 'FileName'.DOC' Directory'\ESCRIBA.DOC    1>NUL'

Result = SysFileTree( "HLP2INF.EXE", 'Files', 'F' )
If Files.0 > 0 Then
   Do
   Result = SysFileTree( Directory"\ESCRIBA.INF", 'Files', 'F' )
   If Files.0 > 0 Then
      Do
      "Erase" Directory"\ESCRIBA.INF"
      End
   "HLP2INF" Directory"\"FileName".HLP"
   "Rename" Directory"\"FileName".INF ESCRIBA.INF"
   End

Say 'Erzeuge Programm-Ordner...'
Type = 'WPFolder'
Folder = '<WP_DESKTOP>'
Parms = "OBJECTID=<ESCRIBAF>;ICONFILE="Directory"\ESCRIBA.ICO;ALWAYSSORT=YES;"
Result = SysCreateObject( Type, 'Escriba', Folder, Parms, 'ReplaceIfExists' )
If Result != 1 Then
   Say 'FEHLER: Ordner nicht erzeugt.'

Say 'Erzeuge Programmobjekt...'
Type = 'WPProgram'
Folder = '<ESCRIBAF>'
Parms = "MINWIN=DESKTOP;PROGTYPE=PM;EXENAME="Directory"\ESCRIBA.EXE;STARTUPDIR="Directory";OBJECTID=<ESCRIBA>;NOPRINT=YES;ASSOCTYPE=Escriba,Pluma,Plain Text,Bitmap,GIF,JPG,Metafile,PCC,PCX,TIFF,HTML;ASSOCFILTER=*.TXT,*.DOC,*.ME,*.BAT,*.CMD,*.MOI,*.BMP,*.GIF,*.JPG,*.JPEG,*.JFI,*.JFIF,*.MET,*.PCC,*.PCX,*.TIF,*.TIFF;*.HTM;*.HTML;"
Result = SysCreateObject( Type, Title, Folder, Parms, 'ReplaceIfExists' )
If Result != 1 Then
   Say 'FEHLER: Objekt nicht erzeugt.'

Say 'Erzeuge Dokumentations-Objekte...'
Type = 'WPProgram'
Parms = "MINWIN=DESKTOP;PROGTYPE=PM;EXENAME="Directory"\ESCRIBA.EXE;STARTUPDIR="Directory";OBJECTID=<ESCRIBAD>;PARAMETERS=ESCRIBA.DOC"
Result = SysCreateObject( Type, "Read Me", Folder, Parms, 'ReplaceIfExists' )
If Result != 1 Then
   Say 'FEHLER: Referenz nicht erzeugt.'

Result = SysFileTree( Directory"\ESCRIBA.INF", 'Files', 'F' )
If Files.0 > 0 Then Do
   Type = 'WPProgram'
   Parms = "MINWIN=DESKTOP;PROGTYPE=PM;EXENAME=VIEW.EXE;STARTUPDIR="Directory";OBJECTID=<ESCRIBAI>;PARAMETERS=ESCRIBA.INF EinfÅhrung;"
   Result = SysCreateObject( Type, "Escriba User's Reference", Folder, Parms, 'ReplaceIfExists' )
   If Result != 1 Then Do
      Say 'FEHLER: Referenz nicht erzeugt.'
      End
   End


/* Initialize the INI file, setting the default language. */

Result = SysIni( Directory"\ESCRIBA.INI", "ESCRIBA", "Language", Language )
If Result = 'ERROR:' Then Do
   Say 'ERROR: Language preference not saved.'
   End


/* PrÅfen, ob die RechtschreibprÅfung vorhanden ist. */

Speller = 1
Result = SysFileTree( "ISPELLER.DLL", 'Files', 'F' )
If Files.0 = 0 Then Speller = 0

Result = SysFileTree( "BUILDHAS.EXE", 'Builder1', 'F' )
Result = SysFileTree( "BUILDHASH.EXE", 'Builder2', 'F' )
If ( ( Builder1.0 = 0 ) & ( Builder2.0 = 0 ) ) Then Speller = 0


/* Installiere die RechtschreibprÅfung, wenn vorhanden. */

If Speller = 1 Then Do
   Say 'Installiere RechtschreibprÅfung...'
   'Copy ISPELLER.DLL' Directory '1>NUL'
   If Builder1.0 Then 
      'Copy BUILDHAS.EXE' Directory '1>NUL'
   Else 
      'Copy BUILDHASH.EXE' Directory'\BUILDHAS.EXE 1>NUL'
   'Copy *.AFF' Directory '1>NUL'
   'Copy *.DIC' Directory '1>NUL'
   Result = SysFileTree( Directory"\*.AFF", 'Affix', 'FO' ) 
   Do Index=1 To Affix.0
      LanguageName = SUBSTR(Affix.Index,1,LENGTH(Affix.Index)-4)
      DictionaryPath = LanguageName".DIC"
      Do While ( \ ( POS('\',LanguageName) = 0 ) )
         LanguageName = SUBSTR(LanguageName,POS('\',LanguageName)+1)
         End /* do */
      Result = SysFileTree( DictionaryPath, 'Dictionary', 'FO' )
      If Dictionary.0 = 1 Then Do
         Say 'Building dictionary for' LanguageName'. . .'
         Directory'\BUILDHAS -s' Dictionary.1 Affix.Index Directory'\'LanguageName'.HSH 1>NUL'
         End /* if */
      End /* do */
   End /* if */

Signal DONE

FAILURE:
Say 'REXX-Fehler.'
Signal DONE

HALT:
Say 'REXX-Halt.'
Signal DONE

SYNTAX:
Say 'REXX-Syntaxfehler.'
Signal DONE

DONE:
Exit
