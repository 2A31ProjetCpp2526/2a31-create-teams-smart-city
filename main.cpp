#include <QApplication>
#include "mainwindow.h"
#include "connection.h"
#include "SportsChallengeDialog.h"
#include"SortingGameWidget.h"
#include"demandedialog.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Optional: set Fusion style globally
    a.setStyle("Fusion");

    // Initialize database connection
    if (!Connection::getInstance()->openConnection()) {
        qDebug() << "Erreur: Impossible de se connecter à la base de données.";
        return -1;
    }

    SortingGameWidget *Dlg= new SortingGameWidget;
    Dlg->show();

    DemandeDialog *DLG=new DemandeDialog ;  // Pass the connected database
    DLG->show();                   // Open the dialog modally



    SportsChallengeDialog *dlg = new SportsChallengeDialog;
    dlg->show(); // non-blocking


   //SportsChallengeDialog dlg;
    //dlg.show();
    //dlg.exec();


    MainWindow w;
    w.show();

    int result = a.exec();

    // Close DB connection before exiting
    Connection::getInstance()->closeConnection();

    return result;
}



