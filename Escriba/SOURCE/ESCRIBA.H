/****************************************************************** ESCRIBA.H
 *                                                                          *
 *                  Escriba Word Processor for OS/2-PM                      *
 *                                                                          *
 ****************************************************************************/

#ifndef ESCRIBA_H
#define ESCRIBA_H

#define REVISION     "Escriba 0.96d"
#define REVISION_NO  (96)

#include "System.h"

#include "HelpWin.h"
#include "Module.h"

#define PROGRAM_NAME "ESCRIBA"
#define CLASS_NAME   PROGRAM_NAME
#define LOGFILE      PROGRAM_NAME ".LOG"

extern char HomePath [] ;
extern HelpWindow *Help ;
extern LanguageModule *Library ;
extern HMODULE LibraryHandle ;

enum {
   WM_LOAD_FILE = WM_USER + 0x400,
   WM_LOAD_DONE,
   WM_PRINT_DONE,
   WM_SAVE_DONE,
   WM_THREAD_STARTED,
   WM_THREAD_DONE,
   WM_ACTIVATED,
   WM_CLOSE_ME,
   WM_QUERY_STATE,
   WM_QUERY_ZOOM,
   WM_SET_ZOOM,
   WM_SET_MARGINS,
   WM_SET_TABS,
   WM_SET_INDENT,
   WM_SET_DATEFMT,
   WM_SET_FONT,
   WM_SET_SIZE,
   WM_SET_EXPANSION,
   WM_SET_LINECOLOR,
   WM_SET_LINEWIDTH,
   WM_SET_FILLCOLOR,
   WM_SET_FILLTYPE,
   WM_SET_LINEEND,
   WM_SET_LINEJOIN,
   WM_SET_LINESPACING,
   WM_SET_RULER,
   WM_SET_TICK,
   WM_SET_LEFTMARGIN,
   WM_SET_RIGHTMARGIN,
   WM_SET_BOTTOMMARGIN,
   WM_SET_TOPMARGIN,
   WM_SET_TAB,
   WM_CLEAR_TAB,
   WM_MOVE_TAB,
   WM_SET_DOCCOLORS,
   WM_SET_DEFAULTS,
   WM_QUERY_DEFAULTS,
   WM_ALTER_FRAME,
   WM_ALTER_WINDOW,
   WM_UPDATE_MOUSE_POINTER,
   WM_QUERY_TEXT_RECTANGLE,
   WM_SET_CODEPAGE,
   WM_SET_LANGUAGE,
   WM_SET_HEADER,
   WM_SET_FOOTER
} ;

#define ID_MAIN                                 1
#define ID_DATAWINDOW                           2
#define ID_TOP                                  3
#define ID_PGUP                                 4
#define ID_PGDN                                 5
#define ID_BOTTOM                               6
#define ID_TABSTOP                              7
#define ID_MARGIN                               8

#define IDD_PROFILE_PATH                        9900
#define   IDD_PROF_ENTRY                        9900
#define   IDD_PROF_ERR                          9901

#define IDM_DUMP                                9910
#define IDM_NEXTDOC                             9911

#define IDD_PROMPT_SAVE                         9920
#define IDD_PROMPT_SAVECONFIRM1                 9921
#define IDD_PROMPT_SAVECONFIRM2                 9922

#define IDD_MESSAGE_SPELLDONE                   9930

#define IDD_ERROR_HELPMANAGER                   9950
#define IDD_ERROR_EXCEPTION                     9951
#define IDD_ERROR_WINREGISTERCLASS              9952
#define IDD_ERROR_CREATECLIENT                  9953
#define IDD_ERROR_TOOMANYDOCUMENTS              9954
#define IDD_ERROR_TOOMANYLOADED                 9955
#define IDD_ERROR_DOCUMENT_CREATE               9956
#define IDD_ERROR_DOCUMENT_LOAD                 9957
#define IDD_ERROR_DOCUMENT_SAVE                 9958
#define IDD_ERROR_CREATEPRINTSTATUS             9959
#define IDD_ERROR_CREATELOADSTATUS              9960
#define IDD_ERROR_BLOCKMEMORY                   9961
#define IDD_ERROR_BADGRAPHICFILE                9962
#define IDD_ERROR_VERIFY1                       9963
#define IDD_ERROR_VERIFY2                       9964
#define IDD_ERROR_VERIFY3                       9965
#define IDD_ERROR_VERIFY4                       9966
#define IDD_ERROR_VERIFY5                       9967
#define IDD_ERROR_VERIFY6                       9968
#define IDD_ERROR_VERIFY7                       9969
#define IDD_ERROR_VERIFY8                       9970
#define IDD_ERROR_IMPORT1                       9971
#define IDD_ERROR_IMPORT2                       9972
#define IDD_ERROR_EXPORT1                       9973
#define IDD_ERROR_EXPORT2                       9974
#define IDD_METAFILE_VERIFY1                    9975
#define IDD_METAFILE_VERIFY2                    9976
#define IDD_METAFILE_VERIFY3                    9977
#define IDD_METAFILE_VERIFY4                    9978
#define IDD_ERROR_LOADLANGUAGE                  9979

#define IDM_MAIN                                ID_MAIN

