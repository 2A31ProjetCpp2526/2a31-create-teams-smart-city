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

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    graphScene = new Graph(this);
    zonesScene = new QGraphicsScene(this);
    ui->graphicsZones_2->setScene(zonesScene);
    ui->graphicsZones_2->setRenderHint(QPainter::Antialiasing);

    binsScene = new QGraphicsScene(this);
    ui->graphicsBins_2->setScene(binsScene);
    ui->graphicsBins_2->setRenderHint(QPainter::Antialiasing);

    // create a persistent label below the bins view to show per-zone messages
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
    connect(ui->btnLoadMap, &QPushButton::clicked, this, &MainWindow::refreshGraph);
    connect(ui->exp, &QPushButton::clicked, this, &MainWindow::on_exp_clicked);
    connect(ui->tri, &QPushButton::clicked, this, &MainWindow::on_tri_clicked);
    connect(ui->affzone, &QPushButton::clicked, this, &MainWindow::afficherZones);
    connect(ui->affpoub, &QPushButton::clicked, this, &MainWindow::afficherPoubelles);
    connect(ui->modzone, &QPushButton::clicked, this, &MainWindow::modZone);
    connect(ui->modpoub, &QPushButton::clicked, this, &MainWindow::modPoubelle);
    connect(ui->twp, &QTableWidget::cellClicked,
            this, &MainWindow::on_twp_cellClicked);
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
        allowedStatuses << "Actif" << "Inactif" << "En_attente";
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
void MainWindow::on_on_pushButton_2_clicked() { ui->stackedWidget_2->setCurrentIndex(1); }
void MainWindow::on_on_pushButton_3_clicked() { ui->stackedWidget_2->setCurrentIndex(2); }

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
/*void MainWindow::on_on_pushButton_14_clicked() { ui->stackedWidget_2->setCurrentIndex(1); }
void MainWindow::on_on_pushButton_15_clicked() { ui->stackedWidget_2->setCurrentIndex(2); }
void MainWindow::on_on_pushButton_16_clicked() { ui->stackedWidget_7->setCurrentIndex(0); }
void MainWindow::on_on_pushButton_17_clicked() { ui->stackedWidget_7->setCurrentIndex(1); }
void MainWindow::on_pushButton_15_clicked() { ui->stackedWidget_7->setCurrentIndex(2); }
*/

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

    // Fill automatically
    for (int i = 0; i < rows; ++i)
        for (int j = 0; j < cols; ++j)
            ui->twp->setItem(i, j,
                             new QTableWidgetItem(model->data(model->index(i, j)).toString()));

    ui->twp->resizeColumnsToContents();
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

// === Bouton de test ===
void MainWindow::on_pushButton_15_clicked()
{
    ui->stackedWidget_7->setCurrentIndex(2);
    qDebug() << "Bouton 15 cliqué !";
}
/*void MainWindow::supprimerEtDecalerLignes()
{
    QPushButton *btn = qobject_cast<QPushButton*>(sender());
    if (!btn) return;

    QTableWidget *table = ui->tableWidget_6;
    for (int row = 0; row < table->rowCount(); ++row) {
        if (table->cellWidget(row, table->columnCount() - 1) == btn) {
            table->removeRow(row);
            break;
        }
    }
}*/
void MainWindow::on_on_pushButton_14_clicked()
{
    // TODO: Implémenter
    ui->stackedWidget_2->setCurrentIndex(1);
    qDebug() << "Bouton 14 cliqué";
}

void MainWindow::on_on_pushButton_15_clicked()
{
    // TODO: Implémenter
    ui->stackedWidget_2->setCurrentIndex(2);
    qDebug() << "Bouton 15 cliqué";
}

void MainWindow::on_on_pushButton_16_clicked()
{
    ui->stackedWidget_7->setCurrentIndex(0);
    // TODO: Implémenter
    qDebug() << "Bouton 16 cliqué";
}

void MainWindow::on_on_pushButton_17_clicked()
{
    ui->stackedWidget_7->setCurrentIndex(1);
    // TODO: Implémenter
    qDebug() << "Bouton 17 cliqué";
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

// Ajouter
void MainWindow::on_ajouterperso_clicked()
{
    QString id = ui->ID->text().trimmed();
    QString nom = ui->Nom->text().trimmed();
    QString prenom = ui->prenom->text().trimmed();
    QString comp = ui->competence->text().trimmed();
    QString zone = ui->Zone->text().trimmed();

    // Validation des champs vides
    if (id.isEmpty() || nom.isEmpty() || prenom.isEmpty() || comp.isEmpty() || zone.isEmpty()) {
        QMessageBox::warning(this, "⚠️ Champ vide", "Tous les champs doivent être remplis !");
        return;
    }

    // --- Contrôle de saisie : uniquement lettres pour nom, prénom, compétence et zone ---
    QRegularExpression regex("^[A-Za-zÀ-ÖØ-öø-ÿ\\s'-]+$"); // Autorise lettres accentuées, espaces et tirets

    if (!regex.match(nom).hasMatch()) {
        QMessageBox::warning(this, "⚠️ Erreur de saisie", "Le champ 'Nom' ne doit contenir que des lettres !");
        return;
    }
    if (!regex.match(prenom).hasMatch()) {
        QMessageBox::warning(this, "⚠️ Erreur de saisie", "Le champ 'Prénom' ne doit contenir que des lettres !");
        return;
    }
    if (!regex.match(comp).hasMatch()) {
        QMessageBox::warning(this, "⚠️ Erreur de saisie", "Le champ 'Compétence' ne doit contenir que des lettres !");
        return;
    }
    if (!regex.match(zone).hasMatch()) {
        QMessageBox::warning(this, "⚠️ Erreur de saisie", "Le champ 'Zone' ne doit contenir que des lettres !");
        return;
    }

    // Vérifier la connexion à la base de données
    QSqlDatabase db = QSqlDatabase::database();
    if (!db.isOpen()) {
        QMessageBox::critical(this, "❌ Erreur de connexion",
                              "La connexion à la base de données n'est pas active !\n\nVeuillez vérifier votre connexion.");
        return;
    }

    // Création de l’objet personnel
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
        // Vérifier l'erreur SQL
        QSqlQuery testQuery;
        testQuery.prepare("SELECT 1 FROM PERSONNELS WHERE ID_PERSONNEL = :ID");
        testQuery.bindValue(":ID", id);

        QString errorMsg = "Échec d'ajout du personnel !\n\n";

        if (testQuery.exec() && testQuery.next()) {
            errorMsg += QString("L'ID '%1' existe déjà dans la base de données.\nVeuillez utiliser un ID différent.").arg(id);
        } else {
            QSqlQuery tableQuery;
            tableQuery.exec("SELECT COUNT(*) FROM PERSONNELS");
            if (tableQuery.lastError().isValid()) {
                QString sqlError = tableQuery.lastError().text();
                errorMsg += QString("Erreur SQL détectée:\n%1\n\n").arg(sqlError);
                errorMsg += "Vérifiez:\n";
                errorMsg += "- Que la table PERSONNELS existe\n";
                errorMsg += "- Que les colonnes sont correctes\n";
                errorMsg += "- Que la connexion à la base de données est active";
            } else {
                errorMsg += "Vérifiez:\n";
                errorMsg += "- Que l'ID n'existe pas déjà\n";
                errorMsg += "- Que les champs sont valides\n";
                errorMsg += "- Que la connexion est active";
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
