#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "lab.h"

#define VALID_OPTIONS "v"  // Defines the valid option(s) for getopt

/**
 * @brief Parse command line args from the user when the shell was launched.
 * 
 * This function processes command-line arguments using `getopt()` (see documentation link below). For
 * now, this function only enabled `-v` to print the shell version, but it is designed so that it is    
 * easy to add more arg options later. To do so, add a new case in the switch statement and add the arg  
 * to the VALID_OPTIONS constant at the top of this file.
 *
 * https://www.gnu.org/software/libc/manual/html_node/Example-of-Getopt.html
 *
 * @param argc Number of args
 * @param argv The arg array
 */
void parse_args(int argc, char **argv) {
    int opt;
    while ((opt = getopt(argc, argv, VALID_OPTIONS)) != -1) {

        switch (opt) {
            case 'v': // shell version, using macros from lab.h
                printf("Shell Version: %d.%d\n", lab_VERSION_MAJOR, lab_VERSION_MINOR);
                exit(EXIT_SUCCESS);
                break;
            case '?': // not a valid option, so print the error and exit.
                if (isprint(optopt)) { // if the opt is printable, print it.
                    fprintf(stderr, "Unknown option '-%c'\n", optopt);
                } else { // if opt is not printable, print its hex value.
                    fprintf(stderr, "Unknown option character '\\x%x'\n", optopt);
                }
                exit(EXIT_FAILURE);
            default:
                abort(); // failsafe exit
        }
    }
}
