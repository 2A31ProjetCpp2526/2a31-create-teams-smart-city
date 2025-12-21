#ifndef HABITANTWINDOW_H
#define HABITANTWINDOW_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QTableView>
#include <QVBoxLayout>
#include "habitantcrud.h"

class HabitantWindow : public QWidget
{
    Q_OBJECT

public:
    explicit HabitantWindow(QWidget *parent = nullptr);
    ~HabitantWindow();

private slots:
    void ajouterHabitant();
    void modifierHabitant();
    void supprimerHabitant();
    void afficherHabitants();

private:
    QLineEdit *idEdit;
    QLineEdit *nomEdit;
    QLineEdit *prenomEdit;
    QLineEdit *adresseEdit;

    QPushButton *addBtn;
    QPushButton *updateBtn;
    QPushButton *deleteBtn;
    QPushButton *refreshBtn;

    QTableView *tableView;
    QVBoxLayout *mainLayout;

    HabitantCRUD habTemp; // Temporary instance
};

#endif // HABITANTWINDOW_H
