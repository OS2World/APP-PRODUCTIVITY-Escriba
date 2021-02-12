#define INCL_BASE
#include <os2.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "System.h"

#include "..\RunTime\EAttr.h"
#include "Escriba.h"
#define DECLARE_FUNCTION_NAMES
#include "External.h"

static BOOL APIENTRY Escriba_Verify ( char *Name ) ;
static BOOL APIENTRY Escriba_Export ( char *Name ) ;

extern void main ( ) {
   FILEFINDBUF3 Found ;
   ULONG Count ( 1 ) ;
   HDIR hDir ( HDIR_CREATE ) ;
   if ( !DosFindFirst ( "*.*", &hDir, FILE_NORMAL, &Found, sizeof(Found), &Count, FIL_STANDARD ) ) {
      do {
         if ( Escriba_Verify ( Found.achName ) ) 
            Escriba_Export ( Found.achName ) ;
      } while ( !DosFindNext ( hDir, &Found, sizeof(Found), &Count ) ); /* enddo */
      DosFindClose ( hDir ) ;
   } /* endif */
}

/****************************************************************************
 *                                                                          *
 *      Verify files of native Escriba/Pluma format.                        *
 *                                                                          *
 ****************************************************************************/

static BOOL APIENTRY Escriba_Verify ( char *Name ) {

   FILE *File = 0 ;
   int i = 0 ;
   size_t Size = 0 ;
   char *Text = 0 ;

   File = fopen ( Name, "rb" ) ;
   if ( File == 0 ) 
      return ( FALSE ) ;

   fseek ( File, 0, SEEK_END ) ;
   Size = (size_t) ftell ( File ) ;
   if ( Size == 0 ) {
      fclose ( File ) ;
      return ( FALSE ) ;
   } /* endif */

   fseek ( File, 0, SEEK_SET ) ;
   Text = new char [ Size ] ;
   if ( Text == 0 ) {
      fclose ( File ) ;
      return ( FALSE ) ;
   } /* endif */

   Size = fread ( Text, 1, Size, File ) ;
   if ( Size == 0 ) {
      delete [] Text ;
      fclose ( File ) ;
      return ( FALSE ) ;
   } /* endif */

   if ( ( Text[i] != FUNCTION_START ) || strnicmp ( Text+1, FunctionNames[FN__PREFACE], strlen(FunctionNames[FN__PREFACE]) ) ) {
      delete [] Text ;
      fclose ( File ) ;
      return ( FALSE ) ;
   } /* endif */

   int Nesting = 0 ;
   for ( i=0; i<Size; i++ ) {
      if ( ( Text[i] == FUNCTION_ESCAPE ) && ( i < Size-2 ) ) {
         if ( ( Text[i+1] == FUNCTION_ESCAPE ) || ( Text[i+1] == FUNCTION_START ) || ( Text[i+1] == FUNCTION_END ) ) {
            i ++ ;
         } else {
            delete [] Text ;
            fclose ( File ) ;
            return ( FALSE ) ;
         } /* endif */
      } else if ( Text[i] == FUNCTION_START ) {
         Nesting ++ ;
         for ( int Type=0; Type<MAX_FUNCTIONS; Type++ )
            if ( !strnicmp ( PCHAR(Text+i+1), FunctionNames[Type], strlen(FunctionNames[Type]) ) )
               break ;
         if ( Type >= MAX_FUNCTIONS ) {
            delete [] Text ;
            fclose ( File ) ;
            return ( FALSE ) ;
         } /* endif */
       } else if ( Text[i] == FUNCTION_END ) {
         Nesting -- ;
      } else if ( ( Text[i] == 0 ) && ( Nesting == 0 ) ) {
         delete [] Text ;
         fclose ( File ) ;
         return ( FALSE ) ;
      } /* endif */
   } /* endfor */

   delete [] Text ;
   fclose ( File ) ;

   if ( Nesting ) 
      return ( FALSE ) ;

   return ( TRUE ) ;
}

/****************************************************************************
 *                                                                          *
 *      Export files of native Escriba/Pluma format.                        *
 *                                                                          *
 ****************************************************************************/

