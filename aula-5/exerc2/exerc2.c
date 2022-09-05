#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#define ROW_SIZE 4
#define COLUMN_SIZE 4

void generateMatrix();
void printMatrix(int matrix[ROW_SIZE][COLUMN_SIZE]);
void mulMat(int mat1[][COLUMN_SIZE], int mat2[][COLUMN_SIZE]);

int main()
{
  int matrix[ROW_SIZE][COLUMN_SIZE];
  int matrix2[ROW_SIZE][COLUMN_SIZE];
  int data;
  int file_pipes[2];
  const char some_data[] = "123";
  pid_t fork_result;

  if (pipe(file_pipes) != 0) {
    printf("Ocorreu um erro.");
    exit(EXIT_FAILURE);
  }

  fork_result = fork();
  generateMatrix(&matrix);
  printMatrix(matrix);

  if (fork_result == 0) {
    close(file_pipes[1]);
    printf("Matriz recebida pelo processo %d.\n", getpid());
    read(file_pipes[0], &matrix2, sizeof(matrix2));
    mulMat(matrix, matrix2);
    close(file_pipes[1]);
  } else {
    close(file_pipes[0]);
    write(file_pipes[1], matrix, sizeof(matrix));
    printf("Matriz enviada pelo processo %d.\n", getpid());
    close(file_pipes[1]);
  }

  exit(EXIT_SUCCESS);
}

void generateMatrix(int matrix[ROW_SIZE][COLUMN_SIZE])
{
  for (int i = 0; i < ROW_SIZE; i++)
  {
    for (int j = 0; j < COLUMN_SIZE; j++)
    {
      matrix[i][j] = (arc4random() % 10); // to run in Mac.
      // matrix[i][j] = (rand() % 10); // to run in Linux.
    }
  }
}

// Obtido pelo site Geek for Geeks - https://www.geeksforgeeks.org/c-program-multiply-two-matrices/
void mulMat(int mat1[][COLUMN_SIZE], int mat2[][COLUMN_SIZE])
{
  int rslt[ROW_SIZE][COLUMN_SIZE];
  printf("Resultado da multiplicação da matriz feita pelo processo %d:\n", getpid());
  printf("---------------------------\n");

  for (int i = 0; i < ROW_SIZE; i++)
  {
    for (int j = 0; j < COLUMN_SIZE; j++)
    {
      rslt[i][j] = 0;

      for (int k = 0; k < ROW_SIZE; k++)
      {
        rslt[i][j] += mat1[i][k] * mat2[k][j];
      }

      printf("%d\t", rslt[i][j]);
    }
    printf("\n");
  }
  printf("----------------------------\n");
}

// Obtido no site geek for geeks: https://www.geeksforgeeks.org/c-program-multiply-two-matrices/
void printMatrix(int matrix[ROW_SIZE][COLUMN_SIZE]) {
  printf("Matrix gerada pelo processo %d:\n", getpid());
  printf("---------------------------\n");
  for (int i = 0; i < ROW_SIZE; i++)
  {
    for (int j = 0; j < COLUMN_SIZE; j++)
      printf("%d\t", matrix[i][j]);
    printf("\n");
  }
  printf("---------------------------\n");
}