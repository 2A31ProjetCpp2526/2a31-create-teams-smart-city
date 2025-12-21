// smartmetal.cpp
#include "smartmetal.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QInputDialog>
#include <QHeaderView>
#include <QTableWidgetItem>
#include <QBrush>
#include <QColor>
#include <QtMath>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlDatabase>
#include "connection.h"

// --------------------------------------------------
// SmartMetalDialog Constructor
// --------------------------------------------------
SmartMetalDialog::SmartMetalDialog(QWidget *parent) : QDialog(parent)
{
    setWindowTitle("SmartMetal Classifier");
    resize(700, 450);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    // TABLE WIDGET SETUP ---------------------------------
    table = new QTableWidget(this);
    table->setColumnCount(6);
    table->setHorizontalHeaderLabels({"Weight", "Conductivity", "Magnetic (0/1)",
                                      "IR Purity", "Radiation", "Classify"});
    table->horizontalHeader()->setStretchLastSection(true);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);

    table->setAlternatingRowColors(true);
    table->setStyleSheet(R"(
        QTableWidget {
            background-color: #f5f5f5;
            gridline-color: #d0d0d0;
            font: 12px "Segoe UI";
        }
        QTableWidget::item {
            selection-background-color: #87cefa;
            selection-color: black;
        }
        QHeaderView::section {
            background-color: #4CAF50;
            color: white;
            font-weight: bold;
            font-size: 13px;
            padding: 4px;
        }
    )");

    table->verticalHeader()->setDefaultSectionSize(30);
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    mainLayout->addWidget(table);

    // BUTTONS REGION -------------------------------------
    QHBoxLayout *btnLayout = new QHBoxLayout();

    addRowBtn = new QPushButton("Add Metal Row", this);
    loadBtn   = new QPushButton("Load Database", this);

    btnLayout->addWidget(addRowBtn);
    btnLayout->addWidget(loadBtn);

    mainLayout->addLayout(btnLayout);

    // ADD ROW BUTTON ACTION -------------------------------------
    connect(addRowBtn, &QPushButton::clicked, [=]() {
        bool ok;
        double weight = QInputDialog::getDouble(this, "Weight", "Enter weight:", 100, 0, 1e6, 2, &ok);
        if (!ok) return;
        double conduct = QInputDialog::getDouble(this, "Conductivity", "Enter conductivity:", 10, 0, 1e6, 2, &ok);
        if (!ok) return;
        int magnetic = QInputDialog::getInt(this, "Magnetic", "Enter magnetic (0/1):", 0, 0, 1, 1, &ok);
        if (!ok) return;
        double irPurity = QInputDialog::getDouble(this, "IR Purity", "Enter IR Purity:", 50, 0, 100, 2, &ok);
        if (!ok) return;
        double radiation = QInputDialog::getDouble(this, "Radiation", "Enter Radiation:", 0, 0, 1e6, 2, &ok);
        if (!ok) return;

        addMetalRow(weight, conduct, magnetic, irPurity, radiation);
        insertIntoDatabase(weight, conduct, magnetic, irPurity, radiation);
    });

    // LOAD DATABASE BUTTON -------------------------------
    connect(loadBtn, &QPushButton::clicked, this, &SmartMetalDialog::loadFromDatabase);

    // INITIAL LOAD ---------------------------------------
    loadFromDatabase();
}

// --------------------------------------------------
// Add Row to Table
// --------------------------------------------------
void SmartMetalDialog::addMetalRow(double weight, double conduct, int magnetic,
                                   double irPurity, double radiation)
{
    int row = table->rowCount();
    table->insertRow(row);

    table->setItem(row, 0, new QTableWidgetItem(QString::number(weight)));
    table->setItem(row, 1, new QTableWidgetItem(QString::number(conduct)));
    table->setItem(row, 2, new QTableWidgetItem(QString::number(magnetic)));
    table->setItem(row, 3, new QTableWidgetItem(QString::number(irPurity)));
    table->setItem(row, 4, new QTableWidgetItem(QString::number(radiation)));

    QPushButton *btn = new QPushButton("Classify", this);
    table->setCellWidget(row, 5, btn);

    // Capture the values for classification at time of button creation
    connect(btn, &QPushButton::clicked, this, [=]() {
        QString result = SmartMetal::classifyMetal(weight, conduct, magnetic, irPurity, radiation);

        QMessageBox::information(this, "Classification Result",
                                 "Metal classified as: " + result);

        applyRowColor(row, result);
    });
}

// --------------------------------------------------
// Apply Row Color
// --------------------------------------------------
void SmartMetalDialog::applyRowColor(int row, const QString &result)
{
    QColor baseColor;

    if (result == "Ferrous") baseColor = QColor("#ffcccc");
    else if (result == "Non-Ferrous") baseColor = QColor("#fff8b2");
    else if (result == "Recyclable") baseColor = QColor("#ccffcc");
    else if (result == "E-Waste") baseColor = QColor("#e6ccff");
    else if (result == "Hazardous") baseColor = QColor("#ff9999");
    else baseColor = QColor("#e0e0e0");

    for (int col = 0; col < table->columnCount(); col++) {
        QTableWidgetItem *item = table->item(row, col);
        if (!item) {
            item = new QTableWidgetItem();
            table->setItem(row, col, item);
        }

        if (col == 2) item->setBackground(baseColor.darker(120));
        else if (col == 4) item->setBackground(baseColor.lighter(130));
        else item->setBackground(baseColor);

        item->setForeground(QBrush(Qt::black));
    }
}

