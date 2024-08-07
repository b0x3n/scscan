/**********************************************************
 *
 *  scscan/src/h/hosts.h
 *
 */


#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h> 
#include <netdb.h>


#ifndef _INCLUDED_HOSTS_H_
#define _INCLUDED_HOSTS_H_

char        *getHostAddress     (char *);
char        *getHostName        (char *);

#include "../c/hosts.c"

#endif

