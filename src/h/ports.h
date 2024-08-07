#include <netdb.h>

#ifndef _INCLUDED_PORTS_H_
#define _INCLUDED_PORTS_H_

int     __getServicePort    (char *, char *);
char    *__getServiceName   (int, char *);

#include "../c/ports.c"

#endif
