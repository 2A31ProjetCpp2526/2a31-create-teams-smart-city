#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QHeaderView>
#include <QPushButton>
#include <QMessageBox>
#include <QDebug>
#include <QInputDialog>
#include <QSqlQueryModel>
#include <QSqlQuery>
#include "zone.h"
#include "poubelle.h"
#include "habitantcrud.h"
#include "vehiculecrud.h"
#include <QSqlError>
#include <QSqlQueryModel>
#include <QCompleter>
#include <QListView>
#include <QLineEdit>
#include <QSettings>
#include <QItemSelectionModel>
#include <QRegularExpression>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    qApp->setStyleSheet(
        "QMessageBox QLabel, QInputDialog QLabel, QLineEdit { color: black; }"
        "QMessageBox QPushButton, QInputDialog QPushButton { color: black; }"
        );

    setupTable();

    // === Habitant helpers (validators, styles, statuses, data load) ===
    chargerStatutsAutorises();
    styliserChampsSaisie();
    chargerDonneesTable();
    loadLastHabitantSelection();

    // Recherche live
    connect(ui->lineEdit_rechH, &QLineEdit::textChanged, this, [this](const QString &t) {
        rechercherTexte(t);
    });

    // Validators
    ui->lineEdit_id->setValidator(new QIntValidator(1, 2147483647, this));
    ui->lineEdit_satisfaction->setValidator(new QIntValidator(0, 100, this));
    ui->lineEdit_contact->setValidator(new QRegularExpressionValidator(QRegularExpression("^\\d{0,15}$"), this));

    // Options de tri
    if (ui->comboBox_trieH) {
        ui->comboBox_trieH->clear();
        ui->comboBox_trieH->addItems(QStringList() << "Nom" << "Prénom" << "Adresse" << "Satisfaction");
        ui->comboBox_trieH->setCurrentIndex(0);
    }
    connect(ui->affzone, &QPushButton::clicked, this, &MainWindow::afficherZones);
    connect(ui->affpoub, &QPushButton::clicked, this, &MainWindow::afficherPoubelles);
    connect(ui->modzone, &QPushButton::clicked, this, &MainWindow::modZone);
    connect(ui->modpoub, &QPushButton::clicked, this, &MainWindow::modPoubelle);
}

MainWindow::~MainWindow()
{
    delete ui;
}

// === Helpers Habitant ===
void MainWindow::chargerDonneesTable()
{
    HabitantCRUD h;
    QSqlQueryModel *model = h.afficher(this);
    ui->tableView_habitants->setModel(model);
    ui->tableView_habitants->resizeColumnsToContents();
}

void MainWindow::chargerStatutsAutorises()
{
    allowedStatuses.clear();
    {
        QSqlQuery q;
        q.prepare("SELECT search_condition FROM user_constraints WHERE table_name = 'HABITANT' AND constraint_name = 'CK_STATUT_HABITANT'");
        if (q.exec() && q.next()) {
            const QString condition = q.value(0).toString();
            QRegularExpression re("'([^']+)'", QRegularExpression::CaseInsensitiveOption);
            QRegularExpressionMatchIterator it = re.globalMatch(condition);
            while (it.hasNext()) {
                const auto m = it.next();
                const QString v = m.captured(1).trimmed();
                if (!v.isEmpty()) allowedStatuses << v;
            }
        }
    }
    if (allowedStatuses.isEmpty()) {
        QSqlQuery q2("SELECT DISTINCT STATUT FROM HABITANT WHERE STATUT IS NOT NULL");
        while (q2.next()) {
            const QString v = q2.value(0).toString().trimmed();
            if (!v.isEmpty()) allowedStatuses << v;
        }
    }

    // Fallback: si rien n'a été trouvé en base, proposer une liste par défaut
    if (allowedStatuses.isEmpty()) {
        allowedStatuses << "Actif" << "Inactif" << "En_attente";
    }

    allowedStatuses.removeDuplicates();
    if (!allowedStatuses.isEmpty()) {
        QCompleter *comp = new QCompleter(allowedStatuses, this);
        comp->setCaseSensitivity(Qt::CaseInsensitive);
        comp->setFilterMode(Qt::MatchContains);
        QListView *popup = new QListView();
        popup->setStyleSheet("QListView { background: #ffffff; color: #111111; }\nQListView::item:selected { background: #e6f0ff; color: #111111; }");
        comp->setPopup(popup);
        ui->lineEdit_statut->setCompleter(comp);
        const QString hint = QString("Valeurs autorisées: %1").arg(allowedStatuses.join(", "));
        ui->lineEdit_statut->setPlaceholderText(hint);
        ui->lineEdit_statut->setToolTip(hint);
    }
}

bool MainWindow::statutAutorise(const QString &value) const
{
    // Si on n'a pas pu récupérer la liste autorisée depuis la base,
    // on bloque par sécurité pour éviter l'erreur ORA-02290 côté Oracle.
    if (allowedStatuses.isEmpty()) return false;
    for (const QString &v : allowedStatuses) {
        if (v.compare(value.trimmed(), Qt::CaseInsensitive) == 0) return true;
    }
    return false;
}

