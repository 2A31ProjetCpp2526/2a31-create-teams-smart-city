#include "mainwindow.h"
#include "connection.h"
#include "zone.h"
#include "poubelles.h"
#include "ChartDialog.h"
#include "sortdialog.h"
#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QSortFilterProxyModel>
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QInputDialog>
#include <QHeaderView>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QTableView>
#include <QCalendarWidget>
#include <QListWidget>
#include <QFileDialog>
#include <QPdfWriter>
#include <QPainter>
#include <QPixmap>
#include <QTimer>
#include <QTextCharFormat>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QDebug>
#include <QCalendar>
#include <QPalette>
#include <QCalendarWidget>
#include "smartmetal.h"
#include "calendrierzones.h"
#include <QSystemTrayIcon>
#include "arduino.h"
#include "rainsensor.h"
#include "calendrierzones.h"
#include "SortingGameWidget.h"
#include "SportsChallengeDialog.h"
#include "DemandeDialog.h"


// Add this line
//QT_CHARTS_USE_NAMESPACE

// --------------------------------------------------------------
// CONSTRUCTOR
// --------------------------------------------------------------
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    proxyModel(nullptr),
    isCalendarVisible(false),
    Ztmp(new Zone()),
    rainSensor(nullptr)  // ADD THIS
{
    setWindowTitle("Dashboard - ZONES VERTES");
    resize(1400, 700);

    // ---- DB Connection ----
    Connection *c = Connection::getInstance();
    if (!c->openConnection()) {
        QMessageBox::critical(this, "Erreur", "Échec de connexion à la base de données !");
    } else {
        // Auto-create table if not exists
        QSqlQuery q;
        QString ddl =
            "BEGIN "
            "   EXECUTE IMMEDIATE 'CREATE TABLE ZONES_VERTES ("
            "       ID_ZONE VARCHAR2(20 BYTE) NOT NULL, "
            "       NOM VARCHAR2(50 BYTE), "
            "       TYPE VARCHAR2(50 BYTE), "
            "       SUPERFICIE NUMBER, "
            "       LOCALISATION VARCHAR2(100 BYTE), "
            "       RESPONSABLE VARCHAR2(50 BYTE), "
            "       ETAT VARCHAR2(50 BYTE), "
            "       PLUIE BINARY_DOUBLE"
            "   )'; "
            "EXCEPTION WHEN OTHERS THEN "
            "   IF SQLCODE = -955 THEN NULL; ELSE RAISE; END IF; "
            "END;";

        if (!q.exec(ddl)) {
            qDebug() << "Table creation error:" << q.lastError().text();
        }
    }

    // Initialize Rain Sensor
    rainSensor = new RainSensor(this);

    setupUI();
    afficherZones();
    setupConnections();

    // Notifications timer
    notificationTimer = new QTimer(this);
    connect(notificationTimer, &QTimer::timeout, this, &MainWindow::checkEventNotifications);
    notificationTimer->start(60000);
}
MainWindow::~MainWindow()
{
    if (notificationTimer && notificationTimer->isActive())
        notificationTimer->stop();

    if (rainSensor) {
        delete rainSensor;
    }

    delete Ztmp;
}





