#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#define TAMPON 1024
#define LSH_TOK_BUFSIZE 64
#define LSH_TOK_DELIM " \t\r\n\a"



void commande_cd(char **args){
	 if (args[1] == NULL)
   {
    fprintf(stderr, "argument manquant\"cd\"\n");
   } 
   else
   {
    chdir(args[1]);
    } 
  }
  
  
  void printDir()
{
    char cwd[1024];
    getcwd(cwd, sizeof(cwd));
    printf("repertoire courant: %s\n ", cwd);
}
  


char *lire_ligne(void)
{
  int taille_tampon = TAMPON;
  int position = 0;
  char *tampon = malloc(sizeof(char) * taille_tampon);
  int c;
  if (!tampon) {
    exit(EXIT_FAILURE);
  }

  while (1) {
    //lire un caractere
    c = getchar();

    // si on a EOF, on le remplace avec null caractere .
    if (c == EOF || c == '\n') {
      tampon[position] = '\0';
      return tampon;
    } else {
      tampon[position] = c;
    }
    position++;

    // si on depasse le buffer ,reallocation
    if (position >= taille_tampon) {
      taille_tampon += TAMPON;
      tampon = realloc(tampon, taille_tampon);
      if (!tampon) {
        exit(EXIT_FAILURE);
      }
    }
  }
}

char **split_ligne(char *line)
{
  int bufsize = LSH_TOK_BUFSIZE, position = 0;
  char **tokens = malloc(bufsize * sizeof(char*));
  char *token;

  if (!tokens) {
    fprintf(stderr, "erreur allcoation\n");
    exit(EXIT_FAILURE);
  }

  token = strtok(line, LSH_TOK_DELIM);
  while (token != NULL) {
    tokens[position] = token;
    position++;

    if (position >= bufsize) {
      bufsize += LSH_TOK_BUFSIZE;
      tokens = realloc(tokens, bufsize * sizeof(char*));
      if (!tokens) {
        fprintf(stderr, "erreur allocation\n");
        exit(EXIT_FAILURE);
      }
    }

    token = strtok(NULL, LSH_TOK_DELIM);
  }
  tokens[position] = NULL;
  return tokens;
}

//exécution des commande
int execution_cmd(char **args)
{
  int i;
    pid_t pid, wpid;
  int status;
  if (args[0] == NULL) {
    //commande vide
    return 1;
  }

//si la commande est cd 
    if (strcmp(args[0], "cd") == 0) {
		printf("%s",args[0]);
	    printf("%s\n",args[1]);
        chdir(args[1]);
       return 1;
    }
    else {
		pid = fork();
  if (pid == 0) {
    // fils
    if (execvp(args[0], args) == -1) {
      perror("lsh");
    }
    exit(EXIT_FAILURE);
  } else if (pid < 0) {
    // erreur fork
    perror("fork");
  } else {
    // Parent
    do {
      wpid = waitpid(pid, &status, WUNTRACED);
    } while (!WIFEXITED(status) && !WIFSIGNALED(status));
  }
		}
  

  return 1;
}


int main(){
	char* username = getenv("USER");
	int status;
	char *line;
     char **args;
	printDir();
	do{
	printf("%s[ubunto]> ",username);
	line = lire_ligne();
	args=split_ligne(line);
	status=execution_cmd(args);
	
} while (status);
return 0;
}
