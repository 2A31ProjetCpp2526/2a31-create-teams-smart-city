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
#include <QtWidgets/QDateTimeEdit>
#include <QtWidgets/QGroupBox>
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
    QStackedWidget *stackedWidget;
    QWidget *page_gestionhabitat;
    QLineEdit *ADRESSE;
    QLabel *label_110;
    QLabel *label_108;
    QLabel *label_109;
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
    QLabel *label_4;
    QLineEdit *TELEPHONE_CONTACT;
    QLabel *label_7;
    QWidget *page_surveillancehabitat;
    QLabel *label;
    QLabel *label_2;
    QLabel *label_3;
    QLineEdit *EAU_CONSO;
    QLineEdit *MOY_CONSO;
    QLineEdit *ETAT_EAU;
    QLabel *label_10;
    QLabel *ETAT_EAU_2;
    QLabel *label_14;
    QLabel *label_15;
    QDateTimeEdit *DATE_RELEVE;
    QLabel *label_6;
    QLineEdit *ID_conso;
    QPushButton *btnModifierConso;
    QLineEdit *ALERTE;
    QLabel *label_16;
    QLabel *ID_HABITAT_3;
    QLineEdit *ID_HABITAT2;
    QGroupBox *groupBoxFiltres;
    QLabel *labelTri;
    QComboBox *comboTriConso;
    QLabel *labelRechHabitat;
    QLineEdit *lineRechercheHabitat;
    QLabel *labelRechEtat;
    QLineEdit *lineRechercheEtat;
    QPushButton *btnReinitialiserFiltre;
    QTableWidget *table_surveillance;
    QPushButton *btnSupprimerConso;
    QPushButton *AJOUTconso;
    QPushButton *btnAfficherConso;
    QPushButton *btnStatistiques;
    QPushButton *btnActiverAlertesAuto;
    QPushButton *btnVerifierSMSEntrants;
    QStackedWidget *stackedWidget_3;
    QWidget *page_gestionhabitat_2;
    QLineEdit *ADRESSE_2;
    QLabel *label_113;
    QLabel *label_114;
    QLabel *label_115;
    QLineEdit *ID_HABITAT_2;
    QLabel *label_116;
    QLabel *label_117;
    QLineEdit *TYPE_LOGEMENT_2;
    QStackedWidget *stackedWidget_4;
    QWidget *page_2;
    QWidget *page_5;
    QWidget *page_6;
    QWidget *page_7;
    QLineEdit *SURFACE_2;
    QLineEdit *NB_PERSONNES_2;
    QTableWidget *TabCrudHabitat_2;
    QPushButton *pushButton_exporter_2;
    QPushButton *btnModifier_2;
    QPushButton *btnSupprimer_2;
    QPushButton *btnAjouter_2;
    QLabel *label_8;
    QPushButton *btnafficher_2;
    QLabel *label_9;
    QLineEdit *TELEPHONE_CONTACT_2;
    QLabel *label_11;
    QWidget *page_surveillancehabitat_2;
    QLabel *label_13;
    QLabel *label_17;
    QLineEdit *EAU_CONSO_2;
    QLineEdit *MOY_CONSO_2;
    QLineEdit *ETAT_EAU_3;
    QLabel *label_18;
    QLabel *ETAT_EAU_4;
    QLabel *label_19;
    QLabel *label_20;
    QDateTimeEdit *DATE_RELEVE_2;
    QLabel *label_21;
    QLineEdit *ID_conso_2;
    QPushButton *btnModifierConso_2;
    QLineEdit *ALERTE_2;
    QLabel *label_22;
    QLabel *ID_HABITAT_4;
    QLineEdit *ID_HABITAT2_2;
    QTableWidget *table_surveillance_2;
    QPushButton *btnSupprimerConso_2;
    QPushButton *AJOUTconso_2;
    QPushButton *btnAfficherConso_2;
    QPushButton *btnStatistiques_4;
    QPushButton *btnVerifierSMSEntrants_2;
    QPushButton *btnActiverAlertesAuto_2;
    QPushButton *btnTestSMS_2;
    QPushButton *btnStatistiques_5;
    QPushButton *btnStatistiques_6;
    QPushButton *btnTestSMS;
    QPushButton *Ghabitant_5;
    QTableView *tableView_2;
    QPushButton *Ghabitant_6;
    QPushButton *Ghabitant_4;
    QPushButton *Ghabitant_2;
    QPushButton *Ghabitant_3;
    QPushButton *surveilalnce_button;
    QPushButton *gestionhabitat_button;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(1831, 1059);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        stackedWidget = new QStackedWidget(centralwidget);
        stackedWidget->setObjectName("stackedWidget");
        stackedWidget->setGeometry(QRect(160, 90, 1641, 831));
        page_gestionhabitat = new QWidget();
        page_gestionhabitat->setObjectName("page_gestionhabitat");
        ADRESSE = new QLineEdit(page_gestionhabitat);
        ADRESSE->setObjectName("ADRESSE");
        ADRESSE->setGeometry(QRect(190, 120, 141, 26));
        label_110 = new QLabel(page_gestionhabitat);
        label_110->setObjectName("label_110");
        label_110->setGeometry(QRect(500, 90, 121, 20));
        label_110->setStyleSheet(QString::fromUtf8("color: rgb(0, 0, 0);"));
        label_108 = new QLabel(page_gestionhabitat);
        label_108->setObjectName("label_108");
        label_108->setGeometry(QRect(20, 90, 91, 20));
        label_108->setStyleSheet(QString::fromUtf8("color: rgb(0, 0, 0);"));
        label_109 = new QLabel(page_gestionhabitat);
        label_109->setObjectName("label_109");
        label_109->setGeometry(QRect(190, 90, 91, 20));
        label_109->setStyleSheet(QString::fromUtf8("color: rgb(0, 0, 0);"));
        ID_HABITAT = new QLineEdit(page_gestionhabitat);
        ID_HABITAT->setObjectName("ID_HABITAT");
        ID_HABITAT->setGeometry(QRect(20, 120, 141, 26));
        label_111 = new QLabel(page_gestionhabitat);
        label_111->setObjectName("label_111");
        label_111->setGeometry(QRect(350, 90, 91, 20));
        label_111->setStyleSheet(QString::fromUtf8("color: rgb(0, 0, 0);"));
        label_112 = new QLabel(page_gestionhabitat);
        label_112->setObjectName("label_112");
        label_112->setGeometry(QRect(660, 90, 91, 20));
        label_112->setStyleSheet(QString::fromUtf8("color: rgb(0, 0, 0);"));
        TYPE_LOGEMENT = new QLineEdit(page_gestionhabitat);
        TYPE_LOGEMENT->setObjectName("TYPE_LOGEMENT");
        TYPE_LOGEMENT->setGeometry(QRect(660, 120, 141, 26));
        stackedWidget_2 = new QStackedWidget(page_gestionhabitat);
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
        SURFACE = new QLineEdit(page_gestionhabitat);
        SURFACE->setObjectName("SURFACE");
        SURFACE->setGeometry(QRect(350, 120, 141, 26));
        NB_PERSONNES = new QLineEdit(page_gestionhabitat);
        NB_PERSONNES->setObjectName("NB_PERSONNES");
        NB_PERSONNES->setGeometry(QRect(510, 120, 141, 26));
        TabCrudHabitat = new QTableWidget(page_gestionhabitat);
        if (TabCrudHabitat->columnCount() < 6)
            TabCrudHabitat->setColumnCount(6);
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
        QTableWidgetItem *__qtablewidgetitem5 = new QTableWidgetItem();
        TabCrudHabitat->setHorizontalHeaderItem(5, __qtablewidgetitem5);
        TabCrudHabitat->setObjectName("TabCrudHabitat");
        TabCrudHabitat->setGeometry(QRect(30, 180, 621, 291));
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
        pushButton_exporter = new QPushButton(page_gestionhabitat);
        pushButton_exporter->setObjectName("pushButton_exporter");
        pushButton_exporter->setGeometry(QRect(570, 490, 121, 31));
        pushButton_exporter->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: rgb(13, 59, 102); /* Bleu fonc\303\251 */\n"
