/****************************************************************** SUPPORT.H
 *                                                                          *
 *                Presentation Manager Support Functions                    *
 *                                                                          *
 ****************************************************************************/

#ifndef SUPPORT_H
#define SUPPORT_H

#include "System.h"

#define NOT !
#define OR  ||
#define AND &&

#define DATEFMT_MM_DD_YY    (0x0000)
#define DATEFMT_DD_MM_YY    (0x0001)
#define DATEFMT_YY_MM_DD    (0x0002)

#define max(a,b)        (((a) > (b)) ? (a) : (b))
#define min(a,b)        (((a) < (b)) ? (a) : (b))

typedef struct Method {
   USHORT Action ;
   PWINDOWPROC pFunction ;
} METHOD, *PMETHOD ;

extern MRESULT DispatchMessage (
   HWND    hwnd,
   ULONG   msg,
   MPARAM  mp1,
   MPARAM  mp2,
   PMETHOD MethodTable,
   USHORT  MethodCount,
   PWINDOWPROC DefaultProcessor
) ;

extern VOID AddSysMenuItem ( HWND hwndFrame, MENUITEM *Item, PSZ Text ) ;

extern VOID AddSysSubMenuItem (
   HWND hwndFrame,
   USHORT SubMenuID,
   MENUITEM *Item,
   PSZ Text
) ;

extern VOID AddMenuItem (
   HWND hwndFrame,
   USHORT MenuID,
   MENUITEM *Item,
   PSZ Text
) ;

extern VOID AddSubMenuItem (
   HWND hwndFrame,
   USHORT MenuID,
   USHORT SubMenuID,
   MENUITEM *Item,
   PSZ Text
) ;

extern VOID RemoveSubMenuItem (
   HWND hwndFrame,
   USHORT MenuID,
   USHORT SubMenuID,
   USHORT ItemID
) ;

extern VOID EnableMenuItem ( HWND Frame, USHORT MenuID, USHORT Item, BOOL Enable ) ;
extern VOID EnableMenuItem ( HWND Menu, USHORT Item, BOOL Enable ) ;

extern VOID CheckMenuItem ( HWND Frame, USHORT MenuID, USHORT Item, BOOL Check ) ;
extern VOID CheckMenuItem ( HWND Menu, USHORT Item, BOOL Check ) ;

extern VOID Add2TaskList ( HWND hwnd, PSZ Name ) ;
extern VOID UpdateTaskListEntry ( HWND Window, char *Title ) ;

extern PPRESPARAMS BuildPresParams (
   USHORT ParmCount,
   PULONG Ids,
   PULONG ByteCounts,
   PUCHAR *Parms
) ;

extern WINDOWPROC Exit ;
extern WINDOWPROC HelpForHelp ;
extern WINDOWPROC ExtendedHelp ;
extern WINDOWPROC KeysHelp ;
extern WINDOWPROC HelpIndex ;

extern int CheckDrive ( int Drive, int &Type, char *FileSystem, char *DiskLabel ) ;
extern int CheckDrive ( int Drive, int &Type ) ;

extern COUNTRYINFO CountryInfo ;
extern void GetCountryInfo ( int Country, int CodePage ) ;
extern char *FormatDecimal ( char *Text, long Number, int Divisor, int Decimal ) ;
extern BOOL DecodeDecimal ( char *Text, double &Value ) ;

extern BOOL GetTitlebarRect ( HAB Anchor, HWND Window, RECTL &Rectangle ) ;

extern ULONG MessageBox ( HWND Parent, HWND Owner, char *Text, char *Title, int ID,
   int Style, HMODULE Library, int BaseStringID ) ;

#endif
