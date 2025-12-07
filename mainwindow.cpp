#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "poubelle.h"
#include "personnel.h"
#include "habitantcrud.h"
#include "vehiculecrud.h"
#include <QHeaderView>
#include <QPushButton>
#include <QMessageBox>
#include <QDebug>
#include <QInputDialog>
#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include <QCompleter>
#include <QListView>
#include <QLineEdit>
#include <QSettings>
#include <QItemSelectionModel>
#include <QRegularExpression>
#include <QFileDialog>
#include <QTextStream>
#include <QPdfWriter>
#include <QPainter>
#include <cmath>
#include <QTableWidgetItem>
#include <QStandardPaths>
#include <QDir>
#include <QFile>
#include <QPrinter>
#include <QTextDocument>
#include <QStringList>
#include <QDialog>
#include <QVBoxLayout>
#include <QPlainTextEdit>
#include <QHeaderView>
#include <QMap>
#include <tuple>
#include <QLabel>
#include "graph.h"
#include <QSqlQuery>
#include <QGraphicsRectItem>
#include <QGraphicsSimpleTextItem>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QHeaderView>
#include <QPushButton>
#include <QMessageBox>
#include <QDebug>
#include <QInputDialog>
#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QTableView>
#include <QAbstractItemView>
#include "zone.h"
#include "poubelle.h"
#include "personnel.h"
#include "habitantcrud.h"
#include "vehiculecrud.h"
#include "satisfaction.h"
#include "demandecrud.h"
#include "affectationp.h"
#include <QSqlError>
#include <QSqlQueryModel>
#include <QCompleter>
#include <QListView>
#include <QLineEdit>
#include <QSettings>
#include <QItemSelectionModel>
#include <QRegularExpression>
#include <QDir>
#include <QStandardPaths>
#include <QDateTime>
#include <QPageSize>
#include <QMenu>
#include <QAction>
#include <QFileDialog>
#include <QTextEdit>
#include <QComboBox>
#include <QInputDialog>
#include <QSqlQuery>
#include <QSqlError>
#include <QtCharts/QChart>
#include <QtCharts/QPieSeries>
#include <QtCharts/QChartView>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QValueAxis>
#include <QProgressBar>
#include <QSpinBox>
#include <QSlider>
#include <QStandardItemModel>
#include <QDialog>
#include <QVBoxLayout>
#include <QUrl>
#include <QUrlQuery>
#include <QDesktopServices>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QPixmap>
#include <QPrinter>
#include <QPageLayout>
#include <QPainter>

#include <QTimer>
#include <QGroupBox>
#include <QFrame>
#include <QScrollBar>
static const double DEMANDE_DELAI_HEURES = 8.0;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , arduino(new Arduino(this))
    , derniereTemperature(0.0)  // Initialiser la température
{
    ui->setupUi(this);

    // ⭐ CHARGER LES DONNÉES AU DÉMARRAGE
    qDebug() << "🚀 Chargement initial des consommations...";
    afficherConsommations();

    // Configuration Twilio
    TWILIO_ACCOUNT_SID = "AC00f47fd2aaba48807f813f80acce555b";
    TWILIO_AUTH_TOKEN = "bf539f5e5bf63a8cb10ce4bf945a958c";
    TWILIO_PHONE_NUMBER = "+17573566519";

    // ✅ Connecter le signal de température Arduino
    connect(arduino, &Arduino::temperatureRecue,
            this, &MainWindow::onTemperatureRecue);

    // ✅ Connexion automatique à l'Arduino sur COM
    QString port = "COM3";
    qDebug() << "Tentative de connexion automatique à" << port;

    if(arduino->connecter(port)) {
        ui->labelStatus->setText("✅ Connecté à " + port);
        qDebug() << "✅ Connexion Arduino réussie !";
    } else {
        ui->labelStatus->setText("❌ Erreur : Arduino non connecté");
        qDebug() << "❌ Échec de connexion à" << port;
        qDebug() << "Vérifiez que :";
        qDebug() << "  - L'Arduino est branché";
        qDebug() << "  - Arduino IDE est fermé";
        qDebug() << "  - Le port est bien" << port;
    }




    graphScene = new Graph(this);
    zonesScene = new QGraphicsScene(this);
    ui->graphicsZones_2->setScene(zonesScene);
    ui->graphicsZones_2->setRenderHint(QPainter::Antialiasing);

    binsScene = new QGraphicsScene(this);
    ui->graphicsBins_2->setScene(binsScene);
    ui->graphicsBins_2->setRenderHint(QPainter::Antialiasing);
    if (!binMessageLabel) {
        QWidget *parent = ui->graphicsBins_2->parentWidget();
        binMessageLabel = new QLabel(parent);
        binMessageLabel->setWordWrap(true);
        binMessageLabel->setStyleSheet("font-weight:bold; color:green;");
        // place under the view based on current geometry; will not auto-resize but fine for standard UI
        QRect g = ui->graphicsBins_2->geometry();
        binMessageLabel->setGeometry(g.x(), g.y() + g.height() + 4, g.width(), 48);
        binMessageLabel->show();
    }

    reloadBinsCache();
    affectationSummaryLabel = nullptr;
    qApp->setStyleSheet(
        "QMessageBox QLabel, QInputDialog QLabel, QLineEdit { color: black; }"
        "QMessageBox QPushButton, QInputDialog QPushButton { color: black; }"
        );

    setupTable();
    refreshGraph();
    // === Habitant helpers (validators, styles, statuses, data load) ===
    chargerStatutsAutorises();
    styliserChampsSaisie();
    chargerDonneesTable();
    loadLastHabitantSelection();

    // Initialiser Arduino
    arduino = new Arduino();
    if(arduino->connect_arduino() == 0) {
        qDebug() << "Arduino connecté avec succès";
    } else {
        qDebug() << "Échec de la connexion à Arduino";
    }

    // Recherche live
    connect(ui->lineEdit_rechH, &QLineEdit::textChanged, this, [this](const QString &t) {
        rechercherTexte(t);
    });

    // Validators
    ui->graphicsZones_2->setRenderHint(QPainter::Antialiasing);
    ui->lineEdit_id->setValidator(new QIntValidator(1, 2147483647, this));
    ui->lineEdit_satisfaction->setValidator(new QIntValidator(0, 100, this));
    ui->lineEdit_contact->setValidator(new QRegularExpressionValidator(QRegularExpression("^\\d{0,15}$"), this));

    // Options de tri
    if (ui->comboBox_trieH) {
        ui->comboBox_trieH->clear();
        ui->comboBox_trieH->addItems(QStringList() << "Nom" << "Prénom" << "Adresse" << "Satisfaction");
        ui->comboBox_trieH->setCurrentIndex(0);
    }

    // === Préparation UI Demande ===
    // ID habitant: placeholder + validateur entier positif
    if (ui->IDdhabitant) {
        ui->IDdhabitant->setPlaceholderText("Entrer l'ID de l'habitant");
        ui->IDdhabitant->setValidator(new QIntValidator(1, 2147483647, this));
    }

    // Reclamation: placeholder (LineEdit ou TextEdit)
    if (ui->reclamation) {
        if (auto le = qobject_cast<QLineEdit*>(ui->reclamation)) {
            le->setPlaceholderText("Décrivez votre demande / réclamation...");
        } else if (auto te = qobject_cast<QTextEdit*>(ui->reclamation)) {
            te->setPlaceholderText("Décrivez votre demande / réclamation...");
        }
    }

    // ProblemeH: peupler la liste si vide
    if (ui->problemeH && ui->problemeH->count() == 0) {
        ui->problemeH->addItems(QStringList{
            "problème technique",
            "propreté/collecte déchets",
            "éclairage public",
            "sécurité",
            "eau/électricité",
            "voirie/infrastructures",
            "réclamation service",
            "autre"
        });
    }

    // Compétence du personnel : utiliser une combobox avec des valeurs prédéfinies
    if (ui->competence && ui->competence->count() == 0) {
        ui->competence->addItems(QStringList{
            "problème technique",
            "problème numérique",
            "problème civique",
            "Logement et habitat",
            "Environnement et propreté",
            "Infrastructures",
            "Nouvel élément",
            "transport",
            "Sécurité",
            "Vie sociale",
            "Éducation"
        });
    }

    // Filtre initial: aucun (tous les statuts)
    demandeStatusFilter.clear();

    // Charger les demandes existantes dans le tableau dédié et styliser le tableau
    if (ui->tabdemande) {
        ui->tabdemande->setAlternatingRowColors(true);
        ui->tabdemande->setSelectionBehavior(QAbstractItemView::SelectRows);
        ui->tabdemande->setSelectionMode(QAbstractItemView::SingleSelection);
        ui->tabdemande->verticalHeader()->setVisible(false);
        ui->tabdemande->horizontalHeader()->setStretchLastSection(true);
        ui->tabdemande->setShowGrid(true);

        // Menu contextuel pour filtrer par statut
        ui->tabdemande->setContextMenuPolicy(Qt::CustomContextMenu);
        connect(ui->tabdemande, &QTableView::customContextMenuRequested, this,
                [this](const QPoint &pos){
                    QMenu menu(this);
                    QAction *actTous     = menu.addAction(tr("Tous les statuts"));
                    menu.addSeparator();
                    QAction *actNouveau  = menu.addAction(tr("Nouveau"));
                    QAction *actEnCours  = menu.addAction(tr("En_cours"));
                    QAction *actCloture  = menu.addAction(tr("Cloture"));
                    QAction *actEchoue   = menu.addAction(tr("Echoue"));
                    QAction *chosen = menu.exec(ui->tabdemande->viewport()->mapToGlobal(pos));
                    if (!chosen) return;
                    if (chosen == actTous)        demandeStatusFilter.clear();
                    else if (chosen == actNouveau) demandeStatusFilter = "Nouveau";
                    else if (chosen == actEnCours) demandeStatusFilter = "En_cours";
                    else if (chosen == actCloture) demandeStatusFilter = "Cloture";
                    else if (chosen == actEchoue)  demandeStatusFilter = "Echoue";
                    refreshDemandeTable();
                });
    }
    refreshDemandeTable();

    // Contexte rapide pour choisir l'emplacement: Bureau / Téléchargements / Parcourir…
    if (ui->emplacementH) {
        ui->emplacementH->setPlaceholderText("Chemin du dossier (Bureau/Téléchargements/Parcourir…)");
        ui->emplacementH->setContextMenuPolicy(Qt::CustomContextMenu);
        connect(ui->emplacementH, &QLineEdit::customContextMenuRequested, this, [this](const QPoint &pos){
            QMenu menu(this);
            const QString bureau = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
            const QString telech = QStandardPaths::writableLocation(QStandardPaths::DownloadLocation);
            QAction *aBureau = menu.addAction(QString::fromUtf8("Bureau (%1)").arg(bureau));
            QAction *aTelech = menu.addAction(QString::fromUtf8("Téléchargements (%1)").arg(telech));
            menu.addSeparator();
            QAction *aParcourir = menu.addAction("Parcourir…");
            QAction *sel = menu.exec(ui->emplacementH->mapToGlobal(pos));
            if (!sel) return;
            if (sel == aBureau) ui->emplacementH->setText(bureau);
            else if (sel == aTelech) ui->emplacementH->setText(telech);
            else if (sel == aParcourir) {
                QString dir = QFileDialog::getExistingDirectory(this, "Choisir un dossier", telech.isEmpty()? QDir::homePath() : telech);
                if (!dir.isEmpty()) ui->emplacementH->setText(dir);
            }
        });
    }

    connect(ui->btnLoadMap, &QPushButton::clicked, this, &MainWindow::refreshGraph);
    connect(ui->exp, &QPushButton::clicked, this, &MainWindow::on_exp_clicked);
    connect(ui->tri, &QPushButton::clicked, this, &MainWindow::on_tri_clicked);
    connect(ui->affzone, &QPushButton::clicked, this, &MainWindow::afficherZones);
    connect(ui->affpoub, &QPushButton::clicked, this, &MainWindow::afficherPoubelles);
    connect(ui->modzone, &QPushButton::clicked, this, &MainWindow::modZone);
    connect(ui->modpoub, &QPushButton::clicked, this, &MainWindow::modPoubelle);
    connect(ui->twp, &QTableWidget::cellClicked,
            this, &MainWindow::on_twp_cellClicked);

    // === Satisfaction tab wiring (optional if widgets exist) ===
    if (QPushButton *btn = findChild<QPushButton*>("btn_satisfaction_refresh")) {
        connect(btn, &QPushButton::clicked, this, &MainWindow::on_btn_satisfaction_refresh_clicked);
    }
    if (QPushButton *btn2 = findChild<QPushButton*>("btn_sondage_submit")) {
        connect(btn2, &QPushButton::clicked, this, &MainWindow::on_btn_sondage_submit_clicked);
    }
    if (QPushButton *btn3 = findChild<QPushButton*>("affichersatisfaction")) {
        connect(btn3, &QPushButton::clicked, this, &MainWindow::on_affichersatisfaction_clicked);
    }
    if (auto slider = findChild<QSlider*>("slider_satisfaction_score")) {
        slider->setMinimum(0);
        slider->setMaximum(10);
        slider->setTickInterval(1);
        slider->setTickPosition(QSlider::TicksBelow);

        auto applyStyle = [slider](int value) {
            QString color;
            if (value <= 3)
                color = "#e74c3c";
            else if (value <= 7)
                color = "#f39c12";
            else
                color = "#27ae60";

            slider->setStyleSheet(QString(
                                      "QSlider::groove:horizontal { height: 6px; background: #d0d7e2; border-radius: 3px; }"
                                      "QSlider::handle:horizontal { background: %1; border-radius: 9px; width: 18px; margin: -6px 0; }"
                                      "QSlider::sub-page:horizontal { background: %1; border-radius: 3px; }"
                                      ).arg(color));
        };

        applyStyle(slider->value());
        connect(slider, &QSlider::valueChanged, this, [applyStyle](int v) { applyStyle(v); });
    }
    if (auto lbl = findChild<QLabel*>("label_sondage_score")) {
        lbl->setText(tr("0 = très insatisfait   5 = neutre   10 = très satisfait"));
    }
    if (auto le = findChild<QLineEdit*>("edit_sondage_commentaire")) {
        le->setPlaceholderText(tr("Exprimez votre avis..."));
    }
    // Ne pas auto-rafraîchir au démarrage pour éviter d'affecter une mauvaise vue
    // L'utilisateur déclenchera l'affichage via les boutons.

    // Styliser le module de gestion des personnels (tables + boutons)
    styliserModulePersonnels();

    // Boutons supplémentaires sur la page d'affectation pour un usage plus professionnel
    if (ui->page_15) {
        // Bandeau de résumé dynamique sous les tableaux
        if (!affectationSummaryLabel) {
            affectationSummaryLabel = new QLabel(ui->page_15);
            affectationSummaryLabel->setGeometry(60, 390, 1070, 40);
            affectationSummaryLabel->setAlignment(Qt::AlignCenter);
            affectationSummaryLabel->setWordWrap(true);
            affectationSummaryLabel->setStyleSheet(
                "QLabel { color:#0a1a2f; font-size:11px; background-color:transparent; }");
        }

        // Boutons de filtre rapides
        QPushButton *btnUrgences = new QPushButton(tr("⚠ Urgences"), ui->page_15);
        btnUrgences->setGeometry(60, 360, 110, 26);
        styliserBoutonPrincipal(btnUrgences);
        connect(btnUrgences, &QPushButton::clicked, this, [this]() {
            chargerTablesAffectation(true, false);
        });

        QPushButton *btnDispo = new QPushButton(tr("✅ Disponibles"), ui->page_15);
        btnDispo->setGeometry(180, 360, 130, 26);
        styliserBoutonPrincipal(btnDispo);
        connect(btnDispo, &QPushButton::clicked, this, [this]() {
            chargerTablesAffectation(false, true);
        });

        QPushButton *btnTout = new QPushButton(tr("↺ Tout"), ui->page_15);
        btnTout->setGeometry(320, 360, 90, 26);
        styliserBoutonPrincipal(btnTout);
        connect(btnTout, &QPushButton::clicked, this, [this]() {
            chargerTablesAffectation(false, false);
        });

        QPushButton *btnDetails = new QPushButton(tr("🔍 Détails demande"), ui->page_15);
        btnDetails->setGeometry(380, 440, 120, 29);
        styliserBoutonPrincipal(btnDetails);

        connect(btnDetails, &QPushButton::clicked, this, [this]() {
            if (!ui || !ui->tableView_habitants_2)
                return;

            QItemSelectionModel *selDem = ui->tableView_habitants_2->selectionModel();
            if (!selDem || !selDem->hasSelection()) {
                QMessageBox::information(this, tr("Détails demande"),
                                         tr("Veuillez sélectionner une demande."));
                return;
            }
            int rowDem = selDem->selectedRows().first().row();
            QAbstractItemModel *modelDem = ui->tableView_habitants_2->model();
            if (!modelDem)
                return;

            bool okId = false;
            int idDemande = modelDem->index(rowDem, 0).data().toInt(&okId);
            if (!okId || idDemande <= 0) {
                QMessageBox::warning(this, tr("Détails demande"),
                                     tr("ID de demande invalide."));
                return;
            }

            auto fmtDate = [](const QDateTime &dt) {
                return dt.isValid() ? dt.toString("dd/MM/yyyy HH:mm") : QString("-");
            };

            QSqlQuery q;
            q.prepare("SELECT ID_DEMANDE, ID_HABITANT, TYPE_PROBLEME, DESCRIPTION, STATUT, "
                      "DATE_CREATION, DATE_AFFECTATION, DATE_RESOLUTION_INTERNE, DATE_RESOLUTION_FINALE "
                      "FROM DEMANDE WHERE ID_DEMANDE = :id");
            q.bindValue(":id", idDemande);
            if (!q.exec()) {
                QMessageBox::warning(this, tr("Détails demande"),
                                     tr("Erreur SQL : %1").arg(q.lastError().text()));
                return;
            }
            if (!q.next()) {
                QMessageBox::information(this, tr("Détails demande"),
                                         tr("Aucune demande trouvée pour l'ID %1.").arg(idDemande));
                return;
            }

            int idDem    = q.value(0).toInt();
            int idHab    = q.value(1).toInt();
            QString type = q.value(2).toString();
            QString desc = q.value(3).toString();
            QString statut = q.value(4).toString();
            QDateTime dc  = q.value(5).toDateTime();
            QDateTime da  = q.value(6).toDateTime();
            QDateTime dr  = q.value(7).toDateTime();
            QDateTime dru = q.value(8).toDateTime();

            // Petit dialogue professionnel avec tableau récapitulatif
            QDialog dlg(this);
            dlg.setWindowTitle(tr("Détails demande"));
            dlg.setModal(true);
            dlg.resize(480, 320);

            QVBoxLayout *layout = new QVBoxLayout(&dlg);

            QTableWidget *tw = new QTableWidget(&dlg);
            tw->setColumnCount(2);
            tw->setRowCount(8);
            tw->setHorizontalHeaderLabels(QStringList() << tr("Champ") << tr("Valeur"));
            tw->verticalHeader()->setVisible(false);
            tw->horizontalHeader()->setStretchLastSection(true);
            tw->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft | Qt::AlignVCenter);
            tw->setEditTriggers(QAbstractItemView::NoEditTriggers);
            tw->setSelectionMode(QAbstractItemView::NoSelection);
            tw->setAlternatingRowColors(true);

            // Palette et style pour un rendu plus professionnel
            QPalette palTable = tw->palette();
            palTable.setColor(QPalette::Base, Qt::white);
            palTable.setColor(QPalette::AlternateBase, QColor("#f7f9fc"));
            palTable.setColor(QPalette::Text, QColor("#0a1a2f"));
            palTable.setColor(QPalette::WindowText, QColor("#0a1a2f"));
            tw->setPalette(palTable);
            tw->setStyleSheet(
                "QTableWidget { border: 1px solid #d0d7e2; gridline-color:#e1e5ee; }"
                "QHeaderView::section { background-color:#0d3273; color:white; font-weight:600; padding:4px 8px; border:none; }"
                "QTableWidget::item { padding:3px 6px; }");

            auto setRow = [tw](int row, const QString &champ, const QString &val) {
                QTableWidgetItem *c = new QTableWidgetItem(champ);
                QFont f = c->font();
                f.setBold(true);
                c->setFont(f);
                tw->setItem(row, 0, c);

                QTableWidgetItem *v = new QTableWidgetItem(val);
                tw->setItem(row, 1, v);
            };

            setRow(0, tr("ID demande"), QString::number(idDem));
            setRow(1, tr("ID habitant"), QString::number(idHab));
            setRow(2, tr("Type"), type);
            setRow(3, tr("Statut"), statut);
            setRow(4, tr("Création"), fmtDate(dc));
            setRow(5, tr("Affectation"), fmtDate(da));
            setRow(6, tr("Résolution interne"), fmtDate(dr));
            setRow(7, tr("Résolution finale"), fmtDate(dru));

            tw->resizeColumnsToContents();
            layout->addWidget(tw);

            // Zone de texte pour la description
            QLabel *lblDesc = new QLabel(tr("Description :"), &dlg);
            lblDesc->setStyleSheet("color: #0a1a2f; font-weight: 600;");
            layout->addWidget(lblDesc);

            QTextEdit *editDesc = new QTextEdit(&dlg);
            editDesc->setReadOnly(true);
            editDesc->setText(desc);
            editDesc->setStyleSheet("color: black; background-color: white;");
            editDesc->setMinimumHeight(80);
            layout->addWidget(editDesc);

            // Bouton Fermer
            QPushButton *btnClose = new QPushButton(tr("Fermer"), &dlg);
            btnClose->setCursor(Qt::PointingHandCursor);
            QObject::connect(btnClose, &QPushButton::clicked, &dlg, &QDialog::accept);
            QHBoxLayout *bottomLayout = new QHBoxLayout();
            bottomLayout->addStretch();
            bottomLayout->addWidget(btnClose);
            layout->addLayout(bottomLayout);

            dlg.exec();
        });

        QPushButton *btnAnnuler = new QPushButton(tr("⟲ Annuler affectation"), ui->page_15);
        btnAnnuler->setGeometry(620, 440, 140, 29);
        styliserBoutonPrincipal(btnAnnuler);

        connect(btnAnnuler, &QPushButton::clicked, this, [this]() {
            if (!ui || !ui->tableView_habitants_2)
                return;

            QItemSelectionModel *selDem = ui->tableView_habitants_2->selectionModel();
            if (!selDem || !selDem->hasSelection()) {
                QMessageBox::information(this, tr("Annuler affectation"),
                                         tr("Veuillez sélectionner une demande."));
                return;
            }

            int rowDem = selDem->selectedRows().first().row();
            QAbstractItemModel *modelDem = ui->tableView_habitants_2->model();
            if (!modelDem)
                return;

            bool okIdDem = false;
            int idDemande = modelDem->index(rowDem, 0).data().toInt(&okIdDem);
            if (!okIdDem || idDemande <= 0) {
                QMessageBox::warning(this, tr("Annuler affectation"),
                                     tr("ID de demande invalide."));
                return;
            }

            if (QMessageBox::question(this, tr("Annuler affectation"),
                                      tr("Voulez-vous vraiment annuler l'affectation de la demande %1 ?").arg(idDemande))
                != QMessageBox::Yes)
                return;

            QString error;
            if (!AffectationP::annulerAffectation(idDemande, error)) {
                QMessageBox::critical(this, tr("Annuler affectation"),
                                      tr("Échec de l'annulation : %1").arg(error));
                return;
            }

            chargerTablesAffectation();
            refreshDemandeTable();
        });

        // Bouton Auto-affecter (utilise le personnel recommandé)
        QPushButton *btnAuto = new QPushButton(tr("⭐ Auto-affecter"), ui->page_15);
        btnAuto->setGeometry(760, 440, 130, 29);
        styliserBoutonPrincipal(btnAuto);
        connect(btnAuto, &QPushButton::clicked, this, [this]() {
            if (!ui || !ui->tableView_habitants_2 || !ui->tableView_personel_2)
                return;

            QItemSelectionModel *selDem = ui->tableView_habitants_2->selectionModel();
            if (!selDem || !selDem->hasSelection()) {
                QMessageBox::information(this, tr("Auto-affectation"),
                                         tr("Veuillez d'abord sélectionner une demande."));
                return;
            }

            int row = selDem->selectedRows().first().row();
            QAbstractItemModel *demModel = ui->tableView_habitants_2->model();
            QAbstractItemModel *persModel = ui->tableView_personel_2->model();
            if (!demModel || !persModel)
                return;

            QString typeProb = demModel->index(row, 2).data().toString();
            QString typeLower = typeProb.toLower();

            int bestRow = -1;
            int bestScore = 0;
            for (int r = 0; r < persModel->rowCount(); ++r) {
                QString comp = persModel->index(r, 5).data().toString().toLower();
                QString dispo = persModel->index(r, 4).data().toString().toLower();
                int score = 0;
                if (dispo.contains("disponible")) score += 2;
                if (typeLower.contains("propret") || typeLower.contains("déchet") || typeLower.contains("dechet")) {
                    if (comp.contains("propret") || comp.contains("déchet") || comp.contains("dechet")) score += 3;
                }
                if (typeLower.contains("éclairage") || typeLower.contains("eclairage")) {
                    if (comp.contains("éclairage") || comp.contains("eclairage") || comp.contains("électric")) score += 3;
                }
                if (typeLower.contains("sécurité") || typeLower.contains("securite")) {
                    if (comp.contains("sécurité") || comp.contains("securite")) score += 3;
                }
                if (typeLower.contains("eau") || typeLower.contains("électricité") || typeLower.contains("electricite")) {
                    if (comp.contains("réseau") || comp.contains("reseau") || comp.contains("eau") || comp.contains("électric")) score += 3;
                }
                if (score > bestScore) { bestScore = score; bestRow = r; }
            }

            if (bestRow < 0 || bestScore <= 0) {
                QMessageBox::information(this, tr("Auto-affectation"),
                                         tr("Aucun personnel recommandé trouvé pour ce type de problème."));
                return;
            }

            ui->tableView_personel_2->selectRow(bestRow);
            on_Affecter_clicked();
        });

        // Bouton pour basculer la disponibilité du personnel sélectionné
        QPushButton *btnToggleDispo = new QPushButton(tr("⏱ Bascule dispo"), ui->page_15);
        btnToggleDispo->setGeometry(240, 440, 130, 29);
        styliserBoutonPrincipal(btnToggleDispo);
        connect(btnToggleDispo, &QPushButton::clicked, this, [this]() {
            if (!ui || !ui->tableView_personel_2)
                return;

            QItemSelectionModel *sel = ui->tableView_personel_2->selectionModel();
            if (!sel || !sel->hasSelection()) {
                QMessageBox::information(this, tr("Disponibilité"),
                                         tr("Veuillez sélectionner un personnel."));
                return;
            }

            int row = sel->selectedRows().first().row();
            QAbstractItemModel *m = ui->tableView_personel_2->model();
            if (!m)
                return;

            QString cin = m->index(row, 0).data().toString();
            QString dispo = m->index(row, 4).data().toString();
            if (cin.isEmpty())
                return;

            QString newDispo;
            if (dispo.compare("Disponible", Qt::CaseInsensitive) == 0)
                newDispo = "Occupe";
            else
                newDispo = "Disponible";

            QSqlQuery q;
            q.prepare("UPDATE PERSONNEL SET DISPONIBILITE = :d WHERE CIN = :c");
            q.bindValue(":d", newDispo);
            q.bindValue(":c", cin);
            if (!q.exec()) {
                QMessageBox::warning(this, tr("Disponibilité"),
                                     tr("Erreur SQL : %1").arg(q.lastError().text()));
                return;
            }

            chargerTablesAffectation();
        });

        // Menu contextuel sur la table des demandes (page d'affectation)
        if (ui->tableView_habitants_2) {
            ui->tableView_habitants_2->setContextMenuPolicy(Qt::CustomContextMenu);
            connect(ui->tableView_habitants_2, &QTableView::customContextMenuRequested,
                    this, [this](const QPoint &pos) {
                        if (!ui || !ui->tableView_habitants_2)
                            return;

                        QModelIndex idx = ui->tableView_habitants_2->indexAt(pos);
                        if (!idx.isValid()) {
                            if (!ui->tableView_habitants_2->selectionModel() ||
                                !ui->tableView_habitants_2->selectionModel()->hasSelection())
                                return;
                            idx = ui->tableView_habitants_2->selectionModel()->selectedRows().first();
                        }

                        int row = idx.row();
                        QAbstractItemModel *m = ui->tableView_habitants_2->model();
                        if (!m)
                            return;

                        int idDem = m->index(row, 0).data().toInt();
                        if (idDem <= 0)
                            return;

                        QMenu menu(this);
                        QAction *actDetails = menu.addAction(tr("🔍 Détails demande"));
                        QAction *actAffecter = menu.addAction(tr("⇄ Affecter (personnel sélectionné)"));
                        QAction *actAffecterReco = menu.addAction(tr("⭐ Affecter au personnel recommandé"));
                        QAction *actVoirSuivi = menu.addAction(tr("📊 Voir dans le suivi"));

                        QAction *chosen = menu.exec(ui->tableView_habitants_2->viewport()->mapToGlobal(pos));
                        if (!chosen)
                            return;

                        if (chosen == actDetails) {
                            // Simuler un clic sur le bouton détails
                            QMetaObject::invokeMethod(this, [this]() {
                                // Réutilise la logique du bouton
                                QList<QPushButton*> btns = ui->page_15->findChildren<QPushButton*>(QString(), Qt::FindDirectChildrenOnly);
                                for (QPushButton *b : btns) {
                                    if (b->text().contains("Détails demande")) {
                                        b->click();
                                        break;
                                    }
                                }
                            }, Qt::QueuedConnection);
                        } else if (chosen == actAffecter) {
                            on_Affecter_clicked();
                        } else if (chosen == actAffecterReco) {
                            // Choisir automatiquement le personnel recommandé puis affecter
                            if (!ui->tableView_personel_2 || !ui->tableView_habitants_2)
                                return;
                            QAbstractItemModel *persModel = ui->tableView_personel_2->model();
                            if (!persModel)
                                return;

                            // Reprendre l'algorithme de recommandation simple
                            QString typeProb = m->index(row, 2).data().toString();
                            QString typeLower = typeProb.toLower();
                            int bestRow = -1;
                            int bestScore = 0;
                            for (int r = 0; r < persModel->rowCount(); ++r) {
                                QString comp = persModel->index(r, 5).data().toString().toLower();
                                QString dispo = persModel->index(r, 4).data().toString().toLower();
                                int score = 0;
                                if (dispo.contains("disponible")) score += 2;
                                if (typeLower.contains("propret") || typeLower.contains("déchet") || typeLower.contains("dechet")) {
                                    if (comp.contains("propret") || comp.contains("déchet") || comp.contains("dechet")) score += 3;
                                }
                                if (typeLower.contains("éclairage") || typeLower.contains("eclairage")) {
                                    if (comp.contains("éclairage") || comp.contains("eclairage") || comp.contains("électric")) score += 3;
                                }
                                if (typeLower.contains("sécurité") || typeLower.contains("securite")) {
                                    if (comp.contains("sécurité") || comp.contains("securite")) score += 3;
                                }
                                if (typeLower.contains("eau") || typeLower.contains("électricité") || typeLower.contains("electricite")) {
                                    if (comp.contains("réseau") || comp.contains("reseau") || comp.contains("eau") || comp.contains("électric")) score += 3;
                                }
                                if (score > bestScore) { bestScore = score; bestRow = r; }
                            }
                            if (bestRow >= 0 && bestScore > 0) {
                                ui->tableView_personel_2->selectRow(bestRow);
                                on_Affecter_clicked();
                            } else {
                                QMessageBox::information(this, tr("Affectation"), tr("Aucun personnel recommandé trouvé pour ce type de problème."));
                            }
                        } else if (chosen == actVoirSuivi) {
                            ouvrirDemandeDansSuivi(idDem);
                        }
                    });
        }
    }

    // Boutons supplémentaires sur la page de suivi (page_16)
    if (ui->page_16) {
        // Filtre urgences / tout
        QPushButton *btnSuiviAll = new QPushButton(tr("↺ Tout"), ui->page_16);
        btnSuiviAll->setObjectName("btnSuiviTout");
        btnSuiviAll->setGeometry(700, 40, 100, 29);
        styliserBoutonPrincipal(btnSuiviAll);
        connect(btnSuiviAll, &QPushButton::clicked, this, [this]() {
            remplirTableSuivi();
            filtrerUrgencesSuivi(false);
        });

        QPushButton *btnSuiviUrg = new QPushButton(tr("⚠ Urgences"), ui->page_16);
        btnSuiviUrg->setObjectName("btnSuiviUrgences");
        btnSuiviUrg->setGeometry(810, 40, 120, 29);
        styliserBoutonPrincipal(btnSuiviUrg);
        connect(btnSuiviUrg, &QPushButton::clicked, this, [this]() {
            remplirTableSuivi();
            filtrerUrgencesSuivi(true);
        });

        // Bouton Détails depuis le suivi
        QPushButton *btnDetailsSuivi = new QPushButton(tr("🔍 Détails"), ui->page_16);
        btnDetailsSuivi->setObjectName("pushButton_detailsSuivi");
        btnDetailsSuivi->setGeometry(230, 540, 110, 29);
        styliserBoutonPrincipal(btnDetailsSuivi);
        connect(btnDetailsSuivi, &QPushButton::clicked, this, &MainWindow::on_pushButton_detailsSuivi_clicked);

        // Bouton Export PDF du suivi
        QPushButton *btnExportSuivi = new QPushButton(tr("💾 Export PDF"), ui->page_16);
        btnExportSuivi->setObjectName("pushButton_exportSuivi");
        btnExportSuivi->setGeometry(350, 540, 130, 29);
        styliserBoutonPrincipal(btnExportSuivi);
        connect(btnExportSuivi, &QPushButton::clicked, this, &MainWindow::on_pushButton_exportSuivi_clicked);

        // Légende explicative des couleurs et indicateurs
        QLabel *legend = new QLabel(ui->page_16);
        legend->setGeometry(860, 540, 250, 90);
        legend->setWordWrap(true);
        legend->setStyleSheet(
            "QLabel {"
            "  background-color: #f7f9fc;"
            "  border: 1px solid #d0d7e2;"
            "  border-radius: 8px;"
            "  padding: 6px 8px;"
            "  font-size: 10px;"
            "  color: #0a1a2f;"
            "}");
        legend->setText(tr(
            "Légende :\n"
            "- Rouge / jaune dans Analyse, Affectation, Résolution : délais proches ou dépassés.\n"
            "- Statut rouge : demande nouvelle en retard.\n"
            "- Statut orange : demande en cours.\n"
            "- Statut vert : demande clôturée.\n"
            "- Progression 25/50/75/100 % : étapes DC, DA, DR, DRU atteintes."));
    }
}

