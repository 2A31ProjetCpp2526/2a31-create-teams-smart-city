#include "calendrierzones.h"
#include <QVBoxLayout>
#include <QTextCharFormat>
#include <QBrush>
#include <QColor>

CalendrierZones::CalendrierZones(QWidget *parent)
    : QWidget(parent)
{
    calendar = new QCalendarWidget(this);
    calendar->setGridVisible(true);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(calendar);
    setLayout(layout);

    connect(calendar, &QCalendarWidget::clicked, this, [this](const QDate &date){
        emit zoneClicked(date); // notify MainWindow
    });
}

bool CalendrierZones::addZone(const QDate &date, const QString &zoneId, const QString &event)
{
    if (zoneId.isEmpty()) return false;

    int index = findZoneIndex(date, zoneId);
    if (index == -1) {
        ZoneData z;
        z.zoneId = zoneId;
        if (!event.isEmpty()) z.events.append(event);
        zones[date].append(z);
    } else {
        if (!event.isEmpty()) zones[date][index].events.append(event);
    }

    highlightZones();
    return true;
}

void CalendrierZones::addEventToZone(const QDate &date, const QString &zoneId, const QString &event)
{
    addZone(date, zoneId, event);
}

void CalendrierZones::removeEventFromZone(const QDate &date, const QString &event)
{
    if (!zones.contains(date)) return;

    auto &zoneList = zones[date];
    for (int i = zoneList.size() - 1; i >= 0; --i) {
        ZoneData &z = zoneList[i];
        int eventIndex = z.events.indexOf(event);
        if (eventIndex != -1) z.events.removeAt(eventIndex);
        if (z.events.isEmpty()) zoneList.removeAt(i);
    }

    if (zoneList.isEmpty()) zones.remove(date);

    highlightZones();
}

int CalendrierZones::findZoneIndex(const QDate &date, const QString &zoneId) const
{
    if (!zones.contains(date)) return -1;
    const QList<ZoneData> &list = zones[date];
    for (int i = 0; i < list.size(); ++i) {
        if (list[i].zoneId == zoneId) return i;
    }
    return -1;
}

void CalendrierZones::highlightZones()
{
    QTextCharFormat format;
    format.setBackground(QBrush(QColor("#a5d6a7"))); // green
    format.setForeground(QBrush(Qt::black));

    calendar->setDateTextFormat(QDate(), QTextCharFormat()); // clear previous

    for (auto it = zones.begin(); it != zones.end(); ++it) {
        calendar->setDateTextFormat(it.key(), format);
    }
}

void CalendrierZones::refresh()
{
    highlightZones();
}

