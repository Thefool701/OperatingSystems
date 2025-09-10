#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>
int main() {
  int p1[2]; // P => C
  int p2[2]; // C => P
  // p1[0] & p2[0] => read
  // p1[1] & p2[1] => write

  if (pipe(p1) == -1) {
    return 1;
  }
  if (pipe(p2) == -1) {
    return 1;
  }
  int pid = fork();
  if (pid == -1) {
    return 2;
  }

  if (pid == 0) {
    close(p1[0]);
    close(p2[1]);
    // Child Process
    int x;
    if (read(p2[0], &x, sizeof(x)) == -1) {
      return 3;
    }
    printf("Recieved %d\n", x);
    x *= 4;
    if (write(p1[1], &x, sizeof(x)) == -1) {
      return 4;
    }
    printf("Wrote %d\n", x);
    close(p1[1]);
    close(p2[0]);

  } else {
    close(p1[1]);
    close(p2[0]);
    // Parent Process
    srand(time(NULL));
    int y = rand() % 10;
    if (write(p2[1], &y, sizeof(y)) == -1) {
      return 5;
    }
    printf("Wrote %d\n", y);
    if (read(p1[0], &y, sizeof(y)) == -1) {
      return 6;
    }
    printf("Result is %d\n", y);
    close(p1[0]);
    close(p2[1]);
    wait(NULL);
  }
  return 0;
}
