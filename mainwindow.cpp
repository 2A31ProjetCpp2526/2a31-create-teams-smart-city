#include"mainwindow.h"
#include "ui_mainwindow.h"
#include "habitat.h"
#include "consommation.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QInputDialog>
#include <QFileDialog>
#include <QTextDocument>
#include <QPrinter>
#include <QDebug>
#include <QNetworkRequest>
#include <QUrl>
#include <QUrlQuery>
#include <QByteArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QEvent>
#include <QDateTime>
#include <QTableWidgetItem>

// =======================
// CONSTRUCTEUR
// =======================
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , networkManager(new QNetworkAccessManager(this))
{
    ui->setupUi(this);
    // LIGNE 35 - MET ÇA :
    string twilio_key = getenv("TWILIO_KEY");  // Variable d'environnement                         // ← VOTRE NUMÉRO TWILIO

    // ========== TIMER POUR ALERTES AUTOMATIQUES ==========
    timerAlertes = new QTimer(this);
    connect(timerAlertes, &QTimer::timeout, this, &MainWindow::verifierGaspillagesEtEnvoyerSMS);

    ui->table_surveillance->setStyleSheet(
        "QTableWidget { background-color: rgb(240,240,240); gridline-color: rgb(204,204,204); border: 2px solid #2980b9; font: 9pt 'Segoe UI'; }"
        "QHeaderView::section { background-color: rgb(13,59,102); color: rgb(248,249,250); font-weight: bold; padding: 5px; border: 1px solid #2980b9; }"
        "QTableWidget::item { padding: 4px; }"
        "QTableWidget::item:selected { background-color: rgb(41,128,185); color: white; }"
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
    }


// =======================
// DESTRUCTEUR
// =======================
MainWindow::~MainWindow()
{
    delete ui;
}

// =======================
// INITIALISATION TABLES
// =======================
void MainWindow::initialiserTable()
{
    ui->TabCrudHabitat->setColumnCount(6);
    ui->TabCrudHabitat->setHorizontalHeaderLabels(
        {"ID", "Adresse", "Surface", "Nb Personnes", "Type Habitat", "Téléphone"}
        );

    ui->table_surveillance->setColumnCount(9);
    ui->table_surveillance->setHorizontalHeaderLabels(
        {"ID Habitat","ID Conso","Eau Conso (m³)","Moy Conso (m³)","Type Anomalie","Source","SMS Envoyé","Localisation","Date Relevé"}
        );
}

// =======================
// NAVIGATION
// =======================
void MainWindow::on_gestionhabitat_button_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
    on_btnafficher_clicked();
}

void MainWindow::on_surveilalnce_button_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
    afficherConsommations();
}

// =======================
// HABITAT CRUD
// =======================
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
    QString adresse = ui->ADRESSE->text().trimmed();
    QString surface = ui->SURFACE->text().trimmed();
    QString nbPersonnes = ui->NB_PERSONNES->text().trimmed();
    QString typeLogement = ui->TYPE_LOGEMENT->text().trimmed();

    // ✅ Générer automatiquement un ID unique
    QSqlQuery maxQuery("SELECT NVL(MAX(ID_HABITAT), 0) + 1 FROM HABITAT");
    int nouveauId = 1;
    if(maxQuery.next()) {
        nouveauId = maxQuery.value(0).toInt();
    }

    bool ok;
    QString telephone = QInputDialog::getText(this,"Numéro de téléphone",
                                              "Entrez le téléphone (format: +216xxxxxxxx):",
                                              QLineEdit::Normal,"+216",&ok);
    if(!ok || telephone.isEmpty()) return;

    QSqlQuery query;
    query.prepare("INSERT INTO HABITAT (ID_HABITAT, ADRESSE, SURFACE, NB_PERSONNES, "
                  "TYPE_LOGEMENT, TELEPHONE_CONTACT) VALUES (?,?,?,?,?,?)");
    query.addBindValue(nouveauId);  // ✅ ID généré automatiquement
    query.addBindValue(adresse);
    query.addBindValue(surface);
    query.addBindValue(nbPersonnes);
    query.addBindValue(typeLogement);
    query.addBindValue(telephone);

    if(!query.exec())
        QMessageBox::critical(this,"Erreur","Impossible d'ajouter: "+query.lastError().text());
    else {
        QMessageBox::information(this, "Succès",
                                 QString("✅ Habitat H%1 ajouté !").arg(nouveauId));
        clearFieldsHabitat();
        on_btnafficher_clicked();
    }
}
void MainWindow::on_btnSupprimer_clicked()
{
    QString id = ui->ID_HABITAT->text().trimmed();

    // Vérifier s'il y a des consommations associées
    QSqlQuery checkQuery;
    checkQuery.prepare("SELECT COUNT(*) FROM CONSOMMATION WHERE ID_HABITAT=?");
    checkQuery.addBindValue(id);

    if(checkQuery.exec() && checkQuery.next() && checkQuery.value(0).toInt() > 0) {
        int reponse = QMessageBox::question(this, "Confirmation",
                                            QString("⚠️ Cet habitat a %1 consommation(s) associée(s).\n\n"
                                                    "Voulez-vous VRAIMENT supprimer l'habitat ET toutes ses consommations ?")
                                                .arg(checkQuery.value(0).toInt()),
                                            QMessageBox::Yes | QMessageBox::No);

        if(reponse == QMessageBox::No) return;

        // Supprimer d'abord les consommations
        QSqlQuery delConso;
        delConso.prepare("DELETE FROM CONSOMMATION WHERE ID_HABITAT=?");
        delConso.addBindValue(id);
        delConso.exec();
    }

    // Supprimer l'habitat
    QSqlQuery query;
    query.prepare("DELETE FROM HABITAT WHERE ID_HABITAT=?");
    query.addBindValue(id);

    if(!query.exec())
        QMessageBox::critical(this,"Erreur","Impossible de supprimer: "+query.lastError().text());
    else {
        QMessageBox::information(this, "Succès", "✅ Habitat supprimé !");
        clearFieldsHabitat();
        on_btnafficher_clicked();
    }
}

