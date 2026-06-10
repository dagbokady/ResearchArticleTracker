#ifndef GESTIONNAIRE_ARTICLES_H
#define GESTIONNAIRE_ARTICLES_H

#include <vector>
#include <memory>
#include <string>
#include "../model/Article.h"

// Service principal qui centralise la liste des articles.
// Joue le role de "controleur" entre la vue et le modele.
class GestionnaireArticles {
public:
    GestionnaireArticles() = default;

    // Cree un nouvel article avec les 8 sections standard.
    // Renvoie un pointeur non-proprietaire pour usage immediat par l'UI.
    Article* creerArticle(const std::string& titre,
                          const std::string& revue,
                          const std::string& dateLimite);

    // Pour le chargement depuis fichier : on injecte un article deja construit.
    void ajouterArticleExistant(std::unique_ptr<Article> a);

    const std::vector<std::unique_ptr<Article>>& articles() const { return m_articles; }

    // Acces direct au i-eme article (renvoie nullptr si index invalide).
    Article* articleAt(int index) const;

    int nombreArticles() const { return static_cast<int>(m_articles.size()); }

    void viderTout();

private:
    std::vector<std::unique_ptr<Article>> m_articles;
};

#endif // GESTIONNAIRE_ARTICLES_H
