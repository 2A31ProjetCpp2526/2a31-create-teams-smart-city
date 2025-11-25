#include "consomation.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QTableView>
#include <QMessageBox>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QDateTime>
#include <QHeaderView>
#include <QAbstractItemView>
#include <QObject>
#include <QStringList>
#include <QSqlQueryModel>
#include <QSqlRecord>

static constexpr const char *COL_DISTANCE = "DISTANCE_KM";
static constexpr const char *COL_CONSO = "CONSO_ENTREE";
static constexpr const char *COL_RESULTAT = "RESULTAT_CALCULE";
static constexpr const char *COL_DATE = "DATE_SAISIE";

consomation::consomation() : idVehicule(0), distance(0.0), consoEntree(0.0), resultatCalcule(0.0) {}

consomation::consomation(int idVehicule, QString matricule, double distance, double consoEntree, double resultatCalcule)
    : idVehicule(idVehicule), matricule(matricule), distance(distance), 
      consoEntree(consoEntree), resultatCalcule(resultatCalcule) {}

bool consomation::validerChamps(QString &messageErreur)
{
    messageErreur.clear();
    QStringList erreurs;

    if (idVehicule <= 0) {
        erreurs << "ID véhicule invalide.";
    }

    if (matricule.trimmed().isEmpty()) {
        erreurs << "Matricule obligatoire.";
    }

    if (distance <= 0) {
        erreurs << "Distance doit être supérieure à zéro.";
    }

    if (consoEntree <= 0) {
        erreurs << "Consommation entrée doit être supérieure à zéro.";
    }

    if (resultatCalcule < 0) {
        erreurs << "Résultat calculé invalide.";
    }

    if (!erreurs.isEmpty()) {
        messageErreur = erreurs.join("\n");
        return false;
    }

    return true;
}

bool consomation::ajouter(QString *messageErreur)
{
    QString messageErreurLocal;
    if (!validerChamps(messageErreurLocal)) {
        if (messageErreur) {
            *messageErreur = messageErreurLocal;
        }
        qDebug() << "Validation échouée:" << messageErreurLocal;
        return false;
    }

    QSqlQuery q;
    q.prepare(QStringLiteral("INSERT INTO CONSOMMATION "
                             "(ID_VEHICULE, MATRICULE, %1, %2, %3, %4) "
                             "VALUES (:id, :mat, :dist, :conso, :result, SYSDATE)")
                  .arg(COL_DISTANCE)
                  .arg(COL_CONSO)
                  .arg(COL_RESULTAT)
                  .arg(COL_DATE));
    q.bindValue(":id", idVehicule);
    q.bindValue(":mat", matricule.trimmed());
    q.bindValue(":dist", distance);
    q.bindValue(":conso", consoEntree);
    q.bindValue(":result", resultatCalcule);

    if (!q.exec()) {
        if (messageErreur) {
            *messageErreur = q.lastError().text();
        } else {
            qDebug() << "Erreur insertion consommation:" << q.lastError().text();
        }
        return false;
    }

    qDebug() << "✅ Consommation ajoutée avec succès - ID Véhicule:" << idVehicule;
    return true;
}

bool consomation::modifier(int idVehicule)
{
    QString messageErreur;
    if (!validerChamps(messageErreur)) {
        return false;
    }

    QSqlQuery q;
    q.prepare("UPDATE CONSOMMATION SET MATRICULE = :mat, DISTANCE = :dist, CONSO_ENTREE = :conso, "
              "RESULTAT_CALCULE = :result WHERE ID_VEHICULE = :id");
    q.bindValue(":id", idVehicule);
    q.bindValue(":mat", matricule.trimmed());
    q.bindValue(":dist", distance);
    q.bindValue(":conso", consoEntree);
    q.bindValue(":result", resultatCalcule);

    if (!q.exec()) {
        qDebug() << "Erreur modification consommation:" << q.lastError().text();
        return false;
    }

    return true;
}

bool consomation::supprimer(int idVehicule)
{
    QSqlQuery q;
    q.prepare("DELETE FROM CONSOMMATION WHERE ID_VEHICULE = :id");
    q.bindValue(":id", idVehicule);

    if (!q.exec()) {
        qDebug() << "Erreur suppression consommation:" << q.lastError().text();
        return false;
    }

    return true;
}

