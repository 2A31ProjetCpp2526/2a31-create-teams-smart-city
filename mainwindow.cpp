#include "mainwindow.h"
#include "connection.h"
#include "calendrierzones.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QInputDialog>
#include <QHeaderView>
#include <QTableWidgetItem>
#include <QCalendarWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QStyleFactory>
#include <QPalette>
#include <QPdfWriter>
#include <QPainter>
#include <QFileDialog>
#include <QTimer>
#include <QDebug>

// ------------------- CONSTRUCTOR -------------------
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    centralWidget(nullptr),
    mainLayout(nullptr),
    leftRightLayout(nullptr),
    calendarWidget(nullptr),
    eventEdit(nullptr),
    addButton(nullptr),
    deleteButton(nullptr),
    statsLabel(nullptr),
    tableWidget(nullptr),
    calLayout(nullptr),
    zonesCalendarWidget(nullptr),
    toggleCalendarBtn(nullptr)
{
    if (!setupDatabase()) {
        QMessageBox::critical(this, "Erreur", "Impossible de connecter à la base de données. L'application va se fermer.");
        //QTimer::singleShot(0, this, &QApplication::quit);
        return;
    }

    setupUI();
    setupStyle();
    setupConnections();

    loadEvents();
    updateStats();
}

// ------------------- DESTRUCTOR -------------------
MainWindow::~MainWindow() {
    Connection::getInstance()->closeConnection();
}

// ------------------- DATABASE -------------------
bool MainWindow::setupDatabase() {
    if (!Connection::getInstance()->openConnection())
        return false;

    QSqlQuery q;

    // Create table if it does not exist
    QString ddlTable1 =
        "BEGIN "
        "EXECUTE IMMEDIATE 'CREATE TABLE TABLE1 ("
        "ZONE VARCHAR2(20 BYTE) NOT NULL, "
        "DATE_EVENT DATE NOT NULL, "
        "ACTION VARCHAR2(20 BYTE) NOT NULL, "
        "CONSTRAINT TABLE1_PK PRIMARY KEY (ZONE))'; "
        "EXCEPTION WHEN OTHERS THEN "
        "IF SQLCODE = -955 THEN NULL; ELSE RAISE; END IF; "
        "END;";

    if (!q.exec(ddlTable1)) {
        //qDebug() << "Erreur lors de la création de TABLE1:" << q.lastError().text();
    }

    return true;
}

// ------------------- UI -------------------
void MainWindow::setupUI() {
    centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    setWindowTitle("🌱 Eco Calendar — Green Zone Planner");

    // LEFT PANEL
    calendarWidget = new QCalendarWidget(this);
    calendarWidget->setGridVisible(true);

    eventEdit = new QLineEdit(this);
    eventEdit->setPlaceholderText("Enter your event...");

    addButton = new QPushButton("➕ Add Event", this);
    deleteButton = new QPushButton("🗑️ Delete Event", this);

    statsLabel = new QLabel("🌿 Events: 0 | Zones: 0", this);
    statsLabel->setAlignment(Qt::AlignCenter);

    tableWidget = new QTableWidget(this);
    tableWidget->setColumnCount(2);
    tableWidget->setHorizontalHeaderLabels(QStringList() << "Date" << "Event");
    tableWidget->horizontalHeader()->setStretchLastSection(true);
    tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

    QVBoxLayout *leftLayout = new QVBoxLayout();
    leftLayout->addWidget(calendarWidget);
    leftLayout->addWidget(eventEdit);

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(addButton);
    buttonLayout->addWidget(deleteButton);
    leftLayout->addLayout(buttonLayout);

    leftLayout->addWidget(statsLabel);
    leftLayout->addWidget(tableWidget);

    // RIGHT PANEL
    zonesCalendarWidget = new CalendrierZones(this);
    zonesCalendarWidget->setVisible(false);

    toggleCalendarBtn = new QPushButton("Afficher Calendrier", this);

    calLayout = new QVBoxLayout();
    calLayout->addWidget(toggleCalendarBtn);
    calLayout->addWidget(zonesCalendarWidget);
    calLayout->addStretch();

    leftRightLayout = new QHBoxLayout();
    leftRightLayout->addLayout(leftLayout, 3);
    leftRightLayout->addLayout(calLayout, 4);

    mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->addLayout(leftRightLayout);
}

