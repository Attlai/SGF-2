#include <time.h>
#include <stdbool.h>

#define NB_MAX_BLOCS 16
#define TAILLE_MAX_BLOC 1024
#define TAILLE_MAX_FILE (NB_MAX_BLOCS * TAILLE_MAX_BLOC)
#define CONTENU_MAX_REPERTOIRES 64
#define TAILLE_MAX_DISK 1024

typedef enum {
    FICHIER,DOSSIER
}type_inode;

typedef enum{
    NORMAL,RECURSIF
}mode_suppression;

//BLOC repr�sente un bloc de data d'un fichier
typedef struct
{
    char contenu[TAILLE_MAX_BLOC];

}BLOC;

//META repr�sente les m�tadonn�es d'un INODE
typedef struct
{
    int taille_fichier;
    clock_t date;
}META;

//ENTRE_REPERTOIRE repr�sente une entr�e d'un r�pertoire, soit un couple (id_inode, nom)
typedef struct
{
    char* nom;
    int id_inode;
}ENTREE_REPERTOIRE;

//BLOC_REPERTOIRE repr�sente le bloc de data unique sur lequel un inode de dossier pointe
//Il contient uniquement des couples de (id,nom) des fichiers qu'il contient
typedef struct
{
    int nb_fichiers;
    ENTREE_REPERTOIRE fichiers_contenus[CONTENU_MAX_REPERTOIRES];
}BLOC_REPERTOIRE;

//INODE repr�sente l'inode d'un fichier/dossier
//BLOC et BLOC_REPERTOIRE repr�sente en r�alit� la m�me chose: le bloc sur lequel pointe l'inode
//Cependant, dans le cas d'un dossier, il s'agira d'un bloc unique
typedef struct
{
    int id;
    char* nom;
    int id_parent;
    type_inode type;
    BLOC blocs[NB_MAX_BLOCS];
    BLOC_REPERTOIRE repertoire;
    META metadata;
}INODE;

//DISK repr�sente un bloc de stockage o� sont stock�s tous les inodes
typedef struct
{
    INODE *superbloc[TAILLE_MAX_DISK];
    int last_id;
}DISK;





