#include "vehiculecrud.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QDebug>
#include <QRegularExpression>

VehiculeCRUD::VehiculeCRUD() : id(0), idHabitant(0) {}

VehiculeCRUD::VehiculeCRUD(int id, QString type, QString matricule, QString capacite,
                           QString itineraire, QString heureDepart, QString heureArrivee,
                           QString statut, QString zone, int idHabitant)
    : id(id), type(type), matricule(matricule), capacite(capacite),
    itineraire(itineraire), heureDepart(heureDepart), heureArrivee(heureArrivee),
    statut(statut), zone(zone), idHabitant(idHabitant) {}

bool VehiculeCRUD::validerChamps(QString &messageErreur)
{
    if (matricule.trimmed().isEmpty()) {
        messageErreur = "Matricule obligatoire.";
        return false;
    }

    QRegularExpression rxMat("^[A-Z0-9-]{4,15}$");
    if (!rxMat.match(matricule).hasMatch()) {
        messageErreur = "Matricule invalide (ex: 123-TUN-456).";
        return false;
    }

    if (type.trimmed().isEmpty()) {
        messageErreur = "Type de véhicule obligatoire.";
        return false;
    }

    if (statut.trimmed().isEmpty()) {
        messageErreur = "Statut du véhicule obligatoire.";
        return false;
    }

    return true;
}

bool VehiculeCRUD::ajouter()
{
    QString erreur;
    if (!validerChamps(erreur)) {
        QMessageBox::warning(nullptr, "Erreur de validation", erreur);
        return false;
    }

    // 🟡 Vérifier que l'ID de l'habitant existe avant d'insérer
    QSqlQuery checkHabitant;
    checkHabitant.prepare("SELECT COUNT(*) FROM HABITANT WHERE ID_HABITANT = :idh");
    checkHabitant.bindValue(":idh", idHabitant);

    if (!checkHabitant.exec()) {
        QMessageBox::critical(nullptr, "Erreur SQL", checkHabitant.lastError().text());
        qDebug() << "Erreur vérification habitant:" << checkHabitant.lastError().text();
        return false;
    }

    checkHabitant.next();
    if (checkHabitant.value(0).toInt() == 0) {
        QMessageBox::warning(nullptr, "Habitant introuvable",
                             "L'ID_HABITANT spécifié n'existe pas dans la base !");
        return false;
    }

    // 🟩 Si tout est bon → exécuter l'insertion
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

    QMessageBox::information(nullptr, "Succès", "Véhicule ajouté avec succès !");
    return true;
}


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
        qDebug() << "Erreur SQL (modifier):" << query.lastError().text();
        return false;
    }

    QMessageBox::information(nullptr, "Succès", "Véhicule modifié avec succès !");
    return true;
}

bool VehiculeCRUD::supprimer(int id)
{
    QSqlQuery query;
    query.prepare("DELETE FROM VEHICULE WHERE ID_VEHICULE = :id");
    query.bindValue(":id", id);

    if (!query.exec()) {
        QMessageBox::critical(nullptr, "Erreur SQL", query.lastError().text());
        qDebug() << "Erreur SQL (supprimer):" << query.lastError().text();
        return false;
    }

    QMessageBox::information(nullptr, "Succès", "Véhicule supprimé avec succès !");
    return true;
}

QSqlQueryModel* VehiculeCRUD::afficher(QObject *parent)
{
    QSqlQueryModel *model = new QSqlQueryModel(parent);
    model->setQuery("SELECT ID_VEHICULE, TYPE_VEHICULE, MATRICULE, CAPACITE, ITINERAIRE, "
                    "HEURE_DEPART, HEURE_ARRIVEE, STATUT, ZONE, ID_HABITANT FROM VEHICULE");

    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Type"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Matricule"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Capacité"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Itinéraire"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Heure départ"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("Heure arrivée"));
    model->setHeaderData(7, Qt::Horizontal, QObject::tr("Statut"));
    model->setHeaderData(8, Qt::Horizontal, QObject::tr("Zone"));
    model->setHeaderData(9, Qt::Horizontal, QObject::tr("ID Habitant"));

    return model;
}

QSqlQueryModel* VehiculeCRUD::rechercher(const QString &text, QObject *parent)
{
    QSqlQueryModel *model = new QSqlQueryModel(parent);
    model->setQuery("SELECT * FROM VEHICULE WHERE "
                    "LOWER(MATRICULE) LIKE LOWER('%" + text + "%') OR "
                             "LOWER(TYPE_VEHICULE) LIKE LOWER('%" + text + "%') OR "
                             "LOWER(STATUT) LIKE LOWER('%" + text + "%')");

    return model;
}
