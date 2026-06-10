#ifndef AUTEUR_H
#define AUTEUR_H

#include <string>

// Represente un auteur d'article scientifique.
// Exemples de roles : "auteur principal", "co-auteur", "encadrant", "contributeur"
class Auteur {
public:
    Auteur() = default;
    Auteur(std::string nom, std::string prenom, std::string role);

    const std::string& nom() const    { return m_nom; }
    const std::string& prenom() const { return m_prenom; }
    const std::string& role() const   { return m_role; }

    void setNom(const std::string& n)    { m_nom = n; }
    void setPrenom(const std::string& p) { m_prenom = p; }
    void setRole(const std::string& r)   { m_role = r; }

    // Identifiant lisible "Prenom Nom (role)"
    std::string identite() const;

private:
    std::string m_nom;
    std::string m_prenom;
    std::string m_role;
};

#endif // AUTEUR_H