// --------------------------------------------------------------
// UI SETUP
// --------------------------------------------------------------
void MainWindow::setupUI()
{
    QWidget *central = new QWidget(this);
    setCentralWidget(central);

    QHBoxLayout *mainLayout = new QHBoxLayout(central);

    // Sidebar
    setupSidebar();
    mainLayout->addWidget(sidebarWidget);

    // LEFT panel
    QVBoxLayout *leftLayout = new QVBoxLayout();
    leftLayout->setSpacing(12);

    setupForm();
    leftLayout->addWidget(totalZonesLabel);
    leftLayout->addWidget(totalAreaLabel);
    leftLayout->addLayout(formLayout);

    setupButtons();
    leftLayout->addLayout(btnLayout);

    // Search Row
    QHBoxLayout *searchLayout = new QHBoxLayout();
    searchEdit = new QLineEdit();
    searchEdit->setPlaceholderText("Rechercher...");
    searchClearBtn = new QPushButton("Clear");
    searchLayout->addWidget(searchEdit);
    searchLayout->addWidget(searchClearBtn);
    leftLayout->addLayout(searchLayout);

    setupTable();
    leftLayout->addWidget(tableView);

    exportPdfBtn = new QPushButton("Exporter PDF");
    exportPdfBtn->setStyleSheet("background-color:#007F8F;color:white;padding:8px;border-radius:8px;");
    leftLayout->addWidget(exportPdfBtn);

    mainLayout->addLayout(leftLayout);

    // Calendar panel
    QVBoxLayout *calLayout = new QVBoxLayout();

    calendar = new QCalendarWidget();
    calendar->setGridVisible(true);
    calendar->setVisible(false);
    calLayout->addWidget(calendar);

    eventList = new QListWidget();
    eventList->setVisible(false);
    calLayout->addWidget(eventList);

    addEventBtn = new QPushButton("Ajouter Événement");
    removeEventBtn = new QPushButton("Supprimer Événement");
    addEventBtn->setVisible(false);
    removeEventBtn->setVisible(false);

    QHBoxLayout *eventBtns = new QHBoxLayout();
    eventBtns->addWidget(addEventBtn);
    eventBtns->addWidget(removeEventBtn);

    calLayout->addLayout(eventBtns);
    mainLayout->addLayout(calLayout);

    sidebarWidget->show();







}


// --------------------------------------------------------------
// SIDEBAR
// --------------------------------------------------------------
void MainWindow::setupSidebar()
{
    sidebarWidget = new QWidget();
    sidebarWidget->setFixedWidth(220);
    sidebarWidget->setStyleSheet("background-color:#005B99;color:white;border-radius:12px;");

    sidebarLayout = new QVBoxLayout(sidebarWidget);
    sidebarLayout->setContentsMargins(15, 15, 15, 15);
    sidebarLayout->setSpacing(12);

    QString btnStyle =
        "QPushButton { background-color:#007F8F;color:white;padding:10px;border-radius:8px;font-weight:bold; }"
        "QPushButton:hover { background-color:#005B99; }";

    calcTotalZonesBtn = new QPushButton("Total Zones");
    calcTotalAreaBtn = new QPushButton("Superficie Totale");
    classifierPoubelleBtn = new QPushButton("Classifier Poubelles");
    sortBtn = new QPushButton("Trier");
    toggleCalendarBtn = new QPushButton("Afficher Calendrier");

    calcTotalZonesBtn->setStyleSheet(btnStyle);
    calcTotalAreaBtn->setStyleSheet(btnStyle);
    classifierPoubelleBtn->setStyleSheet(btnStyle);
    sortBtn->setStyleSheet(btnStyle);
    toggleCalendarBtn->setStyleSheet(btnStyle);

    sidebarLayout->addWidget(calcTotalZonesBtn);
    sidebarLayout->addWidget(calcTotalAreaBtn);
    sidebarLayout->addWidget(classifierPoubelleBtn);
    sidebarLayout->addWidget(sortBtn);
    sidebarLayout->addWidget(toggleCalendarBtn);

    // Add Rain Sensor buttons
    connectRainSensorBtn = new QPushButton("Connecter Capteur Pluie");
    disconnectRainSensorBtn = new QPushButton("Déconnecter Capteur");
    connectRainSensorBtn->setStyleSheet(btnStyle);
    disconnectRainSensorBtn->setStyleSheet(btnStyle);
    sidebarLayout->addWidget(connectRainSensorBtn);
    sidebarLayout->addWidget(disconnectRainSensorBtn);

    disconnectRainSensorBtn->setEnabled(false);  // Initially disabled

    sidebarLayout->addStretch();
    ecoAnalysisBtn = new QPushButton("Analyse Écologique");
    ecoAnalysisBtn->setStyleSheet(btnStyle);
    sidebarLayout->addWidget(ecoAnalysisBtn);

    ChartBtn = new QPushButton("Chartes");
    ChartBtn->setStyleSheet(btnStyle);
    sidebarLayout->addWidget(ChartBtn);

    smartMetalBtn = new QPushButton("Smart Metal Classifier");
    smartMetalBtn->setStyleSheet(btnStyle);
    sidebarLayout->addWidget(smartMetalBtn);
    showEventsBtn = new QPushButton("SortingGame");

    showEventsBtn->setStyleSheet(btnStyle);

    sidebarLayout->addWidget(showEventsBtn);
    QPushButton *sportsChallengeBtn = new QPushButton("🏃 Sports Challenge");
    sportsChallengeBtn->setFixedHeight(60);
    sportsChallengeBtn->setStyleSheet(
        "font-size: 22px; font-weight:bold; background:#5DADEC; color:white; border-radius:15px;"
        );
    QPushButton *DemandeInterventionBtn = new QPushButton("📋 Demandes ");
    DemandeInterventionBtn->setStyleSheet(btnStyle);
    sidebarLayout->addWidget(DemandeInterventionBtn);

    //connect(showEventsBtn, &QPushButton::clicked, this, &MainWindow::showEvents);
}


