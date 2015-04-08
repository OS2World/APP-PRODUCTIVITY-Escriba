/* INSTALL.CMD: Install ISPELLER. */

'@Echo Off'
'CHCP 850'

/* Load REXXUTIL */

Call RxFuncAdd 'SysLoadFuncs', 'REXXUTIL', 'SysLoadFuncs'
Call SysLoadFuncs


/* Initialize */

Signal On Failure Name FAILURE
Signal On Halt Name HALT
Signal On Syntax Name SYNTAX

Call SysCls
Say 'Installing ISPELLER...'
Say ''


/* Ask for the target directory name. */

ISpellPath = VALUE('ISPELL',,'OS2ENVIRONMENT')

Default = SysIni('USER', 'ISpeller', 'Path' )
If Default = 'ERROR:' Then Do
  Default = ISpellPath
  if ISpellPath = "" Then Do
    Default = 'c:\ISpeller'
    End
  End 

If SUBSTR(Default,LENGTH(Default),1) <= ' '
  Then Default = SUBSTR(Default,1,LENGTH(Default)-1)

Say 'Please enter the full name of the directory to which'
Say '  you want ISPELLER installed (default 'Default'): '
Parse Pull Directory
If Directory = "" Then Directory = Default


/* Create the target directory if necessary. */

Result = SysFileTree( Directory, 'Dirs', 'D' )
If Dirs.0 = 0 Then
  Do
  Result = SysMkDir( Directory )
  if Result == 0 Then
    Do
    End
  Else
    Do
    Say 'ERROR: Unable to create target directory.'
    Signal DONE
    End
  End
Say ''


/* Save the target directory. */

SysIni( "USER", "ISpeller", "Path", Directory )


/* Perform the installation. */

Say 'Copying ISPELLER to 'Directory' ...'
'Copy READ.ME       'Directory              '1>NUL'
'Copy ISPELLER.DLL  'Directory              '1>NUL'
'Copy RUNTIME.DLL   'Directory              '1>NUL'
'Copy ISP_*.DLL     'Directory              '1>NUL'
'Copy ISP_*.HLP     'Directory              '1>NUL'
Result = SysFileTree( 'ISPELL.EXE', 'Files', 'F' )
If Files.0 = 1 Then
   'Copy ISPELL.EXE    'Directory              '1>NUL'
Result = SysFileTree( 'BUILDHASH.EXE', 'Files', 'F' )
If Files.0 = 1 Then
   'Copy BUILDHASH.EXE 'Directory'\BUILDHAS.EXE 1>NUL'
Result = SysFileTree( 'BUILDHAS.EXE', 'Files', 'F' )
If Files.0 = 1 Then
   'Copy BUILDHAS.EXE 'Directory'\BUILDHAS.EXE 1>NUL'


/* Build each dictionary found. */

Result = SysFileTree( '*.AFF', 'Files', 'F' ) 
If Files.0 > 0 Then 'Copy *.AFF' Directory '1>NUL'
Result = SysFileTree( '*.DIC', 'Files', 'F' ) 
If Files.0 > 0 Then 'Copy *.DIC' Directory '1>NUL'
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
      Directory'\BUILDHAS -s' Dictionary.1 Affix.Index Directory'\'LanguageName'.HSH 1>NUL 2>NUL'
      End /* if */
   End /* do */

/* Update CONFIG.SYS with SET ISPELL=Directory */

if ( \ ( TRANSLATE(ISpellPath) = TRANSLATE(Directory) ) ) Then Do
   Say ''
   Say 'NOTE: You should update your CONFIG.SYS file to'
   Say '  include the following statement:'
   Say ''
   Say '  SET ISPELL='Directory
   Say ''
   End /* do */

/* Announce completion. */

Say 'Installation complete.'

Signal DONE

FAILURE:
Say 'REXX failure.'
Signal DONE

HALT:
Say 'REXX halt.'
Signal DONE

SYNTAX:
Say 'REXX syntax error.'
Signal DONE

DONE:
Exit
