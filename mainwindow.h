#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QCalendarWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QTableWidget>
#include "calendrierzones.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    // ------------------- UI -------------------
    QWidget *centralWidget;
    QVBoxLayout *mainLayout;
    QHBoxLayout *leftRightLayout;
    QVBoxLayout *calLayout;

    QCalendarWidget *calendarWidget;
    QLineEdit *eventEdit;
    QPushButton *addButton;
    QPushButton *deleteButton;
    QLabel *statsLabel;
    QTableWidget *tableWidget;

    CalendrierZones *zonesCalendarWidget;
    QPushButton *toggleCalendarBtn;

    // ------------------- DATABASE -------------------
    bool setupDatabase();

    // ------------------- UI & STYLE -------------------
    void setupUI();
    void setupStyle();
    void setupConnections();

    // ------------------- CRUD -------------------
    void loadEvents();
    void addEvent();
    void deleteEvent();

    // ------------------- STATS -------------------
    void updateStats();

    // ------------------- CALENDAR -------------------
    void toggleCalendar();

    // ------------------- PDF EXPORT -------------------
    void exportToPdf();

};

#endif // MAINWINDOW_H
