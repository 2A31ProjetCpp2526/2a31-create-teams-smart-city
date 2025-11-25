#ifndef ZONE_H
#define ZONE_H

#include <QString>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QSqlQueryModel>

class Zone
{
public:
    Zone();
    Zone(int id, int population, double x, double y, double l, double h);
    bool modifier(int id);
    bool ajouter();          // Create
    bool supprimer(int id);  // Delete
    QSqlQueryModel* afficher(); // Read / Display all zones

private:
    int id_zone;
    int population;
    double x, y, l, h;
    double surface;
    double densite;
};

#endif // ZONE_H