#define IDM_FILE_MENU                           10
#define   IDM_NEW                               1000
#define   IDM_OPEN                              1001
#define     IDD_FILETYPE                        10010
#define       IDD_FILETYPE_NAME                 10011
#define       IDD_FILETYPE_LIST                 10012
#define   IDM_RECALL                            1002
#define     IDM_RECALL0                         10020
#define     IDM_RECALLMAX                       10029
#define   IDM_MERGE                             1003
#define   IDM_SAVE                              1004
#define   IDM_SAVEAS                            1005
#define   IDM_SAVE_CLOSE                        1006
#define   IDM_CLOSE                             1007
#define   IDM_PRINT                             1008
#define     IDM_PRINT_ALL                       10080
#define     IDM_PRINT_ALL_NORMAL                10081
#define     IDM_PRINT_ALL_REVERSE               10082
#define     IDM_PRINT_PAGE                      10083
#define   IDM_PREFERENCES                       1009
#define     IDD_PREFERENCES                     10090
#define     IDD_PREFERENCES_ANIMATE             10090
#define     IDD_PREFERENCES_RULERS              10091
#define     IDD_PREFERENCES_SCBARS              10092
#define     IDD_PREFERENCES_STATUSLINE          10093
#define     IDD_PREFERENCES_FULLNAMES           10094
#define     IDD_PREFERENCES_SPOOLPAGES          10095
#define     IDD_PREFERENCES_PRINTREVERSE        10096
#define     IDD_PREFERENCES_LOADMOSTRECENT      10097
#define     IDD_PREFERENCES_ZOOM_100            10098
#define     IDD_PREFERENCES_ZOOM_FULLWIDTH      10099
#define     IDD_PREFERENCES_ZOOM_FULLHEIGHT     10100
#define     IDD_PREFERENCES_ZOOM_MARGINS        10101
#define     IDD_PREFERENCES_ZOOM_ALL            10102
#define     IDD_PREFERENCES_ERR                 10199
#define   IDM_DEFAULTS                          1011
#define     IDD_DEFAULTS                        10110
#define       IDD_DEFAULTS_NOTEBOOK             10111
#define       IDD_DEFAULTS_PAGE_AUTHOR          10112
#define         IDD_DEFAULTS_AUTHOR             10113
#define         IDD_DEFAULTS_TITLE_TAG          10114
#define         IDD_DEFAULTS_TITLE              10115
#define         IDD_DEFAULTS_CREATEDATE_TAG     10116
#define         IDD_DEFAULTS_CREATEDATE         10117
#define       IDD_DEFAULTS_PAGE_PRINTER         10118
#define         IDD_DEFAULTS_PRINTER            10119
#define         IDD_DEFAULTS_SETTINGS           10120
#define         IDD_DEFAULTS_FORMNAME           10121
#define         IDD_DEFAULTS_FORMSIZE           10122
#define       IDD_DEFAULTS_PAGE_FONT            10123
#define         IDD_DEFAULTS_FONT               10124
#define         IDD_DEFAULTS_SIZE               10125
#define         IDD_DEFAULTS_COLOR              10126
#define         IDD_DEFAULTS_BOLD               10127
#define         IDD_DEFAULTS_ITALIC             10128
#define         IDD_DEFAULTS_UNDERSCORE         10129
#define         IDD_DEFAULTS_STRIKEOUT          10130
#define         IDD_DEFAULTS_OUTLINE            10131
#define         IDD_DEFAULTS_CAPS               10132
#define       IDD_DEFAULTS_PAGE_LAYOUT1         10133
#define         IDD_DEFAULTS_JUSTIFY            10134
#define         IDD_DEFAULTS_CENTERPAGE         10135
#define         IDD_DEFAULTS_LEFT               10136
#define         IDD_DEFAULTS_RIGHT              10137
#define         IDD_DEFAULTS_TOP                10138
#define         IDD_DEFAULTS_BOTTOM             10139
#define       IDD_DEFAULTS_PAGE_LAYOUT2         10140
#define         IDD_DEFAULTS_TAB                10141
#define         IDD_DEFAULTS_INDENT             10142
#define         IDD_DEFAULTS_LINE_100           10143
#define         IDD_DEFAULTS_LINE_150           10144
#define         IDD_DEFAULTS_LINE_200           10145
#define       IDD_DEFAULTS_PAGE_NATIONAL        10146
#define         IDD_DEFAULTS_CODEPAGE           10147
#define         IDD_DEFAULTS_DATEFMT            10148
#define         IDD_DEFAULTS_DATESMP            10149
#define         IDD_DEFAULTS_METRIC             10150
#define     IDD_DEFAULTS_GLOBAL                 10151
#define     IDD_DEFAULTS_UNITS                  10152
#define     IDD_DEFAULTS_APPLY                  10153
#define     IDD_DEFAULTS_ERR                    10154
#define   IDM_DEFAULTS_CONTEXT                  1016
#define   IDM_CONFIGURE_EXTENSIONS              1017
#define     IDD_CONFIGURE_EXTENSIONS            10170
#define     IDD_CONFIGURE_NOTEBOOK              10171
#define     IDD_CONFIGURE_ERR                   10172
#define   IDM_LANGUAGE_MENU                     1018
#define     IDM_LANGUAGES                       10180
#define     IDM_LANGUAGES_MAX                   10199
#define   IDM_EXIT                              1099