// --------------------------------------------------------------
// FORM
// --------------------------------------------------------------
void MainWindow::setupForm()
{
    idEdit = new QLineEdit();
    nomEdit = new QLineEdit();
    typeEdit = new QLineEdit();
    supEdit = new QLineEdit();
    locEdit = new QLineEdit();
    respEdit = new QLineEdit();
    etatEdit = new QLineEdit();
    pluieEdit = new QLineEdit();

    formLayout = new QGridLayout();
    formLayout->setSpacing(8);

    formLayout->addWidget(new QLabel("ID Zone:"), 0, 0);
    formLayout->addWidget(idEdit, 0, 1);
    formLayout->addWidget(new QLabel("Nom:"), 1, 0);
    formLayout->addWidget(nomEdit, 1, 1);
    formLayout->addWidget(new QLabel("Type:"), 2, 0);
    formLayout->addWidget(typeEdit, 2, 1);
    formLayout->addWidget(new QLabel("Superficie:"), 3, 0);
    formLayout->addWidget(supEdit, 3, 1);
    formLayout->addWidget(new QLabel("Localisation:"), 4, 0);
    formLayout->addWidget(locEdit, 4, 1);
    formLayout->addWidget(new QLabel("Responsable:"), 5, 0);
    formLayout->addWidget(respEdit, 5, 1);
    formLayout->addWidget(new QLabel("État:"), 6, 0);
    formLayout->addWidget(etatEdit, 6, 1);
    formLayout->addWidget(new QLabel("Pluie:"), 7, 0);
    formLayout->addWidget(pluieEdit, 7, 1);

    totalZonesLabel = new QLabel("Total Zones: 0");
    totalAreaLabel = new QLabel("Superficie Totale: 0 ha");

    // Add rain status label
    rainStatusLabel = new QLabel("🌧️ Statut Capteur: Non connecté");
    rainStatusLabel->setStyleSheet("font-weight: bold; padding: 5px;");
    formLayout->addWidget(rainStatusLabel, 8, 0, 1, 2);
}

// --------------------------------------------------------------
// BUTTONS
// --------------------------------------------------------------
void MainWindow::setupButtons()
{
    addBtn = new QPushButton("Ajouter");
    updateBtn = new QPushButton("Modifier");
    deleteBtn = new QPushButton("Supprimer");
    refreshBtn = new QPushButton("Actualiser");

    QString style =
        "QPushButton { background-color:#007F8F;color:white;padding:8px;border-radius:8px;font-weight:bold; }"
        "QPushButton:hover { background-color:#005B99; }";

    addBtn->setStyleSheet(style);
    updateBtn->setStyleSheet(style);
    deleteBtn->setStyleSheet(style);
    refreshBtn->setStyleSheet(style);

    btnLayout = new QHBoxLayout();
    btnLayout->addWidget(addBtn);
    btnLayout->addWidget(updateBtn);
    btnLayout->addWidget(deleteBtn);
    btnLayout->addWidget(refreshBtn);
}

