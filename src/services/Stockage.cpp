#include "Stockage.h"
#include "../utils/Exceptions.h"
#include "../utils/Statuts.h"
#include "../model/SectionsConcretes.h"

#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QString>

#include <unordered_map>
#include <functional>
#include <memory>

Stockage::Stockage(std::string cheminFichier) : m_chemin(std::move(cheminFichier)) {}

// --- Helpers locaux ---

namespace {

// Fabrique une section concrete a partir de son nom. Utilise une map de
// fonctions (illustration de std::map / unordered_map demandee par le sujet).
std::unique_ptr<SectionArticle> creerSectionParNom(const std::string& nom) {
    static const std::unordered_map<std::string,
                                    std::function<std::unique_ptr<SectionArticle>()>> fabriques = {
        {"Resume",                  []{ return std::make_unique<SectionResume>(); }},
        {"Mots-cles",               []{ return std::make_unique<SectionMotsCles>(); }},
        {"Introduction",            []{ return std::make_unique<SectionIntroduction>(); }},
        {"Etat de l'art",           []{ return std::make_unique<SectionEtatArt>(); }},
        {"Methodologie",            []{ return std::make_unique<SectionMethodologie>(); }},
        {"Resultats et discussion", []{ return std::make_unique<SectionResultatsDiscussion>(); }},
        {"Conclusion",              []{ return std::make_unique<SectionConclusion>(); }},
        {"Bibliographie",           []{ return std::make_unique<SectionBibliographie>(); }},
    };
    auto it = fabriques.find(nom);
    if (it == fabriques.end()) return nullptr;
    return it->second();
}

QJsonObject auteurEnJson(const Auteur& a) {
    QJsonObject o;
    o["nom"]    = QString::fromStdString(a.nom());
    o["prenom"] = QString::fromStdString(a.prenom());
    o["role"]   = QString::fromStdString(a.role());
    return o;
}

QJsonObject articleEnJson(const Article& art) {
    QJsonObject o;
    o["titre"]      = QString::fromStdString(art.titre());
    o["revue"]      = QString::fromStdString(art.revue());
    o["dateLimite"] = QString::fromStdString(art.dateLimite());
    o["statut"]     = QString::fromStdString(statutArticleVersTexte(art.statut()));

    // Auteurs
    QJsonArray aut;
    for (const auto& a : art.auteurs()) aut.append(auteurEnJson(*a));
    o["auteurs"] = aut;

    // Sections
    QJsonArray secs;
    for (const auto& s : art.sections()) {
        QJsonObject so;
        so["nom"]    = QString::fromStdString(s->nom());
        so["statut"] = QString::fromStdString(statutSectionVersTexte(s->statut()));
        so["note"]   = QString::fromStdString(s->note());
        if (s->auteur()) {
            so["auteurIdentite"] = QString::fromStdString(s->auteur()->identite());
        } else {
            so["auteurIdentite"] = "";
        }
        secs.append(so);
    }
    o["sections"] = secs;
    return o;
}

} // namespace

void Stockage::sauvegarder(const GestionnaireArticles& gest) const {
    QJsonArray arr;
    for (const auto& a : gest.articles()) arr.append(articleEnJson(*a));

    QJsonDocument doc(arr);
    QFile f(QString::fromStdString(m_chemin));
    if (!f.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        throw ErreurFichier("Impossible d'ouvrir le fichier en ecriture: " + m_chemin);
    }
    f.write(doc.toJson(QJsonDocument::Indented));
    f.close();
}

void Stockage::charger(GestionnaireArticles& gest) const {
    QFile f(QString::fromStdString(m_chemin));
    if (!f.exists()) return;  // premier lancement, pas de donnees
    if (!f.open(QIODevice::ReadOnly)) {
        throw ErreurFichier("Impossible d'ouvrir le fichier en lecture: " + m_chemin);
    }
    QByteArray contenu = f.readAll();
    f.close();

    QJsonParseError err;
    QJsonDocument doc = QJsonDocument::fromJson(contenu, &err);
    if (err.error != QJsonParseError::NoError || !doc.isArray()) {
        throw ErreurFichier("Fichier de sauvegarde corrompu.");
    }

    gest.viderTout();
    for (const auto& v : doc.array()) {
        QJsonObject o = v.toObject();
        auto art = std::make_unique<Article>(
            o["titre"].toString().toStdString(),
            o["revue"].toString().toStdString(),
            o["dateLimite"].toString().toStdString()
        );
        try {
            art->setStatut(texteVersStatutArticle(o["statut"].toString().toStdString()));
        } catch (...) {
            // statut inconnu : on garde "en redaction"
        }

        // Reconstruire les auteurs et garder un index par identite pour
        // pouvoir relier les sections a leur auteur.
        std::unordered_map<std::string, std::shared_ptr<Auteur>> indexAuteurs;
        for (const auto& av : o["auteurs"].toArray()) {
            QJsonObject ao = av.toObject();
            auto auteur = std::make_shared<Auteur>(
                ao["nom"].toString().toStdString(),
                ao["prenom"].toString().toStdString(),
                ao["role"].toString().toStdString()
            );
            indexAuteurs[auteur->identite()] = auteur;
            art->ajouterAuteur(auteur);
        }

        // Reconstruire les sections
        for (const auto& sv : o["sections"].toArray()) {
            QJsonObject so = sv.toObject();
            std::string nomSec = so["nom"].toString().toStdString();
            auto section = creerSectionParNom(nomSec);
            if (!section) continue;  // section inconnue, on ignore
            try {
                section->setStatut(texteVersStatutSection(so["statut"].toString().toStdString()));
            } catch (...) {}
            section->setNote(so["note"].toString().toStdString());
            std::string ident = so["auteurIdentite"].toString().toStdString();
            auto it = indexAuteurs.find(ident);
            if (it != indexAuteurs.end()) section->setAuteur(it->second);
            art->ajouterSection(std::move(section));
        }

        gest.ajouterArticleExistant(std::move(art));
    }
}
