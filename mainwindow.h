#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidgetItem>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QTimer>
#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QValueAxis>
#include <QDialog>
#include <QVBoxLayout>



QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // Navigation
    void on_gestionhabitat_button_clicked();
    void on_surveilalnce_button_clicked();

    // Habitat CRUD
    void on_btnAjouter_clicked();
    void on_btnSupprimer_clicked();
    void on_btnModifier_clicked();
    void on_btnafficher_clicked();
    void on_TabCrudHabitat_itemClicked(QTableWidgetItem *item);
    void on_lineRechercheHabitat_textChanged(const QString &text);

    // Consommation CRUD
    void on_AJOUTconso_clicked();
    void on_btnSupprimerConso_clicked();
    void on_btnModifierConso_clicked();
    void on_table_surveillance_itemClicked(QTableWidgetItem *item);

    // Tri et recherche
   void on_comboTriConso_currentIndexChanged();  // Sans paramètre

    void on_lineRechercheEtat_textChanged(const QString &text);
    void on_btnStatistiques_clicked();

    // SMS et Alertes
    void on_btnActiverAlertesAuto_clicked();
    void on_btnVerifierSMSEntrants_clicked();
    void on_btnTestSMS_clicked();
    void traiterReponseEnvoiSMS(QNetworkReply *reply);

    // Export
    void on_pushButton_exporter_clicked();

private:
    Ui::MainWindow *ui;

    // ========================================
    // Variables membres PRIVÉES
    // ========================================
    // Twilio
    QString TWILIO_ACCOUNT_SID;
    QString TWILIO_AUTH_TOKEN;
    QString TWILIO_PHONE_NUMBER;

    // Network et Timer
    QNetworkAccessManager *networkManager;
    QTimer *timerAlertes;

    // Critère de tri
    QString critereTri;

    // ========================================
    // Méthodes privées
    // ========================================
    void initialiserTable();
    void clearFieldsHabitat();
    void clearFieldsConso();
    void afficherHabitats();
    void afficherConsommations();

    // SMS - GARDEZ UNIQUEMENT CELLE-CI
    bool envoyerSMSTwilio(const QString &numeroTelephone, const QString &message);

    void verifierGaspillagesEtEnvoyerSMS();

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;
};

#endif // MAINWINDOW_H
