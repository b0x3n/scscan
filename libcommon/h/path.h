#include "perr.h"

#ifndef _INCLUDED_PATH_H_
#define _INCLUDED_PATH_H_

#include <sys/stat.h>

#define PATH_OK             0

#define PATH_MAXLEN         256
#define PATH_SEPARATOR      '/'

char        *path_prev      (char *);
char        *path_next      (char *, char *);

#define PATH_TRUE           1
#define PATH_FALSE          0

int         path_isreg      (char *);
int         path_isdir      (char *);
int         path_islnk      (char *);
int         path_isexe      (char *);

#include "../c/path.c"

#endif