void MainWindow::on_btnModifier_clicked()
{
    QString id = ui->ID_HABITAT->text().trimmed();
    QString adresse = ui->ADRESSE->text().trimmed();
    QString surface = ui->SURFACE->text().trimmed();
    QString nbPersonnes = ui->NB_PERSONNES->text().trimmed();
    QString typeLogement = ui->TYPE_LOGEMENT->text().trimmed();

    if(id.isEmpty()) {
        QMessageBox::warning(this, "Attention", "⚠️ Sélectionnez d'abord un habitat !");
        return;
    }

    // ✅ Récupérer l'ancien téléphone depuis la base de données
    QSqlQuery getTelQuery;
    getTelQuery.prepare("SELECT TELEPHONE_CONTACT FROM HABITAT WHERE ID_HABITAT=?");
    getTelQuery.addBindValue(id);

    QString ancienTel = "+216";
    if(getTelQuery.exec() && getTelQuery.next()) {
        ancienTel = getTelQuery.value(0).toString();
    }

    // ✅ Demander le nouveau téléphone
    bool ok;
    QString telephone = QInputDialog::getText(this,
                                              "Modifier le téléphone",
                                              "Nouveau numéro (format: +216xxxxxxxx):",
                                              QLineEdit::Normal,
                                              ancienTel,  // Affiche l'ancien numéro par défaut
                                              &ok);

    if(!ok || telephone.isEmpty()) {
        QMessageBox::warning(this, "Annulé", "Modification annulée !");
        return;
    }

    // Validation du téléphone
    if(!telephone.startsWith("+216")) {
        QMessageBox::warning(this, "Attention",
                             "⚠️ Le numéro doit commencer par +216\nExemple: +21620123456");
        return;
    }

    // ✅ Mettre à jour AVEC le téléphone
    QSqlQuery query;
    query.prepare("UPDATE HABITAT SET ADRESSE=?, SURFACE=?, NB_PERSONNES=?, "
                  "TYPE_LOGEMENT=?, TELEPHONE_CONTACT=? WHERE ID_HABITAT=?");
    query.addBindValue(adresse);
    query.addBindValue(surface);
    query.addBindValue(nbPersonnes);
    query.addBindValue(typeLogement);
    query.addBindValue(telephone);  // ✅ AJOUTÉ
    query.addBindValue(id);

    if(!query.exec())
        QMessageBox::critical(this,"Erreur","Impossible de modifier: "+query.lastError().text());
    else {
        QMessageBox::information(this, "Succès", "✅ Habitat modifié avec succès !");
        clearFieldsHabitat();
        on_btnafficher_clicked();
    }
}

