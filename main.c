/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/cFiles/main.c to edit this template
 */

/* 
 * File:   main.c
 * Author: raz
 *
 * Created on December 8, 2024, 10:20 AM
 */

#include <stdio.h>
#include <stdlib.h>
#include <argp.h>
#include <string.h>
#include <stdbool.h>
#include <stdarg.h>

/*
 * Defines
*/
enum log_level  
{
    TRACE,
    INFO,
    WARN, 
    DEBUG,
    ERROR,
    FATAL, 
    LAST_LVL,
};

#define log_file "/tmp/cat_in_c.logfile"

/*
 Prototypes ====================================================================
*/
void print_version( void ); 
int  parse_opt( int key, char * arg, struct argp_state *state);
void flog(int lvl, const char * fmt, ...);


/*
 Types =========================================================================
*/
struct flags {
    bool show_all;
    bool num_blanks;
    bool numbers;
    bool show_eol;
    bool squeeze;
    bool show_tabs;
    bool show_nonprint;
    int  verbosity;
};

typedef struct _log_level {
    const char * str;
    const int    lvl;
} log_level;

/*
 Globals =======================================================================
*/
// arg parsing
struct flags g_flags = {0};
struct argp_option options[] = 
{
    { "show-all",         'A', 0, 0, "show all (equivalent to -vET)" },
    { "number-nonblank",  'b', 0, 0, "number nonempty output lines, overrides -n"},
    { 0,                  'e', 0, 0, "equivalent to -vE" },
    { "show-ends",        'E', 0, 0, "display $ at end of each line" },
    { "number",           'n', 0, 0, "number all output lines" },
    { "squeeze-blank",    's', 0, 0, "suppress repeated empty output lines" },
    { 0,                  't', 0, 0, "equivalent to -vT" },
    { "show-tabs",        'T', 0, 0, "display TAB character as ^I"},
    { "show-nonprinting", 'v', 0, 0, "use ^ and M- notation, except for LFD and TAB" },
    { "version",          'V', 0, 0, "output version information and quit" },
    { "log-level",        'l', 0, 0, "verbosity level for logging"},
    { 0 }
};

struct argp argp = { options, parse_opt };

log_level log_levels[] = 
{
    { "TRACE",  TRACE    },
    { "INFO",   INFO     },
    { "WARN",   WARN     },
    { "DEBUG",  DEBUG    },
    { "ERROR",  ERROR    },
    { "FATAL",  FATAL    },
    { "LAST",   LAST_LVL }
};


/*==============================================================================
 * Function parse_opt
 * 
 * Description:
 * 
 * Parameters:
 * 
 * Returns: 
 *============================================================================*/
int parse_opt( int key, char * arg, struct argp_state *state)
{
    flog(TRACE, "+parse_opt");
    switch (key) {
        case 'A' : 
        { 
            g_flags.show_all = true; 
            flog(INFO, "show-all is enabled");
            break; 
        }
        case 'b' : 
        { 
            g_flags.num_blanks = true;
            flog(INFO, "num_blanks is enabled");
            break; 
        }
        case 'e' : 
        { 
            g_flags.show_eol = true; 
            flog(INFO, "show_eol is enabled");
            break; 
        }
        case 'E' : 
        { 
            g_flags.show_eol      = true;
            flog(INFO, "show_eol is enabled");
            break;
        }
        case 'n' : 
        { 
            g_flags.numbers       = true;
            flog(INFO, "numbers is enabled");
            break;
        }
        case 's' : 
        { 
            g_flags.squeeze       = true;
            flog(INFO, "squeeze is enabled");
            break;
        }
        case 't' : 
        { 
            g_flags.show_tabs     = true;
            flog(INFO, "show-tabs is enabled");
            break;
        }
        
        case 'v' : 
        { 
            g_flags.show_nonprint = true;
            flog(INFO, "show-nonprint is enabled");
            break;
        }
        case 'V' : 
        { 
            print_version();
            exit(0);
        }
        case 'l' : 
        { 
            int lvl = FATAL;
            if (arg != NULL)
            {
                for (int ii = 0; ii < LAST_LVL; ii++) {
                    if (! strncmp(log_levels[ii].str, arg, strlen(log_levels[ii].str)))
                        lvl = log_levels[ii].lvl;
                }
            }
            g_flags.verbosity = lvl;
            break;
        }

        default: printf(" oops!  what? key = %c\n", key);
    }
    return( 0 );
}

/*==============================================================================
 * Function print_version()
 * 
 * Description:
 * 
 * Parameters:
 * 
 * Returns: 
 *============================================================================*/
static const char * version_string = "version 0.1";

void print_version( void ) {
    printf("%s", version_string);
}

/*==============================================================================
 * Function flog()
 * 
 * Description:
 * 
 * Parameters:
 * 
 * Returns: 
 *============================================================================*/
void flog( int lvl, const char * fmt, ...)
{
    static FILE * h_logfile = NULL;

    // if log file hasn't been opened yet, do it
    if (h_logfile == NULL)
    {
        h_logfile = fopen(log_file, "w+t");
        if (! h_logfile) {
            fprintf(stderr," Failed to open log file -- quitting\n");
            exit(1);
        }
    }

    if (lvl >= g_flags.verbosity)
    {
        va_list args;
        va_start(args, fmt);
        vfprintf(h_logfile, fmt, args);
        fprintf(h_logfile, "\n");
        va_end(args);
        fflush(h_logfile);
    }
}


/*
 M A I N =======================================================================
 */

int main(int argc, char** argv) {

    int rv = 0;

    rv = argp_parse( &argp, argc, argv, 0, 0, 0);

    return( rv );
}