// ------------------- STYLE -------------------
void MainWindow::setupStyle() {
    setStyle(QStyleFactory::create("Fusion"));
    QPalette pal = palette();
    pal.setColor(QPalette::Window, QColor("#e8f5e9"));
    pal.setColor(QPalette::Base, QColor("#ffffff"));
    pal.setColor(QPalette::Highlight, QColor("#81c784"));
    pal.setColor(QPalette::Button, QColor("#a5d6a7"));
    pal.setColor(QPalette::ButtonText, QColor("#1b5e20"));
    setPalette(pal);
}

// ------------------- CONNECTIONS -------------------
void MainWindow::setupConnections() {
    connect(addButton, &QPushButton::clicked, this, &MainWindow::addEvent);
    connect(deleteButton, &QPushButton::clicked, this, &MainWindow::deleteEvent);
    connect(calendarWidget, &QCalendarWidget::selectionChanged, this, &MainWindow::updateStats);
    connect(toggleCalendarBtn, &QPushButton::clicked, this, &MainWindow::toggleCalendar);

    connect(zonesCalendarWidget, &CalendrierZones::zoneClicked, this, [=](const QDate &date){
        bool ok;
        QString zoneId = QInputDialog::getText(this, "Ajouter Zone", "Entrez l'ID de la zone :", QLineEdit::Normal, "", &ok);
        if (!ok || zoneId.isEmpty()) return;

        QString eventText = QInputDialog::getText(this, "Ajouter Événement", "Entrez l'événement :", QLineEdit::Normal, "", &ok);
        if (!ok || eventText.isEmpty()) return;

        zonesCalendarWidget->addEventToZone(date, zoneId, eventText);

        // Optional DB storage
        QSqlQuery query;
        query.prepare("INSERT INTO SAIM.TABLE1 (ZONE, DATE_EVENT, ACTION) VALUES (:zone, TO_DATE(:date,'YYYY-MM-DD'), :action)");
        query.bindValue(":zone", zoneId);
        query.bindValue(":date", date.toString("yyyy-MM-dd"));
        query.bindValue(":action", eventText);
        if (!query.exec()) //qDebug() << "Erreur DB:" << query.lastError();

        loadEvents();
        updateStats();
    });
}

// ------------------- CRUD -------------------
void MainWindow::loadEvents() {
    tableWidget->setRowCount(0);

    QSqlQuery query("SELECT DATE_EVENT, ACTION FROM SAIM.TABLE1 ORDER BY DATE_EVENT ASC");
    int row = 0;
    while (query.next()) {
        QDate d = query.value(0).toDate();
        QString text = query.value(1).toString();

        tableWidget->insertRow(row);
        tableWidget->setItem(row, 0, new QTableWidgetItem(d.toString("yyyy-MM-dd")));
        tableWidget->setItem(row, 1, new QTableWidgetItem(text));
        row++;
    }

    zonesCalendarWidget->refresh();
}

void MainWindow::addEvent() {
    QString eventText = eventEdit->text().trimmed();
    if (eventText.isEmpty()) return;

    QString zoneId = QInputDialog::getText(this, "Ajouter Événement", "Entrez l'ID de Zone :").trimmed();
    if (zoneId.isEmpty()) return;

    QDate selectedDate = calendarWidget->selectedDate();

    // Add to DB
    QSqlQuery query;
    query.prepare("INSERT INTO SAIM.TABLE1 (ZONE, DATE_EVENT, ACTION) VALUES (:zone, TO_DATE(:date,'YYYY-MM-DD'), :action)");
    query.bindValue(":zone", zoneId);
    query.bindValue(":date", selectedDate.toString("yyyy-MM-dd"));
    query.bindValue(":action", eventText);
    if (!query.exec()) ///qDebug() << "Erreur DB:" << query.lastError();

    // Add to CalendrierZones
    zonesCalendarWidget->addEventToZone(selectedDate, zoneId, eventText);

    eventEdit->clear();
    loadEvents();
    updateStats();
}

