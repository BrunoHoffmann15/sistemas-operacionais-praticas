#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

volatile sig_atomic_t counter = 0;

void tratador(int signum) {
  counter++;
}

int main() {
  sigset_t mask;
  struct sigaction action;
  memset(&action, 0, sizeof(action));

  action.sa_handler = &tratador;
  sigaction(SIGUSR1, &action, NULL);

  printf("Meu PID = %d\n Envie SIGUSR1 para continuar, ou SIGINT para sair \n", getpid());

  sigfillset(&mask);
  sigdelset(&mask, SIGUSR1);
  //sigdelset(&mask, SIGINT);

  while (1) {
    sigsuspend(&mask);
    sleep(1);
    printf("Contador %d\n", counter);
  }

  return 0;
}