#ifndef MAINWINDOW_H
#define MAINWINDOW_H

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
#include <QPushButton>
#include <QComboBox>
#include "zone.h"
#include "poubelle.h"
#include "habitantcrud.h"
#include "vehiculecrud.h"
#include "graph.h"
#include "arduino.h"
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void refreshGraph();
    void renderBinsGraph(int zoneId);
    void setupTable();
private slots:
    // Navigation buttons
    void setupGraph();
    void on_pushButton_clicked();
    void on_pushButton2_clicked();
    void on_pushButton3_clicked();

    void on_on_pushButton_clicked();
    void on_on_pushButton_2_clicked();
    void on_on_pushButton_3_clicked();

    void on_Ghabitant_clicked();
    void on_Ghabitats_2_clicked();
    void on_Gpersonnels_2_clicked();
    void on_Gequipements_2_clicked();
    void on_Gvehicules_2_clicked();
    void on_Gespace_clicked();

    void on_on_pushButton_4_clicked();
    void on_on_pushButton_5_clicked();
    void on_on_pushButton_6_clicked();

    void on_on_pushButton_7_clicked();
    void on_on_pushButton_8_clicked();
    void on_on_pushButton_9_clicked();

    // Fixed slots (was causing linker errors)
    void on_on_pushButton_13_clicked();
    void on_on_pushButton_14_clicked();
    void on_on_pushButton_15_clicked();
    void on_on_pushButton_16_clicked();
    void on_on_pushButton_17_clicked();
    void on_pushButton_15_clicked();

    void on_gespub_2_clicked();
    void on_geszone1_clicked();

    void on_pushButton_30_clicked();

    // CRUD buttons
    void on_searchpp_clicked();   // Search poubelle
    void on_tri_2_clicked();      // Sort poubelle
    void on_exp_2_clicked();
    void on_ajzone_clicked();
    void on_ajpoub_clicked();
    void on_statisticpoubelles_clicked();
    void on_suppzone_clicked();
    void on_supppoub_clicked();
    void afficherZones();
    void afficherPoubelles();
    void modZone();   // Modify Zone
    void modPoubelle(); // Modify Poubelle
    void on_twz_cellClicked(int row, int column);   // Table cell click
    void on_twp_cellClicked(int row, int column);
    void on_searchzone_clicked();      // Search zone
    void on_exp_clicked();             // Export table
    void on_tri_clicked();             // Sort table
    void reloadBinsCache();

    // === CRUD HABITANTS ===
    void on_pushButton_ajoutH_clicked();
    void on_pushButton_modifH_clicked();
    void on_pushButton_suppH_clicked();
    void on_pushButton_afficheH_clicked();
    // === CRUD VEHICULES ===
    void on_btn_vehicule_ajouter_clicked();
    void on_btn_vehicule_modifier_clicked();
    void on_btn_vehicule_supprimer_clicked();
    void on_btn_vehicule_rechercher_clicked();
    void on_btn_vehicule_afficher_clicked();
    void on_tableView_vehicule_clicked(const QModelIndex &index);



    // === Recherche & Tri ===
    void on_pushButton_rechH_clicked();
    void on_pushButton_trieH_clicked();
    void on_tableView_habitants_clicked(const QModelIndex &index);

    // === Export Habitant Table ===
    void on_exportH_clicked();

    // === CRUD PERSONNELS ===
    void on_ajouterperso_clicked();
    void on_Modifierperso_clicked();
    void on_Modifierperso_2_clicked();
    void on_Supprimerperso_clicked();
    void on_Affperso_clicked();

    // Generic delete for table rows
    void supprimerEtDecalerLignes();

    void on_DemandeH_clicked();
    void on_suppdemande_clicked();
    void on_statdemande_clicked();

    void on_Affecter_clicked();
    void on_Refresh_clicked();
    void on_Resolution_clicked();
    void on_Resolu_clicked();

    void on_pushButton_detailsSuivi_clicked();
    void on_pushButton_exportSuivi_clicked();

    void on_pushButton_40_clicked(); // Rechercher des personnels (les metiers basiques)
    void on_comboBox_10_currentIndexChanged(int index); // Tri des personnels
    void on_pushButton_18_clicked(); // Exporter personnels en PDF

    void on_btn_satisfaction_refresh_clicked();
    void on_btn_sondage_submit_clicked();
    void on_affichersatisfaction_clicked();

    void on_statistique_clicked();
    void on_statH_clicked();
    
    // Arduino LCD
    void envoyerDonneesArduino(int idHabitant);

private:
    Ui::MainWindow *ui;
    QLabel *binMessageLabel = nullptr;
    void chargerDonneesTable();
    void rechercherTexte(const QString &texte);
    // Validation des valeurs autorisées
    QStringList allowedStatuses;
    QString demandeStatusFilter; // filtre courant pour l'affichage des demandes
    void chargerStatutsAutorises();
    bool statutAutorise(const QString &value) const;
    void styliserChampsSaisie();
    void styliserTableView(QTableView *view);
    void styliserBoutonPrincipal(QPushButton *button);
    void styliserModulePersonnels();
    void updateAffectationSummary();
    QLabel *affectationSummaryLabel;
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
    Graph *graphScene = nullptr;
    QGraphicsScene *zonesScene = nullptr;
    QGraphicsScene *binsScene = nullptr;
     void on_btnEnvoyerArduino_clicked();
         void verifierConnexionArduino();
     void  on_btnEnvoyerArduino2_clicked();

    struct BinInfo {
        int id = 0;
        int zoneId = 0;
        int cap = 0;
        QString status;
    };
    QVector<BinInfo> binsCache;
    int currentBinsZoneId = -1;
    Arduino *arduino = nullptr; // Objet Arduino pour communication série
};

#endif // MAINWINDOW_H
