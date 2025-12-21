#include "maintenance.h"

#include <QWidget>
#include <QLineEdit>
#include <QComboBox>
#include <QTextBrowser>
#include <QLabel>
#include <QTableView>
#include <QPushButton>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QMessageBox>
#include <QDate>
#include <QDateTime>
#include <QDebug>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlDatabase>
#include <QSqlQueryModel>



Maintenance::Maintenance(QWidget *root, QObject *parent)
    : QObject(parent)
    , m_root(root)
{
    initHistory();

    // Connect buttons if they exist
    if (m_root) {
        if (auto btn = m_root->findChild<QPushButton*>("btnMaint_diagnostiquer")) {
            connect(btn, &QPushButton::clicked, this, &Maintenance::diagnostiquer);
            qDebug() << "Maintenance: btnMaint_diagnostiquer connected";
        } else {
            qDebug() << "Maintenance: btnMaint_diagnostiquer NOT FOUND";
        }

        if (auto btn = m_root->findChild<QPushButton*>("btnMaint_ajouter")) {
            connect(btn, &QPushButton::clicked, this, &Maintenance::ajouter);
            qDebug() << "Maintenance: btnMaint_ajouter connected";
        } else {
            qDebug() << "Maintenance: btnMaint_ajouter NOT FOUND";
        }

        if (auto btn = m_root->findChild<QPushButton*>("btnMaint_effacer")) {
            connect(btn, &QPushButton::clicked, this, &Maintenance::effacer);
            qDebug() << "Maintenance: btnMaint_effacer connected";
        } else {
            qDebug() << "Maintenance: btnMaint_effacer NOT FOUND";
        }
    }
}

void Maintenance::initHistory()
{
    if (m_historyModel) return;
    if (!m_root) return;

    QTableView *tv = m_root->findChild<QTableView*>("tableMaint_history");
    if (!tv) return;

    m_historyModel = new QStandardItemModel(this);
    m_historyModel->setColumnCount(6);
    m_historyModel->setHeaderData(0, Qt::Horizontal, "ID véhicule");
    m_historyModel->setHeaderData(1, Qt::Horizontal, "Matricule");
    m_historyModel->setHeaderData(2, Qt::Horizontal, "Problème choisi");
    m_historyModel->setHeaderData(3, Qt::Horizontal, "Alerte");
    m_historyModel->setHeaderData(4, Qt::Horizontal, "Diagnostic");
    m_historyModel->setHeaderData(5, Qt::Horizontal, "Date");

    tv->setModel(m_historyModel);
    tv->setSelectionBehavior(QAbstractItemView::SelectRows);
    tv->setSelectionMode(QAbstractItemView::SingleSelection);
    tv->setSortingEnabled(true);
    tv->resizeColumnsToContents();

    chargerHistoriqueDepuisBase(tv);
}