#define IDM_EDIT_MENU                           20
#define   IDM_UNDO                              2000
#define   IDM_REDO                              2010
#define   IDM_CUT                               2020
#define   IDM_COPY                              2030
#define   IDM_PASTE                             2040
#define   IDM_CLEAR                             2050
#define   IDM_CLEARCLIPBOARD                    2060
#define   IDM_SELECT_ALL                        2070
#define   IDM_PASTE_FROM                        2080
#define     IDM_PASTE_FROM_CLIP                 20800
#define       IDM_PASTE_FROM_CLIP_BITMAP        20801
#define       IDM_PASTE_FROM_CLIP_METAFILE      20802
#define     IDM_PASTE_FROM_SCRN                 20803
#define       IDM_PASTE_FROM_SCRN_ENTIRE        20804
#define       IDM_PASTE_FROM_SCRN_FRAME         20805
#define       IDM_PASTE_FROM_SCRN_WINDOW        20806
#define       IDM_PASTE_FROM_SCRN_RECT          20807
#define       IDM_PASTE_FROM_SCRN_HIDE          20808
#define     IDM_PASTE_FROM_FILE                 20809
#define   IDM_INSERT_FUNCTION                   2090
#define     IDM_DATE                            20900
#define       IDM_DATETODAY                     20901
#define       IDM_DATECURRENT                   20902
#define       IDM_DATECHANGED                   20903
#define       IDM_DATEFIXED                     20904
#define       IDM_DATEFMT                       20905
#define         IDD_DATEFMT                     20906
#define         IDD_DATEFMT_ENTRY               20907
#define         IDD_DATEFMT_SAMPLE              20908
#define         IDD_DATEFMT_APPLY               20909
#define         IDD_DATEFMT_ERR                 20910
#define     IDM_CURRENTPAGE                     20911
#define     IDM_PAGECOUNT                       20912
#define     IDM_SEPARATOR                       20913
#define     IDM_BARCODE                         20914
#define       IDD_BARCODE                       20915
#define         IDD_BARCODE_TYPE                20916
#define         IDD_BARCODE_ENTRY               20917
#define         IDD_BARCODE_CHARS               20918
#define         IDD_BARCODE_ERR                 20919
#define   IDM_SEARCH                            2100
#define     IDD_SEARCH                          21000
#define        IDD_SEARCH_STRING                21001
#define        IDD_SEARCH_REPLACE               21002
#define        IDD_SEARCH_CASESENSITIVE         21003
#define        IDD_SEARCH_REGULAR               21004
#define        IDD_SEARCH_FIND                  21005
#define        IDD_SEARCH_CHANGE                21006
#define        IDD_SEARCH_CHANGEFIND            21007
#define        IDD_SEARCH_CHANGEALL             21008
#define        IDD_SEARCH_ERR                   21009
#define   IDM_FIND                              2110
#define   IDM_FINDNEXT                          2120
#define   IDM_CHANGE                            2130
#define   IDM_CHANGEFIND                        2140
#define   IDM_CHANGEALL                         2150
#define   IDM_GOTOPAGE                          2160
#define     IDD_GOTOPAGE                        21601
#define        IDD_GOTOPAGE_ENTRY               21602
#define        IDD_GOTOPAGE_ERR                 21609
#define   IDM_SPELL                             2170
#define     IDD_SPELL                           21701
#define        IDD_SPELL_OLDWORD                21702
#define        IDD_SPELL_NEWWORD                21703
#define        IDD_SPELL_SKIP                   21704
#define        IDD_SPELL_ACCEPT                 21705
#define        IDD_SPELL_INSERT                 21706
#define        IDD_SPELL_UNCAPS                 21707
#define        IDD_SPELL_REPLACE                21708
#define        IDD_SPELL_ERR                    21709
#define   IDM_ZOOM                              2180
#define     IDM_ZOOM_100                        21800
#define     IDM_ZOOM_FULLWIDTH                  21801
#define     IDM_ZOOM_FULLHEIGHT                 21802
#define     IDM_ZOOM_MARGINS                    21803
#define     IDM_ZOOM_ALL                        21804
#define     IDM_ZOOM_WINDOW                     21805
#define     IDM_ZOOM_SET                        21806
#define       IDD_ZOOM_SET                      21807
#define       IDD_ZOOM_APPLY                    21808
#define       IDD_ZOOM_ERR                      21809

#define IDM_LAYOUT_MENU                         30
#define   IDM_MARGINS                           3000
#define     IDD_MARGINS                         30000
#define     IDD_MARGINS_LEFT                    30000
#define     IDD_MARGINS_RIGHT                   30001
#define     IDD_MARGINS_TOP                     30002
#define     IDD_MARGINS_BOTTOM                  30003
#define     IDD_MARGINS_UNITS                   30004
#define     IDD_MARGINS_SIZE                    30005
#define     IDD_MARGINS_APPLY                   30006
#define     IDD_MARGINS_ERR                     30007
#define   IDM_TABS                              3001
#define     IDD_TABS                            30010
#define     IDD_TABS_POSITION                   30010
#define     IDD_TABS_SET                        30011
#define     IDD_TABS_CLEAR                      30012
#define     IDD_TABS_UNITS                      30013
#define     IDD_TABS_SIZE                       30014
#define     IDD_TABS_APPLY                      30015
#define     IDD_TABS_ERR                        30016
#define   IDM_INDENT                            3002
#define     IDD_INDENT                          30020
#define     IDD_INDENT_DISTANCE                 30021
#define     IDD_INDENT_UNITS                    30022
#define     IDD_INDENT_SIZE                     30023
#define     IDD_INDENT_APPLY                    30024
#define     IDD_INDENT_ERR                      30025
#define   IDM_LINESPACING_MENU                  3003
#define     IDM_LINESPACINGS                    30030
#define     IDM_LINESPACINGS_MAX                30038
#define     IDM_LINESPACINGMGR                  30039
#define   IDM_JUSTIFICATION                     3004
#define     IDM_JUST_LEFT                       30040
#define     IDM_JUST_CENTER                     30041
#define     IDM_JUST_RIGHT                      30042
#define     IDM_JUST_FULL                       30043
#define   IDM_PAGECENTER                        3005
#define   IDM_PAGEBREAK                         3006
#define   IDM_HEADER                            3008
#define     IDD_HEADER                          30080
#define     IDD_HEADER_ENTRY                    30081
#define     IDD_HEADER_APPLY                    30082
#define     IDD_HEADER_ERR                      30083
#define   IDM_FOOTER                            3009
#define     IDD_FOOTER                          30090
#define     IDD_FOOTER_ENTRY                    30091
#define     IDD_FOOTER_APPLY                    30091
#define     IDD_FOOTER_ERR                      30092

