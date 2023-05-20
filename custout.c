#include "shell.h"

void _eputs(char *);
int _puts2(char);
int _write(char, int);
void _puts(char *);
int _putchar(char);

/**
 * _eputs - Prints an error message to the standard error output (stderr)
 * instead of the standard output stream (stdout), allowing
 *				for effective error handling and informative output in the shell program.
 * @massage: the string massage to be printed
 * Return: void
 */
void _eputs(char *massage)
{
	int i = 0;

	if (!massage)
		return;
	/* returns without performing any further actions. */
	while (massage[i] != '\0')
	{
		_puts2(massage[i]);
		i++;
	}
}

/**
 * _puts2 - a modified version of the puts() function
 * used for writing characters to the standard error stream (file descriptor 2)
 * @c: A character to be written to the file descriptor.
 * Return: On success 1.
 * On error, -1 is returned, and errno is set appropriately.
 */
int _puts2(char c)
{
	static int i;
	static char buffer[WRITE_BUF_SIZE];

	if (c == (char)BUF_FLUSH || i >= WRITE_BUF_SIZE)
	{
		write(2, buffer, i);
		i = 0;
	}
	if (c != (char)BUF_FLUSH)
		buffer[i++] = c;
	return (1);
}

/**
 * _write - a helper function used to write a character to a file descriptor.
 * It resembles the write() system call.
 * @c: A character to be written to the file descriptor.
 * @fd: the file descriptor to which the character will be written.
 * Return: always returns 1,
 *				which is typically used to indicate success or that data was written.
 */
int _write(char c, int fd)
{
	static int position;
	static char buffer[WRITE_BUF_SIZE];

	if (c == '\0' || position >= (int)WRITE_BUF_SIZE)
	{
		write(fd, buffer, position);
		position = 0;
		/* resets the position back to 0. */
	}
	if (c != '\0')
		buffer[position++] = c;
	return (1);
}

/**
 * _puts - print a string to the standard output.
 * @string: A pointer that points to the beginning of the string to be printed.
 * Return: void.
 */
void _puts(char *string)
{
	int i = 0;

	if (!string)
		return;
	while (string[i] != '\0')
	{
		_putchar(string[i]);
		i++;
	}
}

/**
 * _putchar - a custom implementation of the putchar() function.
 * It writes a single character to the standard output.
 * @c: A character to be written to the standard output.
 * Return: returns 1 to indicate that the character has been successfully
 *									written.
 */
int _putchar(char c)
{
	static int i;
	static char buffer[WRITE_BUF_SIZE];

	if (c == '\0' || i >= WRITE_BUF_SIZE)
	{
		write(1, buffer, i);
		i = 0;
	}
	if (c != '\0')
		buffer[i++] = c;
	return (1);
}


