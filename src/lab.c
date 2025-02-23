#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "lab.h"
#include <readline/readline.h> 
#include <readline/history.h>
#include <pwd.h>
#include <errno.h>

#define VALID_OPTIONS "v"  // Defines the valid option(s) for getopt

/**
 * @brief Set the shell prompt. This function will attempt to load a prompt
 * from the requested environment variable, if the environment variable is
 * not set a default prompt of "shell>" is returned.  This function calls
 * malloc internally and the caller must free the resulting string.
 *
 * @param env The environment variable
 * @return A dynamically allocated string containing the prompt or "shell>" if
 * the environment variable is not set.
 */
char *get_prompt(const char *env) {
    const char *prompt = getenv(env); // fetch the environment variable
    return strdup(prompt ? prompt : "shell>");
}

/**
 * @brief Changes the current working directory of the shell. Uses the linux system
 * call chdir. With no arguments the users home directory is used as the
 * directory to change to.
 * 
 * In the case where no directory is provided, the function will attempt to use to the user's home 
 * directory. If the HOME environment variable is not set, the function will use the getuid() and 
 * getpwuid() system calls to determine the home directory of the user. The getuid() function returns 
 * the real user ID of the calling process, and the getpwuid() function searches the user database for 
 * an entry with a matching uid.
 * 
 * References:
 * https://man7.org/linux/man-pages/man2/getuid.2.html
 * https://man7.org/linux/man-pages/man3/getpwuid.3p.html
 *
 * @param dir The directory to change to
 * @return  On success, zero is returned.  On error, -1 is returned, and
 * errno is set to indicate the error.
 */
int change_dir(char **dir) {
    const char *path = dir[0]; // target directory

    // If no directory is provided, default to HOME
    if (!path) {
        path = getenv("HOME"); // try using HOME environment variable
        /* If HOME is still null, fallback to the system calls getuid and getpwuid to find out the home directory of the user.*/
        if (!path) {
            struct passwd *user_info = getpwuid(getuid());
            if (!user_info) {
                fprintf(stderr, "cd: Could not determine home directory\n");
                return -1;
            }
            path = user_info->pw_dir;
        }
    }

    // Attempt to change directory
    if (chdir(path) != 0) {
        perror("cd");
        return -1;
    }

    return 0;
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
    // No arguments
    if (!argv[0]) {
        return false;
    }
    // Exit
    if (strcmp(argv[0], "exit") == 0) {
        sh_destroy(sh);
        exit(EXIT_SUCCESS);
    } 
    // Change directory
    if (strcmp(argv[0], "cd") == 0) {
        // +1 below to skip argv[0], "cd", and provide only the directory argument, argv[1].
        change_dir(argv+1);
        return true;
    }
    // History
    if (strcmp(argv[0], "history") == 0) {
        print_history();
        return true;
    }

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
 * "MY_PROMPT" is the name of the environment variable we are using.
 *
 * @param sh
 */
void sh_init(struct shell *sh) {
    sh->prompt = get_prompt("MY_PROMPT"); // set shell's prompt with env variable
    if (!sh->prompt) {
        sh->prompt = strdup("shell>"); // default if env variable is not set
    }
}

/**
 * @brief Destroy shell. Free any allocated memory and resources and exit
 * normally.
 *
 * @param sh
 */
void sh_destroy(struct shell *sh) {
    free(sh->prompt); // free the prompt
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

/**
 * @brief Prints the command history stored by the Readline library.
 * 
 * HIST_ENTRY is a data type provided by the GNU Readline library that represents a single command the 
 * user previously typed into the shell.
 * 
 * Blank lines are not saved in the history, so they will not be printed by this function. If there are 
 * blank lines, they are handled in main.c.
 */
void print_history() {
    // Collect history entries.
    HIST_ENTRY **history_entries = history_list(); 
    // If no history entries, print error message.
    if (!history_entries) {
        fprintf(stderr, "Command history is empty.\n");
        return;
    }
    // Print history entries.
    int index = 0;
    while (history_entries[index]) {
        printf("%d.) %s\n", index+1, history_entries[index]->line);
        index++;
    }
}
