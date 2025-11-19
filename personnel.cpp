#include "personnel.h"
#include <QSqlError>
#include <QSqlRecord>
#include <QSqlQuery>
#include <QDebug>
#include <QMessageBox>

// Fonction utilitaire pour détecter les noms de colonnes réels et le nom de la table
static QString detectTableName(QStringList &columns)
{
    QString tableName = "";
    QSqlQuery detectQuery;

    // Essayer d'abord PERSONNEL (singulier), puis PERSONNELS (pluriel)
    // Utiliser une requête qui fonctionne même si la table est vide
    if (detectQuery.exec("SELECT * FROM PERSONNEL WHERE 1=0")) {
        QSqlRecord record = detectQuery.record();
        for (int i = 0; i < record.count(); i++) {
            columns << record.fieldName(i);
        }
        tableName = "PERSONNEL";
        qDebug() << "✅ Colonnes détectées dans PERSONNEL:" << columns;
    } else if (detectQuery.exec("SELECT * FROM PERSONNELS WHERE 1=0")) {
        QSqlRecord record = detectQuery.record();
        for (int i = 0; i < record.count(); i++) {
            columns << record.fieldName(i);
        }
        tableName = "PERSONNELS";
        qDebug() << "✅ Colonnes détectées dans PERSONNELS:" << columns;
    } else {
        qDebug() << "⚠️ Impossible de détecter les colonnes:" << detectQuery.lastError().text();
        // Essayer avec une autre méthode
        QSqlQuery query2;
        if (query2.exec("SELECT COLUMN_NAME FROM USER_TAB_COLUMNS WHERE TABLE_NAME = 'PERSONNEL' ORDER BY COLUMN_ID")) {
            tableName = "PERSONNEL";
            while (query2.next()) {
                columns << query2.value(0).toString();
            }
            qDebug() << "✅ Colonnes détectées via USER_TAB_COLUMNS (PERSONNEL):" << columns;
        } else if (query2.exec("SELECT COLUMN_NAME FROM USER_TAB_COLUMNS WHERE TABLE_NAME = 'PERSONNELS' ORDER BY COLUMN_ID")) {
            tableName = "PERSONNELS";
            while (query2.next()) {
                columns << query2.value(0).toString();
            }
            qDebug() << "✅ Colonnes détectées via USER_TAB_COLUMNS (PERSONNELS):" << columns;
        }
    }
    return tableName;
}

personnel::personnel() {}

personnel::personnel(QString ID_PERSONNEL, QString NOM, QString PRENOM, QString COMPETENCES, QString ZONE_AFFECTATION)
{
    this->ID_PERSONNEL = ID_PERSONNEL;
    this->NOM = NOM;
    this->PRENOM = PRENOM;
    this->COMPETENCES = COMPETENCES;
    this->ZONE_AFFECTATION = ZONE_AFFECTATION;
}