QSqlQueryModel* consomation::afficher(QObject *parent)
{
    QString sql = QStringLiteral(
        "SELECT ID_VEHICULE, MATRICULE, %1, %2, %3, %4 "
        "FROM CONSOMMATION ORDER BY %4 DESC")
                      .arg(COL_DISTANCE)
                      .arg(COL_CONSO)
                      .arg(COL_RESULTAT)
                      .arg(COL_DATE);

    QSqlQueryModel *model = new QSqlQueryModel(parent);
    model->setQuery(sql);

    if (model->lastError().isValid()) {
        qWarning() << "Erreur affichage consommation:" << model->lastError().text();
        delete model;
        return nullptr;
    }

    model->setHeaderData(0, Qt::Horizontal, "ID Véhicule");
    model->setHeaderData(1, Qt::Horizontal, "Matricule");
    model->setHeaderData(2, Qt::Horizontal, "Distance (km)");
    model->setHeaderData(3, Qt::Horizontal, "Conso entrée (L/kWh)");
    model->setHeaderData(4, Qt::Horizontal, "Résultat calculé");
    model->setHeaderData(5, Qt::Horizontal, "Date saisie");

    return model;
}

// ===================== ConsomationModule =====================

ConsomationModule::ConsomationModule(QWidget *root)
    : m_root(root)
{
    initTable();

    if (!m_root) return;

    if (auto btn = m_root->findChild<QPushButton*>("btnCalculerConso")) {
        QObject::connect(btn, &QPushButton::clicked, btn, [this]() { calculer(); });
    }
    if (auto btn = m_root->findChild<QPushButton*>("btn_effacer_conso")) {
        QObject::connect(btn, &QPushButton::clicked, btn, [this]() { effacer(); });
    }
}

void ConsomationModule::initTable()
{
    if (!m_root) return;
    if (auto tableView = consommationTable()) {
        if (QSqlQueryModel *model = consomation::afficher(tableView)) {
            tableView->setModel(model);
        } else {
            QStandardItemModel *localModel = new QStandardItemModel(0, 6, tableView);
            localModel->setHeaderData(0, Qt::Horizontal, "ID Véhicule");
            localModel->setHeaderData(1, Qt::Horizontal, "Matricule");
            localModel->setHeaderData(2, Qt::Horizontal, "Distance (km)");
            localModel->setHeaderData(3, Qt::Horizontal, "Conso entrée (L/kWh)");
            localModel->setHeaderData(4, Qt::Horizontal, "Résultat calculé");
            localModel->setHeaderData(5, Qt::Horizontal, "Date saisie");
            tableView->setModel(localModel);
        }
        applyTableStyle(tableView);
        tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
        tableView->setSelectionMode(QAbstractItemView::SingleSelection);
        tableView->setSortingEnabled(true);
        tableView->resizeColumnsToContents();
    }
}

void ConsomationModule::applyTableStyle(QTableView *tableView)
{
    if (!tableView) return;
    tableView->setStyleSheet(
        "QTableView {"
        "  color: #000000;"
        "  background-color: #ffffff;"
        "  alternate-background-color: #f2f6fc;"
        "  gridline-color: #dbe3ec;"
        "  border: 1px solid #cfd9e6;"
        "  border-radius: 8px;"
        "  selection-background-color: #c7e0ff;"
        "  selection-color: #000000;"
        "}"
        "QHeaderView::section {"
        "  background-color: #0c2d57;"
        "  color: #ffffff;"
        "  padding: 6px;"
        "  border: none;"
        "  font-weight: 600;"
        "  font-size: 12px;"
        "}"
        "QTableCornerButton::section {"
        "  background-color: #0c2d57;"
        "}"
    );
}

QTableView* ConsomationModule::consommationTable() const
{
    return m_root ? m_root->findChild<QTableView*>("tableViewConso") : nullptr;
}

bool ConsomationModule::vehiculeExiste(int id, const QString &matricule) const
{
    if (id <= 0 || matricule.trimmed().isEmpty()) return false;

    QSqlQuery checkVehicule;
    checkVehicule.prepare("SELECT COUNT(*) FROM VEHICULE WHERE ID_VEHICULE = :id AND MATRICULE = :matricule");
    checkVehicule.bindValue(":id", id);
    checkVehicule.bindValue(":matricule", matricule.trimmed());
    if (checkVehicule.exec() && checkVehicule.next()) {
        return checkVehicule.value(0).toInt() > 0;
    }
    return false;
}

