#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <stdexcept>
#include <string>

// Exception levee lors d'une saisie invalide (titre vide, etc.)
class ErreurSaisie : public std::runtime_error {
public:
    explicit ErreurSaisie(const std::string& message)
        : std::runtime_error(message) {}
};

// Exception levee pour un statut invalide
class ErreurStatut : public std::runtime_error {
public:
    explicit ErreurStatut(const std::string& message)
        : std::runtime_error(message) {}
};

// Exception levee pour un probleme de fichier (sauvegarde / chargement)
class ErreurFichier : public std::runtime_error {
public:
    explicit ErreurFichier(const std::string& message)
        : std::runtime_error(message) {}
};

#endif // EXCEPTIONS_H
