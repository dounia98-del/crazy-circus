#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#ifdef _WIN32
#include <windows.h>
#endif

#include "config.h"
#include "jeu.h"
#include "joueur.h"
#include "partie.h"

int comparerJoueurs(const void *a, const void *b) {
  const Joueur *j1 = (const Joueur *)a;
  const Joueur *j2 = (const Joueur *)b;
  if (j2->score != j1->score)
    return j2->score - j1->score;
  return strcmp(j1->nom, j2->nom);
}

int main(int argc, const char *argv[]) {
#ifdef _WIN32
  SetConsoleOutputCP(65001);
#endif
  srand((unsigned int)time(NULL));

  if (argc < 3) {
    printf("Erreur : Il faut au moins 2 joueurs en arguments.\n");
    return 1;
  }

  Config config = lireConfig("crazy.cfg");
  if (config.nbAnimaux < 2 || config.nbOrdres < 3) {
    printf("Erreur : Configuration invalide.\n");
    libererConfig(&config);
    return 1;
  }

  int nbJoueurs;
  Joueur *joueurs = initialiserJoueurs(argc, argv, &nbJoueurs);

  Partie partie;
  partie.config = config;
  initPartie(&partie, joueurs, nbJoueurs);

  // Affichage des règles
  for (int i = 0; i < config.nbOrdres; i++) {
    char *o = config.ordres[i];
    if (strcmp(o, "KI") == 0)
      printf("KI (B -> R)");
    else if (strcmp(o, "LO") == 0)
      printf("LO (B <- R)");
    else if (strcmp(o, "SO") == 0)
      printf("SO (B <-> R)");
    else if (strcmp(o, "NI") == 0)
      printf("NI (B ^)");
    else if (strcmp(o, "MA") == 0)
      printf("MA (R ^)");

    if (i < config.nbOrdres - 1)
      printf(" | ");
  }
  printf("\n\n");

  // Init Position Courante
  Position posCourante;
  posCourante.bleu.animaux = malloc(config.nbAnimaux * sizeof(char *));
  posCourante.rouge.animaux = malloc(config.nbAnimaux * sizeof(char *));
  posCourante.bleu.capacite = config.nbAnimaux;
  posCourante.rouge.capacite = config.nbAnimaux;

  posCourante.bleu.nbAnimaux = config.nbAnimaux;
  posCourante.rouge.nbAnimaux = 0;
  for (int i = 0; i < config.nbAnimaux; i++)
    posCourante.bleu.animaux[i] = config.animaux[i];

  int jeuTermine = 0;
  Carte *carteEnJeu = NULL;
  char nomJoueur[31];
  char sequence[101];

  while (!jeuTermine) {
    if (carteEnJeu == NULL) {
      if (partie.nbCartes == 0) {
        jeuTermine = 1;
        continue;
      }
      carteEnJeu = tirerCarteAleatoire(&partie);
      for (int i = 0; i < nbJoueurs; i++)
        partie.joueurs[i].peutjouer = 1;
      copierPosition(&carteEnJeu->depart, &posCourante);
      getCarte(carteEnJeu);
    }

    fflush(stdout);

    if (scanf("%30s %100s", nomJoueur, sequence) != 2) {
      break;
    }

    int indexJoueur = -1;
    for (int i = 0; i < nbJoueurs; i++) {
      if (strcmp(partie.joueurs[i].nom, nomJoueur) == 0) {
        indexJoueur = i;
        break;
      }
    }

    if (indexJoueur == -1) {
      printf("Joueur inconnu : %s\n", nomJoueur);
      continue;
    }

    Joueur *joueurActuel = &partie.joueurs[indexJoueur];

    if (joueurActuel->peutjouer == 0) {
      printf("%s ne peut pas jouer\n", joueurActuel->nom);
      continue;
    }

    int syntaxeCorrecte = 1;
    char ordreTemp[3] = {0};

    for (int i = 0; sequence[i] != '\0'; i += 2) {
      if (sequence[i + 1] == '\0') {
        printf("Sequence invalide\n");
        syntaxeCorrecte = 0;
        break;
      }
      ordreTemp[0] = sequence[i];
      ordreTemp[1] = sequence[i + 1];

      if (!estOrdreValide(ordreTemp, &config)) {
        printf("L'ordre %s n'existe pas\n", ordreTemp);
        syntaxeCorrecte = 0;
        break;
      }
    }

    if (syntaxeCorrecte == 0)
      continue;

    if (testerCarte(carteEnJeu, sequence)) {
      printf("%s gagne un point\n\n", joueurActuel->nom);
      ajouterPoint(joueurActuel);
      copierPosition(&posCourante, &carteEnJeu->cible);
      carteEnJeu = NULL;
    } else {
      printf("La sequence ne conduit pas a la situation attendue -- %s ne peut "
             "plus jouer durant ce tour\n",
             joueurActuel->nom);
      joueurActuel->peutjouer = 0;

      int joueursEnLice = 0;
      int indexSurvivant = -1;
      for (int i = 0; i < nbJoueurs; i++) {
        if (partie.joueurs[i].peutjouer == 1) {
          joueursEnLice++;
          indexSurvivant = i;
        }
      }

      if (joueursEnLice == 1) {
        Joueur *survivant = &partie.joueurs[indexSurvivant];
        printf("%s gagne un point car lui/elle seul(e) peut encore jouer "
               "durant ce tour\n\n",
               survivant->nom);
        ajouterPoint(survivant);
        copierPosition(&posCourante, &carteEnJeu->cible);
        carteEnJeu = NULL;
      }
    }
  }

  qsort(joueurs, nbJoueurs, sizeof(Joueur), comparerJoueurs);

  for (int i = 0; i < nbJoueurs; i++) {
    printf("%s %d\n", joueurs[i].nom, joueurs[i].score);
  }

  libererConfig(&config);
  libererPartie(&partie);
  free(posCourante.bleu.animaux);
  free(posCourante.rouge.animaux);
  free(joueurs);

  return 0;
}
