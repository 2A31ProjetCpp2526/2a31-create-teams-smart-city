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
#include <QtWidgets/QComboBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTableView>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QPushButton *Ghabitant_4;
    QPushButton *Ghabitant_6;
    QTableView *tableView_2;
    QPushButton *gestion_button;
    QPushButton *surveilalnce_button;
    QPushButton *Ghabitant;
    QPushButton *Ghabitant_2;
    QPushButton *Ghabitant_5;
    QPushButton *pushButton_16;
    QPushButton *pushButton_15;
    QStackedWidget *stackedWidget;
    QWidget *page;
    QLineEdit *ADRESSE;
    QLabel *label_110;
    QLineEdit *lineEdit_recherche;
    QLabel *label_108;
    QLabel *label_109;
    QLabel *label_recherche;
    QLineEdit *ID_HABITAT;
    QLabel *label_111;
    QLabel *label_112;
    QLineEdit *TYPE_LOGEMENT;
    QStackedWidget *stackedWidget_2;
    QWidget *page_0;
    QWidget *page_3;
    QWidget *page_1;
    QWidget *page_4;
    QLineEdit *SURFACE;
    QLineEdit *NB_PERSONNES;
    QTableWidget *TabCrudHabitat;
    QPushButton *pushButton_exporter;
    QPushButton *btnModifier;
    QPushButton *btnSupprimer;
    QPushButton *btnAjouter;
    QLabel *label_5;
    QPushButton *btnafficher;
    QWidget *page_5;
    QLabel *label;
    QLabel *label_2;
    QComboBox *comboBoxtype2;
    QLabel *label_3;
    QLabel *label_filtremaison;
    QComboBox *comboBox_2;
    QPushButton *pushButtonrafraichir;
    QTableWidget *table_surveillance;
    QPushButton *pushButton_exportsurveillence;
    QWidget *page_2;
    QLabel *label_12;
    QLabel *label_13;
    QComboBox *comboBox_4;
    QPushButton *pushButton_6;
    QTableWidget *tableWidget_2;
    QPushButton *pushbuttonexportanomalie;
    QLabel *label_7;
    QWidget *page_6;
    QComboBox *comboBox_typeconso_2;
    QPushButton *pushrafraichiralerte;
    QTableWidget *tableWidget_3;
    QPushButton *pushButton_10;
    QPushButton *pushButton_11;
    QPushButton *pushButton_12;
    QPushButton *pushButton_13;
    QLabel *label_8;
    QLabel *label_9;
    QLineEdit *lineEdit_surface_2;
    QPushButton *Ghabitant_3;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(1835, 884);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        Ghabitant_4 = new QPushButton(centralwidget);
        Ghabitant_4->setObjectName("Ghabitant_4");
        Ghabitant_4->setGeometry(QRect(-10, 240, 191, 71));
        Ghabitant_4->setStyleSheet(QString::fromUtf8("color: rgb(248, 249, 250);\n"
"font: italic 9pt \"Segoe UI\";\n"
"background-color: rgb(13, 59, 102);"));
        Ghabitant_6 = new QPushButton(centralwidget);
        Ghabitant_6->setObjectName("Ghabitant_6");
        Ghabitant_6->setGeometry(QRect(-10, 160, 191, 71));
        Ghabitant_6->setStyleSheet(QString::fromUtf8("color: rgb(248, 249, 250);\n"
"font: italic 9pt \"Segoe UI\";\n"
"background-color: rgb(13, 59, 102);"));
        tableView_2 = new QTableView(centralwidget);
        tableView_2->setObjectName("tableView_2");
        tableView_2->setGeometry(QRect(-10, -10, 191, 1671));
        tableView_2->setStyleSheet(QString::fromUtf8("color: rgb(0, 0, 127);\n"
"background-color: rgb(0, 0, 93);"));
        gestion_button = new QPushButton(centralwidget);
        gestion_button->setObjectName("gestion_button");
        gestion_button->setGeometry(QRect(200, 0, 301, 51));
        gestion_button->setStyleSheet(QString::fromUtf8("color: rgb(248, 249, 250);\n"
"font: italic 9pt \"Segoe UI\";\n"
"background-color: rgb(13, 59, 102);"));
        surveilalnce_button = new QPushButton(centralwidget);
        surveilalnce_button->setObjectName("surveilalnce_button");
        surveilalnce_button->setGeometry(QRect(520, 0, 291, 51));
        surveilalnce_button->setStyleSheet(QString::fromUtf8("color: rgb(248, 249, 250);\n"
"font: italic 9pt \"Segoe UI\";\n"
"background-color: rgb(13, 59, 102);"));
        Ghabitant = new QPushButton(centralwidget);
        Ghabitant->setObjectName("Ghabitant");
        Ghabitant->setGeometry(QRect(-10, 80, 191, 71));
        Ghabitant->setStyleSheet(QString::fromUtf8("color: rgb(248, 249, 250);\n"
"font: italic 9pt \"Segoe UI\";\n"
"background-color: rgb(13, 59, 102);"));
        Ghabitant_2 = new QPushButton(centralwidget);
        Ghabitant_2->setObjectName("Ghabitant_2");
        Ghabitant_2->setGeometry(QRect(-10, 320, 191, 71));
        Ghabitant_2->setStyleSheet(QString::fromUtf8("color: rgb(248, 249, 250);\n"
"font: italic 9pt \"Segoe UI\";\n"
"background-color: rgb(13, 59, 102);"));
        Ghabitant_5 = new QPushButton(centralwidget);
        Ghabitant_5->setObjectName("Ghabitant_5");
        Ghabitant_5->setGeometry(QRect(-10, 0, 191, 71));
        Ghabitant_5->setStyleSheet(QString::fromUtf8("color: rgb(248, 249, 250);\n"
"font: italic 9pt \"Segoe UI\";\n"
"background-color: rgb(13, 59, 102);"));
        pushButton_16 = new QPushButton(centralwidget);
        pushButton_16->setObjectName("pushButton_16");
        pushButton_16->setGeometry(QRect(1180, 0, 331, 51));
        pushButton_16->setStyleSheet(QString::fromUtf8("color: rgb(248, 249, 250);\n"
"font: italic 9pt \"Segoe UI\";\n"
"background-color: rgb(13, 59, 102);"));
        pushButton_15 = new QPushButton(centralwidget);
        pushButton_15->setObjectName("pushButton_15");
        pushButton_15->setGeometry(QRect(830, 0, 331, 51));
        pushButton_15->setStyleSheet(QString::fromUtf8("color: rgb(248, 249, 250);\n"
"font: italic 9pt \"Segoe UI\";\n"
"background-color: rgb(13, 59, 102);"));
        stackedWidget = new QStackedWidget(centralwidget);
        stackedWidget->setObjectName("stackedWidget");
        stackedWidget->setGeometry(QRect(190, 60, 1321, 731));
        page = new QWidget();
        page->setObjectName("page");
        ADRESSE = new QLineEdit(page);
        ADRESSE->setObjectName("ADRESSE");
        ADRESSE->setGeometry(QRect(320, 210, 141, 26));
        label_110 = new QLabel(page);
        label_110->setObjectName("label_110");
        label_110->setGeometry(QRect(650, 180, 121, 20));
        label_110->setStyleSheet(QString::fromUtf8("color: rgb(0, 0, 0);"));
        lineEdit_recherche = new QLineEdit(page);
        lineEdit_recherche->setObjectName("lineEdit_recherche");
        lineEdit_recherche->setGeometry(QRect(20, 120, 1021, 31));
        label_108 = new QLabel(page);
        label_108->setObjectName("label_108");
        label_108->setGeometry(QRect(90, 180, 91, 20));
        label_108->setStyleSheet(QString::fromUtf8("color: rgb(0, 0, 0);"));
        label_109 = new QLabel(page);
        label_109->setObjectName("label_109");
        label_109->setGeometry(QRect(320, 180, 91, 20));
        label_109->setStyleSheet(QString::fromUtf8("color: rgb(0, 0, 0);"));
        label_recherche = new QLabel(page);
        label_recherche->setObjectName("label_recherche");
        label_recherche->setGeometry(QRect(20, 90, 141, 20));
        label_recherche->setStyleSheet(QString::fromUtf8("color: rgb(0, 0, 0);"));
        ID_HABITAT = new QLineEdit(page);
        ID_HABITAT->setObjectName("ID_HABITAT");
        ID_HABITAT->setGeometry(QRect(90, 210, 141, 26));
        label_111 = new QLabel(page);
        label_111->setObjectName("label_111");
        label_111->setGeometry(QRect(480, 180, 91, 20));
        label_111->setStyleSheet(QString::fromUtf8("color: rgb(0, 0, 0);"));
        label_112 = new QLabel(page);
        label_112->setObjectName("label_112");
        label_112->setGeometry(QRect(820, 180, 91, 20));
        label_112->setStyleSheet(QString::fromUtf8("color: rgb(0, 0, 0);"));
        TYPE_LOGEMENT = new QLineEdit(page);
        TYPE_LOGEMENT->setObjectName("TYPE_LOGEMENT");
        TYPE_LOGEMENT->setGeometry(QRect(820, 210, 141, 26));
        stackedWidget_2 = new QStackedWidget(page);
        stackedWidget_2->setObjectName("stackedWidget_2");
        stackedWidget_2->setGeometry(QRect(1730, 450, 191, 251));
        page_0 = new QWidget();
        page_0->setObjectName("page_0");
        stackedWidget_2->addWidget(page_0);
        page_3 = new QWidget();
        page_3->setObjectName("page_3");
        stackedWidget_2->addWidget(page_3);
        page_1 = new QWidget();
        page_1->setObjectName("page_1");
        stackedWidget_2->addWidget(page_1);
        page_4 = new QWidget();
        page_4->setObjectName("page_4");
        stackedWidget_2->addWidget(page_4);
        SURFACE = new QLineEdit(page);
        SURFACE->setObjectName("SURFACE");
        SURFACE->setGeometry(QRect(480, 210, 141, 26));
        NB_PERSONNES = new QLineEdit(page);
        NB_PERSONNES->setObjectName("NB_PERSONNES");
        NB_PERSONNES->setGeometry(QRect(650, 210, 141, 26));
        TabCrudHabitat = new QTableWidget(page);
        if (TabCrudHabitat->columnCount() < 5)
            TabCrudHabitat->setColumnCount(5);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        TabCrudHabitat->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        TabCrudHabitat->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        TabCrudHabitat->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        TabCrudHabitat->setHorizontalHeaderItem(3, __qtablewidgetitem3);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        TabCrudHabitat->setHorizontalHeaderItem(4, __qtablewidgetitem4);
        TabCrudHabitat->setObjectName("TabCrudHabitat");
        TabCrudHabitat->setGeometry(QRect(120, 270, 621, 291));
        TabCrudHabitat->setStyleSheet(QString::fromUtf8("/* =====================\n"
"   Tableau central (QTableWidget)\n"
"   ===================== */\n"
"QTableWidget {\n"
"    background-color: rgb(240, 240, 240);  /* Fond g\303\251n\303\251ral du tableau */\n"
"    gridline-color: rgb(204, 204, 204);    /* Couleur des lignes de s\303\251paration */\n"
"    border: 2px solid #2980b9;             /* Bordure du tableau */\n"
"    font: 9pt \"Segoe UI\";                  /* Police du contenu des cellules */\n"
"}\n"
"\n"
"/* En-t\303\252tes du tableau */\n"
"QHeaderView::section {\n"
"    background-color: rgb(13, 59, 102);    /* Fond des en-t\303\252tes */\n"
"    color: rgb(248, 249, 250);             /* Texte des en-t\303\252tes */\n"
"    font-weight: bold;                      /* Texte en gras */\n"
"    padding: 5px;                           /* Espacement int\303\251rieur */\n"
"    border: 1px solid #2980b9;             /* Bordure de l\342\200\231en-t\303\252te */\n"
"}\n"
"\n"
"/* Les cellules */\n"
"QTableWidget::item {\n"
"    background-color: rgb(255, 2"
                        "55, 255);  /* Fond blanc des cellules */\n"
"    color: rgb(0, 51, 102);                /* Texte bleu fonc\303\251 */\n"
"    padding: 4px;                           /* Espacement int\303\251rieur */\n"
"    selection-background-color: rgb(41, 128, 185); /* Fond d\342\200\231une cellule s\303\251lectionn\303\251e */\n"
"    selection-color: white;                 /* Texte de la cellule s\303\251lectionn\303\251e */\n"
"}\n"
"\n"
"/* Lignes impaires et paires (optionnel pour effet z\303\251br\303\251) */\n"
"QTableWidget::item:!selected:alternate {\n"
"    background-color: rgb(245, 245, 245);  /* Fond gris tr\303\250s clair pour les lignes altern\303\251es */\n"
"}\n"
""));
        pushButton_exporter = new QPushButton(page);
        pushButton_exporter->setObjectName("pushButton_exporter");
        pushButton_exporter->setGeometry(QRect(610, 590, 121, 31));
        pushButton_exporter->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: rgb(13, 59, 102); /* Bleu fonc\303\251 */\n"