bool personnel::ajouter()
{
    // Détecter les colonnes réelles de la table et le nom de la table
    QStringList realColumns;
    QString tableName = detectTableName(realColumns);

    // Trouver les noms de colonnes réels (insensible à la casse)
    QString idCol, nomCol, prenomCol, compCol, zoneCol;

    for (const QString &col : realColumns) {
        QString colUpper = col.toUpper();
        // CIN est la clé primaire (pas ID_PERSONNEL)
        if (colUpper == "CIN") {
            idCol = col;
        } else if (colUpper == "NOM" || colUpper == "NOMP") {
            nomCol = col;  // Utiliser le nom réel détecté (NOMP si c'est ce qui existe)
        } else if (colUpper == "PRENOM" || colUpper == "PRENOMP") {
            prenomCol = col;  // Utiliser le nom réel détecté (PRENOMP si c'est ce qui existe)
        } else if (colUpper.contains("COMPETENCE")) {
            compCol = col;
        } else if (colUpper.contains("ZONE") || colUpper.contains("AFFECTATION")) {
            // Utiliser le nom réel détecté (ZONE_D_AFFECTATION si c'est ce qui existe)
            if (zoneCol.isEmpty() || colUpper.contains("AFFECTATION")) {
                zoneCol = col;
            }
        }
    }

    // Si aucune colonne n'a été détectée, utiliser les noms par défaut
    if (idCol.isEmpty()) idCol = "CIN";  // Utiliser CIN comme clé primaire
    if (nomCol.isEmpty()) nomCol = "NOM";
    if (prenomCol.isEmpty()) prenomCol = "PRENOM";
    if (compCol.isEmpty()) compCol = "COMPETENCES";
    if (zoneCol.isEmpty()) zoneCol = "ZONE_AFFECTATION";

    qDebug() << "Colonnes utilisées - ID:" << idCol << "NOM:" << nomCol << "PRENOM:" << prenomCol
             << "COMP:" << compCol << "ZONE:" << zoneCol;

    // Si aucune table n'a été détectée, utiliser les valeurs par défaut
    if (tableName.isEmpty()) {
        tableName = "PERSONNEL";
    }

    // Vérifier si le CIN existe déjà
    QSqlQuery checkQuery;
    checkQuery.prepare(QString("SELECT 1 FROM %1 WHERE %2 = :CIN").arg(tableName, idCol));
    checkQuery.bindValue(":CIN", ID_PERSONNEL);

    if (!checkQuery.exec()) {
        // Si PERSONNEL échoue, essayer PERSONNELS
        if (tableName == "PERSONNEL") {
            tableName = "PERSONNELS";
            checkQuery.prepare(QString("SELECT 1 FROM %1 WHERE %2 = :CIN").arg(tableName, idCol));
            checkQuery.bindValue(":CIN", ID_PERSONNEL);
            if (!checkQuery.exec()) {
                QString error = checkQuery.lastError().text();
                qDebug() << "❌ Erreur lors de la vérification du CIN:" << error;
                // Continuer quand même pour l'insertion
            } else if (checkQuery.next()) {
                qDebug() << "⚠️ Un personnel avec le CIN" << ID_PERSONNEL << "existe déjà.";
                return false;
            }
        } else {
            QString error = checkQuery.lastError().text();
            qDebug() << "❌ Erreur lors de la vérification du CIN:" << error;
        }
    } else if (checkQuery.next()) {
        qDebug() << "⚠️ Un personnel avec le CIN" << ID_PERSONNEL << "existe déjà.";
        return false;
    }

    QSqlQuery query;
    query.prepare(QString("INSERT INTO %1 (%2, %3, %4, %5, %6) "
                          "VALUES (:CIN, :NOM, :PRENOM, :COMPETENCES, :ZONE_AFFECTATION)")
                      .arg(tableName, idCol, nomCol, prenomCol, compCol, zoneCol));

    query.bindValue(":CIN", ID_PERSONNEL);
    query.bindValue(":NOM", NOM);
    query.bindValue(":PRENOM", PRENOM);
    query.bindValue(":COMPETENCES", COMPETENCES);
    query.bindValue(":ZONE_AFFECTATION", ZONE_AFFECTATION);

    if (!query.exec()) {
        QString error = query.lastError().text();
        qDebug() << "❌ ERREUR SQL:" << error;
        qDebug() << "❌ Requête:" << query.lastQuery();
        return false;
    }

    qDebug() << "✅ Personnel ajouté avec succès - CIN:" << ID_PERSONNEL;
    return true;
}

QSqlQueryModel *personnel::afficher()
{
    QSqlQueryModel *model = new QSqlQueryModel();
    QStringList realColumns;
    QString tableName = detectTableName(realColumns);

    // Si aucune table n'a été détectée, essayer les deux
    if (tableName.isEmpty()) {
        tableName = "PERSONNEL";
    }

    // Essayer d'abord la table détectée
    QString queryStr = QString("SELECT * FROM %1 ORDER BY 1").arg(tableName);
    model->setQuery(queryStr);

    // Si la table détectée échoue, essayer l'autre
    if (model->lastError().isValid() && tableName == "PERSONNEL") {
        queryStr = "SELECT * FROM PERSONNELS ORDER BY 1";
        model->setQuery(queryStr);
    } else if (model->lastError().isValid() && tableName == "PERSONNELS") {
        queryStr = "SELECT * FROM PERSONNEL ORDER BY 1";
        model->setQuery(queryStr);
    }

    // Vérifier les erreurs
    if (model->lastError().isValid()) {
        qDebug() << "❌ Erreur lors de l'affichage:" << model->lastError().text();
        qDebug() << "❌ Requête:" << queryStr;
    } else {
        qDebug() << "✅ Affichage réussi - Nombre de lignes:" << model->rowCount();
        // Définir les en-têtes des colonnes basés sur les colonnes réelles
        if (model->columnCount() > 0) {
            for (int i = 0; i < model->columnCount(); i++) {
                QString header = model->headerData(i, Qt::Horizontal).toString();
                if (header.isEmpty()) {
                    // Utiliser le nom de la colonne réelle
                    QSqlRecord record = model->record();
                    if (i < record.count()) {
                        header = record.fieldName(i);
                    }
                }
                model->setHeaderData(i, Qt::Horizontal, header);
            }
        }
    }

    return model;
}

