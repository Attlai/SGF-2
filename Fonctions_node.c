#include "struct.h"
#include "Fonctions_node.h"
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "error.h"
#include "macro.h"


//Fonction d'initialisation de DISK
void Initialiser_DISK(DISK *partition)
{
    for(int i=0;i<TAILLE_MAX_DISK;i++)
    {
        partition->superbloc[i] = NULL;
    }
    partition->last_id = 1;
}

//Fonction cherchant si l'un des emplacement du superbloc d'un disk est libre, si oui revoit l'undice de l'emplacement libre, sinon renvoie -1
int Trouver_place_DISK(DISK *partition)
{
    for(int i=1;i<TAILLE_MAX_DISK;i++)
    {
        if(partition->superbloc[i] == NULL)
        {
            char err_msg[200];
            sprintf(err_msg, "BLOC %d DU SUPERBLOC LIBRE\n",i);
            DEBUG(err_msg)
            return i;
        }
    }

    return ERR_NO_FREE_INODE;
}

//Fonction qui ajouter un inode à un DISK, en renseignant le type de fichier, et renvoie l'indice (potentiellement erreur)
int Creer_INODE(DISK* partition,char *name,type_inode type)
{
    INODE * node = NULL;
    int indice = Trouver_place_DISK(partition);
    if(indice != ERR_NO_FREE_INODE)
    {
        node = NEW(INODE)
        partition->superbloc[indice] = node;
    }
    else
    {
        DEBUG("NO FREE SPACE ON DISK\n");
        return indice;
    }
    if(type==DOSSIER)
    {
        for(int i=0;i<CONTENU_MAX_REPERTOIRES;i++)
        {
            node->repertoire.fichiers_contenus[i].id_inode = -1;
            node->repertoire.fichiers_contenus[i].nom = NULL;
        }
    }
    node->id_parent = -1;
    node->nom = name;
    node->metadata.taille_fichier = 0;
    node->type = type;
    
    time_t now = time(0);
    strftime(node->metadata.date, 17, "%Y/%m/%d %H:%M", localtime (&now));
    
    return indice;
}

//Fonction de suppression d'un INODE, en donnant son id
int Remove_INODE(DISK* partition,int rm_id)
{
    INODE *rm_inode = partition->superbloc[rm_id];
    if(rm_inode ==NULL)
    {
        return ERR_UNUSUED_INODE;
    }
    free(rm_inode);
    partition->superbloc[rm_id] = NULL;

    return 0;
}

//Fonction qui retourne un slot libre du contenu d'un répertoire, ou une erreur
int seek_free_slot_folder(INODE* dossier_parent)
{
    for(int i=0;i<CONTENU_MAX_REPERTOIRES;i++)
    {
        if(dossier_parent->repertoire.fichiers_contenus[i].id_inode == -1)
        {
            char val_msg[200];
            sprintf(val_msg, "BLOC %d DU DOSSIER LIBRE\n",i);
            DEBUG(val_msg)
            return i;
        }
    }
    return ERR_DIRECTORY_FULL;
}

//Fonction vérifiant qu'un fichier/dossier avec un nom donné n'existe pas déjà dans le dossier
bool check_inode_name_is_unique(INODE *parent,char *name)
{
    for(int i = 0;i<CONTENU_MAX_REPERTOIRES;i++)
    {
        if(parent->repertoire.fichiers_contenus[i].id_inode != -1)
        {
            if(strcmp(parent->repertoire.fichiers_contenus[i].nom,name)==0)
            {
                DEBUG("NAME ALREADY EXISTS")
                return false;
            }
        }

    }
    return true;
}

//On cherche le slot sur lequel un fichier avec un id donné est placé dans un dossier
int seek_folder_slot_from_inode(INODE* dossier,int id_fichier)
{
    for(int i=0;i<CONTENU_MAX_REPERTOIRES;i++)
    {
        if(dossier->repertoire.fichiers_contenus[i].id_inode == id_fichier)
        {	
            DEBUG1("ID TROUVE SUR SLOT %d DU DOSSIER\n",i)
            return i;
        }
    }
    return ERR_INODE_ID_NOT_FOUND_IN_FOLDER;
}