"    color: rgb(248, 249, 250);          /* Texte blanc */\n"
"    font: italic 9pt \"Segoe UI\";\n"
"}\n"
""));
        btnModifier = new QPushButton(page);
        btnModifier->setObjectName("btnModifier");
        btnModifier->setGeometry(QRect(410, 590, 171, 31));
        btnModifier->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: rgb(13, 59, 102); /* Bleu fonc\303\251 */\n"
"    color: rgb(248, 249, 250);          /* Texte blanc */\n"
"    font: italic 9pt \"Segoe UI\";\n"
"}\n"
"\n"
""));
        btnSupprimer = new QPushButton(page);
        btnSupprimer->setObjectName("btnSupprimer");
        btnSupprimer->setGeometry(QRect(220, 590, 151, 31));
        btnSupprimer->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"	color: rgb(0, 0, 0);\n"
"    background-color: rgb(13, 59, 102); /* Bleu fonc\303\251 */\n"
"    color: rgb(248, 249, 250);          /* Texte blanc */\n"
"    font: italic 9pt \"Segoe UI\";\n"
"}\n"
""));
        btnAjouter = new QPushButton(page);
        btnAjouter->setObjectName("btnAjouter");
        btnAjouter->setGeometry(QRect(40, 590, 141, 31));
        btnAjouter->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: rgb(13, 59, 102); /* Bleu fonc\303\251 */\n"
