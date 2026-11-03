/*
Name        : MB Gagana
Date        : 29/8/2026
batch       : int26001_005
Description :Mini Shell is a Linux-based command-line interpreter developed using **C programming** to understand Linux Internals and system programming.
It executes built-in and external commands using system calls such as `fork()`, `exec()`, and `wait()`.
The project implements **process creation, process management, foreground/background execution, and signal handling**.
It also performs command parsing and provides an interactive shell environment similar to a basic Linux terminal.

*/

#include "main.h"

char prompt[20] = "minishell$ ";
char input_string[50];

int main()
{
    scan_input(prompt, input_string);

    return 0;
}