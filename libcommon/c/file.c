int
_file_readstream(FILE *stream, char **data)
{
    int     return_val = FILE_OK;
    int     char_in;

    while (1) {
        if ((char_in = fgetc(stream)) == EOF)
            break;

        if (*data) {
            if ((*data = realloc(*data, (strlen(*data) + 2))) == NULL) {
                return_val = PERR_REALLOC;
                break;
            }
            *(*data + (strlen(*data) + 1)) = '\0';
            *(*data + strlen(*data)) = char_in;
        } else {
            if ((*data = malloc(2)) == NULL) {
                return_val = PERR_MALLOC;
                break;
            }
            *(*data) = char_in;
            *(*data + 1) = '\0';
        }
    }

    return return_val;
}


char
*_file_opts(char *path, int *append, int *exists)
{
    if (*path == '+' || *path == '-') {
        if (*path == '+') *append = 1;
        else *exists = 1;
        path++;
    }

    if (*path == '+' || *path == '-') {
        if (*path == '+') *append = 1;
        else *exists = 1;
        path++;
    }

    return path;
}


int
file_load(char *path, char **data, int mode)
{
    FILE    *stream;
    int     return_val = FILE_OK;

    if (path == NULL || *path == '\0')
        return FILE_W_NULLPATH;

    if (mode == FILE_MODE_FOPEN)
        stream = fopen(path, "r");
    else
        stream = popen(path, "r");

    if (! stream)
        return PERR_FOPEN;
        
    return_val = _file_readstream(stream, data);

    fclose(stream);
}


int
file_save(char *path, char *data)
{
    FILE    *stream;

    int     append = 0;
    int     exists = 0;

    struct  stat    s;

    if (path == NULL || *path == '\0')
        return FILE_W_NULLPATH;
    if (data == NULL)
        data = "";

    path = _file_opts(path, &append, &exists);

    if (exists) {
        if (stat(path, &s) < 0) {
            if (! S_ISREG(s.st_mode))
                return FILE_W_EXISTERR;
        }
    }

    if (append)
        stream = fopen(path, "a");
    else
        stream = fopen(path, "w");

    fprintf(stream, "%s", data);
    fclose(stream);

    return FILE_OK;
}

