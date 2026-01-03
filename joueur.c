#define _CRT_SECURE_NO_WARNINGS
#include "joueur.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

void lireJoueurs(const char* argv[], int argc) {
    if (argc < 3) {
        printf("Erreur : il faut au moins 2 joueurs.\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 1; i < argc; i++) {
        for (int j = i + 1; j < argc; j++) {
            if (strcmp(argv[i], argv[j]) == 0) {
                printf("Erreur : les noms doivent être distincts.\n");
                exit(EXIT_FAILURE);
            }
        }
    }
}

Joueur* initialiserJoueurs(int argc, const char* argv[], int* nbJoueurs) {
    *nbJoueurs = argc - 1;
    Joueur* joueurs = malloc(*nbJoueurs * sizeof(Joueur));

    for (int i = 0; i < *nbJoueurs; i++) {
        strcpy(joueurs[i].nom, argv[i + 1]);
        joueurs[i].score = 0;
        joueurs[i].peutjouer = true;
    }

    return joueurs;
}

void ajouterPoint(Joueur* joueur) {
    joueur->score++;
}
