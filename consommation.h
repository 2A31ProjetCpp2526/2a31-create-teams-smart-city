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
    QString etat_eau;
    QString date_releve;

public:
    Consommation();
    Consommation(int id_habitat, float eau_conso, float moy_conso, QString date_releve);
    Consommation(int id_habitat, float eau_conso, float moy_conso, QString etat_eau, QString date_releve);

    // Getters
    int getIdConso() const;
    int getIdHabitat() const;
    float getEauConso() const;
    float getMoyConso() const;
    QString getEtatEau() const;
    QString getDateReleve() const;

    // Setters
    void setIdConso(int id);
    void setIdHabitat(int id);
    void setEauConso(float val);
    void setMoyConso(float val);
    void setEtatEau(const QString &val);
    void setDateReleve(const QString &val);

    // Méthodes
    QString calculerEtat(float eau_conso, float moy_conso);
    bool ajouter();
    bool modifier();
    bool supprimer(int id);
    QSqlQueryModel* afficher();
};

#endif // CONSOMMATION_H
