#include "shell.h"

ssize_t get_input(shell_t *);
int chained(shell_t *, char *, size_t *);
void check_chain(shell_t *, char *, size_t *, size_t, size_t);
int _getline(shell_t *, char **, size_t *);
ssize_t read_buffer(shell_t *data, char *buffer, size_t *i);

/**
 * get_input - gets a line minus the newline
 * @data:  A pointer to the shell data structure,
 * which contains information about the current state of the shell.
 * Return: bytes read
 */
ssize_t get_input(shell_t *data)
{
	static char *buffer;
	/* the ';' command chain buffer */
	static size_t i, j, len;
	ssize_t r = 0;
	char **buffer_p = &(data->arg), *p;

	_putchar(BUF_FLUSH);
	r = input_buffer(data, &buffer, &len);
	if (r == -1) /* EOF */
		return (-1);
	if (len) /* we have commands left in the chain buffer */
	{
		j = i;
		/* init new iterator to current buffer position */
		p = buffer + i; /* get pointer for return */

		check_chain(data, buffer, &j, i, len);
		while (j < len) /* iterate to semicolon or end */
		{
			if (chained(data, buffer, &j))
				break;
			j++;
		}

		i = j + 1;	  /* increment past nulled ';'' */
		if (i >= len) /* reached end of buffer? */
		{
			i = len = 0; /* reset position and length */
			data->type = 0;
		}

		*buffer_p = p;	/* pass back pointer to current command position */
		return (_strlen(p)); /* return length of current command */
	}

	*buffer_p = buffer; /* else not a chain, pass back buffer from _getline() */
	return (r);			/* return length of buffer from _getline() */
}

/**
 * chained - checks if the current char in buffer is a chain delimeter
 * (specifically, whether it represents the "||"
 * or "&&" logical operators, or the ";" separator).
 * @data:  A pointer to the shell data structure,
 * which contains information about the current state of the shell.
 * @buffer: the char buffer
 * @p: address of current position in buffer
 * Return: 1 if chain delimeter, 0 otherwise
 */
int chained(shell_t *data, char *buffer, size_t *p)
{
	size_t j = *p;

	if (buffer[j] == '|' && buffer[j + 1] == '|')
	{
		buffer[j] = 0;
		j++;
		data->type = 1;
	}
	else if (buffer[j] == '&' && buffer[j + 1] == '&')
	{
		buffer[j] = 0;
		j++;
		data->type = 2;
	}
	else if (buffer[j] == ';') /* found end of this command */
	{
		buffer[j] = 0; /* replace semicolon with null */
		data->type = 3;
	}
	else
		return (0);
	*p = j;
	return (1);
}

/**
 * check_chain - responsible for checking the chain of commands
 *	and determining the position in the buffer
 *		where the command chain should be terminated.
 * @data:  A pointer to the shell data structure,
 * which contains information about the current state of the shell.
 * @buffer: the input buffer containing the command chain.
 * @current: address of current position in buffer
 * @i: the current index being checked in the buffer.
 * @len: the length of the buffer.
 * Return: Void
 */
void check_chain(shell_t *data, char *buffer,
		size_t *current, size_t i, size_t len)
{
	size_t position = *current;

	if (data->type == 2)
	{
		if (data->status)
		{
			buffer[i] = 0;
			position = len;
		}
	}
	if (data->type == 1)
	{
		if (!data->status)
		{
			buffer[i] = 0;
			position = len;
		}
	}

	*current = position;
}

/**
 * _getline - a utility function used by the shell program
 * to read input from the user and manage the input buffer dynamically.
 * @data:  A pointer to the shell data structure,
 * which contains information about the current state of the shell.
 * @ptr: address of pointer to buffer, preallocated or NULL
 * @length: the length of the input buffer.
 * Return:  returns the length of the newly read line.
 */
int _getline(shell_t *data, char **ptr, size_t *length)
{
	static char buffer[READ_BUF_SIZE];
	static size_t index, len;
	size_t sets;
	ssize_t value = 0, clength = 0;
	char *pointer = NULL, *new = NULL, *c;

	pointer = *ptr;
	if (pointer && length)
		clength = *length;
	if (index == len)
		index = len = 0;

	value = read_buffer(data, buffer, &len);
	if (value == -1 || (value == 0 && len == 0))
		return (-1);

	c = _strchr(buffer + index, '\n');
	sets = c ? 1 + (unsigned int)(c - buffer) : len;
	new = _realloc(pointer, clength, clength ? clength + sets : sets + 1);
	if (!new)
		return (pointer ? free(pointer), -1 : -1);

	if (clength)
		_strncat(new, buffer + index, sets - index);
	else
		_strncpy(new, buffer + index, sets - index + 1);

	clength += sets - index;
	index = sets;
	pointer = new;

	if (length)
		*length = clength;
	*ptr = pointer;
	return (clength);
}

/**
 * read_buffer - responsible for reading input
 *		from the file descriptor into a buffer.
 * @data:  A pointer to the shell data structure,
 *		which contains information about the current state of the shell.
 * @buffer: A character buffer where the input will be stored.
 * @i: A pointer that keeps track of the current position in the buffer.
 * Return: returns the value of reading,
 * which represents the number of bytes read from the file descriptor.
 */
ssize_t read_buffer(shell_t *data, char *buffer, size_t *i)
{
	ssize_t reading = 0;

	if (*i)
		return (0);
	reading = read(data->file_descriptor, buffer, READ_BUF_SIZE);
	if (reading >= 0)
		*i = reading;
	return (reading);
}

