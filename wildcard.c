#include "shell.h"

/**
 * expand_wildcards - expands wildcards in a string
 *		and returns an array of strings.
 * @arg: A pointer to a string, the argument that may contain wildcard chars.
 * Return: returns the original argument as a single element array.
 */
char **expand_wildcards(char *arg)
{
	glob_t glob_result;
	int flags = 0;
	char **result = NULL;
	int i, n;

	if (glob(arg, flags, NULL, &glob_result) != 0)
	{
		result = malloc(sizeof(char *) * 2);
		result[0] = arg;
		result[1] = NULL;
	}
	else
	{
		n = glob_result.gl_pathc;
		result = malloc(sizeof(char *) * (n + 1));
		for (i = 0; i < n; i++)
		{
			result[i] = strdup(glob_result.gl_pathv[i]);
		}
		result[n] = NULL;
		globfree(&glob_result);
	}
	return (result);
}

/**
 * execute_wildcard - expands wildcard chars (* and ?) in command-line argument
 *		and executing the corresponding commands.
 * @data: A pointer to the shell data structure,
 *		which contains information about the current state of the shell.
* Return: If no wildcard expansion occurs for any argument,
 * the function simply calls the parse_command() function
 *				to execute the command as it is.
 */
void execute_wildcard(shell_t *data)
{
	int i, j = 0;
	int wildcard_expanded = 0;
	char **expanded = NULL;

	for (i = 0; data->argv[i]; i++)
	{
		if (strchr(data->argv[i], '*') != NULL || strchr(data->argv[i], '?') != NULL)
		{
			expanded = expand_wildcards(data->argv[i]);
			while (expanded[j] != NULL)
			{
				data->argv[i] = expanded[j];
				wildcard_expanded = 1;
				parse_command(data);
				j++;
			}
			free(expanded);
		}
	}
	/* Execute the command if no wildcard expansion occurred */
	if (!wildcard_expanded)
	{
		parse_command(data);
	}
}