// Bouton "affichersatisfaction": on réutilise le même rafraîchissement central
void MainWindow::on_affichersatisfaction_clicked()
{
    int idH = 0;
    if (ui->lineEdit_id_2) idH = ui->lineEdit_id_2->text().trimmed().toInt();
    if (idH <= 0 && ui->IDdhabitant) idH = ui->IDdhabitant->text().trimmed().toInt();
    if (idH <= 0 && ui->lineEdit_id) idH = ui->lineEdit_id->text().trimmed().toInt();

    if (idH <= 0) {
        QMessageBox::warning(this, tr("Satisfaction habitant"),
                             tr("Veuillez saisir un ID d'habitant valide."));
        return;
    }

    double scoreDemandes = -1.0;
    double scoreSondages = -1.0;
    bool hasDemandes = false;
    bool hasSondages = false;
    QString lastComment;
    QDateTime lastCommentDate;

    {
        QSqlQuery q;
        q.prepare("SELECT STATUT, TYPE_PROBLEME FROM DEMANDE WHERE ID_HABITANT = :id");
        q.bindValue(":id", idH);
        if (q.exec()) {
            double totalWeight = 0.0;
            double resolvedWeight = 0.0;
            while (q.next()) {
                QString statut = q.value(0).toString().trimmed().toLower();
                QString type = q.value(1).toString().trimmed().toLower();

                double importance = 1.0;
                QString t = type;
                if (t.contains("sécurité") || t.contains("securite")
                    || t.contains("eau") || t.contains("électricité") || t.contains("electricite")
                    || t.contains("voirie") || t.contains("route") || t.contains("infrastructure")) {
                    importance = 1.4;
                } else if (t.contains("propreté") || t.contains("proprete")
                           || t.contains("déchet") || t.contains("dechet")
                           || t.contains("éclairage") || t.contains("eclairage")) {
                    importance = 1.2;
                } else if (t.contains("réclamation service") || t.contains("reclamation service")) {
                    importance = 1.0;
                } else {
                    importance = 0.8;
                }

                double factor = 0.0;
                if (statut.contains("cloture") || statut.contains("clôt") || statut.contains("clos")) {
                    factor = 1.0;
                } else if (statut.contains("en_cours") || statut.contains(" cours")) {
                    factor = 0.5;
                } else if (statut.contains("nouveau")) {
                    factor = 0.3;
                } else if (statut.contains("echou")) {
                    factor = 0.0;
                }

                totalWeight += importance;
                resolvedWeight += importance * factor;
            }
            if (totalWeight > 0.0) {
                hasDemandes = true;
                double ratio = resolvedWeight / totalWeight;
                if (ratio < 0.0) ratio = 0.0;
                if (ratio > 1.0) ratio = 1.0;
                scoreDemandes = ratio * 100.0;
            }
        }
    }

    {
        QSqlQuery q;
        QStringList sqls;
        sqls << "SELECT AVG(SCORE), COUNT(*) FROM SATISFACTION WHERE ID_HABITANT = :id"
             << "SELECT AVG(SCORE), COUNT(*) FROM SATISFACTION WHERE ID_HABITANT = :id";
        for (const QString &sql : sqls) {
            q.clear();
            q.prepare(sql);
            q.bindValue(":id", idH);
            if (!q.exec())
                continue;
            if (!q.next())
                continue;
            int cnt = q.value(1).toInt();
            double avg = q.value(0).toDouble();
            if (cnt > 0) {
                hasSondages = true;
                scoreSondages = avg;
                break;
            }
        }
    }

    if (hasSondages) {
        QSqlQuery qLast;
        QStringList sqls2;
        sqls2 << "SELECT COMMENTAIRE, DATE_SAISIE FROM SATISFACTION WHERE ID_HABITANT = :id ORDER BY DATE_SAISIE DESC, ID_SATISFACTION DESC"
              << "SELECT COMMENTAIRE, DATE_SAISIE FROM SATISFACTION WHERE ID_HABITANT = :id ORDER BY DATE_SAISIE DESC, ID_SATISFACTION DESC";
        for (const QString &sql2 : sqls2) {
            qLast.clear();
            qLast.prepare(sql2);
            qLast.bindValue(":id", idH);
            if (!qLast.exec())
                continue;
            if (qLast.next()) {
                lastComment = qLast.value(0).toString();
                lastCommentDate = qLast.value(1).toDateTime();
                if (!lastComment.trimmed().isEmpty())
                    break;
            }
        }
    }

    if (!hasDemandes && !hasSondages) {
        QMessageBox::information(this, tr("Satisfaction habitant"),
                                 tr("Aucune demande ou réponse de satisfaction trouvée pour l'habitant %1.")
                                     .arg(idH));
        return;
    }

    double finalScore = 0.0;
    if (hasDemandes && hasSondages) {
        finalScore = (scoreDemandes + scoreSondages) / 2.0;
    } else if (hasDemandes) {
        finalScore = scoreDemandes;
    } else {
        finalScore = scoreSondages;
    }
    if (finalScore < 0.0) finalScore = 0.0;
    if (finalScore > 100.0) finalScore = 100.0;

    // Envoyer les données à l'Arduino pour affichage sur LCD
    envoyerDonneesArduino(idH);

    QChartView *chartView = nullptr;
    if (hasSondages) {
        QLineSeries *series = new QLineSeries();
        QSqlQuery q;
        QStringList sqls;
        sqls << "SELECT DATE_SAISIE, SCORE FROM SATISFACTION WHERE ID_HABITANT = :id ORDER BY DATE_SAISIE"
             << "SELECT DATE_SAISIE, SCORE FROM SATISFACTION WHERE ID_HABITANT = :id ORDER BY DATE_SAISIE";
        bool filled = false;
        for (const QString &sql : sqls) {
            q.clear();
            q.prepare(sql);
            q.bindValue(":id", idH);
            if (!q.exec())
                continue;
            int index = 0;
            while (q.next()) {
                double s = q.value(1).toDouble();
                series->append(index, s);
                ++index;
            }
            if (series->count() > 0) {
                filled = true;
                break;
            }
        }
        if (filled) {
            QChart *chart = new QChart();
            chart->addSeries(series);
            chart->setTitle(tr("Évolution de la satisfaction"));
            chart->createDefaultAxes();
            QList<QAbstractAxis*> axesY = chart->axes(Qt::Vertical);
            if (!axesY.isEmpty()) {
                if (auto axisY = qobject_cast<QValueAxis*>(axesY.first())) {
                    axisY->setRange(0, 100);
                }
            }
            chartView = new QChartView(chart);
            chartView->setRenderHint(QPainter::Antialiasing);
        } else {
            delete series;
        }
    }

    QDialog *dlg = new QDialog(this);
    dlg->setWindowTitle(tr("Satisfaction de l'habitant %1").arg(idH));
    QVBoxLayout *layout = new QVBoxLayout(dlg);

    QProgressBar *bar = new QProgressBar(dlg);
    bar->setRange(0, 100);
    bar->setValue(static_cast<int>(finalScore + 0.5));
    QString color;
    if (finalScore >= 80.0) {
        color = "#27ae60";
    } else if (finalScore >= 60.0) {
        color = "#f39c12";
    } else {
        color = "#e74c3c";
    }
    bar->setFormat(tr("Score global de satisfaction : %1%").arg(finalScore, 0, 'f', 1));
    bar->setStyleSheet(QString(
                           "QProgressBar { border: 1px solid #cfd9e6; border-radius: 8px;"
                           " background: #ecf0f1; color: #0a1a2f; text-align: center; }"
                           "QProgressBar::chunk { background-color: %1; border-radius: 8px; }"
                           ).arg(color));
    layout->addWidget(bar);

    QLabel *infoLabel = new QLabel(dlg);
    QString info;
    if (hasDemandes && hasSondages) {
        info = tr("Score basé sur les demandes (≈ %1%) et les réponses de sondage (≈ %2%).")
                   .arg(scoreDemandes, 0, 'f', 1)
                   .arg(scoreSondages, 0, 'f', 1);
    } else if (hasDemandes) {
        info = tr("Score basé sur les demandes de l'habitant (≈ %1%).")
                   .arg(scoreDemandes, 0, 'f', 1);
    } else {
        info = tr("Score basé sur les réponses de sondage (≈ %1%).")
                   .arg(scoreSondages, 0, 'f', 1);
    }
    infoLabel->setText(info);
    infoLabel->setStyleSheet("color: #111111; font-size: 11pt;");
    layout->addWidget(infoLabel);

    if (!lastComment.trimmed().isEmpty()) {
        QLabel *commentLabel = new QLabel(dlg);
        commentLabel->setWordWrap(true);
        QString dateStr = lastCommentDate.isValid()
                              ? lastCommentDate.toString("dd/MM/yyyy HH:mm")
                              : tr("date inconnue");
        commentLabel->setText(tr("Dernier commentaire (%1) : %2").arg(dateStr, lastComment));
        commentLabel->setStyleSheet("color: #111111; font-size: 10pt;");
        layout->addWidget(commentLabel);
    }

    if (chartView) {
        layout->addWidget(chartView);
    }

    QPushButton *btnDemandes = new QPushButton(tr("Voir les demandes de cet habitant"), dlg);
    QPushButton *btnQr = new QPushButton(tr("QR code de cette satisfaction"), dlg);

    QHBoxLayout *bottomButtons = new QHBoxLayout();
    bottomButtons->addWidget(btnQr);
    bottomButtons->addStretch();
    bottomButtons->addWidget(btnDemandes);
    layout->addLayout(bottomButtons);

    connect(btnDemandes, &QPushButton::clicked, this, [this, idH]() {
        QSqlQuery q;
        q.prepare("SELECT ID_DEMANDE, TYPE_PROBLEME, DESCRIPTION, STATUT, DATE_CREATION FROM DEMANDE WHERE ID_HABITANT = :id ORDER BY DATE_CREATION DESC");
        q.bindValue(":id", idH);
        if (!q.exec()) {
            QMessageBox::warning(this, tr("Demandes"),
                                 tr("Impossible de charger les demandes : %1").arg(q.lastError().text()));
            return;
        }

        QSqlQueryModel *model = new QSqlQueryModel(this);
        model->setQuery(std::move(q));
        model->setHeaderData(0, Qt::Horizontal, tr("ID"));
        model->setHeaderData(1, Qt::Horizontal, tr("Type de problème"));
        model->setHeaderData(2, Qt::Horizontal, tr("Description"));
        model->setHeaderData(3, Qt::Horizontal, tr("Statut"));
        model->setHeaderData(4, Qt::Horizontal, tr("Date de création"));

        QDialog *dlgDem = new QDialog(this);
        dlgDem->setWindowTitle(tr("Demandes de l'habitant %1").arg(idH));
        QVBoxLayout *lay = new QVBoxLayout(dlgDem);
        QTableView *tv = new QTableView(dlgDem);
        tv->setModel(model);
        tv->setSelectionBehavior(QAbstractItemView::SelectRows);
        tv->setSelectionMode(QAbstractItemView::SingleSelection);
        tv->setAlternatingRowColors(true);
        tv->horizontalHeader()->setStretchLastSection(true);
        tv->setStyleSheet(
            "QTableView {"
            "  background-color: #ffffff;"
            "  color: #111111;"
            "  gridline-color: #d0d7e2;"
            "  selection-background-color: #e6f0ff;"
            "  selection-color: #111111;"
            "}"
            "QHeaderView::section {"
            "  background-color: #f4f7fb;"
            "  color: #0a1a2f;"
            "  font-weight: 600;"
            "  border: 1px solid #d0d7e2;"
            "}"
            );
        tv->resizeColumnsToContents();
        lay->addWidget(tv);
        dlgDem->resize(800, 400);
        dlgDem->exec();
    });

    connect(btnQr, &QPushButton::clicked, this, [this, idH, finalScore]() {
        QString payload = tr("Habitant %1 - Score global de satisfaction : %2%")
        .arg(idH)
            .arg(finalScore, 0, 'f', 1);
        QUrl url(QStringLiteral("https://api.qrserver.com/v1/create-qr-code/"));
        QUrlQuery query;
        query.addQueryItem(QStringLiteral("size"), QStringLiteral("260x260"));
        query.addQueryItem(QStringLiteral("data"), payload);
        url.setQuery(query);

        QDialog *dlgQr = new QDialog(this);
        dlgQr->setWindowTitle(tr("QR code - Habitant %1").arg(idH));
        QVBoxLayout *lay = new QVBoxLayout(dlgQr);
        QLabel *info = new QLabel(tr("Scannez ce QR code avec votre téléphone."), dlgQr);
        QLabel *img = new QLabel(dlgQr);
        img->setAlignment(Qt::AlignCenter);
        lay->addWidget(info);
        lay->addWidget(img, 1);

        QNetworkAccessManager *nam = new QNetworkAccessManager(dlgQr);
        QObject::connect(nam, &QNetworkAccessManager::finished, dlgQr,
                         [dlgQr, img](QNetworkReply *reply) {
                             reply->deleteLater();
                             if (reply->error() != QNetworkReply::NoError) {
                                 img->setText(QObject::tr("Erreur de chargement du QR code : %1")
                                                  .arg(reply->errorString()));
                                 return;
                             }
                             QPixmap pix;
                             if (!pix.loadFromData(reply->readAll())) {
                                 img->setText(QObject::tr("Réponse invalide du service QR."));
                                 return;
                             }
                             img->setPixmap(pix.scaled(260, 260, Qt::KeepAspectRatio, Qt::SmoothTransformation));
                         });

        QNetworkRequest req(url);
        req.setHeader(QNetworkRequest::UserAgentHeader, QStringLiteral("QtQRClient"));
        nam->get(req);

        dlgQr->resize(320, 360);
        dlgQr->exec();
    });

    dlg->resize(650, 450);
    dlg->exec();
}

MainWindow::~MainWindow()
{
    // Fermer la connexion Arduino
    if (arduino) {
        arduino->close_arduino();
        delete arduino;
        arduino = nullptr;
    }
    delete ui;
}

// === Helpers Habitant ===
void MainWindow::chargerDonneesTable()
{
    HabitantCRUD h;
    QSqlQueryModel *model = h.afficher(this);
    ui->tableView_habitants->setModel(model);
    ui->tableView_habitants->resizeColumnsToContents();
}

void MainWindow::refreshDemandeTable()
{
    if (!ui || !ui->tabdemande)
        return;

    QSqlQuery query;
    QString base = "SELECT ID_DEMANDE, ID_HABITANT, TYPE_PROBLEME, DESCRIPTION, STATUT, DATE_CREATION FROM DEMANDE";
    if (!demandeStatusFilter.isEmpty()) {
        base += " WHERE STATUT = :statut";
    }
    base += " ORDER BY DATE_CREATION DESC";
    query.prepare(base);
    if (!demandeStatusFilter.isEmpty()) {
        query.bindValue(":statut", demandeStatusFilter);
    }
    if (!query.exec()) {
        QMessageBox::warning(this, tr("Demandes"),
                             tr("Impossible de charger les demandes : %1")
                                 .arg(query.lastError().text()));
        return;
    }

    QStandardItemModel *model = new QStandardItemModel(this);
    model->setColumnCount(9);
    model->setHeaderData(0, Qt::Horizontal, tr("#"));
    model->setHeaderData(1, Qt::Horizontal, tr("ID habitant"));
    model->setHeaderData(2, Qt::Horizontal, tr("Type de problème"));
    model->setHeaderData(3, Qt::Horizontal, tr("Description"));
    model->setHeaderData(4, Qt::Horizontal, tr("Spécialité conseillée"));
    model->setHeaderData(5, Qt::Horizontal, tr("Statut"));
    model->setHeaderData(6, Qt::Horizontal, tr("Date de création"));
    model->setHeaderData(7, Qt::Horizontal, tr("Délai"));
    model->setHeaderData(8, Qt::Horizontal, tr("Priorité"));

    int row = 0;
    while (query.next()) {
        const int idDemande    = query.value(0).toInt();
        const int idHabitant   = query.value(1).toInt();
        const QString typeProb = query.value(2).toString();
        const QString desc     = query.value(3).toString();
        QString statut         = query.value(4).toString();
        QDateTime dtCreation   = query.value(5).toDateTime();
        QString dateCreate     = dtCreation.toString("yyyy-MM-dd HH:mm");

        // Déterminer le statut affiché, la spécialité et la couleur
        QString statutAffiche = statut;
        QColor bg("#ecf0f1"); // gris clair par défaut
        QString s = statut.trimmed().toLower();
        const QDateTime now = QDateTime::currentDateTime();
        double heures = dtCreation.secsTo(now) / 3600.0;

        if (s == "nouveau") {
            if (heures >= DEMANDE_DELAI_HEURES) {
                // Mettre à jour la BD pour marquer la demande comme échouée
                QSqlQuery qUpdate;
                qUpdate.prepare("UPDATE DEMANDE SET STATUT='Echoue' WHERE ID_DEMANDE = :id");
                qUpdate.bindValue(":id", idDemande);
                qUpdate.exec();

                statutAffiche = "Echoue";
                bg = QColor("#8e0000");      // rouge foncé
            } else {
                bg = QColor("#e74c3c");      // rouge vif
            }
        } else if (s == "en_cours" || s.contains("cours")) {
            bg = QColor("#f39c12");         // orange
        } else if (s == "cloture" || s.contains("clôt") || s.contains("clos")) {
            bg = QColor("#27ae60");         // vert
        }

        // Spécialité conseillée selon le type de problème
        QString typeLower = typeProb.trimmed().toLower();
        QString specialite;
        if (typeLower.contains("propreté") || typeLower.contains("déchet"))
            specialite = tr("Agent propreté");
        else if (typeLower.contains("éclairage"))
            specialite = tr("Électricien public");
        else if (typeLower.contains("sécurité"))
            specialite = tr("Agent de sécurité");
        else if (typeLower.contains("eau") || typeLower.contains("électricité"))
            specialite = tr("Technicien réseaux");
        else if (typeLower.contains("voirie") || typeLower.contains("route") || typeLower.contains("infrastructures"))
            specialite = tr("Service voirie");
        else if (typeLower.contains("réclamation service"))
            specialite = tr("Accueil usagers");
        else
            specialite = tr("Agent municipal");

        // Délai restant / retard par rapport au seuil
        QString delaiText;
        double delta = DEMANDE_DELAI_HEURES - heures;
        int minutesTotal = static_cast<int>(std::round(std::fabs(delta) * 60.0));
        int hPart = minutesTotal / 60;
        int mPart = minutesTotal % 60;
        QString mStr = QString::number(mPart).rightJustified(2, '0');

        bool dejaEchoue = (statutAffiche.trimmed().compare("Echoue", Qt::CaseInsensitive) == 0);
        if (dejaEchoue || heures >= DEMANDE_DELAI_HEURES) {
            delaiText = tr("En retard de %1h%2").arg(hPart).arg(mStr);
        } else {
            delaiText = tr("Reste %1h%2").arg(hPart).arg(mStr);
        }

        model->setItem(row, 0, new QStandardItem(QString::number(idDemande)));
        model->setItem(row, 1, new QStandardItem(QString::number(idHabitant)));
        model->setItem(row, 2, new QStandardItem(typeProb));
        model->setItem(row, 3, new QStandardItem(desc));

        QStandardItem *specItem = new QStandardItem(specialite);
        specItem->setTextAlignment(Qt::AlignCenter);
        model->setItem(row, 4, specItem);

        QStandardItem *statutItem = new QStandardItem(statutAffiche);
        statutItem->setTextAlignment(Qt::AlignCenter);
        model->setItem(row, 5, statutItem);

        QStandardItem *dateItem = new QStandardItem(dateCreate);
        dateItem->setTextAlignment(Qt::AlignCenter);
        model->setItem(row, 6, dateItem);

        QStandardItem *delaiItem = new QStandardItem(delaiText);
        delaiItem->setTextAlignment(Qt::AlignCenter);
        model->setItem(row, 7, delaiItem);

        // Colonne de priorité: seulement un bloc de couleur centré, sans texte
        QStandardItem *colorItem = new QStandardItem();
        colorItem->setEditable(false);
        colorItem->setData(QVariant(), Qt::DisplayRole);
        colorItem->setBackground(bg);
        colorItem->setTextAlignment(Qt::AlignCenter);
        model->setItem(row, 8, colorItem);

        ++row;
    }

    ui->tabdemande->setModel(model);
    ui->tabdemande->resizeColumnsToContents();
}

void MainWindow::chargerTablesAffectation(bool urgences, bool onlyDisponibles)
{
    if (!ui)
        return;

    // Modèle personnalisé pour afficher aussi la charge de travail (nombre de demandes en cours)
    QSqlQueryModel *modelPers = new QSqlQueryModel(this);
    QString basePers =
        "SELECT p.CIN, p.NOM, p.PRENOM, p.ZONE_AFFECTATION, p.DISPONIBILITE, p.COMPETENCES, "
        "       (SELECT COUNT(*) FROM DEMANDE d WHERE d.ID_PERSONNEL_AFFECTE = p.CIN AND d.STATUT = 'En_cours') AS CHARGES "
        "FROM PERSONNEL p";
    if (onlyDisponibles)
        basePers += " WHERE p.DISPONIBILITE = 'Disponible'";
    basePers += " ORDER BY p.CIN";
    modelPers->setQuery(basePers);
    modelPers->setHeaderData(6, Qt::Horizontal, tr("Charges"));

    ui->tableView_personel_2->setModel(modelPers);
    ui->tableView_personel_2->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView_personel_2->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView_personel_2->resizeColumnsToContents();

    QSqlQueryModel *modelDem = new QSqlQueryModel(this);
    QString baseDem =
        "SELECT ID_DEMANDE, ID_HABITANT, TYPE_PROBLEME, STATUT, "
        "DATE_CREATION, ID_PERSONNEL_AFFECTE, DATE_AFFECTATION "
        "FROM DEMANDE";
    if (urgences) {
        // Urgences: statuts en cours ou types critiques
        baseDem += " WHERE STATUT = 'En_cours' OR LOWER(TYPE_PROBLEME) LIKE '%sécurité%' "
                   "OR LOWER(TYPE_PROBLEME) LIKE '%securite%' "
                   "OR LOWER(TYPE_PROBLEME) LIKE '%eau%' "
                   "OR LOWER(TYPE_PROBLEME) LIKE '%électricité%' "
                   "OR LOWER(TYPE_PROBLEME) LIKE '%electricite%' "
                   "OR LOWER(TYPE_PROBLEME) LIKE '%éclairage%' "
                   "OR LOWER(TYPE_PROBLEME) LIKE '%eclairage%'";
    }
    baseDem += " ORDER BY DATE_CREATION DESC";
    modelDem->setQuery(baseDem);

    ui->tableView_habitants_2->setModel(modelDem);
    ui->tableView_habitants_2->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView_habitants_2->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView_habitants_2->resizeColumnsToContents();

    // Mettre à jour le résumé dynamique et écouter les changements de sélection
    if (ui->tableView_personel_2 && ui->tableView_personel_2->selectionModel()) {
        connect(ui->tableView_personel_2->selectionModel(), &QItemSelectionModel::selectionChanged,
                this, [this](const QItemSelection &, const QItemSelection &) { updateAffectationSummary(); });
    }
    if (ui->tableView_habitants_2 && ui->tableView_habitants_2->selectionModel()) {
        connect(ui->tableView_habitants_2->selectionModel(), &QItemSelectionModel::selectionChanged,
                this, [this](const QItemSelection &, const QItemSelection &) { updateAffectationSummary(); });
    }

    updateAffectationSummary();
}

void MainWindow::remplirTableSuivi()
{
    if (!ui || !ui->tableWidget_2)
        return;

    QTableWidget *tw = ui->tableWidget_2;
    tw->clearContents();
    tw->setRowCount(0);
    tw->setColumnCount(8);
    tw->setHorizontalHeaderLabels(
        QStringList() << tr("ID")
                      << tr("Type problème")
                      << tr("Personnel")
                      << tr("Statut")
                      << tr("Analyse (j h m s)")
                      << tr("Affectation (j h m s)")
                      << tr("Résolution (j h m s)")
                      << tr("Progression"));

    QSqlQuery q;
    q.prepare(
        "SELECT d.ID_DEMANDE, d.DATE_CREATION, d.DATE_AFFECTATION, "
        "       d.DATE_RESOLUTION_INTERNE, d.DATE_RESOLUTION_FINALE, "
        "       d.TYPE_PROBLEME, d.STATUT, d.ID_PERSONNEL_AFFECTE, "
        "       p.NOM, p.PRENOM "
        "FROM DEMANDE d "
        "LEFT JOIN PERSONNEL p ON p.CIN = d.ID_PERSONNEL_AFFECTE "
        "ORDER BY d.ID_DEMANDE");
    if (!q.exec()) {
        QMessageBox::warning(this, tr("Suivi"),
                             tr("Impossible de charger les demandes : %1")
                                 .arg(q.lastError().text()));
        return;
    }

    QDateTime now = QDateTime::currentDateTime();

    auto daysBetween = [](const QDateTime &a, const QDateTime &b) -> QString {
        if (!a.isValid() || !b.isValid())
            return "-";
        int d = a.daysTo(b);
        if (d < 0)
            d = 0;
        return QString::number(d);
    };

    auto durationJHMS = [](const QDateTime &a, const QDateTime &b) -> QString {
        if (!a.isValid() || !b.isValid())
            return "-";
        qint64 secs = a.secsTo(b);
        if (secs < 0)
            secs = 0;
        qint64 days = secs / 86400; // 24*60*60
        secs %= 86400;
        qint64 hours = secs / 3600;
        secs %= 3600;
        qint64 minutes = secs / 60;
        qint64 seconds = secs % 60;
        return QString("%1j %2h %3m %4s").arg(days).arg(hours).arg(minutes).arg(seconds);
    };

    auto colorDuration = [](QTableWidgetItem *item, const QString &val, int seuilJour) {
        if (!item)
            return;
        bool ok = false;
        int v = val.toInt(&ok);
        if (!ok)
            return;
        if (v > seuilJour * 2) {
            item->setBackground(QColor("#ffcccc"));   // rouge clair
        } else if (v > seuilJour) {
            item->setBackground(QColor("#ffeeba"));   // jaune clair
        }
    };

    auto formatDate = [](const QDateTime &dt) -> QString {
        return dt.isValid() ? dt.toString("dd/MM/yyyy HH:mm") : QString("-");
    };

    // Statistiques globales pour le dashboard
    int total = 0;
    int nbNouveau = 0;
    int nbEnCours = 0;
    int nbCloture = 0;
    int nbEchoue = 0;
    int nbRetard = 0;

    double sumAnalyse = 0.0; int countAnalyse = 0;
    double sumAffect  = 0.0; int countAffect  = 0;
    double sumResol   = 0.0; int countResol   = 0;

    while (q.next()) {
        int row = tw->rowCount();
        tw->insertRow(row);

        int idDem = q.value(0).toInt();
        QDateTime dc = q.value(1).toDateTime();
        QDateTime da = q.value(2).toDateTime();
        QDateTime dr = q.value(3).toDateTime();
        QDateTime dru = q.value(4).toDateTime();

        QString typeProb = q.value(5).toString();
        QString statut = q.value(6).toString();
        QString idPers = q.value(7).toString().trimmed();
        QString nomPers = q.value(8).toString().trimmed();
        QString prenPers = q.value(9).toString().trimmed();

        QString personnelAffiche;
        if (!nomPers.isEmpty() || !prenPers.isEmpty())
            personnelAffiche = nomPers + " " + prenPers;
        else if (!idPers.isEmpty())
            personnelAffiche = idPers;
        else
            personnelAffiche = tr("Non affecté");

        // Phase courante (Analyse, Affectation, Résolution) en fonction des dates
        QString phase;
        if (!da.isValid() && !dr.isValid()) {
            phase = tr("Analyse");
        } else if (da.isValid() && !dr.isValid()) {
            phase = tr("Affectation");
        } else if (dr.isValid()) {
            phase = tr("Résolution");
        } else {
            phase = statut;
        }

        // Analyse : de la création jusqu'à l'affectation (ou maintenant si pas encore affectée)
        QDateTime finAnalyse = da.isValid() ? da : now;
        QString analyseJours = daysBetween(dc, finAnalyse);           // utilisé pour stats / couleurs
        QString analyseAffiche = durationJHMS(dc, finAnalyse);        // affichage j/h/m/s

        // Affectation : de la date d'affectation jusqu'à la résolution interne (ou maintenant si pas encore résolue)
        QString affectJours = "X";
        QString affectAffiche = "X";
        if (da.isValid()) {
            QDateTime finAffect = dr.isValid() ? dr : now;
            affectJours = daysBetween(da, finAffect);
            affectAffiche = durationJHMS(da, finAffect);
        }

        // Résolution : de la résolution interne jusqu'à la résolution finale (ou maintenant si en cours)
        QString resolJours = "X";
        QString resolAffiche = "X";
        if (dr.isValid()) {
            QDateTime finResol = dru.isValid() ? dru : now;
            resolJours = daysBetween(dr, finResol);
            resolAffiche = durationJHMS(dr, finResol);
        }

        // Items de base
        QTableWidgetItem *idItem = new QTableWidgetItem(QString::number(idDem));
        idItem->setTextAlignment(Qt::AlignCenter);
        QTableWidgetItem *typeItem = new QTableWidgetItem(typeProb);
        QTableWidgetItem *persItem = new QTableWidgetItem(personnelAffiche);
        QTableWidgetItem *statutItem = new QTableWidgetItem(phase);
        QTableWidgetItem *analyseItem = new QTableWidgetItem(analyseAffiche);
        QTableWidgetItem *affectItem = new QTableWidgetItem(affectAffiche);
        QTableWidgetItem *resolItem  = new QTableWidgetItem(resolAffiche);

        tw->setItem(row, 0, idItem);
        tw->setItem(row, 1, typeItem);
        tw->setItem(row, 2, persItem);
        tw->setItem(row, 3, statutItem);
        tw->setItem(row, 4, analyseItem);
        tw->setItem(row, 5, affectItem);
        tw->setItem(row, 6, resolItem);

        // Compteurs globaux
        ++total;
        QString s = statut.trimmed().toLower();
        if (s == "nouveau") ++nbNouveau;
        else if (s == "en_cours" || s.contains("cours")) ++nbEnCours;
        else if (s == "cloture" || s.contains("clôt") || s.contains("clos")) ++nbCloture;
        else if (s.contains("echou")) ++nbEchoue;

        // Couleurs selon statut
        QColor bg("#ecf0f1");
        if (s == "nouveau") {
            bg = QColor("#e74c3c");
        } else if (s == "en_cours" || s.contains("cours")) {
            bg = QColor("#f39c12");
        } else if (s == "cloture" || s.contains("clôt") || s.contains("clos")) {
            bg = QColor("#27ae60");
        }
        statutItem->setBackground(bg);
        statutItem->setForeground(QColor("#ffffff"));

        // Mise en évidence des durées longues et stats de durée
        bool retard = false;
        if (analyseJours != "-" && analyseJours != "X") {
            bool okDur = false; int v = analyseJours.toInt(&okDur);
            if (okDur) { sumAnalyse += v; ++countAnalyse; if (v > 2) retard = true; }
        }
        if (affectJours != "-" && affectJours != "X") {
            bool okDur = false; int v = affectJours.toInt(&okDur);
            if (okDur) { sumAffect += v; ++countAffect; if (v > 2) retard = true; }
        }
        if (resolJours != "-" && resolJours != "X") {
            bool okDur = false; int v = resolJours.toInt(&okDur);
            if (okDur) { sumResol += v; ++countResol; if (v > 5) retard = true; }
        }
        if (retard) ++nbRetard;

        colorDuration(analyseItem, analyseJours, 2);   // seuil 2 jours analyse
        colorDuration(affectItem, affectJours, 2);     // seuil 2 jours affectation
        colorDuration(resolItem,  resolJours, 5);      // seuil 5 jours résolution

        // Barre de progression en fonction des étapes atteintes
        int steps = 0;
        if (dc.isValid()) steps = 1;
        if (da.isValid()) steps = 2;
        if (dr.isValid()) steps = 3;
        if (dru.isValid()) steps = 4;
        int progress = steps * 25; // 25%, 50%, 75%, 100%

        QProgressBar *bar = new QProgressBar(tw);
        bar->setRange(0, 100);
        bar->setValue(progress);
        bar->setTextVisible(true);
        bar->setFormat(QString::number(progress) + "%");
        bar->setStyleSheet(
            "QProgressBar {"
            "  border: 1px solid #d0d7e2;"
            "  border-radius: 6px;"
            "  background: #f5f7fb;"
            "  text-align: center;"
            "  font-size: 11px;"
            "}"
            "QProgressBar::chunk {"
            "  background-color: #27ae60;"
            "  border-radius: 6px;"
            "}"
            );
        tw->setCellWidget(row, 7, bar);

        // Tooltip récapitulatif sur la ligne
        QString tooltip = tr("Création : %1\nAffectation : %2\nRésolution interne : %3\nRésolution finale : %4")
                              .arg(formatDate(dc))
                              .arg(formatDate(da))
                              .arg(formatDate(dr))
                              .arg(formatDate(dru));

        idItem->setToolTip(tooltip);
        typeItem->setToolTip(tooltip);
        persItem->setToolTip(tooltip);
        statutItem->setToolTip(tooltip);
        analyseItem->setToolTip(tooltip);
        affectItem->setToolTip(tooltip);
        resolItem->setToolTip(tooltip);
        bar->setToolTip(tooltip);
    }

    tw->resizeColumnsToContents();

    // Mettre à jour le dashboard global dans tabdemande_2
    if (!ui->tabdemande_2)
        return;

    QStandardItemModel *dash = new QStandardItemModel(this);
    dash->setColumnCount(3);
    dash->setHeaderData(0, Qt::Horizontal, tr("Indicateur"));
    dash->setHeaderData(1, Qt::Horizontal, tr("Valeur"));
    dash->setHeaderData(2, Qt::Horizontal, tr("Commentaire"));

    auto addRow = [dash](int row, const QString &ind, const QString &val, const QString &com) {
        dash->setItem(row, 0, new QStandardItem(ind));
        dash->setItem(row, 1, new QStandardItem(val));
        dash->setItem(row, 2, new QStandardItem(com));
    };

    int rowDash = 0;
    addRow(rowDash++, tr("Total demandes"), QString::number(total),
           tr("Nouvelles: %1, En cours: %2, Clôturées: %3, Échouées: %4")
               .arg(nbNouveau).arg(nbEnCours).arg(nbCloture).arg(nbEchoue));

    addRow(rowDash++, tr("Demandes en retard"), QString::number(nbRetard),
           tr("Au moins une phase dépasse son délai cible"));

    auto avgOrDash = [](double sum, int count) {
        if (count <= 0) return QString("-");
        double v = sum / double(count);
        return QString::number(v, 'f', 1);
    };

    addRow(rowDash++, tr("Analyse moyenne (j)"), avgOrDash(sumAnalyse, countAnalyse),
           tr("Basée sur %1 demandes analysées").arg(countAnalyse));
    addRow(rowDash++, tr("Affectation moyenne (j)"), avgOrDash(sumAffect, countAffect),
           tr("Basée sur %1 demandes affectées").arg(countAffect));
    addRow(rowDash++, tr("Résolution moyenne (j)"), avgOrDash(sumResol, countResol),
           tr("Basée sur %1 demandes résolues").arg(countResol));

    ui->tabdemande_2->setModel(dash);
    ui->tabdemande_2->resizeColumnsToContents();
}