"    color: rgb(248, 249, 250);          /* Texte blanc */\n"
"    font: italic 9pt \"Segoe UI\";\n"
"}\n"
""));
        label_5 = new QLabel(page);
        label_5->setObjectName("label_5");
        label_5->setGeometry(QRect(290, 40, 371, 41));
        QFont font;
        font.setFamilies({QString::fromUtf8("Arial")});
        font.setWeight(QFont::Black);
        label_5->setFont(font);
        label_5->setStyleSheet(QString::fromUtf8("QLabel {\n"
"    font-family: Arial;       /* Police */\n"
"    font-size: 18px;          /* Taille plus grande */\n"
"    font-weight: 900;         /* Tr\303\250s gras (Black) */\n"
"    color: #00008B;           /* Bleu fonc\303\251 */\n"
"}\n"
""));
        btnafficher = new QPushButton(page);
        btnafficher->setObjectName("btnafficher");
        btnafficher->setGeometry(QRect(310, 640, 141, 31));
        btnafficher->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: rgb(13, 59, 102); /* Bleu fonc\303\251 */\n"
"    color: rgb(248, 249, 250);          /* Texte blanc */\n"
"    font: italic 9pt \"Segoe UI\";\n"
"}\n"
""));
        stackedWidget->addWidget(page);
        page_5 = new QWidget();
        page_5->setObjectName("page_5");
        label = new QLabel(page_5);
        label->setObjectName("label");
        label->setGeometry(QRect(280, 10, 561, 41));
        label->setStyleSheet(QString::fromUtf8("QLabel {\n"
"    font-family: Arial;       /* Police */\n"
"    font-size: 18px;          /* Taille plus grande */\n"
"    font-weight: 900;         /* Tr\303\250s gras (Black) */\n"
"    color: #00008B;           /* Bleu fonc\303\251 */\n"
"}\n"
""));
        label_2 = new QLabel(page_5);
        label_2->setObjectName("label_2");
        label_2->setGeometry(QRect(50, 90, 121, 21));
        comboBoxtype2 = new QComboBox(page_5);
        comboBoxtype2->addItem(QString());
        comboBoxtype2->addItem(QString());
        comboBoxtype2->setObjectName("comboBoxtype2");
        comboBoxtype2->setGeometry(QRect(300, 70, 91, 41));
        label_3 = new QLabel(page_5);
        label_3->setObjectName("label_3");
        label_3->setGeometry(QRect(60, 60, 49, 16));
        label_filtremaison = new QLabel(page_5);
        label_filtremaison->setObjectName("label_filtremaison");
        label_filtremaison->setGeometry(QRect(80, 80, 111, 31));
        comboBox_2 = new QComboBox(page_5);
        comboBox_2->setObjectName("comboBox_2");
        comboBox_2->setGeometry(QRect(200, 70, 68, 41));
        pushButtonrafraichir = new QPushButton(page_5);
        pushButtonrafraichir->setObjectName("pushButtonrafraichir");
        pushButtonrafraichir->setGeometry(QRect(420, 70, 131, 41));
        table_surveillance = new QTableWidget(page_5);
        if (table_surveillance->columnCount() < 7)
            table_surveillance->setColumnCount(7);
        QTableWidgetItem *__qtablewidgetitem5 = new QTableWidgetItem();
        table_surveillance->setHorizontalHeaderItem(0, __qtablewidgetitem5);
        QTableWidgetItem *__qtablewidgetitem6 = new QTableWidgetItem();
        table_surveillance->setHorizontalHeaderItem(1, __qtablewidgetitem6);
        QTableWidgetItem *__qtablewidgetitem7 = new QTableWidgetItem();
        table_surveillance->setHorizontalHeaderItem(2, __qtablewidgetitem7);
        QTableWidgetItem *__qtablewidgetitem8 = new QTableWidgetItem();
        table_surveillance->setHorizontalHeaderItem(3, __qtablewidgetitem8);
        QTableWidgetItem *__qtablewidgetitem9 = new QTableWidgetItem();
        table_surveillance->setHorizontalHeaderItem(4, __qtablewidgetitem9);
        QFont font1;
        font1.setWeight(QFont::Light);
        QTableWidgetItem *__qtablewidgetitem10 = new QTableWidgetItem();
        __qtablewidgetitem10->setFont(font1);
        table_surveillance->setHorizontalHeaderItem(5, __qtablewidgetitem10);
        QTableWidgetItem *__qtablewidgetitem11 = new QTableWidgetItem();
        table_surveillance->setHorizontalHeaderItem(6, __qtablewidgetitem11);
        table_surveillance->setObjectName("table_surveillance");
        table_surveillance->setGeometry(QRect(0, 170, 911, 401));
        table_surveillance->setStyleSheet(QString::fromUtf8("/* =====================\n"
"   Tableau central (QTableWidget)\n"
"   ===================== */\n"
"QTableWidget {\n"
"    background-color: rgb(240, 240, 240);  /* Fond g\303\251n\303\251ral du tableau */\n"
"    gridline-color: rgb(204, 204, 204);    /* Couleur des lignes de s\303\251paration */\n"
"    border: 2px solid #2980b9;             /* Bordure du tableau */\n"
"    font: 9pt \"Segoe UI\";                  /* Police du contenu des cellules */\n"
"}\n"
"\n"
"/* En-t\303\252tes du tableau */\n"
"QHeaderView::section {\n"
"    background-color: rgb(13, 59, 102);    /* Fond des en-t\303\252tes */\n"
"    color: rgb(248, 249, 250);             /* Texte des en-t\303\252tes */\n"
"    font-weight: bold;                      /* Texte en gras */\n"
"    padding: 5px;                           /* Espacement int\303\251rieur */\n"
"    border: 1px solid #2980b9;             /* Bordure de l\342\200\231en-t\303\252te */\n"
"}\n"
"\n"
"/* Les cellules */\n"
"QTableWidget::item {\n"
"    background-color: rgb(255, 2"
                        "55, 255);  /* Fond blanc des cellules */\n"
"    color: rgb(0, 51, 102);                /* Texte bleu fonc\303\251 */\n"
"    padding: 4px;                           /* Espacement int\303\251rieur */\n"
"    selection-background-color: rgb(41, 128, 185); /* Fond d\342\200\231une cellule s\303\251lectionn\303\251e */\n"
"    selection-color: white;                 /* Texte de la cellule s\303\251lectionn\303\251e */\n"
"}\n"
"\n"
"/* Lignes impaires et paires (optionnel pour effet z\303\251br\303\251) */\n"
"QTableWidget::item:!selected:alternate {\n"
"    background-color: rgb(245, 245, 245);  /* Fond gris tr\303\250s clair pour les lignes altern\303\251es */\n"
"}\n"
""));
        pushButton_exportsurveillence = new QPushButton(page_5);
        pushButton_exportsurveillence->setObjectName("pushButton_exportsurveillence");
        pushButton_exportsurveillence->setGeometry(QRect(130, 590, 571, 41));
        pushButton_exportsurveillence->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: rgb(13, 59, 102); /* Bleu fonc\303\251 */\n"