// --------------------------------------------------------------
// TABLE
// --------------------------------------------------------------
void MainWindow::setupTable()
{
    tableView = new QTableView();
    tableView->setAlternatingRowColors(true);
    tableView->horizontalHeader()->setStretchLastSection(true);
    tableView->verticalHeader()->setVisible(false);
    tableView->setStyleSheet(
        "QTableView {"
        "    background-color: #f0f8ff;"
        "    alternate-background-color: #dbefff;"
        "    gridline-color: #007F8F;"
        "    color: #000000;"
        "    selection-background-color: #007F8F;"
        "    selection-color: white;"
        "}"
        "QHeaderView::section {"
        "    background-color: #007F8F;"
        "    color: white;"
        "    padding: 4px;"
        "    border: 1px solid #005B99;"
        "}"
        );
    tableView->setAlternatingRowColors(true);
}

// --------------------------------------------------------------
// CONNECTIONS
// --------------------------------------------------------------
void MainWindow::setupConnections()
{
    connect(addBtn, &QPushButton::clicked, this, &MainWindow::ajouterZone);
    connect(updateBtn, &QPushButton::clicked, this, &MainWindow::modifierZone);
    connect(deleteBtn, &QPushButton::clicked, this, &MainWindow::supprimerZone);
    connect(refreshBtn, &QPushButton::clicked, this, &MainWindow::afficherZones);

    connect(calcTotalZonesBtn, &QPushButton::clicked, this, &MainWindow::calculerTotalZones);
    connect(calcTotalAreaBtn, &QPushButton::clicked, this, &MainWindow::calculerTotalSuperficie);
    connect(classifierPoubelleBtn, &QPushButton::clicked, this, &MainWindow::classifierZonePoubelle);


    connect(sortBtn, &QPushButton::clicked, this, &MainWindow::ouvrirTri);
    connect(toggleCalendarBtn, &QPushButton::clicked, this, &MainWindow::toggleCalendar);

    connect(searchClearBtn, &QPushButton::clicked, searchEdit, &QLineEdit::clear);
    connect(searchEdit, &QLineEdit::textChanged, this, &MainWindow::onSearchTextChanged);

    connect(exportPdfBtn, &QPushButton::clicked, this, &MainWindow::exportToPdf);

    connect(calendar, &QCalendarWidget::selectionChanged, this, &MainWindow::onCalendarDateSelected);
    connect(addEventBtn, &QPushButton::clicked, this, &MainWindow::onAddEventClicked);
    connect(removeEventBtn, &QPushButton::clicked, this, &MainWindow::onRemoveEventClicked);

    connect(ecoAnalysisBtn, &QPushButton::clicked, this, &MainWindow::showEcoConclusionDialog);

    connect(smartMetalBtn, &QPushButton::clicked, this, [=]() {
        SmartMetalDialog dlg(this);
        dlg.exec();
    });

    connect(ChartBtn, &QPushButton::clicked, this, [=]() {
        ChartDialog *dlg = new ChartDialog(this);
        dlg->exec();
    });

    // Rain Sensor connections
    connect(connectRainSensorBtn, &QPushButton::clicked, this, &MainWindow::connectRainSensor);
    connect(disconnectRainSensorBtn, &QPushButton::clicked, this, &MainWindow::disconnectRainSensor);
    connect(rainSensor, &RainSensor::rainStateChanged, this, &MainWindow::onRainStateChanged);
    connect(DemandeInterventionBtn,&QPushButton::clicked,this, [=](){
           DemandeDialog *DLG=new DemandeDialog(this);
            DLG->exec();
    });








    //connect(showEventsBtn, &QPushButton::clicked, this, &MainWindow::showEventsDialog);







    // In MainWindow constructor after rainSensor is created
    //connect(rainSensor, &RainSensor::rainDetected, this, &MainWindow::onRainStateChanged);

}
// --------------------------------------------------------------
// CRUD
// --------------------------------------------------------------
void MainWindow::afficherZones()
{
    QSqlQueryModel *model = Ztmp->afficher();
    tableView->setModel(model);
    setupSearchAndSort();
    calculerTotalZones();
    calculerTotalSuperficie();

}

