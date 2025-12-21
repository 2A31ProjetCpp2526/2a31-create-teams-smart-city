#ifndef SMARTMETAL_H
#define SMARTMETAL_H

#include <QDialog>
#include <QTableWidget>
#include <QPushButton>
#include <QString>
#include <QVector>

// --------------------------------------------------
// Training Sample structure
// --------------------------------------------------
struct Sample {
    double weight;
    double conductivity;
    int magnetic;
    double infraredPurity;
    double radiationLevel;
    QString label;
};

// --------------------------------------------------
// SmartMetal Classifier (Static)
// --------------------------------------------------
class SmartMetal
{
public:
    static QString classifyMetal(double weight,
                                 double conduct,
                                 int magnetic,
                                 double irPurity,
                                 double radiation);
};

// --------------------------------------------------
// SmartMetalDialog UI Class
// --------------------------------------------------
class SmartMetalDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SmartMetalDialog(QWidget *parent = nullptr);

private:
    // --- UI Elements ---
    QTableWidget *table;
    QPushButton  *addRowBtn;
    QPushButton  *loadBtn;

    // --- Internal Methods ---
    void addMetalRow(double weight,
                     double conduct,
                     int magnetic,
                     double irPurity,
                     double radiation);

    void applyRowColor(int row, const QString &result);

    void insertIntoDatabase(double weight,
                            double conduct,
                            int magnetic,
                            double irPurity,
                            double radiation);

    void loadFromDatabase();
};

#endif // SMARTMETAL_H


