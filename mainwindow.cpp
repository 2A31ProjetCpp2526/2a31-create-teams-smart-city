#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    int ret = A.connect_arduino();
        //qDebug() << "Connect result:" << ret;

    connect(A.getserial(), SIGNAL(readyRead()), this, SLOT(update_label()));
}

MainWindow::~MainWindow()
{
    A.close_arduino();
    delete ui;
}

void MainWindow::update_label()
{
    QByteArray data = A.read_from_arduino();
    if (!data.isEmpty()) ui->label->setText(QString::fromUtf8(data));
}

void MainWindow::on_pushButton_clicked()
{
    A.write_to_arduino("1"); }

void MainWindow::on_pushButton_2_clicked()
{
    A.write_to_arduino("0"); }

void MainWindow::on_pushButton_3_clicked()
{
    A.write_to_arduino("2"); }

void MainWindow::on_pushButton_4_clicked()
{
    A.write_to_arduino("3"); }
