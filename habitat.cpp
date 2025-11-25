#include "habitat.h"
#include "connexion.h"
#include <QSqlQuery>
#include <QDebug>
#include <QSqlError>

Habitat::Habitat() {}

Habitat::Habitat(int id, QString adresse, int nb_personnes, double surface,
                 QString type_logement, QString telephone)
{
    ID_HABITAT = id;
    ADRESSE = adresse;
    NB_PERSONNES = nb_personnes;
    SURFACE = surface;
    TYPE_LOGEMENT = type_logement;
    TELEPHONE_CONTACT = telephone;
}

// Getters
int Habitat::getID_HABITAT() const { return ID_HABITAT; }
QString Habitat::getADRESSE() const { return ADRESSE; }
int Habitat::getNB_PERSONNES() const { return NB_PERSONNES; }
double Habitat::getSURFACE() const { return SURFACE; }
QString Habitat::getTYPE_LOGEMENT() const { return TYPE_LOGEMENT; }
QString Habitat::getTELEPHONE_CONTACT() const { return TELEPHONE_CONTACT; }

// Ajouter
bool Habitat::ajouter()
{
    QSqlDatabase db = Connexion::createInstance().getDatabase();
    if (!db.isOpen()) return false;
    QSqlQuery query(db);
    query.prepare("INSERT INTO HABITAT (ID_HABITAT, ADRESSE, NB_PERSONNES, SURFACE, TYPE_LOGEMENT, TELEPHONE_CONTACT) "
                  "VALUES (:id, :adresse, :nb, :surface, :type, :tel)");
    query.bindValue(":id", ID_HABITAT);
    query.bindValue(":adresse", ADRESSE);
    query.bindValue(":nb", NB_PERSONNES);
    query.bindValue(":surface", SURFACE);
    query.bindValue(":type", TYPE_LOGEMENT);
    query.bindValue(":tel", TELEPHONE_CONTACT);
    return query.exec();
}

// Afficher
QSqlQueryModel* Habitat::afficher()
{
    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery("SELECT ID_HABITAT, ADRESSE, NB_PERSONNES, SURFACE, TYPE_LOGEMENT, TELEPHONE_CONTACT FROM HABITAT");
    return model;
}

// Supprimer
bool Habitat::supprimer(int id)
{
    QSqlQuery query;
    query.prepare("DELETE FROM HABITAT WHERE ID_HABITAT = :id");
    query.bindValue(":id", id);
    return query.exec();
}

// Modifier
bool Habitat::modifier()
{
    QSqlQuery query;
    query.prepare("UPDATE HABITAT SET ADRESSE=:adresse, NB_PERSONNES=:nb, SURFACE=:surface, TYPE_LOGEMENT=:type, TELEPHONE_CONTACT=:tel "
                  "WHERE ID_HABITAT=:id");
    query.bindValue(":id", ID_HABITAT);
    query.bindValue(":adresse", ADRESSE);
    query.bindValue(":nb", NB_PERSONNES);
    query.bindValue(":surface", SURFACE);
    query.bindValue(":type", TYPE_LOGEMENT);
    query.bindValue(":tel", TELEPHONE_CONTACT);
    return query.exec();
}
