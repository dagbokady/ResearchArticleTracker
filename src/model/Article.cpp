#include "Article.h"
#include "SectionsConcretes.h"
#include "../utils/Exceptions.h"
#include <algorithm>
#include <numeric>

Article::Article(std::string titre, std::string revue, std::string dateLimite)
    : m_titre(std::move(titre)),
      m_revue(std::move(revue)),
      m_dateLimite(std::move(dateLimite)),
      m_statut(StatutArticle::EnRedaction)
{
    if (m_titre.empty()) {
        throw ErreurSaisie("Le titre de l'article est obligatoire.");
    }
}

void Article::setTitre(const std::string& t) {
    if (t.empty()) {
        throw ErreurSaisie("Le titre de l'article ne peut pas etre vide.");
    }
    m_titre = t;
}

void Article::ajouterSection(std::unique_ptr<SectionArticle> section) {
    if (!section) {
        throw ErreurSaisie("Tentative d'ajouter une section nulle.");
    }
    m_sections.push_back(std::move(section));
}

void Article::ajouterSectionsStandards() {
    ajouterSection(std::make_unique<SectionResume>());
    ajouterSection(std::make_unique<SectionMotsCles>());
    ajouterSection(std::make_unique<SectionIntroduction>());
    ajouterSection(std::make_unique<SectionEtatArt>());
    ajouterSection(std::make_unique<SectionMethodologie>());
    ajouterSection(std::make_unique<SectionResultatsDiscussion>());
    ajouterSection(std::make_unique<SectionConclusion>());
    ajouterSection(std::make_unique<SectionBibliographie>());
}

void Article::ajouterAuteur(std::shared_ptr<Auteur> a) {
    if (!a) throw ErreurSaisie("Auteur invalide.");
    m_auteurs.push_back(std::move(a));
}

int Article::nombreSections() const {
    return static_cast<int>(m_sections.size());
}

// Algorithme STL : count_if sur les sections terminees.
int Article::nombreSectionsTerminees() const {
    return static_cast<int>(std::count_if(
        m_sections.begin(), m_sections.end(),
        [](const std::unique_ptr<SectionArticle>& s) {
            return s->statut() == StatutSection::Terminee;
        }));
}

int Article::nombreSectionsEnCours() const {
    return static_cast<int>(std::count_if(
        m_sections.begin(), m_sections.end(),
        [](const std::unique_ptr<SectionArticle>& s) {
            return s->statut() == StatutSection::EnCours;
        }));
}

int Article::pourcentageAvancement() const {
    if (m_sections.empty()) return 0;
    int termines = nombreSectionsTerminees();
    return (termines * 100) / nombreSections();
}

// Regle metier centrale : si toutes les sections sont terminees,
// l'article passe automatiquement a "termine" — sauf s'il est deja
// "soumis" ou "accepte" (statuts plus avances qu'on ne doit pas ecraser).
void Article::synchroniserStatut() {
    if (m_statut == StatutArticle::Soumis || m_statut == StatutArticle::Accepte) {
        return;
    }
    if (!m_sections.empty() && nombreSectionsTerminees() == nombreSections()) {
        m_statut = StatutArticle::Termine;
    } else {
        m_statut = StatutArticle::EnRedaction;
    }
}
