/**********************************************************
 *
 *  scscan/src/c/ports.c
 *
 */


char
*__getServiceName(int port, char *proto)
    {
        static  struct  servent *service_name;
        static  char            *_service;

        service_name = getservbyport(htons(port), NULL);

        if (! service_name)
            _service = "unknown";
        else
            _service = service_name->s_name;

        return _service;
    }


int
__getServicePort(char *name, char *proto)
    {
        struct  servent     *service_name;

        service_name = getservbyname(name, NULL);

        if (! service_name)
            return -1;
        else
            return ntohs(service_name->s_port);
    }