void Maintenance::getDiagnosticForProblem(const QString &probleme, QString &alerte, QString &diagnostic)
{
    QString prob = probleme.trimmed().toLower();

    // Initialiser les valeurs par défaut
    alerte = "🟢 Problème mineur, pas urgent";
    diagnostic = "Aucun diagnostic disponible pour ce problème.";

    // Le moteur chauffe
    if (prob.contains("moteur") && prob.contains("chauff")) {
        alerte = "❌ Moteur surchauffe = risque de casse moteur → entretien urgent";
        diagnostic = "<b>Origine probable :</b><br>"
                     "• Radiateur bouché ou défectueux<br>"
                     "• Thermostat bloqué<br>"
                     "• Pompe à eau défaillante<br>"
                     "• Niveau de liquide de refroidissement insuffisant<br><br>"
                     "<b>Recommandation :</b><br>"
                     "→ Arrêter immédiatement le véhicule<br>"
                     "→ Vérifier le niveau de liquide de refroidissement<br>"
                     "→ Nettoyer le radiateur<br>"
                     "→ Aller au garage pour diagnostic complet";
    }
    // La voiture tremble
    else if (prob.contains("trembl")) {
        alerte = "⚠️ Voiture qui tremble = problème de stabilité → vérification nécessaire";
        diagnostic = "<b>Origine probable :</b><br>"
                     "• Pneus déséquilibrés<br>"
                     "• Rotules ou biellettes usées<br>"
                     "• Amortisseurs défectueux<br>"
                     "• Disques de frein voilés<br><br>"
                     "<b>Recommandation :</b><br>"
                     "→ Vérifier l'équilibrage des pneus<br>"
                     "→ Contrôler la géométrie<br>"
                     "→ Faire vérifier les amortisseurs";
    }
    // La batterie se décharge
    else if (prob.contains("batterie") && (prob.contains("décharg") || prob.contains("decharg"))) {
        alerte = "⚠️ Batterie faible = risque de panne → vérification urgente";
        diagnostic = "<b>Origine probable :</b><br>"
                     "• Alternateur défectueux<br>"
                     "• Batterie en fin de vie<br>"
                     "• Consommation électrique excessive<br>"
                     "• Mauvais contact aux bornes<br><br>"
                     "<b>Recommandation :</b><br>"
                     "→ Tester la tension de la batterie<br>"
                     "→ Vérifier l'alternateur<br>"
                     "→ Nettoyer les bornes de la batterie<br>"
                     "→ Remplacer la batterie si nécessaire";
    }
    // Freins faibles
    else if (prob.contains("frein") && prob.contains("faibl")) {
        alerte = "⚠️ Freins faibles = danger potentiel → intervention urgente";
        diagnostic = "<b>Origine probable :</b><br>"
                     "• Plaquettes de frein usées<br>"
                     "• Disques de frein usés<br>"
                     "• Niveau de liquide de frein bas<br>"
                     "• Fuite dans le circuit de freinage<br><br>"
                     "<b>Recommandation :</b><br>"
                     "→ Vérifier immédiatement les freins<br>"
                     "→ Contrôler le niveau de liquide<br>"
                     "→ Remplacer les plaquettes si nécessaire<br>"
                     "→ Aller au garage en urgence";
    }
    // Fuite d'huile
    else if (prob.contains("fuite") && prob.contains("huile")) {
        alerte = "❌ Fuite d'huile = risque de casse moteur → entretien urgent";
        diagnostic = "<b>Origine probable :</b><br>"
                     "• Joint de culasse défectueux<br>"
                     "• Bouchon de vidange mal serré<br>"
                     "• Filtre à huile mal monté<br>"
                     "• Carter moteur percé<br><br>"
                     "<b>Recommandation :</b><br>"
                     "→ Vérifier le niveau d'huile régulièrement<br>"
                     "→ Localiser la source de la fuite<br>"
                     "→ Aller au garage pour réparation";
    }
    // Fumée noire
    else if (prob.contains("fumée") || prob.contains("fumee") || prob.contains("fume")) {
        alerte = "⚠️ Fumée noire = problème de combustion → vérification nécessaire";
        diagnostic = "<b>Origine probable :</b><br>"
                     "• Mélange air/carburant trop riche<br>"
                     "• Filtre à air encrassé<br>"
                     "• Injecteurs défectueux<br>"
                     "• Sonde lambda défaillante<br><br>"
                     "<b>Recommandation :</b><br>"
                     "→ Vérifier le filtre à air<br>"
                     "→ Contrôler les injecteurs<br>"
                     "→ Faire un diagnostic électronique";
    }
    // Voiture ne démarre pas
    else if (prob.contains("démarre") || prob.contains("demarre") || prob.contains("démar")) {
        alerte = "❌ Voiture ne démarre pas = problème critique → intervention immédiate";
        diagnostic = "<b>Origine probable :</b><br>"
                     "• Batterie déchargée<br>"
                     "• Démarreur défectueux<br>"
                     "• Problème de carburant<br>"
                     "• Bougies ou bobine défaillantes<br><br>"
                     "<b>Recommandation :</b><br>"
                     "→ Vérifier la batterie<br>"
                     "→ Tester le démarreur<br>"
                     "→ Contrôler le niveau de carburant<br>"
                     "→ Appeler un dépanneur si nécessaire";
    }
    // Consommation élevée
    else if (prob.contains("consommation") && (prob.contains("élev") || prob.contains("elev"))) {
        alerte = "⚠️ Consommation élevée = problème d'efficacité → vérification recommandée";
        diagnostic = "<b>Origine probable :</b><br>"
                     "• Filtre à air encrassé<br>"
                     "• Pneus sous-gonflés<br>"
                     "• Problème de carburant<br>"
                     "• Moteur mal réglé<br><br>"
                     "<b>Recommandation :</b><br>"
                     "→ Vérifier la pression des pneus<br>"
                     "→ Remplacer le filtre à air<br>"
                     "→ Faire un diagnostic moteur";
    }
    // Bruit métallique
    else if (prob.contains("bruit") && (prob.contains("métall") || prob.contains("metall"))) {
        alerte = "❌ Bruit métallique = problème mécanique grave → intervention urgente";
        diagnostic = "<b>Origine probable :</b><br>"
                     "• Roulement défectueux<br>"
                     "• Courroie de distribution usée<br>"
                     "• Pièces mécaniques usées<br>"
                     "• Problème de transmission<br><br>"
                     "<b>Recommandation :</b><br>"
                     "→ Arrêter le véhicule immédiatement<br>"
                     "→ Ne pas conduire<br>"
                     "→ Appeler un garage en urgence";
    }
    // Pneus s'usent vite
    else if (prob.contains("pneu") && (prob.contains("us") || prob.contains("usent"))) {
        alerte = "⚠️ Pneus s'usent vite = problème d'alignement → vérification nécessaire";
        diagnostic = "<b>Origine probable :</b><br>"
                     "• Géométrie des roues incorrecte<br>"
                     "• Pression des pneus inadaptée<br>"
                     "• Amortisseurs défectueux<br>"
                     "• Suspension usée<br><br>"
                     "<b>Recommandation :</b><br>"
                     "→ Vérifier la pression des pneus<br>"
                     "→ Faire un contrôle de géométrie<br>"
                     "→ Vérifier les amortisseurs";
    }
}

