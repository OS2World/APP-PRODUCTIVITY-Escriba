/**************************************************************** ESCRIBA.CPP
 *                                                                          *
 *                  Escriba Word Processor for OS/2-PM                      *
 *                                                                          *
 *    (C) Copyright 1992-1997, by Richard W. Papo, all rights reserved.     *
 *                                                                          *
 ****************************************************************************/

#include "System.h"

#include <direct.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Addons.h"
#include "Controls.h"
#include "Debug.h"
#include "Dialog2.h"
#include "Escriba.h"
#include "Except.h"
#include "External.h"
#include "HelpWin.h"
#include "Main.h"
#include "Module.h"
#include "Process.h"
#include "Profile2.h"
#include "ReString.h"
#include "ShareMem.h"
#include "Speller.h"
#include "Support.h"
#include "Window.h"

// #define DEBUG
 
/****************************************************************************
 *                                                                          *
 *                     Definitions & Declarations                           *
 *                                                                          *
 ****************************************************************************/

  // Constants & Macros

#define MEMORY_NAME "\\SHAREMEM\\" PROGRAM_NAME


  // Type Definitions

typedef struct {        // Structure for first instance to declare it's window publicly.
   HWND MainWindow ;
} SHARED_MEMORY ;


  // Function Prototypes

extern int main ( int argc, char *argv[] ) ;


  // Global Data

extern char HomePath [CCHMAXPATH] = { 0 } ;
extern HelpWindow *Help = 0 ;
extern LanguageModule *Library = 0 ;
extern HMODULE LibraryHandle = 0 ;

AddonRegistry AddonList ( IDM_PRODUCT_LIST + 1 ) ;

 
/****************************************************************************
 *                                                                          *
 *      Program Mainline                                                    *
 *                                                                          *
 ****************************************************************************/

