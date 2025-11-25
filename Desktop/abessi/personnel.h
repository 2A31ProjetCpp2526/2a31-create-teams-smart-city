#ifndef PERSONNEL_H
#define PERSONNEL_H

#include <QString>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QSqlQueryModel>
#include <QMessageBox>

class personnel
{
public:
    personnel();
    personnel(QString ID_PERSONNEL, QString NOM, QString PRENOM, QString COMPETENCES, QString ZONE_AFFECTATION);
    
    // Méthodes de la classe
    bool ajouter();
    bool supprimer(QString ID);
    QSqlQueryModel* afficher();
    bool modifier();

    // Getters et Setters
    QString getID_PERSONNEL() const { return ID_PERSONNEL; }
    QString getNOM() const { return NOM; }
    QString getPRENOM() const { return PRENOM; }
    QString getCOMPETENCES() const { return COMPETENCES; }
    QString getZONE_AFFECTATION() const { return ZONE_AFFECTATION; }

    void setID_PERSONNEL(const QString &id) { ID_PERSONNEL = id; }
    void setNOM(const QString &nom) { NOM = nom; }
    void setPRENOM(const QString &prenom) { PRENOM = prenom; }
    void setCOMPETENCES(const QString &competences) { COMPETENCES = competences; }
    void setZONE_AFFECTATION(const QString &zone) { ZONE_AFFECTATION = zone; }

private:
    QString ID_PERSONNEL;
    QString NOM;
    QString PRENOM;
    QString COMPETENCES;
    QString ZONE_AFFECTATION;
};

#endif // PERSONNEL_H
