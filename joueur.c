#define _CRT_SECURE_NO_WARNINGS
#include "joueur.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Joueur* initialiserJoueurs(int argc, const char* argv[], int* nbJoueurs) {
    // Le nombre de joueurs est le nombre d'arguments moins 1 (le nom du programme)
    *nbJoueurs = argc - 1;

    // Allocation dynamique du tableau de joueurs
    Joueur* joueurs = malloc(*nbJoueurs * sizeof(Joueur));
    if (joueurs == NULL) {
        printf("Erreur d'allocation memoire.\n");
        exit(1);
    }

    // Boucle pour créer chaque joueur
    for (int i = 0; i < *nbJoueurs; i++) {
        // argv[0] est le nom du programme, donc le joueur i est dans argv[i+1]
        const char* nomSaisi = argv[i + 1];

        //noms distincts
        for (int j = 0; j < i; j++) {
            if (strcmp(joueurs[j].nom, nomSaisi) == 0) {
                printf("Erreur : Les noms des joueurs doivent etre distincts (%s est en double).\n", nomSaisi);
                // on termine immédiatement
                free(joueurs);
                exit(1);
            }
        }


        // Copie du nom (avec sécurité pour ne pas dépasser 49 caractères)
        strncpy(joueurs[i].nom, nomSaisi, 49);
        joueurs[i].nom[49] = '\0'; // Assure la fin de chaîne

        joueurs[i].score = 0;
        joueurs[i].peutjouer = 1; // 1 = Vrai (peut jouer)
    }

    return joueurs;
}

void ajouterPoint(Joueur* joueur) {
    if (joueur != NULL) {
        joueur->score++;
    }
}