extern int main ( int argc, char *argv[] ) {

  /**************************************************************************
   * Register self.                                                         *
   **************************************************************************/

   Sys_RegisterThread ( ) ;

  /**************************************************************************
   * Set a default exception filter.                                        *
   **************************************************************************/

   REGISTER_EXCEPTION_HANDLER(0);

  /**************************************************************************
   * Start the main block (for constructors/destructors).                   *
   **************************************************************************/

   {

  /**************************************************************************
   * Get the initial file path for loading files from command-line.         *
   **************************************************************************/

   char InitialPath [CCHMAXPATH] ;
   _fullpath ( InitialPath, ".", sizeof(InitialPath) ) ;
   strcat ( InitialPath, "\\" ) ;

  /**************************************************************************
   * Determine the home directory.                                          *
   **************************************************************************/

   char Drive [_MAX_DRIVE+1], Dir[_MAX_DIR+1], Fname[_MAX_FNAME+1], Ext[_MAX_EXT+1] ;
   strupr ( argv[0] ) ;
   _fullpath ( HomePath, argv[0], sizeof(HomePath) ) ;
   _splitpath ( HomePath, Drive, Dir, Fname, Ext ) ;
   if ( Dir[strlen(Dir)-1] == '\\' )
      Dir[strlen(Dir)-1] = 0 ;
   strcpy ( HomePath, Drive ) ;
   strcat ( HomePath, Dir ) ;

   _chdrive ( Drive[0] - 'A' + 1 ) ;
   _chdir ( "\\" ) ;
   _chdir ( Dir ) ;

  /**************************************************************************
   * Set the C RunTime Library error message file handle.                   *
   **************************************************************************/

   #ifdef __DEBUG_ALLOC__
      Log ( "Escriba::main: Program started." ) ;
      _set_crt_msg_handle ( fileno(Logfile) ) ;
   #else
      #ifdef DEBUG
         Log ( "Escriba::main: Program started." ) ;
      #endif // DEBUG
   #endif // __DEBUG_ALLOC__

  /**************************************************************************
   * Determine if another instance of this program is present.              *
   *   If so, pass the command-line information to it.                      *
   **************************************************************************/

   #ifdef DEBUG
      Log ( "Escriba::main: Checking for another instance already loaded." ) ;
   #endif // DEBUG

   PublicMemory Memory ( MEMORY_NAME, sizeof(SHARED_MEMORY), TRUE ) ;
   if ( NOT Memory.QueryCreated() ) {

      // Get the main instance's window handle.  Wait up to 20 seconds if necessary.
      HWND MainWindow = ((SHARED_MEMORY*)Memory.QueryMemory())->MainWindow ;
      clock_t LastClock = clock ( ) ;
      while ( MainWindow == 0 ) {
         if ( ( ( clock() - LastClock ) / CLOCKS_PER_SEC ) > 20 ) {
            Log ( "ERROR: Unable to get previous instance window handle." ) ;
            return ( 1 ) ;
         } /* endif */
         DosSleep ( 100 ) ;
         MainWindow = ((SHARED_MEMORY*)Memory.QueryMemory())->MainWindow ;
      } /* endwhile */

      // If any command-line parameters were given . . .
      if ( argc > 1 ) {

         // Build a command-line string.
         char Parms [512] = { 0 } ;
         int ParmLength = 0 ;
         while ( --argc ) {
            strcpy ( Parms+ParmLength, *(++argv) ) ;
            ParmLength += strlen(*argv) + 1 ;
         } /* endwhile */
         Parms[++ParmLength] = 0 ;

         // Get the original process's ID.
         PID Process ;
         TID Thread ;
         if ( !WinQueryWindowProcess ( MainWindow, &Process, &Thread ) ) {
            char Message [512] ;
            Log ( "ERROR: Unable to query window process.  %s", InterpretWinError(0,Message) ) ;
            return ( 1 ) ;
         } /* endif */

         // Allocate shared memory to hold the current path.
         PVOID Memory1 ;
         APIRET Status = DosAllocSharedMem ( &Memory1, 0, strlen(InitialPath)+1, fALLOC | OBJ_GIVEABLE | OBJ_GETTABLE) ;
         if ( Status ) {
            Log ( "ERROR: Unable to allocate shared memory.  Status %d.", Status ) ;
            return ( 1 ) ;
         } /* endif */
         strcpy ( PCHAR(Memory1), InitialPath ) ;

         // Allocate shared memory to hold the command-line.
         PVOID Memory2 ;
         Status = DosAllocSharedMem ( &Memory2, 0, ParmLength, fALLOC | OBJ_GIVEABLE | OBJ_GETTABLE) ;
         if ( Status ) {
            Log ( "ERROR: Unable to allocate shared memory.  Status %d.", Status ) ;
            return ( 1 ) ;
         } /* endif */
         memcpy ( Memory2, Parms, ParmLength ) ;

         // Make both shared memory blocks available to the original process.
         Status = DosGiveSharedMem ( Memory1, Process, PAG_READ | PAG_WRITE ) ;
         DosFreeMem ( Memory1 ) ;
         if ( Status ) {
            Log ( "ERROR: Unable to give shared memory.  Status %d.", Status ) ;
            return ( 1 ) ;
         } /* endif */
         Status = DosGiveSharedMem ( Memory2, Process, PAG_READ | PAG_WRITE ) ;
         DosFreeMem ( Memory2 ) ;
         if ( Status ) {
            Log ( "ERROR: Unable to give shared memory.  Status %d.", Status ) ;
            return ( 1 ) ;
         } /* endif */

         // Pass the information to the original process.
         Sys_PostMessage ( MainWindow, WM_LOAD_FILE, MPFROMP(Memory1), MPFROMP(Memory2) ) ;

      } /* endif */

      // Bring the previous instance to the fore.
      Sys_SetActiveWindow ( MainWindow ) ;

      // We're outta here . . .
      return ( 0 ) ;

   } /* endif */

  /**************************************************************************
   * If the ISPELL environment variable isn't set, set it to the current    *
   *   directory, before we change it.  This assumes that the current       *
   *   directory is also the directory to which Escriba (and ISPELL) was    *
   *   installed.  This is true with the basic installation.                *
   **************************************************************************/

   #ifdef DEBUG
      Log ( "Escriba::main: Setting ISPELL environment variable." ) ;
   #endif // DEBUG

   {
      char *p = getenv ( "ISPELL" ) ;
      if ( p == 0 ) {
         static char SetString [_MAX_PATH+10] ;
         sprintf ( SetString, "ISPELL=%s", HomePath ) ;
         _putenv ( SetString ) ;
      } /* endif */
   }

  /**************************************************************************
   * Get application language module.                                       *
   **************************************************************************/

   #ifdef DEBUG
      Log ( "Escriba::main: Loading default language." ) ;
   #endif // DEBUG

   char DefaultLanguage [80] = { 0 } ;

   { /* startblock */
      Profile IniFile ( PSZ(PROGRAM_NAME), 0, HomePath ) ;
      if ( IniFile.QueryHandle() ) 
         IniFile.GetString ( "Language", DefaultLanguage, sizeof(DefaultLanguage) ) ;
      else
         strcpy ( DefaultLanguage, "English" ) ;
   } /* endblock */

   Library = Language_Create ( PROGRAM_NAME, REVISION, IDS_TITLE1, DefaultLanguage ) ;
   if ( Library == 0 ) {
      Process Proc ( PROGRAM_NAME, HWND_DESKTOP, 0 ) ;
      Debug ( HWND_DESKTOP, "ERROR: Unable to find language module for %s, %s, %s.", PROGRAM_NAME, REVISION, DefaultLanguage ) ;
      return ( 1 ) ;
   } /* endif */

   LibraryHandle = Library->QueryHandle() ;

  /**************************************************************************
   * Get the program title.                                                 *
   **************************************************************************/

   ResourceString Title ( Library->QueryHandle(), IDS_TITLE ) ;

  /**************************************************************************
   * Determine the default codepage.                                        *
   **************************************************************************/

   #ifdef DEBUG
      Log ( "Escriba::main: Determining codepage to use." ) ;
   #endif // DEBUG

   PUSHORT pCodePage = Library->QueryCodepages() ;
   while ( *pCodePage ) {
      if ( !DosSetProcessCp ( *pCodePage ) )
         break ;
      pCodePage ++ ;
   } /* endwhile */

   if ( *pCodePage == 0 ) {
      ResourceString Format ( Library->QueryHandle(), IDS_WARNING_BADCODEPAGE ) ;
      CHAR Message [200] ;
      sprintf ( Message, PCHAR(Format), *Library->QueryCodepages() ) ;
      Log ( "%s", Message ) ;
      MessageBox ( HWND_DESKTOP, HWND_DESKTOP, PSZ(Message),
         PSZ(Title), 0, MB_ENTER | MB_ICONEXCLAMATION, LibraryHandle, IDS_MESSAGE_BOX_STRINGS ) ;
      return ( 1 ) ;
   } /* endif */
 
  /**************************************************************************
   * Initialize the process with an extra-large message queue.              *
   **************************************************************************/

   #ifdef DEBUG
      Log ( "Escriba::main: Connecting to PM." ) ;
   #endif // DEBUG

   Process Proc ( PCHAR(Title), HWND_DESKTOP, Library->QueryHandle(), 100, *pCodePage ) ;
   if ( Proc.QueryAnchor() == 0 ) 
      return ( 1 ) ;

  /**************************************************************************
   * Register the custom control classes.                                   *
   **************************************************************************/

   #ifdef DEBUG
      Log ( "Escriba::main: Registering custom control classes." ) ;
   #endif // DEBUG

   RegisterControls ( Proc.QueryAnchor() ) ;

  /**************************************************************************
   * Open up debug timer.                                                   *
   **************************************************************************/

   #ifdef DEBUG
      Log ( "Escriba::main: Opening debug timer." ) ;
   #endif // DEBUG

   OpenTimer ( ) ;

  /**************************************************************************
   * Open the registration library, if it is present.                       *
   **************************************************************************/

   #ifdef DEBUG
      Log ( "Escriba::main: About to open registration file." ) ;
   #endif // DEBUG

   char *RegistrationName = PROGRAM_NAME"R" ;
   Module *pRegistration = new Module ( RegistrationName, FALSE ) ;
   if ( pRegistration->QueryHandle() == 0 )
      RegistrationName = "PLUMAR" ;
   delete pRegistration, pRegistration = 0 ;

   Module Registration = Module ( RegistrationName, FALSE ) ;

  /**************************************************************************
   * Load any extenders (except spell-check).                               *
   **************************************************************************/

   #ifdef DEBUG
      Log ( "Escriba::main: About to load extenders." ) ;
   #endif // DEBUG

   AddonList.Build ( DefaultLanguage ) ;

  /**************************************************************************
   * Get the country information.                                           *
   **************************************************************************/

   #ifdef DEBUG
      Log ( "Escriba::main: About to get country information." ) ;
   #endif // DEBUG

   GetCountryInfo ( 0, 0 ) ;

  /**************************************************************************
   * Set any language-specific globals.                                     *
   **************************************************************************/

   #ifdef DEBUG
      Log ( "Escriba::main: About to set language-specific globals." ) ;
   #endif // DEBUG

   ResourceString AllGraphicFiles ( LibraryHandle, IDS_ALL_GRAPHICS ) ;
   GraphicTypeList[0] = PSZ ( AllGraphicFiles ) ;

  /**************************************************************************
   * Check for command-line options and remove them from the argument list. *
   **************************************************************************/

   #ifdef DEBUG
      Log ( "Escriba::main: About to parse the command line." ) ;
   #endif // DEBUG

   ResourceString ResetCommand ( Library->QueryHandle(), IDS_PARMS_RESET ) ;
   BOOL Reset = FALSE ;

// ResourceString PrintCommand ( Library->QueryHandle(), IDS_PARMS_PRINT ) ;
// BOOL Print = FALSE ;

   ResourceString TrapCommand ( Library->QueryHandle(), IDS_PARMS_TRAP ) ;
   BOOL Trap = FALSE ;

   int i = 1 ;
   char **p = argv + 1 ;
   while ( i < argc ) {
      if ( **p == '-' ) {
         if ( !stricmp ( (*p)+1, PCHAR(ResetCommand) ) ) {
            Reset = TRUE ;
//       } else if ( !stricmp ( (*p)+1, PCHAR(PrintCommand) ) ) {
//          Print = TRUE ;
         } else if ( !stricmp ( (*p)+1, PCHAR(TrapCommand) ) ) {
            Trap = TRUE ;
         } else {
            Debug ( HWND_DESKTOP, "ERROR: Invalid command-line parameter '%s'.", (*p)+1 ) ;
            return ( 1 ) ;
         } /* endif */
         for ( int j=i+1; j<argc; j++ ) {
            argv[j-1] = argv[j] ;
         } /* endfor */
         argv[j] = 0 ;
         argc -- ;
         continue ;
      } /* endif */
      i ++ ;
      p ++ ;
   } /* endwhile */

  /**************************************************************************
   * Create the help instance.                                              *
   **************************************************************************/

   #ifdef DEBUG
      Log ( "Escriba::main: About to create the help instance." ) ;
   #endif // DEBUG

   _splitpath ( Library->QueryName(), Drive, Dir, Fname, Ext ) ;
   char HelpFileName [CCHMAXPATH] ;
   sprintf ( HelpFileName, "%s.hlp", Fname ) ;

   ResourceString HelpTitle ( Library->QueryHandle(), IDS_HELPTITLE ) ;

   Help = new HelpWindow ( Proc.QueryAnchor(), 0, ID_MAIN, PSZ(HelpFileName), PSZ(HelpTitle) ) ;

   if ( Help->QueryHandle() == 0 ) {
      ERRORID Error = Sys_GetLastError ( Proc.QueryAnchor() ) ;
      ResourceString Format ( Library->QueryHandle(), IDS_ERROR_CREATEHELP ) ;
      CHAR Message [200] ;
      sprintf ( Message, PCHAR(Format), Error ) ;
      Log ( "%s", Message ) ;
      MessageBox ( HWND_DESKTOP, HWND_DESKTOP, PSZ(Message), PSZ(Title), 0, MB_ENTER | MB_ICONEXCLAMATION, LibraryHandle, IDS_MESSAGE_BOX_STRINGS ) ;
   } /* endif */

  /**************************************************************************
   * Open/create the profile file.  Reset if requested.                     *
   **************************************************************************/

   #ifdef DEBUG
      Log ( "Escriba::main: About to open profile file." ) ;
   #endif // DEBUG

   Profile2 IniFile ( PSZ(PROGRAM_NAME), Proc.QueryAnchor(), Library->QueryHandle(),
      IDD_PROFILE_PATH, Help, Reset ) ;

   if ( IniFile.QueryHandle() == 0 ) {
      ResourceString Message ( Library->QueryHandle(), IDS_ERROR_PRFOPENPROFILE ) ;
      Log ( "%s", PSZ(Message) ) ;
      MessageBox ( HWND_DESKTOP, HWND_DESKTOP, PSZ(Message), PSZ(Title), 0, MB_ENTER | MB_ICONEXCLAMATION, LibraryHandle, IDS_MESSAGE_BOX_STRINGS ) ;
      CloseTimer() ;
      return ( 2 ) ;
   } /* endif */

  /**************************************************************************
   * Get profile data. Try the OS2.INI first, then try for private INI.     *
   *   If obtained from OS2.INI, erase it afterwards and resave it.         *
   **************************************************************************/

   #ifdef DEBUG
      Log ( "Escriba::main: About to get profile data." ) ;
   #endif // DEBUG

   INIDATA IniData ( Registration.QueryHandle() ) ;
   IniFile.GetIniData ( IniData ) ;

   if ( IniData.Language [0] == 0 ) 
      strcpy ( IniData.Language, DefaultLanguage ) ;

  /**************************************************************************
   * Activate the spell checker, if it is present.                          *
   **************************************************************************/

   #ifdef DEBUG
      Log ( "Escriba::main: About to open the default dictionary." ) ;
   #endif // DEBUG

   char SpellerPath [ CCHMAXPATH ] = { 0 } ;
   if ( getenv ( "ISPELL" ) ) {
      strcpy ( SpellerPath, getenv ( "ISPELL" ) ) ;
      strcat ( SpellerPath, "\\" ) ;
   } /* endif */
   strcat ( SpellerPath, "ISPELLER.DLL" ) ;
   Dictionary *Speller = new Dictionary ( SpellerPath, DefaultLanguage ) ;
   if ( Speller->QueryLibrary() == 0 ) {
      #ifdef DEBUG
         Log ( "Escriba::main: Could not find spellchecker.  Will try again through LIBPATH." ) ;
      #endif // DEBUG
      delete Speller, Speller = 0 ;
      Speller = new Dictionary ( "ISPELLER", DefaultLanguage ) ;
   } /* endif */
   if ( Speller->Available() ) {
      #ifdef DEBUG
         Log ( "Escriba::main: Adding dictionary object to extension list." ) ;
      #endif // DEBUG
      AddonList.Add ( Speller ) ;
   } else {
      #ifdef DEBUG
         Log ( "Escriba::main: Dictionary object could not be fully created." ) ;
      #endif // DEBUG
      delete Speller, Speller = 0 ;
   } /* endif */

  /**************************************************************************
   * Create the frame window.                                               *
   **************************************************************************/

   #ifdef DEBUG
      Log ( "Escriba::main: About to create the frame window." ) ;
   #endif // DEBUG

   FRAMECDATA FrameControlData ;
   FrameControlData.cb = sizeof(FrameControlData) ;
   FrameControlData.flCreateFlags =
      FCF_TITLEBAR | FCF_MENU | FCF_ICON | FCF_NOBYTEALIGN | FCF_ACCELTABLE | FCF_SYSMENU |
      ( Trap ? 0 : FCF_SIZEBORDER | FCF_MINMAX ) ;
   FrameControlData.hmodResources = USHORT ( Library->QueryHandle() ) ;
   FrameControlData.idResources = ID_MAIN ;

   Window Frame ( HWND_DESKTOP, WC_FRAME, PSZ(Title),
      IniData.Animate ? WS_ANIMATE : 0,
      0, 0, 0, 0, HWND_DESKTOP, HWND_TOP, ID_MAIN,
      &FrameControlData, NULL ) ;

   if ( Frame.QueryHandle() == 0 ) {
      ERRORID Error = Sys_GetLastError ( Proc.QueryAnchor() ) ;
      ResourceString Format ( Library->QueryHandle(), IDS_ERROR_CREATEFRAME ) ;
      CHAR Message [200] ;
      sprintf ( Message, PCHAR(Format), Error ) ;
      Log ( "%s", Message ) ;
      MessageBox ( HWND_DESKTOP, HWND_DESKTOP, PSZ(Message), PSZ(Title), 0, MB_ENTER | MB_ICONEXCLAMATION, LibraryHandle, IDS_MESSAGE_BOX_STRINGS ) ;
      CloseTimer() ;
      return ( 3 ) ;
   } /* endif */

  /**************************************************************************
   * Associate the help instance with the frame window.                     *
   **************************************************************************/

   Help->Associate ( Frame.QueryHandle() ) ;

  /**************************************************************************
   * Register the client window class.                                      *
   **************************************************************************/

   if ( !WinRegisterClass ( Proc.QueryAnchor(), PSZ(CLASS_NAME),
      MessageProcessor, CS_MOVENOTIFY, sizeof(PVOID) ) ) {
      ERRORID Error = Sys_GetLastError ( Proc.QueryAnchor() ) ;
      ResourceString Format ( Library->QueryHandle(), IDS_ERROR_WINREGISTERCLASS ) ;
      CHAR Message [200] ;
      sprintf ( Message, PCHAR(Format), CLASS_NAME, Error ) ;
      Log ( "%s", Message ) ;
      MessageBox ( HWND_DESKTOP, HWND_DESKTOP, PSZ(Message),
         PSZ(Title), IDD_ERROR_WINREGISTERCLASS, MB_ENTER | MB_ICONEXCLAMATION | MB_HELP, LibraryHandle, IDS_MESSAGE_BOX_STRINGS ) ;
      CloseTimer() ;
      return ( 4 ) ;
   } /* endif */

  /**************************************************************************
   * Build the presentation parameters for the main window.                 *
   **************************************************************************/

   COLOR BackColor = IniData.fMainColors[0] ? IniData.MainColors[0] :
      WinQuerySysColor ( HWND_DESKTOP, SYSCLR_APPWORKSPACE, 0 ) ;
   ULONG Ids[] = { PP_BACKGROUNDCOLOR } ;
   ULONG ByteCounts[] = { sizeof(BackColor) } ;
   PUCHAR Params[] = { PUCHAR(&BackColor) } ;

   PPRESPARAMS PresParams = BuildPresParams ( sizeof(Ids)/sizeof(Ids[0]),
     Ids, ByteCounts, Params ) ;

  /**************************************************************************
   * Create client window.  If this fails, destroy frame and return.        *
   **************************************************************************/

   #ifdef DEBUG
      Log ( "Escriba::main: About to create the client window." ) ;
   #endif // DEBUG

   PARMS Parms ;
   Parms.Filler = 0 ;
   Parms.Library = Library ;
   Parms.Registration = & Registration ;
   Parms.IniFile = & IniFile ;
   Parms.IniData = & IniData ;
   Parms.Speller = Speller ;
   Parms.InitialPath = InitialPath ;
   Parms.argc = argc ;
   Parms.argv = argv ;
   Parms.Trap = Trap ;

   Window Client ( Frame.QueryHandle(), PSZ(CLASS_NAME), PSZ(""), WS_CLIPCHILDREN | WS_CLIPSIBLINGS,
      0, 0, 0, 0, Frame.QueryHandle(), HWND_BOTTOM, FID_CLIENT, &Parms, PresParams ) ;

   free ( PresParams ) ;

   if ( Client.QueryHandle() == 0 ) {
      ERRORID Error = Sys_GetLastError ( Proc.QueryAnchor() ) ;
      ResourceString Format ( Library->QueryHandle(), IDS_ERROR_CREATECLIENT ) ;
      CHAR Message [200] ;
      sprintf ( Message, PCHAR(Format), Error ) ;
      Log ( "%s", Message ) ;
      MessageBox ( HWND_DESKTOP, HWND_DESKTOP, PSZ(Message),
         PSZ(Title), IDD_ERROR_CREATECLIENT, MB_ENTER | MB_ICONEXCLAMATION | MB_HELP, LibraryHandle, IDS_MESSAGE_BOX_STRINGS ) ;
      CloseTimer ( ) ;
      return ( 5 ) ;
   } /* endif */

   ((SHARED_MEMORY*)Memory.QueryMemory())->MainWindow = Client.QueryHandle() ;

  /**************************************************************************
   * Wait for and process messages to the window's queue.  Terminate        *
   *   when the WM_QUIT message is received.                                *
   **************************************************************************/

   #ifdef DEBUG
      Log ( "Escriba::main: About to enter the main message loop." ) ;
   #endif // DEBUG

   Sys_ProcessMessages ( Proc.QueryAnchor() ) ;

  /**************************************************************************
   * Discard all that had been requested of the system.                     *
   **************************************************************************/

   #ifdef DEBUG
      Log ( "Escriba::main: About to close the HR Timer." ) ;
   #endif // DEBUG

   CloseTimer ( ) ;

  /**************************************************************************
   * Invoke nearly all the destructors.                                     *
   **************************************************************************/

   #ifdef DEBUG
      Log ( "Escriba::main: Invoking most destructors." ) ;
   #endif // DEBUG

   } /* Invoke all destructors except the global ones. */

  /**************************************************************************
   * Invoke the remaining destructors.                                      *
   **************************************************************************/

   #ifdef DEBUG
      Log ( "Escriba::main: Clearing the addon list." ) ;
   #endif // DEBUG

   AddonList.Clear ( ) ;

   #ifdef DEBUG
      Log ( "Escriba::main: Clearing the file type list." ) ;
   #endif // DEBUG

   ClearFileTypeList ( ) ;

   #ifdef DEBUG
      Log ( "Escriba::main: Clearing the graphic type list." ) ;
   #endif // DEBUG

   ClearGraphicTypeList ( ) ;

   #ifdef DEBUG
      Log ( "Escriba::main: Destroying the help instance." ) ;
   #endif // DEBUG

   if ( Help ) delete Help ;

   #ifdef DEBUG
      Log ( "Escriba::main: Destroying the language library instance." ) ;
   #endif // DEBUG

   if ( Library ) delete Library ;

  /**************************************************************************
   * Check to see if any memory remains allocated.                          *
   **************************************************************************/

   #ifdef __DEBUG_ALLOC__
      Log ( "Escriba::main: Checking the heap." ) ;
      _dump_allocated ( 64 ) ;
      Log ( "Escriba::main: Program ended." ) ;
      fclose ( Logfile ) ;
   #else
      #ifdef DEBUG
         Log ( "Escriba::main: Program ended." ) ;
      #endif // DEBUG
   #endif // __DEBUG_ALLOC__

  /**************************************************************************
   * Discard the exception filter.                                          *
   **************************************************************************/

   UNREGISTER_EXCEPTION_HANDLER(0);

  /**************************************************************************
   * Terminate the process.                                                 *
   **************************************************************************/

   return ( 0 ) ;
}
