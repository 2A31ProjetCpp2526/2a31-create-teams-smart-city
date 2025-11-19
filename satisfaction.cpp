#include "satisfaction.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <utility>

SatisfactionCRUD::SatisfactionCRUD()
    : id(0), idHabitant(0), idDemande(0), score(0)
{
}

SatisfactionCRUD::SatisfactionCRUD(int idHabitant, int score, int idDemande, const QString &source, const QString &commentaire)
    : id(0), idHabitant(idHabitant), idDemande(idDemande), score(score), source(source), commentaire(commentaire)
{
}

static bool habitantExiste_local(int id)
{
    QSqlQuery q;
    q.prepare("SELECT 1 FROM TOUHA.HABITANT WHERE ID_HABITANT = :id");
    q.bindValue(":id", id);
    return q.exec() && q.next();
}

static bool demandeExiste_local(int id)
{
    if (id <= 0) return true; // facultatif
    QSqlQuery q;
    q.prepare("SELECT 1 FROM TOUHA.DEMANDE WHERE ID_DEMANDE = :id");
    q.bindValue(":id", id);
    return q.exec() && q.next();
}

bool SatisfactionCRUD::ajouter(QString &messageErreur) const
{
    messageErreur.clear();
    if (idHabitant <= 0) { messageErreur = "ID habitant invalide"; return false; }
    if (score < 0 || score > 100) { messageErreur = "Score doit être entre 0 et 100"; return false; }

    if (!habitantExiste_local(idHabitant)) { messageErreur = "Habitant inexistant"; return false; }
    if (!demandeExiste_local(idDemande)) { messageErreur = "Demande inexistante"; return false; }

    // Vérifier l'accès à la table cible pour éviter une erreur silencieuse du driver
    {
        QSqlQuery ping;
        if (!ping.exec("SELECT 1 FROM TOUHA.SATISFACTION WHERE 1=0")) {
            const QString drv = ping.lastError().driverText();
            const QString dbt = ping.lastError().databaseText();
            messageErreur = QString("Accès table SATISFACTION impossible. %1 %2")
                                .arg(drv)
                                .arg(dbt);
            return false;
        }
    }

    QSqlQuery q;
    if (idDemande > 0) {
        q.prepare("INSERT INTO TOUHA.SATISFACTION (ID_HABITANT, ID_DEMANDE, SCORE, DATE_SAISIE, SOURCE, COMMENTAIRE) "
                  "VALUES (:h, :d, :s, SYSDATE, :src, :com)");
        q.bindValue(":h", idHabitant);
        q.bindValue(":d", idDemande);
        q.bindValue(":s", score);
        q.bindValue(":src", source);
        q.bindValue(":com", commentaire);
    } else {
        // Pas d'ID_DEMANDE: ne pas inclure la colonne pour éviter les soucis de typage ODBC/Oracle
        q.prepare("INSERT INTO TOUHA.SATISFACTION (ID_HABITANT, SCORE, DATE_SAISIE, SOURCE, COMMENTAIRE) "
                  "VALUES (:h, :s, SYSDATE, :src, :com)");
        q.bindValue(":h", idHabitant);
        q.bindValue(":s", score);
        q.bindValue(":src", source);
        q.bindValue(":com", commentaire);
    }

    if (!q.exec()) {
        const QString drv = q.lastError().driverText();
        const QString dbt = q.lastError().databaseText();
        QString detail = q.lastError().text();
        if (detail.trimmed().isEmpty()) detail = QString("%1 %2").arg(drv, dbt).trimmed();
        if (detail.trimmed().isEmpty()) detail = QStringLiteral("Échec d'insertion inconnu (aucun message du pilote)");
        messageErreur = detail;
        return false;
    }
    return true;
}