"    color: rgb(248, 249, 250);          /* Texte blanc */\n"
"    font: italic 9pt \"Segoe UI\";\n"
"}\n"
""));
        btnModifier = new QPushButton(page_gestionhabitat);
        btnModifier->setObjectName("btnModifier");
        btnModifier->setGeometry(QRect(380, 490, 171, 31));
        btnModifier->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: rgb(13, 59, 102); /* Bleu fonc\303\251 */\n"
"    color: rgb(248, 249, 250);          /* Texte blanc */\n"
"    font: italic 9pt \"Segoe UI\";\n"
"}\n"
"\n"
""));
        btnSupprimer = new QPushButton(page_gestionhabitat);
        btnSupprimer->setObjectName("btnSupprimer");
        btnSupprimer->setGeometry(QRect(200, 490, 151, 31));
        btnSupprimer->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"	color: rgb(0, 0, 0);\n"
"    background-color: rgb(13, 59, 102); /* Bleu fonc\303\251 */\n"
"    color: rgb(248, 249, 250);          /* Texte blanc */\n"
"    font: italic 9pt \"Segoe UI\";\n"
"}\n"
""));
        btnAjouter = new QPushButton(page_gestionhabitat);
        btnAjouter->setObjectName("btnAjouter");
        btnAjouter->setGeometry(QRect(40, 490, 141, 31));
        btnAjouter->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: rgb(13, 59, 102); /* Bleu fonc\303\251 */\n"
"    color: rgb(248, 249, 250);          /* Texte blanc */\n"
"    font: italic 9pt \"Segoe UI\";\n"
"}\n"
""));
        label_5 = new QLabel(page_gestionhabitat);
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
        btnafficher = new QPushButton(page_gestionhabitat);
        btnafficher->setObjectName("btnafficher");
        btnafficher->setGeometry(QRect(300, 540, 141, 31));
        btnafficher->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: rgb(13, 59, 102); /* Bleu fonc\303\251 */\n"
"    color: rgb(248, 249, 250);          /* Texte blanc */\n"
"    font: italic 9pt \"Segoe UI\";\n"
"}\n"
""));
        label_4 = new QLabel(page_gestionhabitat);
        label_4->setObjectName("label_4");
        label_4->setGeometry(QRect(820, 90, 71, 16));
        TELEPHONE_CONTACT = new QLineEdit(page_gestionhabitat);
        TELEPHONE_CONTACT->setObjectName("TELEPHONE_CONTACT");
        TELEPHONE_CONTACT->setGeometry(QRect(820, 120, 141, 31));
        label_7 = new QLabel(page_gestionhabitat);
        label_7->setObjectName("label_7");
        label_7->setGeometry(QRect(820, 90, 91, 16));
        stackedWidget->addWidget(page_gestionhabitat);
        page_surveillancehabitat = new QWidget();
        page_surveillancehabitat->setObjectName("page_surveillancehabitat");
        label = new QLabel(page_surveillancehabitat);
        label->setObjectName("label");
        label->setGeometry(QRect(140, 0, 561, 41));
        label->setStyleSheet(QString::fromUtf8("QLabel {\n"
"    font-family: Arial;       /* Police */\n"
"    font-size: 18px;          /* Taille plus grande */\n"
"    font-weight: 900;         /* Tr\303\250s gras (Black) */\n"
"    color: #00008B;           /* Bleu fonc\303\251 */\n"
"}\n"
""));
        label_2 = new QLabel(page_surveillancehabitat);
        label_2->setObjectName("label_2");
        label_2->setGeometry(QRect(50, 90, 121, 21));
        label_3 = new QLabel(page_surveillancehabitat);
        label_3->setObjectName("label_3");
        label_3->setGeometry(QRect(60, 60, 49, 16));
        EAU_CONSO = new QLineEdit(page_surveillancehabitat);
        EAU_CONSO->setObjectName("EAU_CONSO");
        EAU_CONSO->setGeometry(QRect(460, 260, 81, 21));
        MOY_CONSO = new QLineEdit(page_surveillancehabitat);
        MOY_CONSO->setObjectName("MOY_CONSO");
        MOY_CONSO->setGeometry(QRect(650, 260, 61, 21));
        ETAT_EAU = new QLineEdit(page_surveillancehabitat);
        ETAT_EAU->setObjectName("ETAT_EAU");
        ETAT_EAU->setGeometry(QRect(790, 260, 91, 21));
        label_10 = new QLabel(page_surveillancehabitat);
        label_10->setObjectName("label_10");
        label_10->setGeometry(QRect(550, 260, 111, 20));
        ETAT_EAU_2 = new QLabel(page_surveillancehabitat);
        ETAT_EAU_2->setObjectName("ETAT_EAU_2");
        ETAT_EAU_2->setGeometry(QRect(720, 220, 111, 91));
        label_14 = new QLabel(page_surveillancehabitat);
        label_14->setObjectName("label_14");
        label_14->setGeometry(QRect(370, 260, 111, 20));
        label_15 = new QLabel(page_surveillancehabitat);
        label_15->setObjectName("label_15");
        label_15->setGeometry(QRect(1020, 260, 111, 20));
        DATE_RELEVE = new QDateTimeEdit(page_surveillancehabitat);
        DATE_RELEVE->setObjectName("DATE_RELEVE");
        DATE_RELEVE->setGeometry(QRect(1120, 260, 151, 23));
        label_6 = new QLabel(page_surveillancehabitat);
        label_6->setObjectName("label_6");
        label_6->setGeometry(QRect(190, 260, 71, 16));
        ID_conso = new QLineEdit(page_surveillancehabitat);
        ID_conso->setObjectName("ID_conso");
        ID_conso->setGeometry(QRect(270, 260, 91, 21));
        btnModifierConso = new QPushButton(page_surveillancehabitat);
        btnModifierConso->setObjectName("btnModifierConso");
        btnModifierConso->setGeometry(QRect(10, 520, 81, 41));
        btnModifierConso->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: rgb(13, 59, 102); /* Bleu fonc\303\251 */\n"
