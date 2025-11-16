#ifndef HABITANTCRUD_H
#define HABITANTCRUD_H

#include <QString>
#include <QSqlQueryModel>
#include <QRegularExpression>

class HabitantCRUD
{
private:
    int id;
    QString nom, prenom, adresse, zone, situation, contact, statut, besoins, satisfaction;

public:
    HabitantCRUD();
    HabitantCRUD(int, QString, QString, QString, QString, QString, QString, QString, QString, QString);

    // CRUD
    bool ajouter();
    bool modifier(int id);
    bool supprimer(int id);
    static QSqlQueryModel* afficher(QObject *parent = nullptr);

    // Validation
    bool validerChamps(QString &messageErreur);
};

#endif // HABITANTCRUD_H
