/*************************************************************** PROFILE2.CPP
 *                                                                          *
 *                     Profile (INI) File Object Class                      *
 *                                                                          *
 ****************************************************************************/

#include "System.h"

#include <direct.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Debug.h"
#include "Escriba.h"
#include "Printer.h"
#include "Profile2.h"
#include "ReString.h"

// #define DEBUG

/****************************************************************************
 *                                                                          *
 *                     Definitions & Declarations                           *
 *                                                                          *
 ****************************************************************************/

#define OFF(type, var)    ((int) &((*(type *) 0).var))
#define SIZE(type, var)   sizeof((*(type *) 0).var)

struct {
   char *Name ;
   int Offset ;
   int Size ;           // < 0 indicates null-terminated string.
} CreateParms [] = {

   { "Language",             OFF(INIDATA,Language),             -int(SIZE(INIDATA,Language))         },
   { "FileType",             OFF(INIDATA,FileType),             -int(SIZE(INIDATA,FileType))         },
   { "GraphicType",          OFF(INIDATA,GraphicType),          -int(SIZE(INIDATA,GraphicType))      },
   { "Animate",              OFF(INIDATA,Animate),               SIZE(INIDATA,Animate)               },
   { "Rulers",               OFF(INIDATA,Rulers),                SIZE(INIDATA,Rulers)                },
   { "ScrollBars",           OFF(INIDATA,ScrollBars),            SIZE(INIDATA,ScrollBars)            },
   { "StatusLine",           OFF(INIDATA,StatusLine),            SIZE(INIDATA,StatusLine)            },
   { "FullNames",            OFF(INIDATA,FullNames),             SIZE(INIDATA,FullNames)             },
   { "SpoolPages",           OFF(INIDATA,SpoolPages),            SIZE(INIDATA,SpoolPages)            },
   { "PrintReverse",         OFF(INIDATA,PrintReverse),          SIZE(INIDATA,PrintReverse)          },
   { "LoadMostRecent",       OFF(INIDATA,LoadMostRecent),        SIZE(INIDATA,LoadMostRecent)        },
   { "HideOnScreenClip",     OFF(INIDATA,HideOnScreenClip),      SIZE(INIDATA,HideOnScreenClip)      },
   { "ZoomType",             OFF(INIDATA,ZoomType),              SIZE(INIDATA,ZoomType)              },
   { "MainColors[0]",        OFF(INIDATA,MainColors[0]),         SIZE(INIDATA,MainColors[0])         },
   { "fMainColors[0]",       OFF(INIDATA,fMainColors[0]),        SIZE(INIDATA,fMainColors[0])        },
   { "MainColors[1]",        OFF(INIDATA,MainColors[1]),         SIZE(INIDATA,MainColors[1])         },
   { "fMainColors[1]",       OFF(INIDATA,fMainColors[1]),        SIZE(INIDATA,fMainColors[1])        },
   { "DocColors[0]",         OFF(INIDATA,DocColors[0]),          SIZE(INIDATA,DocColors[0])          },
   { "fDocColors[0]",        OFF(INIDATA,fDocColors[0]),         SIZE(INIDATA,fDocColors[0])         },
   { "DocColors[1]",         OFF(INIDATA,DocColors[1]),          SIZE(INIDATA,DocColors[1])          },
   { "fDocColors[1]",        OFF(INIDATA,fDocColors[1]),         SIZE(INIDATA,fDocColors[1])         },
   { "RulerColors[0]",       OFF(INIDATA,RulerColors[0]),        SIZE(INIDATA,RulerColors[0])        },
   { "fRulerColors[0]",      OFF(INIDATA,fRulerColors[0]),       SIZE(INIDATA,fRulerColors[0])       },
   { "RulerColors[1]",       OFF(INIDATA,RulerColors[1]),        SIZE(INIDATA,RulerColors[1])        },
   { "fRulerColors[1]",      OFF(INIDATA,fRulerColors[1]),       SIZE(INIDATA,fRulerColors[1])       },
   { "RulerFont",            OFF(INIDATA,RulerFont),            -int(SIZE(INIDATA,RulerFont))        },
   { "fRulerFont",           OFF(INIDATA,fRulerFont),            SIZE(INIDATA,fRulerFont)            },
   { "ExpandCount",          OFF(INIDATA,ExpandCount),           SIZE(INIDATA,ExpandCount)           },
   { "SpacingCount",         OFF(INIDATA,SpacingCount),          SIZE(INIDATA,SpacingCount)          },
   { "SizeCount",            OFF(INIDATA,SizeCount),             SIZE(INIDATA,SizeCount)             },
   { "LineColorCount",       OFF(INIDATA,LineColorCount),        SIZE(INIDATA,LineColorCount)        },
   { "LineTypeCount",        OFF(INIDATA,LineTypeCount),         SIZE(INIDATA,LineTypeCount)         },
   { "LineWidthCount",       OFF(INIDATA,LineWidthCount),        SIZE(INIDATA,LineWidthCount)        },
   { "FillColorCount",       OFF(INIDATA,FillColorCount),        SIZE(INIDATA,FillColorCount)        },
   { "FillTypeCount",        OFF(INIDATA,FillTypeCount),         SIZE(INIDATA,FillTypeCount)         },
   { "LineEndCount",         OFF(INIDATA,LineEndCount),          SIZE(INIDATA,LineEndCount)          },
   { "LineJoinCount",        OFF(INIDATA,LineJoinCount),         SIZE(INIDATA,LineJoinCount)         },
   { "Search_String",        OFF(INIDATA,Search_String),        -int(SIZE(INIDATA,Search_String))    },
   { "Search_Replace",       OFF(INIDATA,Search_Replace),       -int(SIZE(INIDATA,Search_Replace))   },
   { "Search_CaseSensitive", OFF(INIDATA,Search_CaseSensitive),  SIZE(INIDATA,Search_CaseSensitive)  },
   { "Search_Regular",       OFF(INIDATA,Search_Regular),        SIZE(INIDATA,Search_Regular)        },

   { "Title",                OFF(INIDATA,DocParms.Title),       -int(SIZE(INIDATA,DocParms.Title))   },   
   { "Author",               OFF(INIDATA,DocParms.Author),      -int(SIZE(INIDATA,DocParms.Author))  },
   { "CreateDate",           OFF(INIDATA,DocParms.CreateDate),   int(SIZE(INIDATA,DocParms.CreateDate))},
   { "Printer",              OFF(INIDATA,DocParms.Printer),     -int(SIZE(INIDATA,DocParms.Printer)) },
   { "JobSettings",          OFF(INIDATA,DocParms.JobSettings),  SIZE(INIDATA,DocParms.JobSettings)  },
   { "CodePage",             OFF(INIDATA,DocParms.CodePage),     SIZE(INIDATA,DocParms.CodePage)     },
   { "Font",                 OFF(INIDATA,DocParms.Font),        -int(SIZE(INIDATA,DocParms.Font))    },
   { "Size",                 OFF(INIDATA,DocParms.Size),         SIZE(INIDATA,DocParms.Size)         },
   { "Color",                OFF(INIDATA,DocParms.Color),        SIZE(INIDATA,DocParms.Color)        },
   { "FillColor",            OFF(INIDATA,DocParms.FillColor),    SIZE(INIDATA,DocParms.FillColor)    },
   { "Bold",                 OFF(INIDATA,DocParms.Bold),         SIZE(INIDATA,DocParms.Bold)         },
   { "Italic",               OFF(INIDATA,DocParms.Italic),       SIZE(INIDATA,DocParms.Italic)       },
   { "Underscore",           OFF(INIDATA,DocParms.Underscore),   SIZE(INIDATA,DocParms.Underscore)   },
   { "Strikeout",            OFF(INIDATA,DocParms.Strikeout),    SIZE(INIDATA,DocParms.Strikeout)    },
   { "Outline",              OFF(INIDATA,DocParms.Outline),      SIZE(INIDATA,DocParms.Outline)      },
   { "Caps",                 OFF(INIDATA,DocParms.Caps),         SIZE(INIDATA,DocParms.Caps)         },
   { "Expand",               OFF(INIDATA,DocParms.Expand),       SIZE(INIDATA,DocParms.Expand)       },
   { "Justify",              OFF(INIDATA,DocParms.Justify),      SIZE(INIDATA,DocParms.Justify)      },
   { "CenterPage",           OFF(INIDATA,DocParms.CenterPage),   SIZE(INIDATA,DocParms.CenterPage)   },
   { "Metric",               OFF(INIDATA,DocParms.Metric),       SIZE(INIDATA,DocParms.Metric)       },
   { "LeftMargin",           OFF(INIDATA,DocParms.LeftMargin),   SIZE(INIDATA,DocParms.LeftMargin)   },
   { "RightMargin",          OFF(INIDATA,DocParms.RightMargin),  SIZE(INIDATA,DocParms.RightMargin)  },
   { "TopMargin",            OFF(INIDATA,DocParms.TopMargin),    SIZE(INIDATA,DocParms.TopMargin)    },
   { "BottomMargin",         OFF(INIDATA,DocParms.BottomMargin), SIZE(INIDATA,DocParms.BottomMargin) },
   { "TabSpacing",           OFF(INIDATA,DocParms.TabSpacing),   SIZE(INIDATA,DocParms.TabSpacing)   },
   { "Indent",               OFF(INIDATA,DocParms.Indent),       SIZE(INIDATA,DocParms.Indent)       },
   { "LineSpacing",          OFF(INIDATA,DocParms.LineSpacing),  SIZE(INIDATA,DocParms.LineSpacing)  },
   { "DateFormat",           OFF(INIDATA,DocParms.DateFormat),  -int(SIZE(INIDATA,DocParms.DateFormat))},
   { "LineType",             OFF(INIDATA,DocParms.LineType),     SIZE(INIDATA,DocParms.LineType)     },
   { "LineWidth",            OFF(INIDATA,DocParms.LineWidth),    SIZE(INIDATA,DocParms.LineWidth)    },
   { "LineJoin",             OFF(INIDATA,DocParms.LineJoin),     SIZE(INIDATA,DocParms.LineJoin)     },
   { "LineEnd",              OFF(INIDATA,DocParms.LineEnd),      SIZE(INIDATA,DocParms.LineEnd)      },
   { "FillType",             OFF(INIDATA,DocParms.FillType),     SIZE(INIDATA,DocParms.FillType)     },

} ;

