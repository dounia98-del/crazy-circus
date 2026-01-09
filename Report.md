# Rapport de Développement : Crazy Circus

Ce document résume les corrections, améliorations et changements structurels apportés au projet Crazy Circus.

---

## 1. Corrections de Bugs et Stabilité

### Gestion de la Mémoire
- **Correction d'une fuite mémoire majeure** : Dans `partie.c`, les cartes tirées n'étaient plus libérées car la boucle de nettoyage utilisait le nombre de cartes *restantes* dans la pioche. Ajout de `nbCartesTotal` dans la structure `Partie` pour garantir la libération de 100% des cartes allouées au démarrage.
- **Suppression des limites codées en dur** : Le jeu n'est plus limité à 10 animaux. L'allocation mémoire est désormais dynamique dans `main.c`, `jeu.c` et `partie.c`, se basant sur le fichier de configuration.
- **Initialisation sécurisée** : La structure `Config` est désormais initialisée à zéro/NULL par défaut pour éviter des comportements indéfinis en cas de fichier de configuration corrompu.

### Sécurité des Buffers
- **Correction d'un dépassement de buffer (Overflow)** : Dans `joueur.c`, une opération `strncpy` tentait de copier 50 caractères dans un buffer de 30. Correction apportée pour respecter la limite de 29 caractères + `\0`, harmonisée avec la saisie dans `main.c`.
- **Protection du parseur de config** : Ajout d'une limite de lecture (100 mots par ligne) lors du découpage du fichier `crazy.cfg` pour éviter d'écraser la pile.

---

## 2. Refactorisation et Modularité

### Déplacement des Fonctions Utilitaires
Pour améliorer la réutilisation du code et séparer les responsabilités :
- **`estOrdreValide`** : Déplacée de `main.c` vers le module `config.c/h`.
- **`copierPosition`** : Déplacée de `main.c` vers le module `jeu.c/h`.
- **Encodage** : Tous les fichiers ont été convertis en UTF-8 pour assurer une compilation sans avertissements sur tous les systèmes (Windows/Linux).

---

## 3. Système de Tests Unitaires

### Réorganisation Structurelle
Les tests unitaires ont été totalement isolés du code principal :
- **Dossier dédié** : Création du dossier `tests/`.
- **`tests/tests.h` & `tests/tests.c`** : Contiennent désormais toute la logique de diagnostic.

### Indépendance des Tests
- **Exécutable Autonome** : Le fichier `tests/tests.c` possède désormais sa propre fonction `main()`. Cela permet de lancer les tests sans démarrer le jeu.
- **Expérience Utilisateur épurée** : Les tests ne se lancent plus systématiquement au démarrage de `main.c`, permettant au jeu de commencer directement.

---

## 4. Améliorations de l'Interface (UI)

- **Affichage Dynamique** : La fonction `getCarte` dans `partie.c` calcule maintenant la hauteur d'affichage en fonction du nombre réel d'animaux.
- **Alignement Strict** : Correction des fonctions d'affichage pour garantir que les colonnes BLEU et ROUGE restent alignées, même avec des noms d'animaux de tailles différentes.

---

## 5. Guide de Compilation

### Compiler le Jeu
```powershell
gcc main.c config.c jeu.c joueur.c partie.c -o crazycircus.exe
```

### Compiler et Lancer les Tests
```powershell
gcc tests/tests.c config.c jeu.c joueur.c partie.c -o run_tests.exe
./run_tests.exe
```

### Vérification du Succès
- Tous les tests unitaires (Validation des ordres et Moteur de déplacement) passent avec **SUCCÈS**.
- Le système de score a été vérifié : il attribue correctement les points lors d'une réussite ou par élimination des adversaires.