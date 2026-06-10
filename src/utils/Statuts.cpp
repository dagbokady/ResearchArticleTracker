#include "Statuts.h"
#include "Exceptions.h"

std::string statutArticleVersTexte(StatutArticle s) {
    switch (s) {
        case StatutArticle::EnRedaction: return "en redaction";
        case StatutArticle::Termine:     return "termine";
        case StatutArticle::Soumis:      return "soumis";
        case StatutArticle::Accepte:     return "accepte";
    }
    return "en redaction";
}

StatutArticle texteVersStatutArticle(const std::string& t) {
    if (t == "en redaction") return StatutArticle::EnRedaction;
    if (t == "termine")      return StatutArticle::Termine;
    if (t == "soumis")       return StatutArticle::Soumis;
    if (t == "accepte")      return StatutArticle::Accepte;
    throw ErreurStatut("Statut d'article invalide: " + t);
}

std::string statutSectionVersTexte(StatutSection s) {
    switch (s) {
        case StatutSection::NonCommencee: return "non commencee";
        case StatutSection::EnCours:      return "en cours";
        case StatutSection::Terminee:     return "terminee";
    }
    return "non commencee";
}

StatutSection texteVersStatutSection(const std::string& t) {
    if (t == "non commencee") return StatutSection::NonCommencee;
    if (t == "en cours")      return StatutSection::EnCours;
    if (t == "terminee")      return StatutSection::Terminee;
    throw ErreurStatut("Statut de section invalide: " + t);
}
