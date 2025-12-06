#include <QSerialPort>
class MainWindow; // forward declaration to avoid circular include
#ifndef POUBELLE_H
#define POUBELLE_H

#include <QString>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSqlError>
#include <QDebug>

class Poubelle
{
private:
    int id_bin;
    int id_zone;
    double capacite;
    double used_percentage; // 0..100
    QString status;

public:
    Poubelle();
    Poubelle(int id_bin, int id_zone, double capacite, QString status, double used_percentage = 0.0);
    double getUsedPercentage() const;
    void setUsedPercentage(double pct);

    bool ajouter();
    bool supprimer(int id_bin);
    bool modifier(int id_bin);
    bool modifierUsedPercentage(int id_bin, double usedPct);

    // Arduino / Serial integration (static)
    static QSerialPort* pirSerial;
    static MainWindow* mainWindowPtr;
    static void setupPIRSerial(MainWindow* mw = nullptr);
    static void onPirSerialReady();
    static void handlePIRMotion(int binId);
    QSqlQueryModel* afficher();
    QSqlQueryModel* trier(int index);
    QSqlQueryModel* rechercher(int id);
};

#endif // POUBELLE_H
