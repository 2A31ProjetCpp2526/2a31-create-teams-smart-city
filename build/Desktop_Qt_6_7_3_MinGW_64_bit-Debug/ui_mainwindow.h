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
#include <QtGui/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTableView>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *action_Green_Zone_Manager;
    QAction *action_Green_Zone_Manager_2;
    QWidget *centralwidget;
    QTableView *tableView_2;
    QStackedWidget *stackedWidget_2;
    QWidget *page_3;
    QLabel *label_4;
    QComboBox *comboBox_trieH;
    QPushButton *pushButton_trieH;
    QWidget *horizontalLayoutWidget_5;
    QHBoxLayout *horizontalLayout_5;
    QPushButton *pushButton_modifH;
    QPushButton *pushButton_suppH;
    QPushButton *pushButton_ajoutH;
    QLabel *label_6;
    QLabel *label_11;
    QComboBox *comboBox_5;
    QLabel *label_19;
    QComboBox *comboBox_7;
    QLabel *label_31;
    QLabel *label_32;
    QLabel *label_33;
    QLineEdit *lineEdit_12;
    QLineEdit *lineEdit_13;
    QPushButton *pushButton_17;
    QGroupBox *groupBox;
    QLineEdit *lineEdit_nom;
    QLineEdit *lineEdit_prenom;
    QLineEdit *lineEdit_id;
    QLineEdit *lineEdit_adresse;
    QLineEdit *lineEdit_zone;
    QLineEdit *lineEdit_situation;
    QLineEdit *lineEdit_contact;
    QLineEdit *lineEdit_statut;
    QLineEdit *lineEdit_besoins;
    QLineEdit *lineEdit_satisfaction;
    QLabel *label_144;
    QLabel *label_145;
    QLabel *label_146;
    QLabel *label_147;
    QLabel *label_148;
    QLabel *label_149;
    QLabel *label_150;
    QLabel *label_151;
    QLabel *label_152;
    QLabel *label_153;
    QLabel *label_154;
    QTableView *tableView_habitants;
    QPushButton *on_pushButton_13;
    QLineEdit *lineEdit_rechH;
    QPushButton *on_pushButton_15;
    QPushButton *on_pushButton_14;
    QPushButton *pushButton_9;
    QPushButton *pushButton_rechH;
    QWidget *page_4;
    QTableView *tableView_4;
    QLabel *label_9;
    QLineEdit *lineEdit_7;
    QLabel *label_20;
    QLineEdit *lineEdit_8;
    QLabel *label_34;
    QComboBox *comboBox_12;
    QPushButton *pushButton_10;
    QProgressBar *progressBar_6;
    QWidget *page_8;
    QLabel *label_36;
    QLineEdit *lineEdit_4;
    QLabel *label_37;
    QComboBox *comboBox_14;
    QTableView *tableView_5;
    QLabel *label_38;
    QLabel *label_39;
    QLabel *label_40;
    QLabel *label_41;
    QLabel *label_42;
    QLabel *label_43;
    QLabel *label_44;
    QLabel *label_45;
    QStackedWidget *stackedWidget_4;
    QWidget *page_9;
    QWidget *page_10;
    QProgressBar *progressBar_7;
    QProgressBar *progressBar_8;
    QPushButton *Ghabitats_2;
    QPushButton *Ghabitant;
    QPushButton *Ghabitats_3;
    QPushButton *Ghabitats_4;
    QPushButton *Ghabitats_5;
    QPushButton *Ghabitats_6;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(1546, 600);
        action_Green_Zone_Manager = new QAction(MainWindow);
        action_Green_Zone_Manager->setObjectName("action_Green_Zone_Manager");
        QIcon icon(QIcon::fromTheme(QIcon::ThemeIcon::DocumentProperties));
        action_Green_Zone_Manager->setIcon(icon);
        action_Green_Zone_Manager->setMenuRole(QAction::MenuRole::TextHeuristicRole);
        action_Green_Zone_Manager_2 = new QAction(MainWindow);
        action_Green_Zone_Manager_2->setObjectName("action_Green_Zone_Manager_2");
        action_Green_Zone_Manager_2->setIcon(icon);
        action_Green_Zone_Manager_2->setMenuRole(QAction::MenuRole::TextHeuristicRole);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        tableView_2 = new QTableView(centralwidget);
        tableView_2->setObjectName("tableView_2");
        tableView_2->setGeometry(QRect(10, -10, 231, 1641));
        tableView_2->setStyleSheet(QString::fromUtf8("\n"
"    background-color: #001f3f;            /* Deep navy blue */\n"
"    border-top-right-radius: 20px;\n"
"    border-bottom-right-radius: 20px;\n"
"    padding: 20px 10px;\n"
"    border: none;\n"
"    color: #ffffff;\n"
"    font-size: 15px;\n"
"    font-weight: 600;\n"
"    padding: 5px;"));
        stackedWidget_2 = new QStackedWidget(centralwidget);
        stackedWidget_2->setObjectName("stackedWidget_2");
        stackedWidget_2->setGeometry(QRect(260, 0, 1241, 651));
        stackedWidget_2->setStyleSheet(QString::fromUtf8("    background-color: #f4f7fb;      /* soft gray-blue tone */\n"
"    font-family: \"Segoe UI\", \"Roboto\", sans-serif;\n"
"    color: #0a1a2f;\n"
""));
        page_3 = new QWidget();
        page_3->setObjectName("page_3");
        label_4 = new QLabel(page_3);
        label_4->setObjectName("label_4");
        label_4->setGeometry(QRect(10, 500, 121, 31));
        label_4->setStyleSheet(QString::fromUtf8("color: rgb(0, 0, 0);"));
        comboBox_trieH = new QComboBox(page_3);
        comboBox_trieH->addItem(QString());
        comboBox_trieH->addItem(QString());
        comboBox_trieH->setObjectName("comboBox_trieH");
        comboBox_trieH->setGeometry(QRect(10, 540, 76, 26));
        comboBox_trieH->setStyleSheet(QString::fromUtf8("\n"
"    border: 1.5px solid #0078d7;\n"
"    background-color: #ffffff;\n"
"\n"
"    background-color: #0c2d57;\n"
"    color: white;\n"
"    border-radius: 6px;\n"
"    padding: 6px 14px;\n"
"    font-size: 12px;\n"
"    border: none;"));
        pushButton_trieH = new QPushButton(page_3);
        pushButton_trieH->setObjectName("pushButton_trieH");
        pushButton_trieH->setGeometry(QRect(90, 540, 93, 29));
        pushButton_trieH->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: #012a59;\n"
