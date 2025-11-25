#include "affectationp.h"

#include <QSqlQuery>
#include <QSqlError>

static bool execUpdate(QSqlQuery &q, QString &error)
{
    if (!q.exec()) {
        error = q.lastError().text();
        return false;
    }
    return true;
}

bool AffectationP::affecterDemande(int idDemande, const QString &idPersonnel, QString &error)
{
    QSqlQuery q;
    q.prepare("UPDATE DEMANDE "
              "SET ID_PERSONNEL_AFFECTE = :idp, "
              "    DATE_AFFECTATION = SYSDATE, "
              "    STATUT = 'En_cours' "
              "WHERE ID_DEMANDE = :idd");
    q.bindValue(":idp", idPersonnel);
    q.bindValue(":idd", idDemande);
    if (!execUpdate(q, error))
        return false;

    QSqlQuery qAff;
    qAff.prepare("INSERT INTO AFFECTATION (ID_DEMANDE, ID_PERSONNEL, STATUT) "
                 "VALUES (:idd, :idp, 'En_cours')");
    qAff.bindValue(":idd", idDemande);
    qAff.bindValue(":idp", idPersonnel);
    return execUpdate(qAff, error);
}

bool AffectationP::annulerAffectation(int idDemande, QString &error)
{
    QSqlQuery q;
    q.prepare("UPDATE DEMANDE "
              "SET ID_PERSONNEL_AFFECTE = NULL, "
              "    DATE_AFFECTATION = NULL, "
              "    DATE_RESOLUTION_INTERNE = NULL, "
              "    DATE_RESOLUTION_FINALE = NULL, "
              "    STATUT = 'Nouveau' "
              "WHERE ID_DEMANDE = :idd");
    q.bindValue(":idd", idDemande);
    if (!execUpdate(q, error))
        return false;

    QSqlQuery qAff;
    qAff.prepare("UPDATE AFFECTATION "
                 "SET DATE_FIN = SYSDATE, "
                 "    STATUT = 'Annulee' "
                 "WHERE ID_DEMANDE = :idd "
                 "  AND DATE_FIN IS NULL");
    qAff.bindValue(":idd", idDemande);
    return execUpdate(qAff, error);
}

bool AffectationP::fixerResolutionInterne(int idDemande, QString &error)
{
    QSqlQuery q;
    q.prepare("UPDATE DEMANDE "
              "SET DATE_RESOLUTION_INTERNE = SYSDATE "
              "WHERE ID_DEMANDE = :idd");
    q.bindValue(":idd", idDemande);
    return execUpdate(q, error);
}

bool AffectationP::fixerResolutionFinale(int idDemande, QString &error)
{
    QSqlQuery q;
    q.prepare("UPDATE DEMANDE "
              "SET DATE_RESOLUTION_FINALE = SYSDATE, "
              "    STATUT = 'Cloture' "
              "WHERE ID_DEMANDE = :idd");
    q.bindValue(":idd", idDemande);
    if (!execUpdate(q, error))
        return false;

    QSqlQuery qAff;
    qAff.prepare("UPDATE AFFECTATION "
                 "SET DATE_FIN = SYSDATE, "
                 "    STATUT = 'Terminee' "
                 "WHERE ID_DEMANDE = :idd "
                 "  AND DATE_FIN IS NULL");
    qAff.bindValue(":idd", idDemande);
    return execUpdate(qAff, error);
}
