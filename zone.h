#ifndef ZONE_H
#define ZONE_H

#include <QString>
#include <QSqlQueryModel>
#include <QSqlError>

class Zone
{
private:
    QString id_zone;
    QString nom;
    QString type;
    double superficie;
    QString localisation;
    QString responsable;
    QString etat;

    double pluie; // New attribute for PLUIE

    QSqlError m_lastError;

public:
    // === Constructors ===
    Zone();
    Zone(QString id, QString n, QString t, double s, QString loc, QString resp, QString e, double p = 0.0);
    bool setPluieAllZones(double value);

    // === Getters ===
    QString getId();
    QString getNom();
    QString getType();
    double getSuperficie();
    QString getLocalisation();
    QString getResponsable();
    QString getEtat();
    double getPluie(); // Getter for PLUIE

    // === Database operations ===
    bool ajouter();
    QSqlQueryModel* afficher();
    bool supprimer(QString id);
    bool modifier(QString id);

    // === Error getter ===
    QSqlError lastError();

    // === Advanced queries ===
    QSqlQueryModel* afficherTrie(int colonne, bool asc = true);
    QSqlQueryModel* rechercher(const QString &mot);
};

#endif // ZONE_H