"    color: #ffffff;\n"
"    border: 1px solid #013366;\n"
"    border-radius: 8px;\n"
"    padding: 6px 14px;\n"
"    font-size: 13px;\n"
"    font-weight: 500;\n"
"}\n"
"\n"
"QPushButton#actionButton:hover {\n"
"    background-color: #024b8a;\n"
"    border-color: #025aa5;\n"
"}\n"
"\n"
"QPushButton#actionButton:pressed {\n"
"    background-color: #013869;\n"
"}\n"
""));
        horizontalLayoutWidget_5 = new QWidget(page_3);
        horizontalLayoutWidget_5->setObjectName("horizontalLayoutWidget_5");
        horizontalLayoutWidget_5->setGeometry(QRect(190, 510, 338, 80));
        horizontalLayout_5 = new QHBoxLayout(horizontalLayoutWidget_5);
        horizontalLayout_5->setObjectName("horizontalLayout_5");
        horizontalLayout_5->setContentsMargins(0, 0, 0, 0);
        pushButton_modifH = new QPushButton(horizontalLayoutWidget_5);
        pushButton_modifH->setObjectName("pushButton_modifH");
        pushButton_modifH->setStyleSheet(QString::fromUtf8("\n"
"QLineEdit:focus {\n"
"    border: 1.5px solid #0078d7;\n"
"    background-color: #ffffff;\n"
"}\n"
"\n"
"QPushButton {\n"
"    background-color: #0c2d57;\n"
"    color: white;\n"
"    border-radius: 6px;\n"
"    padding: 6px 14px;\n"
"    font-size: 12px;\n"
"    border: none;\n"
"}\n"
"\n"
"QPushButton#searchButton:hover {\n"
"    background-color: #1458a6;\n"
"}"));

        horizontalLayout_5->addWidget(pushButton_modifH);

        pushButton_suppH = new QPushButton(horizontalLayoutWidget_5);
        pushButton_suppH->setObjectName("pushButton_suppH");
        pushButton_suppH->setStyleSheet(QString::fromUtf8("\n"
"QLineEdit:focus {\n"
"    border: 1.5px solid #0078d7;\n"
"    background-color: #ffffff;\n"
"}\n"
"\n"
"QPushButton {\n"
"    background-color: #0c2d57;\n"
"    color: white;\n"
"    border-radius: 6px;\n"
"    padding: 6px 14px;\n"
"    font-size: 12px;\n"
"    border: none;\n"
"}\n"
"\n"
"QPushButton#searchButton:hover {\n"
"    background-color: #1458a6;\n"
"}"));

        horizontalLayout_5->addWidget(pushButton_suppH);

        pushButton_ajoutH = new QPushButton(horizontalLayoutWidget_5);
        pushButton_ajoutH->setObjectName("pushButton_ajoutH");
        pushButton_ajoutH->setStyleSheet(QString::fromUtf8("\n"
"QLineEdit:focus {\n"
"    border: 1.5px solid #0078d7;\n"
"    background-color: #ffffff;\n"
"}\n"
"\n"
"QPushButton {\n"
"    background-color: #0c2d57;\n"
"    color: white;\n"
"    border-radius: 6px;\n"
"    padding: 6px 14px;\n"
"    font-size: 12px;\n"
"    border: none;\n"
"}\n"
"\n"
"QPushButton#searchButton:hover {\n"
"    background-color: #1458a6;\n"
"}"));

        horizontalLayout_5->addWidget(pushButton_ajoutH);

        label_6 = new QLabel(page_3);
        label_6->setObjectName("label_6");
        label_6->setGeometry(QRect(530, 490, 101, 20));
        label_6->setStyleSheet(QString::fromUtf8("color: rgb(0, 0, 0);"));
        label_11 = new QLabel(page_3);
        label_11->setObjectName("label_11");
        label_11->setGeometry(QRect(560, 520, 91, 20));
        label_11->setStyleSheet(QString::fromUtf8("color: rgb(0, 0, 0);"));
        comboBox_5 = new QComboBox(page_3);
        comboBox_5->addItem(QString());
        comboBox_5->addItem(QString());
        comboBox_5->addItem(QString());
        comboBox_5->setObjectName("comboBox_5");
        comboBox_5->setGeometry(QRect(650, 520, 81, 31));
        comboBox_5->setStyleSheet(QString::fromUtf8("\n"
"    border: 1.5px solid #0078d7;\n"
"    background-color: #ffffff;\n"
"\n"
"    background-color: #0c2d57;\n"
"    color: white;\n"
"    border-radius: 6px;\n"
"    padding: 6px 14px;\n"
"    font-size: 12px;\n"
"    border: none;"));
        label_19 = new QLabel(page_3);
        label_19->setObjectName("label_19");
        label_19->setGeometry(QRect(560, 560, 81, 20));
        label_19->setStyleSheet(QString::fromUtf8("color: rgb(0, 0, 0);"));
        comboBox_7 = new QComboBox(page_3);
        comboBox_7->addItem(QString());
        comboBox_7->addItem(QString());
        comboBox_7->setObjectName("comboBox_7");
        comboBox_7->setGeometry(QRect(640, 560, 101, 31));
        comboBox_7->setStyleSheet(QString::fromUtf8("\n"
"    border: 1.5px solid #0078d7;\n"
"    background-color: #ffffff;\n"
"\n"
"    background-color: #0c2d57;\n"
"    color: white;\n"
"    border-radius: 6px;\n"
"    padding: 6px 14px;\n"
"    font-size: 12px;\n"
"    border: none;"));
        label_31 = new QLabel(page_3);
        label_31->setObjectName("label_31");
        label_31->setGeometry(QRect(730, 490, 181, 21));
        label_31->setStyleSheet(QString::fromUtf8("color: rgb(0, 0, 0);"));
        label_32 = new QLabel(page_3);
        label_32->setObjectName("label_32");
        label_32->setGeometry(QRect(790, 530, 141, 31));
        label_32->setStyleSheet(QString::fromUtf8("color: rgb(0, 0, 0);"));
        label_33 = new QLabel(page_3);
        label_33->setObjectName("label_33");
        label_33->setGeometry(QRect(800, 570, 131, 21));
        label_33->setStyleSheet(QString::fromUtf8("color: rgb(0, 0, 0);"));
        lineEdit_12 = new QLineEdit(page_3);
        lineEdit_12->setObjectName("lineEdit_12");
        lineEdit_12->setGeometry(QRect(930, 530, 113, 26));
        lineEdit_12->setStyleSheet(QString::fromUtf8("QLineEdit {\n"
"    background-color: #ffffff;\n"
"    border: 1.5px solid #cfd9e6;\n"
"    border-radius: 8px;\n"
"    padding: 6px 10px;\n"
"    font-size: 13px;\n"
"    color: #0a1a2f;\n"
"    selection-background-color: #c7e0ff;\n"
"    selection-color: #000000;\n"
"}\n"
"\n"
"/* Focused (when user clicks or types) */\n"
"QLineEdit:focus {\n"
"    border: 2px solid #0078d7;          /* Modern blue accent */\n"
"    background-color: #ffffff;\n"
"    outline: none;\n"
"}\n"
"\n"
"/* Hover effect */\n"
"QLineEdit:hover {\n"
"    border: 1.5px solid #9bbadf;\n"
"}\n"
"\n"
"/* Disabled (grayed-out field) */\n"
"QLineEdit:disabled {\n"
"    background-color: #f0f3f6;\n"
"    color: #8c8c8c;\n"
"    border: 1px solid #d0d6df;\n"
"}\n"
"\n"
"/* Placeholder text style */\n"
"QLineEdit::placeholder {\n"
"    color: #a0a9b8;\n"
"    font-style: italic;\n"
"}\n"
"\n"
"/* Optional subtle shadow for depth */\n"
"QLineEdit {\n"
"    box-shadow: 0 0 3px rgba(0, 0, 0, 0.1);\n"
"}"));
        lineEdit_13 = new QLineEdit(page_3);
        lineEdit_13->setObjectName("lineEdit_13");
        lineEdit_13->setGeometry(QRect(940, 570, 113, 26));
        lineEdit_13->setStyleSheet(QString::fromUtf8("QLineEdit {\n"
"    background-color: #ffffff;\n"
"    border: 1.5px solid #cfd9e6;\n"
"    border-radius: 8px;\n"
"    padding: 6px 10px;\n"
"    font-size: 13px;\n"
"    color: #0a1a2f;\n"
"    selection-background-color: #c7e0ff;\n"
"    selection-color: #000000;\n"
"}\n"
"\n"
"/* Focused (when user clicks or types) */\n"
"QLineEdit:focus {\n"
"    border: 2px solid #0078d7;          /* Modern blue accent */\n"
"    background-color: #ffffff;\n"
"    outline: none;\n"
"}\n"
"\n"
"/* Hover effect */\n"
"QLineEdit:hover {\n"
"    border: 1.5px solid #9bbadf;\n"
"}\n"
"\n"
"/* Disabled (grayed-out field) */\n"
"QLineEdit:disabled {\n"
"    background-color: #f0f3f6;\n"
"    color: #8c8c8c;\n"
"    border: 1px solid #d0d6df;\n"
"}\n"
"\n"
"/* Placeholder text style */\n"
"QLineEdit::placeholder {\n"
"    color: #a0a9b8;\n"
"    font-style: italic;\n"
"}\n"
"\n"
"/* Optional subtle shadow for depth */\n"
"QLineEdit {\n"
"    box-shadow: 0 0 3px rgba(0, 0, 0, 0.1);\n"
"}"));
        pushButton_17 = new QPushButton(page_3);
        pushButton_17->setObjectName("pushButton_17");
        pushButton_17->setGeometry(QRect(820, 610, 93, 29));
        pushButton_17->setStyleSheet(QString::fromUtf8("QLineEdit {\n"
"    background-color: #ffffff;\n"
"    border: 1px solid #ccd6e1;\n"
"    border-radius: 8px;\n"
"    padding: 6px 12px;\n"
"    font-size: 13px;\n"
"}\n"
"\n"
"QLineEdit:focus {\n"
"    border: 1.5px solid #0078d7;\n"
"    background-color: #ffffff;\n"
"}\n"
"\n"
"QPushButton {\n"
"    background-color: #0c2d57;\n"
"    color: white;\n"
"    border-radius: 6px;\n"
"    padding: 6px 14px;\n"
"    font-size: 12px;\n"
"    border: none;\n"
"}\n"
"\n"
"QPushButton#searchButton:hover {\n"
"    background-color: #1458a6;\n"
"}color: rgb(248, 249, 250);\n"
"font: italic 9pt \"Segoe UI\";\n"
"background-color: rgb(13, 59, 102);"));
        groupBox = new QGroupBox(page_3);
        groupBox->setObjectName("groupBox");
        groupBox->setGeometry(QRect(890, 110, 271, 321));
        groupBox->setStyleSheet(QString::fromUtf8("\n"
"    background-color: #001f3f;            /* Deep navy blue */\n"
"    border-top-right-radius: 20px;\n"
"    border-bottom-right-radius: 20px;\n"
"    padding: 20px 10px;\n"
"    border: none;\n"
"    color: #ffffff;\n"
"    font-size: 15px;\n"
"    font-weight: 600;\n"
"    padding: 5px;"));
        lineEdit_nom = new QLineEdit(groupBox);
        lineEdit_nom->setObjectName("lineEdit_nom");
        lineEdit_nom->setGeometry(QRect(130, 40, 113, 26));
        lineEdit_nom->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"background-color: rgb(255, 255, 255);"));
        lineEdit_prenom = new QLineEdit(groupBox);
        lineEdit_prenom->setObjectName("lineEdit_prenom");
        lineEdit_prenom->setGeometry(QRect(130, 70, 113, 26));
        lineEdit_prenom->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"background-color: rgb(255, 255, 255);"));
        lineEdit_id = new QLineEdit(groupBox);
        lineEdit_id->setObjectName("lineEdit_id");
        lineEdit_id->setGeometry(QRect(130, 10, 113, 26));
        lineEdit_id->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"background-color: rgb(255, 255, 255);"));
        lineEdit_adresse = new QLineEdit(groupBox);
        lineEdit_adresse->setObjectName("lineEdit_adresse");
        lineEdit_adresse->setGeometry(QRect(130, 100, 113, 26));
        lineEdit_adresse->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"background-color: rgb(255, 255, 255);color: rgb(255, 255, 255);"));
        lineEdit_zone = new QLineEdit(groupBox);
        lineEdit_zone->setObjectName("lineEdit_zone");
        lineEdit_zone->setGeometry(QRect(130, 130, 113, 26));
        lineEdit_zone->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"background-color: rgb(255, 255, 255);color: rgb(255, 255, 255);"));
        lineEdit_situation = new QLineEdit(groupBox);
        lineEdit_situation->setObjectName("lineEdit_situation");
        lineEdit_situation->setGeometry(QRect(130, 160, 113, 26));
        lineEdit_situation->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"background-color: rgb(255, 255, 255);color: rgb(255, 255, 255);"));
        lineEdit_contact = new QLineEdit(groupBox);
        lineEdit_contact->setObjectName("lineEdit_contact");
        lineEdit_contact->setGeometry(QRect(130, 190, 113, 26));
        lineEdit_contact->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"background-color: rgb(255, 255, 255);color: rgb(255, 255, 255);"));
        lineEdit_statut = new QLineEdit(groupBox);
        lineEdit_statut->setObjectName("lineEdit_statut");
        lineEdit_statut->setGeometry(QRect(130, 220, 113, 26));
        lineEdit_statut->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"background-color: rgb(255, 255, 255);color: rgb(255, 255, 255);"));
        lineEdit_besoins = new QLineEdit(groupBox);
        lineEdit_besoins->setObjectName("lineEdit_besoins");
        lineEdit_besoins->setGeometry(QRect(130, 250, 113, 26));
        lineEdit_besoins->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"background-color: rgb(255, 255, 255);color: rgb(255, 255, 255);"));
        lineEdit_satisfaction = new QLineEdit(groupBox);
        lineEdit_satisfaction->setObjectName("lineEdit_satisfaction");
        lineEdit_satisfaction->setGeometry(QRect(130, 280, 113, 26));
        lineEdit_satisfaction->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"background-color: rgb(255, 255, 255);color: rgb(255, 255, 255);"));
        label_144 = new QLabel(groupBox);
        label_144->setObjectName("label_144");
        label_144->setGeometry(QRect(10, 10, 63, 20));
        label_144->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
        label_145 = new QLabel(groupBox);
        label_145->setObjectName("label_145");
        label_145->setGeometry(QRect(10, 40, 63, 20));
        label_145->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
        label_146 = new QLabel(groupBox);
        label_146->setObjectName("label_146");
        label_146->setGeometry(QRect(10, 70, 63, 20));
        label_146->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
        label_147 = new QLabel(groupBox);
        label_147->setObjectName("label_147");
        label_147->setGeometry(QRect(10, 100, 63, 20));
        label_148 = new QLabel(groupBox);
        label_148->setObjectName("label_148");
        label_148->setGeometry(QRect(10, 100, 63, 20));
        label_148->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
        label_149 = new QLabel(groupBox);
        label_149->setObjectName("label_149");
        label_149->setGeometry(QRect(10, 130, 63, 20));
        label_149->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
        label_150 = new QLabel(groupBox);
        label_150->setObjectName("label_150");
        label_150->setGeometry(QRect(10, 160, 63, 20));
        label_150->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
        label_151 = new QLabel(groupBox);
        label_151->setObjectName("label_151");
        label_151->setGeometry(QRect(10, 190, 63, 20));
        label_151->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
        label_152 = new QLabel(groupBox);
        label_152->setObjectName("label_152");
        label_152->setGeometry(QRect(10, 220, 63, 20));
        label_152->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
        label_153 = new QLabel(groupBox);
        label_153->setObjectName("label_153");
        label_153->setGeometry(QRect(10, 250, 63, 20));
        label_153->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
        label_154 = new QLabel(groupBox);
        label_154->setObjectName("label_154");
        label_154->setGeometry(QRect(10, 280, 91, 20));
        label_154->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
        tableView_habitants = new QTableView(page_3);
        tableView_habitants->setObjectName("tableView_habitants");
        tableView_habitants->setGeometry(QRect(10, 120, 871, 321));
        on_pushButton_13 = new QPushButton(page_3);
        on_pushButton_13->setObjectName("on_pushButton_13");
        on_pushButton_13->setGeometry(QRect(10, 20, 351, 33));
        on_pushButton_13->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: #0c2d57;\n"
