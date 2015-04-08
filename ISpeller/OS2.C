#include "Debug.h"

#define INCL_BASE
#include <os2.h>

#include <direct.h>
#include <io.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

extern char *index ( const char *string, int character );

extern char *index ( const char *string, int character )
{
   return ( ( char * ) strchr ( string, character ) );
}

extern int link ( const char *existing, const char *new )
{
   char Command[256];

   strcpy ( Command, "MOVE " );
   strcat ( Command, existing );
   strcat ( Command, " " );
   strcat ( Command, new );
   if ( system ( Command ) )
      return ( 0 );
   return ( 1 );
}

extern char *mktemp ( char *prototype )
{
   return ( tmpnam ( prototype ) );
}

extern unsigned int sleep ( unsigned int seconds )
{
   DosSleep ( seconds * 1000 );
   return ( 0 );
}

extern void Log ( char *Message,... )
{
   FILE *File = fopen ( "log", "a" );

   if ( File ) {
      char Time[9], Date[9];
      va_list Marker;

      fprintf ( File, "%s %s %02i ", _strtime ( Time ), _strdate ( Date ), *_threadid );
      va_start ( Marker, Message );
      vfprintf ( File, Message, Marker );
      va_end ( Marker );
      fprintf ( File, "\n" );
      fclose ( File );
   }                            /* endif */
}
