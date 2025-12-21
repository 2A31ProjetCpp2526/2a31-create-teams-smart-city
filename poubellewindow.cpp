#include "poubellewindow.h"
#include <QMessageBox>
#include <QLabel>
#include <QHeaderView>
#include <QTableView>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QPushButton>

PoubelleWindow::PoubelleWindow(QWidget *parent)
    : QWidget(parent)
{
    setWindowTitle("Gestion des Poubelles");
    resize(700, 500);  // More space

    setupUI();
    afficherPoubelles();
    setupConnections();
}

PoubelleWindow::~PoubelleWindow() {}

// ------------------- UI -------------------
void PoubelleWindow::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(15);
    mainLayout->setContentsMargins(10,10,10,10);

    // --- Form ---
    QGridLayout *formLayout = new QGridLayout();
    formLayout->setSpacing(10);

    formLayout->addWidget(new QLabel("ID Bin:"), 0, 0);
    idBinEdit = new QLineEdit();
    formLayout->addWidget(idBinEdit, 0, 1);

    formLayout->addWidget(new QLabel("ID Zone:"), 1, 0);
    idZoneEdit = new QLineEdit();
    formLayout->addWidget(idZoneEdit, 1, 1);

    formLayout->addWidget(new QLabel("Capacité:"), 2, 0);
    capaciteEdit = new QLineEdit();
    formLayout->addWidget(capaciteEdit, 2, 1);

    formLayout->addWidget(new QLabel("Status:"), 3, 0);
    statusEdit = new QLineEdit();
    formLayout->addWidget(statusEdit, 3, 1);

    mainLayout->addLayout(formLayout);

    // --- Buttons ---
    QHBoxLayout *btnLayout = new QHBoxLayout();
    addBtn = new QPushButton("Ajouter");
    updateBtn = new QPushButton("Modifier");
    deleteBtn = new QPushButton("Supprimer");
    refreshBtn = new QPushButton("Actualiser");

    QList<QPushButton*> btns = {addBtn, updateBtn, deleteBtn, refreshBtn};
    for (auto b : btns) {
        b->setStyleSheet(
            "QPushButton { background-color: #007BFF; color: white; font-weight: bold; padding: 6px 12px; border-radius: 6px; }"
            "QPushButton:hover { background-color: #0056b3; }"
            );
        btnLayout->addWidget(b);
    }
    mainLayout->addLayout(btnLayout);

    // --- Table ---
    tableView = new QTableView(this);
    tableView->setAlternatingRowColors(true);
    tableView->horizontalHeader()->setStretchLastSection(true);
    tableView->verticalHeader()->setVisible(false);
    tableView->setStyleSheet(
        "QTableView { background-color: #E3F2FD; alternate-background-color: #BBDEFB; color: #000; font-size: 13px; }"
        "QHeaderView::section { background-color: #1976D2; color: white; font-weight: bold; }"
        );

    mainLayout->addWidget(tableView);
}

// ------------------- Connections -------------------
void PoubelleWindow::setupConnections()
{
    connect(addBtn, &QPushButton::clicked, this, &PoubelleWindow::ajouterPoubelle);
    connect(updateBtn, &QPushButton::clicked, this, &PoubelleWindow::modifierPoubelle);
    connect(deleteBtn, &QPushButton::clicked, this, &PoubelleWindow::supprimerPoubelle);
    connect(refreshBtn, &QPushButton::clicked, this, &PoubelleWindow::afficherPoubelles);
}

// ------------------- CRUD -------------------
void PoubelleWindow::afficherPoubelles()
{
    tableView->setModel(pTmp.afficher());
}

void PoubelleWindow::ajouterPoubelle()
{
    int idBin = idBinEdit->text().toInt();
    int idZone = idZoneEdit->text().toInt();
    int capacite = capaciteEdit->text().toInt();
    QString status = statusEdit->text();

    Poubelle p(idBin, idZone, capacite, status);
    if(p.ajouter()) {
        QMessageBox::information(this, "Succès", "Poubelle ajoutée !");
        afficherPoubelles();
    } else {
        QMessageBox::critical(this, "Erreur", "Échec de l'ajout !");
    }
}

void PoubelleWindow::modifierPoubelle()
{
    int idBin = idBinEdit->text().toInt();
    int idZone = idZoneEdit->text().toInt();
    int capacite = capaciteEdit->text().toInt();
    QString status = statusEdit->text();

    Poubelle p(idBin, idZone, capacite, status);
    if(p.modifier(idBin)) {
        QMessageBox::information(this, "Succès", "Poubelle modifiée !");
        afficherPoubelles();
    } else {
        QMessageBox::critical(this, "Erreur", "Échec de modification !");
    }
}

void PoubelleWindow::supprimerPoubelle()
{
    int idBin = idBinEdit->text().toInt();
    if(pTmp.supprimer(idBin)) {
        QMessageBox::information(this, "Succès", "Poubelle supprimée !");
        afficherPoubelles();
    } else {
        QMessageBox::critical(this, "Erreur", "Échec de suppression !");
    }
}
