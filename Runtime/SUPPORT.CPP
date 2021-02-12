/**************************************************************** SUPPORT.CPP
 *                                                                          *
 *                Presentation Manager Support Functions                    *
 *                                                                          *
 ****************************************************************************/

#include "System.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Debug.h"
#include "ReString.h"
#include "Support.h"

 
/****************************************************************************
 *                                                                          *
 *                        Message Dispatcher                                *
 *                                                                          *
 ****************************************************************************/

extern MRESULT DispatchMessage
(
  HWND    hwnd,
  ULONG   msg,
  MPARAM1  mp1,
  MPARAM2  mp2,
  PMETHOD MethodTable,
  USHORT  MethodCount,
  PWINDOWPROC DefaultProcessor
)
{
 /***************************************************************************
  * Process messages according to object's class method table.              *
  ***************************************************************************/

  PMETHOD pMethod = MethodTable ;
  USHORT cNumberLeft = MethodCount ;

  while ( ( cNumberLeft ) AND ( pMethod->Action != msg ) ) {
    pMethod ++ ;
    cNumberLeft -- ;
  }

  MRESULT mr ;

  if ( cNumberLeft ) {
    mr = pMethod->pFunction ( hwnd, msg, mp1, mp2 ) ;
  } else {
    if ( DefaultProcessor )
      mr = DefaultProcessor ( hwnd, msg, mp1, mp2 ) ;
    else
      mr = 0 ;
  }

 /***************************************************************************
  * Return result from message processor.                                   *
  ***************************************************************************/

  return ( mr ) ;
}
 
/****************************************************************************
 *                                                                          *
 *                         Add Item to System Menu                          *
 *                                                                          *
 ****************************************************************************/

extern VOID AddSysMenuItem ( HWND hwndFrame, MENUITEM *Item, PSZ Text ) {

 /***************************************************************************
  * Obtain the system menu window handle.                                   *
  ***************************************************************************/

  HWND hwndSysMenu = WinWindowFromID ( hwndFrame, FID_SYSMENU ) ;

 /***************************************************************************
  * Get the system menu's base item and its window handle.                  *
  ***************************************************************************/

  USHORT idSysMenu = SHORT1FROMMR ( Sys_SendMessage ( hwndSysMenu, MM_ITEMIDFROMPOSITION, NULL, NULL ) ) ;

  MENUITEM miSysMenu ;
  Sys_SendMessage ( hwndSysMenu, MM_QUERYITEM,
    MPFROM2SHORT(idSysMenu,FALSE), MPFROMP(&miSysMenu) ) ;

  HWND hwndSysSubMenu = miSysMenu.hwndSubMenu ;

 /***************************************************************************
  * Add the new item to the system menu's submenu, which is what we see.    *
  ***************************************************************************/

  Sys_SendMessage ( hwndSysSubMenu, MM_INSERTITEM, MPFROMP(Item), MPFROMP(Text) ) ;
}
 
/****************************************************************************
 *                                                                          *
 *                   Add Item to Submenu on System Menu                     *
 *                                                                          *
 ****************************************************************************/

extern VOID AddSysSubMenuItem
(
  HWND hwndFrame,
  USHORT SubMenuID,
  MENUITEM *Item,
  PSZ Text
)
{
 /***************************************************************************
  * Obtain the system menu window handle.                                   *
  ***************************************************************************/

  HWND hwndSysMenu = WinWindowFromID ( hwndFrame, FID_SYSMENU ) ;

 /***************************************************************************
  * Get the system menu's base item and its window handle.                  *
  ***************************************************************************/

  USHORT idSysMenu = SHORT1FROMMR ( Sys_SendMessage ( hwndSysMenu, MM_ITEMIDFROMPOSITION, NULL, NULL ) ) ;

  MENUITEM MenuItem ;
  Sys_SendMessage ( hwndSysMenu, MM_QUERYITEM,
    MPFROM2SHORT(idSysMenu,FALSE), MPFROMP(&MenuItem) ) ;

  HWND hwndSysSubMenu = MenuItem.hwndSubMenu ;

 /***************************************************************************
  * Get the submenu's base item and its window handle.                      *
  ***************************************************************************/

  Sys_SendMessage ( hwndSysSubMenu, MM_QUERYITEM,
    MPFROM2SHORT ( SubMenuID, TRUE ),
    (MPARAM) &MenuItem ) ;

  HWND hwndSubMenu = MenuItem.hwndSubMenu ;

 /***************************************************************************
  * Add the new item to the system menu's submenu, which is what we see.    *
  ***************************************************************************/

  Sys_SendMessage ( hwndSubMenu, MM_INSERTITEM, MPFROMP(Item), MPFROMP(Text) ) ;
}
 
