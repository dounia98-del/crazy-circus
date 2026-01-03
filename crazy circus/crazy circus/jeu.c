#define _CRT_SECURE_NO_WARNINGS
#include "jeu.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "partie.h"

void appliquerOrdre(Position* pos, char ordre) {
    switch (ordre) {

    case 'B':   // Déplacer le haut du podium bleu vers rouge
        if (pos->bleu.nbAnimaux > 0) {
            char* animal = pos->bleu.animaux[pos->bleu.nbAnimaux - 1];
            pos->bleu.nbAnimaux--;
            pos->rouge.animaux[pos->rouge.nbAnimaux++] = animal;
        }
        break;

    case 'R':   // Déplacer le haut du podium rouge vers bleu
        if (pos->rouge.nbAnimaux > 0) {
            char* animal = pos->rouge.animaux[pos->rouge.nbAnimaux - 1];
            pos->rouge.nbAnimaux--;
            pos->bleu.animaux[pos->bleu.nbAnimaux++] = animal;
        }
        break;

    case 'S':   // Échanger les deux animaux du haut
        if (pos->bleu.nbAnimaux > 0 && pos->rouge.nbAnimaux > 0) {
            char* tmp = pos->bleu.animaux[pos->bleu.nbAnimaux - 1];
            pos->bleu.animaux[pos->bleu.nbAnimaux - 1] = pos->rouge.animaux[pos->rouge.nbAnimaux - 1];
            pos->rouge.animaux[pos->rouge.nbAnimaux - 1] = tmp;
        }
        break;

    default:
        printf("Ordre inconnu : %c\n", ordre);
        break;
    }
}

void appliquerSequence(Position* pos, const char* sequence) {
    for (int i = 0; sequence[i] != '\0'; i++) {
        appliquerOrdre(pos, sequence[i]);
    }
}

int positionsEgales(Position a, Position b) {
    if (a.bleu.nbAnimaux != b.bleu.nbAnimaux) return 0;
    if (a.rouge.nbAnimaux != b.rouge.nbAnimaux) return 0;

    for (int i = 0; i < a.bleu.nbAnimaux; i++)
        if (a.bleu.animaux[i] != b.bleu.animaux[i]) return 0;

    for (int i = 0; i < a.rouge.nbAnimaux; i++)
        if (a.rouge.animaux[i] != b.rouge.animaux[i]) return 0;

    return 1;
}

void lireOrdres(char* buffer, int taille) {
    printf("Entrez une séquence d'ordres : ");
    scanf("%s", buffer);
}

int testerCarte(const Carte* carte, const char* sequence) {
    Position pos = carte->depart;
    appliquerSequence(&pos, sequence);
    return positionsEgales(pos, carte->cible);
}


void setposition(Position* p) {
    p->bleu.nbAnimaux = 0;
    p->rouge.nbAnimaux = 0;
    p->bleu.capacite = 10;
    p->rouge.capacite = 10;
    p->bleu.animaux = malloc(10 * sizeof(char*));
    p->rouge.animaux = malloc(10 * sizeof(char*));
}

void setpodium(Podium* podium, char** animaux, int nbAnimaux) {
    podium->nbAnimaux = nbAnimaux;
    podium->capacite = nbAnimaux;
    podium->animaux = malloc(nbAnimaux * sizeof(char*));
    for (int i = 0; i < nbAnimaux; i++) {
        podium->animaux[i] = animaux[i];
    }
}

void getpodium(const Podium* podium) {
    for (int i = 0; i < podium->nbAnimaux; i++) {
        printf("%s ", podium->animaux[i]);
    }
    printf("\n");
}

void afficherposition(const Position* p) {
    printf("Bleu : ");
    getpodium(&p->bleu);
    printf("Rouge : ");
    getpodium(&p->rouge);
}


