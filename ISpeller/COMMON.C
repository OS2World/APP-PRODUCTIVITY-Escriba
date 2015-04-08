#include "Debug.h"

#define MAIN

#include "config.h"
#include "ispell.h"
#include "proto.h"
#include "msgs.h"
#include "version.h"

/* ARGSUSED */
VOID *myrealloc ( ptr, size, oldsize )
   VOID *ptr;
   unsigned int size;
   unsigned int oldsize;
{

   return realloc ( ptr, size );
}
