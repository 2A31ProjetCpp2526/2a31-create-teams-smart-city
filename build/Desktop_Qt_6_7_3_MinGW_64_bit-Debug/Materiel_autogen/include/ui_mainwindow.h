/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.7.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QStackedWidget *stackedWidget;
    QWidget *pageMain;
    QFrame *frame_2;
    QLabel *label_9;
    QLabel *label_10;
    QLabel *label_11;
    QLabel *label_12;
    QLabel *label_13;
    QLabel *label_14;
    QPushButton *ajpub_3;
    QPushButton *modpub_3;
    QPushButton *supppub_2;
    QPushButton *pe_2;
    QTableWidget *tableWidget_2;
    QLabel *label_15;
    QLabel *label_16;
    QPushButton *gespub_2;
    QPushButton *geszone1;
    QWidget *page_5;
    QWidget *zzzz;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(1389, 684);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        stackedWidget = new QStackedWidget(centralwidget);
        stackedWidget->setObjectName("stackedWidget");
        stackedWidget->setGeometry(QRect(40, 30, 1301, 571));
        pageMain = new QWidget();
        pageMain->setObjectName("pageMain");
        frame_2 = new QFrame(pageMain);
        frame_2->setObjectName("frame_2");
        frame_2->setGeometry(QRect(10, 80, 1331, 461));
        frame_2->setStyleSheet(QString::fromUtf8("background-color: rgb(180, 180, 180);\n"
"background-color: rgb(206, 206, 206);\n"
""));
        frame_2->setFrameShape(QFrame::Shape::StyledPanel);
        frame_2->setFrameShadow(QFrame::Shadow::Raised);
        label_9 = new QLabel(frame_2);
        label_9->setObjectName("label_9");
        label_9->setGeometry(QRect(10, 10, 221, 51));
        label_9->setStyleSheet(QString::fromUtf8("font: 16pt \"Segoe UI\";\n"
"font: 700 16pt \"Segoe UI\";\n"
"color: rgb(0, 51, 102);"));
        label_10 = new QLabel(frame_2);
        label_10->setObjectName("label_10");
        label_10->setGeometry(QRect(110, 70, 171, 51));
        label_10->setStyleSheet(QString::fromUtf8("font: 16pt \"Segoe UI\";\n"
"font: 700 16pt \"Segoe UI\";\n"
"color: rgb(0, 51, 102);"));
        label_11 = new QLabel(frame_2);
        label_11->setObjectName("label_11");
        label_11->setGeometry(QRect(50, 70, 201, 41));
        label_11->setStyleSheet(QString::fromUtf8("font: 16pt \"Segoe UI\";\n"
"background-color: rgb(173, 216, 230);\n"
"font: 700 14.5pt \"Segoe UI\";\n"
"color: rgb(0, 51, 102);\n"
"border-radius: 15px; "));
        label_12 = new QLabel(frame_2);
        label_12->setObjectName("label_12");
        label_12->setGeometry(QRect(300, 70, 201, 41));
        label_12->setStyleSheet(QString::fromUtf8("font: 16pt \"Segoe UI\";\n"
"background-color: rgb(173, 216, 230);\n"
"font: 700 14.5pt \"Segoe UI\";\n"
"color: rgb(0, 51, 102);\n"
"border-radius: 15px; "));
        label_13 = new QLabel(frame_2);
        label_13->setObjectName("label_13");
        label_13->setGeometry(QRect(560, 70, 201, 41));
        label_13->setStyleSheet(QString::fromUtf8("font: 16pt \"Segoe UI\";\n"
"background-color: rgb(173, 216, 230);\n"
"font: 700 14.5pt \"Segoe UI\";\n"
"color: rgb(0, 51, 102);\n"
"border-radius: 15px; "));
        label_14 = new QLabel(frame_2);
        label_14->setObjectName("label_14");
        label_14->setGeometry(QRect(810, 70, 201, 41));
        label_14->setStyleSheet(QString::fromUtf8("font: 16pt \"Segoe UI\";\n"
"background-color: rgb(173, 216, 230);\n"
"font: 700 14.5pt \"Segoe UI\";\n"
"color: rgb(0, 51, 102);\n"
"border-radius: 15px; "));
        ajpub_3 = new QPushButton(frame_2);
        ajpub_3->setObjectName("ajpub_3");
        ajpub_3->setGeometry(QRect(20, 170, 141, 41));
        ajpub_3->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: #003366; /* main button color */\n"
