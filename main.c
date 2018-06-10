#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "struct.h"
#include "fonctions_liste.h"
#include "fonctions_node.h"


bool test_initialisation_INODE();
bool test_initialisation_DISK();
bool test_creer_INODE_normal();
bool test_creer_INODE_full();
bool test_creer_INODE_11place();
bool test_remove_INODE();
bool test_creer_dossier();
bool test_remove_folder();



#define TEST(test_fonc)  printf(test_fonc()? " + %s : OK\n" : " !! %s : KO\n",  #test_fonc );
#define INIT_TEST    bool test_ok = true;
#define END_TEST     return test_ok;

#define TESTU1(test,message,code)\
              if(test)\
                {\
                printf(message, code); \
                test_ok = false;\
                }

#define TESTU0(test,message)\
              if(test)\
                {\
                printf(message); \
                test_ok = false;\
                }


int main()
{

    //TEST(test_initialisation_INODE)
    TEST(test_initialisation_DISK)
    TEST(test_creer_INODE_normal)
    TEST(test_creer_INODE_11place)
    TEST(test_creer_INODE_full)
    TEST(test_remove_INODE)
    TEST(test_creer_dossier)
    TEST(test_remove_folder)


    return 0;
}

bool test_initialisation_DISK()
{
    INIT_TEST

    DISK p1;
    Initialiser_DISK(&p1);

    TESTU1(p1.last_id != 1,
          "ERR INIT DISK : LAST_ID != 1 (%d)\n", p1.last_id)

    for(int i=0;i<TAILLE_MAX_DISK;i++)
    {
        TESTU1(p1.superbloc[i]!=NULL,
            "ERR INIT DISK : SUPERBLOCK NOT NULL (BLOCK %d)\n",i)
    }

    END_TEST
}

/*bool test_initialisation_INODE()
{
    bool test_ok = true;
    INODE i;
    Initialiser_INODE(&i);

    TESTU(i.id != -1,
          "ERR INIT INODE : ID INITIALIZATION WRONG (id = %d)\n",i.id)

    TESTU(i.nom != NULL,
        "ERR INIT INODE : NAME NOT NULL (%s)\n",i.nom)

    TESTU(i.metadata.taille_fichier != 0,
        "ERR INIT INODE : FILE SIZE NOT NULL (%d)\n",i.metadata.taille_fichier)

    TESTU(strcmp(i.metadata.date_creation, "00/00/0000") != 0,
        "ERR INIT INODE : CREATION DATE INITIALIZATION WRONG (%s)\n",i.metadata.date_creation)

    return test_ok;
}*/

bool test_creer_INODE_normal()
{
    INIT_TEST

    DISK p1;
    Initialiser_DISK(&p1);

    int inode_id = Creer_INODE(&p1,"root",FICHIER);

    TESTU0(inode_id == ERRNO_NO_FREE_INODE,
        "ERR CREA INODE : NO FREE SPACE ON DISK")

    TESTU1(p1.superbloc[inode_id] == NULL,
        "ERR CREA INODE : BLOCK STILL EMPTY (%d)",inode_id)

    END_TEST
}

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

    TESTU0(inode_id == ERRNO_NO_FREE_INODE,
        "ERR CREA INODE : NO FREE SPACE ON DISK")

    TESTU1(inode_id != 11,
        "ERR CREA INODE : WRONG PLACE (%d)",inode_id)

    TESTU1(p1.superbloc[inode_id] == NULL,
        "ERR CREA INODE : BLOCK STILL EMPTY (%d)",inode_id)

    END_TEST
}

bool test_creer_INODE_full()
{
    INIT_TEST

    DISK p1;
    Initialiser_DISK(&p1);
    for(int i=0;i<TAILLE_MAX_DISK;i++)
    {
        p1.superbloc[i] = (INODE*)1;
    }

    int inode_id = Creer_INODE(&p1,"root",FICHIER);

    TESTU0(inode_id != ERRNO_NO_FREE_INODE,
        "ERR CREA INODE FULL: FREE SPACE ON DISK")

    END_TEST
}

bool test_remove_INODE()
{
    INIT_TEST

    DISK p1;
    Initialiser_DISK(&p1);
    int inode_id;

    for(int i=0;i<TAILLE_MAX_DISK;i++)
    {
        Creer_INODE(&p1,"test",FICHIER);
    }

    Remove_INODE(&p1,50);
    inode_id = Trouver_place_DISK(&p1);

    TESTU0(inode_id == ERRNO_NO_FREE_INODE,
           "ERR RM INODE : SLOT NOT FREED")

    TESTU1(inode_id != 50,
           "ERR RM INODE : WRONG SLOT (%d)",inode_id)

    END_TEST
}

bool test_creer_dossier()
{
    INIT_TEST

    DISK p1;
    Initialiser_DISK(&p1);
    int inode_id;


    inode_id = create_folder(&p1,"root",ROOT_PARENT_ID);
    TESTU1(inode_id != ROOT_INODE_ID,
        "ERR CREA DOSSIER : ROOT CREATED WITH WRONG ID (%d)",inode_id)

    //On fait un test pour vérifier qu'en ajoutant un dossier dans root, on le retrouve bien dans la liste des fichiers contenus de root (id, puis nom)
    inode_id = create_folder(&p1,"dossier 1",ROOT_INODE_ID);
    ENTREE_REPERTOIRE entree_dossier = p1.superbloc[ROOT_INODE_ID]->repertoire.fichiers_contenus[0];

    TESTU1(entree_dossier.id_inode != inode_id,
        "ERR CREA DOSSIER : WRONG CHILD ID (%d)",entree_dossier.id_inode)

    TESTU1(strcmp(entree_dossier.nom,"dossier 1") != 0,
        "ERR CREA DOSSIER : WRONG CHILD NAME (%s)",entree_dossier.nom)

    TESTU1(p1.superbloc[inode_id]->repertoire.id_parent != ROOT_INODE_ID,
        "ERR CREA DOSSIER : WRONG PARENT ID (%d)",entree_dossier.id_inode)


    END_TEST
}

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
