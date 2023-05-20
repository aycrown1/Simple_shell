#include <stdlib.h>
#include "shell.h"

void *_realloc(void *, unsigned int, unsigned int);
void _free(char **);
int _free2(void **);
/**
 * _realloc - reallocates a block of memory
 * @ptr: pointer to previous malloc'ated block
 * @old_size: byte size of previous block
 * @new_size: byte size of new block
 *
 * Return: pointer to da ol'block nameen.
 */
void *_realloc(void *ptr, unsigned int old_size, unsigned int new_size)
{
	char *point;

	if (!ptr)
		return (malloc(new_size));
	if (!new_size)
		return (free(ptr), NULL);
	if (new_size == old_size)
		return (ptr);

	point = malloc(new_size);
	if (!point)
		return (NULL);

	old_size = old_size < new_size ? old_size : new_size;
	while (old_size--)
		point[old_size] = ((char *)ptr)[old_size];
	free(ptr);
	return (point);
}

/**
 * _free - frees a string of strings
 * @pp: string of strings
 */
void _free(char **pp)
{
	char **a = pp;

	if (!pp)
		return;
	while (*pp)
		free(*pp++);
	free(a);
}

/**
 * _free2 - frees a pointer and NULLs the address
 * @ptr: address of the pointer to free
 *
 * Return: 1 if freed, otherwise 0.
 */
int _free2(void **ptr)
{
	if (ptr && *ptr)
	{
		free(*ptr);
		*ptr = NULL;
		return (1);
	}
	return (0);
}


