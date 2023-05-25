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
 * Return: The function returns 0 to indicate successful completion,
 *         or a non-zero value in case of an error.
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
		dir = _getenv(data, "OLDPWD=");
		if (!dir)
		{
			_puts(store);
			_putchar('\n');
			return (1);
		}
		_puts(dir);
		_putchar('\n');
		value = chdir(dir);
	}
	else
		value = chdir(data->argv[1]);
	if (value == -1)
	{
		_perror(data, "can't cd to ");
		_eputs(data->argv[1]);
		_puts2('\n');
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
 * Return: The function returns 0 to indicate successful completion.
 */
int env_builtin(shell_t *data)
{
	list_t *env = data->env;

	while (env)
	{
		printf("%s\n", env->string);
		env = env->next;
	}

	return (0);
}
/**
 * setenv_builtin - implements the functionality of the setenv command.
 *		It allows the user to set or update environment variables.
 * @data: A pointer to the shell data structure,
 *		which contains information about the current state of the shell.
 * Return: The function returns 0 to indicate successful completion,
 *		or a non-zero value in case of an error.
 */
int setenv_builtin(shell_t *data)
{
	if (data->argc != 3)
	{
		_eputs("Usage: setenv VARIABLE VALUE\n");
		return (1);
	}

	if (setenv(data->argv[1], data->argv[2], 1) != 0)
	{
		_perror(data, "setenv");
		return (1);
	}

	return (0);
}

/**
 * unsetenv_builtin - implements the functionality of the unsetenv command.
 *		It allows the user to remove environment variables.
 * @data: A pointer to the shell data structure,
 *		which contains information about the current state of the shell.
 * Return: The function returns 0 to indicate successful completion.
 */
int unsetenv_builtin(shell_t *data)
{
	int i;

	if (data->argc < 2)
	{
		_eputs("Usage: unsetenv VARIABLE [VARIABLE...]\n");
		return (1);
	}

	for (i = 1; i < data->argc; i++)
	{
		if (unsetenv(data->argv[i]) != 0)
		{
			_perror(data, "unsetenv");
			return (1);
		}
	}

	return (0);
}