#define IDM_FONT_MENU                           40
#define   IDM_FONTS                             4000
#define   IDM_FONTS_MAX                         4996
#define   IDM_FONTS_PGUP                        4997
#define   IDM_FONTS_PGDN                        4998
#define   IDM_FONTMGR                           4999

#define IDM_SIZE_MENU                           50
#define   IDM_SIZES                             5000
#define   IDM_SIZES_MAX                         5996
#define   IDM_SIZES_PGUP                        5997
#define   IDM_SIZES_PGDN                        5998
#define   IDM_SIZEMGR                           5999

#define IDM_ATTR_MENU                           60
#define   IDM_NORMAL                            6000
#define   IDM_BOLD                              6001
#define   IDM_ITALIC                            6002
#define   IDM_UNDERSCORE                        6003
#define   IDM_STRIKEOUT                         6004
#define   IDM_OUTLINE                           6005
#define   IDM_CAPS                              6006
#define   IDM_EXPAND_MENU                       6100
#define     IDM_EXPANDS                           6101
#define     IDM_EXPANDS_MAX                       6196
#define     IDM_EXPANDS_PGUP                      6197
#define     IDM_EXPANDS_PGDN                      6198
#define     IDM_EXPANDMGR                         6199
#define   IDM_LINECOLOR_MENU                    6200
#define     IDM_LINECOLORS                        6201
#define     IDM_LINECOLORS_MAX                    6296
#define     IDM_LINECOLORS_PGUP                   6297
#define     IDM_LINECOLORS_PGDN                   6298
#define     IDM_LINECOLORMGR                      6299
#define   IDM_LINEWIDTH_MENU                    6300
#define     IDM_LINEWIDTHS                        6301
#define     IDM_LINEWIDTHS_MAX                    6396
#define     IDM_LINEWIDTHS_PGUP                   6397
#define     IDM_LINEWIDTHS_PGDN                   6398
#define     IDM_LINEWIDTHMGR                      6399
#define   IDM_FILLCOLOR_MENU                    6400
#define     IDM_FILLCOLORS                        6401
#define     IDM_FILLCOLORS_MAX                    6496
#define     IDM_FILLCOLORS_PGUP                   6497
#define     IDM_FILLCOLORS_PGDN                   6498
#define     IDM_FILLCOLORMGR                      6499
#define   IDM_FILLTYPE_MENU                     6500
#define     IDM_FILLTYPES                         6501
#define     IDM_FILLTYPES_MAX                     6596
#define     IDM_FILLTYPES_PGUP                    6597
#define     IDM_FILLTYPES_PGDN                    6598
#define     IDM_FILLTYPEMGR                       6599
#define   IDM_LINEEND_MENU                      6600
#define     IDM_LINEENDS                          6601
#define     IDM_LINEENDS_MAX                      6696
#define     IDM_LINEENDS_PGUP                     6697
#define     IDM_LINEENDS_PGDN                     6698
#define     IDM_LINEENDMGR                        6699
#define   IDM_LINEJOIN_MENU                     6700
#define     IDM_LINEJOINS                         6701
#define     IDM_LINEJOINS_MAX                     6796
#define     IDM_LINEJOINS_PGUP                    6797
#define     IDM_LINEJOINS_PGDN                    6798
#define     IDM_LINEJOINMGR                       6799

#define IDM_WINDOW_MENU                         70
#define   IDM_TILE                              7000
#define   IDM_CASCADE                           7001
#define   IDM_ARRANGEICONS                      7002
#define   IDM_WINDOWS                           7003
#define   IDM_WINDOWS_MAX                       7999

#define IDM_HELP_MENU                           80
#define   IDM_HELP_FOR_HELP                     800
#define   IDM_EXTENDED_HELP                     801
#define   IDM_KEYS_HELP                         802
#define   IDM_HELP_INDEX                        803
#define   IDM_ABOUT_MENU                        804
#define     IDM_PRODUCT_LIST                    8040
#define     IDM_PRODUCT_MAX                     8098
#define     IDM_ABOUT                           8040
#define       IDD_ABOUT                         8099

#define IDM_MAIN_CONTEXT                        9000

#define IDM_TEXTMENU                            9100

