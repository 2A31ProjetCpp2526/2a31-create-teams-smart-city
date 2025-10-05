/********************************************************************************
** Form generated from reading UI file 'ajouterpoubelle.ui'
**
** Created by: Qt User Interface Compiler version 6.7.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_AJOUTERPOUBELLE_H
#define UI_AJOUTERPOUBELLE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ajouterpoubelle
{
public:
    QFrame *frame;
    QLabel *label_3;
    QLabel *label_4;
    QPushButton *ajoutpub;
    QPushButton *retourpub;
    QTableWidget *tableWidget;
    QLabel *label_5;
    QLabel *label_6;
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
    QComboBox *comboBox;
    QComboBox *comboBox_2;
    QPushButton *retourpub_2;
    QLabel *label;
    QLabel *label_2;
    QPushButton *geszone;
    QPushButton *gespub;

    void setupUi(QWidget *ajouterpoubelle)
    {
        if (ajouterpoubelle->objectName().isEmpty())
            ajouterpoubelle->setObjectName("ajouterpoubelle");
        ajouterpoubelle->resize(1342, 687);
        frame = new QFrame(ajouterpoubelle);
        frame->setObjectName("frame");
        frame->setGeometry(QRect(20, 130, 1331, 461));
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
        ajoutpub = new QPushButton(frame);
        ajoutpub->setObjectName("ajoutpub");
        ajoutpub->setGeometry(QRect(310, 410, 141, 41));
        ajoutpub->setStyleSheet(QString::fromUtf8("QPushButton {\n"
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
        retourpub = new QPushButton(frame);
        retourpub->setObjectName("retourpub");
        retourpub->setGeometry(QRect(460, 410, 141, 41));
        retourpub->setStyleSheet(QString::fromUtf8("QPushButton {\n"
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
        tableWidget->setGeometry(QRect(580, 130, 701, 211));
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
        label_5->setGeometry(QRect(30, 50, 51, 41));
        label_5->setStyleSheet(QString::fromUtf8("font: 16pt \"Segoe UI\";\n"
"font: 700 12pt \"Segoe UI\";\n"
"color: rgb(0, 51, 102);"));
        label_6 = new QLabel(frame);
        label_6->setObjectName("label_6");
        label_6->setGeometry(QRect(30, 100, 161, 41));
        label_6->setStyleSheet(QString::fromUtf8("font: 16pt \"Segoe UI\";\n"
"font: 700 12pt \"Segoe UI\";\n"
"color: rgb(0, 51, 102);"));
        label_7 = new QLabel(frame);
        label_7->setObjectName("label_7");
        label_7->setGeometry(QRect(30, 160, 161, 41));
        label_7->setStyleSheet(QString::fromUtf8("font: 16pt \"Segoe UI\";\n"
"font: 700 12pt \"Segoe UI\";\n"
"color: rgb(0, 51, 102);"));
        label_8 = new QLabel(frame);
        label_8->setObjectName("label_8");
        label_8->setGeometry(QRect(30, 220, 161, 41));
        label_8->setStyleSheet(QString::fromUtf8("font: 16pt \"Segoe UI\";\n"
"font: 700 12pt \"Segoe UI\";\n"
"color: rgb(0, 51, 102);"));
        label_9 = new QLabel(frame);
        label_9->setObjectName("label_9");
        label_9->setGeometry(QRect(30, 270, 161, 41));
        label_9->setStyleSheet(QString::fromUtf8("font: 16pt \"Segoe UI\";\n"
"font: 700 12pt \"Segoe UI\";\n"
"color: rgb(0, 51, 102);"));
        label_10 = new QLabel(frame);
        label_10->setObjectName("label_10");
        label_10->setGeometry(QRect(30, 320, 161, 41));
        label_10->setStyleSheet(QString::fromUtf8("font: 16pt \"Segoe UI\";\n"
"font: 700 12pt \"Segoe UI\";\n"
"color: rgb(0, 51, 102);"));
        label_11 = new QLabel(frame);
        label_11->setObjectName("label_11");
        label_11->setGeometry(QRect(30, 380, 161, 41));
        label_11->setStyleSheet(QString::fromUtf8("font: 16pt \"Segoe UI\";\n"
"font: 700 12pt \"Segoe UI\";\n"
"color: rgb(0, 51, 102);"));
        lineEdit = new QLineEdit(frame);
        lineEdit->setObjectName("lineEdit");
        lineEdit->setGeometry(QRect(90, 60, 171, 31));
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
        lineEdit_2->setGeometry(QRect(90, 170, 171, 31));
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
        lineEdit_3->setGeometry(QRect(90, 230, 171, 31));
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
        lineEdit_4->setGeometry(QRect(100, 280, 171, 31));
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
        lineEdit_5->setGeometry(QRect(130, 330, 171, 31));
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
        comboBox = new QComboBox(frame);
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->setObjectName("comboBox");
        comboBox->setGeometry(QRect(180, 110, 161, 31));
        comboBox->setStyleSheet(QString::fromUtf8("/* QLineEdit style */\n"
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
        comboBox_2 = new QComboBox(frame);
        comboBox_2->addItem(QString());
        comboBox_2->addItem(QString());
        comboBox_2->addItem(QString());
        comboBox_2->setObjectName("comboBox_2");
        comboBox_2->setGeometry(QRect(90, 390, 161, 31));
        comboBox_2->setStyleSheet(QString::fromUtf8("/* QLineEdit style */\n"
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
        retourpub_2 = new QPushButton(frame);
        retourpub_2->setObjectName("retourpub_2");
        retourpub_2->setGeometry(QRect(1130, 350, 141, 41));
        retourpub_2->setStyleSheet(QString::fromUtf8("QPushButton {\n"
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
        label = new QLabel(ajouterpoubelle);
        label->setObjectName("label");
        label->setGeometry(QRect(-60, 20, 1461, 61));
        label->setStyleSheet(QString::fromUtf8("background-color:#ADD8E6;"));
        label_2 = new QLabel(ajouterpoubelle);
        label_2->setObjectName("label_2");
        label_2->setGeometry(QRect(30, -10, 401, 141));
        label_2->setStyleSheet(QString::fromUtf8("font: 16pt \"Segoe UI\";\n"
"font: 700 16pt \"Segoe UI\";\n"
"color: rgb(0, 51, 102);"));
        geszone = new QPushButton(ajouterpoubelle);
        geszone->setObjectName("geszone");
        geszone->setGeometry(QRect(1100, 30, 171, 41));
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
        gespub = new QPushButton(ajouterpoubelle);
        gespub->setObjectName("gespub");
        gespub->setGeometry(QRect(890, 30, 171, 41));
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

        retranslateUi(ajouterpoubelle);

        QMetaObject::connectSlotsByName(ajouterpoubelle);
    } // setupUi

    void retranslateUi(QWidget *ajouterpoubelle)
    {
        ajouterpoubelle->setWindowTitle(QCoreApplication::translate("ajouterpoubelle", "Form", nullptr));
        label_3->setText(QCoreApplication::translate("ajouterpoubelle", "Ajouter Poubelle", nullptr));
        label_4->setText(QString());
        ajoutpub->setText(QCoreApplication::translate("ajouterpoubelle", "Ajouter", nullptr));
        retourpub->setText(QCoreApplication::translate("ajouterpoubelle", "Retour", nullptr));
        QTableWidgetItem *___qtablewidgetitem = tableWidget->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QCoreApplication::translate("ajouterpoubelle", "ID", nullptr));
        QTableWidgetItem *___qtablewidgetitem1 = tableWidget->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QCoreApplication::translate("ajouterpoubelle", "Type Dechet", nullptr));
        QTableWidgetItem *___qtablewidgetitem2 = tableWidget->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QCoreApplication::translate("ajouterpoubelle", "X", nullptr));
        QTableWidgetItem *___qtablewidgetitem3 = tableWidget->horizontalHeaderItem(3);
        ___qtablewidgetitem3->setText(QCoreApplication::translate("ajouterpoubelle", "Y", nullptr));
        QTableWidgetItem *___qtablewidgetitem4 = tableWidget->horizontalHeaderItem(4);
        ___qtablewidgetitem4->setText(QCoreApplication::translate("ajouterpoubelle", "Zone ", nullptr));
        QTableWidgetItem *___qtablewidgetitem5 = tableWidget->horizontalHeaderItem(5);
        ___qtablewidgetitem5->setText(QCoreApplication::translate("ajouterpoubelle", "Capacite", nullptr));
        QTableWidgetItem *___qtablewidgetitem6 = tableWidget->horizontalHeaderItem(6);
        ___qtablewidgetitem6->setText(QCoreApplication::translate("ajouterpoubelle", "Etat", nullptr));
        label_5->setText(QCoreApplication::translate("ajouterpoubelle", "ID : ", nullptr));
        label_6->setText(QCoreApplication::translate("ajouterpoubelle", "Type de Dechet :", nullptr));
        label_7->setText(QCoreApplication::translate("ajouterpoubelle", "X : ", nullptr));
        label_8->setText(QCoreApplication::translate("ajouterpoubelle", "Y : ", nullptr));
        label_9->setText(QCoreApplication::translate("ajouterpoubelle", "Zone : ", nullptr));
        label_10->setText(QCoreApplication::translate("ajouterpoubelle", "Capacit\303\251 : ", nullptr));
        label_11->setText(QCoreApplication::translate("ajouterpoubelle", "Etat : ", nullptr));
        lineEdit->setText(QCoreApplication::translate("ajouterpoubelle", "120124", nullptr));
        lineEdit_2->setText(QCoreApplication::translate("ajouterpoubelle", "Ex : 21.152", nullptr));
        lineEdit_3->setText(QCoreApplication::translate("ajouterpoubelle", "Ex : 37.0215", nullptr));
        lineEdit_4->setText(QCoreApplication::translate("ajouterpoubelle", "Ex : A", nullptr));
        lineEdit_5->setText(QCoreApplication::translate("ajouterpoubelle", "Ex :50 L", nullptr));
        comboBox->setItemText(0, QCoreApplication::translate("ajouterpoubelle", "Plastique", nullptr));
        comboBox->setItemText(1, QCoreApplication::translate("ajouterpoubelle", "Papier", nullptr));
        comboBox->setItemText(2, QCoreApplication::translate("ajouterpoubelle", "Verre", nullptr));

        comboBox_2->setItemText(0, QCoreApplication::translate("ajouterpoubelle", "Vide", nullptr));
        comboBox_2->setItemText(1, QCoreApplication::translate("ajouterpoubelle", "Plein", nullptr));
        comboBox_2->setItemText(2, QCoreApplication::translate("ajouterpoubelle", "Maintenance", nullptr));

        retourpub_2->setText(QCoreApplication::translate("ajouterpoubelle", "TRI ASC", nullptr));
        label->setText(QString());
        label_2->setText(QCoreApplication::translate("ajouterpoubelle", "Recycling Dashboard\n"
"", nullptr));
        geszone->setText(QCoreApplication::translate("ajouterpoubelle", "Gestion Zone", nullptr));
        gespub->setText(QCoreApplication::translate("ajouterpoubelle", "Gestion Poubelles", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ajouterpoubelle: public Ui_ajouterpoubelle {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_AJOUTERPOUBELLE_H
