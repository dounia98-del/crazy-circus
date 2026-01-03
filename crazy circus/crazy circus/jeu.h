#pragma once
#include <stdio.h>

typedef struct {
    char** animaux;     // tableau de noms d'animaux
    int nbAnimaux;
    int capacite;
} Podium;

typedef struct {
    Podium bleu;
    Podium rouge;
} Position;

void setposition(Position* p);
void setpodium(Podium* podium, char** animaux, int nbAnimaux);
void getpodium(const Podium* podium);
void afficherposition(const Position* p);

void lireOrdres(char* buffer, int taille);
void appliquerOrdre(Position* pos, char ordre);
void appliquerSequence(Position* pos, const char* sequence);
int positionsEgales(Position a, Position b);