static BOOL APIENTRY Escriba_Export ( char *Name ) {

   // Try to open the file.  Complain if not able to and return an error.
   FILE *File = fopen ( PCHAR(Name), "rb+" ) ;
   if ( File == NULL ) 
      return ( FALSE ) ;

   // Get Escriba's icon data.
   PVOID IconData ;
   DosGetResource ( 0, RT_POINTER, ID_MAIN, &IconData ) ;

   // Get the icon size.
   ULONG IconSize ;
   DosQueryResourceSize ( 0, RT_POINTER, ID_MAIN, &IconSize ) ;

   // Pull off the file's full name (name+ext) from the full path.
   char FullName [ CCHMAXPATH + 1 ] ;
   char Drive [_MAX_DRIVE+1] ;
   char Dir   [_MAX_DIR+1] ;
   char Fname [_MAX_FNAME+1] ;
   char Ext   [_MAX_EXT+1] ;
   _splitpath ( Name, Drive, Dir, Fname, Ext ) ;
   strcpy ( FullName, Fname ) ;
   strcat ( FullName, Ext ) ;

   // Build the file's extended attributes.
   static MVMT_VALUE Types [] = {
      { EAT_ASCII,  7, "Escriba"    },
   } ;

   static EADATA Table [] = {
      { PSZ(".TYPE"),     EAT_MVMT,  0, NULL     },     // Type length & value need filling in.
      { PSZ(".VERSION"),  EAT_ASCII, 3, REVISION },
      { PSZ(".LONGNAME"), EAT_ASCII, 0, NULL     },     // Name length & value need filling in.
      { PSZ(".ICON"),     EAT_ICON,  0, NULL     },     // Icon size & value need filling in.
   } ;

   Table[0].Length = USHORT ( BuildMVMTValue ( 0, sizeof(Types)/sizeof(Types[0]), Types ) ) ;
   Table[0].Value = new char [ Table[0].Length ] ;
   BuildMVMTValue ( Table[0].Value, sizeof(Types)/sizeof(Types[0]), Types ) ;

   Table[2].Length = USHORT ( strlen ( FullName ) ) ;
   Table[2].Value = FullName ;

   Table[3].Length = USHORT(IconSize) ;
   Table[3].Value = IconData ;

   PEAOP2 pExtendedAttributes = BuildExtendedAttributes ( sizeof(Table)/sizeof(Table[0]), Table ) ;

   // If the extended attributes got built successfully, then attach them to the file.
   if ( pExtendedAttributes ) {
      DosSetFileInfo ( fileno(File), 2, PUCHAR(pExtendedAttributes), sizeof(*pExtendedAttributes) ) ;
      delete pExtendedAttributes->fpFEA2List ;
      delete pExtendedAttributes ;
   } /* endif */

   // Free the memory allocated for the MVMT value.
   delete [] Table[0].Value ;

   // Release the icon data.
   DosFreeResource ( IconData ) ;

   // Close the file.
   fclose ( File ) ;

   // Return no error.
   return ( TRUE ) ;
}

/****************************************************************************
 *                                                                          *
 *      Build Extended Attribute Item                                       *
 *                                                                          *
 ****************************************************************************/

static ULONG BuildExtendedAttributeItem ( PFEA2 pFEA, PEADATA Item ) {

  /**************************************************************************
   * Store next entry's offset (set to zero at this point).                 *
   **************************************************************************/

   PUCHAR p = PUCHAR ( pFEA ) ;

   if ( pFEA )
      *(PULONG(p)) = 0 ;

   p += sizeof(ULONG) ;

  /**************************************************************************
   * Store flag byte.                                                       *
   **************************************************************************/

   if ( pFEA )
      *p = 0 ;

   p ++ ;

  /**************************************************************************
   * Store count of bytes in name.                                          *
   **************************************************************************/

   if ( pFEA )
      *p = UCHAR ( strlen ( PCHAR(Item->Name) ) ) ;

   p ++ ;

  /**************************************************************************
   * Store count of bytes in value.                                         *
   **************************************************************************/

   if ( pFEA )
      *(PUSHORT(p)) = USHORT ( Item->Length + 2 * sizeof(USHORT) ) ;

   p += sizeof(USHORT) ;

  /**************************************************************************
   * Store name.                                                            *
   **************************************************************************/

   if ( pFEA )
      strcpy ( PCHAR(p), PCHAR(Item->Name) ) ;

   p += strlen ( PCHAR(Item->Name) ) + 1 ;

  /**************************************************************************
   * Store value's type.                                                    *
   **************************************************************************/

   if ( pFEA )
      *(PUSHORT(p)) = Item->Type ;

   p += sizeof(USHORT) ;

  /**************************************************************************
   * Store value's length.                                                  *
   **************************************************************************/

   if ( pFEA )
      *(PUSHORT(p)) = Item->Length ;

   p += sizeof(USHORT) ;

  /**************************************************************************
   * Store value.                                                           *
   **************************************************************************/

   if ( pFEA )
      memcpy ( p, Item->Value, Item->Length ) ;

   p += Item->Length ;

  /**************************************************************************
   * Return count of bytes needed for item.                                 *
   **************************************************************************/

   return ( p - PUCHAR(pFEA) ) ;
}


