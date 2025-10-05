/********************************************************************************
** Form generated from reading UI file 'geszone.ui'
**
** Created by: Qt User Interface Compiler version 6.7.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GESZONE_H
#define UI_GESZONE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_geszone
{
public:
    QFrame *frame;
    QLabel *label_3;
    QLabel *label_4;
    QLabel *label_5;
    QLabel *label_6;
    QLabel *label_7;
    QPushButton *ajzone;
    QPushButton *pe;
    QTableWidget *tableWidget;
    QLabel *label;
    QLabel *label_2;
    QPushButton *gespub;
    QPushButton *geszone1;

    void setupUi(QWidget *geszone)
    {
        if (geszone->objectName().isEmpty())
            geszone->setObjectName("geszone");
        geszone->resize(1357, 683);
        frame = new QFrame(geszone);
        frame->setObjectName("frame");
        frame->setGeometry(QRect(30, 130, 1331, 461));
        frame->setStyleSheet(QString::fromUtf8("background-color: rgb(180, 180, 180);\n"
"background-color: rgb(206, 206, 206);\n"
""));
        frame->setFrameShape(QFrame::Shape::StyledPanel);
        frame->setFrameShadow(QFrame::Shadow::Raised);
        label_3 = new QLabel(frame);
        label_3->setObjectName("label_3");
        label_3->setGeometry(QRect(10, 10, 221, 51));
        label_3->setStyleSheet(QString::fromUtf8("font: 16pt \"Segoe UI\";\n"
"font: 700 16pt \"Segoe UI\";\n"
"color: rgb(0, 51, 102);"));
        label_4 = new QLabel(frame);
        label_4->setObjectName("label_4");
        label_4->setGeometry(QRect(110, 70, 171, 51));
        label_4->setStyleSheet(QString::fromUtf8("font: 16pt \"Segoe UI\";\n"
"font: 700 16pt \"Segoe UI\";\n"
"color: rgb(0, 51, 102);"));
        label_5 = new QLabel(frame);
        label_5->setObjectName("label_5");
        label_5->setGeometry(QRect(70, 70, 251, 41));
        label_5->setStyleSheet(QString::fromUtf8("font: 16pt \"Segoe UI\";\n"
"background-color: rgb(173, 216, 230);\n"
"font: 700 14.5pt \"Segoe UI\";\n"
"color: rgb(0, 51, 102);\n"
"border-radius: 15px; "));
        label_6 = new QLabel(frame);
        label_6->setObjectName("label_6");
        label_6->setGeometry(QRect(420, 70, 291, 41));
        label_6->setStyleSheet(QString::fromUtf8("font: 16pt \"Segoe UI\";\n"
"background-color: rgb(173, 216, 230);\n"
"font: 700 14.5pt \"Segoe UI\";\n"
"color: rgb(0, 51, 102);\n"
"border-radius: 15px; "));
        label_7 = new QLabel(frame);
        label_7->setObjectName("label_7");
        label_7->setGeometry(QRect(800, 70, 361, 41));
        label_7->setStyleSheet(QString::fromUtf8("font: 16pt \"Segoe UI\";\n"
"background-color: rgb(173, 216, 230);\n"
"font: 700 14.5pt \"Segoe UI\";\n"
"color: rgb(0, 51, 102);\n"
"border-radius: 15px; "));
        ajzone = new QPushButton(frame);
        ajzone->setObjectName("ajzone");
        ajzone->setGeometry(QRect(110, 160, 181, 61));
        ajzone->setStyleSheet(QString::fromUtf8("QPushButton {\n"
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
        pe = new QPushButton(frame);
        pe->setObjectName("pe");
        pe->setGeometry(QRect(340, 160, 171, 61));
        pe->setStyleSheet(QString::fromUtf8("QPushButton {\n"
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
        tableWidget = new QTableWidget(frame);
        if (tableWidget->columnCount() < 7)
            tableWidget->setColumnCount(7);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(3, __qtablewidgetitem3);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(4, __qtablewidgetitem4);
        QTableWidgetItem *__qtablewidgetitem5 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(5, __qtablewidgetitem5);
        QTableWidgetItem *__qtablewidgetitem6 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(6, __qtablewidgetitem6);
        tableWidget->setObjectName("tableWidget");
        tableWidget->setGeometry(QRect(270, 240, 701, 211));
        tableWidget->setStyleSheet(QString::fromUtf8("QTableWidget {\n"
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
        label = new QLabel(geszone);
        label->setObjectName("label");
        label->setGeometry(QRect(-30, 40, 1461, 61));
        label->setStyleSheet(QString::fromUtf8("background-color:#ADD8E6;"));
        label_2 = new QLabel(geszone);
        label_2->setObjectName("label_2");
        label_2->setGeometry(QRect(10, 10, 401, 141));
        label_2->setStyleSheet(QString::fromUtf8("font: 16pt \"Segoe UI\";\n"
"font: 700 16pt \"Segoe UI\";\n"
"color: rgb(0, 51, 102);"));
        gespub = new QPushButton(geszone);
        gespub->setObjectName("gespub");
        gespub->setGeometry(QRect(890, 50, 171, 41));
        gespub->setStyleSheet(QString::fromUtf8("QPushButton {\n"
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
        geszone1 = new QPushButton(geszone);
        geszone1->setObjectName("geszone1");
        geszone1->setGeometry(QRect(1090, 50, 171, 41));
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

        retranslateUi(geszone);

        QMetaObject::connectSlotsByName(geszone);
    } // setupUi

    void retranslateUi(QWidget *geszone)
    {
        geszone->setWindowTitle(QCoreApplication::translate("geszone", "Form", nullptr));
        label_3->setText(QCoreApplication::translate("geszone", "Gestion des Zones", nullptr));
        label_4->setText(QString());
        label_5->setText(QCoreApplication::translate("geszone", "             Total Zones :", nullptr));
        label_6->setText(QCoreApplication::translate("geszone", "              Zones Compl\303\250tes :", nullptr));
        label_7->setText(QCoreApplication::translate("geszone", "           Zones Partiellement Pleines :", nullptr));
        ajzone->setText(QCoreApplication::translate("geszone", "Gerer Zone", nullptr));
        pe->setText(QCoreApplication::translate("geszone", "Exporter PDF/Excel ", nullptr));
        QTableWidgetItem *___qtablewidgetitem = tableWidget->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QCoreApplication::translate("geszone", "ID", nullptr));
        QTableWidgetItem *___qtablewidgetitem1 = tableWidget->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QCoreApplication::translate("geszone", "Population", nullptr));
        QTableWidgetItem *___qtablewidgetitem2 = tableWidget->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QCoreApplication::translate("geszone", "n poubelles", nullptr));
        QTableWidgetItem *___qtablewidgetitem3 = tableWidget->horizontalHeaderItem(3);
        ___qtablewidgetitem3->setText(QCoreApplication::translate("geszone", "X", nullptr));
        QTableWidgetItem *___qtablewidgetitem4 = tableWidget->horizontalHeaderItem(4);
        ___qtablewidgetitem4->setText(QCoreApplication::translate("geszone", "Y", nullptr));
        QTableWidgetItem *___qtablewidgetitem5 = tableWidget->horizontalHeaderItem(5);
        ___qtablewidgetitem5->setText(QCoreApplication::translate("geszone", "H", nullptr));
        QTableWidgetItem *___qtablewidgetitem6 = tableWidget->horizontalHeaderItem(6);
        ___qtablewidgetitem6->setText(QCoreApplication::translate("geszone", "L", nullptr));
        label->setText(QString());
        label_2->setText(QCoreApplication::translate("geszone", "Recycling Dashboard\n"
"", nullptr));
        gespub->setText(QCoreApplication::translate("geszone", "Gestion Poubelles", nullptr));
        geszone1->setText(QCoreApplication::translate("geszone", "Gestion Zone", nullptr));
    } // retranslateUi

};

namespace Ui {
    class geszone: public Ui_geszone {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GESZONE_H
