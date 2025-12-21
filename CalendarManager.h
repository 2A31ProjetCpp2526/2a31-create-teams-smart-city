#ifndef CALENDARMANAGER_H
#define CALENDARMANAGER_H

#include <QSqlDatabase>
#include <QDate>
#include <QString>
#include <QStringList>
#include <QMap>
#include <QSet>
#include <QDebug>
#include <QObject>
#include <QSerialPort>
#include <QTimer>

class CalendarManager : public QObject
{
    Q_OBJECT
public:
    explicit CalendarManager(QSqlDatabase db, QObject *parent = nullptr);

    // Événements
    bool addEvent(const QDate &date, const QString &zone, const QString &action);
    bool removeEvent(const QDate &date, const QString &zone, const QString &action);
    void showEvents(const QDate &date);
    void notifyToday();
    void showHighlightedDates();

    // RainSensor
    bool connectRainSensor(const QString &portName, int baudRate);
    void disconnectRainSensor();
    bool isRainSensorConnected() const;

signals:
    void rainStateChanged(bool raining);

private slots:
    void readRainSensor();

private:
    // Base de données
    QSqlDatabase db;
    QMap<QDate, QStringList> events;
    QSet<QDate> highlightedDates;

    // RainSensor Arduino
    QSerialPort *serialPort;
    bool lastRainState;
    QTimer *timer;
    float totalLitres;

    // Paramètres pluie
    float surfaceZone;       // en m²
    float rainIntensity;     // mm/sec approximatif
};

#endif // CALENDARMANAGER_H
