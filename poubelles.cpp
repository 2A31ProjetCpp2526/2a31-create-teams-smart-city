#include "poubelles.h"

Poubelle::Poubelle() : nbPoubelles(0), tauxRemplissage(0), joursDepuisRamassage(0), etat("Propre") {}

Poubelle::Poubelle(int nb, double taux, int jours, const QString &etat)
{
    nbPoubelles = nb;
    tauxRemplissage = taux;
    joursDepuisRamassage = jours;
    this->etat = etat;
}

// ---------------- KNN CLASSIFIER ------------------
QString Poubelle::classifierPoubelle(int nb, double taux, int jours)
{
    QVector<Donnee> base = {
        {10, 30, 1, "Propre"},
        {15, 40, 2, "Propre"},
        {12, 80, 4, "Saturée"},
        {20, 90, 5, "Saturée"},
        {8, 50, 3, "Propre"}
    };

    struct Distance { double value; QString etat; };
    QVector<Distance> distances;

    for (auto &d : base) {
        double dist = std::sqrt(
            std::pow(d.nbPoubelles - nb, 2) +
            std::pow(d.tauxRemplissage - taux, 2) +
            std::pow(d.jours - jours, 2)
            );
        distances.push_back({dist, d.etat});
    }

    std::sort(distances.begin(), distances.end(), [](const Distance &a, const Distance &b){ return a.value < b.value; });

    int k = 3;
    int propre = 0, saturee = 0;
    for (int i = 0; i < k; ++i) {
        if (distances[i].etat == "Propre") propre++;
        else saturee++;
    }

    return (propre > saturee) ? "Propre" : "Saturée";
}

// ---------------- GETTERS -----------------
int Poubelle::getNbPoubelles() const { return nbPoubelles; }
double Poubelle::getTauxRemplissage() const { return tauxRemplissage; }
int Poubelle::getJours() const { return joursDepuisRamassage; }
QString Poubelle::getEtat() const { return etat; }

// ---------------- SETTERS -----------------
void Poubelle::setNbPoubelles(int nb) { nbPoubelles = nb; }
void Poubelle::setTauxRemplissage(double taux) { tauxRemplissage = taux; }
void Poubelle::setJours(int jours) { joursDepuisRamassage = jours; }
void Poubelle::setEtat(const QString &e) { etat = e; }



