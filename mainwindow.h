#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "arduino.h"
#include <QMainWindow>
#include <QTableWidget>
#include <QPushButton>
#include <QTableView>
#include <QSqlQueryModel>
#include <QModelIndex>
#include <QItemSelection>
#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QLineSeries>
#include <QLabel>
#include <QComboBox>
#include <QTimer>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QTextEdit>
#include <QDialog>

#include "zone.h"
#include "poubelle.h"
#include "habitantcrud.h"
#include "vehiculecrud.h"
//#include "maintenance.h"
//#include "consomation.h"
#include "habitat.h"
#include "consommationeau.h"
#include "graph.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    // Graph
    void refreshGraph();
    void renderBinsGraph(int zoneId);
    void setupTable();

private slots:

    // ============================
    // NAVIGATION
    // ============================
    void setupGraph();
    void on_pushButton_clicked();
    void on_pushButton2_clicked();
    void on_pushButton3_clicked();

    void on_on_pushButton_clicked();
    void on_on_pushButton_2_clicked();
    void on_on_pushButton_3_clicked();
    void on_on_pushButton_4_clicked();
    void on_on_pushButton_5_clicked();
    void on_on_pushButton_6_clicked();
    void on_on_pushButton_7_clicked();
    void on_on_pushButton_8_clicked();
    void on_on_pushButton_9_clicked();
    void on_on_pushButton_13_clicked();
    void on_on_pushButton_14_clicked();
    void on_on_pushButton_15_clicked();
    void on_on_pushButton_16_clicked();
    void on_on_pushButton_17_clicked();
    void on_pushButton_15_clicked();

    void on_Ghabitant_clicked();
    void on_Ghabitats_2_clicked();
    void on_Gpersonnels_2_clicked();
    void on_Gequipements_2_clicked();
    void on_Gvehicules_2_clicked();
    void on_Gespace_clicked();
    void on_gespub_2_clicked();
    void on_geszone1_clicked();
    void on_pushButton_30_clicked();

    // ============================
    // MODULE ZONES + POUBELLES
    // ============================
    void on_searchpp_clicked();
    void on_tri_2_clicked();
    void on_exp_2_clicked();
    void on_ajzone_clicked();
    void on_ajpoub_clicked();
    void on_statisticpoubelles_clicked();
    void on_suppzone_clicked();
    void on_supppoub_clicked();
    void afficherZones();
    void afficherPoubelles();
    void modZone();
    void modPoubelle();
    void on_twz_cellClicked(int row, int column);
    void on_twp_cellClicked(int row, int column);
    void on_searchzone_clicked();
    void on_exp_clicked();
    void on_tri_clicked();
    void reloadBinsCache();
    void on_irl_clicked();

public slots:
    void refreshIrlDialog();
    void appendIrlDebug(const QString &msg);

    // ============================
    // MODULE HABITANTS
    // ============================
    void on_pushButton_ajoutH_clicked();
    void on_pushButton_modifH_clicked();
    void on_pushButton_suppH_clicked();
    void on_pushButton_afficheH_clicked();
    void on_pushButton_rechH_clicked();
    void on_pushButton_trieH_clicked();
    void on_tableView_habitants_clicked(const QModelIndex &index);
    void on_exportH_clicked();

    // ============================
    // MODULE VEHICULES
    // ============================
    void on_btn_vehicule_ajouter_clicked();
    void on_btn_vehicule_modifier_clicked();
    void on_btn_vehicule_supprimer_clicked();
    void on_btn_vehicule_rechercher_clicked();
    void on_btn_vehicule_afficher_clicked();
    void on_tableView_vehicule_clicked(const QModelIndex &index);
    void on_btn_vehicule_tri_clicked();
    void on_btn_vehicule_export_clicked();

    // ============================
    // MODULE PERSONNELS
    // ============================
    void on_ajouterperso_clicked();
    void on_Modifierperso_clicked();
    void on_Modifierperso_2_clicked();
    void on_Supprimerperso_clicked();
    void on_Affperso_clicked();
    void supprimerEtDecalerLignes();
    void on_pushButton_40_clicked();
    void on_comboBox_10_currentIndexChanged(int index);
    void on_pushButton_18_clicked();

    // Demandes
    void on_DemandeH_clicked();
    void on_suppdemande_clicked();
    void on_statdemande_clicked();
    void on_Affecter_clicked();
    void on_Refresh_clicked();
    void on_Resolution_clicked();
    void on_Resolu_clicked();
    void on_pushButton_detailsSuivi_clicked();
    void on_pushButton_exportSuivi_clicked();

    // Satisfaction
    void on_btn_satisfaction_refresh_clicked();
    void on_btn_sondage_submit_clicked();
    void on_affichersatisfaction_clicked();
    void on_statistique_clicked();
    void on_statH_clicked();

    // ============================
    // MODULE HABITAT
    // ============================
    void on_gestionhabitat_button_clicked();
    void on_surveilalnce_button_clicked();
    void on_btnAjouter_clicked();
    void on_btnSupprimer_clicked();
    void on_btnModifier_clicked();
    void on_btnafficher_clicked();
    void on_TabCrudHabitat_itemClicked(QTableWidgetItem *item);
    void on_lineRechercheHabitat_textChanged(const QString &text);

    // ============================
    // MODULE CONSOMMATION
    // ============================
    void on_AJOUTconso_clicked();
    void on_btnAfficherConso_clicked();          // ✅ AJOUTÉ
    void on_btnSupprimerConso_clicked();
    void on_btnModifierConso_clicked();
    void on_table_surveillance_itemClicked(QTableWidgetItem *item);
    void on_comboTriConso_currentIndexChanged();
    void on_lineRechercheEtat_textChanged(const QString &text);
    void on_btnStatistiques_clicked();
    void on_pushButton_exporter_clicked();

    // ============================
    // SMS – ALERTES
    // ============================
    void on_btnActiverAlertesAuto_clicked();
    void on_btnVerifierSMSEntrants_clicked();
    void on_btnTestSMS_clicked();
    //arduino

    void onTemperatureRecue(double temp);
    void on_btnEnvoyerArduino_clicked();         // ← AJOUTER
    void on_btnEnvoyerArduino2_clicked();        // ← AJOUTER
    void verifierConnexionArduino();             // ← AJOUTER
    void envoyerDonneesArduino(int idHabitant);  // ← AJOUTER



