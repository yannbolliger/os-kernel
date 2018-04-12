
#include "philosophers.h"

/**
 * Entire list of pipe fds used to initialise.
 */
int fork_pipes[PHIL_NUMBER * 2];

/**
 * Particular pipe fds of a single child process (philosopher).
 */
int fork_pipes_read[2];
int fork_pipes_write[2];

/**
 * ID of a particular philosopher.
 */
int id;

#define PHIL_NAME_LENGTH (11)
const char* names[PHIL_NUMBER] = {
  "Socrates   ",
  "Aristoteles",
  "Descartes  ",
  "Kant       ",
  "Spinoza    ",
  "Rousseau   ",
  "Hobbes     ",
  "Plato      ",
  "Camus      ",
  "Adams      ",
  "P11        ",
  "P12        ",
  "P13        ",
  "P14        ",
  "P15        ",
  "P16        ",
};

const char* name() {
  return names[id];
}

#define TOPIC_NUMBER (7)
#define TOPIC_LENGTH (12)
const char* topics[TOPIC_NUMBER] = {
  "mathematics.",
  "death.      ",
  "life.       ",
  "his parents.",
  "a friend.   ",
  "pizza.      ",
  "deadlock.   ",
};

/**
 * There are no random syscalls so we need something similar to model different
 * waiting, eating and thinking times.
 */

#define RAND_MAX (20)
const int rand_int[RAND_MAX] = {
  13, 7, 3, 19, 0, 5, 11, 12, 16, 4, 2, 9, 18, 15, 6, 14, 8, 10, 17, 1,
};

int rand_index;
int rand_next() {
  rand_index = (rand_index + id) % RAND_MAX;
  return rand_int[rand_index];
}

/**
 * Most important part where the forks (pipe fds) are assigned to every
 * philosopher according to Dijkstra's solution of the problem.
 * Every philosopher has the fork with his ID as his primary fork except the last
 * one (he's leftie).
 * The philosophers always pick up their primary frok first. In that way the
 * forks are always aquired in the same order by all the philosophers.
 */
void close_all_except_own_fork_pipes() {
  // write to own fork e.g. put own fork on the table
  write(fork_pipes[id*2 + 1], "X", 1);

  // assign forks for last philosopher
  if (PHIL_NUMBER - 1 == id) {
    fork_pipes_read[FIRST_FORK] = fork_pipes[0];
    fork_pipes_write[FIRST_FORK] = fork_pipes[1];

    fork_pipes_read[SECOND_FORK] = fork_pipes[id*2];
    fork_pipes_write[SECOND_FORK] = fork_pipes[id*2 + 1];
  }
  else {
    fork_pipes_read[FIRST_FORK]   = fork_pipes[id*2];
    fork_pipes_write[FIRST_FORK]  = fork_pipes[id*2 + 1];

    fork_pipes_read[SECOND_FORK]  = fork_pipes[id*2 + 2];
    fork_pipes_write[SECOND_FORK] = fork_pipes[id*2 + 3];
  }

  // close all other fds
  for (size_t i = 0; i < PHIL_NUMBER; i++) {
    if (i != id && i != (id + 1) % PHIL_NUMBER) {
      close(fork_pipes[2*i]);
      close(fork_pipes[2*i + 1]);
    }
  }
}

void pseudo_wait(const size_t duration) {
  for (size_t i = 0; i < (duration << 20); i++) {
    double a = duration * 555.555;
    double b = i * 77.7777;
    size_t c = a * b;
  }
}

void think() {
  pseudo_wait(rand_next());

  const char* topic = topics[rand_next() % TOPIC_NUMBER];

  write(STDOUT_FILENO, name(), PHIL_NAME_LENGTH);
  write(STDOUT_FILENO, " thinks about ", 14);
  write(STDOUT_FILENO, topic, TOPIC_LENGTH);
  write(STDOUT_FILENO, "\n", 1);
}

int picked_up(const fork_t fork) {
  char x;
  int bytes_read = read(fork_pipes_read[fork], &x, 1);

  if (bytes_read < 0) {
    write(STDERR_FILENO, "Problem with reading from pipe.\n", 32);
    exit(EXIT_FAILURE);
  }
  else return bytes_read;
}

void put_down(const fork_t fork) {
  write(fork_pipes_write[fork], "X", 1);
}

void eat() {

  while (!picked_up(FIRST_FORK)) { /* busily wait */ };
  write(STDOUT_FILENO, name(), PHIL_NAME_LENGTH);
  write(STDOUT_FILENO, " has one fork.\n", 15);

  while (!picked_up(SECOND_FORK)) { /* busily wait */ };
  write(STDOUT_FILENO, name(), PHIL_NAME_LENGTH);
  write(STDOUT_FILENO, " has both forks and starts to eat!\n", 35);

  pseudo_wait(rand_next());

  write(STDOUT_FILENO, name(), PHIL_NAME_LENGTH);
  write(STDOUT_FILENO, " releases its two forks.\n", 25);
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

  write(STDOUT_FILENO, name(), PHIL_NAME_LENGTH);
  write(STDOUT_FILENO, " is done with philosophizing.\n", 30);
  exit(EXIT_SUCCESS);
}

int main() {

  for (size_t i = 0; i < PHIL_NUMBER; i++) {
    int err = pipe(fork_pipes + (i * 2));
    if (err) {
      write(STDERR_FILENO, "Failed to create 16 forks.\n", 27);
      exit(EXIT_FAILURE);
    }
  }

  for (size_t i = 0; i < PHIL_NUMBER; i++) {
    pid_t pid = fork();

    if (0 == pid) philosophize(i);
    else if (pid < 0) {
      write(STDERR_FILENO, "Failed to fork 16 philosophers.\n", 42);
      exit(EXIT_FAILURE);
    }
  }

  exit(EXIT_SUCCESS);
}
