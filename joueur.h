#pragma once
#include <stdbool.h>

typedef struct {
    char nom[30];   // identit� du joueur
    int score;      // points gagn�s
    bool peutjouer;  // booleen : true = peut jouer false = �limin� pour ce tour
} Joueur;

Joueur* initialiserJoueurs(int argc, const char* argv[], int* nbJoueurs);
//@brief Initialise le tableau des joueurs � partir des arguments du programme.
//@param[in]  argc      Nombre d'arguments de la ligne de commande.
//@param[in]  argv      Tableau des arguments(cha�nes de caract�res).
//@param[out] nbJoueurs Pointeur pour stocker le nombre de joueurs cr��s.
//@pre argc >= 2 && argv != NULL && nbJoueurs != NULL
//@return Un tableau dynamique de structures Joueur initialis�es.

void ajouterPoint(Joueur* joueur);
//@brief Incr�mente le score d'un joueur de 1 point.
//@param[in, out] joueur Pointeur vers le joueur qui a gagn�.
//@pre joueur != NULL
