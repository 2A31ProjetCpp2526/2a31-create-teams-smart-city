#ifndef DEMANDE_H
#define DEMANDE_H

#include <QObject>
#include <QSqlDatabase>
#include <QList>
#include <QDateTime>

struct DemandeData {
    QString zoneId;
    QString etat;
    QDateTime dateDemande;
};

class Demande : public QObject
{
    Q_OBJECT
public:
    explicit Demande(QSqlDatabase &database, QObject *parent = nullptr);

    // CRUD methods
    bool creerDemande(const QString &zoneId, const QString &etat, const QDateTime &date);
    QList<DemandeData> lireToutesDemandes();
    QList<DemandeData> lireDemandesParZone(const QString &zoneId);
    bool modifierEtatDemande(const QString &zoneId, const QDateTime &date, const QString &nouvelEtat);
    bool supprimerDemande(const QString &zoneId, const QDateTime &date);

    bool estConnecte() const;

signals:
    void erreurOracle(const QString &message);

private:
    QSqlDatabase &db;
};

#endif // DEMANDE_H