namespace {
QLineEdit *findLineEdit(QWidget *root, const QStringList &names)
{
    if (!root) return nullptr;
    for (const QString &name : names) {
        if (auto le = root->findChild<QLineEdit*>(name)) {
            return le;
        }
    }
    return nullptr;
}
}

void ConsomationModule::calculer()
{
    if (!m_root) return;

    auto idEdit = findLineEdit(m_root, {"lineIDvehicule", "lineIDVehicule", "lineEdit_id_vehicule"});
    auto matriculeEdit = findLineEdit(m_root, {"lineMatricule", "lineEdit_matricule"});
    auto distanceEdit = findLineEdit(m_root, {"lineDistance", "lineEdit_distance", "lineEdit_3"});
    auto consoEdit = findLineEdit(m_root, {"lineConso", "lineEdit_conso"});
    auto resultLabel = m_root->findChild<QLabel*>("label_conso_resultat");
    if (!resultLabel) resultLabel = m_root->findChild<QLabel*>("labelResultatConso");
    if (!resultLabel) resultLabel = m_root->findChild<QLabel*>("label_132");

    if (!idEdit || !matriculeEdit || !distanceEdit || !consoEdit || !resultLabel) {
        QStringList missing;
        if (!idEdit) missing << "ID";
        if (!matriculeEdit) missing << "Matricule";
        if (!distanceEdit) missing << "Distance";
        if (!consoEdit) missing << "Carburant";
        if (!resultLabel) missing << "Résultat";
        QMessageBox::warning(m_root, "Interface incomplète",
                             QString("Widgets manquants: %1.\nVérifiez les objectName dans Qt Designer.")
                             .arg(missing.join(", ")));
        return;
    }

    QString distanceStr = distanceEdit->text().trimmed();
    QString consoStr = consoEdit->text().trimmed();
    if (distanceStr.isEmpty() || consoStr.isEmpty()) {
        QMessageBox::warning(m_root, "Champs vides", "Veuillez remplir la distance et le carburant consommé.");
        return;
    }

    bool ok1 = false;
    bool ok2 = false;
    double distance = distanceStr.toDouble(&ok1);
    double conso = consoStr.toDouble(&ok2);
    if (!ok1 || !ok2 || distance <= 0 || conso <= 0) {
        QMessageBox::warning(m_root, "Valeurs invalides", "Distance et carburant doivent être des valeurs numériques positives.");
        return;
    }

    double resultat = (conso / distance) * 100.0;
    QString resultatStr = QString::number(resultat, 'f', 2) + " L/100km";
    if (resultLabel) {
        resultLabel->setText(resultatStr);
    }

    QString idStr = idEdit ? idEdit->text().trimmed() : QString();
    QString matricule = matriculeEdit ? matriculeEdit->text().trimmed() : QString();

    if (idStr.isEmpty() || matricule.isEmpty()) {
        QMessageBox::warning(m_root, "Informations manquantes",
                             "Veuillez saisir l'ID du véhicule et son matricule avant de calculer.");
        return;
    }

    bool okId = false;
    int id = idStr.toInt(&okId);
    if (!okId || id <= 0) {
        QMessageBox::warning(m_root, "ID Véhicule invalide", "L'ID du véhicule doit être un entier positif.");
        return;
    }

    if (!vehiculeExiste(id, matricule)) {
        QMessageBox::warning(m_root, "Véhicule inexistant",
                             "Aucun véhicule avec cet ID et ce matricule n'existe dans la base.");
        return;
    }

    consomation consoObj(id, matricule, distance, conso, resultat);
    QString erreur;
    if (!consoObj.validerChamps(erreur)) {
        QMessageBox::warning(m_root, "Erreur de validation", erreur);
        return;
    }

    QString erreurAjout;
    bool enregistre = consoObj.ajouter(&erreurAjout);
    if (!enregistre) {
        QString msg = "Échec de l'enregistrement de la consommation.";
        if (!erreurAjout.isEmpty()) {
            msg += "\n\nDétail: " + erreurAjout;
        }
        msg += "\nLes données seront affichées localement.";
        QMessageBox::warning(m_root, "Erreur base de données", msg);
    }

    QTableView *tableView = consommationTable();
    if (!tableView) {
        QMessageBox::warning(m_root, "Table manquante", "Le tableau de consommation est introuvable.");
        return;
    }

    if (enregistre) {
        if (QSqlQueryModel *dbModel = consomation::afficher(tableView)) {
            tableView->setModel(dbModel);
            applyTableStyle(tableView);
            tableView->resizeColumnsToContents();
            tableView->setAlternatingRowColors(true);
        }
    } else {
        remplirModelLocal(tableView);
        QString dateStr = QDateTime::currentDateTime().toString("dd/MM/yyyy HH:mm");
        ajouterLigneLocale(tableView, idStr, matricule, distanceStr, consoStr, resultatStr, dateStr);
    }

    QMessageBox::information(m_root, "Calcul réussi",
                             QString("Consommation calculée: %1").arg(resultatStr));
}

