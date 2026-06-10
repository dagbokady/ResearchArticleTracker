#ifndef ARTICLE_H
#define ARTICLE_H

#include <string>
#include <vector>
#include <memory>
#include "SectionArticle.h"
#include "Auteur.h"
#include "../utils/Statuts.h"

// Represente un article scientifique : titre, revue cible, date limite,
// auteurs et liste de sections de redaction.
class Article {
public:
    Article(std::string titre, std::string revue, std::string dateLimite);

    // --- accesseurs simples ---
    const std::string& titre() const       { return m_titre; }
    const std::string& revue() const       { return m_revue; }
    const std::string& dateLimite() const  { return m_dateLimite; }
    StatutArticle statut() const           { return m_statut; }

    void setTitre(const std::string& t);
    void setRevue(const std::string& r)        { m_revue = r; }
    void setDateLimite(const std::string& d)   { m_dateLimite = d; }
    // Permet de passer manuellement a "soumis" ou "accepte".
    void setStatut(StatutArticle s)            { m_statut = s; }

    // --- sections : on stocke des pointeurs vers la classe abstraite,
    // ce qui permet le polymorphisme (chaque section a son comportement). ---
    const std::vector<std::unique_ptr<SectionArticle>>& sections() const { return m_sections; }
    std::vector<std::unique_ptr<SectionArticle>>& sections()             { return m_sections; }

    void ajouterSection(std::unique_ptr<SectionArticle> section);

    // Cree les 8 sections standard demandees par le cahier des charges.
    void ajouterSectionsStandards();

    // --- auteurs ---
    const std::vector<std::shared_ptr<Auteur>>& auteurs() const { return m_auteurs; }
    void ajouterAuteur(std::shared_ptr<Auteur> a);

    // --- logique metier : avancement et synchronisation des statuts ---
    int nombreSections() const;
    int nombreSectionsTerminees() const;
    int nombreSectionsEnCours() const;
    int pourcentageAvancement() const;  // 0..100

    // Si toutes les sections sont terminees et que le statut n'est ni
    // "soumis" ni "accepte", passe automatiquement l'article a "termine".
    void synchroniserStatut();

private:
    std::string m_titre;
    std::string m_revue;
    std::string m_dateLimite;
    StatutArticle m_statut;

    std::vector<std::unique_ptr<SectionArticle>> m_sections;
    std::vector<std::shared_ptr<Auteur>> m_auteurs;
};

#endif // ARTICLE_H
