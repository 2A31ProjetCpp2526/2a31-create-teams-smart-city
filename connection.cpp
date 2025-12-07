#include "connection.h"
#include <QDebug>

Connection::Connection()
{
}

bool Connection::createconnect()
{
    bool test = false;
    // Utilise QODBC et la source de données nommée "Proje_2A2526"
    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
    db.setDatabaseName("city");//inserer le nom de la source de données
    db.setUserName("imen");//inserer nom de l'utilisateur
    db.setPassword("admin123");//inserer mot de passe de cet utilisateur

    if (db.open()) {
        test = true;
        qDebug() << "Connected to ODBC DSN Proje_2A2526";
    } else {
        qDebug() << "DB open error:" << db.lastError().text();
    }
    return test;
}
