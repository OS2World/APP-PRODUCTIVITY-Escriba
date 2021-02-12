/**************************************************************** REGISTER.C
 *                                                                         *
 *                       Create Registered DLLs.                           *
 *                                                                         *
 ***************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static unsigned short UpdateCRC16 ( unsigned short Crc, unsigned char Byte ) ;

extern void main ( void ) {

  static struct {
    char Name [80] ;
    char Nickname [9] ;
    long Serial ;
  } Clients [] = {

    // Paid Clients
//  { "Charles E. Hilton",		"Hilton",    1L },
//  { "Brian E. Ferguson",		"Ferguson",  2L },
//  { "Mark R. Kaehny", 		"Kaehny",    3L },
//  { "Ernst W. Winter",		"Winter",    4L },
//  { "Dr. Robert Raven",		"Raven",     5L },
//  { "Mark Nipper",			"Nipper",    6L },
//  { "William T. Quick",		"Quick",     7L },
//  { "Romeo Dais",			"Dais",      8L },
//  { "Christopher L. Nagle, PhD",	"Nagle",     9L },
//  { "Robert W. Pasco",		"Pasco",    10L },
//  { "Thomas V. Schill",		"Schill",   11L },
//  { "Ray Walsh",			"Walsh",    12L },
//  { "T. J. Collinson",		"Collinso", 13L },
//  { "Lars Ferguson",			"LFerguso", 14L },
//  { "Leland C. Sheppard",		"Sheppard", 15L },
//  { "Fumihiro Kaneko",		"Kaneko",   16L },
//  { "Larry R. Nielsen",		"Nielsen",  17L },
//  { "Joseph I. Valenzuela",		"Valenz",   18L },
//  { "Russel H. Patterson, Jr., M.D.", "Patterso", 19L },
//  { "Dr. W. Paul Jones",		"WPJones",  20L },
//  { "Peter Rodriguez",		"PRodrig",  21L },
//  { "Patrick Plante", 		"PPlante",  22L },
//  { "Gorm J›rgensen", 		"GJorgen",  23L },
//  { "Glenn I. Gundersen",		"GGunder",  24L },
//  { "Pierre Jovanovic",		"PJovano",  25L },
//  { "Louis K. C. Chan",		"LKCChan",  26L },
//  { "Glenn R. Williams",		"Williams", 27L },
//  { "R. R. Ronkin",			"Ronkin",   28L },
//  { "Kris Steenhaut", 		"Steenhau", 29L },
//  { "Mark Dodel",             	"MDodel",   30L },
    { "Tony Pater",             	"TPater",   31L },

    // Free Distribution
//  { "Rick Papo",		  "Papo",     100000L },  // Me
//  { "Great Lakes Data Systems", "GLDS",     100001L },  // Sponsor
//  { "Jaap van Veen",		  "VanVeen",  100002L },  // Dutch
//  { "Harry Bush",		  "Bush",     100003L },  // Beta Tester
//  { "Xavier Caball‚ Micola",    "Caballe",  100004L },  // Spanish/Catalan
//  { "Bjorn Fahller",            "Fahller",  100005L },  // Beta Tester
//  { "Per Gjerlov",              "Gjerlov",  100006L },  // Beta Tester
//  { "Herbert Klaeren",          "Klaeren",  100007L },  // Beta Tester
//  { "Frank Mersmann",           "Mersmann", 100008L },  // German
//  { "Joseph Negron",            "Negron",   100009L },  // Beta Tester
//  { "Marvin Nipper",            "Nipper",   100010L },  // Beta Tester
//  { "Raphael Vanney", 	  "Vanney",   100013L },  // French
//  { "Bryan Walker",             "Walker",   100014L },  // Beta Tester
//  { "Anders Gjerlov", 	  "AGjerlov", 100015L },  // Danish?
  } ;

  short Crc ;
  FILE *File ;
  unsigned i, j ;

  File = fopen ( "ESCRIBAR", "w" ) ;
  fprintf ( File, "ESCRIBAR.DLL : $*.DEF $*.OBJ\n" ) ;
  fprintf ( File, "  LINK386 /NOLOGO $*, $*.dll /align:16, nul,/NOD:LLIBCE LLIBCEP, $* ;\n\n" ) ;
  fprintf ( File, "ESCRIBAR.OBJ : $*.C\n" ) ;
  fprintf ( File, "  cl /c /nologo /AL $*.C\n\n" ) ;
  fclose ( File ) ;

  File = fopen ( "ESCRIBAR.DEF", "w" ) ;
  fprintf ( File, "LIBRARY ESCRIBAR\n" ) ;
  fprintf ( File, "EXPORTS\n" ) ;
  fprintf ( File, "  _Name\n" ) ;
  fprintf ( File, "  _Serial\n" ) ;
  fprintf ( File, "  _Check\n" ) ;
  fclose ( File ) ;

  for ( i=0; i<sizeof(Clients)/sizeof(Clients[0]); i++ ) {
    printf ( "Generating package for %s (%06li).\n", Clients[i].Name, Clients[i].Serial ) ;

    Crc = 0 ;
    for ( j=0; j<strlen(Clients[i].Name); j++ ) {
      Crc = UpdateCRC16 ( Crc, Clients[i].Name[j] ) ;
    }
    Crc = UpdateCRC16 ( Crc, *(((unsigned char*)&Clients[i].Serial)+0) ) ;
    Crc = UpdateCRC16 ( Crc, *(((unsigned char*)&Clients[i].Serial)+1) ) ;
    Crc = UpdateCRC16 ( Crc, *(((unsigned char*)&Clients[i].Serial)+2) ) ;
    Crc = UpdateCRC16 ( Crc, *(((unsigned char*)&Clients[i].Serial)+3) ) ;

    File = fopen ( "ESCRIBAR.C", "w" ) ;
    fprintf ( File, "char far *Name = \"%s\" ;\n", Clients[i].Name ) ;
    fprintf ( File, "unsigned long Serial = %luL ;\n", Clients[i].Serial ) ;
    fprintf ( File, "unsigned short Check = %u ;\n", Crc ) ;
    fclose ( File ) ;

    File = fopen ( "ESCRIBAR.CMD", "w" ) ;
    fprintf ( File, "@Echo Off\n" ) ;
    fprintf ( File, "SET INCLUDE=E:\\BUILD\\INCLUDE\\SYS;\n" ) ;
    fprintf ( File, "SET LIB=E:\\BUILD\\LIB\\SYS;\n" ) ;
    fprintf ( File, "NMAKE /NOLOGO /F ESCRIBAR\n" ) ;
    fprintf ( File, "PKZIP -o %s ESCRIBAR.DLL REGISTER.DOC\n", Clients[i].Nickname ) ;
    fclose ( File ) ;

    system ( "ESCRIBAR" ) ;

    printf ( "\n\n" ) ;
  }

  system ( "Erase Escribar.*" ) ;
}


/***************************************************************************
 *                                                                         *
 *  Update CRC-16 with next byte in block to be verified.                  *
 *                                                                         *
 ***************************************************************************/

static unsigned short UpdateCRC16 ( unsigned short Crc, unsigned char Byte ) {

  register unsigned short r4, r5, r6, r7 ;

  r4 = Byte ;
  r5 = Crc ;

  r5 ^= r4 ;
  r6 = r5 << 8 ;
  r5 = r5 >> 8 ;
  r7 = r6 ;
  r7 = r7 << 1 ;
  r7 ^= r6 ;
  r6 = r7 ;
  r7 = r7 << 2 ;
  r7 ^= r6 ;
  r6 = r7 ;
  r7 = r7 << 4 ;
  r7 ^= r6 ;
  r5 ^= r7 ;
  r7 = r7 >> 2 ;
  r5 ^= r7 ;
  r7 = r7 >> 13 ;
  r5 ^= r7 ;

  return ( r5 ) ;
}
