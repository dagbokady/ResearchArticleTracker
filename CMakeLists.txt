cmake_minimum_required(VERSION 3.16)
project(ResearchArticleTracker LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5 COMPONENTS Widgets REQUIRED)

add_executable(ResearchArticleTracker
    src/main.cpp

    src/model/Auteur.h
    src/model/Auteur.cpp
    src/model/SectionArticle.h
    src/model/SectionArticle.cpp
    src/model/SectionsConcretes.h
    src/model/SectionsConcretes.cpp
    src/model/Article.h
    src/model/Article.cpp

    src/services/GestionnaireArticles.h
    src/services/GestionnaireArticles.cpp
    src/services/Stockage.h
    src/services/Stockage.cpp

    src/utils/Exceptions.h
    src/utils/Statuts.h
    src/utils/Statuts.cpp

    src/view/FenetrePrincipale.h
    src/view/FenetrePrincipale.cpp
)

target_link_libraries(ResearchArticleTracker PRIVATE Qt5::Widgets)
