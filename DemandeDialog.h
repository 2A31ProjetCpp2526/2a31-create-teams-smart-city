#ifndef DEMANDEDIALOG_H
#define DEMANDEDIALOG_H

#include <QDialog>
#include <QTableWidget>
#include <QLineEdit>
#include <QComboBox>
#include <QDateTimeEdit>
#include <QPushButton>
#include <QDateTime>
#include <QSqlDatabase>

class DemandeDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DemandeDialog(QWidget *parent = nullptr);

private slots:
    void ajouterDemande();
    void rafraichirDemandes();

private:
    // ---------- UI ----------
    QTableWidget *tableDemandes;
    QLineEdit *zoneIdEdit;
    QLineEdit *maintenanceEdit;
    QComboBox *maintenanceCombo;
    QDateTimeEdit *dateEdit;
    QPushButton *btnAjouter;

    // ---------- MÉTHODES ----------
    void setupUI();
    void chargerDemandes();
    void addDemandeRow(const QString &zoneId,
                       const QString &etat,
                       const QDateTime &date,
                       const QString &maintenance);

    bool creerDemande(const QString &zoneId,
                      const QString &etat,
                      const QDateTime &date,
                      const QString &maintenance);

    bool estPluvieux() const;
    bool estConnecte() const;
};

#endif // DEMANDEDIALOG_H
