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

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void setupTable();

private slots:
    // Navigation buttons
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
    void on_ajzone_clicked();
    void on_ajpoub_clicked();
    void on_suppzone_clicked();
    void on_supppoub_clicked();
    void afficherZones();
    void afficherPoubelles();
    void modZone();   // Modify Zone
    void modPoubelle(); // Modify Poubelle
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

    void on_btn_satisfaction_refresh_clicked();
    void on_btn_sondage_submit_clicked();
    void on_affichersatisfaction_clicked();

    void on_statistique_clicked();
    void on_statH_clicked();

private:
    Ui::MainWindow *ui;
    void chargerDonneesTable();
    void rechercherTexte(const QString &texte);
    // Validation des valeurs autorisées
    QStringList allowedStatuses;
    QString demandeStatusFilter; // filtre courant pour l'affichage des demandes
    void chargerStatutsAutorises();
    bool statutAutorise(const QString &value) const;
    void styliserChampsSaisie();
    void saveLastHabitantId(int id);
    void loadLastHabitantSelection();
    void populateEditsFromRow(int row);
    void afficherPersonnel(QTableView *tableView, QSqlQueryModel *model);
    void refreshSatisfactionTable();
    QTableView* resolveSatisfactionTableView() const;
    void refreshDemandeTable();
    void updateSatisfactionKpi();

};

#endif // MAINWINDOW_H
