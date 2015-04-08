/* NDRLANDS.CMD : Installeer de Nederlandse versie van Escriba. */

'@Echo Off'
'CHCP 850'

/* Zet de titel van het programma */

Title = 'Escriba 0.96d'
Language = 'Nederlands'
FileName = 'Ndrlands'


/* Laad REXXUTIL */

Call RxFuncAdd 'SysLoadFuncs', 'REXXUTIL', 'SysLoadFuncs'
Call SysLoadFuncs


/* Initialiseer */

Signal On Failure Name FAILURE
Signal On Halt Name HALT
Signal On Syntax Name SYNTAX

Call SysCls
Say 'Installatie van Escriba nu bezig...'
Say ''


/* Controleer of de verschillende bestanden beschikbaar zijn. */

Result = SysFileTree( 'ESCRIBA.EXE', 'Files', 'F' )
If Files.0 = 0 Then
   Do
   Say 'FOUT: ESCRIBA.EXE niet gevonden!'
   Signal DONE
   End

Result = SysFileTree( Filename".DLL", 'Files', 'F' )
If Files.0 = 0 Then
   Do
   Say 'FOUT: 'Filename'.DLL niet gevonden!'
   Signal DONE
   End

Result = SysFileTree( Filename".HLP", 'Files', 'F')
If Files.0 = 0 Then
   Do
   Say 'FOUT: 'Filename'.HLP niet gevonden!'
   Signal DONE
   End


/* Vraag naar de directory waarin het programma geplaatst moet worden */

BootDrive = Filespec('Drive',Value('SYSTEM_INI',,'OS2ENVIRONMENT'))
Default = SysIni('USER', 'ESCRIBA', 'INIPATH' )
If Default = 'ERROR:'
  Then Default = BootDrive'\OS2\Apps'
If SUBSTR(Default,LENGTH(Default),1) <= ' '
  Then Default = SUBSTR(Default,1,LENGTH(Default)-1)

Say 'Geef a.u.b. de volledige naam van de directory waarin'
Say ' ESCRIBA geplaatst moet worden (standaard 'Default'): '
Parse Pull Directory
If Directory = "" Then Directory = Default


/* Maak de aangegeven directory indien nodig aan */

Result = SysFileTree( Directory, 'Dirs', 'D')
If Dirs.0 = 0 Then
   Do
   Result = SysMkDir( Directory )
   If Result == 0 Then
      Do
      End
   Else
      Do
      Say 'FOUT: Onmogelijk om de aangegeven directory aan te maken.'
      Signal DONE
      End
   End
Say ''


/* Save the target directory. */

SysIni( "USER", "ESCRIBA", "INIPATH", Directory )


/* Voer de installatie uit */

Say 'ESCRIBA wordt op 'Directory' gekopieerd...'
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

Say 'Programmafolder wordt gemaakt...'
Type = 'WPFolder'
Folder = '<WP_DESKTOP>'
Parms = "OBJECTID=<ESCRIBAF>;ICONFILE="Directory"\ESCRIBA.ICO;ALWAYSSORT=YES;"
Result = SysCreateObject( Type, 'Escriba', Folder, Parms, 'ReplaceIfExists' )
If Result != 1 Then
   Say 'FOUT: Folder niet gemaakt.'

Say 'Het programma-object wordt gemaakt...'
Type = 'WPProgram'
Folder = '<ESCRIBAF>'
Parms = "MINWIN=DESKTOP;PROGTYPE=PM;EXENAME="Directory"\ESCRIBA.EXE;STARTUPDIR="Directory";OBJECTID=<ESCRIBA>;NOPRINT=YES;ASSOCTYPE=Escriba,Pluma,Plain Text,Bitmap,GIF,JPG,Metafile,PCC,PCX,TIFF,HTML;ASSOCFILTER=*.TXT,*.DOC,*.ME,*.BAT,*.CMD,*.MOI,*.BMP,*.GIF,*.JPG,*.JPEG,*.JFI,*.JFIF,*.MET,*.PCC,*.PCX,*.TIF,*.TIFF;*.HTM;*.HTML;"
Result = SysCreateObject( Type, Title, Folder, Parms, 'ReplaceIfExists' )
If Result != 1 Then
   Say 'FOUT: Object niet gemaakt.'

Say 'Objecten voor documentatie worden gemaakt...'
Type = 'WPProgram'
Parms = "MINWIN=DESKTOP;PROGTYPE=PM;EXENAME="Directory"\ESCRIBA.EXE;STARTUPDIR="Directory";OBJECTID=<ESCRIBAD>;PARAMETERS=ESCRIBA.DOC"
Result = SysCreateObject( Type, "Lees mij", Folder, Parms, 'ReplaceIfExists' )
If Result != 1 Then
   Say 'FOUT: Schaduw niet gemaakt.'

Result = SysFileTree( Directory"\ESCRIBA.INF", 'Files', 'F' )
If Files.0 > 0 Then Do
   Type = 'WPProgram'
   Parms = "MINWIN=DESKTOP;PROGTYPE=PM;EXENAME=VIEW.EXE;STARTUPDIR="Directory";OBJECTID=<ESCRIBAI>;PARAMETERS=ESCRIBA.INF Inleiding;"
   Result = SysCreateObject( Type, "Escriba gebruikers-informatie", Folder, Parms, 'ReplaceIfExists' )
   If Result != 1 Then Do
      Say 'FOUT: Schaduw niet gemaakt.'
      End
   End


/* Stel het INI-bestand in, stel de standaardtaal in. */

Result = SysIni( Directory"\ESCRIBA.INI", "ESCRIBA", "Language", Language )
If Result = 'ERROR:' Then Do
   Say 'ERROR: Taalvoorkeur niet opgeslagen.'
   End


/* Ga na of er spellingcontrole is. */

Speller = 1
Result = SysFileTree( "ISPELLER.DLL", 'Files', 'F' )
If Files.0 = 0 Then Speller = 0

Result = SysFileTree( "BUILDHAS.EXE", 'Builder1', 'F' )
Result = SysFileTree( "BUILDHASH.EXE", 'Builder2', 'F' )
If ( ( Builder1.0 = 0 ) & ( Builder2.0 = 0 ) ) Then Speller = 0


/* Installeer de spellingcontrole als die er is. */

If Speller = 1 Then Do
   Say 'Installeert spellingcontrole...'
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
Say 'REXX fout.'
Signal DONE

HALT:
Say 'REXX stop.'
Signal DONE

SYNTAX:
Say 'REXX taalfout.'
Signal DONE

DONE:
Exit
