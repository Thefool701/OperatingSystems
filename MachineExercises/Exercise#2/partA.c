#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
  if (signal(SIGCHLD, SIG_IGN) == SIG_ERR) {
    perror("signal");
    exit(EXIT_FAILURE);
  }
  int id = fork();
  if (id == -1) {
    perror("fork");
    exit(EXIT_FAILURE);
  }

  while (1) {
    if (id == 0) {
      int childPid = getpid();
      int childPpid = getppid();
      printf("[Child]: PID %jd, PPID %jd\n", (intmax_t)childPid,
             (intmax_t)childPpid);

    } else {
      int parentPid = getpid();
      int parentPpid = getppid();
      printf("[Parent]: PID %jd, PPID %jd\n", (intmax_t)parentPid,
             (intmax_t)parentPpid);
    }
  }
}
