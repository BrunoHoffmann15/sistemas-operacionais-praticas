#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

#define CURL "curl "
#define OUTPUT " -o "
#define DESTINATION "temp_file"
#define PIPE " | "
#define EGREP "grep -oi "
#define QUOTATION "'"
#define WC "wc -l"
#define ECHOMIN 512
#define ECHOMAX 1048576

void get_page(char *val, int file_descriptor[2]);
void find_occurences(char *match_data);
char *get_data_from_file(char returned_value[ECHOMAX]);
void execute_command(char *command);

int main(int argc, char *argv[])
{
  char data[ECHOMAX];
  int file_pipes[2];
  pid_t fork_result;

  if (argc < 3)
  {
    printf("Digite o valor <url> e <palavra>. (ex: ./exercicio1 https://g1.globo.com coronavirus) \n");
    exit(EXIT_FAILURE);
  }

  if (pipe(file_pipes) == 0)
  {
    fork_result = fork();

    if (fork_result == 0)
    {
      close(0);
      close(file_pipes[1]);
      dup(file_pipes[0]);
      find_occurences(argv[2]);
      close(file_pipes[0]);
    }
    else
    {
      close(file_pipes[0]);
      get_page(argv[1], file_pipes);
      close(file_pipes[1]);
    }
  }

  exit(EXIT_SUCCESS);
}

void get_page(char *val, int file_descriptor[2])
{
  char command[ECHOMIN];
  char returned_value[ECHOMAX];

  strcpy(command, CURL);
  strcat(command, val);
  strcat(command, OUTPUT);
  strcat(command, DESTINATION);

  system(command);

  get_data_from_file(returned_value);

  write(file_descriptor[1], returned_value, strlen(returned_value));
}

char *get_data_from_file(char returned_value[ECHOMAX])
{
  FILE *fp;
  char buffer[ECHOMAX];

  fp = fopen(DESTINATION, "r");

  int i = 0;
  while (1)
  {
    buffer[i] = fgetc(fp);
    if (buffer[i] == EOF)
      break;
    ++i;
  }

  pclose(fp);

  strcpy(returned_value, buffer);

  return returned_value;
}

void find_occurences(char *match_data)
{
  char command[ECHOMAX];

  strcat(command, EGREP);
  strcat(command, QUOTATION);
  strcat(command, match_data);
  strcat(command, QUOTATION);
  strcat(command, PIPE);
  strcat(command, WC);
  system(command);
}