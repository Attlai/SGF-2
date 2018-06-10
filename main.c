#include <stdio.h>
#include <stdlib.h>
#include "struct.h"
#include "fonctions_liste.h"
#include "fonctions_node.h"

void test_creation_INODE();

int main()
{
    test_creation_INODE();

    return 0;
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
