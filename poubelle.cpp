#include "poubelle.h"
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QRegularExpression>
#include <QTimer>
#include "mainwindow.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSqlError>
#include <QDebug>

Poubelle::Poubelle() : id_bin(0), id_zone(0), capacite(0.0), used_percentage(0.0), status("VIDE") {}

Poubelle::Poubelle(int id_bin, int id_zone, double capacite, QString status, double used_percentage)
{
    this->id_bin = id_bin;
    this->id_zone = id_zone;
    this->capacite = capacite;
    this->status = status.trimmed().toUpper();
    this->used_percentage = used_percentage;
}

double Poubelle::getUsedPercentage() const { return used_percentage; }
void Poubelle::setUsedPercentage(double pct) { used_percentage = pct; }

bool Poubelle::ajouter()
{
    QSqlDatabase db = QSqlDatabase::database();
    if (!db.isOpen()) {
        qDebug() << "❌ Database not open!";
        return false;
    }

    QSqlQuery query(db);
    query.prepare("INSERT INTO GESPUB (ID_BIN, CAPACITE, STATUS, ID_ZONE, USED_PERCENTAGE) "
                  "VALUES (:id_bin, :capacite, :status, :id_zone, :used_percentage)");
    query.bindValue(":id_bin", id_bin);
    query.bindValue(":id_zone", id_zone);
    query.bindValue(":capacite", capacite);
    query.bindValue(":used_percentage", used_percentage);
    query.bindValue(":status", status.trimmed().toUpper());

    if (query.exec()) {
        qDebug() << "✅ Poubelle ajoutée avec succès";
        return true;
    } else {
        QString err = query.lastError().text();
        qDebug() << "❌ Erreur d'ajout poubelle:" << err;
        if (err.contains("ORA-02290")) {
            qDebug() << "❌ Constraint violation: likely invalid STATUS value";
        }
        return false;
    }
}

bool Poubelle::supprimer(int id_bin)
{
    QSqlQuery query;
    query.prepare("DELETE FROM GESPUB WHERE ID_BIN = :id_bin");
    query.bindValue(":id_bin", id_bin);

    if (query.exec()) {
        qDebug() << "✅ Poubelle supprimée avec succès";
        return true;
    } else {
        qDebug() << "❌ Erreur suppression poubelle:" << query.lastError().text();
        return false;
    }
}

bool Poubelle::modifier(int id_bin)
{
    QSqlQuery query;
    query.prepare(
        "UPDATE GESPUB "
        "SET CAPACITE = :capacite, STATUS = :status, ID_ZONE = :id_zone "
        "WHERE ID_BIN = :id_bin"
        );
    query.bindValue(":id_zone", id_zone);
    query.bindValue(":capacite", capacite);
    query.bindValue(":status", status.trimmed().toUpper());
    query.bindValue(":id_bin", id_bin);

    if (query.exec()) {
        qDebug() << "✅ Poubelle modifiée avec succès";
        return true;
    } else {
        QString err = query.lastError().text();
        qDebug() << "❌ Erreur modification poubelle:" << err;
        if (err.contains("ORA-02290")) {
            qDebug() << "❌ Constraint violation: likely invalid STATUS value";
        }
        return false;
    }
}

QSqlQueryModel* Poubelle::afficher()
{
    QSqlQueryModel *model = new QSqlQueryModel();
    model->setQuery("SELECT * FROM GESPUB");

    if (model->lastError().isValid())
        qDebug() << "❌ Erreur affichage:" << model->lastError().text();

    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID_BIN"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("CAPACITE"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("STATUS"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("ID_ZONE"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("USED_PERCENTAGE"));

    return model;
}

QSqlQueryModel* Poubelle::rechercher(int id)
{
    QSqlQueryModel *model = new QSqlQueryModel();
    model->setQuery("SELECT * FROM GESPUB WHERE ID_BIN = " + QString::number(id));

    if (model->lastError().isValid())
        qDebug() << "❌ Erreur recherche:" << model->lastError().text();

    return model;
}

QSqlQueryModel* Poubelle::trier(int index)
{
    QSqlQueryModel *model = new QSqlQueryModel();
    QString queryStr;

    switch(index) {
    case 0: queryStr = "SELECT * FROM GESPUB ORDER BY ID_BIN"; break;
    case 1: queryStr = "SELECT * FROM GESPUB ORDER BY CAPACITE"; break;
    case 2: queryStr = "SELECT * FROM GESPUB ORDER BY STATUS"; break;
    default: queryStr = "SELECT * FROM GESPUB"; break;
    }

    model->setQuery(queryStr);

    if (model->lastError().isValid())
        qDebug() << "❌ Erreur tri:" << model->lastError().text();

    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID_BIN"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("CAPACITE"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("STATUS"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("ID_ZONE"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("USED_PERCENTAGE"));

    return model;
}

bool Poubelle::modifierUsedPercentage(int id_bin, double usedPct)
{
    QSqlQuery query;
    QString newStatus = (usedPct >= 100.0) ? QString("PLEIN") : QString("VIDE");

    query.prepare("UPDATE GESPUB SET USED_PERCENTAGE = :used_percentage, STATUS = :status WHERE ID_BIN = :id_bin");
    query.bindValue(":used_percentage", usedPct);
    query.bindValue(":status", newStatus);
    query.bindValue(":id_bin", id_bin);

    if (query.exec()) {
        qDebug() << "✅ Poubelle updated used percentage:" << id_bin << usedPct;
        return true;
    } else {
        qDebug() << "❌ Erreur update used percentage:" << query.lastError().text();
        return false;
    }
}

