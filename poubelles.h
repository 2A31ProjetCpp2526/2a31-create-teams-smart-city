#ifndef POUBELLE_H
#define POUBELLE_H

#include <QString>
#include <QVector>
#include <QMap>
#include <cmath>
#include <algorithm>

class Poubelle
{
public:
    // Structure correspondant aux données de la table POUBELLE
    struct Donnee {
        int nbPoubelles;          // NOMBRE_POUBELLE
        double tauxRemplissage;   // POURCENTAGE
        int jours;                // JOURS
        QString etat;             // CLASSIFICATION
    };

    Poubelle();
    Poubelle(int nb, double taux, int jours, const QString &etat);

    // Classifier une poubelle selon KNN
    static QString classifierPoubelle(int nb, double taux, int jours);

    // Getters
    int getNbPoubelles() const;
    double getTauxRemplissage() const;
    int getJours() const;
    QString getEtat() const;

    // Setters
    void setNbPoubelles(int nb);
    void setTauxRemplissage(double taux);
    void setJours(int jours);
    void setEtat(const QString &etat);

private:
    int nbPoubelles;
    double tauxRemplissage;
    int joursDepuisRamassage;
    QString etat;
};

#endif // POUBELLE_H