void MainWindow::on_btnafficher_clicked()
{
    afficherHabitats();
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

void MainWindow::on_lineRechercheHabitat_textChanged(const QString &text)
{
    ui->TabCrudHabitat->setRowCount(0);
    QSqlQuery query;
    query.prepare("SELECT * FROM HABITAT WHERE ADRESSE LIKE ? OR TYPE_LOGEMENT LIKE ?");
    QString recherche = "%" + text + "%";
    query.addBindValue(recherche);
    query.addBindValue(recherche);
    if(query.exec()){
        int row=0;
        while(query.next()){
            ui->TabCrudHabitat->insertRow(row);
            for(int col=0; col<6; col++)
                ui->TabCrudHabitat->setItem(row,col,new QTableWidgetItem(query.value(col).toString()));
            row++;
        }
    }
}

// =======================
// CONSOMMATION CRUD
// =======================
void MainWindow::afficherConsommations()
{
    ui->table_surveillance->setRowCount(0);

    QString sql = QString("SELECT c.ID_HABITAT, c.ID_CONSO, c.EAU_CONSO, c.MOY_CONSO, "
                          "c.TYPE_ANOMALIE, c.SOURCE_DETECTION, c.SMS_ENVOYE, "
                          "c.LOCALISATION_FUITE, TO_CHAR(c.DATE_RELEVE, 'DD/MM/YYYY HH24:MI') "
                          "FROM CONSOMMATION c ORDER BY %1").arg(critereTri);

    QSqlQuery query(sql);

    int row = 0;
    while(query.next()) {
        ui->table_surveillance->insertRow(row);

        // Colonne 0: ID Habitat
        ui->table_surveillance->setItem(row, 0,
                                        new QTableWidgetItem("H" + query.value(0).toString()));

        // Colonne 1: ID Conso
        ui->table_surveillance->setItem(row, 1,
                                        new QTableWidgetItem("C" + query.value(1).toString()));

        // Colonne 2: Eau Conso
        ui->table_surveillance->setItem(row, 2,
                                        new QTableWidgetItem(query.value(2).toString()));

        // Colonne 3: Moy Conso
        ui->table_surveillance->setItem(row, 3,
                                        new QTableWidgetItem(query.value(3).toString()));

        // Colonne 4: Type Anomalie (avec couleurs)
        QString typeAnomalie = query.value(4).toString();
        QTableWidgetItem *itemType = new QTableWidgetItem(typeAnomalie);

        if(typeAnomalie == "Fuite") {
            itemType->setBackground(QColor(255, 100, 100));
            itemType->setForeground(QColor(255, 255, 255));
        }
        else if(typeAnomalie == "Gaspillage") {
            itemType->setBackground(QColor(255, 200, 100));
        }
        else if(typeAnomalie == "Sous-Consommation") {
            itemType->setBackground(QColor(255, 245, 200));
        }
        else {
            itemType->setBackground(QColor(200, 255, 200));
        }
        ui->table_surveillance->setItem(row, 4, itemType);

        // Colonne 5: Source
        ui->table_surveillance->setItem(row, 5,
                                        new QTableWidgetItem(query.value(5).toString()));

        // Colonne 6: SMS Envoyé
        QString smsText = (query.value(6).toInt() == 1) ? "✅" : "⏳";
        ui->table_surveillance->setItem(row, 6,
                                        new QTableWidgetItem(smsText));

        // Colonne 7: Localisation
        QString loc = query.value(7).toString();
        ui->table_surveillance->setItem(row, 7,
                                        new QTableWidgetItem(loc.isEmpty() ? "-" : loc));

        // Colonne 8: Date
        ui->table_surveillance->setItem(row, 8,
                                        new QTableWidgetItem(query.value(8).toString()));

        row++;
    }

    ui->table_surveillance->resizeColumnsToContents();
}
// =======================
// AJOUT CONSOMMATION AVEC SMS AUTOMATIQUE
// =======================
void MainWindow::on_AJOUTconso_clicked()
{
    // ========================================
    // VALIDATION DES CHAMPS
    // ========================================
    if(ui->ID_HABITAT2->text().isEmpty() ||
        ui->EAU_CONSO->text().isEmpty() ||
        ui->MOY_CONSO->text().isEmpty()) {
        QMessageBox::warning(this, "Attention", "⚠️ Veuillez remplir tous les champs obligatoires !");
        return;
    }

    QString idHabitat = ui->ID_HABITAT2->text();
    if(idHabitat.startsWith("H")) {
        idHabitat = idHabitat.mid(1); // Retirer le "H"
    }

    double eauConso = ui->EAU_CONSO->text().toDouble();
    double moyConso = ui->MOY_CONSO->text().toDouble();
    QString dateReleve = ui->DATE_RELEVE->dateTime().toString("dd/MM/yyyy HH:mm");

    // ========================================
    // 🤖 DÉTECTION AUTOMATIQUE DU TYPE D'ANOMALIE
    // ========================================
    QString typeAnomalie = "";
    QString couleurAffichage = "";
    QString icone = "";

    if(eauConso > moyConso * 1.8) {
        // Consommation 80% supérieure → Fuite probable
        typeAnomalie = "Fuite";
        couleurAffichage = "#FF6B6B"; // Rouge
        icone = "🚨";
    }
    else if(eauConso > moyConso * 1.3) {
        // Consommation 30% supérieure → Gaspillage
        typeAnomalie = "Gaspillage";
        couleurAffichage = "#FFA500"; // Orange
        icone = "⚠️";
    }
    else if(eauConso < moyConso * 0.5) {
        // Consommation 50% inférieure → Sous-consommation
        typeAnomalie = "Sous-Consommation";
        couleurAffichage = "#87CEEB"; // Bleu clair
        icone = "📉";
    }
    else {
        // Consommation normale
        typeAnomalie = "Normal";
        couleurAffichage = "#90EE90"; // Vert clair
        icone = "✅";
    }

    // ========================================
    // 💬 DEMANDER CONFIRMATION À L'UTILISATEUR
    // ========================================
    QString messageConfirmation = QString(
                                      "<html><body style='font-family: Segoe UI;'>"
                                      "<h3 style='color: %1;'>%2 Type détecté : <b>%3</b></h3>"
                                      "<hr>"
                                      "<table style='width:100%; margin-top:10px;'>"
                                      "<tr><td><b>Habitat :</b></td><td>H%4</td></tr>"
                                      "<tr><td><b>Consommation :</b></td><td><span style='color:%1; font-size:16px;'><b>%5 m³</b></span></td></tr>"
                                      "<tr><td><b>Moyenne :</b></td><td>%6 m³</td></tr>"
                                      "<tr><td><b>Écart :</b></td><td>%7%</td></tr>"
                                      "<tr><td><b>Date :</b></td><td>%8</td></tr>"
                                      "</table>"
                                      "<hr>"
                                      "<p><b>Voulez-vous ajouter cette consommation ?</b></p>"
                                      "</body></html>"
                                      )
                                      .arg(couleurAffichage)
                                      .arg(icone)
                                      .arg(typeAnomalie)
                                      .arg(idHabitat)
                                      .arg(eauConso, 0, 'f', 2)
                                      .arg(moyConso, 0, 'f', 2)
                                      .arg(((eauConso - moyConso) / moyConso * 100), 0, 'f', 1)
                                      .arg(dateReleve);

    QMessageBox msgBox(this);
    msgBox.setWindowTitle("Confirmation d'ajout");
    msgBox.setTextFormat(Qt::RichText);
    msgBox.setText(messageConfirmation);
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::Yes);

    // Style de la boîte de dialogue
    msgBox.setStyleSheet(
        "QMessageBox {"
        "   background-color: white;"
        "   min-width: 400px;"
        "}"
        "QPushButton {"
        "   background-color: #2980b9;"
        "   color: white;"
        "   border: none;"
        "   padding: 8px 20px;"
        "   border-radius: 5px;"
        "   min-width: 80px;"
        "}"
        "QPushButton:hover {"
        "   background-color: #3498db;"
        "}"
        );

    if(msgBox.exec() == QMessageBox::No) {
        return; // L'utilisateur a annulé
    }

    // ========================================
    // 💾 INSERTION DANS LA BASE DE DONNÉES
    // ========================================
    QSqlQuery query;
    query.prepare("INSERT INTO CONSOMMATION (ID_HABITAT, EAU_CONSO, MOY_CONSO, "
                  "TYPE_ANOMALIE, DATE_RELEVE, SOURCE_DETECTION, SMS_ENVOYE) "
                  "VALUES (?, ?, ?, ?, TO_DATE(?, 'DD/MM/YYYY HH24:MI'), 'Capteur IoT', 0)");

    query.addBindValue(idHabitat);
    query.addBindValue(eauConso);
    query.addBindValue(moyConso);
    query.addBindValue(typeAnomalie);
    query.addBindValue(dateReleve);

    if(!query.exec()) {
        QMessageBox::critical(this, "Erreur", "Erreur d'insertion : " + query.lastError().text());
        return;
    }

    // ========================================
    // 📱 ENVOI AUTOMATIQUE DU SMS (si Fuite ou Gaspillage)
    // ========================================
    bool fuiteDetectee = (typeAnomalie == "Fuite" || typeAnomalie == "Gaspillage");

    if(fuiteDetectee) {
        // Récupérer le numéro de téléphone de l'habitat
        QSqlQuery queryTel;
        queryTel.prepare("SELECT TELEPHONE_CONTACT FROM HABITAT WHERE ID_HABITAT = ?");
        queryTel.addBindValue(idHabitat);

        QString numeroTelephone = "";
        if(queryTel.exec() && queryTel.next()) {
            numeroTelephone = queryTel.value(0).toString();
        }

        if(!numeroTelephone.isEmpty()) {
            QString messageSMS = QString(
                                     "%1 ALERTE %2 !\n"
                                     "Habitat : H%3\n"
                                     "Consommation : %4 m³\n"
                                     "Moyenne : %5 m³\n"
                                     "Date : %6"
                                     )
                                     .arg(icone)
                                     .arg(typeAnomalie.toUpper())
                                     .arg(idHabitat)
                                     .arg(eauConso, 0, 'f', 2)
                                     .arg(moyConso, 0, 'f', 2)
                                     .arg(dateReleve);

            // Envoyer le SMS
            bool smsEnvoye = envoyerSMSTwilio(numeroTelephone, messageSMS);

            if(smsEnvoye) {
                // Mettre à jour le statut SMS dans la base
                QSqlQuery updateSMS;
                updateSMS.prepare("UPDATE CONSOMMATION SET SMS_ENVOYE = 1 "
                                  "WHERE ID_HABITAT = ? AND DATE_RELEVE = TO_DATE(?, 'DD/MM/YYYY HH24:MI')");
                updateSMS.addBindValue(idHabitat);
                updateSMS.addBindValue(dateReleve);
                updateSMS.exec();

                // Notification à l'utilisateur
                QMessageBox::information(this, "Succès",
                                         QString("✅ Consommation ajoutée avec succès !\n\n"
                                                 "📱 SMS d'alerte envoyé à : %1")
                                             .arg(numeroTelephone));
            }
            else {
                QMessageBox::warning(this, "Attention",
                                     "⚠️ Consommation ajoutée mais l'envoi du SMS a échoué.");
            }
        }
        else {
            QMessageBox::warning(this, "Attention",
                                 QString("⚠️ %1 détectée mais aucun numéro de téléphone enregistré !")
                                     .arg(typeAnomalie));
        }
    }
    else {
        QMessageBox::information(this, "Succès",
                                 QString("✅ Consommation ajoutée avec succès !\n\n"
                                         "Type : %1 %2")
                                     .arg(icone)
                                     .arg(typeAnomalie));
    }

    // ========================================
    // 🔄 ACTUALISATION ET RÉINITIALISATION
    // ========================================
    afficherConsommations();

    // Réinitialiser les champs
    ui->ID_HABITAT2->clear();
    ui->EAU_CONSO->clear();
    ui->MOY_CONSO->clear();
    ui->DATE_RELEVE->setDateTime(QDateTime::currentDateTime());
}
// =======================
// MODIFIER CONSOMMATION
// =======================
void MainWindow::on_btnModifierConso_clicked()
{
    int row = ui->table_surveillance->currentRow();
    if(row < 0) {
        QMessageBox::warning(this, "Attention", "⚠️ Sélectionnez d'abord une consommation !");
        return;
    }

    QString idConso = ui->table_surveillance->item(row, 1)->text();
    QString ancienneValeur = ui->table_surveillance->item(row, 2)->text();

    bool ok;
    QString nouvelleValeur = QInputDialog::getText(this,
                                                   "Modifier consommation",
                                                   "Nouvelle consommation d'eau (m³):",
                                                   QLineEdit::Normal,
                                                   ancienneValeur,
                                                   &ok);

    if(!ok || nouvelleValeur.isEmpty()) {
        return;
    }

    QSqlQuery query;
    query.prepare("UPDATE CONSOMMATION SET EAU_CONSO=? WHERE ID_CONSO=?");
    query.addBindValue(nouvelleValeur);
    query.addBindValue(idConso);

    if(!query.exec())
        QMessageBox::critical(this, "Erreur", "Impossible de modifier: " + query.lastError().text());
    else {
        QMessageBox::information(this, "Succès", "✅ Consommation modifiée !");
        afficherConsommations();
    }
}

