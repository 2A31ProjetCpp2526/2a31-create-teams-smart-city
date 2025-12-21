#include"connection.h"
#include "Demande.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

Demande::Demande(QSqlDatabase &database, QObject *parent)
    : QObject(parent), db(database)
{
}

bool Demande::estConnecte() const
{
    return db.isOpen();
}

/* =========================
 * CREATE
 * ========================= */
bool Demande::creerDemande(const QString &zoneId, const QString &etat, const QDateTime &date)
{
    if (!estConnecte()) return false;

    QSqlQuery query(db);
    query.prepare(
        "INSERT INTO SAIM.DEMANDE1 (ZONE_ID, ETAT, DATE_DEMANDE) "
        "VALUES (:zone_id, :etat, :date_demande)"
        );

    query.bindValue(":zone_id", zoneId);
    query.bindValue(":etat", etat);
    query.bindValue(":date_demande", date);

    if (!query.exec()) {
        emit erreurOracle(query.lastError().text());
        return false;
    }

    return true;
}

/* =========================
 * READ – toutes les demandes
 * ========================= */
QList<DemandeData> Demande::lireToutesDemandes()
{
    QList<DemandeData> liste;
    QSqlQuery query(db);
    query.exec(
        "SELECT ZONE_ID, ETAT, DATE_DEMANDE "
        "FROM SAIM.DEMANDE1 "
        "ORDER BY DATE_DEMANDE DESC"
        );

    while (query.next()) {
        DemandeData d;
        d.zoneId = query.value(0).toString();
        d.etat = query.value(1).toString();
        d.dateDemande = query.value(2).toDateTime();
        liste.append(d);
    }

    return liste;
}

/* =========================
 * READ – par zone
 * ========================= */
QList<DemandeData> Demande::lireDemandesParZone(const QString &zoneId)
{
    QList<DemandeData> liste;
    QSqlQuery query(db);
    query.prepare(
        "SELECT ZONE_ID, ETAT, DATE_DEMANDE "
        "FROM SAIM.DEMANDE1 "
        "WHERE ZONE_ID = :zone_id"
        );
    query.bindValue(":zone_id", zoneId);
    query.exec();

    while (query.next()) {
        DemandeData d;
        d.zoneId = query.value(0).toString();
        d.etat = query.value(1).toString();
        d.dateDemande = query.value(2).toDateTime();
        liste.append(d);
    }

    return liste;
}

/* =========================
 * UPDATE
 * ========================= */
bool Demande::modifierEtatDemande(const QString &zoneId, const QDateTime &date, const QString &nouvelEtat)
{
    QSqlQuery query(db);
    query.prepare(
        "UPDATE SAIM.DEMANDE1 "
        "SET ETAT = :etat "
        "WHERE ZONE_ID = :zone_id "
        "AND DATE_DEMANDE = :date_demande"
        );

    query.bindValue(":etat", nouvelEtat);
    query.bindValue(":zone_id", zoneId);
    query.bindValue(":date_demande", date);

    if (!query.exec()) {
        emit erreurOracle(query.lastError().text());
        return false;
    }

    return query.numRowsAffected() > 0;
}

/* =========================
 * DELETE
 * ========================= */
bool Demande::supprimerDemande(const QString &zoneId, const QDateTime &date)
{
    QSqlQuery query(db);
    query.prepare(
        "DELETE FROM SAIM.DEMANDE1 "
        "WHERE ZONE_ID = :zone_id "
        "AND DATE_DEMANDE = :date_demande"
        );

    query.bindValue(":zone_id", zoneId);
    query.bindValue(":date_demande", date);

    if (!query.exec()) {
        emit erreurOracle(query.lastError().text());
        return false;
    }

    return query.numRowsAffected() > 0;
}
