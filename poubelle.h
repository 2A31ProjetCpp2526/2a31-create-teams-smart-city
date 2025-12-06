#ifndef POUBELLE_H
#define POUBELLE_H

#include <QString>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSqlError>
#include <QDebug>

class Poubelle
{
private:
    int id_bin;
    int id_zone;
    double capacite;
    QString status;

public:
    Poubelle();
    Poubelle(int id_bin, int id_zone, double capacite, QString status);

    bool ajouter();
    bool supprimer(int id_bin);
    bool modifier(int id_bin);
    QSqlQueryModel* afficher();
    QSqlQueryModel* trier(int index);
    QSqlQueryModel* rechercher(int id);
};

#endif // POUBELLE_H