void MainWindow::ajouterZone()
{
    Zone z(idEdit->text(), nomEdit->text(), typeEdit->text(),
           supEdit->text().toDouble(), locEdit->text(),
           respEdit->text(), etatEdit->text(), pluieEdit->text().toDouble());

    if (z.ajouter()) {
        afficherZones();
        clearForm();
        QMessageBox::information(this, "Succès", "Zone ajoutée avec succès.");
    } else {
        QMessageBox::warning(this, "Erreur", "Impossible d'ajouter la zone.");
    }
}

void MainWindow::modifierZone()
{
    Zone z(idEdit->text(), nomEdit->text(), typeEdit->text(),
           supEdit->text().toDouble(), locEdit->text(),
           respEdit->text(), etatEdit->text(), pluieEdit->text().toDouble());

    if (z.modifier(idEdit->text())) {
        afficherZones();
        QMessageBox::information(this, "Succès", "Zone modifiée avec succès.");
    } else {
        QMessageBox::warning(this, "Erreur", "Impossible de modifier la zone.");
    }
}

void MainWindow::supprimerZone()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Confirmation",
                                  "Êtes-vous sûr de vouloir supprimer cette zone?",
                                  QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        if (Ztmp->supprimer(idEdit->text())) {
            afficherZones();
            clearForm();
            QMessageBox::information(this, "Succès", "Zone supprimée avec succès.");
        } else {
            QMessageBox::warning(this, "Erreur", "Impossible de supprimer la zone.");
        }
    }
}

void MainWindow::clearForm()
{
    idEdit->clear();
    nomEdit->clear();
    typeEdit->clear();
    supEdit->clear();
    locEdit->clear();
    respEdit->clear();
    etatEdit->clear();
    pluieEdit->clear();
}

// --------------------------------------------------------------
// CALCULS
// --------------------------------------------------------------
void MainWindow::calculerTotalZones()
{
    QSqlQuery q("SELECT COUNT(*) FROM ZONES_VERTES");
    if (q.next()) {
        totalZonesLabel->setText("Total Zones: " + q.value(0).toString());
    }
}

void MainWindow::calculerTotalSuperficie()
{
    QSqlQuery q("SELECT SUM(SUPERFICIE) FROM ZONES_VERTES");
    if (q.next()) {
        double superficie = q.value(0).toDouble();
        totalAreaLabel->setText("Superficie Totale: " + QString::number(superficie) + " ha");
    }
}

// --------------------------------------------------------------
// CLASSIFICATION POUBELLES
// --------------------------------------------------------------
void MainWindow::classifierZonePoubelle()
{
    int nbP = QInputDialog::getInt(this, "Nombre de poubelles", "Entrez le nombre :", 10, 1, 100);
    double taux = QInputDialog::getDouble(this, "Taux remplissage", "En % :", 60, 0, 100);
    int jours = QInputDialog::getInt(this, "Dernier ramassage", "Jours écoulés :", 2, 0, 30);

    QString r = Poubelle::classifierPoubelle(nbP, taux, jours);
    QString couleur = (r == "Propre") ? "green" : "red";

    // Affichage de la classification
    QMessageBox::information(this, "Classification",
                             "La zone est : <b><font color='" + couleur + "'>" + r + "</font></b>");

    // ---------------- Demande si saturée ----------------
    if (r == "Saturée") {
        // On demande l'ID de la zone pour créer la demande
        bool ok;
        QString zoneId = QInputDialog::getText(this, "Demande Personnel",
                                               "Entrez l'ID de la zone à nettoyer :", QLineEdit::Normal,
                                               "", &ok);
        if (ok && !zoneId.isEmpty()) {
            // Créer la demande
            //demandeHandler->creerDemande(zoneId, r);
        }
    }
}

