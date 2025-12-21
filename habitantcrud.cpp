#include "habitantcrud.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QDebug>

HabitantCRUD::HabitantCRUD() : id(0) {}
HabitantCRUD::HabitantCRUD(int id, QString nom, QString adresse)
    : id(id), nom(nom), adresse(adresse) {}

// --- Ajouter ---
bool HabitantCRUD::ajouter()
{
    QSqlQuery query;
    query.prepare("INSERT INTO HABITANT (ID_HA, NOM_HA, ADRESSE) "
                  "VALUES (:id, :nom, :adresse)");
    query.bindValue(":id", id);
    query.bindValue(":nom", nom);
    query.bindValue(":adresse", adresse);

    if (!query.exec()) {
        qDebug() << "Erreur ajout Habitant:" << query.lastError().text();
        QMessageBox::critical(nullptr, "Erreur SQL", query.lastError().text());
        return false;
    }

    QMessageBox::information(nullptr, "Succès", "Habitant ajouté !");
    return true;
}

// --- Afficher ---
QSqlQueryModel* HabitantCRUD::afficher(QObject *parent)
{
    QSqlQueryModel *model = new QSqlQueryModel(parent);
    model->setQuery("SELECT ID_HA, NOM_HA, ADRESSE FROM HABITANT ORDER BY ID_HA");

    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Nom"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Adresse"));

    return model;
}

// --- Supprimer ---
bool HabitantCRUD::supprimer(int id)
{
    QSqlQuery query;
    query.prepare("DELETE FROM HABITANT WHERE ID_HA = :id");
    query.bindValue(":id", id);

    if (!query.exec()) {
        QMessageBox::critical(nullptr, "Erreur SQL", query.lastError().text());
        return false;
    }

    QMessageBox::information(nullptr, "Succès", "Habitant supprimé !");
    return true;
}

// --- Modifier ---
bool HabitantCRUD::modifier(int id)
{
    QSqlQuery query;
    query.prepare("UPDATE HABITANT SET NOM_HA = :nom, ADRESSE = :adresse WHERE ID_HA = :id");
    query.bindValue(":nom", nom);
    query.bindValue(":adresse", adresse);
    query.bindValue(":id", id);

    if (!query.exec()) {
        QMessageBox::critical(nullptr, "Erreur SQL", query.lastError().text());
        return false;
    }

    QMessageBox::information(nullptr, "Succès", "Habitant modifié");
}
