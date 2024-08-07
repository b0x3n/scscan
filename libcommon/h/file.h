#include "path.h"

#ifndef _INCLUDED_FILE_H_
#define _INCLUDED_FILE_H_

#define FILE_W_NULLPATH     1
#define FILE_W_EXISTERR     2

#define FILE_OK             0

#define FILE_MODE_FOPEN     0
#define FILE_MODE_POPEN     1

int     _file_readstream    (FILE *, char **);
char    *_file_opts         (char *, int *, int *);
int     file_load           (char *, char **, int);
int     file_save           (char *, char *);

#include "../c/file.c"

#endif
