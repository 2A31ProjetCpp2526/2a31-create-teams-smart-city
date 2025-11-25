#ifndef HABITAT_H
#define HABITAT_H

#include <QString>
#include <QSqlQueryModel>

class Habitat
{
private:
    int ID_HABITAT;
    QString ADRESSE;
    int NB_PERSONNES;
    double SURFACE;
    QString TYPE_LOGEMENT;
    QString TELEPHONE_CONTACT;  // ← nouveau

public:
    Habitat();
    Habitat(int id, QString adresse, int nb_personnes, double surface,
            QString type_logement, QString telephone = "+216");

    // Getters
    int getID_HABITAT() const;
    QString getADRESSE() const;
    int getNB_PERSONNES() const;
    double getSURFACE() const;
    QString getTYPE_LOGEMENT() const;
    QString getTELEPHONE_CONTACT() const;

    // CRUD
    bool ajouter();
    QSqlQueryModel* afficher();
    bool supprimer(int id);
    bool modifier();
};

#endif // HABITAT_H