void MainWindow::styliserChampsSaisie()
{
    const QString lineEditCss =
        "QLineEdit {"
        "  color: #111111;"
        "  background-color: #ffffff;"
        "  border: 1px solid #5dade2;"
        "  selection-background-color: #cfe3ff;"
        "  selection-color: #111111;"
        "}";

    QVector<QLineEdit*> edits = {
        ui->lineEdit_id,
        ui->lineEdit_nom,
        ui->lineEdit_prenom,
        ui->lineEdit_adresse,
        ui->lineEdit_zone,
        ui->lineEdit_situation,
        ui->lineEdit_contact,
        ui->lineEdit_statut,
        ui->lineEdit_besoins,
        ui->lineEdit_satisfaction
    };
    for (QLineEdit *e : edits) {
        if (!e) continue;
        e->setStyleSheet(lineEditCss);
        QPalette pal = e->palette();
        pal.setColor(QPalette::Text, QColor("#111111"));
        pal.setColor(QPalette::PlaceholderText, QColor("#6c757d"));
        e->setPalette(pal);
    }
}

void MainWindow::saveLastHabitantId(int id)
{
    QSettings s("Touha", "Ghabitants");
    s.setValue("last_habitant_id", id);
}

void MainWindow::loadLastHabitantSelection()
{
    QSettings s("Touha", "Ghabitants");
    bool ok = false;
    int lastId = s.value("last_habitant_id").toInt(&ok);
    if (!ok || lastId <= 0 || !ui->tableView_habitants || !ui->tableView_habitants->model()) return;
    QAbstractItemModel *m = ui->tableView_habitants->model();
    int targetRow = -1;
    for (int r = 0; r < m->rowCount(); ++r) {
        const QVariant v = m->index(r, 0).data();
        if (v.toInt() == lastId) { targetRow = r; break; }
    }
    if (targetRow < 0) return;
    QItemSelectionModel *sel = ui->tableView_habitants->selectionModel();
    if (!sel) return;
    sel->select(m->index(targetRow, 0), QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);
    ui->tableView_habitants->scrollTo(m->index(targetRow, 0), QAbstractItemView::PositionAtCenter);
    populateEditsFromRow(targetRow);
}

void MainWindow::populateEditsFromRow(int row)
{
    QAbstractItemModel *m = ui->tableView_habitants->model();
    if (!m || row < 0 || row >= m->rowCount()) return;
    ui->lineEdit_id->setText(m->index(row, 0).data().toString());
    ui->lineEdit_nom->setText(m->index(row, 1).data().toString());
    ui->lineEdit_prenom->setText(m->index(row, 2).data().toString());
    ui->lineEdit_adresse->setText(m->index(row, 3).data().toString());
    ui->lineEdit_zone->setText(m->index(row, 4).data().toString());
    ui->lineEdit_situation->setText(m->index(row, 5).data().toString());
    ui->lineEdit_contact->setText(m->index(row, 6).data().toString());
    ui->lineEdit_statut->setText(m->index(row, 7).data().toString());
    ui->lineEdit_besoins->setText(m->index(row, 8).data().toString());
    ui->lineEdit_satisfaction->setText(m->index(row, 9).data().toString());
}

void MainWindow::rechercherTexte(const QString &texte)
{
    const QString critere = texte.trimmed();
    if (critere.isEmpty()) { chargerDonneesTable(); return; }
    QString like = critere;
    QSqlQueryModel *model = new QSqlQueryModel(this);
    model->setQuery(QString(
        "SELECT ID_HABITANT, NOM, PRENOM, ADRESSE, ZONE_RESIDENCE, SITUATION_FAMILIALE, CONTACT, STATUT, BESOINS_SPECIFIQUES, NIVEAU_SATISFACTION "
        "FROM HABITANT WHERE NOM LIKE '%%1%' OR PRENOM LIKE '%%1%' OR ADRESSE LIKE '%%1%' OR ZONE_RESIDENCE LIKE '%%1%' OR STATUT LIKE '%%1%' OR CONTACT LIKE '%%1%'"
    ).arg(like.replace("'", "''")));
    ui->tableView_habitants->setModel(model);
    ui->tableView_habitants->resizeColumnsToContents();
}
void MainWindow::setupTable()
{
    QTableWidget *table = ui->tableWidget_6;

    table->setSortingEnabled(true);
    table->horizontalHeader()->setSortIndicatorShown(true);
    table->horizontalHeader()->setSectionsClickable(true);

    int currentCols = table->columnCount();
    bool hasDeleteColumn = false;

    if (currentCols > 0 && table->horizontalHeaderItem(currentCols - 1))
    {
        QString lastHeader = table->horizontalHeaderItem(currentCols - 1)->text();
        if (lastHeader == " ") hasDeleteColumn = true;
    }

    if (!hasDeleteColumn)
    {
        table->insertColumn(currentCols);
        table->setHorizontalHeaderItem(currentCols, new QTableWidgetItem(" "));
    }

    for (int row = 0; row < table->rowCount(); ++row)
    {
        QPushButton *btnSupprimer = new QPushButton("❌");
        btnSupprimer->setFixedSize(30, 25);
        btnSupprimer->setStyleSheet(
            "QPushButton { "
            "background-color: #b0000; "
            "color: white; "
            "border-radius: 6px; "
            "font-weight: bold;"
            "} "
            "QPushButton:hover { background-color: #ff4444; }"
            );
        connect(btnSupprimer, &QPushButton::clicked, this, &MainWindow::supprimerEtDecalerLignes);
        table->setCellWidget(row, table->columnCount() - 1, btnSupprimer);
    }

    table->resizeColumnsToContents();
}

