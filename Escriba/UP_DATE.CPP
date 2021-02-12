// This program serves to automatically update the date-stamps in selected files.
// For the moment, all dates are expected to be in 'dd MMM yyyy' format, which 
// of course does not work in the general case, though it works fine for the
// languages currently supported.

#define INCL_BASE
#include <os2.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

struct Language {
   char *Name ;
   char *Abbrev ;
   char *Months [12] ;
} ;

static Language English  = { "English",  "_ENU", 
   { "January", "February", "March", "April", "May",  "June",  "July",    "August",   "September", "October", "November",  "December"  } } ;
static Language Espanol  = { "Espanol",  "_ESP", 
   { "enero",   "febrero",  "marzo", "abril", "mayo", "junio", "julio",   "agosto",   "setiembre", "octubre", "noviembre", "diciembre" } } ;
static Language Deutsch  = { "Deutsch",  "_DEU", 
   { "Januar",  "Februar",  "M„rz",  "April", "Mai",  "Juni",  "Juli",    "August",   "September", "Oktober", "November",  "Dezember"  } } ;
static Language Francais = { "Francais", "_FRA", 
   { "janvier", "f‚vrier",  "mars",  "avril", "mai",  "juin",  "juillet", "ao–t",     "septembre", "octobre", "novembre",  "d‚cembre"  } } ;
static Language NdrLands = { "NdrLands", "_NLD", 
   { "januari", "februari", "maart", "april", "mei",  "juni",  "juli",    "augustus", "september", "october", "november",  "december"  } } ;

static Language *Languages [] = { &English, &Espanol, &Deutsch, &Francais, &NdrLands } ;

static char *List [] = {
// "*.doc", // This entry gets special treatment: only the last date in the file gets changed.
   "*.me",
   "*.diz",
   "deutsch\\*.rc", 
   "english\\*.rc", 
   "espanol\\*.rc", 
   "francais\\*.rc", 
   "ndrlands\\*.rc",
   "addons\\bmp\\*.rc", 
   "addons\\gif\\*.rc", 
   "addons\\html\\*.rc", 
   "addons\\jpeg\\*.rc", 
   "addons\\pcx\\*.rc", 
   "addons\\ptext\\*.rc", 
   "addons\\rtf\\*.rc", 
   "addons\\tiff\\*.rc",
   "..\\ispeller\\*.diz", 
   "..\\ispeller\\*.rc", 
   "..\\ispeller\\*.me"
} ;

extern void main ( void ) {
   for ( int i=0; i<sizeof(List)/sizeof(List[0]); i++ ) {
      char Drive [_MAX_DRIVE+1] ;
      char Dir [_MAX_DIR+1] ;
      char FName [_MAX_FNAME+1] ;
      char Ext [_MAX_EXT+1] ;
      _splitpath ( List[i], Drive, Dir, FName, Ext ) ;
      HDIR hDir ( (HDIR) HDIR_CREATE ) ;
      FILEFINDBUF3 Found ;
      ULONG Count ( 1 ) ;
      if ( !DosFindFirst ( (PSZ)List[i], &hDir, FILE_NORMAL, &Found, sizeof(Found), &Count, FIL_STANDARD ) ) {
         do {
            Language *pLanguage = &English ;
            for ( int j=0; j<sizeof(Languages)/sizeof(Languages[0]); j++ ) {
               strupr ( Found.achName ) ;
               char Name [80] ; strcpy ( Name, Languages[j]->Name ) ; strupr ( Name ) ;
               char Abbrev [10] ; strcpy ( Abbrev, Languages[j]->Abbrev ) ; strupr ( Abbrev ) ;
               if ( strstr ( Found.achName, Name ) ) {
                  pLanguage = Languages [j] ;
                  break ;
               } /* endif */
               if ( strstr ( Found.achName, Abbrev ) ) {
                  pLanguage = Languages [j] ;
                  break ;
               } /* endif */
            } /* endfor */
            char Path [_MAX_PATH] ; strcpy ( Path, Drive ) ; strcat ( Path, Dir ) ; strcat ( Path, Found.achName ) ;
            FILE *InputFile = fopen ( Path, "r" ) ;
            if ( InputFile ) {
               FILE *TempFile = fopen ( "UP_DATE.TMP", "w" ) ;
               if ( TempFile ) {
                  int Changed ( FALSE ) ;
                  char Buffer [500] ;
                  while ( fgets ( Buffer, sizeof(Buffer), InputFile ) ) {
                     for ( int k=0; k<12; k++ ) 
                        if ( strstr ( Buffer, pLanguage->Months[k] ) ) 
                           break ;
                     if ( k >= 12 ) {
                        fprintf ( TempFile, "%s", Buffer ) ;
                        continue ;
                     } /* endif */
                     char *pstart = strstr ( Buffer, pLanguage->Months[k] ) ;
                     char *pend = pstart + strlen(pLanguage->Months[k]) ;
                     while ( *pend && ( *pend == ' ' ) ) pend ++ ;
                     int Year = atoi ( pend ) ;
                     if ( ( Year < 1990 ) || ( Year > 2100 ) ) {
                        fprintf ( TempFile, "%s", Buffer ) ;
                        continue ;
                     } /* endif */
                     while ( *pend && ( *pend >= '0' ) && ( *pend <= '9' ) ) pend ++ ;
                     while ( ( pstart > Buffer ) && ( *(pstart-1) == ' ' ) ) pstart -- ;
                     while ( ( pstart > Buffer ) && ( *(pstart-1) >= '0' ) && ( *(pstart-1) <= '9' ) ) pstart -- ;
                     time_t Time = time ( 0 ) ;
                     struct tm Date = * localtime ( &Time ) ;
                     char NewDate [80] ;
                     sprintf ( NewDate, "%02i %s %04i", Date.tm_mday, pLanguage->Months[Date.tm_mon], Date.tm_year+1900 ) ;
                     printf ( "Updating '%s'.  \"%0.*s\" -> \"%s\"\n", Path, pend-pstart, pstart, NewDate ) ;
                     fprintf ( TempFile, "%0.*s", pstart-Buffer, Buffer ) ;
                     fprintf ( TempFile, "%s", NewDate ) ;
                     fprintf ( TempFile, "%s", pend ) ;
                     Changed = TRUE ;
                  } /* endwhile */
                  fclose ( TempFile ) ;
                  fclose ( InputFile ) ;
                  if ( Changed ) {
                     DosCopy ( (PSZ)"UP_DATE.TMP", (PSZ)Path, DCPY_EXISTING ) ;
                     remove ( "UP_DATE.TMP" ) ;
                  } /* endif */
               } else {
                  fclose ( InputFile ) ;
                  fprintf ( stderr, "ERROR: Unable to open temporary file.\n" ) ;
               } /* endif */
            } else {
               fprintf ( stderr, "ERROR: Unable to open file '%s'.\n", Path ) ;
            } /* endif */
         } while ( !DosFindNext ( hDir, &Found, sizeof(Found), &Count ) ); /* enddo */
         DosFindClose ( hDir ) ;
      } /* endif */
   } /* endfor */
}
