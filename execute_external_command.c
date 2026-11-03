#include "main.h"

int pid = 0;
int status;

void execute_external_commands(char *input_string)
{
    char *argv[100];
    
    int i, argc = 0;
    int pipe_flag = 0;

    char command_str[50];
    strcpy(command_str, input_string);

    char *token = strtok(command_str, " ");

    while(token != NULL)
    {
        argv[argc++] = token;

        token = strtok(NULL, " ");
    }

    argv[argc] = NULL;

    for (i = 0; argv[i] != NULL; i++)
    {
        if(strcmp(argv[i], "|") == 0)
        {
            pipe_flag = 1;
            break;
        }
    }

    if(pipe_flag == 1)
    {   
        implement_n_pipe(argc, argv);
        return;
    }

    pid = fork();

    if(pid == 0)  // Child
    {
        signal(SIGINT, SIG_DFL);
        signal(SIGTSTP, SIG_DFL);
            
        int ret = execvp(argv[0], argv);

        if(ret == -1)
        {
            perror("Execvp");
        }
    }
    else if(pid > 0)  // Parent
    {
        //wait(NULL);
        waitpid(pid, &status, WUNTRACED);
        pid = 0;
    }
}

void implement_n_pipe(int argc, char *argv[])
{
    int cmd_pos[argc];
    int cmd_count = 1;

    int prev_fd[2], curr_fd[2];

    cmd_pos[0] = 0;

    for (int i = 0; argv[i] != NULL; i++)
    {
        if(strcmp(argv[i], "|") == 0)
        {
            cmd_pos[cmd_count] = i + 1;
            cmd_count++;

            argv[i] = NULL;
        }
    }
    for (int i = 0; i < cmd_count; i++) // loop for each command in pipeline
    {
        if (i < cmd_count - 1) // create pipe except for last command
        {
            if (pipe(curr_fd) == -1) // create a new pipe
            {
                perror("pipe"); // pipe creation failed
                exit(1);
            }
        }

        pid = fork(); // create child process

        if (pid == 0) // child process
        {
            if (i > 0) // not the first command
            {
                dup2(prev_fd[0], 0); // read from previous pipe
                close(prev_fd[0]);   // close read end
                close(prev_fd[1]);   // close write end
            }

            if (i < cmd_count - 1) // not the last command
            {
                dup2(curr_fd[1], 1); // write to current pipe
                close(curr_fd[0]);   // close read end
                close(curr_fd[1]);   // close write end
            }

            execvp(argv[cmd_pos[i]], &argv[cmd_pos[i]]);  // execute command
            
            perror("execvp"); // exec failed
            exit(1);
        }
        else if (pid > 0) // parent process
        {
            if (i > 0) // close previous pipe
            {
                close(prev_fd[0]); // close read end
                close(prev_fd[1]); // close write end
            }

            if (i < cmd_count - 1) // save current pipe as previous
            {
                prev_fd[0] = curr_fd[0]; // store read end
                prev_fd[1] = curr_fd[1]; // store write end
            }
        }
        else // fork failed
        {
            perror("fork");
            exit(1);
        }
    }

    for (int i = 0; i < cmd_count; i++) // wait for all child processes
    {
        wait(NULL);
    }
}