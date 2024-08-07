#include "fout.h"

#ifndef _INCLUDED_PERR_H_
#define _INCLUDED_PERR_H_

#define PERR_MSGLEN         1024

#define PERR_STREAM         stderr

#define PERR_SUCCESS        EXIT_SUCCESS
#define PERR_FAILURE        EXIT_FAILURE
#define PERR_NOEXIT         -1

#define PERR_W_NULLFMT      1

#define PERR_OK             0

#define _PERR_MALLOC        -1
#define _PERR_REALLOC       -2
#define _PERR_FOPEN         -3
#define _PERR_POPEN         -4
#define _PERR_STAT          -5

#define PERR_BASE   -10
#ifdef  PERR_BASE
#if     PERR_BASE > PERR_OK
#error  PERR_BASE must be <= PERR_OK
#endif
#else
#define PERR_BASE           PERR_OK
#endif

#define PERR_MALLOC         (PERR_BASE + _PERR_MALLOC)
#define PERR_REALLOC        (PERR_BASE + _PERR_REALLOC)
#define PERR_FOPEN          (PERR_BASE + _PERR_FOPEN)
#define PERR_POPEN          (PERR_BASE + _PERR_POPEN)
#define PERR_STAT           (PERR_BASE + _PERR_STAT)

extern  char                *(*__fout_msg)(int);

void    __perr_init         (void) __attribute__ ((constructor));
char    *perr_msg           (int);
int     fperr               (FILE *, int, char *, ...);
int     perr                (FILE *, int, int);

#include "../c/perr.c"

#endif