// --------------------------------------------------
// Load from Oracle DB (RECYCLAGE)
// --------------------------------------------------
void SmartMetalDialog::loadFromDatabase()
{
    // Clear existing rows to avoid duplicates
    table->setRowCount(0);

    // Optionally ensure DB connection exists:
    if (!QSqlDatabase::database().isValid() || !QSqlDatabase::database().isOpen()) {
// If you have a Connection helper, try to create/open the connection
// (connection.h should provide Connection::createConnection() if available)
#ifdef CONNECTION_CREATE_FN
        Connection::createConnection();
#endif
    }

    QSqlQuery q;
    if (!q.exec("SELECT WEIGHT, CONDUCTIVITY, MAGNETIC, IR_PURITY, RADIATION FROM RECYCLAGE")) {
        QMessageBox::warning(this, "DB Error",
                             "Failed to load RECYCLAGE table:\n" + q.lastError().text());
        return;
    }

    while (q.next()) {
        // Note: your RECYCLAGE columns are VARCHAR2 in Oracle — we convert to double/int safely
        double w  = q.value(0).toString().toDouble();
        double c  = q.value(1).toString().toDouble();
        int m     = q.value(2).toString().toInt();
        double ir = q.value(3).toString().toDouble();
        double r  = q.value(4).toString().toDouble();

        addMetalRow(w, c, m, ir, r);
    }
}

// --------------------------------------------------
// INSERT INTO ORACLE TABLE RECYCLAGE
// --------------------------------------------------
void SmartMetalDialog::insertIntoDatabase(double weight, double conduct, int magnetic,
                                          double irPurity, double radiation)
{
    QSqlQuery q;

    q.prepare("INSERT INTO RECYCLAGE "
              "(WEIGHT, CONDUCTIVITY, MAGNETIC, IR_PURITY, RADIATION) "
              "VALUES (:w, :c, :m, :i, :r)");

    // Your RECYCLAGE columns are VARCHAR2(20). Bind as strings to be safe.
    q.bindValue(":w", QString::number(weight));
    q.bindValue(":c", QString::number(conduct));  // PRIMARY KEY in your CREATE TABLE
    q.bindValue(":m", QString::number(magnetic));
    q.bindValue(":i", QString::number(irPurity));
    q.bindValue(":r", QString::number(radiation));

    if (!q.exec()) {
        QMessageBox::warning(this, "Insert Error",
                             "Insert into RECYCLAGE failed:\n" + q.lastError().text());
    }
}

// --------------------------------------------------
// SmartMetal KNN Classifier
// --------------------------------------------------
QString SmartMetal::classifyMetal(double weight, double conduct, int magnetic,
                                  double irPurity, double radiation)
{
    // Training samples (assumes Sample { double weight, conductivity; int magnetic; double infraredPurity; double radiationLevel; QString label; })
    QVector<Sample> training = {
        {300, 10, 1, 50, 0, "Ferrous"},
        {450, 12, 1, 55, 0, "Ferrous"},
        {80,  40, 0, 85, 0, "Non-Ferrous"},
        {120, 38, 0, 90, 0, "Non-Ferrous"},
        {100, 55, 0, 95, 0, "Recyclable"},
        {150, 60, 0, 97, 0, "Recyclable"},
        {200, 15, 0, 30, 50, "E-Waste"},
        {250, 20, 0, 35, 70, "E-Waste"},
        {50,  5,  0, 20, 90, "Hazardous"},
        {60,  7,  0, 25, 85, "Hazardous"}
    };

    struct Dist { double d; QString label; };
    QVector<Dist> distList;
    distList.reserve(training.size());

    for (const auto &s : training) {
        double d = qSqrt(
            qPow(s.weight - weight, 2) +
            qPow(s.conductivity - conduct, 2) +
            qPow(s.magnetic - magnetic, 2) +
            qPow(s.infraredPurity - irPurity, 2) +
            qPow(s.radiationLevel - radiation, 2)
            );
        distList.append({d, s.label});
    }

    std::sort(distList.begin(), distList.end(),
              [](const Dist &a, const Dist &b) { return a.d < b.d; });

    const int k = 3;
    QMap<QString, int> count;

    for (int i = 0; i < k && i < distList.size(); ++i) {
        count[distList[i].label]++;
    }

    QString best = "Hazardous";
    int maxVal = -1;
    for (auto it = count.begin(); it != count.end(); ++it) {
        if (it.value() > maxVal) {
            maxVal = it.value();
            best = it.key();
        }
    }

    return best;
}

