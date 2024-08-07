/**********************************************************
 *
 *  scscan/src/c/sock.c
 *
 */


int
tcpConnect(char *host, int port, int timeout)
    {
        int     _sock = socket(AF_INET, SOCK_STREAM, 0);

        struct timeval _to;    

        struct  sockaddr_in _in;
        struct  hostent     *_h_ent;

        int     __status;
        fd_set  _sock_set;

        int         _result;
        socklen_t _result_len = sizeof(_result);

        if (_sock < 0)
            return -1;

        _h_ent = gethostbyname(host);

        if (! _h_ent)
            return -3;

        bzero((char *) &_in, sizeof(struct sockaddr_in));
        bcopy((char *) _h_ent->h_addr, &_in.sin_addr.s_addr, _h_ent->h_length);
        
        _in.sin_family = AF_INET;
        _in.sin_port = htons(port);

        _to.tv_sec = 0;
        _to.tv_usec = timeout;

        fcntl(_sock, F_SETFL, O_NONBLOCK);
        __status = connect(_sock, (struct sockaddr *) &_in, sizeof(_in));

        if (__status < 0 && errno != EINPROGRESS)
            return -5;

        if (__status == 0)
        {
            close(_sock);
            return -1;
        }

        FD_ZERO(&_sock_set);
        FD_SET(_sock, &_sock_set);

        if (select((_sock + 1), NULL, &_sock_set, NULL, &_to) == 1) {
            getsockopt(_sock, SOL_SOCKET, SO_ERROR, &_result, &_result_len);

            if (_result == 0)
            {
                close(_sock);
                return 1;
            }
        }

        close(_sock);
        return 2;
    }

