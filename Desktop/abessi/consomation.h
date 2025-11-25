#ifndef CONSOMATION_H
#define CONSOMATION_H

#include <QString>
#include <QSqlQueryModel>

class QWidget;
class QTableView;

class consomation
{
private:
    int idVehicule;
    QString matricule;
    double distance;
    double consoEntree;
    double resultatCalcule;

public:
    consomation();
    consomation(int idVehicule, QString matricule, double distance, double consoEntree, double resultatCalcule);

    // CRUD
    bool ajouter(QString *messageErreur = nullptr);
    bool modifier(int idVehicule);
    bool supprimer(int idVehicule);
    static QSqlQueryModel* afficher(QObject *parent = nullptr);
    
    // Validation
    bool validerChamps(QString &messageErreur);
};

class ConsomationModule : public QObject
{
public:
    explicit ConsomationModule(QWidget *root);

private:
    QWidget *m_root = nullptr;

    void initTable();
    void applyTableStyle(QTableView *tableView);
    QTableView* consommationTable() const;
    bool vehiculeExiste(int id, const QString &matricule) const;
    void remplirModelLocal(QTableView *tableView);
    void ajouterLigneLocale(QTableView *tableView,
                            const QString &idStr,
                            const QString &matricule,
                            const QString &distanceStr,
                            const QString &consoStr,
                            const QString &resultatStr,
                            const QString &dateStr);
    void calculer();
    void effacer();
};

#endif // CONSOMATION_H
