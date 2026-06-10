#include "GestionnaireArticles.h"
#include "../utils/Exceptions.h"

Article* GestionnaireArticles::creerArticle(const std::string& titre,
                                            const std::string& revue,
                                            const std::string& dateLimite) {
    auto art = std::make_unique<Article>(titre, revue, dateLimite);
    art->ajouterSectionsStandards();
    Article* ptr = art.get();
    m_articles.push_back(std::move(art));
    return ptr;
}

void GestionnaireArticles::ajouterArticleExistant(std::unique_ptr<Article> a) {
    if (!a) throw ErreurSaisie("Article invalide.");
    m_articles.push_back(std::move(a));
}

Article* GestionnaireArticles::articleAt(int index) const {
    if (index < 0 || index >= static_cast<int>(m_articles.size())) {
        return nullptr;
    }
    return m_articles[index].get();
}

void GestionnaireArticles::viderTout() {
    m_articles.clear();
}
