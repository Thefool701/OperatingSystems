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
  int p1[2]; // P => C
  int p2[2]; // C => P
  // p1[0] & p2[0] => read
  // p1[1] & p2[1] => write

  if (pipe(p1) == -1) {
    printf("Pipe 1 failed.\n");
    return -1;
  }
  if (pipe(p2) == -1) {
    printf("Pipe 2 failed.\n");
    return -1;
  }
  int id = fork();

  if (id == 0) {
    // Child Process
    close(p1[1]);
    close(p2[0]);
    int childPid = getpid();
    int stringLength;
    char diffMsg[100];
    char reversedMsg[100];
    if (read(p1[0], &diffMsg, sizeof(diffMsg)) == -1) {
      printf("Reading error occured.\n");
      return 3;
    }
    close(p1[0]);
    stringLength = strlen(diffMsg);
    printf("Child(%jd): Recieved message\n", (intmax_t)childPid);
    printf("\n");
    printf(
        "Child(%jd): Reversing the case of the string and sending to Parent\n",
        (intmax_t)childPid);
    printf("\n");
    reverseString(diffMsg, stringLength);
    if (write(p2[1], diffMsg, sizeof(diffMsg)) == -1) {
      printf("Writing reversed string error occured.\n");
      return 4;
    }
    close(p2[1]);

  } else {
    // Parent Process
    close(p1[0]);
    close(p2[1]);
    int parentPid = getpid();
    char msg[100];
    printf("\nInput string message: ");
    scanf("%[^\n]%*c", msg);
    printf("\n");
    if (write(p1[1], msg, sizeof(msg)) == -1) {
      printf("Writing error occured.\n");
      return 2;
    }
    printf("PARENT(%jd): Sending [%s] to Child\n", (intmax_t)parentPid, msg);
    printf("\n");

    char reversedMsg[100];
    if (read(p2[0], &reversedMsg, sizeof(reversedMsg)) == -1) {
      printf("Reading reversed message error occured.\n");
      return 5;
    }
    close(p2[0]);
    printf("PARENT(%jd): Recieved [%s] from Child\n", (intmax_t)parentPid,
           reversedMsg);

    close(p1[1]);
    close(p2[0]);
    wait(NULL);
  }
}
