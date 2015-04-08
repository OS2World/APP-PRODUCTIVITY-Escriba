/******************************************************************* THREAD.H
 *                                                                          *
 *                       Thread Starter Function                            *
 *                                                                          *
 ****************************************************************************/

#ifndef THREAD_H
#define THREAD_H

#include "System.h"

extern TID StartThread ( char *Name, void(*Function)(void*), int StackSize, void *Parameter, int Class=PRTYC_NOCHANGE, int Delta=0 ) ;

#endif
