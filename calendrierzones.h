#ifndef CALENDRIERZONES_H
#define CALENDRIERZONES_H

#include <QWidget>
#include <QCalendarWidget>
#include <QMap>
#include <QDate>
#include <QString>
#include <QList>
#include <QColor>

struct ZoneData {
    QString zoneId;
    QList<QString> events;
};

class CalendrierZones : public QWidget
{
    Q_OBJECT
public:
    explicit CalendrierZones(QWidget *parent = nullptr);

    // Add a zone and an optional event on a given date
    bool addZone(const QDate &date, const QString &zoneId, const QString &event = QString());

    // Add an event to an existing zone
    void addEventToZone(const QDate &date, const QString &zoneId, const QString &event);

    // Remove a specific event from a date
    void removeEventFromZone(const QDate &date, const QString &event);

    // Refresh the calendar highlights
    void refresh();

    // Set the background color of the entire calendar
    void setBackgroundColor(const QColor &color);

signals:
    void zoneClicked(const QDate &date);

private:
    QCalendarWidget *calendar;

    // Stores all zones and their events by date
    QMap<QDate, QList<ZoneData>> zones;

    // Helper functions
    int findZoneIndex(const QDate &date, const QString &zoneId) const;
    void highlightZones();
};

#endif // CALENDRIERZONES_H


