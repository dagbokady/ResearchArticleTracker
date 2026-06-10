#include "Auteur.h"
#include "../utils/Exceptions.h"

Auteur::Auteur(std::string nom, std::string prenom, std::string role)
    : m_nom(std::move(nom)), m_prenom(std::move(prenom)), m_role(std::move(role))
{
    if (m_nom.empty() || m_prenom.empty()) {
        throw ErreurSaisie("Le nom et le prenom d'un auteur sont obligatoires.");
    }
}

std::string Auteur::identite() const {
    std::string s = m_prenom + " " + m_nom;
    if (!m_role.empty()) s += " (" + m_role + ")";
    return s;
}