void MainWindow::supprimerEtDecalerLignes()
{
    QTableWidget *table = ui->tableWidget_6;
    QPushButton *button = qobject_cast<QPushButton*>(sender());
    if (!button) return;

    int deleteRow = -1;

    for (int i = 0; i < table->rowCount(); ++i)
    {
        if (table->cellWidget(i, table->columnCount() - 1) == button)
        {
            deleteRow = i;
            break;
        }
    }

    if (deleteRow == -1) return;

    table->removeRow(deleteRow);

    int deleteCol = table->columnCount() - 1;
    for (int row = 0; row < table->rowCount(); ++row)
    {
        QWidget *widget = table->cellWidget(row, deleteCol);
        if (!widget)
        {
            QPushButton *btnSupprimer = new QPushButton("❌");
            btnSupprimer->setFixedSize(30, 25);
            btnSupprimer->setStyleSheet(
                "QPushButton { background-color: #b0000; color: white; border-radius: 6px; font-weight: bold; }"
                "QPushButton:hover { background-color: #ff4444; }"
                );
            connect(btnSupprimer, &QPushButton::clicked, this, &MainWindow::supprimerEtDecalerLignes);
            table->setCellWidget(row, deleteCol, btnSupprimer);
        }
    }
    QPushButton *btn = qobject_cast<QPushButton*>(sender());
    if (!btn) return;

    //QTableWidget *table = ui->tableWidget_6;
    for (int row = 0; row < table->rowCount(); ++row) {
        if (table->cellWidget(row, table->columnCount() - 1) == btn) {
            table->removeRow(row);
            break;
        }
    }
}

// ==================== Stacked Widgets Navigation ====================
void MainWindow::on_pushButton_clicked() { ui->stackedWidget_2->setCurrentIndex(0); }
void MainWindow::on_pushButton2_clicked() { ui->stackedWidget_2->setCurrentIndex(1); }
void MainWindow::on_pushButton3_clicked() { ui->stackedWidget_2->setCurrentIndex(2); }

void MainWindow::on_on_pushButton_clicked() { ui->stackedWidget_2->setCurrentIndex(0); }
void MainWindow::on_on_pushButton_2_clicked() { ui->stackedWidget_2->setCurrentIndex(1); }
void MainWindow::on_on_pushButton_3_clicked() { ui->stackedWidget_2->setCurrentIndex(2); }

void MainWindow::on_Ghabitant_clicked() { ui->stackedWidget->setCurrentIndex(0); }
void MainWindow::on_Ghabitats_2_clicked() { ui->stackedWidget->setCurrentIndex(1); }
void MainWindow::on_Gpersonnels_2_clicked() { ui->stackedWidget->setCurrentIndex(2); }
void MainWindow::on_Gequipements_2_clicked() { ui->stackedWidget->setCurrentIndex(4); }
void MainWindow::on_Gvehicules_2_clicked() { ui->stackedWidget->setCurrentIndex(5); }
void MainWindow::on_Gespace_clicked() { ui->stackedWidget->setCurrentIndex(3); }

void MainWindow::on_on_pushButton_4_clicked() { ui->stackedWidget_5->setCurrentIndex(1); }
void MainWindow::on_on_pushButton_5_clicked() { ui->stackedWidget_5->setCurrentIndex(2); }
void MainWindow::on_on_pushButton_6_clicked() { ui->stackedWidget_5->setCurrentIndex(0); }

void MainWindow::on_on_pushButton_7_clicked() { ui->stackedWidget_6->setCurrentIndex(0); }
void MainWindow::on_on_pushButton_8_clicked() { ui->stackedWidget_6->setCurrentIndex(1); }
void MainWindow::on_on_pushButton_9_clicked() { ui->stackedWidget_6->setCurrentIndex(2); }

void MainWindow::on_on_pushButton_13_clicked() { ui->stackedWidget_2->setCurrentIndex(0); }
/*void MainWindow::on_on_pushButton_14_clicked() { ui->stackedWidget_2->setCurrentIndex(1); }
void MainWindow::on_on_pushButton_15_clicked() { ui->stackedWidget_2->setCurrentIndex(2); }
void MainWindow::on_on_pushButton_16_clicked() { ui->stackedWidget_7->setCurrentIndex(0); }
void MainWindow::on_on_pushButton_17_clicked() { ui->stackedWidget_7->setCurrentIndex(1); }
void MainWindow::on_pushButton_15_clicked() { ui->stackedWidget_7->setCurrentIndex(2); }
*/

void MainWindow::on_gespub_2_clicked() { ui->stackedWidget_3->setCurrentIndex(0); }
void MainWindow::on_geszone1_clicked() { ui->stackedWidget_3->setCurrentIndex(1); }