// =======================
// SUPPRIMER CONSOMMATION
// =======================
void MainWindow::on_btnSupprimerConso_clicked()
{
    int row = ui->table_surveillance->currentRow();
    if(row < 0) {
        QMessageBox::warning(this, "Attention", "⚠️ Sélectionnez une consommation !");
        return;
    }

    QString idConsoText = ui->table_surveillance->item(row, 1)->text();
    // Retirer le "C"
    QString idConso = idConsoText.mid(1);

    int reponse = QMessageBox::question(this, "Confirmation",
                                        QString("Voulez-vous vraiment supprimer la consommation %1 ?").arg(idConsoText),
                                        QMessageBox::Yes | QMessageBox::No);

    if(reponse == QMessageBox::No) return;

    QSqlQuery query;
    query.prepare("DELETE FROM CONSOMMATION WHERE ID_CONSO=?");
    query.addBindValue(idConso);

    if(!query.exec())
        QMessageBox::critical(this, "Erreur", "Impossible de supprimer: " + query.lastError().text());
    else {
        QMessageBox::information(this, "Succès", "✅ Consommation supprimée !");
        afficherConsommations();
    }
}

// =======================
// CLICK SUR TABLE
// =======================
void MainWindow::on_table_surveillance_itemClicked(QTableWidgetItem *item)
{
    Q_UNUSED(item);
}

