#include "shell.h"

int interactive(shell_t *);
int populate(shell_t *);
ssize_t input_buffer(shell_t *, char **, size_t *);
void remove_comments(char *buffer);

/**
 * main - acts as the central control point of the program
 * coordinating the execution flow and providing
 * an entry point for the operating system to launch the shell program.
 * @argc: the number of command line arguments passed to the program.
 * @argv: array of strings representing the command line arguments.
 * Return: 0 on success, 1 on error
 */
int main(int argc, char **argv)
{
	shell_t data[] = {SHELL_INIT};
	int fd = 2;

	fd += 3;
	/* Check if the program is executed with a command-line argument */
	if (argc == 2)
	{
		/* Open the file specified in the command-line argument in read-only mode */
		fd = open(argv[1], O_RDONLY);

		/* Check if there was an error opening the file */
		if (fd == -1)
		{
			/* Check the specific error conditions */
			if (errno == EACCES)
				exit(126);
			if (errno == ENOENT)
			{
				_eputs(argv[0]);
				_eputs(": 0: Can't open ");
				_eputs(argv[1]);
				_puts2('\n');
				_puts2(BUF_FLUSH);
				exit(127);
			}

			return (EXIT_FAILURE);
		}

		/**
		 * Set the file descriptor in the shell_t structure
		 * to the opened file descriptor
		 */
		data->file_descriptor = fd;
	}

	/* Populate the shell_t structure with environment variables */
	populate(data);

	/**
	 * Execute the shell program with
	 *the provided data and command-line arguments
	 */
	execute(data, argv);

	return (EXIT_SUCCESS);
}

/**
 * populate - populates env linked list.
 * initializes the shell_t structure's env field with environment variables.
 *	and creating a linked list of environment variables
 *		by iterating through the environ array
 * and appending each environment variable to the linked list.
 * @data:  A pointer to the shell data structure,
 *		which contains information about the current state of the shell.
 * Return: returns 0 to indicate success.
 */
int populate(shell_t *data)
{
	list_t *node = NULL;
	size_t i;

	/* Iterate through the environ array */
	for (i = 0; environ[i]; i++)
		append_node(&node, environ[i], 0);

	/**
	 * Assign the linked list of environment variables
	 * to the shell structure's env field
	 */
	data->env = node;

	return (0);
}

/**
 * interactive - checks whether the current shell is running in
 *		an interactive mode.
 *	It allows users to interact with the shell, input commands,
 * and receive output and error messages in real-time.
 * @data:  A pointer to the shell data structure,
 *		which contains information about the current state of the shell.
 * Return: 1 if interactive mode, 0 otherwise
 */
int interactive(shell_t *data)
{
	return (isatty(STDIN_FILENO) && data->file_descriptor <= 2);
}

/**
 * input_buffer - reading input from the user.
 * handle reading input from the user and storing it in a buffer.
 * provides functionality to handle cases
 *		when the buffer is empty and needs to be filled with new input.
 * @data:  A pointer to the shell data structure,
 *		which contains information about the current state of the shell.
 * @buffer: pointer holds the input buffer.
 * @len: the length of the buffer.
 * Return: number of characters read from the input.
 */
ssize_t input_buffer(shell_t *data, char **buffer, size_t *len)
{
	ssize_t read = 0;
	size_t len_p = 0;

	if (!*len) /* if nothing left in the buffer, fill it */
	{
		free(*buffer);
		*buffer = NULL;
		signal(SIGINT, sigintHandler);
/* 1 if using system getline() */
#define USED_GETLINE 0
#if USED_GETLINE
		read = getline(buffer, &len_p, stdin);
#else
		read = _getline(data, buffer, &len_p);
#endif
		if (read > 0)
		{
			if ((*buffer)[read - 1] == '\n')
			{
				(*buffer)[read - 1] = '\0';
				/* remove trailing newline */
				read--;
			}
			data->display = 1;
			remove_comments(*buffer);
			{
				*len = read;
				data->Buffer = buffer;
			}
		}
	}
	return (read);
}

/**
 * remove_comments - function replaces first instance of '#' with '\0'
 * @buffer: address of the string to modify
 *
 * Return: void
 */
void remove_comments(char *buffer)
{
	int i;

	/* Iterate through the characters in the buffer */
	for (i = 0; buffer[i] != '\0'; i++)
	{
		/**
		 * Check if the current character is '#' and either
		* it is at the beginning of the line or preceded by a space
		*/
		if (buffer[i] == '#' && (!i || buffer[i - 1] == ' '))
		{
			buffer[i] = '\0';
			/* Replace '#' with null character to effectively remove the comment */
			break;
			/* Stop iterating once a comment is found and removed */
		}
	}
}
