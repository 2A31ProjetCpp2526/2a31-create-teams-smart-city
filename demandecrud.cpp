#include "demandecrud.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>

DemandeCRUD::DemandeCRUD() : idHabitant(0) {}

DemandeCRUD::DemandeCRUD(int idHabitant, const QString &typeProbleme, const QString &description, const QString &statut)
    : idHabitant(idHabitant), typeProbleme(typeProbleme), description(description), statut(statut)
{
}

bool DemandeCRUD::habitantExiste(int id)
{
    QSqlQuery q;
    q.prepare("SELECT 1 FROM HABITANT WHERE ID_HABITANT = :id");
    q.bindValue(":id", id);
    return q.exec() && q.next();
}

bool DemandeCRUD::ajouter(QString &messageErreur) const
{
    messageErreur.clear();
    if (idHabitant <= 0) { messageErreur = "ID habitant invalide"; return false; }
    if (typeProbleme.trimmed().isEmpty()) { messageErreur = "Veuillez choisir le type de problème"; return false; }
    if (description.trimmed().isEmpty()) { messageErreur = "Veuillez saisir la réclamation"; return false; }

    if (!habitantExiste(idHabitant)) {
        messageErreur = "L'habitant n'existe pas";
        return false;
    }

    // Hypothèse: table DEMANDE(ID_DEMANDE auto via trigger/sequence, ID_HABITANT, TYPE_PROBLEME, DESCRIPTION, STATUT, DATE_CREATION)
    QSqlQuery q;
    q.prepare("INSERT INTO DEMANDE (ID_HABITANT, TYPE_PROBLEME, DESCRIPTION, STATUT, DATE_CREATION) "
              "VALUES (:id_h, :type, :desc, :statut, SYSDATE)");
    q.bindValue(":id_h", idHabitant);
    q.bindValue(":type", typeProbleme.trimmed());
    q.bindValue(":desc", description.trimmed());
    q.bindValue(":statut", statut.isEmpty() ? QString("Nouveau") : statut.trimmed());

    if (!q.exec()) {
        messageErreur = q.lastError().text();
        return false;
    }
    return true;
}