"    color: rgb(248, 249, 250);          /* Texte blanc */\n"
"    font: italic 9pt \"Segoe UI\";\n"
"}\n"
"\n"
""));
        ALERTE = new QLineEdit(page_surveillancehabitat);
        ALERTE->setObjectName("ALERTE");
        ALERTE->setGeometry(QRect(950, 260, 61, 21));
        label_16 = new QLabel(page_surveillancehabitat);
        label_16->setObjectName("label_16");
        label_16->setGeometry(QRect(890, 260, 49, 16));
        ID_HABITAT_3 = new QLabel(page_surveillancehabitat);
        ID_HABITAT_3->setObjectName("ID_HABITAT_3");
        ID_HABITAT_3->setGeometry(QRect(10, 260, 111, 20));
        ID_HABITAT2 = new QLineEdit(page_surveillancehabitat);
        ID_HABITAT2->setObjectName("ID_HABITAT2");
        ID_HABITAT2->setGeometry(QRect(110, 260, 71, 21));
        groupBoxFiltres = new QGroupBox(page_surveillancehabitat);
        groupBoxFiltres->setObjectName("groupBoxFiltres");
        groupBoxFiltres->setGeometry(QRect(20, 40, 931, 181));
        groupBoxFiltres->setStyleSheet(QString::fromUtf8("QGroupBox { font-weight: bold; border: 2px solid #2980b9; border-radius: 8px; margin-top: 10px; padding: 15px; background-color: #f8f9fa; } QGroupBox::title { color: #0d3b66; subcontrol-origin: margin; left: 10px; padding: 0 5px; }"));
        labelTri = new QLabel(groupBoxFiltres);
        labelTri->setObjectName("labelTri");
        labelTri->setGeometry(QRect(10, 20, 81, 21));
        comboTriConso = new QComboBox(groupBoxFiltres);
        comboTriConso->addItem(QString());
        comboTriConso->addItem(QString());
        comboTriConso->addItem(QString());
        comboTriConso->addItem(QString());
        comboTriConso->addItem(QString());
        comboTriConso->addItem(QString());
        comboTriConso->setObjectName("comboTriConso");
        comboTriConso->setGeometry(QRect(10, 50, 241, 31));
        labelRechHabitat = new QLabel(groupBoxFiltres);
        labelRechHabitat->setObjectName("labelRechHabitat");
        labelRechHabitat->setGeometry(QRect(270, 20, 121, 21));
        lineRechercheHabitat = new QLineEdit(groupBoxFiltres);
        lineRechercheHabitat->setObjectName("lineRechercheHabitat");
        lineRechercheHabitat->setGeometry(QRect(280, 50, 101, 21));
        labelRechEtat = new QLabel(groupBoxFiltres);
        labelRechEtat->setObjectName("labelRechEtat");
        labelRechEtat->setGeometry(QRect(430, 20, 121, 16));
        lineRechercheEtat = new QLineEdit(groupBoxFiltres);
        lineRechercheEtat->setObjectName("lineRechercheEtat");
        lineRechercheEtat->setGeometry(QRect(430, 50, 141, 21));
        btnReinitialiserFiltre = new QPushButton(groupBoxFiltres);
        btnReinitialiserFiltre->setObjectName("btnReinitialiserFiltre");
        btnReinitialiserFiltre->setGeometry(QRect(30, 100, 171, 31));
        btnReinitialiserFiltre->setStyleSheet(QString::fromUtf8("QPushButton { background-color: #2980b9; color: white; border: none; padding: 8px 15px; border-radius: 5px; font-weight: bold; } QPushButton:hover { background-color: #3498db; } QPushButton:pressed { background-color: #1c5980; }"));
        table_surveillance = new QTableWidget(page_surveillancehabitat);
        if (table_surveillance->columnCount() < 9)
            table_surveillance->setColumnCount(9);
        QTableWidgetItem *__qtablewidgetitem6 = new QTableWidgetItem();
        table_surveillance->setHorizontalHeaderItem(0, __qtablewidgetitem6);
        QTableWidgetItem *__qtablewidgetitem7 = new QTableWidgetItem();
        table_surveillance->setHorizontalHeaderItem(1, __qtablewidgetitem7);
        QTableWidgetItem *__qtablewidgetitem8 = new QTableWidgetItem();
        table_surveillance->setHorizontalHeaderItem(2, __qtablewidgetitem8);
        QTableWidgetItem *__qtablewidgetitem9 = new QTableWidgetItem();
        table_surveillance->setHorizontalHeaderItem(3, __qtablewidgetitem9);
        QTableWidgetItem *__qtablewidgetitem10 = new QTableWidgetItem();
        table_surveillance->setHorizontalHeaderItem(4, __qtablewidgetitem10);
        QTableWidgetItem *__qtablewidgetitem11 = new QTableWidgetItem();
        table_surveillance->setHorizontalHeaderItem(5, __qtablewidgetitem11);
        QTableWidgetItem *__qtablewidgetitem12 = new QTableWidgetItem();
        table_surveillance->setHorizontalHeaderItem(6, __qtablewidgetitem12);
        QTableWidgetItem *__qtablewidgetitem13 = new QTableWidgetItem();
        table_surveillance->setHorizontalHeaderItem(7, __qtablewidgetitem13);
        QTableWidgetItem *__qtablewidgetitem14 = new QTableWidgetItem();
        table_surveillance->setHorizontalHeaderItem(8, __qtablewidgetitem14);
        table_surveillance->setObjectName("table_surveillance");
        table_surveillance->setGeometry(QRect(130, 290, 881, 291));
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
        btnSupprimerConso = new QPushButton(page_surveillancehabitat);
        btnSupprimerConso->setObjectName("btnSupprimerConso");
        btnSupprimerConso->setGeometry(QRect(10, 410, 81, 41));
        btnSupprimerConso->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: rgb(13, 59, 102); /* Bleu fonc\303\251 */\n"
"    color: rgb(248, 249, 250);          /* Texte blanc */\n"
"    font: italic 9pt \"Segoe UI\";\n"
"}\n"
"\n"
"\n"
""));
        AJOUTconso = new QPushButton(page_surveillancehabitat);
        AJOUTconso->setObjectName("AJOUTconso");
        AJOUTconso->setGeometry(QRect(10, 350, 81, 41));
        AJOUTconso->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: rgb(13, 59, 102); /* Bleu fonc\303\251 */\n"
"    color: rgb(248, 249, 250);          /* Texte blanc */\n"
"    font: italic 9pt \"Segoe UI\";\n"
"}\n"
""));
        btnAfficherConso = new QPushButton(page_surveillancehabitat);
        btnAfficherConso->setObjectName("btnAfficherConso");
        btnAfficherConso->setGeometry(QRect(10, 460, 81, 41));
        btnAfficherConso->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: rgb(13, 59, 102); /* Bleu fonc\303\251 */\n"
"    color: rgb(248, 249, 250);          /* Texte blanc */\n"
"    font: italic 9pt \"Segoe UI\";\n"
"}\n"
"\n"
""));
        btnStatistiques = new QPushButton(page_surveillancehabitat);
        btnStatistiques->setObjectName("btnStatistiques");
        btnStatistiques->setGeometry(QRect(150, 590, 211, 31));
        btnStatistiques->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: rgb(13, 59, 102); /* Bleu fonc\303\251 */\n"
"    color: rgb(248, 249, 250);          /* Texte blanc */\n"
"    font: italic 9pt \"Segoe UI\";\n"
"}\n"
"\n"
""));
        btnActiverAlertesAuto = new QPushButton(page_surveillancehabitat);
        btnActiverAlertesAuto->setObjectName("btnActiverAlertesAuto");
        btnActiverAlertesAuto->setGeometry(QRect(390, 590, 211, 31));
        btnActiverAlertesAuto->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: rgb(13, 59, 102); /* Bleu fonc\303\251 */\n"
"    color: rgb(248, 249, 250);          /* Texte blanc */\n"
"    font: italic 9pt \"Segoe UI\";\n"
"}\n"
"\n"
""));
        btnVerifierSMSEntrants = new QPushButton(page_surveillancehabitat);
        btnVerifierSMSEntrants->setObjectName("btnVerifierSMSEntrants");
        btnVerifierSMSEntrants->setGeometry(QRect(610, 590, 211, 31));
        btnVerifierSMSEntrants->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: rgb(13, 59, 102); /* Bleu fonc\303\251 */\n"
