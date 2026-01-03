#include "partie.h"
#include "jeu.h"
#include "joueur.h"
#include "config.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>


char*** genererToutesPermutations(char* animaux[], int n, int* nbPermutations) {
    int total = 1;
    for (int i = 2; i <= n; ++i) total *= i;
    *nbPermutations = total;

    char*** permutations = malloc(total * sizeof(char**));
    for (int i = 0; i < total; ++i) {
        permutations[i] = malloc(n * sizeof(char*));
    }

    char** copie = malloc(n * sizeof(char*));
    for (int i = 0; i < n; ++i) {
        copie[i] = animaux[i];
    }

    int* c = malloc(n * sizeof(int));
    for (int i = 0; i < n; ++i) {
        c[i] = 0;
    }

    for (int i = 0; i < n; ++i) {
        permutations[0][i] = copie[i];
    }

    int index = 1;
    int i = 0;

    while (i < n) {
        if (c[i] < i) {
            int j = (i % 2 == 0) ? 0 : c[i];

            char* temp = copie[i];
            copie[i] = copie[j];
            copie[j] = temp;

            for (int k = 0; k < n; ++k) {
                permutations[index][k] = copie[k];
            }
            index++;

            c[i]++;
            i = 0;
        }
        else {
            c[i] = 0;
            i++;
        }
    }

    free(c);
    free(copie);

    return permutations;
}

void afficherCartes(char*** permutations, int nbPermutations, int n) {
    for (int p = 0; p < nbPermutations; ++p) {
        char** permut = permutations[p];

        for (int sep = 0; sep <= n; ++sep) {
            for (int i = 0; i < sep; ++i) {
                printf("%s", permut[i]);
            }

            printf("|");

            for (int i = sep; i < n; ++i) {
                printf("%s", permut[i]);
            }

            printf("\n");
        }

        printf("------\n");
    }
}

Carte creerCarteDepuisPermutation(char** permut, int n, int sep) {
    Carte carte;

    carte.depart.bleu.nbAnimaux = 0;
    carte.depart.rouge.nbAnimaux = 0;

    // On suppose que les tableaux animaux des podiums sont déjà alloués
    // ou que tu as une capacité max fixée ailleurs

    for (int i = 0; i < sep; i++) {
        carte.depart.bleu.animaux[carte.depart.bleu.nbAnimaux++] = permut[i];
    }

    for (int i = sep; i < n; i++) {
        carte.depart.rouge.animaux[carte.depart.rouge.nbAnimaux++] = permut[i];
    }

    carte.cible = carte.depart;

    return carte;
}

Carte* genererToutesLesCartes(char*** permutations, int nbPermutations, int n, int* nbCartes) {
    int total = nbPermutations * (n + 1);
    *nbCartes = total;

    Carte* cartes = malloc(total * sizeof(Carte));
    int index = 0;

    for (int p = 0; p < nbPermutations; p++) {
        char** permut = permutations[p];

        for (int sep = 0; sep <= n; sep++) {
            cartes[index++] = creerCarteDepuisPermutation(permut, n, sep);
        }
    }

    return cartes;
}

void initPartie(Partie* partie, Joueur* joueurs, int nbJoueurs) {
    partie->joueurs = joueurs;
    partie->nbJoueurs = nbJoueurs;
    partie->touractuel = 0;

    int nbPermutations;
    char*** permutations = genererToutesPermutations(partie->config.animaux,
        partie->config.nbAnimaux,
        &nbPermutations);

    partie->cartes = genererToutesLesCartes(permutations,
        nbPermutations,
        partie->config.nbAnimaux,
        &partie->nbCartes);

    for (int i = 0; i < nbPermutations; i++) {
        free(permutations[i]);
    }
    free(permutations);
}

void getCarte(const Carte* carte) {
    printf("Départ :\n");
    printf("  Bleu : ");
    for (int i = 0; i < carte->depart.bleu.nbAnimaux; i++) {
        printf("%s ", carte->depart.bleu.animaux[i]);
    }
    printf("\n");

    printf("  Rouge : ");
    for (int i = 0; i < carte->depart.rouge.nbAnimaux; i++) {
        printf("%s ", carte->depart.rouge.animaux[i]);
    }
    printf("\n");

    printf("Cible :\n");
    printf("  Bleu : ");
    for (int i = 0; i < carte->cible.bleu.nbAnimaux; i++) {
        printf("%s ", carte->cible.bleu.animaux[i]);
    }
    printf("\n");

    printf("  Rouge : ");
    for (int i = 0; i < carte->cible.rouge.nbAnimaux; i++) {
        printf("%s ", carte->cible.rouge.animaux[i]);
    }
    printf("\n");
}

Carte* tirerCarteAleatoire(Partie* partie) {
    int index = rand() % partie->nbCartes;
    return &partie->cartes[index];
}

void joueurSuivant(Partie* partie) {
    partie->touractuel = (partie->touractuel + 1) % partie->nbJoueurs;
}
