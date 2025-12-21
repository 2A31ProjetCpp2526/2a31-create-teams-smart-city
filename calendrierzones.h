#ifndef CALENDRIERZONES_H
#define CALENDRIERZONES_H

#include <QWidget>
#include <QCalendarWidget>
#include <QMap>
#include <QDate>
#include <QString>
#include <QStringList>

struct ZoneData {
    QString zoneId;
    QStringList events; // List of events for this zone/date
};

class CalendrierZones : public QWidget
{
    Q_OBJECT
public:
    explicit CalendrierZones(QWidget *parent = nullptr);

    bool addZone(const QDate &date, const QString &zoneId, const QString &event = QString());
    void addEventToZone(const QDate &date, const QString &zoneId, const QString &event);
    void removeEventFromZone(const QDate &date, const QString &event);
    void refresh();

    const QMap<QDate, QList<ZoneData>>& getZones() const { return zones; }

signals:
    void zoneClicked(const QDate &date);

private:
    QCalendarWidget *calendar;
    QMap<QDate, QList<ZoneData>> zones; // Date -> list of ZoneData

    void highlightZones();
    int findZoneIndex(const QDate &date, const QString &zoneId) const;
};

#endif // CALENDRIERZONES_H

