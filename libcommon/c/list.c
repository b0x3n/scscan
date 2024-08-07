LIST
list_new(
    int max_items, 
    int max_item_size, 
    int flags
) {
    LIST    list = {
        NULL, 0,
        max_items, max_item_size,
        NULL,
        flags
    };

    return list;
}


void
list_free(LIST *list)
{
    if (list->item) {
        free(list->item);
        list->item = NULL;
    }

    list->items = 0;

    if (list->pad) {
        free(list->pad);
        list->pad = NULL;
    }
}


int
list_pad(LIST *list, char *pad)
{
    if (list->pad) {
        free(list->pad);
        list->pad = NULL;
    }

    if (! pad)
        return LIST_OK;

    if ((list->pad = malloc(sizeof(char) * (strlen(pad) + 1))) == NULL)
        return PERR_MALLOC;

    strcpy(list->pad, pad);

    return LIST_OK;
}


int
list_ispad(LIST *list, char *pad)
{
    if (list->pad) {
        if (list->flags & LIST_F_IGCASE) {
            if (strcasecmp(list->pad, pad) == 0)
                return 1;
        } else {
            if (strcmp(list->pad, pad) == 0)
                return 1;
        }
    }

    return 0;
}


int
list_find(LIST *list, char *item)
{
    int     index;

    if (list->item != NULL) {
        if (item == NULL)
            item = "";

        for (index = 0; index < list->items; index++) {
            if (list->flags & LIST_F_IGCASE) {
                if (strcasecmp(list->item[index], item) == 0)
                    return index;
            } else {
                if (strcmp(list->item[index], item) == 0)
                    return index;
            }
        }
    }

    return -1;
}


void
list_dump(FILE *stream, LIST *list)
{
    int     index;

    if (stream == NULL)
        stream = FOUT_STREAM;

    fprintf(stdout, "Dumping a list of %d items:\n\n", list->items);
    for (index = 0; index < list->items; index++)
        fprintf(stream, "\tItem %d: %s\n", index, list->item[index]);
    fprintf(stream, "\n");
}


int
list_validate(LIST *list, char *item, char **validated)
{
    int     return_val = LIST_OK;
    char    *tmp = NULL;

    if (item == NULL)
        return LIST_W_NULLITEM;

    if ((tmp = malloc(sizeof(char) * (strlen(item) + 1))) == NULL)
        return PERR_MALLOC;

    strcpy(tmp, item);
    *validated = NULL;

    if (list->max_items > 0) {
        if (list->items >= list->max_items) {
            return_val = LIST_W_LISTFULL;
            goto _LBL_LIST_VALIDATE_END;
        }
    }

    if (list->max_item_size > 0) {
        if (strlen(tmp) > list->max_item_size) {
            if (list->flags & LIST_F_RESIZE)
                *(tmp + list->max_item_size) = '\0';
            else {
                return_val = LIST_W_NORESIZE;
                goto _LBL_LIST_VALIDATE_END;
            }
        }
    }

    if (list_ispad(list, item))
            goto _LBL_LIST_VALIDATE_END;

    if (list->flags & LIST_F_NODUPL) {
        if (list_find(list, tmp) >= LIST_OK) {
            return_val = LIST_W_DUPLITEM;
            goto _LBL_LIST_VALIDATE_END;
        }
    }


_LBL_LIST_VALIDATE_END:

    if (tmp) {
        if (return_val == LIST_OK) {
            *validated = malloc(sizeof(char) * (strlen(tmp) + 1));
            if (! *validated)
                return_val = PERR_MALLOC;
            else
                strcpy(*validated, tmp);
        }

        free(tmp);
    }

    return return_val;
}


int
list_add(LIST *list, char *item)
{
    int     return_val = LIST_OK;
    int     item_no;

    char    *validated = NULL;

    if (! item)
        return LIST_W_NULLITEM;

    return_val = list_validate(list, item, &validated);

    if (return_val != LIST_OK)
        goto _LBL_LIST_ADD_END;

    if (list->item) {
        if ((list->item = realloc(list->item, (sizeof(char *) * (list->items + 1)))) == NULL) {
            return_val = PERR_REALLOC;
            goto _LBL_LIST_ADD_END;
        }
    } else {
        if ((list->item = malloc(sizeof(char *))) == NULL) {
            return_val = PERR_MALLOC;
            goto _LBL_LIST_ADD_END;
        }
    }

    if ((list->item[list->items] = malloc(strlen(validated) + 1)) == NULL) {
        return_val = PERR_MALLOC;
        goto _LBL_LIST_ADD_END;
    }

    strcpy(list->item[list->items++], validated);


_LBL_LIST_ADD_END:

    if (validated)
        free(validated);

    return return_val;
}


