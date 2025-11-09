#include "mainwindow.h"
#include "connection.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QMessageBox>
#include <QHeaderView>
#include <QDebug>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("CRUD - ZONES_VERTES (Oracle/ODBC)");
    resize(950, 550);

    // === Database connection ===
    Connection *c = Connection::getInstance();
    if (!c->openConnection()) {
        QMessageBox::critical(this, "Erreur", "Échec de connexion à la base de données !");
        return;
    }

    // === Create table if not exists (Oracle version) ===
    QSqlQuery q;
    QString ddl =
        "BEGIN "
        "EXECUTE IMMEDIATE 'CREATE TABLE ZONES_VERTES ("
        "ID_ZONE VARCHAR2(20 BYTE) PRIMARY KEY, "
        "NOM VARCHAR2(50 BYTE), "
        "TYPE VARCHAR2(50 BYTE), "
        "SUPERFICIE NUMBER(*,0), "
        "LOCALISATION VARCHAR2(100 BYTE), "
        "RESPONSABLE VARCHAR2(50 BYTE), "
        "ETAT VARCHAR2(50 BYTE))'; "
        "EXCEPTION WHEN OTHERS THEN "
        "IF SQLCODE = -955 THEN NULL; ELSE RAISE; END IF; "
        "END;";
    q.exec(ddl);

    // === Central widget ===
    QWidget *central = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(central);
    setCentralWidget(central);

    // === Input fields ===
    idEdit = new QLineEdit();
    nomEdit = new QLineEdit();
    typeEdit = new QLineEdit();
    supEdit = new QLineEdit();
    locEdit = new QLineEdit();
    respEdit = new QLineEdit();
    etatEdit = new QLineEdit();

    idEdit->setPlaceholderText("ID_ZONE");
    nomEdit->setPlaceholderText("NOM");
    typeEdit->setPlaceholderText("TYPE");
    supEdit->setPlaceholderText("SUPERFICIE");
    locEdit->setPlaceholderText("LOCALISATION");
    respEdit->setPlaceholderText("RESPONSABLE");
    etatEdit->setPlaceholderText("ETAT");

    QString fieldStyle = "QLineEdit { border: 2px solid #4DD0E1; border-radius: 6px; "
                         "padding: 5px; background-color: #E0F7FA; color: #003366; }";
    QList<QLineEdit*> edits = {idEdit, nomEdit, typeEdit, supEdit, locEdit, respEdit, etatEdit};
    for (auto e : edits)
        e->setStyleSheet(fieldStyle);

    // === Buttons ===
    addBtn = new QPushButton("Ajouter");
    deleteBtn = new QPushButton("Supprimer");
    updateBtn = new QPushButton("Modifier");
    refreshBtn = new QPushButton("Actualiser");

    QString btnStyle = "QPushButton { background-color: #00838F; color: white; "
                       "padding: 6px 12px; border-radius: 6px; font-weight: bold; }"
                       "QPushButton:hover { background-color: #006064; }";
    addBtn->setStyleSheet(btnStyle);
    deleteBtn->setStyleSheet(btnStyle);
    updateBtn->setStyleSheet(btnStyle);
    refreshBtn->setStyleSheet(btnStyle);

    // === Layouts ===
    QGridLayout *formLayout = new QGridLayout();
    formLayout->addWidget(new QLabel("ID Zone:"), 0, 0);
    formLayout->addWidget(idEdit, 0, 1);
    formLayout->addWidget(new QLabel("Nom:"), 1, 0);
    formLayout->addWidget(nomEdit, 1, 1);
    formLayout->addWidget(new QLabel("Type:"), 2, 0);
    formLayout->addWidget(typeEdit, 2, 1);
    formLayout->addWidget(new QLabel("Superficie:"), 3, 0);
    formLayout->addWidget(supEdit, 3, 1);
    formLayout->addWidget(new QLabel("Localisation:"), 4, 0);
    formLayout->addWidget(locEdit, 4, 1);
    formLayout->addWidget(new QLabel("Responsable:"), 5, 0);
    formLayout->addWidget(respEdit, 5, 1);
    formLayout->addWidget(new QLabel("État:"), 6, 0);
    formLayout->addWidget(etatEdit, 6, 1);

    layout->addLayout(formLayout);

    QHBoxLayout *btnLayout = new QHBoxLayout();
    btnLayout->addWidget(addBtn);
    btnLayout->addWidget(updateBtn);
    btnLayout->addWidget(deleteBtn);
    btnLayout->addWidget(refreshBtn);
    layout->addLayout(btnLayout);

    // === Table ===
    tableView = new QTableView(this);
    tableView->setStyleSheet("QTableView { background-color: #E0F7FA; "
                             "alternate-background-color: #B2EBF2; color: #003366; }"
                             "QHeaderView::section { background-color: #00838F; color: white; }");
    tableView->horizontalHeader()->setStretchLastSection(true);
    layout->addWidget(tableView);

    afficherZones();

    // === Signals ===
    connect(addBtn, &QPushButton::clicked, this, &MainWindow::ajouterZone);
    connect(deleteBtn, &QPushButton::clicked, this, &MainWindow::supprimerZone);
    connect(updateBtn, &QPushButton::clicked, this, &MainWindow::modifierZone);
    connect(refreshBtn, &QPushButton::clicked, this, &MainWindow::afficherZones);
}

MainWindow::~MainWindow() {}

void MainWindow::afficherZones()
{
    tableView->setModel(Ztmp.afficher());
}

void MainWindow::ajouterZone()
{
    QString id = idEdit->text();
    QString nom = nomEdit->text();
    QString type = typeEdit->text();
    double sup = supEdit->text().toDouble();
    QString loc = locEdit->text();
    QString resp = respEdit->text();
    QString etat = etatEdit->text();

    Zone z(id, nom, type, sup, loc, resp, etat);
    if (z.ajouter()) {
        QMessageBox::information(this, "Succès", "Zone ajoutée avec succès !");
        afficherZones();
    } else {
        QMessageBox::critical(this, "Erreur", "Échec d’ajout !");
    }
}

void MainWindow::supprimerZone()
{
    QString id = idEdit->text();
    if (Ztmp.supprimer(id)) {
        QMessageBox::information(this, "Succès", "Zone supprimée !");
        afficherZones();
    } else {
        QMessageBox::critical(this, "Erreur", "Échec de suppression !");
    }
}

void MainWindow::modifierZone()
{
    QString id = idEdit->text();
    QString nom = nomEdit->text();
    QString type = typeEdit->text();
    double sup = supEdit->text().toDouble();
    QString loc = locEdit->text();
    QString resp = respEdit->text();
    QString etat = etatEdit->text();

    Zone z(id, nom, type, sup, loc, resp, etat);
    if (z.modifier(id)) {
        QMessageBox::information(this, "Succès", "Zone modifiée !");
        afficherZones();
    } else {
        QMessageBox::critical(this, "Erreur", "Échec de modification !");
    }
}