void MainWindow::filtrerUrgencesSuivi(bool urgencesSeulement)
{
    if (!ui || !ui->tableWidget_2)
        return;

    QTableWidget *tw = ui->tableWidget_2;
    int rows = tw->rowCount();
    for (int r = 0; r < rows; ++r) {
        bool urgent = false;

        // Colonnes 4,5,6 : Analyse, Affectation, Résolution (jours)
        for (int c = 4; c <= 6; ++c) {
            QTableWidgetItem *it = tw->item(r, c);
            if (!it)
                continue;
            bool ok = false;
            int v = it->text().toInt(&ok);
            if (!ok)
                continue;

            if (c == 4 || c == 5) {
                if (v > 2) { urgent = true; break; }
            } else if (c == 6) {
                if (v > 5) { urgent = true; break; }
            }
        }

        if (urgencesSeulement)
            tw->setRowHidden(r, !urgent);
        else
            tw->setRowHidden(r, false);
    }
}

void MainWindow::chargerStatutsAutorises()
{
    allowedStatuses.clear();
    {
        QSqlQuery q;
        q.prepare("SELECT search_condition FROM user_constraints WHERE table_name = 'HABITANT' AND constraint_name = 'CK_STATUT_HABITANT'");
        if (q.exec() && q.next()) {
            const QString condition = q.value(0).toString();
            QRegularExpression re("'([^']+)'", QRegularExpression::CaseInsensitiveOption);
            QRegularExpressionMatchIterator it = re.globalMatch(condition);
            while (it.hasNext()) {
                const auto m = it.next();
                const QString v = m.captured(1).trimmed();
                if (!v.isEmpty()) allowedStatuses << v;
            }
        }
    }
    if (allowedStatuses.isEmpty()) {
        QSqlQuery q2("SELECT DISTINCT STATUT FROM HABITANT WHERE STATUT IS NOT NULL");
        while (q2.next()) {
            const QString v = q2.value(0).toString().trimmed();
            if (!v.isEmpty()) allowedStatuses << v;
        }
    }

    // Fallback: si rien n'a été trouvé en base, proposer une liste par défaut
    if (allowedStatuses.isEmpty()) {
        allowedStatuses << "Actif" << "Inactif" << "En attente";
    }

    allowedStatuses.removeDuplicates();
    if (!allowedStatuses.isEmpty()) {
        QCompleter *comp = new QCompleter(allowedStatuses, this);
        comp->setCaseSensitivity(Qt::CaseInsensitive);
        comp->setFilterMode(Qt::MatchContains);
        QListView *popup = new QListView();
        popup->setStyleSheet("QListView { background: #ffffff; color: #111111; }\nQListView::item:selected { background: #e6f0ff; color: #111111; }");
        comp->setPopup(popup);
        ui->lineEdit_statut->setCompleter(comp);
        const QString hint = QString("Valeurs autorisées: %1").arg(allowedStatuses.join(", "));
        ui->lineEdit_statut->setPlaceholderText(hint);
        ui->lineEdit_statut->setToolTip(hint);
    }
}

bool MainWindow::statutAutorise(const QString &value) const
{
    // Si aucune liste n'a pu être chargée depuis la BD, on autorise tout
    if (allowedStatuses.isEmpty())
        return true;

    const QString trimmed = value.trimmed();
    for (const QString &v : allowedStatuses) {
        if (v.compare(trimmed, Qt::CaseInsensitive) == 0)
            return true;
    }
    return false;
}

// === Création d'une Demande (Réclamation) ===
void MainWindow::on_DemandeH_clicked()
{
    // Widgets attendus: IDdhabitant (QLineEdit), reclamation (QLineEdit ou QTextEdit), problemeH (QComboBox)
    int idH = 0;
    if (ui->IDdhabitant) idH = ui->IDdhabitant->text().trimmed().toInt();

    QString description;
    if (ui->reclamation) {
        // Essayer comme QLineEdit
        if (auto le = qobject_cast<QLineEdit*>(ui->reclamation)) description = le->text().trimmed();
        // Essayer comme QTextEdit (si dans l'UI c'est un QTextEdit)
        else if (auto te = qobject_cast<QTextEdit*>(ui->reclamation)) description = te->toPlainText().trimmed();
    }

    QString typeProbleme = ui->problemeH ? ui->problemeH->currentText().trimmed() : QString();

    QString err;
    DemandeCRUD d(idH, typeProbleme, description);
    if (!d.ajouter(err)) {
        QMessageBox::warning(this, "Demande", err.isEmpty()? QString("Échec de création de la demande") : err);
        return;
    }

    QMessageBox::information(this, "Demande", "Votre demande a été enregistrée avec succès.");
    // Rafraîchir le tableau des demandes si la vue existe
    refreshDemandeTable();

    if (ui->reclamation) {
        if (auto le = qobject_cast<QLineEdit*>(ui->reclamation)) le->clear();
        else if (auto te = qobject_cast<QTextEdit*>(ui->reclamation)) te->clear();
    }
}

void MainWindow::on_suppdemande_clicked()
{
    if (!ui || !ui->tabdemande) {
        QMessageBox::warning(this, tr("Demandes"), tr("Table des demandes introuvable."));
        return;
    }

    QItemSelectionModel *sel = ui->tabdemande->selectionModel();
    if (!sel || !sel->hasSelection()) {
        QMessageBox::information(this, tr("Demandes"), tr("Veuillez sélectionner une demande à supprimer."));
        return;
    }

    int row = sel->selectedRows().first().row();
    QAbstractItemModel *model = ui->tabdemande->model();
    if (!model) return;

    // La première colonne (#) contient l'ID_DEMANDE
    bool ok = false;
    int idDemande = model->index(row, 0).data().toInt(&ok);
    if (!ok || idDemande <= 0) {
        QMessageBox::warning(this, tr("Demandes"), tr("Impossible de déterminer l'ID de la demande sélectionnée."));
        return;
    }

    if (QMessageBox::question(this, tr("Demandes"),
                              tr("Supprimer la demande %1 ?").arg(idDemande),
                              QMessageBox::Yes | QMessageBox::No) != QMessageBox::Yes) {
        return;
    }

    QSqlQuery q;
    q.prepare("DELETE FROM DEMANDE WHERE ID_DEMANDE = :id");
    q.bindValue(":id", idDemande);
    if (!q.exec()) {
        QMessageBox::critical(this, tr("Demandes"),
                              tr("Échec de la suppression : %1").arg(q.lastError().text()));
        return;
    }

    refreshDemandeTable();
}

void MainWindow::on_statdemande_clicked()
{
    // 1) Statistiques par statut
    QSqlQuery q;
    q.prepare("SELECT STATUT, DATE_CREATION FROM DEMANDE");
    if (!q.exec()) {
        QMessageBox::warning(this, tr("Statistiques demandes"),
                             tr("Impossible de charger les demandes : %1")
                                 .arg(q.lastError().text()));
        return;
    }

    int nbTotal   = 0;
    int nbNouveau = 0;
    int nbEnCours = 0;
    int nbCloture = 0;
    int nbEchoue  = 0;

    const QDateTime now = QDateTime::currentDateTime();

    while (q.next()) {
        ++nbTotal;
        QString s = q.value(0).toString().trimmed().toLower();
        QDateTime dt = q.value(1).toDateTime();
        double heures = dt.secsTo(now) / 3600.0;

        bool isNouveau = s.contains("nouveau");
        bool isEnCours = s.contains("en_cours") || s.contains(" cours");
        bool isCloture = s.contains("cloture") || s.contains("clôt") || s.contains("clos");
        bool isEchoue  = s.contains("echou") || (isNouveau && heures >= DEMANDE_DELAI_HEURES);

        if (isEchoue) {
            ++nbEchoue;
        } else if (isEnCours) {
            ++nbEnCours;
        } else if (isCloture) {
            ++nbCloture;
        } else if (isNouveau) {
            ++nbNouveau;
        }
    }

    if (nbTotal <= 0) {
        QMessageBox::information(this, tr("Statistiques demandes"),
                                 tr("Aucune demande trouvée dans la table DEMANDE."));
        return;
    }

    QPieSeries *seriesStatut = new QPieSeries();
    auto addSliceStatut = [seriesStatut](const QString &label, int count, const QColor &color) {
        double value = count > 0 ? static_cast<double>(count) : 0.001; // pour afficher dans la légende même si 0
        QPieSlice *slice = seriesStatut->append(label, value);
        slice->setBrush(color);
    };

    addSliceStatut(tr("Nouvelles (%1)").arg(nbNouveau), nbNouveau, QColor("#3498db"));
    addSliceStatut(tr("En cours (%1)").arg(nbEnCours), nbEnCours, QColor("#f39c12"));
    addSliceStatut(tr("Clôturées (%1)").arg(nbCloture), nbCloture, QColor("#27ae60"));
    addSliceStatut(tr("Échouées (%1)").arg(nbEchoue), nbEchoue, QColor("#8e0000"));

    seriesStatut->setLabelsVisible(true);

    QChart *chartStatut = new QChart();
    chartStatut->addSeries(seriesStatut);
    chartStatut->setTitle(tr("Répartition des demandes par statut"));
    chartStatut->legend()->setAlignment(Qt::AlignRight);

    QChartView *viewStatut = new QChartView(chartStatut);
    viewStatut->setRenderHint(QPainter::Antialiasing);

    // 2) Statistiques par type de problème
    QSqlQuery qTypes;
    qTypes.prepare("SELECT TYPE_PROBLEME, COUNT(*) FROM DEMANDE GROUP BY TYPE_PROBLEME");
    bool hasTypeData = qTypes.exec();

    QChartView *viewTypes = nullptr;
    if (hasTypeData) {
        QPieSeries *seriesType = new QPieSeries();
        QVector<QColor> palette = {
            QColor("#2980b9"), QColor("#8e44ad"), QColor("#16a085"),
            QColor("#d35400"), QColor("#c0392b"), QColor("#7f8c8d")
        };
        int idxColor = 0;
        while (qTypes.next()) {
            const QString type = qTypes.value(0).toString();
            int count = qTypes.value(1).toInt();
            if (count <= 0) continue;
            QColor c = palette.at(idxColor % palette.size());
            ++idxColor;
            QPieSlice *slice = seriesType->append(QString("%1 (%2)").arg(type).arg(count), count);
            slice->setBrush(c);
        }

        if (!seriesType->slices().isEmpty()) {
            seriesType->setLabelsVisible(true);
            QChart *chartType = new QChart();
            chartType->addSeries(seriesType);
            chartType->setTitle(tr("Répartition des demandes par type de problème"));
            chartType->legend()->setAlignment(Qt::AlignRight);

            viewTypes = new QChartView(chartType);
            viewTypes->setRenderHint(QPainter::Antialiasing);
        }
    }

    // 3) Affichage dans une boîte de dialogue
    QDialog *dlg = new QDialog(this);
    dlg->setWindowTitle(tr("Statistiques des demandes"));
    QVBoxLayout *layout = new QVBoxLayout(dlg);
    layout->addWidget(viewStatut);
    if (viewTypes)
        layout->addWidget(viewTypes);
    dlg->resize(800, 600);
    dlg->exec();
}

void MainWindow::on_Affecter_clicked()
{
    if (!ui || !ui->tableView_personel_2 || !ui->tableView_habitants_2)
        return;

    QItemSelectionModel *selDem = ui->tableView_habitants_2->selectionModel();
    if (!selDem || !selDem->hasSelection()) {
        QMessageBox::warning(this, tr("Affectation"),
                             tr("Veuillez sélectionner une demande."));
        return;
    }
    int rowDem = selDem->selectedRows().first().row();
    QAbstractItemModel *modelDem = ui->tableView_habitants_2->model();
    if (!modelDem) {
        QMessageBox::warning(this, tr("Affectation"),
                             tr("Aucun modèle de données pour les demandes."));
        return;
    }

    bool okIdDem = false;
    int idDemande = modelDem->index(rowDem, 0).data().toInt(&okIdDem);
    if (!okIdDem || idDemande <= 0) {
        QMessageBox::warning(this, tr("Affectation"),
                             tr("ID de demande invalide."));
        return;
    }

    QItemSelectionModel *selPers = ui->tableView_personel_2->selectionModel();
    if (!selPers || !selPers->hasSelection()) {
        QMessageBox::warning(this, tr("Affectation"),
                             tr("Veuillez sélectionner un personnel."));
        return;
    }
    int rowPers = selPers->selectedRows().first().row();
    QAbstractItemModel *modelPers = ui->tableView_personel_2->model();
    if (!modelPers) {
        QMessageBox::warning(this, tr("Affectation"),
                             tr("Aucun modèle de données pour les personnels."));
        return;
    }

    QString idPersonnel = modelPers->index(rowPers, 0).data().toString().trimmed();
    if (idPersonnel.isEmpty()) {
        QMessageBox::warning(this, tr("Affectation"),
                             tr("ID du personnel invalide."));
        return;
    }

    QString error;
    if (!AffectationP::affecterDemande(idDemande, idPersonnel, error)) {
        QMessageBox::critical(this, tr("Affectation"),
                              tr("Échec de l'affectation : %1")
                                  .arg(error));
        return;
    }

    QMessageBox::information(this, tr("Affectation"),
                             tr("Demande %1 affectée à %2.")
                                 .arg(idDemande)
                                 .arg(idPersonnel));

    chargerTablesAffectation();
    refreshDemandeTable();
}

void MainWindow::on_Refresh_clicked()
{
    remplirTableSuivi();
}

void MainWindow::on_Resolution_clicked()
{
    if (!ui || !ui->tableWidget_2)
        return;

    QTableWidget *tw = ui->tableWidget_2;
    QItemSelectionModel *sel = tw->selectionModel();
    if (!sel || !sel->hasSelection()) {
        QMessageBox::warning(this, tr("Suivi"),
                             tr("Veuillez sélectionner une ligne."));
        return;
    }

    int row = sel->selectedRows().first().row();
    QTableWidgetItem *idItem = tw->item(row, 0);
    if (!idItem) {
        QMessageBox::warning(this, tr("Suivi"),
                             tr("Impossible de lire l'ID de la demande."));
        return;
    }

    bool ok = false;
    int idDem = idItem->text().toInt(&ok);
    if (!ok || idDem <= 0) {
        QMessageBox::warning(this, tr("Suivi"),
                             tr("ID de demande invalide."));
        return;
    }

    QString error;
    if (!AffectationP::fixerResolutionInterne(idDem, error)) {
        QMessageBox::critical(this, tr("Suivi"),
                              tr("Échec de la mise à jour : %1")
                                  .arg(error));
        return;
    }

    remplirTableSuivi();
    refreshDemandeTable();
}

void MainWindow::on_pushButton_detailsSuivi_clicked()
{
    if (!ui || !ui->tableWidget_2)
        return;

    QTableWidget *tw = ui->tableWidget_2;
    QItemSelectionModel *sel = tw->selectionModel();
    if (!sel || !sel->hasSelection()) {
        QMessageBox::information(this, tr("Détails demande"),
                                 tr("Veuillez sélectionner une demande dans le tableau de suivi."));
        return;
    }

    int row = sel->selectedRows().first().row();
    QTableWidgetItem *idItem = tw->item(row, 0);
    if (!idItem) {
        QMessageBox::warning(this, tr("Détails demande"),
                             tr("Impossible de lire l'ID de la demande."));
        return;
    }

    bool ok = false;
    int idDemande = idItem->text().toInt(&ok);
    if (!ok || idDemande <= 0) {
        QMessageBox::warning(this, tr("Détails demande"),
                             tr("ID de demande invalide."));
        return;
    }

    auto fmtDate = [](const QDateTime &dt) {
        return dt.isValid() ? dt.toString("dd/MM/yyyy HH:mm") : QString("-");
    };

    QSqlQuery q;
    q.prepare("SELECT ID_DEMANDE, ID_HABITANT, TYPE_PROBLEME, DESCRIPTION, STATUT, "
              "DATE_CREATION, DATE_AFFECTATION, DATE_RESOLUTION_INTERNE, DATE_RESOLUTION_FINALE "
              "FROM DEMANDE WHERE ID_DEMANDE = :id");
    q.bindValue(":id", idDemande);
    if (!q.exec()) {
        QMessageBox::warning(this, tr("Détails demande"),
                             tr("Erreur SQL : %1").arg(q.lastError().text()));
        return;
    }
    if (!q.next()) {
        QMessageBox::information(this, tr("Détails demande"),
                                 tr("Aucune demande trouvée pour l'ID %1.").arg(idDemande));
        return;
    }

    int idDem    = q.value(0).toInt();
    int idHab    = q.value(1).toInt();
    QString type = q.value(2).toString();
    QString desc = q.value(3).toString();
    QString statut = q.value(4).toString();
    QDateTime dc  = q.value(5).toDateTime();
    QDateTime da  = q.value(6).toDateTime();
    QDateTime dr  = q.value(7).toDateTime();
    QDateTime dru = q.value(8).toDateTime();

    QDialog dlg(this);
    dlg.setWindowTitle(tr("Détails demande"));
    dlg.setModal(true);
    dlg.resize(500, 340);

    QVBoxLayout *layout = new QVBoxLayout(&dlg);

    QTableWidget *twInfo = new QTableWidget(&dlg);
    twInfo->setColumnCount(2);
    twInfo->setRowCount(8);
    twInfo->setHorizontalHeaderLabels(QStringList() << tr("Champ") << tr("Valeur"));
    twInfo->verticalHeader()->setVisible(false);
    twInfo->horizontalHeader()->setStretchLastSection(true);
    twInfo->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    twInfo->setEditTriggers(QAbstractItemView::NoEditTriggers);
    twInfo->setSelectionMode(QAbstractItemView::NoSelection);
    twInfo->setAlternatingRowColors(true);

    // Palette et style pour une meilleure visibilité
    QPalette palTable = twInfo->palette();
    palTable.setColor(QPalette::Base, Qt::white);
    palTable.setColor(QPalette::AlternateBase, QColor("#f7f9fc"));
    palTable.setColor(QPalette::Text, QColor("#0a1a2f"));
    palTable.setColor(QPalette::WindowText, QColor("#0a1a2f"));
    twInfo->setPalette(palTable);
    twInfo->setStyleSheet(
        "QTableWidget { border: 1px solid #d0d7e2; gridline-color:#e1e5ee; }"
        "QHeaderView::section { background-color:#0d3273; color:white; font-weight:600; padding:4px 8px; border:none; }"
        "QTableWidget::item { padding:3px 6px; }");

    auto setRow = [twInfo](int row, const QString &champ, const QString &val) {
        QTableWidgetItem *c = new QTableWidgetItem(champ);
        QFont f = c->font();
        f.setBold(true);
        c->setFont(f);
        twInfo->setItem(row, 0, c);

        QTableWidgetItem *v = new QTableWidgetItem(val);
        twInfo->setItem(row, 1, v);
    };

    setRow(0, tr("ID demande"), QString::number(idDem));
    setRow(1, tr("ID habitant"), QString::number(idHab));
    setRow(2, tr("Type"), type);
    setRow(3, tr("Statut"), statut);
    setRow(4, tr("Création"), fmtDate(dc));
    setRow(5, tr("Affectation"), fmtDate(da));
    setRow(6, tr("Résolution interne"), fmtDate(dr));
    setRow(7, tr("Résolution finale"), fmtDate(dru));

    twInfo->resizeColumnsToContents();
    layout->addWidget(twInfo);

    QLabel *lblDesc = new QLabel(tr("Description :"), &dlg);
    lblDesc->setStyleSheet("color: #0a1a2f; font-weight: 600;");
    layout->addWidget(lblDesc);

    QTextEdit *editDesc = new QTextEdit(&dlg);
    editDesc->setReadOnly(true);
    editDesc->setText(desc);
    editDesc->setStyleSheet("color: black; background-color: white;");
    editDesc->setMinimumHeight(80);
    layout->addWidget(editDesc);

    QPushButton *btnClose = new QPushButton(tr("Fermer"), &dlg);
    btnClose->setCursor(Qt::PointingHandCursor);
    QObject::connect(btnClose, &QPushButton::clicked, &dlg, &QDialog::accept);
    QHBoxLayout *bottomLayout = new QHBoxLayout();
    bottomLayout->addStretch();
    bottomLayout->addWidget(btnClose);
    layout->addLayout(bottomLayout);

    dlg.exec();
}

void MainWindow::on_pushButton_exportSuivi_clicked()
{
    if (!ui || !ui->tableWidget_2)
        return;

    QString defaultPath = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
    QString fileName = QFileDialog::getSaveFileName(
        this, tr("Exporter le suivi en PDF"),
        defaultPath + "/suivi_demandes.pdf",
        tr("PDF (*.pdf)"));

    if (fileName.isEmpty())
        return;

    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(fileName);
    printer.setPageSize(QPageSize(QPageSize::A4));
    printer.setPageOrientation(QPageLayout::Landscape);

    QPainter painter;
    if (!painter.begin(&printer)) {
        QMessageBox::warning(this, tr("Export PDF"), tr("Impossible de créer le fichier PDF."));
        return;
    }

    painter.setRenderHint(QPainter::Antialiasing);

    QRect rect = painter.viewport();
    int margin = 40;
    QRect drawRect(rect.left() + margin, rect.top() + margin,
                   rect.width() - 2 * margin, rect.height() - 2 * margin);

    QSize tableSize = ui->tableWidget_2->size();
    if (tableSize.width() <= 0 || tableSize.height() <= 0)
        tableSize = QSize(800, 400);

    double xScale = double(drawRect.width()) / tableSize.width();
    double yScale = double(drawRect.height()) / tableSize.height();
    double scale = qMin(xScale, yScale);

    painter.save();
    painter.translate(drawRect.topLeft());
    painter.scale(scale, scale);
    ui->tableWidget_2->render(&painter);
    painter.restore();

    painter.end();

    QMessageBox::information(this, tr("Export PDF"),
                             tr("Le suivi a été exporté dans :\n%1").arg(fileName));
}

void MainWindow::on_Resolu_clicked()
{
    if (!ui || !ui->tableWidget_2)
        return;

    QTableWidget *tw = ui->tableWidget_2;
    QItemSelectionModel *sel = tw->selectionModel();
    if (!sel || !sel->hasSelection()) {
        QMessageBox::warning(this, tr("Suivi"),
                             tr("Veuillez sélectionner une ligne."));
        return;
    }

    int row = sel->selectedRows().first().row();
    QTableWidgetItem *idItem = tw->item(row, 0);
    if (!idItem) {
        QMessageBox::warning(this, tr("Suivi"),
                             tr("Impossible de lire l'ID de la demande."));
        return;
    }

    bool ok = false;
    int idDem = idItem->text().toInt(&ok);
    if (!ok || idDem <= 0) {
        QMessageBox::warning(this, tr("Suivi"),
                             tr("ID de demande invalide."));
        return;
    }

    QString error;
    if (!AffectationP::fixerResolutionFinale(idDem, error)) {
        QMessageBox::critical(this, tr("Suivi"),
                              tr("Échec de la mise à jour : %1")
                                  .arg(error));
        return;
    }

    remplirTableSuivi();
    refreshDemandeTable();
}

void MainWindow::styliserChampsSaisie()
{
    const QString lineEditCss =
        "QLineEdit {"
        "  color: #111111;"
        "  background-color: #ffffff;"
        "  border: 1px solid #5dade2;"
        "  selection-background-color: #cfe3ff;"
        "  selection-color: #111111;"
        "}";

    QVector<QLineEdit*> edits = {
        ui->lineEdit_id,
        ui->lineEdit_nom,
        ui->lineEdit_prenom,
        ui->lineEdit_adresse,
        ui->lineEdit_zone,
        ui->lineEdit_situation,
        ui->lineEdit_contact,
        ui->lineEdit_statut,
        ui->lineEdit_besoins,
        ui->lineEdit_satisfaction
    };
    for (QLineEdit *e : edits) {
        if (!e) continue;
        e->setStyleSheet(lineEditCss);
        QPalette pal = e->palette();
        pal.setColor(QPalette::Text, QColor("#111111"));
        pal.setColor(QPalette::PlaceholderText, QColor("#6c757d"));
        e->setPalette(pal);
    }
}

void MainWindow::styliserTableView(QTableView *view)
{
    if (!view)
        return;

    view->setAlternatingRowColors(true);
    view->setSelectionBehavior(QAbstractItemView::SelectRows);
    view->setSelectionMode(QAbstractItemView::SingleSelection);
    view->setShowGrid(false);
    view->verticalHeader()->setVisible(false);
    if (view->horizontalHeader()) {
        view->horizontalHeader()->setStretchLastSection(true);
        view->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        view->horizontalHeader()->setStyleSheet(
            "QHeaderView::section {"
            "  background-color: #0d3273;"
            "  color: white;"
            "  font-weight: 600;"
            "  padding: 6px 8px;"
            "  border: none;"
            "  border-right: 1px solid #214a8a;"
            "}"
            );
    }

    view->setStyleSheet(
        "QTableView {"
        "  background-color: #ffffff;"
        "  border: 1px solid #d0d7e2;"
        "  gridline-color: #e1e5ee;"
        "  selection-background-color: #c7e0ff;"
        "  selection-color: #0a1a2f;"
        "}"
        "QTableView::item {"
        "  padding: 4px 6px;"
        "}"
        "QTableView::item:selected {"
        "  background-color: #c7e0ff;"
        "  color: #0a1a2f;"
        "}"
        );
}

void MainWindow::styliserBoutonPrincipal(QPushButton *button)
{
    if (!button)
        return;

    button->setCursor(Qt::PointingHandCursor);
    button->setStyleSheet(
        "QPushButton {"
        "  background-color: #012a59;"
        "  color: #ffffff;"
        "  border-radius: 10px;"
        "  padding: 8px 18px;"
        "  font-size: 13px;"
        "  font-weight: 500;"
        "}"
        "QPushButton:hover {"
        "  background-color: #024b8a;"
        "}"
        "QPushButton:pressed {"
        "  background-color: #013869;"
        "}"
        );
}

void MainWindow::styliserModulePersonnels()
{
    // Tables du module personnels
    styliserTableView(ui->tableView_personel);
    styliserTableView(ui->tableView_personel_2);
    styliserTableView(ui->tableView_habitants_2);

    if (ui->tableWidget_2)
        styliserTableView(ui->tableWidget_2);
    if (ui->tabdemande_2)
        styliserTableView(ui->tabdemande_2);

    // Boutons d'action (affectation et suivi)
    if (ui->Affecter) {
        ui->Affecter->setText(tr("⇄ Affecter"));
        styliserBoutonPrincipal(ui->Affecter);
    }
    if (ui->Refresh) {
        ui->Refresh->setText(tr("🔄 Refresh"));
        styliserBoutonPrincipal(ui->Refresh);
    }
    if (ui->Resolution) {
        ui->Resolution->setText(tr("⚙ Résolution"));
        styliserBoutonPrincipal(ui->Resolution);
    }
    if (ui->Resolu) {
        ui->Resolu->setText(tr("✔ Résolu"));
        styliserBoutonPrincipal(ui->Resolu);
    }
}

void MainWindow::ouvrirDemandeDansSuivi(int idDemande)
{
    if (!ui || !ui->stackedWidget_6 || !ui->tableWidget_2)
        return;

    ui->stackedWidget_6->setCurrentIndex(2);
    remplirTableSuivi();

    QTableWidget *tw = ui->tableWidget_2;
    int foundRow = -1;
    for (int r = 0; r < tw->rowCount(); ++r) {
        QTableWidgetItem *it = tw->item(r, 0);
        if (it && it->text().toInt() == idDemande) {
            foundRow = r;
            break;
        }
    }

    if (foundRow >= 0) {
        tw->selectRow(foundRow);
        tw->scrollToItem(tw->item(foundRow, 0), QAbstractItemView::PositionAtCenter);
    }
}