#define IDM_BITMAPMENU                          9200
#define   IDM_BITMAP_BACKGROUND                 9201
#define   IDM_BITMAP_NOTEXTASIDE                9202
#define   IDM_BITMAP_HALFTONE                   9203
#define   IDM_BITMAP_REVERSE                    9204
#define   IDM_BITMAP_OPTIMIZE                   9205
#define   IDM_BITMAP_POSITION                   9206
#define     IDM_BITMAP_POSITION_NEXT_CHAR       9207
#define     IDM_BITMAP_POSITION_NEXT_LINE       9208
#define     IDM_BITMAP_POSITION_EDGE_LEFT       9209 
#define     IDM_BITMAP_POSITION_EDGE_RIGHT      9210
#define     IDM_BITMAP_POSITION_EDGE_TOP        9211
#define     IDM_BITMAP_POSITION_EDGE_BOTTOM     9212
#define     IDM_BITMAP_POSITION_MARGIN_LEFT     9213
#define     IDM_BITMAP_POSITION_MARGIN_RIGHT    9214
#define     IDM_BITMAP_POSITION_MARGIN_TOP      9215
#define     IDM_BITMAP_POSITION_MARGIN_BOTTOM   9216
#define     IDM_BITMAP_POSITION_CENTERED_X      9217
#define     IDM_BITMAP_POSITION_CENTERED_Y      9218
#define   IDM_BITMAP_SIZE                       9219
#define     IDM_BITMAP_SIZE_EDGETOEDGE_X        9220
#define     IDM_BITMAP_SIZE_EDGETOEDGE_Y        9221
#define     IDM_BITMAP_SIZE_MARGINTOMARGIN_X    9222
#define     IDM_BITMAP_SIZE_MARGINTOMARGIN_Y    9223
#define   IDM_BITMAP_ASPECT                     9224
#define     IDM_BITMAP_ASPECT_FLOAT             9225
#define     IDM_BITMAP_ASPECT_FIX_X             9226
#define     IDM_BITMAP_ASPECT_FIX_Y             9227

#define IDM_BORDERMENU                          9300
#define   IDM_BORDER_SIZE                       9301
#define   IDM_BORDER_COLOR                      9302
#define   IDM_BORDER_CAPTION                    9304

#define IDM_DEFAULTMENU                         9400
#define   IDM_REFRESH                           9401

