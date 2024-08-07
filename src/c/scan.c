/**********************************************************
 *
 *  scscan/src/c/scan.c
 *
 */


int
beginScan(OPTIONS *options)
    {

        if (options->flags & SCAN_HRANGE)
            return _beginRangeScan(options);
        else
            return _beginListScan(options);

    }


int
_beginRangeScan(OPTIONS *options)
    {
        int         _return_value = PERR_OK;
        int         _loct_status;

        char        *_direction = "forward";
        char        *_scan_type = "port list";

        unsigned    int         _start_port;
        unsigned    int         _end_port;

        int         _ports = options->ports.items;
        char        *_hostname;

        OCTETS      _from = octetsNew(options->hosts.item[0]);
        OCTETS      _to = octetsNew(options->hosts.item[1]);

        FILE        *_err = options->stream_err;

        if (_from.err)
            fperr(_err, PERR_FAILURE, "Invalid IP address: %`bright`%`:cyan`%s%`reset`\n", options->hosts.item[0]);
        if (_to.err)
            fperr(_err, PERR_FAILURE, "Invalid IP address: %`bright`%`:cyan`%s%`reset`\n", options->hosts.item[1]);

        _loct_status = octetsCompare(&_from, &_to);

        if (_loct_status == OCTETS_LLTR)
        {
            if (options->flags & SCAN_HINVRT)
                _direction = "reverse";
        }
        else if (_loct_status == OCTETS_LGTR)
        {
            _direction = "reverse";

            if (options->flags & SCAN_HINVRT)
                _direction = "forward";
        }

        if (options->flags & SCAN_PRANGE)
        {
            _scan_type = "port range";

            _start_port = (int) atol(options->ports.item[0]);
            _end_port = (int) atol(options->ports.item[1]);

            if (_start_port < _end_port)
                _ports = ((_end_port - _start_port) + 1);
            else if (_start_port > _end_port)
                _ports = ((_start_port - _end_port) + 1);
            else
                _ports = 1;

            if (options->flags & SCAN_PINVRT) {
                _scan_type = "port range (inverted)";
                _ports = ((65536 - _ports) + 1);
            }
        }

        options->_out("\nBeginning %`bright`range%`reset` scan from %`bright`%`:cyan`%s%`reset` to %`bright`%`:cyan`%s%`reset` in a %`bright`%s%`reset` direction\n\n", options->hosts.item[0], options->hosts.item[1], _direction);  

        while (1)
        {
            options->_out("\tScanning %`bright`%s%`reset` (%`bright`%d%`reset` ports) on host %`bright`%`:cyan`%s%`reset`", _scan_type, _ports, _from.ip);
        
            if ((_hostname = getHostName(_from.ip)) != NULL)
                options->_out(" (%`bright`%`:green`%s%`reset`)", _hostname);
            options->_out("\n\n");
            
            // if (! (options->flags & SCAN_NOTCP)) {
                 if (strcmp(_scan_type, "port list") == 0)
                     __scanPortList(options, "TCP", _from.ip);
                 else
                     __scanPortRange(options, _from.ip, _start_port, _end_port, _scan_type, "TCP");
            // } 

            if (octetsCompare(&_from, &_to) == OCTETS_LEQR)
                break;

            if (strcmp(_direction, "forward") == 0)
                octetsNext(&_from);
            else
                octetsPrevious(&_from);
        }

        return PERR_OK;
    }


/**
 * _beginListScan()
 *
 *  This is a little simpler than the range scan - in
 *  this instance, the given ports/port range will be
 *  scanned for each host IP in the list.
 *
 */
int
_beginListScan(OPTIONS *options)
    {
        int         _return_value = PERR_OK;

        char        *_scan_type = "port list";

        unsigned    int         _start_port;
        unsigned    int         _end_port;

        int         _host_no;

        int         _ports = options->ports.items;
        char        *_hostname;

        FILE        *_err = options->stream_err;

        if (options->flags & SCAN_PRANGE)
        {
            _scan_type = "port range";

            _start_port = (int) atol(options->ports.item[0]);
            _end_port = (int) atol(options->ports.item[1]);

            if (_start_port < _end_port)
                _ports = ((_end_port - _start_port) + 1);
            else if (_start_port > _end_port)
                _ports = ((_start_port - _end_port) + 1);
            else
                _ports = 1;

            if (options->flags & SCAN_PINVRT) {
                _scan_type = "port range (inverted)";
                _ports = ((65536 - _ports) + 1);
            }
        }

        options->_out("\nBeginning %`bright`list%`reset` scan (%`bright`%d%`reset`hosts)\n\n", options->hosts.items);  

        for (_host_no = 0; _host_no < options->hosts.items; _host_no++)
        {
            options->_out("\tScanning %`bright`%s%`reset` (%`bright`%d%`reset` ports) on host %`bright`%`:cyan`%s%`reset`\n\n", _scan_type, _ports, options->hosts.item[_host_no]);
        
        //    if (! (options->flags & SCAN_NOTCP)) {
                if (strcmp(_scan_type, "port list") == 0)
                    __scanPortList(options, "TCP", options->hosts.item[_host_no]);
                else
                    __scanPortRange(options, options->hosts.item[_host_no], _start_port, _end_port, _scan_type, "TCP");
        //    } 
        }

        return PERR_OK;
    }


