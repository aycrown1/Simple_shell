#include "shell.h"

int exit_builtin(shell_t *);
int cd_builtin(shell_t *);
int env_builtin(shell_t *);
int setenv_builtin(shell_t *);
int unsetenv_builtin(shell_t *);

/**
 * exit_builtin - implements the functionality of the exit command.
 * It allows the user to exit the shell program.
 * @data: A pointer to the shell data structure,
 *		which contains information about the current state of the shell.
 * Return: returns -2 to indicate the desired action to the shell program.
 */
int exit_builtin(shell_t *data)
{
	int exitcheck;

	if (data->argv[1]) /* If there is an exit arguement */
	{
		exitcheck = _atoi(data->argv[1]);
		/* converts the argument to an integer */
		if (exitcheck == -1)
		{
			data->status = 2;
			_perror(data, "Illegal number: ");
			/* prints an error message */
			_eputs(data->argv[1]);
			_puts2('\n');
			return (1);
		}
		data->estatus = _atoi(data->argv[1]);
		return (-2);
	}
	data->estatus = -1;
	return (-2);
}

/**
 * cd_builtin - implements the functionality of the cd command.
 *		It allows the user to change the current working directory.
 * @data: A pointer to the shell data structure,
 *		which contains information about the current state of the shell.
 * Return: the function returns 0 to indicate successful completion,
 *		or 1 in some specific cases.
 */
int cd_builtin(shell_t *data)
{
	char *store, *dir, buffer[1024];
	int value;

	store = getcwd(buffer, 1024);
	if (!store)
		_puts("Failed to get current working directory.\n");

	if (!data->argv[1])
	{
		dir = _getenv(data, "HOME=");
		if (!dir)
		{
			value = 1;
			chdir((dir = _getenv(data, "PWD=")) ? dir : "/");
		}
		else
			value = chdir(dir);
	}
	else if (_strcmp(data->argv[1], "-") == 0)
	{
		if (!_getenv(data, "OLDPWD="))
		{
			_puts(store);
			_putchar('\n');
			return (1);
		}
		_puts(_getenv(data, "OLDPWD=")), _putchar('\n');
		value = 1;
		chdir((dir = _getenv(data, "OLDPWD=")) ? dir : "/");
	}
	else
		value = chdir(data->argv[1]);
	if (value == -1)
	{
		_perror(data, "can't cd to ");
		_eputs(data->argv[1]), _puts2('\n');
	}
	else
	{
		_setenv(data, "OLDPWD", _getenv(data, "PWD="));
		_setenv(data, "PWD", getcwd(buffer, 1024));
	}
	return (0);
}

/**
 * env_builtin - implements the functionality of the env command.
 *		It is used to print the environment variables.
 * @data: A pointer to the shell data structure,
 *		which contains information about the current state of the shell.
 * Return: returns 0 to indicate successful completion.
 */
int env_builtin(shell_t *data)
{
	print_strnode(data->env);
	return (0);
}

/**
 * setenv_builtin - implements the functionality of the setenv command.
 *		It allows the user to set or update environment variables.
 * @data: A pointer to the shell data structure,
 *which contains information about the current state of the shell.
 *  Return: returns 0 to indicate successful completion.
 *		Otherwise, it returns 1 to indicate an error.
 */
int setenv_builtin(shell_t *data)
{
	if (data->argc != 3)
	{
		_eputs("Incorrect number of arguements\n");
		return (1);
	}
	if (_setenv(data, data->argv[1], data->argv[2]))
		return (0);
	return (1);
}

/**
 * unsetenv_builtin - implements the functionality of the unsetenv command.
 *		It allows the user to remove environment variables.
 * @data: A pointer to the shell data structure,
 *		which contains information about the current state of the shell.
 * Return: the function returns 0 to indicate successful completion.
 */
int unsetenv_builtin(shell_t *data)
{
	int count;

	if (data->argc == 1)
	{
		_eputs("Too few arguements.\n");
		return (1);
	}
	for (count = 1; count <= data->argc; count++)
		_unsetenv(data, data->argv[i]);

	return (0);
}