bool SatisfactionCRUD::modifier(int idSatisfaction, QString &messageErreur) const
{
    messageErreur.clear();
    if (idSatisfaction <= 0) { messageErreur = "ID satisfaction invalide"; return false; }
    if (idHabitant <= 0) { messageErreur = "ID habitant invalide"; return false; }
    if (score < 0 || score > 100) { messageErreur = "Score doit être entre 0 et 100"; return false; }

    if (!habitantExiste_local(idHabitant)) { messageErreur = "Habitant inexistant"; return false; }
    if (!demandeExiste_local(idDemande)) { messageErreur = "Demande inexistante"; return false; }

    QSqlQuery q;
    if (idDemande > 0) {
        q.prepare("UPDATE TOUHA.SATISFACTION SET ID_HABITANT=:h, ID_DEMANDE=:d, SCORE=:s, SOURCE=:src, COMMENTAIRE=:com WHERE ID_SATISFACTION=:id");
        q.bindValue(":h", idHabitant);
        q.bindValue(":d", idDemande);
        q.bindValue(":s", score);
        q.bindValue(":src", source);
        q.bindValue(":com", commentaire);
        q.bindValue(":id", idSatisfaction);
    } else {
        q.prepare("UPDATE TOUHA.SATISFACTION SET ID_HABITANT=:h, ID_DEMANDE=NULL, SCORE=:s, SOURCE=:src, COMMENTAIRE=:com WHERE ID_SATISFACTION=:id");
        q.bindValue(":h", idHabitant);
        q.bindValue(":s", score);
        q.bindValue(":src", source);
        q.bindValue(":com", commentaire);
        q.bindValue(":id", idSatisfaction);
    }

    if (!q.exec()) { messageErreur = q.lastError().text(); return false; }
    return true;
}

bool SatisfactionCRUD::supprimer(int idSatisfaction, QString &messageErreur)
{
    messageErreur.clear();
    if (idSatisfaction <= 0) { messageErreur = "ID satisfaction invalide"; return false; }
    QSqlQuery q;
    q.prepare("DELETE FROM TOUHA.SATISFACTION WHERE ID_SATISFACTION = :id");
    q.bindValue(":id", idSatisfaction);
    if (!q.exec()) {
        const QString drv = q.lastError().driverText();
        const QString dbt = q.lastError().databaseText();
        QString detail = q.lastError().text();
        if (detail.trimmed().isEmpty()) detail = QString("%1 %2").arg(drv, dbt).trimmed();
        if (detail.trimmed().isEmpty()) detail = QStringLiteral("Échec de suppression inconnu (aucun message du pilote)");
        messageErreur = detail;
        return false;
    }
    return q.numRowsAffected() > 0;
}

QSqlQueryModel* SatisfactionCRUD::afficher(QObject *parent)
{
    QSqlQueryModel *m = new QSqlQueryModel(parent);
    m->setQuery("SELECT ID_SATISFACTION, ID_HABITANT, ID_DEMANDE, SCORE, DATE_SAISIE, SOURCE, COMMENTAIRE "
                "FROM TOUHA.SATISFACTION ORDER BY DATE_SAISIE DESC, ID_SATISFACTION DESC");
    m->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    m->setHeaderData(1, Qt::Horizontal, QObject::tr("Habitant"));
    m->setHeaderData(2, Qt::Horizontal, QObject::tr("Demande"));
    m->setHeaderData(3, Qt::Horizontal, QObject::tr("Score"));
    m->setHeaderData(4, Qt::Horizontal, QObject::tr("Date"));
    m->setHeaderData(5, Qt::Horizontal, QObject::tr("Source"));
    m->setHeaderData(6, Qt::Horizontal, QObject::tr("Commentaire"));
    return m;
}

QSqlQueryModel* SatisfactionCRUD::afficherParHabitant(int idHabitant, QObject *parent)
{
    QSqlQueryModel *m = new QSqlQueryModel(parent);
    QSqlQuery q;
    q.prepare("SELECT ID_SATISFACTION, ID_HABITANT, ID_DEMANDE, SCORE, DATE_SAISIE, SOURCE, COMMENTAIRE "
              "FROM TOUHA.SATISFACTION WHERE ID_HABITANT = :id ORDER BY DATE_SAISIE DESC, ID_SATISFACTION DESC");
    q.bindValue(":id", idHabitant);
    if (!q.exec()) {
        // fallback empty model with error if needed
        return m;
    }
    m->setQuery(std::move(q));
    m->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    m->setHeaderData(1, Qt::Horizontal, QObject::tr("Habitant"));
    m->setHeaderData(2, Qt::Horizontal, QObject::tr("Demande"));
    m->setHeaderData(3, Qt::Horizontal, QObject::tr("Score"));
    m->setHeaderData(4, Qt::Horizontal, QObject::tr("Date"));
    m->setHeaderData(5, Qt::Horizontal, QObject::tr("Source"));
    m->setHeaderData(6, Qt::Horizontal, QObject::tr("Commentaire"));
    return m;
}
