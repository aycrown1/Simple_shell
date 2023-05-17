#include "shell.h"

/**
 * alias_builtin - implements the functionality of the alias command.
 *		It allows the user to define or list aliases for commands.
 * @data: A pointer to the shell data structure
 *		which contains information about the current state of the shell.
 *  Return: returns 0 to indicate successful completion.
 */
int alias_builtin(shell_t *data)
{
	int i = 0;
	char *position = NULL;
	list_t *node = NULL;

	if (data->argc == 1)
	{
		node = data->alias;
		while (node)
		{
			print_alias(node);
			node = node->next;
		}
		return (0);
	}
	for (i = 1; data->argv[i]; i++)
	{
		position = _strchr(data->argv[i], '=');
		if (position)
			set_alias(data, data->argv[i]);
		else
			print_alias(node_prefix(data->alias, data->argv[i], '='));
	}

	return (0);
}

/**
 * unset_alias - responsible for removing an alias from the alias list.
 *		It resembles the unset command in a shell.
 * @data: A pointer to the shell data structure,
*		which contains information about the current state of the shell.
 * @string: the alias to be unset.
 * Return: returns the value returned by remove_index(),
 *		which indicates the success or failure of the removal operation.
 */
int unset_alias(shell_t *data, char *string)
{
	char *point, store;
	int ret;

	point = _strchr(string, '=');
	/* to find the equals sign (=) in the string*/
	if (!point)
		return (1);
	store = *point;
	*point = 0;
	ret = remove_index(&(data->alias),
		 find_index(data->alias,
		node_prefix(data->alias, string, -1)));
	*point = store;
	return (ret);
}

/**
 * set_alias - sets an alias for a command.
 *		It takes a string in the format alias_name=command
 *		and adds it to the alias list in the shell data structure.
 * @data: A pointer to the shell data structure,
 *		which contains information about the current state of the shell.
 * @string: the alias to be set.
 * Return: returns the result of calling the append_node() function
 *		to add the new alias to the alias list.
 *		Finally, the function returns 0 to indicate successful completion.
 */
int set_alias(shell_t *data, char *string)
{
	char *point;

	point = _strchr(string, '=');
	if (!point)
		return (1);
	if (!*++point)
		return (unset_alias(data, string));

	unset_alias(data, string);
	return (append_node(&(data->alias), string, 0) == NULL);
}

/**
 * print_alias - prints the name and value of an alias.
 * @node: A pointer to a linked list node that represents an alias.
 * The node contains a string in the format alias_name=alias_value.
 * Return: returns 0 if it successfully prints the alias
 *		or returns 1 if the input node parameter is NULL.
 */
int print_alias(list_t *node)
{
	char *point = NULL, *index = NULL;

	if (node)
	{
		point = _strchr(node->string, '=');
		for (index = node->string; index <= point; index++)
			_putchar(*index);
		_putchar('\'');
		_puts(point + 1);
		_puts("'\n");
		return (0);
	}
	return (1);
}

/**
 * replace_alias - replaces the command name in the data->argv[0] argument
 * with the corresponding alias value, if an alias exists for that command.
 * @data: A pointer to the shell data structure
 *	which contains information about the current state of the shell.
 * Return: returns 1 to indicate that the alias replacement was successful.
 */
int replace_alias(shell_t *data)
{
	int index;
	list_t *node;
	char *point;

	for (index = 0; index < 10; index++)
	{
		node = node_prefix(data->alias, data->argv[0], '=');
		if (!node)
			return (0);
		free(data->argv[0]);
		point = _strchr(node->string, '=');
		if (!point)
			return (0);
		point = _strdup(point + 1);
		if (!point)
			return (0);
		data->argv[0] = point;
	}
	return (1);
}

