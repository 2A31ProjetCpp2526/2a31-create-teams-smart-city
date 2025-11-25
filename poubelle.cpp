#include "poubelle.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSqlError>
#include <QDebug>

Poubelle::Poubelle() {}

Poubelle::Poubelle(int id_bin, int id_zone, double capacite, QString status)
{
    this->id_bin = id_bin;
    this->id_zone = id_zone;
    this->capacite = capacite;
    this->status = status;
}

bool Poubelle::ajouter()
{
    QSqlDatabase db = QSqlDatabase::database();
    if (!db.isOpen()) {
        qDebug() << "❌ Database not open!";
        return false;
    }

    QSqlQuery query(db);
    query.prepare("INSERT INTO GESPUB (ID_BIN, CAPACITE, STATUS, ID_ZONE) "
                  "VALUES (:id_bin, :capacite, :status, :id_zone)");
    query.bindValue(":id_bin", id_bin);
    query.bindValue(":id_zone", id_zone);
    query.bindValue(":capacite", capacite);
    query.bindValue(":status", status);

    if (query.exec()) {
        qDebug() << "✅ Poubelle ajoutée avec succès";
        return true;
    } else {
        qDebug() << "❌ Erreur d'ajout poubelle:" << query.lastError().text();
        return false;
    }
}

bool Poubelle::supprimer(int id_bin)
{
    QSqlQuery query;
    query.prepare("DELETE FROM GESPUB WHERE ID_BIN = :id_bin");
    query.bindValue(":id_bin", id_bin);

    if (query.exec()) {
        qDebug() << "✅ Poubelle supprimée avec succès";
        return true;
    } else {
        qDebug() << "❌ Erreur suppression poubelle:" << query.lastError().text();
        return false;
    }
}

bool Poubelle::modifier(int id_bin)
{
    QSqlQuery query;
    query.prepare(
        "UPDATE GESPUB "
        "SET CAPACITE = :capacite, STATUS = :status, ID_ZONE = :id_zone "
        "WHERE ID_BIN = :id_bin"
        );
    query.bindValue(":id_zone", id_zone);
    query.bindValue(":capacite", capacite);
    query.bindValue(":status", status);
    query.bindValue(":id_bin", id_bin);

    if (query.exec()) {
        qDebug() << "✅ Poubelle modifiée avec succès";
        return true;
    } else {
        qDebug() << "❌ Erreur modification poubelle:" << query.lastError().text();
        return false;
    }
}

QSqlQueryModel* Poubelle::afficher()
{
    QSqlQueryModel *model = new QSqlQueryModel();
    model->setQuery("SELECT * FROM GESPUB");

    if (model->lastError().isValid())
        qDebug() << "❌ Erreur affichage:" << model->lastError().text();

    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID_BIN"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("CAPACITE"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("STATUS"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("ID_ZONE"));

    return model;
}

QSqlQueryModel* Poubelle::rechercher(int id)
{
    QSqlQueryModel *model = new QSqlQueryModel();
    model->setQuery("SELECT * FROM GESPUB WHERE ID_BIN = " + QString::number(id));

    if (model->lastError().isValid())
        qDebug() << "❌ Erreur recherche:" << model->lastError().text();

    return model;
}

QSqlQueryModel* Poubelle::trier(int index)
{
    QSqlQueryModel *model = new QSqlQueryModel();
    QString queryStr;

    switch(index) {
    case 0: queryStr = "SELECT * FROM GESPUB ORDER BY ID_BIN"; break;
    case 1: queryStr = "SELECT * FROM GESPUB ORDER BY CAPACITE"; break;
    case 2: queryStr = "SELECT * FROM GESPUB ORDER BY STATUS"; break;
    default: queryStr = "SELECT * FROM GESPUB"; break;
    }

    model->setQuery(queryStr);

    if (model->lastError().isValid())
        qDebug() << "❌ Erreur tri:" << model->lastError().text();

    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID_BIN"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("CAPACITE"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("STATUS"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("ID_ZONE"));

    return model;
}