bool personnel::supprimer(QString ID)
{
    // Détecter les colonnes réelles de la table et le nom de la table
    QStringList realColumns;
    QString tableName = detectTableName(realColumns);

    // Trouver le nom de la colonne ID (CIN)
    QString idCol;
    for (const QString &col : realColumns) {
        QString colUpper = col.toUpper();
        if (colUpper == "CIN") {
            idCol = col;
            break;
        }
    }

    // Si aucune colonne n'a été détectée, utiliser le nom par défaut
    if (idCol.isEmpty()) idCol = "CIN";

    // Si aucune table n'a été détectée, utiliser les valeurs par défaut
    if (tableName.isEmpty()) {
        tableName = "PERSONNEL";
    }

    // Vérifier que le personnel existe
    QSqlQuery checkQuery;
    checkQuery.prepare(QString("SELECT 1 FROM %1 WHERE %2 = :ID").arg(tableName, idCol));
    checkQuery.bindValue(":ID", ID);

    if (!checkQuery.exec()) {
        // Si PERSONNEL échoue, essayer PERSONNELS
        if (tableName == "PERSONNEL") {
            tableName = "PERSONNELS";
            checkQuery.prepare(QString("SELECT 1 FROM %1 WHERE %2 = :ID").arg(tableName, idCol));
            checkQuery.bindValue(":ID", ID);
            if (!checkQuery.exec()) {
                qDebug() << "❌ Erreur lors de la vérification du CIN:" << checkQuery.lastError().text();
                QMessageBox::critical(nullptr, "Erreur SQL", checkQuery.lastError().text());
                return false;
            }
        } else {
            qDebug() << "❌ Erreur lors de la vérification du CIN:" << checkQuery.lastError().text();
            QMessageBox::critical(nullptr, "Erreur SQL", checkQuery.lastError().text());
            return false;
        }
    }

    if (!checkQuery.next()) {
        qDebug() << "⚠️ Aucun personnel avec le CIN" << ID << "n'existe.";
        QMessageBox::warning(nullptr, "Avertissement", QString("Le personnel avec le CIN '%1' n'existe pas.").arg(ID));
        return false;
    }

    QSqlQuery query;
    query.prepare(QString("DELETE FROM %1 WHERE %2 = :ID").arg(tableName, idCol));
    query.bindValue(":ID", ID);

    if (!query.exec()) {
        QString error = query.lastError().text();
        qDebug() << "❌ Erreur suppression:" << error;
        QMessageBox::critical(nullptr, "Erreur SQL", QString("Erreur lors de la suppression:\n%1").arg(error));
        return false;
    }

    qDebug() << "✅ Personnel supprimé avec succès - CIN:" << ID;
    return true;
}