/*void MainWindow::on_pushButton_30_clicked()
{
    QTableWidget *table = ui->tableWidget_6;
    int newRow = table->rowCount();
    table->insertRow(newRow);

    int deleteCol = table->columnCount() - 1;
    for (int col = 0; col < deleteCol; ++col)
        table->setItem(newRow, col, new QTableWidgetItem(""));

    QPushButton *btnSupprimer = new QPushButton("❌");
    btnSupprimer->setFixedSize(30, 25);
    btnSupprimer->setStyleSheet(
        "QPushButton { background-color: #b0000; color: white; border-radius: 6px; font-weight: bold; }"
        "QPushButton:hover { background-color: #ff4444; }"
        );

    connect(btnSupprimer, &QPushButton::clicked, this, &MainWindow::supprimerEtDecalerLignes);
    table->setCellWidget(newRow, deleteCol, btnSupprimer);
    table->resizeColumnsToContents();
}
*/
// ==================== CRUD Add Mahdy====================
void MainWindow::on_ajzone_clicked()
{
    // --- Contrôle de saisie ---
    if (ui->idzone->text().isEmpty() ||
        ui->pop->text().isEmpty() ||
        ui->x->text().isEmpty() ||
        ui->y->text().isEmpty() ||
        ui->l->text().isEmpty() ||
        ui->h->text().isEmpty())
    {
        QMessageBox::warning(this, "⚠️ Champ vide", "Tous les champs doivent être remplis !");
        return;
    }

    int idzone = ui->idzone->text().toInt();
    int pop = ui->pop->text().toInt();
    double x = ui->x->text().toDouble();
    double y = ui->y->text().toDouble();
    double l = ui->l->text().toDouble();
    double h = ui->h->text().toDouble();

    Zone z(idzone, pop, x, y, l, h);
    if (z.ajouter())
        QMessageBox::information(this, "✅ Succès", "Zone ajoutée avec succès !");
    else
        QMessageBox::critical(this, "❌ Erreur", "Échec d'ajout de la zone !");
}

void MainWindow::on_ajpoub_clicked()
{
    // --- Contrôle de saisie ---
    if (ui->pubid->text().isEmpty() ||
        ui->zoneid->text().isEmpty() ||
        ui->cap->text().isEmpty() ||
        ui->status->text().isEmpty())
    {
        QMessageBox::warning(this, "⚠️ Champ vide", "Tous les champs doivent être remplis !");
        return;
    }

    QString status = ui->status->text().trimmed().toLower();
    if (status != "vide" && status != "plein")
    {
        QMessageBox::warning(this, "⚠️ Statut invalide", "Le statut doit être soit 'vide' soit 'plein' !");
        return;
    }

    int zoneid = ui->zoneid->text().toInt();
    QSqlQuery checkZone;
    checkZone.prepare("SELECT COUNT(*) FROM GZONE WHERE ID_ZONE = :id_zone");
    checkZone.bindValue(":id_zone", zoneid);
    if (!checkZone.exec() || !checkZone.next())
    {
        QMessageBox::critical(this, "❌ Erreur", "Erreur lors de la vérification de la zone !");
        return;
    }
    if (checkZone.value(0).toInt() == 0)
    {
        QMessageBox::warning(this, "⚠️ Zone inexistante", "L'ID de zone n'existe pas dans la table GZONE !");
        return;
    }

    int pubid = ui->pubid->text().toInt();
    int cap = ui->cap->text().toInt();

    Poubelle p(pubid, zoneid, cap, status);
    if (p.ajouter())
        QMessageBox::information(this, "✅ Succès", "Poubelle ajoutée avec succès !");
    else
        QMessageBox::critical(this, "❌ Erreur", "Échec d'ajout de la poubelle !");
}

// ==================== CRUD Delete ====================
void MainWindow::on_suppzone_clicked()
{
    int idToDelete = QInputDialog::getInt(this, "Supprimer Zone", "Entrez l'ID de la zone :");

    Zone z;
    if (z.supprimer(idToDelete))
    {
        QMessageBox::information(this, "✅ Succès", "Zone supprimée avec succès !");
        QTableWidget *table = ui->twz;
        for (int row = 0; row < table->rowCount(); ++row)
        {
            QTableWidgetItem *item = table->item(row, 0);
            if (item && item->text().toInt() == idToDelete)
            {
                table->removeRow(row);
                break;
            }
        }
    }
    else
        QMessageBox::critical(this, "❌ Erreur", "Échec de la suppression de la zone !");
}

void MainWindow::on_supppoub_clicked()
{
    int idToDelete = QInputDialog::getInt(this, "Supprimer Poubelle", "Entrez l'ID de la poubelle :");

    Poubelle p;
    if (p.supprimer(idToDelete))
    {
        QMessageBox::information(this, "✅ Succès", "Poubelle supprimée avec succès !");
        QTableWidget *table = ui->twp;
        for (int row = 0; row < table->rowCount(); ++row)
        {
            QTableWidgetItem *item = table->item(row, 0);
            if (item && item->text().toInt() == idToDelete)
            {
                table->removeRow(row);
                break;
            }
        }
    }
    else
        QMessageBox::critical(this, "❌ Erreur", "Échec de la suppression de la poubelle !");
}

