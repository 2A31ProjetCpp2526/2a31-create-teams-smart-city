#ifndef POUBELLEWINDOW_H
#define POUBELLEWINDOW_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QTableView>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include "poubelle.h"

class PoubelleWindow : public QWidget
{
    Q_OBJECT

public:
    explicit PoubelleWindow(QWidget *parent = nullptr);
    ~PoubelleWindow();

private slots:
    void afficherPoubelles();
    void ajouterPoubelle();
    void modifierPoubelle();
    void supprimerPoubelle();

private:
    QLineEdit *idBinEdit;
    QLineEdit *idZoneEdit;
    QLineEdit *capaciteEdit;
    QLineEdit *statusEdit;

    QPushButton *addBtn;
    QPushButton *updateBtn;
    QPushButton *deleteBtn;
    QPushButton *refreshBtn;

    QTableView *tableView;
    Poubelle pTmp;

    void setupUI();
    void setupConnections();
};

#endif // POUBELLEWINDOW_H
