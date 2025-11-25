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

static const double DEMANDE_DELAI_HEURES = 8.0;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    affectationSummaryLabel = nullptr;
    qApp->setStyleSheet(
        "QMessageBox QLabel, QInputDialog QLabel, QLineEdit { color: black; }"
        "QMessageBox QPushButton, QInputDialog QPushButton { color: black; }"
        );

    setupTable();

    // === Habitant helpers (validators, styles, statuses, data load) ===
    chargerStatutsAutorises();
    styliserChampsSaisie();
    chargerDonneesTable();
    loadLastHabitantSelection();

    // Recherche live
    connect(ui->lineEdit_rechH, &QLineEdit::textChanged, this, [this](const QString &t) {
        rechercherTexte(t);
    });

    // Validators
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
    connect(ui->affzone, &QPushButton::clicked, this, &MainWindow::afficherZones);
    connect(ui->affpoub, &QPushButton::clicked, this, &MainWindow::afficherPoubelles);
    connect(ui->modzone, &QPushButton::clicked, this, &MainWindow::modZone);
    connect(ui->modpoub, &QPushButton::clicked, this, &MainWindow::modPoubelle);

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
		sqls << "SELECT AVG(SCORE), COUNT(*) FROM TOUHA.SATISFACTION WHERE ID_HABITANT = :id"
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
		sqls2 << "SELECT COMMENTAIRE, DATE_SAISIE FROM TOUHA.SATISFACTION WHERE ID_HABITANT = :id ORDER BY DATE_SAISIE DESC, ID_SATISFACTION DESC"
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

	QChartView *chartView = nullptr;
	if (hasSondages) {
		QLineSeries *series = new QLineSeries();
		QSqlQuery q;
		QStringList sqls;
		sqls << "SELECT DATE_SAISIE, SCORE FROM TOUHA.SATISFACTION WHERE ID_HABITANT = :id ORDER BY DATE_SAISIE"
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
			if (auto axisY = qobject_cast<QValueAxis*>(chart->axisY())) {
				axisY->setRange(0, 100);
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
}

void MainWindow::on_ajpoub_clicked()
{
    // --- Contrôle de saisie ---
    if (ui->pubid->text().isEmpty() ||
        ui->zoneid->text().isEmpty() ||
        ui->cap->text().isEmpty() ||
        ui->status->text().isEmpty())
    {
        QMessageBox::warning(this, "⚠️ Champ vide", "Tous les champs doivent être remplis !");
        return;
    }

    QString status = ui->status->text().trimmed().toLower();
    if (status != "vide" && status != "plein")
    {
        QMessageBox::warning(this, "⚠️ Statut invalide", "Le statut doit être soit 'vide' soit 'plein' !");
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
    }
    else
        QMessageBox::critical(this, "❌ Erreur", "Échec de la suppression de la zone !");
}

void MainWindow::on_supppoub_clicked()
{
    int idToDelete = QInputDialog::getInt(this, "Supprimer Poubelle", "Entrez l'ID de la poubelle :");

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

    ui->twp->setRowCount(0);
    int rows = model->rowCount();
    int cols = model->columnCount();
    ui->twp->setColumnCount(cols);

    for (int j = 0; j < cols; ++j)
        ui->twp->setHorizontalHeaderItem(j, new QTableWidgetItem(model->headerData(j, Qt::Horizontal).toString()));

    for (int i = 0; i < rows; ++i)
    {
        ui->twp->insertRow(i);
        for (int j = 0; j < cols; ++j)
        {
            ui->twp->setItem(i, j, new QTableWidgetItem(model->data(model->index(i, j)).toString()));
        }
    }
    ui->twp->resizeColumnsToContents();
}

// ==================== CRUD Modify ====================
void MainWindow::modZone()
{
    int idToModify = QInputDialog::getInt(this, "Modifier Zone", "Entrez l'ID de la zone :");

    int newPop = QInputDialog::getInt(this, "Modifier Zone", "Nouvelle population :");
    double newX = QInputDialog::getDouble(this, "Modifier Zone", "Nouvelle coordonnée X :");
    double newY = QInputDialog::getDouble(this, "Modifier Zone", "Nouvelle coordonnée Y :");
    double newL = QInputDialog::getDouble(this, "Modifier Zone", "Nouvelle longueur :");
    double newH = QInputDialog::getDouble(this, "Modifier Zone", "Nouvelle hauteur :");

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
    }
    else
        QMessageBox::critical(this, "❌ Erreur", "Échec de modification de la zone !");
}

void MainWindow::modPoubelle()
{
    int idToModify = QInputDialog::getInt(this, "Modifier Poubelle", "Entrez l'ID de la poubelle :");

    int newZoneId = QInputDialog::getInt(this, "Modifier Poubelle", "Nouvel ID de zone :");
    int newCap = QInputDialog::getInt(this, "Modifier Poubelle", "Nouvelle capacité :");
    QString newStatus = QInputDialog::getText(this, "Modifier Poubelle", "Nouveau statut :").trimmed().toLower();

    // --- Contrôle de saisie ---
    if (newStatus.isEmpty())
    {
        QMessageBox::warning(this, "⚠️ Champ vide", "Le statut ne peut pas être vide !");
        return;
    }
    if (newStatus != "vide" && newStatus != "plein")
    {
        QMessageBox::warning(this, "⚠️ Statut invalide", "Le statut doit être 'vide' ou 'plein' !");
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
                               "FROM TOUHA.SATISFACTION ORDER BY DATE_SAISIE DESC, ID_SATISFACTION DESC");
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
        "SELECT AVG(SCORE), COUNT(*) FROM TOUHA.SATISFACTION",
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
        qh.prepare("SELECT 1 FROM TOUHA.HABITANT WHERE ID_HABITANT = :id");
        qh.bindValue(":id", idH);
        if (!qh.exec() || !qh.next()) {
            const QString raw = qh.lastError().text().trimmed();
            QMessageBox mb(this);
            mb.setIcon(QMessageBox::Warning);
            mb.setWindowTitle(tr("Sondage"));
            mb.setTextFormat(Qt::RichText);
            mb.setStyleSheet("QLabel{color:black;} QPushButton{color:black;}");
            mb.setText(tr("Habitant inexistant (ID %1) ou accès refusé à TOUHA.HABITANT.").arg(idH));
            if (!raw.isEmpty()) mb.setInformativeText(raw);
            qDebug() << "Sondage warn:" << mb.text() << "; info=" << raw;
            mb.exec();
            return;
        }
    }
    {
        QSqlQuery ping;
        if (!ping.exec("SELECT 1 FROM TOUHA.SATISFACTION WHERE 1=0")) {
            const QString raw = ping.lastError().text().trimmed();
            QMessageBox mb(this);
            mb.setIcon(QMessageBox::Warning);
            mb.setWindowTitle(tr("Sondage"));
            mb.setTextFormat(Qt::RichText);
            mb.setStyleSheet("QLabel{color:black;} QPushButton{color:black;}");
            mb.setText(tr("Accès à TOUHA.SATISFACTION impossible (droits?)."));
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
    model->setQuery(q);
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

