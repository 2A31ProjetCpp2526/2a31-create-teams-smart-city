#ifndef PERSONNEL_H
#define PERSONNEL_H

#include <QString>
#include <QSqlQuery>
#include <QSqlQueryModel>

class personnel
{
public:
    personnel();
    personnel(QString, QString, QString, QString, QString);

    bool ajouter();
    QSqlQueryModel *afficher();
    bool supprimer(QString);
    bool modifier();

private:
    QString ID_PERSONNEL, NOM, PRENOM, COMPETENCES, ZONE_AFFECTATION;
};

#endif // PERSONNEL_H
