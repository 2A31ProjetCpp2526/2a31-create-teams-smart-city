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
#include "consomation.h"
#include "maintenance.h"
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
#include <QStandardItemModel>
#include <QStandardItem>
#include <QDialog>
#include <QVBoxLayout>
#include <QFont>
#include <QDateEdit>
#include <QTextBrowser>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
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

    // === Charger aussi la table des véhicules au démarrage ===
    chargerTableVehicules();
    
    // === Initialiser le module de maintenance véhicules ===
    m_maintenance = new Maintenance(this, this);
    if (m_maintenance) {
        // Style labels for better visibility (ce sont des QLabel, pas QTextBrowser)
        if (auto alertes = findChild<QLabel*>("textMaint_alertes")) {
            alertes->setWordWrap(true);
            alertes->setAlignment(Qt::AlignLeft | Qt::AlignTop);
            alertes->setStyleSheet("QLabel { background-color: #ffffff; color: #000000; font: 10pt 'Segoe UI'; padding: 8px; border: 2px solid #5dade2; border-radius: 5px; min-height: 50px; }");
        }
        if (auto diag = findChild<QLabel*>("textMaint_diag")) {
            diag->setWordWrap(true);
            diag->setAlignment(Qt::AlignLeft | Qt::AlignTop);
            diag->setStyleSheet("QLabel { background-color: #ffffff; color: #000000; font: 10pt 'Segoe UI'; padding: 8px; border: 2px solid #5dade2; border-radius: 5px; min-height: 100px; }");
        }
        
        // Initialiser le ComboBox de problèmes - essayer les deux noms
        QComboBox *combo = findChild<QComboBox*>("comboMaint_probleme");
        if (!combo) {
            combo = findChild<QComboBox*>("comboMaint_type");
        }
        if (combo) {
            combo->clear();
            combo->addItems(QStringList() << 
                "Sélectionner un problème" <<
                "Le moteur chauffe" <<
                "La voiture tremble" <<
                "La batterie se décharge" <<
                "Freins faibles" <<
                "Fuite d'huile" <<
                "Fumée noire" <<
                "Voiture ne démarre pas" <<
                "Consommation élevée" <<
                "Bruit métallique" <<
                "Pneus s'usent vite"
            );
            combo->setCurrentIndex(0);
            qDebug() << "ComboBox initialisé:" << combo->objectName();
        } else {
            qDebug() << "WARNING: ComboBox de problème non trouvé (comboMaint_probleme ou comboMaint_type)";
        }
        
        // Connect buttons - essayer plusieurs noms possibles
        if (auto b = findChild<QPushButton*>("btnMaint_diagnostiquer")) {
            connect(b, &QPushButton::clicked, m_maintenance, &Maintenance::diagnostiquer);
            qDebug() << "Button btnMaint_diagnostiquer connected";
        } else if (auto b = findChild<QPushButton*>("btnMaint_calculer")) {
            connect(b, &QPushButton::clicked, m_maintenance, &Maintenance::diagnostiquer);
            qDebug() << "Button btnMaint_calculer connected";
        } else {
            qDebug() << "WARNING: Diagnostic button not found!";
        }
        
        if (auto b = findChild<QPushButton*>("btnMaint_ajouter")) {
            connect(b, &QPushButton::clicked, m_maintenance, &Maintenance::ajouter);
        }
        
        if (auto b = findChild<QPushButton*>("btnMaint_effacer")) {
            connect(b, &QPushButton::clicked, m_maintenance, &Maintenance::effacer);
        }

        const QStringList maintLabels = {
            "label_135", "label_3", "label_58", "label_79",
            "label_80", "label_85", "label_86"
        };
        for (const QString &name : maintLabels) {
            if (auto lbl = findChild<QLabel*>(name)) {
                QString style = lbl->styleSheet();
                if (!style.contains("color")) {
                    style += "color: white;";
                } else {
                    style.replace(QRegularExpression("color:[^;]+;"), "color: white;");
                }
                lbl->setStyleSheet(style);
            }
        }
    }

    // === Module Consommation ===
    m_consoModule = new ConsomationModule(this);

    // Recherche live
    connect(ui->lineEdit_rechH, &QLineEdit::textChanged, this, [this](const QString &t) {
        rechercherTexte(t);
    });

    // Validators
    ui->lineEdit_id->setValidator(new QIntValidator(1, 2147483647, this));
    ui->lineEdit_satisfaction->setValidator(new QIntValidator(0, 100, this));
    ui->lineEdit_contact->setValidator(new QRegularExpressionValidator(QRegularExpression("^\\d{0,15}$"), this));

    // === Validators Vehicule ===
    // CAPACITE: uniquement des chiffres
    if (ui->lineEdit_vehicule_capacite)
        ui->lineEdit_vehicule_capacite->setValidator(new QRegularExpressionValidator(QRegularExpression("^\\d*$"), this));

    // ITINERAIRE: uniquement des lettres (et espaces)
    QRegularExpression rxLettres("^[A-Za-z\\s]*$");
    if (ui->lineEdit_vehicule_itineraire)
        ui->lineEdit_vehicule_itineraire->setValidator(new QRegularExpressionValidator(rxLettres, this));
    
    // HEURE_DEPART et HEURE_ARRIVEE: uniquement des chiffres
    QRegularExpression rxNombres("^\\d*$");
    if (ui->lineEdit_vehicule_hdep)
        ui->lineEdit_vehicule_hdep->setValidator(new QRegularExpressionValidator(rxNombres, this));
    if (ui->lineEdit_vehicule_harr)
        ui->lineEdit_vehicule_harr->setValidator(new QRegularExpressionValidator(rxNombres, this));

    // === Initialisation Export Vehicule ===
    // Format combo box (A3, A4, A5)
    if (auto combo = findChild<QComboBox*>("comboBox_vehicule_format")) {
        combo->clear();
        combo->addItems(QStringList() << "A3" << "A4" << "A5");
        combo->setCurrentText("A4");
    }
    
    // Qualité combo box (standard)
    if (auto combo = findChild<QComboBox*>("comboBox_vehicule_qualite")) {
        combo->clear();
        combo->addItem("standard");
        combo->setCurrentText("standard");
    }
    
    // Contexte rapide pour choisir l'emplacement: Bureau / Téléchargements / Parcourir…
    if (auto emplacementV = findChild<QLineEdit*>("emplacementV")) {
        emplacementV->setPlaceholderText("Chemin du dossier (Bureau/Téléchargements/Parcourir…)");
        emplacementV->setContextMenuPolicy(Qt::CustomContextMenu);
        connect(emplacementV, &QLineEdit::customContextMenuRequested, this, [this, emplacementV](const QPoint &pos){
            QMenu menu(this);
            const QString bureau = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
            const QString telech = QStandardPaths::writableLocation(QStandardPaths::DownloadLocation);
            QAction *aBureau = menu.addAction(QString::fromUtf8("Bureau (%1)").arg(bureau));
            QAction *aTelech = menu.addAction(QString::fromUtf8("Téléchargements (%1)").arg(telech));
            menu.addSeparator();
            QAction *aParcourir = menu.addAction("Parcourir…");
            QAction *sel = menu.exec(emplacementV->mapToGlobal(pos));
            if (!sel) return;
            if (sel == aBureau) emplacementV->setText(bureau);
            else if (sel == aTelech) emplacementV->setText(telech);
            else if (sel == aParcourir) {
                QString dir = QFileDialog::getExistingDirectory(this, "Choisir un dossier", telech.isEmpty()? QDir::homePath() : telech);
                if (!dir.isEmpty()) emplacementV->setText(dir);
            }
        });
    }

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

    // === Styling: Consommation section ===
    // Result label: white background, dark text, nicer font
    if (ui->label_conso_resultat) {
        ui->label_conso_resultat->setStyleSheet(
            "QLabel {"
            "  background-color: #ffffff;"
            "  color: #111111;"
            "  border-radius: 8px;"
            "  padding: 8px 12px;"
            "}"
        );
        QFont f = ui->label_conso_resultat->font();
        f.setFamily("Segoe UI");
        f.setPointSize(11);
        f.setBold(true);
        ui->label_conso_resultat->setFont(f);
    }

    // Labels around ID / Matricule / Distance / Conso / Résultat
    // Reuse the same style as an existing Habitant label (near satisfaction/score)
    QFont baseConsoLabelFont = ui->label_37 ? ui->label_37->font() : this->font();
    auto styleConsoLabel = [baseConsoLabelFont](QLabel *lbl) {
        if (!lbl) return;
        lbl->setFont(baseConsoLabelFont);
        // White text so it matches the global dark background
        lbl->setStyleSheet("QLabel { color: #ffffff; }");
    };
    styleConsoLabel(ui->label_39);   // ID:
    styleConsoLabel(ui->label_40);   // matricule:
    styleConsoLabel(ui->label_56);   // distance parcourue
    styleConsoLabel(ui->label_44);   // carburant consommé
    styleConsoLabel(ui->label_57);   // résultat:

    // === Connect & style Export Vehicule button manually ===
    if (QPushButton *btnExport = findChild<QPushButton*>("btn_vehicule_exporter")) {
        connect(btnExport, &QPushButton::clicked, this, &MainWindow::on_btn_vehicule_export_clicked);
        btnExport->setStyleSheet(
            "QPushButton {"
            "  background-color: #0c2d57;"
            "  color: #ffffff;"
            "  border-radius: 8px;"
            "  padding: 6px 14px;"
            "}"
            "QPushButton:hover { background-color: #1159a6; }"
            "QPushButton:pressed { background-color: #08305d; }"
        );
        QFont bf = btnExport->font();
        bf.setFamily("Segoe UI");
        bf.setPointSize(9);
        bf.setBold(true);
        btnExport->setFont(bf);
        qDebug() << "Export vehicule button connected:" << btnExport->objectName();
    } else {
        qDebug() << "WARNING: Export vehicule button 'btn_vehicule_exporter' not found!";
    }

    // Ne pas auto-rafraîchir au démarrage pour éviter d'affecter une mauvaise vue
    // L'utilisateur déclenchera l'affichage via les boutons.
}

