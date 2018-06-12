//Ce fichier ne doit contenir que des fonctions de test
//Ces fonctions ne servent aucunement au fonctionnement du SGF

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "struct.h"
#include "fonctions_test.h"
#include "macro.h"
#include "error.h"
#include "fonctions_node.h"

//Test de la fonction d'initialisation d'un DISK
bool test_initialisation_DISK()
{
    INIT_TEST

    DISK p1;
    Initialiser_DISK(&p1);

    //On vérifie que le "dernier id" est égal à 1, après initialisation
    TESTU1(p1.last_id != 1,
          "ERR INIT DISK : LAST_ID != 1 (%d)\n", p1.last_id)

    //On vérifie que chaque slot du superbloc est nul
    for(int i=0;i<TAILLE_MAX_DISK;i++)
    {
        TESTU1(p1.superbloc[i]!=NULL,
            "ERR INIT DISK : SUPERBLOCK NOT NULL (BLOCK %d)\n",i)
    }

    END_TEST
}

//Test de la fonction de creation d'un INODE
bool test_creer_INODE_normal()
{
    INIT_TEST

    DISK p1;
    Initialiser_DISK(&p1);

    int inode_id = Creer_INODE(&p1,"root",FICHIER);

    //On vérifie qu'il y a bien de la place sur superbloc pour placer le INODE
    TESTU0(inode_id == ERR_NO_FREE_INODE,
        "ERR CREA INODE : NO FREE SPACE ON DISK")

    //On vérifie que le bloc où on a été créé l'INODE n'est plus null
    TESTU1(p1.superbloc[inode_id] == NULL,
        "ERR CREA INODE : BLOCK STILL EMPTY (%d)",inode_id)

    END_TEST
}

//Test de la fonction de creation d'un INODE dans le cas où on créé 11 INODES
bool test_creer_INODE_11place()
{
    INIT_TEST

    DISK p1;
    Initialiser_DISK(&p1);

    for(int i = 1;i<11;i++)
    {
        p1.superbloc[i] = (INODE*)1;
    }

    int inode_id = Creer_INODE(&p1,"root",FICHIER);

    //On vérifie que le superbloc n'est pas plein
    TESTU0(inode_id == ERR_NO_FREE_INODE,
        "ERR CREA INODE : NO FREE SPACE ON DISK")

    //On vérifie qu'en créant 11 INODEs, le 11e inodes est bien placé sur le slot 11 (sachant que le disque est vide au départ)
    TESTU1(inode_id != 11,
        "ERR CREA INODE : WRONG PLACE (%d)",inode_id)

    //On vérifie que que le slot du 11e inode n'est pas null
    TESTU1(p1.superbloc[inode_id] == NULL,
        "ERR CREA INODE : BLOCK STILL EMPTY (%d)",inode_id)

    END_TEST
}

//Test de la fonction de creation d'un inode dans le cas où le superbloc est entièrement rempli
bool test_creer_INODE_full()
{
    INIT_TEST

    DISK p1;
    Initialiser_DISK(&p1);

    //On remplit tout le disque
    for(int i=0;i<TAILLE_MAX_DISK;i++)
    {
        p1.superbloc[i] = (INODE*)1;
    }

    int inode_id = Creer_INODE(&p1,"root",FICHIER);

    //On vérifie qu'on récupère bien l'erreur "plus de place" si on veut ajouter un inode
    TESTU0(inode_id != ERR_NO_FREE_INODE,
        "ERR CREA INODE FULL: FREE SPACE ON DISK")

    END_TEST
}

//Test de la fonction de suppression d'un inode
bool test_remove_INODE()
{
    INIT_TEST

    DISK p1;
    Initialiser_DISK(&p1);
    int inode_id;

    //On remplit le disque
    for(int i=0;i<TAILLE_MAX_DISK;i++)
    {
        Creer_INODE(&p1,"test",FICHIER);
    }

    Remove_INODE(&p1,50);
    inode_id = Trouver_place_DISK(&p1);

    //On vérifie que qu'après avoir supprimé l'inode sur le slot 50, on a pas d'erreur de "plus de place"
    TESTU0(inode_id == ERR_NO_FREE_INODE,
           "ERR RM INODE : SLOT NOT FREED")

    //On vérifie que le slot où il reste de la place est le slot 50
    TESTU1(inode_id != 50,
           "ERR RM INODE : WRONG SLOT (%d)",inode_id)

    END_TEST
}

