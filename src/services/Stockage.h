#ifndef STOCKAGE_H
#define STOCKAGE_H

#include <string>
#include "GestionnaireArticles.h"

// Service de sauvegarde / chargement sur disque au format JSON.
// On utilise QJsonDocument pour rester simple et portable.
class Stockage {
public:
    explicit Stockage(std::string cheminFichier);

    // Sauvegarde l'ensemble des articles dans le fichier.
    // Leve ErreurFichier en cas de probleme.
    void sauvegarder(const GestionnaireArticles& gest) const;

    // Charge les articles depuis le fichier. Si le fichier n'existe pas,
    // la fonction ne fait rien (premier lancement).
    void charger(GestionnaireArticles& gest) const;

private:
    std::string m_chemin;
};

#endif // STOCKAGE_H
