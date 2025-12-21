#include "calendarmanager.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QStringList>
#include <QByteArray>

CalendarManager::CalendarManager(QSqlDatabase db, QObject *parent)
    : QObject(parent), db(db), serialPort(nullptr), lastRainState(false),
    totalLitres(0), surfaceZone(2.0), rainIntensity(0.5)
{
    timer = new QTimer(this);
    timer->setInterval(1000); // lecture toutes les secondes
    connect(timer, &QTimer::timeout, this, &CalendarManager::readRainSensor);
}

// ====================
// Gestion événements
// ====================
bool CalendarManager::addEvent(const QDate &date, const QString &zone, const QString &action)
{
    if (!db.isOpen()) {
        qDebug() << "Erreur : base non connectée.";
        return false;
    }

    QString dateStr = date.toString("yyyy-MM-dd");
    QSqlQuery q(db);
    q.prepare("INSERT INTO SAIM.TABLE1 (ZONE, DATE_EVENT, ACTION) "
              "VALUES (:zone, TO_DATE(:d,'YYYY-MM-DD'), :action)");
    q.bindValue(":zone", zone);
    q.bindValue(":d", dateStr);
    q.bindValue(":action", action);

    if (!q.exec()) {
        qDebug() << "Erreur SQL:" << q.lastError().text();
        return false;
    }

    QString text = zone + ": " + action;
    events[date].append(text);
    highlightedDates.insert(date);
    qDebug() << "Événement ajouté:" << text << "le" << date.toString();
    return true;
}

bool CalendarManager::removeEvent(const QDate &date, const QString &zone, const QString &action)
{
    if (!db.isOpen()) {
        qDebug() << "Erreur : base non connectée.";
        return false;
    }

    QString dateStr = date.toString("yyyy-MM-dd");
    QSqlQuery q(db);
    q.prepare("DELETE FROM SAIM.TABLE1 "
              "WHERE ZONE = :zone AND DATE_EVENT = TO_DATE(:d,'YYYY-MM-DD') AND ACTION = :action");
    q.bindValue(":zone", zone);
    q.bindValue(":d", dateStr);
    q.bindValue(":action", action);

    if (!q.exec()) {
        qDebug() << "Erreur SQL:" << q.lastError().text();
        return false;
    }

    QString text = zone + ": " + action;
    events[date].removeOne(text);
    if (events[date].isEmpty())
        highlightedDates.remove(date);

    qDebug() << "Événement supprimé:" << text << "le" << date.toString();
    return true;
}

void CalendarManager::showEvents(const QDate &date)
{
    qDebug() << "Événements pour le" << date.toString() << ":";
    if (!events.contains(date) || events[date].isEmpty()) {
        qDebug() << "  Aucun événement.";
        return;
    }
    for (const QString &evt : events[date])
        qDebug() << "  -" << evt;
}

void CalendarManager::notifyToday()
{
    QDate today = QDate::currentDate();
    if (!events.contains(today)) return;

    for (const QString &evt : events[today])
        qDebug() << "[Notification] Aujourd'hui:" << evt;
}

void CalendarManager::showHighlightedDates()
{
    qDebug() << "Dates avec événements (surlignées):";
    for (const QDate &d : highlightedDates)
        qDebug() << " -" << d.toString();
}

// ====================
// Gestion RainSensor
// ====================
bool CalendarManager::connectRainSensor(const QString &portName, int baudRate)
{
    serialPort = new QSerialPort(this);
    serialPort->setPortName(portName);
    serialPort->setBaudRate(baudRate);
    serialPort->setDataBits(QSerialPort::Data8);
    serialPort->setParity(QSerialPort::NoParity);
    serialPort->setStopBits(QSerialPort::OneStop);
    serialPort->setFlowControl(QSerialPort::NoFlowControl);

    connect(serialPort, &QSerialPort::readyRead, this, &CalendarManager::readRainSensor);

    if (serialPort->open(QIODevice::ReadOnly)) {
        qDebug() << "RainSensor connecté sur le port:" << portName;
        timer->start();
        return true;
    } else {
        qDebug() << "Erreur connexion RainSensor:" << serialPort->errorString();
        return false;
    }
}

void CalendarManager::disconnectRainSensor()
{
    if (serialPort && serialPort->isOpen()) {
        serialPort->close();
        qDebug() << "RainSensor déconnecté.";
    }
    if (timer->isActive()) timer->stop();
}

bool CalendarManager::isRainSensorConnected() const
{
    return serialPort && serialPort->isOpen();
}

void CalendarManager::readRainSensor()
{
    if (!serialPort) return;

    QByteArray data = serialPort->readAll();
    QString message = QString::fromUtf8(data).trimmed();

    bool raining = false;

    if (message.contains("The rain is detected"))
        raining = true;
    else if (message.contains("The rain is NOT detected"))
        raining = false;
    else
        return; // message inconnu

    if (raining != lastRainState) {
        lastRainState = raining;
        emit rainStateChanged(raining);
        qDebug() << "État pluie changé:" << (raining ? "Pluie" : "Pas de pluie");
    }

    if (raining) {
        float litres = rainIntensity * surfaceZone; // litres/sec approximatif
        totalLitres += litres;
        qDebug() << "Litres estimés accumulés:" << totalLitres;
    }
}