"    color: #ffffff;\n"
"    border-radius: 8px;\n"
"    padding: 8px 16px;\n"
"    margin: 0 5px;\n"
"    font-size: 13px;\n"
"    font-style: italic;\n"
"    border: none;\n"
"}\n"
"\n"
"QPushButton#topTab:hover {\n"
"    background-color: #1c4f94;\n"
"}\n"
"\n"
"QPushButton#topTab:pressed {\n"
"    background-color: #0a2343;\n"
"}"));
        lineEdit_rechH = new QLineEdit(page_3);
        lineEdit_rechH->setObjectName("lineEdit_rechH");
        lineEdit_rechH->setGeometry(QRect(20, 60, 942, 35));
        lineEdit_rechH->setStyleSheet(QString::fromUtf8("QLineEdit {\n"
"    background-color: #ffffff;\n"
"    border: 1.5px solid #cfd9e6;\n"
"    border-radius: 8px;\n"
"    padding: 6px 10px;\n"
"    font-size: 13px;\n"
"    color: #0a1a2f;\n"
"    selection-background-color: #c7e0ff;\n"
"    selection-color: #000000;\n"
"}\n"
"\n"
"/* Focused (when user clicks or types) */\n"
"QLineEdit:focus {\n"
"    border: 2px solid #0078d7;          /* Modern blue accent */\n"
"    background-color: #ffffff;\n"
"    outline: none;\n"
"}\n"
"\n"
"/* Hover effect */\n"
"QLineEdit:hover {\n"
"    border: 1.5px solid #9bbadf;\n"
"}\n"
"\n"
"/* Disabled (grayed-out field) */\n"
"QLineEdit:disabled {\n"
"    background-color: #f0f3f6;\n"
"    color: #8c8c8c;\n"
"    border: 1px solid #d0d6df;\n"
"}\n"
"\n"
"/* Placeholder text style */\n"
"QLineEdit::placeholder {\n"
"    color: #a0a9b8;\n"
"    font-style: italic;\n"
"}\n"
"\n"
"/* Optional subtle shadow for depth */\n"
"QLineEdit {\n"
"    box-shadow: 0 0 3px rgba(0, 0, 0, 0.1);\n"
"}"));
        on_pushButton_15 = new QPushButton(page_3);
        on_pushButton_15->setObjectName("on_pushButton_15");
        on_pushButton_15->setGeometry(QRect(800, 20, 368, 33));
        on_pushButton_15->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: #0c2d57;\n"
