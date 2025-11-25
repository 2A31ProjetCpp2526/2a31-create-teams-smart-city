#ifndef CONSOMMATION_H
#define CONSOMMATION_H

#include <QString>
#include <QSqlQueryModel>

class Consommation
{
private:
    int id_conso;
    int id_habitat;
    float eau_conso;
    float moy_conso;
    QString type_anomalie;     // Normal, Fuite, Gaspillage
    QString date_releve;
    QString localisation_fuite; // cuisine, salle de bain, etc.
    QString source_detection;   // Automatique / Habitant
    QString sms_envoye;         // Oui / Non

public:
    Consommation();

    // constructeurs possibles
    Consommation(int id_habitat, float eau_conso, float moy_conso, QString date_releve);
    Consommation(int id_habitat, float eau_conso, float moy_conso,
                 QString type_anomalie, QString date_releve,
                 QString localisation, QString source, QString sms);

    // getters
    int getIdConso() const;
    int getIdHabitat() const;
    float getEauConso() const;
    float getMoyConso() const;
    QString getTypeAnomalie() const;
    QString getDateReleve() const;
    QString getLocalisation() const;
    QString getSourceDetection() const;
    QString getSmsEnvoye() const;

    // setters
    void setIdConso(int id);
    void setIdHabitat(int id);
    void setEauConso(float val);
    void setMoyConso(float val);
    void setTypeAnomalie(const QString &val);
    void setDateReleve(const QString &val);
    void setLocalisation(const QString &val);
    void setSourceDetection(const QString &val);
    void setSmsEnvoye(const QString &val);

    // logique
    QString detecter_anomalie(float conso, float moy);

    // CRUD
    bool ajouter();
    bool modifier();
    bool supprimer(int id);
    QSqlQueryModel* afficher();
};

#endif
