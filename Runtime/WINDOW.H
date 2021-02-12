// Class WINDOW: Encapsulates the startup/shutdown logic for a OS/2-PM window.

#ifndef WINDOW_H
#define WINDOW_H

#include "System.h"

#include "Mutex.h"
#include "Process.h"

#ifdef __NT__
   #define MIT_END     (-1)
   #define MIS_TEXT    MF_STRING
   #define MIS_BITMAP  MF_BITMAP
   #define DID_OK      1
   #define DID_CANCEL  2
#endif

enum PositionClass {
   MINIMIZE,
   RESTORE,
   MAXIMIZE
} ;

enum AnchorCorner {
   ANCHOR_BL,
   ANCHOR_TL,
   ANCHOR_BR,
   ANCHOR_TR
} ;

class WindowClass {

   private:
      char *Name ;
      int Registered ;

   public:
      WindowClass ( Process &Proc, int Style ) ;
      WindowClass ( char *name ) ;

      char *QueryName ( ) { return ( Name ) ; }
      int IsRegistered ( ) { return ( Registered ) ; }
} ;

class Window {

   private:

      HWND Handle ;
      int Created ;
      Mutex Lock ;
      BOOL ControlsHidden ;
      long FrameFlags ;
      int NumberOfHiddenControls ;
      HWND HiddenControls [10] ;

   protected:
      BOOL DebugMessages ;
      int MessageIndent ;
      ULONG BackColor ;  BOOL fBackColor ;
      ULONG TextColor ;  BOOL fTextColor ;
      char  FontNameSize [80] ; BOOL fFontNameSize ;
      USHORT AnchorPoint ; BOOL fAnchored ;

   public:

      Window ( HWND Parent, char *ClassName, char *Title, ULONG Style,
         int x, int y, int cx, int cy, HWND Owner, HWND Zorder,
         ULONG Id=0, void *Parms=0, void *PresParams=0 ) ;
      Window ( WindowClass &WndClass, HWND Parent, char *Title, int Style,
         int x, int y, int cx, int cy, HWND Owner, HWND Zorder, 
         int Id=0, void *Parms=0 ) ;
      Window ( HWND handle ) ;

      virtual ~Window ( ) ;

      ANCHOR QueryAnchor ( ) ;
      HWND QueryHandle ( ) { return ( Handle ) ; }
      BOOL QueryControlsHidden () { return ( ControlsHidden ) ; }

      void Set_DebugMessages ( BOOL Flag ) { DebugMessages = Flag ; MessageIndent = 0 ; }
      void Set_BackColor ( ULONG color ) ;
      void Set_TextColor ( ULONG color ) ;
      void Set_FontNameSize ( char *fontnamesize ) ;
      void Set_AnchorPoint ( AnchorCorner anchorpoint ) { AnchorPoint = anchorpoint ; }

      void Invalidate ( RECTL &Rectangle ) ;
      void Invalidate ( ) ;
      void Update ( ) ;

      void Destroy ( ) ;

      void Reposition ( int x, int y, int cx, int cy, PositionClass State ) ;
      void Show ( ) ;
      void Hide ( ) ;
      void Move ( int x, int y ) ;
      void Size ( int cx, int cy ) ;
      void Minimize ( ) ;
      void Maximize ( ) ;
      void Restore ( ) ;
      void Activate ( ) ;
      void Deactivate ( ) ;
      void Surface ( ) ;
      void Sink ( ) ;

      void SetFocus ( ) ;

      void Enable ( int Flag = TRUE ) ;
      void Disable ( ) ;

      void SetData ( void *data ) ;
      void *GetData ( ) ;

      void SetText ( char *text ) ;
      void GetText ( char *text, int textsize ) ;

      HWND Child ( int ID ) { return ( CHILD(Handle,ID) ) ; }
      HWND Parent ( ) { return ( PARENT(Handle) ) ; }

      void StartTimer ( int ID, int Milliseconds ) ;
      void StopTimer ( int ID ) ;

      RECTL Rectangle ( ) ;

      void SetWindowText ( char *title ) ;
      char *GetWindowText ( char String[], int StringSize ) ;

      void Add2TaskList ( ) ;