void MainWindow::updateAffectationSummary()
{
    if (!ui || !affectationSummaryLabel)
        return;

    QString demandeText;
    QString personnelText;
    QString recoText;

    // --- Demande sélectionnée ---
    int demRow = -1;
    if (ui->tableView_habitants_2 && ui->tableView_habitants_2->selectionModel() &&
        ui->tableView_habitants_2->selectionModel()->hasSelection()) {
        demRow = ui->tableView_habitants_2->selectionModel()->selectedRows().first().row();
    }
    QAbstractItemModel *demModel = ui->tableView_habitants_2 ? ui->tableView_habitants_2->model() : nullptr;
    QString typeProb;

    if (demModel && demRow >= 0) {
        int idDem = demModel->index(demRow, 0).data().toInt();
        int idHab = demModel->index(demRow, 1).data().toInt();
        typeProb  = demModel->index(demRow, 2).data().toString();
        QString statut = demModel->index(demRow, 3).data().toString();
        demandeText = tr("#%1 – %2 – %3 (habitant %4)")
                          .arg(idDem)
                          .arg(typeProb)
                          .arg(statut)
                          .arg(idHab);
    }

    // --- Personnel sélectionné ---
    int persRow = -1;
    if (ui->tableView_personel_2 && ui->tableView_personel_2->selectionModel() &&
        ui->tableView_personel_2->selectionModel()->hasSelection()) {
        persRow = ui->tableView_personel_2->selectionModel()->selectedRows().first().row();
    }
    QAbstractItemModel *persModel = ui->tableView_personel_2 ? ui->tableView_personel_2->model() : nullptr;

    if (persModel && persRow >= 0) {
        QString cin   = persModel->index(persRow, 0).data().toString();
        QString nom   = persModel->index(persRow, 1).data().toString();
        QString pren  = persModel->index(persRow, 2).data().toString();
        QString zone  = persModel->index(persRow, 3).data().toString();
        QString dispo = persModel->index(persRow, 4).data().toString();
        personnelText = tr("%1 %2 (CIN %3, Zone %4, %5)")
                            .arg(nom)
                            .arg(pren)
                            .arg(cin)
                            .arg(zone)
                            .arg(dispo);
    }

    // --- Recommandation simple de personnel selon le type de problème ---
    if (persModel && !typeProb.trimmed().isEmpty()) {
        QString typeLower = typeProb.toLower();
        int bestRow = -1;
        int bestScore = 0;

        for (int r = 0; r < persModel->rowCount(); ++r) {
            QString comp = persModel->index(r, 5).data().toString().toLower();
            QString dispo = persModel->index(r, 4).data().toString().toLower();

            int score = 0;
            // Bonus si disponible
            if (dispo.contains("disponible"))
                score += 2;

            // Mots-clés simples selon le type de problème
            if (typeLower.contains("propret") || typeLower.contains("déchet") || typeLower.contains("dechet")) {
                if (comp.contains("propret") || comp.contains("déchet") || comp.contains("dechet"))
                    score += 3;
            }
            if (typeLower.contains("éclairage") || typeLower.contains("eclairage")) {
                if (comp.contains("éclairage") || comp.contains("eclairage") || comp.contains("électric"))
                    score += 3;
            }
            if (typeLower.contains("sécurité") || typeLower.contains("securite")) {
                if (comp.contains("sécurité") || comp.contains("securite"))
                    score += 3;
            }
            if (typeLower.contains("eau") || typeLower.contains("électricité") || typeLower.contains("electricite")) {
                if (comp.contains("réseau") || comp.contains("reseau") || comp.contains("eau") || comp.contains("électric"))
                    score += 3;
            }

            if (score > bestScore) {
                bestScore = score;
                bestRow = r;
            }
        }

        if (bestRow >= 0 && bestScore > 0) {
            QString cin   = persModel->index(bestRow, 0).data().toString();
            QString nom   = persModel->index(bestRow, 1).data().toString();
            QString pren  = persModel->index(bestRow, 2).data().toString();
            QString comp  = persModel->index(bestRow, 5).data().toString();
            recoText = tr("Recommandé : %1 %2 (CIN %3, compétences : %4)")
                           .arg(nom)
                           .arg(pren)
                           .arg(cin)
                           .arg(comp);
        }
    }

    QString summary;
    if (!demandeText.isEmpty())
        summary += tr("Demande sélectionnée : %1").arg(demandeText);
    else
        summary += tr("Aucune demande sélectionnée.");

    summary += "\n";
    if (!personnelText.isEmpty())
        summary += tr("Personnel sélectionné : %1").arg(personnelText);
    else
        summary += tr("Aucun personnel sélectionné.");

    if (!recoText.isEmpty()) {
        summary += "\n" + recoText;
    }

    affectationSummaryLabel->setText(summary);
}

void MainWindow::saveLastHabitantId(int id)
{
    QSettings s("Touha", "Ghabitants");
    s.setValue("last_habitant_id", id);
}

void MainWindow::loadLastHabitantSelection()
{
    QSettings s("Touha", "Ghabitants");
    bool ok = false;
    int lastId = s.value("last_habitant_id").toInt(&ok);
    if (!ok || lastId <= 0 || !ui->tableView_habitants || !ui->tableView_habitants->model()) return;
    QAbstractItemModel *m = ui->tableView_habitants->model();
    int targetRow = -1;
    for (int r = 0; r < m->rowCount(); ++r) {
        const QVariant v = m->index(r, 0).data();
        if (v.toInt() == lastId) { targetRow = r; break; }
    }
    if (targetRow < 0) return;
    QItemSelectionModel *sel = ui->tableView_habitants->selectionModel();
    if (!sel) return;
    sel->select(m->index(targetRow, 0), QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);
    ui->tableView_habitants->scrollTo(m->index(targetRow, 0), QAbstractItemView::PositionAtCenter);
    populateEditsFromRow(targetRow);
}

void MainWindow::populateEditsFromRow(int row)
{
    QAbstractItemModel *m = ui->tableView_habitants->model();
    if (!m || row < 0 || row >= m->rowCount()) return;
    ui->lineEdit_id->setText(m->index(row, 0).data().toString());
    ui->lineEdit_nom->setText(m->index(row, 1).data().toString());
    ui->lineEdit_prenom->setText(m->index(row, 2).data().toString());
    ui->lineEdit_adresse->setText(m->index(row, 3).data().toString());
    ui->lineEdit_zone->setText(m->index(row, 4).data().toString());
    ui->lineEdit_situation->setText(m->index(row, 5).data().toString());
    ui->lineEdit_contact->setText(m->index(row, 6).data().toString());
    ui->lineEdit_statut->setText(m->index(row, 7).data().toString());
    ui->lineEdit_besoins->setText(m->index(row, 8).data().toString());
    ui->lineEdit_satisfaction->setText(m->index(row, 9).data().toString());
}

void MainWindow::rechercherTexte(const QString &texte)
{
    const QString critere = texte.trimmed();
    if (critere.isEmpty()) { chargerDonneesTable(); return; }
    QString like = critere;
    QSqlQueryModel *model = new QSqlQueryModel(this);
    model->setQuery(QString(
                        "SELECT ID_HABITANT, NOM, PRENOM, ADRESSE, ZONE_RESIDENCE, SITUATION_FAMILIALE, CONTACT, STATUT, BESOINS_SPECIFIQUES, NIVEAU_SATISFACTION "
                        "FROM HABITANT WHERE NOM LIKE '%%1%' OR PRENOM LIKE '%%1%' OR ADRESSE LIKE '%%1%' OR ZONE_RESIDENCE LIKE '%%1%' OR STATUT LIKE '%%1%' OR CONTACT LIKE '%%1%'"
                        ).arg(like.replace("'", "''")));
    ui->tableView_habitants->setModel(model);
    ui->tableView_habitants->resizeColumnsToContents();
}
void MainWindow::setupTable()
{
    QTableWidget *table = ui->twz;

    table->setSortingEnabled(true);
    table->horizontalHeader()->setSortIndicatorShown(true);
    table->horizontalHeader()->setSectionsClickable(true);

    int currentCols = table->columnCount();
    bool hasDeleteColumn = false;

    if (currentCols > 0 && table->horizontalHeaderItem(currentCols - 1))
    {
        QString lastHeader = table->horizontalHeaderItem(currentCols - 1)->text();
        if (lastHeader == " ") hasDeleteColumn = true;
    }

    if (!hasDeleteColumn)
    {
        table->insertColumn(currentCols);
        table->setHorizontalHeaderItem(currentCols, new QTableWidgetItem(" "));
    }

    for (int row = 0; row < table->rowCount(); ++row)
    {
        QPushButton *btnSupprimer = new QPushButton("❌");
        btnSupprimer->setFixedSize(30, 25);
        btnSupprimer->setStyleSheet(
            "QPushButton { "
            "background-color: #b0000; "
            "color: white; "
            "border-radius: 6px; "
            "font-weight: bold;"
            "} "
            "QPushButton:hover { background-color: #ff4444; }"
            );
        connect(btnSupprimer, &QPushButton::clicked, this, &MainWindow::supprimerEtDecalerLignes);
        table->setCellWidget(row, table->columnCount() - 1, btnSupprimer);
    }

    table->resizeColumnsToContents();
}

void MainWindow::supprimerEtDecalerLignes()
{
    QTableWidget *table = ui->twz;
    QPushButton *button = qobject_cast<QPushButton*>(sender());
    if (!button) return;

    int deleteRow = -1;

    for (int i = 0; i < table->rowCount(); ++i)
    {
        if (table->cellWidget(i, table->columnCount() - 1) == button)
        {
            deleteRow = i;
            break;
        }
    }

    if (deleteRow == -1) return;

    table->removeRow(deleteRow);

    int deleteCol = table->columnCount() - 1;
    for (int row = 0; row < table->rowCount(); ++row)
    {
        QWidget *widget = table->cellWidget(row, deleteCol);
        if (!widget)
        {
            QPushButton *btnSupprimer = new QPushButton("❌");
            btnSupprimer->setFixedSize(30, 25);
            btnSupprimer->setStyleSheet(
                "QPushButton { background-color: #b0000; color: white; border-radius: 6px; font-weight: bold; }"
                "QPushButton:hover { background-color: #ff4444; }"
                );
            connect(btnSupprimer, &QPushButton::clicked, this, &MainWindow::supprimerEtDecalerLignes);
            table->setCellWidget(row, deleteCol, btnSupprimer);
        }
    }
    QPushButton *btn = qobject_cast<QPushButton*>(sender());
    if (!btn) return;

    //QTableWidget *table = ui->tableWidget_6;
    for (int row = 0; row < table->rowCount(); ++row) {
        if (table->cellWidget(row, table->columnCount() - 1) == btn) {
            table->removeRow(row);
            break;
        }
    }
}

// ==================== Stacked Widgets Navigation ====================
void MainWindow::on_pushButton_clicked() { ui->stackedWidget_2->setCurrentIndex(0); }
void MainWindow::on_pushButton2_clicked() { ui->stackedWidget_2->setCurrentIndex(1); }
void MainWindow::on_pushButton3_clicked() { ui->stackedWidget_2->setCurrentIndex(2); }

void MainWindow::on_on_pushButton_clicked() { ui->stackedWidget_2->setCurrentIndex(0); }
void MainWindow::on_on_pushButton_2_clicked() { ui->stackedWidget_2->setCurrentIndex(2); }
void MainWindow::on_on_pushButton_3_clicked() { ui->stackedWidget_2->setCurrentIndex(3); }

void MainWindow::on_Ghabitant_clicked() { ui->stackedWidget->setCurrentIndex(0); }
void MainWindow::on_Ghabitats_2_clicked() { ui->stackedWidget->setCurrentIndex(1); }
void MainWindow::on_Gpersonnels_2_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
}
void MainWindow::on_Gequipements_2_clicked() { ui->stackedWidget->setCurrentIndex(4); }
void MainWindow::on_Gvehicules_2_clicked() { ui->stackedWidget->setCurrentIndex(5); }
void MainWindow::on_Gespace_clicked() { ui->stackedWidget->setCurrentIndex(3); }

void MainWindow::on_on_pushButton_4_clicked() { ui->stackedWidget_5->setCurrentIndex(1); }
void MainWindow::on_on_pushButton_5_clicked() { ui->stackedWidget_5->setCurrentIndex(2); }
void MainWindow::on_on_pushButton_6_clicked() { ui->stackedWidget_5->setCurrentIndex(0); }

void MainWindow::on_on_pushButton_7_clicked()
{
    ui->stackedWidget_6->setCurrentIndex(0);
}

void MainWindow::on_on_pushButton_8_clicked()
{
    ui->stackedWidget_6->setCurrentIndex(1);
    chargerTablesAffectation();
}

void MainWindow::on_on_pushButton_9_clicked()
{
    ui->stackedWidget_6->setCurrentIndex(2);
    remplirTableSuivi();
}

void MainWindow::on_on_pushButton_13_clicked() { ui->stackedWidget_2->setCurrentIndex(0); }
void MainWindow::on_on_pushButton_14_clicked() { ui->stackedWidget_2->setCurrentIndex(2); }
void MainWindow::on_on_pushButton_15_clicked() { ui->stackedWidget_2->setCurrentIndex(3); }
void MainWindow::on_on_pushButton_16_clicked() { ui->stackedWidget_7->setCurrentIndex(0); }
void MainWindow::on_on_pushButton_17_clicked() { ui->stackedWidget_7->setCurrentIndex(1); }
void MainWindow::on_pushButton_15_clicked() { ui->stackedWidget_7->setCurrentIndex(2); }

void MainWindow::on_gespub_2_clicked() { ui->stackedWidget_3->setCurrentIndex(0); }
void MainWindow::on_geszone1_clicked() { ui->stackedWidget_3->setCurrentIndex(1); }

/*void MainWindow::on_pushButton_30_clicked()
{
    QTableWidget *table = ui->tableWidget_6;
    int newRow = table->rowCount();
    table->insertRow(newRow);

    int deleteCol = table->columnCount() - 1;
    for (int col = 0; col < deleteCol; ++col)
        table->setItem(newRow, col, new QTableWidgetItem(""));

    QPushButton *btnSupprimer = new QPushButton("❌");
    btnSupprimer->setFixedSize(30, 25);
    btnSupprimer->setStyleSheet(
        "QPushButton { background-color: #b0000; color: white; border-radius: 6px; font-weight: bold; }"
        "QPushButton:hover { background-color: #ff4444; }"
        );

    connect(btnSupprimer, &QPushButton::clicked, this, &MainWindow::supprimerEtDecalerLignes);
    table->setCellWidget(newRow, deleteCol, btnSupprimer);
    table->resizeColumnsToContents();
}
*/
// ==================== CRUD Add Mahdy====================
void MainWindow::setupGraph()
{
    if (!graphScene)
        graphScene = new Graph(this);

    if (!zonesScene) {
        zonesScene = new QGraphicsScene(this);
        ui->graphicsZones_2->setScene(zonesScene);
        ui->graphicsZones_2->setRenderHint(QPainter::Antialiasing);
    }

    if (!binsScene) {
        binsScene = new QGraphicsScene(this);
        ui->graphicsBins_2->setScene(binsScene);
        ui->graphicsBins_2->setRenderHint(QPainter::Antialiasing);
    }
}

// Render simple bins visualization for a zone (capacity + status)
void MainWindow::renderBinsGraph(int zoneId)
{
    setupGraph();
    if (!binsScene) return;

    binsScene->clear();
    currentBinsZoneId = zoneId;

    const Graph::Zone *zone = graphScene ? graphScene->zoneById(zoneId) : nullptr;
    if (!zone) {
        statusBar()->showMessage(tr("Zone %1 introuvable pour le graphe").arg(zoneId), 5000);
        return;
    }

    // capacity calculation (keep your constant)
    const double CAPACITY_PER_1000_POP = 10.0;
    const double requiredCapacity = (zone->pop * CAPACITY_PER_1000_POP) / 1000.0;

    // layout tunables
    const int cols = 10;
    const qreal startX = 20.0;
    const qreal startY = 20.0;
    const qreal binSize = 16.0;           // square size
    const qreal spacingX = 24.0;          // horizontal step between bins
    const qreal spacingY = 30.0;          // vertical step (room for capacity label)

    int index = 0;
    int functionalCapacity = 0;
    QStringList messages;

    // fonts (created once)
    QFont idFont;
    idFont.setBold(true);
    idFont.setPointSize(9);

    QFont capFont;
    capFont.setPointSize(3);

    for (const BinInfo &b : binsCache) {
        if (b.zoneId != zoneId) continue;

        const int col = index % cols;
        const int row = index / cols;
        QRectF rect(startX + col * spacingX,
                    startY + row * spacingY,
                    binSize,
                    binSize);

        // Draw the bin rectangle
        QPen pen(Qt::black);
        QGraphicsRectItem *item = binsScene->addRect(rect, pen);
        item->setZValue(0); // rectangle behind text

        // status -> fill/outline rules
        const QString status = b.status.trimmed().toUpper();
        if (status == QLatin1String("BROKEN")) {
            item->setBrush(QColor(255, 165, 0)); // orange
            messages << tr("Bin %1 is broken").arg(b.id);
        } else if (status == QLatin1String("DAMAGED") || status == QLatin1String("ENDOMMAGE")) {
            item->setBrush(Qt::NoBrush);
            QPen p = item->pen();
            p.setStyle(Qt::DashLine);
            p.setColor(Qt::red);
            item->setPen(p);
            messages << tr("Bin %1 needs repair").arg(b.id);
        } else if (status == QLatin1String("FULL") || status == QLatin1String("PLEIN")) {
            item->setBrush(Qt::red);
            messages << tr("Bin %1 is full").arg(b.id);
        } else {
            item->setBrush(Qt::green);
            functionalCapacity += b.cap;
        }

        // --- ID label (centered inside the bin) ---
        QGraphicsSimpleTextItem *idLabel = binsScene->addSimpleText(QString::number(b.id));
        idLabel->setFont(idFont);
        idLabel->setBrush(Qt::black);
        // bounding rect may depend on font, so measure it
        QRectF idBounds = idLabel->boundingRect();
        qreal idX = rect.left() + (rect.width() - idBounds.width()) / 2.0;
        qreal idY = rect.top()  + (rect.height() - idBounds.height()) / 2.0;
        idLabel->setPos(idX, idY);
        idLabel->setZValue(10); // above the rectangle

        // --- Capacity label below the bin (small, black) ---
        QGraphicsSimpleTextItem *capLabel = binsScene->addSimpleText(QString::number(b.cap));
        capLabel->setFont(capFont);
        capLabel->setBrush(Qt::black); // changed to black
        QRectF capBounds = capLabel->boundingRect();
        qreal capX = rect.left() + (rect.width() - capBounds.width()) / 2.0;
        qreal capY = rect.bottom() + 2.0;
        capLabel->setPos(capX, capY);
        capLabel->setZValue(10);

        // optional: tooltip for more info
        QString tip = tr("Bin %1\nCapacity: %2\nStatus: %3")
                          .arg(b.id)
                          .arg(b.cap)
                          .arg(b.status);
        item->setToolTip(tip);
        idLabel->setToolTip(tip);
        capLabel->setToolTip(tip);

        ++index;
    }

    // determine zone validity message and style
    QString resultMsg;
    QString colorStyle;
    if (functionalCapacity > requiredCapacity) {
        resultMsg = tr("Zone valide");
        colorStyle = "color:green; font-weight:bold;";
    } else if (std::abs(functionalCapacity - requiredCapacity) < 0.001) {
        resultMsg = tr("Zone limite, surveiller la capacité");
        colorStyle = "color:orange; font-weight:bold;";
    } else {
        resultMsg = tr("Zone non valide : capacité insuffisante");
        colorStyle = "color:red; font-weight:bold;";
    }

    QString mainMessage = tr("Zone %1 → Capacité requise: %2 | Capacité disponible: %3\n%4")
                              .arg(zoneId)
                              .arg(QString::number(requiredCapacity, 'f', 2))
                              .arg(functionalCapacity)
                              .arg(resultMsg);

    if (!messages.isEmpty())
        mainMessage += "\n" + messages.join("\n");

    // Display summary under the bins graph as requested
    if (binMessageLabel) {
        binMessageLabel->setStyleSheet(colorStyle);
        binMessageLabel->setText(mainMessage);
    } else {
        statusBar()->showMessage(mainMessage, 8000);
    }

    // Fit the scene into view if we have items
    if (binsScene && !binsScene->items().isEmpty())
        ui->graphicsBins_2->fitInView(binsScene->itemsBoundingRect(), Qt::KeepAspectRatio);
}

void MainWindow::reloadBinsCache()
{
    binsCache.clear();

    QSqlQuery query;
    if (!query.exec("SELECT ID_BIN, ID_ZONE, CAPACITE, STATUS FROM GESPUB"))
        return;

    while (query.next()) {
        BinInfo b;
        b.id = query.value("ID_BIN").toInt();
        b.zoneId = query.value("ID_ZONE").toInt();
        b.cap = query.value("CAPACITE").toInt();
        b.status = query.value("STATUS").toString();
        binsCache.append(b);
    }
}

// Call this whenever the database changes to refresh the map
void MainWindow::refreshGraph()
{
    setupGraph();
    if (!graphScene || !zonesScene)
        return;

    graphScene->loadFromDatabase();
    graphScene->renderToScene(zonesScene);

    if (!zonesScene->items().isEmpty())
        ui->graphicsZones_2->fitInView(zonesScene->itemsBoundingRect(), Qt::KeepAspectRatio);

    reloadBinsCache();

    if (currentBinsZoneId > 0)
        renderBinsGraph(currentBinsZoneId);

    ui->graphicsZones_2->viewport()->update();
}
void MainWindow::on_exp_2_clicked()
{
    QString fileNameBase = ui->filenamep->text().trimmed();

    if (fileNameBase.isEmpty()) {
        QMessageBox::warning(this, "⚠️ Nom vide", "Entrez un nom de fichier !");
        return;
    }

    QString downloadsPath = QStandardPaths::writableLocation(QStandardPaths::DownloadLocation);
    QString fullPath;

    // PDF
    if (ui->typeexp_2->currentIndex() == 0)
    {
        fullPath = downloadsPath + "/" + fileNameBase + ".pdf";

        QString html = "<h2>Liste des poubelles</h2><table border='1' cellspacing='0' cellpadding='3'><tr>";

        for (int col = 0; col < ui->twp->columnCount(); ++col)
            html += "<th>" + ui->twp->horizontalHeaderItem(col)->text() + "</th>";

        html += "</tr>";

        for (int row = 0; row < ui->twp->rowCount(); ++row)
        {
            html += "<tr>";
            for (int col = 0; col < ui->twp->columnCount(); ++col)
                html += "<td>" + ui->twp->item(row, col)->text() + "</td>";
            html += "</tr>";
        }

        html += "</table>";

        QTextDocument doc;
        doc.setHtml(html);

        QPrinter printer;                  // Qt6 compatible
        printer.setOutputFormat(QPrinter::PdfFormat);
        printer.setOutputFileName(fullPath);

        doc.print(&printer);
    }
    else
    {
        // CSV
        fullPath = downloadsPath + "/" + fileNameBase + ".csv";
        QFile file(fullPath);

        if(file.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            QTextStream out(&file);

            for (int col = 0; col < ui->twp->columnCount(); ++col)
            {
                out << ui->twp->horizontalHeaderItem(col)->text();
                if (col < ui->twp->columnCount() - 1) out << ",";
            }
            out << "\n";

            for (int row = 0; row < ui->twp->rowCount(); ++row)
            {
                for (int col = 0; col < ui->twp->columnCount(); ++col)
                {
                    out << ui->twp->item(row, col)->text();
                    if (col < ui->twp->columnCount() - 1) out << ",";
                }
                out << "\n";
            }

            file.close();
        }
    }

    QMessageBox::information(this, "✅ Export", "Fichier exporté :\n" + fullPath);
}
void MainWindow::on_tri_2_clicked()
{
    int sortIndex = ui->cbt->currentIndex();

    Poubelle p;
    QSqlQueryModel* model = p.trier(sortIndex);

    // RESET ONLY ROWS (not headers)
    ui->twp->setRowCount(0);

    // SET COLUMN COUNT FROM MODEL
    ui->twp->setColumnCount(model->columnCount());

    // SET HEADERS
    for (int c = 0; c < model->columnCount(); ++c) {
        ui->twp->setHorizontalHeaderItem(
            c,
            new QTableWidgetItem(model->headerData(c, Qt::Horizontal).toString())
            );
    }

    // INSERT DATA
    for (int r = 0; r < model->rowCount(); ++r) {
        ui->twp->insertRow(r);
        for (int c = 0; c < model->columnCount(); ++c) {
            ui->twp->setItem(
                r,
                c,
                new QTableWidgetItem(model->data(model->index(r, c)).toString())
                );
        }
    }
}
void MainWindow::on_searchpp_clicked()
{
    QString searchId = ui->searchp->text().trimmed();

    if(searchId.isEmpty()) {
        QMessageBox::warning(this, "⚠️ Vide", "Veuillez entrer un ID !");
        return;
    }

    bool found = false;

    for (int row = 0; row < ui->twp->rowCount(); ++row)
    {
        bool match = (ui->twp->item(row, 0)->text() == searchId);

        for (int col = 0; col < ui->twp->columnCount(); ++col)
        {
            if (match)
            {
                ui->twp->item(row, col)->setBackground(Qt::yellow);
                found = true;
            }
            else
            {
                ui->twp->item(row, col)->setBackground(Qt::white);
            }
        }
    }

    if (!found)
        QMessageBox::information(this, "❌ Non trouvé", "Aucune poubelle trouvée !");
}
void MainWindow::on_searchzone_clicked()
{
    QString searchId = ui->searchz->text().trimmed();
    if (searchId.isEmpty()) {
        QMessageBox::warning(this, "⚠️ Champ vide", "Veuillez entrer un ID de zone !");
        return;
    }

    bool found = false;
    for (int row = 0; row < ui->twz->rowCount(); ++row) {
        bool match = (ui->twz->item(row, 0)->text() == searchId);
        for (int col = 0; col < ui->twz->columnCount(); ++col) {
            if (match) {
                ui->twz->item(row, col)->setBackground(Qt::yellow);
                found = true;
            } else {
                // Reset background if not matching
                ui->twz->item(row, col)->setBackground(Qt::white);
            }
        }
    }

    if (!found) {
        QMessageBox::information(this, "ℹ️ Non trouvé", "Aucune zone avec cet ID n'a été trouvée !");
    }
}
void MainWindow::on_exp_clicked()
{
    QString fileNameBase = ui->nameexp->text().trimmed();

    if (fileNameBase.isEmpty()) {
        QMessageBox::warning(this, "⚠️ Nom manquant", "Veuillez entrer un nom pour le fichier !");
        return;
    }

    // Get Downloads folder
    QString downloadsPath = QStandardPaths::writableLocation(QStandardPaths::DownloadLocation);
    QString fullPath;

    if (ui->typeexp->currentIndex() == 0) { // PDF
        fullPath = downloadsPath + QDir::separator() + fileNameBase + ".pdf";

        // --- Create PDF using QTextDocument + QPrinter ---
        QString html;
        html += "<h2>Liste des zones exportée</h2>";
        html += "<table border='1' cellspacing='0' cellpadding='3'>";

        // Header
        html += "<tr>";
        for (int col = 0; col < ui->twz->columnCount(); ++col)
            html += "<th>" + ui->twz->horizontalHeaderItem(col)->text() + "</th>";
        html += "</tr>";

        // Data
        for (int row = 0; row < ui->twz->rowCount(); ++row) {
            html += "<tr>";
            for (int col = 0; col < ui->twz->columnCount(); ++col) {
                html += "<td>" + ui->twz->item(row, col)->text() + "</td>";
            }
            html += "</tr>";
        }

        html += "</table>";

        QTextDocument doc;
        doc.setHtml(html);

        QPrinter printer(QPrinter::PrinterResolution);
        printer.setOutputFormat(QPrinter::PdfFormat);
        printer.setOutputFileName(fullPath);

        doc.print(&printer);

    } else { // EXCEL / CSV
        fullPath = downloadsPath + QDir::separator() + fileNameBase + ".csv";
        QFile file(fullPath);
        if(file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream out(&file);
            // Optional: write headers
            for (int col = 0; col < ui->twz->columnCount(); ++col) {
                out << ui->twz->horizontalHeaderItem(col)->text();
                if (col != ui->twz->columnCount()-1) out << ",";
            }
            out << "\n";

            // Write table content
            for (int row = 0; row < ui->twz->rowCount(); ++row) {
                for (int col = 0; col < ui->twz->columnCount(); ++col) {
                    out << ui->twz->item(row, col)->text();
                    if (col != ui->twz->columnCount()-1) out << ",";
                }
                out << "\n";
            }
            file.close();
        }
    }

    QMessageBox::information(this, "✅ Export terminé", "Fichier exporté dans le dossier Downloads:\n" + fullPath);
}
void MainWindow::on_tri_clicked()
{
    int sortIndex = ui->tris->currentIndex(); // 0 = ID_ZONE, 1 = POPULATION

    Zone z;
    QSqlQueryModel* model = z.trier(sortIndex); // <-- you need to create this function in Zone

    ui->twz->clear();
    ui->twz->setRowCount(model->rowCount());
    ui->twz->setColumnCount(model->columnCount());

    for (int c = 0; c < model->columnCount(); ++c)
        ui->twz->setHorizontalHeaderItem(c, new QTableWidgetItem(model->headerData(c, Qt::Horizontal).toString()));

    for (int r = 0; r < model->rowCount(); ++r)
    {
        for (int c = 0; c < model->columnCount(); ++c)
        {
            QString val = model->data(model->index(r, c)).toString();
            ui->twz->setItem(r, c, new QTableWidgetItem(val));
        }
    }
}
void MainWindow::on_twp_cellClicked(int row, int column)
{
    Q_UNUSED(column);  // removes warning

    QString id_bin   = ui->twp->item(row, 0)->text(); // ID_BIN
    QString capacite = ui->twp->item(row, 1)->text(); // CAPACITE
    QString stat     = ui->twp->item(row, 2)->text(); // STATUS
    QString id_zone  = ui->twp->item(row, 3)->text(); // ID_ZONE

    ui->pubid->setText(id_bin);
    ui->cap->setText(capacite);
    // status is a QComboBox now, set current text
    ui->status->setCurrentText(stat);
    ui->zoneid->setText(id_zone);
}

void MainWindow::on_twz_cellClicked(int row, int column)
{
    Q_UNUSED(column); // remove warning

    ui->idzone->setText(ui->twz->item(row, 0)->text()); // ID_ZONE
    ui->pop->setText(ui->twz->item(row, 1)->text());    // POPULATION
    ui->x->setText(ui->twz->item(row, 2)->text());      // X
    ui->y->setText(ui->twz->item(row, 3)->text());      // Y
    ui->l->setText(ui->twz->item(row, 4)->text());      // LENGTH
    ui->h->setText(ui->twz->item(row, 5)->text());      // HEIGHT

    // Show associated poubelles for the selected zone
    int zid = ui->twz->item(row, 0)->text().toInt();
    reloadBinsCache();
    renderBinsGraph(zid);
}
void MainWindow::on_ajzone_clicked()
{
    // --- Contrôle de saisie ---
    if (ui->idzone->text().isEmpty() ||
        ui->pop->text().isEmpty() ||
        ui->x->text().isEmpty() ||
        ui->y->text().isEmpty() ||
        ui->l->text().isEmpty() ||
        ui->h->text().isEmpty())
    {
        QMessageBox::warning(this, "⚠️ Champ vide", "Tous les champs doivent être remplis !");
        return;
    }

    int idzone = ui->idzone->text().toInt();
    int pop = ui->pop->text().toInt();
    double x = ui->x->text().toDouble();
    double y = ui->y->text().toDouble();
    double l = ui->l->text().toDouble();
    double h = ui->h->text().toDouble();

    Zone z(idzone, pop, x, y, l, h);
    if (z.ajouter())
        QMessageBox::information(this, "✅ Succès", "Zone ajoutée avec succès !");
    else
        QMessageBox::critical(this, "❌ Erreur", "Échec d'ajout de la zone !");

    // Keep the graph active after a DB insert
    refreshGraph();
}

void MainWindow::on_ajpoub_clicked()
{
    // --- Contrôle de saisie ---
    if (ui->pubid->text().isEmpty() ||
        ui->zoneid->text().isEmpty() ||
        ui->cap->text().isEmpty() ||
        ui->status->currentText().isEmpty())
    {
        QMessageBox::warning(this, "⚠️ Champ vide", "Tous les champs doivent être remplis !");
        return;
    }

    QString status = ui->status->currentText().trimmed().toLower();
    // allow 'vide', 'plein' or 'broken' (english)
    if (status != "vide" && status != "plein" && status != "broken")
    {
        QMessageBox::warning(this, "⚠️ Statut invalide", "Le statut doit être 'vide', 'plein' ou 'broken' !");
        return;
    }

    int zoneid = ui->zoneid->text().toInt();
    QSqlQuery checkZone;
    checkZone.prepare("SELECT COUNT(*) FROM GZONE WHERE ID_ZONE = :id_zone");
    checkZone.bindValue(":id_zone", zoneid);
    if (!checkZone.exec() || !checkZone.next())
    {
        QMessageBox::critical(this, "❌ Erreur", "Erreur lors de la vérification de la zone !");
        return;
    }
    if (checkZone.value(0).toInt() == 0)
    {
        QMessageBox::warning(this, "⚠️ Zone inexistante", "L'ID de zone n'existe pas dans la table GZONE !");
        return;
    }

    int pubid = ui->pubid->text().toInt();
    int cap = ui->cap->text().toInt();

    Poubelle p(pubid, zoneid, cap, status);
    if (p.ajouter())
        QMessageBox::information(this, "✅ Succès", "Poubelle ajoutée avec succès !");
    else
        QMessageBox::critical(this, "❌ Erreur", "Échec d'ajout de la poubelle !");

    // After adding a bin, refresh bin view for the zone
    reloadBinsCache();
    renderBinsGraph(zoneid);
}

