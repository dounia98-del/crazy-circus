# Checklist de Conformité Détaillée - Crazy Circus

Ce document confirme la vérification de chaque règle spécifique du sujet.

---

## 1. Initialisation et Jeu
- [x] **Au moins 2 joueurs** : Vérifié dans `main.c` (`argc < 3`).
- [x] **Noms distincts** : Vérifié dans `joueur.c` avec un `strcmp` sur les noms déjà saisis.
- [x] **Configuration (2 animaux / 3 ordres min)** : Vérifié dans `main.c` après la lecture du fichier `crazy.cfg`.
- [x] **Identification du joueur** : Le programme vérifie que le nom saisi correspond à un joueur enregistré (`indexJoueur == -1`).
- [x] **Élimination par erreur** : Un joueur qui se trompe (`peutjouer = 0`) ne peut plus proposer de séquence pour le reste du tour.

---

## 2. Séquences et Syntaxe
- [x] **Droit à l'erreur de syntaxe** : Si une séquence contient un ordre inconnu, le programme affiche un message d'erreur mais ne disqualifie pas le joueur (utilisation de `continue` dans la boucle de saisie).
- [x] **Attribution des points (Succès)** : Si `testerCarte` est vrai, le joueur gagne un point et une nouvelle carte est tirée.
- [x] **Attribution des points (Élimination)** : Si la disqualification d'un joueur laisse un seul survivant en lice, ce survivant gagne le point du tour automatiquement.

---

## 3. Classement Final
- [x] **Ordre de priorité** :
    1. Score décroissant.
    2. Ordre alphabétique (ascendant) en cas d'égalité.
- [x] **Implémentation** : La fonction `comparerJoueurs` utilise `j2->score - j1->score` (décroissant) et `strcmp(j1->nom, j2->nom)` (ascendant).
- [x] **Format d'affichage** : Sortie brute `Nom Score` sans décoration superflue, conforme aux exemples.

---

## 4. Nettoyage de la Mémoire
- [x] **Libération complète** : Toutes les allocations (joueurs, config, cartes, podiums) sont libérées via `libererConfig`, `libererPartie` et `free` en fin de `main`.
- [x] **Robustesse** : Utilisation de `nbCartesTotal` pour garantir que même les cartes déjà piochées sont libérées de la mémoire.