"    color: rgb(248, 249, 250);          /* Texte blanc */\n"
"    font: italic 9pt \"Segoe UI\";\n"
"}\n"
"\n"
""));
        stackedWidget_3 = new QStackedWidget(page_surveillancehabitat);
        stackedWidget_3->setObjectName("stackedWidget_3");
        stackedWidget_3->setGeometry(QRect(900, 630, 1641, 831));
        page_gestionhabitat_2 = new QWidget();
        page_gestionhabitat_2->setObjectName("page_gestionhabitat_2");
        ADRESSE_2 = new QLineEdit(page_gestionhabitat_2);
        ADRESSE_2->setObjectName("ADRESSE_2");
        ADRESSE_2->setGeometry(QRect(190, 120, 141, 26));
        label_113 = new QLabel(page_gestionhabitat_2);
        label_113->setObjectName("label_113");
        label_113->setGeometry(QRect(500, 90, 121, 20));
        label_113->setStyleSheet(QString::fromUtf8("color: rgb(0, 0, 0);"));
        label_114 = new QLabel(page_gestionhabitat_2);
        label_114->setObjectName("label_114");
        label_114->setGeometry(QRect(20, 90, 91, 20));
        label_114->setStyleSheet(QString::fromUtf8("color: rgb(0, 0, 0);"));
        label_115 = new QLabel(page_gestionhabitat_2);
        label_115->setObjectName("label_115");
        label_115->setGeometry(QRect(190, 90, 91, 20));
        label_115->setStyleSheet(QString::fromUtf8("color: rgb(0, 0, 0);"));
        ID_HABITAT_2 = new QLineEdit(page_gestionhabitat_2);
        ID_HABITAT_2->setObjectName("ID_HABITAT_2");
        ID_HABITAT_2->setGeometry(QRect(20, 120, 141, 26));
        label_116 = new QLabel(page_gestionhabitat_2);
        label_116->setObjectName("label_116");
        label_116->setGeometry(QRect(350, 90, 91, 20));
        label_116->setStyleSheet(QString::fromUtf8("color: rgb(0, 0, 0);"));
        label_117 = new QLabel(page_gestionhabitat_2);
        label_117->setObjectName("label_117");
        label_117->setGeometry(QRect(660, 90, 91, 20));
        label_117->setStyleSheet(QString::fromUtf8("color: rgb(0, 0, 0);"));
        TYPE_LOGEMENT_2 = new QLineEdit(page_gestionhabitat_2);
        TYPE_LOGEMENT_2->setObjectName("TYPE_LOGEMENT_2");
        TYPE_LOGEMENT_2->setGeometry(QRect(660, 120, 141, 26));
        stackedWidget_4 = new QStackedWidget(page_gestionhabitat_2);
        stackedWidget_4->setObjectName("stackedWidget_4");
        stackedWidget_4->setGeometry(QRect(1730, 450, 191, 251));
        page_2 = new QWidget();
        page_2->setObjectName("page_2");
        stackedWidget_4->addWidget(page_2);
        page_5 = new QWidget();
        page_5->setObjectName("page_5");
        stackedWidget_4->addWidget(page_5);
        page_6 = new QWidget();
        page_6->setObjectName("page_6");
        stackedWidget_4->addWidget(page_6);
        page_7 = new QWidget();
        page_7->setObjectName("page_7");
        stackedWidget_4->addWidget(page_7);
        SURFACE_2 = new QLineEdit(page_gestionhabitat_2);
        SURFACE_2->setObjectName("SURFACE_2");
        SURFACE_2->setGeometry(QRect(350, 120, 141, 26));
        NB_PERSONNES_2 = new QLineEdit(page_gestionhabitat_2);
        NB_PERSONNES_2->setObjectName("NB_PERSONNES_2");
        NB_PERSONNES_2->setGeometry(QRect(510, 120, 141, 26));
        TabCrudHabitat_2 = new QTableWidget(page_gestionhabitat_2);
        if (TabCrudHabitat_2->columnCount() < 6)
            TabCrudHabitat_2->setColumnCount(6);
        QTableWidgetItem *__qtablewidgetitem15 = new QTableWidgetItem();
        TabCrudHabitat_2->setHorizontalHeaderItem(0, __qtablewidgetitem15);
        QTableWidgetItem *__qtablewidgetitem16 = new QTableWidgetItem();
        TabCrudHabitat_2->setHorizontalHeaderItem(1, __qtablewidgetitem16);
        QTableWidgetItem *__qtablewidgetitem17 = new QTableWidgetItem();
        TabCrudHabitat_2->setHorizontalHeaderItem(2, __qtablewidgetitem17);
        QTableWidgetItem *__qtablewidgetitem18 = new QTableWidgetItem();
        TabCrudHabitat_2->setHorizontalHeaderItem(3, __qtablewidgetitem18);
        QTableWidgetItem *__qtablewidgetitem19 = new QTableWidgetItem();
        TabCrudHabitat_2->setHorizontalHeaderItem(4, __qtablewidgetitem19);
        QTableWidgetItem *__qtablewidgetitem20 = new QTableWidgetItem();
        TabCrudHabitat_2->setHorizontalHeaderItem(5, __qtablewidgetitem20);
        TabCrudHabitat_2->setObjectName("TabCrudHabitat_2");
        TabCrudHabitat_2->setGeometry(QRect(30, 180, 621, 291));
        TabCrudHabitat_2->setStyleSheet(QString::fromUtf8("/* =====================\n"
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
        pushButton_exporter_2 = new QPushButton(page_gestionhabitat_2);
        pushButton_exporter_2->setObjectName("pushButton_exporter_2");
        pushButton_exporter_2->setGeometry(QRect(570, 490, 121, 31));
        pushButton_exporter_2->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: rgb(13, 59, 102); /* Bleu fonc\303\251 */\n"
"    color: rgb(248, 249, 250);          /* Texte blanc */\n"
"    font: italic 9pt \"Segoe UI\";\n"
"}\n"
""));
        btnModifier_2 = new QPushButton(page_gestionhabitat_2);
        btnModifier_2->setObjectName("btnModifier_2");
        btnModifier_2->setGeometry(QRect(380, 490, 171, 31));
        btnModifier_2->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: rgb(13, 59, 102); /* Bleu fonc\303\251 */\n"
"    color: rgb(248, 249, 250);          /* Texte blanc */\n"
"    font: italic 9pt \"Segoe UI\";\n"
"}\n"
"\n"
""));
        btnSupprimer_2 = new QPushButton(page_gestionhabitat_2);
        btnSupprimer_2->setObjectName("btnSupprimer_2");
        btnSupprimer_2->setGeometry(QRect(200, 490, 151, 31));
        btnSupprimer_2->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"	color: rgb(0, 0, 0);\n"
"    background-color: rgb(13, 59, 102); /* Bleu fonc\303\251 */\n"
"    color: rgb(248, 249, 250);          /* Texte blanc */\n"
"    font: italic 9pt \"Segoe UI\";\n"
"}\n"
""));
        btnAjouter_2 = new QPushButton(page_gestionhabitat_2);
        btnAjouter_2->setObjectName("btnAjouter_2");
        btnAjouter_2->setGeometry(QRect(40, 490, 141, 31));
        btnAjouter_2->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: rgb(13, 59, 102); /* Bleu fonc\303\251 */\n"
"    color: rgb(248, 249, 250);          /* Texte blanc */\n"
"    font: italic 9pt \"Segoe UI\";\n"
"}\n"
""));
        label_8 = new QLabel(page_gestionhabitat_2);
        label_8->setObjectName("label_8");
        label_8->setGeometry(QRect(290, 40, 371, 41));
        label_8->setFont(font);
        label_8->setStyleSheet(QString::fromUtf8("QLabel {\n"
"    font-family: Arial;       /* Police */\n"
"    font-size: 18px;          /* Taille plus grande */\n"
"    font-weight: 900;         /* Tr\303\250s gras (Black) */\n"
"    color: #00008B;           /* Bleu fonc\303\251 */\n"
"}\n"
""));
        btnafficher_2 = new QPushButton(page_gestionhabitat_2);
        btnafficher_2->setObjectName("btnafficher_2");
        btnafficher_2->setGeometry(QRect(300, 540, 141, 31));
        btnafficher_2->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: rgb(13, 59, 102); /* Bleu fonc\303\251 */\n"
