#define _GNU_SOURCE
#define _FILE_OFFSET_BITS 64
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>

int checkSourceFile(int src, char srcName[20]) {
  src = open(srcName, O_RDONLY);
  if (src < 0) {
    printf("Source File does not exist. Try Again.\n");
    return 1;
  }
  return 0;
}

int checkDestinationFile(int dest, char destName[20]) {
  dest = open(destName, O_RDONLY);
  if (dest < 0) {
    printf("Destination File does not exist. Try Again.\n");
    return 1;
  }
  return 0;
}
int main() {
  char destName[20];
  char srcName[20];
  int src;
  int dest;
  off_t len, ret;
  struct stat stat;

  // Prompt for infile & Check if infile exists
  do {
    printf("\nEnter Source File: ");
    scanf("%s", srcName);
  } while (checkSourceFile(src, srcName) == 1);
  src = open(srcName, O_RDONLY);
  if (fstat(src, &stat) == -1) {
    perror("fstat");
    exit(EXIT_FAILURE);
  }
  len = stat.st_size;

  // Prompt for outfile & Check if outfile exists
  do {
    printf("\nEnter Destination File: ");
    scanf("%s", destName);
  } while (checkSourceFile(dest, destName) == 1);
  dest = open(destName, O_RDONLY | O_WRONLY | O_TRUNC, 0644);

  // Copy contents
  do {
    ret = copy_file_range(src, NULL, dest, NULL, len, 0);
    if (ret == -1) {
      perror("copy_file_range");
      exit(EXIT_FAILURE);
    }
    len -= ret;
  } while (len > 0 && ret > 0);

  close(src);
  close(dest);
  exit(EXIT_SUCCESS);
}
