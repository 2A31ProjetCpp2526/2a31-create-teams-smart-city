#ifndef VEHICULECRUD_H
#define VEHICULECRUD_H

#include <QString>
#include <QSqlQueryModel>

class VehiculeCRUD
{
public:
    // Constructeurs
    VehiculeCRUD();
    VehiculeCRUD(int id, QString type, QString matricule, QString capacite,
                 QString itineraire, QString heureDepart, QString heureArrivee,
                 QString statut, QString zone, int idHabitant);

    // Attributs
    int id;
    QString type;
    QString matricule;
    QString capacite;
    QString itineraire;
    QString heureDepart;
    QString heureArrivee;
    QString statut;
    QString zone;
    int idHabitant;

    // Méthodes CRUD
    bool validerChamps(QString &messageErreur);
    bool ajouter();
    bool modifier(int id);
    bool supprimer(int id);
    QSqlQueryModel* afficher(QObject *parent = nullptr);
    QSqlQueryModel* rechercher(const QString &text, QObject *parent = nullptr);
};

#endif // VEHICULECRUD_H