#define IDS_HELPTITLE                           0x0000
#define IDS_TITLE                               0x0001
#define IDS_TITLE1                              0x0002
#define IDS_NEW_DOCUMENT                        0x0003
#define IDS_TYPE                                0x0004
#define IDS_PAGE                                0x0005
#define IDS_OF                                  0x0006
#define IDS_MODIFIED                            0x0007
#define IDS_JOB_NAME                            0x0008
#define IDS_UNTITLED                            0x0009
#define IDS_DEFAULTFONT                         0x000A
#define IDS_STATUSFONT                          0x000B
#define IDS_PRINTING                            0x000C
#define IDS_LOADING                             0x000D
#define IDS_BOLD                                0x000E
#define IDS_ITALIC                              0x000F
#define IDS_ATTR_NORMAL                         0x0010
#define IDS_ATTR_BOLD                           0x0011
#define IDS_ATTR_ITALIC                         0x0012
#define IDS_ATTR_UNDERSCORE                     0x0013
#define IDS_ATTR_STRIKEOUT                      0x0014
#define IDS_ATTR_OUTLINE                        0x0015
#define IDS_ATTR_CAPS                           0x0016
#define IDS_UNITS_ENGLISH                       0x0017
#define IDS_UNITS_METRIC                        0x0018
#define IDS_VERIFY_ERROR1                       0x0019
#define IDS_VERIFY_ERROR2                       0x001A
#define IDS_VERIFY_ERROR3                       0x001B
#define IDS_VERIFY_ERROR4                       0x001C
#define IDS_VERIFY_ERROR5                       0x001D
#define IDS_VERIFY_ERROR6                       0x001E
#define IDS_VERIFY_ERROR7                       0x001F
#define IDS_VERIFY_ERROR8                       0x0020
#define IDS_IMPORT_ERROR1                       0x0021
#define IDS_IMPORT_ERROR2                       0x0022
#define IDS_EXPORT_ERROR1                       0x0023
#define IDS_EXPORT_ERROR2                       0x0024
#define IDS_MARGINS_PAGESIZE                    0x0025
#define IDS_MARGINS_ERROR0                      0x0026
#define IDS_MARGINS_ERROR1                      0x0027
#define IDS_MARGINS_ERROR2                      0x0028
#define IDS_MARGINS_ERROR3                      0x0029
#define IDS_MARGINS_ERROR4                      0x002A
#define IDS_MARGINS_ERROR5                      0x002B
#define IDS_MARGINS_ERROR6                      0x002C
#define IDS_TABS_MARGINS                        0x002D
#define IDS_TABS_ERROR1                         0x002E
#define IDS_TABS_ERROR2                         0x002F
#define IDS_TABS_ERROR3                         0x0030
#define IDS_TABS_ERROR4                         0x0031
#define IDS_TABS_ERROR5                         0x0032
#define IDS_TABS_ERROR6                         0x0033
#define IDS_INDENT_MARGINS                      0x0034
#define IDS_INDENT_ERROR1                       0x0035
#define IDS_INDENT_ERROR2                       0x0036
#define IDS_BARCODE_ERROR1                      0x0037
#define IDS_BARCODE_ERROR2                      0x0038
#define IDS_PRINTER_ERROR0                      0x0039
#define IDS_PRINTER_ERROR1                      0x003A
#define IDS_PRINTER_ERROR2                      0x003B
#define IDS_PRINTER_ERROR3                      0x003C
#define IDS_PRINTER_ERROR4                      0x003D
#define IDS_PRINTER_ERROR5                      0x003E
#define IDS_PRINTER_ERROR6                      0x003F 
#define IDS_PRINTER_ERROR7                      0x0040
#define IDS_PRINTER_ERROR8                      0x0041
#define IDS_PRINTER_ERROR9                      0x0042
#define IDS_DEFAULTS_PAGE_AUTHOR                0x0043
#define IDS_DEFAULTS_PAGE_PRINTER               0x0044
#define IDS_DEFAULTS_PAGE_FONT                  0x0045
#define IDS_DEFAULTS_PAGE_LAYOUT                0x0046
#define IDS_DEFAULTS_PAGE_NATIONAL              0x0047
#define IDS_DEFAULTS_METRIC1                    0x0048
#define IDS_DEFAULTS_ENGLISH1                   0x0049
#define IDS_DEFAULTS_METRIC2                    0x004A
#define IDS_DEFAULTS_ENGLISH2                   0x004B
#define IDS_DEFAULTS_ERROR1                     0x004C
#define IDS_DEFAULTS_ERROR2                     0x004D
#define IDS_ZOOM_ERROR1                         0x004E
#define IDS_ZOOM_ERROR2                         0x004F
#define IDS_SEARCH_ERROR1                       0x0050
#define IDS_SEARCH_ERROR2                       0x0051
#define IDS_SEARCH_ERROR3                       0x0052
#define IDS_SEARCH_ERROR4                       0x0053
#define IDS_HEADER_TOO_LARGE                    0x0054
#define IDS_HEADER_BAD_FUNCTION                 0x0055
#define IDS_ZOOM0                               0x0056
#define IDS_ZOOM1                               0x0057
#define IDS_PASTEGRAPHIC0                       0x0058
#define IDS_PASTEGRAPHIC1                       0x0059
#define IDS_MOVEGRAPHIC                         0x005A
#define IDS_SIZEGRAPHIC                         0x005B
#define IDS_MOVETEXT                            0x005C
#define IDS_DATEFMT_ERROR1                      0x005D
#define IDS_DATEFMT_HEADER                      0x005E
#define IDS_DATEFMT_YEAR2                       0x005F
#define IDS_DATEFMT_YEAR4                       0x0060 
#define IDS_DATEFMT_MON                         0x0061
#define IDS_DATEFMT_MONTH                       0x0062
#define IDS_DATEFMT_MONTHS                      0x0063
#define IDS_DATEFMT_DAY                         0x0064
#define IDS_DATEFMT_WEEKDAY                     0x0065
#define IDS_DATEFMT_WKDAY                       0x0066
#define IDS_DATEFMT_WEEKDAYS                    0x0067
#define IDS_DATEFMT_DAY0                        0x0068
#define IDS_DATEFMT_DAY1                        0x0069
#define IDS_DATEFMT_DAY2                        0x006A
#define IDS_DATEFMT_DAY3                        0x006B
#define IDS_DATEFMT_DAY4                        0x006C
#define IDS_DATEFMT_DAY5                        0x006D
#define IDS_DATEFMT_DAY6                        0x006E
#define IDS_DATEFMT_MONS                        0x006F
#define IDS_DATEFMT_MONTH1                      0x0070
#define IDS_DATEFMT_MONTH2                      0x0071
#define IDS_DATEFMT_MONTH3                      0x0072
#define IDS_DATEFMT_MONTH4                      0x0073
#define IDS_DATEFMT_MONTH5                      0x0074
#define IDS_DATEFMT_MONTH6                      0x0075
#define IDS_DATEFMT_MONTH7                      0x0076
#define IDS_DATEFMT_MONTH8                      0x0077
#define IDS_DATEFMT_MONTH9                      0x0078
#define IDS_DATEFMT_MONTH10                     0x0079
#define IDS_DATEFMT_MONTH11                     0x007A
#define IDS_DATEFMT_MONTH12                     0x007B
#define IDS_SAVING                              0x007C 
#define IDS_MERGING                             0x007D
#define IDS_OPEN_DOCUMENT                       0x007E
#define IDS_MERGE_DOCUMENT                      0x007F
#define IDS_SAVE_DOCUMENT                       0x0080
#define IDS_OPEN_GRAPHIC                        0x0081
#define IDS_SELECTED_FORM                       0x0082
#define IDS_POINT                               0x0083
#define IDS_REGISTERED_TO                       0x0084
#define IDS_SERIAL_NUMBER                       0x0085
#define IDS_MORE_PGUP                           0x0086
#define IDS_MORE_PGDN                           0x0087
#define IDS_MANAGER                             0x0088
#define IDS_EXPAND                              0x0089
#define IDS_LOW_MEMORY                          0x008A
#define IDS_CHANGEDATE                          0x008B
#define IDS_CURRENTDATE                         0x008C
#define IDS_GRAPHIC                             0x008D
#define IDS_FUNCTION                            0x008E
#define IDS_ENDDOCUMENT                         0x008F
#define IDS_ENDPAGE                             0x0090 
#define IDS_ENDLINE                             0x0091
#define IDS_TAB                                 0x0092
#define IDS_SPACE                               0x0093
#define IDS_HYPHEN                              0x0094
#define IDS_CHAR                                0x0095
#define IDS_CURSOR                              0x0096
#define IDS_DORMANT                             0x0097
#define IDS_ON                                  0x0098
#define IDS_OFF                                 0x0099
#define IDS_INLINE                              0x009A
#define IDS_LEFTEDGE                            0x009B
#define IDS_RIGHTEDGE                           0x009C
#define IDS_BOTTOMEDGE                          0x009D
#define IDS_TOPEDGE                             0x009E
#define IDS_LEFTMARGIN                          0x009F
#define IDS_RIGHTMARGIN                         0x00A0
#define IDS_BOTTOMMARGIN                        0x00A1
#define IDS_TOPMARGIN                           0x00A2
#define IDS_CENTERED                            0x00A3
#define IDS_EDGETOEDGE                          0x00A4 
#define IDS_MARGINTOMARGIN                      0x00A5
#define IDS_BITMAP                              0x00A6
#define IDS_METAFILE                            0x00A7
#define IDS_UPCA                                0x00A8
#define IDS_UPCE                                0x00A9
#define IDS_CODE39                              0x00AA
#define IDS_POSTNET                             0x00AB
#define IDS_EAN8                                0x00AC
#define IDS_EAN13                               0x00AD
#define IDS_ALL_DOCUMENTS                       0x00AE
#define IDS_ALL_GRAPHICS                        0x00AF
#define IDS_WINDOW_BY_ME_NODATE                 0x00B0
#define IDS_WINDOW_BY_ME                        0x00B1 
#define IDS_WINDOW_BY_OTHER                     0x00B2
#define IDS_GOTOPAGE_ERROR1                     0x00B3
                                                      