// Static members initialization
QSerialPort* Poubelle::pirSerial = nullptr;
MainWindow* Poubelle::mainWindowPtr = nullptr;

void Poubelle::setupPIRSerial(MainWindow* mw)
{
    mainWindowPtr = mw;
    // Setup port similarly to previous logic
#ifdef QT_SERIALPORT_LIB
    QSerialPortInfo selectedPort;
    for (const QSerialPortInfo &info : QSerialPortInfo::availablePorts()) {
        QString desc = info.description().toLower();
        QString man = info.manufacturer().toLower();
        if (desc.contains("arduino") || man.contains("arduino") || desc.contains("ch340") || man.contains("ch340") || desc.contains("usb serial")) {
            selectedPort = info;
            break;
        }
    }
    if (selectedPort.portName().isEmpty()) {
        auto ports = QSerialPortInfo::availablePorts();
        if (!ports.isEmpty()) selectedPort = ports.first();
    }
    if (selectedPort.portName().isEmpty()) {
        qDebug() << "No serial ports found for PIR";
        return;
    }

    pirSerial = new QSerialPort(selectedPort, mainWindowPtr);
    pirSerial->setBaudRate(QSerialPort::Baud9600);
    pirSerial->setDataBits(QSerialPort::Data8);
    pirSerial->setParity(QSerialPort::NoParity);
    pirSerial->setStopBits(QSerialPort::OneStop);
    pirSerial->setFlowControl(QSerialPort::NoFlowControl);
    if (!pirSerial->open(QIODevice::ReadOnly)) {
        qDebug() << "Failed to open serial port" << selectedPort.portName() << pirSerial->errorString();
        delete pirSerial; pirSerial = nullptr;
        return;
    }
    QObject::connect(pirSerial, &QSerialPort::readyRead, [](){ Poubelle::onPirSerialReady(); });
    qDebug() << "PIR serial opened:" << selectedPort.portName();
#endif
}

void Poubelle::onPirSerialReady()
{
#ifdef QT_SERIALPORT_LIB
    if (!pirSerial) return;
    while (pirSerial->canReadLine()) {
        QByteArray line = pirSerial->readLine().trimmed();
        QString s = QString::fromUtf8(line).trimmed();
        qDebug() << "PIR serial msg:" << s;
        if (mainWindowPtr) {
            QString formatted;
            if (s.toUpper().contains("MOTION") || s.toUpper().contains("DETECT")) {
                formatted = QString("🔴 MOTION: %1").arg(s);
            } else if (s.toUpper().contains("BIN")) {
                formatted = QString("📦 BIN: %1").arg(s);
            } else {
                formatted = QString("📡 %1").arg(s);
            }
            QMetaObject::invokeMethod(mainWindowPtr, "appendIrlDebug", Qt::QueuedConnection, Q_ARG(QString, formatted));
        }
        int binId = -1;
        QRegularExpression re("BIN:(\\d+)", QRegularExpression::CaseInsensitiveOption);
        auto m = re.match(s);
        if (m.hasMatch()) binId = m.captured(1).toInt();
        if (s.toUpper().contains("MOTION") || s.toUpper().contains("DETECT")) {
            if (binId >= 0) handlePIRMotion(binId);
        }
    }
#endif
}

void Poubelle::handlePIRMotion(int binId)
{
    // Get current cap and used value from DB direct
    QSqlQuery q;
    q.prepare("SELECT CAPACITE, COALESCE(USED_PERCENTAGE,0), ID_ZONE FROM GESPUB WHERE ID_BIN = :id");
    q.bindValue(":id", binId);
    if (!q.exec() || !q.next()) {
        qDebug() << "PIR: db lookup failed for bin" << binId;
        return;
    }
    double cap = q.value(0).toDouble();
    double used = q.value(1).toDouble();
    int zoneId = q.value(2).toInt();
    if (cap <= 0.0) return;

    double increment = (1.0 / cap) * 100.0;
    double newUsed = qBound(0.0, used + increment, 100.0);

    Poubelle p(binId, zoneId, cap, "");
    if (p.modifierUsedPercentage(binId, newUsed)) {
        qDebug() << "PIR: updated bin" << binId << "to" << newUsed;
        // Ask UI to refresh
        if (mainWindowPtr) {
                // Refresh UI from main thread
                QMetaObject::invokeMethod(mainWindowPtr, "reloadBinsCache", Qt::QueuedConnection);
                QMetaObject::invokeMethod(mainWindowPtr, "afficherPoubelles", Qt::QueuedConnection);
                QMetaObject::invokeMethod(mainWindowPtr, "refreshIrlDialog", Qt::QueuedConnection);
                QTimer::singleShot(0, Poubelle::mainWindowPtr, [zoneId](){ if (Poubelle::mainWindowPtr) Poubelle::mainWindowPtr->renderBinsGraph(zoneId); });
            }
    }
}