//Fonction de creation d'un dossier
int create_folder(DISK *partition,char* nom,int id_parent)
{
    int inode_id;
    // si id_parent est NUL => création racine
    if(id_parent == ROOT_PARENT_ID)
    {
        if(partition->superbloc[ROOT_INODE_ID] != NULL)
        {
            return ERR_ROOT_ALRDY_CREATED;
        }
        else
        {
            DEBUG("CREATION RACINE")
            inode_id = Creer_INODE(partition,nom,DOSSIER);
            if(inode_id != ROOT_INODE_ID)
            {
                return ERR_ROOT_WRONG_ID;
            }
            //le parent de root est considéré comme étant lui-même
            partition->superbloc[ROOT_INODE_ID]->id_parent = ROOT_INODE_ID;
            return inode_id;
        }
    }
    //Creation (ou pas) de repertoire non racine
    else
    {
        //si parent n'est pas un dossier => pas de creation
        if(partition->superbloc[id_parent]->type != DOSSIER)
        {
            return ERR_PARENT_NOT_FOLDER;
        }
        //non racine + parent dossier -> creation du dossier
        else
        {
            if(!check_inode_name_is_unique(partition->superbloc[id_parent],nom))
            {
                return ERR_NAME_ALREADY_USED_IN_FOLDER;
            }
            DEBUG("CREATION DOSSIER")
            inode_id = Creer_INODE(partition,nom,DOSSIER);
            //Si plus de place => remontée erreur
            if(inode_id == ERR_NO_FREE_INODE)
            {
                return inode_id;
            }
            else
            {

                INODE *inode_parent = partition->superbloc[id_parent];
                //On cherche si on peut rajouter un dossier au répertoire
                int slot = seek_free_slot_folder(inode_parent);
                DEBUG1("SLOT NUMBER IS %d",slot)
                if(slot == ERR_DIRECTORY_FULL)
                {
                    return slot;
                }
                //On ajoute le dossier dans le slot libre récupéré du parent
                inode_parent->repertoire.fichiers_contenus[slot].id_inode = inode_id;
                inode_parent->repertoire.fichiers_contenus[slot].nom = nom;
                partition->superbloc[inode_id]->id_parent = id_parent;
                inode_parent->repertoire.nb_fichiers = inode_parent->repertoire.nb_fichiers+1;
                
                return inode_id;
            }

        }
    }
}

//Fonction de suppression d'un dossier
int remove_folder(DISK* partition,int id_dossier,mode_suppression mode)
{
    INODE * dossier = partition->superbloc[id_dossier];

    //On vérifie que le dossier existe
    if(dossier == NULL)
    {
        return ERR_UNUSUED_INODE;
    }
    if(dossier->type != DOSSIER)
    {
        return ERR_WRONG_TYPE;
    }
    else
    {
        //On vérifie que le dossier est vide
        if(dossier->repertoire.nb_fichiers != 0)
        {
            return ERR_DIRECTORY_NOT_EMPTY;
        }
        else
        {
            
            //Si le dossier n'est pas la racine, on le supprime de son parent
            if(id_dossier != ROOT_INODE_ID)
            {
                int id_parent = dossier->id_parent;
                int slot = seek_folder_slot_from_inode(partition->superbloc[id_parent],id_dossier);

                if(slot == ERR_INODE_ID_NOT_FOUND_IN_FOLDER)
                {
                    return slot;
                }
                INODE *inode_parent = partition->superbloc[id_parent];
                inode_parent->repertoire.fichiers_contenus[slot].id_inode = -1;
                inode_parent->repertoire.fichiers_contenus[slot].nom = NULL;
                inode_parent->repertoire.nb_fichiers = inode_parent->repertoire.nb_fichiers-1;
                Remove_INODE(partition,id_dossier);
            }
            return 0;
        }
    }
}

//Fonction de création d'un fichier
int create_file(DISK *partition,char *nom,int id_parent)
{
    int inode_id;
    INODE * parent = partition->superbloc[id_parent];

    if(parent == NULL)
    {
        return ERR_UNUSUED_INODE;
    }
    else if(parent->type == FICHIER)
    {
        return ERR_INODE_IS_NOT_A_FOLDER;
    }
    else if(parent->repertoire.nb_fichiers == CONTENU_MAX_REPERTOIRES)
    {
        return ERR_DIRECTORY_FULL;
    }
    else
    {
        if(!check_inode_name_is_unique(parent,nom))
        {
            return ERR_NAME_ALREADY_USED_IN_FOLDER;
        }
        DEBUG("CREATION DOSSIER")
        int slot = seek_free_slot_folder(parent);
        inode_id = Creer_INODE(partition,nom,FICHIER);
        if(inode_id == ERR_NO_FREE_INODE)
        {
            return inode_id;
        }
        parent->repertoire.fichiers_contenus[slot].id_inode = inode_id;
        parent->repertoire.fichiers_contenus[slot].nom = nom;
        parent->repertoire.nb_fichiers = parent->repertoire.nb_fichiers + 1;
        partition->superbloc[inode_id]->id_parent = id_parent;

        return inode_id;
    }
}

