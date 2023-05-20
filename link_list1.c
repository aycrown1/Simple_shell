#include "shell.h"

size_t list_size(const list_t *);
char **list_to_strings(list_t *);
list_t *node_prefix(list_t *, char *, char);
ssize_t find_index(list_t *, list_t *);
/**
 * list_size - determines length of linked list
 * @h: pointer to first node
 * Return: size of list
 */
size_t list_size(const list_t *h)
{
	size_t i = 0;

	while (h)
	{
		h = h->next;
		i++;
	}
	return (i);
}

/**
 * list_to_strings - returns an array of strings of the list->string
 * @head: pointer to first node
 * Return: array of strings
 */
char **list_to_strings(list_t *head)
{
	list_t *node = head;
	size_t i = list_size(head), j;
	char **strings;
	char *string;

	if (!head || !i)
		return (NULL);
	strings = malloc(sizeof(char *) * (i + 1));
	if (!strings)
		return (NULL);
	for (i = 0; node; node = node->next, i++)
	{
		string = malloc(_strlen(node->string) + 1);
		if (!string)
		{
			for (j = 0; j < i; j++)
				free(strings[j]);
			free(strings);
			return (NULL);
		}

		string = _strcpy(string, node->string);
		strings[i] = string;
	}
	strings[i] = NULL;
	return (strings);
}

/**
 * node_prefix - returns node whose string starts with prefix
 * @node: pointer to list head
 * @prefix: string to match
 * @c: the next character after prefix to match
 * Return: match node or null
 */
list_t *node_prefix(list_t *node, char *prefix, char c)
{
	char *p = NULL;

	while (node)
	{
		p = _strstr(node->string, prefix);
		if (p && ((c == '\0') || (*p == c)))
			return (node);
		node = node->next;
	}
	return (NULL);
}

/**
 * find_index - gets the index of a node
 * @head: pointer to list head
 * @node: pointer to the node
 * Return: index of node or -1
 */
ssize_t find_index(list_t *head, list_t *node)
{
	size_t i = 0;

	while (head)
	{
		if (head == node)
			return (i);
		head = head->next;
		i++;
	}
	return (-1);
}

