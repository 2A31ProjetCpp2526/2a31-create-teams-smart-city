#include "connection.h"
#include <QMessageBox>

Connection::Connection()
{
}

bool Connection::createconnect()
{
    bool test = false;
    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
    
    // Afficher les pilotes disponibles pour le débogage
    qDebug() << "Drivers available:" << QSqlDatabase::drivers();
    
    db.setDatabaseName("Source_Projet2A");
    // Utiliser ici le même utilisateur que dans SQL Developer
    db.setUserName("SYSTEM");
    db.setPassword("Youssefab27@");

    if (db.open()) {
        qDebug() << "Connexion à la base de données établie avec succès!";
        // On utilise simplement le schéma par défaut de l'utilisateur connecté
        test = true;
    } else {
        QString error = db.lastError().text();
        qDebug() << "Échec de la connexion à la base de données:" << error;
        
        // Afficher un message d'erreur détaillé
        QMessageBox::critical(nullptr, "Erreur de connexion", 
                            "Impossible de se connecter à la base de données.\n\n"
                            "Veuillez vérifier que :\n"
                            "1. Le service Oracle est démarré\n"
                            "2. La source de données ODBC 'Projet2A' est correctement configurée\n"
                            "3. Le nom d'utilisateur et le mot de passe sont corrects\n\n"
                            "Détails de l'erreur :\n" + error);
    }

    return test;
}
