#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "config.h"
#include "joueur.h"
#include "partie.h"
#include "jeu.h"

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
    confTest.ordres = malloc(2 * sizeof(char*));
    confTest.ordres[0] = "KI";
    confTest.ordres[1] = "LO";

    int t1_ok = estOrdreValide("KI", &confTest);  // Doit être 1 (Vrai)
    int t1_ko = estOrdreValide("MA", &confTest);  // Doit être 0 (Faux)

    if (t1_ok == 1 && t1_ko == 0) {
        printf("SUCCES\n");
        nbReussites++;
    }
    else {
        printf("ECHEC\n");
    }
    nbTests++;
    free(confTest.ordres); // Petit nettoyage local

    // --- TEST 2 : Moteur de jeu (testerCarte) ---
    printf("[TEST 2] Verification de testerCarte() (Deplacement KI)... ");

    // On fabrique une carte manuellement : LION sur BLEU -> LION sur ROUGE
    Carte c;
    c.depart.bleu.animaux = malloc(5 * sizeof(char*));
    c.depart.rouge.animaux = malloc(5 * sizeof(char*));
    c.cible.bleu.animaux = malloc(5 * sizeof(char*));
    c.cible.rouge.animaux = malloc(5 * sizeof(char*));

    c.depart.bleu.capacite = 5; c.depart.rouge.capacite = 5;
    c.cible.bleu.capacite = 5; c.cible.rouge.capacite = 5;

    // Etat DEPART : LION sur BLEU
    c.depart.bleu.nbAnimaux = 1; c.depart.bleu.animaux[0] = "LION";
    c.depart.rouge.nbAnimaux = 0;

    // Etat CIBLE : LION sur ROUGE
    c.cible.bleu.nbAnimaux = 0;
    c.cible.rouge.nbAnimaux = 1; c.cible.rouge.animaux[0] = "LION";

    // Test : L'ordre "KI" (Bleu vers Rouge) doit réussir
    int res_juste = testerCarte(&c, "KI");
    // Test : L'ordre "LO" (Rouge vers Bleu) doit échouer
    int res_faux = testerCarte(&c, "LO");

    if (res_juste == 1 && res_faux == 0) {
        printf("SUCCES\n");
        nbReussites++;
    }
    else {
        printf("ECHEC (Attendu 1/0, obtenu %d/%d)\n", res_juste, res_faux);
    }
    nbTests++;

    // Nettoyage mémoire du test
    free(c.depart.bleu.animaux); free(c.depart.rouge.animaux);
    free(c.cible.bleu.animaux); free(c.cible.rouge.animaux);

    printf("=====================================\n");
    printf("RESULTAT FINAL : %d / %d tests valides.\n", nbReussites, nbTests);
    printf("=====================================\n\n");
}
    

int comparerJoueurs(const void* a, const void* b) {
    const Joueur* j1 = (const Joueur*)a;
    const Joueur* j2 = (const Joueur*)b;
    if (j2->score != j1->score) return j2->score - j1->score;
    return strcmp(j1->nom, j2->nom);
}

int estOrdreValide(const char* ordre, const Config* config) {
    for (int i = 0; i < config->nbOrdres; i++) {
        if (strcmp(ordre, config->ordres[i]) == 0) return 1;
    }
    return 0;
}

void copierPosition(Position* destination, const Position* source) {
    destination->bleu.nbAnimaux = source->bleu.nbAnimaux;
    for (int i = 0; i < source->bleu.nbAnimaux; i++)
        destination->bleu.animaux[i] = source->bleu.animaux[i];

    destination->rouge.nbAnimaux = source->rouge.nbAnimaux;
    for (int i = 0; i < source->rouge.nbAnimaux; i++)
        destination->rouge.animaux[i] = source->rouge.animaux[i];
}