#define IDS_METAFILE_VERIFY1                    0x0200
#define IDS_METAFILE_VERIFY2                    0x0201
#define IDS_METAFILE_VERIFY3                    0x0202
#define IDS_METAFILE_VERIFY4                    0x0203
#define IDS_METAFILE_VERIFY5                    0x0204
#define IDS_METAFILE_VERIFY6                    0x0205

#define IDS_FN_VERSION                          0x1000
#define IDS_FN_PREFACE                          0x1001
#define IDS_FN_PRINTER                          0x1002
#define IDS_FN_FORM                             0x1003
#define IDS_FN_CODEPAGE                         0x1004
#define IDS_FN_FONT                             0x1005
#define IDS_FN_SIZE                             0x1006
#define IDS_FN_COLOR                            0x1007
#define IDS_FN_FILLCOLOR                        0x1008
#define IDS_FN_BOLD                             0x1009
#define IDS_FN_ITALIC                           0x100A
#define IDS_FN_UNDERSCORE                       0x100B
#define IDS_FN_STRIKEOUT                        0x100C
#define IDS_FN_OUTLINE                          0x100D
#define IDS_FN_CAPS                             0x100E
#define IDS_FN_EXPAND                           0x100F
#define IDS_FN_TOPMARGIN                        0x1010
#define IDS_FN_BOTTOMMARGIN                     0x1011
#define IDS_FN_LEFTMARGIN                       0x1012
#define IDS_FN_RIGHTMARGIN                      0x1013
#define IDS_FN_LEFTJUSTIFY                      0x1014
#define IDS_FN_CENTERJUSTIFY                    0x1015
#define IDS_FN_RIGHTJUSTIFY                     0x1016
#define IDS_FN_FULLJUSTIFY                      0x1017
#define IDS_FN_CENTERPAGE                       0x1018
#define IDS_FN_TABSET                           0x1019
#define IDS_FN_TABCLEAR                         0x101A
#define IDS_FN_TABSPACING                       0x101B
#define IDS_FN_INDENT                           0x101C
#define IDS_FN_METRIC                           0x101D
#define IDS_FN_GRAPHIC                          0x101E
#define IDS_FN_DATEFIXED                        0x101F
#define IDS_FN_DATECURRENT                      0x1020
#define IDS_FN_DATECHANGED                      0x1021
#define IDS_FN_DATEFORMAT                       0x1022
#define IDS_FN_LINESPACING                      0x1023
#define IDS_FN_JOBSETTINGS                      0x1024
#define IDS_FN_PAGENUMBER                       0x1025
#define IDS_FN_PAGECOUNT                        0x1026
#define IDS_FN_LINETYPE                         0x1027
#define IDS_FN_LINEWIDTH                        0x1028
#define IDS_FN_LINEEND                          0x1029
#define IDS_FN_LINEJOIN                         0x102A
#define IDS_FN_FILLTYPE                         0x102B
#define IDS_FN_GLYPH                            0x102C
#define IDS_FN_AUTHOR                           0x102D
#define IDS_FN_CREATEDATE                       0x102E
#define IDS_FN_COMMENT                          0x102F
#define IDS_FN_SEPARATOR                        0x1030
#define IDS_FN_TITLE                            0x1031
#define IDS_FN_HEADER                           0x1032
#define IDS_FN_FOOTER                           0x1033

