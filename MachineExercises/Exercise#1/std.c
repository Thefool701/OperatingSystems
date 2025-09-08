#include <stdio.h>
#include <unistd.h>

int checkSourceFile(FILE *src, char srcName[20]) {
  src = fopen(srcName, "r");
  if (src == NULL) {
    printf("Source File does not exist. Try Again.\n");
    return 1;
  }
  return 0;
}

int checkDestinationFile(FILE *dest, char destName[20]) {
  dest = fopen(destName, "r");
  if (dest == NULL) {
    printf("Destination File does not exist. Try Again.\n");
    return 1;
  }
  return 0;
}
int main() {
  char destName[20];
  char srcName[20];
  FILE *src;
  FILE *dest;
  char a;
  // Prompt for infile & Check if infile exists
  do {
    printf("\nEnter Source File: ");
    scanf("%s", srcName);
  } while (checkSourceFile(src, srcName) == 1);
  src = fopen(srcName, "r");
  // Prompt for outfile & Check if outfile exists
  do {
    printf("\nEnter Destination File: ");
    scanf("%s", destName);
  } while (checkSourceFile(dest, destName) == 1);
  dest = fopen(destName, "w");

  // Copy contents
  while ((a = fgetc(src)) != EOF) {
    fputc(a, dest);
  }
  fclose(src);
  fclose(dest);

  return 0;
}
