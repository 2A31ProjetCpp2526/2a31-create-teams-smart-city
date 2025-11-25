#ifndef HABITATCRUD_H
#define HABITATCRUD_H

#include <QString>
#include <QSqlQueryModel>

class HabitatCRUD
{
private:
    int id_habitat;
    QString type_logement;
    double surface;
    QString etat;
    QString adresse;
    int nb_personne;
    int id_zone;

public:
    HabitatCRUD();
    HabitatCRUD(int, QString, double, QString, QString, int, int);

    // CRUD
    bool ajouter();
    bool modifier(int id);
    bool supprimer(int id);
    static QSqlQueryModel* afficher(QObject *parent = nullptr);

    // Validation
    bool validerChamps(QString &messageErreur);
};

#endif // HABITATCRUD_H