void MainWindow::on_statisticpoubelles_clicked()
{
    // Aggregate stats from GESPUB
    QSqlQuery q;

    // Basic counts
    int totalBins = 0;
    double sumCap = 0.0;
    double avgCap = 0.0;
    double minCap = 0.0, maxCap = 0.0;

    if (q.exec("SELECT COUNT(*), SUM(CAPACITE), AVG(CAPACITE), MIN(CAPACITE), MAX(CAPACITE) FROM GESPUB") && q.next()) {
        totalBins = q.value(0).toInt();
        sumCap = q.value(1).toDouble();
        avgCap = q.value(2).toDouble();
        minCap = q.value(3).toDouble();
        maxCap = q.value(4).toDouble();
    }

    // Status breakdown
    QMap<QString, int> statusCounts;
    if (q.exec("SELECT STATUS, COUNT(*) FROM GESPUB GROUP BY STATUS")) {
        while (q.next()) {
            QString status = q.value(0).toString();
            int cnt = q.value(1).toInt();
            statusCounts[status] = cnt;
        }
    }

    // Per-zone breakdown
    QVector<std::tuple<int,int,double>> perZone; // zoneId, count, sumcap
    if (q.exec("SELECT ID_ZONE, COUNT(*) AS CNT, SUM(CAPACITE) AS SUMCAP FROM GESPUB GROUP BY ID_ZONE ORDER BY CNT DESC")) {
        while (q.next()) {
            int zid = q.value(0).toInt();
            int cnt = q.value(1).toInt();
            double sc = q.value(2).toDouble();
            perZone.push_back({zid, cnt, sc});
        }
    }

    // Build dialog
    QDialog dlg(this);
    dlg.setWindowTitle(tr("Statistiques Poubelles"));
    QVBoxLayout *layout = new QVBoxLayout(&dlg);

    QString header = tr("Total poubelles: %1\nCapacité totale: %2\nCapacité moyenne: %3\nCapacité min: %4 | max: %5")
                         .arg(totalBins)
                         .arg(QString::number(sumCap, 'f', 2))
                         .arg(QString::number(avgCap, 'f', 2))
                         .arg(QString::number(minCap, 'f', 2))
                         .arg(QString::number(maxCap, 'f', 2));

    QPlainTextEdit *summary = new QPlainTextEdit(header);
    summary->setReadOnly(true);
    summary->setStyleSheet("color:black;");
    layout->addWidget(summary);

    // Status breakdown text
    QStringList statusLines;
    for (auto it = statusCounts.cbegin(); it != statusCounts.cend(); ++it) {
        statusLines << QString("%1: %2").arg(it.key()).arg(it.value());
    }
    QPlainTextEdit *statusView = new QPlainTextEdit(statusLines.join("\n"));
    statusView->setReadOnly(true);
    statusView->setStyleSheet("color:black;");
    layout->addWidget(statusView);

    // Table for per-zone breakdown
    QTableWidget *zoneTable = new QTableWidget((int)perZone.size(), 3, &dlg);
    zoneTable->setHorizontalHeaderLabels(QStringList() << tr("Zone") << tr("# Poubelles") << tr("Capacité totale"));
    zoneTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    for (int i = 0; i < (int)perZone.size(); ++i) {
        zoneTable->setItem(i, 0, new QTableWidgetItem(QString::number(std::get<0>(perZone[i]))));
        zoneTable->setItem(i, 1, new QTableWidgetItem(QString::number(std::get<1>(perZone[i]))));
        zoneTable->setItem(i, 2, new QTableWidgetItem(QString::number(std::get<2>(perZone[i]), 'f', 2)));
    }
    zoneTable->setStyleSheet("QTableWidget { color: black; }");
    layout->addWidget(zoneTable);

    dlg.exec();
}

// ==================== CRUD Delete ====================
void MainWindow::on_suppzone_clicked()
{
    int idToDelete = QInputDialog::getInt(this, "Supprimer Zone", "Entrez l'ID de la zone :");

    Zone z;
    if (z.supprimer(idToDelete))
    {
        QMessageBox::information(this, "✅ Succès", "Zone supprimée avec succès !");
        QTableWidget *table = ui->twz;
        for (int row = 0; row < table->rowCount(); ++row)
        {
            QTableWidgetItem *item = table->item(row, 0);
            if (item && item->text().toInt() == idToDelete)
            {
                table->removeRow(row);
                break;
            }
        }
        // Update graph after deletion
        refreshGraph();
    }
    else
        QMessageBox::critical(this, "❌ Erreur", "Échec de la suppression de la zone !");
}

void MainWindow::on_supppoub_clicked()
{
    int idToDelete = ui->pubid->text().toInt();

    Poubelle p;
    if (p.supprimer(idToDelete))
    {
        QMessageBox::information(this, "✅ Succès", "Poubelle supprimée avec succès !");
        QTableWidget *table = ui->twp;
        for (int row = 0; row < table->rowCount(); ++row)
        {
            QTableWidgetItem *item = table->item(row, 0);
            if (item && item->text().toInt() == idToDelete)
            {
                table->removeRow(row);
                break;
            }
        }
        // refresh bins view
        reloadBinsCache();
        renderBinsGraph(ui->zoneid->text().toInt());
    }
    else
        QMessageBox::critical(this, "❌ Erreur", "Échec de la suppression de la poubelle !");
}

// ==================== Display / Affichage ====================
void MainWindow::afficherZones()
{
    Zone z;
    QSqlQueryModel *model = z.afficher();

    ui->twz->setRowCount(0);
    int rows = model->rowCount();
    int cols = model->columnCount();
    ui->twz->setColumnCount(cols);

    for (int j = 0; j < cols; ++j)
        ui->twz->setHorizontalHeaderItem(j, new QTableWidgetItem(model->headerData(j, Qt::Horizontal).toString()));

    for (int i = 0; i < rows; ++i)
    {
        ui->twz->insertRow(i);
        for (int j = 0; j < cols; ++j)
        {
            ui->twz->setItem(i, j, new QTableWidgetItem(model->data(model->index(i, j)).toString()));
        }
    }
    ui->twz->resizeColumnsToContents();
}
void MainWindow::afficherPoubelles()
{
    Poubelle p;
    QSqlQueryModel *model = p.afficher();

    int rows = model->rowCount();
    int cols = model->columnCount();

    ui->twp->clear();
    ui->twp->setRowCount(rows);
    ui->twp->setColumnCount(cols);

    // Set headers
    for (int j = 0; j < cols; ++j)
        ui->twp->setHorizontalHeaderItem(j,
                                         new QTableWidgetItem(model->headerData(j, Qt::Horizontal).toString()));

    // Find USED_PERCENTAGE column index
    int usedColIndex = -1;
    for (int j = 0; j < cols; ++j) {
        QString header = model->headerData(j, Qt::Horizontal).toString().toUpper();
        if (header.contains("USED_PERCENTAGE") || header.contains("USED PERCENTAGE") || header.contains("PERCENTAGE")) {
            usedColIndex = j;
            break;
        }
    }

    // Fill automatically
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            if (j == usedColIndex) {
                // Use progress bar for percentage
                double val = model->data(model->index(i, j)).toDouble();
                QProgressBar *bar = new QProgressBar(ui->twp);
                bar->setRange(0, 100);
                bar->setValue(static_cast<int>(val + 0.5));
                bar->setTextVisible(true);
                bar->setFormat(QString::number(qBound(0, static_cast<int>(val+0.5), 100)) + "%");
                bar->setStyleSheet(
                    "QProgressBar { border: 1px solid #cfd9e6; border-radius: 8px; background: #eeeeee; }"
                    "QProgressBar::chunk { background-color: #8a8a8a; border-radius: 8px; }"
                );
                ui->twp->setCellWidget(i, j, bar);
            } else {
                ui->twp->setItem(i, j,
                                 new QTableWidgetItem(model->data(model->index(i, j)).toString()));
            }
        }
    }

    ui->twp->resizeColumnsToContents();
    // Make sure the IRL button is visible when the poubelles are displayed
    ui->irl->setVisible(true);
    // If IRL dialog is currently open, refresh it so it stays in sync
    if (irlDialog) refreshIrlDialog();
}
// ==================== CRUD Modify ====================
void MainWindow::modZone()
{
    int idToModify = ui->idzone->text().toInt();
    int newPop     = ui->pop->text().toInt();
    double newX    = ui->x->text().toDouble();
    double newY    = ui->y->text().toDouble();
    double newL    = ui->l->text().toDouble();
    double newH    = ui->h->text().toDouble();

    // --- Contrôle : vérifier valeurs ---
    if (newPop <= 0 || newL <= 0 || newH <= 0)
    {
        QMessageBox::warning(this, "⚠️ Données invalides", "Les valeurs numériques doivent être positives !");
        return;
    }

    Zone z(idToModify, newPop, newX, newY, newL, newH);
    if (z.modifier(idToModify))
    {
        QMessageBox::information(this, "✅ Succès", "Zone modifiée avec succès !");
        afficherZones();
        refreshGraph();
    }
    else
        QMessageBox::critical(this, "❌ Erreur", "Échec de modification de la zone !");
}

void MainWindow::modPoubelle()
{
    int idToModify = ui->pubid->text().toInt();
    int newZoneId  = ui->zoneid->text().toInt();
    int newCap     = ui->cap->text().toInt();
    QString newStatus = ui->status->currentText().trimmed().toLower();

    // --- Contrôle de saisie ---
    if (newStatus.isEmpty())
    {
        QMessageBox::warning(this, "⚠️ Champ vide", "Le statut ne peut pas être vide !");
        return;
    }
    if (newStatus != "vide" && newStatus != "plein" && newStatus != "broken")
    {
        QMessageBox::warning(this, "⚠️ Statut invalide", "Le statut doit être 'vide', 'plein' ou 'broken' !");
        return;
    }

    QSqlQuery checkZone;
    checkZone.prepare("SELECT COUNT(*) FROM GZONE WHERE ID_ZONE = :id_zone");
    checkZone.bindValue(":id_zone", newZoneId);
    if (!checkZone.exec() || !checkZone.next() || checkZone.value(0).toInt() == 0)
    {
        QMessageBox::warning(this, "⚠️ Zone inexistante", "L'ID de zone n'existe pas dans la table GZONE !");
        return;
    }

    if (newCap <= 0)
    {
        QMessageBox::warning(this, "⚠️ Capacité invalide", "La capacité doit être positive !");
        return;
    }

    Poubelle p(idToModify, newZoneId, newCap, newStatus);
    if (p.modifier(idToModify))
    {
        QMessageBox::information(this, "✅ Succès", "Poubelle modifiée avec succès !");
        afficherPoubelles();
        reloadBinsCache();
        renderBinsGraph(newZoneId);
    }
    else
        QMessageBox::critical(this, "❌ Erreur", "Échec de modification de la poubelle !");
}
// === CRUD Habitants ===

void MainWindow::on_pushButton_ajoutH_clicked()
{
    int id = ui->lineEdit_id->text().toInt();
    QString nom = ui->lineEdit_nom->text();
    QString prenom = ui->lineEdit_prenom->text();
    QString adresse = ui->lineEdit_adresse->text();
    QString zone = ui->lineEdit_zone->text();
    QString situation = ui->lineEdit_situation->text();
    QString contact = ui->lineEdit_contact->text();
    QString statut = ui->lineEdit_statut->text();
    QString besoins = ui->lineEdit_besoins->text();
    QString satisfaction = ui->lineEdit_satisfaction->text();

    HabitantCRUD h(id, nom, prenom, adresse, zone, situation, contact, statut, besoins, satisfaction);
    QString erreur;
    if (!statutAutorise(statut)) {
        const QString liste = allowedStatuses.join("\n - ");
        QMessageBox::warning(this, "Valeur non autorisée",
                             QString("La valeur de 'statut' n'est pas autorisée.\n\nValeurs permises:\n - %1")
                                 .arg(liste.isEmpty() ? QString("(liste indisponible)") : liste));
        return;
    }
    if (!h.validerChamps(erreur)) {
        QMessageBox::warning(this, "Erreur", erreur);
        return;
    }

    if (h.ajouter()) {
        QMessageBox::information(this, "Succès", "Habitant ajouté !");
        chargerDonneesTable();
        saveLastHabitantId(id);
        loadLastHabitantSelection();
        ui->lineEdit_id->clear();
        ui->lineEdit_nom->clear();
        ui->lineEdit_prenom->clear();
        ui->lineEdit_adresse->clear();
        ui->lineEdit_zone->clear();
        ui->lineEdit_situation->clear();
        ui->lineEdit_contact->clear();
        ui->lineEdit_statut->clear();
        ui->lineEdit_besoins->clear();
        ui->lineEdit_satisfaction->clear();
    } else {
        QMessageBox::critical(this, "Erreur", "Échec de l'ajout.");
    }
}

void MainWindow::on_pushButton_modifH_clicked()
{
    int id = ui->lineEdit_id->text().toInt();
    if (id <= 0) {
        QMessageBox::warning(this, "Erreur", "ID invalide !");
        return;
    }

    QString nom = ui->lineEdit_nom->text();
    QString prenom = ui->lineEdit_prenom->text();
    QString adresse = ui->lineEdit_adresse->text();
    QString zone = ui->lineEdit_zone->text();
    QString situation = ui->lineEdit_situation->text();
    QString contact = ui->lineEdit_contact->text();
    QString statut = ui->lineEdit_statut->text();
    QString besoins = ui->lineEdit_besoins->text();
    QString satisfaction = ui->lineEdit_satisfaction->text();

    HabitantCRUD h(id, nom, prenom, adresse, zone, situation, contact, statut, besoins, satisfaction);
    QString erreur;
    if (!statutAutorise(statut)) {
        const QString liste = allowedStatuses.join("\n - ");
        QMessageBox::warning(this, "Valeur non autorisée",
                             QString("La valeur de 'statut' n'est pas autorisée.\n\nValeurs permises:\n - %1")
                                 .arg(liste.isEmpty() ? QString("(liste indisponible)") : liste));
        return;
    }
    if (!h.validerChamps(erreur)) {
        QMessageBox::warning(this, "Erreur", erreur);
        return;
    }

    if (h.modifier(id)) {
        QMessageBox::information(this, "Succès", "Habitant modifié !");
        chargerDonneesTable();
        saveLastHabitantId(id);
        loadLastHabitantSelection();
    } else {
        QMessageBox::critical(this, "Erreur", "Échec de la modification.");
    }
}

void MainWindow::on_pushButton_suppH_clicked()
{
    int id = ui->lineEdit_id->text().toInt();
    if (id <= 0) {
        QMessageBox::warning(this, "Erreur", "ID invalide !");
        return;
    }

    if (QMessageBox::question(this, "Confirmation", "Supprimer cet habitant ?") == QMessageBox::Yes) {
        HabitantCRUD h;
        if (h.supprimer(id)) {
            QMessageBox::information(this, "Succès", "Habitant supprimé !");
            chargerDonneesTable();
        } else {
            QMessageBox::critical(this, "Erreur", "Échec de la suppression.");
        }
    }
}

void MainWindow::on_pushButton_afficheH_clicked()
{
    chargerDonneesTable();
}

void MainWindow::on_tableView_habitants_clicked(const QModelIndex &index)
{
    if (!index.isValid()) return;
    populateEditsFromRow(index.row());
}

void MainWindow::on_pushButton_rechH_clicked()
{
    rechercherTexte(ui->lineEdit_rechH->text());
}

void MainWindow::on_pushButton_trieH_clicked()
{
    QString critere = ui->comboBox_trieH->currentText();
    QString order;

    if (critere == "Nom" || critere == "nom") order = "NOM ASC";
    else if (critere == "Prénom" || critere == "prenom" || critere == "Prénom ") order = "PRENOM ASC";
    else if (critere == "Adresse" || critere == "adresse") order = "ADRESSE ASC";
    else if (critere == "Satisfaction" || critere == "satisfaction") order = "NIVEAU_SATISFACTION DESC";
    else order = "ID_HABITANT ASC";

    QSqlQueryModel *model = new QSqlQueryModel(this);
    model->setQuery("SELECT ID_HABITANT, NOM, PRENOM, ADRESSE, ZONE_RESIDENCE, SITUATION_FAMILIALE, CONTACT, STATUT, BESOINS_SPECIFIQUES, NIVEAU_SATISFACTION FROM HABITANT ORDER BY " + order);

    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Nom"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Prénom"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Adresse"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Zone"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Situation"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("Contact"));
    model->setHeaderData(7, Qt::Horizontal, QObject::tr("Statut"));
    model->setHeaderData(8, Qt::Horizontal, QObject::tr("Besoins"));
    model->setHeaderData(9, Qt::Horizontal, QObject::tr("Satisfaction"));

    ui->tableView_habitants->setModel(model);
    ui->tableView_habitants->resizeColumnsToContents();
}

// (duplication supprimée) La fonction rechercherTexte est définie plus haut

// === Gestion dynamique du tableau avec bouton ❌ ===
void MainWindow::on_pushButton_30_clicked()
{
    QTableWidget *table = ui->twz;

    int newRow = table->rowCount();
    table->insertRow(newRow);

    int deleteCol = table->columnCount() - 1;
    for (int col = 0; col < deleteCol; ++col)
    {
        table->setItem(newRow, col, new QTableWidgetItem(""));
    }

    QPushButton *btnSupprimer = new QPushButton("❌");
    btnSupprimer->setFixedSize(30, 25);
    btnSupprimer->setStyleSheet(
        "QPushButton { background-color: #b00000; color: white; border-radius: 6px; font-weight: bold; }"
        "QPushButton:hover { background-color: #ff4444; }"
        );

    connect(btnSupprimer, &QPushButton::clicked, this, &MainWindow::supprimerEtDecalerLignes);

    table->setCellWidget(newRow, deleteCol, btnSupprimer);
    table->resizeColumnsToContents();
}

// === Implémente cette fonction dans le .cpp et déclare-la dans le .h ===
// void MainWindow::supprimerEtDecalerLignes()
// {
//     // À implémenter : supprimer la ligne du bouton cliqué
// }

// === Export Habitant Table to PDF ===
void MainWindow::on_exportH_clicked()
{
    QAbstractItemModel *model = ui->tableView_habitants ? ui->tableView_habitants->model() : nullptr;
    if (!model || model->rowCount() == 0) {
        QMessageBox::warning(this, "Export", "Aucune donnée à exporter.");
        return;
    }

    const QString format = ui->formatH ? ui->formatH->currentText().trimmed().toUpper() : QString("A4");
    const QString qualite = ui->qualiteH ? ui->qualiteH->currentText().trimmed().toLower() : QString("standard");
    QString nom = ui->nomfichierH ? ui->nomfichierH->text().trimmed() : QString();
    QString dossier = ui->emplacementH ? ui->emplacementH->text().trimmed() : QString();

    if (nom.isEmpty()) nom = QString("habitants_%1").arg(QDateTime::currentDateTime().toString("yyyyMMdd_HHmmss"));
    if (!nom.endsWith(".pdf", Qt::CaseInsensitive)) nom += ".pdf";

    if (dossier.isEmpty()) {
        dossier = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
    }

    QDir dir(dossier);
    if (!dir.exists()) {
        if (!dir.mkpath(".")) {
            QMessageBox::critical(this, "Export", "Impossible de créer le dossier de destination.");
            return;
        }
    }

    const QString filePath = dir.filePath(nom);

    QPageSize::PageSizeId pageSize = QPageSize::A4;
    if (format == "A3") pageSize = QPageSize::A3;
    else if (format == "A4") pageSize = QPageSize::A4;
    else if (format == "A5") pageSize = QPageSize::A5;

    int dpi = 96;
    if (qualite == "***" || qualite == "haute" || qualite == "high") dpi = 300;
    else if (qualite == "standard") dpi = 150; // un peu mieux que 96 pour lisibilité

    const bool ok = HabitantCRUD::exportModelToPdf(model, filePath, pageSize, dpi, "Liste des Habitants");
    if (ok) {
        QMessageBox::information(this, "Export", QString("Fichier PDF généré:\n%1").arg(filePath));
    } else {
        QMessageBox::critical(this, "Export", "Échec de la génération du PDF.");
    }
}

void MainWindow::on_statistique_clicked()
{
    // Délègue au slot principal de statistiques habitants
    on_statH_clicked();
}

void MainWindow::on_statH_clicked()
{
    // Aller sur la page des statistiques habitants
    if (ui && ui->stackedWidget_2)
        ui->stackedWidget_2->setCurrentIndex(1);

    // Choix de la statistique (adresse / satisfaction)
    QString critere;
    if (ui->statcH)
        critere = ui->statcH->currentText().trimmed().toLower();

    bool parAdresse = critere.startsWith("adresse");
    bool parSatisfaction = critere.startsWith("satisfaction");

    if (!parAdresse && !parSatisfaction)
        parSatisfaction = true; // valeur par défaut

    // 1) Requête SQL d'agrégation
    QSqlQuery query;
    if (parAdresse) {
        query.prepare("SELECT ADRESSE, COUNT(*) FROM HABITANT "
                      "GROUP BY ADRESSE ORDER BY COUNT(*) DESC");
    } else {
        query.prepare("SELECT NIVEAU_SATISFACTION, COUNT(*) FROM HABITANT "
                      "GROUP BY NIVEAU_SATISFACTION ORDER BY COUNT(*) DESC");
    }

    if (!query.exec()) {
        QMessageBox::warning(this, "Statistiques habitants",
                             "Erreur SQL: " + query.lastError().text());
        return;
    }

    // 2) Alimenter la table tablestatH et collecter les données pour le graphique
    QList<QString> labels;
    QList<int> valeurs;

    if (ui->tablestatH) {
        QStandardItemModel *model = new QStandardItemModel(this);
        model->setColumnCount(3);
        model->setHeaderData(0, Qt::Horizontal,
                             parAdresse ? "Adresse" : "Niveau de satisfaction");
        model->setHeaderData(1, Qt::Horizontal, "Nombre d'habitants");
        model->setHeaderData(2, Qt::Horizontal, "Pourcentage (%)");

        // On lit d'abord toutes les lignes pour calculer le total
        QVector<QPair<QString,int>> rows;
        while (query.next()) {
            const QString label = query.value(0).toString();
            const int count = query.value(1).toInt();
            rows.append(qMakePair(label, count));
            labels << label;
            valeurs << count;
        }

        int total = 0;
        for (int v : valeurs)
            total += v;
        if (total <= 0)
            total = 1; // éviter division par zéro

        int row = 0;
        for (const auto &entry : rows) {
            const QString &label = entry.first;
            const int count = entry.second;
            double pct = (100.0 * count) / total;

            model->setItem(row, 0, new QStandardItem(label));
            model->setItem(row, 1, new QStandardItem(QString::number(count)));
            model->setItem(row, 2, new QStandardItem(QString::number(pct, 'f', 1) + " %"));
            ++row;
        }

        ui->tablestatH->setModel(model);
        ui->tablestatH->resizeColumnsToContents();
    } else {
        // Même si la table n'existe pas, il faut consommer la requête pour le graphique
        while (query.next()) {
            labels << query.value(0).toString();
            valeurs << query.value(1).toInt();
        }
    }

    if (labels.isEmpty()) {
        QMessageBox::information(this, "Statistiques habitants",
                                 "Aucune donnée trouvée pour les statistiques.");
        return;
    }

    // Calcul du pourcentage pour chaque catégorie
    double totalPct = 0.0;
    for (int v : valeurs)
        totalPct += v;
    if (totalPct <= 0.0)
        totalPct = 1.0; // éviter division par zéro

    // 3) Création d'une courbe de points (line chart) en pourcentage
    QChart *chart = new QChart();
    chart->setTitle(parAdresse
                        ? "Répartition (en %) des habitants par adresse"
                        : "Répartition (en %) des habitants par niveau de satisfaction");

    QLineSeries *series = new QLineSeries();
    series->setName(parAdresse ? "Adresse" : "Satisfaction");
    series->setColor(QColor("#00AEEF"));
    series->setPointsVisible(true);
    series->setPointLabelsVisible(false);

    // On place les points à x = 0,1,2,... pour chaque catégorie, en % sur l'axe Y
    for (int i = 0; i < valeurs.size(); ++i) {
        double pct = (100.0 * valeurs.at(i)) / totalPct;
        series->append(i, pct);
    }

    // Ligne un peu plus épaisse et marqueurs plus visibles
    QPen pen(series->pen());
    pen.setWidth(2);
    series->setPen(pen);
    series->setMarkerSize(8.0);
    chart->addSeries(series);

    // Axe X numérique aligné avec les indices, plus un axe de catégories pour les labels
    QBarCategoryAxis *axisX = new QBarCategoryAxis();
    axisX->append(labels);
    axisX->setTitleText(parAdresse ? "Adresse" : "Niveau de satisfaction");
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    QValueAxis *axisY = new QValueAxis();
    axisY->setRange(0, 100);              // Pourcentage de 0 à 100 %
    axisY->setTickCount(5);               // 0, 25, 50, 75, 100 (approx.)
    axisY->setLabelFormat("%.0f");
    axisY->setTitleText("Pourcentage d'habitants (%)");
    axisY->setGridLineVisible(true);
    axisY->setGridLineColor(QColor(255, 255, 255, 40));
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);
    chart->setBackgroundVisible(false);

    QChartView *chartViewPct = new QChartView(chart);
    chartViewPct->setRenderHint(QPainter::Antialiasing);
    chartViewPct->setMinimumSize(600, 250);

    chartViewPct->setStyleSheet(
        "QChartView {"
        "    background-color: #1D1047;"
        "    border: 2px solid #382A79;"
        "    border-radius: 5px;"
        "    padding: 10px;"
        "}"
        "QChart {"
        "    background-color: transparent;"
        "    border: none;"
        "}"
        "QLegend {"
        "    background-color: rgba(40, 27, 91, 0.7);"
        "    color: #FFFFFF;"
        "    border: 1px solid #382A79;"
        "    border-radius: 5px;"
        "    padding: 6px;"
        "}"
        );

    // 3bis) Graphique en barres pour les valeurs brutes (nombre d'habitants)
    QChart *chartCount = new QChart();
    chartCount->setTitle(parAdresse
                             ? "Nombre d'habitants par adresse"
                             : "Nombre d'habitants par niveau de satisfaction");

    QBarSeries *seriesCount = new QBarSeries();
    QBarSet *setCount = new QBarSet("Nombre d'habitants");
    for (int v : valeurs)
        *setCount << v;
    setCount->setColor(QColor("#00AEEF"));
    seriesCount->append(setCount);
    chartCount->addSeries(seriesCount);

    QBarCategoryAxis *axisXCount = new QBarCategoryAxis();
    axisXCount->append(labels);
    axisXCount->setTitleText(parAdresse ? "Adresse" : "Niveau de satisfaction");
    chartCount->addAxis(axisXCount, Qt::AlignBottom);
    seriesCount->attachAxis(axisXCount);

    int maxValCount = 0;
    for (int v : valeurs)
        if (v > maxValCount) maxValCount = v;
    if (maxValCount <= 0) maxValCount = 1;

    // Un peu de marge au-dessus de la barre la plus haute
    int upperBound = maxValCount + 1;

    QValueAxis *axisYCount = new QValueAxis();
    axisYCount->setRange(0, upperBound);
    axisYCount->setLabelFormat("%.0f");
    // Nombre de graduations raisonnable (max 6)
    int tickCount = upperBound + 1;
    if (tickCount < 3) tickCount = 3;
    if (tickCount > 6) tickCount = 6;
    axisYCount->setTickCount(tickCount);
    axisYCount->setTitleText("Nombre d'habitants");
    axisYCount->setGridLineVisible(true);
    axisYCount->setGridLineColor(QColor(255, 255, 255, 40));
    chartCount->addAxis(axisYCount, Qt::AlignLeft);
    seriesCount->attachAxis(axisYCount);

    chartCount->legend()->setVisible(true);
    chartCount->legend()->setAlignment(Qt::AlignBottom);
    chartCount->setBackgroundVisible(false);

    QChartView *chartViewCount = new QChartView(chartCount);
    chartViewCount->setRenderHint(QPainter::Antialiasing);
    chartViewCount->setMinimumSize(600, 250);
    chartViewCount->setStyleSheet(chartViewPct->styleSheet());

    // 4) Affichage dans une boîte de dialogue pour ne pas casser la mise en page existante
    QDialog *dlg = new QDialog(this);
    dlg->setWindowTitle(parAdresse
                            ? "Statistiques  habitants par adresse"
                            : "Statistiques  habitants par niveau de satisfaction");
    QVBoxLayout *layout = new QVBoxLayout(dlg);
    layout->addWidget(chartViewPct);
    layout->addWidget(chartViewCount);
    dlg->resize(900, 650);
    dlg->exec();
}

void MainWindow::refreshSatisfactionTable()
{
    QTableView *tv = resolveSatisfactionTableView();
    if (!tv) {
        QMessageBox::warning(this, tr("Satisfaction"),
                             tr("Aucune table de satisfaction (QTableView) trouvée dans l'UI."));
        return;
    }

    // Construire un modèle basé sur une requête SQL directe avec fallback de schéma
    QSqlQueryModel *model = new QSqlQueryModel(tv);
    QSqlQuery q;

    // 1) Essayer avec le schéma TOUHA
    QString sql = QStringLiteral("SELECT ID_SATISFACTION, ID_HABITANT, ID_DEMANDE, SCORE, DATE_SAISIE, SOURCE, COMMENTAIRE "
                                 "FROM SATISFACTION ORDER BY DATE_SAISIE DESC, ID_SATISFACTION DESC");
    if (!q.exec(sql)) {
        // 2) Fallback sans schéma
        sql = QStringLiteral("SELECT ID_SATISFACTION, ID_HABITANT, ID_DEMANDE, SCORE, DATE_SAISIE, SOURCE, COMMENTAIRE "
                             "FROM SATISFACTION ORDER BY DATE_SAISIE DESC, ID_SATISFACTION DESC");
        if (!q.exec(sql)) {
            const QString errText = q.lastError().text().trimmed();
            QMessageBox::warning(this, tr("Satisfaction"),
                                 tr("Impossible de charger les données de satisfaction.\nDernière erreur SQL: %1")
                                     .arg(errText.isEmpty() ? tr("(aucun message renvoyé par le pilote)")
                                                            : errText));
            delete model;
            return;
        }
    }

    model->setQuery(std::move(q));
    model->setHeaderData(0, Qt::Horizontal, tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, tr("Habitant"));
    model->setHeaderData(2, Qt::Horizontal, tr("Demande"));
    model->setHeaderData(3, Qt::Horizontal, tr("Score"));
    model->setHeaderData(4, Qt::Horizontal, tr("Date"));
    model->setHeaderData(5, Qt::Horizontal, tr("Source"));
    model->setHeaderData(6, Qt::Horizontal, tr("Commentaire"));

    tv->setModel(model);
    tv->setSelectionBehavior(QAbstractItemView::SelectRows);
    tv->setSelectionMode(QAbstractItemView::SingleSelection);
    tv->setSortingEnabled(true);
    tv->resizeColumnsToContents();

    // Mettre à jour le KPI global de satisfaction
    updateSatisfactionKpi();
}

QTableView* MainWindow::resolveSatisfactionTableView() const
{
    // Retourner uniquement des vues explicitement nommées pour éviter de polluer d'autres tables
    if (auto tv = findChild<QTableView*>("tableView_satisfaction")) return tv;
    if (auto tv = findChild<QTableView*>("tableView_satisfaction_2")) return tv;
    return nullptr;
}

void MainWindow::updateSatisfactionKpi()
{
    if (!ui)
        return;

    QProgressBar *bar = findChild<QProgressBar*>("progressBar_7");
    if (!bar)
        return;

    // Calculer le score moyen de satisfaction (0..100)
    QSqlQuery q;
    double avgScore = -1.0;
    int count = 0;

    // Essayer d'abord avec le schéma TOUHA, puis sans
    QStringList queries = {
        "SELECT AVG(SCORE), COUNT(*) FROM SATISFACTION",
        "SELECT AVG(SCORE), COUNT(*) FROM SATISFACTION"
    };

    for (const QString &sql : queries) {
        q.clear();
        if (!q.exec(sql))
            continue;
        if (q.next()) {
            avgScore = q.value(0).toDouble();
            count    = q.value(1).toInt();
            if (count > 0)
                break;
        }
    }

    bar->setMaximum(100);

    if (count <= 0 || avgScore < 0.0) {
        bar->setValue(0);
        bar->setFormat(tr("Aucune donnée de satisfaction"));
        bar->setToolTip(QString());
        bar->setStyleSheet(
            "QProgressBar { border: 1px solid #cfd9e6; border-radius: 8px;"
            " background: #ecf0f1; color: #0a1a2f; text-align: center; }"
            "QProgressBar::chunk { background-color: #95a5a6; border-radius: 8px; }"
            );
        return;
    }

    int score = static_cast<int>(avgScore + 0.5);
    if (score < 0) score = 0;
    if (score > 100) score = 100;

    QString color;
    if (score >= 80)
        color = "#27ae60";       // vert
    else if (score >= 60)
        color = "#f39c12";       // orange
    else
        color = "#e74c3c";       // rouge

    bar->setValue(score);
    bar->setFormat(tr("Score global : %1% (%2 réponses)").arg(score).arg(count));
    bar->setToolTip(tr("Moyenne des scores de satisfaction (0..100) sur %1 réponses").arg(count));

    bar->setStyleSheet(
        QString(
            "QProgressBar { border: 1px solid #cfd9e6; border-radius: 8px;"
            " background: #ecf0f1; color: #0a1a2f; text-align: center; }"
            "QProgressBar::chunk { background-color: %1; border-radius: 8px; }"
            ).arg(color));
}

