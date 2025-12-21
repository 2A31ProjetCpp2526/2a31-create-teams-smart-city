#ifndef MAINTENANCE_H
#define MAINTENANCE_H

#include <QObject>

class QWidget;
class QStandardItemModel;
class QTableView;

class Maintenance : public QObject
{
    Q_OBJECT

public:
    explicit Maintenance(QWidget *root, QObject *parent = nullptr);

public slots:
    void diagnostiquer();
    void ajouter();
    void effacer();

private:
    QWidget *m_root = nullptr;
    QStandardItemModel *m_historyModel = nullptr;

    void initHistory();
    void getDiagnosticForProblem(const QString &probleme, QString &alerte, QString &diagnostic);
    bool vehiculeExiste(const QString &idVehicule, const QString &matricule) const;
    bool chargerHistoriqueDepuisBase(QTableView *tv = nullptr);
    bool enregistrerHistoriqueSql(int idVehicule,
                                  const QString &matricule,
                                  const QString &probleme,
                                  const QString &alerte,
                                  const QString &diagnostic,
                                  QString &erreurSql);
};

#endif // MAINTENANCE_H
