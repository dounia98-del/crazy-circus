#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "config.h"
#include "joueur.h"
#include "partie.h"
#include "jeu.h"

// Fonction pour comparer deux joueurs 
int comparerJoueurs(const void* a, const void* b) {
    const Joueur* j1 = (const Joueur*)a;
    const Joueur* j2 = (const Joueur*)b;

    //On regarde d'abord le score (le plus grand d'abord)
    if (j2->score != j1->score) {
        return j2->score - j1->score;
    }
    //Si égalité, on trie par ordre alphabétique
    return strcmp(j1->nom, j2->nom);
}

// Fonction pour vérifier si un ordre existe dans la config
int estOrdreValide(const char* ordre, const Config* config) {
    for (int i = 0; i < config->nbOrdres; i++) {
        // strcmp renvoie 0 si les chaines sont identiques
        if (strcmp(ordre, config->ordres[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

// Fonction pour copier les animaux d'une position A vers une position B
void copierPosition(Position* destination, const Position* source) {
    destination->bleu.nbAnimaux = source->bleu.nbAnimaux;
    for (int i = 0; i < source->bleu.nbAnimaux; i++)
        destination->bleu.animaux[i] = source->bleu.animaux[i];

    destination->rouge.nbAnimaux = source->rouge.nbAnimaux;
    for (int i = 0; i < source->rouge.nbAnimaux; i++)
        destination->rouge.animaux[i] = source->rouge.animaux[i];
}

int main(int argc, const char* argv[]) {
    srand((unsigned int)time(NULL));

    // Vérif il faut au moins 2 joueurs
    if (argc < 3) {
        printf("Erreur : Il faut au moins 2 joueurs en arguments.\n");
        return 1;
    }

    // 1. Lecture de la configuration
    Config config = lireConfig("crazy.cfg");
    if (config.nbAnimaux < 2 || config.nbOrdres < 3) {
        printf("Erreur : Configuration invalide (min 2 animaux et 3 ordres).\n");
        return 1;
    }

    // 2. Initialisation des joueurs
    int nbJoueurs;
    Joueur* joueurs = initialiserJoueurs(argc, argv, &nbJoueurs);

    // 3. Initialisation de la partie
    Partie partie;
    partie.config = config;
    initPartie(&partie, joueurs, nbJoueurs);

    // 4. Affichage des règles
    printf("KI (B -> R) | LO (B <- R) | SO (B <-> R) | NI (B ^) | MA (R ^)\n\n");

    // On crée une variable pour stocker où sont les animaux en ce moment
    Position posCourante;
    posCourante.bleu.animaux = malloc(10 * sizeof(char*));
    posCourante.rouge.animaux = malloc(10 * sizeof(char*));
    posCourante.bleu.capacite = 10;
    posCourante.rouge.capacite = 10;

    // Au début, tous les animaux sont sur le bleu (comme dans le fichier config)
    posCourante.bleu.nbAnimaux = config.nbAnimaux;
    posCourante.rouge.nbAnimaux = 0;
    for (int i = 0; i < config.nbAnimaux; i++) {
        posCourante.bleu.animaux[i] = config.animaux[i];
    }

    // Variables pour le jeu
    int jeuTermine = 0;
    Carte* carteEnJeu = NULL; // Pointeur vers la carte qu'on essaie de résoudre
    char nomJoueur[50];       // Pour stocker le nom tapé 
    char sequence[100];       // Pour stocker les ordres tapés 

    while (!jeuTermine) {

        // Si on n'a pas de carte active (début de partie ou point gagné)
        if (carteEnJeu == NULL) {
            if (partie.nbCartes == 0) {
                // Plus de cartes dans le paquet donc fin du jeu
                jeuTermine = 1;
                continue;
            }

            // On tire une nouvelle carte (qui contient juste une CIBLE aléatoire)
            carteEnJeu = tirerCarteAleatoire(&partie);

            // Tout le monde a le droit de jouer pour ce nouveau tour
            for (int i = 0; i < nbJoueurs; i++) {
                partie.joueurs[i].peutjouer = 1;
            }

            // On met à jour le DEPART de la carte avec la vraie position du jeu
            copierPosition(&carteEnJeu->depart, &posCourante);

            // On affiche la carte
            getCarte(carteEnJeu);
        }

        // On force l'affichage pour être sûr que tout apparaisse
        fflush(stdout);

		// Tape le nom et la séquence
        if (scanf("%s %s", nomJoueur, sequence) != 2) {
            break; 
        }

        // On cherche quel joueur a tapé son nom
        int indexJoueur = -1;
        for (int i = 0; i < nbJoueurs; i++) {
            if (strcmp(partie.joueurs[i].nom, nomJoueur) == 0) {
                indexJoueur = i;
                break;
            }
        }

        // Si le nom n'existe pas
        if (indexJoueur == -1) {
            printf("Joueur inconnu : %s\n", nomJoueur);
            continue;
        }

        Joueur* joueurActuel = &partie.joueurs[indexJoueur];

        // Si le joueur est déjà éliminé pour ce tour
        if (joueurActuel->peutjouer == 0) {
            printf("%s ne peut pas jouer\n", joueurActuel->nom);
            continue;
        }

        // On vérifie si les ordres existent 
        int syntaxeCorrecte = 1;
        char ordreTemp[3] = { 0 }; // Pour stocker 2 lettres

        // On avance de 2 en 2 dans la chaine 
        for (int i = 0; sequence[i] != '\0'; i += 2) {
            // S'il reste une lettre toute seule à la fin -> Erreur
            if (sequence[i + 1] == '\0') {
                printf("Sequence invalide (nombre impair de caracteres)\n");
                syntaxeCorrecte = 0;
                break;
            }

            // On prend les 2 lettres
            ordreTemp[0] = sequence[i];
            ordreTemp[1] = sequence[i + 1];

            // On vérifie dans la config
            if (!estOrdreValide(ordreTemp, &config)) {
                printf("L'ordre %s n'existe pas\n", ordreTemp);
                syntaxeCorrecte = 0;
                break;
            }
        }

        if (syntaxeCorrecte == 0) {
            // Si l'ordre n'existe pas, on redemande
            continue;
        }

        // On teste si la séquence marche
        if (testerCarte(carteEnJeu, sequence)) {
            // si victoire
            printf("%s gagne un point\n\n", joueurActuel->nom);
            ajouterPoint(joueurActuel);

            // Les animaux bougent : la position courante devient la cible atteinte
            copierPosition(&posCourante, &carteEnJeu->cible);

            // On met carteEnJeu à NULL pour en tirer une nouvelle au prochain tour de boucle
            carteEnJeu = NULL;
        }
        else {
            // si perdu
            printf("La sequence ne conduit pas a la situation attendue -- %s ne peut plus jouer durant ce tour\n", joueurActuel->nom);
            joueurActuel->peutjouer = 0; // Le joueur est éliminé

            // On regarde s'il reste des survivants
            int joueursEnLice = 0;
            int indexSurvivant = -1;
            for (int i = 0; i < nbJoueurs; i++) {
                if (partie.joueurs[i].peutjouer == 1) {
                    joueursEnLice++;
                    indexSurvivant = i;
                }
            }

            // S'il ne reste qu'un seul joueur
            if (joueursEnLice == 1) {
                Joueur* survivant = &partie.joueurs[indexSurvivant];
                printf("%s gagne un point car lui seul peut encore jouer durant ce tour\n\n", survivant->nom);
                ajouterPoint(survivant);

                
                copierPosition(&posCourante, &carteEnJeu->cible);

                carteEnJeu = NULL; // On passera au tour suivant
            }
        }
    }

	// Jeu terminé, on affiche les scores finaux
    // qsort c une fonction standard de <stdlib.h> pour trier un tableau
    qsort(joueurs, nbJoueurs, sizeof(Joueur), comparerJoueurs);


    for (int i = 0; i < nbJoueurs; i++) {
        printf("%s %d\n", joueurs[i].nom, joueurs[i].score);
    }

    libererConfig(&config);
    return 0;
}