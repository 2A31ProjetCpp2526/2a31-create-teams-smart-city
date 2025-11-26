#ifndef GRAPH_H
#define GRAPH_H

#include <QObject>
#include <QVector>
#include <QString>

class QGraphicsScene;

class Graph : public QObject
{
    Q_OBJECT
public:
    explicit Graph(QObject *parent = nullptr);
    ~Graph(); // <- Added destructor

    struct Zone {
        int id = 0;
        int pop = 0;
        double x = 0.0;
        double y = 0.0;
        double h = 0.0;
        double l = 0.0;
        double surface = 0.0;
        double density = 0.0;
    };

    void clear();
    void addZone(int id, int pop, double x, double y, double h, double l);

    QVector<Zone> zones() const;
    bool hasZone(int id) const;
    const Zone* zoneByIdPtr(int id) const;
    const Zone* zoneById(int id) const { return zoneByIdPtr(id); }

    void renderToScene(QGraphicsScene *scene) const;
    void loadFromDatabase();

private:
    QVector<Zone> m_zones;
    static QString densityColor(double density);
};

#endif // GRAPH_H
