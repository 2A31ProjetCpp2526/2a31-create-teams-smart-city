#include "mainwindow.h"
#include "connection.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Connection *c = Connection::getInstance();
    if (!c->openConnection()) return -1;

    MainWindow w;
    w.show();
    return a.exec();
}
