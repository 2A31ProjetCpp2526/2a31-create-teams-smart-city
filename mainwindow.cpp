#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "habitat.h"
#include "consommation.h"
#include <QSqlQueryModel>
#include <QTableWidgetItem>
#include <QKeyEvent>
#include <QMessageBox>
#include <QDateTime>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>
#include <QDebug>
#include <QBrush>
#include <QColor>
#include <QFont>
#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QLabel>
#include <QPushButton>
#include <QTableWidget>
#include <QFileDialog>
#include <QTextDocument>
#include <QPrinter>
#include <QFile>
#include <QTextStream>
#include <QTabWidget>
#include <QGridLayout>
#include <QScrollArea>
#include <QFrame>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    qDebug() << "=== DÉBUT MainWindow Constructor ===";

    ui->setupUi(this);

    ui->table_surveillance->setStyleSheet(
        "QTableWidget {"
        "    background-color: rgb(240, 240, 240);"
        "    gridline-color: rgb(204, 204, 204);"
        "    border: 2px solid #2980b9;"
        "    font: 9pt 'Segoe UI';"
        "}"
        "QHeaderView::section {"
        "    background-color: rgb(13, 59, 102);"
        "    color: rgb(248, 249, 250);"
        "    font-weight: bold;"
        "    padding: 5px;"
        "    border: 1px solid #2980b9;"
        "}"
        "QTableWidget::item {"
        "    padding: 4px;"
        "}"
        "QTableWidget::item:selected {"
        "    background-color: rgb(41, 128, 185);"
        "    color: white;"
        "}"
        );

    initialiserTable();
    ui->stackedWidget->setCurrentIndex(0);
    on_btnafficher_clicked();

    ui->ID_HABITAT->installEventFilter(this);
    ui->ADRESSE->installEventFilter(this);
    ui->SURFACE->installEventFilter(this);
    ui->NB_PERSONNES->installEventFilter(this);
    ui->TYPE_LOGEMENT->installEventFilter(this);

    ui->DATE_RELEVE->setDateTime(QDateTime::currentDateTime());
    critereTri = "DATE_RELEVE DESC";

    qDebug() << "=== FIN MainWindow Constructor ===";
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initialiserTable()
{
    ui->TabCrudHabitat->setColumnCount(5);
    QStringList headersHabitat = {"ID", "Adresse", "Surface", "Nb Personnes", "Type Habitat"};
    ui->TabCrudHabitat->setHorizontalHeaderLabels(headersHabitat);

    ui->table_surveillance->setColumnCount(7);
    QStringList headersConso = {"ID Habitat", "ID Conso", "Eau Conso (m³)", "Moy Conso (m³)", "État Eau", "Alerte", "Date Relevé"};
    ui->table_surveillance->setHorizontalHeaderLabels(headersConso);

    qDebug() << "✓ Tables initialisées";
}

// ========================================
// NAVIGATION
// ========================================

void MainWindow::on_gestionhabitat_button_clicked()  // ✅ BON NOM
{
    qDebug() << "Navigation: Gestion Habitat";
    ui->stackedWidget->setCurrentIndex(0);
    on_btnafficher_clicked();
}
void MainWindow::on_surveilalnce_button_clicked()
{
    qDebug() << "Navigation: Surveillance Consommation";
    ui->stackedWidget->setCurrentIndex(1);
    afficherConsommations();
}

// ========================================
// HABITAT - CRUD
// ========================================

void MainWindow::clearFieldsHabitat()
{
    ui->ID_HABITAT->clear();
    ui->ADRESSE->clear();
    ui->SURFACE->clear();
    ui->NB_PERSONNES->clear();
    ui->TYPE_LOGEMENT->clear();
}

void MainWindow::on_btnAjouter_clicked()
{
    qDebug() << "=== DÉBUT on_btnAjouter_clicked ===";

    if(ui->ADRESSE->text().isEmpty()) {
        QMessageBox::warning(this, "Erreur", "⚠️ Veuillez remplir l'adresse !");
        return;
    }

    QSqlQuery query;
    query.exec("SELECT NVL(MAX(ID_HABITAT), 0) + 1 FROM HABITAT");
    int nextId = 1;
    if(query.next()) {
        nextId = query.value(0).toInt();
    }

    QString adresse = ui->ADRESSE->text();
    double surface = ui->SURFACE->text().toDouble();
    int nb = ui->NB_PERSONNES->text().toInt();
    QString type = ui->TYPE_LOGEMENT->text();

    Habitat h(nextId, adresse, nb, surface, type);
    if(h.ajouter()) {
        QMessageBox::information(this, "Succès", QString("✅ Habitat H%1 ajouté !").arg(nextId));
        on_btnafficher_clicked();
        clearFieldsHabitat();
    } else {
        QMessageBox::critical(this, "Erreur", "❌ Échec de l'ajout !");
    }

    qDebug() << "=== FIN on_btnAjouter_clicked ===";
}