void Maintenance::diagnostiquer()
{
    if (!m_root) return;

    QLineEdit *idEdit = m_root->findChild<QLineEdit*>("lineMaint_id");
    QLineEdit *matriculeEdit = m_root->findChild<QLineEdit*>("lineMaint_matricule");
    QString idVehicule = idEdit ? idEdit->text().trimmed() : QString();
    QString matricule = matriculeEdit ? matriculeEdit->text().trimmed() : QString();

    if (idVehicule.isEmpty() || matricule.isEmpty()) {
        QMessageBox::warning(m_root, "Maintenance", "Veuillez saisir l'ID du véhicule et le matricule.");
        return;
    }

    if (!vehiculeExiste(idVehicule, matricule)) {
        QMessageBox::warning(m_root, "Véhicule inexistant",
                             "Aucun véhicule avec cet ID et ce matricule n'existe.\n"
                             "Veuillez d'abord le créer dans la gestion des véhicules.");
        return;
    }

    // Essayer les deux noms possibles pour le ComboBox
    QComboBox *problemeCombo = m_root->findChild<QComboBox*>("comboMaint_probleme");
    if (!problemeCombo) {
        problemeCombo = m_root->findChild<QComboBox*>("comboMaint_type");
    }

    QLabel *alertes = m_root->findChild<QLabel*>("textMaint_alertes");
    QLabel *diag = m_root->findChild<QLabel*>("textMaint_diag");

    // Vérifier que le problème est sélectionné
    if (!problemeCombo) {
        QMessageBox::warning(m_root, "Erreur", "Le ComboBox de problème est introuvable. Cherché: comboMaint_probleme et comboMaint_type");
        qDebug() << "ComboBox not found! Searching all QComboBox...";
        QList<QComboBox*> allCombos = m_root->findChildren<QComboBox*>();
        QStringList names;
        for (QComboBox *c : allCombos) {
            names << c->objectName();
        }
        qDebug() << "Found QComboBox:" << names;
        return;
    }

    qDebug() << "ComboBox trouvé:" << problemeCombo->objectName();

    QString probleme = problemeCombo->currentText();
    // Enlever le préfixe "Problème : " si présent
    if (probleme.startsWith("Problème : ")) {
        probleme = probleme.mid(11); // Enlever "Problème : "
    }
    if (probleme.isEmpty() || probleme == "Sélectionner un problème" || probleme.contains("Sélectionner")) {
        QMessageBox::warning(m_root, "Erreur", "Veuillez sélectionner un problème.");
        qDebug() << "Problème invalide:" << probleme;
        return;
    }
    qDebug() << "Problème validé:" << probleme;

    // Obtenir l'alerte et le diagnostic
    QString alerte, diagnostic;
    getDiagnosticForProblem(probleme, alerte, diagnostic);

    // Afficher l'alerte (QLabel)
    if (alertes) {
        alertes->setText(alerte);
        alertes->setWordWrap(true);
        alertes->setAlignment(Qt::AlignLeft | Qt::AlignTop);
        // Appliquer une couleur selon le type d'alerte avec fond clair et texte visible
        if (alerte.startsWith("❌")) {
            alertes->setStyleSheet("QLabel { color: #cc0000; font-weight: bold; font-size: 11pt; background-color: #ffe6e6; padding: 8px; border: 2px solid #cc0000; border-radius: 5px; }");
        } else if (alerte.startsWith("⚠️")) {
            alertes->setStyleSheet("QLabel { color: #b8860b; font-weight: bold; font-size: 11pt; background-color: #fff4e6; padding: 8px; border: 2px solid #e6a700; border-radius: 5px; }");
        } else {
            alertes->setStyleSheet("QLabel { color: #006600; font-weight: bold; font-size: 11pt; background-color: #e6ffe6; padding: 8px; border: 2px solid #006600; border-radius: 5px; }");
        }
        qDebug() << "Alerte affichée:" << alerte;
    } else {
        QMessageBox::warning(m_root, "Erreur", "Le widget 'textMaint_alertes' est introuvable.");
        qDebug() << "textMaint_alertes not found!";
    }

    // Afficher le diagnostic (QLabel)
    if (diag) {
        // Convertir le HTML en texte simple pour QLabel
        QString diagText = diagnostic;
        diagText.replace("<br>", "\n");
        diagText.replace("<b>", "");
        diagText.replace("</b>", "");
        // Garder les puces
        diagText.replace("•", "• ");

        QString fullText = "Problème : " + probleme + "\n\n" + diagText;
        diag->setText(fullText);
        diag->setWordWrap(true);
        diag->setAlignment(Qt::AlignLeft | Qt::AlignTop);
        // Style avec fond blanc et texte noir bien visible
        diag->setStyleSheet("QLabel { color: #000000; background-color: #ffffff; font-size: 10pt; padding: 8px; border: 2px solid #5dade2; border-radius: 5px; }");
        qDebug() << "Diagnostic affiché pour problème:" << probleme;
    } else {
        QMessageBox::warning(m_root, "Erreur", "Le widget 'textMaint_diag' est introuvable.");
        qDebug() << "textMaint_diag not found!";
    }
}

