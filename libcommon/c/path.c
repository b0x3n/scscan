char
*path_prev(char *path)
{
    static  char    prev_path[PATH_MAXLEN] = "";
    char    *dst;

    if (path == NULL || *path == '\0')
        return path;

    strncpy(prev_path, path, PATH_MAXLEN);
    dst = &prev_path[(strlen(prev_path) - 1)];

    if (*dst == PATH_SEPARATOR)
        *dst-- = '\0';

    while (dst != prev_path) {
        if (*dst == PATH_SEPARATOR)
            break;
        *dst-- = '\0';
    }

    return prev_path;
}

char
*path_next(char *path, char *next)
{
    static  char    next_path[PATH_MAXLEN] = "";

    if (path == NULL)
        path = "";

    if (next == NULL)
        next = "";

    if (*(path + (strlen(path) - 1)) != PATH_SEPARATOR)
        snprintf(next_path, PATH_MAXLEN, "%s/", path);
    else
        strncpy(next_path, path, PATH_MAXLEN);

    if (*next == PATH_SEPARATOR)
        strncat(next_path, (next + 1), PATH_MAXLEN);
    else
        strncat(next_path, next, PATH_MAXLEN);

    return next_path;
}


int
path_isreg(char *path)
{
    struct stat     s;

    if (stat(path, &s) < 0)
        return PERR_STAT;

    if (! S_ISREG(s.st_mode))
        return PATH_FALSE;

    return PATH_TRUE;
}

int
path_isdir(char *path)
{
    struct stat     s;

    if (stat(path, &s) < 0)
        return PERR_STAT;

    if (! S_ISDIR(s.st_mode))
        return PATH_FALSE;

    return PATH_TRUE;
}


int
path_islnk(char *path)
{
    struct stat     s;

    if (stat(path, &s) < 0)
        return PERR_STAT;

    if (! S_ISLNK(s.st_mode))
        return PATH_FALSE;

    return PATH_TRUE;
}


int
path_isexe(char *path)
{
    struct stat     s;

    if (stat(path, &s) < 0)
        return PERR_STAT;

    if (s.st_mode & S_IXUSR)
        return PATH_TRUE;

    return PATH_FALSE;
}