/****************************************************************************
 *                                                                          *
 *      Build Extended Attributes                                           *
 *                                                                          *
 ****************************************************************************/

extern PEAOP2 BuildExtendedAttributes ( ULONG Count, EADATA Table[] ) {

  /**************************************************************************
   * Find out how much memory will be needed for the block.                 *
   **************************************************************************/

   ULONG cbEA = sizeof(FEA2LIST) - sizeof(FEA2) ;

   for ( int i=0; i<Count; i++ ) {
      cbEA += BuildExtendedAttributeItem ( 0, &Table[i] ) ;
      while ( cbEA % 4 )
         cbEA ++ ;
   } /* endfor i */

  /**************************************************************************
   * Allocate memory for the FEA2 list.                                     *
   **************************************************************************/

   PFEA2LIST pFEAList = PFEA2LIST ( new char [ cbEA ] )  ;

   if ( pFEAList == 0 )
      return ( 0 ) ;

  /**************************************************************************
   * Construct the extended attributes.                                     *
   **************************************************************************/

   PFEA2 pFEA = pFEAList->list ;

   for ( i=0; i<Count; i++ ) {
      PFEA2 Start = PFEA2 ( pFEA ) ;
      PUCHAR p = PUCHAR(pFEA) + BuildExtendedAttributeItem ( pFEA, &Table[i] ) ;
      pFEA = PFEA2 ( p ) ;
      while ( (PUCHAR(pFEA)-PUCHAR(pFEAList->list)) % 4 ) {
         PUCHAR p = PUCHAR ( pFEA ) + 1 ;
         pFEA = PFEA2 ( p ) ;
      } /* endwhile */
     if ( i < Count-1 )
        Start->oNextEntryOffset = PUCHAR(pFEA) - PUCHAR(Start) ;
     else
        Start->oNextEntryOffset = 0 ;
   } /* endfor i */

   pFEAList->cbList = PUCHAR(pFEA) - PUCHAR(pFEAList) ;

  /**************************************************************************
   * Allocate memory for the EA header block.                               *
   **************************************************************************/

   PEAOP2 pExtendedAttributes = PEAOP2 ( new char [ sizeof(EAOP2) ] ) ;

   if ( pExtendedAttributes == 0 ) {
      delete pFEAList ;
      return ( 0 ) ;
   } /* endif */

  /**************************************************************************
   * Fill in the extended attribute header block and return its address.    *
   **************************************************************************/

   pExtendedAttributes->fpGEA2List = 0 ;
   pExtendedAttributes->fpFEA2List = pFEAList ;
   pExtendedAttributes->oError = 0 ;

   return ( pExtendedAttributes ) ;
}


/****************************************************************************
 *                                                                          *
 *      Build Multi-Value Multi-Type EA Item's Value                        *
 *                                                                          *
 ****************************************************************************/

extern ULONG BuildMVMTValue ( PVOID Value, ULONG Count, MVMT_VALUE Table[] ) {

  /**************************************************************************
   * Store the number of values.                                            *
   **************************************************************************/

   PUCHAR p = PUCHAR ( Value ) ;

   if ( Value )
      *(PUSHORT(p)) = USHORT ( Count ) ;

   p += sizeof(USHORT) ;

  /**************************************************************************
   * Store the multiple values.                                             *
   **************************************************************************/

   for ( int i=0; i<Count; i++ ) {

      if ( Value )
         *(PUSHORT(p)) = Table[i].Type ;

      p += sizeof(USHORT) ;

      if ( Value )
         *(PUSHORT(p)) = Table[i].Length ;

      p += sizeof(USHORT) ;

      if ( Value )
         memcpy ( p, Table[i].Value, Table[i].Length ) ;

      p += Table[i].Length ;

   } /* endfor i */

  /**************************************************************************
   * Return the total byte count.                                           *
   **************************************************************************/

   return ( p - PUCHAR(Value) ) ;
}



