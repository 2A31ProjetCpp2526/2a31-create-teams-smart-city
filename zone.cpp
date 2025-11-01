#include "zone.h"

Zone::Zone() {}

Zone::Zone(int id, int population, double x, double y, double l, double h)
{
    this->id_zone = id;
    this->population = population;
    this->x = x;
    this->y = y;
    this->l = l;
    this->h = h;

    this->surface = l * h;
    this->densite = (surface > 0) ? (population / surface) : 0;
}

bool Zone::ajouter()
{
    QSqlQuery query;
    query.prepare("INSERT INTO GZONE (ID_ZONE, POPULATION, X, Y, L, H, SURFACE, DENSITE) "
                  "VALUES (:id_zone, :population, :x, :y, :l, :h, :surface, :densite)");
    query.bindValue(":id_zone", id_zone);
    query.bindValue(":population", population);
    query.bindValue(":x", x);
    query.bindValue(":y", y);
    query.bindValue(":l", l);
    query.bindValue(":h", h);
    query.bindValue(":surface", surface);
    query.bindValue(":densite", densite);

    if (query.exec()) {
        qDebug() << "✅ Zone ajoutée";
        return true;
    } else {
        qDebug() << "❌ Erreur ajout zone:" << query.lastError().text();
        return false;
    }
}

bool Zone::supprimer(int id)
{
    QSqlQuery query;
    query.prepare("DELETE FROM GZONE WHERE ID_ZONE = :id_zone");
    query.bindValue(":id_zone", id);

    if (query.exec()) {
        qDebug() << "✅ Zone supprimée";
        return true;
    } else {
        qDebug() << "❌ Erreur suppression zone:" << query.lastError().text();
        return false;
    }
}

QSqlQueryModel* Zone::afficher()
{
    QSqlQueryModel *model = new QSqlQueryModel();
    model->setQuery("SELECT * FROM GZONE");
    return model;
}

bool Zone::modifier(int id)
{
    QSqlQuery query;
    query.prepare("UPDATE GZONE SET POPULATION=:population, X=:x, Y=:y, L=:l, H=:h, "
                  "SURFACE=:surface, DENSITE=:densite WHERE ID_ZONE=:id_zone");
    query.bindValue(":population", population);
    query.bindValue(":x", x);
    query.bindValue(":y", y);
    query.bindValue(":l", l);
    query.bindValue(":h", h);
    query.bindValue(":surface", surface);
    query.bindValue(":densite", densite);
    query.bindValue(":id_zone", id);

    return query.exec();
}
