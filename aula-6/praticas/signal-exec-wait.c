#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>

void tratador(int signum)
{
  pid_t p = fork();
  if (p == 0)
    execlp("ps", "ps", "-ax", NULL);
}

int main()
{
  sigset_t mask;
  struct sigaction action;
  memset(&action, 0, sizeof(action));

  action.sa_handler = &tratador;
  
  sigaction(SIGUSR2, &action, NULL);

  printf("Meu PID = %d", getpid());
  fflush(stdout);

  sigfillset(&mask);
  sigdelset(&mask, SIGUSR2);
  sigdelset(&mask, SIGINT); // sai do programa
  
  while (1)
  {
    sigsuspend(&mask);
  }
  return 0;
}