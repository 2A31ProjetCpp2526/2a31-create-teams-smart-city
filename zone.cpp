#include "zone.h"
#include <QSqlQuery>
#include <QVariant>
#include <QDebug>

Zone::Zone() {}

Zone::Zone(QString id, QString n, QString t, double s, QString loc, QString resp, QString e)
    : id_zone(id), nom(n), type(t), superficie(s), localisation(loc), responsable(resp), etat(e) {}

// === Getters ===
QString Zone::getId() { return id_zone; }
QString Zone::getNom() { return nom; }
QString Zone::getType() { return type; }
double Zone::getSuperficie() { return superficie; }
QString Zone::getLocalisation() { return localisation; }
QString Zone::getResponsable() { return responsable; }
QString Zone::getEtat() { return etat; }

// === Ajouter ===
bool Zone::ajouter()
{
    QSqlQuery query;
    query.prepare("INSERT INTO ZONES_VERTES (ID_ZONE, NOM, TYPE, SUPERFICIE, LOCALISATION, RESPONSABLE, ETAT) "
                  "VALUES (:id, :nom, :type, :sup, :loc, :resp, :etat)");
    query.bindValue(":id", id_zone);
    query.bindValue(":nom", nom);
    query.bindValue(":type", type);
    query.bindValue(":sup", superficie);
    query.bindValue(":loc", localisation);
    query.bindValue(":resp", responsable);
    query.bindValue(":etat", etat);

    if (!query.exec()) {
        m_lastError = query.lastError();
        qDebug() << "Erreur ajout:" << m_lastError.text();
        return false;
    }
    return true;
}

// === Afficher ===
QSqlQueryModel* Zone::afficher()
{
    QSqlQueryModel *model = new QSqlQueryModel();
    model->setQuery("SELECT ID_ZONE, NOM, TYPE, SUPERFICIE, LOCALISATION, RESPONSABLE, ETAT FROM ZONES_VERTES");

    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID Zone"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Nom"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Type"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Superficie"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Localisation"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Responsable"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("État"));

    return model;
}

// === Supprimer ===
bool Zone::supprimer(QString id)
{
    QSqlQuery query;
    query.prepare("DELETE FROM ZONES_VERTES WHERE ID_ZONE = :id");
    query.bindValue(":id", id);

    if (!query.exec()) {
        m_lastError = query.lastError();
        qDebug() << "Erreur suppression:" << m_lastError.text();
        return false;
    }
    return true;
}

// === Modifier ===
bool Zone::modifier(QString id)
{
    QSqlQuery query;
    query.prepare("UPDATE ZONES_VERTES SET NOM = :nom, TYPE = :type, SUPERFICIE = :sup, "
                  "LOCALISATION = :loc, RESPONSABLE = :resp, ETAT = :etat WHERE ID_ZONE = :id");
    query.bindValue(":id", id);
    query.bindValue(":nom", nom);
    query.bindValue(":type", type);
    query.bindValue(":sup", superficie);
    query.bindValue(":loc", localisation);
    query.bindValue(":resp", responsable);
    query.bindValue(":etat", etat);

    if (!query.exec()) {
        m_lastError = query.lastError();
        qDebug() << "Erreur modification:" << m_lastError.text();
        return false;
    }
    return true;
}

// === Error getter ===
QSqlError Zone::lastError()
{
    return m_lastError;
}