void MainWindow::on_btn_satisfaction_refresh_clicked()
{
    refreshSatisfactionTable();
}

void MainWindow::on_btn_sondage_submit_clicked()
{
    // Vérifier la connexion BD
    QSqlDatabase db = QSqlDatabase::database();
    if (!db.isValid() || !db.isOpen()) {
        QMessageBox mb(this);
        mb.setIcon(QMessageBox::Warning);
        mb.setWindowTitle(tr("Sondage"));
        mb.setTextFormat(Qt::RichText);
        mb.setStyleSheet("QLabel{color:black;} QPushButton{color:black;}");
        mb.setText(tr("<b>Base de données non disponible.</b><br/>Veuillez vérifier la connexion."));
        qDebug() << "Sondage warn:" << "DB not open";
        mb.exec();
        return;
    }

    // Récupérer ID habitant depuis des champs possibles
    int idH = 0;
    if (ui->lineEdit_id_2) idH = ui->lineEdit_id_2->text().trimmed().toInt();
    if (idH <= 0 && ui->lineEdit_id) idH = ui->lineEdit_id->text().trimmed().toInt();
    if (idH <= 0) {
        if (auto le = findChild<QLineEdit*>("IDdhabitant")) idH = le->text().trimmed().toInt();
    }

    // Récupérer score à partir d'un QSpinBox ou QSlider
    int score = -1;
    if (auto spin = findChild<QSpinBox*>("spin_satisfaction_score")) score = spin->value();
    if (score < 0) {
        if (auto slider = findChild<QSlider*>("slider_satisfaction_score")) score = slider->value() * 10;
    }
    // Fallback: scanner tous les QSpinBox et adapter l'échelle (0..10 => *10, 0..5 => *20)
    if (score < 0) {
        const auto spins = findChildren<QSpinBox*>();
        for (QSpinBox *s : spins) {
            if (!s) continue;
            const int minv = s->minimum();
            const int maxv = s->maximum();
            int v = s->value();
            if (minv <= 0 && maxv >= 100) { score = v; break; }
            if (minv <= 0 && maxv == 10)  { score = v * 10; break; }
            if (minv <= 0 && maxv == 5)   { score = v * 20; break; }
        }
    }

    // Commentaire optionnel
    QString commentaire;
    if (auto le = findChild<QLineEdit*>("edit_sondage_commentaire")) commentaire = le->text();
    if (commentaire.isEmpty())
        if (auto te = findChild<QTextEdit*>("edit_sondage_commentaire")) commentaire = te->toPlainText();

    if (idH <= 0) {
        QMessageBox::warning(this, tr("Sondage"), tr("Veuillez saisir/choisir un ID d'habitant valide."));
        return;
    }
    if (score < 0 || score > 100) {
        QMessageBox::warning(this, tr("Sondage"), tr("Score manquant: indiquez une valeur (0..10, 0..5 ou 0..100) dans le contrôle prévu."));
        return;
    }

    // Préchecks explicites pour éviter un message vide
    {
        QSqlQuery qh;
        qh.prepare("SELECT 1 FROM HABITANT WHERE ID_HABITANT = :id");
        qh.bindValue(":id", idH);
        if (!qh.exec() || !qh.next()) {
            const QString raw = qh.lastError().text().trimmed();
            QMessageBox mb(this);
            mb.setIcon(QMessageBox::Warning);
            mb.setWindowTitle(tr("Sondage"));
            mb.setTextFormat(Qt::RichText);
            mb.setStyleSheet("QLabel{color:black;} QPushButton{color:black;}");
            mb.setText(tr("Habitant inexistant (ID %1) ou accès refusé à HABITANT.").arg(idH));
            if (!raw.isEmpty()) mb.setInformativeText(raw);
            qDebug() << "Sondage warn:" << mb.text() << "; info=" << raw;
            mb.exec();
            return;
        }
    }
    {
        QSqlQuery ping;
        if (!ping.exec("SELECT 1 FROM SATISFACTION WHERE 1=0")) {
            const QString raw = ping.lastError().text().trimmed();
            QMessageBox mb(this);
            mb.setIcon(QMessageBox::Warning);
            mb.setWindowTitle(tr("Sondage"));
            mb.setTextFormat(Qt::RichText);
            mb.setStyleSheet("QLabel{color:black;} QPushButton{color:black;}");
            mb.setText(tr("Accès à SATISFACTION impossible (droits?)."));
            if (!raw.isEmpty()) mb.setInformativeText(raw);
            qDebug() << "Sondage warn:" << mb.text() << "; info=" << raw;
            mb.exec();
            return;
        }
    }

    QString err;
    SatisfactionCRUD s(idH, score, 0, "UI", commentaire);
    if (!s.ajouter(err)) {
        const QString raw = err.trimmed();
        QMessageBox mb(this);
        mb.setIcon(QMessageBox::Warning);
        mb.setWindowTitle(tr("Sondage"));
        mb.setTextFormat(Qt::RichText);
        mb.setStyleSheet("QLabel{color:black;} QPushButton{color:black;}");
        mb.setText(tr("Échec d'enregistrement de la réponse de satisfaction."));
        mb.setInformativeText(raw.isEmpty() ? tr("Vérifiez l'existence de l'habitant et la connexion BD.") : raw);
        qDebug() << "Sondage insert failed dialog:" << mb.text() << "; info=" << (raw.isEmpty()? "no-details" : raw);
        mb.exec();
        qDebug() << "Sondage insert failed:" << (raw.isEmpty()? "no-details" : raw);
        return;
    }

    QMessageBox::information(this, tr("Sondage"), tr("Réponse enregistrée."));
    refreshSatisfactionTable();
}

// ==================== CRUD VEHICULE ====================

#include "vehiculecrud.h"

void MainWindow::on_btn_vehicule_ajouter_clicked()
{
    int id = ui->lineEdit_vehicule_id->text().toInt();
    QString type = ui->combo_vehicule_type->currentText();
    QString matricule = ui->lineEdit_vehicule_matricule->text();
    QString capacite = ui->lineEdit_vehicule_capacite->text();
    QString itineraire = ui->lineEdit_vehicule_itineraire->text();
    QString hdep = ui->lineEdit_vehicule_hdep->text();
    QString harr = ui->lineEdit_vehicule_harr->text();
    QString statut = ui->combo_vehicule_statut->currentText();
    QString zone = ui->lineEdit_vehicule_zone->text();
    int id_habitant = ui->lineEdit_vehicule_idhabitant->text().toInt();

    VehiculeCRUD v(id, type, matricule, capacite, itineraire, hdep, harr, statut, zone, id_habitant);
    QString erreur;

    if (!v.validerChamps(erreur)) {
        QMessageBox::warning(this, "Erreur de validation", erreur);
        return;
    }

    if (v.ajouter()) {
        QMessageBox::information(this, "Succès", "Véhicule ajouté !");
        ui->tableView_vehicule->setModel(v.afficher());
    } else {
        QMessageBox::critical(this, "Erreur", "Échec de l'ajout du véhicule.");
    }
}
void MainWindow::on_btn_vehicule_modifier_clicked()
{
    // Vérifier si un ID est sélectionné
    QString idText = ui->lineEdit_vehicule_id->text().trimmed();
    if (idText.isEmpty()) {
        QMessageBox::warning(this, "Avertissement",
                             "⚠️ Veuillez d'abord sélectionner un véhicule dans la table.");
        return;
    }

    int id = idText.toInt();
    if (id <= 0) {
        QMessageBox::warning(this, "Erreur", "ID invalide !");
        return;
    }

    // Récupérer les champs
    QString type = ui->combo_vehicule_type->currentText().trimmed();
    QString matricule = ui->lineEdit_vehicule_matricule->text().trimmed();
    QString capacite = ui->lineEdit_vehicule_capacite->text().trimmed();
    QString itineraire = ui->lineEdit_vehicule_itineraire->text().trimmed();
    QString hdep = ui->lineEdit_vehicule_hdep->text().trimmed();
    QString harr = ui->lineEdit_vehicule_harr->text().trimmed();
    QString statut = ui->combo_vehicule_statut->currentText().trimmed();
    QString zone = ui->lineEdit_vehicule_zone->text().trimmed();
    QString idHabitantText = ui->lineEdit_vehicule_idhabitant->text().trimmed();

    // Vérifier si au moins un champ est renseigné
    if (type.isEmpty() && matricule.isEmpty() && capacite.isEmpty() && itineraire.isEmpty() &&
        hdep.isEmpty() && harr.isEmpty() && statut.isEmpty() && zone.isEmpty() && idHabitantText.isEmpty()) {
        QMessageBox::warning(this, "Avertissement",
                             "⚠️ Aucun champ à modifier. Remplissez au moins un champ avant de cliquer sur Modifier.");
        return;
    }

    int id_habitant = idHabitantText.toInt();

    // Créer l’objet CRUD
    VehiculeCRUD v(id, type, matricule, capacite, itineraire, hdep, harr, statut, zone, id_habitant);

    QString erreur;
    if (!v.validerChamps(erreur)) {
        QMessageBox::warning(this, "Erreur de validation", erreur);
        return;
    }

    // Confirmation
    if (QMessageBox::question(this, "Confirmation",
                              "Voulez-vous vraiment modifier ce véhicule ?",
                              QMessageBox::Yes | QMessageBox::No) == QMessageBox::No) {
        return;
    }

    // Modifier
    if (v.modifier(id)) {
        QMessageBox::information(this, "Succès", "✅ Véhicule modifié avec succès !");
        ui->tableView_vehicule->setModel(v.afficher());
    } else {
        QMessageBox::critical(this, "Erreur", "❌ Échec de la modification.");
    }
}


void MainWindow::on_tableView_vehicule_clicked(const QModelIndex &index)
{
    if (!index.isValid()) return;
    QAbstractItemModel *model = ui->tableView_vehicule->model();
    int row = index.row();

    ui->lineEdit_vehicule_id->setText(model->index(row, 0).data().toString());
    ui->combo_vehicule_type->setCurrentText(model->index(row, 1).data().toString());
    ui->lineEdit_vehicule_matricule->setText(model->index(row, 2).data().toString());
    ui->lineEdit_vehicule_capacite->setText(model->index(row, 3).data().toString());
    ui->lineEdit_vehicule_itineraire->setText(model->index(row, 4).data().toString());
    ui->lineEdit_vehicule_hdep->setText(model->index(row, 5).data().toString());
    ui->lineEdit_vehicule_harr->setText(model->index(row, 6).data().toString());
    ui->combo_vehicule_statut->setCurrentText(model->index(row, 7).data().toString());
    ui->lineEdit_vehicule_zone->setText(model->index(row, 8).data().toString());
    ui->lineEdit_vehicule_idhabitant->setText(model->index(row, 9).data().toString());
}

void MainWindow::on_btn_vehicule_supprimer_clicked()
{
    int id = ui->lineEdit_vehicule_id->text().toInt();

    if (id <= 0) {
        QMessageBox::warning(this, "Erreur", "ID de véhicule invalide !");
        return;
    }

    // ✅ Confirmation avant suppression
    if (QMessageBox::question(this, "Confirmation",
                              QString("Voulez-vous vraiment supprimer le véhicule avec ID %1 ?").arg(id))
        == QMessageBox::Yes)
    {
        VehiculeCRUD v;
        if (v.supprimer(id)) {
            QMessageBox::information(this, "Succès", "Véhicule supprimé avec succès !");
            ui->tableView_vehicule->setModel(v.afficher()); // 🔄 refresh table
            ui->lineEdit_vehicule_id->clear();
        } else {
            QMessageBox::warning(this, "Échec", "Aucun véhicule trouvé avec cet ID ou erreur de suppression.");
        }
    }
}

void MainWindow::on_btn_vehicule_rechercher_clicked()
{
    QString texte = ui->lineEdit_vehicule_recherche->text();
    VehiculeCRUD v;
    ui->tableView_vehicule->setModel(v.rechercher(texte));
}
void MainWindow::on_btn_vehicule_afficher_clicked()
{
    VehiculeCRUD v;
    QSqlQueryModel *model = v.afficher();
    ui->tableView_vehicule->setModel(model);
    ui->tableView_vehicule->resizeColumnsToContents();
}
// Fonction pour afficher les données dans le QTableView
void MainWindow::afficherPersonnel(QTableView *tableView, QSqlQueryModel *model)
{
    if (!tableView || !model) {
        qDebug() << "Erreur: tableView ou model est null";
        return;
    }

    // Définir le modèle pour le QTableView
    tableView->setModel(model);

    // Ajuster la largeur des colonnes
    tableView->resizeColumnsToContents();

    // Activer le tri si nécessaire
    tableView->setSortingEnabled(true);
}

int MainWindow::suggestNextPersonnelId() const
{
    QSqlDatabase db = QSqlDatabase::database();
    if (!db.isOpen())
        return 1;

    int nextId = 1;
    bool found = false;

    auto tryMax = [&](const QString &table) {
        QSqlQuery q;
        q.prepare(QString("SELECT MAX(CIN) FROM %1").arg(table));
        if (!q.exec())
            return false;
        if (!q.next())
            return true;
        QVariant v = q.value(0);
        if (!v.isNull()) {
            bool ok = false;
            int maxId = v.toInt(&ok);
            if (ok && maxId >= 0) {
                nextId = maxId + 1;
                found = true;
            }
        }
        return true;
    };

    if (!tryMax("PERSONNEL")) {
        tryMax("PERSONNELS");
    } else if (!found) {
        tryMax("PERSONNELS");
    }

    return nextId;
}

// Ajouter
void MainWindow::on_ajouterperso_clicked()
{
    QString id = QString::number(suggestNextPersonnelId());
    QString nom = ui->Nom->text().trimmed();
    QString prenom = ui->prenom->text().trimmed();
    QString comp = ui->competence->currentText().trimmed();
    QString zone = ui->Zone->text().trimmed();

    // Validation des champs (l'ID est généré automatiquement)
    if (nom.isEmpty() || prenom.isEmpty() || comp.isEmpty() || zone.isEmpty()) {
        QMessageBox::warning(this, "⚠️ Champ vide", "Tous les champs doivent être remplis !");
        return;
    }

    // Vérifier la connexion à la base de données
    QSqlDatabase db = QSqlDatabase::database();
    if (!db.isOpen()) {
        QMessageBox::critical(this, "❌ Erreur de connexion",
                              "La connexion à la base de données n'est pas active !\n\nVeuillez vérifier votre connexion.");
        return;
    }

    personnel p(id, nom, prenom, comp, zone);
    if (p.ajouter()) {
        afficherPersonnel(ui->tableView_personel, p.afficher());
        QMessageBox::information(this, "✅ Succès", "Personnel ajouté avec succès !");
        // Vider les champs après ajout réussi
        ui->Nom->clear();
        ui->prenom->clear();
        ui->competence->setCurrentIndex(0);
        ui->Zone->clear();
    } else {
        // Récupérer l'erreur SQL réelle
        QSqlQuery testQuery;
        testQuery.prepare("SELECT 1 FROM PERSONNELS WHERE ID_PERSONNEL = :ID");
        testQuery.bindValue(":ID", id);

        QString errorMsg = "Échec d'ajout du personnel !\n\n";

        if (testQuery.exec() && testQuery.next()) {
            errorMsg += QString("L'ID '%1' existe déjà dans la base de données.\nVeuillez utiliser un ID différent.").arg(id);
        } else {
            // Tester si la table existe
            QSqlQuery tableQuery;
            tableQuery.exec("SELECT COUNT(*) FROM PERSONNELS");
            if (tableQuery.lastError().isValid()) {
                QString sqlError = tableQuery.lastError().text();
                errorMsg += QString("Erreur SQL détectée:\n%1\n\n").arg(sqlError);
                errorMsg += "Vérifiez:\n";
                errorMsg += "- Que la table PERSONNELS existe dans la base de données\n";
                errorMsg += "- Que les noms de colonnes sont corrects\n";
                errorMsg += "- Que la connexion à la base de données est active";
            } else {
                errorMsg += "Vérifiez:\n";
                errorMsg += "- Que l'ID n'existe pas déjà\n";
                errorMsg += "- Que tous les champs sont valides\n";
                errorMsg += "- Que la connexion à la base de données est active";
            }
        }

        QMessageBox::critical(this, "❌ Erreur", errorMsg);
    }
}

// Supprimer
void MainWindow::on_Supprimerperso_clicked()
{
    QString id;
    if (ui->tableView_personel && ui->tableView_personel->selectionModel() &&
        ui->tableView_personel->selectionModel()->hasSelection()) {
        int row = ui->tableView_personel->selectionModel()->selectedRows().first().row();
        QAbstractItemModel *model = ui->tableView_personel->model();
        if (model)
            id = model->index(row, 0).data().toString().trimmed();
    }

    // Validation de la sélection
    if (id.isEmpty()) {
        QMessageBox::warning(this, "⚠️ Sélection requise", "Veuillez sélectionner un personnel dans le tableau à supprimer !");
        return;
    }

    // Confirmation avant suppression
    int ret = QMessageBox::question(this, "⚠️ Confirmation",
                                    QString("Êtes-vous sûr de vouloir supprimer le personnel avec l'ID : %1 ?").arg(id),
                                    QMessageBox::Yes | QMessageBox::No);

    if (ret == QMessageBox::Yes) {
        personnel p;
        if (p.supprimer(id)) {
            afficherPersonnel(ui->tableView_personel, p.afficher());
            QMessageBox::information(this, "✅ Succès", "Personnel supprimé avec succès !");
            // Vider les champs après suppression
            ui->Nom->clear();
            ui->prenom->clear();
            ui->competence->setCurrentIndex(0);
            ui->Zone->clear();
        } else {
            // Récupérer l'erreur SQL réelle pour afficher un message plus détaillé
            QString errorMsg = "Échec de suppression !\n\n";

            // Essayer de détecter la colonne CIN
            QSqlQuery detectQuery;
            QString idCol = "CIN";
            if (detectQuery.exec("SELECT * FROM PERSONNEL WHERE ROWNUM = 1")) {
                QSqlRecord record = detectQuery.record();
                for (int i = 0; i < record.count(); i++) {
                    QString col = record.fieldName(i);
                    if (col.toUpper() == "CIN") {
                        idCol = col;
                        break;
                    }
                }
            }

            // Vérifier si le CIN existe
            QSqlQuery checkQuery;
            checkQuery.prepare(QString("SELECT 1 FROM PERSONNEL WHERE %1 = :ID").arg(idCol));
            checkQuery.bindValue(":ID", id);
            if (checkQuery.exec() && !checkQuery.next()) {
                errorMsg += QString("Le CIN '%1' n'existe pas dans la base de données.").arg(id);
            } else {
                errorMsg += "Vérifiez que la connexion à la base de données est active.";
            }

            QMessageBox::critical(this, "❌ Erreur", errorMsg);
        }
    }
}

// Modifier
void MainWindow::on_Modifierperso_clicked()
{
    QString id;
    if (ui->tableView_personel && ui->tableView_personel->selectionModel() &&
        ui->tableView_personel->selectionModel()->hasSelection()) {
        int row = ui->tableView_personel->selectionModel()->selectedRows().first().row();
        QAbstractItemModel *model = ui->tableView_personel->model();
        if (model)
            id = model->index(row, 0).data().toString().trimmed();
    }

    QString nom = ui->Nom->text().trimmed();
    QString prenom = ui->prenom->text().trimmed();
    QString comp = ui->competence->currentText().trimmed();
    QString zone = ui->Zone->text().trimmed();

    // Validation de la sélection
    if (id.isEmpty()) {
        QMessageBox::warning(this, "⚠️ Sélection requise", "Veuillez sélectionner un personnel dans le tableau pour la modification !");
        return;
    }

    if (nom.isEmpty() || prenom.isEmpty() || comp.isEmpty() || zone.isEmpty()) {
        QMessageBox::warning(this, "⚠️ Champ vide", "Tous les champs doivent être remplis pour la modification !");
        return;
    }

    personnel p(id, nom, prenom, comp, zone);
    if (p.modifier()) {
        afficherPersonnel(ui->tableView_personel, p.afficher());
        QMessageBox::information(this, "✅ Succès", "Personnel modifié avec succès !");
    } else {
        // Récupérer l'erreur SQL réelle pour afficher un message plus détaillé
        QString errorMsg = "Échec de modification !\n\n";

        // Essayer de détecter la colonne CIN
        QSqlQuery detectQuery;
        QString idCol = "CIN";
        if (detectQuery.exec("SELECT * FROM PERSONNEL WHERE ROWNUM = 1")) {
            QSqlRecord record = detectQuery.record();
            for (int i = 0; i < record.count(); i++) {
                QString col = record.fieldName(i);
                if (col.toUpper() == "CIN") {
                    idCol = col;
                    break;
                }
            }
        }

        // Vérifier si le CIN existe
        QSqlQuery checkQuery;
        checkQuery.prepare(QString("SELECT 1 FROM PERSONNEL WHERE %1 = :ID").arg(idCol));
        checkQuery.bindValue(":ID", id);
        if (checkQuery.exec() && !checkQuery.next()) {
            errorMsg += QString("Le CIN '%1' n'existe pas dans la base de données.\nVeuillez d'abord ajouter ce personnel.").arg(id);
        } else {
            errorMsg += "Vérifiez que la connexion à la base de données est active.";
        }

        QMessageBox::critical(this, "❌ Erreur", errorMsg);
    }
}

// Afficher
void MainWindow::on_Affperso_clicked()
{
    // Vérifier la connexion
    QSqlDatabase db = QSqlDatabase::database();
    if (!db.isOpen()) {
        QMessageBox::critical(this, "❌ Erreur de connexion",
                              "La connexion à la base de données n'est pas active !\n\nVeuillez vérifier votre connexion.");
        return;
    }

    personnel p;
    QSqlQueryModel *model = p.afficher();
    if (model) {
        // Vérifier s'il y a des erreurs dans le modèle
        if (model->lastError().isValid()) {
            QString error = model->lastError().text();
            QMessageBox::critical(this, "❌ Erreur SQL",
                                  QString("Erreur lors de l'affichage:\n%1\n\nVérifiez que la table PERSONNELS existe dans la base de données.").arg(error));
            return;
        }

        afficherPersonnel(ui->tableView_personel, model);
        QMessageBox::information(this, "✅ Succès", QString("Affichage de %1 personnel(s)").arg(model->rowCount()));
    } else {
        QMessageBox::warning(this, "⚠️ Avertissement", "Aucun personnel trouvé ou erreur lors de l'affichage.");
    }
}

// Slot pour le bouton Modifierperso_2 (affichage)
void MainWindow::on_Modifierperso_2_clicked()
{
    on_Affperso_clicked();
}

// === Recherche, tri et export pour "les metiers basiques" (personnels) ===

void MainWindow::on_pushButton_40_clicked()
{
    // Recherche des personnels via la barre "Rechercher des personnels"
    if (!ui || !ui->tableView_personel)
        return;

    QString texte = ui->lineEdit_19 ? ui->lineEdit_19->text().trimmed() : QString();

    QSqlQuery q;
    QString sql =
        "SELECT CIN, NOM, PRENOM, ZONE_AFFECTATION, DISPONIBILITE, COMPETENCES "
        "FROM PERSONNEL";

    bool hasFilter = !texte.isEmpty();
    bool okInt = false;
    int cinValue = texte.toInt(&okInt);

    if (hasFilter) {
        sql += " WHERE ";
        QStringList conds;
        if (okInt) {
            conds << "CIN = :cin";
        }
        conds << "UPPER(NOM) LIKE UPPER(:tlike)"
              << "UPPER(PRENOM) LIKE UPPER(:tlike)"
              << "UPPER(COMPETENCES) LIKE UPPER(:tlike)"
              << "UPPER(ZONE_AFFECTATION) LIKE UPPER(:tlike)";
        sql += conds.join(" OR ");
    }

    sql += " ORDER BY NOM";
    q.prepare(sql);
    if (hasFilter) {
        if (okInt)
            q.bindValue(":cin", cinValue);
        q.bindValue(":tlike", "%" + texte + "%");
    }

    if (!q.exec()) {
        QMessageBox::warning(this, tr("Recherche personnels"),
                             tr("Erreur SQL lors de la recherche : %1").arg(q.lastError().text()));
        return;
    }

    QSqlQueryModel *model = new QSqlQueryModel(this);
    model->setQuery(std::move(q));
    model->setHeaderData(0, Qt::Horizontal, tr("CIN"));
    model->setHeaderData(1, Qt::Horizontal, tr("Nom"));
    model->setHeaderData(2, Qt::Horizontal, tr("Prénom"));
    model->setHeaderData(3, Qt::Horizontal, tr("Zone"));
    model->setHeaderData(4, Qt::Horizontal, tr("Disponibilité"));
    model->setHeaderData(5, Qt::Horizontal, tr("Compétences"));

    afficherPersonnel(ui->tableView_personel, model);
}

void MainWindow::on_comboBox_10_currentIndexChanged(int)
{
    // Tri des personnels via la combo "TRIER PAR" de l'onglet metiers basiques
    if (!ui || !ui->tableView_personel || !ui->comboBox_10)
        return;

    QString critere = ui->comboBox_10->currentText();
    QString orderCol = "NOM";

    QString c = critere.toLower().trimmed();
    if (c.contains("prenom"))
        orderCol = "PRENOM";
    else if (c.contains("zone"))
        orderCol = "ZONE_AFFECTATION";
    else if (c.contains("dispo"))
        orderCol = "DISPONIBILITE";
    else if (c.contains("compet"))
        orderCol = "COMPETENCES";
    else
        orderCol = "NOM";

    QSqlQueryModel *model = new QSqlQueryModel(this);
    model->setQuery(
        "SELECT CIN, NOM, PRENOM, ZONE_AFFECTATION, DISPONIBILITE, COMPETENCES "
        "FROM PERSONNEL ORDER BY " + orderCol);

    model->setHeaderData(0, Qt::Horizontal, tr("CIN"));
    model->setHeaderData(1, Qt::Horizontal, tr("Nom"));
    model->setHeaderData(2, Qt::Horizontal, tr("Prénom"));
    model->setHeaderData(3, Qt::Horizontal, tr("Zone"));
    model->setHeaderData(4, Qt::Horizontal, tr("Disponibilité"));
    model->setHeaderData(5, Qt::Horizontal, tr("Compétences"));

    afficherPersonnel(ui->tableView_personel, model);
}

void MainWindow::on_pushButton_18_clicked()
{
    // Exporter le tableau des personnels en PDF
    if (!ui || !ui->tableView_personel)
        return;

    QString defaultPath = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
    QString fileName = QFileDialog::getSaveFileName(
        this, tr("Exporter les personnels en PDF"),
        defaultPath + "/personnels.pdf",
        tr("PDF (*.pdf)"));

    if (fileName.isEmpty())
        return;

    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(fileName);
    printer.setPageSize(QPageSize(QPageSize::A4));
    printer.setPageOrientation(QPageLayout::Landscape);

    QPainter painter;
    if (!painter.begin(&printer)) {
        QMessageBox::warning(this, tr("Export PDF"), tr("Impossible de créer le fichier PDF."));
        return;
    }

    painter.setRenderHint(QPainter::Antialiasing);

    // Marge autour du tableau
    QRect rect = painter.viewport();
    int margin = 40;
    QRect drawRect = QRect(rect.left() + margin, rect.top() + margin,
                           rect.width() - 2 * margin, rect.height() - 2 * margin);

    // Ajuster l'échelle pour faire tenir le QTableView dans la page
    QSize tableSize = ui->tableView_personel->size();
    if (tableSize.width() <= 0 || tableSize.height() <= 0) {
        tableSize = QSize(800, 400);
    }
    double xScale = double(drawRect.width()) / tableSize.width();
    double yScale = double(drawRect.height()) / tableSize.height();
    double scale = qMin(xScale, yScale);

    painter.save();
    painter.translate(drawRect.topLeft());
    painter.scale(scale, scale);
    ui->tableView_personel->render(&painter);
    painter.restore();

    painter.end();

    QMessageBox::information(this, tr("Export PDF"),
                             tr("Les personnels ont été exportés dans :\n%1").arg(fileName));
}
/*
void MainWindow::envoyerDonneesArduino(int idHabitant)
{
    if (!arduino) {
        qDebug() << "Arduino n'est pas initialisé";
        return;
    }

    // Vérifier si Arduino est connecté
    if (!arduino->getserial() || !arduino->getserial()->isOpen()) {
        qDebug() << "Tentative de reconnexion à Arduino...";
        if (arduino->connect_arduino() != 0) {
            qDebug() << "Impossible de se connecter à Arduino";
            return;
        }
    }

    // Récupérer le score moyen depuis la base de données
    QSqlQuery q;
    q.prepare("SELECT AVG(SCORE) as SCORE_MOYEN, COUNT(*) as NB_SCORES "
              "FROM SATISFACTION WHERE ID_HABITANT = :id");
    q.bindValue(":id", idHabitant);

    int score = 0;
    if (q.exec() && q.next()) {
        int nbScores = q.value("NB_SCORES").toInt();
        if (nbScores > 0) {
            score = qRound(q.value("SCORE_MOYEN").toDouble());
        } else {
            // Si aucun score trouvé, essayer de récupérer le score depuis les demandes
            // (logique similaire à on_affichersatisfaction_clicked)
            QSqlQuery qDemandes;
            qDemandes.prepare("SELECT STATUT FROM DEMANDE WHERE ID_HABITANT = :id");
            qDemandes.bindValue(":id", idHabitant);
            if (qDemandes.exec()) {
                int total = 0;
                int resolved = 0;
                while (qDemandes.next()) {
                    QString statut = qDemandes.value(0).toString();
                    total++;
                    if (statut == "Cloture") {
                        resolved++;
                    }
                }
                if (total > 0) {
                    score = qRound((double(resolved) / total) * 100.0);
                }
            }
        }
    } else {
        qDebug() << "Erreur lors de la récupération du score:" << q.lastError().text();
        return;
    }

    // Envoyer les données à l'Arduino
    qDebug() << "Envoi à Arduino - ID_HABITANT:" << idHabitant << ", SCORE:" << score;
    arduino->envoyer_habitant_score(idHabitant, score);
}
*/
#include <QMessageBox>
void MainWindow::verifierConnexionArduino()
{
    if (!arduino) {
        arduino = new Arduino();
    }

    if (arduino->connect_arduino() == 0) {
        QMessageBox::information(this, "Arduino",
                                 "✅ Arduino connecté avec succès sur le port :\n" +
                                     arduino->getarduino_port_name());
    } else {
        QMessageBox::warning(this, "Arduino",
                             "❌ Aucun Arduino détecté.\n"
                             "Vérifiez :\n"
                             "• La carte est branchée\n"
                             "• Le câble USB fonctionne\n"
                             "• Le bon pilote est installé (CH340/FTDI)\n"
                             "• L'adresse VID/PID correspond à votre modèle.");
    }
}
void MainWindow::envoyerDonneesArduino(int idHabitant)
{
    if (!arduino) {
        QMessageBox::critical(this, "Erreur",
                              "Arduino n'est pas initialisé.");
        return;
    }

    // Vérifier si Arduino est connecté
    if (!arduino->getserial() || !arduino->getserial()->isOpen()) {
        QMessageBox::warning(this, "Connexion Arduino",
                             "Tentative de reconnexion à Arduino...");

        if (arduino->connect_arduino() != 0) {
            QMessageBox::critical(this, "Erreur",
                                  "Impossible de se connecter à Arduino.");
            return;
        }
    }

    // Récupérer le score moyen depuis la base de données
    QSqlQuery q;
    q.prepare("SELECT AVG(SCORE) as SCORE_MOYEN, COUNT(*) as NB_SCORES "
              "FROM SATISFACTION WHERE ID_HABITANT = :id");
    q.bindValue(":id", idHabitant);

    int score = 0;
    if (q.exec() && q.next()) {
        int nbScores = q.value("NB_SCORES").toInt();
        if (nbScores > 0) {
            score = qRound(q.value("SCORE_MOYEN").toDouble());
        } else {
            // Aucun score : essayer avec les demandes
            QSqlQuery qDemandes;
            qDemandes.prepare("SELECT STATUT FROM SATISFACTION WHERE ID_HABITANT = :id");
            qDemandes.bindValue(":id", idHabitant);

            if (qDemandes.exec()) {
                int total = 0;
                int resolved = 0;
                while (qDemandes.next()) {
                    QString statut = qDemandes.value(0).toString();
                    total++;
                    if (statut == "Cloture") {
                        resolved++;
                    }
                }
                if (total > 0) {
                    score = qRound((double(resolved) / total) * 100.0);
                }
            }
        }
    } else {
        QMessageBox::critical(this, "Erreur SQL",
                              "Erreur lors de la récupération du score :\n" + q.lastError().text());
        return;
    }

    // Envoyer les données à l'Arduino
    QMessageBox::information(this, "Envoi Arduino",
                             "ID Habitant : " + QString::number(idHabitant) +
                                 "\nScore envoyé : " + QString::number(score));

    arduino->envoyer_habitant_score(idHabitant, score);
}

