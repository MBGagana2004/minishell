#include "main.h"

extern int status;

void execute_internal_commands(char *input_string)
{
    if(strncmp(input_string, "exit", 4) == 0)
    {
        exit(0);  // Exiting from the program
    }
    else if(strncmp(input_string, "pwd", 3) == 0)
    {
        char path[100];

        getcwd(path, sizeof(path));

        printf("%s\n", path);  // Prints the current working directory 
    }
    else if(strncmp(input_string, "cd", 2) == 0)
    {
        chdir(input_string + 3);  // Change the directory
    }
    else if(strncmp(input_string, "echo", 4) == 0)
    {
        echo(input_string + 5, status);
    }
    else if(strcmp(input_string, "clear") == 0)
    {
        system("clear");  // Clear's the terminal
    }
}

void echo(char *input_string, int status)
{
    if(strcmp(input_string, "$SHELL") == 0)
    {
        printf("%s\n", getenv("SHELL"));  // Prints the shell 
    }
    else if(strcmp(input_string, "$$") == 0)
    {
        printf("%d\n", getpid());  // Pid of the process
    }
    else if(strcmp(input_string, "$?") == 0)
    {
        printf("%d\n", WIFEXITED(status));  // Exit status of the previous process
    }
    else
    {
        printf("%s\n", input_string);
    }
}