//Fonction de suppression d'un fichier
int remove_file(DISK* partition,int id_fichier)
{

    INODE * fichier_cible = partition->superbloc[id_fichier];


    if(fichier_cible == NULL)
    {
        return ERR_UNUSUED_INODE;
    }
    if(fichier_cible->type != FICHIER)
    {
        return ERR_WRONG_TYPE;
    }

    int id_parent = fichier_cible->id_parent;
    INODE * parent = partition->superbloc[id_parent];

    if(parent == NULL)
    {
        return ERR_UNUSUED_PARENT_INODE;
    }
    else
    {
        ;
        int slot = seek_folder_slot_from_inode(partition->superbloc[id_parent],id_fichier);
        parent->repertoire.fichiers_contenus[slot].id_inode = -1;
        parent->repertoire.fichiers_contenus[slot].nom = NULL;
        parent->repertoire.nb_fichiers = parent->repertoire.nb_fichiers - 1;
        Remove_INODE(partition,id_fichier);

        return 0;
    }
}

//Fonctions affichant la liste des dossiers/fichiers contenus dans un dossier
int list_content_folder(DISK *partition,int id_dossier_pere)
{
    INODE * dossier_parent = partition->superbloc[id_dossier_pere];

    if(dossier_parent == NULL)
    {
        return ERR_UNUSUED_PARENT_INODE;
    }
    if(dossier_parent->type != DOSSIER)
    {
        return ERR_PARENT_NOT_FOLDER;
    }
    printf("    %s :\n",dossier_parent->nom);
    if(dossier_parent->repertoire.nb_fichiers == 0)
    {
        printf("	   Dossier vide\n");
        return 0;
    }
    int indice = 0;
    for(int i=0;i<CONTENU_MAX_REPERTOIRES;i++)
    {
        if(dossier_parent->repertoire.fichiers_contenus[i].id_inode != -1)
        {
            indice = dossier_parent->repertoire.fichiers_contenus[i].id_inode;
            printf("	  %s ",partition->superbloc[indice]->nom);
            if(partition->superbloc[indice]->type == FICHIER)
            {
                printf("\tFile");
            }
            else
            {
                printf("\tFolder");
            }
            printf("\t%s\n",partition->superbloc[indice]->metadata.date);
        }
    }
    printf("\n");
    return 0;
}

//Fonction de départ qui renvoie une partition, créer un dossier root et initialise la valeur de l'id du dossier actuel à celui de root
DISK Initialize_System(int* current_id)
{
    DISK partition;
    Initialiser_DISK(&partition);
    create_folder(&partition,"root",ROOT_PARENT_ID);
    *current_id = ROOT_INODE_ID;

    return partition;
}

//Fonction permettant de se déplacer dans l'un des dossier contenus dans le dossier actuel, ou bien son père
int change_current_directory(DISK* partition,int *current_id,char* nom_destination)
{
    int id_pere = *current_id;

    if(partition->superbloc[id_pere] == NULL)
    {
        return ERR_UNUSUED_PARENT_INODE;
    }
    if(partition->superbloc[id_pere] == FICHIER)
    {
        return ERR_PARENT_NOT_FOLDER;
    }

    INODE * parent= partition->superbloc[id_pere];
    
    //Si jamais le nom la chaine de caractère de destination est "..", on remonte dans le père
    //Comme le root a été initialisé comme étant père de lui-même, si on va dans le père de root, on ne bouge pas
    if(strcmp(nom_destination,"..")==0)
    {
		*current_id = parent->id_parent;
	}
		

    for(int i =0;i<CONTENU_MAX_REPERTOIRES;i++)
    {
        if(parent->repertoire.fichiers_contenus[i].id_inode != -1)
        {
            if(strcmp(parent->repertoire.fichiers_contenus[i].nom,nom_destination)==0 )
            {
                int indice = parent->repertoire.fichiers_contenus[i].id_inode;
                if(partition->superbloc[indice]->type == FICHIER)
                {
                    return ERR_DESTINATION_NOT_A_FOLDER;
                }
                *current_id = parent->repertoire.fichiers_contenus[i].id_inode;
                return 0;
            }
        }
    }
    return ERR_FOLDER_NOT_FOUND;


}

int seek_id(DISK* partition,int current_id,char* nom)
{
	INODE * parent= partition->superbloc[current_id];
	
	for(int i =0;i<CONTENU_MAX_REPERTOIRES;i++)
    {
        if(parent->repertoire.fichiers_contenus[i].id_inode != -1)
        {
            if(strcmp(parent->repertoire.fichiers_contenus[i].nom,nom)==0 )
            {
                return parent->repertoire.fichiers_contenus[i].id_inode;
			}
        }
    }
    return ERR_TARGET_NOT_FOUND;
}

