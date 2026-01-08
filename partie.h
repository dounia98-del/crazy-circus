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
//@brief
void afficherCartes(char*** permutations, int nbPermutations, int n);
Carte creerCarteDepuisPermutation(char** permut, int n, int sep);
Carte* genererToutesLesCartes(char*** permutations, int nbPermutations, int n, int* nbCartes);

void initPartie(Partie* partie, Joueur* joueurs, int nbJoueurs);
//@brief Initialise la partie : génère toutes les permutations et crée les cartes.
//@param partie Pointeur vers la structure Partie à initialiser.
//@param joueurs Tableau des joueurs déjà créés.
//@param nbJoueurs Nombre de joueurs.
void getCarte(const Carte* carte);
//@brief Affiche une carte (Départ et Cible) selon le format strict du sujet.
//@param carte Pointeur vers la carte à afficher.
Carte* tirerCarteAleatoire(Partie* partie);
//@brief Tire une carte aléatoire dans la pioche et la retire du paquet.
//@param partie Pointeur vers la partie en cours.
//@return Un pointeur vers la carte tirée(ou NULL si paquet vide).

void joueurSuivant(Partie* partie);
//@brief Passe au joueur suivant (gestion circulaire).
//@param partie Pointeur vers la partie.
int testerCarte(const Carte* carte, const char* sequence);
//@brief Vérifie si une séquence d'ordres permet de passer du départ à la cible.
//@param carte La carte défi(contenant départ et cible).
//@param sequence La chaîne de caractères contenant les ordres(ex: "KILOMA").
//@return 1 si la séquence est correcte, 0 sinon.
void libererPartie(Partie* partie);
//@brief Libère toute la mémoire allouée dynamiquement dans la structure Partie.
//@param partie Pointeur vers la structure à nettoyer.