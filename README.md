# Minishell

A lightweight Unix shell implemented in C, supporting built-in commands, external command execution, piping, job control, and signal handling.

---

## Features

- **Built-in commands**: `echo`, `cd`, `pwd`, `clear`, `exit`, `fg`, `bg`, `jobs`
- **External command execution** via `execvp`
- **Pipeline support** (`|`) — N-stage pipes handled
- **Job control** — stop (`Ctrl+Z`), resume foreground (`fg`) and background (`bg`)
- **Signal handling** — `SIGINT`, `SIGTSTP`, `SIGCHLD`
- **Custom prompt** — change prompt with `PS1=<new_prompt>`
- **Color-coded prompt** using ANSI escape codes

---

## Project Structure

```
minishell/
├── main.c                  # Entry point
├── main.h                  # Header — structs, macros, function declarations
├── scan_input.c            # Input loop, command routing, external command loader
├── execute_internal_cmd.c  # Built-in command implementations
├── execute_external_cmd.c  # External command execution + N-pipe support
├── signal_handler.c        # Signal handler (SIGINT, SIGTSTP, SIGCHLD)
├── insert_at_first.c       # Job list — insert node
├── delete_at_first.c       # Job list — delete node
├── print.c                 # Job list — display
└── external_commands.txt   # List of known external commands
```

---

## Build & Run

```bash
gcc main.c scan_input.c execute_internal_cmd.c execute_external_cmd.c \
    signal_handler.c insert_at_first.c delete_at_first.c print.c -o minishell

./minishell
```

> **Note:** Uses `<stdio_ext.h>` (`__fpurge`) — Linux/glibc only.

---

## Usage Examples

```bash
minishell$ pwd
/home/user

minishell$ echo Hello World
Hello World

minishell$ ls | grep .c
main.c
scan_input.c
...

minishell$ sleep 10        # Press Ctrl+Z to stop
minishell$ jobs
[1]+   Stopped    sleep 10

minishell$ fg              # Resume in foreground
minishell$ bg              # Resume in background

minishell$ PS1=myshell>    # Change prompt
myshell> exit