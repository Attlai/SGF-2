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