void Maintenance::ajouter()
{
    initHistory();
    if (!m_historyModel || !m_root) return;

    QTableView *tv = m_root->findChild<QTableView*>("tableMaint_history");
    if (!tv) {
        QMessageBox::warning(m_root, "Erreur", "La table d'historique est introuvable.");
        return;
    }

    QLineEdit *idEdit = m_root->findChild<QLineEdit*>("lineMaint_id");
    QLineEdit *matriculeEdit = m_root->findChild<QLineEdit*>("lineMaint_matricule");

    // Essayer les deux noms possibles pour le ComboBox
    QComboBox *problemeCombo = m_root->findChild<QComboBox*>("comboMaint_probleme");
    if (!problemeCombo) {
        problemeCombo = m_root->findChild<QComboBox*>("comboMaint_type");
    }

    QLabel *alertes = m_root->findChild<QLabel*>("textMaint_alertes");
    QLabel *diag = m_root->findChild<QLabel*>("textMaint_diag");

    if (!problemeCombo) {
        QMessageBox::warning(m_root, "Erreur", "Le ComboBox de problème est introuvable.");
        qDebug() << "ComboBox not found in ajouter()";
        return;
    }

    // Vérifier que les champs obligatoires sont remplis
    QString idText = idEdit ? idEdit->text().trimmed() : QString();
    QString matriculeText = matriculeEdit ? matriculeEdit->text().trimmed() : QString();
    QString problemeText = problemeCombo ? problemeCombo->currentText() : QString();

    // Enlever le préfixe "Problème : " si présent
    if (problemeText.startsWith("Problème : ")) {
        problemeText = problemeText.mid(11);
    }

    if (idText.isEmpty()) {
        QMessageBox::warning(m_root, "Erreur", "Veuillez saisir l'ID du véhicule.");
        return;
    }
    bool okIdNumber = false;
    int idValue = idText.toInt(&okIdNumber);
    if (!okIdNumber || idValue <= 0) {
        QMessageBox::warning(m_root, "Erreur", "L'ID du véhicule doit être un nombre entier positif.");
        return;
    }

    if (matriculeText.isEmpty()) {
        QMessageBox::warning(m_root, "Erreur", "Veuillez saisir le matricule du véhicule.");
        return;
    }

    if (problemeText.isEmpty() || problemeText == "Sélectionner un problème" || problemeText.contains("Sélectionner")) {
        QMessageBox::warning(m_root, "Erreur", "Veuillez sélectionner un problème.");
        qDebug() << "Problème invalide dans ajouter:" << problemeText;
        return;
    }

    if (!vehiculeExiste(idText, matriculeText)) {
        QMessageBox::warning(m_root, "Véhicule inexistant",
                             "Aucun véhicule avec cet ID et ce matricule n'existe.\n"
                             "Veuillez d'abord le créer dans la gestion des véhicules.");
        return;
    }

    qDebug() << "Ajout - ID:" << idText << "Matricule:" << matriculeText << "Problème:" << problemeText;

    // Récupérer l'alerte et le diagnostic depuis les QLabel
    QString alerteText = alertes ? alertes->text().trimmed() : QString();
    QString diagText = diag ? diag->text().trimmed() : QString();

    // Si les labels sont vides, générer l'alerte et le diagnostic
    if (alerteText.isEmpty() || diagText.isEmpty()) {
        qDebug() << "Labels vides, génération de l'alerte et diagnostic pour:" << problemeText;
        getDiagnosticForProblem(problemeText, alerteText, diagText);
        // Convertir le diagnostic HTML en texte simple
        diagText.replace("<br>", "\n");
        diagText.replace("<b>", "");
        diagText.replace("</b>", "");
        diagText.replace("•", "• ");
        // Enlever le préfixe "Problème : " du diagnostic si présent
        if (diagText.startsWith("Problème : ")) {
            int idx = diagText.indexOf("\n\n");
            if (idx > 0) {
                diagText = diagText.mid(idx + 2);
            }
        }
    }

    qDebug() << "Alerte:" << alerteText.left(50) << "...";
    qDebug() << "Diagnostic:" << diagText.left(50) << "...";

    // Date actuelle
    QString dateText = QDateTime::currentDateTime().toString("dd/MM/yyyy HH:mm");

    // Première tentative: sauvegarder dans la base
    QString erreurSql;
    if (enregistrerHistoriqueSql(idValue, matriculeText, problemeText, alerteText, diagText, erreurSql)) {
        chargerHistoriqueDepuisBase(tv);
        QMessageBox::information(m_root, "Succès", "Maintenance ajoutée à l'historique.");
        return;
    } else {
        if (!erreurSql.isEmpty()) {
            qDebug() << "Insertion historique maintenance échouée:" << erreurSql;
            QMessageBox::warning(m_root, "Maintenance",
                                 "Impossible d'enregistrer l'historique dans la base.\n"
                                 "Les données seront stockées localement.\n\nDétail: " + erreurSql);
        }
    }

    // Fallback: s'appuyer sur le modèle local
    if (tv->model() != m_historyModel) {
        tv->setModel(m_historyModel);
    }

    if (!m_historyModel) {
        qDebug() << "m_historyModel est null, initialisation...";
        initHistory();
        if (!m_historyModel) {
            QMessageBox::critical(m_root, "Erreur", "Impossible d'initialiser le modèle de la table d'historique.");
            return;
        }
    }

    int row = m_historyModel->rowCount();
    m_historyModel->insertRow(row);
    m_historyModel->setItem(row, 0, new QStandardItem(idText));
    m_historyModel->setItem(row, 1, new QStandardItem(matriculeText));
    m_historyModel->setItem(row, 2, new QStandardItem(problemeText));
    m_historyModel->setItem(row, 3, new QStandardItem(alerteText));
    m_historyModel->setItem(row, 4, new QStandardItem(diagText));
    m_historyModel->setItem(row, 5, new QStandardItem(dateText));

    tv->resizeColumnsToContents();
    tv->scrollTo(m_historyModel->index(row, 0));

    QMessageBox::information(m_root, "Succès", "Maintenance ajoutée localement (base indisponible).");
}

