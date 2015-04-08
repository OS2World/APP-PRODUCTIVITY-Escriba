/* ISPELLER User API functions */

#ifndef ISPELLER_H
#define ISPELLER_H


 /* Add-on extension type identifiers. */

#define TYPE_ISPELLER "ISPELLER"


 /* Standard functions for all add-on extensions. */

extern char *APIENTRY Identify ( void );        /* Identify extension
                                                 * type.                     */
extern char *APIENTRY ProductName ( void );     /* Give module name/title.                      */
extern int APIENTRY ProductVersion ( void );    /* Give version number x
                                                 * 100.                   */
extern void APIENTRY ProductInformation ( HWND Owner ); /* Display product
                                                         * information/copyright
                                                         * dialog. */
extern int APIENTRY ProductConfigure    /* Provide configuration notebook
                                         * page info.    */
 ( int MaxPages, char Names[][64], char MajorTabs[][32], char MinorTabs[][32], PFNWP Processors[], HMODULE Libraries[], int DialogIDs[] );
extern BOOL APIENTRY SetLanguage ( char *Language );    /* Set language for
                                                         * menus/dialogs/texts.        */


 /* Spell-Checker Functions.                     */

extern void APIENTRY Speller_SetHome ( char *Path );
extern char *APIENTRY Speller_DefaultDictionary ( void );
extern int APIENTRY Speller_OpenDictionary ( char *Path );
extern void APIENTRY Speller_CloseDictionary (  );
extern BOOL APIENTRY Speller_Ready (  );
extern int APIENTRY Speller_BuildWord ( char *( *APIENTRY GetPtr ) ( void *Object, long *Offset, long Delta ), void *Object, char *Word, long *Start, long *End, long *Next );
extern int APIENTRY Speller_FindWord ( char *Word );
extern int APIENTRY Speller_FindReplacements ( char *Word, int ListSize, int EntrySize, void *List, int *EasyCount );
extern int APIENTRY Speller_Accept ( char *Word );
extern int APIENTRY Speller_Insert ( char *Word );
extern int APIENTRY Speller_UnCaps ( char *Word );


#ifdef LOCAL_DEFINES

 /* Identification */

#define REVISION     "ISpeller 0.96d"
#define REVISION_NO  (96)


 /* Local Resource Identifiers */

#define ID_HELPTABLE                            1

#define IDD_PRODUCTINFO                         1
#define IDD_CONFIGURE                           2
#define   IDD_CONFIGURE_DICT                    20
#define   IDD_CONFIGURE_BUILD                   21
#define   IDD_CONFIGURE_REMOVE                  22
#define IDD_BUILD                               3
#define   IDD_BUILD_WORDS                       30
#define   IDD_BUILD_AFFIX                       31
#define   IDD_BUILD_DICT                        32
#define   IDD_BUILD_WORDSF                      33
#define   IDD_BUILD_AFFIXF                      34
#define   IDD_BUILD_ERR                         35

#define IDS_TITLE                               1
#define IDS_TITLE1                              2
#define IDS_HELPTITLE                           3
#define IDS_ERROR_CREATEHELP                    4
#define IDS_MAJORTAB                            5
#define IDS_DICTIONARY                          6
#define IDS_BUILD_WORDS                         7
#define IDS_BUILD_AFFIX                         8
#define IDS_BUILD_DICT                          9
#define IDS_ERROR_CONFIGURE_NODICT              10
#define IDS_ERROR_CONFIGURE_BADDICT             11
#define IDS_ERROR_CONFIGURE_MUSTSELECT          12
#define IDS_ERROR_BUILD_NOTAVAIL                13
#define IDS_ERROR_BUILD_NOWORDLIST              14
#define IDS_ERROR_BUILD_NOAFFIX                 15
#define IDS_ERROR_BUILD_NODICT                  16
#define IDS_ERROR_BUILD_NOBUILDER               17
#define IDS_ERROR_BUILD_BUILDERR                18

#endif          /* LOCAL_DEFINES */

#endif          /* ISPELLER_H */
