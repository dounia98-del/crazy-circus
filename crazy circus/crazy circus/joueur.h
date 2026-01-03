#pragma once
#include <stdbool.h>

typedef struct {
    char nom[30];   // identité du joueur
    int score;      // points gagnés
    bool peutjouer;  // true = peut jouer false = éliminé pour ce tour
} Joueur;

void lireJoueurs(const char* argv[], int argc);
//@brief Valide les joueurs passé en ligne de commande .
//@param argv Tableau des arguments de la ligne de commande.
//@param argc Nombre d'arguments dans argv.
//@pre Doit y avoir au moins 2 joueurs avec des noms distincts.

Joueur* initialiserJoueurs(int argc, const char* argv[], int* nbJoueurs);
//@brief Initialise les joueurs du jeu.
//@param argc Nombre d'arguments dans argv.
//@param argv Tableau des arguments de la ligne de commande.
//@param nbJoueurs Pointeur vers un entier où sera stocké le nombre de joueurs initialisés.
//@return Un tableau dynamique de structures Joueur initialisées.

 

// !!!! metrre les in and out dans les commentaires des fonctions !!!!

void ajouterPoint(Joueur* joueur);
