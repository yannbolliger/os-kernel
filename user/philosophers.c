

//#include "libc.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#define PHIL_NUMBER (16)
#define ROUNDS (10)

typedef enum {
  FIRST_FORK,
  SECOND_FORK
} fork_t;

int fork_pipes[PHIL_NUMBER * 2];
int fork_pipes_read[2];
int fork_pipes_write[2];

int id;

const char* names[PHIL_NUMBER] = {
  "Socrates",
  "Aristoteles",
  "Descartes",
  "Kant",
  "Spinoza",
  "Rousseau",
  "Hobbes",
  "Plato",
  "Camus",
  "Adams",
  "P11",
  "P12",
  "P13",
  "P14",
  "P15",
  "P16",
};

const char* name() {
  return names[id];
}

#define TOPIC_NUMBER (7)
const char* topics[TOPIC_NUMBER] = {
  "mathematics", "death", "life", "his parents", "a friend", "pizza", "deadlock"
};

#define RAND_MAX (20)
const int rand_int[RAND_MAX] = {
  13, 7, 3, 19, 0, 5, 11, 12, 16, 4, 2, 9, 18, 15, 6, 14, 8, 10, 17, 1,
};

int rand_index;
int rand_next() {
  rand_index = (rand_index + id) % RAND_MAX;
  return rand_int[rand_index];
}

void close_all_except_own_fork_pipes() {
  // write to own fork e.g. put own fork on the table
  write(fork_pipes[id*2 + 1], "X", 1);

  // assign own fork_pipes
  if (PHIL_NUMBER - 1 == id) {
    printf("%s is leftie.\n", name());
    fork_pipes_read[FIRST_FORK] = fork_pipes[0];
    fork_pipes_write[FIRST_FORK] = fork_pipes[1];

    fork_pipes_read[SECOND_FORK] = fork_pipes[id*2];
    fork_pipes_write[SECOND_FORK] = fork_pipes[id*2 + 1];
  }
  else {
    printf("%s is rightie.\n", name());
    fork_pipes_read[FIRST_FORK]   = fork_pipes[id*2];
    fork_pipes_write[FIRST_FORK]  = fork_pipes[id*2 + 1];
    fork_pipes_read[SECOND_FORK]  = fork_pipes[id*2 + 2];
    fork_pipes_write[SECOND_FORK] = fork_pipes[id*2 + 3];
  }

  // close all others
  for (size_t i = 0; i < PHIL_NUMBER; i++) {
    if (i != id && i != (id + 1) % PHIL_NUMBER) {
      close(fork_pipes[2*i]);
      close(fork_pipes[2*i + 1]);
    }
  }
}

void pseudo_wait(const size_t duration) {
  for (size_t i = 0; i < (duration << 10); i++) {
    double a = duration * 555.555;
    double b = i * 77.7777;
    size_t c = a * b;
  }
  sleep(duration/10);
}

void think() {
  pseudo_wait(rand_next());

  const char* topic = topics[rand_next() % TOPIC_NUMBER];

  printf("%s", name());
  printf(" thinks about ");
  printf("%s", topic);
  printf(".\n");
}

int picked_up(const fork_t fork) {
  char x;
  int bytes_read = read(fork_pipes_read[fork], &x, 1);

  if (bytes_read < 0) {
    printf("Problem with reading from pipe.\n");
    exit(EXIT_FAILURE);
  }
  else return bytes_read;
}

void put_down(const fork_t fork) {
  write(fork_pipes_write[fork], "X", 1);
}

void eat() {

  while (!picked_up(FIRST_FORK)) {};
  printf("%s has one fork.\n", name());

  while (!picked_up(SECOND_FORK)) {};
  printf("%s has both forks and starts to eat!\n", name());

  pseudo_wait(rand_next());

  printf("%s releases its two forks.\n", name());
  put_down(FIRST_FORK);
  put_down(SECOND_FORK);
}

void philosophize(const int p_id) {
  id = p_id;
  close_all_except_own_fork_pipes();

  for (size_t i = 0; i < ROUNDS; i++) {
    think();
    eat();
  }
  printf("%s is done with %d rounds of philosophizing.\n", name(), ROUNDS);
  exit(EXIT_SUCCESS);
}

int main() {

  for (size_t i = 0; i < PHIL_NUMBER; i++) {
    int err = pipe(fork_pipes + (i * 2));
    if (err) {
      printf("Failed to create 16 forks.\n");
      exit(EXIT_FAILURE);
    }
  }

  for (size_t i = 0; i < PHIL_NUMBER; i++) {
    pid_t pid = fork();

    if (0 == pid) philosophize(i);
    else if (pid < 0) {
      printf("Failed to fork 16 philosophers.\n");
      exit(EXIT_FAILURE);
    }
  }

  exit(EXIT_SUCCESS);
}
