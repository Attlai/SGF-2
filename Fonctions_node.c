#include "struct.h"
#include "fonctions_node.h"
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#define NEW(type) malloc(sizeof(type));

#ifdef DEBUG_ON
#define DEBUG(msg) printf(msg);
#define DEBUG1(msg,i) printf(msg,i);
#else
#define DEBUG(msg)
#define DEBUG1(msg,i)
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
    if(type==DOSSIER)
    {
        node->repertoire.id_parent = -1;
        for(int i=0;i<CONTENU_MAX_REPERTOIRES;i++)
        {
            node->repertoire.fichiers_contenus[i].id_inode = -1;
            node->repertoire.fichiers_contenus[i].nom = NULL;
        }
    }
    node->nom = name;
    node->metadata.taille_fichier = 0;
    node->metadata.date = clock();
    node->type = type;

    return indice;
}

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
            partition->superbloc[ROOT_INODE_ID]->repertoire.id_parent = ROOT_INODE_ID;
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
            DEBUG("CREATION FICHIER")
            inode_id = Creer_INODE(partition,nom,DOSSIER);
            //Si plus de place => remontée erreur
            if(inode_id == ERRNO_NO_FREE_INODE)
            {
                return inode_id;
            }
            else
            {

                INODE *inode_parent = partition->superbloc[id_parent];
                //On cherche si on peut rajouter un fichier au répertoire
                int slot = seek_free_slot_folder(inode_parent);
                DEBUG1("SLOT NUMBER IS %d",slot)
                if(slot == ERR_DIRECTORY_FULL)
                {
                    return slot;
                }
                inode_parent->repertoire.fichiers_contenus[slot].id_inode = inode_id;
                inode_parent->repertoire.fichiers_contenus[slot].nom = nom;
                partition->superbloc[inode_id]->repertoire.id_parent = id_parent;
                inode_parent->repertoire.nb_fichiers = inode_parent->repertoire.nb_fichiers+1;
                return inode_id;
            }

        }
    }
}

//int parcourir_contenu_dossier(INODE *dossier_parent,)

int remove_folder(DISK* partition,int id_dossier,mode_suppression mode)
{
    INODE * dossier = partition->superbloc[id_dossier];

    //On vérifie que le dossier existe
    if(dossier == NULL)
    {
        return ERR_UNUSUED_INODE;
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
            Remove_INODE(partition,id_dossier);
            //Si le dossier n'est pas la racine, on le supprime de son parent
            if(id_dossier != ROOT_INODE_ID)
            {
                int id_parent = dossier->repertoire.id_parent;
                int slot = seek_folder_slot_from_inode(partition->superbloc[id_parent],id_dossier);

                if(slot == ERR_INODE_ID_NOT_FOUND_IN_FOLDER)
                {
                    return slot;
                }
                INODE *inode_parent = partition->superbloc[id_parent];
                inode_parent->repertoire.fichiers_contenus[slot].id_inode = -1;
                inode_parent->repertoire.fichiers_contenus[slot].nom = NULL;
                inode_parent->repertoire.nb_fichiers = inode_parent->repertoire.nb_fichiers-1;
            }
            return 0;
        }
    }
}

//int create_file(DISK *partition,char *nom,)