/****************************************************************************
 *                                                                          *
 *                           Get Profile Data                               *
 *                                                                          *
 ****************************************************************************/

BOOL Profile2::GetIniData ( INIDATA &IniData ) {

  /**************************************************************************
   * Get the window's current size and position.                            *
   **************************************************************************/

   #ifdef DEBUG
      Log ( "Profile2::GetIniData() Started.  Getting position." ) ;
   #endif // DEBUG

   IniData.fPosition = FALSE ;
   if ( GetItem ( "Position", &IniData.Position, sizeof(IniData.Position) ) )
      IniData.fPosition = TRUE ;

  /**************************************************************************
   * Get the program's last used directory and move there.                  *
   **************************************************************************/

   #ifdef DEBUG
      Log ( "Profile2::GetIniData() Getting working directory." ) ;
   #endif // DEBUG

   int Drive ;
   if ( GetItem ( "Drive", &Drive, sizeof(Drive) ) )
      _chdrive ( int ( Drive ) ) ;

   char Directory [CCHMAXPATH] ;
   if ( GetString ( "Directory", Directory, sizeof(Directory) ) ) {
      _chdir ( "\\" ) ;
      _chdir ( Directory ) ;
   } /* endif */

  /**************************************************************************
   * Get the current recall list.                                           *
   **************************************************************************/

   #ifdef DEBUG
      Log ( "Profile2::GetIniData() Getting recall list." ) ;
   #endif // DEBUG

   for ( int i=0; i<MAX_RECALL; i++ ) {

      char Name [10] ;
      sprintf ( Name, "File%i", i ) ;

      GetString ( Name, IniData.Files[i] ) ;

   } /* endif */

  /**************************************************************************
   * Go get the saved profile values, if they are to be found.              *
   **************************************************************************/

   #ifdef DEBUG
      Log ( "Profile2::GetIniData() Getting saved profile information." ) ;
   #endif // DEBUG

   for ( i=0; i<sizeof(CreateParms)/sizeof(CreateParms[0]); i++ ) {
      if ( CreateParms[i].Size > 0 ) {
         GetItem ( CreateParms[i].Name, ((PUCHAR)&IniData)+CreateParms[i].Offset, CreateParms[i].Size ) ;
      } else {
         GetString ( CreateParms[i].Name, ((PCHAR)&IniData)+CreateParms[i].Offset, -CreateParms[i].Size ) ;
      } /* endif */
   } /* endfor */

  /**************************************************************************
   * Get the line spacing table.                                            *
   **************************************************************************/

   #ifdef DEBUG
      Log ( "Profile2::GetIniData() Getting line spacing table." ) ;
   #endif // DEBUG

   if ( IniData.SpacingCount )
      if ( !GetItem ( "Spacings", IniData.Spacings, IniData.SpacingCount*sizeof(IniData.Spacings[0]) ) )
         IniData.SpacingCount = 0 ;

   if ( IniData.SpacingCount == 0 ) {
      IniData.Spacings[IniData.SpacingCount++] = 80 ;
      IniData.Spacings[IniData.SpacingCount++] = 90 ;
      IniData.Spacings[IniData.SpacingCount++] = 100 ;
      IniData.Spacings[IniData.SpacingCount++] = 110 ;
      IniData.Spacings[IniData.SpacingCount++] = 120 ;
      IniData.Spacings[IniData.SpacingCount++] = 150 ;
      IniData.Spacings[IniData.SpacingCount++] = 200 ;

   } /* endif */

  /**************************************************************************
   * Get the size table.                                                    *
   **************************************************************************/

   #ifdef DEBUG
      Log ( "Profile2::GetIniData() Getting font size table." ) ;
   #endif // DEBUG

   if ( IniData.SizeCount )
      if ( !GetItem ( "Sizes", IniData.Sizes, IniData.SizeCount*sizeof(IniData.Sizes[0]) ) )
         IniData.SizeCount = 0 ;

   if ( IniData.SizeCount == 0 ) {
      IniData.Sizes[IniData.SizeCount++] = 80 ;
      IniData.Sizes[IniData.SizeCount++] = 90 ;
      IniData.Sizes[IniData.SizeCount++] = 100 ;
      IniData.Sizes[IniData.SizeCount++] = 110 ;
      IniData.Sizes[IniData.SizeCount++] = 120 ;
      IniData.Sizes[IniData.SizeCount++] = 130 ;
      IniData.Sizes[IniData.SizeCount++] = 140 ;
      IniData.Sizes[IniData.SizeCount++] = 150 ;
      IniData.Sizes[IniData.SizeCount++] = 160 ;
      IniData.Sizes[IniData.SizeCount++] = 170 ;
      IniData.Sizes[IniData.SizeCount++] = 180 ;
      IniData.Sizes[IniData.SizeCount++] = 200 ;
      IniData.Sizes[IniData.SizeCount++] = 220 ;
      IniData.Sizes[IniData.SizeCount++] = 240 ;
      IniData.Sizes[IniData.SizeCount++] = 280 ;
      IniData.Sizes[IniData.SizeCount++] = 320 ;
      IniData.Sizes[IniData.SizeCount++] = 360 ;
      IniData.Sizes[IniData.SizeCount++] = 400 ;
      IniData.Sizes[IniData.SizeCount++] = 440 ;
      IniData.Sizes[IniData.SizeCount++] = 480 ;

   } /* endif */

  /**************************************************************************
   * Get the expansions table.                                              *
   **************************************************************************/

   #ifdef DEBUG
      Log ( "Profile2::GetIniData() Getting expansion table." ) ;
   #endif // DEBUG

   if ( IniData.ExpandCount )
      if ( !GetItem ( "Expansions", IniData.Expansions, IniData.ExpandCount*sizeof(IniData.Expansions[0]) ) )
         IniData.ExpandCount = 0 ;

   if ( IniData.ExpandCount == 0 ) {
      IniData.Expansions[IniData.ExpandCount++] = 50 ;
      IniData.Expansions[IniData.ExpandCount++] = 75 ;
      IniData.Expansions[IniData.ExpandCount++] = 100 ;
      IniData.Expansions[IniData.ExpandCount++] = 125 ;
      IniData.Expansions[IniData.ExpandCount++] = 150 ;
      IniData.Expansions[IniData.ExpandCount++] = 175 ;
      IniData.Expansions[IniData.ExpandCount++] = 200 ;

   } /* endif */

  /**************************************************************************
   * Get the line color table.                                              *
   **************************************************************************/

   #ifdef DEBUG
      Log ( "Profile2::GetIniData() Getting line color table." ) ;
   #endif // DEBUG

   if ( IniData.LineColorCount )
      if ( !GetItem ( "LineColors", IniData.LineColors, IniData.LineColorCount*sizeof(IniData.LineColors[0]) ) )
         IniData.LineColorCount = 0 ;

   if ( IniData.LineColorCount == 0 ) {
      IniData.LineColors[IniData.LineColorCount++] = RGB_BLACK ;
      IniData.LineColors[IniData.LineColorCount++] = 0x000080 ;        // Dark Blue
      IniData.LineColors[IniData.LineColorCount++] = 0x008000 ;        // Dark Green
      IniData.LineColors[IniData.LineColorCount++] = 0x008080 ;        // Dark Cyan
      IniData.LineColors[IniData.LineColorCount++] = 0x800000 ;        // Dark Red
      IniData.LineColors[IniData.LineColorCount++] = 0x800080 ;        // Dark Magenta
      IniData.LineColors[IniData.LineColorCount++] = 0x808000 ;        // Dark Yellow (Brown)
      IniData.LineColors[IniData.LineColorCount++] = 0x808080 ;        // Dark Grey
      IniData.LineColors[IniData.LineColorCount++] = 0xCCCCCC ;        // Grey
      IniData.LineColors[IniData.LineColorCount++] = RGB_BLUE ;
      IniData.LineColors[IniData.LineColorCount++] = RGB_GREEN ;
      IniData.LineColors[IniData.LineColorCount++] = RGB_CYAN ;
      IniData.LineColors[IniData.LineColorCount++] = RGB_RED ;
      IniData.LineColors[IniData.LineColorCount++] = RGB_PINK ;
      IniData.LineColors[IniData.LineColorCount++] = RGB_YELLOW ;
      IniData.LineColors[IniData.LineColorCount++] = RGB_WHITE ;

   } /* endif */

  /**************************************************************************
   * Get the line type table.                                               *
   **************************************************************************/

   #ifdef DEBUG
      Log ( "Profile2::GetIniData() Getting line type table." ) ;
   #endif // DEBUG

   if ( IniData.LineTypeCount )
      if ( !GetItem ( "LineTypes", IniData.LineTypes, IniData.LineTypeCount*sizeof(IniData.LineTypes[0]) ) )
         IniData.LineTypeCount = 0 ;

   if ( IniData.LineTypeCount == 0 ) {
      IniData.LineTypes[IniData.LineTypeCount++] = LINETYPE_DEFAULT ;
      IniData.LineTypes[IniData.LineTypeCount++] = LINETYPE_DOT ;
      IniData.LineTypes[IniData.LineTypeCount++] = LINETYPE_SHORTDASH ;
      IniData.LineTypes[IniData.LineTypeCount++] = LINETYPE_DASHDOT ;
      IniData.LineTypes[IniData.LineTypeCount++] = LINETYPE_DOUBLEDOT ;
      IniData.LineTypes[IniData.LineTypeCount++] = LINETYPE_LONGDASH ;
      IniData.LineTypes[IniData.LineTypeCount++] = LINETYPE_DASHDOUBLEDOT ;
      IniData.LineTypes[IniData.LineTypeCount++] = LINETYPE_SOLID ;
      IniData.LineTypes[IniData.LineTypeCount++] = LINETYPE_ALTERNATE ;

   } /* endif */

  /**************************************************************************
   * Get the line width table.                                              *
   **************************************************************************/

   #ifdef DEBUG
      Log ( "Profile2::GetIniData() Getting line width table." ) ;
   #endif // DEBUG

   if ( IniData.LineWidthCount )
      if ( !GetItem ( "LineWidths", IniData.LineWidths, IniData.LineWidthCount*sizeof(IniData.LineWidths[0]) ) )
         IniData.LineWidthCount = 0 ;

   if ( IniData.LineWidthCount == 0 ) {
      IniData.LineWidths[IniData.LineWidthCount++] = 1 ;
      IniData.LineWidths[IniData.LineWidthCount++] = 10 ;
      IniData.LineWidths[IniData.LineWidthCount++] = 20 ;
      IniData.LineWidths[IniData.LineWidthCount++] = 30 ;
      IniData.LineWidths[IniData.LineWidthCount++] = 40 ;
      IniData.LineWidths[IniData.LineWidthCount++] = 50 ;
      IniData.LineWidths[IniData.LineWidthCount++] = 60 ;
      IniData.LineWidths[IniData.LineWidthCount++] = 70 ;
      IniData.LineWidths[IniData.LineWidthCount++] = 80 ;
      IniData.LineWidths[IniData.LineWidthCount++] = 90 ;
      IniData.LineWidths[IniData.LineWidthCount++] = 100 ;

   } /* endif */

  /**************************************************************************
   * Get the fill color table.                                              *
   **************************************************************************/

   #ifdef DEBUG
      Log ( "Profile2::GetIniData() Getting fill color table." ) ;
   #endif // DEBUG

   if ( IniData.FillColorCount )
      if ( !GetItem ( "FillColors", IniData.FillColors, IniData.FillColorCount*sizeof(IniData.FillColors[0]) ) )
         IniData.FillColorCount = 0 ;

   if ( IniData.FillColorCount == 0 ) {
      IniData.FillColors[IniData.FillColorCount++] = RGB_BLACK ;
      IniData.FillColors[IniData.FillColorCount++] = 0x000080 ;        // Dark Blue
      IniData.FillColors[IniData.FillColorCount++] = 0x008000 ;        // Dark Green
      IniData.FillColors[IniData.FillColorCount++] = 0x008080 ;        // Dark Cyan
      IniData.FillColors[IniData.FillColorCount++] = 0x800000 ;        // Dark Red
      IniData.FillColors[IniData.FillColorCount++] = 0x800080 ;        // Dark Magenta
      IniData.FillColors[IniData.FillColorCount++] = 0x808000 ;        // Dark Yellow (Brown)
      IniData.FillColors[IniData.FillColorCount++] = 0x808080 ;        // Dark Grey
      IniData.FillColors[IniData.FillColorCount++] = 0xCCCCCC ;        // Grey
      IniData.FillColors[IniData.FillColorCount++] = RGB_BLUE ;
      IniData.FillColors[IniData.FillColorCount++] = RGB_GREEN ;
      IniData.FillColors[IniData.FillColorCount++] = RGB_CYAN ;
      IniData.FillColors[IniData.FillColorCount++] = RGB_RED ;
      IniData.FillColors[IniData.FillColorCount++] = RGB_PINK ;
      IniData.FillColors[IniData.FillColorCount++] = RGB_YELLOW ;
      IniData.FillColors[IniData.FillColorCount++] = RGB_WHITE ;

   } /* endif */

  /**************************************************************************
   * Get the fill type table.                                               *
   **************************************************************************/

   #ifdef DEBUG
      Log ( "Profile2::GetIniData() Getting fill type table." ) ;
   #endif // DEBUG

   if ( IniData.FillTypeCount ) 
      if ( !GetItem ( "FillTypes", IniData.FillTypes, IniData.FillTypeCount*sizeof(IniData.FillTypes[0]) ) )
         IniData.FillTypeCount = 0 ;

   if ( IniData.FillTypeCount == 0 ) {
      IniData.FillTypes[IniData.FillTypeCount++] = PATSYM_DEFAULT ;
      IniData.FillTypes[IniData.FillTypeCount++] = PATSYM_DENSE1 ;
      IniData.FillTypes[IniData.FillTypeCount++] = PATSYM_DENSE2 ;
      IniData.FillTypes[IniData.FillTypeCount++] = PATSYM_DENSE3 ;
      IniData.FillTypes[IniData.FillTypeCount++] = PATSYM_DENSE4 ;
      IniData.FillTypes[IniData.FillTypeCount++] = PATSYM_DENSE5 ;
      IniData.FillTypes[IniData.FillTypeCount++] = PATSYM_DENSE6 ;
      IniData.FillTypes[IniData.FillTypeCount++] = PATSYM_DENSE7 ;
      IniData.FillTypes[IniData.FillTypeCount++] = PATSYM_DENSE8 ;
      IniData.FillTypes[IniData.FillTypeCount++] = PATSYM_VERT ;
      IniData.FillTypes[IniData.FillTypeCount++] = PATSYM_HORIZ ;
      IniData.FillTypes[IniData.FillTypeCount++] = PATSYM_DIAG1 ;
      IniData.FillTypes[IniData.FillTypeCount++] = PATSYM_DIAG2 ;
      IniData.FillTypes[IniData.FillTypeCount++] = PATSYM_DIAG3 ;
      IniData.FillTypes[IniData.FillTypeCount++] = PATSYM_DIAG4 ;
      IniData.FillTypes[IniData.FillTypeCount++] = PATSYM_NOSHADE ;
      IniData.FillTypes[IniData.FillTypeCount++] = PATSYM_SOLID ;
      IniData.FillTypes[IniData.FillTypeCount++] = PATSYM_HALFTONE ;
      IniData.FillTypes[IniData.FillTypeCount++] = PATSYM_HATCH ;
      IniData.FillTypes[IniData.FillTypeCount++] = PATSYM_DIAGHATCH ;

   } /* endif */

  /**************************************************************************
   * Get the line end table.                                                *
   **************************************************************************/

   #ifdef DEBUG
      Log ( "Profile2::GetIniData() Getting line end table." ) ;
   #endif // DEBUG

   if ( IniData.LineEndCount )
      if ( !GetItem ( "LineEnds", IniData.LineEnds, IniData.LineEndCount*sizeof(IniData.LineEnds[0]) ) )
        IniData.LineEndCount = 0 ;

   if ( IniData.LineEndCount == 0 ) {
      IniData.LineEnds[IniData.LineEndCount++] = LINEEND_FLAT ;
      IniData.LineEnds[IniData.LineEndCount++] = LINEEND_SQUARE ;
      IniData.LineEnds[IniData.LineEndCount++] = LINEEND_ROUND ;

   } /* endif */

  /**************************************************************************
   * Get the line join table.                                               *
   **************************************************************************/

   #ifdef DEBUG
      Log ( "Profile2::GetIniData() Getting line join table." ) ;
   #endif // DEBUG

   if ( IniData.LineJoinCount )
      if ( !GetItem ( "LineJoins", IniData.LineJoins, IniData.LineJoinCount*sizeof(IniData.LineJoins[0]) ) )
         IniData.LineJoinCount = 0 ;

   if ( IniData.LineJoinCount == 0 ) {
      IniData.LineJoins[IniData.LineJoinCount++] = LINEJOIN_BEVEL ;
      IniData.LineJoins[IniData.LineJoinCount++] = LINEJOIN_ROUND ;
      IniData.LineJoins[IniData.LineJoinCount++] = LINEJOIN_MITRE ;

   } /* endif */

  /**************************************************************************
   * Return FALSE, indicating no error.                                     *
   **************************************************************************/

   #ifdef DEBUG
      Log ( "Profile2::GetIniData() Done." ) ;
   #endif // DEBUG

   return ( FALSE ) ;
}
 
