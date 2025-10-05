/********************************************************************************
** Form generated from reading UI file 'ajouterzone.ui'
**
** Created by: Qt User Interface Compiler version 6.7.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_AJOUTERZONE_H
#define UI_AJOUTERZONE_H

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

class Ui_ajouterzone
{
public:
    QFrame *frame;
    QLabel *label_3;
    QLabel *label_4;
    QPushButton *ajzone;
    QPushButton *rzone;
    QLabel *label_5;
    QLabel *label_7;
    QLabel *label_8;
    QLabel *label_9;
    QLabel *label_10;
    QLabel *label_11;
    QLineEdit *lineEdit;
    QLineEdit *lineEdit_2;
    QLineEdit *lineEdit_3;
    QLineEdit *lineEdit_4;
    QLineEdit *lineEdit_5;
    QLineEdit *lineEdit_6;
    QTableWidget *tableWidget;
    QPushButton *rzone_2;
    QPushButton *rzone_3;
    QLabel *label_6;
    QLabel *label;
    QLabel *label_2;
    QPushButton *gespub;
    QPushButton *geszone;

    void setupUi(QWidget *ajouterzone)
    {
        if (ajouterzone->objectName().isEmpty())
            ajouterzone->setObjectName("ajouterzone");
        ajouterzone->resize(1339, 685);
        frame = new QFrame(ajouterzone);
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
        ajzone = new QPushButton(frame);
        ajzone->setObjectName("ajzone");
        ajzone->setGeometry(QRect(30, 410, 141, 41));
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
        rzone = new QPushButton(frame);
        rzone->setObjectName("rzone");
        rzone->setGeometry(QRect(500, 410, 141, 41));
        rzone->setStyleSheet(QString::fromUtf8("QPushButton {\n"
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
        label_5->setGeometry(QRect(30, 60, 71, 41));
        label_5->setStyleSheet(QString::fromUtf8("font: 16pt \"Segoe UI\";\n"
"font: 700 12pt \"Segoe UI\";\n"
"color: rgb(0, 51, 102);"));
        label_7 = new QLabel(frame);
        label_7->setObjectName("label_7");
        label_7->setGeometry(QRect(30, 170, 161, 41));
        label_7->setStyleSheet(QString::fromUtf8("font: 16pt \"Segoe UI\";\n"
"font: 700 12pt \"Segoe UI\";\n"
"color: rgb(0, 51, 102);"));
        label_8 = new QLabel(frame);
        label_8->setObjectName("label_8");
        label_8->setGeometry(QRect(30, 230, 161, 41));
        label_8->setStyleSheet(QString::fromUtf8("font: 16pt \"Segoe UI\";\n"
"font: 700 12pt \"Segoe UI\";\n"
"color: rgb(0, 51, 102);"));
        label_9 = new QLabel(frame);
        label_9->setObjectName("label_9");
        label_9->setGeometry(QRect(20, 110, 161, 41));
        label_9->setStyleSheet(QString::fromUtf8("font: 16pt \"Segoe UI\";\n"
"font: 700 12pt \"Segoe UI\";\n"
"color: rgb(0, 51, 102);"));
        label_10 = new QLabel(frame);
        label_10->setObjectName("label_10");
        label_10->setGeometry(QRect(30, 290, 161, 41));
        label_10->setStyleSheet(QString::fromUtf8("font: 16pt \"Segoe UI\";\n"
"font: 700 12pt \"Segoe UI\";\n"
"color: rgb(0, 51, 102);"));
        label_11 = new QLabel(frame);
        label_11->setObjectName("label_11");
        label_11->setGeometry(QRect(30, 340, 161, 41));
        label_11->setStyleSheet(QString::fromUtf8("font: 16pt \"Segoe UI\";\n"
"font: 700 12pt \"Segoe UI\";\n"
"color: rgb(0, 51, 102);"));
        lineEdit = new QLineEdit(frame);
        lineEdit->setObjectName("lineEdit");
        lineEdit->setGeometry(QRect(120, 70, 171, 31));
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
        lineEdit_2 = new QLineEdit(frame);
        lineEdit_2->setObjectName("lineEdit_2");
        lineEdit_2->setGeometry(QRect(80, 180, 171, 31));
        lineEdit_2->setStyleSheet(QString::fromUtf8("/* QLineEdit style */\n"
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
        lineEdit_3 = new QLineEdit(frame);
        lineEdit_3->setObjectName("lineEdit_3");
        lineEdit_3->setGeometry(QRect(80, 240, 171, 31));
        lineEdit_3->setStyleSheet(QString::fromUtf8("/* QLineEdit style */\n"
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
        lineEdit_4 = new QLineEdit(frame);
        lineEdit_4->setObjectName("lineEdit_4");
        lineEdit_4->setGeometry(QRect(130, 120, 171, 31));
        lineEdit_4->setStyleSheet(QString::fromUtf8("/* QLineEdit style */\n"
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
        lineEdit_5 = new QLineEdit(frame);
        lineEdit_5->setObjectName("lineEdit_5");
        lineEdit_5->setGeometry(QRect(80, 290, 171, 31));
        lineEdit_5->setStyleSheet(QString::fromUtf8("/* QLineEdit style */\n"
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
        lineEdit_6 = new QLineEdit(frame);
        lineEdit_6->setObjectName("lineEdit_6");
        lineEdit_6->setGeometry(QRect(80, 350, 171, 31));
        lineEdit_6->setStyleSheet(QString::fromUtf8("/* QLineEdit style */\n"
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
        tableWidget = new QTableWidget(frame);
        if (tableWidget->columnCount() < 9)
            tableWidget->setColumnCount(9);
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
        QTableWidgetItem *__qtablewidgetitem7 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(7, __qtablewidgetitem7);
        QTableWidgetItem *__qtablewidgetitem8 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(8, __qtablewidgetitem8);
        tableWidget->setObjectName("tableWidget");
        tableWidget->setGeometry(QRect(380, 100, 901, 211));
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
        rzone_2 = new QPushButton(frame);
        rzone_2->setObjectName("rzone_2");
        rzone_2->setGeometry(QRect(340, 410, 141, 41));
        rzone_2->setStyleSheet(QString::fromUtf8("QPushButton {\n"
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
        rzone_3 = new QPushButton(frame);
        rzone_3->setObjectName("rzone_3");
        rzone_3->setGeometry(QRect(190, 410, 141, 41));
        rzone_3->setStyleSheet(QString::fromUtf8("QPushButton {\n"
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
        label_6 = new QLabel(frame);
        label_6->setObjectName("label_6");
        label_6->setGeometry(QRect(1120, 60, 221, 41));
        label_6->setStyleSheet(QString::fromUtf8("font: 16pt \"Segoe UI\";\n"
"font: 700 6pt \"Segoe UI\";\n"
"color: rgb(0, 51, 102);"));
        label = new QLabel(ajouterzone);
        label->setObjectName("label");
        label->setGeometry(QRect(-60, 50, 1461, 61));
        label->setStyleSheet(QString::fromUtf8("background-color:#ADD8E6;"));
        label_2 = new QLabel(ajouterzone);
        label_2->setObjectName("label_2");
        label_2->setGeometry(QRect(20, 20, 401, 141));
        label_2->setStyleSheet(QString::fromUtf8("font: 16pt \"Segoe UI\";\n"
"font: 700 16pt \"Segoe UI\";\n"
"color: rgb(0, 51, 102);"));
        gespub = new QPushButton(ajouterzone);
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
        geszone = new QPushButton(ajouterzone);
        geszone->setObjectName("geszone");
        geszone->setGeometry(QRect(1100, 60, 171, 41));
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

        retranslateUi(ajouterzone);

        QMetaObject::connectSlotsByName(ajouterzone);
    } // setupUi

    void retranslateUi(QWidget *ajouterzone)
    {
        ajouterzone->setWindowTitle(QCoreApplication::translate("ajouterzone", "Form", nullptr));
        label_3->setText(QCoreApplication::translate("ajouterzone", "Ajouter Zone", nullptr));
        label_4->setText(QString());
        ajzone->setText(QCoreApplication::translate("ajouterzone", "Ajouter", nullptr));
        rzone->setText(QCoreApplication::translate("ajouterzone", "Retour", nullptr));
        label_5->setText(QCoreApplication::translate("ajouterzone", "ID_ Zone : ", nullptr));
        label_7->setText(QCoreApplication::translate("ajouterzone", "X : ", nullptr));
        label_8->setText(QCoreApplication::translate("ajouterzone", "Y : ", nullptr));
        label_9->setText(QCoreApplication::translate("ajouterzone", "Population  : ", nullptr));
        label_10->setText(QCoreApplication::translate("ajouterzone", "H  : ", nullptr));
        label_11->setText(QCoreApplication::translate("ajouterzone", "L : ", nullptr));
        lineEdit->setText(QCoreApplication::translate("ajouterzone", "120124", nullptr));
        lineEdit_2->setText(QCoreApplication::translate("ajouterzone", "Ex : 21.152", nullptr));
        lineEdit_3->setText(QCoreApplication::translate("ajouterzone", "Ex : 37.0215", nullptr));
        lineEdit_4->setText(QCoreApplication::translate("ajouterzone", "Ex : 66 p", nullptr));
        lineEdit_5->setText(QCoreApplication::translate("ajouterzone", "Ex :120", nullptr));
        lineEdit_6->setText(QCoreApplication::translate("ajouterzone", "Ex : 240", nullptr));
        QTableWidgetItem *___qtablewidgetitem = tableWidget->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QCoreApplication::translate("ajouterzone", "ID", nullptr));
        QTableWidgetItem *___qtablewidgetitem1 = tableWidget->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QCoreApplication::translate("ajouterzone", "Population", nullptr));
        QTableWidgetItem *___qtablewidgetitem2 = tableWidget->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QCoreApplication::translate("ajouterzone", "n poubelles", nullptr));
        QTableWidgetItem *___qtablewidgetitem3 = tableWidget->horizontalHeaderItem(3);
        ___qtablewidgetitem3->setText(QCoreApplication::translate("ajouterzone", "X", nullptr));
        QTableWidgetItem *___qtablewidgetitem4 = tableWidget->horizontalHeaderItem(4);
        ___qtablewidgetitem4->setText(QCoreApplication::translate("ajouterzone", "Y", nullptr));
        QTableWidgetItem *___qtablewidgetitem5 = tableWidget->horizontalHeaderItem(5);
        ___qtablewidgetitem5->setText(QCoreApplication::translate("ajouterzone", "H", nullptr));
        QTableWidgetItem *___qtablewidgetitem6 = tableWidget->horizontalHeaderItem(6);
        ___qtablewidgetitem6->setText(QCoreApplication::translate("ajouterzone", "L", nullptr));
        QTableWidgetItem *___qtablewidgetitem7 = tableWidget->horizontalHeaderItem(7);
        ___qtablewidgetitem7->setText(QCoreApplication::translate("ajouterzone", "	Densit\303\251", nullptr));
        QTableWidgetItem *___qtablewidgetitem8 = tableWidget->horizontalHeaderItem(8);
        ___qtablewidgetitem8->setText(QCoreApplication::translate("ajouterzone", "Surface", nullptr));
        rzone_2->setText(QCoreApplication::translate("ajouterzone", "Supprimer", nullptr));
        rzone_3->setText(QCoreApplication::translate("ajouterzone", "Modifier", nullptr));
        label_6->setText(QCoreApplication::translate("ajouterzone", "Surface (m\302\262)	Densit\303\251 (p/m\302\262)", nullptr));
        label->setText(QString());
        label_2->setText(QCoreApplication::translate("ajouterzone", "Recycling Dashboard\n"
"", nullptr));
        gespub->setText(QCoreApplication::translate("ajouterzone", "Gestion Poubelles", nullptr));
        geszone->setText(QCoreApplication::translate("ajouterzone", "Gestion Zone", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ajouterzone: public Ui_ajouterzone {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_AJOUTERZONE_H
