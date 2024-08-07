/**********************************************************
 *
 *  scscan/src/h/octets.h
 *
 */

#ifndef _INCLUDED_OCTETS_H_
#define _INCLUDED_OCTETS_H_

#define OCTETS_MAX      4

#define OCTETS_LLTR     -1
#define OCTETS_LEQR     0
#define OCTETS_LGTR     1

typedef struct __octets
    {
        char            ip[16];
        char            *err;

        unsigned char   octet[OCTETS_MAX];
    }
OCTETS;

OCTETS      octetsNew       (char *);
int         octetsCompare   (OCTETS *, OCTETS *);

#include "../c/octets.c"

#endif