"    color: rgb(248, 249, 250);          /* Texte blanc */\n"
"    font: italic 9pt \"Segoe UI\";\n"
"}\n"
""));
        label_9 = new QLabel(page_gestionhabitat_2);
        label_9->setObjectName("label_9");
        label_9->setGeometry(QRect(820, 90, 71, 16));
        TELEPHONE_CONTACT_2 = new QLineEdit(page_gestionhabitat_2);
        TELEPHONE_CONTACT_2->setObjectName("TELEPHONE_CONTACT_2");
        TELEPHONE_CONTACT_2->setGeometry(QRect(820, 120, 141, 31));
        label_11 = new QLabel(page_gestionhabitat_2);
        label_11->setObjectName("label_11");
        label_11->setGeometry(QRect(820, 90, 91, 16));
        stackedWidget_3->addWidget(page_gestionhabitat_2);
        page_surveillancehabitat_2 = new QWidget();
        page_surveillancehabitat_2->setObjectName("page_surveillancehabitat_2");
        label_13 = new QLabel(page_surveillancehabitat_2);
        label_13->setObjectName("label_13");
        label_13->setGeometry(QRect(50, 90, 121, 21));
        label_17 = new QLabel(page_surveillancehabitat_2);
        label_17->setObjectName("label_17");
        label_17->setGeometry(QRect(60, 60, 49, 16));
        EAU_CONSO_2 = new QLineEdit(page_surveillancehabitat_2);
        EAU_CONSO_2->setObjectName("EAU_CONSO_2");
        EAU_CONSO_2->setGeometry(QRect(460, 260, 81, 21));
        MOY_CONSO_2 = new QLineEdit(page_surveillancehabitat_2);
        MOY_CONSO_2->setObjectName("MOY_CONSO_2");
        MOY_CONSO_2->setGeometry(QRect(650, 260, 61, 21));
        ETAT_EAU_3 = new QLineEdit(page_surveillancehabitat_2);
        ETAT_EAU_3->setObjectName("ETAT_EAU_3");
        ETAT_EAU_3->setGeometry(QRect(790, 260, 91, 21));
        label_18 = new QLabel(page_surveillancehabitat_2);
        label_18->setObjectName("label_18");
        label_18->setGeometry(QRect(550, 260, 111, 20));
        ETAT_EAU_4 = new QLabel(page_surveillancehabitat_2);
        ETAT_EAU_4->setObjectName("ETAT_EAU_4");
        ETAT_EAU_4->setGeometry(QRect(720, 220, 111, 91));
        label_19 = new QLabel(page_surveillancehabitat_2);
        label_19->setObjectName("label_19");
        label_19->setGeometry(QRect(370, 260, 111, 20));
        label_20 = new QLabel(page_surveillancehabitat_2);
        label_20->setObjectName("label_20");
        label_20->setGeometry(QRect(1020, 260, 111, 20));
        DATE_RELEVE_2 = new QDateTimeEdit(page_surveillancehabitat_2);
        DATE_RELEVE_2->setObjectName("DATE_RELEVE_2");
        DATE_RELEVE_2->setGeometry(QRect(1120, 260, 151, 23));
        label_21 = new QLabel(page_surveillancehabitat_2);
        label_21->setObjectName("label_21");
        label_21->setGeometry(QRect(190, 260, 71, 16));
        ID_conso_2 = new QLineEdit(page_surveillancehabitat_2);
        ID_conso_2->setObjectName("ID_conso_2");
        ID_conso_2->setGeometry(QRect(270, 260, 91, 21));
        btnModifierConso_2 = new QPushButton(page_surveillancehabitat_2);
        btnModifierConso_2->setObjectName("btnModifierConso_2");
        btnModifierConso_2->setGeometry(QRect(10, 520, 81, 41));
        btnModifierConso_2->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: rgb(13, 59, 102); /* Bleu fonc\303\251 */\n"
"    color: rgb(248, 249, 250);          /* Texte blanc */\n"
"    font: italic 9pt \"Segoe UI\";\n"
"}\n"
"\n"
""));
        ALERTE_2 = new QLineEdit(page_surveillancehabitat_2);
        ALERTE_2->setObjectName("ALERTE_2");
        ALERTE_2->setGeometry(QRect(950, 260, 61, 21));
        label_22 = new QLabel(page_surveillancehabitat_2);
        label_22->setObjectName("label_22");
        label_22->setGeometry(QRect(890, 260, 49, 16));
        ID_HABITAT_4 = new QLabel(page_surveillancehabitat_2);
        ID_HABITAT_4->setObjectName("ID_HABITAT_4");
        ID_HABITAT_4->setGeometry(QRect(10, 260, 111, 20));
        ID_HABITAT2_2 = new QLineEdit(page_surveillancehabitat_2);
        ID_HABITAT2_2->setObjectName("ID_HABITAT2_2");
        ID_HABITAT2_2->setGeometry(QRect(110, 260, 71, 21));
        table_surveillance_2 = new QTableWidget(page_surveillancehabitat_2);
        if (table_surveillance_2->columnCount() < 9)
            table_surveillance_2->setColumnCount(9);
        QTableWidgetItem *__qtablewidgetitem21 = new QTableWidgetItem();
        table_surveillance_2->setHorizontalHeaderItem(0, __qtablewidgetitem21);
        QTableWidgetItem *__qtablewidgetitem22 = new QTableWidgetItem();
        table_surveillance_2->setHorizontalHeaderItem(1, __qtablewidgetitem22);
        QTableWidgetItem *__qtablewidgetitem23 = new QTableWidgetItem();
        table_surveillance_2->setHorizontalHeaderItem(2, __qtablewidgetitem23);
        QTableWidgetItem *__qtablewidgetitem24 = new QTableWidgetItem();
        table_surveillance_2->setHorizontalHeaderItem(3, __qtablewidgetitem24);
        QTableWidgetItem *__qtablewidgetitem25 = new QTableWidgetItem();
        table_surveillance_2->setHorizontalHeaderItem(4, __qtablewidgetitem25);
        QTableWidgetItem *__qtablewidgetitem26 = new QTableWidgetItem();
        table_surveillance_2->setHorizontalHeaderItem(5, __qtablewidgetitem26);
        QTableWidgetItem *__qtablewidgetitem27 = new QTableWidgetItem();
        table_surveillance_2->setHorizontalHeaderItem(6, __qtablewidgetitem27);
        QTableWidgetItem *__qtablewidgetitem28 = new QTableWidgetItem();
        table_surveillance_2->setHorizontalHeaderItem(7, __qtablewidgetitem28);
        QTableWidgetItem *__qtablewidgetitem29 = new QTableWidgetItem();
        table_surveillance_2->setHorizontalHeaderItem(8, __qtablewidgetitem29);
        table_surveillance_2->setObjectName("table_surveillance_2");
        table_surveillance_2->setGeometry(QRect(130, 290, 881, 291));
        table_surveillance_2->setStyleSheet(QString::fromUtf8("/* =====================\n"
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
        btnSupprimerConso_2 = new QPushButton(page_surveillancehabitat_2);
        btnSupprimerConso_2->setObjectName("btnSupprimerConso_2");
        btnSupprimerConso_2->setGeometry(QRect(10, 410, 81, 41));
        btnSupprimerConso_2->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: rgb(13, 59, 102); /* Bleu fonc\303\251 */\n"
"    color: rgb(248, 249, 250);          /* Texte blanc */\n"
"    font: italic 9pt \"Segoe UI\";\n"
"}\n"
"\n"
"\n"
""));
        AJOUTconso_2 = new QPushButton(page_surveillancehabitat_2);
        AJOUTconso_2->setObjectName("AJOUTconso_2");
        AJOUTconso_2->setGeometry(QRect(10, 350, 81, 41));
        AJOUTconso_2->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: rgb(13, 59, 102); /* Bleu fonc\303\251 */\n"
"    color: rgb(248, 249, 250);          /* Texte blanc */\n"
"    font: italic 9pt \"Segoe UI\";\n"
"}\n"
""));
        btnAfficherConso_2 = new QPushButton(page_surveillancehabitat_2);
        btnAfficherConso_2->setObjectName("btnAfficherConso_2");
        btnAfficherConso_2->setGeometry(QRect(10, 460, 81, 41));
        btnAfficherConso_2->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: rgb(13, 59, 102); /* Bleu fonc\303\251 */\n"
