#include "FenetrePrincipale.h"
#include "../utils/Exceptions.h"
#include "../utils/Statuts.h"
#include "../model/SectionArticle.h"

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QGroupBox>
#include <QHeaderView>
#include <QInputDialog>
#include <QMessageBox>
#include <QDialog>
#include <QLineEdit>
#include <QDialogButtonBox>
#include <QCloseEvent>
#include <QStandardPaths>
#include <QDir>

#include <algorithm>

FenetrePrincipale::FenetrePrincipale(QWidget* parent) : QMainWindow(parent) {
    // Determine un chemin de sauvegarde portable (dossier de donnees applis).
    QString dossier = QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation);
    QDir().mkpath(dossier);
    QString chemin = dossier + "/articles.json";
    m_stockage = std::make_unique<Stockage>(chemin.toStdString());

    construireUI();

    // Chargement initial des donnees, gestion d'erreurs.
    try {
        m_stockage->charger(m_gest);
    } catch (const std::exception& e) {
        QMessageBox::warning(this, "Chargement",
            QString("Impossible de charger les donnees: ") + e.what());
    }
    rafraichirListeArticles();
}

void FenetrePrincipale::closeEvent(QCloseEvent* event) {
    sauvegarderSilencieux();
    event->accept();
}

void FenetrePrincipale::sauvegarderSilencieux() {
    try {
        m_stockage->sauvegarder(m_gest);
    } catch (const std::exception& e) {
        QMessageBox::warning(this, "Sauvegarde",
            QString("Echec de la sauvegarde: ") + e.what());
    }
}

// ---------------------------------------------------------------------------
// Construction de l'interface
// ---------------------------------------------------------------------------
void FenetrePrincipale::construireUI() {
    setWindowTitle("Research Article Tracker");
    resize(1100, 650);

    auto* central = new QWidget(this);
    auto* mainLayout = new QHBoxLayout(central);

    // ---- Colonne gauche : liste des articles ----
    auto* gauche = new QVBoxLayout();
    auto* lblArticles = new QLabel("<b>Articles</b>");
    m_listeArticles = new QListWidget();
    auto* btnNouv = new QPushButton("Nouvel article");
    auto* btnSupp = new QPushButton("Supprimer");

    gauche->addWidget(lblArticles);
    gauche->addWidget(m_listeArticles);
    gauche->addWidget(btnNouv);
    gauche->addWidget(btnSupp);

    connect(btnNouv, &QPushButton::clicked, this, &FenetrePrincipale::onNouvelArticle);
    connect(btnSupp, &QPushButton::clicked, this, &FenetrePrincipale::onSupprimerArticle);
    connect(m_listeArticles, &QListWidget::itemSelectionChanged,
            this, &FenetrePrincipale::onArticleSelectionne);

    // ---- Colonne droite : details de l'article ----
    auto* droite = new QVBoxLayout();

    // Bandeau infos article
    auto* infoBox = new QGroupBox("Article selectionne");
    auto* infoForm = new QFormLayout();
    m_labTitre = new QLabel("-");
    m_labRevue = new QLabel("-");
    m_labDate  = new QLabel("-");
    m_comboStatutArticle = new QComboBox();
    m_comboStatutArticle->addItems({"en redaction", "termine", "soumis", "accepte"});
    infoForm->addRow("Titre:", m_labTitre);
    infoForm->addRow("Revue/Conference:", m_labRevue);
    infoForm->addRow("Date limite:", m_labDate);
    infoForm->addRow("Statut:", m_comboStatutArticle);
    infoBox->setLayout(infoForm);
    connect(m_comboStatutArticle, QOverload<int>::of(&QComboBox::activated),
            this, &FenetrePrincipale::onChangerStatutArticle);

    // Tableau des sections
    auto* secBox = new QGroupBox("Sections");
    auto* secLayout = new QVBoxLayout();
    m_tableSections = new QTableWidget(0, 5);
    m_tableSections->setHorizontalHeaderLabels(
        {"Section", "Statut", "Auteur", "Note", "Priorite"});
    m_tableSections->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_tableSections->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_tableSections->setEditTriggers(QAbstractItemView::NoEditTriggers);

    auto* secBtns = new QHBoxLayout();
    auto* btnStatutSec = new QPushButton("Changer statut");
    auto* btnAssocAut  = new QPushButton("Associer auteur");
    auto* btnNote      = new QPushButton("Modifier note");
    secBtns->addWidget(btnStatutSec);
    secBtns->addWidget(btnAssocAut);
    secBtns->addWidget(btnNote);
    secLayout->addWidget(m_tableSections);
    secLayout->addLayout(secBtns);
    secBox->setLayout(secLayout);

    connect(btnStatutSec, &QPushButton::clicked, this, &FenetrePrincipale::onChangerStatutSection);
    connect(btnAssocAut,  &QPushButton::clicked, this, &FenetrePrincipale::onAssocierAuteurSection);
    connect(btnNote,      &QPushButton::clicked, this, &FenetrePrincipale::onModifierNoteSection);

    // Tableau de bord
    auto* dashBox = new QGroupBox("Tableau de bord");
    auto* dashForm = new QFormLayout();
    m_labTotal        = new QLabel("0");
    m_labTermine      = new QLabel("0");
    m_labEnCours      = new QLabel("0");
    m_labStatutGlobal = new QLabel("-");
    m_barProgress     = new QProgressBar();
    m_barProgress->setRange(0, 100);
    dashForm->addRow("Sections au total:", m_labTotal);
    dashForm->addRow("Terminees:", m_labTermine);
    dashForm->addRow("En cours:", m_labEnCours);
    dashForm->addRow("Statut global:", m_labStatutGlobal);
    dashForm->addRow("Avancement:", m_barProgress);
    dashBox->setLayout(dashForm);

    // Auteurs de l'article
    auto* autBox = new QGroupBox("Auteurs de l'article");
    auto* autLayout = new QVBoxLayout();
    m_listeAuteurs = new QListWidget();
    auto* btnNouvAut = new QPushButton("Ajouter auteur");
    autLayout->addWidget(m_listeAuteurs);
    autLayout->addWidget(btnNouvAut);
    autBox->setLayout(autLayout);
    connect(btnNouvAut, &QPushButton::clicked, this, &FenetrePrincipale::onNouvelAuteur);

    // Disposition droite
    droite->addWidget(infoBox);
    droite->addWidget(secBox, 1);

    auto* basDroit = new QHBoxLayout();
    basDroit->addWidget(dashBox, 1);
    basDroit->addWidget(autBox, 1);
    droite->addLayout(basDroit);

    mainLayout->addLayout(gauche, 1);
    mainLayout->addLayout(droite, 3);
    setCentralWidget(central);
}

