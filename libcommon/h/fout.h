#include "ansi.h"

#ifndef _INCLUDED_FOUT_H_
#define _INCLUDED_FOUT_H_

#include <errno.h>
#include <stdarg.h>
#include <unistd.h>

#define FOUT_OK             0

#define FOUT_W_NULLFMT      1
#define FOUT_W_PARAMLEN     2
#define FOUT_W_STRINGLEN    3
#define FOUT_W_PARAMSMAX    4
#define FOUT_W_PARAMSLEN    5
#define FOUT_W_MSGPTRNULL   6

#define FOUT_STREAM         stdout

#define FOUT_FMT_PARAMS     4
#define FOUT_FMT_PARAMLEN   80
#define FOUT_FMT_STRINGLEN  320

#define FOUT_FMT_ID         '%'
#define FOUT_FMT_PARAM      '.'
#define FOUT_FMT_ANSI       '`'
#define FOUT_FMT_SEPARATOR  ':'

#define FOUT_FMT_CHAR       'c'
#define FOUT_FMT_FLOAT      'f'
#define FOUT_FMT_HEXLC      'x'
#define FOUT_FMT_HEXUC      'X'
#define FOUT_FMT_INT        'd'
#define FOUT_FMT_STRING     's'

#define FOUT_FMT_ERRNO      'e'
#define FOUT_FMT_ERRMSG     'm'

#define FOUT_F_NOANSI       0x01
#define FOUT_F_SHOWANSI     0x02

char    *(*__fout_msg)(int) = NULL;

int     __fout_flags        (int);
int     __fout_param        (char *, char *);
int     __fout_ansi         (FILE *, char *, char *, va_list);
int     __fout_arg          (FILE *, char *, char *, va_list);
int     _fout               (FILE *, char *, va_list);
int     fout                (FILE *, char *, ...);

#include "../c/fout.c"

#endif