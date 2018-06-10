#include <time.h>
#include <stdbool.h>

#define NB_MAX_BLOCS 16
#define TAILLE_MAX_BLOC 1024
#define TAILLE_MAX_FILE (NB_MAX_BLOCS * TAILLE_MAX_BLOC)
#define CONTENU_MAX_REPERTOIRES 64
#define TAILLE_MAX_DISK 1024




typedef struct LISTE_Fichier LISTE_Fichier;
typedef struct Fichier Fichier;
typedef struct CHEMIN CHEMIN;

typedef enum {
    FICHIER,DOSSIER
}type_inode;

typedef enum{
    NORMAL,RECURSIF
}mode_suppression;

typedef struct
{
    char contenu[TAILLE_MAX_BLOC];

}BLOC;

typedef struct
{
    int taille_fichier;
    clock_t date;
}META;

typedef struct
{
    char* nom;
    int id_inode;
}ENTREE_REPERTOIRE;

typedef struct
{
    int nb_fichiers;
    ENTREE_REPERTOIRE fichiers_contenus[CONTENU_MAX_REPERTOIRES];
}BLOC_REPERTOIRE;

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

typedef struct
{
    INODE *superbloc[TAILLE_MAX_DISK];
    int last_id;
}DISK;