bool personnel::modifier()
{
    // Détecter les colonnes réelles de la table et le nom de la table
    QStringList realColumns;
    QString tableName = detectTableName(realColumns);

    // Trouver les noms de colonnes réels
    QString idCol, nomCol, prenomCol, compCol, zoneCol;

    for (const QString &col : realColumns) {
        QString colUpper = col.toUpper();
        // CIN est la clé primaire
        if (colUpper == "CIN") {
            idCol = col;
        } else if (colUpper == "NOM" || colUpper == "NOMP") {
            nomCol = col;
        } else if (colUpper == "PRENOM" || colUpper == "PRENOMP") {
            prenomCol = col;
        } else if (colUpper.contains("COMPETENCE")) {
            compCol = col;
        } else if (colUpper.contains("ZONE") || colUpper.contains("AFFECTATION")) {
            // Utiliser le nom réel détecté (ZONE_D_AFFECTATION si c'est ce qui existe)
            if (zoneCol.isEmpty() || colUpper.contains("AFFECTATION")) {
                zoneCol = col;
            }
        }
    }

    // Si aucune colonne n'a été détectée, utiliser les noms par défaut
    if (idCol.isEmpty()) idCol = "CIN";
    if (nomCol.isEmpty()) nomCol = "NOM";
    if (prenomCol.isEmpty()) prenomCol = "PRENOM";
    if (compCol.isEmpty()) compCol = "COMPETENCES";
    if (zoneCol.isEmpty()) zoneCol = "ZONE_AFFECTATION";

    qDebug() << "Modification - Colonnes utilisées - ID:" << idCol << "NOM:" << nomCol << "PRENOM:" << prenomCol
             << "COMP:" << compCol << "ZONE:" << zoneCol;

    // Si aucune table n'a été détectée, utiliser les valeurs par défaut
    if (tableName.isEmpty()) {
        tableName = "PERSONNEL";
    }

    // Vérifier que le personnel existe
    QSqlQuery checkQuery;
    checkQuery.prepare(QString("SELECT 1 FROM %1 WHERE %2 = :CIN").arg(tableName, idCol));
    checkQuery.bindValue(":CIN", ID_PERSONNEL);

    if (!checkQuery.exec()) {
        // Si PERSONNEL échoue, essayer PERSONNELS
        if (tableName == "PERSONNEL") {
            tableName = "PERSONNELS";
            checkQuery.prepare(QString("SELECT 1 FROM %1 WHERE %2 = :CIN").arg(tableName, idCol));
            checkQuery.bindValue(":CIN", ID_PERSONNEL);
            if (!checkQuery.exec()) {
                QString error = checkQuery.lastError().text();
                qDebug() << "❌ Erreur lors de la vérification du CIN:" << error;
                qDebug() << "❌ Table:" << tableName << "Colonne:" << idCol;
                QMessageBox::critical(nullptr, "Erreur SQL", QString("Erreur lors de la vérification:\n%1").arg(error));
                return false;
            }
        } else {
            QString error = checkQuery.lastError().text();
            qDebug() << "❌ Erreur lors de la vérification du CIN:" << error;
            qDebug() << "❌ Table:" << tableName << "Colonne:" << idCol;
            QMessageBox::critical(nullptr, "Erreur SQL", QString("Erreur lors de la vérification:\n%1").arg(error));
            return false;
        }
    }

    if (!checkQuery.next()) {
        qDebug() << "⚠️ Aucun personnel avec le CIN" << ID_PERSONNEL << "n'existe.";
        QMessageBox::warning(nullptr, "Avertissement", QString("Le personnel avec le CIN '%1' n'existe pas.").arg(ID_PERSONNEL));
        return false;
    }

    QSqlQuery query;
    query.prepare(QString("UPDATE %1 SET %2 = :NOM, %3 = :PRENOM, %4 = :COMPETENCES, %5 = :ZONE_AFFECTATION "
                          "WHERE %6 = :CIN")
                      .arg(tableName, nomCol, prenomCol, compCol, zoneCol, idCol));

    query.bindValue(":CIN", ID_PERSONNEL);
    query.bindValue(":NOM", NOM);
    query.bindValue(":PRENOM", PRENOM);
    query.bindValue(":COMPETENCES", COMPETENCES);
    query.bindValue(":ZONE_AFFECTATION", ZONE_AFFECTATION);

    if (!query.exec()) {
        QString error = query.lastError().text();
        qDebug() << "❌ ERREUR SQL:" << error;
        qDebug() << "❌ Requête:" << query.lastQuery();
        qDebug() << "❌ Table:" << tableName;
        qDebug() << "❌ Colonnes - NOM:" << nomCol << "PRENOM:" << prenomCol << "COMP:" << compCol << "ZONE:" << zoneCol << "ID:" << idCol;
        QMessageBox::critical(nullptr, "Erreur SQL", QString("Erreur lors de la modification:\n%1\n\nRequête: %2").arg(error, query.lastQuery()));
        return false;
    }

    qDebug() << "✅ Personnel modifié avec succès - CIN:" << ID_PERSONNEL;
    return true;
}

