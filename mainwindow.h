#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include <QPushButton>
#include "zone.h"
#include "poubelle.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void setupTable();

private slots:
    // Navigation buttons
    void on_pushButton_clicked();
    void on_pushButton2_clicked();
    void on_pushButton3_clicked();

    void on_on_pushButton_clicked();
    void on_on_pushButton_2_clicked();
    void on_on_pushButton_3_clicked();

    void on_Ghabitant_clicked();
    void on_Ghabitats_2_clicked();
    void on_Gpersonnels_2_clicked();
    void on_Gequipements_2_clicked();
    void on_Gvehicules_2_clicked();
    void on_Gespace_clicked();

    void on_on_pushButton_4_clicked();
    void on_on_pushButton_5_clicked();
    void on_on_pushButton_6_clicked();

    void on_on_pushButton_7_clicked();
    void on_on_pushButton_8_clicked();
    void on_on_pushButton_9_clicked();

    // Fixed slots (was causing linker errors)
    void on_on_pushButton_13_clicked();
    void on_on_pushButton_14_clicked();
    void on_on_pushButton_15_clicked();
    void on_on_pushButton_16_clicked();
    void on_on_pushButton_17_clicked();
    void on_pushButton_15_clicked();

    void on_gespub_2_clicked();
    void on_geszone1_clicked();

    void on_pushButton_30_clicked();

    // CRUD buttons
    void on_ajzone_clicked();
    void on_ajpoub_clicked();
    void on_suppzone_clicked();
    void on_supppoub_clicked();
    void afficherZones();
    void afficherPoubelles();
    void modZone();   // Modify Zone
    void modPoubelle(); // Modify Poubelle

    // Generic delete for table rows
    void supprimerEtDecalerLignes();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
