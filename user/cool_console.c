
#include "cool_console.h"

#define DESC_MAX (127)

typedef struct {
  void (*fn)(void);
  char name[PROGRAM_NAME_MAX + 1];
  char desc[DESC_MAX + 1];
} fn_name_t;

/**
 * COMMANDS
 * Available commands in the shell.
 * Implemented below.
 */
void cmd_execute();
void cmd_terminate();
void cmd_nice();
void cmd_help();

#define COMMAND_NUMBER (4)
fn_name_t commands[COMMAND_NUMBER] = {
  { &cmd_execute,   "execute", " <program> : Execute one of the available programs."},
  { &cmd_terminate, "terminate", " <pid>   : Terminate the process."},
  { &cmd_nice,      "nice", " <pid> <prio> : Change the priority of the process."},
  { &cmd_help,      "help", "              : Prints this message."},
};

/**
 * PROGRAMS
 * Available programs for the execute command.
 */
extern void main_P3();
extern void main_P4();
extern void main_P5();
extern void main_pipe1();
extern void main_pipe2();
extern void main_philosophers();

#define PROGRAM_NUMBER (6)
fn_name_t programs[PROGRAM_NUMBER] = {
  { &main_P3, "P3", "" },
  { &main_P4, "P4", "" },
  { &main_P5, "P5", "" },
  { &main_pipe1, "pipe1", "" },
  { &main_pipe2, "pipe2", "" },
  { &main_philosophers, "philosophers", "" },
};

void* fn_load(fn_name_t* fn_names, size_t fn_number, char* x) {
  for (int i = 0; i < fn_number; ++i) {
    if (0 == strcmp(x, fn_names[i].name))
      return fn_names[i].fn;
  }
  return NULL;
}

void print_process_pid(pid_t pid) {
  puts(" process with PID: ", 19);

  char number[3] = {0};
  itoa(number, pid);
  puts(number, 3);

  puts(".\n", 2);
}

void cmd_execute() {
  void* program = fn_load(programs, PROGRAM_NUMBER, strtok(NULL, " "));

  // only fork if valid program requested
  if (NULL != program) {
    pid_t pid = fork();

    // child starts new program
    if (0 == pid) {
      exec(program);
    }
    // parent informs shell user
    else if (pid > 0) {
      puts("Started", 7);
      print_process_pid(pid);
    }
    // error handling
    else {
      puts("Failed to start the process.\n", 29);
    }
  }
  // error handling
  else {
    puts("This program is not available.\n", 31);
  }
}

void cmd_terminate() {
  pid_t pid = atoi(strtok(NULL, " "));
  int s   = atoi(strtok(NULL, " "));

  int err = kill(pid, s);

  if (err) {
    puts("Failed to terminate", 19);
    print_process_pid(pid);
  }
  else {
    puts("Terminated", 10);
    print_process_pid(pid);
  }
}

void cmd_help() {
  puts("Available commands:\n\n", 21);

  for (int i = 0; i < COMMAND_NUMBER; ++i) {
    puts(commands[i].name, PROGRAM_NAME_MAX);
    puts(commands[i].desc, DESC_MAX);
    puts("\n", 1);
  }

  puts("\n", 1);
}

void cmd_nice() {
  pid_t pid = atoi(strtok(NULL, " "));
  int prio  = atoi(strtok(NULL, " "));

  int err = kill(pid, prio);

  if (err) puts("Failed to change", 16);
  else puts("Changed", 7);

  puts(" the nice value of", 18);
  print_process_pid(pid);
}

void main_cool_console () {
  char* p, x[1024];

  puts("\n\n*********************************************************\n", 60);
  puts("Welcome to the console of the OS kernel by Yann Bolliger!\n", 59);
  puts("--\n", 3);
  puts("Type help for a list of commands.\n\n", 35);

  while (1) {
    puts("kernel $ ", 9);
    gets(x, 1024);
    p = strtok(x, " ");

    void (*command)(void) = fn_load(commands, COMMAND_NUMBER, p);

    if (command) command();
    else puts("Command not found.\n", 19);
  }

  exit(EXIT_SUCCESS);
}
