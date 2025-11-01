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
    connect(ui->affzone, &QPushButton::clicked, this, &MainWindow::afficherZones);
    connect(ui->affpoub, &QPushButton::clicked, this, &MainWindow::afficherPoubelles);
    connect(ui->modzone, &QPushButton::clicked, this, &MainWindow::modZone);
    connect(ui->modpoub, &QPushButton::clicked, this, &MainWindow::modPoubelle);
}

MainWindow::~MainWindow()
{
    delete ui;
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
void MainWindow::on_on_pushButton_14_clicked() { ui->stackedWidget_2->setCurrentIndex(1); }
void MainWindow::on_on_pushButton_15_clicked() { ui->stackedWidget_2->setCurrentIndex(2); }
void MainWindow::on_on_pushButton_16_clicked() { ui->stackedWidget_7->setCurrentIndex(0); }
void MainWindow::on_on_pushButton_17_clicked() { ui->stackedWidget_7->setCurrentIndex(1); }
void MainWindow::on_pushButton_15_clicked() { ui->stackedWidget_7->setCurrentIndex(2); }

void MainWindow::on_gespub_2_clicked() { ui->stackedWidget_3->setCurrentIndex(0); }
void MainWindow::on_geszone1_clicked() { ui->stackedWidget_3->setCurrentIndex(1); }

void MainWindow::on_pushButton_30_clicked()
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
