/**********************************************************
 *
 *  scscan/src/c/hosts.c
 *
 */


char
*getHostAddress(char *hostname)
    {  
        struct in_addr **addrs_inet;
        struct hostent *h_ent = gethostbyname(hostname);

        static  char    _addr[20] = "";

        if (h_ent == NULL)
            return NULL;

        if (h_ent->h_addrtype == AF_INET)
        {
            addrs_inet = (struct in_addr **)h_ent->h_addr_list;
            strncpy(_addr, inet_ntoa(*addrs_inet[0]), 20);
        }

        return _addr;
    }


char
*getHostName(char *addr)
    {
        struct      sockaddr_in _sa;
        socklen_t   _len;

        static      char        _hostname[NI_MAXHOST] = "";

        memset(&_sa, 0, sizeof(struct sockaddr_in));

        _sa.sin_family = AF_INET;
        _sa.sin_addr.s_addr = inet_addr(addr);
        _len = sizeof(struct sockaddr_in);

        if (getnameinfo(
            (struct sockaddr *) &_sa,
            _len,
            _hostname,
            sizeof(_hostname), 
            NULL,
            0,
            NI_NAMEREQD
        ) < 0)
            return NULL;

        return _hostname;          
    }

