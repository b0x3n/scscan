#define FOUT_IS_SPACE(c)    \
    (c == ' '       ||      \
     c == '\t'      ||      \
     c == '\n')             \
? 1 : 0


int
__fout_flags(int flags)
{
    static  int _flags = 0;

    if (flags >= 0)
        _flags = flags;

    return _flags;
}


int
__fout_param(char *fmt, char *param)
{
    int     return_val = FOUT_OK;
    char    *str = param;

    memset(param, '\0', FOUT_FMT_PARAMLEN);

    while (*fmt) {
        if (! isdigit(*fmt))
            break;

        if (strlen(param) >= (FOUT_FMT_PARAMLEN - 1))
            return FOUT_W_PARAMLEN;

        *str++ = *fmt++;
        *str = '\0';
    }

    return return_val;
}


int
__fout_ansi(FILE *stream, char *fmt, char *ansi_string, va_list args)
{
    int     return_val = FOUT_OK;
    int     index = 0;

    char    *dst;
    char    *str = ansi_string;

    char    ansi_strings[FOUT_FMT_PARAMS][FOUT_FMT_PARAMLEN] = {
        "", "", "", ""
    };

    dst = &ansi_strings[0][0];

    memset(ansi_string, '\0', FOUT_FMT_STRINGLEN);

    if (*fmt == FOUT_FMT_ANSI) {
        *str++ = *fmt++;
        *str = '\0';
    }

    while (*fmt) {
        if (strlen(ansi_string) >= (FOUT_FMT_STRINGLEN - 1))
            return FOUT_W_STRINGLEN;

        if (*fmt == FOUT_FMT_ANSI) {
            *str++ = *fmt++;
            *str = '\0';

            break;
        }

        if (*fmt == FOUT_FMT_SEPARATOR) {
            *str++ = *fmt++;
            *str = '\0';

            if (++index >= FOUT_FMT_PARAMS)
                return FOUT_W_PARAMSMAX;

            dst = &ansi_strings[index][0];

            continue;
        }

        if (FOUT_IS_SPACE(*fmt)) {
            *str++ = *fmt++;
            *str = '\0';

            continue;
        }

        if (strlen(ansi_strings[index]) >= (FOUT_FMT_PARAMLEN - 1))
            return FOUT_W_PARAMLEN;

        *dst++ = *str++ = *fmt++;
        *dst = *str = '\0';
    }

    for (index = 0; index < FOUT_FMT_PARAMS; index++) {
        if (ansi_strings[index][0] == FOUT_FMT_ID) {
            if (ansi_strings[index][1] == FOUT_FMT_INT)
                snprintf(ansi_strings[index], FOUT_FMT_PARAMLEN, "%d", (int) va_arg(args, int));
            else if (ansi_strings[index][1] == FOUT_FMT_STRING)
                strncpy(ansi_strings[index], (char *) va_arg(args, char *), FOUT_FMT_PARAMLEN);
            else
                return ANSI_W_NOTFOUND;
        }
    }

    if (! (__fout_flags(-1) & FOUT_F_NOANSI)) {
        if ((strcmp(ansi_strings[0], "cursor") == 0) || (strcmp(ansi_strings[0], "7") == 0)) {
            return_val = ansi_setcursor(
                stream,
                ansi_strings[1],
                ansi_strings[2],
                ansi_strings[3]
            );
        } else {
            return_val = ansi_setall(
                stream,
                ansi_strings[0],
                ansi_strings[1],
                ansi_strings[2]
            );
        }
    }

    return return_val;
}


int
__fout_arg(FILE *stream, char *param, char *fmt, va_list args)
{
    char    fmt_string[FOUT_FMT_PARAMLEN] = "";

    if (*param != '\0')
        snprintf(fmt_string, FOUT_FMT_PARAMLEN, "%%.%s%c", param, *fmt);
    else
        snprintf(fmt_string, FOUT_FMT_PARAMLEN, "%%%c", *fmt);
        
    if (*fmt == FOUT_FMT_CHAR)
        fprintf(stream, "%c", (char) va_arg(args, int));
    else if (*fmt == FOUT_FMT_FLOAT)
        fprintf(stream, fmt_string, (float) va_arg(args, double));
    else if (*fmt == FOUT_FMT_HEXLC)
        fprintf(stream, fmt_string, (int) va_arg(args, int));
    else if (*fmt == FOUT_FMT_HEXUC)
        fprintf(stream, fmt_string, (int) va_arg(args, int));
    else if (*fmt == FOUT_FMT_INT)
        fprintf(stream, "%d", (int) va_arg(args, int));
    else if (*fmt == FOUT_FMT_STRING)
        fprintf(stream, "%s", (char *) va_arg(args, char *));
    else if (*fmt == FOUT_FMT_ERRNO)
        fprintf(stream, "%s", strerror(errno));
    else if (*fmt == FOUT_FMT_ERRMSG) {
        if (__fout_msg == NULL)
            return FOUT_W_MSGPTRNULL;
        fprintf(stream, "%s", __fout_msg((int) va_arg(args, int)));
    }
    else
        fprintf(stream, "%c%c", FOUT_FMT_ID, *fmt);
}


int
_fout(FILE *stream, char *fmt, va_list args)
{
    int     return_val = FOUT_OK;

    char    param[FOUT_FMT_PARAMLEN];
    char    ansi_string[FOUT_FMT_STRINGLEN];

    if (fmt == NULL || *fmt == '\0')
        return FOUT_W_NULLFMT;

    if (stream == NULL)
        stream = FOUT_STREAM;

    while (*fmt) {
        if (*fmt == FOUT_FMT_ID) {
            fmt++;

            if (*fmt == FOUT_FMT_ANSI) {
                if ((return_val = __fout_ansi(stream, fmt, &ansi_string[0], args)) != FOUT_OK)
                    break;

                if (__fout_flags(-1) & FOUT_F_SHOWANSI)
                    fprintf(stream, "%s", ansi_string);

                fmt += strlen(ansi_string);

                continue;
            }

            if (*fmt == FOUT_FMT_PARAM) {
                fmt++;

                if ((return_val = __fout_param(fmt, &param[0])) != FOUT_OK)
                    break;

                fmt += strlen(param);
            }

            if ((return_val = __fout_arg(stream, param, fmt, args)) < 0)
                break;
        } else
            fprintf(stream, "%c", *fmt);

        fmt++;
    }

    fflush(stream);

    return return_val;
}


int
fout(FILE *stream, char *fmt, ...)
{
    va_list     args;
    int         return_val = FOUT_OK;

    if (fmt == NULL || *fmt == '\0')
        return FOUT_W_NULLFMT;

    va_start(args, fmt);
    return_val = _fout(stream, fmt, args);
    va_end(args);

    return return_val;
}

