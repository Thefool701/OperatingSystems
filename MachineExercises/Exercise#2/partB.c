#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
  char *args[] = {"./count", NULL};
  pid_t parentPid;
  pid_t childPid;
  if (signal(SIGCHLD, SIG_IGN) == SIG_ERR) {
    perror("signal");
    exit(EXIT_FAILURE);
  }
  childPid = fork();
  if (childPid == -1) {
    perror("fork");
    exit(EXIT_FAILURE);
  }
  parentPid = getpid();

  if (childPid != 0) {
    printf("[PARENT]: PID %jd, wait for child with PID %jd\n",
           (intmax_t)parentPid, (intmax_t)childPid);
    wait(NULL);
  }

  if (childPid == 0) {
    childPid = getppid();
    printf("[CHILD]: PID %jd, starts counting: \n", (intmax_t)childPid);
    execvp(args[0], args);
  }
  if (childPid != 0) {
    printf("[PARENT]: Child with PID %jd finished and unloaded.",
           (intmax_t)childPid);
  }

  exit(EXIT_SUCCESS);
}
