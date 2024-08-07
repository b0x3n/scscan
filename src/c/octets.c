/**********************************************************
 *
 *  scscan/src/c/octets.c
 *
 */

extern  OPTIONS __options;

OCTETS
octetsNew(char *ip_addr)
    {
        OCTETS  octets = {
            "",
            NULL,
            { 0, 0, 0, 0 }
        };

        int     _return_value;
        LIST    _oct = list_new(0, 0, 0);

        FILE    *_err = __options.stream_err;

        _return_value = list_tokens(&_oct, ip_addr, ".");

        if (_return_value < PERR_OK)
            perr(_err, PERR_FAILURE, _return_value);

        strncpy(octets.ip, ip_addr, 16);

        if (_oct.items != 4 || strlen(ip_addr) >16) {
            octets.err = "Invalid IP address";
            return octets;
        }

        octets.octet[0] = (unsigned char) atol(_oct.item[0]);
        octets.octet[1] = (unsigned char) atol(_oct.item[1]);
        octets.octet[2] = (unsigned char) atol(_oct.item[2]);
        octets.octet[3] = (unsigned char) atol(_oct.item[3]);

        list_free(&_oct);

        return octets;
    }


int
octetsCompare(OCTETS *loct, OCTETS *roct)
    {
        if (loct->octet[0] < roct->octet[0])
            return OCTETS_LLTR;
        else if (loct->octet[0] > roct->octet[0])
            return OCTETS_LGTR;
        else
        {
            if (loct->octet[1] < roct->octet[1])
                return OCTETS_LLTR;
            else if (loct->octet[1] > roct->octet[1])
                return OCTETS_LGTR;
            else
            {
                if (loct->octet[2] < roct->octet[2])
                    return OCTETS_LLTR;
                else if (loct->octet[2] > roct->octet[2])
                    return OCTETS_LGTR;
                else
                {
                    if (loct->octet[3] < roct->octet[3])
                        return OCTETS_LLTR;
                    else if (loct->octet[3] > roct->octet[3])
                        return OCTETS_LGTR;
                    else
                        return OCTETS_LEQR;
                }
            }
        }
    }


void
octetsNext(OCTETS *octets)
    {
        if (octets->octet[3] == 255)
        {
            octets->octet[3] = 0;
            if (octets->octet[2] == 255)
            {
                octets->octet[2] = 0;
                if (octets->octet[1] == 255)
                {
                    octets->octet[1] = 0;
                    if (octets->octet[0] == 255)
                        octets->octet[0] = 0;
                    else
                        octets->octet[0]++;
                }
                else
                    octets->octet[1]++;
            }
            else
                octets->octet[2]++;
        }
        else
            octets->octet[3]++;

        snprintf(octets->ip, 16, "%d.%d.%d.%d.", (int) octets->octet[0], (int) octets->octet[1], (int) octets->octet[2], (int) octets->octet[3]);
    }


void
octetsPrevious(OCTETS *octets)
    {
        if (octets->octet[3] == 0)
        {
            octets->octet[3] = 255;
            if (octets->octet[2] == 0)
            {
                octets->octet[2] = 255;
                if (octets->octet[1] == 0)
                {
                    octets->octet[1] = 255;
                    if (octets->octet[0] == 0)
                        octets->octet[0] = 255;
                    else
                        octets->octet[0]--;
                }
                else
                    octets->octet[1]--;
            }
            else
                octets->octet[2]--;
        }
        else
            octets->octet[3]--;

        snprintf(octets->ip, 16, "%d.%d.%d.%d.", (int) octets->octet[0], (int) octets->octet[1], (int) octets->octet[2], (int) octets->octet[3]);
    }