// ---------------------------------------------------------------------------
// Acces aux elements selectionnes
// ---------------------------------------------------------------------------
Article* FenetrePrincipale::articleCourant() const {
    int row = m_listeArticles->currentRow();
    return m_gest.articleAt(row);
}

int FenetrePrincipale::sectionCouranteIndex() const {
    return m_tableSections->currentRow();
}

// ---------------------------------------------------------------------------
// Rafraichissements
// ---------------------------------------------------------------------------
void FenetrePrincipale::rafraichirListeArticles() {
    int rowAvant = m_listeArticles->currentRow();
    m_listeArticles->clear();
    for (const auto& a : m_gest.articles()) {
        QString libelle = QString::fromStdString(a->titre())
            + "  [" + QString::fromStdString(statutArticleVersTexte(a->statut())) + "]";
        m_listeArticles->addItem(libelle);
    }
    if (rowAvant >= 0 && rowAvant < m_listeArticles->count()) {
        m_listeArticles->setCurrentRow(rowAvant);
    } else if (m_listeArticles->count() > 0) {
        m_listeArticles->setCurrentRow(0);
    } else {
        rafraichirDetailArticle();
    }
}

void FenetrePrincipale::rafraichirDetailArticle() {
    Article* art = articleCourant();
    if (!art) {
        m_labTitre->setText("-");
        m_labRevue->setText("-");
        m_labDate->setText("-");
        m_comboStatutArticle->setCurrentIndex(0);
        m_tableSections->setRowCount(0);
        m_listeAuteurs->clear();
        rafraichirTableauDeBord();
        return;
    }

    m_labTitre->setText(QString::fromStdString(art->titre()));
    m_labRevue->setText(QString::fromStdString(art->revue()));
    m_labDate->setText(QString::fromStdString(art->dateLimite()));
    m_comboStatutArticle->setCurrentText(
        QString::fromStdString(statutArticleVersTexte(art->statut())));

    // Sections
    const auto& secs = art->sections();
    m_tableSections->setRowCount(static_cast<int>(secs.size()));
    for (int i = 0; i < static_cast<int>(secs.size()); ++i) {
        const SectionArticle& s = *secs[i];
        m_tableSections->setItem(i, 0, new QTableWidgetItem(QString::fromStdString(s.nom())));
        m_tableSections->setItem(i, 1, new QTableWidgetItem(
            QString::fromStdString(statutSectionVersTexte(s.statut()))));
        m_tableSections->setItem(i, 2, new QTableWidgetItem(
            s.auteur() ? QString::fromStdString(s.auteur()->identite()) : "-"));
        m_tableSections->setItem(i, 3, new QTableWidgetItem(QString::fromStdString(s.note())));
        m_tableSections->setItem(i, 4, new QTableWidgetItem(QString::number(s.priorite())));
    }

    // Auteurs
    m_listeAuteurs->clear();
    for (const auto& a : art->auteurs()) {
        m_listeAuteurs->addItem(QString::fromStdString(a->identite()));
    }

    rafraichirTableauDeBord();
}

