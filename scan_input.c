#include "main.h"

P_list *head = NULL;
P_list *tail = NULL;

extern int status;
extern int job_count;

char *external_cmd[154];

void scan_input(char *prompt, char *input_string)
{
    system("clear");  // used to clear the terminal

    extract_external_commands(external_cmd);

    signal(SIGINT, signal_handler);  // Registering the signal to signal_handler
    signal(SIGTSTP, signal_handler);
    signal(SIGCHLD, signal_handler);

    while(1)
    {
        bzero(input_string, 50);  // Updating the input string with NULL
        
        fflush(stdout);  // Clearing the Standard output buffer

        printf(ANSI_COLOR_GREEN);
        printf("%s", prompt);
        printf(ANSI_COLOR_RESET);

        scanf("%[^\n]", input_string);  // Get the new prompt from user
        
        __fpurge(stdin);  // Clearing the Standard input buffer

        if(input_string[0] == '\n' || input_string[0] == '\0')
        {
            continue;
        }
        if(strncmp(input_string, "PS1=", 4) == 0)
        {
            // After PS1= it should not have any spaces and atleast one character need to be present
            if(input_string[4] != ' ' && input_string[4] != '\0')
            {
                strcpy(prompt, input_string + 4);
            }
            else
            {
                printf("No command found\n");
            }
        }
        else if(strcmp(input_string, "jobs") == 0)  // Displays all the stopped process list 
        {
            print_list(tail);  
        }
        else if(strcmp(input_string, "fg") == 0)  // Continue the stopped process in Foreground
        {
            if (head == NULL)
            {
                printf("fg : No current job to continue\n");
                continue;
            }

            printf("%s\n", head->command);

            pid_t pid = head->pid;

            kill(pid, SIGCONT);  // Continue the stopped process

            waitpid(pid, &status, WUNTRACED);  // Wait for foreground process

            if (WIFEXITED(status) || WIFSIGNALED(status))  // If process exited, remove from job list 
            {
                delete_at_first(&head, &tail);
                job_count--;
            }
        }
        else if(strcmp(input_string, "bg") == 0)  // Continue the stopped process in Background
        {
            if (head == NULL)
            {
                printf("bg : No current job to continue\n");
                continue;
            }

            printf("[%d]+  %s  &\n", job_count, head->command);

            kill(head->pid, SIGCONT);

            delete_at_first(&head, &tail);
            job_count--;
        }
        else
        {
            char *command = get_command(input_string);

            int res = check_command_type(command);

            if(res == BUILTIN)
            {
                execute_internal_commands(input_string);
            }
            else if(res == EXTERNAL)
            {
                execute_external_commands(input_string);
            }
            else
            {
                printf("%s : Command not found\n", input_string);
            }
        }
    }
}

char *get_command(char *input_string)
{
    char *command = (char *)malloc(50 * sizeof(char));

    int i = 0;

    while(input_string[i] != ' ' && input_string[i] != '\0')
    {
        command[i] = input_string[i];  // Copy till reaches the space
        i++;
    }

    command[i] = '\0';

    return command;
}

int check_command_type(char *command)
{
    // List of all built in Commands
	char *builtins[] = {"echo", "printf", "read", "cd", "clear", "pwd", "pushd", "popd", "dirs", "let", "eval",
						"set", "unset", "export", "declare", "typeset", "readonly", "getopts", "source",
						"exit", "exec", "shopt", "caller", "true", "type", "hash", "bind", "help", NULL};


    for (int i = 0; builtins[i] != NULL; i++)
    {
        if(strcmp(command, builtins[i]) == 0)
        {
            return BUILTIN;
        }
    }

    for (int j = 0; external_cmd[j] != NULL; j++)
    {
        if(strcmp(command, external_cmd[j]) == 0)
        {
            return EXTERNAL;
        }
    }

    return NO_COMMAND;
}

void extract_external_commands(char **external_commands)
{    
    // Opening the txt file in read mode
    int fd = open("external_commands.txt", O_RDONLY);

     if (fd == -1) 
     {
         perror("Error opening file");
         return;
     }

    char str[20], ch;

    for (int i = 0; i < 153; i++)
    {
        int j = 0;

        while (read(fd, &ch, 1) > 0 && ch != '\n')
        {
            str[j++] = ch;
        }

        str[j] = '\0';  // j is used to overcome that '\r' (carriage return)

        external_commands[i] = (char *)malloc(strlen(str) + 1);
        
        strcpy(external_commands[i], str);
    }

    external_commands[152] = NULL;
}