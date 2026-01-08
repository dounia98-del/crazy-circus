#pragma once
#include <stdbool.h>

typedef struct {
    char nom[30];   // identité du joueur
    int score;      // points gagnés
    bool peutjouer;  // booleen : true = peut jouer false = éliminé pour ce tour
} Joueur;

void lireJoueurs(const char* argv[], int argc);
//@brief Valide les joueurs passé en ligne de commande .
//@param argv Tableau des arguments de la ligne de commande.
//@param argc Nombre d'arguments dans argv.
//@pre Doit y avoir au moins 2 joueurs avec des noms distincts.
//@return Un tableau dynamique de structures Joueur initialisées.
Joueur* initialiserJoueurs(int argc, const char* argv[], int* nbJoueurs);
//@brief Initialise le tableau des joueurs à partir des arguments du programme.
//@param[in]  argc      Nombre d'arguments de la ligne de commande.
//@param[in]  argv      Tableau des arguments(chaînes de caractères).
//@param[out] nbJoueurs Pointeur pour stocker le nombre de joueurs créés.
//@pre argc >= 2 && argv != NULL && nbJoueurs != NULL
//@return Un tableau dynamique de structures Joueur initialisées.

void ajouterPoint(Joueur* joueur);
//@brief Incrémente le score d'un joueur de 1 point.
//@param[in, out] joueur Pointeur vers le joueur qui a gagné.
//@pre joueur != NULL