// ==================== Display / Affichage ====================
void MainWindow::afficherZones()
{
    Zone z;
    QSqlQueryModel *model = z.afficher();

    ui->twz->setRowCount(0);
    int rows = model->rowCount();
    int cols = model->columnCount();
    ui->twz->setColumnCount(cols);

    for (int j = 0; j < cols; ++j)
        ui->twz->setHorizontalHeaderItem(j, new QTableWidgetItem(model->headerData(j, Qt::Horizontal).toString()));

    for (int i = 0; i < rows; ++i)
    {
        ui->twz->insertRow(i);
        for (int j = 0; j < cols; ++j)
        {
            ui->twz->setItem(i, j, new QTableWidgetItem(model->data(model->index(i, j)).toString()));
        }
    }
    ui->twz->resizeColumnsToContents();
}

void MainWindow::afficherPoubelles()
{
    Poubelle p;
    QSqlQueryModel *model = p.afficher();

    ui->twp->setRowCount(0);
    int rows = model->rowCount();
    int cols = model->columnCount();
    ui->twp->setColumnCount(cols);

    for (int j = 0; j < cols; ++j)
        ui->twp->setHorizontalHeaderItem(j, new QTableWidgetItem(model->headerData(j, Qt::Horizontal).toString()));

    for (int i = 0; i < rows; ++i)
    {
        ui->twp->insertRow(i);
        for (int j = 0; j < cols; ++j)
        {
            ui->twp->setItem(i, j, new QTableWidgetItem(model->data(model->index(i, j)).toString()));
        }
    }
    ui->twp->resizeColumnsToContents();
}

// ==================== CRUD Modify ====================
void MainWindow::modZone()
{
    int idToModify = QInputDialog::getInt(this, "Modifier Zone", "Entrez l'ID de la zone :");

    int newPop = QInputDialog::getInt(this, "Modifier Zone", "Nouvelle population :");
    double newX = QInputDialog::getDouble(this, "Modifier Zone", "Nouvelle coordonnée X :");
    double newY = QInputDialog::getDouble(this, "Modifier Zone", "Nouvelle coordonnée Y :");
    double newL = QInputDialog::getDouble(this, "Modifier Zone", "Nouvelle longueur :");
    double newH = QInputDialog::getDouble(this, "Modifier Zone", "Nouvelle hauteur :");

    // --- Contrôle : vérifier valeurs ---
    if (newPop <= 0 || newL <= 0 || newH <= 0)
    {
        QMessageBox::warning(this, "⚠️ Données invalides", "Les valeurs numériques doivent être positives !");
        return;
    }

    Zone z(idToModify, newPop, newX, newY, newL, newH);
    if (z.modifier(idToModify))
    {
        QMessageBox::information(this, "✅ Succès", "Zone modifiée avec succès !");
        afficherZones();
    }
    else
        QMessageBox::critical(this, "❌ Erreur", "Échec de modification de la zone !");
}

void MainWindow::modPoubelle()
{
    int idToModify = QInputDialog::getInt(this, "Modifier Poubelle", "Entrez l'ID de la poubelle :");

    int newZoneId = QInputDialog::getInt(this, "Modifier Poubelle", "Nouvel ID de zone :");
    int newCap = QInputDialog::getInt(this, "Modifier Poubelle", "Nouvelle capacité :");
    QString newStatus = QInputDialog::getText(this, "Modifier Poubelle", "Nouveau statut :").trimmed().toLower();

    // --- Contrôle de saisie ---
    if (newStatus.isEmpty())
    {
        QMessageBox::warning(this, "⚠️ Champ vide", "Le statut ne peut pas être vide !");
        return;
    }
    if (newStatus != "vide" && newStatus != "plein")
    {
        QMessageBox::warning(this, "⚠️ Statut invalide", "Le statut doit être 'vide' ou 'plein' !");
        return;
    }

    QSqlQuery checkZone;
    checkZone.prepare("SELECT COUNT(*) FROM GZONE WHERE ID_ZONE = :id_zone");
    checkZone.bindValue(":id_zone", newZoneId);
    if (!checkZone.exec() || !checkZone.next() || checkZone.value(0).toInt() == 0)
    {
        QMessageBox::warning(this, "⚠️ Zone inexistante", "L'ID de zone n'existe pas dans la table GZONE !");
        return;
    }

    if (newCap <= 0)
    {
        QMessageBox::warning(this, "⚠️ Capacité invalide", "La capacité doit être positive !");
        return;
    }

    Poubelle p(idToModify, newZoneId, newCap, newStatus);
    if (p.modifier(idToModify))
    {
        QMessageBox::information(this, "✅ Succès", "Poubelle modifiée avec succès !");
        afficherPoubelles();
    }
    else
        QMessageBox::critical(this, "❌ Erreur", "Échec de modification de la poubelle !");
}
// === CRUD Habitants ===

