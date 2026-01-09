#include "tests.h"
#include "../config.h"
#include "../jeu.h"
#include "../joueur.h"
#include "../partie.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#include <windows.h>
#endif

// Macro utilitaire pour les tests
#define ASSERT_TEST(cond, msg)                                                 \
  nbTests++;                                                                   \
  if (cond) {                                                                  \
    nbReussites++;                                                             \
  } else {                                                                     \
    printf("  [ECHEC] %s\n", msg);                                             \
  }

void lancerTestsUnitaires() {
  printf("\n=== DEMARRAGE DES TESTS UNITAIRES ===\n");
  int nbReussites = 0;
  int nbTests = 0;

  // --- TEST 1 : Config & Ordres ---
  printf("[TEST 1] Configuration et Validation d'ordres... ");
  Config config;
  config.nbOrdres = 2;
  config.ordres = malloc(2 * sizeof(char *));
  config.ordres[0] = "KI";
  config.ordres[1] = "LO";

  ASSERT_TEST(estOrdreValide("KI", &config) == 1, "KI devrait être valide");
  ASSERT_TEST(estOrdreValide("MA", &config) == 0, "MA devrait être invalide");

  free(config.ordres);
  printf("%d/%d réussis\n", nbReussites, nbTests);

  // --- TEST 2 : Logique Joueurs ---
  printf("[TEST 2] Initialisation des Joueurs... ");
  int startReussites = nbReussites;
  int nbJoueurs = 0;
  const char *args[] = {"program", "Alice", "Bob"};
  Joueur *joueurs = initialiserJoueurs(3, args, &nbJoueurs);

  ASSERT_TEST(nbJoueurs == 2, "Devrait y avoir 2 joueurs");
  ASSERT_TEST(strcmp(joueurs[0].nom, "Alice") == 0,
              "Le premier joueur devrait être Alice");
  ASSERT_TEST(joueurs[1].score == 0, "Le score initial doit être 0");

  ajouterPoint(&joueurs[0]);
  ASSERT_TEST(joueurs[0].score == 1, "Le point n'a pas été ajouté");

  free(joueurs);
  printf("%d/%d réussis\n", nbReussites - startReussites,
         nbTests -
             (nbTests - (nbReussites - startReussites)));

  // --- TEST 3 : Moteur de Jeu (Position & Deplacement) ---
  printf("[TEST 3] Moteur de Jeu (Deplacement & Egalité)... ");
  startReussites = nbReussites;

  Position p1, p2;
  p1.bleu.animaux = malloc(2 * sizeof(char *));
  p1.rouge.animaux = malloc(2 * sizeof(char *));
  p1.bleu.nbAnimaux = 2;
  p1.bleu.animaux[0] = "LION";
  p1.bleu.animaux[1] = "OURS";
  p1.rouge.nbAnimaux = 0;
  p1.bleu.capacite = 5;
  p1.rouge.capacite = 5;

  p2.bleu.animaux = malloc(2 * sizeof(char *));
  p2.rouge.animaux = malloc(2 * sizeof(char *));
  p2.bleu.capacite = 5;
  p2.rouge.capacite = 5;
  copierPosition(&p2, &p1);

  ASSERT_TEST(positionsEgales(p1, p2) == 1,
              "Les positions devraient être égales");

  appliquerOrdre(&p2, 'K'); // KI: Bleu -> Rouge (OURS bouge)
  ASSERT_TEST(p2.bleu.nbAnimaux == 1, "Le podium bleu devrait avoir 1 animal");
  ASSERT_TEST(p2.rouge.nbAnimaux == 1,
              "Le podium rouge devrait avoir 1 animal");
  ASSERT_TEST(strcmp(p2.rouge.animaux[0], "OURS") == 0,
              "L'ours devrait être sur le podium rouge");
  ASSERT_TEST(positionsEgales(p1, p2) == 0,
              "Les positions ne devraient plus être égales");

  free(p1.bleu.animaux);
  free(p1.rouge.animaux);
  free(p2.bleu.animaux);
  free(p2.rouge.animaux);
  printf("OK\n");

  // --- TEST 4 : Moteur de Cartes (Permutations) ---
  printf("[TEST 4] Génération de Permutations... ");
  startReussites = nbReussites;
  char *animaux[] = {"LION", "OURS", "ELEPHANT"};
  int nbPerm;
  char ***perms = genererToutesPermutations(animaux, 3, &nbPerm);

  ASSERT_TEST(nbPerm == 6, "3 animaux devraient donner 6 permutations (3!)");

  for (int i = 0; i < nbPerm; i++)
    free(perms[i]);
  free(perms);
  printf("OK\n");

  printf("\n=====================================\n");
  printf("RESULTAT FINAL : %d / %d tests réussis.\n", nbReussites, nbTests);
  printf("=====================================\n\n");
}

int main() {
  #ifdef _WIN32
  SetConsoleOutputCP(65001);
  #endif
  lancerTestsUnitaires();

  return 0;
}
