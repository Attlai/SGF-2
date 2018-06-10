#include <stdio.h>
#include <stdlib.h>
#include "struct.h"
#include "fonctions_liste.h"
#include "fonctions_node.h"

void test_creation_INODE();
bool test_initialisation_DISK();


int main()
{
    test_creation_INODE();
    printf(test_initialisation_DISK()? " + test_initialisation_DISK : OK" : " !! test_initialisation_DISK : KO");


    return 0;
}

bool test_initialisation_DISK()
{
    DISK p1;
    bool err_init = true;

    Initialiser_DISK(&p1);
    if(p1.last_id != 1)
    {
        printf("ERR INIT DISK : LAST_ID != 1 (%d)\n",p1.last_id);
        err_init=false;
    }

    for(int i=0;i<TAILLE_MAX_DISK;i++)
    {
        if(p1.superbloc[i]!=NULL)
        {
            printf("ERR INIT DISK : SUPERBLOCK NOT NULL (BLOCK %d)\n",i);
            err_init = false;
            break;
        }
    }
    return err_init;

}

void test_creation_INODE()
{
    INODE i;
    Initialiser_INODE(&i);
    printf("%d\n",i.id);
    if(i.nom == NULL)
    {
        printf("NOM NUL\n");
    }
    printf("%d\n",i.metadata.taille_fichier);
    printf("%s\n",i.metadata.date_creation);
    printf("\n");
}
