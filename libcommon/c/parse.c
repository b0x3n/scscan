int
parse_add_line(
    LIST *lines, 
    char **line, 
    char **token, 
    int datalen, 
    int line_no
) {

    int _return_val = LIST_OK;

    memset(*token, '\0', (datalen + 1));
    snprintf(*token, datalen, "%d %s", line_no, *line);

    if ((_return_val = list_add(lines, *token)) < LIST_OK)
        return _return_val;
                    
    memset(*line, '\0', (datalen + 1));
    
    return LIST_OK;

}

int
parse_lines(LIST *lines, char *data)
{

    char    *_line = NULL;
    char    *_token = NULL;

    char    *_src = data;
    char    *_dst;

    char    _quote_char = '\0';

    int     _this_line = 1;
    int     _curr_line = 1;

    int     _datalen;

    int     _return_val = PARSE_OK;

    if (data == NULL || *data == '\0')
        return PARSE_W_NODATA;

    _datalen = (strlen(data) + 40);

    _line = malloc(sizeof(char) * (_datalen + 1));
    _token = malloc(sizeof(char) * (_datalen + 1));

    if (! _line || ! _token)
        return PERR_MALLOC;

    memset(_line, '\0', (_datalen + 1));

    _dst = _line;

    while (*_src) {
        if (PARSE_T_EOL(_src)) {
            _curr_line++;
            
            if (*_src == '\r') _src++;

            if (*_src == '\n') {
                if (_quote_char != '\0')
                    *_dst++ = *_src++;
                else
                    _src++;

                *_dst = '\0';
            }

            if (! PARSE_T_QUOTE(_quote_char)) {
                if (_dst != _line) {
                    _return_val = parse_add_line(
                        lines, &_line, &_token, _datalen, _this_line
                    );

                    if (_return_val < LIST_OK)
                        goto _lbl_parse_lines_end;

                    _dst = _line;
                }
            }

            continue;
        }

        if (PARSE_T_QUOTE(*_src)) {
            if (_quote_char == '\0')
                _quote_char = *_src;
            else {
                if (_quote_char == *_src)
                    _quote_char = '\0';
            }
        }

        if (_dst == _line)
            _this_line = _curr_line;

        *_dst++ = *_src++;
        *_dst = '\0';
    }

    if (_dst != _line)
        _return_val = parse_add_line(
            lines, &_line, &_token, _datalen, _this_line
        );

_lbl_parse_lines_end:

    if (_line)
        free(_line);

    if (_token)
        free(_token);

    if (_return_val < LIST_OK)
        return _return_val;

    if (_quote_char != '\0')
        return _this_line;

    return LIST_OK;
}

int
parse_tokens(LIST *tokens, char *line)
{

    char    *_token = NULL;
    
    char    *_src;
    char    *_dst;

    char    _quote_char = '\0';

    int     _return_val = LIST_OK;

    if ((_token = malloc(sizeof(char) * (strlen(line)) + 1)) == NULL)
        return PERR_MALLOC;

    memset(_token, '\0', (strlen(line) + 1));

    _src = line;
    _dst = _token;

    while (*_src) {
        if (PARSE_T_SPACE(*_src)) {
            *_src++;
            continue;
        }

        if (PARSE_T_SINGLE(*_src)) {
            *_dst++ = *_src++;
            *_dst = '\0';

            goto _lbl_add_parsed_token;
        }

        if (PARSE_T_QUOTE(*_src)) {
            _quote_char = *_dst++ = *_src++;
            *_dst = '\0';

            while (*_src) {
                *_dst++ = *_src++;
                *_dst = '\0';

                if (*(_src - 1) == _quote_char)
                    break;
            }

            goto _lbl_add_parsed_token;
        }

        while (*_src) {
            if (PARSE_T_SPACE(*_src) || PARSE_T_SINGLE(*_src))
                break;

            *_dst++ = *_src++;
            *_dst = '\0';
        }

_lbl_add_parsed_token:

        if ((_return_val = list_add(tokens, _token)) < LIST_OK)
            goto _lbl_parse_tokens_end;

        memset(_token, '\0', (strlen(line) + 1));
        _dst = _token;

        if (*_src == '\0')
            goto _lbl_parse_tokens_end;
    }

    goto _lbl_add_parsed_token;

_lbl_parse_tokens_end:

    free(_token);

    return _return_val;

}

