#include "habitantwindow.h"
#include <QHBoxLayout>
#include <QMessageBox>

HabitantWindow::HabitantWindow(QWidget *parent) : QWidget(parent)
{
    setWindowTitle("Gestion Habitant");
    resize(800, 600);

    mainLayout = new QVBoxLayout(this);

    // --- Form ---
    idEdit = new QLineEdit(); idEdit->setPlaceholderText("ID");
    nomEdit = new QLineEdit(); nomEdit->setPlaceholderText("Nom");

    adresseEdit = new QLineEdit(); adresseEdit->setPlaceholderText("Adresse");

    QHBoxLayout *formLayout = new QHBoxLayout();
    formLayout->addWidget(idEdit);
    formLayout->addWidget(nomEdit);

    formLayout->addWidget(adresseEdit);

    mainLayout->addLayout(formLayout);

    // --- Buttons ---
    addBtn = new QPushButton("Ajouter");
    updateBtn = new QPushButton("Modifier");
    deleteBtn = new QPushButton("Supprimer");
    refreshBtn = new QPushButton("Actualiser");

    QHBoxLayout *btnLayout = new QHBoxLayout();
    btnLayout->addWidget(addBtn);
    btnLayout->addWidget(updateBtn);
    btnLayout->addWidget(deleteBtn);
    btnLayout->addWidget(refreshBtn);

    mainLayout->addLayout(btnLayout);

    // --- Table ---
    tableView = new QTableView();
    tableView->setAlternatingRowColors(true);
    mainLayout->addWidget(tableView);

    // --- Connections ---
    connect(addBtn, &QPushButton::clicked, this, &HabitantWindow::ajouterHabitant);
    connect(updateBtn, &QPushButton::clicked, this, &HabitantWindow::modifierHabitant);
    connect(deleteBtn, &QPushButton::clicked, this, &HabitantWindow::supprimerHabitant);
    connect(refreshBtn, &QPushButton::clicked, this, &HabitantWindow::afficherHabitants);

    afficherHabitants();
}

HabitantWindow::~HabitantWindow() {}

void HabitantWindow::ajouterHabitant()
{
    HabitantCRUD h(idEdit->text().toInt(), nomEdit->text(), prenomEdit->text(),
                   adresseEdit->text(), "", "", "");
    if (h.ajouter()) afficherHabitants();
}

void HabitantWindow::modifierHabitant()
{
    HabitantCRUD h(idEdit->text().toInt(), nomEdit->text(), prenomEdit->text(),
                   adresseEdit->text(), "", "", "");
    if (h.modifier(idEdit->text().toInt())) afficherHabitants();
}

void HabitantWindow::supprimerHabitant()
{
    int id = idEdit->text().toInt();
    if (habTemp.supprimer(id)) afficherHabitants();
}

void HabitantWindow::afficherHabitants()
{
    tableView->setModel(habTemp.afficher(this));
}
