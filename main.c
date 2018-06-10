#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "struct.h"
#include "fonctions_liste.h"
#include "fonctions_node.h"


bool test_initialisation_INODE();
bool test_initialisation_DISK();


int main()
{
    printf(test_initialisation_INODE()? " + test_initialisation_INODE : OK\n" : " !! test_initialisation_INODE : KO\n");
    printf(test_initialisation_DISK()? " + test_initialisation_DISK : OK\n" : " !! test_initialisation_DISK : KO\n");


    return 0;
}

bool test_initialisation_DISK()
{
    DISK p1;
    bool test_ok = true;

    Initialiser_DISK(&p1);
    if(p1.last_id != 1)
    {
        printf("ERR INIT DISK : LAST_ID != 1 (%d)\n",p1.last_id);
        test_ok=false;
    }

    for(int i=0;i<TAILLE_MAX_DISK;i++)
    {
        if(p1.superbloc[i]!=NULL)
        {
            printf("ERR INIT DISK : SUPERBLOCK NOT NULL (BLOCK %d)\n",i);
            test_ok = false;
            break;
        }
    }
    return test_ok;

}

bool test_initialisation_INODE()
{
    bool test_ok = true;
    INODE i;
    Initialiser_INODE(&i);
    if(i.id != -1)
    {
        printf("ERR INIT INODE : ID INITIALIZATION WRONG (id = %d)\n",i.id);
        test_ok = false;
    }
    if(i.nom != NULL)
    {
        printf("ERR INIT INODE : NAME NOT NULL (%s)\n",i.nom);
        test_ok = false;
    }
    if(i.metadata.taille_fichier != 0)
    {
        printf("ERR INIT INODE : FILE SIZE NOT NULL (%d)\n",i.metadata.taille_fichier);
        test_ok = false;
    }
    if(strcmp(i.metadata.date_creation, "00/00/0000") != 0)
    {
        printf("ERR INIT INODE : CREATION DATE INITIALIZATION WRONG (%s)\n",i.metadata.date_creation);
        test_ok = false;
    }
    return test_ok;
}
