# Research Article Tracker

Outil de planification et de suivi de la redaction d'articles scientifiques.
Mini-projet M1 SIGL — Langage C++.

## Pre-requis

- C++17 ou superieur
- Qt 5 (module Widgets)
- CMake >= 3.16 **ou** qmake

## Compilation

### Option 1 — CMake (recommande)

```bash
mkdir build && cd build
cmake ..
make
./ResearchArticleTracker
```

### Option 2 — qmake

```bash
qmake ResearchArticleTracker.pro
make
./ResearchArticleTracker
```

### Option 3 — Qt Creator

Ouvrir le fichier `ResearchArticleTracker.pro` dans Qt Creator,
puis lancer la compilation et l'execution depuis l'IDE.

## Utilisation

1. **Nouvel article** : clic sur *Nouvel article*, renseigner titre, revue
   et date limite. Les 8 sections standard sont creees automatiquement.
2. **Ajouter un auteur** : clic sur *Ajouter auteur* dans le panneau de droite.
3. **Associer un auteur a une section** : selectionner une ligne dans le
   tableau des sections, puis *Associer auteur*.
4. **Modifier le statut d'une section** : selectionner une section,
   puis *Changer statut*. Quand toutes les sections sont *terminee*,
   l'article passe automatiquement a *termine* (sauf s'il est deja
   *soumis* ou *accepte*).
5. **Note** : *Modifier note* pour ajouter une observation sur une section.

Les donnees sont sauvegardees automatiquement dans :

- Linux : `~/.local/share/M1-SIGL/ResearchArticleTracker/articles.json`
- Windows : `%LOCALAPPDATA%\M1-SIGL\ResearchArticleTracker\articles.json`

## Architecture

```
src/
 |-- model/        classes metier (Article, Auteur, SectionArticle...)
 |-- view/         interface graphique Qt
 |-- services/     gestionnaire d'articles, sauvegarde JSON
 |-- utils/        enums (statuts) et exceptions
 |-- main.cpp
```
# ResearchArticleTracker
# ResearchArticleTracker