// --------------------------------------------------------------
// CALENDAR
// --------------------------------------------------------------
void MainWindow::onAddEventClicked()
{
    if (!calendar || !eventList) return;

    QDate d = calendar->selectedDate();
    QString zone = QInputDialog::getText(this, "Zone", "Nom de la zone :");
    QString action = QInputDialog::getText(this, "Action", "Action :");

    if (zone.isEmpty() || action.isEmpty())
        return;

    QSqlQuery q;
    q.prepare("INSERT INTO SAIM.TABLE1 (ZONE, DATE_EVENT, ACTION) "
              "VALUES (:zone, :date_event, :action)");
    q.bindValue(":zone", zone);
    q.bindValue(":date_event", d);  // <--- bind QDate directement
    q.bindValue(":action", action);

    if (!q.exec()) {
        QMessageBox::warning(this, "Erreur SQL", "Impossible d'ajouter l'événement : " + q.lastError().text());
        return;
    }

    QString displayText = zone + ": " + action;
    events[d].append(displayText);
    eventList->addItem(displayText);

    updateCalendarHighlights();
    QMessageBox::information(this, "Succès", "Événement ajouté.");
}
void MainWindow::onRemoveEventClicked()
{
    auto selectedItemsList = eventList->selectedItems();
    if (selectedItemsList.isEmpty()) {
        QMessageBox::warning(this, "Aucune sélection", "Veuillez sélectionner un événement à supprimer.");
        return;
    }

    QDate d = calendar->selectedDate();

    for (QListWidgetItem *item : selectedItemsList) {
        QString text = item->text();
        QStringList parts = text.split(": ");
        if (parts.size() != 2)
            continue;

        QString zone = parts[0];
        QString action = parts[1];

        QSqlQuery q;
        q.prepare("DELETE FROM SAIM.TABLE1 "
                  "WHERE ZONE = :zone AND DATE_EVENT = :date_event AND ACTION = :action");
        q.bindValue(":zone", zone);
        q.bindValue(":date_event", d);  // <--- bind QDate directement
        q.bindValue(":action", action);

        if (!q.exec()) {
            QMessageBox::warning(this, "Erreur SQL", q.lastError().text());
            continue;
        }

        events[d].removeOne(text);
        delete item;
    }

    updateCalendarHighlights();
}


void MainWindow::toggleCalendar()
{
    // Inverse l'état de visibilité du calendrier
    isCalendarVisible = !isCalendarVisible;

    // Appliquer la visibilité aux widgets liés
    calendar->setVisible(isCalendarVisible);
    eventList->setVisible(isCalendarVisible);
    addEventBtn->setVisible(isCalendarVisible);
    removeEventBtn->setVisible(isCalendarVisible);

    // Mettre à jour le texte du bouton
    toggleCalendarBtn->setText(isCalendarVisible ? "Masquer Calendrier" : "Afficher Calendrier");

    // Optionnel : mettre à jour les couleurs des jours avec événements
    updateCalendarHighlights();
}
void MainWindow::onCalendarDateSelected()
{
    // Vider la liste d'événements pour la date sélectionnée
    eventList->clear();

    // Récupérer la date sélectionnée dans le calendrier
    QDate selectedDate = calendar->selectedDate();

    // Vérifier si des événements existent pour cette date
    if (events.contains(selectedDate)) {
        // Ajouter tous les événements à la liste
        eventList->addItems(events[selectedDate]);
    }
}
// Set the calendar background to light green



void MainWindow::checkEventNotifications()
{
    QDate today = QDate::currentDate();
    if (!events.contains(today)) return;

    for (const QString &evt : events[today]) {
        QMessageBox::information(this, "Rappel", "Aujourd'hui : " + evt);
    }
}

// --------------------------------------------------------------
// SEARCH + SORT
// --------------------------------------------------------------
void MainWindow::setupSearchAndSort()
{
    if (!proxyModel) {
        proxyModel = new QSortFilterProxyModel(this);
    }

    proxyModel->setSourceModel(tableView->model());
    proxyModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
    proxyModel->setFilterKeyColumn(-1);
    proxyModel->setDynamicSortFilter(true);

    tableView->setModel(proxyModel);
    tableView->setSortingEnabled(true);
}

void MainWindow::onSearchTextChanged(const QString &text)
{
    if (proxyModel) {
        proxyModel->setFilterFixedString(text);
    }
}

