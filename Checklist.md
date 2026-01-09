# Checklist de Conformité - Crazy Circus

Ce document récapitule la conformité du projet par rapport aux exigences du sujet `sujet-s1.02.pdf`.

---

## 1. Exigences Fonctionnelles

- [x] **Arguments de ligne de commande** : Les noms des joueurs (minimum 2, distincts) sont récupérés via `argv`. Le programme s'arrête prématurément avec un message informatif si les conditions ne sont pas remplies.
- [x] **Fichier de configuration (`crazy.cfg`)** : Lecture dynamique des animaux et des ordres autorisés dans le répertoire courant.
- [x] **Validation de la config** : Le programme s'arrête si le nombre d'animaux est inférieur à 2 ou si le nombre d'ordres est inférieur à 3 (avec message informatif sur une ligne).
- [x] **Rappel des ordres** : Affichage initial de la ligne de syntaxe des ordres autorisés (ex: `KI (B -> R) | LO (B <- R) ...`).
- [x] **Pioche de cartes** : Gestion d'un paquet de 24 cartes (pour 3 animaux) sans remise. Fin de partie automatique quand la pioche est vide.
- [x] **Position atteinte** : La position finale d'un tour devient la position de départ du tour suivant.

---

## 2. Logique du Jeu et Ordres

- [x] **KI** : L'animal en haut du podium bleu saute vers le podium rouge.
- [x] **LO** : L'animal en haut du podium rouge saute vers le podium bleu.
- [x] **SO** : Échange des sommets des deux podiums.
- [x] **NI** : Rotation du podium bleu (bas vers haut).
- [x] **MA** : Rotation du podium rouge (bas vers haut).
- [x] **Validation de séquence** : Vérification stricte si la séquence fournie par le joueur mène exactement à la position cible.
- [x] **Gestion des erreurs** : Un joueur qui donne une mauvaise séquence est éliminé pour le tour en cours.
- [x] **Dernier survivant** : Si un seul joueur reste en lice durant un tour, il remporte le point automatiquement.

---

## 3. Formats et Affichage (Stricts)

- [x] **Alignement des podiums** : Respect strict de l'alignement vertical des animaux sur les podiums et de l'alignement horizontal (Départ ==> Cible).
- [x] **Messages informatifs** : Libres mais restreints à une seule ligne de texte par message.
- [x] **Classement final** : Affichage par score décroissant, puis par ordre alphabétique en cas d'égalité. Format strict : `Nom Score`.

---

## 4. Code et Structure

- [x] **Modularité** : Séparation logique entre `main.c`, `config`, `jeu`, `joueur` et `partie`.
- [x] **Documentation** : Tous les fichiers `.h` contiennent les prototypes documentés (rôles, paramètres, pré-conditions).
- [x] **Tests Unitaires** : Isolés dans un dossier `tests/` avec un exécutable autonome (`tests/tests.c`).
- [x] **Rapport de projet** : Document `Report.md` complété détaillant les choix techniques et corrections.
