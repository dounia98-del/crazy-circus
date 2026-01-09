#define _CRT_SECURE_NO_WARNINGS
#include "jeu.h"
#include "partie.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// fonction qui fait que l'animal du bas (indice 0) passe tout en haut.
void rotationPodium(Podium *p) {
  if (p->nbAnimaux < 2)
    return; // Pas de rotation si 0 ou 1 animal

  // On garde l'animal du bas
  char *animalDuBas = p->animaux[0];

  // On décale tout le monde vers le bas
  for (int i = 0; i < p->nbAnimaux - 1; i++) {
    p->animaux[i] = p->animaux[i + 1];
  }

  // On remet l'animal du bas tout en haut
  p->animaux[p->nbAnimaux - 1] = animalDuBas;
}

void appliquerOrdre(Position *pos, char codeInterne) {
  switch (codeInterne) {

  case 'K': // KI : Bleu vers Rouge
    if (pos->bleu.nbAnimaux > 0 && pos->rouge.nbAnimaux < pos->rouge.capacite) {
      char *animal = pos->bleu.animaux[pos->bleu.nbAnimaux - 1];
      pos->bleu.nbAnimaux--;
      pos->rouge.animaux[pos->rouge.nbAnimaux++] = animal;
    }
    break;

  case 'L': // LO : Rouge vers Bleu
    if (pos->rouge.nbAnimaux > 0 && pos->bleu.nbAnimaux < pos->bleu.capacite) {
      char *animal = pos->rouge.animaux[pos->rouge.nbAnimaux - 1];
      pos->rouge.nbAnimaux--;
      pos->bleu.animaux[pos->bleu.nbAnimaux++] = animal;
    }
    break;

  case 'S': // SO : Echange sommets
    if (pos->bleu.nbAnimaux > 0 && pos->rouge.nbAnimaux > 0) {
      char *tmp = pos->bleu.animaux[pos->bleu.nbAnimaux - 1];
      pos->bleu.animaux[pos->bleu.nbAnimaux - 1] =
          pos->rouge.animaux[pos->rouge.nbAnimaux - 1];
      pos->rouge.animaux[pos->rouge.nbAnimaux - 1] = tmp;
    }
    break;

  case 'N': // NI : Rotation Bleu (Bas vers Haut)
    rotationPodium(&pos->bleu);
    break;

  case 'M': // MA : Rotation Rouge (Bas vers Haut)
    rotationPodium(&pos->rouge);
    break;

  default:

    break;
  }
}

void appliquerSequence(Position *pos, const char *sequence) {
  int i = 0;
  // On parcourt la chaine tant qu'il y a au moins 2 caractères
  while (sequence[i] != '\0' && sequence[i + 1] != '\0') {

    char c1 = sequence[i];
    char c2 = sequence[i + 1];

    // identification de l'ordre (2 lettres -> 1 code interne)
    char codeInterne = 0;

    if (c1 == 'K' && c2 == 'I')
      codeInterne = 'K';
    else if (c1 == 'L' && c2 == 'O')
      codeInterne = 'L';
    else if (c1 == 'S' && c2 == 'O')
      codeInterne = 'S';
    else if (c1 == 'N' && c2 == 'I')
      codeInterne = 'N';
    else if (c1 == 'M' && c2 == 'A')
      codeInterne = 'M';

    if (codeInterne != 0) {
      appliquerOrdre(pos, codeInterne);
    } else {
      printf("Ordre inconnu ignore : %c%c\n", c1, c2);
    }

    // On avance de 2 caractères
    i += 2;
  }
}

int positionsEgales(Position a, Position b) {
  if (a.bleu.nbAnimaux != b.bleu.nbAnimaux)
    return 0;
  if (a.rouge.nbAnimaux != b.rouge.nbAnimaux)
    return 0;

  for (int i = 0; i < a.bleu.nbAnimaux; i++)
    if (strcmp(a.bleu.animaux[i], b.bleu.animaux[i]) != 0)
      return 0;

  for (int i = 0; i < a.rouge.nbAnimaux; i++)
    if (strcmp(a.rouge.animaux[i], b.rouge.animaux[i]) != 0)
      return 0;

  return 1;
}

void lireOrdres(char *buffer, int taille) {
  if (scanf("%s", buffer) != 1) {
    buffer[0] = '\0';
  }
}

int testerCarte(const Carte *carte, const char *sequence) {
  Position pos;
  int n = carte->depart.bleu.nbAnimaux + carte->depart.rouge.nbAnimaux;

  // Allocation temporaire dynamique
  pos.bleu.animaux = malloc(n * sizeof(char *));
  pos.rouge.animaux = malloc(n * sizeof(char *));
  pos.bleu.capacite = n;
  pos.rouge.capacite = n;

  // Copie de l'état initial
  pos.bleu.nbAnimaux = carte->depart.bleu.nbAnimaux;
  pos.rouge.nbAnimaux = carte->depart.rouge.nbAnimaux;
  for (int i = 0; i < pos.bleu.nbAnimaux; i++)
    pos.bleu.animaux[i] = carte->depart.bleu.animaux[i];
  for (int i = 0; i < pos.rouge.nbAnimaux; i++)
    pos.rouge.animaux[i] = carte->depart.rouge.animaux[i];

  // Application
  appliquerSequence(&pos, sequence);

  // Vérification
  int res = positionsEgales(pos, carte->cible);

  // on nettoie
  free(pos.bleu.animaux);
  free(pos.rouge.animaux);

  return res;
}
