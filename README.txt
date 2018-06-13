Ce SGF ne gère pas la permanence des données, toutes les données sont stockées dans des variables.
Ce SGF ne gère pas non plus les primitives de gestion de data des fichiers, on ne peut pas écrire ou lire dans les fichiers, mais on peu faire toute autre type de gestion basique
La commande "my_cd" ne permet que que de déplacer de père en fils, ou de fils en père.


Le SGF ne fonctionne pas sur systeme Windows en raison de l'absence de la bibliothèque wait.h


Ligne de commande pour la compilation:

gcc main.c Fonctions_nodes.c fonctions_test.c shellTerminal.c