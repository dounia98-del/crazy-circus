#define _CRT_SECURE_NO_WARNINGS
#include "jeu.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "partie.h"

// Fonction interne pour gérer la rotation (NI et MA)
// L'animal du BAS (indice 0) passe tout en HAUT.
void rotationPodium(Podium* p) {
    if (p->nbAnimaux < 2) return; // Pas de rotation si 0 ou 1 animal

    // 1. On garde l'animal du bas
    char* animalDuBas = p->animaux[0];

    // 2. On décale tout le monde vers le bas
    for (int i = 0; i < p->nbAnimaux - 1; i++) {
        p->animaux[i] = p->animaux[i + 1];
    }

    // 3. On remet l'animal du bas tout en haut
    p->animaux[p->nbAnimaux - 1] = animalDuBas;
}

void appliquerOrdre(Position* pos, char codeInterne) {
    switch (codeInterne) {

    case 'K':   // KI : Bleu vers Rouge
        if (pos->bleu.nbAnimaux > 0) {
            char* animal = pos->bleu.animaux[pos->bleu.nbAnimaux - 1];
            pos->bleu.nbAnimaux--;
            pos->rouge.animaux[pos->rouge.nbAnimaux++] = animal;
        }
        break;

    case 'L':   // LO : Rouge vers Bleu
        if (pos->rouge.nbAnimaux > 0) {
            char* animal = pos->rouge.animaux[pos->rouge.nbAnimaux - 1];
            pos->rouge.nbAnimaux--;
            pos->bleu.animaux[pos->bleu.nbAnimaux++] = animal;
        }
        break;

    case 'S':   // SO : Echange sommets
        if (pos->bleu.nbAnimaux > 0 && pos->rouge.nbAnimaux > 0) {
            char* tmp = pos->bleu.animaux[pos->bleu.nbAnimaux - 1];
            pos->bleu.animaux[pos->bleu.nbAnimaux - 1] = pos->rouge.animaux[pos->rouge.nbAnimaux - 1];
            pos->rouge.animaux[pos->rouge.nbAnimaux - 1] = tmp;
        }
        break;

    case 'N':   // NI : Rotation Bleu (Bas vers Haut)
        rotationPodium(&pos->bleu);
        break;

    case 'M':   // MA : Rotation Rouge (Bas vers Haut)
        rotationPodium(&pos->rouge);
        break;

    default:
        // On ignore les erreurs silencieusement ou on affiche pour debug
        break;
    }
}

void appliquerSequence(Position* pos, const char* sequence) {
    int i = 0;
    // On parcourt la chaine tant qu'il y a au moins 2 caractères (ex: "KI")
    while (sequence[i] != '\0' && sequence[i + 1] != '\0') {

        char c1 = sequence[i];
        char c2 = sequence[i + 1];

        // Identification de l'ordre (2 lettres -> 1 code interne)
        char codeInterne = 0;

        if (c1 == 'K' && c2 == 'I') codeInterne = 'K';
        else if (c1 == 'L' && c2 == 'O') codeInterne = 'L';
        else if (c1 == 'S' && c2 == 'O') codeInterne = 'S';
        else if (c1 == 'N' && c2 == 'I') codeInterne = 'N';
        else if (c1 == 'M' && c2 == 'A') codeInterne = 'M';

        if (codeInterne != 0) {
            appliquerOrdre(pos, codeInterne);
        }
        else {
            printf("Ordre inconnu ignore : %c%c\n", c1, c2);
        }

        // On avance de 2 caractères
        i += 2;
    }
}

int positionsEgales(Position a, Position b) {
    if (a.bleu.nbAnimaux != b.bleu.nbAnimaux) return 0;
    if (a.rouge.nbAnimaux != b.rouge.nbAnimaux) return 0;

    for (int i = 0; i < a.bleu.nbAnimaux; i++)
        if (strcmp(a.bleu.animaux[i], b.bleu.animaux[i]) != 0) return 0; // strcmp pour comparer les mots

    for (int i = 0; i < a.rouge.nbAnimaux; i++)
        if (strcmp(a.rouge.animaux[i], b.rouge.animaux[i]) != 0) return 0;

    return 1;
}

void lireOrdres(char* buffer, int taille) {
    // printf("Votre coup > "); // Optionnel selon le sujet
    // scanf s'arrête au premier espace, c'est ce qu'on veut ici.
    scanf("%s", buffer);
}

int testerCarte(const Carte* carte, const char* sequence) {
    Position pos;
    // Allocation temporaire
    pos.bleu.animaux = malloc(10 * sizeof(char*));
    pos.rouge.animaux = malloc(10 * sizeof(char*));
    pos.bleu.capacite = 10; pos.rouge.capacite = 10;

    // Copie de l'état initial
    pos.bleu.nbAnimaux = carte->depart.bleu.nbAnimaux;
    pos.rouge.nbAnimaux = carte->depart.rouge.nbAnimaux;
    for (int i = 0; i < pos.bleu.nbAnimaux; i++) pos.bleu.animaux[i] = carte->depart.bleu.animaux[i];
    for (int i = 0; i < pos.rouge.nbAnimaux; i++) pos.rouge.animaux[i] = carte->depart.rouge.animaux[i];

    // Application
    appliquerSequence(&pos, sequence);

    // Vérification
    int res = positionsEgales(pos, carte->cible);

    // Nettoyage
    free(pos.bleu.animaux);
    free(pos.rouge.animaux);

    return res;
}

// Les fonctions setposition, setpodium... (pas utilisées dans le main actuel mais gardées pour compiler)
void setposition(Position* p) {}
void setpodium(Podium* podium, char** animaux, int nbAnimaux) {}
void getpodium(const Podium* podium) {}
void afficherposition(const Position* p) {}