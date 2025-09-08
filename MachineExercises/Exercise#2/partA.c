#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
  pid_t parentPid;
  pid_t parentPpid;
  pid_t childPid;
  pid_t childPpid;
  if (signal(SIGCHLD, SIG_IGN) == SIG_ERR) {
    perror("signal");
    exit(EXIT_FAILURE);
  }
  childPid = fork();
  if (childPid == -1) {
    perror("fork");
    exit(EXIT_FAILURE);
  }

  while (1) {
    parentPid = getpid();
    parentPpid = getppid();
    printf("[PARENT]: PID %jd, PPID %jd\n", (intmax_t)parentPid,
           (intmax_t)parentPpid);
    printf("[CHILD]: PID %jd, PPID %jd\n", (intmax_t)childPid,
           (intmax_t)parentPid);
  }
}
