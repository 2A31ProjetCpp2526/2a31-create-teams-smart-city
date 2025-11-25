#include "habitatcrud.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QDebug>
#include <QRegularExpression>

HabitatCRUD::HabitatCRUD()
    : id_habitat(0), surface(0.0), nb_personne(0), id_zone(0)
{}

HabitatCRUD::HabitatCRUD(int id, QString type, double surf, QString etat,
                         QString adr, int nbp, int idz)
    : id_habitat(id), type_logement(type), surface(surf),
    etat(etat), adresse(adr), nb_personne(nbp), id_zone(idz)
{}

bool HabitatCRUD::validerChamps(QString &messageErreur)
{
    if (id_habitat <= 0) {
        messageErreur = "ID_HABITAT doit être un entier positif.";
        return false;
    }
    if (type_logement.trimmed().isEmpty()) {
        messageErreur = "Le type de logement est obligatoire.";
        return false;
    }
    if (surface <= 0) {
        messageErreur = "La surface doit être supérieure à 0.";
        return false;
    }
    if (adresse.trimmed().length() < 3) {
        messageErreur = "Adresse trop courte.";
        return false;
    }
    if (nb_personne < 0) {
        messageErreur = "Nombre de personnes invalide.";
        return false;
    }
    if (id_zone <= 0) {
        messageErreur = "ID_ZONE doit être un entier positif.";
        return false;
    }
    return true;
}

// === AJOUTER ===
bool HabitatCRUD::ajouter()
{
    QSqlQuery query;

    QSqlQuery check;
    check.prepare("SELECT 1 FROM HABITAT WHERE ID_HABITAT = :id");
    check.bindValue(":id", id_habitat);
    if (!check.exec()) {
        QMessageBox::critical(nullptr, "Erreur SQL", check.lastError().text());
        return false;
    }
    if (check.next()) {
        QMessageBox::warning(nullptr, "Doublon", "Cet ID existe déjà.");
        return false;
    }

    query.prepare("INSERT INTO HABITAT (ID_HABITAT, TYPE_LOGEMENT, SURFACE, ETAT, "
                  "ADRESSE, NB_PERSONNE, ID_ZONE) "
                  "VALUES (:id, :type, :surface, :etat, :adr, :nbp, :idz)");

    query.bindValue(":id", id_habitat);
    query.bindValue(":type", type_logement);
    query.bindValue(":surface", surface);
    query.bindValue(":etat", etat);
    query.bindValue(":adr", adresse);
    query.bindValue(":nbp", nb_personne);
    query.bindValue(":idz", id_zone);

    if (!query.exec()) {
        QMessageBox::critical(nullptr, "Erreur SQL", query.lastError().text());
        qDebug() << "Erreur ajout:" << query.lastError().text();
        return false;
    }

    QMessageBox::information(nullptr, "Succès", "Habitat ajouté avec succès !");
    return true;
}

// === AFFICHER ===
QSqlQueryModel* HabitatCRUD::afficher(QObject *parent)
{
    QSqlQueryModel *model = new QSqlQueryModel(parent);
    model->setQuery("SELECT ID_HABITAT, TYPE_LOGEMENT, SURFACE, ETAT, "
                    "ADRESSE, NB_PERSONNE, ID_ZONE FROM HABITAT ORDER BY ID_HABITAT");

    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Type"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Surface"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("État"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Adresse"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Nb Personnes"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("ID Zone"));

    return model;
}

// === SUPPRIMER ===
bool HabitatCRUD::supprimer(int id)
{
    QSqlQuery query;
    query.prepare("DELETE FROM HABITAT WHERE ID_HABITAT = :id");
    query.bindValue(":id", id);

    if (!query.exec()) {
        QMessageBox::critical(nullptr, "Erreur SQL", query.lastError().text());
        return false;
    }

    QMessageBox::information(nullptr, "Succès", "Habitat supprimé avec succès !");
    return true;
}

// === MODIFIER ===
bool HabitatCRUD::modifier(int id)
{
    QString erreur;
    if (!validerChamps(erreur)) {
        QMessageBox::warning(nullptr, "Erreur de validation", erreur);
        return false;
    }

    QSqlQuery query;
    query.prepare("UPDATE HABITAT SET TYPE_LOGEMENT = :type, SURFACE = :surface, "
                  "ETAT = :etat, ADRESSE = :adr, NB_PERSONNE = :nbp, ID_ZONE = :idz "
                  "WHERE ID_HABITAT = :id");

    query.bindValue(":type", type_logement);
    query.bindValue(":surface", surface);
    query.bindValue(":etat", etat);
    query.bindValue(":adr", adresse);
    query.bindValue(":nbp", nb_personne);
    query.bindValue(":idz", id_zone);
    query.bindValue(":id", id);

    if (!query.exec()) {
        QMessageBox::critical(nullptr, "Erreur SQL", query.lastError().text());
        return false;
    }

    QMessageBox::information(nullptr, "Succès", "Habitat modifié avec succès !");
    return true;
}
