#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QHeaderView>
#include <QPushButton>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setupTable(); // active le tri et ajoute les croix sur les lignes déjà présentes
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupTable()
{
    QTableWidget *table = ui->tableWidget_6;

    // Activer le tri sur toutes les colonnes existantes
    table->setSortingEnabled(true);
    table->horizontalHeader()->setSortIndicatorShown(true);
    table->horizontalHeader()->setSectionsClickable(true);

    // Ajouter une colonne supplémentaire à la fin si elle n’existe pas déjà pour la croix
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

    // Ajouter les boutons ❌ à chaque ligne existante
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

// 🔹 Vide le contenu et décale les lignes du dessous vers le haut
void MainWindow::supprimerEtDecalerLignes()
{
    QTableWidget *table = ui->tableWidget_6;
    QPushButton *button = qobject_cast<QPushButton*>(sender());
    if (!button)
        return;

    int deleteRow = -1;

    // Trouver la ligne du bouton cliqué
    for (int i = 0; i < table->rowCount(); ++i)
    {
        if (table->cellWidget(i, table->columnCount() - 1) == button)
        {
            deleteRow = i;
            break;
        }
    }

    if (deleteRow == -1)
        return;

    // Supprimer la ligne
    table->removeRow(deleteRow);

    // 🔄 Réajouter la colonne de boutons ❌ sur les lignes restantes
    int deleteCol = table->columnCount() - 1;
    for (int row = 0; row < table->rowCount(); ++row)
    {
        QWidget *widget = table->cellWidget(row, deleteCol);
        if (!widget)
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
            table->setCellWidget(row, deleteCol, btnSupprimer);
        }
    }
}

// === le reste de ton code inchangé ===

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

void MainWindow::on_gespub_2_clicked() { ui->stackedWidget_3->setCurrentIndex(0); }
void MainWindow::on_geszone1_clicked() { ui->stackedWidget_3->setCurrentIndex(1); }

void MainWindow::on_pushButton_30_clicked()
{
    QTableWidget *table = ui->tableWidget_6;

    int newRow = table->rowCount();
    table->insertRow(newRow);

    // Remplir les colonnes normales avec des cellules vides
    int deleteCol = table->columnCount() - 1;
    for (int col = 0; col < deleteCol; ++col)
    {
        table->setItem(newRow, col, new QTableWidgetItem(""));
    }

    // Ajouter le bouton ❌ dans la dernière colonne
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


void MainWindow::on_on_pushButton_13_clicked()
{
    ui->stackedWidget_2->setCurrentIndex(0);
}


void MainWindow::on_on_pushButton_14_clicked()
{
    ui->stackedWidget_2->setCurrentIndex(1);
}


void MainWindow::on_on_pushButton_15_clicked()
{
    ui->stackedWidget_2->setCurrentIndex(2);
}


void MainWindow::on_on_pushButton_16_clicked()
{
    ui->stackedWidget_7->setCurrentIndex(0);
}


void MainWindow::on_on_pushButton_17_clicked()
{
    ui->stackedWidget_7->setCurrentIndex(1);
}


void MainWindow::on_pushButton_15_clicked()
{
    ui->stackedWidget_7->setCurrentIndex(2);
}

