/**
 | We can just include all of the header files in
 | the h/ directory and we're good to go.
 |
 | The way the system is built, some systems do
 | depend on others...
 |
 | Including ansi.h will include nothing more, it
 | is the only .h file that doesn't include any
 | of the other .h files.
 |
 | fout.h will include:
 |  h/ansi.h
 |
 | perr.h will include:
 |  h/fout.h 
 |
 | And thus includes ansi.h by extension.
 |
 | path.h will include:
 |  h/perr.h
 |
 | And thus includes ansi.h and fout.h by extension.
 |
 | file.h includes:
 |  h/path.h
 |
 | Which, again, includes perr.h, ansi.h and fout.h
 | by extension.
 |
 | list.h includes:
 |  h/perr.h
 |
 | And thus includes ansi.h and fout.h
 |
 | Lastly, parse.h includes:
 |  h/perr.h
 |  h/list.h
 |
 | And thus includes ansi.h, fout.h and perr.h
 |
 | TL;DR - if we simply include file.h and parse.h
 | they will include everything else for us!
 |
 */

/**
 | h/file.h includes:
 |
 |  h/path.h
 |  h/perr.h
 |  h/fout.h
 |  h/ansi.h
 */
#include "h/file.h"

/**
 | h/parse.h includes:
 |
 |  h/list.h
 |  h/perr.h
 |  h/fout.h
 |  h/ansi.h
 |
 | This is fine, since each individual .h uses
 | preprocessor directives to avoid multiple
 | includes of the same file.
 */
#include "h/parse.h"
