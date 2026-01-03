#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "config.h"
#include "joueur.h"
#include "partie.h"
#include "jeu.h"



int main(int argc, const char* argv[]) {
    printf("=== Crazy Circus ===\n");
    printf("Joueurs : ");
    for (int i = 1; i < argc; i++) {
        printf("%s ", argv[i]);
    }
    printf("\n");

    srand((unsigned int)time(NULL));

 
    
    // Lire la config
    Config config = lireConfig("crazy.cfg");
    afficherConfig(&config);

    // Initialiser les joueurs
    int nbJoueurs;
    Joueur* joueurs = initialiserJoueurs(argc, argv, &nbJoueurs);

    Partie partie;
    partie.config = config;

    initPartie(&partie, joueurs, nbJoueurs);

    char ordres[100];

    while (1) {
        printf("\n===== Tour du joueur %s =====\n", partie.joueurs[partie.touractuel].nom);

        Carte* carte = tirerCarteAleatoire(&partie);
        getCarte(carte);

        lireOrdres(ordres, 100);

        if (testerCarte(carte, ordres)) {
            printf("Bravo ! Cible atteinte.\n");
            ajouterPoint(&partie.joueurs[partie.touractuel]);
        }
        else {
            printf("Raté !\n");
        }

        joueurSuivant(&partie);

        // Fin de partie simple
        for (int i = 0; i < partie.nbJoueurs; i++) {
            if (partie.joueurs[i].score >= 3) {
                printf("Le joueur %s a gagné !\n", partie.joueurs[i].nom);
                return 0;
            }
        }
    }

    libererConfig(&config);


    return 0;
}

