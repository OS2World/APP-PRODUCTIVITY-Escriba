/****************************************************************** DIALOG2.H
 *                                                                          *
 *                       Basic Dialog Definitions                           *
 *                                                                          *
 ****************************************************************************/

#ifndef DIALOG2_H
#define DIALOG2_H

#include "System.h"

#include "Addons.h"


        // Definitions for standard dialog subclassing.

extern WINDOWPROC Dialog_Processor ;


        // Definitions for Document File Dialog.

typedef struct {
   BOOL SelectingForLoad ;              // Flag: File is being selected for Open/Load/Merge.
   PSZ *TypeList ;                      // -> Array of pointers to file extended type strings.
   char **WildcardList ;                // -> Array of pointers to file wildcard strings.
   Addon **ExtenderList ;               // -> Array of pointers to file import extender objects.
   int Index ;                          // Current index into arrays.
   int FilterIndex ;                    // Currently selected filter index (-1=AllFiles,0=AllTypes,1->n=Type).
} FILEDIALOG_DATA, *PFILEDIALOG_DATA ;

extern PSZ FileTypeList[] ;
extern char *FileWildcardList[] ;
extern Addon *FileExtenderList[] ;
extern void PrepareFileTypeList ( BOOL ForExport = FALSE ) ;
extern void ClearFileTypeList ( void ) ;
extern WINDOWPROC FileDialog_Processor ;


        // Additional definitions for Graphics File Dialog.

extern PSZ GraphicTypeList[] ;
extern char *GraphicWildcardList[] ;
extern Addon *GraphicExtenderList[] ;
extern void PrepareGraphicTypeList ( void ) ;
extern void ClearGraphicTypeList ( void ) ;


        // Determine file type against extender list.

extern int GetFileTypeIndex ( HWND Window, char *Name, PSZ *TypeList, Addon *ExtenderList[], int Index ) ;

#endif
