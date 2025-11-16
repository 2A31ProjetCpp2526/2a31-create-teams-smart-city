#include "consommation.h"
#include <QVariant>
#include <QDebug>
#include <QSqlError>
#include <QSqlQuery>

// Constructeurs
Consommation::Consommation()
    : id_conso(0), id_habitat(0), eau_conso(0), moy_conso(0), etat_eau("Normal"), date_releve("") {}

Consommation::Consommation(int id_habitat, float eau_conso, float moy_conso, QString date_releve)
    : id_habitat(id_habitat), eau_conso(eau_conso), moy_conso(moy_conso), date_releve(date_releve)
{
    etat_eau = calculerEtat(eau_conso, moy_conso);
}

Consommation::Consommation(int id_habitat, float eau_conso, float moy_conso, QString etat_eau, QString date_releve)
    : id_habitat(id_habitat), eau_conso(eau_conso), moy_conso(moy_conso), etat_eau(etat_eau), date_releve(date_releve) {}

// Getters
int Consommation::getIdConso() const { return id_conso; }
int Consommation::getIdHabitat() const { return id_habitat; }
float Consommation::getEauConso() const { return eau_conso; }
float Consommation::getMoyConso() const { return moy_conso; }
QString Consommation::getEtatEau() const { return etat_eau; }
QString Consommation::getDateReleve() const { return date_releve; }

// Setters
void Consommation::setIdConso(int id) { id_conso = id; }
void Consommation::setIdHabitat(int id) { id_habitat = id; }
void Consommation::setEauConso(float val) { eau_conso = val; }
void Consommation::setMoyConso(float val) { moy_conso = val; }
void Consommation::setEtatEau(const QString &val) { etat_eau = val; }
void Consommation::setDateReleve(const QString &val) { date_releve = val; }

// Calculer état
QString Consommation::calculerEtat(float eau_conso, float moy_conso)
{
    if(eau_conso > moy_conso * 1.5f) return "Fuite";
    else if(eau_conso < moy_conso * 0.5f) return "Gaspillage";
    return "Normal";
}

// Ajouter
bool Consommation::ajouter()
{
    QSqlQuery query;

    if (!query.prepare("INSERT INTO CONSOMMATION (ID_HABITAT, EAU_CONSO, MOY_CONSO, ETAT_EAU, DATE_RELEVE) "
                       "VALUES (:id_habitat, :eau_conso, :moy_conso, :etat_eau, TO_DATE(:date_releve,'YYYY-MM-DD HH24:MI'))")) {
        qDebug() << "Erreur prepare ajouter:" << query.lastError().text();
        return false;
    }

    query.bindValue(":id_habitat", id_habitat);
    query.bindValue(":eau_conso", eau_conso);
    query.bindValue(":moy_conso", moy_conso);
    query.bindValue(":etat_eau", etat_eau);
    query.bindValue(":date_releve", date_releve);

    if (!query.exec()) {
        qDebug() << "Erreur exec ajouter:" << query.lastError().text();
        return false;
    }

    qDebug() << "✓ Consommation ajoutée avec succès";
    return true;
}

// Modifier
bool Consommation::modifier()
{
    QSqlQuery query;

    if (!query.prepare("UPDATE CONSOMMATION SET ID_HABITAT=:id_habitat, EAU_CONSO=:eau_conso, "
                       "MOY_CONSO=:moy_conso, ETAT_EAU=:etat_eau, "
                       "DATE_RELEVE=TO_DATE(:date_releve,'YYYY-MM-DD HH24:MI') "
                       "WHERE ID_CONSO=:id")) {
        qDebug() << "Erreur prepare modifier:" << query.lastError().text();
        return false;
    }

    query.bindValue(":id", id_conso);
    query.bindValue(":id_habitat", id_habitat);
    query.bindValue(":eau_conso", eau_conso);
    query.bindValue(":moy_conso", moy_conso);
    query.bindValue(":etat_eau", etat_eau);
    query.bindValue(":date_releve", date_releve);

    if (!query.exec()) {
        qDebug() << "Erreur exec modifier:" << query.lastError().text();
        return false;
    }

    qDebug() << "✓ Consommation modifiée avec succès";
    return true;
}

// Supprimer
bool Consommation::supprimer(int id)
{
    QSqlQuery query;

    if (!query.prepare("DELETE FROM CONSOMMATION WHERE ID_CONSO=:id")) {
        qDebug() << "Erreur prepare supprimer:" << query.lastError().text();
        return false;
    }

    query.bindValue(":id", id);

    if (!query.exec()) {
        qDebug() << "Erreur exec supprimer:" << query.lastError().text();
        return false;
    }

    qDebug() << "✓ Consommation supprimée avec succès";
    return true;
}

// Afficher
QSqlQueryModel* Consommation::afficher()
{
    QSqlQueryModel *model = new QSqlQueryModel();

    // Ordre: ID_HABITAT, ID_CONSO, EAU_CONSO, MOY_CONSO, ETAT_EAU, DATE_RELEVE
    model->setQuery("SELECT ID_HABITAT, ID_CONSO, EAU_CONSO, MOY_CONSO, ETAT_EAU, "
                    "TO_CHAR(DATE_RELEVE, 'YYYY-MM-DD HH24:MI') as DATE_RELEVE "
                    "FROM CONSOMMATION "
                    "ORDER BY ID_CONSO");

    if (model->lastError().isValid()) {
        qDebug() << "Erreur afficher:" << model->lastError().text();
    } else {
        qDebug() << "✓ Affichage des consommations réussi -" << model->rowCount() << "lignes";
    }

    return model;
}
