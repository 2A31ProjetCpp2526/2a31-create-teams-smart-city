#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QPushButton>
#include <QTableView>
#include <QComboBox>
#include "zone.h"
#include <QMainWindow>
#include <QTableWidget>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QVBoxLayout>

#include "connection.h"





class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void afficherZones();
    void ajouterZone();
    void supprimerZone();
    void modifierZone();
    void afficherResponsables();
    void afficherMiniMap();


private:
    // === Widgets ===
    QLineEdit *idEdit;
    QLineEdit *nomEdit;
    QLineEdit *typeEdit;
    QLineEdit *supEdit;
    QLineEdit *locEdit;
    QLineEdit *respEdit;
    QLineEdit *etatEdit;

    QPushButton *addBtn;
    QPushButton *deleteBtn;
    QPushButton *updateBtn;
    QPushButton *refreshBtn;

    QTableView *tableView;


    QTableWidget *tableResponsables;
    QGraphicsView *mapView;
    QGraphicsScene *scene;



    void setupUI();

    // === Data ===
    Zone Ztmp;





};

#endif // MAINWINDOW_H
