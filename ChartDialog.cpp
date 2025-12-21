#include "ChartDialog.h"
#include <QVBoxLayout>
#include <QSqlQuery>
#include <QPainter>

ChartDialog::ChartDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle("Graphiques - Zones Vertes");
    resize(900, 700);

    QVBoxLayout *layout = new QVBoxLayout(this);

    // Create two chart views (bar + pie)
    QChartView *barView = new QChartView(this);
    barView->setRenderHint(QPainter::Antialiasing);
    barView->setMinimumHeight(330);

    QChartView *pieView = new QChartView(this);
    pieView->setRenderHint(QPainter::Antialiasing);
    pieView->setMinimumHeight(330);

    layout->addWidget(barView);
    layout->addWidget(pieView);

    loadBarChart(barView);
    loadPieChart(pieView);
}

// ---------------------------------------------------------
// BAR CHART: shows each zone's superficie (NOM / SUPERFICIE)
// ---------------------------------------------------------
void ChartDialog::loadBarChart(QChartView *view)
{
    QSqlQuery q("SELECT NOM, NVL(SUPERFICIE,0) FROM ZONES_VERTES");

    QBarSet *set = new QBarSet("Superficie (ha)");
    QStringList categories;

    // Read DB rows; keep values in a QVector for stability
    QVector<double> values;
    while (q.next()) {
        QString name = q.value(0).toString();
        double superficie = q.value(1).toDouble();
        categories << name;
        values.append(superficie);
    }

    for (double v : values) *set << v;

    QBarSeries *series = new QBarSeries();
    series->append(set);

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Superficie par zone");
    chart->setAnimationOptions(QChart::SeriesAnimations);

    QBarCategoryAxis *axisX = new QBarCategoryAxis();
    axisX->append(categories);
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    chart->createDefaultAxes();

    view->setChart(chart);
}

// ---------------------------------------------------------
// PIE CHART: percentage of zones by ETAT
// ---------------------------------------------------------
void ChartDialog::loadPieChart(QChartView *view)
{
    // FIXED: table name was incorrect (ZONES_VERTES instead of ZONE_VERTES)
    QSqlQuery q("SELECT NVL(ETAT,'Inconnu') AS ETAT, COUNT(*) "
                "FROM ZONES_VERTES "
                "GROUP BY NVL(ETAT,'Inconnu')");

    QPieSeries *series = new QPieSeries();

    while (q.next()) {
        QString etat = q.value(0).toString();
        int count = q.value(1).toInt();
        if (count > 0)
            series->append(etat, count);
    }

    // If empty → prevents invisible chart
    if (series->slices().isEmpty()) {
        series->append("Aucune donnée", 1);
    }

    series->setLabelsVisible(true);

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Répartition des zones par état");
    chart->legend()->setAlignment(Qt::AlignRight);

    view->setChart(chart);
}


