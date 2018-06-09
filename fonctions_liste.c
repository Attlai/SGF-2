#include "struct.h"
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>




void Afficher_chemin_proc(CHEMIN *path)
{
    if(path == NULL)
    {
        printf("\n");
    }
    else
    {
        printf("/%d",path->Id);
        Afficher_chemin_proc(path->suiv);
    }
}

void Afficher_chemin(Fichier f)
{
    printf("CHEMIN %s :\n",f.nom_fichier);
    Afficher_chemin_proc(f.path);
}

CHEMIN *Aggrandir_chemin(CHEMIN *C,int new_id)
{
    CHEMIN* tmp = malloc(sizeof(CHEMIN));
	tmp->Id = new_id;
	tmp->suiv = C;

	return tmp;
}

CHEMIN *Remonter_chemin(CHEMIN *c,Fichier *f)
{
    CHEMIN *tmp = malloc(sizeof(CHEMIN));

    tmp->Id = f->Id;
    tmp->suiv = c;
    if(f->parent == NULL)
    {
        return tmp;
    }
    else
    {
        return Remonter_chemin(tmp,f->parent);
    }
}

bool Liste_est_vide(LISTE_Fichier* L)
{
    if(L == NULL)
    {
        return true;
    }
	else
    {
        return false;
    }
}

Fichier* Fichier_Vide()
{
    Fichier* f = malloc(sizeof(Fichier));
    f->Id = -1;
    f->contenu = NULL;

    return f;
}

bool Fichier_est_vide(Fichier f)
{
    if(f.Id == -1)
    {
        return true;
    }
    else
    {
        return false;
    }
}

LISTE_Fichier *Renvoyer_suivant(LISTE_Fichier* L)
{
    if(Liste_est_vide(L))
    {
        return NULL;
    }
    else
    {
        return L->suiv;
    }
}

void Afficher_liste_contenu_proc(LISTE_Fichier *succ)
{
    if(Liste_est_vide(succ)){}
    else
    {
        printf("FILS: %s\n",succ->f->nom_fichier);
        Afficher_liste_contenu_proc(Renvoyer_suivant(succ));
    }
}

void Afficher_liste_contenu(Fichier* parent)
{
    printf("LISTE contenu DE %s :\n",parent->nom_fichier);
    Afficher_liste_contenu_proc(parent->contenu);
}

LISTE_Fichier *Ajouter_Fichier_proc(LISTE_Fichier *L,Fichier * fi)
{
    LISTE_Fichier* tmp = malloc(sizeof(LISTE_Fichier));
	tmp->f = fi;
	tmp->suiv = L;
	return tmp;
}

void Ajouter_Fichier(Fichier *parent,Fichier *fils)
{

    if(parent != NULL)
    {
        LISTE_Fichier* new_fic = malloc(sizeof(LISTE_Fichier));
        new_fic->f = fils;
        new_fic->suiv = NULL;

        LISTE_Fichier * contenu = parent->contenu;
        if(contenu == NULL)
        {
            parent->contenu = new_fic;
        }
        else
        {
            while(contenu->suiv != NULL)
            {
                contenu = contenu->suiv;
            }
            contenu->suiv = new_fic;
        }
    }
    else{
            printf("PARENT NULL\n");
    }
}

Fichier* Chercher_Fichier(Fichier* root,CHEMIN *destination)
{
    //Fichier f = Chercher_Fichier_proc(root,destination->suiv);
    printf("BOUCLE\n");
    if(root->Id != destination->Id)
    {
        printf("Pas de correspondance\n");
        return Fichier_Vide();
    }
    else
    {
        Afficher_liste_contenu(root);
        printf("ID ACTUEL : %d\n",root->Id);
        printf("ID CHERCHE : %d\n",destination->suiv->Id);
        if(destination->suiv == NULL)
        {
            printf("FINI\n");
            return root;
        }
        else
        {
            LISTE_Fichier* enfants = enfants = root->contenu;
            while(enfants != NULL)
            {
                printf("ID FILS : %d\n",enfants->f->Id);
                if(enfants->f->Id == destination->suiv->Id)
                {
                    printf("FILS TROUVE\n");
                    return Chercher_Fichier(enfants->f,destination->suiv);
                }
                else
                {
                    enfants = enfants->suiv;
                }
            }
            return Fichier_Vide();
            printf("PAS NORMAL");
        }
    }
}

Fichier* Creer_Fichier(Fichier * parent,int* id,char * name,type_fichier t)
{
    Fichier *f = (Fichier*) malloc(sizeof(Fichier));
    f->Id = (*id)++;
    f->contenu =  NULL;
    f->parent = parent;
    Ajouter_Fichier(parent,f);
    //f->path = malloc(sizeof(CHEMIN));
    //f->path->suiv = NULL;
    //f->path->Id = f->Id;
    //f->path = Remonter_chemin(f->path,f->parent);
    f->nom_fichier = name;
    f->type = t;
    return f;
}

Fichier Creer_root()
{
    Fichier f;
    f.Id = 0;
    f.contenu = malloc(sizeof(LISTE_Fichier));
    f.contenu = NULL;
    f.parent = NULL;
    f.path = malloc(sizeof(CHEMIN));
    f.path->Id = 0;
    f.path->suiv = NULL;
    f.nom_fichier = "root";
    f.type = DOSSIER;
    return f;
}

void Afficher_Arbre(Fichier* racine)
{
    printf("ID NOEUD : %d\nNOM NOEUD: %s\n",racine->Id,racine->nom_fichier);
    LISTE_Fichier* contenu = racine->contenu;
    while(contenu != NULL)
    {
        Afficher_Arbre(contenu->f);
        contenu = contenu->suiv;
    }
}