"    color: white;              /* text color */\n"
"    border: none;\n"
"    padding: 8px 16px;\n"
"    border-radius: 6px;\n"
"    font-size: 14px;\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"    background-color: #00509e; /* hover color */\n"
"}\n"
"\n"
"QPushButton:pressed {\n"
"    background-color: #001f4d; /* darker when pressed */\n"
"}\n"
""));
        modpub_3 = new QPushButton(frame_2);
        modpub_3->setObjectName("modpub_3");
        modpub_3->setGeometry(QRect(190, 170, 141, 41));
        modpub_3->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: #003366; /* main button color */\n"
"    color: white;              /* text color */\n"
"    border: none;\n"
"    padding: 8px 16px;\n"
"    border-radius: 6px;\n"
"    font-size: 14px;\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"    background-color: #00509e; /* hover color */\n"
"}\n"
"\n"
"QPushButton:pressed {\n"
"    background-color: #001f4d; /* darker when pressed */\n"
"}\n"
""));
        supppub_2 = new QPushButton(frame_2);
        supppub_2->setObjectName("supppub_2");
        supppub_2->setGeometry(QRect(360, 170, 141, 41));
        supppub_2->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: #003366; /* main button color */\n"
"    color: white;              /* text color */\n"
"    border: none;\n"
"    padding: 8px 16px;\n"
"    border-radius: 6px;\n"
"    font-size: 14px;\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"    background-color: #00509e; /* hover color */\n"
"}\n"
"\n"
"QPushButton:pressed {\n"
"    background-color: #001f4d; /* darker when pressed */\n"
"}\n"
""));
        pe_2 = new QPushButton(frame_2);
        pe_2->setObjectName("pe_2");
        pe_2->setGeometry(QRect(530, 170, 151, 41));
        pe_2->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: #003366; /* main button color */\n"
"    color: white;              /* text color */\n"
"    border: none;\n"
"    padding: 8px 16px;\n"
"    border-radius: 6px;\n"
"    font-size: 14px;\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"    background-color: #00509e; /* hover color */\n"
"}\n"
"\n"
"QPushButton:pressed {\n"
"    background-color: #001f4d; /* darker when pressed */\n"
"}\n"
""));
        tableWidget_2 = new QTableWidget(frame_2);
        if (tableWidget_2->columnCount() < 7)
            tableWidget_2->setColumnCount(7);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        tableWidget_2->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        tableWidget_2->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        tableWidget_2->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        tableWidget_2->setHorizontalHeaderItem(3, __qtablewidgetitem3);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        tableWidget_2->setHorizontalHeaderItem(4, __qtablewidgetitem4);
        QTableWidgetItem *__qtablewidgetitem5 = new QTableWidgetItem();
        tableWidget_2->setHorizontalHeaderItem(5, __qtablewidgetitem5);
        QTableWidgetItem *__qtablewidgetitem6 = new QTableWidgetItem();
        tableWidget_2->setHorizontalHeaderItem(6, __qtablewidgetitem6);
        tableWidget_2->setObjectName("tableWidget_2");
        tableWidget_2->setGeometry(QRect(270, 240, 701, 211));
        tableWidget_2->setStyleSheet(QString::fromUtf8("QTableWidget {\n"
"    background-color: #E0F7FF;      /* soft light blue background */\n"
"    border-radius: 10px;            /* rounded corners */\n"
"    gridline-color: #ADD8E6;        /* grid lines color */\n"
"    font-size: 14px;\n"
"}\n"
"\n"
"QHeaderView::section {\n"
"    background-color: #003366;\n"
"    color: white;\n"
"    font-size: 16px;\n"
"    font-weight: bold;\n"
"    border: 1px solid #003366;\n"
"    padding: 0px; /* remove extra padding */\n"
"}\n"
"\n"
"QTableWidget::item:selected {\n"
"    background-color: #87CEEB;      /* sky blue when selected */\n"
"    color: #003366;\n"
"}\n"
"\n"
"QTableWidget::item:hover {\n"
"    background-color: #D0E4F5;      /* light hover effect */\n"
"}"));
        label_15 = new QLabel(pageMain);
        label_15->setObjectName("label_15");
        label_15->setGeometry(QRect(-30, 0, 1461, 61));
        label_15->setStyleSheet(QString::fromUtf8("background-color:#ADD8E6;"));
        label_16 = new QLabel(pageMain);
        label_16->setObjectName("label_16");
        label_16->setGeometry(QRect(40, 10, 311, 61));
        label_16->setStyleSheet(QString::fromUtf8("font: 16pt \"Segoe UI\";\n"
"font: 700 16pt \"Segoe UI\";\n"
"color: rgb(0, 51, 102);"));
        gespub_2 = new QPushButton(pageMain);
        gespub_2->setObjectName("gespub_2");
        gespub_2->setGeometry(QRect(830, 10, 171, 41));
        gespub_2->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: #003366; /* main button color */\n"