#define IDS_PARMS_RESET                         0x2000
#define IDS_PARMS_PRINT                         0x2001
#define IDS_PARMS_TRAP                          0x2002
#define IDS_PROMPT_SAVE                         0x2003
#define IDS_PROMPT_SAVECONFIRM1                 0x2004
#define IDS_PROMPT_SAVECONFIRM2                 0x2005
#define IDS_MESSAGE_SPELLDONE                   0x2006
#define IDS_ERROR_EXCEPTION                     0x2007
#define IDS_ERROR_PRFOPENPROFILE                0x2008
#define IDS_ERROR_CREATEHELP                    0x2009
#define IDS_ERROR_CREATEFRAME                   0x200A
#define IDS_ERROR_WINREGISTERCLASS              0x200B
#define IDS_ERROR_CREATECLIENT                  0x200C
#define IDS_ERROR_TOOMANYDOCUMENTS              0x200D
#define IDS_ERROR_TOOMANYLOADED                 0x200E
#define IDS_ERROR_DOCUMENT_CREATE               0x200F
#define IDS_ERROR_DOCUMENT_LOAD                 0x2010
#define IDS_ERROR_DOCUMENT_SAVE                 0x2011
#define IDS_ERROR_DOCUMENT_NONAME               0x2012
#define IDS_ERROR_CREATEPRINTSTATUS             0x2013
#define IDS_ERROR_CREATELOADSTATUS              0x2014
#define IDS_ERROR_BLOCKMEMORY                   0x2015
#define IDS_ERROR_BADGRAPHICFILE                0x2016
#define IDS_ERROR_SPELL_REPLACE                 0x2017
#define IDS_ERROR_FILEMEMORY                    0x2018
#define IDS_ERROR_NOFILETYPE                    0x2019
#define IDS_ERROR_INVALIDPATH                   0x201A
#define IDS_ERROR_NOPATH                        0x201B
#define IDS_ERROR_LOADLANGUAGE                  0x201C

#define IDS_WARNING_BADCODEPAGE                 0x3000
#define IDS_WARNING_DOSQUERYCP                  0x3001
#define IDS_WARNING_CODEPAGENOTACTIVE           0x3002

#define IDS_MESSAGE_BOX_STRINGS                 0x4000
#define IDS_OK                                  0x4001
#define IDS_CANCEL                              0x4002
#define IDS_ABORT                               0x4003
#define IDS_RETRY                               0x4004
#define IDS_IGNORE                              0x4005
#define IDS_YES                                 0x4006
#define IDS_NO                                  0x4007
#define IDS_HELP                                0x4008
#define IDS_ENTER                               0x4009

#define IDS_FDLG_FILENAME_TXT1                  0x4100
#define IDS_FDLG_FILENAME_TXT2                  0x4101
#define IDS_FDLG_DRIVE_TXT                      0x4102
#define IDS_FDLG_FILTER_TXT                     0x4103
#define IDS_FDLG_DIRECTORY_TXT                  0x4104
#define IDS_FDLG_FILES_TXT                      0x4105
#define IDS_FDLG_HELP_PB                        0x4106
#define IDS_FDLG_APPLY_PB                       0x4107
#define IDS_FDLG_OK_PB1                         0x4108
#define IDS_FDLG_OK_PB2                         0x4109
#define IDS_FDLG_CANCEL_PB                      0x410A

#define IDS_CODEPAGES                           0x7000

#define IDS_HMERR                               0x8000
#define IDS_HMERR_EXTHELPUNDEFINED              0x8001
#define IDS_HMERR_NO_FRAME_WND_IN_CHAIN         0x8002
#define IDS_HMERR_INVALID_ASSOC_APP_WND         0x8003
#define IDS_HMERR_INVALID_ASSOC_HELP_IN         0x8004
#define IDS_HMERR_INVALID_DESTROY_HELP_         0x8005
#define IDS_HMERR_NO_HELP_INST_IN_CHAIN         0x8006
#define IDS_HMERR_INVALID_HELP_INSTANCE         0x8007
#define IDS_HMERR_INVALID_QUERY_APP_WND         0x8008
#define IDS_HMERR_HELP_INST_CALLED_INVA         0x8009
#define IDS_HMERR_HELPTABLE_UNDEFINE            0x800A
#define IDS_HMERR_HELP_INSTANCE_UNDEFIN         0x800B
#define IDS_HMERR_HELPITEM_NOT_FOUND            0x800C
#define IDS_HMERR_INVALID_HELPSUBITEM_S         0x800D
#define IDS_HMERR_HELPSUBITEM_NOT_FOUND         0x800E
#define IDS_HMERR_INDEX_NOT_FOUND               0x800F
#define IDS_HMERR_CONTENT_NOT_FOUND             0x8010
#define IDS_HMERR_OPEN_LIB_FILE                 0x8011
#define IDS_HMERR_READ_LIB_FILE                 0x8012
#define IDS_HMERR_CLOSE_LIB_FILE                0x8013
#define IDS_HMERR_INVALID_LIB_FILE              0x8014
#define IDS_HMERR_NO_MEMORY                     0x8015
#define IDS_HMERR_ALLOCATE_SEGMENT              0x8016
#define IDS_HMERR_FREE_MEMORY                   0x8017
#define IDS_HMERR_PANEL_NOT_FOUND               0x8018
#define IDS_HMERR_DATABASE_NOT_OPEN             0x8019
#define IDS_HMERR_UNKNOWN                       0x801A
#define IDS_HELPMODE_FRAME                      0x801B
#define IDS_HELPMODE_MENU                       0x801C
#define IDS_HELPMODE_WINDOW                     0x801D
#define IDS_HELPMODE_UNKNOWN                    0x801E
#define IDS_HELPSUBITEMNOTFOUND                 0x801F

#endif
