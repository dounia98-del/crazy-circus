#include "partie.h"
#include "jeu.h"
#include "joueur.h"
#include "config.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// Permutations
char*** genererToutesPermutations(char* animaux[], int n, int* nbPermutations) {
    int total = 1; for (int i = 2; i <= n; ++i) total *= i;
    *nbPermutations = total;
    char*** permutations = malloc(total * sizeof(char**));
    for (int i = 0; i < total; ++i) permutations[i] = malloc(n * sizeof(char*));
    char** copie = malloc(n * sizeof(char*));
    for (int i = 0; i < n; ++i) copie[i] = animaux[i];
    int* c = malloc(n * sizeof(int)); for (int i = 0; i < n; ++i) c[i] = 0;
    for (int i = 0; i < n; ++i) permutations[0][i] = copie[i];
    int index = 1, i = 0;
    while (i < n) {
        if (c[i] < i) {
            int j = (i % 2 == 0) ? 0 : c[i];
            char* temp = copie[i]; copie[i] = copie[j]; copie[j] = temp;
            for (int k = 0; k < n; ++k) permutations[index][k] = copie[k];
            index++; c[i]++; i = 0;
        }
        else { c[i] = 0; i++; }
    }
    free(c); free(copie); return permutations;
}
void afficherCartes(char*** permutations, int nbPermutations, int n) {}

// Création Carte 
Carte creerCarteDepuisPermutation(char** permut, int n, int sep) {
    Carte carte;
    carte.depart.bleu.animaux = (char**)malloc(10 * sizeof(char*));
    carte.depart.rouge.animaux = (char**)malloc(10 * sizeof(char*));
    carte.depart.bleu.capacite = 10; carte.depart.rouge.capacite = 10;
    carte.depart.bleu.nbAnimaux = 0; carte.depart.rouge.nbAnimaux = 0;
    for (int i = 0; i < sep; i++) carte.depart.bleu.animaux[carte.depart.bleu.nbAnimaux++] = permut[i];
    for (int i = sep; i < n; i++) carte.depart.rouge.animaux[carte.depart.rouge.nbAnimaux++] = permut[i];

    carte.cible.bleu.animaux = (char**)malloc(10 * sizeof(char*));
    carte.cible.rouge.animaux = (char**)malloc(10 * sizeof(char*));
    carte.cible.bleu.capacite = 10; carte.cible.rouge.capacite = 10;
    carte.cible.bleu.nbAnimaux = carte.depart.bleu.nbAnimaux;
    carte.cible.rouge.nbAnimaux = carte.depart.rouge.nbAnimaux;
    for (int i = 0; i < carte.depart.bleu.nbAnimaux; i++) carte.cible.bleu.animaux[i] = carte.depart.bleu.animaux[i];
    for (int i = 0; i < carte.depart.rouge.nbAnimaux; i++) carte.cible.rouge.animaux[i] = carte.depart.rouge.animaux[i];
    return carte;
}

Carte* genererToutesLesCartes(char*** permutations, int nbPermutations, int n, int* nbCartes) {
    int total = nbPermutations * (n + 1);
    *nbCartes = total;
    Carte* cartes = malloc(total * sizeof(Carte));
    int index = 0;
    for (int p = 0; p < nbPermutations; p++) {
        char** permut = permutations[p];
        for (int sep = 0; sep <= n; sep++) cartes[index++] = creerCarteDepuisPermutation(permut, n, sep);
    }
    return cartes;
}

void initPartie(Partie* partie, Joueur* joueurs, int nbJoueurs) {
    partie->joueurs = joueurs;
    partie->nbJoueurs = nbJoueurs;
    partie->touractuel = 0;
    int nbPermutations;
    char*** permutations = genererToutesPermutations(partie->config.animaux, partie->config.nbAnimaux, &nbPermutations);
    partie->cartes = genererToutesLesCartes(permutations, nbPermutations, partie->config.nbAnimaux, &partie->nbCartes);
    for (int i = 0; i < nbPermutations; i++) free(permutations[i]);
    free(permutations);
}

// On utilise une largeur fixe de 12 caractères pour que tout soit droit.
void getCarte(const Carte* carte) {
    int hauteur = 3;

    for (int y = hauteur - 1; y >= 0; y--) {
        // --- DEPART ---
        if (y < carte->depart.bleu.nbAnimaux) printf("%-12s", carte->depart.bleu.animaux[y]);
        else printf("%-12s", "");

        if (y < carte->depart.rouge.nbAnimaux) printf("%-12s", carte->depart.rouge.animaux[y]);
        else printf("%-12s", "");

        printf("   "); // Espace central

        // --- CIBLE ---
        if (y < carte->cible.bleu.nbAnimaux) printf("%-12s", carte->cible.bleu.animaux[y]);
        else printf("%-12s", "");

        if (y < carte->cible.rouge.nbAnimaux) printf("%-12s", carte->cible.rouge.animaux[y]);
        else printf("%-12s", "");

        printf("\n");
    }

    
    printf("----        ----           ==>  ----        ----\n");
    printf("BLEU        ROUGE               BLEU        ROUGE\n");
}


// Cette fonction elle pioche une vraie carte du paquet et réduit le paquet pour ne pas la revoir.
Carte* tirerCarteAleatoire(Partie* partie) {
    if (partie->nbCartes == 0) return NULL; // Plus de cartes

	// On choisit un index au hasard parmi les cartes qui restent
    int index = rand() % partie->nbCartes;

    // On récupère l'adresse de cette carte
    Carte* carteChoisie = &partie->cartes[index];

    // Pour supprimer la carte du paquet sans tout décaler,
    // on échange la carte choisie avec la derniere carte du tableau,
    // puis on réduit la taille du tableau de 1.

    if (index != partie->nbCartes - 1) {
        Carte temp = partie->cartes[index];
        partie->cartes[index] = partie->cartes[partie->nbCartes - 1];
        partie->cartes[partie->nbCartes - 1] = temp;
    }

   
    // on va décrémenter le nombre de cartes et renvoyer celle qu'on a mise à la fin
    partie->nbCartes--;
    return &partie->cartes[partie->nbCartes];
}

void joueurSuivant(Partie* partie) {
    partie->touractuel = (partie->touractuel + 1) % partie->nbJoueurs;
}