/****************************************************************************
 *                                                                          *
 *                           Put Profile Data                               *
 *                                                                          *
 ****************************************************************************/

void Profile2::PutIniData ( INIDATA &IniData ) {

  /**************************************************************************
   * Save the window's current drive and directory.                         *
   **************************************************************************/

   int Drive = _getdrive ( ) ;
   PutItem ( "Drive", &Drive, sizeof(Drive) ) ;

   char Directory [ CCHMAXPATH ] ;
   getcwd ( Directory, sizeof(Directory) ) ;
   PutString ( "Directory", Directory ) ;

  /**************************************************************************
   * Save the window's current size and position.                           *
   **************************************************************************/

   PutItem ( "Position", &IniData.Position, ULONG(sizeof(IniData.Position)) ) ;

  /**************************************************************************
   * Save the current recall list.                                          *
   **************************************************************************/

   for ( int i=0; i<MAX_RECALL; i++ ) {

      char Name [10] ;
      sprintf ( Name, "File%i", i ) ;

      if ( IniData.Files[i] ) 
         PutString ( Name, IniData.Files[i] ) ;
      else
         Reset ( Name ) ;

   } /* endfor */

  /**************************************************************************
   * Save the new document creation parameters.                             *
   **************************************************************************/

   for ( i=0; i<sizeof(CreateParms)/sizeof(CreateParms[0]); i++ ) {
      if ( CreateParms[i].Size > 0 ) {
         PutItem ( CreateParms[i].Name, ((PUCHAR)&IniData)+CreateParms[i].Offset, CreateParms[i].Size ) ;
      } else {
         PutString ( CreateParms[i].Name, ((PCHAR)&IniData)+CreateParms[i].Offset ) ;
      } /* endif */
   } /* endfor */

  /**************************************************************************
   * Save the line spacing table.                                           *
   **************************************************************************/

   PutItem ( "Spacings", IniData.Spacings, IniData.SpacingCount*sizeof(IniData.Spacings[0]) ) ;

  /**************************************************************************
   * Save the size table.                                                   *
   **************************************************************************/

   PutItem ( "Sizes", IniData.Sizes, IniData.SizeCount*sizeof(IniData.Sizes[0]) ) ;

  /**************************************************************************
   * Save the expansions table.                                             *
   **************************************************************************/

   PutItem ( "Expansions", IniData.Expansions, IniData.ExpandCount*sizeof(IniData.Expansions[0]) ) ;

  /**************************************************************************
   * Save the line color table.                                             *
   **************************************************************************/

   PutItem ( "LineColors", IniData.LineColors, IniData.LineColorCount*sizeof(IniData.LineColors[0]) ) ;

  /**************************************************************************
   * Save the line type table.                                              *
   **************************************************************************/

   PutItem ( "LineTypes", IniData.LineTypes, IniData.LineTypeCount*sizeof(IniData.LineTypes[0]) ) ;

  /**************************************************************************
   * Save the line width table.                                             *
   **************************************************************************/

   PutItem ( "LineWidths", IniData.LineWidths, IniData.LineWidthCount*sizeof(IniData.LineWidths[0]) ) ;

  /**************************************************************************
   * Save the fill color table.                                             *
   **************************************************************************/

   PutItem ( "FillColors", IniData.FillColors, IniData.FillColorCount*sizeof(IniData.FillColors[0]) ) ;

  /**************************************************************************
   * Save the fill type table.                                              *
   **************************************************************************/

   PutItem ( "FillTypes", IniData.FillTypes, IniData.FillTypeCount*sizeof(IniData.FillTypes[0]) ) ;

  /**************************************************************************
   * Save the line end table.                                               *
   **************************************************************************/

   PutItem ( "LineEnds", IniData.LineEnds, IniData.LineEndCount*sizeof(IniData.LineEnds[0]) ) ;

  /**************************************************************************
   * Save the line join table.                                              *
   **************************************************************************/

   PutItem ( "LineJoins", IniData.LineJoins, IniData.LineJoinCount*sizeof(IniData.LineJoins[0]) ) ;
}

