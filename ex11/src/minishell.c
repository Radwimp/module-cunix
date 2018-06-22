#define _GNU_SOURCE
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>
#include <signal.h>
#include <string.h>
#include "../include/linked_list.h"
#include "../include/hash.h"

int vash_echo(char **args);
int vash_export(char **args);
int vash_env(char **args);
int vash_ls(char **args);
int vash_exit(char **args);

char *builtin_str[] = {
  "echo", 
  "export", 
  "env", 
  "ls", 
  "exit"
};

node_t *exp_lst;
hashtable_t  *exp_tbl;

int (*builtin_func[]) (char **) = {
  &vash_echo, 
  &vash_export, 
  &vash_env, 
  &vash_ls, 
  &vash_exit
};

int vash_echo(char **args)
{
  int i, j;
  char *str;

  i = 0;
  j = -1;
  while (args[++i] != NULL)
  {
    if (args[i][0] == '$')
    {
      str = args[i];
      while (++j <= strlen(str))
        str[j] = str[j + 1];
      printf("%s ", hash_get(exp_tbl, str));
    }
    else
      printf("%s ", args[i]);
  }
  printf("\n");
  return 1;
}

int vash_export(char **args)
{
  char *token;
  char s[2] = "=";
  token = strtok(args[1], s);
  if (exp_lst == NULL && exp_tbl == NULL)
  {
    exp_lst = list_create(token);
    exp_tbl = hash_create(18);
  }
  else list_push(exp_lst, token);
  char *tmp = token;
  token = strtok(NULL, s);
  hash_set(exp_tbl, tmp, token);
  return 1;
}

int vash_env(char **args)
{
  node_t *tmp;
  
  tmp = exp_lst;
  while( tmp != NULL)
  {
    printf("%s: %s\n", tmp->data, hash_get(exp_tbl, tmp->data));
    tmp = tmp->next;
  }
  return 1;
}

int vash_ls(char **args)
{
  struct dirent *pDirent;
  DIR *pDir;

  if (args[1])
  {
    pDir = opendir(args[1]);
    if (pDir == NULL)
    {
      printf ("Cannot open directory '%s'\n", args[1]);
      return 1;
    }
  }
  else
    pDir = opendir(".");
  while ((pDirent = readdir(pDir)) != NULL)
    printf ("[%s] ", pDirent->d_name);
  closedir (pDir);
  printf("\n");
  return 1;
}

int vash_exit(char **args)
{
  return 0;
}

int vash_execute(char **args)
{
  int i;

  if (args[0] == NULL)
    return 1;
  for (i = 0; i < sizeof(builtin_str) / sizeof(char *); i++)
  {
    if (strcmp(args[0], builtin_str[i]) == 0)
      return (*builtin_func[i])(args);
  }
  return 1;
}

char *vash_read_line()
{
  char *line;
  size_t bufsize; 
    
  line = NULL;
  bufsize = 0;
  getline(&line, &bufsize, stdin);
  return line;
}

#define VASH_TOK_BUFSIZE 64
#define VASH_TOK_DELIM " \t\r\n\a"
char **vash_split_line(char *line)
{
  int bufsize, position;
  char **tokens;
  char *token;

  bufsize = VASH_TOK_BUFSIZE;
  position = 0;
  tokens = malloc(bufsize * sizeof(char*));
  if (!tokens)
  {
    fprintf(stderr, "vash: allocation error\n");
    exit(EXIT_FAILURE);
  }
  token = strtok(line, VASH_TOK_DELIM);
  while (token != NULL)
  {
    tokens[position] = token;
    position++;
    if (position >= bufsize)
    {
      bufsize += VASH_TOK_BUFSIZE;
      tokens = realloc(tokens, bufsize * sizeof(char*));
      if (!tokens)
      {
        fprintf(stderr, "vash: allocation error\n");
        exit(EXIT_FAILURE);
      }
    }
    token = strtok(NULL, VASH_TOK_DELIM);
  }
  tokens[position] = NULL;
  return tokens;
}

void signalhandler(int sig)
{
  if (sig == SIGINT)
  {
    puts("");
    write(1, "$_> ", 5);
    signal(SIGINT, signalhandler);
  }
}

int main()
{
  char *line;
  char **args;
  int status;

  do {
    printf("$_> ");
    signal(SIGINT, signalhandler);
    line = vash_read_line();
    args = vash_split_line(line);
    status = vash_execute(args);

    free(line);
    free(args);
  } while (status);
  return EXIT_SUCCESS;
}
