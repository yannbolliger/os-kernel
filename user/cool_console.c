
#include "cool_console.h"


typedef struct {
  void* program_main;
  char program_name[PROGRAM_NAME_MAX + 1];
} program_t;

extern void main_P3();
extern void main_P4();
extern void main_P5();
extern void main_pipe1();
extern void main_pipe2();

#define PROGRAM_NUMBER (5)
program_t programs[PROGRAM_NUMBER] = {
  { &main_P3, "P3" },
  { &main_P4, "P4" },
  { &main_P5, "P5" },
  { &main_pipe1, "pipe1" },
  { &main_pipe2, "pipe2" },
};

void* program_load (char* x) {
  for (int i = 0; i < PROGRAM_NUMBER; ++i) {
    if (0 == strcmp(x, programs[i].program_name))
      return programs[i].program_main;
  }
  return NULL;
}

/* The behaviour of a console process can be summarised as an infinite
 * loop over three main steps, namely
 *
 * 1. write a command prompt then read a command,
 * 2. split the command into space-separated tokens using strtok, then
 * 3. execute whatever steps the command dictates.
 *
 * As is, the console only recognises the following commands:
 *
 * a. execute <program name>
 *
 *    This command will use fork to create a new process; the parent
 *    (i.e., the console) will continue as normal, whereas the child
 *    uses exec to replace the process image and thereby execute a
 *    different (named) program.  For example,
 *
 *    execute P3
 *
 *    would execute the user program named P3.
 *
 * b. terminate <process ID>
 *
 *    This command uses kill to send a terminate or SIG_TERM signal
 *    to a specific process (identified via the PID provided); this
 *    acts to forcibly terminate the process, vs. say that process
 *    using exit to terminate itself.  For example,
 *
 *    terminate 3
 *
 *    would terminate the process whose PID is 3.
 */

void main_cool_console () {
  char* p, x[1024];

  while (1) {
    puts("kernel $ ", 9);
    gets(x, 1024);
    p = strtok(x, " ");

    if (0 == strcmp(p, "execute")) {
      void* program = program_load(strtok(NULL, " "));

      // only fork if valid program requested
      if (NULL != program) {
        pid_t pid = fork();

        // child starts new program
        if (0 == pid) {
          exec(program);
        }
        // parent informs shell user
        else if (pid > 0) {
          char number[3];
          itoa(number, pid);
          puts("Started process with PID: ", 26);
          puts(number, 3);
          puts(".\n", 2);
        }
        // error handling
        else {
          puts("Failed to start process.\n", 25);
        }
      }
      // error handling
      else {
        puts("This program is not available.\n", 31);
      }
    }
    else if (0 == strcmp(p, "terminate")) {
      pid_t pid = atoi(strtok(NULL, " "));
      int s   = atoi(strtok(NULL, " "));

      kill(pid, s);
    }
    else {
      puts("unknown command\n", 16);
    }
  }

  exit(EXIT_SUCCESS);
}