/**
 * __scanPortRange()
 *
 *  Scans a range of ports.
 *
 */
int
__scanPortRange(OPTIONS *options, char *host, int start, int end, char *direction, char *protocol)
    {
        int         _port = start;
        int         _sock;

        char        *_service;

        while (1)
        {            
            _service = __getServiceName(_port, "TCP");

            options->_out("\t\tScanning %`bright`%s%`reset` Port %`bright`%`:cyan`%d%`reset` (%`bright`%`:cyan`%s%`reset`): ", protocol, _port, _service);

            if (strcmp(protocol, "TCP") == 0)
                _sock = tcpConnect(host, _port, options->timeout);

            if (_sock < 0)
            {
                fprintf(stderr, "%s\n", strerror(errno));
                options->_out("\n");
            }
            else {
                if (_sock == 0)
                    options->_out("%`bright`%`:red`CLOSED%`reset`\n");
                else
                {
                    if (_sock == 2)
                        options->_out("%`bright`%`:magenta`TIMEOUT%`reset`\n");
                    else
                        options->_out("%`bright`%`:green`OPEN%`reset`\n");
                }
            }

            if (_port == end)
                break;

        //  Next port...
        //
            if (options->flags & SCAN_PINVRT)
            {
                if (start < end)
                {
                    if (--_port < 1)
                        _port = 65535;
                }
                else
                {
                    if (++_port > 65535)
                        _port = 1;
                }
            }
            else
            {
                if (start < end)
                {
                    if (++_port > 65535)
                        _port = 1;
                }
                else
                {
                    if (--_port < 1)
                        _port = 65535;
                }
            }
        }

        options->_out("\n");
    }


/**
 * __scanPortList()
 *
 *  Scan a list of ports as opposed to a range.
 *
 */
int
__scanPortList(OPTIONS *options, char *protocol, char *host)
    {
        unsigned    int         _port;
        int                     _port_no;

        char                    *_service;
        char                    _str_port[8];

        int                     _sock;

        if (options->flags & SCAN_PINVRT)
        {
            for (_port_no = 1; _port_no < 65535; _port_no++)
            {
                _service = __getServiceName(_port_no, "TCP");
                snprintf(_str_port, 8, "%d", _port_no);

                if (list_find(&options->ports, _str_port) >= PERR_OK)
                    continue;

                options->_out("\t\tScanning %`bright`%s%`reset` Port %`bright`%`:cyan`%d%`reset` (%`bright`%`:green`%s%`reset`): \n", protocol, _port_no, _service);
                       if (strcmp(protocol, "TCP") == 0)
                _sock = tcpConnect(host, _port, options->timeout);

                if (_sock < 0)
                {
                    fprintf(stderr, "%s\n", strerror(errno));
                    options->_out("\n");
                }
                else {
                    if (_sock == 0)
                        options->_out("%`bright`%`:red`CLOSED%`reset`\n");
                    else
                    {
                        if (_sock == 2)
                            options->_out("%`bright`%`:magenta`TIMEOUT%`reset`\n");
                        else
                            options->_out("%`bright`%`:green`OPEN%`reset`\n");
                    }
                }
            }

            return 0;
        }

        for (_port_no = 0; _port_no < options->ports.items; _port_no++)
        {
            _port = (unsigned int) atol(options->ports.item[_port_no]);
            _service = __getServiceName(_port, "TCP");

            options->_out("\t\tScanning %`bright`%s%`reset` Port %`bright`%`:cyan`%d%`reset` (%`bright`%`:green`%s%`reset`): \n", protocol, _port, _service);
        }

        options->_out("\n");
    }