void MainWindow::ouvrirTri()
{
    SortDialog dialog(this);

    if (dialog.exec() == QDialog::Accepted) {
        if (!proxyModel) {
            setupSearchAndSort();
        }

        int col = dialog.primaryColumn();
        Qt::SortOrder order = dialog.primaryOrder();

        proxyModel->sort(col, order);

        int sec = dialog.secondaryColumn();
        if (sec != -1) {
            proxyModel->sort(sec, dialog.secondaryOrder());
        }
    }
}

// --------------------------------------------------------------
// PDF EXPORT
// --------------------------------------------------------------
void MainWindow::exportToPdf()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Exporter en PDF", "", "PDF (*.pdf)");

    if (fileName.isEmpty()) return;
    if (!fileName.endsWith(".pdf")) fileName += ".pdf";

    QPdfWriter writer(fileName);
    writer.setPageSize(QPageSize(QPageSize::A4));
    writer.setResolution(300);

    QPainter painter(&writer);

    painter.setFont(QFont("Arial", 16, QFont::Bold));
    painter.drawText(QRect(0, 0, writer.width(), 50), Qt::AlignCenter, "Rapport - Zones Vertes");

    int y = 70;

    // Capture table view
    QPixmap tablePixmap(tableView->viewport()->size());
    tableView->viewport()->render(&tablePixmap);

    QPixmap scaled = tablePixmap.scaledToWidth(writer.width() - 40, Qt::SmoothTransformation);
    painter.drawPixmap(20, y, scaled);

    y += scaled.height() + 20;

    // Add summary
    QSqlQuery q("SELECT COUNT(*), NVL(SUM(SUPERFICIE),0) FROM ZONES_VERTES");

    if (q.next()) {
        QString summary =
            "Total zones: " + q.value(0).toString() +
            "   |   Superficie totale: " + q.value(1).toString() + " ha";

        painter.setFont(QFont("Arial", 10));
        painter.drawText(20, y, summary);
    }

    // Add timestamp
    painter.setFont(QFont("Arial", 8));
    painter.drawText(20, writer.height() - 20,
                     "Généré le: " + QDateTime::currentDateTime().toString("dd/MM/yyyy HH:mm"));

    painter.end();

    QMessageBox::information(this, "PDF Export", "PDF exporté avec succès !");
}

// --------------------------------------------------------------
// ENVIRONMENTAL CALCULATIONS
// --------------------------------------------------------------
double MainWindow::computeOxygenProduction(double superficieHa)
{
    return superficieHa * 1000.0; // kg/year
}

double MainWindow::computeCO2Reduction(double superficieHa)
{
    return superficieHa * 6000.0; // kg/year
}

// --------------------------------------------------------------
// ECO ANALYSIS
// --------------------------------------------------------------
void MainWindow::showEcoConclusionDialog()
{
    // 1. Read total superficies
    QSqlQuery q1("SELECT NVL(SUM(SUPERFICIE),0) FROM ZONES_VERTES");
    q1.next();
    double superficieTotale = q1.value(0).toDouble();

    // 2. Read number of zones
    QSqlQuery q2("SELECT COUNT(*) FROM ZONES_VERTES");
    q2.next();
    int totalZones = q2.value(0).toInt();

    // 3. Compute values
    double oxygen = computeOxygenProduction(superficieTotale);
    double co2 = computeCO2Reduction(superficieTotale);

    QString msg =
        "Analyse Écologique:\n\n"
        "Superficie totale : " + QString::number(superficieTotale) + " ha\n"
                                              "Nombre de zones : " + QString::number(totalZones) + "\n\n"
                                        "Production estimée d'Oxygène : " + QString::number(oxygen) + " kg/an\n"
                                    "Absorption estimée de CO₂ : " + QString::number(co2) + " kg/an\n\n"
                                 "Conclusion :\n"
                                 "Une grande superficie végétale augmente la production d'oxygène et réduit "
                                 "significativement le CO₂, améliorant la qualité environnementale de la ville.";

    QMessageBox::information(this, "Analyse Environnementale", msg);
}

