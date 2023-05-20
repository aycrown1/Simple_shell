## Simple shell in C
This project is an ALX SE project that is tasked to create a simple shell in C language.
In creating this program, we have to apply all that we have learnt from the beginning of learning C language.
So this repository contains an implementation of a simple UNIX shell in the C programming language.

### What is a shell?

A shell is a program that serves as an interface between the user and the operating system. It allows users to interact with the operating system by accepting commands and executing them. The shell provides a command-line environment where users can type commands, and it interprets and executes those commands.



### How the Shell program works
#### [EXECUTE COMMAND](execute.c)
* Main Execution Loop `execute()`:
  - The function enters a loop that continues until `r` (input read status initialized to `0`) becomes `-1` or `results` (the result of command execution initialized to `0`) becomes `-2`.
  - Inside the loop, the function performs the following steps:
  - The `clear_data()` function is called to clear the shell data structure (`data`).
  - If the shell is in interactive mode (as determined by `interactive(data))`, it displays the shell prompt (`$ `) using `_puts("$ ")`.
  - Calls the `get_input()` function is called to retrieve input from the user. The return value `r` indicates the status of the input retrieval.
  - If the input is not an error (`r != -1`), it sets the data in the `shell_t` structure and searches for built-in commands using `find_builtin(data)`.
  - If the result of the search is `-1`, it calls `execute_wildcard()` to handle wildcard expansions which is responsible for executing commands that involve wildcard expansions, if no wildcard expansion occurred (`wildcard_expanded()` is `0`), it calls [`parse_command()`](#parse_command) which is responsible for parsing and executing a command to execute the command without wildcard expansion.
  - After the loop, it frees the data using `free_data()` and performs additional cleanup.
  - Depending on the execution mode and status, it may call `exit()` to terminate the program.

#### [HANDLE BUILTINS COMMANDS](finder.c)
* `find_builtin()`: function is responsible for finding and executing a built-in command based on the command name.
  - It defines an array of structures called builtins, where each structure contains the name of a built-in command and a pointer to its corresponding function.
  - It iterates through the builtins array using a for loop and compares the command name `(data->argv[0])` with the name in each structure using the `_strcmp()` function.
  - If a match is found, it increments data->count and executes the corresponding built-in function by calling `builtins[i].func(data)`.
  - It breaks out of the loop after finding a match to avoid unnecessary iterations.
  - Finally, it returns the value obtained from the execution of the built-in function. If no match is found, it returns the initial value of `-1`.
* [builtins](builtin.c): Each built-in function is responsible for executing the specific functionality associated with its command.
  * `exit_builtin()`: Handles the `exit` command and exits the shell.
  * `cd_builtin()`: Handles the `cd` command to change the current directory.
  * `env_builtin()`: Handles the `env` command to print the current environment variables.
  * `setenv_builtin()`: Handles the `setenv` command that sets the value of an environment variable.
  * `unsetenv_builtin()`:  Handles the `unsetenv` command that unsets an environment variable.
  * `alias_builtin()`: Implements the functionality of the `alias` command to define or list aliases for commands.

#### [PARSE AND HANDLE EXTERNAL COMMANDS](parse_command.c)
* `parse_command()`: responsible for parsing and executing a command.
  - If `data->display` is `1`, it increments the data->count variable and sets `data->display` to 0.
  - It iterates through the `data->arg[]` array to count the number of non-delimiter arguments (k variable).
  - If `k` is 0 (no non-delimiter arguments), it returns without further processing.
  - It calls `find_path()` to search for the command's executable path based on the PATH environment variable and the command name.
  - If a valid path is found (path is not NULL), it updates `data->path` with the found path and calls `_forks()` to create a child process and execute the command.
  - If no valid path is found and certain conditions are met (interactive mode, `PATH` environment variable exists, or command starts with '/'), it calls `iscommand()` to check if the command is a valid executable file. If true, it also calls `_forks()` to execute the command.
  - If the command is not found or an error occurs, it sets the `data->status` variable accordingly and prints an error message using `_perror()`.

  * `_forks()`: This function is responsible for creating a child process and executing the command within that process.
   - It uses the `fork()` system call to create a child process.
   - If the `fork()` call fails (`returns -1`), it prints an error message and returns.
   - If the `fork()` call succeeds in the child process (`returns 0`), it uses `execve()` to execute the command specified by `data->path` with the arguments in `data->argv[]` and the environment variables obtained from `get_environ()`.
   - If `execve()` fails (`returns -1`), it frees the data using `free_data()`, checks the specific error condition (`EACCES` for permission denied), and exits with the appropriate status code.
   - In the parent process, it uses `wait()` to wait for the child process to terminate, and then checks the status of the child process.
   - If the child process terminated normally (`WIFEXITED`), it sets `data->status` to the exit status (`WEXITSTATUS`) and prints an error message if necessary.




#### [THE MAIN FUNCTION](main.c)

The program starts execution from the main function in the main.c file. the execution flow for printing and receiving input from the user, including wildcard expansion and built-in functions, until the user exits using the `exit` command, can be explained using the following functions:

  * The function checks if the program is executed with a command-line argument by checking the value of `argc`.
  * If `argc` is equal to `2`, it means a command-line argument is provided.
  * In this case, the function attempts to open the file specified in the command-line argument in read-only mode using `open(argv[1], O_RDONLY)`.
  * It checks if there was an error opening the file by comparing the returned file descriptor (fd) with `-1`.
  * If there was an error opening the file, the function checks specific error conditions.
  * If the error is due to insufficient permissions (`EACCES`), the program exits with status `126`.
  * If the error is due to the file not found (`ENOENT`), it displays an error message and exits with status `127`.
  * If the file is successfully opened, the file descriptor (`fd`) is assigned to the `file_descriptor` field in the `shell_t *data` structure (`data`).
  * The `populate()` function is then called to populate the `shell_t` structure with environment variables.
  * Finally, the `execute()` function is called, passing the data structure and the command-line arguments (`argv`) as arguments. This function executes the shell program using the provided data and command-line arguments.
  * The `main()` function returns `EXIT_SUCCESS` to indicate a successful execution of the program.

### THE FLOWCHART

 



