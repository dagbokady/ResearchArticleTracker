#include "SectionsConcretes.h"
#include "../utils/Exceptions.h"

// Resume : si la section est terminee, on exige qu'une note (le resume
// lui-meme) ait ete saisie. Cela illustre que les classes derivees
// renforcent les contraintes de la classe abstraite.
void SectionResume::valider() const {
    SectionArticle::valider();
    if (m_statut == StatutSection::Terminee && m_note.empty()) {
        throw ErreurSaisie("La section Resume est marquee terminee mais aucun resume n'a ete saisi.");
    }
}

// Mots-cles : si termines, on attend au moins une virgule (au minimum
// deux mots-cles). Comportement specifique distinct de la classe mere.
void SectionMotsCles::valider() const {
    SectionArticle::valider();
    if (m_statut == StatutSection::Terminee) {
        if (m_note.empty() || m_note.find(',') == std::string::npos) {
            throw ErreurSaisie(
                "Les mots-cles doivent contenir au moins deux entrees separees par une virgule.");
        }
    }
}

// Methodologie : si terminee, exige une note descriptive d'au moins
// 10 caracteres. Critere arbitraire mais illustratif.
void SectionMethodologie::valider() const {
    SectionArticle::valider();
    if (m_statut == StatutSection::Terminee && m_note.size() < 10) {
        throw ErreurSaisie(
            "La methodologie est marquee terminee mais sa description est trop courte.");
    }
}