"    color: white;              /* text color */\n"
"    border: none;\n"
"    padding: 8px 16px;\n"
"    border-radius: 6px;\n"
"    font-size: 14px;\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"    background-color: #00509e; /* hover color */\n"
"}\n"
"\n"
"QPushButton:pressed {\n"
"    background-color: #001f4d; /* darker when pressed */\n"
"}\n"
""));
        geszone1 = new QPushButton(pageMain);
        geszone1->setObjectName("geszone1");
        geszone1->setGeometry(QRect(1050, 10, 171, 41));
        geszone1->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: #003366; /* main button color */\n"
"    color: white;              /* text color */\n"
"    border: none;\n"
"    padding: 8px 16px;\n"
"    border-radius: 6px;\n"
"    font-size: 14px;\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"    background-color: #00509e; /* hover color */\n"
"}\n"
"\n"
"QPushButton:pressed {\n"
"    background-color: #001f4d; /* darker when pressed */\n"
"}\n"
""));
        stackedWidget->addWidget(pageMain);
        page_5 = new QWidget();
        page_5->setObjectName("page_5");
        stackedWidget->addWidget(page_5);
        zzzz = new QWidget();
        zzzz->setObjectName("zzzz");
        stackedWidget->addWidget(zzzz);
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 1389, 21));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        stackedWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        label_9->setText(QCoreApplication::translate("MainWindow", "Gestion des Poubelles\n"
"", nullptr));
        label_10->setText(QString());
        label_11->setText(QCoreApplication::translate("MainWindow", "    Total Poubelles :", nullptr));
        label_12->setText(QCoreApplication::translate("MainWindow", "             Plein  :", nullptr));
        label_13->setText(QCoreApplication::translate("MainWindow", "              Vide :", nullptr));
        label_14->setText(QCoreApplication::translate("MainWindow", "       Maintenance :", nullptr));
        ajpub_3->setText(QCoreApplication::translate("MainWindow", "Ajouter", nullptr));
        modpub_3->setText(QCoreApplication::translate("MainWindow", "Modifier", nullptr));
        supppub_2->setText(QCoreApplication::translate("MainWindow", "Supprimer", nullptr));
        pe_2->setText(QCoreApplication::translate("MainWindow", "Exporter PDF/Excel ", nullptr));
        QTableWidgetItem *___qtablewidgetitem = tableWidget_2->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QCoreApplication::translate("MainWindow", "ID", nullptr));
        QTableWidgetItem *___qtablewidgetitem1 = tableWidget_2->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QCoreApplication::translate("MainWindow", "Type Dechet", nullptr));
        QTableWidgetItem *___qtablewidgetitem2 = tableWidget_2->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QCoreApplication::translate("MainWindow", "X", nullptr));
        QTableWidgetItem *___qtablewidgetitem3 = tableWidget_2->horizontalHeaderItem(3);
        ___qtablewidgetitem3->setText(QCoreApplication::translate("MainWindow", "Y", nullptr));
        QTableWidgetItem *___qtablewidgetitem4 = tableWidget_2->horizontalHeaderItem(4);
        ___qtablewidgetitem4->setText(QCoreApplication::translate("MainWindow", "Zone ", nullptr));
        QTableWidgetItem *___qtablewidgetitem5 = tableWidget_2->horizontalHeaderItem(5);
        ___qtablewidgetitem5->setText(QCoreApplication::translate("MainWindow", "Capacite", nullptr));
        QTableWidgetItem *___qtablewidgetitem6 = tableWidget_2->horizontalHeaderItem(6);
        ___qtablewidgetitem6->setText(QCoreApplication::translate("MainWindow", "Etat", nullptr));
        label_15->setText(QString());
        label_16->setText(QCoreApplication::translate("MainWindow", "Recycling Dashboard\n"
"", nullptr));
        gespub_2->setText(QCoreApplication::translate("MainWindow", "Gestion Poubelles", nullptr));
        geszone1->setText(QCoreApplication::translate("MainWindow", "Gestion Zone", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