"    color: rgb(248, 249, 250);          /* Texte blanc */\n"
"    font: italic 9pt \"Segoe UI\";\n"
"}\n"
"\n"
""));
        btnStatistiques_4 = new QPushButton(page_surveillancehabitat_2);
        btnStatistiques_4->setObjectName("btnStatistiques_4");
        btnStatistiques_4->setGeometry(QRect(160, 590, 211, 31));
        btnStatistiques_4->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: rgb(13, 59, 102); /* Bleu fonc\303\251 */\n"
"    color: rgb(248, 249, 250);          /* Texte blanc */\n"
"    font: italic 9pt \"Segoe UI\";\n"
"}\n"
"\n"
""));
        btnVerifierSMSEntrants_2 = new QPushButton(page_surveillancehabitat_2);
        btnVerifierSMSEntrants_2->setObjectName("btnVerifierSMSEntrants_2");
        btnVerifierSMSEntrants_2->setGeometry(QRect(1040, 380, 141, 31));
        btnVerifierSMSEntrants_2->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: rgb(13, 59, 102); /* Bleu fonc\303\251 */\n"
"    color: rgb(248, 249, 250);          /* Texte blanc */\n"
"    font: italic 9pt \"Segoe UI\";\n"
"}\n"
"\n"
""));
        btnActiverAlertesAuto_2 = new QPushButton(page_surveillancehabitat_2);
        btnActiverAlertesAuto_2->setObjectName("btnActiverAlertesAuto_2");
        btnActiverAlertesAuto_2->setGeometry(QRect(1030, 320, 141, 31));
        btnActiverAlertesAuto_2->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: rgb(13, 59, 102); /* Bleu fonc\303\251 */\n"
"    color: rgb(248, 249, 250);          /* Texte blanc */\n"
"    font: italic 9pt \"Segoe UI\";\n"
"}\n"
"\n"
"\n"
""));
        btnTestSMS_2 = new QPushButton(page_surveillancehabitat_2);
        btnTestSMS_2->setObjectName("btnTestSMS_2");
        btnTestSMS_2->setGeometry(QRect(1060, 440, 131, 31));
        btnTestSMS_2->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: rgb(13, 59, 102); /* Bleu fonc\303\251 */\n"
"    color: rgb(248, 249, 250);          /* Texte blanc */\n"
"    font: italic 9pt \"Segoe UI\";\n"
"}\n"
"\n"
""));
        btnStatistiques_5 = new QPushButton(page_surveillancehabitat_2);
        btnStatistiques_5->setObjectName("btnStatistiques_5");
        btnStatistiques_5->setGeometry(QRect(390, 590, 211, 31));
        btnStatistiques_5->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: rgb(13, 59, 102); /* Bleu fonc\303\251 */\n"
"    color: rgb(248, 249, 250);          /* Texte blanc */\n"
"    font: italic 9pt \"Segoe UI\";\n"
"}\n"
"\n"
""));
        btnStatistiques_6 = new QPushButton(page_surveillancehabitat_2);
        btnStatistiques_6->setObjectName("btnStatistiques_6");
        btnStatistiques_6->setGeometry(QRect(610, 590, 211, 31));
        btnStatistiques_6->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: rgb(13, 59, 102); /* Bleu fonc\303\251 */\n"
"    color: rgb(248, 249, 250);          /* Texte blanc */\n"
"    font: italic 9pt \"Segoe UI\";\n"
"}\n"
"\n"
""));
        stackedWidget_3->addWidget(page_surveillancehabitat_2);
        btnTestSMS = new QPushButton(page_surveillancehabitat);
        btnTestSMS->setObjectName("btnTestSMS");
        btnTestSMS->setGeometry(QRect(840, 590, 211, 31));
        btnTestSMS->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: rgb(13, 59, 102); /* Bleu fonc\303\251 */\n"
