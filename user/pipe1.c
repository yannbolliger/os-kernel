
#include <string.h>

#include "libc.h"


void main_pipe1() {
  int fd[2] = {0};

  write(STDOUT_FILENO, "Opening a pipe.\n", 16);
  int err = pipe(fd);

  if (err) {
    write(STDOUT_FILENO, "Failed to open a pipe.\n", 23);
    exit(EXIT_FAILURE);
  }

  #define LENGTH 35
  const char input[LENGTH] = "Writing this content to the pipe.\n";

  write(STDOUT_FILENO, "Opened a pipe.\n", 15);
  write(STDOUT_FILENO, input, LENGTH);

  err = write(fd[1], input, LENGTH);
  if (err != LENGTH) {
    write(STDOUT_FILENO, "Failed to write to pipe.\n", 25);
    exit(EXIT_FAILURE);
  }

  char output[LENGTH] = {0};
  err = read(fd[0], output, LENGTH);
  if (err != LENGTH) {
    write(STDOUT_FILENO, "Failed to read to pipe.\n", 24);
    exit(EXIT_FAILURE);
  }

  if (0 == strcmp(input, output)) {
    write(STDOUT_FILENO, "Successfully read from pipe.\n", 29);
    exit(EXIT_SUCCESS);
  }
  else {
    write(STDOUT_FILENO, "Content is not equal.\n", 22);
    exit(EXIT_FAILURE);
  }
}
