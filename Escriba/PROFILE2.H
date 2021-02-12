/***************************************************************** PROFILE2.H
 *                                                                          *
 *                      Profile Object definitions                          *
 *                                                                          *
 ****************************************************************************/

#ifndef PROFILE2_H
#define PROFILE2_H

#include "System.h"

#include "DocParms.h"
#include "Escriba.h"
#include "Mutex.h"
#include "Profile.h"
#include "ReString.h"

#define MAX_RECALL      10
#define MAX_SPACINGS    10
#define MAX_SIZES       50
#define MAX_EXPANSIONS  10
#define MAX_LINECOLORS  50
#define MAX_LINETYPES   50
#define MAX_LINEWIDTHS  50
#define MAX_FILLCOLORS  50
#define MAX_FILLTYPES   50
#define MAX_LINEENDS     3
#define MAX_LINEJOINS    3

typedef struct _IniData {
   BOOL fPosition ;
   SWP Position ;
   PSZ Files [MAX_RECALL] ;
   char Language [80] ;
   char FileType [80] ;
   char GraphicType [80] ;
   BOOL Animate ;
   BOOL Rulers ;
   BOOL ScrollBars ;
   BOOL StatusLine ;
   BOOL FullNames ;
   BOOL SpoolPages ;
   BOOL PrintReverse ;
   BOOL LoadMostRecent ;
   BOOL HideOnScreenClip ;
   long ZoomType ;
   COLOR MainColors [2] ;
   BOOL fMainColors [2] ;
   COLOR DocColors [2] ;
   BOOL fDocColors [2] ;
   COLOR RulerColors [2] ;
   BOOL fRulerColors [2] ;
   UCHAR RulerFont [80] ;
   BOOL fRulerFont ;
   DocumentParms DocParms ;
   USHORT SpacingCount ;
   USHORT Spacings [ MAX_SPACINGS ] ;
   USHORT SizeCount ;
   USHORT Sizes [ MAX_SIZES ] ;
   USHORT ExpandCount ;
   USHORT Expansions [ MAX_EXPANSIONS ] ;
   USHORT LineColorCount ;
   COLOR  LineColors [ MAX_LINECOLORS ] ;
   USHORT LineTypeCount ;
   ULONG  LineTypes [ MAX_LINETYPES ] ;
   USHORT LineWidthCount ;
   ULONG  LineWidths [ MAX_LINEWIDTHS ] ;
   USHORT FillColorCount ;
   COLOR  FillColors [ MAX_FILLCOLORS ] ;
   USHORT FillTypeCount ;
   ULONG  FillTypes [ MAX_FILLTYPES ] ;
   USHORT LineEndCount ;
   ULONG  LineEnds [ MAX_LINEENDS ] ;
   USHORT LineJoinCount ;
   ULONG  LineJoins [ MAX_LINEJOINS ] ;
   char Search_String [80] ;
   char Search_Replace [80] ;
   BOOL Search_CaseSensitive ;
   BOOL Search_Regular ;

   Mutex FileListLock ;

   _IniData ( HMODULE RegistrationLibrary = 0 ) ;
   ~_IniData ( ) ;
   void UpdateFileList ( char *Filename ) ;

} INIDATA, *PINIDATA ;

class Profile2 : public Profile {

   public:

      Profile2 ( char *Name, HAB Anchor, HMODULE Library, int DialogID, HelpWindow *Help, BOOL ResetFlag ) :
         Profile ( Name, Anchor, Library, DialogID, Help, ResetFlag ) { } 
      Profile2 ( char *Name, HAB Anchor, char *HomePath ) : Profile ( Name, Anchor, HomePath ) { } 

      BOOL GetIniData ( INIDATA &IniData ) ;
      void PutIniData ( INIDATA &IniData ) ;
} ;

#endif