"    color: rgb(248, 249, 250);          /* Texte blanc */\n"
"    font: italic 9pt \"Segoe UI\";\n"
"}\n"
""));
        stackedWidget->addWidget(page_5);
        page_2 = new QWidget();
        page_2->setObjectName("page_2");
        label_12 = new QLabel(page_2);
        label_12->setObjectName("label_12");
        label_12->setGeometry(QRect(800, 740, 111, 31));
        label_13 = new QLabel(page_2);
        label_13->setObjectName("label_13");
        label_13->setGeometry(QRect(800, 740, 111, 31));
        comboBox_4 = new QComboBox(page_2);
        comboBox_4->addItem(QString());
        comboBox_4->addItem(QString());
        comboBox_4->setObjectName("comboBox_4");
        comboBox_4->setGeometry(QRect(240, 70, 91, 41));
        pushButton_6 = new QPushButton(page_2);
        pushButton_6->setObjectName("pushButton_6");
        pushButton_6->setGeometry(QRect(390, 70, 131, 41));
        tableWidget_2 = new QTableWidget(page_2);
        if (tableWidget_2->columnCount() < 10)
            tableWidget_2->setColumnCount(10);
        QTableWidgetItem *__qtablewidgetitem12 = new QTableWidgetItem();
        tableWidget_2->setHorizontalHeaderItem(0, __qtablewidgetitem12);
        QTableWidgetItem *__qtablewidgetitem13 = new QTableWidgetItem();
        tableWidget_2->setHorizontalHeaderItem(1, __qtablewidgetitem13);
        QTableWidgetItem *__qtablewidgetitem14 = new QTableWidgetItem();
        tableWidget_2->setHorizontalHeaderItem(2, __qtablewidgetitem14);
        QTableWidgetItem *__qtablewidgetitem15 = new QTableWidgetItem();
        tableWidget_2->setHorizontalHeaderItem(3, __qtablewidgetitem15);
        QTableWidgetItem *__qtablewidgetitem16 = new QTableWidgetItem();
        tableWidget_2->setHorizontalHeaderItem(4, __qtablewidgetitem16);
        QTableWidgetItem *__qtablewidgetitem17 = new QTableWidgetItem();
        tableWidget_2->setHorizontalHeaderItem(5, __qtablewidgetitem17);
        QTableWidgetItem *__qtablewidgetitem18 = new QTableWidgetItem();
        tableWidget_2->setHorizontalHeaderItem(6, __qtablewidgetitem18);
        QTableWidgetItem *__qtablewidgetitem19 = new QTableWidgetItem();
        tableWidget_2->setHorizontalHeaderItem(7, __qtablewidgetitem19);
        QTableWidgetItem *__qtablewidgetitem20 = new QTableWidgetItem();
        __qtablewidgetitem20->setFont(font1);
        tableWidget_2->setHorizontalHeaderItem(8, __qtablewidgetitem20);
        QTableWidgetItem *__qtablewidgetitem21 = new QTableWidgetItem();
        tableWidget_2->setHorizontalHeaderItem(9, __qtablewidgetitem21);
        tableWidget_2->setObjectName("tableWidget_2");
        tableWidget_2->setGeometry(QRect(10, 150, 1101, 401));
        tableWidget_2->setStyleSheet(QString::fromUtf8("/* =====================\n"
"   Tableau central (QTableWidget)\n"
"   ===================== */\n"
"QTableWidget {\n"
"    background-color: rgb(240, 240, 240);  /* Fond g\303\251n\303\251ral du tableau */\n"
"    gridline-color: rgb(204, 204, 204);    /* Couleur des lignes de s\303\251paration */\n"
"    border: 2px solid #2980b9;             /* Bordure du tableau */\n"
"    font: 9pt \"Segoe UI\";                  /* Police du contenu des cellules */\n"
"}\n"
"\n"
"/* En-t\303\252tes du tableau */\n"
"QHeaderView::section {\n"
"    background-color: rgb(13, 59, 102);    /* Fond des en-t\303\252tes */\n"
"    color: rgb(248, 249, 250);             /* Texte des en-t\303\252tes */\n"
"    font-weight: bold;                      /* Texte en gras */\n"
"    padding: 5px;                           /* Espacement int\303\251rieur */\n"
"    border: 1px solid #2980b9;             /* Bordure de l\342\200\231en-t\303\252te */\n"
"}\n"
"\n"
"/* Les cellules */\n"
"QTableWidget::item {\n"
"    background-color: rgb(255, 2"
                        "55, 255);  /* Fond blanc des cellules */\n"
"    color: rgb(0, 51, 102);                /* Texte bleu fonc\303\251 */\n"
"    padding: 4px;                           /* Espacement int\303\251rieur */\n"
"    selection-background-color: rgb(41, 128, 185); /* Fond d\342\200\231une cellule s\303\251lectionn\303\251e */\n"
"    selection-color: white;                 /* Texte de la cellule s\303\251lectionn\303\251e */\n"
"}\n"
"\n"
"/* Lignes impaires et paires (optionnel pour effet z\303\251br\303\251) */\n"
"QTableWidget::item:!selected:alternate {\n"
"    background-color: rgb(245, 245, 245);  /* Fond gris tr\303\250s clair pour les lignes altern\303\251es */\n"
"}\n"
""));
        pushbuttonexportanomalie = new QPushButton(page_2);
        pushbuttonexportanomalie->setObjectName("pushbuttonexportanomalie");
        pushbuttonexportanomalie->setGeometry(QRect(190, 570, 571, 41));
        pushbuttonexportanomalie->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: rgb(13, 59, 102); /* Bleu fonc\303\251 */\n"
