//Fichier contenant les fonctions relatives à l'interface utilisateur et les commandes utilisées par lui

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>

#include "struct.h"
#include "Fonctions_node.h"
#include "error.h"
#include "shellTerminal.h"

#define TAMPON 1024
#define LSH_TOK_BUFSIZE 64
#define LSH_TOK_DELIM " \t\r\n\a"



void commande_cd(char **args)
{
	if (args[1] == NULL)
	{
		fprintf(stderr, "argument manquant\"cd\"\n");
	}
	else
	{
		char * destination;
		
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
	if (!tampon) 
	{
		exit(EXIT_FAILURE);
	}

	while (1)
	{
		//lire un caractere
		c = getchar();

		// si on a EOF, on le remplace avec null caractere .
		if (c == EOF || c == '\n') 
		{
			tampon[position] = '\0';
			return tampon;
		} 
		else 
		{
			tampon[position] = c;
		}
		position++;

		// si on depasse le buffer ,reallocation
		if (position >= taille_tampon) 
		{
			taille_tampon += TAMPON;
			tampon = realloc(tampon, taille_tampon);
			if (!tampon) 
			{
				exit(EXIT_FAILURE);
			}
		}
	}
}

//Sépare la chaine de caractères de la commande en plusieurs arguments
char **split_ligne(char *line)
{
	int bufsize = LSH_TOK_BUFSIZE, position = 0;
	char **tokens = malloc(bufsize * sizeof(char*));
	char *token;

	if (!tokens) 
	{
		fprintf(stderr, "erreur allcoation\n");
		exit(EXIT_FAILURE);
	}

	token = strtok(line, LSH_TOK_DELIM);
	while (token != NULL) 
	{
		tokens[position] = token;
		position++;

		if (position >= bufsize) 
		{
			bufsize += LSH_TOK_BUFSIZE;
			tokens = realloc(tokens, bufsize * sizeof(char*));
			if (!tokens) 
			{
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
int execution_cmd(char **args,DISK *partition,int* current_id)
{
	if(args[0] == NULL)
		return ERR_NO_ARGUMENT;
	
	char * commande = args[0];
	
	if(strcmp(commande , "my_exit")==0)
	{
		printf("   Now exiting\n");
		return EXIT;
	}
	else if(strcmp(commande, "my_ls")==0)
	{
		if(args[1]!=0)
			return ERR_TOO_MANY_ARGUMENTS;
		return list_content_folder(partition,*current_id);
	}
	else if(strcmp(commande, "my_mkdir")==0)
	{
		if(args[1]==NULL)
			return ERR_NOT_ENOUGH_ARGUMENTS;
		if(args[2]!=NULL)
			return ERR_TOO_MANY_ARGUMENTS;
		return create_folder(partition,args[1],*current_id);
	}
	else if(strcmp(commande, "my_rmdir")==0)
	{
		if(args[1]==NULL)
			return ERR_NOT_ENOUGH_ARGUMENTS;
		if(args[2]!=NULL)
			return ERR_TOO_MANY_ARGUMENTS;
		
		int indice = seek_id(partition,*current_id,args[1]);
		if (indice == ERR_TARGET_NOT_FOUND)
		{
			return indice;
		}
		return remove_folder(partition,indice,NORMAL);
	}
	else if(strcmp(commande,"my_touch")==0)
	{
		if(args[1]==NULL)
			return ERR_NOT_ENOUGH_ARGUMENTS;
		if(args[2]!=NULL)
			return ERR_TOO_MANY_ARGUMENTS;
		return create_file(partition,args[1],*current_id);
	}
	else if(strcmp(commande,"my_rm")==0)
	{
		if(args[1]==NULL)
			return ERR_NOT_ENOUGH_ARGUMENTS;
		if(args[2]!=NULL)
			return ERR_TOO_MANY_ARGUMENTS;
		
		int indice = seek_id(partition,*current_id,args[1]);
		if (indice == ERR_TARGET_NOT_FOUND)
		{
			return indice;
		}
		return remove_file(partition,indice);
	}
	else if(strcmp(commande,"my_cd")==0)
	{
		if(args[1]==NULL)
			return ERR_NOT_ENOUGH_ARGUMENTS;
		if(args[2]!=NULL)
			return ERR_TOO_MANY_ARGUMENTS;
		return change_current_directory(partition,current_id,args[1]);
	}
	else if(strcmp(commande,"my_echo")==0)
	{
		int i=1;
		printf("   ");
		while(args[i]!=NULL)
		{
			printf("%s ",args[i]);
			i++;
		}
		printf("\n");
	}
	else if(strcmp(commande,"my_man")==0)
	{
		if(args[1]==NULL)
			return ERR_NOT_ENOUGH_ARGUMENTS;
		if(args[2]!=NULL)
			return ERR_TOO_MANY_ARGUMENTS;
		display_syntax(args[1]);
	}
	else if(strcmp(commande,"my_help")==0)
	{
		printf("   List of commands:\n");
		printf("    my_exit\n    my_help\n    my_man\n    my_ls\n    my_mkdir\n    my_rmdir\n    my_touch\n    my_rm\n    my_cd\n");
	}
	else if(strcmp(commande,"my_man")==0)
	{
		return display_syntax(args[1]);
	}
	else
	{
		return ERR_COMMAND_DOESNT_EXIST;
	}
	

	return 0;
}

int display_syntax(char * command)
{
	if(strcmp(command,"my_exit")==0)
		printf("   my_exit  : Exits the shell\n");
	else if(strcmp(command,"my_help")==0)
		printf("   my_help <command_name>  :  Displays the list of all available commands\n");
	else if(strcmp(command,"my_man")==0)
		printf("   Are you fucking stupid?\n");
	else if(strcmp(command,"my_ls")==0)
		printf("   my_ls  :  Shows the list of files and folder inside the current directory\n");
	else if(strcmp(command,"my_mkdir")==0)
		printf("   my_mkdir <new_folder_name>  :  Creates a new folder inside the current directory\n");
	else if(strcmp(command,"my_rmdir")==0)
		printf("   my_rmdir <target_folder_name>  :  Removes a folder inside the current directory\n");
	else if(strcmp(command,"my_touch")==0)
		printf("   my_touch <new_file_name>  :  Creates a new file inside the current directory\n");
	else if(strcmp(command,"my_rm")==0)
		printf("   my_rm <target_file_name>  :  Removes a file inside the current directory\n");
	else if(strcmp(command,"my_cd")==0)
		printf("   my_cd <destination_folder_name>  :  Changes the current directory to one of the folder inside it. Type .. for the destination get back to the parent folder\n");
	else
		return ERR_COMMAND_DOESNT_EXIST;
		
	return 0;
			
}

