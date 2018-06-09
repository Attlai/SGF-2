
typedef struct LISTE_Fichier LISTE_Fichier;
typedef struct Fichier Fichier;
typedef struct CHEMIN CHEMIN;

struct Fichier
{
    int Id;
    LISTE_Fichier *contenu;
    Fichier * parent;
    CHEMIN *path;
    //char id_User;
    //char* id_Etranger[];
    char *nom_fichier;
    int type;

};

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

typedef enum {
    FICHIER,DOSSIER
}type_fichier;

