#include "shell.h"

char *_getenv(shell_t *, const char *);
int _unsetenv(shell_t *, char *);
int _setenv(shell_t *, char *, char *);
char **get_environ(shell_t *);

/**
 * _getenv - allows the shell program to retrieve
 *			the value of a specific environment variable based on its name.
 * It searches through the linked list of environment variables
 *		and returns the value associated with the specified name.
 * @data: A pointer to the shell data structure,
 *		which contains information about the current state of the shell.
 * @name: the name of the environment variable whose value is to be retrieved.
 * Return: returns NULL to indicate that the environ variable was not found.
 */
char *_getenv(shell_t *data, const char *name)
{
	list_t *node = data->env;
	char *env;

	while (node)
	{
		env = _strstr(node->string, name);
		if (env && *env)
			return (env);
		node = node->next;
	}
	return (NULL);
}

/**
 * _setenv - for setting or updating environment variables within the shell's
 *		environment by either updating an existing node in the linked list
 * or appending a new node.
 * @data: A pointer to the shell data structure,
 *		which contains information about the current state of the shell.
 * @name: the name of the environment variable to set or modify.
 * @value: the value to assign to the environment variable.
 *  Return: returns 0 to indicate success.
 */
int _setenv(shell_t *data, char *name, char *value)
{
	char *buffer = NULL;
	list_t *node;
	char *env;

	if (!name || !value)
		return (0);

	buffer = malloc(_strlen(name) + _strlen(value) + 2);
	if (!buffer)
		return (1);
	_strcpy(buffer, name);
	_strcat(buffer, "=");
	_strcat(buffer, value);
	node = data->env;
	while (node)
	{
		env = _strstr(node->string, name);
		if (env && *env == '=')
		{
			free(node->string);
			node->string = buffer;
			data->modified = 1;
			return (0);
		}
		node = node->next;
	}
	append_node(&(data->env), buffer, 0);
	free(buffer);
	data->modified = 1;
	return (0);
}

/**
 * _unsetenv - provides the functionality to remove a specified env variable
 *		from the shell's environment variables, allowing the user
 *			to remove unwanted variables during the execution of the shell program.
 * @data: A pointer to the shell data structure,
 *		which contains information about the current state of the shell.
 * @name: the name of the environment variable to be unset.
 * Return: returns data->modified, which indicates whether
 *		any modifications were made to the environment variables.
 */
int _unsetenv(shell_t *data, char *name)
{
	list_t *node = data->env;
	size_t counter = 0;
	char *env;

	if (!node || !name)
		return (0);

	while (node)
	{
		env = _strstr(node->string, name);
		if (env && *env == '=')
		{
			data->modified = remove_index(&(data->env), counter);
			counter = 0;
			node = data->env;
			continue;
		}
		node = node->next;
		counter++;
	}
	return (data->modified);
}

/**
 * get_environ - retrieve the environment variables as an array of strings.
 * @data: A pointer to the shell data structure,
 *		which contains information about the current state of the shell.
 * Return: returns the environ field,
 *		which now contains the environment variables as an array of strings.
 */
char **get_environ(shell_t *data)
{
	if (!data->environ || data->modified)
	{
		data->environ = list_to_strings(data->env);
		data->modified = 0;
	}

	return (data->environ);
}

