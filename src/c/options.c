/**********************************************************
 *
 *  scscan/src/c/options.c
 *
 */


struct _options
    {
        FILE    *stream_out;
        FILE    *stream_err;

        LIST    hosts;      // List of host addresses
        LIST    ports;      // list of posts to scan. 

        int     timeout;    // Timeout for TCP connect.
        
        int     flags;
#define SCAN_HRANGE 0x0001  // Scan range of host IP's
#define SCAN_PRANGE 0x0002  // Scan port range
#define SCAN_HINVRT 0x0004  // Invert host scan
#define SCAN_PINVRT 0x0008  // Invert port scan

#define SCAN_SRVCE  0x0100  // Show port service names
#define SCAN_NOISY  0x0200  // Verbose output
#define SCAN_COLOR  0x0400  // ANSI colours in output

        int         (*_out) (char *, ...);
    };


OPTIONS     __options;


char
*getOption(char **argv, int argc, int arg)
    {
        if ((++arg) >= argc)
            return NULL;

        return *(argv + arg);
    }


/**
 * Function processes and applies command-line
 * options.
 */
int
optionsSort(char **argv, int argc, OPTIONS *options)
    {

        int         arg;
        int         _return_value = PERR_OK;

        char        *option = NULL;
        char        _port_string[8];

        int         _port;

        FILE        *_err = options->stream_err;


        for (arg = 1; arg < argc; arg++)
        {

        //  The --host option - allows you to add a new
        //  host IP to the list.
        //
            if (strcasecmp(argv[arg], "--host") == 0)
            {
                if ((option = getOption(argv, argc, arg++)) == NULL)
                    fperr(_err, PERR_FAILURE, "%`bright`%`:red`Error%`reset`: Option %`bright`--host%`reset` expects a parameter\n");
        
                if (! isdigit(argv[arg][0]))
                {
                    fprintf(stdout, "HERE?\n\n");
                    if ((option = getHostAddress(argv[arg])) == NULL)
                        fperr(_err, PERR_FAILURE, "%`bright`%`:red`Error%`reset`: Unknown hostname %`bright`%s%`reset`\n", argv[arg]);
                
                    fprintf(stdout, "IP Address: |%s|\n", option);
                }

                if ((_return_value = list_add(&options->hosts, option)) < PERR_OK)
                    return _return_value;

                continue;
            }

        //  The --port option - allows you to add a new
        //  posrt to the list.
        //
            if (strcasecmp(argv[arg], "--port") == 0)
            {
                if ((option = getOption(argv, argc, arg++)) == NULL)
                    fperr(_err, PERR_FAILURE, "%`bright`%`:red`Error%`reset`: Option %`bright`--port%`reset` expects a parameter\n");
        
                if (isalpha(argv[arg][0]))
                {
                    _port = __getServicePort(argv[arg], "TCP");

                    if (_port == -1)
                        fperr(_err, PERR_FAILURE, "%`bright`%`:red`Error%`reset`: Unknown service %`bright`%s%`reset`\n", argv[arg]);
                    
                    snprintf(_port_string, 8, "%d", _port);
                    option = &_port_string[0];
                }
                else
                    option = argv[arg];

                if ((_return_value = list_add(&options->ports, option)) < PERR_OK)
                    return _return_value;

                continue;
            }

        //  The --out option allows you to speciy a file to
        //  write all standard output to - you may not want to
        //  enable -color if you're using this or --err as it
        //  will output the ansi escape sequences.
        //
            if (strcasecmp(argv[arg], "--out") == 0)
            {
                if ((option = getOption(argv, argc, arg++)) == NULL)
                    fperr(_err, PERR_FAILURE, "%`bright`%`:red`Error%`reset`: Option %`bright`--out%`reset` expects a parameter\n");
        
                fclose(options->stream_out);

                if (argv[arg][0] == '+')
                    options->stream_out = fopen(&argv[arg][1], "a");
                else
                    options->stream_out = fopen(argv[arg], "w");

                if (! options->stream_out)
                    perr(_err, PERR_FAILURE, PERR_FOPEN);
                
                continue;
            }

        //  Similar to the --out option only allows you to
        //  specify an error stream other than stderr.
        //
            if (strcasecmp(argv[arg], "--err") == 0)
            {
                if ((option = getOption(argv, argc, arg++)) == NULL)
                    fperr(_err, PERR_FAILURE, "%`bright`%`:red`Error%`reset`: Option %`bright`--err%`reset` expects a parameter\n");
        
                if (argv[arg][0] == '+')
                    options->stream_err = fopen(&argv[arg][1], "a");
                else
                    options->stream_err = fopen(argv[arg], "w");

                if (! options->stream_err)
                    perr(stderr, PERR_FAILURE, PERR_FOPEN);

                _err = options->stream_err;

                continue;
            }

        //  Set the socket connect timeout.
        //
            if (strcasecmp(argv[arg], "--timeout") == 0)
            {
                if ((option = getOption(argv, argc, arg++)) == NULL)
                    fperr(_err, PERR_FAILURE, "%`bright`%`:red`Error%`reset`: Option %`bright`--err%`reset` expects a parameter\n");

                options->timeout = atoi(option);

                continue;
            }

        //  Switches (no parameter required)
        //
            if (strcasecmp(argv[arg], "-hrange") == 0)
                options->flags ^= SCAN_HRANGE;
            else if (strcasecmp(argv[arg], "-prange") == 0)
                options->flags ^= SCAN_PRANGE;
            else if (strcasecmp(argv[arg], "-invert-hosts") == 0)
                options->flags ^= SCAN_HINVRT;
            else if (strcasecmp(argv[arg], "-invert-ports") == 0)
                options->flags ^= SCAN_PINVRT;
            else if (strcasecmp(argv[arg], "-services") == 0)
                options->flags ^= SCAN_SRVCE;
            else if (strcasecmp(argv[arg], "-silent") == 0)
                options->flags ^= SCAN_NOISY;
            else if (strcasecmp(argv[arg], "-color") == 0)
                options->flags ^= SCAN_COLOR;
            else
                fperr(_err, PERR_FAILURE, "Unknown option: %s\n", argv[arg]);
        }

    //  If no hosts are specified then localhost is
    //  scanned by default.
    //
        if (__options.hosts.items == 0)
        {
            if ((_return_value = list_add(&__options.hosts, "127.0.0.1")) < PERR_OK)
                perr(_err, PERR_FAILURE, _return_value);
        }


    //  If no ports are specified, default to the range
    //  1 - 10000.
    //
        if (__options.ports.items == 0)
        {
            if ((_return_value = list_add(&__options.ports, "1")) < PERR_OK)
                perr(_err, PERR_FAILURE, _return_value);
            if ((_return_value = list_add(&__options.ports, "10000")) < PERR_OK)
                perr(_err, PERR_FAILURE, _return_value);
        
            __options.flags |= SCAN_PRANGE;
        }

    
        return _return_value;

    }


/**
 * Output function for the OPTIONS struct, the
 * _out member of the struct should be pointed to
 * this.
 */
int
__optionOut(char *fmt, ...)
    {
        va_list     args;
        int         return_val = FOUT_OK;

    //  Bail if the SCAN_NOISY flag is unset (no
    //  standard output).
    //
        if (! (__options.flags & SCAN_NOISY))
            return FOUT_OK;

        if (fmt == NULL || *fmt == '\0')
            return FOUT_W_NULLFMT;

        va_start(args, fmt);
        return_val = _fout(__options.stream_out, fmt, args);
        va_end(args);

        return return_val;
    }