// =======================
// 📱 FONCTION ENVOI SMS TWILIO - CORRIGÉE
// =======================
bool MainWindow::envoyerSMSTwilio(const QString &numeroDestinataire, const QString &message)
{
    // Vérifier que les identifiants Twilio sont configurés
    if(TWILIO_ACCOUNT_SID.isEmpty() || TWILIO_AUTH_TOKEN.isEmpty() || TWILIO_PHONE_NUMBER.isEmpty()) {
        qDebug() << "❌ ERREUR: Identifiants Twilio non configurés !";
        return false;
    }

    // Vérifier les paramètres
    if(numeroDestinataire.isEmpty() || message.isEmpty()) {
        qDebug() << "❌ ERREUR: Numéro ou message vide !";
        return false;
    }

    // Construire l'URL de l'API Twilio
    QUrl url("https://api.twilio.com/2010-04-01/Accounts/" + TWILIO_ACCOUNT_SID + "/Messages.json");

    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    // Préparer les données POST
    QUrlQuery postData;
    postData.addQueryItem("From", TWILIO_PHONE_NUMBER);
    postData.addQueryItem("To", numeroDestinataire);
    postData.addQueryItem("Body", message);

    QByteArray data = postData.toString(QUrl::FullyEncoded).toUtf8();

    // Authentification Basic
    QString concatenation = TWILIO_ACCOUNT_SID + ":" + TWILIO_AUTH_TOKEN;
    QByteArray auth = "Basic " + concatenation.toUtf8().toBase64();
    request.setRawHeader("Authorization", auth);

    // Envoyer la requête POST
    QNetworkReply *reply = networkManager->post(request, data);

    // Attendre la réponse de manière synchrone
    QEventLoop loop;
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    // Vérifier le résultat
    bool success = false;
    if(reply->error() == QNetworkReply::NoError) {
        QByteArray response = reply->readAll();
        qDebug() << "✅ SMS envoyé avec succès !";
        qDebug() << "Réponse Twilio:" << response;
        success = true;
    }
    else {
        qDebug() << "❌ Erreur lors de l'envoi du SMS:" << reply->errorString();
        success = false;
    }

    reply->deleteLater();
    return success;
}

