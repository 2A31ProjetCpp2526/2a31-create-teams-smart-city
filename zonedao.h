#ifndef ZONEDAO_H
#define ZONEDAO_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDate>
#include <QDebug>
#include <QList>
#include <QString>

struct ZoneData {
    QString zone;
    QDate dateEvent;
    QString event;
};

class ZoneDAO
{
public:
    ZoneDAO();
    ~ZoneDAO();

    bool initDatabase();
    bool connectDatabase(const QString& dbPath);
    bool createTable();

    // CRUD Operations
    bool createZone(const ZoneData& data);
    QList<ZoneData> readZonesForDate(const QDate& date);
    QList<ZoneData> readAllZones();
    bool updateZone(const ZoneData& oldData, const ZoneData& newData);
    bool deleteZone(const ZoneData& data);

private:
    QSqlDatabase db;
};

#endif // ZONEDAO_H