      BOOL AddSysMenuSeparator ( ) ;
      BOOL AddSysMenuItem ( char *Text, USHORT Id, SHORT Position=MIT_END, USHORT Style=MIS_TEXT,
         USHORT Attribute=0, HWND SubMenu=0, ULONG ItemHandle=0 ) ; 

      int MessageBox ( char *Title, char *Message, int Id, int Style ) ;
      int MessageBox ( char *Message, int Id, int Style ) ;

      void PopupMenu ( HWND Menu, int x, int y ) ;

      void HideControls ( BOOL fHide ) ;
      void Resize ( int cx, int cy ) ;
      void Drag ( ) ;

      void SetPresParam ( int ID, int Size, void *Data ) ;
      void RemovePresParam ( int ID ) ;

      MRESULT SendMessage ( MESG Message, MPARAM1 Parm1 = 0, MPARAM2 Parm2 = 0 ) ;
      int PostMessage ( MESG Message, MPARAM1 Parm1 = 0, MPARAM2 Parm2 = 0 ) ;

      virtual MRESULT MessageProcessor ( HWND hwnd, MESG msg, MPARAM1 mp1, MPARAM2 mp2 ) ;
} ;

class SubclassedWindow {

   private:
      WINDOWPROC *OldMessageProcessor ;

   protected:
      HWND Handle ;
      BOOL DebugMessages ;
      int MessageIndent ;

   public:
      SubclassedWindow ( HWND handle ) ;
      virtual ~SubclassedWindow ( ) ;

      void Set_DebugMessages ( BOOL Flag ) { DebugMessages = Flag ; MessageIndent = 0 ; }
      void Enable ( int Flag = TRUE ) { WinEnableWindow ( Handle, Flag ) ; }
      void Disable ( ) { WinEnableWindow ( Handle, FALSE ) ; }

      virtual MRESULT MessageProcessor ( HWND hwnd, MESG msg, MPARAM1 mp1, MPARAM2 mp2 ) ;
} ;

class Checkbox : public Window {
   public:
      Checkbox ( HWND control ) ;
      virtual ~Checkbox ( ) ;
      void Set ( int Flag = TRUE ) ;
      int Query ( ) ;
} ;

class Entryfield : public Window {
   public:
      Entryfield ( HWND control ) ;
      virtual ~Entryfield ( ) ;
      void SetLimit ( int Limit ) ;
      void QuerySelection ( int &First, int &Last ) ;
      void Insert ( char *Text ) ;
} ;

class StaticText : public Window {
   public:
      StaticText ( HWND control ) ;
      virtual ~StaticText ( ) ;
} ;

class SpinButton : public Window {
   public:
      SpinButton ( HWND control ) ;
      void SetLimits ( int LowerLimit, int UpperLimit ) ;
      void Set ( int Value ) ;
      int Query ( ) ;
} ;

class Slider : public Window {

   private:
      Entryfield *EntryField ;
      ULONG MinLevel, MaxLevel ;
      int Divisor ;
      char *Suffix ;

   public:
      Slider ( HWND control, HWND entryfield, ULONG Level, ULONG minlevel, ULONG maxlevel, int divisor, char *suffix ) ;
      Slider ( HWND control, HWND entryfield ) ;
      Slider ( HWND control, ULONG Level, ULONG minlevel, ULONG maxlevel, int divisor, char *suffix ) ;
      Slider ( HWND control ) ;
      virtual ~Slider ( ) ;
      void Set ( ULONG Level, BOOL SetText=TRUE ) ;
      ULONG Read ( ) ;
      void Enable ( int Flag ) ;
} ;

class ListBox : public SubclassedWindow { 

   public:
      ListBox ( HWND handle ) ;
      virtual ~ListBox ( ) ;
      virtual MRESULT MessageProcessor ( HWND hwnd, MESG msg, MPARAM1 mp1, MPARAM2 mp2 ) ;
      void Clear ( ) ;
      void Insert ( char *String, int Where=-1 ) ;
      void Delete ( int Where ) ;
      void SelectItem ( int Index ) ;
      int QueryCount ( ) ;
      int QuerySelection ( int Previous = LIT_FIRST ) ;
      void SetTopIndex ( int Index ) ;
      int QueryTopIndex ( ) ;
      void QueryItemText ( int Index, char *Text, int TextSize ) ;
} ;

#endif 
