#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
  char *args[] = {"./count", NULL};
  int parentPid;
  int id;
  if (signal(SIGCHLD, SIG_IGN) == SIG_ERR) {
    perror("signal");
    exit(EXIT_FAILURE);
  }
  id = fork();
  if (id == -1) {
    perror("fork");
    exit(EXIT_FAILURE);
  }

  if (id != 0) {
    parentPid = getpid();
    printf("[PARENT]: PID %jd, wait for child with PID %jd\n",
           (intmax_t)parentPid, (intmax_t)id);
    wait(NULL);
  }

  if (id == 0) {
    id = getpid();
    printf("[CHILD]: PID %jd, starts counting: \n", (intmax_t)id);
    execvp(args[0], args);
  }
  if (id != 0) {
    printf("[PARENT]: Child with PID %jd finished and unloaded.", (intmax_t)id);
  }

  exit(EXIT_SUCCESS);
}
