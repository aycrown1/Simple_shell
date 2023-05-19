#include "shell.h"

list_t *append_node(list_t **, const char *, int);
size_t print_strnode(const list_t *);
int remove_index(list_t **, unsigned int);
void free_list(list_t **);

/**
 * append_node - adds a node to the end of the list
 * @head: address of pointer to head node
 * @string: string field of node
 * @num: node index used by history
 * Return: size of list
 */
list_t *append_node(list_t **head, const char *string, int num)
{
list_t *new_node, *node;

if (!head)
return (NULL);

node = *head;
new_node = malloc(sizeof(list_t));
if (!new_node)
return (NULL);
memset((void *)new_node, 0, sizeof(list_t));
new_node->num = num;
if (string)
{
new_node->string = _strdup(string);
if (!new_node->string)
{
free(new_node);
return (NULL);
}
}
if (node)
{
while (node->next)
node = node->next;
node->next = new_node;
}
else
*head = new_node;
return (new_node);
}

/**
 * print_strnode - prints only the string value of a list_t linked list
 * @h: pointer to first node
 * Return: size of list
 */
size_t print_strnode(const list_t *h)
{
size_t i = 0;

while (h)
{
_puts(h->string ? h->string : "(nil)");
_puts("\n");
h = h->next;
i++;
}
return (i);
}

/**
 * remove_index - deletes node at given index
 * @head: address of pointer to first node
 * @index: index of node to delete
 * Return: 1 on success, 0 on failure
 */
int remove_index(list_t **head, unsigned int index)
{
list_t *current, *previous;
unsigned int i = 0;

if (!head || !*head)
return (0);

if (!index)
{
current = *head;
*head = (*head)->next;
free(current->string);
free(current);
return (1);
}
current = *head;
while (current)
{
if (i == index)
{
previous->next = current->next;
free(current->string);
free(current);
return (1);
}
i++;
previous = current;
current = current->next;
}
return (0);
}

/**
 * free_list - frees all nodes of a list
 * @head_ptr: address of pointer to head node
 * Return: void
 */
void free_list(list_t **head_ptr)
{
list_t *node, *next_node, *head;

if (!head_ptr || !*head_ptr)
return;
head = *head_ptr;
node = head;
while (node)
{
next_node = node->next;
free(node->string);
free(node);
node = next_node;
}
*head_ptr = NULL;
}
