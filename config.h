#pragma once

typedef struct {
    char** animaux;
    int nbAnimaux;
    char** ordres;
    int nbOrdres;
} Config;


Config lireConfig(const char* nomFichier);
//@brief Lit et valide la configuration du jeu à partir d'un fichier.
//@param nomFichier Le nom du fichier de configuration.
//@return Une structure Config contenant les animaux et les ordres autorisés.
//@note Le fichier doit avoir au moins 2 animaux et 3 ordres autorisés.

void afficherConfig(const Config* config);
//@brief Affiche la configuration du jeu.
//@param config Pointeur vers la structure Config à afficher. 

void libererConfig(Config* config);




