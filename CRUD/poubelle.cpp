#include "poubelle.h"

Poubelle::Poubelle() {}

Poubelle::Poubelle(int id_bin, int id_zone, int capacite, QString status)
{
    this->id_bin = id_bin;
    this->id_zone = id_zone;
    this->capacite = capacite;
    this->status = status;
}

bool Poubelle::ajouter()
{
    QSqlQuery query;
    query.prepare("INSERT INTO GESPUB (ID_BIN, ID_ZONE, CAPACITE, STATUS) "
                  "VALUES (:id_bin, :id_zone, :capacite, :status)");
    query.bindValue(":id_bin", id_bin);
    query.bindValue(":id_zone", id_zone);
    query.bindValue(":capacite", capacite);
    query.bindValue(":status", status);

    if (query.exec()) {
        qDebug() << "✅ Poubelle ajoutée";
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
        qDebug() << "✅ Poubelle supprimée";
        return true;
    } else {
        qDebug() << "❌ Erreur suppression poubelle:" << query.lastError().text();
        return false;
    }
}

QSqlQueryModel* Poubelle::afficher()
{
    QSqlQueryModel *model = new QSqlQueryModel();
    model->setQuery("SELECT * FROM GESPUB");
    return model;
}
bool Poubelle::modifier(int id_bin)
{
    QSqlQuery query;
    query.prepare("UPDATE GESPUB SET ID_ZONE=:id_zone, CAPACITE=:capacite, STATUS=:status "
                  "WHERE ID_BIN=:id_bin");
    query.bindValue(":id_zone", id_zone);
    query.bindValue(":capacite", capacite);
    query.bindValue(":status", status);
    query.bindValue(":id_bin", id_bin);

    return query.exec();
}
