#define NB_MAX_BLOCS 16
#define TAILLE_MAX_BLOC 1024
#define TAILLE_MAX_FILE (NB_MAX_BLOCS * TAILLE_MAX_BLOC)
#define CONTENU_MAX_REPERTOIRES 64


typedef struct LISTE_Fichier LISTE_Fichier;
typedef struct Fichier Fichier;
typedef struct CHEMIN CHEMIN;

typedef enum {
    FICHIER,DOSSIER
}type_fichier;

struct Fichier
{
    int Id;
    LISTE_Fichier *contenu;
    Fichier * parent;
    CHEMIN *path;
    char *nom_fichier;
    int type;

};

typedef struct
{
    char contenu[TAILLE_MAX_BLOC];

}BLOC;

typedef struct
{
    int taille_fichier;
    char date_creation[10+1];
}META;

typedef struct
{
    char* nom_fichier;
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
    type_fichier type;
    BLOC blocs[NB_MAX_BLOCS];
    BLOC_REPERTOIRE repertoire;
    META metadata;
}INODE;

struct LISTE_Fichier
{
    Fichier* f;
    LISTE_Fichier *suiv;
};

struct CHEMIN
{
    int Id;
    CHEMIN *suiv;
};



