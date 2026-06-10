#include <QApplication>
#include "view/FenetrePrincipale.h"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    QApplication::setApplicationName("ResearchArticleTracker");
    QApplication::setOrganizationName("M1-SIGL");

    FenetrePrincipale w;
    w.show();
    return app.exec();
}
