QT       += core gui widgets

CONFIG   += c++17
TARGET   = ResearchArticleTracker
TEMPLATE = app

SOURCES += \
    src/main.cpp \
    src/model/Auteur.cpp \
    src/model/SectionArticle.cpp \
    src/model/SectionsConcretes.cpp \
    src/model/Article.cpp \
    src/services/GestionnaireArticles.cpp \
    src/services/Stockage.cpp \
    src/utils/Statuts.cpp \
    src/view/FenetrePrincipale.cpp

HEADERS += \
    src/model/Auteur.h \
    src/model/SectionArticle.h \
    src/model/SectionsConcretes.h \
    src/model/Article.h \
    src/services/GestionnaireArticles.h \
    src/services/Stockage.h \
    src/utils/Exceptions.h \
    src/utils/Statuts.h \
    src/view/FenetrePrincipale.h
