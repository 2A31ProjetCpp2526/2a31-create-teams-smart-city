/********************************************************************************
** Form generated from reading UI file 'exporterpoubelle.ui'
**
** Created by: Qt User Interface Compiler version 6.7.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_EXPORTERPOUBELLE_H
#define UI_EXPORTERPOUBELLE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_exporterpoubelle
{
public:
    QLabel *label;
    QPushButton *gespub;
    QLabel *label_2;
    QPushButton *geszone;
    QFrame *frame;
    QLabel *label_3;
    QLabel *label_4;
    QPushButton *pdf;
    QPushButton *excel;
    QLabel *label_5;
    QTableWidget *tableWidget;
    QPushButton *ret;

    void setupUi(QWidget *exporterpoubelle)
    {
        if (exporterpoubelle->objectName().isEmpty())
            exporterpoubelle->setObjectName("exporterpoubelle");
        exporterpoubelle->resize(1340, 681);
        label = new QLabel(exporterpoubelle);
        label->setObjectName("label");
        label->setGeometry(QRect(-60, 60, 1461, 61));
        label->setStyleSheet(QString::fromUtf8("background-color:#ADD8E6;"));
        gespub = new QPushButton(exporterpoubelle);
        gespub->setObjectName("gespub");
        gespub->setGeometry(QRect(930, 70, 171, 41));
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
        label_2 = new QLabel(exporterpoubelle);
        label_2->setObjectName("label_2");
        label_2->setGeometry(QRect(20, 30, 401, 141));
        label_2->setStyleSheet(QString::fromUtf8("font: 16pt \"Segoe UI\";\n"
"font: 700 16pt \"Segoe UI\";\n"
"color: rgb(0, 51, 102);"));
        geszone = new QPushButton(exporterpoubelle);
        geszone->setObjectName("geszone");
        geszone->setGeometry(QRect(1110, 70, 171, 41));
        geszone->setStyleSheet(QString::fromUtf8("QPushButton {\n"
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
        frame = new QFrame(exporterpoubelle);
        frame->setObjectName("frame");
        frame->setGeometry(QRect(20, 140, 1311, 461));
        frame->setStyleSheet(QString::fromUtf8("background-color: rgb(180, 180, 180);\n"
"background-color: rgb(206, 206, 206);\n"
""));
        frame->setFrameShape(QFrame::Shape::NoFrame);
        label_3 = new QLabel(frame);
        label_3->setObjectName("label_3");
        label_3->setGeometry(QRect(10, 10, 221, 41));
        label_3->setStyleSheet(QString::fromUtf8("font: 16pt \"Segoe UI\";\n"
"font: 700 16pt \"Segoe UI\";\n"
"color: rgb(0, 51, 102);"));
        label_4 = new QLabel(frame);
        label_4->setObjectName("label_4");
        label_4->setGeometry(QRect(110, 70, 171, 51));
        label_4->setStyleSheet(QString::fromUtf8("font: 16pt \"Segoe UI\";\n"
"font: 700 16pt \"Segoe UI\";\n"
"color: rgb(0, 51, 102);"));
        pdf = new QPushButton(frame);
        pdf->setObjectName("pdf");
        pdf->setGeometry(QRect(100, 170, 141, 41));
        pdf->setStyleSheet(QString::fromUtf8("QPushButton {\n"
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
        excel = new QPushButton(frame);
        excel->setObjectName("excel");
        excel->setGeometry(QRect(290, 170, 141, 41));
        excel->setStyleSheet(QString::fromUtf8("QPushButton {\n"
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
        label_5 = new QLabel(frame);
        label_5->setObjectName("label_5");
        label_5->setGeometry(QRect(40, 100, 371, 51));
        label_5->setStyleSheet(QString::fromUtf8("font: 16pt \"Segoe UI\";\n"
"font: 700 10pt \"Segoe UI\";\n"
"color: rgb(0, 51, 102);"));
        tableWidget = new QTableWidget(frame);
        if (tableWidget->columnCount() < 4)
            tableWidget->setColumnCount(4);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(3, __qtablewidgetitem3);
        tableWidget->setObjectName("tableWidget");
        tableWidget->setGeometry(QRect(620, 120, 401, 251));
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
        ret = new QPushButton(frame);
        ret->setObjectName("ret");
        ret->setGeometry(QRect(60, 410, 141, 41));
        ret->setStyleSheet(QString::fromUtf8("QPushButton {\n"
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

        retranslateUi(exporterpoubelle);

        QMetaObject::connectSlotsByName(exporterpoubelle);
    } // setupUi

    void retranslateUi(QWidget *exporterpoubelle)
    {
        exporterpoubelle->setWindowTitle(QCoreApplication::translate("exporterpoubelle", "Form", nullptr));
        label->setText(QString());
        gespub->setText(QCoreApplication::translate("exporterpoubelle", "Gestion Poubelles", nullptr));
        label_2->setText(QCoreApplication::translate("exporterpoubelle", "Recycling Dashboard\n"
"", nullptr));
        geszone->setText(QCoreApplication::translate("exporterpoubelle", "Gestion Zone", nullptr));
        label_3->setText(QCoreApplication::translate("exporterpoubelle", "Exporter poubelle", nullptr));
        label_4->setText(QString());
        pdf->setText(QCoreApplication::translate("exporterpoubelle", "Exporter PDF", nullptr));
        excel->setText(QCoreApplication::translate("exporterpoubelle", "Exporter Excel", nullptr));
        label_5->setText(QCoreApplication::translate("exporterpoubelle", "Exporter la liste des poubelles au format PDF ou Excel :\n"
"\n"
"", nullptr));
        QTableWidgetItem *___qtablewidgetitem = tableWidget->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QCoreApplication::translate("exporterpoubelle", "ID", nullptr));
        QTableWidgetItem *___qtablewidgetitem1 = tableWidget->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QCoreApplication::translate("exporterpoubelle", "Type Dechet", nullptr));
        QTableWidgetItem *___qtablewidgetitem2 = tableWidget->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QCoreApplication::translate("exporterpoubelle", "Zone ", nullptr));
        QTableWidgetItem *___qtablewidgetitem3 = tableWidget->horizontalHeaderItem(3);
        ___qtablewidgetitem3->setText(QCoreApplication::translate("exporterpoubelle", "Etat", nullptr));
        ret->setText(QCoreApplication::translate("exporterpoubelle", "Retour", nullptr));
    } // retranslateUi

};

namespace Ui {
    class exporterpoubelle: public Ui_exporterpoubelle {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_EXPORTERPOUBELLE_H
