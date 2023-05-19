#include "shell.h"

int execute(shell_t *, char **);
void set_data(shell_t *, char **);
int replace_vars(shell_t *);
int strep(char **, char *);
char *_itoa(long int, int, int);

/**
 * execute -executing commands entered by the user. It handles the input, parsing, and execution of commands.
 * @data: A pointer to the shell data structure, which contains information about the current state of the shell.
 * @argv: array of strings representing the command line arguments from main()
 * Return: returns the results value.
 */
int execute(shell_t *data, char **argv)
{
	ssize_t reads = 0;
	int result = 0;

	while (reads != -1 && result != -2)
	{
		clear_data(data);
		if (interactive(data))
			_puts("$ ");
		_puts2(BUF_FLUSH);
		reads = get_input(data);
		if (reads != -1)
		{
			set_data(data, argv);
			result = find_builtin(data);
			if (result == -1)
				execute_wildcard(data);
		}
		else if (interactive(data))
			_putchar('\n');
		free_data(data, 0);
	}
	free_data(data, 1);
	if (!interactive(data) && data->status)
		exit(data->status);
	if (result == -2)
	{
		if (data->estatus == -1)
			exit(data->status);
		exit(data->estatus);
	}
	return (result);
}

/**
 * set_data - setting up the shell data structure based on the command-line arguments provided to the shell program.
 * @data: A pointer to the shell data structure, which contains information about the current state of the shell.
 * @argv: array of strings representing the command line arguments.
 */
void set_data(shell_t *data, char **argv)
{
	int i = 0;

	data->fname = argv[0];
	if (data->arg)
	{
		data->argv = strtow(data->arg, " \t");
		if (!data->argv)
		{
			data->argv = malloc(sizeof(char *) * 2);
			if (data->argv)
			{
				data->argv[0] = _strdup(data->arg);
				data->argv[1] = NULL;
			}
		}
		for (i = 0; data->argv && data->argv[i]; i++)
		/* an empty statement inside the loop body. */
			;
		data->argc = i;
		replace_alias(data);
		replace_vars(data);
	}
}

/**
 * replace_vars - replacing variables in command arguments with their corresponding values.
 * typically called before executing a command to ensure
 * that any variable references in the command arguments are replaced with their corresponding values.
 * This allows the shell to substitute variables dynamically during command execution.
 * @data: A pointer to the shell data structure, which contains information about the current state of the shell.
 * Return: returns 0 to indicate successful completion.
 */
int replace_vars(shell_t *data)
{
    int i = 0;
    list_t *node;

    for (i = 0; data->argv[i]; i++)
    {
        if (data->argv[i][0] != '$' || !data->argv[i][1])
            continue;
        if (!_strcmp(data->argv[i], "$?"))
        {
            strep(&(data->argv[i]), _strdup(_itoa(data->status, 10, 0)));
            continue;
        }
        if (!_strcmp(data->argv[i], "$$"))
        {
            strep(&(data->argv[i]), _strdup(_itoa(getpid(), 10, 0)));
            continue;
        }
        node = node_prefix(data->env, &data->argv[i][1], '=');
        if (node)
        {
            strep(&(data->argv[i]), _strdup(_strchr(node->string, '=') + 1));
            continue;
        }
        /* If none of the above cases apply, the argument is replaced with an empty string.*/
        strep(&data->argv[i], _strdup(""));
    }

    return (0);
}


/**
 * strep - utility function that helps in replacing the value of a pointer.
 * used in cases where you have a pointer to a string that needs to be updated dynamically with a new value.
 * It simplifies the process of replacing the old string with the new one
 * while correctly handling memory allocation and deallocation.
 * @old: original string that needs to be replaced.
 * @new: the new string that will replace the old one.
 * Return: returns 1 to indicate that the replacement was successful.
 */
int strep(char **old, char *new)
{
	free(*old);
	*old = new;
	return (1);
}

/**
 * _itoa - converts an integer to a string representation in a given base.
 * It resembles the standard library function itoa() (integer to ASCII).
 * @number: number that needs to be converted to a string.
 * @base: base in which the number should be represented as a string.
 * Typically, it can be 2, 8, 10, or 16, corresponding to binary, octal, decimal, or hexadecimal representations.
 * @flags: A set of flags that control the behavior of the function.
 * The flags are represented as bitwise options.
 * Return: returns a pointer to the resulting string.
 */
char *_itoa(long int number, int base, int flags)
{
	static char *array;
	static char buffer[50];
	char sign = 0;
	char *ptr;
	unsigned long num = number;

	if (!(flags & 2) && number < 0)
	{
		num = -number;
		sign = '-';
	}
	array = flags & 1 ? "0123456789abcdef" : "0123456789ABCDEF";
	ptr = &buffer[49];
	*ptr = '\0';

	do
	{
		*--ptr = array[num % base];
		num /= base;
	} while (num != 0);

	if (sign)
		*--ptr = sign;
	return (ptr);
}
