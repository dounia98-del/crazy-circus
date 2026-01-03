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
void afficherCartes(char*** permutations, int nbPermutations, int n);
Carte creerCarteDepuisPermutation(char** permut, int n, int sep);
Carte* genererToutesLesCartes(char*** permutations, int nbPermutations, int n, int* nbCartes);

void initPartie(Partie* partie, Joueur* joueurs, int nbJoueurs);
void getCarte(const Carte* carte);
Carte* tirerCarteAleatoire(Partie* partie);
void joueurSuivant(Partie* partie);
int testerCarte(const Carte* carte, const char* sequence);