void MainWindow::deleteEvent() {
    int row = tableWidget->currentRow();
    if (row < 0) return;

    QDate date = QDate::fromString(tableWidget->item(row, 0)->text(), "yyyy-MM-dd");
    QString action = tableWidget->item(row, 1)->text();

    // Delete from DB
    QSqlQuery query;
    query.prepare("DELETE FROM SAIM.TABLE1 WHERE DATE_EVENT = TO_DATE(:date,'YYYY-MM-DD') AND ACTION = :action");
    query.bindValue(":date", date.toString("yyyy-MM-dd"));
    query.bindValue(":action", action);
    if (!query.exec())
        //qDebug() << "Erreur DB:" << query.lastError();

    // Delete from CalendrierZones
    zonesCalendarWidget->removeEventFromZone(date, action);

    tableWidget->removeRow(row);
    updateStats();
}

// ------------------- STATS -------------------
void MainWindow::updateStats() {
    QDate selected = calendarWidget->selectedDate();

    int eventCount = 0;
    QSet<QString> uniqueZones;

    const auto &zonesMap = zonesCalendarWidget->getZones();

    for (int i = 0; i < tableWidget->rowCount(); ++i) {
        QDate d = QDate::fromString(tableWidget->item(i, 0)->text(), "yyyy-MM-dd");
        if (d.month() == selected.month() && d.year() == selected.year()) {
            eventCount++;
            QString zoneId;
            if (zonesMap.contains(d)) {
                for (const ZoneData &z : zonesMap[d]) {
                    if (z.events.contains(tableWidget->item(i,1)->text())) {
                        zoneId = z.zoneId;
                        break;
                    }
                }
            }
            if (!zoneId.isEmpty()) uniqueZones.insert(zoneId);
        }
    }

    statsLabel->setText(QString("🌿 Events: %1 | Zones: %2").arg(eventCount).arg(uniqueZones.size()));
}

// ------------------- CALENDAR -------------------
void MainWindow::toggleCalendar() {
    bool visible = zonesCalendarWidget->isVisible();
    zonesCalendarWidget->setVisible(!visible);
    toggleCalendarBtn->setText(visible ? "Afficher Calendrier" : "Masquer Calendrier");

    if (!visible) {
        zonesCalendarWidget->refresh();
        updateStats();
    }
}

// ------------------- PDF EXPORT (stub) -------------------
void MainWindow::exportToPdf() {
    QString fileName = QFileDialog::getSaveFileName(this, "Exporter en PDF", QString(), "PDF files (*.pdf)");
    if (fileName.isEmpty()) return;
    if (!fileName.endsWith(".pdf", Qt::CaseInsensitive)) fileName += ".pdf";

    QPdfWriter writer(fileName);
    writer.setPageSize(QPageSize(QPageSize::A4));
    writer.setResolution(300);

    QPainter painter(&writer);
    painter.setFont(QFont("Arial", 16, QFont::Bold));
    painter.drawText(QRect(0,0,writer.width(),50), Qt::AlignCenter, "Rapport - Zones Vertes");

    if (tableWidget) {
        QPixmap tablePixmap(tableWidget->viewport()->size());
        tableWidget->viewport()->render(&tablePixmap);
        QPixmap scaledTable = tablePixmap.scaledToWidth(writer.width()-40, Qt::SmoothTransformation);
        painter.drawPixmap(20, 70, scaledTable);
    }

    painter.end();
    QMessageBox::information(this, "Export", "PDF exporté : " + fileName);
}
