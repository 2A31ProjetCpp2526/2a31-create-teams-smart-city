#ifndef POUBELLE_H
#define POUBELLE_H

#include <QString>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QSqlQueryModel>

class Poubelle
{
public:
    Poubelle();
    Poubelle(int id_bin, int id_zone, int capacite, QString status);
    bool modifier(int id_bin);
    bool ajouter();           // Create
    bool supprimer(int id_bin); // Delete
    QSqlQueryModel* afficher(); // Read / Display all poubelles

private:
    int id_bin;
    int id_zone;
    int capacite;
    QString status;
};

#endif // POUBELLE_H
