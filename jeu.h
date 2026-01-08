#pragma once
#include <stdio.h>

typedef struct {
    char** animaux;  // tableau de noms d'animaux
    int nbAnimaux;  // Nombre actuel d'animaux sur le podium
    int capacite;  //  Capacité maximale
} Podium;

typedef struct {
    Podium bleu;
    Podium rouge;
} Position;


void lireOrdres(char* buffer, int taille);
//@brief Lit une séquence d'ordres saisie par l'utilisateur.
//@param buffer Chaîne où stocker la saisie.
//@param taille Taille maximale du buffer.
void appliquerOrdre(Position* pos, char ordre);
//@brief Applique un ordre unique(ex: 'K', 'N') sur une position.
//@param pos Pointeur vers la position actuelle.
//@param ordre Le caractère représentant l'ordre interne.

void appliquerSequence(Position* pos, const char* sequence);
//@brief Applique une suite d'ordres complète sur une position.
//@param pos Pointeur vers la position de départ(sera modifiée).
//@param sequence Chaîne de caractères contenant la suite d'ordres (ex: "KILOMA").

int positionsEgales(Position a, Position b);
//@brief Compare deux positions pour voir si elles sont identiques.
//@param a La première position.
//@param b La deuxième position(généralement la cible).
//@return 1 si les positions sont identiques, 0 sinon.

