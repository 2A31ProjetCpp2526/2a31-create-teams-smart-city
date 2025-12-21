#include "vehiculecrud.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QRegularExpression>
#include <QDebug>

// ================= VehiculeCRUD =================

VehiculeCRUD::VehiculeCRUD() : id(0), idHabitant(0) {}

VehiculeCRUD::VehiculeCRUD(int id, QString type, QString matricule, QString capacite,
                           QString itineraire, QString heureDepart, QString heureArrivee,
                           QString statut, QString zone, int idHabitant)
    : id(id), type(type), matricule(matricule), capacite(capacite),
    itineraire(itineraire), heureDepart(heureDepart), heureArrivee(heureArrivee),
    statut(statut), zone(zone), idHabitant(idHabitant) {}

// Validation des champs
bool VehiculeCRUD::validerChamps(QString &messageErreur)
{
    QStringList erreurs;

    if (matricule.trimmed().isEmpty())
        erreurs << "Matricule obligatoire.";
    else if (!QRegularExpression("^[A-Z0-9-]{4,15}$").match(matricule).hasMatch())
        erreurs << "Matricule invalide (ex: 123-TUN-456).";

    if (type.trimmed().isEmpty()) erreurs << "Type de véhicule obligatoire.";
    if (statut.trimmed().isEmpty()) erreurs << "Statut du véhicule obligatoire.";

    if (heureDepart.trimmed().isEmpty())
        erreurs << "Heure de départ obligatoire.";
    else if (!QRegularExpression("^\\d+$").match(heureDepart).hasMatch())
        erreurs << "Heure de départ doit contenir uniquement des chiffres.";

    if (heureArrivee.trimmed().isEmpty())
        erreurs << "Heure d'arrivée obligatoire.";
    else if (!QRegularExpression("^\\d+$").match(heureArrivee).hasMatch())
        erreurs << "Heure d'arrivée doit contenir uniquement des chiffres.";

    if (!erreurs.isEmpty()) {
        messageErreur = erreurs.join("\n");
        return false;
    }
    return true;
}

// Ajouter un véhicule
bool VehiculeCRUD::ajouter()
{
    QString erreur;
    if (!validerChamps(erreur)) {
        QMessageBox::warning(nullptr, "Erreur de validation", erreur);
        return false;
    }

    QSqlQuery query;
    query.prepare("INSERT INTO VEHICULE (TYPE_VEHICULE, MATRICULE, CAPACITE, ITINERAIRE, "
                  "HEURE_DEPART, HEURE_ARRIVEE, STATUT, ZONE, ID_HABITANT) "
                  "VALUES (:type, :mat, :cap, :itin, :hdep, :harr, :stat, :zone, :idh)");
    query.bindValue(":type", type);
    query.bindValue(":mat", matricule);
    query.bindValue(":cap", capacite);
    query.bindValue(":itin", itineraire);
    query.bindValue(":hdep", heureDepart);
    query.bindValue(":harr", heureArrivee);
    query.bindValue(":stat", statut);
    query.bindValue(":zone", zone);
    query.bindValue(":idh", idHabitant);

    if (!query.exec()) {
        QMessageBox::critical(nullptr, "Erreur SQL", query.lastError().text());
        qDebug() << "Erreur SQL (ajouter):" << query.lastError().text();
        return false;
    }

    return true;
}

// Modifier un véhicule
bool VehiculeCRUD::modifier(int id)
{
    QString erreur;
    if (!validerChamps(erreur)) {
        QMessageBox::warning(nullptr, "Erreur de validation", erreur);
        return false;
    }

    QSqlQuery query;
    query.prepare("UPDATE VEHICULE SET TYPE_VEHICULE=:type, MATRICULE=:mat, CAPACITE=:cap, "
                  "ITINERAIRE=:itin, HEURE_DEPART=:hdep, HEURE_ARRIVEE=:harr, "
                  "STATUT=:stat, ZONE=:zone, ID_HABITANT=:idh WHERE ID_VEHICULE=:id");
    query.bindValue(":id", id);
    query.bindValue(":type", type);
    query.bindValue(":mat", matricule);
    query.bindValue(":cap", capacite);
    query.bindValue(":itin", itineraire);
    query.bindValue(":hdep", heureDepart);
    query.bindValue(":harr", heureArrivee);
    query.bindValue(":stat", statut);
    query.bindValue(":zone", zone);
    query.bindValue(":idh", idHabitant);

    if (!query.exec()) {
        QMessageBox::critical(nullptr, "Erreur SQL", query.lastError().text());
        return false;
    }

    return true;
}

// Supprimer un véhicule
bool VehiculeCRUD::supprimer(int id)
{
    QSqlQuery query;
    query.prepare("DELETE FROM VEHICULE WHERE ID_VEHICULE = :id");
    query.bindValue(":id", id);

    if (!query.exec()) {
        QMessageBox::critical(nullptr, "Erreur SQL", query.lastError().text());
        return false;
    }
    return true;
}

// Afficher tous les véhicules
QSqlQueryModel* VehiculeCRUD::afficher(QObject *parent)
{
    QSqlQueryModel *model = new QSqlQueryModel(parent);
    model->setQuery("SELECT ID_VEHICULE, TYPE_VEHICULE, MATRICULE, CAPACITE, ITINERAIRE, "
                    "HEURE_DEPART, HEURE_ARRIVEE, STATUT, ZONE, ID_HABITANT FROM VEHICULE");
    return model;
}

// Rechercher véhicule
QSqlQueryModel* VehiculeCRUD::rechercher(const QString &text, QObject *parent)
{
    QSqlQueryModel *model = new QSqlQueryModel(parent);
    model->setQuery("SELECT * FROM VEHICULE WHERE "
                    "LOWER(MATRICULE) LIKE LOWER('%" + text + "%') OR "
                             "LOWER(TYPE_VEHICULE) LIKE LOWER('%" + text + "%') OR "
                             "LOWER(STATUT) LIKE LOWER('%" + text + "%')");
    return model;
}
