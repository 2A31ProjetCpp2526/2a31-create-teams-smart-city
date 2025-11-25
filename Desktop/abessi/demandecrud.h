#ifndef DEMANDECRUD_H
#define DEMANDECRUD_H

#include <QString>

class DemandeCRUD
{
private:
    int idHabitant;
    QString typeProbleme;
    QString description;
    QString statut; // Nouveau, En_cours, Cloture

public:
    DemandeCRUD();
    DemandeCRUD(int idHabitant, const QString &typeProbleme, const QString &description, const QString &statut = QString("Nouveau"));

    bool ajouter(QString &messageErreur) const;
    static bool habitantExiste(int idHabitant);
};

#endif // DEMANDECRUD_H
