#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();   // Métier → index 0
    void on_pushButton2_clicked();  // Demande → index 1
    void on_pushButton3_clicked();  // Satisfaction → index 2

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

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
