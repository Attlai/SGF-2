#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "struct.h"
#include "Fonctions_node.h"
#include "error.h"
#include "macro.h"
#include "fonctions_test.h"
#include "shellTerminal.h"

void display_error(int error_code);

int main()
{
    //On fait juste une série de test pour vérifier que toutes les fonctions marchent. Si il y a un probleme avec l'une d'entre elles, on aura un printf d'erreur
    TEST(test_initialisation_DISK)
    TEST(test_creer_INODE_normal)
    TEST(test_creer_INODE_11place)
    TEST(test_creer_INODE_full)
    TEST(test_remove_INODE)
    TEST(test_creer_dossier)
    TEST(test_remove_folder)
    TEST(test_creer_fichier)
    TEST(test_remove_fichier)
    //TEST(test_list_content)
    TEST(test_initialisation_system)
    TEST(test_changement_de_location)



    //début véritable du programme
    int status;
	char *line;
    char **args;
    int i;
    pid_t pid, wpid;
    bool running = true;

    //On lance dans un processus fils
	pid = fork();
	if(pid == 0)
	{
	    //initialisation du system
		int current_id = 0;
		DISK partition = Initialize_System(&current_id);
		//boucle d'exécution du shell
		do
		{
			printf("  Current directory : %s\n ",partition.superbloc[current_id]->nom);
			printf("  > ");
			//on récupère la ligne tappée
			line = lire_ligne();
			//On la découpe en arguments
			args = split_ligne(line);
			//On envoie les arguments dans la fonction qui va exécuter les commandes
			status = execution_cmd(args,&partition,&current_id);
			//Et enfin, on récupère les erreurs et on affiche les messages d'erreur correspondants
			display_error(status);
			//En cas de code "d'erreur" EXIT, on quitte la boucle -> fin du programme
			if(status == EXIT)
				running = false;

		}while(running);

		exit(0);
	}
	else
	{
		wait(NULL);
	}

    return 0;
}




void display_error(int error_code)
{
	switch(error_code)
	{
		case ERR_DIRECTORY_FULL:
			fprintf(stderr, "   Error : The directory is full\n");
			break;
		case ERR_NAME_ALREADY_USED_IN_FOLDER:
			fprintf(stderr, "   Error : This name is already in use this folder\n");
			break;
		case ERR_TARGET_NOT_FOUND:
			fprintf(stderr, "   Error : The specified name does not exist in this folder\n");
			break;
		case ERR_FOLDER_NOT_FOUND:
			fprintf(stderr, "   Error : The specified name does not exist in this folder\n");
			break;
		case ERR_WRONG_TYPE:
			fprintf(stderr, "   Error : Wrong type of the second argument\n");
			break;
		case ERR_DESTINATION_NOT_A_FOLDER:
			fprintf(stderr, "   Error : The destination is not a folder\n");
			break;
		case ERR_TOO_MANY_ARGUMENTS:
			fprintf(stderr, "   Error : Too many arguments\n");
			break;
		case ERR_NO_ARGUMENT:
			fprintf(stderr, "   Error : No command\n");
			break;
		case ERR_NOT_ENOUGH_ARGUMENTS:
			fprintf(stderr, "   Error : Arguments missing\n");
		case ERR_COMMAND_DOESNT_EXIST:
			fprintf(stderr, "   Error: This command doesn't exist\n");
			break;
	}
}

