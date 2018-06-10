#include "struct.h"
#include "fonctions_node.h"
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>


void Initialiser_META(META* metadata)
{
    metadata->taille_fichier = 0;
    char string[11] = "00/00/0000";
    strncpy(metadata->date_creation,string,11);
}

void Initialiser_INODE(INODE *node)
{
    node->id = -1;
    node->nom = NULL;
    Initialiser_META(&node->metadata);
}