void MainWindow::on_btnSupprimer_clicked()
{
    qDebug() << "=== DÉBUT on_btnSupprimer_clicked ===";

    QList<QTableWidgetItem*> selected = ui->TabCrudHabitat->selectedItems();
    if(selected.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Sélectionnez une ligne !");
        return;
    }

    int row = selected.first()->row();
    QString idText = ui->TabCrudHabitat->item(row,0)->text();
    int id = idText.mid(1).toInt();

    Habitat h;
    if(h.supprimer(id)) {
        QMessageBox::information(this, "Succès", "✅ Habitat supprimé !");
        on_btnafficher_clicked();
        clearFieldsHabitat();
    } else {
        QMessageBox::critical(this, "Erreur", "❌ Échec de suppression !");
    }

    qDebug() << "=== FIN on_btnSupprimer_clicked ===";
}

void MainWindow::on_btnModifier_clicked()
{
    qDebug() << "=== DÉBUT on_btnModifier_clicked ===";

    QString idText = ui->ID_HABITAT->text();
    if(idText.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "⚠️ Sélectionnez un habitat à modifier !");
        return;
    }

    int id = idText.startsWith("H") ? idText.mid(1).toInt() : idText.toInt();

    QString adresse = ui->ADRESSE->text();
    double surface = ui->SURFACE->text().toDouble();
    int nb = ui->NB_PERSONNES->text().toInt();
    QString type = ui->TYPE_LOGEMENT->text();

    Habitat h(id, adresse, nb, surface, type);
    if(h.modifier()) {
        QMessageBox::information(this, "Succès", "✅ Habitat modifié !");
        on_btnafficher_clicked();
        clearFieldsHabitat();
    } else {
        QMessageBox::critical(this, "Erreur", "❌ Échec de modification !");
    }

    qDebug() << "=== FIN on_btnModifier_clicked ===";
}

void MainWindow::on_btnafficher_clicked()
{
    qDebug() << "=== DÉBUT on_btnafficher_clicked ===";

    Habitat h;
    QSqlQueryModel *model = h.afficher();
    ui->TabCrudHabitat->setRowCount(0);

    for(int i=0; i<model->rowCount(); ++i){
        ui->TabCrudHabitat->insertRow(i);

        QString idHabitat = "H" + model->data(model->index(i,0)).toString();

        ui->TabCrudHabitat->setItem(i,0,new QTableWidgetItem(idHabitat));
        ui->TabCrudHabitat->setItem(i,1,new QTableWidgetItem(model->data(model->index(i,1)).toString()));
        ui->TabCrudHabitat->setItem(i,2,new QTableWidgetItem(model->data(model->index(i,3)).toString()));
        ui->TabCrudHabitat->setItem(i,3,new QTableWidgetItem(model->data(model->index(i,2)).toString()));
        ui->TabCrudHabitat->setItem(i,4,new QTableWidgetItem(model->data(model->index(i,4)).toString()));
    }
    delete model;

    qDebug() << "=== FIN on_btnafficher_clicked ===";
}

void MainWindow::on_TabCrudHabitat_itemClicked(QTableWidgetItem *item)
{
    int row = item->row();
    ui->ID_HABITAT->setText(ui->TabCrudHabitat->item(row,0)->text());
    ui->ADRESSE->setText(ui->TabCrudHabitat->item(row,1)->text());
    ui->SURFACE->setText(ui->TabCrudHabitat->item(row,2)->text());
    ui->NB_PERSONNES->setText(ui->TabCrudHabitat->item(row,3)->text());
    ui->TYPE_LOGEMENT->setText(ui->TabCrudHabitat->item(row,4)->text());
}

// ========================================
// CONSOMMATION - CRUD
// ========================================

void MainWindow::clearFieldsConso()
{
    ui->EAU_CONSO->clear();
    ui->MOY_CONSO->clear();
    ui->ETAT_EAU->clear();
    ui->DATE_RELEVE->setDateTime(QDateTime::currentDateTime());
}

