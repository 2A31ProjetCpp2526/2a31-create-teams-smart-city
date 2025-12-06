#ifndef AFFECTATIONP_H
#define AFFECTATIONP_H

#include <QString>

class AffectationP
{
public:
    static bool affecterDemande(int idDemande, const QString &idPersonnel, QString &error);
    static bool annulerAffectation(int idDemande, QString &error);
    static bool fixerResolutionInterne(int idDemande, QString &error);
    static bool fixerResolutionFinale(int idDemande, QString &error);
};

#endif // AFFECTATIONP_H
