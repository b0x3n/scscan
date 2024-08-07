#include "perr.h"

#ifndef _INCLUDED_LIST_H_
#define _INCLUDED_LIST_H_

typedef struct __list
{
    char    **item;
    int     items;

    int     max_items;
    int     max_item_size;
    char    *pad;

    int     flags;
#define     LIST_F_NODUPL       0x01
#define     LIST_F_IGCASE       0x02
#define     LIST_F_RESIZE       0x04
#define     LIST_F_RTWRNG       0x08
} LIST;

#define     LIST_W_NULLITEM     1
#define     LIST_W_LISTFULL     2
#define     LIST_W_NORESIZE     3
#define     LIST_W_DUPLITEM     4
#define     LIST_W_RANGEERR     5

#define     LIST_OK             PERR_OK

LIST        list_new            (int, int, int);
void        list_free           (LIST *);
int         list_pad            (LIST *, char *);
int         list_ispad          (LIST *, char *);
int         list_find           (LIST *, char *);
void        list_dump           (FILE *, LIST *);
int         list_validate       (LIST *, char *, char **);
int         list_add            (LIST *, char *);
int         list_paditem        (LIST *, int, char *);
int         list_insert         (LIST *, int, char *);
int         list_remove         (LIST *, int);
int         list_replace        (LIST *, int, char *);
int         list_tokens         (LIST *, char *, char *);

#include "../c/list.c"

#endif
