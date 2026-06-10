#ifndef SECTION_ARTICLE_H
#define SECTION_ARTICLE_H

#include <string>
#include <memory>
#include "../utils/Statuts.h"

class Auteur;

// Classe abstraite representant une section de redaction d'un article.
// Chaque section concrete devra preciser son nom, sa description et,
// le cas echeant, des regles specifiques (priorite, longueur attendue...).
class SectionArticle {
public:
    SectionArticle();
    virtual ~SectionArticle() = default;

    // --- methodes virtuelles pures : comportement specifique ---
    virtual std::string nom() const = 0;
    virtual std::string description() const = 0;
    // Priorite indicative (1 = haute, 3 = basse). Permet par exemple de
    // trier les sections ou de mettre en avant les plus critiques.
    virtual int priorite() const = 0;

    // Validation specifique de la section : par defaut on verifie juste
    // que la note n'est pas absurde, mais chaque section peut ajouter
    // des contraintes (ex: mots-cles non vides si terminee).
    virtual void valider() const;

    // --- attributs communs a toutes les sections ---
    StatutSection statut() const          { return m_statut; }
    void setStatut(StatutSection s)       { m_statut = s; }

    const std::string& note() const       { return m_note; }
    void setNote(const std::string& n)    { m_note = n; }

    // L'auteur responsable est partage : plusieurs sections peuvent
    // pointer vers le meme objet Auteur stocke dans l'Article.
    std::shared_ptr<Auteur> auteur() const            { return m_auteur; }
    void setAuteur(std::shared_ptr<Auteur> a)         { m_auteur = std::move(a); }

    bool estTerminee() const { return m_statut == StatutSection::Terminee; }

protected:
    StatutSection m_statut;
    std::string m_note;
    std::shared_ptr<Auteur> m_auteur;
};

#endif // SECTION_ARTICLE_H