void Maintenance::effacer()
{
    if (!m_root) return;

    // Effacer les champs de saisie
    if (auto e = m_root->findChild<QLineEdit*>("lineMaint_id")) e->clear();
    if (auto e = m_root->findChild<QLineEdit*>("lineMaint_matricule")) e->clear();

    // Réinitialiser le ComboBox de problème
    if (auto c = m_root->findChild<QComboBox*>("comboMaint_probleme")) {
        c->setCurrentIndex(0);
    }

    // Effacer les zones d'alerte et diagnostic
    if (auto lbl = m_root->findChild<QLabel*>("textMaint_alertes")) {
        lbl->clear();
        lbl->setStyleSheet("");
    }
    if (auto lbl = m_root->findChild<QLabel*>("textMaint_diag")) {
        lbl->clear();
        lbl->setStyleSheet("");
    }
}

bool Maintenance::chargerHistoriqueDepuisBase(QTableView *tv)
{
    if (!m_root) return false;
    if (!tv) tv = m_root->findChild<QTableView*>("tableMaint_history");
    if (!tv) return false;

    QSqlQueryModel *model = new QSqlQueryModel(tv);
    model->setQuery("SELECT ID_VEHICULE, MATRICULE, PROBLEME, ALERTE, DIAGNOSTIC, "
                    "TO_CHAR(DATE_SAISIE, 'DD/MM/YYYY HH24:MI') AS DATE_SAISIE "
                    "FROM MAINTENANCE_HISTORY ORDER BY DATE_SAISIE DESC");

    if (model->lastError().isValid()) {
        qDebug() << "Impossible de charger l'historique maintenance depuis la base:"
                   << model->lastError().text();
        delete model;
        tv->setModel(m_historyModel);
        return false;
    }

    model->setHeaderData(0, Qt::Horizontal, "ID Véhicule");
    model->setHeaderData(1, Qt::Horizontal, "Matricule");
    model->setHeaderData(2, Qt::Horizontal, "Problème choisi");
    model->setHeaderData(3, Qt::Horizontal, "Alerte");
    model->setHeaderData(4, Qt::Horizontal, "Diagnostic");
    model->setHeaderData(5, Qt::Horizontal, "Date");

    tv->setModel(model);
    tv->setSelectionBehavior(QAbstractItemView::SelectRows);
    tv->setSelectionMode(QAbstractItemView::SingleSelection);
    tv->setSortingEnabled(true);
    tv->resizeColumnsToContents();
    return true;
}