void MainWindow::on_pushButton_ajoutH_clicked()
{
    int id = ui->lineEdit_id->text().toInt();
    QString nom = ui->lineEdit_nom->text();
    QString prenom = ui->lineEdit_prenom->text();
    QString adresse = ui->lineEdit_adresse->text();
    QString zone = ui->lineEdit_zone->text();
    QString situation = ui->lineEdit_situation->text();
    QString contact = ui->lineEdit_contact->text();
    QString statut = ui->lineEdit_statut->text();
    QString besoins = ui->lineEdit_besoins->text();
    QString satisfaction = ui->lineEdit_satisfaction->text();

    HabitantCRUD h(id, nom, prenom, adresse, zone, situation, contact, statut, besoins, satisfaction);
    QString erreur;
    if (!statutAutorise(statut)) {
        const QString liste = allowedStatuses.join("\n - ");
        QMessageBox::warning(this, "Valeur non autorisée",
                             QString("La valeur de 'statut' n'est pas autorisée.\n\nValeurs permises:\n - %1")
                                 .arg(liste.isEmpty() ? QString("(liste indisponible)") : liste));
        return;
    }
    if (!h.validerChamps(erreur)) {
        QMessageBox::warning(this, "Erreur", erreur);
        return;
    }

    if (h.ajouter()) {
        QMessageBox::information(this, "Succès", "Habitant ajouté !");
        chargerDonneesTable();
        saveLastHabitantId(id);
        loadLastHabitantSelection();
        ui->lineEdit_id->clear();
        ui->lineEdit_nom->clear();
        ui->lineEdit_prenom->clear();
        ui->lineEdit_adresse->clear();
        ui->lineEdit_zone->clear();
        ui->lineEdit_situation->clear();
        ui->lineEdit_contact->clear();
        ui->lineEdit_statut->clear();
        ui->lineEdit_besoins->clear();
        ui->lineEdit_satisfaction->clear();
    } else {
        QMessageBox::critical(this, "Erreur", "Échec de l'ajout.");
    }
}

void MainWindow::on_pushButton_modifH_clicked()
{
    int id = ui->lineEdit_id->text().toInt();
    if (id <= 0) {
        QMessageBox::warning(this, "Erreur", "ID invalide !");
        return;
    }

    QString nom = ui->lineEdit_nom->text();
    QString prenom = ui->lineEdit_prenom->text();
    QString adresse = ui->lineEdit_adresse->text();
    QString zone = ui->lineEdit_zone->text();
    QString situation = ui->lineEdit_situation->text();
    QString contact = ui->lineEdit_contact->text();
    QString statut = ui->lineEdit_statut->text();
    QString besoins = ui->lineEdit_besoins->text();
    QString satisfaction = ui->lineEdit_satisfaction->text();

    HabitantCRUD h(id, nom, prenom, adresse, zone, situation, contact, statut, besoins, satisfaction);
    QString erreur;
    if (!statutAutorise(statut)) {
        const QString liste = allowedStatuses.join("\n - ");
        QMessageBox::warning(this, "Valeur non autorisée",
                             QString("La valeur de 'statut' n'est pas autorisée.\n\nValeurs permises:\n - %1")
                                 .arg(liste.isEmpty() ? QString("(liste indisponible)") : liste));
        return;
    }
    if (!h.validerChamps(erreur)) {
        QMessageBox::warning(this, "Erreur", erreur);
        return;
    }

    if (h.modifier(id)) {
        QMessageBox::information(this, "Succès", "Habitant modifié !");
        chargerDonneesTable();
        saveLastHabitantId(id);
        loadLastHabitantSelection();
    } else {
        QMessageBox::critical(this, "Erreur", "Échec de la modification.");
    }
}

void MainWindow::on_pushButton_suppH_clicked()
{
    int id = ui->lineEdit_id->text().toInt();
    if (id <= 0) {
        QMessageBox::warning(this, "Erreur", "ID invalide !");
        return;
    }

    if (QMessageBox::question(this, "Confirmation", "Supprimer cet habitant ?") == QMessageBox::Yes) {
        HabitantCRUD h;
        if (h.supprimer(id)) {
            QMessageBox::information(this, "Succès", "Habitant supprimé !");
            chargerDonneesTable();
        } else {
            QMessageBox::critical(this, "Erreur", "Échec de la suppression.");
        }
    }
}

void MainWindow::on_pushButton_afficheH_clicked()
{
    chargerDonneesTable();
}

void MainWindow::on_tableView_habitants_clicked(const QModelIndex &index)
{
    if (!index.isValid()) return;
    populateEditsFromRow(index.row());
}

void MainWindow::on_pushButton_rechH_clicked()
{
    rechercherTexte(ui->lineEdit_rechH->text());
}

