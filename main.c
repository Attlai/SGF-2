#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "struct.h"
#include "fonctions_node.h"
#include "error.h"
#include "macro.h"
#include "fonctions_test.h"



int main()
{
    //On fait juste une série de test pour vérifier que toutes les fonctions marchent. Si il y a un probleme avec l'une d'entre elles, on aura un printf d'erreur
    TEST(test_initialisation_DISK)
    TEST(test_creer_INODE_normal)
    TEST(test_creer_INODE_11place)
    TEST(test_creer_INODE_full)
    TEST(test_remove_INODE)
    TEST(test_creer_dossier)
    TEST(test_remove_folder)
    TEST(test_creer_fichier)
    TEST(test_remove_fichier)
    TEST(test_list_content)

    return 0;
}

