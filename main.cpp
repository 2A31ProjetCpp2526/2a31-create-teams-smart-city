#include <QApplication>
#include "mainwindow.h"
#include "connection.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Optional: set Fusion style globally
    a.setStyle("Fusion");

    // Initialize database connection
    if (!Connection::getInstance()->openConnection()) {
        //qDebug() << "Erreur: Impossible de se connecter à la base de données.";
        return -1;
    }

    MainWindow w;
    w.show();

    int result = a.exec();

    // Close DB connection before exiting
    Connection::getInstance()->closeConnection();

    return result;
}

