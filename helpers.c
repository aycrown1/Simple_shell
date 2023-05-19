#include "shell.h"

void free_data(shell_t *, int);
void _perror(shell_t *, char *);
int decimal(int, int);
char *duplicates(char *, int, int);
int isdelimeter(char, char *);

/**
 * free_data - frees shell_t struct fields
 * @data:  A pointer to the shell data structure, which contains information about the current state of the shell.
 * @all: true if freeing all fields
 */
void free_data(shell_t *data, int all)
{
_free(data->argv);
data->argv = NULL;
data->path = NULL;
if (all)
{
if (!data->Buffer)
free(data->arg);
if (data->env)
free_list(&(data->env));
if (data->alias)
free_list(&(data->alias));
_free(data->environ);
data->environ = NULL;
_free2((void **)data->Buffer);
if (data->file_descriptor > 2)
close(data->file_descriptor);
_putchar(BUF_FLUSH);
}
}

/**
 * _perror - prints an error message
 * @data: A pointer to the shell data structure, which contains information about the current state of the shell.
 * @str: string containing specified error type.
 * Return: 0 if no numbers in string, converted number otherwise
 *-1 on error
 */
void _perror(shell_t *data, char *str)
{

_eputs(data->fname);
_eputs(": ");
decimal(data->count, STDERR_FILENO);
_eputs(": ");
_eputs(data->argv[0]);
_eputs(": ");
_eputs(str);
}

/**
 * decimal - function prints a decimal (integer) number (base 10)
 * @input: the input
 * @fd: the filedescriptor to write to
 *
 * Return: number of characters printed
 */
int decimal(int input, int fd)
{
int (*__putchar)(char) = _putchar;
int i, count = 0;
unsigned int _abs_, current;

if (fd == STDERR_FILENO)
__putchar = _puts2;
if (input < 0)
{
_abs_ = -input;
__putchar('-');
count++;
}
else
_abs_ = input;
current = _abs_;
for (i = 1000000000; i > 1; i /= 10)
{
if (_abs_ / i)
{
__putchar('0' + current / i);
count++;
}
current %= i;
}
__putchar('0' + current);
count++;

return (count);
}

/**
 * duplicates - duplicates characters into a static buffer
 * @pathstr: the PATH string
 * @start: starting index
 * @stop: stopping index
 * Return: a pointer to the duplicated portion in the buffer.
 * used to extract individual paths from the PATH environment variable.
 */
char *duplicates(char *pathstr, int start, int stop)
{
static char buffer[1024];
int i = 0, k = 0;

for (k = 0, i = start; i < stop; i++)
/**
 * By skipping the colon character,
 * the function extracts only the path string without the colon separator.
 */
if (pathstr[i] != ':')
buffer[k++] = pathstr[i];
buffer[k] = 0;
return (buffer);
}

/**
 * isdelimeter - identify delimiter characters in the input and tokenize the command line.
 * It allows the program to identify and separate different parts of a command
 * or argument based on the specified delimiter characters.
 * @character: the input character to check
 * @delimeter: pointer to the delimeter string
 * Return: 1 if true, 0 if false
 */
int isdelimeter(char character, char *delimeter)
{
while (*delimeter)
if (*delimeter++ == character)
return (1);
return (0);
}
