#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

void reverseString(char *recievedMsg, int stringLength) {
  for (int i = 0; i < stringLength; i++) {
    if (isupper(recievedMsg[i])) {
      recievedMsg[i] = tolower(recievedMsg[i]);
    } else if (islower(recievedMsg[i])) {
      recievedMsg[i] = toupper(recievedMsg[i]);
    }
  }
}

int main() {
  int fd[2];
  // fd[0] - read
  // fd[1] - write

  if (pipe(fd) == -1) {
    printf("Error occured opening pipe.\n");
    return -1;
  }

  int id = fork();
  if (id == -1) {
    printf("Forking failed.\n");
  }

  if (id != 0) {
    // Parent Process
    close(fd[0]);
    int parentPid = getpid();
    char msg[100];
    printf("\nInput string message: ");
    scanf("%[^\n]%*c", msg);
    printf("\n");
    if (write(fd[1], msg, sizeof(msg)) == -1) {
      printf("Writing error occured.\n");
      return 2;
    }
    printf("PARENT(%jd): Sending [%s] to Child\n", (intmax_t)parentPid, msg);
    printf("\n");
    close(fd[1]);
  } else {
    // Child Process
    int childPid = getpid();
    int stringLength;
    char diffMsg[100];
    char reversedMsg[100];
    if (read(fd[0], &diffMsg, sizeof(diffMsg)) == -1) {
      printf("Reading error occured.\n");
      return 3;
    }
    close(fd[0]);
    stringLength = strlen(diffMsg);
    printf("Child(%jd): Recieved message\n", (intmax_t)childPid);
    printf("\n");
    printf("Reversing the case of the string and sending to parent\n");
    printf("\n");
    reverseString(diffMsg, stringLength);
    if (write(fd[1], reversedMsg, sizeof(reversedMsg)) == -1) {
      printf("Writing reversed string error occured.\n");
      return 4;
    }
    close(fd[1]);
  }
  if (id != 0) {
    close(fd[1]);
    int parentPid = getpid();
    // Parent Process
    char reversedMsg[100];
    if (read(fd[0], &reversedMsg, sizeof(reversedMsg)) == -1) {
      printf("Reading reversed message error occured.\n");
      return 5;
    }
    close(fd[0]);
    printf("PARENT(%jd): Recieved [%s] from Child\n", (intmax_t)parentPid,
           reversedMsg);
  } else {
    close(fd[0]);
    close(fd[1]);
  }

  return 0;
}
