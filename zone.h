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

public:
    Zone();
    Zone(QString, QString, QString, double, QString, QString, QString);

    // --- Getters ---
    QString getId();
    QString getNom();
    QString getType();
    double getSuperficie();
    QString getLocalisation();
    QString getResponsable();
    QString getEtat();

    // --- CRUD Operations ---
    bool ajouter();
    QSqlQueryModel* afficher();
    bool supprimer(QString);
    bool modifier(QString);

    // --- For debugging ---
    QSqlError lastError();
private:
    QSqlError m_lastError;
};
// =======================================================
// CLASSES METIER AVANCÉES
// =======================================================

#include <QMap>
#include <QPair>
#include <QList>
#include <QSqlQuery>

class ZoneMetier
{
public:
    // Calcul de la superficie totale
    static double calculerSuperficieTotale();

    // Récupère (Responsable, Localisation)
    static QList<QPair<QString, QString>> responsablesEtLocalisations();

    // Statistiques sur l’état des zones
    static QMap<QString, int> statistiquesEtat();

    // Recommandation simple : zones à rénover
    static QList<QString> zonesARenover(double seuilSuperficie, QString etatCible);
};

#endif // ZONE_H
