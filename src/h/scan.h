#include <netdb.h>


/**********************************************************
 *
 *  scscan/src/h/scan.h
 *
 */

#include "ports.h"
#include "options.h"
#include "octets.h"

#ifndef _INCLUDED_SCAN_H_
#define _INCLUDED_SCAN_H_

int     beginScan           (OPTIONS *);

int     _beginRangeScan     (OPTIONS *);
int     _beginListScan      (OPTIONS *);

int     __scanPortRange     (OPTIONS *, char *, int, int, char *, char *);
int     __scanPortList      (OPTIONS *, char *, char *);

#include "../c/scan.c"

#endif

