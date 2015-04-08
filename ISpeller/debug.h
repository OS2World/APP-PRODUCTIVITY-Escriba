#ifndef DEBUG_H
#define DEBUG_H
#if defined(__IBMC__) || defined(__IBMCPP__)
#ifdef __DEBUG_ALLOC__
#pragma strings(readonly)
#include <stdlib.h>
#endif          /* __DEBUG_ALLOC__ */
#endif          /* __IBMCPP__ */
#endif          /* DEBUG_H */
