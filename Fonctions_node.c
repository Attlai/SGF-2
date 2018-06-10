#include "struct.h"
#include "fonctions_node.h"
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#define NEW(type) malloc(sizeof(type));

#ifdef DEBUG_ON
#define DEBUG(msg) printf(msg);
#else
#define DEBUG(msg)
#endif //

/*//Fonction d'unitialisation d'un META
void Initialiser_META(META* metadata)
{
    metadata->taille_fichier = 0;
    char string[11] = "00/00/0000";
    strncpy(metadata->date_creation,string,11);
}

//Fonction d'initialisation de INODE, peu utile mais plus propre
void Initialiser_INODE(INODE *node)
{
    node->id = -1;
    node->nom = NULL;
    Initialiser_META(&node->metadata);
}*/

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

    return ERRNO_NO_FREE_INODE;
}

//Fonction qui ajouter un inode à un DISK, en renseignant le type de fichier, et renvoie l'indice (potentiellement erreur)
int Creer_INODE(DISK* partition,char *name,type_inode type)
{
    INODE * node = NULL;
    int indice = Trouver_place_DISK(partition);
    if(indice != ERRNO_NO_FREE_INODE)
    {
        node = NEW(INODE)
        partition->superbloc[indice] = node;
    }
    else
    {
        DEBUG("NO FREE SPACE ON DISK\n");
        return indice;
    }
    node->nom = name;
    node->metadata.taille_fichier = 0;
    node->metadata.date = clock();

    return indice;
}

int Remove_INODE(DISK* partition,int rm_id)
{
    INODE *rm_inode = partition->superbloc[rm_id];
    free(rm_inode);
    partition->superbloc[rm_id] = NULL;
}