bool Maintenance::enregistrerHistoriqueSql(int idVehicule,
                                           const QString &matricule,
                                           const QString &probleme,
                                           const QString &alerte,
                                           const QString &diagnostic,
                                           QString &erreurSql)
{
    QSqlQuery query;
    query.prepare("INSERT INTO MAINTENANCE_HISTORY "
                  "(ID_VEHICULE, MATRICULE, PROBLEME, ALERTE, DIAGNOSTIC, DATE_SAISIE) "
                  "VALUES (:id, :matricule, :probleme, :alerte, :diagnostic, SYSDATE)");
    query.bindValue(":id", idVehicule);
    query.bindValue(":matricule", matricule.trimmed());
    query.bindValue(":probleme", probleme);
    query.bindValue(":alerte", alerte);
    query.bindValue(":diagnostic", diagnostic);

    if (!query.exec()) {
        erreurSql = query.lastError().text();
        return false;
    }

    return true;
}

bool Maintenance::vehiculeExiste(const QString &idVehicule, const QString &matricule) const
{
    bool ok = false;
    int id = idVehicule.toInt(&ok);
    if (!ok || id <= 0) {
        qDebug() << "vehiculeExiste: ID invalide" << idVehicule;
        return false;
    }

    const QString matriculeTrim = matricule.trimmed();
    if (matriculeTrim.isEmpty()) {
        qDebug() << "vehiculeExiste: matricule vide";
        return false;
    }

    QSqlQuery query;
    query.prepare("SELECT COUNT(*) FROM VEHICULE WHERE ID_VEHICULE = :id AND MATRICULE = :matricule");
    query.bindValue(":id", id);
    query.bindValue(":matricule", matriculeTrim);

    if (!query.exec()) {
        qDebug() << "vehiculeExiste: erreur SQL" << query.lastError().text();
        return false;
    }

    if (query.next()) {
        return query.value(0).toInt() > 0;
    }

    return false;
}
