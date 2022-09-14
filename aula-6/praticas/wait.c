#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>

int main () {
  pid_t pid;
  char *message;
  int n;
  int exit_code;

  printf("Fork program starting: PID=%d\n", getpid());
  pid = fork();

  switch (pid)
  {
    case -1:
      perror("fork failed \n");
      exit(1);
    case 0:
      message = "child: %d\n";
      n = 5;
      exit_code = 37;
      break;
    default:
      message = "parent: %d\n";
      n = 3;
      exit_code = 0;
      break;
  }

  for (; n > 0; n--) {
    printf(message, getpid());
    sleep(1);
  }

  if (pid != 0) {
    int stat_val;
    pid_t child_pid;

    child_pid = fork();

    printf("Parent terminated\n");
    if(WIFEXITED(stat_val))
      printf("Msg from parent: child proc %d exited with code %d \n", pid, WEXITSTATUS(stat_val));
    else
      printf("Msg from parent: child proc %d terminated abnormally \n", pid);
  } else {
    printf("Child has terminated \n");
  }

  exit(exit_code);
}