/****************************************************************************
 *                                                                          *
 *  IniData: Constructor                                                    *
 *                                                                          *
 ****************************************************************************/

_IniData::_IniData ( HMODULE RegistrationLibrary ) : 
   DocParms(RegistrationLibrary), FileListLock("_IniData::FileListLock"), 
   Animate(TRUE), Rulers(TRUE), ScrollBars(TRUE), StatusLine(TRUE), FullNames(TRUE), SpoolPages(TRUE),
   PrintReverse(FALSE), LoadMostRecent(FALSE), HideOnScreenClip(TRUE), ZoomType(IDM_ZOOM_100), fRulerFont(FALSE), 
   SpacingCount(0), SizeCount(0), LineColorCount(0), LineTypeCount(0), LineWidthCount(0), 
   FillColorCount(0), FillTypeCount(0), LineEndCount(0), LineJoinCount(0), 
   Search_CaseSensitive(FALSE), Search_Regular(FALSE) {

   FileListLock.Request ( ) ;
   for ( int i=0; i<MAX_RECALL; i++ ) 
      Files[i] = 0 ;
   FileListLock.Release ( ) ;

   strcpy ( Language, Library->QueryLanguage() ) ;
   strcpy ( FileType, "" ) ;
   strcpy ( GraphicType, "" ) ;

   fMainColors[0] = FALSE ;
   fMainColors[1] = FALSE ;
   fDocColors[0] = FALSE ;
   fDocColors[1] = FALSE ;
   fRulerColors[0] = FALSE ;
   fRulerColors[1] = FALSE ;

   ResourceString DefaultFont ( Library->QueryHandle(), IDS_DEFAULTFONT ) ;
   strcpy ( PCHAR(RulerFont), PCHAR(DefaultFont) ) ;

   memset ( Search_String, 0, sizeof(Search_String) ) ;
   memset ( Search_Replace, 0, sizeof(Search_Replace) ) ;

} /* endmethod */

