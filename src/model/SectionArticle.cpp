#include "SectionArticle.h"
#include "../utils/Exceptions.h"

SectionArticle::SectionArticle()
    : m_statut(StatutSection::NonCommencee), m_note(""), m_auteur(nullptr)
{}

// Validation de base : une section ne peut pas etre marquee "terminee"
// sans avoir un auteur responsable. Les classes derivees peuvent
// renforcer cette regle.
void SectionArticle::valider() const {
    if (m_statut == StatutSection::Terminee && !m_auteur) {
        throw ErreurSaisie(
            "La section \"" + nom() + "\" est terminee mais n'a pas d'auteur responsable.");
    }
}