int
list_paditem(LIST *list, int index, char *item)
{
    int     return_val = LIST_OK;
    int     item_no = list->items;

    while (1) {
        if (item_no == index)
            break;

        if ((return_val = list_add(list, list->pad)) != LIST_OK)
            break;

        item_no++;
    }

    if (return_val == LIST_OK)
        return_val = list_add(list, item);

    return return_val;
}


int
list_insert(LIST *list, int index, char *item)
{
    int     return_val = LIST_OK;
    int     item_no;

    char    *validated = NULL;

    if (index < 0)
        return LIST_W_RANGEERR;

    if (index >= list->items) {
        if ((list->max_items <= 0 || index < list->max_items) && list->pad) {
            if ((return_val = list_paditem(list, index, item)) != LIST_OK)
                return return_val;
            return return_val;
        } else
            return LIST_W_RANGEERR;
    }
    
    if (list->max_items > 0 && list->items >= list->max_items)
        return LIST_W_LISTFULL;

    if ((return_val = list_validate(list, item, &validated)) != LIST_OK)
        return return_val;

    if (list->item == NULL) {
        if ((list->item = malloc(sizeof(char *))) == NULL) {
            return_val = PERR_MALLOC;
            goto _LBL_LIST_INSERT_END;
        }
    } else {
        if ((list->item = realloc(list->item, (sizeof(char *) * (list->items + 1)))) == NULL) {
            return_val = PERR_REALLOC;
            goto _LBL_LIST_INSERT_END;
        }
    }

    for (item_no = list->items; item_no > index; item_no--)
        list->item[item_no] = list->item[(item_no - 1)];

    list->items++;

    if ((list->item[index] = malloc(sizeof(char) * (strlen(validated) + 1))) == NULL) {
        return_val = PERR_MALLOC;
        goto _LBL_LIST_INSERT_END;
    }

    strcpy(list->item[index], validated);

_LBL_LIST_INSERT_END:

    if (validated)
        free(validated);

    return return_val;
}


int
list_remove(LIST *list, int index)
{
    int     item_no;

    if (index < 0 || index >= list->items)
        return LIST_W_RANGEERR;

    if (list->items == 1 && index == 0) {
        list_free(list);
        return LIST_OK;
    }

    if (list->item[index])
        free(list->item[index]);

    for (item_no = (index + 1); item_no < list->items; item_no++)
        list->item[(item_no - 1)] = list->item[item_no];

    list->items--;

    list->item = realloc(list->item, (sizeof(char *) * list->items));

    if (! list->item)
        return PERR_REALLOC;

    return LIST_OK;
}


int
list_replace(LIST *list, int index, char *item)
{
    int     return_val = LIST_OK;

    if ((return_val = list_remove(list, index)) != LIST_OK)
        return return_val;

    if ((return_val = list_insert(list, index, item)) != LIST_OK)
        return return_val;

    return return_val;
}


int
list_tokens(LIST *list, char *string, char *sep)
{
    int     return_val = LIST_OK;

    char    *tmp;
    char    *tok;

    if (string == NULL)
        return LIST_W_NULLITEM;

    if (sep == NULL || *sep == '\0')
        sep = " ";

    if ((tmp = malloc(sizeof(char) * (strlen(string) + 1))) == NULL)
        return PERR_MALLOC;

    strcpy(tmp, string);
    tok = strtok(tmp, sep);

    while (tok) {
        if ((return_val = list_add(list, tok)) != LIST_OK) {
            if (return_val > LIST_OK) {
                if (return_val == LIST_W_LISTFULL)
                    return return_val;
                if (list->flags & LIST_F_RTWRNG)
                    return return_val;
            } else
                return return_val;
        }

        tok = strtok(NULL, sep);
    }
    
_LBL_LIST_TOKENS_END:

    if (tmp)
        free(tmp);

    return return_val;

}