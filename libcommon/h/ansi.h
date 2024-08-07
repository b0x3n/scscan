#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef _INCLUDED_ANSI_H_
#define _INCLUDED_ANSI_H_

#define ANSI_STREAM         stdout

#define ANSI_ATTRIBUTES     8

char    *__ansi_attributes[ANSI_ATTRIBUTES] = {
    "reset",
    "bright",
    "dim",
    "underline",
    "blink",
    "reverse",
    "hidden",
    "cursor"
};

#define ANSI_CURSOR         7

char    *__ansi_cursor[ANSI_CURSOR] = {
    "clear",
    "end",
    "up",
    "down",
    "right",
    "left",
    "position"
};

char    *__ansi_movecursor[ANSI_CURSOR] = {
    "2J",
    "K",
    "A",
    "B",
    "C",
    "D",
    "H"
};

#define ANSI_COLOURS        8

char    *__ansi_colours[ANSI_COLOURS] = {
    "black",
    "red",
    "green",
    "yellow",
    "blue",
    "magenta",
    "cyan",
    "white"
};

#define ANSI_OK             0

#define ANSI_W_NULLSTRING   -1
#define ANSI_W_OUTOFRANGE   -2
#define ANSI_W_NOTFOUND     -3

int     ansi_lookup         (char **, int, char *);
int     ansi_attrno         (char *);
int     ansi_cursorno       (char *);
int     ansi_colourno       (char *);

#define ANSI_ESC            0x1B

#define ANSI_FG_OFFSET      30
#define ANSI_BG_OFFSET      40

int     ansi_setattr        (FILE *, char *);
int     ansi_setfg          (FILE *, char *);
int     ansi_setbg          (FILE *, char *);
int     ansi_setall         (FILE *, char *, char *, char *);
void    ansi_reset          (FILE *);

#include "../c/ansi.c"

#endif