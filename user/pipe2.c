
#include <string.h>

#include "libc.h"


void main_pipe1() {
  int fd[2] = {0};

  // open pipe
  int err = pipe(fd);
  if (err) {
    write(STDOUT_FILENO, "Failed to open a pipe.\n", 23);
    exit(EXIT_FAILURE);
  }
  else write(STDOUT_FILENO, "Opened a pipe.\n", 15);

  #define OUT_LENGTH 20
  char output[OUT_LENGTH] = {0};

  // read empty pipe
  int bytes_read = read(fd[0], output, OUT_LENGTH);
  if (bytes_read != 0) {
    write(STDOUT_FILENO, "Failed to read from empty pipe.\n", 32);
    exit(EXIT_FAILURE);
  }

  #define IN_LENGTH 7
  const char input[IN_LENGTH] = "Tester\n";

  // fill pipe completely
  int bytes_written = 1;
  while (bytes_written > 0) {
    bytes_written = write(fd[1], input, IN_LENGTH);
  }

  if (bytes_written == 0) write(STDOUT_FILENO, "Filled pipe.\n", 13);
  else {
    write(STDOUT_FILENO, "Failed to fill pipe.\n", 21);
    exit(EXIT_FAILURE);
  }

  // read some of it
  bytes_read = read(fd[0], output, OUT_LENGTH);
  if (bytes_read != OUT_LENGTH) {
    write(STDOUT_FILENO, "Failed to read from pipe.\n", 26);
    exit(EXIT_FAILURE);
  }

  // fill again
  bytes_written = write(fd[1], "Write again something but too much.", 35);
  if (bytes_written != OUT_LENGTH) {
    write(STDOUT_FILENO, "Failed to fill pipe again.\n", 27);
    exit(EXIT_FAILURE);
  }

  // read all of it and write it to STDOUT as a check
  while (bytes_read > 0) {
    bytes_read = read(fd[0], output, OUT_LENGTH);
    write(STDOUT_FILENO, output, bytes_read);
  }

  if (bytes_read == 0) exit(EXIT_SUCCESS);
  else {
    write(STDOUT_FILENO, "Failed to read entire pipe.\n", 28);
    exit(EXIT_FAILURE);
  }
}
