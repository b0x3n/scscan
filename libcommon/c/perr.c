void
__perr_init(void)
{
    __fout_msg = perr_msg;
}


char
*perr_msg(int perr_no)
{
    static  char    msg[PERR_MSGLEN] = "No error";

    if (perr_no != PERR_OK) {
        if (perr_no > PERR_OK)
            snprintf(msg, PERR_MSGLEN, "Warning: %d", perr_no);
        else {
            if (perr_no == PERR_MALLOC)
                strncpy(msg, "malloc(): ", PERR_MSGLEN);
            else if (perr_no == PERR_REALLOC)
                strncpy(msg, "realloc(): ", PERR_MSGLEN);
            else if (perr_no == PERR_FOPEN)
                strncpy(msg, "fopen(): ", PERR_MSGLEN);
            else if (perr_no == PERR_STAT)
                strncpy(msg, "stat(): ", PERR_MSGLEN);
            else
                snprintf(msg, PERR_MSGLEN, "Unknown error (%d): ", perr_no);

            strncat(msg, strerror(errno), PERR_MSGLEN);
        }
    }

    return msg;
}


int
fperr(FILE *stream, int exit_status, char *fmt, ...)
{
    int     return_val = FOUT_OK;
    va_list args;

    if (! stream)
        stream = PERR_STREAM;

    if (! *fmt)
        return PERR_W_NULLFMT;

    va_start(args, fmt);
    return_val = _fout(stream, fmt, args);
    va_end(args);

    if (exit_status != PERR_NOEXIT) {
        // if (stream != FOUT_STREAM && stream != PERR_STREAM) {
        //     stream = NULL;
        //     fclose(stream);
        // }
        exit(exit_status);
    }

    return return_val;
}


int
perr(FILE *stream, int exit_status, int perr_no)
{
    return fperr(stream, exit_status, "%m\n", perr_no);
}
