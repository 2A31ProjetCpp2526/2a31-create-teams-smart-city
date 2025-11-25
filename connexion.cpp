#include "connexion.h"
#include <QSqlError>
#include <QDebug>

Connexion::Connexion()
{
    // Création de la connexion avec QODBC pour Oracle
    db = QSqlDatabase::addDatabase("QODBC");

    // DSN / Utilisateur / Mot de passe
    db.setDatabaseName("city");
    db.setUserName("imen");
    db.setPassword("admin123");
}

// Singleton
Connexion& Connexion::createInstance()
{
    static Connexion instance;
    return instance;
}

Connexion::~Connexion()
{
    fermerConnexion();
}

// Ouvrir connexion
bool Connexion::ouvrirConnexion()
{
    if (db.open()) {
        qDebug() << "✅ Connexion ouverte avec succès.";
        return true;
    } else {
        qDebug() << "❌ Échec ouverture DB:" << db.lastError().text();
        return false;
    }
}

// Fermer connexion
void Connexion::fermerConnexion()
{
    if (db.isOpen()) {
        db.close();
        qDebug() << "🔒 Connexion fermée.";
    }
}
