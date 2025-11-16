#include "habitat.h"
#include "connexion.h"
#include <QDebug>
#include <QSqlQuery>
#include <QSqlError>

Habitat::Habitat() {}

Habitat::Habitat(int id, QString adresse, int nb_personnes, double surface, QString type_logement)
{
    ID_HABITAT = id;
    ADRESSE = adresse;
    NB_PERSONNES = nb_personnes;
    SURFACE = surface;
    TYPE_LOGEMENT = type_logement;
}

// Getters
int Habitat::getID_HABITAT() const { return ID_HABITAT; }
QString Habitat::getADRESSE() const { return ADRESSE; }
int Habitat::getNB_PERSONNES() const { return NB_PERSONNES; }
double Habitat::getSURFACE() const { return SURFACE; }
QString Habitat::getTYPE_LOGEMENT() const { return TYPE_LOGEMENT; }

// Ajouter
bool Habitat::ajouter()
{
    qDebug() << "=== DÉBUT Habitat::ajouter ===";

    QSqlDatabase db = Connexion::createInstance().getDatabase();
    if (!db.isOpen()) {
        qDebug() << "❌ Base de données fermée";
        return false;
    }

    if (!db.transaction()) {
        qDebug() << "❌ Erreur transaction:" << db.lastError().text();
        return false;
    }

    QSqlQuery query(db);

    if (!query.prepare("INSERT INTO HABITAT (ID_HABITAT, ADRESSE, NB_PERSONNES, SURFACE, TYPE_LOGEMENT) "
                       "VALUES (:id, :adresse, :nb, :surface, :type)")) {
        qDebug() << "❌ Erreur prepare ajouter:" << query.lastError().text();
        db.rollback();
        return false;
    }

    query.bindValue(":id", ID_HABITAT);
    query.bindValue(":adresse", ADRESSE);
    query.bindValue(":nb", NB_PERSONNES);
    query.bindValue(":surface", SURFACE);
    query.bindValue(":type", TYPE_LOGEMENT);

    if (!query.exec()) {
        qDebug() << "❌ Erreur exec ajouter:" << query.lastError().text();
        db.rollback();
        return false;
    }

    if (!db.commit()) {
        qDebug() << "❌ Erreur commit:" << db.lastError().text();
        return false;
    }

    qDebug() << "✓ Habitat H" << ID_HABITAT << "ajouté avec succès";
    qDebug() << "=== FIN Habitat::ajouter ===";
    return true;
}

// Afficher
QSqlQueryModel* Habitat::afficher()
{
    qDebug() << "=== DÉBUT Habitat::afficher ===";

    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery("SELECT ID_HABITAT, ADRESSE, NB_PERSONNES, SURFACE, TYPE_LOGEMENT FROM HABITAT ORDER BY ID_HABITAT",
                    Connexion::createInstance().getDatabase());

    if (model->lastError().isValid()) {
        qDebug() << "❌ Erreur afficher:" << model->lastError().text();
    } else {
        qDebug() << "✓ Affichage Habitat réussi -" << model->rowCount() << "lignes";
    }

    model->setHeaderData(0, Qt::Horizontal, "ID");
    model->setHeaderData(1, Qt::Horizontal, "Adresse");
    model->setHeaderData(2, Qt::Horizontal, "Nb Personnes");
    model->setHeaderData(3, Qt::Horizontal, "Surface");
    model->setHeaderData(4, Qt::Horizontal, "Type Logement");

    qDebug() << "=== FIN Habitat::afficher ===";
    return model;
}

// Supprimer
bool Habitat::supprimer(int id)
{
    qDebug() << "=== DÉBUT Habitat::supprimer H" << id << "===";

    QSqlDatabase db = Connexion::createInstance().getDatabase();
    if (!db.isOpen()) {
        qDebug() << "❌ Base de données fermée";
        return false;
    }

    if (!db.transaction()) {
        qDebug() << "❌ Erreur transaction:" << db.lastError().text();
        return false;
    }

    QSqlQuery query(db);

    if (!query.prepare("DELETE FROM HABITAT WHERE ID_HABITAT = :id")) {
        qDebug() << "❌ Erreur prepare supprimer:" << query.lastError().text();
        db.rollback();
        return false;
    }

    query.bindValue(":id", id);

    if (!query.exec()) {
        qDebug() << "❌ Erreur exec supprimer:" << query.lastError().text();
        db.rollback();
        return false;
    }

    if (!db.commit()) {
        qDebug() << "❌ Erreur commit:" << db.lastError().text();
        return false;
    }

    qDebug() << "✓ Habitat supprimé avec succès";
    qDebug() << "=== FIN Habitat::supprimer ===";
    return true;
}

// Modifier
bool Habitat::modifier()
{
    qDebug() << "=== DÉBUT Habitat::modifier ===";

    QSqlDatabase db = Connexion::createInstance().getDatabase();
    if (!db.isOpen()) {
        qDebug() << "❌ Base de données fermée";
        return false;
    }

    if (!db.transaction()) {
        qDebug() << "❌ Erreur transaction:" << db.lastError().text();
        return false;
    }

    QSqlQuery query(db);

    if (!query.prepare("UPDATE HABITAT SET ADRESSE = :adresse, NB_PERSONNES = :nb, "
                       "SURFACE = :surface, TYPE_LOGEMENT = :type WHERE ID_HABITAT = :id")) {
        qDebug() << "❌ Erreur prepare modifier:" << query.lastError().text();
        db.rollback();
        return false;
    }

    query.bindValue(":id", ID_HABITAT);
    query.bindValue(":adresse", ADRESSE);
    query.bindValue(":nb", NB_PERSONNES);
    query.bindValue(":surface", SURFACE);
    query.bindValue(":type", TYPE_LOGEMENT);

    if (!query.exec()) {
        qDebug() << "❌ Erreur exec modifier:" << query.lastError().text();
        db.rollback();
        return false;
    }

    if (!db.commit()) {
        qDebug() << "❌ Erreur commit:" << db.lastError().text();
        return false;
    }

    qDebug() << "✓ Habitat modifié avec succès";
    qDebug() << "=== FIN Habitat::modifier ===";
    return true;
}
