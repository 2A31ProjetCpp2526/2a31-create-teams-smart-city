#include "mainwindow.h"
#include "connexion.h"
#include <QApplication>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include <QIcon>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // ✅ Récupération de l'instance unique et ouverture de la connexion
    Connexion& conn = Connexion::createInstance();
    if (!conn.ouvrirConnexion()) {
        QMessageBox::critical(
            nullptr,
            QObject::tr("Base de données"),
            QObject::tr("Échec de la connexion à Oracle.\nCliquez sur OK pour quitter."),
            QMessageBox::Ok
            );
        return -1;
    } else {
        QMessageBox::information(
            nullptr,
            QObject::tr("Base de données"),
            QObject::tr("Connexion réussie à Oracle.\nCliquez sur OK pour continuer."),
            QMessageBox::Ok
            );
    }

    // ✅ Chargement du style (facultatif)
    QFile f(":/style.qss");
    if (f.open(QFile::ReadOnly | QFile::Text)) {
        QTextStream ts(&f);
        a.setStyleSheet(ts.readAll());
    }

    // ✅ Création de la fenêtre principale
    MainWindow w;
    w.setWindowIcon(QIcon(":/Ressource/images/Votre_texte_de_paragraphe.png"));
    w.show();

    return a.exec();
}