void MainWindow::afficherConsommations()
{
    qDebug() << "=== DÉBUT afficherConsommations ===";

    QString requete = QString(
                          "SELECT ID_HABITAT, ID_CONSO, EAU_CONSO, MOY_CONSO, ETAT_EAU, "
                          "TO_CHAR(DATE_RELEVE, 'YYYY-MM-DD HH24:MI') AS DATE_STR "
                          "FROM CONSOMMATION "
                          "ORDER BY %1"
                          ).arg(critereTri);

    qDebug() << "Requête SQL:" << requete;

    QSqlQuery query;
    if(!query.exec(requete)) {
        qDebug() << "❌ Erreur:" << query.lastError().text();
        return;
    }

    ui->table_surveillance->setRowCount(0);
    int row = 0;

    while(query.next()){
        ui->table_surveillance->insertRow(row);

        QString etatEau = query.value(4).toString();
        QString idHabitat = "H" + query.value(0).toString();
        QString idConso = "C" + query.value(1).toString();

        ui->table_surveillance->setItem(row, 0, new QTableWidgetItem(idHabitat));
        ui->table_surveillance->setItem(row, 1, new QTableWidgetItem(idConso));
        ui->table_surveillance->setItem(row, 2, new QTableWidgetItem(query.value(2).toString()));
        ui->table_surveillance->setItem(row, 3, new QTableWidgetItem(query.value(3).toString()));

        QTableWidgetItem *etatItem = new QTableWidgetItem(etatEau);
        QTableWidgetItem *alerteItem = new QTableWidgetItem();

        if(etatEau == "Fuite") {
            etatItem->setData(Qt::BackgroundRole, QColor(255, 0, 0));
            etatItem->setData(Qt::ForegroundRole, QColor(255, 255, 255));

            alerteItem->setText("⚠️ FUITE DÉTECTÉE !");
            alerteItem->setData(Qt::BackgroundRole, QColor(200, 0, 0));
            alerteItem->setData(Qt::ForegroundRole, QColor(255, 255, 255));

            QFont boldFont;
            boldFont.setBold(true);
            boldFont.setPointSize(10);
            etatItem->setData(Qt::FontRole, boldFont);
            alerteItem->setData(Qt::FontRole, boldFont);

        } else if(etatEau == "Sous-Consommation") {
            etatItem->setData(Qt::BackgroundRole, QColor(255, 165, 0));
            etatItem->setData(Qt::ForegroundRole, QColor(255, 255, 255));

            QFont font;
            font.setBold(true);
            font.setPointSize(10);
            etatItem->setData(Qt::FontRole, font);

        } else {
            etatItem->setData(Qt::BackgroundRole, QColor(0, 200, 0));
            etatItem->setData(Qt::ForegroundRole, QColor(255, 255, 255));
        }

        ui->table_surveillance->setItem(row, 4, etatItem);
        ui->table_surveillance->setItem(row, 5, alerteItem);
        ui->table_surveillance->setItem(row, 6, new QTableWidgetItem(query.value(5).toString()));

        row++;
    }

    qDebug() << "✅" << row << "lignes";
    qDebug() << "=== FIN afficherConsommations ===";
}

void MainWindow::on_AJOUTconso_clicked()
{
    qDebug() << "=== DÉBUT on_AJOUTconso_clicked ===";

    QString idHabitatText = ui->ID_HABITAT2->text().trimmed();

    if(idHabitatText.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "⚠️ Veuillez entrer un ID d'habitat !");
        return;
    }

    int idHabitat = 0;
    if(idHabitatText.startsWith("H", Qt::CaseInsensitive)) {
        idHabitat = idHabitatText.mid(1).toInt();
    } else {
        idHabitat = idHabitatText.toInt();
    }

    if(idHabitat <= 0) {
        QMessageBox::warning(this, "Erreur", "⚠️ ID habitat invalide !");
        return;
    }

    QSqlQuery checkQuery;
    checkQuery.prepare("SELECT COUNT(*) FROM HABITAT WHERE ID_HABITAT = :id");
    checkQuery.bindValue(":id", idHabitat);

    if(!checkQuery.exec() || !checkQuery.next() || checkQuery.value(0).toInt() == 0) {
        QMessageBox::warning(this, "Erreur", QString("⚠️ L'habitat H%1 n'existe pas !").arg(idHabitat));
        return;
    }

    if(ui->EAU_CONSO->text().isEmpty() || ui->MOY_CONSO->text().isEmpty()) {
        QMessageBox::warning(this, "Erreur", "⚠️ Remplissez tous les champs !");
        return;
    }

    float eauConso = ui->EAU_CONSO->text().toFloat();
    float moyConso = ui->MOY_CONSO->text().toFloat();
    QString dateReleve = ui->DATE_RELEVE->dateTime().toString("yyyy-MM-dd hh:mm");

    Consommation c(idHabitat, eauConso, moyConso, dateReleve);

    if(c.ajouter()) {
        QSqlQuery query;
        query.exec("SELECT MAX(ID_CONSO) FROM CONSOMMATION");
        int idConso = 0;
        if(query.next()) {
            idConso = query.value(0).toInt();
        }

        QString etat = c.getEtatEau();
        QString message = QString("✅ Consommation C%1 ajoutée pour H%2 !\n\nÉtat: %3")
                              .arg(idConso).arg(idHabitat).arg(etat);

        if(etat == "Fuite") {
            message += "\n⚠️ ATTENTION: Fuite détectée !";
        }

        QMessageBox::information(this, "Succès", message);
        afficherConsommations();
        clearFieldsConso();
    } else {
        QMessageBox::critical(this, "Erreur", "❌ Échec !");
    }

    qDebug() << "=== FIN on_AJOUTconso_clicked ===";
}// ========================================
// SUITE DE mainwindow.cpp - PARTIE 2
// Copiez ce code APRÈS la partie 1
// ========================================

void MainWindow::on_btnSupprimerConso_clicked()
{
    qDebug() << "=== DÉBUT on_btnSupprimerConso_clicked ===";

    QList<QTableWidgetItem*> selected = ui->table_surveillance->selectedItems();
    if(selected.isEmpty()){
        QMessageBox::warning(this, "Erreur", "Sélectionnez une ligne !");
        return;
    }

    int row = selected.first()->row();
    QString idText = ui->table_surveillance->item(row, 1)->text();
    int id = idText.mid(1).toInt();

    Consommation c;
    if(c.supprimer(id)){
        QMessageBox::information(this, "Succès", QString("✅ C%1 supprimée !").arg(id));
        afficherConsommations();
        clearFieldsConso();
    } else {
        QMessageBox::critical(this, "Erreur", "❌ Échec !");
    }

    qDebug() << "=== FIN on_btnSupprimerConso_clicked ===";
}