// =======================
// TRAITER RÉPONSE SMS (OPTIONNEL - Pour usage asynchrone)
// =======================


// =======================
// VÉRIFIER GASPILLAGES ET ENVOYER SMS
// =======================
void MainWindow::verifierGaspillagesEtEnvoyerSMS()
{
    // Récupérer toutes les consommations anormales non notifiées
    QSqlQuery query;
    query.prepare("SELECT C.ID_HABITAT, C.EAU_CONSO, C.MOY_CONSO, H.TELEPHONE_CONTACT "
                  "FROM CONSOMMATION C "
                  "JOIN HABITAT H ON C.ID_HABITAT = H.ID_HABITAT "
                  "WHERE C.SMS_ENVOYE = 0 AND C.EAU_CONSO > C.MOY_CONSO * 1.5");

    if(!query.exec()) {
        qDebug() << "Erreur requête gaspillages:" << query.lastError().text();
        return;
    }

    int compteur = 0;
    while(query.next()) {
        QString idHabitat = query.value(0).toString();
        double eauConso = query.value(1).toDouble();
        double moyConso = query.value(2).toDouble();
        QString telephone = query.value(3).toString();

        if(!telephone.isEmpty()) {
            QString message = QString("⚠️ ALERTE AUTOMATIQUE\n"
                                      "Habitat: H%1\n"
                                      "Consommation anormale détectée: %2 m³\n"
                                      "Moyenne: %3 m³")
                                  .arg(idHabitat)
                                  .arg(eauConso, 0, 'f', 2)
                                  .arg(moyConso, 0, 'f', 2);

            if(envoyerSMSTwilio(telephone, message)) {
                // Mettre à jour le statut
                QSqlQuery updateQuery;
                updateQuery.prepare("UPDATE CONSOMMATION SET SMS_ENVOYE = 1 WHERE ID_HABITAT = ?");
                updateQuery.addBindValue(idHabitat);
                updateQuery.exec();
                compteur++;
            }
        }
    }

    if(compteur > 0) {
        QMessageBox::information(this, "Alertes envoyées",
                                 QString("✅ %1 SMS d'alerte envoyés !").arg(compteur));
    }
}

// =======================
// ACTIVER ALERTES AUTOMATIQUES
// =======================
void MainWindow::on_btnActiverAlertesAuto_clicked()
{
    if(!timerAlertes->isActive()) {
        timerAlertes->start(60000); // Vérifier toutes les 60 secondes
        QMessageBox::information(this, "Alertes activées",
                                 "✅ Surveillance automatique activée !\n"
                                 "Vérification toutes les 60 secondes.");
    }
    else {
        timerAlertes->stop();
        QMessageBox::information(this, "Alertes désactivées",
                                 "⏸️ Surveillance automatique désactivée.");
    }
}


bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
    Q_UNUSED(obj)
    Q_UNUSED(event)
    return QMainWindow::eventFilter(obj, event);
}

// =======================
// AFFICHAGE HABITATS
// =======================
void MainWindow::afficherHabitats()
{
    ui->TabCrudHabitat->setRowCount(0);
    QSqlQuery query("SELECT * FROM HABITAT");
    int row = 0;
    while(query.next()) {
        ui->TabCrudHabitat->insertRow(row);
        for(int col = 0; col < 6; col++)
            ui->TabCrudHabitat->setItem(row, col, new QTableWidgetItem(query.value(col).toString()));
        row++;
    }
}



