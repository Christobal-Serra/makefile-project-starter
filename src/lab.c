#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "lab.h"
#include <readline/readline.h> 
#include <readline/history.h>

#define VALID_OPTIONS "v"  // Defines the valid option(s) for getopt

/**
 * @brief Set the shell prompt. This function will attempt to load a prompt
 * from the requested environment variable, if the environment variable is
 * not set a default prompt of "shell>" is returned.  This function calls
 * malloc internally and the caller must free the resulting string.
 *
 * @param env The environment variable
 * @return const char* The prompt
 */
char *get_prompt(const char *env) {
    // TODO
    return NULL;
}

/**
 * Changes the current working directory of the shell. Uses the linux system
 * call chdir. With no arguments the users home directory is used as the
 * directory to change to.
 *
 * @param dir The directory to change to
 * @return  On success, zero is returned.  On error, -1 is returned, and
 * errno is set to indicate the error.
 */
int change_dir(char **dir) {
    // TODO
    return -1;
}

/**
 * @brief Convert line read from the user into to format that will work with
 * execvp. We limit the number of arguments to ARG_MAX loaded from sysconf.
 * This function allocates memory that must be reclaimed with the cmd_free
 * function.
 *
 * @param line The line to process
 *
 * @return The line read in a format suitable for exec
 */
char **cmd_parse(const char *line) {
    // TODO
    return NULL;
}

/**
 * @brief Free the line that was constructed with parse_cmd
 *
 * @param line the line to free
 */
void cmd_free(char **line) {
    // TODO
}

/**
 * @brief Trim the whitespace from the start and end of a string.
 * For example "   ls -a   " becomes "ls -a". This function modifies
 * the argument line so that all printable chars are moved to the
 * front of the string
 *
 * @param line The line to trim
 * @return The new line with no whitespace
 */
char *trim_white(char *line) {
    // TODO
    return line;
}


/**
 * @brief Takes an argument list and checks if the first argument is a
 * built in command such as exit, cd, jobs, etc. If the command is a
 * built in command this function will handle the command and then return
 * true. If the first argument is NOT a built in command this function will
 * return false.
 *
 * @param sh The shell
 * @param argv The command to check
 * @return True if the command was a built in command
 */
bool do_builtin(struct shell *sh, char **argv) {
    // TODO
    return false;
}

/**
 * @brief Initialize the shell for use. Allocate all data structures
 * Grab control of the terminal and put the shell in its own
 * process group. NOTE: This function will block until the shell is
 * in its own program group. Attaching a debugger will always cause
 * this function to fail because the debugger maintains control of
 * the subprocess it is debugging.
 *
 * @param sh
 */
void sh_init(struct shell *sh) {
    // TODO
}

/**
 * @brief Destroy shell. Free any allocated memory and resources and exit
 * normally.
 *
 * @param sh
 */
void sh_destroy(struct shell *sh) {
    // TODO
}

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
