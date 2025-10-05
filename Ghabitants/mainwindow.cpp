#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

// Métier → première page
void MainWindow::on_pushButton_clicked()
{
    ui->stackedWidget_2->setCurrentIndex(0);
}

// Demande → deuxième page
void MainWindow::on_pushButton2_clicked()
{
    ui->stackedWidget_2->setCurrentIndex(1);
}

// Satisfaction → troisième page
void MainWindow::on_pushButton3_clicked()
{
    ui->stackedWidget_2->setCurrentIndex(2);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_on_pushButton_clicked()
{
    ui->stackedWidget_2->setCurrentIndex(0);
}


void MainWindow::on_on_pushButton_2_clicked()
{
    ui->stackedWidget_2->setCurrentIndex(1);

}


void MainWindow::on_on_pushButton_3_clicked()
{
    ui->stackedWidget_2->setCurrentIndex(2);
}


void MainWindow::on_Ghabitant_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}


void MainWindow::on_Ghabitats_2_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}


void MainWindow::on_Gpersonnels_2_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
}


void MainWindow::on_Gequipements_2_clicked()
{
    ui->stackedWidget->setCurrentIndex(4);
}


void MainWindow::on_Gvehicules_2_clicked()
{
    ui->stackedWidget->setCurrentIndex(5);
}

void MainWindow::on_Gespace_clicked()
{
    ui->stackedWidget->setCurrentIndex(3);
}

void MainWindow::on_on_pushButton_4_clicked()
{
    ui->stackedWidget_5->setCurrentIndex(1);
}


void MainWindow::on_on_pushButton_5_clicked()
{
      ui->stackedWidget_5->setCurrentIndex(2);
}


void MainWindow::on_on_pushButton_6_clicked()
{
      ui->stackedWidget_5->setCurrentIndex(0);
}


void MainWindow::on_on_pushButton_7_clicked()
{
   ui->stackedWidget_6->setCurrentIndex(0);
}


void MainWindow::on_on_pushButton_8_clicked()
{
    ui->stackedWidget_6->setCurrentIndex(1);
}


void MainWindow::on_on_pushButton_9_clicked()
{
    ui->stackedWidget_6->setCurrentIndex(2);
}