/****************************************************************************
 *                                                                          *
 *  IniData: Destructor                                                     *
 *                                                                          *
 ****************************************************************************/

_IniData::~_IniData ( ) {
   FileListLock.Request ( ) ;
   for ( int i=0; i<MAX_RECALL; i++ ) {
      if ( Files[i] ) {
         free ( Files[i] ) ;
         Files[i] = 0 ;
      } /* endif */
   } /* endfor */
   FileListLock.Release ( ) ;
} /* endmethod */

/****************************************************************************
 *                                                                          *
 *  IniData: Update File Recall List                                        *
 *                                                                          *
 ****************************************************************************/

void _IniData::UpdateFileList ( char *Filename ) {

   // Lock the list.
   FileListLock.Request ( ) ;

   // Fold the entire filename to upper case.
   char Text1 [_MAX_PATH] ;
   strcpy ( Text1, PCHAR(Filename) ) ;
   COUNTRYCODE Country = { 0, 0 } ;
   DosMapCase ( strlen(PCHAR(Text1)), &Country, PCHAR(Text1) ) ;

   // Check if the file already exists in the list.
   for ( int i=0; i<MAX_RECALL; i++ ) {
      if ( Files[i] ) {
         char Text2 [_MAX_PATH] ;
         strcpy ( Text2, PCHAR(Files[i]) ) ;
         COUNTRYCODE Country = { 0, 0 } ;
         DosMapCase ( strlen(PCHAR(Text2)), &Country, PCHAR(Text2) ) ;
         if ( !strcmp ( Text1, Text2 ) )
            break ;
      } /* endif */
   } /* endfor */

   // If the file already exists in the list, bring it to the top and return.
   if ( i < MAX_RECALL ) {
      PSZ File = Files[i] ;
      while ( i > 0 ) {
         Files[i] = Files[i-1] ;
         i -- ;
      } /* endwhile */
      Files[0] = File ;
      return ;
   } /* endif */

   // If the list is full, discard the last entry.
   if ( Files[MAX_RECALL-1] ) 
      free ( Files[MAX_RECALL-1] ) ;

   // Move all the other entries down a notch.
   for ( i=MAX_RECALL-1; i>0; i-- ) 
      Files[i] = Files[i-1] ;

   // Save the new filename at the top of the list.
   Files[0] = PSZ ( malloc ( strlen ( PCHAR(Filename) ) + 1 ) ) ;
   if ( Files[0] ) 
      strcpy ( PCHAR(Files[0]), PCHAR(Filename) ) ;

   // Release the list.
   FileListLock.Release ( ) ;

} /* endmethod */

