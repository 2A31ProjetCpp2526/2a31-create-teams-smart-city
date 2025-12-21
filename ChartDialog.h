#ifndef CHARTDIALOG_H
#define CHARTDIALOG_H

#include <QDialog>
#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QChart>

//QT_CHARTS_USE_NAMESPACE

    class ChartDialog : public QDialog
{
    Q_OBJECT
public:
    explicit ChartDialog(QWidget *parent = nullptr);

private:
    void loadBarChart(QChartView *view);
    void loadPieChart(QChartView *view);
};

#endif // CHARTDIALOG_H