"    color: rgb(248, 249, 250);          /* Texte blanc */\n"
"    font: italic 9pt \"Segoe UI\";\n"
"}\n"
""));
        label_7 = new QLabel(page_2);
        label_7->setObjectName("label_7");
        label_7->setGeometry(QRect(240, 20, 561, 41));
        label_7->setStyleSheet(QString::fromUtf8("QLabel {\n"
"    font-family: Arial;       /* Police */\n"
"    font-size: 18px;          /* Taille plus grande */\n"
"    font-weight: 900;         /* Tr\303\250s gras (Black) */\n"
"    color: #00008B;           /* Bleu fonc\303\251 */\n"
"}\n"
""));
        stackedWidget->addWidget(page_2);
        page_6 = new QWidget();
        page_6->setObjectName("page_6");
        comboBox_typeconso_2 = new QComboBox(page_6);
        comboBox_typeconso_2->addItem(QString());
        comboBox_typeconso_2->addItem(QString());
        comboBox_typeconso_2->setObjectName("comboBox_typeconso_2");
        comboBox_typeconso_2->setGeometry(QRect(300, 50, 91, 41));
        pushrafraichiralerte = new QPushButton(page_6);
        pushrafraichiralerte->setObjectName("pushrafraichiralerte");
        pushrafraichiralerte->setGeometry(QRect(450, 50, 131, 41));
        tableWidget_3 = new QTableWidget(page_6);
        if (tableWidget_3->columnCount() < 12)
            tableWidget_3->setColumnCount(12);
        QTableWidgetItem *__qtablewidgetitem22 = new QTableWidgetItem();
        tableWidget_3->setHorizontalHeaderItem(0, __qtablewidgetitem22);
        QTableWidgetItem *__qtablewidgetitem23 = new QTableWidgetItem();
        tableWidget_3->setHorizontalHeaderItem(1, __qtablewidgetitem23);
        QTableWidgetItem *__qtablewidgetitem24 = new QTableWidgetItem();
        tableWidget_3->setHorizontalHeaderItem(2, __qtablewidgetitem24);
        QTableWidgetItem *__qtablewidgetitem25 = new QTableWidgetItem();
        tableWidget_3->setHorizontalHeaderItem(3, __qtablewidgetitem25);
        QTableWidgetItem *__qtablewidgetitem26 = new QTableWidgetItem();
        tableWidget_3->setHorizontalHeaderItem(4, __qtablewidgetitem26);
        QTableWidgetItem *__qtablewidgetitem27 = new QTableWidgetItem();
        tableWidget_3->setHorizontalHeaderItem(5, __qtablewidgetitem27);
        QTableWidgetItem *__qtablewidgetitem28 = new QTableWidgetItem();
        tableWidget_3->setHorizontalHeaderItem(6, __qtablewidgetitem28);
        QTableWidgetItem *__qtablewidgetitem29 = new QTableWidgetItem();
        tableWidget_3->setHorizontalHeaderItem(7, __qtablewidgetitem29);
        QTableWidgetItem *__qtablewidgetitem30 = new QTableWidgetItem();
        tableWidget_3->setHorizontalHeaderItem(8, __qtablewidgetitem30);
        QTableWidgetItem *__qtablewidgetitem31 = new QTableWidgetItem();
        __qtablewidgetitem31->setFont(font1);
        tableWidget_3->setHorizontalHeaderItem(9, __qtablewidgetitem31);
        QTableWidgetItem *__qtablewidgetitem32 = new QTableWidgetItem();
        tableWidget_3->setHorizontalHeaderItem(10, __qtablewidgetitem32);
        QTableWidgetItem *__qtablewidgetitem33 = new QTableWidgetItem();
        tableWidget_3->setHorizontalHeaderItem(11, __qtablewidgetitem33);
        tableWidget_3->setObjectName("tableWidget_3");
        tableWidget_3->setGeometry(QRect(10, 130, 1111, 371));
        tableWidget_3->setStyleSheet(QString::fromUtf8("/* =====================\n"
"   Tableau central (QTableWidget)\n"
"   ===================== */\n"
"QTableWidget {\n"
"    background-color: rgb(240, 240, 240);  /* Fond g\303\251n\303\251ral du tableau */\n"
"    gridline-color: rgb(204, 204, 204);    /* Couleur des lignes de s\303\251paration */\n"
"    border: 2px solid #2980b9;             /* Bordure du tableau */\n"
"    font: 9pt \"Segoe UI\";                  /* Police du contenu des cellules */\n"
"}\n"
"\n"
"/* En-t\303\252tes du tableau */\n"
"QHeaderView::section {\n"
"    background-color: rgb(13, 59, 102);    /* Fond des en-t\303\252tes */\n"
"    color: rgb(248, 249, 250);             /* Texte des en-t\303\252tes */\n"
"    font-weight: bold;                      /* Texte en gras */\n"
"    padding: 5px;                           /* Espacement int\303\251rieur */\n"
"    border: 1px solid #2980b9;             /* Bordure de l\342\200\231en-t\303\252te */\n"
"}\n"
"\n"
"/* Les cellules */\n"
"QTableWidget::item {\n"
"    background-color: rgb(255, 2"
                        "55, 255);  /* Fond blanc des cellules */\n"
"    color: rgb(0, 51, 102);                /* Texte bleu fonc\303\251 */\n"
"    padding: 4px;                           /* Espacement int\303\251rieur */\n"
"    selection-background-color: rgb(41, 128, 185); /* Fond d\342\200\231une cellule s\303\251lectionn\303\251e */\n"
"    selection-color: white;                 /* Texte de la cellule s\303\251lectionn\303\251e */\n"
"}\n"
"\n"
"/* Lignes impaires et paires (optionnel pour effet z\303\251br\303\251) */\n"
"QTableWidget::item:!selected:alternate {\n"
"    background-color: rgb(245, 245, 245);  /* Fond gris tr\303\250s clair pour les lignes altern\303\251es */\n"
"}\n"
""));
        pushButton_10 = new QPushButton(page_6);
        pushButton_10->setObjectName("pushButton_10");
        pushButton_10->setGeometry(QRect(30, 530, 161, 31));
        pushButton_10->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: rgb(13, 59, 102); /* Bleu fonc\303\251 */\n"
