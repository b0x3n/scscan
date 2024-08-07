extern  char    *__ansi_attributes[ANSI_ATTRIBUTES];
extern  char    *__ansi_cursor[ANSI_CURSOR];
extern  char    *__ansi_colours[ANSI_COLOURS];


int
_ansi_lookup(
    char **array,
    int array_size,
    char *ansi_string
)
{
    int     index = ANSI_OK;

    if (ansi_string == NULL || *ansi_string == '\0')
        return ANSI_W_NULLSTRING;

    if (strlen(ansi_string) == 1 && isdigit(*ansi_string)) {
        index = atoi(ansi_string);
        if (index < ANSI_OK || index >= array_size)
            return ANSI_W_OUTOFRANGE;
    } else {
        while (1) {
            if (index >= array_size)
                return ANSI_W_NOTFOUND;
            if (strcasecmp(ansi_string, *(array + index)) == 0)
                break;
            index++;
        }
    }

    return index;
}


int
ansi_attrno(char *ansi_attr)
{
    int     return_val;
    
    return _ansi_lookup(
        __ansi_attributes,
        ANSI_ATTRIBUTES,
        ansi_attr
    );
}


int
ansi_cursorno(char *ansi_cursor)
{
    int     return_val;
    
    return _ansi_lookup(
        __ansi_cursor,
        ANSI_CURSOR,
        ansi_cursor
    );
}


int
ansi_colourno(char *ansi_colour)
{
    int     return_val;
    
    return _ansi_lookup(
        __ansi_colours,
        ANSI_COLOURS,
        ansi_colour
    );
}


int
ansi_setattr(FILE *stream, char *attr)
{
    int     attr_no = ansi_attrno(attr);

    if (stream == NULL)
        stream = stdout;

    if (attr_no < ANSI_OK)
        return attr_no;

    fprintf(stream, "%c[%dm", ANSI_ESC, attr_no);
    fflush(stream);

    return ANSI_OK;
}


int
ansi_setfg(FILE *stream, char *colour)
{
    int     colour_no = ansi_colourno(colour);

    if (stream == NULL)
        stream = stdout;

    if (colour_no < ANSI_OK)
        return colour_no;

    colour_no += ANSI_FG_OFFSET;

    fprintf(stream, "%c[%dm", ANSI_ESC, colour_no);
    fflush(stream);

    return ANSI_OK;
}


int
ansi_setbg(FILE *stream, char *colour)
{
    int     colour_no = ansi_colourno(colour);

    if (stream == NULL)
        stream = stdout;

    if (colour_no < ANSI_OK)
        return colour_no;

    colour_no += ANSI_BG_OFFSET;

    fprintf(stream, "%c[%dm", ANSI_ESC, colour_no);
    fflush(stream);

    return ANSI_OK;
}


int
ansi_setcursor(FILE *stream, char *move, char *opt, char *param)
{
    int     return_val = ANSI_OK;

    if ((return_val = ansi_cursorno(move)) < ANSI_OK)
        return return_val;

    if (stream == NULL)
        stream = ANSI_STREAM;

    if (return_val < 2)
        fprintf(stream, "%c[%s", ANSI_ESC, __ansi_movecursor[return_val]);
    else if (return_val > 1 && return_val < 6)
        fprintf(stream, "%c[%s%s", ANSI_ESC, opt, __ansi_movecursor[return_val]);
    else
        fprintf(stream, "%c[%s;%s%s", ANSI_ESC, opt, param, __ansi_movecursor[return_val]);

    if (! return_val)
        fprintf(stream, "%c[0;0;H", ANSI_ESC);

    return ANSI_OK;
}


int
ansi_setall(FILE *stream, char *attr, char *fg, char *bg)
{
    int     return_val;

    if (attr && *attr != '\0') {
        if ((return_val = ansi_setattr(stream, attr)) < ANSI_OK)
            return return_val;
    }

    if (fg && *fg != '\0') {
        if ((return_val = ansi_setfg(stream, fg)) < ANSI_OK)
            return return_val;
    }

    if (bg && *bg != '\0') {
        if ((return_val = ansi_setbg(stream, bg)) < ANSI_OK)
            return return_val;
    }

    return ANSI_OK;
}


void
ansi_reset(FILE *stream)
{
    ansi_setattr(stream, "reset");
}

