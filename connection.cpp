#include "connection.h"

Connection::Connection()
{

}

bool Connection::createconnect()
{bool test=false;
    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
    db.setDatabaseName("projet_2A2526");//inserer le nom de la source de données
    db.setUserName("etudiant2A");//inserer nom de l'utilisateur
    db.setPassword("projetcpp2526");//inserer mot de passe de cet utilisateur

    if (db.open())
        test=true;





    return  test;
}