"    color: rgb(248, 249, 250);          /* Texte blanc */\n"
"    font: italic 9pt \"Segoe UI\";\n"
"}\n"
""));
        pushButton_11 = new QPushButton(page_6);
        pushButton_11->setObjectName("pushButton_11");
        pushButton_11->setGeometry(QRect(200, 530, 181, 31));
        pushButton_11->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: rgb(13, 59, 102); /* Bleu fonc\303\251 */\n"
"    color: rgb(248, 249, 250);          /* Texte blanc */\n"
"    font: italic 9pt \"Segoe UI\";\n"
"}\n"
""));
        pushButton_12 = new QPushButton(page_6);
        pushButton_12->setObjectName("pushButton_12");
        pushButton_12->setGeometry(QRect(390, 530, 181, 31));
        pushButton_12->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: rgb(13, 59, 102); /* Bleu fonc\303\251 */\n"
"    color: rgb(248, 249, 250);          /* Texte blanc */\n"
"    font: italic 9pt \"Segoe UI\";\n"
"}\n"
""));
        pushButton_13 = new QPushButton(page_6);
        pushButton_13->setObjectName("pushButton_13");
        pushButton_13->setGeometry(QRect(590, 530, 171, 31));
        pushButton_13->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: rgb(13, 59, 102); /* Bleu fonc\303\251 */\n"
