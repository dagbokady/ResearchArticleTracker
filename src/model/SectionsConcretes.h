#ifndef SECTIONS_CONCRETES_H
#define SECTIONS_CONCRETES_H

#include "SectionArticle.h"

// --- Resume : haute priorite, doit etre concis ---
class SectionResume : public SectionArticle {
public:
    std::string nom() const override        { return "Resume"; }
    std::string description() const override{ return "Resume bref de l'article (~200 mots)."; }
    int priorite() const override           { return 1; }
    // Contrainte specifique : si terminee, la note (qui sert de resume)
    // ne doit pas etre vide.
    void valider() const override;
};

// --- Mots-cles : doit contenir des virgules quand termine ---
class SectionMotsCles : public SectionArticle {
public:
    std::string nom() const override        { return "Mots-cles"; }
    std::string description() const override{ return "Liste de mots-cles separes par des virgules."; }
    int priorite() const override           { return 2; }
    void valider() const override;
};

// --- Introduction : priorite haute, peu de contraintes specifiques ---
class SectionIntroduction : public SectionArticle {
public:
    std::string nom() const override        { return "Introduction"; }
    std::string description() const override{ return "Contexte, probleme, contribution."; }
    int priorite() const override           { return 1; }
};

// --- Etat de l'art ---
class SectionEtatArt : public SectionArticle {
public:
    std::string nom() const override        { return "Etat de l'art"; }
    std::string description() const override{ return "Travaux existants et positionnement."; }
    int priorite() const override           { return 2; }
};

// --- Methodologie : priorite haute, validation renforcee ---
class SectionMethodologie : public SectionArticle {
public:
    std::string nom() const override        { return "Methodologie"; }
    std::string description() const override{ return "Approche, materiel, demarche experimentale."; }
    int priorite() const override           { return 1; }
    // Une methodologie sans note descriptive n'a pas de sens.
    void valider() const override;
};

// --- Resultats et discussion ---
class SectionResultatsDiscussion : public SectionArticle {
public:
    std::string nom() const override        { return "Resultats et discussion"; }
    std::string description() const override{ return "Resultats obtenus et interpretation."; }
    int priorite() const override           { return 1; }
};

// --- Conclusion ---
class SectionConclusion : public SectionArticle {
public:
    std::string nom() const override        { return "Conclusion"; }
    std::string description() const override{ return "Synthese et perspectives."; }
    int priorite() const override           { return 2; }
};

// --- Bibliographie : priorite basse ---
class SectionBibliographie : public SectionArticle {
public:
    std::string nom() const override        { return "Bibliographie"; }
    std::string description() const override{ return "Liste des references citees."; }
    int priorite() const override           { return 3; }
};

#endif // SECTIONS_CONCRETES_H
