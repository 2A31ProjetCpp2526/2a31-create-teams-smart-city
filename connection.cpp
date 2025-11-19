#include "connection.h"

Connection::Connection()
{

}

bool Connection::createconnect()
{bool test=false;
    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
    db.setDatabaseName("smartcity");//inserer le nom de la source de données
    db.setUserName("touha");//inserer nom de l'utilisateur
    db.setPassword("t");//inserer mot de passe de cet utilisateur

    if (db.open())
    {
        test=true;
        // Positionner le schéma courant pour éviter d'avoir à préfixer TOUHA.
        QSqlQuery q(db);
        if (!q.exec("ALTER SESSION SET CURRENT_SCHEMA=TOUHA")) {
            qDebug() << "ALTER SESSION SET CURRENT_SCHEMA failed:" << q.lastError().text();
        }
    }





    return  test;
}
