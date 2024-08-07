/**********************************************************
 *
 *  scscan/src/h/sock.h
 *
 */

#include <sys/socket.h>
#include <sys/types.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h> 
#include <netdb.h>
#include <fcntl.h>

#ifndef _INCLUDED_SOCK_H_
#define _INCLUDED_SOCK_H_

int     tcpConnect     (char *, int, int);

#include "../c/sock.c"

#endif

