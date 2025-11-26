#include "graph.h"
#include <QGraphicsRectItem>
#include <QGraphicsScene>
#include <QGraphicsSimpleTextItem>
#include <QBrush>
#include <QPen>
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QDebug>

Graph::Graph(QObject *parent)
    : QObject(parent)
{
}

// Define destructor to fix vtable error
Graph::~Graph() {}

void Graph::clear()
{
    m_zones.clear();
}

void Graph::addZone(int id, int pop, double x, double y, double h, double l)
{
    double surface = h * l;
    double density = surface > 0.0 ? (static_cast<double>(pop) / surface) : 0.0;

    for (Zone &z : m_zones) {
        if (z.id == id) {
            z.pop = pop;
            z.x = x;
            z.y = y;
            z.h = h;
            z.l = l;
            z.surface = surface;
            z.density = density;
            return;
        }
    }

    Zone zone;
    zone.id = id;
    zone.pop = pop;
    zone.x = x;
    zone.y = y;
    zone.h = h;
    zone.l = l;
    zone.surface = surface;
    zone.density = density;
    m_zones.append(zone);
}

QVector<Graph::Zone> Graph::zones() const
{
    return m_zones;
}

bool Graph::hasZone(int id) const
{
    for (const Zone &z : m_zones) {
        if (z.id == id) return true;
    }
    return false;
}

const Graph::Zone* Graph::zoneByIdPtr(int id) const
{
    for (const Zone &z : m_zones) {
        if (z.id == id) return &z;
    }
    return nullptr;
}

void Graph::renderToScene(QGraphicsScene *scene) const
{
    if (!scene) return;
    scene->clear();

    const double baseScale = 4.0;
    const double posScale = 8.0;

    for (const Zone &z : m_zones) {

        double w = z.l * baseScale;
        double h = z.h * baseScale;
        double left = z.x * posScale + 10.0;
        double top  = (200.0 - z.y * posScale) - h;

        QRectF rect(left, top, w, h);

        QColor brushColor(Qt::green);
        QString c = densityColor(z.density);
        if (c == "green") brushColor = Qt::green;
        else if (c == "gold") brushColor = QColor(255,215,0);
        else brushColor = Qt::red;

        // Draw rectangle
        QGraphicsRectItem *item =
            scene->addRect(rect, QPen(Qt::black), QBrush(brushColor));
        item->setOpacity(0.6);
        item->setZValue(0);

        // Add zone ID text (centered)
        QGraphicsSimpleTextItem *label =
            scene->addSimpleText(QString::number(z.id));

        QFont f = label->font();
        f.setPointSize(45);
        f.setBold(true);
        label->setFont(f);
        label->setBrush(Qt::black);

        QRectF t = label->boundingRect();
        double x = left + (w - t.width()) / 2.0;
        double y = top  + (h - t.height()) / 2.0;

        label->setPos(x, y);
        label->setZValue(1000);
    }
}

void Graph::loadFromDatabase()
{
    clear();

    QSqlQuery query;
    if (!query.exec("SELECT ID_ZONE, POPULATION, X, Y, H, L FROM GZONE")) {
        qDebug() << "Graph::loadFromDatabase SQL Error:" << query.lastError().text();
        return;
    }

    while (query.next()) {
        const int id = query.value("ID_ZONE").toInt();
        const int pop = query.value("POPULATION").toInt();
        const double x = query.value("X").toDouble();
        const double y = query.value("Y").toDouble();
        const double h = query.value("H").toDouble();
        const double l = query.value("L").toDouble();
        addZone(id, pop, x, y, h, l);
    }
}

QString Graph::densityColor(double density)
{
    if (density < 20.0) return QStringLiteral("green");
    if (density < 60.0) return QStringLiteral("gold");
    return QStringLiteral("red");
}