void MainWindow::on_btnModifierConso_clicked()
{
    qDebug() << "=== DÉBUT on_btnModifierConso_clicked ===";

    QList<QTableWidgetItem*> selected = ui->table_surveillance->selectedItems();
    if(selected.isEmpty()){
        QMessageBox::warning(this, "Erreur", "Sélectionnez une ligne !");
        return;
    }

    int row = selected.first()->row();
    QString idConsoText = ui->table_surveillance->item(row, 1)->text();
    int idConso = idConsoText.mid(1).toInt();

    QString idHabitatText = ui->ID_HABITAT2->text();
    int idHabitat = idHabitatText.startsWith("H") ? idHabitatText.mid(1).toInt() : idHabitatText.toInt();

    float eau = ui->EAU_CONSO->text().toFloat();
    float moy = ui->MOY_CONSO->text().toFloat();
    QString etat = ui->ETAT_EAU->text();
    QString date = ui->DATE_RELEVE->dateTime().toString("yyyy-MM-dd hh:mm");

    Consommation c(idHabitat, eau, moy, etat, date);
    c.setIdConso(idConso);

    if(c.modifier()){
        QMessageBox::information(this, "Succès", QString("✅ C%1 modifiée !").arg(idConso));
        afficherConsommations();
        clearFieldsConso();
    } else {
        QMessageBox::critical(this, "Erreur", "❌ Échec !");
    }

    qDebug() << "=== FIN on_btnModifierConso_clicked ===";
}

void MainWindow::on_table_surveillance_itemClicked(QTableWidgetItem *item)
{
    int row = item->row();
    ui->ID_HABITAT2->setText(ui->table_surveillance->item(row, 0)->text());
    ui->EAU_CONSO->setText(ui->table_surveillance->item(row, 2)->text());
    ui->MOY_CONSO->setText(ui->table_surveillance->item(row, 3)->text());
    ui->ETAT_EAU->setText(ui->table_surveillance->item(row, 4)->text());
    QDateTime dt = QDateTime::fromString(ui->table_surveillance->item(row, 6)->text(), "yyyy-MM-dd hh:mm");
    ui->DATE_RELEVE->setDateTime(dt);
}

// ========================================
// TRI ET RECHERCHE
// ========================================

void MainWindow::on_comboTriConso_currentIndexChanged(int index)
{
    qDebug() << "=== Tri, index:" << index;

    switch(index) {
    case 0:
        critereTri = "DATE_RELEVE DESC";
        break;
    case 1:
        critereTri = "DATE_RELEVE ASC";
        break;
    case 2:
        critereTri = "EAU_CONSO DESC";
        break;
    case 3:
        critereTri = "EAU_CONSO ASC";
        break;
    case 4:
        critereTri = "ETAT_EAU ASC";
        break;
    case 5:
        critereTri = "CASE WHEN ETAT_EAU='Fuite' THEN 1 WHEN ETAT_EAU='Sous-Consommation' THEN 2 ELSE 3 END, DATE_RELEVE DESC";
        break;
    case 6:
        critereTri = "ID_HABITAT ASC";
        break;
    default:
        critereTri = "DATE_RELEVE DESC";
    }

    afficherConsommations();
}

void MainWindow::on_lineRechercheHabitat_textChanged(const QString &text)
{
    if(text.isEmpty()) {
        for(int i = 0; i < ui->table_surveillance->rowCount(); ++i) {
            ui->table_surveillance->setRowHidden(i, false);
        }
        return;
    }

    for(int i = 0; i < ui->table_surveillance->rowCount(); ++i) {
        if(ui->table_surveillance->item(i, 0)) {
            QString idHabitat = ui->table_surveillance->item(i, 0)->text();
            bool match = idHabitat.contains(text, Qt::CaseInsensitive);
            ui->table_surveillance->setRowHidden(i, !match);
        }
    }
}

void MainWindow::on_lineRechercheEtat_textChanged(const QString &text)
{
    if(text.isEmpty()) {
        for(int i = 0; i < ui->table_surveillance->rowCount(); ++i) {
            ui->table_surveillance->setRowHidden(i, false);
        }
        return;
    }

    for(int i = 0; i < ui->table_surveillance->rowCount(); ++i) {
        if(ui->table_surveillance->item(i, 4)) {
            QString etat = ui->table_surveillance->item(i, 4)->text();
            bool match = etat.contains(text, Qt::CaseInsensitive);
            ui->table_surveillance->setRowHidden(i, !match);
        }
    }
}

void MainWindow::on_btnReinitialiserFiltre_clicked()
{
    ui->lineRechercheHabitat->clear();
    ui->lineRechercheEtat->clear();
    ui->comboTriConso->setCurrentIndex(0);
    afficherConsommations();
    QMessageBox::information(this, "Info", "✅ Filtres réinitialisés !");
}


// ========================================
// STATISTIQUES
// ========================================