"    color: rgb(248, 249, 250);          /* Texte blanc */\n"
"    font: italic 9pt \"Segoe UI\";\n"
"}\n"
""));
        label_8 = new QLabel(page_6);
        label_8->setObjectName("label_8");
        label_8->setGeometry(QRect(330, 10, 351, 16));
        label_8->setStyleSheet(QString::fromUtf8("QLabel {\n"
"    font-family: Arial;       /* Police */\n"
"    font-size: 18px;          /* Taille plus grande */\n"
"    font-weight: 900;         /* Tr\303\250s gras (Black) */\n"
"    color: #00008B;           /* Bleu fonc\303\251 */\n"
"}\n"
"\n"
""));
        label_9 = new QLabel(page_6);
        label_9->setObjectName("label_9");
        label_9->setGeometry(QRect(30, 60, 81, 20));
        lineEdit_surface_2 = new QLineEdit(page_6);
        lineEdit_surface_2->setObjectName("lineEdit_surface_2");
        lineEdit_surface_2->setGeometry(QRect(90, 60, 141, 26));
        stackedWidget->addWidget(page_6);
        Ghabitant_3 = new QPushButton(centralwidget);
        Ghabitant_3->setObjectName("Ghabitant_3");
        Ghabitant_3->setGeometry(QRect(-10, 400, 191, 71));
        Ghabitant_3->setStyleSheet(QString::fromUtf8("color: rgb(248, 249, 250);\n"
"font: italic 9pt \"Segoe UI\";\n"
"background-color: rgb(13, 59, 102);"));
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 1835, 26));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        stackedWidget->setCurrentIndex(0);
        stackedWidget_2->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        Ghabitant_4->setText(QCoreApplication::translate("MainWindow", "Gestion v\303\251hicules", nullptr));
        Ghabitant_6->setText(QCoreApplication::translate("MainWindow", "Gestion habitants", nullptr));
        gestion_button->setText(QCoreApplication::translate("MainWindow", "Gestion des logements", nullptr));
        surveilalnce_button->setText(QCoreApplication::translate("MainWindow", "Surveillance intelligente", nullptr));
        Ghabitant->setText(QCoreApplication::translate("MainWindow", "Gestion habitants", nullptr));
        Ghabitant_2->setText(QCoreApplication::translate("MainWindow", "Gestion habitats", nullptr));
        Ghabitant_5->setText(QCoreApplication::translate("MainWindow", "Gestion de personnel", nullptr));
        pushButton_16->setText(QCoreApplication::translate("MainWindow", "\360\237\224\224 ALERTES CONSOMMATEURS", nullptr));
        pushButton_15->setText(QCoreApplication::translate("MainWindow", "D\303\251tection des anomalies", nullptr));
        label_110->setText(QCoreApplication::translate("MainWindow", "Nb personnes : ", nullptr));
        label_108->setText(QCoreApplication::translate("MainWindow", "Id :", nullptr));
        label_109->setText(QCoreApplication::translate("MainWindow", "Adresse :", nullptr));
        label_recherche->setText(QCoreApplication::translate("MainWindow", "Recherche :", nullptr));
        label_111->setText(QCoreApplication::translate("MainWindow", "Surface :", nullptr));
        label_112->setText(QCoreApplication::translate("MainWindow", "Type habitat : ", nullptr));
        QTableWidgetItem *___qtablewidgetitem = TabCrudHabitat->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QCoreApplication::translate("MainWindow", "Id", nullptr));
        QTableWidgetItem *___qtablewidgetitem1 = TabCrudHabitat->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QCoreApplication::translate("MainWindow", "Adresse", nullptr));
        QTableWidgetItem *___qtablewidgetitem2 = TabCrudHabitat->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QCoreApplication::translate("MainWindow", "Nb personnes ", nullptr));
        QTableWidgetItem *___qtablewidgetitem3 = TabCrudHabitat->horizontalHeaderItem(3);
        ___qtablewidgetitem3->setText(QCoreApplication::translate("MainWindow", "Surface", nullptr));
        QTableWidgetItem *___qtablewidgetitem4 = TabCrudHabitat->horizontalHeaderItem(4);
        ___qtablewidgetitem4->setText(QCoreApplication::translate("MainWindow", "Type Habitat ", nullptr));
        pushButton_exporter->setText(QCoreApplication::translate("MainWindow", "Exporter \360\237\223\202", nullptr));
        btnModifier->setText(QCoreApplication::translate("MainWindow", "Modifier \342\234\217\357\270\217", nullptr));
        btnSupprimer->setText(QCoreApplication::translate("MainWindow", "Supprimer \342\235\214", nullptr));
        btnAjouter->setText(QCoreApplication::translate("MainWindow", "Ajouter", nullptr));
        label_5->setText(QCoreApplication::translate("MainWindow", "Habitat et Gaspillage ", nullptr));
        btnafficher->setText(QCoreApplication::translate("MainWindow", "Statistiques", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "\360\237\214\277 SURVEILLANCE INTELLIGENTE \342\200\224 TEMPS R\303\211EL     ", nullptr));
        label_2->setText(QString());
        comboBoxtype2->setItemText(0, QCoreApplication::translate("MainWindow", "eau ", nullptr));
        comboBoxtype2->setItemText(1, QCoreApplication::translate("MainWindow", "electricit\303\251", nullptr));

        label_3->setText(QString());
        label_filtremaison->setText(QCoreApplication::translate("MainWindow", "Filtre par maison", nullptr));
        pushButtonrafraichir->setText(QCoreApplication::translate("MainWindow", "Rafraichir ", nullptr));
        QTableWidgetItem *___qtablewidgetitem5 = table_surveillance->horizontalHeaderItem(0);
        ___qtablewidgetitem5->setText(QCoreApplication::translate("MainWindow", "Maison", nullptr));
        QTableWidgetItem *___qtablewidgetitem6 = table_surveillance->horizontalHeaderItem(1);
        ___qtablewidgetitem6->setText(QCoreApplication::translate("MainWindow", "Eau", nullptr));
        QTableWidgetItem *___qtablewidgetitem7 = table_surveillance->horizontalHeaderItem(2);
        ___qtablewidgetitem7->setText(QCoreApplication::translate("MainWindow", "Electricit\303\251", nullptr));
        QTableWidgetItem *___qtablewidgetitem8 = table_surveillance->horizontalHeaderItem(3);
        ___qtablewidgetitem8->setText(QCoreApplication::translate("MainWindow", "Consommation normale ", nullptr));
        QTableWidgetItem *___qtablewidgetitem9 = table_surveillance->horizontalHeaderItem(4);
        ___qtablewidgetitem9->setText(QCoreApplication::translate("MainWindow", "Moyenne de consommation", nullptr));
        QTableWidgetItem *___qtablewidgetitem10 = table_surveillance->horizontalHeaderItem(5);
        ___qtablewidgetitem10->setText(QCoreApplication::translate("MainWindow", "Etat", nullptr));
        pushButton_exportsurveillence->setText(QCoreApplication::translate("MainWindow", "Exporter \360\237\223\202", nullptr));
        label_12->setText(QCoreApplication::translate("MainWindow", "Filtre par maison", nullptr));
        label_13->setText(QCoreApplication::translate("MainWindow", "Filtre par maison", nullptr));
        comboBox_4->setItemText(0, QCoreApplication::translate("MainWindow", "eau ", nullptr));
        comboBox_4->setItemText(1, QCoreApplication::translate("MainWindow", "electricit\303\251", nullptr));

        pushButton_6->setText(QCoreApplication::translate("MainWindow", "Rafraichir ", nullptr));
        QTableWidgetItem *___qtablewidgetitem11 = tableWidget_2->horizontalHeaderItem(0);
        ___qtablewidgetitem11->setText(QCoreApplication::translate("MainWindow", "Date", nullptr));
        QTableWidgetItem *___qtablewidgetitem12 = tableWidget_2->horizontalHeaderItem(1);
        ___qtablewidgetitem12->setText(QCoreApplication::translate("MainWindow", "Id_anomalie", nullptr));
        QTableWidgetItem *___qtablewidgetitem13 = tableWidget_2->horizontalHeaderItem(2);
        ___qtablewidgetitem13->setText(QCoreApplication::translate("MainWindow", "Maison", nullptr));
        QTableWidgetItem *___qtablewidgetitem14 = tableWidget_2->horizontalHeaderItem(3);
        ___qtablewidgetitem14->setText(QCoreApplication::translate("MainWindow", "Eau", nullptr));
        QTableWidgetItem *___qtablewidgetitem15 = tableWidget_2->horizontalHeaderItem(4);
        ___qtablewidgetitem15->setText(QCoreApplication::translate("MainWindow", "Electricit\303\251", nullptr));
        QTableWidgetItem *___qtablewidgetitem16 = tableWidget_2->horizontalHeaderItem(5);
        ___qtablewidgetitem16->setText(QCoreApplication::translate("MainWindow", "Consommation normale ", nullptr));
        QTableWidgetItem *___qtablewidgetitem17 = tableWidget_2->horizontalHeaderItem(6);
        ___qtablewidgetitem17->setText(QCoreApplication::translate("MainWindow", "Moyenne de consommation", nullptr));
        QTableWidgetItem *___qtablewidgetitem18 = tableWidget_2->horizontalHeaderItem(7);
        ___qtablewidgetitem18->setText(QCoreApplication::translate("MainWindow", "Cause possible ", nullptr));
        QTableWidgetItem *___qtablewidgetitem19 = tableWidget_2->horizontalHeaderItem(8);
        ___qtablewidgetitem19->setText(QCoreApplication::translate("MainWindow", "Etat", nullptr));
        pushbuttonexportanomalie->setText(QCoreApplication::translate("MainWindow", "Exporter \360\237\223\202", nullptr));
        label_7->setText(QCoreApplication::translate("MainWindow", "\360\237\214\277 Detection des anomalies ", nullptr));
        comboBox_typeconso_2->setItemText(0, QCoreApplication::translate("MainWindow", "eau ", nullptr));
        comboBox_typeconso_2->setItemText(1, QCoreApplication::translate("MainWindow", "electricit\303\251", nullptr));

        pushrafraichiralerte->setText(QCoreApplication::translate("MainWindow", "Rafraichir ", nullptr));
        QTableWidgetItem *___qtablewidgetitem20 = tableWidget_3->horizontalHeaderItem(0);
        ___qtablewidgetitem20->setText(QCoreApplication::translate("MainWindow", "Date", nullptr));
        QTableWidgetItem *___qtablewidgetitem21 = tableWidget_3->horizontalHeaderItem(1);
        ___qtablewidgetitem21->setText(QCoreApplication::translate("MainWindow", "Maison", nullptr));
        QTableWidgetItem *___qtablewidgetitem22 = tableWidget_3->horizontalHeaderItem(2);
        ___qtablewidgetitem22->setText(QCoreApplication::translate("MainWindow", "Eau", nullptr));
        QTableWidgetItem *___qtablewidgetitem23 = tableWidget_3->horizontalHeaderItem(3);
        ___qtablewidgetitem23->setText(QCoreApplication::translate("MainWindow", "Electricit\303\251", nullptr));
        QTableWidgetItem *___qtablewidgetitem24 = tableWidget_3->horizontalHeaderItem(4);
        ___qtablewidgetitem24->setText(QCoreApplication::translate("MainWindow", "Consommation normale ", nullptr));
        QTableWidgetItem *___qtablewidgetitem25 = tableWidget_3->horizontalHeaderItem(5);
        ___qtablewidgetitem25->setText(QCoreApplication::translate("MainWindow", "Moyenne de consommation", nullptr));
        QTableWidgetItem *___qtablewidgetitem26 = tableWidget_3->horizontalHeaderItem(6);
        ___qtablewidgetitem26->setText(QCoreApplication::translate("MainWindow", "New Column", nullptr));
        QTableWidgetItem *___qtablewidgetitem27 = tableWidget_3->horizontalHeaderItem(7);
        ___qtablewidgetitem27->setText(QCoreApplication::translate("MainWindow", "Cause possible ", nullptr));
        QTableWidgetItem *___qtablewidgetitem28 = tableWidget_3->horizontalHeaderItem(8);
        ___qtablewidgetitem28->setText(QCoreApplication::translate("MainWindow", "Gravit\303\251", nullptr));
        QTableWidgetItem *___qtablewidgetitem29 = tableWidget_3->horizontalHeaderItem(9);
        ___qtablewidgetitem29->setText(QCoreApplication::translate("MainWindow", "Etat", nullptr));
        QTableWidgetItem *___qtablewidgetitem30 = tableWidget_3->horizontalHeaderItem(10);
        ___qtablewidgetitem30->setText(QCoreApplication::translate("MainWindow", "SMS pret ", nullptr));
        pushButton_10->setText(QCoreApplication::translate("MainWindow", "Envoyer message s\303\251lectionn\303\251", nullptr));
        pushButton_11->setText(QCoreApplication::translate("MainWindow", "Envoyer \303\240 tous", nullptr));
        pushButton_12->setText(QCoreApplication::translate("MainWindow", "Export listes des alertes", nullptr));
        pushButton_13->setText(QCoreApplication::translate("MainWindow", "cr\303\251er une intervention", nullptr));
        label_8->setText(QCoreApplication::translate("MainWindow", "\360\237\224\224 ALERTES CONSOMMATEURS", nullptr));
        label_9->setText(QCoreApplication::translate("MainWindow", "Maison", nullptr));
        Ghabitant_3->setText(QCoreApplication::translate("MainWindow", "Gestion poubelles ", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