"    color: #ffffff;\n"
"    border-radius: 8px;\n"
"    padding: 8px 16px;\n"
"    margin: 0 5px;\n"
"    font-size: 13px;\n"
"    font-style: italic;\n"
"    border: none;\n"
"}\n"
"\n"
"QPushButton#topTab:hover {\n"
"    background-color: #1c4f94;\n"
"}\n"
"\n"
"QPushButton#topTab:pressed {\n"
"    background-color: #0a2343;\n"
"}"));
        on_pushButton_14 = new QPushButton(page_3);
        on_pushButton_14->setObjectName("on_pushButton_14");
        on_pushButton_14->setGeometry(QRect(430, 20, 321, 33));
        on_pushButton_14->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: #0c2d57;\n"
"    color: #ffffff;\n"
"    border-radius: 8px;\n"
"    padding: 8px 16px;\n"
"    margin: 0 5px;\n"
"    font-size: 13px;\n"
"    font-style: italic;\n"
"    border: none;\n"
"}\n"
"\n"
"QPushButton#topTab:hover {\n"
"    background-color: #1c4f94;\n"
"}\n"
"\n"
"QPushButton#topTab:pressed {\n"
"    background-color: #0a2343;\n"
"}"));
        pushButton_9 = new QPushButton(page_3);
        pushButton_9->setObjectName("pushButton_9");
        pushButton_9->setGeometry(QRect(1010, 70, 76, 28));
        pushButton_9->setStyleSheet(QString::fromUtf8("\n"
"QLineEdit:focus {\n"
"    border: 1.5px solid #0078d7;\n"
"    background-color: #ffffff;\n"
"}\n"
"\n"
"QPushButton {\n"
"    background-color: #0c2d57;\n"
"    color: white;\n"
"    border-radius: 6px;\n"
"    padding: 6px 14px;\n"
"    font-size: 12px;\n"
"    border: none;\n"
"}\n"
"\n"
"QPushButton#searchButton:hover {\n"
"    background-color: #1458a6;\n"
"}"));
        pushButton_rechH = new QPushButton(page_3);
        pushButton_rechH->setObjectName("pushButton_rechH");
        pushButton_rechH->setGeometry(QRect(1140, 70, 87, 28));
        pushButton_rechH->setStyleSheet(QString::fromUtf8("\n"
"QLineEdit:focus {\n"
"    border: 1.5px solid #0078d7;\n"
"    background-color: #ffffff;\n"
"}\n"
"\n"
"QPushButton {\n"
"    background-color: #0c2d57;\n"
"    color: white;\n"
"    border-radius: 6px;\n"
"    padding: 6px 14px;\n"
"    font-size: 12px;\n"
"    border: none;\n"
"}\n"
"\n"
"QPushButton#searchButton:hover {\n"
"    background-color: #1458a6;\n"
"}"));
        stackedWidget_2->addWidget(page_3);
        page_4 = new QWidget();
        page_4->setObjectName("page_4");
        tableView_4 = new QTableView(page_4);
        tableView_4->setObjectName("tableView_4");
        tableView_4->setGeometry(QRect(20, 140, 1221, 341));
        label_9 = new QLabel(page_4);
        label_9->setObjectName("label_9");
        label_9->setGeometry(QRect(20, 160, 111, 20));
        label_9->setStyleSheet(QString::fromUtf8("color: rgb(0, 0, 0);"));
        lineEdit_7 = new QLineEdit(page_4);
        lineEdit_7->setObjectName("lineEdit_7");
        lineEdit_7->setGeometry(QRect(120, 150, 241, 31));
        lineEdit_7->setStyleSheet(QString::fromUtf8("QLineEdit {\n"
"    background-color: #ffffff;\n"
"    border: 1.5px solid #cfd9e6;\n"
"    border-radius: 8px;\n"
"    padding: 6px 10px;\n"
"    font-size: 13px;\n"
"    color: #0a1a2f;\n"
"    selection-background-color: #c7e0ff;\n"
"    selection-color: #000000;\n"
"}\n"
"\n"
"/* Focused (when user clicks or types) */\n"
"QLineEdit:focus {\n"
"    border: 2px solid #0078d7;          /* Modern blue accent */\n"
"    background-color: #ffffff;\n"
"    outline: none;\n"
"}\n"
"\n"
"/* Hover effect */\n"
"QLineEdit:hover {\n"
"    border: 1.5px solid #9bbadf;\n"
"}\n"
"\n"
"/* Disabled (grayed-out field) */\n"
"QLineEdit:disabled {\n"
"    background-color: #f0f3f6;\n"
"    color: #8c8c8c;\n"
"    border: 1px solid #d0d6df;\n"
"}\n"
"\n"
"/* Placeholder text style */\n"
"QLineEdit::placeholder {\n"
"    color: #a0a9b8;\n"
"    font-style: italic;\n"
"}\n"
"\n"
"/* Optional subtle shadow for depth */\n"
"QLineEdit {\n"
"    box-shadow: 0 0 3px rgba(0, 0, 0, 0.1);\n"
"}"));
        label_20 = new QLabel(page_4);
        label_20->setObjectName("label_20");
        label_20->setGeometry(QRect(20, 210, 121, 31));
        label_20->setStyleSheet(QString::fromUtf8("color: rgb(0, 0, 0);"));
        lineEdit_8 = new QLineEdit(page_4);
        lineEdit_8->setObjectName("lineEdit_8");
        lineEdit_8->setGeometry(QRect(130, 220, 611, 91));
        lineEdit_8->setStyleSheet(QString::fromUtf8("QLineEdit {\n"
"    background-color: #ffffff;\n"
"    border: 1.5px solid #cfd9e6;\n"
"    border-radius: 8px;\n"
"    padding: 6px 10px;\n"
"    font-size: 13px;\n"
"    color: #0a1a2f;\n"
"    selection-background-color: #c7e0ff;\n"
"    selection-color: #000000;\n"
"}\n"
"\n"
"/* Focused (when user clicks or types) */\n"
"QLineEdit:focus {\n"
"    border: 2px solid #0078d7;          /* Modern blue accent */\n"
"    background-color: #ffffff;\n"
"    outline: none;\n"
"}\n"
"\n"
"/* Hover effect */\n"
"QLineEdit:hover {\n"
"    border: 1.5px solid #9bbadf;\n"
"}\n"
"\n"
"/* Disabled (grayed-out field) */\n"
"QLineEdit:disabled {\n"
"    background-color: #f0f3f6;\n"
"    color: #8c8c8c;\n"
"    border: 1px solid #d0d6df;\n"
"}\n"
"\n"
"/* Placeholder text style */\n"
"QLineEdit::placeholder {\n"
"    color: #a0a9b8;\n"
"    font-style: italic;\n"
"}\n"
"\n"
"/* Optional subtle shadow for depth */\n"
"QLineEdit {\n"
"    box-shadow: 0 0 3px rgba(0, 0, 0, 0.1);\n"
"}"));
        label_34 = new QLabel(page_4);
        label_34->setObjectName("label_34");
        label_34->setGeometry(QRect(20, 350, 161, 20));
        label_34->setStyleSheet(QString::fromUtf8("color: rgb(0, 0, 0);"));
        comboBox_12 = new QComboBox(page_4);
        comboBox_12->addItem(QString());
        comboBox_12->addItem(QString());
        comboBox_12->addItem(QString());
        comboBox_12->addItem(QString());
        comboBox_12->addItem(QString());
        comboBox_12->addItem(QString());
        comboBox_12->addItem(QString());
        comboBox_12->addItem(QString());
        comboBox_12->addItem(QString());
        comboBox_12->addItem(QString());
        comboBox_12->addItem(QString());
        comboBox_12->addItem(QString());
        comboBox_12->setObjectName("comboBox_12");
        comboBox_12->setGeometry(QRect(180, 340, 201, 41));
        comboBox_12->setStyleSheet(QString::fromUtf8("\n"
"    background-color: #012a59;\n"
"    color: #ffffff;\n"
"    border: 2px solid #013366;\n"
"    border-radius: 10px;\n"
"    padding: 8px 14px;\n"
"    font-size: 13px;\n"
"    font-weight: 500;\n"
"    text-align: left;\n"
"    transition: all 0.3s ease-in-out;\n"
"    background-color: #024b8a;\n"
"    border-color: #025aa5;\n"
"    background-color: #013869;\n"
"    border-color: #011f3f;\n"
"\n"
""));
        pushButton_10 = new QPushButton(page_4);
        pushButton_10->setObjectName("pushButton_10");
        pushButton_10->setGeometry(QRect(430, 430, 93, 31));
        pushButton_10->setStyleSheet(QString::fromUtf8("/* ==== BUTTONS STYLE ==== */\n"
"QPushButton {\n"
"    background-color: #012a59;\n"
"    color: #ffffff;\n"
"    border: 2px solid #013366;\n"
"    border-radius: 10px;\n"
"    padding: 8px 14px;\n"
"    font-size: 13px;\n"
"    font-weight: 500;\n"
"    text-align: left;\n"
"    transition: all 0.3s ease-in-out;\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"    background-color: #024b8a;\n"
"    border-color: #025aa5;\n"
"}\n"
"\n"
"QPushButton:pressed {\n"
"    background-color: #013869;\n"
"    border-color: #011f3f;\n"
"}\n"
""));
        progressBar_6 = new QProgressBar(page_4);
        progressBar_6->setObjectName("progressBar_6");
        progressBar_6->setGeometry(QRect(550, 430, 118, 23));
        progressBar_6->setStyleSheet(QString::fromUtf8("QLineEdit {\n"
"    background-color: #ffffff;\n"
"    border: 1.5px solid #cfd9e6;\n"
"    border-radius: 8px;\n"
"    padding: 6px 10px;\n"
"    font-size: 13px;\n"
"    color: #0a1a2f;\n"
"    selection-background-color: #c7e0ff;\n"
"    selection-color: #000000;\n"
"}\n"
"\n"
"/* Focused (when user clicks or types) */\n"
"QLineEdit:focus {\n"
"    border: 2px solid #0078d7;          /* Modern blue accent */\n"
"    background-color: #ffffff;\n"
"    outline: none;\n"
"}\n"
"\n"
"/* Hover effect */\n"
"QLineEdit:hover {\n"
"    border: 1.5px solid #9bbadf;\n"
"}\n"
"\n"
"/* Disabled (grayed-out field) */\n"
"QLineEdit:disabled {\n"
"    background-color: #f0f3f6;\n"
"    color: #8c8c8c;\n"
"    border: 1px solid #d0d6df;\n"
"}\n"
"\n"
"/* Placeholder text style */\n"
"QLineEdit::placeholder {\n"
"    color: #a0a9b8;\n"
"    font-style: italic;\n"
"}\n"
"\n"
"/* Optional subtle shadow for depth */\n"
"QLineEdit {\n"
"    box-shadow: 0 0 3px rgba(0, 0, 0, 0.1);\n"
"}"));
        progressBar_6->setValue(0);
        stackedWidget_2->addWidget(page_4);
        page_8 = new QWidget();
        page_8->setObjectName("page_8");
        label_36 = new QLabel(page_8);
        label_36->setObjectName("label_36");
        label_36->setGeometry(QRect(20, 140, 131, 20));
        label_36->setStyleSheet(QString::fromUtf8("color: rgb(0, 0, 0);"));
        lineEdit_4 = new QLineEdit(page_8);
        lineEdit_4->setObjectName("lineEdit_4");
        lineEdit_4->setGeometry(QRect(120, 140, 161, 26));
        label_37 = new QLabel(page_8);
        label_37->setObjectName("label_37");
        label_37->setGeometry(QRect(20, 230, 141, 20));
        label_37->setStyleSheet(QString::fromUtf8("color: rgb(0, 0, 0);"));
        comboBox_14 = new QComboBox(page_8);
        comboBox_14->addItem(QString());
        comboBox_14->addItem(QString());
        comboBox_14->addItem(QString());
        comboBox_14->addItem(QString());
        comboBox_14->addItem(QString());
        comboBox_14->addItem(QString());
        comboBox_14->addItem(QString());
        comboBox_14->setObjectName("comboBox_14");
        comboBox_14->setGeometry(QRect(20, 270, 181, 41));
        comboBox_14->setStyleSheet(QString::fromUtf8("\n"
"    background-color: #012a59;\n"
"    color: #ffffff;\n"
"    border: 2px solid #013366;\n"
"    border-radius: 10px;\n"
"    padding: 8px 14px;\n"
"    font-size: 13px;\n"
"    font-weight: 500;\n"
"    text-align: left;\n"
"    transition: all 0.3s ease-in-out;\n"
"    background-color: #024b8a;\n"
"    border-color: #025aa5;\n"
"\n"
"    background-color: #013869;\n"
"    border-color: #011f3f;\n"
""));
        tableView_5 = new QTableView(page_8);
        tableView_5->setObjectName("tableView_5");
        tableView_5->setGeometry(QRect(250, 210, 651, 381));
        label_38 = new QLabel(page_8);
        label_38->setObjectName("label_38");
        label_38->setGeometry(QRect(300, 250, 121, 20));
        label_38->setStyleSheet(QString::fromUtf8("color: rgb(0, 0, 0);\n"
"font: 700 italic 11pt \"Segoe UI\";"));
        label_39 = new QLabel(page_8);
        label_39->setObjectName("label_39");
        label_39->setGeometry(QRect(300, 340, 111, 20));
        label_39->setStyleSheet(QString::fromUtf8("color: rgb(0, 0, 0);"));
        label_40 = new QLabel(page_8);
        label_40->setObjectName("label_40");
        label_40->setGeometry(QRect(270, 370, 191, 20));
        label_40->setStyleSheet(QString::fromUtf8("color: rgb(0, 0, 0);\n"
"font: 600 italic 10pt \"Segoe UI\";"));
        label_41 = new QLabel(page_8);
        label_41->setObjectName("label_41");
        label_41->setGeometry(QRect(350, 430, 421, 131));
        label_41->setPixmap(QPixmap(QString::fromUtf8("../../../../../Downloads/CRUD/Sans-titre---1.png")));
        label_42 = new QLabel(page_8);
        label_42->setObjectName("label_42");
        label_42->setGeometry(QRect(660, 250, 221, 20));
        label_42->setStyleSheet(QString::fromUtf8("color: rgb(0, 0, 0);\n"
"font: 600 italic 10pt \"Segoe UI\";"));
        label_43 = new QLabel(page_8);
        label_43->setObjectName("label_43");
        label_43->setGeometry(QRect(1020, 650, 391, 131));
        label_43->setPixmap(QPixmap(QString::fromUtf8("../../../../../Downloads/CRUD/Sans-titre---2.png")));
        label_44 = new QLabel(page_8);
        label_44->setObjectName("label_44");
        label_44->setGeometry(QRect(450, 270, 391, 131));
        label_44->setStyleSheet(QString::fromUtf8("color: rgb(0, 0, 0);"));
        label_44->setPixmap(QPixmap(QString::fromUtf8("../../../../../Downloads/CRUD/Sans-titre---2.png")));
        label_45 = new QLabel(page_8);
        label_45->setObjectName("label_45");
        label_45->setGeometry(QRect(290, 490, 451, 81));
        label_45->setPixmap(QPixmap(QString::fromUtf8("../../../../../Downloads/CRUD/Sans-titre---3.png")));
        stackedWidget_4 = new QStackedWidget(page_8);
        stackedWidget_4->setObjectName("stackedWidget_4");
        stackedWidget_4->setGeometry(QRect(0, -100, 1191, 301));
        page_9 = new QWidget();
        page_9->setObjectName("page_9");
        stackedWidget_4->addWidget(page_9);
        page_10 = new QWidget();
        page_10->setObjectName("page_10");
        stackedWidget_4->addWidget(page_10);
        progressBar_7 = new QProgressBar(page_8);
        progressBar_7->setObjectName("progressBar_7");
        progressBar_7->setGeometry(QRect(120, 320, 118, 23));
        progressBar_7->setStyleSheet(QString::fromUtf8("color: rgb(0, 0, 0);"));
        progressBar_7->setValue(24);
        progressBar_8 = new QProgressBar(page_8);
        progressBar_8->setObjectName("progressBar_8");
        progressBar_8->setGeometry(QRect(770, 560, 118, 23));
        progressBar_8->setStyleSheet(QString::fromUtf8("color: rgb(0, 0, 0);"));
        progressBar_8->setValue(24);
        stackedWidget_2->addWidget(page_8);
        Ghabitats_2 = new QPushButton(centralwidget);
        Ghabitats_2->setObjectName("Ghabitats_2");
        Ghabitats_2->setGeometry(QRect(30, 90, 161, 51));
        Ghabitats_2->setStyleSheet(QString::fromUtf8("/* ==== BUTTONS STYLE ==== */\n"
"QPushButton {\n"
"    background-color: #012a59;\n"
"    color: #ffffff;\n"
"    border: 2px solid #013366;\n"
"    border-radius: 10px;\n"
"    padding: 8px 14px;\n"
"    font-size: 13px;\n"
"    font-weight: 500;\n"
"    text-align: left;\n"
"    transition: all 0.3s ease-in-out;\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"    background-color: #024b8a;\n"
"    border-color: #025aa5;\n"
"}\n"
"\n"
"QPushButton:pressed {\n"
"    background-color: #013869;\n"
"    border-color: #011f3f;\n"
"}\n"
""));
        Ghabitant = new QPushButton(centralwidget);
        Ghabitant->setObjectName("Ghabitant");
        Ghabitant->setGeometry(QRect(30, 30, 161, 51));
        Ghabitant->setStyleSheet(QString::fromUtf8("/* ==== BUTTONS STYLE ==== */\n"
"QPushButton {\n"
"    background-color: #012a59;\n"
"    color: #ffffff;\n"
"    border: 2px solid #013366;\n"
"    border-radius: 10px;\n"
"    padding: 8px 14px;\n"
"    font-size: 13px;\n"
"    font-weight: 500;\n"
"    text-align: left;\n"
"    transition: all 0.3s ease-in-out;\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"    background-color: #024b8a;\n"
"    border-color: #025aa5;\n"
"}\n"
"\n"
"QPushButton:pressed {\n"
"    background-color: #013869;\n"
"    border-color: #011f3f;\n"
"}\n"
""));
        Ghabitats_3 = new QPushButton(centralwidget);
        Ghabitats_3->setObjectName("Ghabitats_3");
        Ghabitats_3->setGeometry(QRect(30, 170, 161, 51));
        Ghabitats_3->setStyleSheet(QString::fromUtf8("/* ==== BUTTONS STYLE ==== */\n"
"QPushButton {\n"
"    background-color: #012a59;\n"
"    color: #ffffff;\n"
"    border: 2px solid #013366;\n"
"    border-radius: 10px;\n"
"    padding: 8px 14px;\n"
"    font-size: 13px;\n"
"    font-weight: 500;\n"
"    text-align: left;\n"
"    transition: all 0.3s ease-in-out;\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"    background-color: #024b8a;\n"
"    border-color: #025aa5;\n"
"}\n"
"\n"
"QPushButton:pressed {\n"
"    background-color: #013869;\n"
"    border-color: #011f3f;\n"
"}\n"
""));
        Ghabitats_4 = new QPushButton(centralwidget);
        Ghabitats_4->setObjectName("Ghabitats_4");
        Ghabitats_4->setGeometry(QRect(30, 240, 161, 51));
        Ghabitats_4->setStyleSheet(QString::fromUtf8("/* ==== BUTTONS STYLE ==== */\n"
"QPushButton {\n"
"    background-color: #012a59;\n"
"    color: #ffffff;\n"
"    border: 2px solid #013366;\n"
"    border-radius: 10px;\n"
"    padding: 8px 14px;\n"
"    font-size: 13px;\n"
"    font-weight: 500;\n"
"    text-align: left;\n"
"    transition: all 0.3s ease-in-out;\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"    background-color: #024b8a;\n"
"    border-color: #025aa5;\n"
"}\n"
"\n"
"QPushButton:pressed {\n"
"    background-color: #013869;\n"
"    border-color: #011f3f;\n"
"}\n"
""));
        Ghabitats_5 = new QPushButton(centralwidget);
        Ghabitats_5->setObjectName("Ghabitats_5");
        Ghabitats_5->setGeometry(QRect(30, 330, 161, 51));
        Ghabitats_5->setStyleSheet(QString::fromUtf8("/* ==== BUTTONS STYLE ==== */\n"
"QPushButton {\n"
"    background-color: #012a59;\n"
"    color: #ffffff;\n"
"    border: 2px solid #013366;\n"
"    border-radius: 10px;\n"
"    padding: 8px 14px;\n"
"    font-size: 13px;\n"
"    font-weight: 500;\n"
"    text-align: left;\n"
"    transition: all 0.3s ease-in-out;\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"    background-color: #024b8a;\n"
"    border-color: #025aa5;\n"
"}\n"
"\n"
"QPushButton:pressed {\n"
"    background-color: #013869;\n"
"    border-color: #011f3f;\n"
"}\n"
""));
        Ghabitats_6 = new QPushButton(centralwidget);
        Ghabitats_6->setObjectName("Ghabitats_6");
        Ghabitats_6->setGeometry(QRect(30, 430, 161, 51));
        Ghabitats_6->setStyleSheet(QString::fromUtf8("/* ==== BUTTONS STYLE ==== */\n"
"QPushButton {\n"
"    background-color: #012a59;\n"
"    color: #ffffff;\n"
"    border: 2px solid #013366;\n"
"    border-radius: 10px;\n"
"    padding: 8px 14px;\n"
"    font-size: 13px;\n"
"    font-weight: 500;\n"
"    text-align: left;\n"
"    transition: all 0.3s ease-in-out;\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"    background-color: #024b8a;\n"
"    border-color: #025aa5;\n"
"}\n"
"\n"
"QPushButton:pressed {\n"
"    background-color: #013869;\n"
"    border-color: #011f3f;\n"
"}\n"
""));
        MainWindow->setCentralWidget(centralwidget);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        stackedWidget_2->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        action_Green_Zone_Manager->setText(QCoreApplication::translate("MainWindow", "\360\237\214\277 Green Zone Manager", nullptr));
        action_Green_Zone_Manager_2->setText(QCoreApplication::translate("MainWindow", "\360\237\214\277 Green Zone Manager", nullptr));
        label_4->setText(QCoreApplication::translate("MainWindow", "TRIER PAR:", nullptr));
        comboBox_trieH->setItemText(0, QCoreApplication::translate("MainWindow", "nom", nullptr));
        comboBox_trieH->setItemText(1, QCoreApplication::translate("MainWindow", "adresse ", nullptr));

        pushButton_trieH->setText(QCoreApplication::translate("MainWindow", "TRIER", nullptr));
        pushButton_modifH->setText(QCoreApplication::translate("MainWindow", "Modifier ", nullptr));
        pushButton_suppH->setText(QCoreApplication::translate("MainWindow", "Supprimer \360\237\227\221\357\270\217", nullptr));
        pushButton_ajoutH->setText(QCoreApplication::translate("MainWindow", "Ajouter \342\234\224\357\270\217", nullptr));
        label_6->setText(QCoreApplication::translate("MainWindow", "EXPORT PDF :", nullptr));
        label_11->setText(QCoreApplication::translate("MainWindow", "FORMAT:", nullptr));
        comboBox_5->setItemText(0, QCoreApplication::translate("MainWindow", "A3", nullptr));
        comboBox_5->setItemText(1, QCoreApplication::translate("MainWindow", "A4", nullptr));
        comboBox_5->setItemText(2, QCoreApplication::translate("MainWindow", "A5", nullptr));

        label_19->setText(QCoreApplication::translate("MainWindow", "QUALITE:", nullptr));
        comboBox_7->setItemText(0, QCoreApplication::translate("MainWindow", "standard", nullptr));
        comboBox_7->setItemText(1, QCoreApplication::translate("MainWindow", "***", nullptr));

        label_31->setText(QCoreApplication::translate("MainWindow", "DESTINATION DU FICHIER :", nullptr));
        label_32->setText(QCoreApplication::translate("MainWindow", "NOM DU FICHIER :", nullptr));
        label_33->setText(QCoreApplication::translate("MainWindow", "EMPLACEMENT :", nullptr));
        pushButton_17->setText(QCoreApplication::translate("MainWindow", "Export", nullptr));
        groupBox->setTitle(QString());
        label_144->setText(QCoreApplication::translate("MainWindow", "id:", nullptr));
        label_145->setText(QCoreApplication::translate("MainWindow", "nom:", nullptr));
        label_146->setText(QCoreApplication::translate("MainWindow", "prenom:", nullptr));
        label_147->setText(QString());
        label_148->setText(QCoreApplication::translate("MainWindow", "adresse:", nullptr));
        label_149->setText(QCoreApplication::translate("MainWindow", "zone:", nullptr));
        label_150->setText(QCoreApplication::translate("MainWindow", "situation:", nullptr));
        label_151->setText(QCoreApplication::translate("MainWindow", "contact:", nullptr));
        label_152->setText(QCoreApplication::translate("MainWindow", "statut:", nullptr));
        label_153->setText(QCoreApplication::translate("MainWindow", "besoins:", nullptr));
        label_154->setText(QCoreApplication::translate("MainWindow", "satisfaction:", nullptr));
        on_pushButton_13->setText(QString());
        lineEdit_rechH->setText(QCoreApplication::translate("MainWindow", "Rechercher des habitants \360\237\224\216", nullptr));
        on_pushButton_15->setText(QCoreApplication::translate("MainWindow", "satisfaction", nullptr));
        on_pushButton_14->setText(QCoreApplication::translate("MainWindow", "demmande", nullptr));
        pushButton_9->setText(QCoreApplication::translate("MainWindow", "Chercher", nullptr));
        pushButton_rechH->setText(QCoreApplication::translate("MainWindow", "Rechercher", nullptr));
        label_9->setText(QCoreApplication::translate("MainWindow", "ID d'habitant :", nullptr));
        label_20->setText(QCoreApplication::translate("MainWindow", "RECLAMATION :", nullptr));
        label_34->setText(QCoreApplication::translate("MainWindow", "choisir votre probleme :", nullptr));
        comboBox_12->setItemText(0, QCoreApplication::translate("MainWindow", "probleme technique", nullptr));
        comboBox_12->setItemText(1, QCoreApplication::translate("MainWindow", "probleme numerique", nullptr));
        comboBox_12->setItemText(2, QCoreApplication::translate("MainWindow", "probleme civilaire", nullptr));
        comboBox_12->setItemText(3, QCoreApplication::translate("MainWindow", "Logement et habitat", nullptr));
        comboBox_12->setItemText(4, QCoreApplication::translate("MainWindow", "Environnement et propret\303\251", nullptr));
        comboBox_12->setItemText(5, QCoreApplication::translate("MainWindow", "Infrastructures", nullptr));
        comboBox_12->setItemText(6, QCoreApplication::translate("MainWindow", "Nouvel \303\251l\303\251ment", nullptr));
        comboBox_12->setItemText(7, QCoreApplication::translate("MainWindow", "transport", nullptr));
        comboBox_12->setItemText(8, QCoreApplication::translate("MainWindow", "S\303\251curit\303\251", nullptr));
        comboBox_12->setItemText(9, QCoreApplication::translate("MainWindow", "Vie sociale", nullptr));
        comboBox_12->setItemText(10, QCoreApplication::translate("MainWindow", "\303\211ducation", nullptr));
        comboBox_12->setItemText(11, QCoreApplication::translate("MainWindow", "bien-\303\252tre", nullptr));

        pushButton_10->setText(QCoreApplication::translate("MainWindow", "Demande", nullptr));
        label_36->setText(QCoreApplication::translate("MainWindow", "ID d'habitant :", nullptr));
        label_37->setText(QCoreApplication::translate("MainWindow", "score par rapport :", nullptr));
        comboBox_14->setItemText(0, QCoreApplication::translate("MainWindow", "Logement et habitat", nullptr));
        comboBox_14->setItemText(1, QCoreApplication::translate("MainWindow", "Environnement et propret\303\251", nullptr));
        comboBox_14->setItemText(2, QCoreApplication::translate("MainWindow", "Infrastructures", nullptr));
        comboBox_14->setItemText(3, QCoreApplication::translate("MainWindow", "transport", nullptr));
        comboBox_14->setItemText(4, QCoreApplication::translate("MainWindow", "S\303\251curit\303\251", nullptr));
        comboBox_14->setItemText(5, QCoreApplication::translate("MainWindow", "\303\211ducation", nullptr));
        comboBox_14->setItemText(6, QCoreApplication::translate("MainWindow", "bien-\303\252tre", nullptr));

        label_38->setText(QCoreApplication::translate("MainWindow", "Score Global", nullptr));
        label_39->setText(QCoreApplication::translate("MainWindow", "% de satisfaction", nullptr));
        label_40->setText(QCoreApplication::translate("MainWindow", "Satisfaction par service :", nullptr));
        label_41->setText(QString());
        label_42->setText(QCoreApplication::translate("MainWindow", "Satisfaction par adresse  :", nullptr));
        label_43->setText(QString());
        label_44->setText(QString());
        label_45->setText(QString());
        Ghabitats_2->setText(QCoreApplication::translate("MainWindow", "    Gestion habitats", nullptr));
        Ghabitant->setText(QCoreApplication::translate("MainWindow", "    Gestion habitants", nullptr));
        Ghabitats_3->setText(QCoreApplication::translate("MainWindow", "Gestion equipement", nullptr));
        Ghabitats_4->setText(QCoreApplication::translate("MainWindow", "Gestion personelle", nullptr));
        Ghabitats_5->setText(QCoreApplication::translate("MainWindow", "Gestion zones vertes", nullptr));
        Ghabitats_6->setText(QCoreApplication::translate("MainWindow", " Gestion vehile ", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