void FenetrePrincipale::rafraichirTableauDeBord() {
    Article* art = articleCourant();
    if (!art) {
        m_labTotal->setText("0");
        m_labTermine->setText("0");
        m_labEnCours->setText("0");
        m_labStatutGlobal->setText("-");
        m_barProgress->setValue(0);
        return;
    }
    m_labTotal->setText(QString::number(art->nombreSections()));
    m_labTermine->setText(QString::number(art->nombreSectionsTerminees()));
    m_labEnCours->setText(QString::number(art->nombreSectionsEnCours()));
    m_labStatutGlobal->setText(QString::fromStdString(statutArticleVersTexte(art->statut())));
    m_barProgress->setValue(art->pourcentageAvancement());
}

// ---------------------------------------------------------------------------
// Actions sur les articles
// ---------------------------------------------------------------------------
void FenetrePrincipale::onNouvelArticle() {
    // Petite boite de dialogue maison avec 3 champs : titre, revue, date.
    QDialog dlg(this);
    dlg.setWindowTitle("Nouvel article");
    auto* form = new QFormLayout(&dlg);
    auto* eTitre = new QLineEdit();
    auto* eRevue = new QLineEdit();
    auto* eDate  = new QLineEdit();
    eDate->setPlaceholderText("AAAA-MM-JJ (indicatif)");
    form->addRow("Titre*:", eTitre);
    form->addRow("Revue/Conference:", eRevue);
    form->addRow("Date limite:", eDate);
    auto* boutons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    form->addRow(boutons);
    connect(boutons, &QDialogButtonBox::accepted, &dlg, &QDialog::accept);
    connect(boutons, &QDialogButtonBox::rejected, &dlg, &QDialog::reject);

    if (dlg.exec() != QDialog::Accepted) return;

    try {
        m_gest.creerArticle(
            eTitre->text().trimmed().toStdString(),
            eRevue->text().trimmed().toStdString(),
            eDate->text().trimmed().toStdString()
        );
    } catch (const std::exception& e) {
        QMessageBox::warning(this, "Erreur", e.what());
        return;
    }
    rafraichirListeArticles();
    m_listeArticles->setCurrentRow(m_gest.nombreArticles() - 1);
    sauvegarderSilencieux();
}

void FenetrePrincipale::onSupprimerArticle() {
    int row = m_listeArticles->currentRow();
    if (row < 0) return;
    if (QMessageBox::question(this, "Confirmation",
            "Supprimer cet article ?") != QMessageBox::Yes) return;

    // On reconstruit le vector sans l'element supprime.
    auto& articles = const_cast<std::vector<std::unique_ptr<Article>>&>(m_gest.articles());
    articles.erase(articles.begin() + row);
    rafraichirListeArticles();
    sauvegarderSilencieux();
}

void FenetrePrincipale::onArticleSelectionne() {
    rafraichirDetailArticle();
}

void FenetrePrincipale::onChangerStatutArticle() {
    Article* art = articleCourant();
    if (!art) return;
    try {
        art->setStatut(texteVersStatutArticle(
            m_comboStatutArticle->currentText().toStdString()));
        // Si l'utilisateur a choisi un statut moins avance que ce que la
        // synchro automatique imposerait, on resynchronise.
        art->synchroniserStatut();
    } catch (const std::exception& e) {
        QMessageBox::warning(this, "Erreur", e.what());
    }
    rafraichirListeArticles();
    sauvegarderSilencieux();
}

