#ifndef VEHICULECRUD_H
#define VEHICULECRUD_H

#include <QString>
#include <QSqlQueryModel>

class VehiculeCRUD
{
private:
    int id;
    QString type, matricule, capacite, itineraire;
    QString heureDepart, heureArrivee, statut, zone;
    int idHabitant;

public:
    VehiculeCRUD();
    VehiculeCRUD(int, QString, QString, QString, QString, QString, QString, QString, QString, int);

    // CRUD
    bool ajouter();
    bool modifier(int id);
    bool supprimer(int id);
    static QSqlQueryModel* afficher(QObject *parent = nullptr);
    static QSqlQueryModel* rechercher(const QString &text, QObject *parent = nullptr);

    // Validation
    bool validerChamps(QString &messageErreur);
};

#endif // VEHICULECRUD_H
