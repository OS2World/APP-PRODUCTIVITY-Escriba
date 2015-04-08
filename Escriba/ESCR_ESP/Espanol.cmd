/* ESPANOL.CMD: Instalaci¢n de ESCRIBA en espa¤ol. */

'@Echo Off'
'CHCP 850'

/* T¡tulo del objeto a instalar . */

Title = 'Escriba 0.96d'
Language = 'Espa¤ol'
FileName = 'Espanol'


/* Cargar REXXUTIL */

Call RxFuncAdd 'SysLoadFuncs', 'REXXUTIL', 'SysLoadFuncs'
Call SysLoadFuncs


/* Inicializaci¢n */

Signal On Failure Name FAILURE
Signal On Halt Name HALT
Signal On Syntax Name SYNTAX

Call SysCls
Say 'Instalando ESCRIBA...'
Say ''


/* Verificar la existencia de los ficheros. */

Result = SysFileTree( 'ESCRIBA.EXE', 'Files', 'F' )
If Files.0 = 0 Then
   Do
   Say 'ERROR: ­No se encuentra ESCRIBA.EXE!'
   Signal DONE
   End

Result = SysFileTree( FileName".DLL", 'Files', 'F' )
If Files.0 = 0 Then
   Do
   Say 'ERROR: ­No se encuentra 'FileName'.DLL!'
   Signal DONE
   End

Result = SysFileTree( FileName".HLP", 'Files', 'F' )
If Files.0 = 0 Then
   Do
   Say 'ERROR: ­No se encuentra 'FileName'.HLP!'
   Signal DONE
   End


/* Preguntar el directorio destino. */

BootDrive = Filespec('Drive',Value('SYSTEM_INI',,'OS2ENVIRONMENT'))
Default = SysIni('USER', 'ESCRIBA', 'INIPATH' )
If Default = 'ERROR:'
  Then Default = BootDrive'\OS2\Apps'
If SUBSTR(Default,LENGTH(Default),1) <= ' '
  Then Default = SUBSTR(Default,1,LENGTH(Default)-1)

Say 'Introduzca, por favor, el nombre completo del directorio en donde'
Say '  desea instalar ESCRIBA (por defecto es 'Default'): '
Parse Pull Directory
If Directory = "" Then Directory = Default


/* Crear el directorio si es necesario. */

Result = SysFileTree( Directory, 'Dirs', 'D' )
If Dirs.0 = 0 Then
   Do
   Result = SysMkDir( Directory )
   if Result == 0 Then
      Do
      End
   Else
      Do
      Say 'ERROR: ­Imposible de crear el directorio destino!'
      Signal DONE
      End
   End
Say ''


/* Guardar el nombre del directorio destino. */

SysIni( "USER", "ESCRIBA", "INIPATH", Directory )


/* Completar la instalaci¢n. */

Say 'Copiando ESCRIBA a 'Directory' ...'
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

Say 'Creating program folder...'
Type = 'WPFolder'
Folder = '<WP_DESKTOP>'
Parms = "OBJECTID=<ESCRIBAF>;ICONFILE="Directory"\ESCRIBA.ICO;ALWAYSSORT=YES;"
Result = SysCreateObject( Type, 'Escriba', Folder, Parms, 'ReplaceIfExists' )
If Result != 1 Then
   Say 'ERROR: No pudo hacerse el cuaderno.'

Say 'Creando el objeto del programa...'
Type = 'WPProgram'
Folder = '<ESCRIBAF>'
Parms = "MINWIN=DESKTOP;PROGTYPE=PM;EXENAME="Directory"\ESCRIBA.EXE;STARTUPDIR="Directory";OBJECTID=<ESCRIBA>;NOPRINT=YES;ASSOCTYPE=Escriba,Pluma,Plain Text,Bitmap,GIF,JPG,Metafile,PCC,PCX,TIFF,HTML;ASSOCFILTER=*.TXT,*.DOC,*.ME,*.BAT,*.CMD,*.MOI,*.BMP,*.GIF,*.JPG,*.JPEG,*.JFI,*.JFIF,*.MET,*.PCC,*.PCX,*.TIF,*.TIFF;*.HTM;*.HTML;"
Result = SysCreateObject( Type, Title, Folder, Parms, 'ReplaceIfExists' )
If Result != 1 Then
   Say 'ERROR: ­No se ha podido crear el objeto!'

Say 'Creating documentation objects...'
Type = 'WPProgram'
Parms = "MINWIN=DESKTOP;PROGTYPE=PM;EXENAME="Directory"\ESCRIBA.EXE;STARTUPDIR="Directory";OBJECTID=<ESCRIBAD>;PARAMETERS=ESCRIBA.DOC"
Result = SysCreateObject( Type, "Leame", Folder, Parms, 'ReplaceIfExists' )
If Result != 1 Then
   Say 'ERROR: ­No se ha podido crear el objeto sombra.'

Result = SysFileTree( Directory"\ESCRIBA.INF", 'Files', 'F' )
If Files.0 > 0 Then Do
   Type = 'WPProgram'
   Parms = "MINWIN=DESKTOP;PROGTYPE=PM;EXENAME=VIEW.EXE;STARTUPDIR="Directory";OBJECTID=<ESCRIBAI>;PARAMETERS=ESCRIBA.INF Introducci¢n;"
   Result = SysCreateObject( Type, "Referencia del usuario para Escriba", Folder, Parms, 'ReplaceIfExists' )
   If Result != 1 Then Do
      Say 'ERROR: ­No se ha podido crear el objeto sombra.'
      End
   End


/* Initialize the INI file, setting the default language. */

Result = SysIni( Directory"\ESCRIBA.INI", "ESCRIBA", "Language", Language )
If Result = 'ERROR:' Then Do
   Say 'ERROR: No se puedo grabar la preferencia para espa¤ol.'
   End


/* See if the spell checker is present. */

Speller = 1
Result = SysFileTree( "ISPELLER.DLL", 'Files', 'F' )
If Files.0 = 0 Then Speller = 0

Result = SysFileTree( "BUILDHAS.EXE", 'Builder1', 'F' )
Result = SysFileTree( "BUILDHASH.EXE", 'Builder2', 'F' )
If ( ( Builder1.0 = 0 ) & ( Builder2.0 = 0 ) ) Then Speller = 0


/* Install the spell checker, if present. */

If Speller = 1 Then Do
   Say 'Instalando el chequeador de ortograf¡a...'
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
Say 'Error en REXX.'
Signal DONE

HALT:
Say 'REXX interrumpido.'
Signal DONE

SYNTAX:
Say 'Error de s¡ntaxis REXX.'
Signal DONE

DONE:
Exit