// ---------------------------------------------------------------------------
// Actions sur les auteurs
// ---------------------------------------------------------------------------
void FenetrePrincipale::onNouvelAuteur() {
    Article* art = articleCourant();
    if (!art) {
        QMessageBox::information(this, "Info", "Selectionnez d'abord un article.");
        return;
    }

    QDialog dlg(this);
    dlg.setWindowTitle("Nouvel auteur");
    auto* form = new QFormLayout(&dlg);
    auto* eNom    = new QLineEdit();
    auto* ePrenom = new QLineEdit();
    auto* cbRole  = new QComboBox();
    cbRole->addItems({"auteur principal", "co-auteur", "encadrant", "contributeur"});
    form->addRow("Nom*:", eNom);
    form->addRow("Prenom*:", ePrenom);
    form->addRow("Role:", cbRole);
    auto* boutons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    form->addRow(boutons);
    connect(boutons, &QDialogButtonBox::accepted, &dlg, &QDialog::accept);
    connect(boutons, &QDialogButtonBox::rejected, &dlg, &QDialog::reject);

    if (dlg.exec() != QDialog::Accepted) return;

    try {
        auto a = std::make_shared<Auteur>(
            eNom->text().trimmed().toStdString(),
            ePrenom->text().trimmed().toStdString(),
            cbRole->currentText().toStdString()
        );
        art->ajouterAuteur(a);
    } catch (const std::exception& e) {
        QMessageBox::warning(this, "Erreur", e.what());
        return;
    }
    rafraichirDetailArticle();
    sauvegarderSilencieux();
}

// ---------------------------------------------------------------------------
// Actions sur les sections
// ---------------------------------------------------------------------------
void FenetrePrincipale::onAssocierAuteurSection() {
    Article* art = articleCourant();
    int idx = sectionCouranteIndex();
    if (!art || idx < 0 || idx >= art->nombreSections()) {
        QMessageBox::information(this, "Info", "Selectionnez une section.");
        return;
    }
    if (art->auteurs().empty()) {
        QMessageBox::information(this, "Info",
            "Aucun auteur disponible. Ajoutez d'abord un auteur a l'article.");
        return;
    }

    QStringList choix;
    choix << "(aucun)";
    for (const auto& a : art->auteurs()) {
        choix << QString::fromStdString(a->identite());
    }
    bool ok = false;
    QString sel = QInputDialog::getItem(this, "Associer un auteur",
        "Choisir l'auteur responsable:", choix, 0, false, &ok);
    if (!ok) return;

    auto& sec = art->sections()[idx];
    if (sel == "(aucun)") {
        sec->setAuteur(nullptr);
    } else {
        for (const auto& a : art->auteurs()) {
            if (QString::fromStdString(a->identite()) == sel) {
                sec->setAuteur(a);
                break;
            }
        }
    }
    rafraichirDetailArticle();
    sauvegarderSilencieux();
}

void FenetrePrincipale::onChangerStatutSection() {
    Article* art = articleCourant();
    int idx = sectionCouranteIndex();
    if (!art || idx < 0 || idx >= art->nombreSections()) {
        QMessageBox::information(this, "Info", "Selectionnez une section.");
        return;
    }

    QStringList choix {"non commencee", "en cours", "terminee"};
    auto& sec = art->sections()[idx];
    int courant = 0;
    if (sec->statut() == StatutSection::EnCours) courant = 1;
    else if (sec->statut() == StatutSection::Terminee) courant = 2;

    bool ok = false;
    QString sel = QInputDialog::getItem(this, "Statut de section",
        "Nouveau statut:", choix, courant, false, &ok);
    if (!ok) return;

    StatutSection ancien = sec->statut();
    try {
        sec->setStatut(texteVersStatutSection(sel.toStdString()));
        sec->valider();           // validation polymorphique specifique
        art->synchroniserStatut(); // regle metier centrale
    } catch (const std::exception& e) {
        sec->setStatut(ancien);   // rollback si la validation echoue
        QMessageBox::warning(this, "Erreur", e.what());
    }
    rafraichirListeArticles();
    sauvegarderSilencieux();
}

void FenetrePrincipale::onModifierNoteSection() {
    Article* art = articleCourant();
    int idx = sectionCouranteIndex();
    if (!art || idx < 0 || idx >= art->nombreSections()) {
        QMessageBox::information(this, "Info", "Selectionnez une section.");
        return;
    }
    auto& sec = art->sections()[idx];
    bool ok = false;
    QString nouv = QInputDialog::getMultiLineText(this, "Note / observation",
        "Note pour la section \"" + QString::fromStdString(sec->nom()) + "\":",
        QString::fromStdString(sec->note()), &ok);
    if (!ok) return;
    sec->setNote(nouv.toStdString());
    rafraichirDetailArticle();
    sauvegarderSilencieux();
}