"    color: rgb(248, 249, 250);          /* Texte blanc */\n"
"    font: italic 9pt \"Segoe UI\";\n"
"}\n"
"\n"
""));
        stackedWidget->addWidget(page_surveillancehabitat);
        Ghabitant_5 = new QPushButton(centralwidget);
        Ghabitant_5->setObjectName("Ghabitant_5");
        Ghabitant_5->setGeometry(QRect(-30, 30, 191, 71));
        Ghabitant_5->setStyleSheet(QString::fromUtf8("color: rgb(248, 249, 250);\n"
"font: italic 9pt \"Segoe UI\";\n"
"background-color: rgb(13, 59, 102);"));
        tableView_2 = new QTableView(centralwidget);
        tableView_2->setObjectName("tableView_2");
        tableView_2->setGeometry(QRect(-30, 10, 191, 1671));
        tableView_2->setStyleSheet(QString::fromUtf8("color: rgb(0, 0, 127);\n"
"background-color: rgb(0, 0, 93);"));
        Ghabitant_6 = new QPushButton(centralwidget);
        Ghabitant_6->setObjectName("Ghabitant_6");
        Ghabitant_6->setGeometry(QRect(-30, 150, 191, 71));
        Ghabitant_6->setStyleSheet(QString::fromUtf8("color: rgb(248, 249, 250);\n"
"font: italic 9pt \"Segoe UI\";\n"
"background-color: rgb(13, 59, 102);"));
        Ghabitant_4 = new QPushButton(centralwidget);
        Ghabitant_4->setObjectName("Ghabitant_4");
        Ghabitant_4->setGeometry(QRect(-30, 240, 191, 71));
        Ghabitant_4->setStyleSheet(QString::fromUtf8("color: rgb(248, 249, 250);\n"
"font: italic 9pt \"Segoe UI\";\n"
"background-color: rgb(13, 59, 102);"));
        Ghabitant_2 = new QPushButton(centralwidget);
        Ghabitant_2->setObjectName("Ghabitant_2");
        Ghabitant_2->setGeometry(QRect(-30, 70, 191, 71));
        Ghabitant_2->setStyleSheet(QString::fromUtf8("color: rgb(248, 249, 250);\n"
"font: italic 9pt \"Segoe UI\";\n"
"background-color: rgb(13, 59, 102);"));
        Ghabitant_3 = new QPushButton(centralwidget);
        Ghabitant_3->setObjectName("Ghabitant_3");
        Ghabitant_3->setGeometry(QRect(-30, 320, 191, 71));
        Ghabitant_3->setStyleSheet(QString::fromUtf8("color: rgb(248, 249, 250);\n"
"font: italic 9pt \"Segoe UI\";\n"
"background-color: rgb(13, 59, 102);"));
        surveilalnce_button = new QPushButton(centralwidget);
        surveilalnce_button->setObjectName("surveilalnce_button");
        surveilalnce_button->setGeometry(QRect(550, 10, 551, 51));
        surveilalnce_button->setStyleSheet(QString::fromUtf8("color: rgb(248, 249, 250);\n"
"font: italic 9pt \"Segoe UI\";\n"
"background-color: rgb(13, 59, 102);"));
        gestionhabitat_button = new QPushButton(centralwidget);
        gestionhabitat_button->setObjectName("gestionhabitat_button");
        gestionhabitat_button->setGeometry(QRect(170, 10, 381, 51));
        gestionhabitat_button->setStyleSheet(QString::fromUtf8("color: rgb(248, 249, 250);\n"
"font: italic 9pt \"Segoe UI\";\n"
"background-color: rgb(13, 59, 102);"));
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 1831, 26));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        stackedWidget->setCurrentIndex(0);
        stackedWidget_2->setCurrentIndex(0);
        stackedWidget_3->setCurrentIndex(1);
        stackedWidget_4->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        label_110->setText(QCoreApplication::translate("MainWindow", "Nb personnes : ", nullptr));
        label_108->setText(QCoreApplication::translate("MainWindow", "Id :", nullptr));
        label_109->setText(QCoreApplication::translate("MainWindow", "Adresse :", nullptr));
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
        QTableWidgetItem *___qtablewidgetitem5 = TabCrudHabitat->horizontalHeaderItem(5);
        ___qtablewidgetitem5->setText(QCoreApplication::translate("MainWindow", "T\303\251l\303\251phone", nullptr));
        pushButton_exporter->setText(QCoreApplication::translate("MainWindow", "Exporter \360\237\223\202", nullptr));
        btnModifier->setText(QCoreApplication::translate("MainWindow", "Modifier \342\234\217\357\270\217", nullptr));
        btnSupprimer->setText(QCoreApplication::translate("MainWindow", "Supprimer \342\235\214", nullptr));
        btnAjouter->setText(QCoreApplication::translate("MainWindow", "Ajouter", nullptr));
        label_5->setText(QCoreApplication::translate("MainWindow", "Habitat et Gaspillage ", nullptr));
        btnafficher->setText(QCoreApplication::translate("MainWindow", "Afficher", nullptr));
        label_4->setText(QString());
        label_7->setText(QCoreApplication::translate("MainWindow", "Telephone :", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "\360\237\214\277 SURVEILLANCE INTELLIGENTE \342\200\224 TEMPS R\303\211EL     ", nullptr));
        label_2->setText(QString());
        label_3->setText(QString());
        label_10->setText(QCoreApplication::translate("MainWindow", "MOY_CONSO", nullptr));
        ETAT_EAU_2->setText(QCoreApplication::translate("MainWindow", "ETAT_EAU ", nullptr));
        label_14->setText(QCoreApplication::translate("MainWindow", "EAU_CONSO", nullptr));
        label_15->setText(QCoreApplication::translate("MainWindow", "DATE_RELEVE", nullptr));
        label_6->setText(QCoreApplication::translate("MainWindow", "ID_CONSO", nullptr));
        btnModifierConso->setText(QCoreApplication::translate("MainWindow", "modifier", nullptr));
        label_16->setText(QCoreApplication::translate("MainWindow", "ALERTE", nullptr));
        ID_HABITAT_3->setText(QCoreApplication::translate("MainWindow", "ID_HABITAT", nullptr));
        ID_HABITAT2->setText(QString());
        groupBoxFiltres->setTitle(QCoreApplication::translate("MainWindow", "\"\360\237\224\215 Tri et Recherche\"", nullptr));
        labelTri->setText(QCoreApplication::translate("MainWindow", "\"Trier par:\"", nullptr));
        comboTriConso->setItemText(0, QCoreApplication::translate("MainWindow", "1. \360\237\223\205 Date (Plus r\303\251cent)", nullptr));
        comboTriConso->setItemText(1, QCoreApplication::translate("MainWindow", "2. \360\237\223\205 Date (Plus ancien)", nullptr));
        comboTriConso->setItemText(2, QCoreApplication::translate("MainWindow", "3. \360\237\223\212 Consommation (\303\211lev\303\251e \342\206\222 Faible)", nullptr));
        comboTriConso->setItemText(3, QCoreApplication::translate("MainWindow", "4. \360\237\223\212 Consommation (Faible \342\206\222 \303\211lev\303\251e)", nullptr));
        comboTriConso->setItemText(4, QCoreApplication::translate("MainWindow", "6. \342\232\240\357\270\217 Fuites d'abord", nullptr));
        comboTriConso->setItemText(5, QCoreApplication::translate("MainWindow", "7. \360\237\217\240 ID Habitat", nullptr));

        labelRechHabitat->setText(QCoreApplication::translate("MainWindow", "\"Rechercher Habitat:\"", nullptr));
        lineRechercheHabitat->setText(QCoreApplication::translate("MainWindow", "\360\237\224\215 H1, H2...\"", nullptr));
        labelRechEtat->setText(QCoreApplication::translate("MainWindow", "\"Rechercher \303\211tat:\"", nullptr));
        lineRechercheEtat->setText(QCoreApplication::translate("MainWindow", " \"\360\237\224\215 Fuite, Normal...\"", nullptr));
        btnReinitialiserFiltre->setText(QCoreApplication::translate("MainWindow", "actualiser ", nullptr));
        QTableWidgetItem *___qtablewidgetitem6 = table_surveillance->horizontalHeaderItem(0);
        ___qtablewidgetitem6->setText(QCoreApplication::translate("MainWindow", "ID_Habitat", nullptr));
        QTableWidgetItem *___qtablewidgetitem7 = table_surveillance->horizontalHeaderItem(1);
        ___qtablewidgetitem7->setText(QCoreApplication::translate("MainWindow", "ID_conso", nullptr));
        QTableWidgetItem *___qtablewidgetitem8 = table_surveillance->horizontalHeaderItem(2);
        ___qtablewidgetitem8->setText(QCoreApplication::translate("MainWindow", "Eau_coso", nullptr));
        QTableWidgetItem *___qtablewidgetitem9 = table_surveillance->horizontalHeaderItem(3);
        ___qtablewidgetitem9->setText(QCoreApplication::translate("MainWindow", "Moy_conso", nullptr));
        QTableWidgetItem *___qtablewidgetitem10 = table_surveillance->horizontalHeaderItem(4);
        ___qtablewidgetitem10->setText(QCoreApplication::translate("MainWindow", "Type Anomalie", nullptr));
        QTableWidgetItem *___qtablewidgetitem11 = table_surveillance->horizontalHeaderItem(5);
        ___qtablewidgetitem11->setText(QCoreApplication::translate("MainWindow", "Source", nullptr));
        QTableWidgetItem *___qtablewidgetitem12 = table_surveillance->horizontalHeaderItem(6);
        ___qtablewidgetitem12->setText(QCoreApplication::translate("MainWindow", "SMS Envoy\303\251 ", nullptr));
        QTableWidgetItem *___qtablewidgetitem13 = table_surveillance->horizontalHeaderItem(7);
        ___qtablewidgetitem13->setText(QCoreApplication::translate("MainWindow", "Localisation", nullptr));
        QTableWidgetItem *___qtablewidgetitem14 = table_surveillance->horizontalHeaderItem(8);
        ___qtablewidgetitem14->setText(QCoreApplication::translate("MainWindow", "Date_releve", nullptr));
        btnSupprimerConso->setText(QCoreApplication::translate("MainWindow", "supprimer", nullptr));
        AJOUTconso->setText(QCoreApplication::translate("MainWindow", "ajouter", nullptr));
        btnAfficherConso->setText(QCoreApplication::translate("MainWindow", "afficher", nullptr));
        btnStatistiques->setText(QCoreApplication::translate("MainWindow", "statistiques ", nullptr));
        btnActiverAlertesAuto->setText(QCoreApplication::translate("MainWindow", "\342\226\266\357\270\217 Activer Alertes Auto", nullptr));
        btnVerifierSMSEntrants->setText(QCoreApplication::translate("MainWindow", "\360\237\223\245 V\303\251rifier SMS Entrants", nullptr));
        label_113->setText(QCoreApplication::translate("MainWindow", "Nb personnes : ", nullptr));
        label_114->setText(QCoreApplication::translate("MainWindow", "Id :", nullptr));
        label_115->setText(QCoreApplication::translate("MainWindow", "Adresse :", nullptr));
        label_116->setText(QCoreApplication::translate("MainWindow", "Surface :", nullptr));
        label_117->setText(QCoreApplication::translate("MainWindow", "Type habitat : ", nullptr));
        QTableWidgetItem *___qtablewidgetitem15 = TabCrudHabitat_2->horizontalHeaderItem(0);
        ___qtablewidgetitem15->setText(QCoreApplication::translate("MainWindow", "Id", nullptr));
        QTableWidgetItem *___qtablewidgetitem16 = TabCrudHabitat_2->horizontalHeaderItem(1);
        ___qtablewidgetitem16->setText(QCoreApplication::translate("MainWindow", "Adresse", nullptr));
        QTableWidgetItem *___qtablewidgetitem17 = TabCrudHabitat_2->horizontalHeaderItem(2);
        ___qtablewidgetitem17->setText(QCoreApplication::translate("MainWindow", "Nb personnes ", nullptr));
        QTableWidgetItem *___qtablewidgetitem18 = TabCrudHabitat_2->horizontalHeaderItem(3);
        ___qtablewidgetitem18->setText(QCoreApplication::translate("MainWindow", "Surface", nullptr));
        QTableWidgetItem *___qtablewidgetitem19 = TabCrudHabitat_2->horizontalHeaderItem(4);
        ___qtablewidgetitem19->setText(QCoreApplication::translate("MainWindow", "Type Habitat ", nullptr));
        QTableWidgetItem *___qtablewidgetitem20 = TabCrudHabitat_2->horizontalHeaderItem(5);
        ___qtablewidgetitem20->setText(QCoreApplication::translate("MainWindow", "T\303\251l\303\251phone", nullptr));
        pushButton_exporter_2->setText(QCoreApplication::translate("MainWindow", "Exporter \360\237\223\202", nullptr));
        btnModifier_2->setText(QCoreApplication::translate("MainWindow", "Modifier \342\234\217\357\270\217", nullptr));
        btnSupprimer_2->setText(QCoreApplication::translate("MainWindow", "Supprimer \342\235\214", nullptr));
        btnAjouter_2->setText(QCoreApplication::translate("MainWindow", "Ajouter", nullptr));
        label_8->setText(QCoreApplication::translate("MainWindow", "Habitat et Gaspillage ", nullptr));
        btnafficher_2->setText(QCoreApplication::translate("MainWindow", "Afficher", nullptr));
        label_9->setText(QString());
        label_11->setText(QCoreApplication::translate("MainWindow", "Telephone :", nullptr));
        label_13->setText(QString());
        label_17->setText(QString());
        label_18->setText(QCoreApplication::translate("MainWindow", "MOY_CONSO", nullptr));
        ETAT_EAU_4->setText(QCoreApplication::translate("MainWindow", "ETAT_EAU ", nullptr));
        label_19->setText(QCoreApplication::translate("MainWindow", "EAU_CONSO", nullptr));
        label_20->setText(QCoreApplication::translate("MainWindow", "DATE_RELEVE", nullptr));
        label_21->setText(QCoreApplication::translate("MainWindow", "ID_CONSO", nullptr));
        btnModifierConso_2->setText(QCoreApplication::translate("MainWindow", "\342\226\266\357\270\217 Activer Alertes Auto", nullptr));
        label_22->setText(QCoreApplication::translate("MainWindow", "ALERTE", nullptr));
        ID_HABITAT_4->setText(QCoreApplication::translate("MainWindow", "ID_HABITAT", nullptr));
        ID_HABITAT2_2->setText(QString());
        QTableWidgetItem *___qtablewidgetitem21 = table_surveillance_2->horizontalHeaderItem(0);
        ___qtablewidgetitem21->setText(QCoreApplication::translate("MainWindow", "ID_Habitat", nullptr));
        QTableWidgetItem *___qtablewidgetitem22 = table_surveillance_2->horizontalHeaderItem(1);
        ___qtablewidgetitem22->setText(QCoreApplication::translate("MainWindow", "ID_conso", nullptr));
        QTableWidgetItem *___qtablewidgetitem23 = table_surveillance_2->horizontalHeaderItem(2);
        ___qtablewidgetitem23->setText(QCoreApplication::translate("MainWindow", "Eau_coso", nullptr));
        QTableWidgetItem *___qtablewidgetitem24 = table_surveillance_2->horizontalHeaderItem(3);
        ___qtablewidgetitem24->setText(QCoreApplication::translate("MainWindow", "Moy_conso", nullptr));
        QTableWidgetItem *___qtablewidgetitem25 = table_surveillance_2->horizontalHeaderItem(4);
        ___qtablewidgetitem25->setText(QCoreApplication::translate("MainWindow", "Type Anomalie", nullptr));
        QTableWidgetItem *___qtablewidgetitem26 = table_surveillance_2->horizontalHeaderItem(5);
        ___qtablewidgetitem26->setText(QCoreApplication::translate("MainWindow", "Source", nullptr));
        QTableWidgetItem *___qtablewidgetitem27 = table_surveillance_2->horizontalHeaderItem(6);
        ___qtablewidgetitem27->setText(QCoreApplication::translate("MainWindow", "SMS Envoy\303\251 ", nullptr));
        QTableWidgetItem *___qtablewidgetitem28 = table_surveillance_2->horizontalHeaderItem(7);
        ___qtablewidgetitem28->setText(QCoreApplication::translate("MainWindow", "Localisation", nullptr));
        QTableWidgetItem *___qtablewidgetitem29 = table_surveillance_2->horizontalHeaderItem(8);
        ___qtablewidgetitem29->setText(QCoreApplication::translate("MainWindow", "Date_releve", nullptr));
        btnSupprimerConso_2->setText(QCoreApplication::translate("MainWindow", "\342\226\266\357\270\217 Activer Alertes Auto", nullptr));
        AJOUTconso_2->setText(QCoreApplication::translate("MainWindow", "\342\226\266\357\270\217 Activer Alertes Auto", nullptr));
        btnAfficherConso_2->setText(QCoreApplication::translate("MainWindow", "\342\226\266\357\270\217 Activer Alertes Auto", nullptr));
        btnStatistiques_4->setText(QCoreApplication::translate("MainWindow", "\342\226\266\357\270\217 Activer Alertes Auto", nullptr));
        btnVerifierSMSEntrants_2->setText(QCoreApplication::translate("MainWindow", "\342\226\266\357\270\217 Activer Alertes Auto", nullptr));
        btnActiverAlertesAuto_2->setText(QCoreApplication::translate("MainWindow", "\342\226\266\357\270\217 Activer Alertes Auto", nullptr));
        btnTestSMS_2->setText(QCoreApplication::translate("MainWindow", "\342\226\266\357\270\217 Activer Alertes Auto", nullptr));
        btnStatistiques_5->setText(QCoreApplication::translate("MainWindow", "\342\226\266\357\270\217 Activer Alertes Auto", nullptr));
        btnStatistiques_6->setText(QCoreApplication::translate("MainWindow", "\342\226\266\357\270\217 Activer Alertes Auto", nullptr));
        btnTestSMS->setText(QCoreApplication::translate("MainWindow", "\360\237\247\252 Test SM", nullptr));
        Ghabitant_5->setText(QCoreApplication::translate("MainWindow", "\342\226\266\357\270\217 Activer Alertes Auto", nullptr));
        Ghabitant_6->setText(QCoreApplication::translate("MainWindow", "\342\226\266\357\270\217 Activer Alertes Auto", nullptr));
        Ghabitant_4->setText(QCoreApplication::translate("MainWindow", "\342\226\266\357\270\217 Activer Alertes Auto", nullptr));
        Ghabitant_2->setText(QCoreApplication::translate("MainWindow", "\342\226\266\357\270\217 Activer Alertes Auto", nullptr));
        Ghabitant_3->setText(QCoreApplication::translate("MainWindow", "\342\226\266\357\270\217 Activer Alertes Auto", nullptr));
        surveilalnce_button->setText(QCoreApplication::translate("MainWindow", "\342\226\266\357\270\217 Activer Alertes Auto", nullptr));
        gestionhabitat_button->setText(QCoreApplication::translate("MainWindow", "\342\226\266\357\270\217 Activer Alertes Auto", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
