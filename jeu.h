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
//@brief Lit une séquence d'ordres saisie par l'utilisateur via l'entrée standard.
//@param[out] buffer Chaîne où stocker la saisie utilisateur.
//@param[in]  taille Taille maximale du buffer(pour éviter les débordements).
//@pre buffer != NULL && taille > 0
void appliquerOrdre(Position* pos, char ordre);
//@brief Applique un ordre unique(ex: 'K', 'N') sur une position donnée.
//@param[in, out] pos   Pointeur vers la position actuelle(sera modifiée).
//@param[in]     ordre Le caractère représentant l'ordre interne.
//@pre pos != NULL

void appliquerSequence(Position* pos, const char* sequence);
//@brief Applique une suite d'ordres complète sur une position.
//@param[in, out] pos      Pointeur vers la position de départ(sera modifiée).
//@param[in]     sequence Chaîne de caractères contenant la suite d'ordres (ex: "KILOMA").
//@pre pos != NULL && sequence != NULL

int positionsEgales(Position a, Position b);
//@brief Compare deux positions pour voir si elles sont strictement identiques.
//@param[in] a La première position.
//@param[in] b La deuxième position(généralement la cible).
//@return 1 si les positions sont identiques(mêmes animaux, même ordre), 0 sinon.

