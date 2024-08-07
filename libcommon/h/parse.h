#ifndef _INCLUDED_PARSE_H_
#define _INCLUDED_PARSE_H_

#include "perr.h"
#include "list.h"

#define PARSE_OK            PERR_OK

#define PARSE_W_NODATA      1

#define PARSE_T_EOL(c)      \
    (*c == '\n'     ||      \
     (*c == '\r'    &&      \
      *(c + 1) == '\n')     \
    )                       \
? 1 : 0

#define PARSE_T_QUOTE(c)    \
    (c == '\''      ||      \
     c == '\"'      ||      \
     c == '`')              \
? 1 : 0

#define PARSE_T_SPACE(c)    \
    (c == ' '       ||      \
     c == '\t')             \
? 1 : 0

#define PARSE_T_SINGLE(c)   \
    (c == '.'       ||      \
     c == ','       ||      \
     c == '['       ||      \
     c == ']'       ||      \
     c == '{'       ||      \
     c == '}'       ||      \
     c == '('       ||      \
     c == ')'       ||      \
     c == '+'       ||      \
     c == '-'       ||      \
     c == '/'       ||      \
     c == '*'       ||      \
     c == '$'       ||      \
     c == '~'       ||      \
     c == '#'       ||      \
     c == '%'       ||      \
     c == '^'       ||      \
     c == '<'       ||      \
     c == '>'       ||      \
     c == '!'       ||      \
     c == '?')              \
? 1 : 0

int     parse_add_line  (LIST *, char **, char **, int, int);
int     parse_lines     (LIST *, char *);
int     parse_tokens    (LIST *, char *);

#include "../c/parse.c"

#endif

