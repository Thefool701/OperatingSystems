#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define SIZE 5
// Struct that holds the 2D array
typedef struct {
  int **A;
  int **B;
  int **result;
  int rows;
  int cols;
} Array2D;

// Function to generate numbers for the matrix
int random_number(int min, int max) { return min + rand() % (max - min + 1); }

// Function to multiply matrices
void multiply_matrices(int A[SIZE][SIZE], int B[SIZE][SIZE],
                       int result[SIZE][SIZE]) {
  for (int i = 0; i < SIZE / 2; i++) {
    for (int j = 0; j < SIZE / 2; j++) {
      result[i][j] = 0;
      for (int k = 0; k < SIZE / 2; k++) {
        result[i][j] += A[i][k] * B[k][j];
      }
    }
  }
}

// Thread function
void *multi_calc(void *arg) {
  Array2D *data = (Array2D *)arg;
  int **A = data->A;
  int **B = data->B;
  int **result = data->result;
  int rows = data->rows;
  int cols = data->cols;

  for (int i = 0; i < SIZE; i++) {
    for (int j = 0; j < SIZE; j++) {
      result[i][j] = 0;
      for (int k = 0; k < SIZE; k++) {
        result[i][j] += A[i][k] * B[k][j];
      }
    }
  }

  return NULL;
}

int main() {
  int **A = malloc(SIZE * sizeof(int *));
  if (!A) {
    fprintf(stderr, "Memory allocation failed!\n");
    return 1;
  }
  for (int i = 0; i < SIZE; i++) {
    A[i] = malloc(SIZE * sizeof(int));
    for (int j = 0; j < SIZE; j++) {
      A[i][j] = random_number(1, 5);
    }
  }
  int **B = malloc(SIZE * sizeof(int *));
  if (!B) {
    fprintf(stderr, "Memory allocation failed!\n");
    return 1;
  }
  for (int i = 0; i < SIZE; i++) {
    B[i] = malloc(SIZE * sizeof(int));
    for (int j = 0; j < SIZE; j++) {
      B[i][j] = random_number(1, 5);
    }
  }
  int **result = malloc(SIZE * sizeof(int *));
  if (!result) {
    fprintf(stderr, "Memory allocation failed!\n");
    return 1;
  }
  for (int i = 0; i < SIZE; i++) {
    result[i] = malloc(SIZE * sizeof(int));
    for (int j = 0; j < SIZE; j++) {
      result[i][j] = 0;
    }
  }

  Array2D result_data = {A, B, result, SIZE, SIZE};

  pthread_t th1;
  if (pthread_create(&th1, NULL, multi_calc, &result_data) != 0) {
    printf("thread creation failed...\n");
    return 2;
  }

  pthread_join(th1, NULL);

  for (int i = 0; i < SIZE; i++) {
    for (int j = 0; j < SIZE; j++) {
      printf("[%d]", result[i][j]);
    }
    printf("\n");
  }

  for (int i = 0; i < SIZE; i++) {
    free(A[i]);
  }
  for (int i = 0; i < SIZE; i++) {
    free(B[i]);
  }
  for (int i = 0; i < SIZE; i++) {
    free(result[i]);
  }
  free(A);
  free(B);
  free(result);
  return 0;
}