void MainWindow::updateCalendarHighlights()
{
    //QPalette pal = calendarWidget->palette();
    QTextCharFormat highlight;
    highlight.setBackground(QColor("#81c784"));  // Light green
    highlight.setForeground(Qt::white);          // Text color on highlighted day

    QTextCharFormat clear; // default

    // Clear previous formats
    for (const QDate &d : events.keys())
        calendar->setDateTextFormat(d, clear);

    // Apply highlights
    for (auto it = events.begin(); it != events.end(); ++it) {
        if (!it.value().isEmpty())
            calendar->setDateTextFormat(it.key(), highlight);
    }
}

// --------------------------------------------------------------
// RAIN SENSOR METHODS
// --------------------------------------------------------------

void MainWindow::connectRainSensor()
{
    // You might want to make the port configurable
    QString portname = "COM6";  // Change to your Arduino port (COM3 on Windows, /dev/ttyUSB0 on Linux)
    int baudRate = 9600;

    if (rainSensor->connectToArduino(portname, baudRate)) {
        rainStatusLabel->setText("✅ Capteur connecté: En attente...");
        rainStatusLabel->setStyleSheet("color: green; font-weight: bold; padding: 5px;");
        connectRainSensorBtn->setEnabled(false);
        disconnectRainSensorBtn->setEnabled(true);
        QMessageBox::information(this, "Succès", "Capteur de pluie connecté avec succès!");
    } else {
        rainStatusLabel->setText("❌ Échec de connexion");
        rainStatusLabel->setStyleSheet("color: red; font-weight: bold; padding: 5px;");
        QMessageBox::warning(this, "Erreur",
                             "Impossible de connecter le capteur de pluie.\n"
                             "Vérifiez que:\n"
                             "1. L'Arduino est branché\n"
                             "2. Le port " + portname + " est correct\n"
                                              "3. Aucune autre application n'utilise le port");
    }
}

void MainWindow::disconnectRainSensor()
{
    rainSensor->disconnectFromArduino();
    rainStatusLabel->setText("🌧️ Statut Capteur: Non connecté");
    rainStatusLabel->setStyleSheet("font-weight: bold; padding: 5px;");
    connectRainSensorBtn->setEnabled(true);
    disconnectRainSensorBtn->setEnabled(false);
    QMessageBox::information(this, "Info", "Capteur de pluie déconnecté.");
}

void MainWindow::onRainStateChanged(bool rainDetected)
{
    if (rainDetected) {
        rainStatusLabel->setText("🌧️ Pluie détectée!");
        rainStatusLabel->setStyleSheet("color: blue; font-weight: bold; padding: 5px; background-color: #e3f2fd;");

        // Show simple QMessageBox notification
        QMessageBox::information(this, "Détection de Pluie",
                                 "⚠️ La pluie a été détectée par le capteur!", "⚠️ Stop Arrozage!");

        // Update pluie field in form (optional)
        pluieEdit->setText("1");

        // Update all zones in the database
        QSqlQuery query;
        query.prepare("UPDATE ZONES_VERTES SET PLUIE = 1, ETAT = 'Pluvieux(stop Arrozage)'");
        if (!query.exec()) {
            qDebug() << "Erreur mise à jour pluie pour toutes les zones:" << query.lastError().text();
        }

        // Optional: refresh the table to show changes immediately
        //loadTable(); // Make sure you have this function to reload your table

    } else {
        rainStatusLabel->setText("☀️ Pas de pluie détectée");
        rainStatusLabel->setStyleSheet("color: orange; font-weight: bold; padding: 5px;");

        // Update pluie field in form
        pluieEdit->setText("0");  // Or "Non détectée"

        // Reset all zones if needed
        QSqlQuery query;
        query.prepare("UPDATE ZONES_VERTES SET PLUIE = 0, ETAT = 'Sec'");
        if (!query.exec()) {
            qDebug() << "Erreur réinitialisation pluie pour toutes les zones:" << query.lastError().text();
        }

        // Optional: refresh the table to show changes immediately
        //loadTable();
    }
}

// --------------------------------------------------------------
// DEMANDE INTERVENTION METHODS
// --------------------------------------------------------------

