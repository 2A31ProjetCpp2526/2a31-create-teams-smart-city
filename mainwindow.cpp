#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Create only geszone window
    geszoneWindow = new geszone();

    // Connect back signal
    connect(geszoneWindow, &geszone::requestBack, this, [this]() { this->show(); });

    // MainWindow button
    connect(ui->geszone1, &QPushButton::clicked, this, [this]() {
        this->hide();
        geszoneWindow->show();
    });
}

MainWindow::~MainWindow()
{
    delete ui;
    delete geszoneWindow;
}
