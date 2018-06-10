#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "struct.h"
#include "fonctions_liste.h"
#include "fonctions_node.h"


bool test_initialisation_INODE();
bool test_initialisation_DISK();

#define TEST(test_fonc)  printf(test_fonc()? " + %s : OK\n" : " !! %s : KO\n",  #test_fonc );
#define TESTU(test,message,code)\
              if(test)\
                {\
                printf(message, code); \
                test_ok = false;\
                }


int main()
{
    TEST(test_initialisation_INODE)
    TEST(test_initialisation_DISK);



    return 0;
}

bool test_initialisation_DISK()
{
    DISK p1;
    bool test_ok = true;

    Initialiser_DISK(&p1);

    TESTU(p1.last_id != 1,
          "ERR INIT DISK : LAST_ID != 1 (%d)\n", p1.last_id)

    for(int i=0;i<TAILLE_MAX_DISK;i++)
    {
        TESTU(p1.superbloc[i]!=NULL,
            "ERR INIT DISK : SUPERBLOCK NOT NULL (BLOCK %d)\n",i)
    }
    return test_ok;

}

bool test_initialisation_INODE()
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
}


