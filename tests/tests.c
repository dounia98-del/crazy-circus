#include "tests.h"
#include "../config.h"
#include "../jeu.h"
#include "../partie.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ======================================================================
// MODULE DE TESTS UNITAIRES
// Fonction isolée pour valider la logique du jeu sans lancer une partie.
// ======================================================================
void lancerTestsUnitaires() {
  printf("\n=== DEMARRAGE DES TESTS UNITAIRES ===\n");
  int nbReussites = 0;
  int nbTests = 0;

  // --- TEST 1 : Validation des ordres (Config) ---
  printf("[TEST 1] Verification de estOrdreValide()... ");
  Config confTest;
  confTest.nbOrdres = 2;
  confTest.ordres = malloc(2 * sizeof(char *));
  confTest.ordres[0] = "KI";
  confTest.ordres[1] = "LO";

  int t1_ok = estOrdreValide("KI", &confTest); // Doit être 1 (Vrai)
  int t1_ko = estOrdreValide("MA", &confTest); // Doit être 0 (Faux)

  if (t1_ok == 1 && t1_ko == 0) {
    printf("SUCCES\n");
    nbReussites++;
  } else {
    printf("ECHEC\n");
  }
  nbTests++;
  free(confTest.ordres); // Petit nettoyage local

  // --- TEST 2 : Moteur de jeu (testerCarte) ---
  printf("[TEST 2] Verification de testerCarte() (Deplacement KI)... ");

  // On fabrique une carte manuellement : LION sur BLEU -> LION sur ROUGE
  Carte c;
  c.depart.bleu.animaux = malloc(5 * sizeof(char *));
  c.depart.rouge.animaux = malloc(5 * sizeof(char *));
  c.cible.bleu.animaux = malloc(5 * sizeof(char *));
  c.cible.rouge.animaux = malloc(5 * sizeof(char *));

  c.depart.bleu.capacite = 5;
  c.depart.rouge.capacite = 5;
  c.cible.bleu.capacite = 5;
  c.cible.rouge.capacite = 5;

  // Etat DEPART : LION sur BLEU
  c.depart.bleu.nbAnimaux = 1;
  c.depart.bleu.animaux[0] = "LION";
  c.depart.rouge.nbAnimaux = 0;

  // Etat CIBLE : LION sur ROUGE
  c.cible.bleu.nbAnimaux = 0;
  c.cible.rouge.nbAnimaux = 1;
  c.cible.rouge.animaux[0] = "LION";

  // Test : L'ordre "KI" (Bleu vers Rouge) doit réussir
  int res_juste = testerCarte(&c, "KI");
  // Test : L'ordre "LO" (Rouge vers Bleu) doit échouer
  int res_faux = testerCarte(&c, "LO");

  if (res_juste == 1 && res_faux == 0) {
    printf("SUCCES\n");
    nbReussites++;
  } else {
    printf("ECHEC (Attendu 1/0, obtenu %d/%d)\n", res_juste, res_faux);
  }
  nbTests++;

  // Nettoyage mémoire du test
  free(c.depart.bleu.animaux);
  free(c.depart.rouge.animaux);
  free(c.cible.bleu.animaux);
  free(c.cible.rouge.animaux);

  printf("=====================================\n\n");
}

int main() {
  lancerTestsUnitaires();
  return 0;
}
