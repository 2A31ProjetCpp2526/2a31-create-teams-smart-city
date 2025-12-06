#include "consommationeau.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

// Constructeur par défaut
Consommation::Consommation()
    : id_conso(0), id_habitat(0), eau_conso(0), moy_conso(0),
    type_anomalie("Normal"), date_releve(""),
    localisation_fuite(""), source_detection("Automatique"),
    sms_envoye("Non") {}

// Constructeur automatique
Consommation::Consommation(int id_habitat, float eau, float moy, QString date)
    : id_habitat(id_habitat), eau_conso(eau), moy_conso(moy), date_releve(date)
{
    type_anomalie = detecter_anomalie(eau, moy);
    localisation_fuite = "";
    source_detection = "Automatique";
    sms_envoye = "Non";
}

// Constructeur complet
Consommation::Consommation(int id_habitat, float eau, float moy,
                           QString anomalie, QString date,
                           QString localisation, QString source, QString sms)
    : id_habitat(id_habitat), eau_conso(eau), moy_conso(moy),
    type_anomalie(anomalie), date_releve(date),
    localisation_fuite(localisation), source_detection(source),
    sms_envoye(sms) {}

// GETTERS
int Consommation::getIdConso() const { return id_conso; }
int Consommation::getIdHabitat() const { return id_habitat; }
float Consommation::getEauConso() const { return eau_conso; }
float Consommation::getMoyConso() const { return moy_conso; }
QString Consommation::getTypeAnomalie() const { return type_anomalie; }
QString Consommation::getDateReleve() const { return date_releve; }
QString Consommation::getLocalisation() const { return localisation_fuite; }
QString Consommation::getSourceDetection() const { return source_detection; }
QString Consommation::getSmsEnvoye() const { return sms_envoye; }

// SETTERS
void Consommation::setIdConso(int id) { id_conso = id; }
void Consommation::setIdHabitat(int id) { id_habitat = id; }
void Consommation::setEauConso(float v) { eau_conso = v; }
void Consommation::setMoyConso(float v) { moy_conso = v; }
void Consommation::setTypeAnomalie(const QString &v) { type_anomalie = v; }
void Consommation::setDateReleve(const QString &v) { date_releve = v; }
void Consommation::setLocalisation(const QString &v) { localisation_fuite = v; }
void Consommation::setSourceDetection(const QString &v) { source_detection = v; }
void Consommation::setSmsEnvoye(const QString &v) { sms_envoye = v; }

// Détection automatique
QString Consommation::detecter_anomalie(float conso, float moy)
{
    if (conso > moy * 1.5f) return "Fuite";
    if (conso < moy * 0.4f) return "Gaspillage";
    return "Normal";
}

// Ajouter
bool Consommation::ajouter()
{
    qDebug() << "=== DÉBUT Consommation::ajouter ===";
    qDebug() << "ID_HABITAT:" << id_habitat;
    qDebug() << "EAU_CONSO:" << eau_conso;
    qDebug() << "MOY_CONSO:" << moy_conso;
    qDebug() << "DATE_RELEVE:" << date_releve;

    // Détection automatique de l'anomalie
    QString typeAnomalie = "Normal";
    QString sourceDetection = "Automatique";
    QString localisation = "";

    if(eau_conso > moy_conso * 1.5) {
        typeAnomalie = "Fuite";
        localisation = "À déterminer";
    }
    else if(eau_conso > moy_conso * 1.3) {
        typeAnomalie = "Gaspillage";
    }
    else if(eau_conso < moy_conso * 0.5) {
        typeAnomalie = "Sous-Consommation";
    }

    QSqlQuery query;

    // ✅ CORRECTION : Utiliser TO_DATE pour la conversion
    query.prepare("INSERT INTO CONSOMMATION "
                  "(ID_HABITAT, EAU_CONSO, MOY_CONSO, TYPE_ANOMALIE, "
                  "SOURCE_DETECTION, SMS_ENVOYE, LOCALISATION_FUITE, DATE_RELEVE) "
                  "VALUES (:habitat, :eau, :moy, :type, :source, 0, :loc, "
                  "TO_DATE(:date, 'YYYY-MM-DD HH24:MI'))");

    query.bindValue(":habitat", id_habitat);
    query.bindValue(":eau", eau_conso);
    query.bindValue(":moy", moy_conso);
    query.bindValue(":type", typeAnomalie);
    query.bindValue(":source", sourceDetection);
    query.bindValue(":loc", localisation);
    query.bindValue(":date", date_releve);

    if(!query.exec()) {
        qDebug() << "❌ Erreur SQL:" << query.lastError().text();
        qDebug() << "Requête:" << query.lastQuery();
        qDebug() << "=== FIN Consommation::ajouter (ÉCHEC) ===";
        return false;
    }

    qDebug() << "✅ Consommation ajoutée avec succès !";
    qDebug() << "Type détecté:" << typeAnomalie;
    qDebug() << "=== FIN Consommation::ajouter (SUCCÈS) ===";
    return true;
}

// Modifier
bool Consommation::modifier()
{
    QSqlQuery q;
    q.prepare("UPDATE CONSOMMATION SET "
              "ID_HABITAT=:id, EAU_CONSO=:eau, MOY_CONSO=:moy, "
              "TYPE_ANOMALIE=:anom, DATE_RELEVE=TO_DATE(:date,'YYYY-MM-DD HH24:MI'), "
              "LOCALISATION_FUITE=:loc, SOURCE_DETECTION=:src, SMS_ENVOYE=:sms "
              "WHERE ID_CONSO=:conso");

    q.bindValue(":conso", id_conso);
    q.bindValue(":id", id_habitat);
    q.bindValue(":eau", eau_conso);
    q.bindValue(":moy", moy_conso);
    q.bindValue(":anom", type_anomalie);
    q.bindValue(":date", date_releve);
    q.bindValue(":loc", localisation_fuite);
    q.bindValue(":src", source_detection);
    q.bindValue(":sms", sms_envoye);

    return q.exec();
}

// Supprimer
bool Consommation::supprimer(int id)
{
    QSqlQuery q;
    q.prepare("DELETE FROM CONSOMMATION WHERE ID_CONSO=:id");
    q.bindValue(":id", id);
    return q.exec();
}

// Afficher
QSqlQueryModel* Consommation::afficher()
{
    QSqlQueryModel *m = new QSqlQueryModel();
    m->setQuery("SELECT ID_CONSO, ID_HABITAT, EAU_CONSO, MOY_CONSO, "
                "TYPE_ANOMALIE, LOCALISATION_FUITE, SOURCE_DETECTION, SMS_ENVOYE, "
                "TO_CHAR(DATE_RELEVE,'YYYY-MM-DD HH24:MI') AS DATE_RELEVE "
                "FROM CONSOMMATION ORDER BY ID_CONSO ASC");
    return m;
}