void MainWindow::on_btnEnvoyerArduino_clicked()
{
    // 1. Récupérer l'ID de l'habitant (selon ton interface)
    // Exemple 1 : depuis un QLineEdit
    // Bouton "Lister les ports"
    QStringList ports;
    for (const QSerialPortInfo &info : QSerialPortInfo::availablePorts()) {
        ports << info.portName() + " : " + info.description();
    }
    QMessageBox::information(this, "Ports disponibles", ports.join("\n"));
    bool ok;
    int idHabitant = ui->lineEdit_id->text().toInt(&ok);
    if (!ok || idHabitant <= 0) {
        QMessageBox::warning(this, "ID invalide",
                             "Veuillez entrer un identifiant d'habitant valide (nombre entier > 0).");
        return;
    }

    // Exemple 2 (alternatif) : depuis une table sélectionnée
    // QModelIndex index = ui->tableViewHabitants->currentIndex();
    // if (!index.isValid()) { /* erreur */ }
    // int idHabitant = model->data(model->index(index.row(), 0), Qt::UserRole).toInt();

    // 2. Appeler ta fonction existante
    envoyerDonneesArduino(idHabitant);
}
void MainWindow::on_btnEnvoyerArduino2_clicked()
{
    // 1. Vérifier que l'objet Arduino existe
    if (!arduino) {
        QMessageBox::critical(this, "Erreur", "Arduino non initialisé.");
        return;
    }

    // 2. Vérifier que le port est ouvert — sinon, reconnecter
    if (!arduino->getserial() || !arduino->getserial()->isOpen()) {
        QMessageBox::warning(this, "Arduino", "Reconnexion en cours...");
        if (arduino->connect_arduino() != 0) {
            QMessageBox::critical(this, "Erreur", "Impossible de se connecter à Arduino.");
            return;
        }
        // Attente après connexion
    }

    // ✅ 3. ENVOYER "OK" au format compatible avec ton Arduino
    // Ton Arduino attend "ID:SCORE\n" → on va envoyer un faux couple, ex: "0:0"
    // OU mieux : modifie temporairement ton Arduino pour qu'il affiche "OK" si reçoit "OK\n"

    // 🔹 Option 1 (recommandée) : utilise le format existant → affiche "ID habitant 0", "Score = 0"
    // arduino->envoyer_habitant_score(0, 0);

    // 🔹 Option 2 (meilleure) : envoie directement "OK\n" → et on modifie légèrement l'Arduino (voir ci-dessous)
    arduino->write_to_arduino("OK\n");

    QMessageBox::information(this, "Test", "✅ Commande 'OK' envoyée à l'Arduino.");
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
    ui->stackedWidget_4->setCurrentIndex(0);
    on_btnafficher_clicked();
}

void MainWindow::on_surveilalnce_button_clicked()
{
    ui->stackedWidget_4->setCurrentIndex(1);
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
    ui->COMBO_ZONE->setCurrentIndex(0);  // ✅ Réinitialiser le ComboBox
}
void MainWindow::on_btnAjouter_clicked()
{
    // 1️⃣ Vérification zone
    if(ui->COMBO_ZONE->currentIndex() <= 0) {
        QMessageBox::warning(this, "Attention", "Veuillez sélectionner une zone !");
        return;
    }

    int idZone = ui->COMBO_ZONE->currentData().toInt();
    QString adresse       = ui->ADRESSE->text().trimmed();
    double surface        = ui->SURFACE->text().toDouble();
    int nbPers            = ui->NB_PERSONNES->text().toInt();
    QString typeLog       = ui->TYPE_LOGEMENT->text().trimmed();

    // 2️⃣ Numéro de téléphone
    bool okTel = false;
    QString telephone = QInputDialog::getText(this, "Téléphone",
        "Entrez le numéro de contact :", QLineEdit::Normal, "+216", &okTel);

    if(!okTel || telephone.trimmed().isEmpty()) return;

    // ============================================
    // 3️⃣ RÉCUPÉRER LE NOUVEL ID AVANT INSERTION
    // ============================================
    int nextID = 0;

    QSqlQuery qID;
    qID.exec("SELECT NVL(MAX(ID_HABITAT), 0) + 1 FROM HABITAT");

    if(qID.next()) {
        nextID = qID.value(0).toInt();
    } else {
        QMessageBox::critical(this, "Erreur", "Impossible d’obtenir le prochain ID !");
        return;
    }

    // ============================================
    // 4️⃣ INSERTION AVEC PARAMÈTRES “?” (OBLIGATOIRE)
    // ============================================

    QSqlQuery qInsert;

    qInsert.prepare(
        "INSERT INTO HABITAT (ID_HABITAT, ADRESSE, SURFACE, NB_PERSONNES, "
        "TYPE_LOGEMENT, TELEPHONE_CONTACT, ID_ZONE) "
        "VALUES (?, ?, ?, ?, ?, ?, ?)"
    );

    // IMPORTANT : ordre EXACT = ordre du INSERT
    qInsert.addBindValue(nextID);
    qInsert.addBindValue(adresse);
    qInsert.addBindValue(surface);
    qInsert.addBindValue(nbPers);
    qInsert.addBindValue(typeLog);
    qInsert.addBindValue(telephone);
    qInsert.addBindValue(idZone);

    if(!qInsert.exec()) {
        QMessageBox::critical(this, "Erreur d’insertion",
            "Insertion impossible !\n" + qInsert.lastError().text());
        qDebug() << "ERREUR SQL INSERT :" << qInsert.lastError().text();
        return;
    }

    // ============================================
    // 5️⃣ CONFIRMATION
    // ============================================

    QMessageBox::information(this, "Succès",
        QString("Habitat ajouté avec succès !\n\nID = %1").arg(nextID));

    clearFieldsHabitat();
    on_btnafficher_clicked();
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

    // ✅ Récupérer l'ID de la zone
    QVariant zoneData = ui->COMBO_ZONE->currentData();
    if(!zoneData.isValid() || zoneData.isNull()) {
        QMessageBox::warning(this, "Attention", "⚠️ Veuillez sélectionner une zone !");
        return;
    }
    int idZone = zoneData.toInt();

    // Récupérer l'ancien téléphone
    QSqlQuery getTelQuery;
    getTelQuery.prepare("SELECT TELEPHONE_CONTACT FROM HABITAT WHERE ID_HABITAT=?");
    getTelQuery.addBindValue(id);

    QString ancienTel = "+216";
    if(getTelQuery.exec() && getTelQuery.next()) {
        ancienTel = getTelQuery.value(0).toString();
    }

    bool ok;
    QString telephone = QInputDialog::getText(this,
                                              "Modifier le téléphone",
                                              "Nouveau numéro (format: +216xxxxxxxx):",
                                              QLineEdit::Normal,
                                              ancienTel,
                                              &ok);

    if(!ok || telephone.isEmpty()) {
        QMessageBox::warning(this, "Annulé", "Modification annulée !");
        return;
    }

    if(!telephone.startsWith("+216")) {
        QMessageBox::warning(this, "Attention",
                             "⚠️ Le numéro doit commencer par +216\nExemple: +21620123456");
        return;
    }

    QSqlQuery query;
    query.prepare("UPDATE HABITAT SET ADRESSE=?, SURFACE=?, NB_PERSONNE=?, "
                  "TYPE_LOGEMENT=?, TELEPHONE_CONTACT=?, ID_ZONE=? WHERE ID_HABITAT=?");
    query.addBindValue(adresse);
    query.addBindValue(surface);
    query.addBindValue(nbPersonnes);
    query.addBindValue(typeLogement);
    query.addBindValue(telephone);
    query.addBindValue(idZone);  // ✅ ID de la zone
    query.addBindValue(id);

    if(!query.exec())
        QMessageBox::critical(this,"Erreur","Impossible de modifier: "+query.lastError().text());
    else {
        QMessageBox::information(this, "Succès", "✅ Habitat modifié avec succès !");
        clearFieldsHabitat();
        on_btnafficher_clicked();
    }
}

void MainWindow::afficherHabitats()
{
    ui->TabCrudHabitat->setRowCount(0);

    QSqlQuery query("SELECT ID_HABITAT, ADRESSE, SURFACE, NB_PERSONNE, TYPE_LOGEMENT, TELEPHONE_CONTACT, ID_ZONE FROM HABITAT");

    int row = 0;
    while(query.next()) {
        ui->TabCrudHabitat->insertRow(row);
        for(int col = 0; col < 7; col++) {  // 7 colonnes
            ui->TabCrudHabitat->setItem(row, col, new QTableWidgetItem(query.value(col).toString()));
        }
        row++;
    }
}

void MainWindow::on_TabCrudHabitat_itemClicked(QTableWidgetItem *item)
{
    int row = item->row();
    ui->ID_HABITAT->setText(ui->TabCrudHabitat->item(row, 0)->text());
    ui->ADRESSE->setText(ui->TabCrudHabitat->item(row, 1)->text());
    ui->SURFACE->setText(ui->TabCrudHabitat->item(row, 2)->text());
    ui->NB_PERSONNES->setText(ui->TabCrudHabitat->item(row, 3)->text());
    ui->TYPE_LOGEMENT->setText(ui->TabCrudHabitat->item(row, 4)->text());

    // ✅ Sélectionner la zone dans le ComboBox
    int idZone = ui->TabCrudHabitat->item(row, 6)->text().toInt();

    for(int i = 0; i < ui->COMBO_ZONE->count(); i++) {
        if(ui->COMBO_ZONE->itemData(i).toInt() == idZone) {
            ui->COMBO_ZONE->setCurrentIndex(i);
            break;
        }
    }
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
void MainWindow::chargerZones()
{
    qDebug() << "📍 Chargement des zones...";

    // Nettoyer le ComboBox
    ui->COMBO_ZONE->clear();

    // Ajouter l'élément par défaut
    ui->COMBO_ZONE->addItem("-- Sélectionner une zone --", QVariant());

    // Préparer la requête (seulement ID_ZONE, sans NOM_ZONE)
    QSqlQuery query;
    QString sql = "SELECT ID_ZONE FROM GZONE ORDER BY ID_ZONE";

    if(!query.exec(sql)) {
        qDebug() << "❌ Erreur SQL:" << query.lastError().text();
        QMessageBox::critical(this, "Erreur",
            "Impossible de charger les zones !\n\n" + query.lastError().text());
        return;
    }

    // Parcourir les résultats
    int count = 0;
    while(query.next()) {
        int idZone = query.value(0).toInt();

        // Afficher simplement "Zone X"
        QString displayText = QString("Zone %1").arg(idZone);
        ui->COMBO_ZONE->addItem(displayText, idZone);

        qDebug() << "   ✅ Ajouté:" << displayText;
        count++;
    }

    qDebug() << "✅" << count << "zones chargées dans le ComboBox";

    if(count == 0) {
        QMessageBox::warning(this, "Aucune zone",
            "⚠️ La table GZONE ne contient aucune zone !");
    }
}

// =======================
// CONSOMMATION CRUD
// =======================
// =======================
// CONSOMMATION CRUD
void MainWindow::afficherConsommations()
{
    qDebug() << "========================================";
    qDebug() << "🔄 Affichage des consommations";

    // FORCER le nombre de colonnes
    ui->table_surveillance->setColumnCount(10);
    ui->table_surveillance->clearContents();
    ui->table_surveillance->setRowCount(0);

    // DÉSACTIVER les couleurs alternées
    ui->table_surveillance->setAlternatingRowColors(false);

    // Définir les en-têtes
    QStringList headers;
    headers << "ID_Habitat" << "ID_conso" << "Eau_conso" << "Moy_conso"
            << "Type Anomalie" << "Source" << "SMS Envoyé" << "Localisation"
            << "Date_releve" << "Température";
    ui->table_surveillance->setHorizontalHeaderLabels(headers);

    // Vérifier la connexion
    QSqlDatabase db = QSqlDatabase::database();
    if(!db.isOpen()) {
        qDebug() << "❌ Base non connectée";
        return;
    }

    QSqlQuery query(db);
    QString sql = "SELECT ID_HABITAT, ID_CONSO, EAU_CONSO, MOY_CONSO, "
                  "TYPE_ANOMALIE, SOURCE_DETECTION, "
                  "CASE WHEN SMS_ENVOYE = 1 THEN 'Oui' ELSE 'Non' END AS SMS, "
                  "'' AS LOCALISATION, "
                  "TO_CHAR(DATE_RELEVE, 'DD/MM/YYYY HH24:MI') AS DATE_STR, "
                  "NVL(TEMPERATURE, 0) AS TEMP "
                  "FROM CONSOMMATION "
                  "ORDER BY DATE_RELEVE DESC";

    if(!query.exec(sql)) {
        qDebug() << "❌ Erreur SQL:" << query.lastError().text();
        return;
    }

    int row = 0;
    while(query.next()) {
        ui->table_surveillance->insertRow(row);

        // Récupérer les valeurs importantes
        double consommation = query.value(2).toDouble();  // EAU_CONSO (colonne 2)
        double moyenne = query.value(3).toDouble();       // MOY_CONSO (colonne 3)
        double temperature = query.value(9).toDouble();   // TEMP (colonne 9)
        QString typeAnomalie = query.value(4).toString(); // TYPE_ANOMALIE (colonne 4)

        // =======================
        // COLONNES 0-8 (sans couleur spéciale)
        // =======================
        for(int col = 0; col < 9; col++) {
            QTableWidgetItem *item = new QTableWidgetItem(query.value(col).toString());
            item->setTextAlignment(Qt::AlignCenter);

            // 🎨 COLORIER SEULEMENT LA COLONNE "Type Anomalie" (colonne 4)
            if(col == 4) {  // Type Anomalie
                if(typeAnomalie == "Fuite") {
                    item->setBackground(QColor(255, 200, 200)); // Rouge clair
                    item->setForeground(Qt::darkRed);
                }
                else if(typeAnomalie == "Gaspillage") {
                    item->setBackground(QColor(255, 235, 155)); // Orange clair
                    item->setForeground(Qt::darkYellow);
                }
                else if(typeAnomalie == "Sous-Consommation") {
                    item->setBackground(QColor(200, 230, 255)); // Bleu clair
                    item->setForeground(Qt::darkBlue);
                }
                else { // Normal
                    item->setBackground(QColor(220, 255, 220)); // Vert clair
                    item->setForeground(Qt::darkGreen);
                }
            }
            else if(col == 2) {  // 🎨 COLONNE "Eau_conso" (consommation)
                double ratio = (moyenne > 0) ? (consommation / moyenne) : 1.0;

                if(ratio > 1.5) { // >150%
                    item->setBackground(QColor(255, 245, 200)); // Jaune clair
                    item->setForeground(Qt::darkYellow);
                }
                else if(ratio < 0.7) { // <70%
                    item->setBackground(QColor(230, 240, 255)); // Bleu clair
                    item->setForeground(Qt::darkBlue);
                }
                else {
                    item->setBackground(QColor(240, 255, 240)); // Vert clair
                    item->setForeground(Qt::darkGreen);
                }
            }

            ui->table_surveillance->setItem(row, col, item);
        }

        // =======================
        // 🎨 COLONNE 9 : TEMPÉRATURE (COLORÉE)
        // =======================
        QTableWidgetItem *tempItem = new QTableWidgetItem(
            QString::number(temperature, 'f', 1) + " °C"
        );
        tempItem->setTextAlignment(Qt::AlignCenter);

        // 🎨 COULEUR SELON LA TEMPÉRATURE
        if(temperature < 18.0 && temperature > 0.0) {
            tempItem->setBackground(QColor(173, 216, 230)); // Bleu froid
            tempItem->setForeground(Qt::darkBlue);
        }
        else if(temperature > 30.0) {
            tempItem->setBackground(QColor(255, 182, 193)); // Rouge chaud
            tempItem->setForeground(Qt::darkRed);
        }
        else if(temperature > 0.0) {
            tempItem->setBackground(QColor(144, 238, 144)); // Vert normal
            tempItem->setForeground(Qt::darkGreen);
        }
        else {
            tempItem->setBackground(QColor(240, 240, 240)); // Gris (pas de mesure)
            tempItem->setForeground(Qt::darkGray);
        }

        ui->table_surveillance->setItem(row, 9, tempItem);

        row++;
    }

    // Ajuster la largeur des colonnes
    ui->table_surveillance->resizeColumnsToContents();

    // Ajouter un peu de marge
    for(int col = 0; col < ui->table_surveillance->columnCount(); col++) {
        int width = ui->table_surveillance->columnWidth(col);
        ui->table_surveillance->setColumnWidth(col, width + 10);
    }

    qDebug() << "✅" << row << "lignes affichées";
    qDebug() << "🎨 Colonnes colorées : Type Anomalie, Eau_conso, Température";
    qDebug() << "========================================";
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

    // ⭐ RÉCUPÉRER LA TEMPÉRATURE ARDUINO
    double temperature = derniereTemperature;  // ← Utilisez la variable membre

    // ⚠️ Vérifier que l'Arduino est connecté
    if(!arduino->estConnecte()) {
        QMessageBox::warning(this, "Attention",
            "⚠️ Arduino non connecté !\nLa température sera enregistrée à 0°C.");
        temperature = 0.0;
    }

    // ========================================
    // 🤖 DÉTECTION AUTOMATIQUE DU TYPE D'ANOMALIE
    // ========================================
    QString typeAnomalie = "";
    QString couleurAffichage = "";
    QString icone = "";

    if(eauConso > moyConso * 1.8) {
        typeAnomalie = "Fuite";
        couleurAffichage = "#FF6B6B"; // Rouge
        icone = "🚨";
    }
    else if(eauConso > moyConso * 1.3) {
        typeAnomalie = "Gaspillage";
        couleurAffichage = "#FFA500"; // Orange
        icone = "⚠️";
    }
    else if(eauConso < moyConso * 0.5) {
        typeAnomalie = "Sous-Consommation";
        couleurAffichage = "#87CEEB"; // Bleu clair
        icone = "📉";
    }
    else {
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
        "<tr><td><b>🌡️ Température :</b></td><td><span style='color:#2980b9;'><b>%9 °C</b></span></td></tr>"  // ⭐ AJOUTÉ
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
    .arg(dateReleve)
    .arg(temperature, 0, 'f', 1);  // ⭐ AJOUTÉ

    QMessageBox msgBox(this);
    msgBox.setWindowTitle("Confirmation d'ajout");
    msgBox.setTextFormat(Qt::RichText);
    msgBox.setText(messageConfirmation);
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::Yes);

    msgBox.setStyleSheet(
        "QMessageBox {"
        "   background-color: white;"
        "   min-width: 450px;"  // ← Agrandi pour la température
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
        return;
    }

    // ========================================
    // 💾 INSERTION DANS LA BASE DE DONNÉES
    // ========================================
    QSqlQuery query;
    query.prepare("INSERT INTO CONSOMMATION (ID_HABITAT, EAU_CONSO, MOY_CONSO, "
                  "TYPE_ANOMALIE, DATE_RELEVE, SOURCE_DETECTION, SMS_ENVOYE, TEMPERATURE) "
                  "VALUES (?, ?, ?, ?, TO_DATE(?, 'DD/MM/YYYY HH24:MI'), 'Capteur IoT', 0, ?)");
    query.addBindValue(idHabitat);
    query.addBindValue(eauConso);
    query.addBindValue(moyConso);
    query.addBindValue(typeAnomalie);
    query.addBindValue(dateReleve);
    query.addBindValue(temperature);  // ⭐ Température ajoutée

    if(!query.exec()) {
        QMessageBox::critical(this, "Erreur", "Erreur d'insertion : " + query.lastError().text());
        qDebug() << "❌ Erreur SQL:" << query.lastError().text();
        return;
    }

    qDebug() << "✅ Consommation ajoutée avec température:" << temperature << "°C";

    // ========================================
    // 📱 ENVOI AUTOMATIQUE DU SMS
    // ========================================
    bool fuiteDetectee = (typeAnomalie == "Fuite" || typeAnomalie == "Gaspillage");

    if(fuiteDetectee) {
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
                "Température : %6 °C\n"  // ⭐ AJOUTÉ
                "Date : %7"
            )
            .arg(icone)
            .arg(typeAnomalie.toUpper())
            .arg(idHabitat)
            .arg(eauConso, 0, 'f', 2)
            .arg(moyConso, 0, 'f', 2)
            .arg(temperature, 0, 'f', 1)  // ⭐ AJOUTÉ
            .arg(dateReleve);

            bool smsEnvoye = envoyerSMSTwilio(numeroTelephone, messageSMS);

            if(smsEnvoye) {
                QSqlQuery updateSMS;
                updateSMS.prepare("UPDATE CONSOMMATION SET SMS_ENVOYE = 1 "
                                  "WHERE ID_HABITAT = ? AND DATE_RELEVE = TO_DATE(?, 'DD/MM/YYYY HH24:MI')");
                updateSMS.addBindValue(idHabitat);
                updateSMS.addBindValue(dateReleve);
                updateSMS.exec();

                QMessageBox::information(this, "Succès",
                    QString("✅ Consommation ajoutée avec succès !\n\n"
                            "🌡️ Température : %1 °C\n"
                            "📱 SMS d'alerte envoyé à : %2")
                    .arg(temperature, 0, 'f', 1)
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
                    "Type : %1 %2\n"
                    "🌡️ Température : %3 °C")
            .arg(icone)
            .arg(typeAnomalie)
            .arg(temperature, 0, 'f', 1));
    }

    // ========================================
    // 🔄 ACTUALISATION ET RÉINITIALISATION
    // ========================================
    afficherConsommations();

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



bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
    Q_UNUSED(obj)
    Q_UNUSED(event)
    return QMainWindow::eventFilter(obj, event);
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
    // Nettoyer le texte de recherche
    QString recherche = text.trimmed();

    // Si vide ou placeholder, afficher tout
    if(recherche.isEmpty() || recherche.contains("🔍")) {
        afficherConsommations();
        return;
    }

    // Recherche dynamique
    ui->table_surveillance->setRowCount(0);

    QSqlQuery query;
    query.prepare(
        "SELECT c.ID_HABITAT, c.ID_CONSO, c.EAU_CONSO, c.MOY_CONSO, "
        "c.TYPE_ANOMALIE, c.SOURCE_DETECTION, c.SMS_ENVOYE, "
        "c.LOCALISATION_FUITE, TO_CHAR(c.DATE_RELEVE, 'DD/MM/YYYY HH24:MI') "
        "FROM CONSOMMATION c "
        "WHERE UPPER(c.TYPE_ANOMALIE) LIKE UPPER(:recherche) "
        "   OR UPPER(c.SOURCE_DETECTION) LIKE UPPER(:recherche) "
        "   OR UPPER(c.LOCALISATION_FUITE) LIKE UPPER(:recherche) "
        "   OR TO_CHAR(c.ID_HABITAT) LIKE :recherche "
        "   OR TO_CHAR(c.ID_CONSO) LIKE :recherche "
        "ORDER BY " + critereTri
        );

    query.bindValue(":recherche", "%" + recherche + "%");

    if(!query.exec()) {
        QMessageBox::critical(this, "Erreur", "Erreur recherche: " + query.lastError().text());
        return;
    }

    int row = 0;
    int nbResultats = 0;

    while(query.next()) {
        ui->table_surveillance->insertRow(row);

        // Colonne 0: ID Habitat
        QTableWidgetItem *itemHabitat = new QTableWidgetItem("H" + query.value(0).toString());
        itemHabitat->setTextAlignment(Qt::AlignCenter);
        ui->table_surveillance->setItem(row, 0, itemHabitat);

        // Colonne 1: ID Conso
        QTableWidgetItem *itemConso = new QTableWidgetItem("C" + query.value(1).toString());
        itemConso->setTextAlignment(Qt::AlignCenter);
        ui->table_surveillance->setItem(row, 1, itemConso);

        // Colonne 2: Eau Conso
        QTableWidgetItem *itemEau = new QTableWidgetItem(
            QString::number(query.value(2).toDouble(), 'f', 2) + " m³"
            );
        itemEau->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
        ui->table_surveillance->setItem(row, 2, itemEau);

        // Colonne 3: Moy Conso
        QTableWidgetItem *itemMoy = new QTableWidgetItem(
            QString::number(query.value(3).toDouble(), 'f', 2) + " m³"
            );
        itemMoy->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
        ui->table_surveillance->setItem(row, 3, itemMoy);

        // Colonne 4: Type Anomalie (avec couleurs et highlight)
        QString typeAnomalie = query.value(4).toString();
        QTableWidgetItem *itemType = new QTableWidgetItem(typeAnomalie);
        itemType->setTextAlignment(Qt::AlignCenter);

        // Highlighting si correspond à la recherche
        bool isMatch = typeAnomalie.contains(recherche, Qt::CaseInsensitive);

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

        // Bordure jaune si match
        if(isMatch) {
            itemType->setData(Qt::UserRole, "match");
            QFont font = itemType->font();
            font.setBold(true);
            itemType->setFont(font);
        }

        ui->table_surveillance->setItem(row, 4, itemType);

        // Colonne 5: Source
        QString source = query.value(5).toString();
        QTableWidgetItem *itemSource = new QTableWidgetItem(source);
        itemSource->setTextAlignment(Qt::AlignCenter);
        if(source.contains(recherche, Qt::CaseInsensitive)) {
            QFont font = itemSource->font();
            font.setBold(true);
            itemSource->setFont(font);
            itemSource->setBackground(QColor(255, 255, 150));
        }
        ui->table_surveillance->setItem(row, 5, itemSource);

        // Colonne 6: SMS Envoyé
        QString smsText = (query.value(6).toInt() == 1) ? "✅" : "⏳";
        QTableWidgetItem *itemSMS = new QTableWidgetItem(smsText);
        itemSMS->setTextAlignment(Qt::AlignCenter);
        ui->table_surveillance->setItem(row, 6, itemSMS);

        // Colonne 7: Localisation
        QString loc = query.value(7).toString();
        QTableWidgetItem *itemLoc = new QTableWidgetItem(loc.isEmpty() ? "-" : loc);
        itemLoc->setTextAlignment(Qt::AlignCenter);
        if(!loc.isEmpty() && loc.contains(recherche, Qt::CaseInsensitive)) {
            QFont font = itemLoc->font();
            font.setBold(true);
            itemLoc->setFont(font);
            itemLoc->setBackground(QColor(255, 255, 150));
        }
        ui->table_surveillance->setItem(row, 7, itemLoc);

        // Colonne 8: Date
        QTableWidgetItem *itemDate = new QTableWidgetItem(query.value(8).toString());
        itemDate->setTextAlignment(Qt::AlignCenter);
        ui->table_surveillance->setItem(row, 8, itemDate);

        row++;
        nbResultats++;
    }

    ui->table_surveillance->resizeColumnsToContents();

    // Afficher le nombre de résultats dans la barre de statut (optionnel)
    statusBar()->showMessage(
        QString("🔍 %1 résultat(s) trouvé(s) pour '%2'")
            .arg(nbResultats)
            .arg(recherche),
        3000
        );
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
void MainWindow::on_btnafficher_clicked()
{

    chargerDonneesTable();
}

// ========== SLOT TEMPÉRATURE ==========
void MainWindow::onTemperatureRecue(double temp)
{
    qDebug() << "🌡️ Température reçue:" << temp << "°C";

    derniereTemperature = temp;

    // 1. Afficher dans le label
    if(ui->labeltemperature) {
        ui->labeltemperature->setText(QString::number(temp, 'f', 1) + " °C");
        ui->labeltemperature->setStyleSheet(
            QString("color: %1; font-weight: bold; font-size: 14px;")
            .arg(temp > 30.0 ? "red" : (temp < 18.0 ? "blue" : "green"))
        );
    }

    // 2. Mettre à jour la base de données
    QSqlQuery query;
    query.prepare("UPDATE CONSOMMATION SET TEMPERATURE = :temp "
                  "WHERE ID_CONSO = (SELECT MAX(ID_CONSO) FROM CONSOMMATION)");
    query.bindValue(":temp", temp);

    if(query.exec()) {
        qDebug() << "✅ Température mise à jour dans la base";

        // 3. OPTION A : Rafraîchir UNIQUEMENT la dernière ligne (plus rapide)
        afficherDerniereConsommation();

        // 4. OPTION B : Rafraîchir TOUT le tableau (pour les couleurs)
        // Décommente la ligne suivante si tu veux que TOUTES les couleurs se mettent à jour :
        // afficherConsommations();

    } else {
        qDebug() << "❌ Erreur mise à jour température:" << query.lastError().text();
    }
}

void MainWindow::afficherDerniereConsommation()
{
    if(ui->table_surveillance->rowCount() == 0) {
        afficherConsommations(); // Si tableau vide, le remplir complètement
        return;
    }

    QSqlQuery query;
    query.prepare("SELECT ID_HABITAT, ID_CONSO, EAU_CONSO, MOY_CONSO, "
                  "TYPE_ANOMALIE, SOURCE_DETECTION, "
                  "CASE WHEN SMS_ENVOYE = 1 THEN 'Oui' ELSE 'Non' END AS SMS, "
                  "'' AS LOCALISATION, "
                  "TO_CHAR(DATE_RELEVE, 'DD/MM/YYYY HH24:MI') AS DATE_STR, "
                  "NVL(TEMPERATURE, 0) AS TEMP "
                  "FROM CONSOMMATION "
                  "WHERE ID_CONSO = (SELECT MAX(ID_CONSO) FROM CONSOMMATION)");

    if(query.exec() && query.next()) {
        // Mettre à jour la ligne 0 du tableau
        double consommation = query.value(2).toDouble();
        double moyenne = query.value(3).toDouble();
        double temperature = query.value(9).toDouble();
        QString typeAnomalie = query.value(4).toString();

        // Mettre à jour toutes les cellules de la première ligne
        for(int col = 0; col < 9; col++) {
            QTableWidgetItem *item = ui->table_surveillance->item(0, col);
            if(item) {
                item->setText(query.value(col).toString());

                // 🎨 APPLIQUER LES COULEURS (comme dans afficherConsommations)
                if(col == 4) { // Type Anomalie
                    if(typeAnomalie == "Fuite") {
                        item->setBackground(QColor(255, 200, 200));
                        item->setForeground(Qt::darkRed);
                    }
                    else if(typeAnomalie == "Gaspillage") {
                        item->setBackground(QColor(255, 235, 155));
                        item->setForeground(Qt::darkYellow);
                    }
                    else if(typeAnomalie == "Sous-Consommation") {
                        item->setBackground(QColor(200, 230, 255));
                        item->setForeground(Qt::darkBlue);
                    }
                    else {
                        item->setBackground(QColor(220, 255, 220));
                        item->setForeground(Qt::darkGreen);
                    }
                }
                else if(col == 2) { // Eau_conso
                    double ratio = (moyenne > 0) ? (consommation / moyenne) : 1.0;
                    if(ratio > 1.5) {
                        item->setBackground(QColor(255, 245, 200));
                        item->setForeground(Qt::darkYellow);
                    }
                    else if(ratio < 0.7) {
                        item->setBackground(QColor(230, 240, 255));
                        item->setForeground(Qt::darkBlue);
                    }
                    else {
                        item->setBackground(QColor(240, 255, 240));
                        item->setForeground(Qt::darkGreen);
                    }
                }
            }
        }

        // 🎨 Mettre à jour la cellule température
        QTableWidgetItem *tempItem = ui->table_surveillance->item(0, 9);
        if(tempItem) {
            tempItem->setText(QString::number(temperature, 'f', 1) + " °C");

            // Appliquer la couleur température
            if(temperature < 18.0 && temperature > 0.0) {
                tempItem->setBackground(QColor(173, 216, 230));
                tempItem->setForeground(Qt::darkBlue);
            }
            else if(temperature > 30.0) {
                tempItem->setBackground(QColor(255, 182, 193));
                tempItem->setForeground(Qt::darkRed);
            }
            else if(temperature > 0.0) {
                tempItem->setBackground(QColor(144, 238, 144));
                tempItem->setForeground(Qt::darkGreen);
            }
        }

        qDebug() << "✅ Dernière consommation rafraîchie avec couleurs";
    }
}

void MainWindow::on_irl_clicked()
{
    // ══════════════════════════════════════════════════════════════════════════
    // 🎨 MODERN IRL CAPTURE DIALOG - Smart City Waste Management
    // ══════════════════════════════════════════════════════════════════════════
    
    // Create the dialog once and store pointer in MainWindow so it can be refreshed
    if (irlDialog) {
        // Already open, bring it to front
        irlDialog->raise();
        irlDialog->activateWindow();
        return;
    }
    QDialog *dlg = new QDialog(this);
    dlg->setWindowTitle(tr("🗑️ IRL Capture - Smart Bins Monitor"));
    dlg->setMinimumSize(900, 700);
    dlg->setModal(true);
    
    // ══════════════════════════════════════════════════════════════════════════
    // 🎨 MODERN DARK THEME STYLESHEET
    // ══════════════════════════════════════════════════════════════════════════
    dlg->setStyleSheet(R"(
        QDialog {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:1,
                stop:0 #1a1a2e, stop:0.5 #16213e, stop:1 #0f3460);
            border-radius: 15px;
        }
        QLabel {
            color: #eaeaea;
            font-family: 'Segoe UI', Arial;
        }
        QLabel#titleLabel {
            font-size: 24px;
            font-weight: bold;
            color: #00d9ff;
            padding: 10px;
        }
        QLabel#subtitleLabel {
            font-size: 14px;
            color: #a0a0a0;
            padding-bottom: 10px;
        }
        QTableWidget {
            background-color: rgba(30, 30, 50, 200);
            alternate-background-color: rgba(40, 40, 70, 200);
            border: 2px solid #3a3a5a;
            border-radius: 10px;
            gridline-color: #3a3a5a;
            color: #ffffff;
            font-size: 12px;
            selection-background-color: #00d9ff;
            selection-color: #000000;
        }
        QTableWidget::item {
            padding: 8px;
            border-bottom: 1px solid #3a3a5a;
        }
        QHeaderView::section {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #4a4a7a, stop:1 #3a3a5a);
            color: #00d9ff;
            font-weight: bold;
            font-size: 13px;
            padding: 10px;
            border: none;
            border-bottom: 2px solid #00d9ff;
        }
        QPushButton {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #00d9ff, stop:1 #0099cc);
            color: #000000;
            border: none;
            border-radius: 8px;
            padding: 8px 16px;
            font-weight: bold;
            font-size: 12px;
        }
        QPushButton:hover {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #33e0ff, stop:1 #00bbee);
        }
        QPushButton:pressed {
            background: #0077aa;
        }
        QPushButton#refreshBtn {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #00ff88, stop:1 #00cc66);
            color: #000000;
        }
        QPushButton#refreshBtn:hover {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #33ffaa, stop:1 #00ee77);
        }
        QPushButton#closeBtn {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #ff4444, stop:1 #cc0000);
            color: #ffffff;
        }
        QPushButton#closeBtn:hover {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #ff6666, stop:1 #ee2222);
        }
        QTextEdit#debugConsole {
            background-color: rgba(0, 0, 0, 220);
            color: #00ff00;
            font-family: 'Consolas', 'Courier New', monospace;
            font-size: 11px;
            border: 2px solid #00ff00;
            border-radius: 8px;
            padding: 8px;
        }
        QGroupBox {
            background-color: rgba(40, 40, 70, 150);
            border: 2px solid #4a4a7a;
            border-radius: 10px;
            margin-top: 15px;
            font-weight: bold;
            color: #00d9ff;
        }
        QGroupBox::title {
            subcontrol-origin: margin;
            subcontrol-position: top left;
            padding: 5px 15px;
            background: #3a3a5a;
            border-radius: 5px;
        }
        QFrame#statusFrame {
            background: rgba(0, 217, 255, 30);
            border: 2px solid #00d9ff;
            border-radius: 10px;
            padding: 10px;
        }
        QLabel#arduinoStatus {
            font-size: 14px;
            font-weight: bold;
        }
    )");
    
    QVBoxLayout *mainLayout = new QVBoxLayout(dlg);
    mainLayout->setSpacing(15);
    mainLayout->setContentsMargins(20, 20, 20, 20);
    
    // ══════════════════════════════════════════════════════════════════════════
    // 🏷️ HEADER SECTION
    // ══════════════════════════════════════════════════════════════════════════
    QLabel *titleLabel = new QLabel(tr("🗑️ Smart Bins IRL Monitor"));
    titleLabel->setObjectName("titleLabel");
    titleLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(titleLabel);
    
    QLabel *subtitleLabel = new QLabel(tr("Real-time waste bin monitoring with Arduino PIR sensor integration"));
    subtitleLabel->setObjectName("subtitleLabel");
    subtitleLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(subtitleLabel);
    
    // ══════════════════════════════════════════════════════════════════════════
    // 📡 ARDUINO CONNECTION STATUS
    // ══════════════════════════════════════════════════════════════════════════
    QFrame *statusFrame = new QFrame();
    statusFrame->setObjectName("statusFrame");
    QHBoxLayout *statusLayout = new QHBoxLayout(statusFrame);
    
    QLabel *arduinoIcon = new QLabel("🔌");
    arduinoIcon->setStyleSheet("font-size: 24px;");
    statusLayout->addWidget(arduinoIcon);
    
    QLabel *arduinoStatus = new QLabel();
    arduinoStatus->setObjectName("arduinoStatus");
    
    // Check Arduino connection status
    bool arduinoConnected = (Poubelle::pirSerial != nullptr && Poubelle::pirSerial->isOpen());
    if (arduinoConnected) {
        arduinoStatus->setText(tr("✅ Arduino Connected - Port: %1").arg(Poubelle::pirSerial->portName()));
        arduinoStatus->setStyleSheet("color: #00ff88;");
    } else {
        arduinoStatus->setText(tr("❌ Arduino Disconnected - No PIR sensor detected"));
        arduinoStatus->setStyleSheet("color: #ff4444;");
    }
    statusLayout->addWidget(arduinoStatus);
    statusLayout->addStretch();
    
    // Reconnect button
    QPushButton *reconnectBtn = new QPushButton(tr("🔄 Reconnect"));
    reconnectBtn->setFixedWidth(120);
    connect(reconnectBtn, &QPushButton::clicked, [this, arduinoStatus]() {
        Poubelle::setupPIRSerial(this);
        bool connected = (Poubelle::pirSerial != nullptr && Poubelle::pirSerial->isOpen());
        if (connected) {
            arduinoStatus->setText(tr("✅ Arduino Connected - Port: %1").arg(Poubelle::pirSerial->portName()));
            arduinoStatus->setStyleSheet("color: #00ff88;");
        } else {
            arduinoStatus->setText(tr("❌ Arduino Disconnected - Connection failed"));
            arduinoStatus->setStyleSheet("color: #ff4444;");
        }
    });
    statusLayout->addWidget(reconnectBtn);
    
    mainLayout->addWidget(statusFrame);
    
    // ══════════════════════════════════════════════════════════════════════════
    // 📊 BINS TABLE WITH BATTERY-STYLE PROGRESS
    // ══════════════════════════════════════════════════════════════════════════
    QGroupBox *binsGroup = new QGroupBox(tr("📊 Waste Bins Status"));
    QVBoxLayout *binsLayout = new QVBoxLayout(binsGroup);
    
    QSqlQuery q;
    if (!q.exec("SELECT ID_BIN, ID_ZONE, CAPACITE, STATUS, COALESCE(USED_PERCENTAGE,0) FROM GESPUB ORDER BY ID_BIN")) {
        QMessageBox::warning(this, tr("Erreur"), tr("Impossible de lire les poubelles: %1").arg(q.lastError().text()));
        delete dlg;
        return;
    }

    QVector<QVector<QString>> binsList;
    while (q.next()) {
        QVector<QString> row;
        row.append(q.value(0).toString()); // id
        row.append(q.value(1).toString()); // zone
        row.append(q.value(2).toString()); // cap
        row.append(q.value(3).toString()); // status
        row.append(q.value(4).toString()); // used
        binsList.append(row);
    }

    // Create a dedicated table for the dialog and keep a pointer in MainWindow
    QTableWidget *binTable = new QTableWidget(binsList.isEmpty() ? 1 : (int)binsList.size(), 6, dlg);
    irlBinTable = binTable; // store pointer for refresh
    binTable->setObjectName("binTable");
    binTable->setHorizontalHeaderLabels(QStringList() << tr("🆔 ID") << tr("📍 Zone") << tr("📦 Capacité") << tr("📋 Status") << tr("🔋 Remplissage") << tr("🎯 Action"));
    binTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    binTable->setAlternatingRowColors(true);
    binTable->verticalHeader()->setVisible(false);
    binTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    binTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    
    if (binsList.isEmpty()) {
        binTable->setItem(0, 0, new QTableWidgetItem(tr("Aucune poubelle disponible")));
        binTable->setSpan(0, 0, 1, 6);
    } else {
        for (int i = 0; i < (int)binsList.size(); ++i) {
            // ID
            QTableWidgetItem *idItem = new QTableWidgetItem(binsList[i][0]);
            idItem->setTextAlignment(Qt::AlignCenter);
            idItem->setFont(QFont("Segoe UI", 11, QFont::Bold));
            binTable->setItem(i, 0, idItem);
            
            // Zone
            QTableWidgetItem *zoneItem = new QTableWidgetItem(binsList[i][1]);
            zoneItem->setTextAlignment(Qt::AlignCenter);
            binTable->setItem(i, 1, zoneItem);
            
            // Capacité
            QTableWidgetItem *capItem = new QTableWidgetItem(binsList[i][2] + " L");
            capItem->setTextAlignment(Qt::AlignCenter);
            binTable->setItem(i, 2, capItem);
            
            // Status avec icône
            QString statusText = binsList[i][3];
            QString statusIcon = "⚪";
            QString statusColor = "#888888";
            if (statusText == "VIDE") { statusIcon = "🟢"; statusColor = "#00ff88"; }
            else if (statusText == "PLEIN") { statusIcon = "🔴"; statusColor = "#ff4444"; }
            else if (statusText == "BROKEN" || statusText == "ENDOMMAGE") { statusIcon = "🟡"; statusColor = "#ffaa00"; }
            
            QTableWidgetItem *statusItem = new QTableWidgetItem(statusIcon + " " + statusText);
            statusItem->setTextAlignment(Qt::AlignCenter);
            statusItem->setForeground(QColor(statusColor));
            binTable->setItem(i, 3, statusItem);
            
            // ══════════════════════════════════════════════════════════════════
            // 🔋 BATTERY-STYLE PROGRESS BAR
            // ══════════════════════════════════════════════════════════════════
            int val = (int)binsList[i][4].toDouble();
            
            // Create battery-style widget
            QWidget *batteryWidget = new QWidget();
            QHBoxLayout *batteryLayout = new QHBoxLayout(batteryWidget);
            batteryLayout->setContentsMargins(5, 5, 5, 5);
            batteryLayout->setSpacing(5);
            
            // Battery container
            QFrame *batteryFrame = new QFrame();
            batteryFrame->setFixedSize(100, 30);
            
            // Color based on fill level (like battery)
            QString fillColor;
            QString glowColor;
            if (val < 30) {
                fillColor = "#00ff88";  // Green - empty/low
                glowColor = "rgba(0, 255, 136, 0.5)";
            } else if (val < 60) {
                fillColor = "#ffcc00";  // Yellow - medium
                glowColor = "rgba(255, 204, 0, 0.5)";
            } else if (val < 85) {
                fillColor = "#ff8800";  // Orange - high
                glowColor = "rgba(255, 136, 0, 0.5)";
            } else {
                fillColor = "#ff3333";  // Red - almost full
                glowColor = "rgba(255, 51, 51, 0.5)";
            }
            
            batteryFrame->setStyleSheet(QString(R"(
                QFrame {
                    background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                        stop:0 #2a2a4a, stop:0.5 #1a1a3a, stop:1 #2a2a4a);
                    border: 2px solid #5a5a8a;
                    border-radius: 6px;
                }
            )"));
            
            // Inner fill (the actual battery level)
            QFrame *fillFrame = new QFrame(batteryFrame);
            int fillWidth = qMax(4, (int)(90.0 * val / 100.0));
            fillFrame->setGeometry(4, 4, fillWidth, 22);
            fillFrame->setStyleSheet(QString(R"(
                QFrame {
                    background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                        stop:0 %1, stop:0.5 %1, stop:1 %1);
                    border-radius: 4px;
                    border: none;
                }
            )").arg(fillColor));
            
            // Percentage label
            QLabel *pctLabel = new QLabel(QString::number(val) + "%");
            pctLabel->setStyleSheet(QString("color: %1; font-weight: bold; font-size: 12px;").arg(fillColor));
            pctLabel->setAlignment(Qt::AlignCenter);
            
            batteryLayout->addWidget(batteryFrame);
            batteryLayout->addWidget(pctLabel);
            batteryLayout->addStretch();
            
            binTable->setCellWidget(i, 4, batteryWidget);
            
            // ══════════════════════════════════════════════════════════════════
            // 🎯 IRL ACTION WIDGET (PIR + Vider)
            // ══════════════════════════════════════════════════════════════════
            QWidget *actionWidget = new QWidget();
            QHBoxLayout *actionLayout = new QHBoxLayout(actionWidget);
            actionLayout->setContentsMargins(0, 0, 0, 0);
            actionLayout->setSpacing(6);

            QPushButton *pirBtn = new QPushButton(tr("🔴 PIR"));
            pirBtn->setProperty("bin_id", binsList[i][0]);
            pirBtn->setCursor(Qt::PointingHandCursor);
            connect(pirBtn, &QPushButton::clicked, this, [this, pirBtn]() {
                bool ok;
                int binId = pirBtn->property("bin_id").toInt(&ok);
                if (!ok) return;
                showBigBatteryDialog(binId);
            });

            QPushButton *viderBtn = new QPushButton(tr("🧹 Vider"));
            viderBtn->setProperty("bin_id", binsList[i][0]);
            viderBtn->setCursor(Qt::PointingHandCursor);
            connect(viderBtn, &QPushButton::clicked, this, [this, viderBtn]() {
                bool ok;
                int binId = viderBtn->property("bin_id").toInt(&ok);
                if (!ok) return;
                // Create temporary Poubelle object to update DB
                Poubelle p;
                if (p.modifierUsedPercentage(binId, 0.0)) {
                    QMetaObject::invokeMethod(this, "appendIrlDebug", Qt::QueuedConnection,
                                              Q_ARG(QString, QString("🧹 Vider: poubelle %1 vidée (0%%, statut VIDE)").arg(binId)));
                    QMetaObject::invokeMethod(this, "refreshIrlDialog", Qt::QueuedConnection);
                } else {
                    QMetaObject::invokeMethod(this, "appendIrlDebug", Qt::QueuedConnection,
                                              Q_ARG(QString, QString("❌ Échec vidage poubelle %1").arg(binId)));
                }
            });

            actionLayout->addWidget(pirBtn);
            actionLayout->addWidget(viderBtn);
            actionLayout->addStretch();
            binTable->setCellWidget(i, 5, actionWidget);
        }
    }
    
    binTable->resizeRowsToContents();
    binsLayout->addWidget(binTable);
    mainLayout->addWidget(binsGroup);
    
    // ══════════════════════════════════════════════════════════════════════════
    // 🖥️ DEBUG CONSOLE FOR ARDUINO MESSAGES
    // ══════════════════════════════════════════════════════════════════════════
    QGroupBox *debugGroup = new QGroupBox(tr("🖥️ Arduino Debug Console"));
    QVBoxLayout *debugLayout = new QVBoxLayout(debugGroup);
    
    QTextEdit *debugConsole = new QTextEdit();
    irlDebugConsole = debugConsole; // store pointer for refresh/logs
    debugConsole->setObjectName("debugConsole");
    debugConsole->setReadOnly(true);
    debugConsole->setMaximumHeight(150);
    debugConsole->setPlaceholderText(tr("Arduino serial messages will appear here..."));
    
    // Add initial connection info
    QString initMsg = QString("[%1] === IRL Monitor Started ===\n").arg(QDateTime::currentDateTime().toString("hh:mm:ss"));
    if (arduinoConnected) {
        initMsg += QString("[%1] ✅ Arduino connected on port: %2\n")
            .arg(QDateTime::currentDateTime().toString("hh:mm:ss"))
            .arg(Poubelle::pirSerial->portName());
        initMsg += QString("[%1] 📡 Waiting for PIR sensor data...\n")
            .arg(QDateTime::currentDateTime().toString("hh:mm:ss"));
    } else {
        initMsg += QString("[%1] ❌ No Arduino connection - Click 'Reconnect' to try again\n")
            .arg(QDateTime::currentDateTime().toString("hh:mm:ss"));
    }
    debugConsole->setText(initMsg);
    
    debugLayout->addWidget(debugConsole);
    mainLayout->addWidget(debugGroup);
    
    // ══════════════════════════════════════════════════════════════════════════
    // 🔘 BUTTON BAR
    // ══════════════════════════════════════════════════════════════════════════
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->setSpacing(15);
    
    QPushButton *refreshBtn = new QPushButton(tr("🔄 Refresh Data"));
    refreshBtn->setObjectName("refreshBtn");
    refreshBtn->setFixedHeight(40);
    connect(refreshBtn, &QPushButton::clicked, [this, dlg]() {
        dlg->accept();
        on_irl_clicked(); // Reopen with fresh data
    });
    buttonLayout->addWidget(refreshBtn);
    
    QPushButton *testBtn = new QPushButton(tr("🧪 Test All Bins"));
    testBtn->setFixedHeight(40);
    connect(testBtn, &QPushButton::clicked, [this, debugConsole]() {
        debugConsole->append(QString("[%1] 🧪 Testing all bins...")
            .arg(QDateTime::currentDateTime().toString("hh:mm:ss")));
        
        QSqlQuery testQ;
        testQ.exec("SELECT ID_BIN FROM GESPUB");
        while (testQ.next()) {
            int bid = testQ.value(0).toInt();
            debugConsole->append(QString("[%1] → Testing bin #%2")
                .arg(QDateTime::currentDateTime().toString("hh:mm:ss"))
                .arg(bid));
        }
        debugConsole->append(QString("[%1] ✅ Test complete")
            .arg(QDateTime::currentDateTime().toString("hh:mm:ss")));
    });
    buttonLayout->addWidget(testBtn);
    
    buttonLayout->addStretch();
    
    QPushButton *closeBtn = new QPushButton(tr("✖ Close"));
    closeBtn->setObjectName("closeBtn");
    closeBtn->setFixedHeight(40);
    closeBtn->setFixedWidth(120);
    connect(closeBtn, &QPushButton::clicked, dlg, &QDialog::accept);
    buttonLayout->addWidget(closeBtn);
    
    mainLayout->addLayout(buttonLayout);
    
    // ══════════════════════════════════════════════════════════════════════════
    // 📡 LIVE SERIAL DATA MONITORING (if connected)
    // ══════════════════════════════════════════════════════════════════════════
    if (arduinoConnected && Poubelle::pirSerial) {
        // Create a timer to poll for new serial data
        QTimer *serialTimer = new QTimer(dlg);
        connect(serialTimer, &QTimer::timeout, [debugConsole]() {
            if (Poubelle::pirSerial && Poubelle::pirSerial->canReadLine()) {
                QByteArray data = Poubelle::pirSerial->readLine().trimmed();
                QString msg = QString::fromUtf8(data);
                if (!msg.isEmpty()) {
                    QString timestamp = QDateTime::currentDateTime().toString("hh:mm:ss");
                    QString formatted;
                    if (msg.toUpper().contains("MOTION")) {
                        formatted = QString("[%1] 🔴 MOTION DETECTED: %2").arg(timestamp).arg(msg);
                    } else if (msg.toUpper().contains("BIN")) {
                        formatted = QString("[%1] 📦 BIN DATA: %2").arg(timestamp).arg(msg);
                    } else {
                        formatted = QString("[%1] 📡 %2").arg(timestamp).arg(msg);
                    }
                    debugConsole->append(formatted);
                    // Auto-scroll to bottom
                    debugConsole->verticalScrollBar()->setValue(debugConsole->verticalScrollBar()->maximum());
                }
            }
        });
        serialTimer->start(100); // Poll every 100ms
    }
    // Keep pointer on the MainWindow so handlePIRMotion can request a refresh
    irlDialog = dlg;
    connect(dlg, &QDialog::finished, this, [this](int){
        // clear pointers when dialog closed
        irlDialog = nullptr;
        irlBinTable = nullptr;
        irlDebugConsole = nullptr;
    });
    // Ensure dialog shows current data immediately
    refreshIrlDialog();
    // Execute the dialog (modal). It will process events so queued refreshes will still work.
    dlg->exec();
    delete dlg;
}

