#ifndef STATUTS_H
#define STATUTS_H

#include <string>

// Statut global d'un article scientifique
enum class StatutArticle {
    EnRedaction,
    Termine,
    Soumis,
    Accepte
};

// Statut d'une section de redaction
enum class StatutSection {
    NonCommencee,
    EnCours,
    Terminee
};

// Conversions string <-> enum (utilisees par l'UI et la sauvegarde)
std::string statutArticleVersTexte(StatutArticle s);
StatutArticle texteVersStatutArticle(const std::string& t);

std::string statutSectionVersTexte(StatutSection s);
StatutSection texteVersStatutSection(const std::string& t);

#endif // STATUTS_H