void MainWindow::on_btnStatistiques_clicked()
{
    QDialog *dlg = new QDialog(this);
    dlg->setWindowTitle("📊 Statistiques Complètes");
    dlg->resize(1200, 900);

    // ScrollArea pour éviter que les boutons soient coupés
    QScrollArea *scrollArea = new QScrollArea(dlg);
    scrollArea->setWidgetResizable(true);
    scrollArea->setStyleSheet("QScrollArea { border: none; background-color: white; }");

    QWidget *contentWidget = new QWidget();
    QVBoxLayout *mainLay = new QVBoxLayout(contentWidget);
    mainLay->setSpacing(25);
    mainLay->setContentsMargins(30, 30, 30, 30);

    // EN-TÊTE
    QLabel *titre = new QLabel("📊 RAPPORT STATISTIQUES");
    titre->setAlignment(Qt::AlignCenter);
    titre->setStyleSheet("font-size: 24pt; font-weight: bold; color: #0d3b66; padding: 15px;");
    mainLay->addWidget(titre);

    QLabel *date = new QLabel("📅 Date : " + QDateTime::currentDateTime().toString("dd/MM/yyyy à hh:mm"));
    date->setAlignment(Qt::AlignCenter);
    date->setStyleSheet("font-size: 12pt; color: #666; padding-bottom: 15px;");
    mainLay->addWidget(date);

    // VUE D'ENSEMBLE
    QLabel *lblGeneral = new QLabel("📈 VUE D'ENSEMBLE");
    lblGeneral->setStyleSheet("font-size: 18pt; font-weight: bold; color: #0d3b66; padding: 15px 0;");
    mainLay->addWidget(lblGeneral);

    // CARTES STATISTIQUES - EN LIGNE
    QHBoxLayout *cardsLayout = new QHBoxLayout();
    cardsLayout->setSpacing(15);

    QSqlQuery q;
    q.exec("SELECT COUNT(*) FROM HABITAT");
    int totalH = q.next() ? q.value(0).toInt() : 0;

    q.exec("SELECT COUNT(*) FROM CONSOMMATION");
    int totalC = q.next() ? q.value(0).toInt() : 0;

    q.exec("SELECT COUNT(*) FROM CONSOMMATION WHERE ETAT_EAU = 'Fuite'");
    int fuites = q.next() ? q.value(0).toInt() : 0;

    q.exec("SELECT COUNT(*) FROM CONSOMMATION WHERE ETAT_EAU = 'Sous-Consommation'");
    int sous = q.next() ? q.value(0).toInt() : 0;

    q.exec("SELECT COUNT(*) FROM CONSOMMATION WHERE ETAT_EAU = 'Normal'");
    int norm = q.next() ? q.value(0).toInt() : 0;

    QString cardStyle = "background-color: %1; border: 3px solid %2; border-radius: 10px; padding: 20px; min-width: 180px; min-height: 120px;";

    QLabel *card1 = new QLabel(QString("<center><div style='font-size:40pt; font-weight:bold; color:#2980b9;'>%1</div>"
                                       "<div style='font-size:13pt; color:#34495e;'>Habitats</div></center>").arg(totalH));
    card1->setStyleSheet(cardStyle.arg("#e3f2fd", "#2980b9"));
    cardsLayout->addWidget(card1);

    QLabel *card2 = new QLabel(QString("<center><div style='font-size:40pt; font-weight:bold; color:#2980b9;'>%1</div>"
                                       "<div style='font-size:13pt; color:#34495e;'>Consommations</div></center>").arg(totalC));
    card2->setStyleSheet(cardStyle.arg("#e3f2fd", "#2980b9"));
    cardsLayout->addWidget(card2);

    QLabel *card3 = new QLabel(QString("<center><div style='font-size:40pt; font-weight:bold; color:#e74c3c;'>🔴 %1</div>"
                                       "<div style='font-size:13pt; color:#c0392b; font-weight:bold;'>FUITES</div></center>").arg(fuites));
    card3->setStyleSheet(cardStyle.arg("#ffebee", "#e74c3c"));
    cardsLayout->addWidget(card3);

    QLabel *card4 = new QLabel(QString("<center><div style='font-size:40pt; font-weight:bold; color:#ff9800;'>🟠 %1</div>"
                                       "<div style='font-size:13pt; color:#e65100;'>Sous-Conso</div></center>").arg(sous));
    card4->setStyleSheet(cardStyle.arg("#fff3e0", "#ff9800"));
    cardsLayout->addWidget(card4);

    QLabel *card5 = new QLabel(QString("<center><div style='font-size:40pt; font-weight:bold; color:#27ae60;'>🟢 %1</div>"
                                       "<div style='font-size:13pt; color:#1e8449;'>Normal</div></center>").arg(norm));
    card5->setStyleSheet(cardStyle.arg("#e8f5e9", "#27ae60"));
    cardsLayout->addWidget(card5);

    mainLay->addLayout(cardsLayout);

    // SÉPARATEUR
    QFrame *line1 = new QFrame();
    line1->setFrameShape(QFrame::HLine);
    line1->setStyleSheet("background-color: #bdc3c7; min-height: 2px; max-height: 2px;");
    mainLay->addWidget(line1);

    // PAR TYPE D'HABITAT
    QLabel *lblType = new QLabel("🏠 CONSOMMATION PAR TYPE D'HABITAT");
    lblType->setStyleSheet("font-size: 18pt; font-weight: bold; color: #0d3b66; padding: 15px 0;");
    mainLay->addWidget(lblType);

    QTableWidget *tab2 = new QTableWidget();
    tab2->setColumnCount(5);
    tab2->setHorizontalHeaderLabels({"Type", "Nb Habitats", "Conso Moyenne", "Conso Totale", "Évaluation"});
    tab2->setStyleSheet(
        "QTableWidget { font-size: 13pt; gridline-color: #ddd; background-color: white; border: 2px solid #34495e; }"
        "QHeaderView::section { background-color: #34495e; color: white; font-size: 14pt; font-weight: bold; padding: 12px; border: 1px solid #2c3e50; }"
        "QTableWidget::item { padding: 10px; border-bottom: 1px solid #ecf0f1; }"
        );
    tab2->verticalHeader()->setVisible(false);
    tab2->horizontalHeader()->setStretchLastSection(true);

    q.prepare("SELECT h.TYPE_LOGEMENT, COUNT(DISTINCT h.ID_HABITAT), "
              "ROUND(AVG(c.EAU_CONSO), 2), ROUND(SUM(c.EAU_CONSO), 2), "
              "CASE WHEN AVG(c.EAU_CONSO) > AVG(c.MOY_CONSO)*1.5 THEN 'Élevée' "
              "WHEN AVG(c.EAU_CONSO) < AVG(c.MOY_CONSO)*0.5 THEN 'Faible' ELSE 'Normal' END "
              "FROM HABITAT h LEFT JOIN CONSOMMATION c ON h.ID_HABITAT = c.ID_HABITAT "
              "WHERE c.EAU_CONSO IS NOT NULL GROUP BY h.TYPE_LOGEMENT ORDER BY 4 DESC");

    if(q.exec()) {
        int r = 0;
        while(q.next()) {
            tab2->insertRow(r);
            QFont font; font.setPointSize(12);

            for(int col = 0; col < 4; col++) {
                QString text = q.value(col).toString();
                if(col >= 2) text += " m³";
                QTableWidgetItem *it = new QTableWidgetItem(text);
                it->setFont(font);
                it->setTextAlignment(Qt::AlignCenter);
                tab2->setItem(r, col, it);
            }

            QString etat = q.value(4).toString();
            QTableWidgetItem *it5 = new QTableWidgetItem(etat);
            QFont boldFont; boldFont.setPointSize(13); boldFont.setBold(true);
            it5->setFont(boldFont);
            it5->setTextAlignment(Qt::AlignCenter);
            if(etat == "Élevée") {
                it5->setBackground(QBrush(QColor(255,200,200)));
                it5->setForeground(QBrush(QColor(139,0,0)));
            } else if(etat == "Normal") {
                it5->setBackground(QBrush(QColor(200,255,200)));
                it5->setForeground(QBrush(QColor(0,100,0)));
            } else {
                it5->setBackground(QBrush(QColor(255,230,200)));
            }
            tab2->setItem(r, 4, it5);
            r++;
        }
    }
    tab2->resizeColumnsToContents();
    tab2->setFixedHeight(200);
    mainLay->addWidget(tab2);

    // PAR NOMBRE DE PERSONNES
    QLabel *lblPers = new QLabel("👥 CONSOMMATION PAR NOMBRE DE PERSONNES");
    lblPers->setStyleSheet("font-size: 18pt; font-weight: bold; color: #0d3b66; padding: 15px 0;");
    mainLay->addWidget(lblPers);

    QTableWidget *tab3 = new QTableWidget();
    tab3->setColumnCount(5);
    tab3->setHorizontalHeaderLabels({"Nb Personnes", "Nb Habitats", "m³/Personne", "Conso Moyenne", "Recommandation"});
    tab3->setStyleSheet(tab2->styleSheet());
    tab3->verticalHeader()->setVisible(false);
    tab3->horizontalHeader()->setStretchLastSection(true);

    q.prepare("SELECT h.NB_PERSONNES, COUNT(DISTINCT h.ID_HABITAT), "
              "ROUND(AVG(c.EAU_CONSO) / NULLIF(h.NB_PERSONNES, 0), 2), ROUND(AVG(c.EAU_CONSO), 2) "
              "FROM HABITAT h LEFT JOIN CONSOMMATION c ON h.ID_HABITAT = c.ID_HABITAT "
              "WHERE h.NB_PERSONNES > 0 AND c.EAU_CONSO IS NOT NULL "
              "GROUP BY h.NB_PERSONNES ORDER BY h.NB_PERSONNES");

    if(q.exec()) {
        int r = 0;
        while(q.next()) {
            tab3->insertRow(r);
            QFont font; font.setPointSize(12);

            tab3->setItem(r, 0, new QTableWidgetItem(q.value(0).toString() + " pers"));
            tab3->item(r, 0)->setFont(font); tab3->item(r, 0)->setTextAlignment(Qt::AlignCenter);

            tab3->setItem(r, 1, new QTableWidgetItem(q.value(1).toString()));
            tab3->item(r, 1)->setFont(font); tab3->item(r, 1)->setTextAlignment(Qt::AlignCenter);

            float parPers = q.value(2).toFloat();
            tab3->setItem(r, 2, new QTableWidgetItem(QString::number(parPers, 'f', 2) + " m³"));
            QFont boldFont; boldFont.setPointSize(13); boldFont.setBold(true);
            tab3->item(r, 2)->setFont(boldFont); tab3->item(r, 2)->setTextAlignment(Qt::AlignCenter);

            tab3->setItem(r, 3, new QTableWidgetItem(q.value(3).toString() + " m³"));
            tab3->item(r, 3)->setFont(font); tab3->item(r, 3)->setTextAlignment(Qt::AlignCenter);

            QString reco = parPers > 50 ? "⚠️ Élevée" : (parPers > 30 ? "✅ Normale" : "💡 Économe");
            QTableWidgetItem *it5 = new QTableWidgetItem(reco);
            it5->setFont(boldFont); it5->setTextAlignment(Qt::AlignCenter);
            if(parPers > 50) {
                it5->setBackground(QBrush(QColor(255,200,200)));
                it5->setForeground(QBrush(QColor(139,0,0)));
            } else if(parPers <= 30) {
                it5->setBackground(QBrush(QColor(200,230,255)));
                it5->setForeground(QBrush(QColor(0,0,139)));
            } else {
                it5->setBackground(QBrush(QColor(200,255,200)));
                it5->setForeground(QBrush(QColor(0,100,0)));
            }
            tab3->setItem(r, 4, it5);
            r++;
        }
    }
    tab3->resizeColumnsToContents();
    tab3->setFixedHeight(200);
    mainLay->addWidget(tab3);

    // SÉPARATEUR
    QFrame *line2 = new QFrame();
    line2->setFrameShape(QFrame::HLine);
    line2->setStyleSheet("background-color: #bdc3c7; min-height: 2px; max-height: 2px;");
    mainLay->addWidget(line2);

    // BOUTONS D'EXPORT - GRANDS ET VISIBLES
    QHBoxLayout *btnLayout = new QHBoxLayout();
    btnLayout->setSpacing(20);

    QPushButton *btnPDF = new QPushButton("📄 EXPORTER PDF");
    btnPDF->setFixedSize(280, 70);
    btnPDF->setStyleSheet(
        "QPushButton { background-color: #2980b9; color: white; font-size: 16pt; font-weight: bold; "
        "border-radius: 10px; border: none; }"
        "QPushButton:hover { background-color: #3498db; }"
        "QPushButton:pressed { background-color: #1c5980; }"
        );
    connect(btnPDF, &QPushButton::clicked, this, &MainWindow::exporterStatistiquesPDF);

    QPushButton *btnCSV = new QPushButton("📊 EXPORTER CSV");
    btnCSV->setFixedSize(280, 70);
    btnCSV->setStyleSheet(
        "QPushButton { background-color: #27ae60; color: white; font-size: 16pt; font-weight: bold; "
        "border-radius: 10px; border: none; }"
        "QPushButton:hover { background-color: #2ecc71; }"
        "QPushButton:pressed { background-color: #1e8449; }"
        );
    connect(btnCSV, &QPushButton::clicked, this, &MainWindow::exporterStatistiquesCSV);

    QPushButton *btnClose = new QPushButton("❌ FERMER");
    btnClose->setFixedSize(280, 70);
    btnClose->setStyleSheet(
        "QPushButton { background-color: #e74c3c; color: white; font-size: 16pt; font-weight: bold; "
        "border-radius: 10px; border: none; }"
        "QPushButton:hover { background-color: #c0392b; }"
        "QPushButton:pressed { background-color: #a93226; }"
        );
    connect(btnClose, &QPushButton::clicked, dlg, &QDialog::close);

    btnLayout->addStretch();
    btnLayout->addWidget(btnPDF);
    btnLayout->addWidget(btnCSV);
    btnLayout->addWidget(btnClose);
    btnLayout->addStretch();

    mainLay->addLayout(btnLayout);
    mainLay->addStretch();

    // Assigner le contenu au ScrollArea
    scrollArea->setWidget(contentWidget);

    // Layout principal du dialogue
    QVBoxLayout *dialogLayout = new QVBoxLayout(dlg);
    dialogLayout->setContentsMargins(0, 0, 0, 0);
    dialogLayout->addWidget(scrollArea);

    dlg->exec();
}

