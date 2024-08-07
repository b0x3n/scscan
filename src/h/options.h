/**********************************************************
 *
 *  scscan/src/h/options.h
 *
 */

#include "hosts.h"
#include "ports.h"
#include "sock.h"

#ifndef _INCLUDED_OPTIONS_H_
#define _INCLUDED_OPTIONS_H_

typedef struct  _options    OPTIONS;

char    *getOption      (char **, int, int);
int     sortOptions     (char **, int, OPTIONS);
int     __optionOut     (char *, ...);

#include "../c/options.c"

#endif

