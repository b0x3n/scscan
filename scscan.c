#include "libcommon/libcommon.h"

#define APP_NAME        "scscan"
#define APP_VERSION     1
#define APP_REVISION    1
#define APP_AUTHOR      "M. Nealon"

#include "src/h/options.h"
#include "src/h/scan.h"


/**
 * See:
 *
 *      src/h/options.h
 *
 * For more info.
 */
extern      OPTIONS     __options;


/**
 * Constructor and destructor functions, these
 * handle initialisation and cleanup.
 */
void        __up        (void) __attribute__ ((constructor));
void        __down      (void) __attribute__ ((destructor));


/**
 * __up() and __down() call these.
 */
void        _up         (void);
void        _down       (void);


void        closeStream (FILE **);


int
main(int argc, char **argv)
    {

        int             _return_value = PERR_OK;
        int             item;

        FILE            *_err = stderr;


    //  Sorts command line options - see:
    //
    //      src/h/options.h
    //
        if ((_return_value = optionsSort(
            argv,
            argc,
            &__options
        )) < PERR_OK)
            goto _lbl_end_main;

    //  Processing options may have reset the error
    //  stream.
    //
        _err = __options.stream_err;


        __options._out("%`bright`%`:cyan`%s%`reset` v-%`bright`%d.%d%`reset`\n\n", APP_NAME, APP_VERSION, APP_REVISION);

    //  Config check and output dump (if the SCAN_NOISY
    //  flag is set).
    //
    //  The -colour option is used to set the FOUT_F_NOANSI
    //  option (see libcommon/h/fout.h)
    //
        if (! (__options.flags & SCAN_COLOR))
            __fout_flags(FOUT_F_NOANSI);

    //  If the SCAN_HRANGE option is set, then there
    //  should be 2 hosts in the list.
    //
        if (__options.flags & SCAN_HRANGE)
        {
            if (__options.hosts.items < 2)
                fperr(_err, PERR_FAILURE, "%`bright`%`:red`Error%`reset`: to scan a %`bright`range%`reset` of hosts, %`bright`2%`reset` host addresses are required\n");
            __options._out("Scanning host range %`bright`%s%`reset` - %`bright`%s%`reset`\n", __options.hosts.item[0], __options.hosts.item[1]);
            __options._out("\n");
        }
        else
        {
            __options._out("Scanning %`bright`%d%`reset` hosts\n\n", __options.hosts.items);
            for (item = 0; item < __options.hosts.items; item++)
                __options._out("\tHost %`bright`%d%`reset`: %`bright`%`:cyan`%s%`reset`\n", item, __options.hosts.item[item]);
            __options._out("\n");
        }

    //  Similarly - if the SCAN_PRANGE flag is set, then 2
    //  port numbers are required to continue.
    //
        if (__options.flags & SCAN_PRANGE)
        {
            if (__options.ports.items < 2)
                fperr(_err, PERR_FAILURE, "%`bright`%`:red`Error%`reset`: to scan a %`bright`range%`reset` of ports, %`bright`2%`reset` port numbers are required\n");
            __options._out("Scanning port range %`bright`%s%`reset` - %`bright`%s%`reset`\n", __options.ports.item[0], __options.ports.item[1]);
            __options._out("\n");
        }
        else
        {
            __options._out("Scanning %`bright`%d%`reset` ports per host\n\n", __options.ports.items);
            for (item = 0; item < __options.ports.items; item++)
                __options._out("\tPort %`bright`%d%`reset`: %`bright`%`:cyan`%s%`reset`\n", item, __options.ports.item[item]);
            __options._out("\n");
        }

        __options._out("Connection timeout value %`bright`%d%`reset` usec\n\n", __options.timeout);


        _return_value = beginScan(&__options);


_lbl_end_main:

        if (_return_value < PERR_OK)
            perr(_err, PERR_FAILURE, _return_value);

        exit(EXIT_SUCCESS);

    }


/**
 * initialises default options.
 */
void
_up(void)
    {
        __options.stream_out = stdout;
        __options.stream_err = stderr;

        __options.hosts = list_new(0, 0, 0);
        __options.ports = list_new(0, 0, 0);

        __options.timeout = 500000;

    //  Default options set - noisy (verbose)
    //  and no UDP scans.
    //
        __options.flags = (SCAN_NOISY);

    //  Set the output function.
    //
        __options._out = __optionOut;
    }


/**
 * Cleanup, close files, free memory, etc.
 */
void
_down(void)
    {
        closeStream(&__options.stream_out);
        closeStream(&__options.stream_err);

        list_free(&__options.hosts);
        list_free(&__options.ports);
    }


void
__up(void)
    {
        _up();
    }


void
__down(void)
    {
        _down();
    }


/**
 * If *stream points to a non-standard stream then
 * the stream is closed and the pointer NULL'd
 */
void
closeStream(FILE **stream)
    {
        if (! *stream || ! stream)
            return;

        if (*stream != stdout && *stream != stderr) {
            fclose(*stream);
            *stream = NULL;
        }
    }

