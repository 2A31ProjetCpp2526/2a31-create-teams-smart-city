#include "connection.h"

Connection::Connection()
{

}

bool Connection::createconnect()
{bool test=false;
    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
    db.setDatabaseName("Proje_2A2526");//inserer le nom de la source de données
    db.setUserName("mahdy");//inserer nom de l'utilisateur
    db.setPassword("mahdy25");//inserer mot de passe de cet utilisateur

    if (db.open())
        test=true;





    return  test;
}