void MainWindow::on_pushButton_trieH_clicked()
{
    QString critere = ui->comboBox_trieH->currentText();
    QString order;

    if (critere == "Nom" || critere == "nom") order = "NOM ASC";
    else if (critere == "Prénom" || critere == "prenom" || critere == "Prénom ") order = "PRENOM ASC";
    else if (critere == "Adresse" || critere == "adresse") order = "ADRESSE ASC";
    else if (critere == "Satisfaction" || critere == "satisfaction") order = "NIVEAU_SATISFACTION DESC";
    else order = "ID_HABITANT ASC";

    QSqlQueryModel *model = new QSqlQueryModel(this);
    model->setQuery("SELECT ID_HABITANT, NOM, PRENOM, ADRESSE, ZONE_RESIDENCE, SITUATION_FAMILIALE, CONTACT, STATUT, BESOINS_SPECIFIQUES, NIVEAU_SATISFACTION FROM HABITANT ORDER BY " + order);

    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Nom"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Prénom"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Adresse"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Zone"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Situation"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("Contact"));
    model->setHeaderData(7, Qt::Horizontal, QObject::tr("Statut"));
    model->setHeaderData(8, Qt::Horizontal, QObject::tr("Besoins"));
    model->setHeaderData(9, Qt::Horizontal, QObject::tr("Satisfaction"));

    ui->tableView_habitants->setModel(model);
    ui->tableView_habitants->resizeColumnsToContents();
}

// (duplication supprimée) La fonction rechercherTexte est définie plus haut

// === Gestion dynamique du tableau avec bouton ❌ ===
void MainWindow::on_pushButton_30_clicked()
{
    QTableWidget *table = ui->tableWidget_6;

    int newRow = table->rowCount();
    table->insertRow(newRow);

    int deleteCol = table->columnCount() - 1;
    for (int col = 0; col < deleteCol; ++col)
    {
        table->setItem(newRow, col, new QTableWidgetItem(""));
    }

    QPushButton *btnSupprimer = new QPushButton("❌");
    btnSupprimer->setFixedSize(30, 25);
    btnSupprimer->setStyleSheet(
        "QPushButton { background-color: #b00000; color: white; border-radius: 6px; font-weight: bold; }"
        "QPushButton:hover { background-color: #ff4444; }"
        );

    connect(btnSupprimer, &QPushButton::clicked, this, &MainWindow::supprimerEtDecalerLignes);

    table->setCellWidget(newRow, deleteCol, btnSupprimer);
    table->resizeColumnsToContents();
}

// === Implémente cette fonction dans le .cpp et déclare-la dans le .h ===
// void MainWindow::supprimerEtDecalerLignes()
// {
//     // À implémenter : supprimer la ligne du bouton cliqué
// }

// === Bouton de test ===
void MainWindow::on_pushButton_15_clicked()
{
    ui->stackedWidget_7->setCurrentIndex(2);
    qDebug() << "Bouton 15 cliqué !";
}
/*void MainWindow::supprimerEtDecalerLignes()
{
    QPushButton *btn = qobject_cast<QPushButton*>(sender());
    if (!btn) return;

    QTableWidget *table = ui->tableWidget_6;
    for (int row = 0; row < table->rowCount(); ++row) {
        if (table->cellWidget(row, table->columnCount() - 1) == btn) {
            table->removeRow(row);
            break;
        }
    }
}*/
void MainWindow::on_on_pushButton_14_clicked()
{
    // TODO: Implémenter
    ui->stackedWidget_2->setCurrentIndex(1);
    qDebug() << "Bouton 14 cliqué";
}

void MainWindow::on_on_pushButton_15_clicked()
{
    // TODO: Implémenter
    ui->stackedWidget_2->setCurrentIndex(2);
    qDebug() << "Bouton 15 cliqué";
}

void MainWindow::on_on_pushButton_16_clicked()
{
    ui->stackedWidget_7->setCurrentIndex(0);
    // TODO: Implémenter
    qDebug() << "Bouton 16 cliqué";
}

void MainWindow::on_on_pushButton_17_clicked()
{
    ui->stackedWidget_7->setCurrentIndex(1);
    // TODO: Implémenter
    qDebug() << "Bouton 17 cliqué";
}
// ==================== CRUD VEHICULE ====================

#include "vehiculecrud.h"