/****************************************************************************
 *                                                                          *
 *                           Add Item to Menu                               *
 *                                                                          *
 ****************************************************************************/

extern VOID AddMenuItem
(
  HWND hwndFrame,
  USHORT MenuID,
  MENUITEM *Item,
  PSZ Text
)
{
 /***************************************************************************
  * Obtain the menu window handle.                                          *
  ***************************************************************************/

  HWND hwndMenu = WinWindowFromID ( hwndFrame, MenuID ) ;

 /***************************************************************************
  * Add the new item to the menu.                                           *
  ***************************************************************************/

  Sys_SendMessage ( hwndMenu, MM_INSERTITEM, MPFROMP(Item), MPFROMP(Text) ) ;
}
 
/****************************************************************************
 *                                                                          *
 *                        Add Item to SubMenu                               *
 *                                                                          *
 ****************************************************************************/

extern VOID AddSubMenuItem
(
  HWND hwndFrame,
  USHORT MenuID,
  USHORT SubMenuID,
  MENUITEM *Item,
  PSZ Text
)
{
 /***************************************************************************
  * Obtain the menu window handle.                                          *
  ***************************************************************************/

  HWND hwndMenu = WinWindowFromID ( hwndFrame, MenuID ) ;

 /***************************************************************************
  * Obtain the submenu window handle.                                       *
  ***************************************************************************/

  MENUITEM MenuItem ;
  Sys_SendMessage ( hwndMenu, MM_QUERYITEM,
    MPFROM2SHORT ( SubMenuID, TRUE ),
    (MPARAM) &MenuItem ) ;

  HWND hwndSubMenu = MenuItem.hwndSubMenu ;

 /***************************************************************************
  * Add the new item to the menu.                                           *
  ***************************************************************************/

  Sys_SendMessage ( hwndSubMenu, MM_INSERTITEM, MPFROMP(Item), MPFROMP(Text) ) ;
}
 
/****************************************************************************
 *                                                                          *
 *                       Remove Item from SubMenu                           *
 *                                                                          *
 ****************************************************************************/