//Test de la fonction de creation d'un fichier
bool test_creer_dossier()
{
    INIT_TEST

    DISK p1;
    Initialiser_DISK(&p1);
    int inode_id;


    inode_id = create_folder(&p1,"root",ROOT_PARENT_ID);

    //On vérifie que le dosssier est créé sur le bon slot du disque
    TESTU1(inode_id != ROOT_INODE_ID,
        "ERR CREA DOSSIER : ROOT CREATED WITH WRONG ID (%d)",inode_id)

    //On fait un test pour vérifier qu'en ajoutant un dossier dans root, on le retrouve bien dans la liste des fichiers contenus de root (id, puis nom)
    inode_id = create_folder(&p1,"dossier 1",ROOT_INODE_ID);
    ENTREE_REPERTOIRE entree_dossier = p1.superbloc[ROOT_INODE_ID]->repertoire.fichiers_contenus[0];

    TESTU1(entree_dossier.id_inode != inode_id,
        "ERR CREA DOSSIER : WRONG CHILD ID (%d)",entree_dossier.id_inode)

    TESTU1(strcmp(entree_dossier.nom,"dossier 1") != 0,
        "ERR CREA DOSSIER : WRONG CHILD NAME (%s)",entree_dossier.nom)

    TESTU1(p1.superbloc[inode_id]->id_parent != ROOT_INODE_ID,
        "ERR CREA DOSSIER : WRONG PARENT ID (%d)",entree_dossier.id_inode)


    END_TEST
}

//Test de la fonction de la suppression d'un dossier
bool test_remove_folder()
{
    INIT_TEST

    DISK p1;
    Initialiser_DISK(&p1);
    int inode_id;

    inode_id = remove_folder(&p1,42,NORMAL);

    TESTU0(inode_id != ERR_UNUSUED_INODE,
          "ERR RM DOSSSIER : FOLDER DOES NOT EXIST")

    inode_id = create_folder(&p1,"root",ROOT_PARENT_ID);
    inode_id = create_folder(&p1,"dossier 1",ROOT_INODE_ID);

    int err_nb = remove_folder(&p1,ROOT_INODE_ID,NORMAL);

    TESTU0(err_nb != ERR_DIRECTORY_NOT_EMPTY,
        "ERR RM DOSSIER : DELETED NOT EMPTY FOLDER")

    remove_folder(&p1,inode_id,NORMAL);

    TESTU0(p1.superbloc[ROOT_INODE_ID]->repertoire.nb_fichiers != 0,
        "ERR RM DOSSIER : ROOT STILL HAS CHILD")

    END_TEST
}

//Test de la fonction de creation d'un fichier
bool test_creer_fichier()
{
    INIT_TEST

    DISK p1;
    Initialiser_DISK(&p1);

    create_folder(&p1,"root",ROOT_PARENT_ID);
    create_file(&p1,"fichier 1",ROOT_INODE_ID);
    create_file(&p1,"fichier 2",ROOT_INODE_ID);

    BLOC_REPERTOIRE root_contenu = p1.superbloc[ROOT_INODE_ID]->repertoire;

    TESTU1(root_contenu.nb_fichiers != 2,
        "ERR CREA FICHIER : WRONG FILE NUMBER (%d)",root_contenu.nb_fichiers)

    char* name[2] = {NULL};
    int k = 0;
    for(int i=0;i<CONTENU_MAX_REPERTOIRES;i++)
    {
        if(root_contenu.fichiers_contenus[i].id_inode != -1)
        {
            name[k] = root_contenu.fichiers_contenus[i].nom;
            k++;
        }
    }

    TESTU1(k!=2,
        "ERR CREA FICHIERS : WRONG INSTANCIATED INODES NUMBER (%d)",k)

    for(int i=0;i<2;i++)
    {
        TESTU1(((strcmp(name[i],"fichier 1")!=0)&&((strcmp(name[i],"fichier 2")!=0))),
            "ERR CREA FICHIERS : WRONG FIRST FILE NAME IN FOLDER (%s)",name[i])
    }

    END_TEST
}