// =======================
// TRI ET STATISTIQUES
// =======================
void MainWindow::on_btnStatistiques_clicked()
{
    QSqlQuery query;

    query.exec("SELECT COUNT(*), SUM(EAU_CONSO), AVG(EAU_CONSO), "
               "MAX(EAU_CONSO), MIN(EAU_CONSO) FROM CONSOMMATION");

    if(!query.next()) {
        QMessageBox::warning(this, "Attention", "Aucune donnée disponible.");
        return;
    }

    int total = query.value(0).toInt();
    double somme = query.value(1).toDouble();
    double moyenne = query.value(2).toDouble();
    double maximum = query.value(3).toDouble();
    double minimum = query.value(4).toDouble();

    // Statistiques par type
    QSqlQuery queryType;
    queryType.exec("SELECT TYPE_ANOMALIE, COUNT(*), SUM(EAU_CONSO) "
                   "FROM CONSOMMATION GROUP BY TYPE_ANOMALIE "
                   "ORDER BY COUNT(*) DESC");

    QString detailsTypes = "";
    while(queryType.next()) {
        QString type = queryType.value(0).toString();
        int count = queryType.value(1).toInt();
        double sommeType = queryType.value(2).toDouble();

        QString couleur = "#4CAF50";
        if(type == "Fuite") couleur = "#F44336";
        else if(type == "Gaspillage") couleur = "#FF9800";
        else if(type == "Sous-Consommation") couleur = "#2196F3";

        detailsTypes += QString(
                            "<tr>"
                            "<td style='padding:8px; border-bottom:1px solid #ddd;'><span style='color:%1; font-size:20px;'>●</span> <b>%2</b></td>"
                            "<td style='padding:8px; text-align:center; border-bottom:1px solid #ddd;'>%3</td>"
                            "<td style='padding:8px; text-align:right; border-bottom:1px solid #ddd;'><b>%4 m³</b></td>"
                            "</tr>"
                            ).arg(couleur).arg(type).arg(count).arg(sommeType, 0, 'f', 2);
    }

    QString message = QString(
                          "<html><body style='font-family: Segoe UI;'>"
                          "<div style='background: linear-gradient(135deg, #667eea 0%, #764ba2 100%); color: white; padding: 20px; border-radius: 10px; text-align: center; margin-bottom: 20px;'>"
                          "<h1 style='margin:0;'>📊 STATISTIQUES</h1>"
                          "<p style='margin:5px 0 0 0;'>Analyse des consommations d'eau</p>"
                          "</div>"

                          "<table style='width:100%; border-collapse: collapse;'>"
                          "<tr>"
                          "<td style='background:#f8f9fa; border-left:4px solid #667eea; padding:15px; border-radius:8px;'>"
                          "<div style='color:#6c757d; font-size:12px;'>📈 TOTAL</div>"
                          "<div style='color:#667eea; font-size:24px; font-weight:bold;'>%1</div>"
                          "</td>"
                          "<td style='background:#f8f9fa; border-left:4px solid #667eea; padding:15px; border-radius:8px;'>"
                          "<div style='color:#6c757d; font-size:12px;'>💧 SOMME</div>"
                          "<div style='color:#667eea; font-size:24px; font-weight:bold;'>%2 m³</div>"
                          "</td>"
                          "</tr>"
                          "<tr><td colspan='2' style='height:10px;'></td></tr>"
                          "<tr>"
                          "<td style='background:#f8f9fa; border-left:4px solid #667eea; padding:15px; border-radius:8px;'>"
                          "<div style='color:#6c757d; font-size:12px;'>📊 MOYENNE</div>"
                          "<div style='color:#667eea; font-size:24px; font-weight:bold;'>%3 m³</div>"
                          "</td>"
                          "<td style='background:#f8f9fa; border-left:4px solid #667eea; padding:15px; border-radius:8px;'>"
                          "<div style='color:#6c757d; font-size:12px;'>⚡ MAX / 🔻 MIN</div>"
                          "<div style='color:#F44336; font-size:20px; font-weight:bold;'>%4 m³</div>"
                          "<div style='color:#4CAF50; font-size:20px; font-weight:bold;'>%5 m³</div>"
                          "</td>"
                          "</tr>"
                          "</table>"

                          "<h3 style='color:#667eea; border-bottom:2px solid #667eea; padding:10px 0;'>📋 Répartition par Type</h3>"
                          "<table style='width:100%; border-collapse:collapse;'>"
                          "<thead><tr style='background:#667eea; color:white;'>"
                          "<th style='padding:12px; text-align:left;'>Type</th>"
                          "<th style='padding:12px; text-align:center;'>Nombre</th>"
                          "<th style='padding:12px; text-align:right;'>Consommation</th>"
                          "</tr></thead>"
                          "<tbody>%6</tbody>"
                          "</table>"
                          "</body></html>"
                          )
                          .arg(total)
                          .arg(somme, 0, 'f', 2)
                          .arg(moyenne, 0, 'f', 2)
                          .arg(maximum, 0, 'f', 2)
                          .arg(minimum, 0, 'f', 2)
                          .arg(detailsTypes);

    QMessageBox msgBox(this);
    msgBox.setWindowTitle("📊 Statistiques");
    msgBox.setTextFormat(Qt::RichText);
    msgBox.setText(message);
    msgBox.setStyleSheet(
        "QMessageBox { min-width: 650px; min-height: 500px; }"
        "QPushButton { background: #667eea; color: white; border-radius: 6px; padding: 10px 30px; font-weight: bold; }"
        );
    msgBox.exec();
}