void MainWindow::refreshIrlDialog()
{
    if (!irlDialog || !irlDialog->isVisible() || !irlBinTable)
        return;
    // Re-fetch data and repopulate the dialog's table similar to afficherPoubelles
    QSqlQuery q;
    if (!q.exec("SELECT ID_BIN, ID_ZONE, CAPACITE, STATUS, COALESCE(USED_PERCENTAGE,0) FROM GESPUB ORDER BY ID_BIN")) {
        if (irlDebugConsole) irlDebugConsole->append(QString("[%1] ❌ Failed to reload bins: %2").arg(QDateTime::currentDateTime().toString("hh:mm:ss")).arg(q.lastError().text()));
        return;
    }
    QVector<QVector<QString>> binsList;
    while (q.next()) {
        QVector<QString> row;
        row.append(q.value(0).toString()); // id
        row.append(q.value(1).toString()); // zone
        row.append(q.value(2).toString()); // cap
        row.append(q.value(3).toString()); // status
        row.append(q.value(4).toString()); // used
        binsList.append(row);
    }
    irlBinTable->clearContents();
    int rows = binsList.isEmpty() ? 1 : binsList.size();
    irlBinTable->setRowCount(rows);
    irlBinTable->setColumnCount(6);
    irlBinTable->setHorizontalHeaderLabels(QStringList() << tr("🆔 ID") << tr("📍 Zone") << tr("📦 Capacité") << tr("📋 Status") << tr("🔋 Remplissage") << tr("🎯 Action"));
    if (binsList.isEmpty()) {
        irlBinTable->setItem(0, 0, new QTableWidgetItem(tr("Aucune poubelle disponible")));
        irlBinTable->setSpan(0, 0, 1, 6);
    } else {
        for (int i = 0; i < (int)binsList.size(); ++i) {
            // ID
            QTableWidgetItem *idItem = new QTableWidgetItem(binsList[i][0]);
            idItem->setTextAlignment(Qt::AlignCenter);
            irlBinTable->setItem(i, 0, idItem);
            
            // Zone
            QTableWidgetItem *zoneItem = new QTableWidgetItem(binsList[i][1]);
            zoneItem->setTextAlignment(Qt::AlignCenter);
            irlBinTable->setItem(i, 1, zoneItem);
            
            // Capacité
            QTableWidgetItem *capItem = new QTableWidgetItem(binsList[i][2] + " L");
            capItem->setTextAlignment(Qt::AlignCenter);
            irlBinTable->setItem(i, 2, capItem);
            
            // Status
            QString statusText = binsList[i][3];
            QString statusIcon = "⚪";
            QString statusColor = "#888888";
            if (statusText == "VIDE") { statusIcon = "🟢"; statusColor = "#00ff88"; }
            else if (statusText == "PLEIN") { statusIcon = "🔴"; statusColor = "#ff4444"; }
            else if (statusText == "BROKEN" || statusText == "ENDOMMAGE") { statusIcon = "🟡"; statusColor = "#ffaa00"; }
            QTableWidgetItem *statusItem = new QTableWidgetItem(statusIcon + " " + statusText);
            statusItem->setTextAlignment(Qt::AlignCenter);
            statusItem->setForeground(QColor(statusColor));
            irlBinTable->setItem(i, 3, statusItem);
            
            // Battery-style
            int val = (int)binsList[i][4].toDouble();
            QWidget *batteryWidget = new QWidget();
            QHBoxLayout *batteryLayout = new QHBoxLayout(batteryWidget);
            batteryLayout->setContentsMargins(5, 5, 5, 5);
            batteryLayout->setSpacing(5);
            QFrame *batteryFrame = new QFrame();
            batteryFrame->setFixedSize(100, 30);
            QString fillColor;
            if (val < 30) fillColor = "#00ff88";
            else if (val < 60) fillColor = "#ffcc00";
            else if (val < 85) fillColor = "#ff8800";
            else fillColor = "#ff3333";
            batteryFrame->setStyleSheet(QString(R"(
                QFrame { background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #2a2a4a, stop:0.5 #1a1a3a, stop:1 #2a2a4a); border: 2px solid #5a5a8a; border-radius: 6px; }
            )"));
            QFrame *fillFrame = new QFrame(batteryFrame);
            int fillWidth = qMax(4, (int)(90.0 * val / 100.0));
            fillFrame->setGeometry(4, 4, fillWidth, 22);
            fillFrame->setStyleSheet(QString("QFrame { background: %1; border-radius: 4px; border: none; }").arg(fillColor));
            QLabel *pctLabel = new QLabel(QString::number(val) + "%");
            pctLabel->setStyleSheet(QString("color: %1; font-weight: bold; font-size: 12px;").arg(fillColor));
            pctLabel->setAlignment(Qt::AlignCenter);
            batteryLayout->addWidget(batteryFrame);
            batteryLayout->addWidget(pctLabel);
            batteryLayout->addStretch();
            irlBinTable->setCellWidget(i, 4, batteryWidget);
            
            // IRL action widget (PIR + Vider)
            QWidget *actionWidget = new QWidget();
            QHBoxLayout *actionLayout = new QHBoxLayout(actionWidget);
            actionLayout->setContentsMargins(0, 0, 0, 0);
            actionLayout->setSpacing(6);

            QPushButton *pirBtn = new QPushButton(tr("🔴 PIR"));
            pirBtn->setProperty("bin_id", binsList[i][0]);
            pirBtn->setCursor(Qt::PointingHandCursor);
            connect(pirBtn, &QPushButton::clicked, this, [this, pirBtn]() {
                bool ok;
                int binId = pirBtn->property("bin_id").toInt(&ok);
                if (!ok) return;
                showBigBatteryDialog(binId);
            });

            QPushButton *viderBtn = new QPushButton(tr("🧹 Vider"));
            viderBtn->setProperty("bin_id", binsList[i][0]);
            viderBtn->setCursor(Qt::PointingHandCursor);
            connect(viderBtn, &QPushButton::clicked, this, [this, viderBtn]() {
                bool ok;
                int binId = viderBtn->property("bin_id").toInt(&ok);
                if (!ok) return;
                // Create temporary Poubelle object to update DB
                Poubelle p;
                if (p.modifierUsedPercentage(binId, 0.0)) {
                    QMetaObject::invokeMethod(this, "appendIrlDebug", Qt::QueuedConnection,
                                              Q_ARG(QString, QString("🧹 Vider: poubelle %1 vidée (0%%, statut VIDE)").arg(binId)));
                    QMetaObject::invokeMethod(this, "refreshIrlDialog", Qt::QueuedConnection);
                } else {
                    QMetaObject::invokeMethod(this, "appendIrlDebug", Qt::QueuedConnection,
                                              Q_ARG(QString, QString("❌ Échec vidage poubelle %1").arg(binId)));
                }
            });

            actionLayout->addWidget(pirBtn);
            actionLayout->addWidget(viderBtn);
            actionLayout->addStretch();
            irlBinTable->setCellWidget(i, 5, actionWidget);
        }
    }
    irlBinTable->resizeRowsToContents();
}

void MainWindow::appendIrlDebug(const QString &msg)
{
    if (!irlDebugConsole)
        return;
    QString timestamp = QDateTime::currentDateTime().toString("hh:mm:ss");
    irlDebugConsole->append(QString("[%1] %2").arg(timestamp).arg(msg));
    irlDebugConsole->verticalScrollBar()->setValue(irlDebugConsole->verticalScrollBar()->maximum());
}

void MainWindow::showBigBatteryDialog(int binId)
{
    // create a large battery dialog for a specific bin
    QDialog *d = new QDialog(this);
    d->setWindowTitle(tr("🔋 Bin #%1 - Battery View").arg(binId));
    d->setMinimumSize(420, 320);
    d->setModal(true);
    d->setStyleSheet(R"(
        QDialog { background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #08121a, stop:1 #0f3460); }
        QLabel { color: #ffffff; }
        QPushButton { background: #00d9ff; color: #000000; border-radius: 8px; padding: 10px 16px; font-weight: bold; }
    )");

    QVBoxLayout *lay = new QVBoxLayout(d);
    lay->setContentsMargins(20, 20, 20, 20);
    lay->setSpacing(15);

    QLabel *titleLabel = new QLabel(tr("Bin #%1" ).arg(binId));
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("font-size: 20px; font-weight: bold; color: #00ff88;");
    lay->addWidget(titleLabel);

    // Large battery container
    QWidget *batteryContainer = new QWidget(d);
    QHBoxLayout *bLay = new QHBoxLayout(batteryContainer);
    bLay->setContentsMargins(10, 10, 10, 10);
    bLay->setSpacing(15);

    QFrame *bigBattery = new QFrame(batteryContainer);
    bigBattery->setFixedSize(320, 140);
    bigBattery->setStyleSheet("QFrame{ background: qlineargradient(x1:0,y1:0,x2:0,y2:1, stop:0 #16213e, stop:1 #0f2745); border: 2px solid #3a5a8a; border-radius: 12px;} ");

    // inner fill
    QFrame *bigFill = new QFrame(bigBattery);
    bigFill->setGeometry(8, 8, 0, 124); // width updated later
    bigFill->setStyleSheet("QFrame{ background: #00ff88; border-radius: 8px;} ");

    // Percentage label
    QLabel *bigPct = new QLabel("0%", batteryContainer);
    bigPct->setAlignment(Qt::AlignCenter);
    bigPct->setStyleSheet("font-size: 30px; font-weight: bold; color: #ffffff;");
    bLay->addWidget(bigBattery);
    bLay->addWidget(bigPct);
    bLay->addStretch();
    lay->addWidget(batteryContainer);

    // Info line
    QLabel *info = new QLabel(tr("Status: ..."));
    info->setAlignment(Qt::AlignCenter);
    info->setStyleSheet("color: #a0a0a0;");
    lay->addWidget(info);

    // Buttons
    QHBoxLayout *btnLay = new QHBoxLayout();
    QPushButton *pirBtn = new QPushButton(tr("🔄 Simulate PIR"));
    QPushButton *closeBtn = new QPushButton(tr("✖ Close"));
    btnLay->addStretch();
    btnLay->addWidget(pirBtn);
    btnLay->addWidget(closeBtn);
    lay->addLayout(btnLay);

    // Load function for updating UI (reads DB for bin)
    auto updateUi = [bigFill, bigPct, info, bigBattery, binId]() {
        QSqlQuery q;
        q.prepare("SELECT COALESCE(USED_PERCENTAGE,0), STATUS FROM GESPUB WHERE ID_BIN = :id");
        q.bindValue(":id", binId);
        if (!q.exec() || !q.next()) return;
        double used = q.value(0).toDouble();
        QString status = q.value(1).toString();
        int pct = qBound(0, static_cast<int>(used + 0.5), 100);
        // update fill width
        int maxWidth = bigBattery->width() - 16; // 8 margins each side
        int fillW = qMax(4, (int)(maxWidth * pct / 100.0));
        bigFill->setGeometry(8, 8, fillW, bigBattery->height() - 16);
        // set color based on level
        QString fillColor;
        if (pct < 30) fillColor = "#00ff88";
        else if (pct < 60) fillColor = "#ffcc00";
        else if (pct < 85) fillColor = "#ff8800";
        else fillColor = "#ff3333";
        bigFill->setStyleSheet(QString("QFrame{ background: %1; border-radius: 8px;}").arg(fillColor));
        bigPct->setText(QString::number(pct) + "%");
        info->setText(tr("Status: %1").arg(status));
    };

    // initial update
    updateUi();

    // Timer to auto-update while dialog open
    QTimer *t = new QTimer(d);
    connect(t, &QTimer::timeout, d, updateUi);
    t->start(400);

    // Simulate PIR click
    connect(pirBtn, &QPushButton::clicked, this, [this, binId, updateUi]() {
        // Simulate the motion via Poubelle; this will modify the DB
        Poubelle::handlePIRMotion(binId);
        // add a log to the IRL debug console
        QMetaObject::invokeMethod(this, "appendIrlDebug", Qt::QueuedConnection, Q_ARG(QString, QString("🔴 Simulated PIR for bin %1").arg(binId)));
        updateUi();
    });

    connect(closeBtn, &QPushButton::clicked, d, &QDialog::accept);
    d->exec();
    delete d;
}

// ============================================================================
// STUB FUNCTIONS - Déclarées dans le header mais pas encore implémentées
// ============================================================================

void MainWindow::on_btn_vehicule_tri_clicked()
{
    // TODO: Implement vehicle sorting functionality
    QMessageBox::information(this, tr("Tri Véhicules"), tr("Fonctionnalité de tri des véhicules à implémenter."));
}

void MainWindow::on_btn_vehicule_export_clicked()
{
    // TODO: Implement vehicle export functionality
    QMessageBox::information(this, tr("Export Véhicules"), tr("Fonctionnalité d'export des véhicules à implémenter."));
}

void MainWindow::on_btnAfficherConso_clicked()
{
    // TODO: Implement consumption display functionality
    QMessageBox::information(this, tr("Consommation"), tr("Fonctionnalité d'affichage de consommation à implémenter."));
}

void MainWindow::on_btnActiverAlertesAuto_clicked()
{
    // TODO: Implement auto-alerts activation
    QMessageBox::information(this, tr("Alertes Automatiques"), tr("Fonctionnalité d'alertes automatiques à implémenter."));
}

void MainWindow::on_btnVerifierSMSEntrants_clicked()
{
    // TODO: Implement incoming SMS verification
    QMessageBox::information(this, tr("SMS Entrants"), tr("Fonctionnalité de vérification des SMS entrants à implémenter."));
}

void MainWindow::on_btnTestSMS_clicked()
{
    // TODO: Implement SMS test functionality
    QMessageBox::information(this, tr("Test SMS"), tr("Fonctionnalité de test SMS à implémenter."));
}