//Test de la fonction de suppression d'un fichier
bool test_remove_fichier()
{
    INIT_TEST

    DISK p1;
    Initialiser_DISK(&p1);
    int inode_id;

    create_folder(&p1,"root",ROOT_PARENT_ID);
    inode_id = create_file(&p1,"fichier 1",ROOT_INODE_ID);
    create_file(&p1,"fichier 2",ROOT_INODE_ID);
    remove_file(&p1,inode_id);

    BLOC_REPERTOIRE root_contenu = p1.superbloc[ROOT_INODE_ID]->repertoire;

    TESTU1(root_contenu.nb_fichiers != 1,
        "ERR RM FICHIER : WRONG FILE NUMBER (%d)",root_contenu.nb_fichiers)

    int nb_fichiers = 0;
    int k;
    for(int i=0;i<CONTENU_MAX_REPERTOIRES;i++)
    {
        if(root_contenu.fichiers_contenus[i].id_inode != -1)
        {
            nb_fichiers++;
            k = i;
        }
    }

    TESTU1(nb_fichiers != 1,
        "ERR RM FICHIER : BAD INODE CLEANING (%d full instead of 1)",nb_fichiers)

    TESTU1(strcmp("fichier 2",root_contenu.fichiers_contenus[k].nom)!=0,
           "ERR RM FICHIER : WRONG FILE LEFT (%s)",root_contenu.fichiers_contenus[k].nom)

    END_TEST
}

bool test_list_content()
{
    INIT_TEST

    DISK p1;
    Initialiser_DISK(&p1);
    int code_erreur = 0;

    create_folder(&p1,"root",ROOT_PARENT_ID);
    create_file(&p1,"fichier 1",ROOT_INODE_ID);
    create_file(&p1,"fichier 2",ROOT_INODE_ID);
    create_file(&p1,"fichier 3",ROOT_INODE_ID);
    create_folder(&p1,"dossier 1",ROOT_INODE_ID);

    code_erreur = list_content_folder(&p1,2);
    TESTU0(code_erreur != ERR_PARENT_NOT_FOLDER,
        "ERR LIST : CAN LIST ON A FILE")

    code_erreur = list_content_folder(&p1,60);
    TESTU0(code_erreur != ERR_UNUSUED_PARENT_INODE,
        " ERR LIST : CAN LIST ON UNUSUED INODE")

    code_erreur = list_content_folder(&p1,5);
    TESTU0(code_erreur != ERR_EMPTY_FOLDER,
        " ERR LIST : DONT RETURN EMPTY FOLDER")

    list_content_folder(&p1,1);
    list_content_folder(&p1,5);

    END_TEST

}

bool test_initialisation_system()
{
    INIT_TEST

    int current_id = 0;
    DISK partition = Initialize_System(&current_id);

    TESTU1(current_id != 1,
        "ERR INIT : WRONG STARTING ID (%d)",current_id)

    int indice;

    bool all_null = true;
    for(int i= 2;i<TAILLE_MAX_DISK;i++)
    {
        if(partition.superbloc[i] != NULL)
        {
            all_null = false;
            indice = i;
            break;
        }
    }

    TESTU1(all_null == false,
        "ERR INIT : ATLEAST ONE BLOCK NOT NULL (%d)",indice)

    END_TEST
}

bool test_changement_de_location()
{
    INIT_TEST

    DISK p1;
    Initialiser_DISK(&p1);
    int code_erreur = 0;
    int current_id = ROOT_INODE_ID;

    create_folder(&p1,"root",ROOT_PARENT_ID);
    create_file(&p1,"fichier 1",ROOT_INODE_ID);
    create_file(&p1,"fichier 2",ROOT_INODE_ID);
    create_file(&p1,"fichier 3",ROOT_INODE_ID);
    create_folder(&p1,"dossier 1",ROOT_INODE_ID);
    create_file(&p1,"fichier 4",ROOT_INODE_ID);

    code_erreur = change_current_directory(&p1,&current_id,"fichier 2");

    TESTU0(code_erreur != ERR_DESTINATION_NOT_A_FOLDER,
        "ERR CD : CD ON A FILE")

    current_id = ROOT_INODE_ID;
    code_erreur = change_current_directory(&p1,&current_id,"dossier 2");

    TESTU0(code_erreur != ERR_FOLDER_NOT_FOUND,
        "ERR CD : CD ON A NON EXISTANT FOLDER")

    current_id = ROOT_INODE_ID;
    code_erreur = change_current_directory(&p1,&current_id,"dossier 1");

    TESTU1(current_id != 5,
        "ERR CD : WRONG NEW LOCATION (%s)",p1.superbloc[current_id]->nom)

    END_TEST

}
