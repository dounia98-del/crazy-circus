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
        exit(EXIT_FAILURE);
    }

    // Lire le nombre d'animaux
    fscanf(fichier, "%d\n", &config.nbAnimaux);
    config.animaux = malloc(config.nbAnimaux * sizeof(char*));
    for (int i = 0; i < config.nbAnimaux; i++) {
        char buffer[30];
        fscanf(fichier, "%s\n", buffer);
        config.animaux[i] = malloc(strlen(buffer) + 1);
        strcpy(config.animaux[i], buffer);

    }

    // Lire le nombre d'ordres
    fscanf(fichier, "%d\n", &config.nbOrdres);
    config.ordres = malloc(config.nbOrdres * sizeof(char*));
    for (int i = 0; i < config.nbOrdres; i++) {
        char buffer[5];
        fscanf(fichier, "%s\n", buffer);
        config.animaux[i] = malloc(strlen(buffer) + 1);
        strcpy(config.animaux[i], buffer);
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
    for (int i = 0; i < config->nbAnimaux; i++) {
        free(config->animaux[i]);
    }
    free(config->animaux);

    for (int i = 0; i < config->nbOrdres; i++) {
        free(config->ordres[i]);
    }
    free(config->ordres);
}
