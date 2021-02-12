/****************************************************************** READERS.H
 *                                                                          *
 *  READER Definitions & Functions                                          *
 *                                                                          *
 ****************************************************************************/

#ifndef READERS_H
#define READERS_H

  // OS/2 Definitions, if not already loaded.

#include "System.h"

  // Data Types

typedef struct {
   PUCHAR Buffer ;
   ULONG Index ;
   ULONG Size ;
} TEXTDATA, *PTEXTDATA ;

typedef SHORT (TEXTREADER) (PTEXTDATA) ;
typedef TEXTREADER *PTEXTREADER ;
typedef ULONG (TEXTTELLER) (PTEXTDATA) ;
typedef TEXTTELLER *PTEXTTELLER ;

typedef struct {
   HFILE Handle ;
   ULONG Capacity ;
   PUCHAR Buffer ;
   ULONG Index ;
   ULONG Size ;
} FILEDATA, *PFILEDATA ;

typedef SHORT (FILEREADER) (PFILEDATA) ;
typedef FILEREADER *PFILEREADER ;
typedef ULONG (FILETELLER) (PFILEDATA) ;
typedef FILETELLER *PFILETELLER ;

typedef SHORT (READER) (PVOID) ;
typedef READER *PREADER ;
typedef ULONG (TELLER) (PVOID) ;
typedef TELLER *PTELLER ;


  // Function Prototypes

extern FILEREADER ReadFile ;
extern FILEREADER PeekFile ;
extern FILETELLER TellFile ;

extern TEXTREADER ReadText ;
extern TEXTREADER PeekText ;
extern TEXTTELLER TellText ;

#endif
