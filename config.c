#define _CRT_SECURE_NO_WARNINGS
#include "config.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Config lireConfig(const char* nomFichier) {
    Config config;
    FILE* fichier = fopen(nomFichier, "r");

    if (!fichier) {
        printf("Erreur : impossible d'ouvrir le fichier %s\n", nomFichier);
        // On met une pause pour voir l'erreur
        getchar();
        exit(EXIT_FAILURE);
    }

    char ligneLue[1024];   // Pour stocker la ligne lue dans le fichier
    char* mot;             // Pointeur vers le mot découpé
    char* stockageTemp[100]; // Tableau temporaire pour garder les mots
    int compteur;

    // --- 1. LECTURE DES ANIMAUX (Ligne 1) ---
    // On lit toute la première ligne du fichier
    if (fgets(ligneLue, 1024, fichier) != NULL) {
        compteur = 0;

        // On découpe la phrase à chaque espace " " ou saut de ligne "\n"
        mot = strtok(ligneLue, " \n\r");

        // Tant qu'on trouve des mots...
        while (mot != NULL) {
            // 1. On réserve la mémoire (taille du mot + 1 pour le caractère de fin)
            stockageTemp[compteur] = malloc(strlen(mot) + 1);

            // 2. On copie le mot trouvé dedans
            strcpy(stockageTemp[compteur], mot);

            compteur++;
            mot = strtok(NULL, " \n\r"); // On passe au mot suivant
        }

        // On enregistre le résultat final dans la structure
        config.nbAnimaux = compteur;
        config.animaux = malloc(compteur * sizeof(char*));
        for (int i = 0; i < compteur; i++) {
            config.animaux[i] = stockageTemp[i];
        }
    }

    // --- 2. LECTURE DES ORDRES (Ligne 2) ---
    // On lit toute la deuxième ligne
    if (fgets(ligneLue, 1024, fichier) != NULL) {
        compteur = 0;

        mot = strtok(ligneLue, " \n\r");

        while (mot != NULL) {
            // Mêmes étapes : allocation + copie
            stockageTemp[compteur] = malloc(strlen(mot) + 1);
            strcpy(stockageTemp[compteur], mot);

            compteur++;
            mot = strtok(NULL, " \n\r");
        }

        config.nbOrdres = compteur;
        // Attention à bien allouer 'ordres' et pas 'animaux'
        config.ordres = malloc(compteur * sizeof(char*));
        for (int i = 0; i < compteur; i++) {
            config.ordres[i] = stockageTemp[i];
        }
    }

    fclose(fichier);
    return config;
}

void afficherConfig(const Config* config) {
    printf("Animaux (%d) : ", config->nbAnimaux);
    for (int i = 0; i < config->nbAnimaux; i++) {
        printf("%s ", config->animaux[i]);
    }
    printf("\n");

    printf("Ordres (%d) : ", config->nbOrdres);
    for (int i = 0; i < config->nbOrdres; i++) {
        printf("%s ", config->ordres[i]);
    }
    printf("\n");
}

void libererConfig(Config* config) {
    // On libère la mémoire de chaque mot, puis du tableau
    for (int i = 0; i < config->nbAnimaux; i++) {
        free(config->animaux[i]);
    }
    free(config->animaux);

    for (int i = 0; i < config->nbOrdres; i++) {
        free(config->ordres[i]);
    }
    free(config->ordres);
}