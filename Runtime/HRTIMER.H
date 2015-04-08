/**********************************************************************
 * MODULE NAME :  hrtimer.h              AUTHOR:  Rick Fishman        *
 * DATE WRITTEN:  11-24-91                                            *
 *                                                                    *
 * DESCRIPTION:                                                       *
 *                                                                    *
 *  Contains definitions for use with HRTIMER.SYS.                    *
 *                                                                    *
 **********************************************************************/

#include "System.h"

typedef struct _TIMESTAMP
{
    ULONG   ulMs;
    ULONG   ulNs;
    ULONG   ulVersion;

} TIMESTAMP, *PTIMESTAMP;