extern VOID RemoveSubMenuItem
(
  HWND hwndFrame,
  USHORT MenuID,
  USHORT SubMenuID,
  USHORT ItemID
)
{
 /***************************************************************************
  * Obtain the menu window handle.                                          *
  ***************************************************************************/

  HWND hwndMenu = WinWindowFromID ( hwndFrame, MenuID ) ;

 /***************************************************************************
  * Obtain the submenu window handle.                                       *
  ***************************************************************************/

  MENUITEM MenuItem ;
  Sys_SendMessage ( hwndMenu, MM_QUERYITEM,
    MPFROM2SHORT ( SubMenuID, TRUE ),
    (MPARAM) &MenuItem ) ;

  HWND hwndSubMenu = MenuItem.hwndSubMenu ;

 /***************************************************************************
  * Remove the item from the menu.                                          *
  ***************************************************************************/

  Sys_SendMessage ( hwndSubMenu, MM_REMOVEITEM, MPFROM2SHORT(ItemID,TRUE), 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Enable/Disable menu item.                                           *
 *                                                                          *
 ****************************************************************************/

extern VOID EnableMenuItem ( HWND Frame, USHORT MenuID, USHORT ItemID, BOOL Enable ) {
   EnableMenuItem ( WinWindowFromID(Frame,MenuID), ItemID, Enable ) ;
}

extern VOID EnableMenuItem ( HWND Menu, USHORT ItemID, BOOL Enable ) {
   Sys_SendMessage ( Menu, MM_SETITEMATTR, MPFROM2SHORT(ItemID,TRUE), MPFROM2SHORT(MIA_DISABLED,Enable?0:MIA_DISABLED) ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Check/Uncheck menu item.                                            *
 *                                                                          *
 ****************************************************************************/

extern VOID CheckMenuItem ( HWND Frame, USHORT MenuID, USHORT ItemID, BOOL Check ) {
   CheckMenuItem ( WinWindowFromID(Frame,MenuID), ItemID, Check ) ;
}

extern VOID CheckMenuItem ( HWND Menu, USHORT ItemID, BOOL Check ) {
   Sys_SendMessage ( Menu, MM_SETITEMATTR, MPFROM2SHORT(ItemID,TRUE), MPFROM2SHORT(MIA_CHECKED,Check?MIA_CHECKED:0) ) ;
}
 
/****************************************************************************
 *                                                                          *
 *                        Add Program to Task List                          *
 *                                                                          *
 ****************************************************************************/

extern VOID Add2TaskList ( HWND hwnd, PSZ Name ) {

 /***************************************************************************
  * Get the window's process ID.                                            *
  ***************************************************************************/

  PID pid ;
  WinQueryWindowProcess ( hwnd, &pid, PTID(NULL) ) ;

 /***************************************************************************
  * Add an entry to the system task list.                                   *
  ***************************************************************************/

  SWCNTRL swctl ;
  swctl.hwnd = hwnd ;
  swctl.hwndIcon = 0 ;
  swctl.hprog = 0 ;
  swctl.idProcess = pid ;
  swctl.idSession = 0 ;
  swctl.uchVisibility = SWL_VISIBLE ;
  swctl.fbJump = SWL_JUMPABLE ;
  strcpy ( swctl.szSwtitle, (PCHAR)Name ) ;

  WinAddSwitchEntry ( &swctl ) ;
}
 
/****************************************************************************
 *                                                                          *
 *                    Update Program's TaskList Entry                       *
 *                                                                          *
 ****************************************************************************/

extern VOID UpdateTaskListEntry ( HWND Window, char *Title ) {

   HSWITCH Handle = WinQuerySwitchHandle ( Window, 0 ) ;

   if ( Handle ) {

      SWCNTRL Entry ;
      if ( !WinQuerySwitchEntry ( Handle, &Entry ) ) {

         strcpy ( Entry.szSwtitle, Title ) ;

         WinChangeSwitchEntry ( Handle, &Entry ) ;

      } /* endif */

   } /* endif */
}

/****************************************************************************
 *                                                                          *
 *  Build Presentation Parameters                                           *
 *                                                                          *
 ****************************************************************************/

extern PPRESPARAMS BuildPresParams
(
  USHORT ParmCount,
  PULONG Ids,
  PULONG ByteCounts,
  PUCHAR *Parms
)
{
 /***************************************************************************
  * Determine final size of presentation parameter block.                   *
  ***************************************************************************/

  ULONG Size = sizeof(ULONG) ;

  for ( int i=0; i<ParmCount; i++ ) {
    Size += sizeof(ULONG) ;
    Size += sizeof(ULONG) ;
    Size += ByteCounts[i] ;
  }

 /***************************************************************************
  * Allocate memory for block.  Return if unable to do so.                  *
  ***************************************************************************/

  PPRESPARAMS PresParams = (PPRESPARAMS) new char [ Size ] ;

  if ( PresParams == NULL )
    return ( PresParams ) ;

 /***************************************************************************
  * Initialize the block header.                                            *
  ***************************************************************************/

  PresParams->cb = Size - sizeof(PresParams->cb) ;

 /***************************************************************************
  * Load the presentation parameters into the block.                        *
  ***************************************************************************/

  PPARAM Param = PresParams->aparam ;

  for ( i=0; i<ParmCount; i++ ) {
    Param->id = Ids[i] ;
    Param->cb = ByteCounts[i] ;
    memcpy ( Param->ab, Parms[i], (USHORT)ByteCounts[i] ) ;
    PUCHAR p = PUCHAR ( Param ) ;
    p += sizeof(Param->id) ;
    p += sizeof(Param->cb) ;
    p += ByteCounts[i] ;
    Param = PPARAM ( p ) ;
  }

 /***************************************************************************
  * Return the pointer to the block.  It will need freeing by the caller.   *
  ***************************************************************************/

  return ( PresParams ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process Exit menu command.                                          *
 *                                                                          *
 ****************************************************************************/

extern MRESULT APIENTRY Exit ( HWND hwnd, MESG, MPARAM1, MPARAM2 ) {

  /**************************************************************************
   * Post a WM_CLOSE message to the window.                                 *
   **************************************************************************/

   Sys_PostMessage ( hwnd, WM_CLOSE, 0, 0 ) ;

  /**************************************************************************
   * Done.                                                                  *
   **************************************************************************/

   return ( MRFROMSHORT ( 0 ) ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process Help For Help menu command.                                 *
 *                                                                          *
 ****************************************************************************/

extern MRESULT APIENTRY HelpForHelp ( HWND hwnd, MESG, MPARAM1, MPARAM2 ) {

  /**************************************************************************
   * Get the help instance window handle, if any.                           *
   **************************************************************************/

   HWND HelpWindow = WinQueryHelpInstance ( hwnd ) ;

  /**************************************************************************
   * If help is available, pass the request on to the help window.          *
   **************************************************************************/

   if ( HelpWindow )
      Sys_SendMessage ( HelpWindow, HM_DISPLAY_HELP, 0, 0 ) ;

  /**************************************************************************
   * Done.                                                                  *
   **************************************************************************/

   return ( MRFROMSHORT ( 0 ) ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process Extended Help menu command.                                 *
 *                                                                          *
 ****************************************************************************/

extern MRESULT APIENTRY ExtendedHelp ( HWND hwnd, MESG, MPARAM1, MPARAM2 ) {

  /**************************************************************************
   * Get the help instance window handle, if any.                           *
   **************************************************************************/

   HWND HelpWindow = WinQueryHelpInstance ( hwnd ) ;

  /**************************************************************************
   * If help is available, pass the request on to the help window.          *
   **************************************************************************/

   if ( HelpWindow )
      Sys_SendMessage ( HelpWindow, HM_EXT_HELP, 0, 0 ) ;

  /**************************************************************************
   * Done.                                                                  *
   **************************************************************************/

   return ( MRFROMSHORT ( 0 ) ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process Keys Help menu command.                                     *
 *                                                                          *
 ****************************************************************************/

extern MRESULT APIENTRY KeysHelp ( HWND hwnd, MESG, MPARAM1, MPARAM2 ) {

  /**************************************************************************
   * Get the help instance window handle, if any.                           *
   **************************************************************************/

   HWND HelpWindow = WinQueryHelpInstance ( hwnd ) ;

  /**************************************************************************
   * If help is available, pass the request on to the help window.          *
   **************************************************************************/

   if ( HelpWindow )
      Sys_SendMessage ( HelpWindow, HM_KEYS_HELP, 0, 0 ) ;

  /**************************************************************************
   * Done.                                                                  *
   **************************************************************************/

   return ( MRFROMSHORT ( 0 ) ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Process Help Index menu command.                                    *
 *                                                                          *
 ****************************************************************************/

extern MRESULT APIENTRY HelpIndex ( HWND hwnd, MESG, MPARAM1, MPARAM2 ) {

  /**************************************************************************
   * Get the help instance window handle, if any.                           *
   **************************************************************************/

   HWND HelpWindow = WinQueryHelpInstance ( hwnd ) ;

  /**************************************************************************
   * If help is available, pass the request on to the help window.          *
   **************************************************************************/

   if ( HelpWindow )
      Sys_SendMessage ( HelpWindow, HM_HELP_INDEX, 0, 0 ) ;

  /**************************************************************************
   * Done.                                                                  *
   **************************************************************************/

   return ( MRFROMSHORT ( 0 ) ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Query for Drive Type, File System and Label.                        *
 *                                                                          *
 *      Returns 0 for removable local, +1 for fixed local, -1 for remote.   *
 *                                                                          *
 ****************************************************************************/

extern int CheckDrive ( int Drive, int &Type ) {
   return ( CheckDrive ( Drive, Type, 0, 0 ) ) ;
}

extern int CheckDrive ( int Drive, int &Type, char *FileSystem, char *DiskLabel ) {

  /**************************************************************************
   * First, check to see if drive is local or remote.                       *
   **************************************************************************/

   UCHAR Path [3] ;
   Path[0] = (UCHAR) ( Drive + 'A' - 1 ) ;
   Path[1] = ':' ;
   Path[2] = 0 ;

   DosError ( FERR_DISABLEHARDERR ) ;

   UCHAR Buffer [1024] ;
   ULONG Size = sizeof(Buffer) ;
   APIRET Status = DosQueryFSAttach ( PSZ(Path), 0, FSAIL_QUERYNAME, (PFSQBUFFER2)Buffer, &Size ) ;
   DosError ( FERR_ENABLEHARDERR ) ;

   if ( Status )
      return ( int ( Status ) ) ;

  /**************************************************************************
   * Get the file system name.                                              *
   **************************************************************************/

   if ( FileSystem ) {
      USHORT cbName = PFSQBUFFER2(Buffer)->cbName ;
      strcpy ( FileSystem, PCHAR(PFSQBUFFER2(Buffer+cbName)->szFSDName) ) ;
   } /* endif */

  /**************************************************************************
   * Get the drive label.                                                   *
   **************************************************************************/

   if ( DiskLabel ) {
      FSINFO Info ;
      if ( DosQueryFSInfo ( Drive, FSIL_VOLSER, PUCHAR(&Info), sizeof(Info) ) == 0 )
         strcpy ( PCHAR(DiskLabel), PCHAR(Info.vol.szVolLabel) ) ;
   } /* endif */

  /**************************************************************************
   * If remote drive, return now.                                           *
   **************************************************************************/

   if ( PFSQBUFFER2(Buffer)->iType == FSAT_REMOTEDRV ) {
      Type = -1 ;
      return ( 0 ) ;
   } /* endif */

  /**************************************************************************
   * Attempt to open the local drive as an entire device.  If unable to do  *
   *   so, return an error.                                                 *
   **************************************************************************/

   ULONG Action ;
   HFILE Handle ;
   Status = DosOpen ( PSZ(Path), &Handle, &Action, 0, 0, FILE_OPEN,
      OPEN_ACCESS_READONLY | OPEN_SHARE_DENYNONE |
      OPEN_FLAGS_DASD | OPEN_FLAGS_FAIL_ON_ERROR, 0 ) ;

   if ( Status )
      return ( int ( Status ) ) ;

  /**************************************************************************
   * Check to see if the drive has removable media.  We cannot monitor such.*
   **************************************************************************/

   UCHAR Command = 0 ;
   UCHAR NonRemovable ;
   ULONG LengthIn = sizeof(Command) ;
   ULONG LengthOut = sizeof(NonRemovable);
   DosDevIOCtl ( Handle, 8, 0x20, &Command, sizeof(Command), &LengthIn, &NonRemovable, sizeof(NonRemovable), &LengthOut ) ;
   Type = NonRemovable ;

  /**************************************************************************
   * Close the drive.                                                       *
   **************************************************************************/

   DosClose ( Handle ) ;

  /**************************************************************************
   * Return no error.                                                       *
   **************************************************************************/

   return ( 0 ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Get Country Information                                             *
 *                                                                          *
 ****************************************************************************/

extern COUNTRYINFO CountryInfo = { 0 } ;

extern void GetCountryInfo ( int Country, int CodePage ) {

   // Get the default country information.
   COUNTRYCODE CountryCode ;
   CountryCode.country = Country ;
   CountryCode.codepage = CodePage ;
   ULONG Count ;
   APIRET Status = DosQueryCtryInfo ( sizeof(CountryInfo), &CountryCode, &CountryInfo, &Count ) ;
   if ( Status ) {
      CountryInfo.fsDateFmt = DATEFMT_MM_DD_YY ;        // iDate
      CountryInfo.szCurrency[0] = '$' ;                 // sCurrency
      CountryInfo.szCurrency[1] = 0 ;
      CountryInfo.szThousandsSeparator[0] = ',' ;       // sThousand
      CountryInfo.szThousandsSeparator[1] = 0 ;
      CountryInfo.szDecimal[0] = '.' ;                  // sDecimal
      CountryInfo.szDecimal[1] = 0 ;
      CountryInfo.szDateSeparator[0] = '/' ;            // sDate
      CountryInfo.szDateSeparator[1] = 0 ;
      CountryInfo.szTimeSeparator[0] = ':' ;            // sTime
      CountryInfo.szTimeSeparator[1] = 0 ;
      CountryInfo.fsCurrencyFmt = 0 ;                   // iCurrency
      CountryInfo.cDecimalPlace = 2 ;                   // iDigits
      CountryInfo.fsTimeFmt = 0 ;                       // iTime
      CountryInfo.szDataSeparator[0] = ',' ;            // sList
      CountryInfo.szDataSeparator[1] = 0 ;
   } /* endif */

   // Get the customized values.

   char Text [10] ;
   ULONG Size = 10 ;
   if ( PrfQueryProfileData ( HINI_USERPROFILE, PSZ("PM_National"), PSZ("iCurrency"), Text, &Size ) )
      CountryInfo.fsCurrencyFmt = UCHAR ( atoi ( Text ) ) ;

   Size = 10 ;
   if ( PrfQueryProfileData ( HINI_USERPROFILE, PSZ("PM_National"), PSZ("iDate"), Text, &Size ) )
      CountryInfo.fsDateFmt = atoi ( Text ) ;

   Size = 10 ;
   if ( PrfQueryProfileData ( HINI_USERPROFILE, PSZ("PM_National"), PSZ("iDigits"), Text, &Size ) )
      CountryInfo.cDecimalPlace = UCHAR ( atoi ( Text ) ) ;

   Size = 10 ;
   if ( PrfQueryProfileData ( HINI_USERPROFILE, PSZ("PM_National"), PSZ("iTime"), Text, &Size ) )
      CountryInfo.fsTimeFmt = UCHAR ( atoi ( Text ) ) ;

   Size = 5 ;
   PrfQueryProfileData ( HINI_USERPROFILE, PSZ("PM_National"), PSZ("sCurrency"), CountryInfo.szCurrency, &Size ) ;

   Size = 2 ;
   PrfQueryProfileData ( HINI_USERPROFILE, PSZ("PM_National"), PSZ("sList"), CountryInfo.szDataSeparator, &Size ) ;

   Size = 2 ;
   PrfQueryProfileData ( HINI_USERPROFILE, PSZ("PM_National"), PSZ("sDate"), CountryInfo.szDateSeparator, &Size ) ;

   Size = 2 ;
   PrfQueryProfileData ( HINI_USERPROFILE, PSZ("PM_National"), PSZ("sDecimal"), CountryInfo.szDecimal, &Size ) ;

   Size = 2 ;
   PrfQueryProfileData ( HINI_USERPROFILE, PSZ("PM_National"), PSZ("sThousand"), CountryInfo.szThousandsSeparator, &Size ) ;

   Size = 2 ;
   PrfQueryProfileData ( HINI_USERPROFILE, PSZ("PM_National"), PSZ("sTime"), CountryInfo.szTimeSeparator, &Size ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Format decimal number in local manner.                              *
 *                                                                          *
 ****************************************************************************/

extern char *FormatDecimal ( char *Text, long Number, int Divisor, int Decimal ) {

   // Format the decimal number as requested.
   sprintf ( Text, "%.*lf", Decimal, double(Number)/Divisor ) ;

   // Replace the decimal character with whatever's appropriate for the locale.
   Text[strlen(Text)-Decimal-1] = CountryInfo.szDecimal[0] ;

   // Return the text buffer address.
   return ( Text ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Decode decimal number in local manner.                              *
 *                                                                          *
 ****************************************************************************/

extern BOOL DecodeDecimal ( char *Text, double &Value ) {

   long Divisor = 1 ;
   BOOL DecimalFound = FALSE ;
   BOOL DigitsFound = FALSE ;
   BOOL SignFound = FALSE ;
   int Sign = + 1 ;

   Value = 0.0 ;

   char *p = Text ;
   while ( *p ) {

      if ( ( *p >= '0' ) AND ( *p <= '9' ) ) {
         if ( DecimalFound ) 
            Divisor *= 10 ;
         else
            Value *= 10 ;
         Value += double ( *p - '0' ) / Divisor ;
         DigitsFound = TRUE ;

      } else if ( *p == CountryInfo.szDecimal[0] ) {
         if ( DecimalFound ) {
            Value *= Sign ;
            return ( DigitsFound ) ;
         } /* endif */
         DecimalFound = TRUE ; 
         DigitsFound = TRUE ;

      } else if ( *p == '+' ) {
         if ( SignFound ) {
            Value *= Sign ;
            return ( DigitsFound ) ;
         } /* endif */
         SignFound = TRUE ;
         Sign = + 1 ;
         if ( DigitsFound ) {
            Value *= Sign ;
            return ( DigitsFound ) ;
         } /* endif */

      } else if ( *p == '-' ) {
         if ( SignFound ) {
            Value *= Sign ;
            return ( DigitsFound ) ;
         } /* endif */
         SignFound = TRUE ;
         Sign = - 1 ;
         if ( DigitsFound ) {
            Value *= Sign ;
            return ( DigitsFound ) ;
         } /* endif */

      } else if ( ( *p == ' ' ) OR ( *p == '\t' ) ) {
         if ( DigitsFound OR SignFound ) {
            Value *= Sign ;
            return ( DigitsFound ) ;
         } /* endif */

      } else {
         Value *= Sign ;
         return ( DigitsFound ) ;

      } /* endif */

      p ++ ;

   } /* endwhile */

   Value *= Sign ;
   return ( DigitsFound ) ;
}
 
/****************************************************************************
 *                                                                          *
 *      Get Title-bar Rectangle (courtesy of Shinji Takasugi)               *
 *                                                                          *
 ****************************************************************************/

extern BOOL GetTitlebarRect ( HAB Anchor, HWND Window, RECTL &Rectangle ) {

   if ( WinQueryWindowRect ( Window, &Rectangle ) ) {

      HENUM henum = WinBeginEnumWindows ( Window ) ;
      HWND hwndEnum = WinGetNextWindow ( henum ) ;  
      while ( hwndEnum ) {

         RECTL rectEnum ;
         WinQueryWindowRect ( hwndEnum, &rectEnum ) ;

         RECTL rectTemp ;
         WinSubtractRect ( Anchor, &rectTemp, &Rectangle, &rectEnum ) ;

         Rectangle = rectTemp ;
         hwndEnum = WinGetNextWindow ( henum ) ; 

      } /* endwhile */

      WinEndEnumWindows ( henum ) ;

      return ( TRUE ) ;

   } /* endif */

   return ( FALSE ) ;
}

/****************************************************************************
 *                                                                          *
 *      WinMessageBox Replacement                                           *
 *                                                                          *
 ****************************************************************************/

extern ULONG MessageBox ( HWND Parent, HWND Owner, char *Text, char *Title, int ID,
   int Style, HMODULE Library, int BaseStringID ) {

   // Reset MB_HELP, because it causes problems with WinMessageBox2.
   Style &= ~MB_HELP ;  

#if 0

   ULONG Result = WinMessageBox ( Parent, Owner, PSZ(Text), PSZ(Title), ID, Style ) ;

#else

   ResourceString Yes    ( Library, BaseStringID+MBID_YES,    __FILE__, __LINE__ ) ;
   ResourceString No     ( Library, BaseStringID+MBID_NO,     __FILE__, __LINE__ ) ;
   ResourceString Ok     ( Library, BaseStringID+MBID_OK,     __FILE__, __LINE__ ) ;
   ResourceString Enter  ( Library, BaseStringID+MBID_ENTER,  __FILE__, __LINE__ ) ;
   ResourceString Cancel ( Library, BaseStringID+MBID_CANCEL, __FILE__, __LINE__ ) ;
   ResourceString Abort  ( Library, BaseStringID+MBID_ABORT,  __FILE__, __LINE__ ) ;
   ResourceString Retry  ( Library, BaseStringID+MBID_RETRY,  __FILE__, __LINE__ ) ;
   ResourceString Ignore ( Library, BaseStringID+MBID_IGNORE, __FILE__, __LINE__ ) ;
   ResourceString Help   ( Library, BaseStringID+MBID_HELP,   __FILE__, __LINE__ ) ;

   int Count ;
   switch ( Style & 0x0F ) {
      case MB_OK:
      case MB_ENTER:
      case MB_CANCEL:
         Count = 1 ;
         break;
      case MB_OKCANCEL:
      case MB_RETRYCANCEL:
      case MB_YESNO:
      case MB_ENTERCANCEL:
         Count = 2 ;
         break;
      case MB_ABORTRETRYIGNORE:
      case MB_YESNOCANCEL:
         Count = 3 ;
         break;
   } /* endswitch */

   if ( Style & MB_HELP ) 
      Count ++ ;

   int InfoSize = sizeof(MB2INFO) + sizeof(MB2D)*(Count-1) ;
   MB2INFO *Info = (MB2INFO*) new char [ InfoSize ] ;
   memset ( Info, 0, InfoSize ) ;
   Info->cb = InfoSize ;
   Info->hIcon = 0 ;
   Info->cButtons = Count ;
   Info->flStyle = Style ;     
   Info->hwndNotify = 0 ;

   for ( int i=0; i<Count; i++ ) 
      Info->mb2d[i].flStyle = LONG ( WS_VISIBLE | WS_PARENTCLIP | WS_SYNCPAINT | BS_PUSHBUTTON | BS_AUTOSIZE ) ;

   switch ( Style & 0x000F ) {
      case MB_OK:
         strcpy ( Info->mb2d[0].achText, PCHAR(Ok) ) ; Info->mb2d[0].idButton = MBID_OK ;
         break;
      case MB_ENTER:
         strcpy ( Info->mb2d[0].achText, PCHAR(Enter) ) ; Info->mb2d[0].idButton = MBID_ENTER ;
         break;
      case MB_CANCEL:
         strcpy ( Info->mb2d[0].achText, PCHAR(Cancel) ) ; Info->mb2d[0].idButton = MBID_CANCEL ;
         break;
      case MB_OKCANCEL:
         strcpy ( Info->mb2d[0].achText, PCHAR(Ok) ) ; Info->mb2d[0].idButton = MBID_OK ;
         strcpy ( Info->mb2d[1].achText, PCHAR(Cancel) ) ; Info->mb2d[1].idButton = MBID_CANCEL ;
         break;
      case MB_RETRYCANCEL:
         strcpy ( Info->mb2d[0].achText, PCHAR(Retry) ) ; Info->mb2d[0].idButton = MBID_RETRY ;
         strcpy ( Info->mb2d[1].achText, PCHAR(Cancel) ) ; Info->mb2d[1].idButton = MBID_CANCEL ;
         break;
      case MB_YESNO:
         strcpy ( Info->mb2d[0].achText, PCHAR(Yes) ) ; Info->mb2d[0].idButton = MBID_YES ;
         strcpy ( Info->mb2d[1].achText, PCHAR(No) ) ; Info->mb2d[1].idButton = MBID_NO ;
         break;
      case MB_ENTERCANCEL:
         strcpy ( Info->mb2d[0].achText, PCHAR(Enter) ) ; Info->mb2d[0].idButton = MBID_ENTER ;
         strcpy ( Info->mb2d[1].achText, PCHAR(Cancel) ) ; Info->mb2d[1].idButton = MBID_CANCEL ;
         break;
      case MB_ABORTRETRYIGNORE:
         strcpy ( Info->mb2d[0].achText, PCHAR(Abort) ) ; Info->mb2d[0].idButton = MBID_ABORT ;
         strcpy ( Info->mb2d[1].achText, PCHAR(Retry) ) ; Info->mb2d[1].idButton = MBID_RETRY ;
         strcpy ( Info->mb2d[2].achText, PCHAR(Ignore) ) ; Info->mb2d[2].idButton = MBID_IGNORE ;
         break;
      case MB_YESNOCANCEL:
         strcpy ( Info->mb2d[0].achText, PCHAR(Yes) ) ; Info->mb2d[0].idButton = MBID_YES ;
         strcpy ( Info->mb2d[1].achText, PCHAR(No) ) ; Info->mb2d[1].idButton = MBID_NO ;
         strcpy ( Info->mb2d[2].achText, PCHAR(Cancel) ) ; Info->mb2d[2].idButton = MBID_CANCEL ;
         break;
   } /* endswitch */

   Info->mb2d[0].flStyle |= WS_GROUP ;

   if ( Style & MB_HELP ) {
      strcpy ( Info->mb2d[Count-1].achText, PCHAR(Help) ) ; 
      Info->mb2d[Count-1].idButton = MBID_HELP ;
      Info->mb2d[Count-1].flStyle |= BS_HELP | BS_NOPOINTERFOCUS ;
   } /* endif */

   switch ( Style & 0x0F00 ) {
      case MB_DEFBUTTON1: if ( Count >= 1 ) Info->mb2d[0].flStyle |= BS_DEFAULT ; break ;
      case MB_DEFBUTTON2: if ( Count >= 2 ) Info->mb2d[1].flStyle |= BS_DEFAULT ; break ;
      case MB_DEFBUTTON3: if ( Count >= 3 ) Info->mb2d[2].flStyle |= BS_DEFAULT ; break ;
   } /* endswitch */

   ULONG Result = WinMessageBox2 ( Parent, Owner, PSZ(Text), PSZ(Title), ID, Info ) ;

   delete Info ;

#endif 

   return ( Result ) ;

}

