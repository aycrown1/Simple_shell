#include "shell.h"
#include <sys/stat.h>

void parse_command(shell_t *);
void _forks(shell_t *);
int iscommand(shell_t *, char *);
char *find_path(shell_t *, char *, char *);
int _atoi(char *);


/**
 * parse_command - finds a command in PATH
  * responsible for finding and executing a command entered by the user in the shell.
 * @data:  A pointer to the shell data structure,
 * which contains information about the current state of the shell.
 * Return: void
 */
void parse_command(shell_t *data)
{
	char *path = NULL;
	int i, k;

	data->path = data->argv[0];
	if (data->display == 1)
	{
		data->count++;
		data->display = 0;
	}
	for (i = 0, k = 0; data->arg[i]; i++)
		if (!isdelimeter(data->arg[i], " \t\n"))
			k++;
	if (!k)
		return;

	path = find_path(data, _getenv(data, "PATH="), data->argv[0]);
	if (path)
	{
		data->path = path;
		_forks(data);
	}
	else
	{
		if ((interactive(data) || _getenv(data, "PATH=") || data->argv[0][0] == '/') && iscommand(data, data->argv[0]))
			_forks(data);
		else if (*(data->arg) != '\n')
		{
			data->status = 127;
			_perror(data, "not found\n");
		}
	}
}

/**
 * _forks - creates a child process and execute the command in that process.
 * @data:  A pointer to the shell data structure,
 * which contains information about the current state of the shell.
 * Return: void
 */
void _forks(shell_t *data)
{
	pid_t child_pid;

	child_pid = fork();
	if (child_pid == -1)
	{
		perror("Error:");
		return;
	}
	if (child_pid == 0)
	{
		if (execve(data->path, data->argv, get_environ(data)) == -1)
		{
			free_data(data, 1);
			if (errno == EACCES)
				exit(126);
			exit(1);
		}
	}
	else
	{
		wait(&(data->status));
		if (WIFEXITED(data->status))
		{
			data->status = WEXITSTATUS(data->status);
			if (data->status == 126)
				_perror(data, "Permission denied\n");
		}
	}
}

/**
 * iscommand - determines if a file is an executable command
 * @data:  A pointer to the shell data structure,
 * which contains information about the current state of the shell.
 * @path: path to the file
 *
 * Return: 1 if true, 0 otherwise
 */
int iscommand(shell_t *data, char *path)
{
	struct stat st;

	(void)data;
	if (!path || stat(path, &st))
		return (0);

	if (st.st_mode & S_IFREG)
	{
		return (1);
	}
	return (0);
}

/**
 * find_path - finds this cmd in the PATH string
 * @data:  A pointer to the shell data structure,
 * which contains information about the current state of the shell.
 * @pathstr: the PATH string
 * @cmd: the command to find
 * Return: full path of cmd if found or NULL
 */
char *find_path(shell_t *data, char *pathstr, char *cmd)
{
	int i = 0, curr_pos = 0;
	char *path;

	if (!pathstr)
		return (NULL);
	if ((_strlen(cmd) > 2) && _strstr(cmd, "./"))
	{
		if (iscommand(data, cmd))
			return (cmd);
	}
	while (1)
	{
		if (!pathstr[i] || pathstr[i] == ':')
		/* allowing the Simple Shell program
		 to tokenize the PATH variable into individual paths.*/
		{
			path = duplicates(pathstr, curr_pos, i);
			if (!*path)
				_strcat(path, cmd);
			else
			{
				_strcat(path, "/");
				_strcat(path, cmd);
			}
			if (iscommand(data, path))
				return (path);
			if (!pathstr[i])
				break;
			curr_pos = i;
		}
		i++;
	}
	return (NULL);
}

/**
 * _atoi - converts a string to an integer.
 * it handle error conditions, such as invalid characters
 * and integer overflow, and provides a way to differentiate between successful conversions
 * and erroneous situations. 
 * @s: the string to be converted
 * Return: 0 if no numbers in string, converted number otherwise -1 on error
 */
int _atoi(char *s)
{
	int i = 0, sign = 1;
	unsigned long int result = 0;

	/* Check for invalid characters or empty string */
	/* Skip leading whitespace characters */
	while (isspace(s[i]))
		i++;

	/* Check for invalid characters or empty string */
	if (s[i] == '\0')
		return (-1);
	if (s[i] == '+')
	{
		i++;
	}
	for (i = 0; s[i] != '\0'; i++)
	{
		if (s[i] >= '0' && s[i] <= '9')
		{
			/* Check for integer overflow */
			if (result > INT_MAX / 10)
				return (-1);
			result *= 10;
			/*  Check for integer overflow */
			if (result > INT_MAX - (s[i] - '0'))
				return (-1);
			result += (s[i] - '0');
		}
		else
			return (-1);
	}
	return (sign * (int)result);
}