int main(int argc, const char* argv[]) {
    lancerTestsUnitaires();
    srand((unsigned int)time(NULL));

    if (argc < 3) {
        printf("Erreur : Il faut au moins 2 joueurs en arguments.\n");
        return 1;
    }

    Config config = lireConfig("crazy.cfg");
    if (config.nbAnimaux < 2 || config.nbOrdres < 3) {
        printf("Erreur : Configuration invalide.\n");
        libererConfig(&config); // On nettoie 
        return 1;
    }

    int nbJoueurs;
    Joueur* joueurs = initialiserJoueurs(argc, argv, &nbJoueurs);

    Partie partie;
    partie.config = config;
    initPartie(&partie, joueurs, nbJoueurs);

    // 4. Affichage des règles (DYNAMIQUE selon la config)
    for (int i = 0; i < config.nbOrdres; i++) {
        char* o = config.ordres[i];
        if (strcmp(o, "KI") == 0) printf("KI (B -> R)");
        else if (strcmp(o, "LO") == 0) printf("LO (B <- R)");
        else if (strcmp(o, "SO") == 0) printf("SO (B <-> R)");
        else if (strcmp(o, "NI") == 0) printf("NI (B ^)");
        else if (strcmp(o, "MA") == 0) printf("MA (R ^)");

        // Séparateur " | " sauf pour le dernier
        if (i < config.nbOrdres - 1) printf(" | ");
    }
    printf("\n\n");

    // Init Position Courante
    Position posCourante;
    posCourante.bleu.animaux = malloc(10 * sizeof(char*));
    posCourante.rouge.animaux = malloc(10 * sizeof(char*));
    posCourante.bleu.capacite = 10; posCourante.rouge.capacite = 10;

    posCourante.bleu.nbAnimaux = config.nbAnimaux;
    posCourante.rouge.nbAnimaux = 0;
    for (int i = 0; i < config.nbAnimaux; i++) posCourante.bleu.animaux[i] = config.animaux[i];

    int jeuTermine = 0;
    Carte* carteEnJeu = NULL;

    char nomJoueur[51]; // 50 char + \0
    char sequence[101]; // 100 char + \0

    while (!jeuTermine) {
        if (carteEnJeu == NULL) {
            if (partie.nbCartes == 0) {
                jeuTermine = 1;
                continue;
            }
            carteEnJeu = tirerCarteAleatoire(&partie);
            for (int i = 0; i < nbJoueurs; i++) partie.joueurs[i].peutjouer = 1;
            copierPosition(&carteEnJeu->depart, &posCourante);
            getCarte(carteEnJeu);
        }

        fflush(stdout);

        if (scanf("%50s %100s", nomJoueur, sequence) != 2) {
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

        Joueur* joueurActuel = &partie.joueurs[indexJoueur];

        if (joueurActuel->peutjouer == 0) {
            printf("%s ne peut pas jouer\n", joueurActuel->nom);
            continue;
        }

        int syntaxeCorrecte = 1;
        char ordreTemp[3] = { 0 };

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

        if (syntaxeCorrecte == 0) continue;

        if (testerCarte(carteEnJeu, sequence)) {
            printf("%s gagne un point\n\n", joueurActuel->nom);
            ajouterPoint(joueurActuel);
            copierPosition(&posCourante, &carteEnJeu->cible);
            carteEnJeu = NULL;
        }
        else {
            printf("La sequence ne conduit pas a la situation attendue -- %s ne peut plus jouer durant ce tour\n", joueurActuel->nom);
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
                Joueur* survivant = &partie.joueurs[indexSurvivant];
                printf("%s gagne un point car lui/elle seul(e) peut encore jouer durant ce tour\n\n", survivant->nom);
                ajouterPoint(survivant);
                copierPosition(&posCourante, &carteEnJeu->cible);
                carteEnJeu = NULL;
            }
        }
    }

    qsort(joueurs, nbJoueurs, sizeof(Joueur), comparerJoueurs);

    printf("\n=== CLASSEMENT ===\n");
    for (int i = 0; i < nbJoueurs; i++) {
        printf("%s %d\n", joueurs[i].nom, joueurs[i].score);
    }

    // --- NETTOYAGE FINAL ---

    libererConfig(&config);
    libererPartie(&partie);

    free(posCourante.bleu.animaux);
    free(posCourante.rouge.animaux);


    free(joueurs);

    return 0;
}

