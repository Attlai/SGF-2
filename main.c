#include <stdio.h>
#include <stdlib.h>
#include "struct.h"
#include "fonctions_liste.h"

int main()
{
    int Last_Id = 1;

    /*Fichier root = Creer_root();
    printf("ID ROOT : %d\nNOM ROOT: %s\n",root.Id,root.nom_fichier);
    printf("CREATION 1\n");
    Fichier f1 = Creer_Fichier(&root,&Last_Id,"fichier 1",DOSSIER);
    printf("ID F1 : %d\nNOM F1: %s\n",f1.Id,f1.nom_fichier);
    printf("ID PERE F1 : %d\nNOM PERE F1: %s\n",f1.pere->Id,f1.pere->nom_fichier);*/

    Fichier *root = Creer_Fichier(NULL, &Last_Id , "root",DOSSIER);
    printf("ID NOEUD : %d\nNOM NOEUD: %s\n",root->Id,root->nom_fichier);
    Fichier *f2 = Creer_Fichier(root, &Last_Id, "fichier 2",FICHIER);
    Fichier *f3 = Creer_Fichier(root, &Last_Id, "fichier 3",FICHIER);
    Fichier *f4 = Creer_Fichier(f3, &Last_Id,"fichier 3",FICHIER);
    Fichier *f5 = Creer_Fichier(f2, &Last_Id,"fichier 3",FICHIER);

    Afficher_Arbre(root);


    //Afficher_chemin(f5);

    //printf("ID SUCCESSEUR 1 : %d\n",f1.Successeurs->f.Id);
    //printf("ID Final : %d\n",Chercher_Fichier(root,f5.path).Id);*/

    return 0;
}