void MainWindow::on_lineRechercheEtat_textChanged(const QString &text)
{
    // Ignorer le texte placeholder
    if(text.contains("🔍") || text.trimmed().isEmpty()) {
        afficherConsommations();
        return;
    }

    ui->table_surveillance->setRowCount(0);

    QSqlQuery query;
    query.prepare("SELECT c.ID_HABITAT, c.ID_CONSO, c.EAU_CONSO, c.MOY_CONSO, "
                  "c.TYPE_ANOMALIE, c.SOURCE_DETECTION, c.SMS_ENVOYE, "
                  "c.LOCALISATION_FUITE, TO_CHAR(c.DATE_RELEVE, 'DD/MM/YYYY HH24:MI') "
                  "FROM CONSOMMATION c "
                  "WHERE UPPER(c.TYPE_ANOMALIE) LIKE UPPER(?) OR UPPER(c.SOURCE_DETECTION) LIKE UPPER(?) "
                  "ORDER BY " + critereTri);

    QString recherche = "%" + text.trimmed() + "%";
    query.addBindValue(recherche);
    query.addBindValue(recherche);

    if(!query.exec()) {
        QMessageBox::critical(this, "Erreur", "Erreur recherche: " + query.lastError().text());
        return;
    }

    int row = 0;
    while(query.next()) {
        ui->table_surveillance->insertRow(row);

        // Colonne 0: ID Habitat
        ui->table_surveillance->setItem(row, 0,
                                        new QTableWidgetItem("H" + query.value(0).toString()));

        // Colonne 1: ID Conso
        ui->table_surveillance->setItem(row, 1,
                                        new QTableWidgetItem("C" + query.value(1).toString()));

        // Colonne 2: Eau Conso
        ui->table_surveillance->setItem(row, 2,
                                        new QTableWidgetItem(query.value(2).toString()));

        // Colonne 3: Moy Conso
        ui->table_surveillance->setItem(row, 3,
                                        new QTableWidgetItem(query.value(3).toString()));

        // Colonne 4: Type Anomalie (avec couleurs)
        QString typeAnomalie = query.value(4).toString();
        QTableWidgetItem *itemType = new QTableWidgetItem(typeAnomalie);

        if(typeAnomalie == "Fuite") {
            itemType->setBackground(QColor(255, 100, 100));
            itemType->setForeground(QColor(255, 255, 255));
        }
        else if(typeAnomalie == "Gaspillage") {
            itemType->setBackground(QColor(255, 200, 100));
        }
        else if(typeAnomalie == "Sous-Consommation") {
            itemType->setBackground(QColor(255, 245, 200));
        }
        else {
            itemType->setBackground(QColor(200, 255, 200));
        }
        ui->table_surveillance->setItem(row, 4, itemType);

        // Colonne 5: Source
        ui->table_surveillance->setItem(row, 5,
                                        new QTableWidgetItem(query.value(5).toString()));

        // Colonne 6: SMS Envoyé
        QString smsText = (query.value(6).toInt() == 1) ? "✅" : "⏳";
        ui->table_surveillance->setItem(row, 6,
                                        new QTableWidgetItem(smsText));

        // Colonne 7: Localisation
        QString loc = query.value(7).toString();
        ui->table_surveillance->setItem(row, 7,
                                        new QTableWidgetItem(loc.isEmpty() ? "-" : loc));

        // Colonne 8: Date
        ui->table_surveillance->setItem(row, 8,
                                        new QTableWidgetItem(query.value(8).toString()));

        row++;
    }

    ui->table_surveillance->resizeColumnsToContents();
}

// =======================
// EXPORTATION DONNÉES
// =======================

void MainWindow::on_pushButton_exporter_clicked()
{
    // Demander à l'utilisateur où sauvegarder le fichier
    QString fileName = QFileDialog::getSaveFileName(this,
                                                    "Exporter les données",
                                                    QDir::homePath() + "/habitats_export.csv",
                                                    "Fichiers CSV (*.csv);;Tous les fichiers (*)");

    if(fileName.isEmpty()) {
        return; // L'utilisateur a annulé
    }

    // Créer et ouvrir le fichier
    QFile file(fileName);
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "Erreur",
                              "Impossible de créer le fichier : " + file.errorString());
        return;
    }

    QTextStream out(&file);
    out.setEncoding(QStringConverter::Utf8); // Support des caractères spéciaux

    // Écrire les en-têtes
    QStringList headers;
    for(int col = 0; col < ui->TabCrudHabitat->columnCount(); col++) {
        headers << ui->TabCrudHabitat->horizontalHeaderItem(col)->text();
    }
    out << headers.join(";") << "\n";

    // Écrire les données
    for(int row = 0; row < ui->TabCrudHabitat->rowCount(); row++) {
        QStringList rowData;
        for(int col = 0; col < ui->TabCrudHabitat->columnCount(); col++) {
            QTableWidgetItem *item = ui->TabCrudHabitat->item(row, col);
            rowData << (item ? item->text() : "");
        }
        out << rowData.join(";") << "\n";
    }

    file.close();

    // Message de confirmation
    QMessageBox::information(this, "Succès",
                             QString("✅ Données exportées avec succès !\n\n"
                                     "📁 Fichier : %1\n"
                                     "📊 Lignes exportées : %2")
                                 .arg(fileName)
                                 .arg(ui->TabCrudHabitat->rowCount()));
}
void MainWindow::on_comboTriConso_currentIndexChanged()
{
    int index = ui->comboTriConso->currentIndex();  // Récupérer l'index manuellement

    switch(index){
    case 0: critereTri = "DATE_RELEVE DESC"; break;           // Plus récent
    case 1: critereTri = "DATE_RELEVE ASC"; break;            // Plus ancien
    case 2: critereTri = "EAU_CONSO DESC"; break;             // Élevée → Faible
    case 3: critereTri = "EAU_CONSO ASC"; break;              // Faible → Élevée
    case 4: critereTri = "TYPE_ANOMALIE DESC"; break;         // Fuites d'abord
    case 5: critereTri = "ID_HABITAT ASC"; break;             // ID Habitat
    default: critereTri = "DATE_RELEVE DESC";
    }
    afficherConsommations();
}