void ConsomationModule::effacer()
{
    if (!m_root) return;

    auto idEdit = findLineEdit(m_root, {"lineIDvehicule", "lineIDVehicule", "lineEdit_id_vehicule"});
    auto matriculeEdit = findLineEdit(m_root, {"lineMatricule", "lineEdit_matricule"});
    auto distanceEdit = findLineEdit(m_root, {"lineDistance", "lineEdit_distance", "lineEdit_3"});
    auto consoEdit = findLineEdit(m_root, {"lineConso", "lineEdit_conso"});
    auto resultLabel = m_root->findChild<QLabel*>("label_conso_resultat");
    if (!resultLabel) resultLabel = m_root->findChild<QLabel*>("labelResultatConso");
    if (!resultLabel) resultLabel = m_root->findChild<QLabel*>("label_132");

    if (idEdit) idEdit->clear();
    if (matriculeEdit) matriculeEdit->clear();
    if (distanceEdit) distanceEdit->clear();
    if (consoEdit) consoEdit->clear();
    if (resultLabel) resultLabel->clear();
}

void ConsomationModule::remplirModelLocal(QTableView *tableView)
{
    if (!tableView) return;
    if (qobject_cast<QStandardItemModel*>(tableView->model())) {
        return;
    }

    QStandardItemModel *model = new QStandardItemModel(tableView);
    model->setColumnCount(6);
    model->setHeaderData(0, Qt::Horizontal, "ID Véhicule");
    model->setHeaderData(1, Qt::Horizontal, "Matricule");
    model->setHeaderData(2, Qt::Horizontal, "Distance (km)");
    model->setHeaderData(3, Qt::Horizontal, "Conso entrée (L/kWh)");
    model->setHeaderData(4, Qt::Horizontal, "Résultat calculé");
    model->setHeaderData(5, Qt::Horizontal, "Date saisie");

    QAbstractItemModel *oldModel = tableView->model();
    if (oldModel) {
        for (int r = 0; r < oldModel->rowCount(); ++r) {
            QList<QStandardItem*> rowItems;
            for (int c = 0; c < qMin(oldModel->columnCount(), 6); ++c) {
                rowItems << new QStandardItem(oldModel->index(r, c).data().toString());
            }
            while (rowItems.size() < 6) {
                rowItems << new QStandardItem();
            }
            model->appendRow(rowItems);
        }
    }
    tableView->setModel(model);
    applyTableStyle(tableView);
    tableView->setAlternatingRowColors(true);
}

void ConsomationModule::ajouterLigneLocale(QTableView *tableView,
                                           const QString &idStr,
                                           const QString &matricule,
                                           const QString &distanceStr,
                                           const QString &consoStr,
                                           const QString &resultatStr,
                                           const QString &dateStr)
{
    if (!tableView) return;
    QStandardItemModel *model = qobject_cast<QStandardItemModel*>(tableView->model());
    if (!model) {
        remplirModelLocal(tableView);
        model = qobject_cast<QStandardItemModel*>(tableView->model());
        if (!model) return;
    }

    int row = model->rowCount();
    model->insertRow(row);
    model->setItem(row, 0, new QStandardItem(idStr));
    model->setItem(row, 1, new QStandardItem(matricule));
    model->setItem(row, 2, new QStandardItem(distanceStr));
    model->setItem(row, 3, new QStandardItem(consoStr));
    model->setItem(row, 4, new QStandardItem(resultatStr));
    model->setItem(row, 5, new QStandardItem(dateStr));
    tableView->resizeColumnsToContents();
    tableView->scrollToBottom();
    tableView->setAlternatingRowColors(true);
}

