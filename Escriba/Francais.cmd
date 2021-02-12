/* FRANCAIS.CMD : Installation de ESCRIBA, version franáaise. */

'@Echo Off'
'CHCP 850'

/* Nom de l'objet Ö installer */

Title = 'Escriba 0,97'
Language = 'Franáais'
FileName = 'Francais'

/* Chargement de REXXUTIL */

Call RxFuncAdd 'SysLoadFuncs', 'REXXUTIL', 'SysLoadFuncs'
Call SysLoadFuncs


/* Initialisations */

Signal On Failure Name FAILURE
Signal On Halt Name HALT
Signal On Syntax Name SYNTAX

Call SysCls
Say "Installation d'Escriba..."
Say ""


/* VÇrification de l'existence des diffÇrents fichiers */

Result = SysFileTree( 'ESCRIBA.EXE', 'Files', 'F' )
If Files.0 = 0 Then
   Do
   Say 'ERREUR : ESCRIBA.EXE introuvable !'
   Signal DONE
   End

Result = SysFileTree( FileName".DLL", 'Files', 'F' )
If Files.0 = 0 Then
   Do
   Say 'ERREUR : 'FileName'.DLL introuvable !'
   Signal DONE
   End

Result = SysFileTree( FileName".HLP", 'Files', 'F' )
If Files.0 = 0 Then
   Do
   Say 'ERREUR : 'FileName'.HLP introuvable !'
   Signal DONE
   End


/* Saisie du rÇpertoire de destination */

BootDrive = Filespec('Drive',Value('SYSTEM_INI',,'OS2ENVIRONMENT'))
Default = SysIni('USER', 'ESCRIBA', 'INIPATH' )
If Default = 'ERROR:'
  Then Default = BootDrive'\OS2\Apps'
If SUBSTR(Default,LENGTH(Default),1) <= ' '
  Then Default = SUBSTR(Default,1,LENGTH(Default)-1)

Say 'Veuillez saisir le nom complet du rÇpertoire oó Escriba doit àtre installÇ'
Say '(par dÇfaut, 'Default') : '
Parse Pull Directory
If Directory = "" Then Directory = Default


/* Si nÇcessaire, crÇation du rÇpertoire destination */

Result = SysFileTree( Directory, 'Dirs', 'D' )
If Dirs.0 = 0 Then
   Do
   Result = SysMkDir( Directory )
   if Result == 0 Then
      Do
      End
   Else
      Do
      Say 'ERREUR : Impossible de crÇer le rÇpertoire destination.'
      Signal DONE
      End
   End
Say ''


/* Sauvegarde du rÇpertoire d'installation */

SysIni( "USER", "ESCRIBA", "INIPATH", Directory )


/* Installation. */

Say 'Copie de Escriba dans 'Directory' ...'
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

Say 'CrÇation du dossier...'
Type = 'WPFolder'
Folder = '<WP_DESKTOP>'
Parms = "OBJECTID=<ESCRIBAF>;ICONFILE="Directory"\ESCRIBA.ICO;ALWAYSSORT=YES;"
Result = SysCreateObject( Type, 'Escriba', Folder, Parms, 'ReplaceIfExists' )
If Result != 1 Then
   Say 'ERREUR : Le dossier n'D2C(39)'a pas ÇtÇ crÇÇ.'

Say 'CrÇation de l'D2C(39)'objet programme...'
Type = 'WPProgram'
Folder = '<ESCRIBAF>'
Parms = "MINWIN=DESKTOP;PROGTYPE=PM;EXENAME="Directory"\ESCRIBA.EXE;STARTUPDIR="Directory";OBJECTID=<ESCRIBA>;NOPRINT=YES;ASSOCTYPE=Escriba,Pluma,Plain Text,Bitmap,GIF,HTML,JPG,Metafile,PCC,PCX,RTF,TIFF;ASSOCFILTER=*.TXT,*.DOC,*.ME,*.BAT,*.CMD,*.MOI,*.BMP,*.GIF,*.HTM,*.HTML,*.JPG,*.JPEG,*.JFI,*.JFIF,*.MET,*.PCC,*.PCX,*.RTF,*.TIF,*.TIFF;"
Result = SysCreateObject( Type, Title, Folder, Parms, 'ReplaceIfExists' )
If Result != 1 Then
   Say 'ERREUR : L'D2C(39)'objet n'D2C(39)'a pas ÇtÇ crÇÇ.'

Say 'CrÇation des objets documentation...'
Type = 'WPProgram'
Parms = "MINWIN=DESKTOP;PROGTYPE=PM;EXENAME="Directory"\ESCRIBA.EXE;STARTUPDIR="Directory";OBJECTID=<ESCRIBAD>;PARAMETERS=ESCRIBA.DOC"
Result = SysCreateObject( Type, "Lisez moi", Folder, Parms, 'ReplaceIfExists' )
If Result != 1 Then
   Say 'ERREUR : L'D2C(39)'objet documentation n'D2C(39)'a pas ÇtÇ crÇÇ.'

Result = SysFileTree( Directory"\ESCRIBA.INF", 'Files', 'F' )
If Files.0 > 0 Then Do
   Type = 'WPProgram'
   Parms = "MINWIN=DESKTOP;PROGTYPE=PM;EXENAME=VIEW.EXE;STARTUPDIR="Directory";OBJECTID=<ESCRIBAI>;PARAMETERS=ESCRIBA.INF Introduction;"
   Result = SysCreateObject( Type, "Manuel utilisateur d'Escriba", Folder, Parms, 'ReplaceIfExists' )
   If Result != 1 Then Do
      Say 'ERREUR : L'D2C(39)'objet documentation n'D2C(39)'a pas ÇtÇ crÇÇ.'
      End
   End


/* Initialisation du fichier INI, configuration de la langue par dÇfaut */

Result = SysIni( Directory"\ESCRIBA.INI", "ESCRIBA", "Language", Language )
If Result = 'ERROR:' Then Do
   Say "ERREUR : La langue Ö utiliser n'a pas ÇtÇ enregistrÇe."
   End


/* voyons si le correcteur orhographique est prÇsent */

Speller = 1
Result = SysFileTree( "ISPELLER.DLL", 'Files', 'F' )
If Files.0 = 0 Then Speller = 0

Result = SysFileTree( "BUILDHAS.EXE", 'Builder1', 'F' )
Result = SysFileTree( "BUILDHASH.EXE", 'Builder2', 'F' )
If ( ( Builder1.0 = 0 ) & ( Builder2.0 = 0 ) ) Then Speller = 0


/* s'il est prÇsent, installation du correcteur orthographique */

If Speller = 1 Then Do
   Say 'Installation du correcteur orthographique...'
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
Say 'êchec REXX.'
Signal DONE

HALT:
Say 'Halte REXX.'
Signal DONE

SYNTAX:
Say 'Erreur de syntaxe REXX.'
Signal DONE

DONE:
Exit
