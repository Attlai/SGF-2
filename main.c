#include <stdio.h>
#include <stdlib.h>
#include "struct.h"
#include "fonctions_liste.h"
#include "fonctions_node.h"

int main()
{
    int Last_Id = 1;

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
    for(int i=0;i<NB_MAX_BLOCS;i++)
    {
        //printf("%s\n",i.blocs[i].contenu);
    }

    return 0;
}
