#ifndef CONNEXION_H
#define CONNEXION_H

#include <QSqlDatabase>

class Connexion
{
private:
    QSqlDatabase db;

    Connexion();
    Connexion(const Connexion&) = delete;
    Connexion& operator=(const Connexion&) = delete;

public:
    ~Connexion();

    static Connexion& createInstance();

    bool ouvrirConnexion();
    void fermerConnexion();

    QSqlDatabase& getDatabase() { return db; } // pour exécuter les queries
};

#endif // CONNEXION_H