// Bouton "affichersatisfaction": affiche les attributs de la table SATISFACTION
void MainWindow::on_affichersatisfaction_clicked()
{
    QTableView *tv = resolveSatisfactionTableView();
    if (!tv) {
        QMessageBox::warning(this, tr("Satisfaction"), tr("Aucune table de satisfaction (QTableView) trouvée dans l'UI."));
        return;
    }

    QSqlQueryModel *model = SatisfactionCRUD::afficher(tv);
    tv->setModel(model);
    if (model && model->lastError().isValid()) {
        QMessageBox::warning(this, tr("Satisfaction"),
                             tr("Erreur lors du chargement de SATISFACTION:\n%1")
                                 .arg(model->lastError().text()));
    }
    tv->setSelectionBehavior(QAbstractItemView::SelectRows);
    tv->setSelectionMode(QAbstractItemView::SingleSelection);
    tv->setSortingEnabled(true);
    tv->resizeColumnsToContents();
}

MainWindow::~MainWindow()
{
    delete m_consoModule;
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

// Charger la table des véhicules dans le QTableView dédié
void MainWindow::chargerTableVehicules()
{
    if (!ui->tableView_vehicule) return;
    VehiculeCRUD v;
    QSqlQueryModel *model = v.afficher(this);
    ui->tableView_vehicule->setModel(model);
    // Rendre la table plus lisible (fond blanc, texte noir)
    ui->tableView_vehicule->setStyleSheet("QTableView { background-color: #ffffff; color: #000000; }");
    ui->tableView_vehicule->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView_vehicule->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView_vehicule->setSortingEnabled(true);
    ui->tableView_vehicule->resizeColumnsToContents();
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

    if (ui->reclamation) {
        if (auto le = qobject_cast<QLineEdit*>(ui->reclamation)) le->clear();
        else if (auto te = qobject_cast<QTextEdit*>(ui->reclamation)) te->clear();
    }
}

bool MainWindow::statutAutorise(const QString &value) const
{
    // Si on n'a pas pu récupérer la liste autorisée depuis la base,
    // on bloque par sécurité pour éviter l'erreur ORA-02290 côté Oracle.
    if (allowedStatuses.isEmpty()) return false;
    for (const QString &v : allowedStatuses) {
        if (v.compare(value.trimmed(), Qt::CaseInsensitive) == 0) return true;
    }
    return false;
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
        // Habitant
        ui->lineEdit_id,
        ui->lineEdit_nom,
        ui->lineEdit_prenom,
        ui->lineEdit_adresse,
        ui->lineEdit_zone,
        ui->lineEdit_situation,
        ui->lineEdit_contact,
        ui->lineEdit_statut,
        ui->lineEdit_besoins,
        ui->lineEdit_satisfaction,

        // Véhicule
        ui->lineEdit_vehicule_id,
        ui->lineEdit_vehicule_matricule,
        ui->lineEdit_vehicule_capacite,
        ui->lineEdit_vehicule_itineraire,
        ui->lineEdit_vehicule_hdep,
        ui->lineEdit_vehicule_harr,
        ui->lineEdit_vehicule_zone,
        ui->lineEdit_vehicule_idhabitant,
        ui->lineEdit_vehicule_recherche,

        // Consommation
        findChild<QLineEdit*>("lineIDVehicule"),
        findChild<QLineEdit*>("lineMatricule"),
        findChild<QLineEdit*>("lineEdit_3"),
        findChild<QLineEdit*>("lineDistance"),
        findChild<QLineEdit*>("lineConso")
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
void MainWindow::on_Gpersonnels_2_clicked() { ui->stackedWidget->setCurrentIndex(2); }
void MainWindow::on_Gequipements_2_clicked() { ui->stackedWidget->setCurrentIndex(4); }
void MainWindow::on_Gvehicules_2_clicked() { ui->stackedWidget->setCurrentIndex(5); }
void MainWindow::on_Gespace_clicked() { ui->stackedWidget->setCurrentIndex(3); }

void MainWindow::on_on_pushButton_4_clicked() { ui->stackedWidget_5->setCurrentIndex(1); }
void MainWindow::on_on_pushButton_5_clicked() { ui->stackedWidget_5->setCurrentIndex(2); }
void MainWindow::on_on_pushButton_6_clicked() { ui->stackedWidget_5->setCurrentIndex(0); }

void MainWindow::on_on_pushButton_7_clicked() { ui->stackedWidget_6->setCurrentIndex(0); }
void MainWindow::on_on_pushButton_8_clicked() { ui->stackedWidget_6->setCurrentIndex(1); }
void MainWindow::on_on_pushButton_9_clicked() { ui->stackedWidget_6->setCurrentIndex(2); }

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
    if (ui && ui->stackedWidget_2)
        ui->stackedWidget_2->setCurrentIndex(1);
}

void MainWindow::on_statH_clicked()
{
    // Affiche la même page de statistiques pour l'instant
    if (ui && ui->stackedWidget_2)
        ui->stackedWidget_2->setCurrentIndex(1);
}

// === Satisfaction View / Table population ===
void MainWindow::refreshSatisfactionTable()
{
    QTableView *tv = resolveSatisfactionTableView();
    if (!tv) {
        QMessageBox::warning(this, tr("Satisfaction"), tr("Aucune table de satisfaction (QTableView) trouvée dans l'UI."));
        return;
    }

    // Récupérer un filtre ID si présent dans l'UI
    int idFilter = 0;
    if (ui->lineEdit_id) idFilter = ui->lineEdit_id->text().trimmed().toInt();
    if (idFilter <= 0) {
        if (auto le = findChild<QLineEdit*>("IDdhabitant")) idFilter = le->text().trimmed().toInt();
    }
    if (idFilter <= 0) {
        if (auto le = findChild<QLineEdit*>("lineEdit_satisfaction_id")) idFilter = le->text().trimmed().toInt();
    }

    // Essayer d'abord la vue d'agrégation si elle existe
    std::unique_ptr<QSqlQueryModel> model(new QSqlQueryModel());
    QString base = "SELECT id_habitant, nom, besoins, satisfaction, type_demande FROM vw_habitant_satisfaction";
    QString baseSchema = "SELECT id_habitant, nom, besoins, satisfaction, type_demande FROM TOUHA.vw_habitant_satisfaction";
    if (idFilter > 0) {
        model->setQuery(QString("%1 WHERE id_habitant = %2 ORDER BY id_habitant").arg(base).arg(idFilter));
        if (model->lastError().isValid())
            model->setQuery(QString("%1 WHERE id_habitant = %2 ORDER BY id_habitant").arg(baseSchema).arg(idFilter));
    } else {
        model->setQuery(base + " ORDER BY id_habitant");
        if (model->lastError().isValid())
            model->setQuery(baseSchema + " ORDER BY id_habitant");
    }

    if (model->lastError().isValid()) {
        // Repli: afficher la table SATISFACTION brute via SatisfactionCRUD
        QSqlQueryModel *fallback = SatisfactionCRUD::afficher(tv);
        if (fallback && !fallback->lastError().isValid()) {
            tv->setModel(fallback);
            tv->resizeColumnsToContents();
            return;
        }
        const QString errText = model->lastError().text().trimmed().isEmpty()
                                    ? tr("Erreur SQL inconnue (aucun message renvoyé par le pilote)")
                                    : model->lastError().text();
        QMessageBox::warning(this, tr("Satisfaction"),
                             tr("Impossible de charger les données (vue et table).\nDernière erreur SQL: %1")
                                 .arg(errText));
        return;
    }
    model->setHeaderData(0, Qt::Horizontal, tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, tr("Nom"));
    model->setHeaderData(2, Qt::Horizontal, tr("Besoins"));
    model->setHeaderData(3, Qt::Horizontal, tr("Satisfaction"));
    model->setHeaderData(4, Qt::Horizontal, tr("Demande"));
    tv->setModel(model.release());
    tv->resizeColumnsToContents();
}

QTableView* MainWindow::resolveSatisfactionTableView() const
{
    // Retourner uniquement des vues explicitement nommées pour éviter de polluer d'autres tables
    if (auto tv = findChild<QTableView*>("tableView_satisfaction")) return tv;
    if (auto tv = findChild<QTableView*>("tableView_satisfaction_2")) return tv;
    return nullptr;
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
    if (ui->lineEdit_id) idH = ui->lineEdit_id->text().trimmed().toInt();
    if (idH <= 0) {
        if (auto le = findChild<QLineEdit*>("IDdhabitant")) idH = le->text().trimmed().toInt();
    }

    // Récupérer score à partir d'un QSpinBox ou QSlider
    int score = -1;
    if (auto spin = findChild<QSpinBox*>("spin_satisfaction_score")) score = spin->value();
    if (score < 0) {
        if (auto slider = findChild<QSlider*>("slider_satisfaction_score")) score = slider->value();
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
    // Réutiliser le même helper que celui appelé au démarrage
    chargerTableVehicules();
}

void MainWindow::on_btn_vehicule_tri_clicked()
{
    if (!ui->tableView_vehicule) return;

    // Critère choisi dans le combo de tri des véhicules (ID, type, matricule, capacite)
    QString critere = ui->comboBox_3 ? ui->comboBox_3->currentText().trimmed() : QString();
    QString order;

    // Mapper les libellés UI vers les colonnes de la table VEHICULE
    if (critere.compare("ID", Qt::CaseInsensitive) == 0)
        order = "ID_VEHICULE ASC";
    else if (critere.compare("type", Qt::CaseInsensitive) == 0)
        order = "TYPE_VEHICULE ASC";
    else if (critere.compare("matricule", Qt::CaseInsensitive) == 0)
        order = "MATRICULE ASC";
    else if (critere.compare("capacite", Qt::CaseInsensitive) == 0)
        // D'abord trier les valeurs numériques de CAPACITE comme des nombres,
        // puis les valeurs non numériques (ex: 'bfbfb') à la fin.
        order = "CASE WHEN REGEXP_LIKE(CAPACITE, '^\\d+$') THEN TO_NUMBER(CAPACITE) ELSE NULL END ASC, CAPACITE ASC";
    else
        order = "ID_VEHICULE ASC";

    QSqlQueryModel *model = new QSqlQueryModel(this);
    model->setQuery(
        "SELECT ID_VEHICULE, TYPE_VEHICULE, MATRICULE, CAPACITE, ITINERAIRE, "
        "HEURE_DEPART, HEURE_ARRIVEE, STATUT, ZONE, ID_HABITANT FROM VEHICULE ORDER BY " + order);

    ui->tableView_vehicule->setModel(model);
    ui->tableView_vehicule->resizeColumnsToContents();
}

// === Export Vehicule Table to PDF ===
void MainWindow::on_btn_vehicule_export_clicked()
{
    QMessageBox::information(this, "Debug", "Export vehicule function called!");
    qDebug() << "Export vehicule button clicked";
    
    if (!ui->tableView_vehicule) {
        QMessageBox::warning(this, "Export", "La table des véhicules n'est pas disponible.");
        return;
    }
    
    QAbstractItemModel *model = ui->tableView_vehicule->model();
    if (!model) {
        QMessageBox::warning(this, "Export", "Aucun modèle de données disponible. Veuillez d'abord afficher les véhicules.");
        return;
    }
    
    if (model->rowCount() == 0) {
        QMessageBox::warning(this, "Export", "Aucune donnée à exporter. La table est vide.");
        return;
    }
    
    qDebug() << "Model has" << model->rowCount() << "rows";

    QComboBox *formatCombo = findChild<QComboBox*>("comboBox_vehicule_format");
    QComboBox *qualiteCombo = findChild<QComboBox*>("comboBox_vehicule_qualite");
    QLineEdit *nomEdit = findChild<QLineEdit*>("nomV");
    QLineEdit *dossierEdit = findChild<QLineEdit*>("emplacementV");
    
    const QString format = formatCombo ? formatCombo->currentText().trimmed().toUpper() : QString("A4");
    const QString qualite = qualiteCombo ? qualiteCombo->currentText().trimmed().toLower() : QString("standard");
    QString nom = nomEdit ? nomEdit->text().trimmed() : QString();
    QString dossier = dossierEdit ? dossierEdit->text().trimmed() : QString();

    if (nom.isEmpty()) nom = QString("vehicules_%1").arg(QDateTime::currentDateTime().toString("yyyyMMdd_HHmmss"));
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

    const bool ok = HabitantCRUD::exportModelToPdf(model, filePath, pageSize, dpi, "Liste des Véhicules");
    if (ok) {
        QMessageBox::information(this, "Export", QString("Fichier PDF généré:\n%1").arg(filePath));
    } else {
        QMessageBox::critical(this, "Export", "Échec de la génération du PDF.");
    }
}
//////////////////////////////////////////////personel/////////////////////////////////////
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
////////////////////////////////////personellll//////////////////////////////////////////////
// Ajouter
void MainWindow::on_ajouterperso_clicked()
{
    QString id = ui->ID->text().trimmed();
    QString nom = ui->Nom->text().trimmed();
    QString prenom = ui->prenom->text().trimmed();
    QString comp = ui->competence->text().trimmed();
    QString zone = ui->Zone->text().trimmed();

    // Validation des champs
    if (id.isEmpty() || nom.isEmpty() || prenom.isEmpty() || comp.isEmpty() || zone.isEmpty()) {
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
        ui->ID->clear();
        ui->Nom->clear();
        ui->prenom->clear();
        ui->competence->clear();
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
    QString id = ui->ID->text().trimmed();

    // Validation de l'ID
    if (id.isEmpty()) {
        QMessageBox::warning(this, "⚠️ Champ vide", "Veuillez entrer l'ID du personnel à supprimer !");
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
            ui->ID->clear();
            ui->Nom->clear();
            ui->prenom->clear();
            ui->competence->clear();
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
    QString id = ui->ID->text().trimmed();
    QString nom = ui->Nom->text().trimmed();
    QString prenom = ui->prenom->text().trimmed();
    QString comp = ui->competence->text().trimmed();
    QString zone = ui->Zone->text().trimmed();

    // Validation des champs
    if (id.isEmpty()) {
        QMessageBox::warning(this, "⚠️ Champ vide", "L'ID est obligatoire pour la modification !");
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


