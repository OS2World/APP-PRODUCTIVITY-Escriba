/********************************************************************* MAIN.H
 *                                                                          *
 *                        Main Window Processor                             *
 *                                                                          *
 ****************************************************************************/

#ifndef MAIN_H
#define MAIN_H

#include "System.h"

#include "Module.h"
#include "Process.h"
#include "Profile2.h"
#include "Speller.h"

typedef struct {
   short       Filler ;
   LanguageModule *Library ;
   Module     *Registration ;
   Profile2   *IniFile ;
   INIDATA    *IniData ;
   Dictionary *Speller ;
   char       *InitialPath ;
   int         argc ;
   char      **argv ;
   BOOL        Trap ;
} PARMS, *PPARMS ;

extern WINDOWPROC MessageProcessor ;

#endif
