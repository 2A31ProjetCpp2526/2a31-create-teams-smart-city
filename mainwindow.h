#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "CalendarManager.h"
#include <QMainWindow>
#include <QSortFilterProxyModel>
#include <QSqlQueryModel>
#include <QMap>
#include <QDate>
#include <QTimer>
#include "zone.h"
#include "rainsensor.h"

//#include "eventscalendar.h"
// ADD THIS LINE

class QLineEdit;
class QPushButton;
class QTableView;
class QLabel;
class QCalendarWidget;
class QListWidget;
class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QWidget;
class SmartMetalDialog;
class DemandeDialog;
class ChartDialog;
class RainSensor;  // ADD THIS LINE if not already included

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    //void showEventsDialog();
    void formatDateForOracle(const QDate &date);

private slots:
    // CRUD Operations
    void afficherZones();
    void ajouterZone();
    void modifierZone();
    void supprimerZone();
    //void onShowEventsClicked();
    //void RainSensor+

    // Calculations
    void calculerTotalZones();
    void calculerTotalSuperficie();

    // Classification
    void classifierZonePoubelle();

    // Calendar
    void toggleCalendar();
    void onCalendarDateSelected();
    void onAddEventClicked();
    void onRemoveEventClicked();
    void checkEventNotifications();
    //void loadEvents();

    // Search & Sort
    void setupSearchAndSort();
    void onSearchTextChanged(const QString &text);
    void ouvrirTri();

    // PDF Export
    void exportToPdf();
    //void showEventsFromDB();

    // Environmental Analysis
    void showEcoConclusionDialog();

    // Rain Sensor slots - ADD THESE
    void onRainStateChanged(bool rainDetected);
    void connectRainSensor();
    void disconnectRainSensor();
   // void handleZoneClicked(const QDate &date);

private:
    // UI Setup Methods
    void setupUI();
    void setupSidebar();
    void setupForm();
    void setupButtons();
    void setupTable();
    void setupConnections();
    void setupCalendarConnections();
    //EventsCalendar eventsCalendar;
    QPushButton* showEventsBtn;
    QPushButton* DemandeInterventionBtn;

    //void showEvents();


    // Helper Methods
    void clearForm();
    //void formatDateForOracle(const QDate &date);
    //QString formatDateForOracle(const QDate &date);
    //void formatDateForOracle(const QDate &date);
    double computeOxygenProduction(double superficieHa);
    double computeCO2Reduction(double superficieHa);
    void updateCalendarHighlights();
    //void formatDateForOracle(QDate const&);
   // void loadEventsFromDb();
    void updateEventList();
    //void loadTable();
    void onZoneClicked(const QDate&);
    void on_btnDemande_clicked();
    void ouvrirDemandes();


    //CalendarColorManager *colorManager;
    //QCalendarWidget *calendarWidget;

    //void showEventsDialog();

private:
    // UI Elements
    QWidget *sidebarWidget;
    QVBoxLayout *sidebarLayout;
    QCalendarWidget *calendar;

    // Form Fields
    QLineEdit *idEdit;
    QLineEdit *nomEdit;
    QLineEdit *typeEdit;
    QLineEdit *supEdit;
    QLineEdit *locEdit;
    QLineEdit *respEdit;
    QLineEdit *etatEdit;
    QLineEdit *pluieEdit;

    // Buttons
    QPushButton *addBtn;
    QPushButton *updateBtn;
    QPushButton *deleteBtn;
    QPushButton *refreshBtn;
    QPushButton *calcTotalZonesBtn;
    QPushButton *calcTotalAreaBtn;
    QPushButton *classifierPoubelleBtn;
    QPushButton *sortBtn;
    QPushButton *toggleCalendarBtn;
    QPushButton *ecoAnalysisBtn;
    QPushButton *ChartBtn;
    QPushButton *smartMetalBtn;
    QPushButton *exportPdfBtn;
    //QPushButton *DemandeBtn;


    // Rain Sensor Buttons - ADD THESE
    QPushButton *connectRainSensorBtn;
    QPushButton *disconnectRainSensorBtn;
    QPushButton *SportsChallengeBtn;
    QPushButton *SortingGameWidgetBtn;

    // Sidebar Buttons
    QPushButton *addEventBtn;
    QPushButton*removeEventBtn;
    QPushButton *btnShowEvents;


    // Search
    QLineEdit *searchEdit;
    QPushButton *searchClearBtn;

    // Table
    QTableView *tableView;
    QSortFilterProxyModel *proxyModel;

    // Calendar
    //QCalendarWidget *calendar;
    QListWidget *eventList;
    bool isCalendarVisible;
    QMap<QDate, QStringList> events;

    // Labels
    QLabel *totalZonesLabel;
    void setupStyle();
    QLabel *totalAreaLabel;
    QLabel *rainStatusLabel;  // ADD THIS LINE

    // Layouts
    QGridLayout *formLayout;
    QHBoxLayout *btnLayout;

    // Database
    Zone *Ztmp;

    // Timer
    QTimer *notificationTimer;
    //Demande *demandeHandler;

    // Rain Sensor - ADD THIS
    RainSensor *rainSensor;
    //CalendrierZones* calendrierZones;
    //QTableWidget* eventTable;  // to show events per date

};

#endif // MAINWINDOW_H
