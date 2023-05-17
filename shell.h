#ifndef SHELL_H
#define SHELL_H

#include <stdio.h> /* input and output functions */
#include <stdlib.h> /* memory allocation and process control functions */
#include <unistd.h> /* POSIX operating system API functions */
#include <string.h> /* string manipulation functions */
#include <sys/types.h>
/* data types used in system calls, such as pid_t (process ID type) */
#include <sys/wait.h> /* process management and waiting */
#include <sys/stat.h> /* file status & information */
#include <limits.h> /* constants and limits */
#include <fcntl.h> /* file control options and functions */
#include <errno.h> /* the errno variable */
#include <ctype.h> /* character classification and conversion */
#include <glob.h> /* pattern matching and file globbing */

#define READ_BUF_SIZE 1024
#define WRITE_BUF_SIZE 1024
#define BUF_FLUSH -1

extern char **environ;

/**
 * struct list_s - singly linked list
 * @num: the number field
 * @string: a string
 * @next: points to the next node
 */
typedef struct list_s
{
	int num;
	char *string;
	struct list_s *next;
} list_t;

list_t *append_node(list_t **, const char *, int);
size_t print_strnode(const list_t *);
int remove_index(list_t **, unsigned int);
void free_list(list_t **);
size_t list_size(const list_t *);
char **list_to_strings(list_t *);
list_t *node_prefix(list_t *, char *, char);
ssize_t find_index(list_t *, list_t *);

/**
 * struct Shell - Structure used to store the state of the shell
 * @arg:the argument that was passed to the shell.
 * @argv: an array of strings contains the arguments that were passed.
 * @path: a string, the current working directory of the shell.
 * @argc: the number of arguments passed to the shell.
 * @count: the current line number of the shell input.
 * @estatus: the exit status of the last command executed by the shell.
 * @display: A flag indicating whether to display line numbers.
 * @fname: the name of the current file being executed by the shell.
 * @env: a linked list that represents the shell's environment variables.
 * @environ: An array of strings that reps the shell's environment variables.
 * @alias:  a linked list that represents the shell's alias commands.
 * @modified: A flag indicating if the env variables have been modified.
 * @status: the current status of the shell.
 * @Buffer: the command buffer of the shell.
 * @type: the type of command buffer being used by the shell.
 * @file_descriptor: holds the file descriptor used for reading input.
 */
typedef struct Shell
{
	char *arg;
	char **argv;
	char *path;
	int argc;
	unsigned int count;
	int estatus;
	int display;
	char *fname;
	list_t *env;
	list_t *alias;
	char **environ;
	int modified;
	int status;
	char **Buffer;
	int type;
	int file_descriptor;
} shell_t;

/*a macro that initializes a struct of type shell with default values.*/
#define SHELL_INIT                                  \
	{                                            \
		NULL, NULL, NULL, 0, 0, 0, 0, NULL, \
		  NULL, NULL, NULL, 0, 0, NULL, 0, 0	\
	}

/**
 * struct builtin - contains a builtin string and related function
 * @type: the builtin command flag
 * @func: the function
 */
typedef struct builtin
{
	char *type;
	int (*func)(shell_t *);
} builtin_t;

/* declearations of the prototypes of various built-in functions */

int exit_builtin(shell_t *);
int cd_builtin(shell_t *);
int env_builtin(shell_t *);
int setenv_builtin(shell_t *);
int unsetenv_builtin(shell_t *);
int alias_builtin(shell_t *);

int set_alias(shell_t *, char *);
int unset_alias(shell_t *, char *);
int replace_alias(shell_t *);
int print_alias(list_t *node);
int execute(shell_t *, char **);
int replace_vars(shell_t *);
int strep(char **, char *);
char *_itoa(long int, int, int);
int interactive(shell_t *);
int populate(shell_t *);
ssize_t input_buffer(shell_t *, char **, size_t *);
void remove_comments(char *buffer);
int _strlen(char *);
int _strcmp(char *, char *);
char *_strchr(char *, char);
char *_strcat(char *, char *);
char *_strcpy(char *, char *);
char *_strdup(const char *);
char *_strncpy(char *, char *, int);
char *_strncat(char *, char *, int);
char **strtow(char *, char *);
char *_strstr(const char *, const char *);
char **expand_wildcards(char *);
void execute_wildcard(shell_t *);
void _puts(char *);
int _putchar(char);
int _puts2(char);
void _eputs(char *);
int _write(char, int);
void _perror(shell_t *, char *);
int decimal(int, int);
void _free(char **);
int isdelimeter(char, char *);
void sigintHandler(int);
void clear_data(shell_t *);
void free_data(shell_t *, int);
int _free2(void **);
ssize_t get_input(shell_t *);
int chained(shell_t *, char *, size_t *);
void check_chain(shell_t *, char *, size_t *, size_t, size_t);
ssize_t read_buffer(shell_t *data, char *buffer, size_t *i);
void set_data(shell_t *, char **);
int find_builtin(shell_t *);
void parse_command(shell_t *);
void _forks(shell_t *);
char **get_environ(shell_t *);
int iscommand(shell_t *, char *);
char *find_path(shell_t *, char *, char *);
char *duplicates(char *, int, int);
int _getline(shell_t *, char **, size_t *);
void *_realloc(void *, unsigned int, unsigned int);
char *_getenv(shell_t *, const char *);
int _unsetenv(shell_t *, char *);
int _setenv(shell_t *, char *, char *);
int _atoi(char *);

#endif