void MainWindow::on_btn_vehicule_ajouter_clicked()
{
    int id = ui->lineEdit_vehicule_id->text().toInt();
    QString type = ui->combo_vehicule_type->currentText();
    QString matricule = ui->lineEdit_vehicule_matricule->text();
    QString capacite = ui->lineEdit_vehicule_capacite->text();
    QString itineraire = ui->lineEdit_vehicule_itineraire->text();
    QString hdep = ui->lineEdit_vehicule_hdep->text();
    QString harr = ui->lineEdit_vehicule_harr->text();
    QString statut = ui->combo_vehicule_statut->currentText();
    QString zone = ui->lineEdit_vehicule_zone->text();
    int id_habitant = ui->lineEdit_vehicule_idhabitant->text().toInt();

    VehiculeCRUD v(id, type, matricule, capacite, itineraire, hdep, harr, statut, zone, id_habitant);
    QString erreur;

    if (!v.validerChamps(erreur)) {
        QMessageBox::warning(this, "Erreur de validation", erreur);
        return;
    }

    if (v.ajouter()) {
        QMessageBox::information(this, "Succès", "Véhicule ajouté !");
        ui->tableView_vehicule->setModel(v.afficher());
    } else {
        QMessageBox::critical(this, "Erreur", "Échec de l'ajout du véhicule.");
    }
}
void MainWindow::on_btn_vehicule_modifier_clicked()
{
    // Vérifier si un ID est sélectionné
    QString idText = ui->lineEdit_vehicule_id->text().trimmed();
    if (idText.isEmpty()) {
        QMessageBox::warning(this, "Avertissement",
                             "⚠️ Veuillez d'abord sélectionner un véhicule dans la table.");
        return;
    }

    int id = idText.toInt();
    if (id <= 0) {
        QMessageBox::warning(this, "Erreur", "ID invalide !");
        return;
    }

    // Récupérer les champs
    QString type = ui->combo_vehicule_type->currentText().trimmed();
    QString matricule = ui->lineEdit_vehicule_matricule->text().trimmed();
    QString capacite = ui->lineEdit_vehicule_capacite->text().trimmed();
    QString itineraire = ui->lineEdit_vehicule_itineraire->text().trimmed();
    QString hdep = ui->lineEdit_vehicule_hdep->text().trimmed();
    QString harr = ui->lineEdit_vehicule_harr->text().trimmed();
    QString statut = ui->combo_vehicule_statut->currentText().trimmed();
    QString zone = ui->lineEdit_vehicule_zone->text().trimmed();
    QString idHabitantText = ui->lineEdit_vehicule_idhabitant->text().trimmed();

    // Vérifier si au moins un champ est renseigné
    if (type.isEmpty() && matricule.isEmpty() && capacite.isEmpty() && itineraire.isEmpty() &&
        hdep.isEmpty() && harr.isEmpty() && statut.isEmpty() && zone.isEmpty() && idHabitantText.isEmpty()) {
        QMessageBox::warning(this, "Avertissement",
                             "⚠️ Aucun champ à modifier. Remplissez au moins un champ avant de cliquer sur Modifier.");
        return;
    }

    int id_habitant = idHabitantText.toInt();

    // Créer l’objet CRUD
    VehiculeCRUD v(id, type, matricule, capacite, itineraire, hdep, harr, statut, zone, id_habitant);

    QString erreur;
    if (!v.validerChamps(erreur)) {
        QMessageBox::warning(this, "Erreur de validation", erreur);
        return;
    }

    // Confirmation
    if (QMessageBox::question(this, "Confirmation",
                              "Voulez-vous vraiment modifier ce véhicule ?",
                              QMessageBox::Yes | QMessageBox::No) == QMessageBox::No) {
        return;
    }

    // Modifier
    if (v.modifier(id)) {
        QMessageBox::information(this, "Succès", "✅ Véhicule modifié avec succès !");
        ui->tableView_vehicule->setModel(v.afficher());
    } else {
        QMessageBox::critical(this, "Erreur", "❌ Échec de la modification.");
    }
}


void MainWindow::on_tableView_vehicule_clicked(const QModelIndex &index)
{
    if (!index.isValid()) return;
    QAbstractItemModel *model = ui->tableView_vehicule->model();
    int row = index.row();

    ui->lineEdit_vehicule_id->setText(model->index(row, 0).data().toString());
    ui->combo_vehicule_type->setCurrentText(model->index(row, 1).data().toString());
    ui->lineEdit_vehicule_matricule->setText(model->index(row, 2).data().toString());
    ui->lineEdit_vehicule_capacite->setText(model->index(row, 3).data().toString());
    ui->lineEdit_vehicule_itineraire->setText(model->index(row, 4).data().toString());
    ui->lineEdit_vehicule_hdep->setText(model->index(row, 5).data().toString());
    ui->lineEdit_vehicule_harr->setText(model->index(row, 6).data().toString());
    ui->combo_vehicule_statut->setCurrentText(model->index(row, 7).data().toString());
    ui->lineEdit_vehicule_zone->setText(model->index(row, 8).data().toString());
    ui->lineEdit_vehicule_idhabitant->setText(model->index(row, 9).data().toString());
}

void MainWindow::on_btn_vehicule_supprimer_clicked()
{
    int id = ui->lineEdit_vehicule_id->text().toInt();

    if (id <= 0) {
        QMessageBox::warning(this, "Erreur", "ID de véhicule invalide !");
        return;
    }

    // ✅ Confirmation avant suppression
    if (QMessageBox::question(this, "Confirmation",
                              QString("Voulez-vous vraiment supprimer le véhicule avec ID %1 ?").arg(id))
        == QMessageBox::Yes)
    {
        VehiculeCRUD v;
        if (v.supprimer(id)) {
            QMessageBox::information(this, "Succès", "Véhicule supprimé avec succès !");
            ui->tableView_vehicule->setModel(v.afficher()); // 🔄 refresh table
            ui->lineEdit_vehicule_id->clear();
        } else {
            QMessageBox::warning(this, "Échec", "Aucun véhicule trouvé avec cet ID ou erreur de suppression.");
        }
    }
}

void MainWindow::on_btn_vehicule_rechercher_clicked()
{
    QString texte = ui->lineEdit_vehicule_recherche->text();
    VehiculeCRUD v;
    ui->tableView_vehicule->setModel(v.rechercher(texte));
}
void MainWindow::on_btn_vehicule_afficher_clicked()
{
    VehiculeCRUD v;
    QSqlQueryModel *model = v.afficher();
    ui->tableView_vehicule->setModel(model);
    ui->tableView_vehicule->resizeColumnsToContents();
}