// ========================================
// EXPORTS
// ========================================

void MainWindow::exporterStatistiquesPDF()
{
    QString fn = QFileDialog::getSaveFileName(this, "PDF",
                                              QDir::homePath() + "/stats_" + QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss") + ".pdf", "PDF (*.pdf)");
    if(fn.isEmpty()) return;

    QPrinter p(QPrinter::PrinterResolution);
    p.setOutputFormat(QPrinter::PdfFormat);
    p.setPageSize(QPageSize::A4);  // ← CORRIGÉ pour Qt6
    p.setOutputFileName(fn);

    QTextDocument doc;
    doc.setHtml(genererHTMLStatistiques());
    doc.print(&p);

    QMessageBox::information(this, "Succès", "✅ PDF exporté !");
}

void MainWindow::exporterStatistiquesCSV()
{
    QString fn = QFileDialog::getSaveFileName(this, "CSV",
                                              QDir::homePath() + "/stats_" + QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss") + ".csv", "CSV (*.csv)");
    if(fn.isEmpty()) return;

    QFile f(fn);
    if(!f.open(QIODevice::WriteOnly | QIODevice::Text)) return;

    QTextStream out(&f);
    out << "STATISTIQUES\n";
    out << "Date:," << QDateTime::currentDateTime().toString("dd/MM/yyyy hh:mm") << "\n\n";

    QSqlQuery q;
    q.exec("SELECT COUNT(*) FROM HABITAT");
    out << "Habitats:," << (q.next() ? q.value(0).toInt() : 0) << "\n";

    q.exec("SELECT COUNT(*) FROM CONSOMMATION");
    out << "Consommations:," << (q.next() ? q.value(0).toInt() : 0) << "\n";

    q.exec("SELECT COUNT(*) FROM CONSOMMATION WHERE ETAT_EAU = 'Fuite'");
    out << "Fuites:," << (q.next() ? q.value(0).toInt() : 0) << "\n\n";

    out << "ALERTES\n";
    out << "ID C,ID H,Adr,Conso,État,Date\n";

    q.prepare("SELECT c.ID_CONSO, c.ID_HABITAT, h.ADRESSE, c.EAU_CONSO, c.ETAT_EAU, "
              "TO_CHAR(c.DATE_RELEVE, 'DD/MM/YYYY') FROM CONSOMMATION c "
              "LEFT JOIN HABITAT h ON c.ID_HABITAT = h.ID_HABITAT "
              "WHERE c.ETAT_EAU IN ('Fuite', 'Sous-Consommation')");

    if(q.exec()) {
        while(q.next()) {
            out << "C" << q.value(0).toString() << ","
                << "H" << q.value(1).toString() << ","
                << q.value(2).toString() << ","
                << q.value(3).toString() << ","
                << q.value(4).toString() << ","
                << q.value(5).toString() << "\n";
        }
    }

    f.close();
    QMessageBox::information(this, "Succès", "✅ CSV exporté !");
}

QString MainWindow::genererHTMLStatistiques()
{
    QString html = "<html><head><meta charset='UTF-8'><style>"
                   "body{font-family:Arial;margin:20px;}"
                   "h1{color:#0d3b66;text-align:center;}"
                   "table{width:100%;border-collapse:collapse;margin:20px 0;}"
                   "th{background:#0d3b66;color:white;padding:10px;}"
                   "td{padding:8px;border-bottom:1px solid #ddd;}"
                   "</style></head><body>";

    html += "<h1>📊 STATISTIQUES</h1>";
    html += "<p>Date : " + QDateTime::currentDateTime().toString("dd/MM/yyyy hh:mm") + "</p>";

    QSqlQuery q;
    q.exec("SELECT COUNT(*) FROM HABITAT");
    html += "<p><b>Habitats :</b> " + QString::number(q.next() ? q.value(0).toInt() : 0) + "</p>";

    q.exec("SELECT COUNT(*) FROM CONSOMMATION");
    html += "<p><b>Consommations :</b> " + QString::number(q.next() ? q.value(0).toInt() : 0) + "</p>";

    html += "<h2>Alertes</h2><table><tr><th>ID C</th><th>ID H</th><th>Adr</th><th>Conso</th><th>État</th><th>Date</th></tr>";

    q.prepare("SELECT c.ID_CONSO, c.ID_HABITAT, h.ADRESSE, c.EAU_CONSO, c.ETAT_EAU, "
              "TO_CHAR(c.DATE_RELEVE, 'DD/MM/YYYY') FROM CONSOMMATION c "
              "LEFT JOIN HABITAT h ON c.ID_HABITAT = h.ID_HABITAT "
              "WHERE c.ETAT_EAU IN ('Fuite', 'Sous-Consommation')");

    if(q.exec()) {
        while(q.next()) {
            html += "<tr><td>C" + q.value(0).toString() + "</td>"
                                                          "<td>H" + q.value(1).toString() + "</td>"
                                              "<td>" + q.value(2).toString() + "</td>"
                                              "<td>" + q.value(3).toString() + " m³</td>"
                                              "<td>" + q.value(4).toString() + "</td>"
                                              "<td>" + q.value(5).toString() + "</td></tr>";
        }
    }

    html += "</table></body></html>";
    return html;
}

// ========================================
// EVENT FILTER
// ========================================

bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
    if(event->type() == QEvent::KeyPress){
        QKeyEvent *ke = static_cast<QKeyEvent*>(event);
        if(ke->key() == Qt::Key_Return || ke->key() == Qt::Key_Enter){
            if(obj == ui->ID_HABITAT) ui->ADRESSE->setFocus();
            else if(obj == ui->ADRESSE) ui->SURFACE->setFocus();
            else if(obj == ui->SURFACE) ui->NB_PERSONNES->setFocus();
            else if(obj == ui->NB_PERSONNES) ui->TYPE_LOGEMENT->setFocus();
            else if(obj == ui->TYPE_LOGEMENT) on_btnAjouter_clicked();
            return true;
        }
    }
    return QMainWindow::eventFilter(obj,event);
}
