#include "shell.h"

int find_builtin(shell_t *);
void clear_data(shell_t *);
void sigintHandler(int);

/**
 * find_builtin - finding and executing
 *					built-in commands based on the user's input.
 * @data: A pointer to the shell data structure,
 *	which contains information about the current state of the shell.
 * Return: returns the value, which is either -1
 *	if the command is not a built-in command
 *	or the return value of the executed built-in function.
 */
int find_builtin(shell_t *data)
{
	int i, value = -1;
	builtin_t builtins[] = {
		{"exit", exit_builtin},
		{"env", env_builtin},
		{"setenv", setenv_builtin},
		{"unsetenv", unsetenv_builtin},
		{"cd", cd_builtin},
		{"alias", alias_builtin},
		{NULL, NULL}};

	for (i = 0; builtins[i].type; i++)
		if (_strcmp(data->argv[0], builtins[i].type) == 0)
		{
			data->count++;
			value = builtins[i].func(data);
			break;
		}
	return (value);
}

/**
 * clear_data - a utility function used to clear
 *		the data fields of the shell_t structure.
 * @data: A pointer to the shell data structure,
 *	which contains information about the current state of the shell.
 */
void clear_data(shell_t *data)
{
	data->arg = NULL;
	data->argv = NULL;
	data->path = NULL;
	data->argc = 0;
}

/**
 * sigintHandler - a signal handler for the SIGINT signal,
 * which is typically generated when the user presses Ctrl+C in the shell.
 * @sig_num: the signal number
 * marked as unused in the function declaration to suppress compiler warnings.
 * Return: void
 */
void sigintHandler(__attribute__((unused)) int sig_num)
{
	_puts("\n");
	_puts("$ ");
	_putchar(BUF_FLUSH);
}

