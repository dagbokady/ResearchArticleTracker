#ifndef FENETRE_PRINCIPALE_H
#define FENETRE_PRINCIPALE_H

#include <QMainWindow>
#include <QListWidget>
#include <QTableWidget>
#include <QLabel>
#include <QProgressBar>
#include <QPushButton>
#include <QComboBox>
#include <memory>

#include "../services/GestionnaireArticles.h"
#include "../services/Stockage.h"

// Fenetre principale de l'application.
// Architecture sobre : une liste d'articles a gauche, le detail de
// l'article selectionne a droite (tableau des sections + tableau de bord).
class FenetrePrincipale : public QMainWindow {
    Q_OBJECT
public:
    explicit FenetrePrincipale(QWidget* parent = nullptr);

protected:
    void closeEvent(QCloseEvent* event) override;

private slots:
    void onNouvelArticle();
    void onSupprimerArticle();
    void onArticleSelectionne();
    void onChangerStatutArticle();

    void onNouvelAuteur();
    void onAssocierAuteurSection();
    void onChangerStatutSection();
    void onModifierNoteSection();

private:
    // --- helpers UI ---
    void construireUI();
    void rafraichirListeArticles();
    void rafraichirDetailArticle();
    void rafraichirTableauDeBord();
    Article* articleCourant() const;
    int sectionCouranteIndex() const;

    void sauvegarderSilencieux();

    // --- modele / services ---
    GestionnaireArticles m_gest;
    std::unique_ptr<Stockage> m_stockage;

    // --- widgets ---
    QListWidget* m_listeArticles;
    QTableWidget* m_tableSections;
    QLabel* m_labTitre;
    QLabel* m_labRevue;
    QLabel* m_labDate;
    QComboBox* m_comboStatutArticle;

    QLabel* m_labTotal;
    QLabel* m_labTermine;
    QLabel* m_labEnCours;
    QLabel* m_labStatutGlobal;
    QProgressBar* m_barProgress;

    QListWidget* m_listeAuteurs;
};

#endif // FENETRE_PRINCIPALE_H
