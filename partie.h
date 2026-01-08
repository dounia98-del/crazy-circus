#pragma once
#include "jeu.h"
#include "joueur.h"
#include "config.h"

typedef struct {
    Position depart;
    Position cible;
} Carte;

typedef struct {
    Config config;
    Joueur* joueurs;
    int nbJoueurs;
    Carte* cartes;
    int nbCartes;
    int touractuel;
} Partie;

char*** genererToutesPermutations(char* animaux[], int n, int* nbPermutations);
//@brief Génère mathématiquement toutes les permutations possibles des animaux.
//@param[in] animaux Tableau des noms d'animaux.
//@param[in] n Nombre d'animaux.
//@param[out] nbPermutations Pointeur pour stocker le nombre total de permutations trouvées.
//@pre animaux != NULL && n > 0 && nbPermutations != NULL
//@return Un tableau à 3 dimensions contenant toutes les combinaisons.
void afficherCartes(char*** permutations, int nbPermutations, int n);
//@brief Affiche les permutations générées(Fonction utilitaire de debug).
//@param[in] permutations Le tableau des permutations.
//@param[in] nbPermutations Le nombre de permutations.
//@param[in] n Le nombre d'animaux.
//@pre permutations != NULL
Carte creerCarteDepuisPermutation(char** permut, int n, int sep);
//@brief Crée une carte de jeu à partir d'une permutation d'animaux donnée.
//@param[in] permut La liste d'animaux dans un ordre précis.
//@param[in] n Nombre d'animaux total.
//@param[in] sep Index de séparation entre le podium bleu et le rouge.
//@pre permut != NULL && n > 0 && sep >= 0 && sep <= n
//@return Une structure Carte initialisée.
Carte* genererToutesLesCartes(char*** permutations, int nbPermutations, int n, int* nbCartes);
//@brief Génère l'ensemble complet des cartes possibles pour la partie.
//@param[in] permutations Toutes les dispositions d'animaux possibles.
//@param[in] nbPermutations Nombre de dispositions.
//@param[in] n Nombre d'animaux.
//@param[out] nbCartes Pointeur pour stocker le nombre total de cartes créées.
//@return Un tableau dynamique contenant toutes les cartes.
void initPartie(Partie* partie, Joueur* joueurs, int nbJoueurs);
//@brief Initialise la partie : génère toutes les permutations et crée les cartes.
//@param[in,out] partie Pointeur vers la structure Partie à initialiser.
//@param[in] joueurs Tableau des joueurs déjà créés.
//@param[in] nbJoueurs Nombre de joueurs.
void getCarte(const Carte* carte);
//@brief Affiche une carte (Départ et Cible) selon le format strict du sujet.
//@param[in] carte Pointeur vers la carte à afficher.
//@pre carte != NULL
Carte* tirerCarteAleatoire(Partie* partie);
//@brief Tire une carte aléatoire dans la pioche et la retire du paquet.
//@param[in,out] partie Pointeur vers la partie en cours.
//@pre partie != NULL && partie->nbCartes > 0
//@return Un pointeur vers la carte tirée(ou NULL si paquet vide).

void joueurSuivant(Partie* partie);
//@brief Passe au joueur suivant (gestion circulaire).
//@param[in,out] partie Pointeur vers la partie.
//@pre partie != NULL && partie->nbJoueurs > 0
int testerCarte(const Carte* carte, const char* sequence);
//@brief Vérifie si une séquence d'ordres permet de passer du départ à la cible.
//@param[in] carte La carte défi(contenant départ et cible).
//@param[in] sequence La chaîne de caractères contenant les ordres(ex: "KILOMA").
// @pre carte != NULL && sequence != NULL
//@return 1 si la séquence est correcte, 0 sinon.
void libererPartie(Partie* partie);
//@brief Libère toute la mémoire allouée dynamiquement dans la structure Partie.
//@param[in,out] partie Pointeur vers la structure à nettoyer.
//@pre partie != NULL