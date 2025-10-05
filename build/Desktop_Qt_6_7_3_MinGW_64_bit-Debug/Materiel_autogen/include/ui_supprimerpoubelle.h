/********************************************************************************
** Form generated from reading UI file 'supprimerpoubelle.ui'
**
** Created by: Qt User Interface Compiler version 6.7.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SUPPRIMERPOUBELLE_H
#define UI_SUPPRIMERPOUBELLE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_supprimerpoubelle
{
public:
    QFrame *frame;
    QLabel *label_3;
    QLabel *label_4;
    QPushButton *suppp;
    QPushButton *ret;
    QTableWidget *tableWidget;
    QLabel *label_5;
    QLineEdit *lineEdit;
    QLabel *label;
    QPushButton *gespub;
    QPushButton *geszone;
    QLabel *label_2;

    void setupUi(QWidget *supprimerpoubelle)
    {
        if (supprimerpoubelle->objectName().isEmpty())
            supprimerpoubelle->setObjectName("supprimerpoubelle");
        supprimerpoubelle->resize(1346, 684);
        frame = new QFrame(supprimerpoubelle);
        frame->setObjectName("frame");
        frame->setGeometry(QRect(10, 130, 1331, 461));
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
        suppp = new QPushButton(frame);
        suppp->setObjectName("suppp");
        suppp->setGeometry(QRect(110, 150, 141, 41));
        suppp->setStyleSheet(QString::fromUtf8("QPushButton {\n"
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
        ret = new QPushButton(frame);
        ret->setObjectName("ret");
        ret->setGeometry(QRect(270, 150, 141, 41));
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
        tableWidget->setGeometry(QRect(460, 70, 701, 271));
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
        label_5 = new QLabel(frame);
        label_5->setObjectName("label_5");
        label_5->setGeometry(QRect(50, 80, 51, 41));
        label_5->setStyleSheet(QString::fromUtf8("font: 16pt \"Segoe UI\";\n"
"font: 700 15pt \"Segoe UI\";\n"
"color: rgb(0, 51, 102);"));
        lineEdit = new QLineEdit(frame);
        lineEdit->setObjectName("lineEdit");
        lineEdit->setGeometry(QRect(100, 80, 171, 41));
        lineEdit->setStyleSheet(QString::fromUtf8("/* QLineEdit style */\n"
"QLineEdit {\n"
"    background-color: #E0F7FF;      /* light blue background */\n"
"    border: 1px solid #003366;      /* dark blue border */\n"
"    padding: 6px;                   /* inner spacing */\n"
"    font-size: 14px;\n"
"    color: #003366;\n"
"}\n"
"\n"
"/* QComboBox style */\n"
"QComboBox {\n"
"    background-color: #E0F7FF;\n"
"    border: 1px solid #003366;\n"
"    padding: 6px;\n"
"    font-size: 14px;\n"
"    color: #003366;\n"
"}\n"
"QComboBox QAbstractItemView {\n"
"    background-color: #E0F7FF;\n"
"    selection-background-color: #87CEEB; /* highlight on selection */\n"
"}\n"
"\n"
"/* QPushButton style */\n"
"QPushButton {\n"
"    background-color: #003366;       /* dark blue button */\n"
"    color: white;                     /* text color */\n"
"    border-radius: 6px;\n"
"    padding: 8px 16px;\n"
"    font-size: 14px;\n"
"}\n"
"QPushButton:hover {\n"
"    background-color: #00509e;       /* lighter blue on hover */\n"
"}\n"
"QPushButton:pressed {\n"
"    backg"
                        "round-color: #001f4d;       /* darker blue when pressed */\n"
"}\n"
"\n"
"/* QLabel (optional for titles) */\n"
"QLabel {\n"
"    font-size: 16px;\n"
"    font-weight: bold;\n"
"    color: #003366;\n"
"}\n"
""));
        label = new QLabel(supprimerpoubelle);
        label->setObjectName("label");
        label->setGeometry(QRect(-60, 50, 1461, 61));
        label->setStyleSheet(QString::fromUtf8("background-color:#ADD8E6;"));
        gespub = new QPushButton(supprimerpoubelle);
        gespub->setObjectName("gespub");
        gespub->setGeometry(QRect(860, 60, 171, 41));
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
        geszone = new QPushButton(supprimerpoubelle);
        geszone->setObjectName("geszone");
        geszone->setGeometry(QRect(1070, 60, 171, 41));
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
        label_2 = new QLabel(supprimerpoubelle);
        label_2->setObjectName("label_2");
        label_2->setGeometry(QRect(30, 20, 401, 141));
        label_2->setStyleSheet(QString::fromUtf8("font: 16pt \"Segoe UI\";\n"
"font: 700 16pt \"Segoe UI\";\n"
"color: rgb(0, 51, 102);"));

        retranslateUi(supprimerpoubelle);

        QMetaObject::connectSlotsByName(supprimerpoubelle);
    } // setupUi

    void retranslateUi(QWidget *supprimerpoubelle)
    {
        supprimerpoubelle->setWindowTitle(QCoreApplication::translate("supprimerpoubelle", "Form", nullptr));
        label_3->setText(QCoreApplication::translate("supprimerpoubelle", "Supprimer Poubelle", nullptr));
        label_4->setText(QString());
        suppp->setText(QCoreApplication::translate("supprimerpoubelle", "Supprimer", nullptr));
        ret->setText(QCoreApplication::translate("supprimerpoubelle", "Retour", nullptr));
        QTableWidgetItem *___qtablewidgetitem = tableWidget->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QCoreApplication::translate("supprimerpoubelle", "ID", nullptr));
        QTableWidgetItem *___qtablewidgetitem1 = tableWidget->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QCoreApplication::translate("supprimerpoubelle", "Type Dechet", nullptr));
        QTableWidgetItem *___qtablewidgetitem2 = tableWidget->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QCoreApplication::translate("supprimerpoubelle", "X", nullptr));
        QTableWidgetItem *___qtablewidgetitem3 = tableWidget->horizontalHeaderItem(3);
        ___qtablewidgetitem3->setText(QCoreApplication::translate("supprimerpoubelle", "Y", nullptr));
        QTableWidgetItem *___qtablewidgetitem4 = tableWidget->horizontalHeaderItem(4);
        ___qtablewidgetitem4->setText(QCoreApplication::translate("supprimerpoubelle", "Zone ", nullptr));
        QTableWidgetItem *___qtablewidgetitem5 = tableWidget->horizontalHeaderItem(5);
        ___qtablewidgetitem5->setText(QCoreApplication::translate("supprimerpoubelle", "Capacite", nullptr));
        QTableWidgetItem *___qtablewidgetitem6 = tableWidget->horizontalHeaderItem(6);
        ___qtablewidgetitem6->setText(QCoreApplication::translate("supprimerpoubelle", "Etat", nullptr));
        label_5->setText(QCoreApplication::translate("supprimerpoubelle", "ID : ", nullptr));
        lineEdit->setText(QCoreApplication::translate("supprimerpoubelle", "120124", nullptr));
        label->setText(QString());
        gespub->setText(QCoreApplication::translate("supprimerpoubelle", "Gestion Poubelles", nullptr));
        geszone->setText(QCoreApplication::translate("supprimerpoubelle", "Gestion Zone", nullptr));
        label_2->setText(QCoreApplication::translate("supprimerpoubelle", "Recycling Dashboard\n"
"", nullptr));
    } // retranslateUi

};

namespace Ui {
    class supprimerpoubelle: public Ui_supprimerpoubelle {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SUPPRIMERPOUBELLE_H
