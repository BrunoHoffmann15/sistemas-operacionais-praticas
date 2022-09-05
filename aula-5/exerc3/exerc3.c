/* primos.c – identifica todos os numeros primos ate um certo valor*/
// complile com -lm. Por exemplo: “gcc primo.c -o primo -lm”

#include <time.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int verifica_se_primo(long int);
int verificar_parte(int inicio, int fim, int imprimir);
void executar_sem_concorrencia(int inicio, int fim, int imprimir);
void executar_com_concorrencia(int numero, int imprimir);

int main(int argc, char *argv[])
{
  long int numero = 0;
  short int imprimir, concorrencia;

  if (argc != 4)
  {
    printf("Uso correto: %s <numero> <imprimir=1,nao_imprimir=0> <concorrencia=1,sem_concorrencia=0>\n\n", argv[0]);
    return 1;
  }

  numero = atol(argv[1]);
  imprimir = atoi(argv[2]);
  concorrencia = atoi(argv[3]);

  if (concorrencia) {
    executar_com_concorrencia(numero, imprimir);
  } else {
    executar_sem_concorrencia(0, numero, imprimir);
  }

  return 0;
}

void executar_sem_concorrencia(int inicio, int fim, int imprimir)
{
  clock_t start, end;

  start = clock();
  
  verificar_parte(inicio, fim, imprimir);

  end = clock();

  double time_spent = (double)(end - start) / CLOCKS_PER_SEC;
  printf("O tempo de execução foi de %f segundos.", time_spent);
}

void executar_com_concorrencia(int numero, int imprimir)
{
  int inicio, fim, status;
  pid_t child_pid;
  child_pid = fork();

  if (child_pid == 0)
  {
    inicio = 0;
    fim = numero / 2;

    verificar_parte(inicio, fim, imprimir);
  }
  else 
  {
    clock_t start, end;
    inicio = numero / 2;
    fim = numero;

    start = clock();
    verificar_parte(inicio, fim, imprimir);
    waitpid(child_pid, &status, 0);
    end = clock();

    double time_spent = (double)(end - start) / CLOCKS_PER_SEC;
    printf("O tempo de execução foi de %f segundos.", time_spent);
  }
}

int verificar_parte(int inicio, int fim, int imprimir)
{
  short int result;

  for (int num_int = inicio; num_int < fim; num_int++)
  {
    result = verifica_se_primo(num_int);
    if (imprimir == 1)
      if (result == 1)
        printf("%d eh primo.\n", num_int);
  }
}

int verifica_se_primo(long int numero)
{
  long int ant;
  for (ant = 2; ant <= (long int)sqrt(numero); ant++)
  {
    if (numero % ant == 0)
      return 0;
  }
  if (ant * ant >= numero)
    return 1;
}