private:
    Ui::MainWindow *ui;
    Arduino *arduino;
    double derniereTemperature = 0.0;

    // ============================
    // Variables SMART CITY
    // ============================
    QLabel *binMessageLabel = nullptr;
    QStringList allowedStatuses;
    QString demandeStatusFilter;
    QLabel *affectationSummaryLabel = nullptr;
    QGraphicsScene *zonesScene = nullptr;
    QGraphicsScene *binsScene = nullptr;
    Graph *graphScene = nullptr;

    struct BinInfo {
        int id = 0;
        int zoneId = 0;
        int cap = 0;
        QString status;
        double usedPercentage = 0.0;
    };
    QVector<BinInfo> binsCache;
    int currentBinsZoneId = -1;

    // IRL Dialog pointers
    QDialog *irlDialog = nullptr;
    QTableWidget *irlBinTable = nullptr;
    QTextEdit *irlDebugConsole = nullptr;


    // ============================
    // HABITAT & CONSOMMATION
    // ============================
    Habitat habitatManager;
    Consommation consommationManager;
    int selectedHabitatId = -1;
    int selectedConsoId = -1;

    // ============================
    // TWILIO
    // ============================
    QString TWILIO_ACCOUNT_SID;
    QString TWILIO_AUTH_TOKEN;
    QString TWILIO_PHONE_NUMBER;

    QNetworkAccessManager *networkManager = nullptr;
    QTimer *timerAlertes = nullptr;
    QString critereTri;

    // ============================
    // MÉTHODES PRIVÉES
    // ============================
    void chargerDonneesTable();
    void rechercherTexte(const QString &texte);
    void chargerStatutsAutorises();
    bool statutAutorise(const QString &value) const;
    void styliserChampsSaisie();
    void styliserTableView(QTableView *view);
    void styliserBoutonPrincipal(QPushButton *button);
    void styliserModulePersonnels();
    void updateAffectationSummary();
    void ouvrirDemandeDansSuivi(int idDemande);
    void saveLastHabitantId(int id);
    void loadLastHabitantSelection();
    void populateEditsFromRow(int row);
    void afficherPersonnel(QTableView *tableView, QSqlQueryModel *model);
    int suggestNextPersonnelId() const;
    void refreshSatisfactionTable();
    QTableView* resolveSatisfactionTableView() const;
    void refreshDemandeTable();
    void chargerTablesAffectation(bool urgences = false, bool onlyDisponibles = false);
    void remplirTableSuivi();
    void filtrerUrgencesSuivi(bool urgencesSeulement);
    void updateSatisfactionKpi();
    void chargerTableVehicules();

    // Habitat / Conso
    void initialiserTable();
    void clearFieldsHabitat();
    void clearFieldsConso();
    void afficherHabitats();
    void afficherConsommations();
    void afficherDerniereConsommation() ;


    bool envoyerSMSTwilio(const QString &numeroTelephone, const QString &message);
    void verifierGaspillagesEtEnvoyerSMS();
    void chargerZones();
    void showBigBatteryDialog(int binId);
protected:
    bool eventFilter(QObject *obj, QEvent *event) override;
};

#endif // MAINWINDOW_H
