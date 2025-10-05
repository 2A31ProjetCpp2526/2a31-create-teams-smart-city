#include "geszone.h"
#include "ui_geszone.h"

geszone::geszone(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::geszone)
{
    ui->setupUi(this);

    // Connect back button
    connect(ui->gespub, &QPushButton::clicked, this, &geszone::goBack);
}

geszone::~geszone()
{
    delete ui;
}

void geszone::goBack()
{
    emit requestBack();
    this->hide();
}
