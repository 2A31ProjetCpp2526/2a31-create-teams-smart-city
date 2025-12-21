#include "DemandeDialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QMessageBox>
#include <QDebug>
#include <QTime>
#include <QTableWidgetItem>
#include "connection.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QInputDialog>
#include <QHeaderView>
#include <QTableWidgetItem>
#include <QBrush>
#include <QColor>
#include <QtMath>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlDatabase>

// ⚡ Simulation d’une "base" en mémoire
struct Poubelle {
    static QString classifierPoubelle(int nb, double taux, int jours){
        if(nb>10 || taux>70) return "Saturée";
        if(nb>5 || taux>40) return "Remplie";
        return "Propre";
    }
};

// Couleur texte lisible
static void setCellColor(QTableWidgetItem *item, const QColor &bgColor)
{
    item->setBackground(bgColor);
    int brightness = (bgColor.red()*299 + bgColor.green()*587 + bgColor.blue()*114)/1000;
    item->setForeground(brightness < 128 ? Qt::white : Qt::black);
}

// ------------------------------
DemandeDialog::DemandeDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle("Gestion des Demandes");
    resize(800,500);

    setupUI();
    chargerDemandes();
}

// ------------------------------
void DemandeDialog::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    tableDemandes = new QTableWidget(this);
    tableDemandes->setColumnCount(4);
    tableDemandes->setHorizontalHeaderLabels({"Zone ID","État","Date","Maintenance"});
    tableDemandes->horizontalHeader()->setStretchLastSection(true);
    tableDemandes->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tableDemandes->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tableDemandes->setAlternatingRowColors(true);
    tableDemandes->verticalHeader()->setDefaultSectionSize(30);
    mainLayout->addWidget(tableDemandes);

    QHBoxLayout *formLayout = new QHBoxLayout;
    zoneIdEdit = new QLineEdit(this); zoneIdEdit->setPlaceholderText("Zone ID");
    maintenanceEdit = new QLineEdit(this); maintenanceEdit->setPlaceholderText("nb;taux;jours");
    maintenanceCombo = new QComboBox(this);
    maintenanceCombo->addItems({"Faible","Moyenne","Élevée","Stop Arrosage"});
    dateEdit = new QDateTimeEdit(QDateTime::currentDateTime(),this);
    dateEdit->setDisplayFormat("yyyy-MM-dd HH:mm:ss");
    formLayout->addWidget(zoneIdEdit);
    formLayout->addWidget(maintenanceEdit);
    formLayout->addWidget(maintenanceCombo);
    formLayout->addWidget(dateEdit);
    mainLayout->addLayout(formLayout);

    QHBoxLayout *btnLayout = new QHBoxLayout;
    btnAjouter = new QPushButton("Ajouter",this);
    QPushButton *btnRafraichir = new QPushButton("Rafraîchir",this);
    btnLayout->addWidget(btnAjouter); btnLayout->addWidget(btnRafraichir);
    mainLayout->addLayout(btnLayout);

    connect(btnAjouter,&QPushButton::clicked,this,&DemandeDialog::ajouterDemande);
    connect(btnRafraichir,&QPushButton::clicked,this,&DemandeDialog::rafraichirDemandes);
}

// ------------------------------
bool DemandeDialog::estPluvieux() const {
    int h = QTime::currentTime().hour();
    return h>=18 || h<=6;
}

// ------------------------------
bool DemandeDialog::estConnecte() const { return true; } // simulation

// ------------------------------


// ------------------------------


// ------------------------------
void DemandeDialog::addDemandeRow(const QString &zoneId,const QString &etat,const QDateTime &date,const QString &maintenance)
{
    int row = tableDemandes->rowCount();
    tableDemandes->insertRow(row);
    tableDemandes->setItem(row,0,new QTableWidgetItem(zoneId));

    QTableWidgetItem *etatItem = new QTableWidgetItem(etat);
    if(etat=="Propre") setCellColor(etatItem,Qt::green);
    else if(etat=="Remplie") setCellColor(etatItem,Qt::yellow);
    else if(etat=="Saturée") setCellColor(etatItem,Qt::darkYellow);
    else if(etat=="Débordante") setCellColor(etatItem,Qt::red);
    tableDemandes->setItem(row,1,etatItem);

    tableDemandes->setItem(row,2,new QTableWidgetItem(date.toString("yyyy-MM-dd HH:mm:ss")));
    tableDemandes->setItem(row,3,new QTableWidgetItem(maintenance));
}

// ------------------------------
void DemandeDialog::ajouterDemande()
{
    if(zoneIdEdit->text().isEmpty()){ QMessageBox::warning(this,"Erreur","Zone ID obligatoire"); return; }

    QString etat="Propre"; QString maintenance;

    bool classified=false;
    if(!maintenanceEdit->text().isEmpty()){
        QStringList p=maintenanceEdit->text().split(';');
        if(p.size()==3){
            bool ok1,ok2,ok3;
            int nb=p[0].toInt(&ok1);
            double taux=p[1].toDouble(&ok2);
            int jours=p[2].toInt(&ok3);
            if(ok1 && ok2 && ok3){
                etat=Poubelle::classifierPoubelle(nb,taux,jours);
                maintenance=maintenanceEdit->text();
                classified=true;
            }
        }
    }

    if(!classified){
        maintenance=maintenanceCombo->currentText();
        if(maintenance=="Faible") etat="Propre";
        else if(maintenance=="Moyenne") etat="Remplie";
        else if(maintenance=="Élevée"){ etat="Saturée"; maintenance="Ramassage"; }
        else if(maintenance=="Stop Arrosage") etat="Débordante";
    }

    if(estPluvieux()){ maintenance+=" - Stop Arrosage"; if(etat=="Propre") etat="Remplie"; }

    creerDemande(zoneIdEdit->text(),etat,dateEdit->dateTime(),maintenance);
    addDemandeRow(zoneIdEdit->text(),etat,dateEdit->dateTime(),maintenance);

    zoneIdEdit->clear(); maintenanceEdit->clear();
}

// ------------------------------

bool DemandeDialog::creerDemande(const QString &zoneId,
                                 const QString &etat,
                                 const QDateTime &date,
                                 const QString &maintenance)
{
    if (!estConnecte()) {
        QMessageBox::critical(this, "Erreur", "Connexion Oracle non établie");
        return false;
    }

    QSqlQuery query;
    query.prepare(
        "INSERT INTO SAIM.DEMANDE1 "
        "(ZONE_ID, ETAT, DATE_DEMANDE, MAINTENACE) "
        "VALUES (:zone, :etat, :date_demande, :maintenance)"
        );

    query.bindValue(":zone", zoneId);
    query.bindValue(":etat", etat);
    query.bindValue(":date_demande", date);
    query.bindValue(":maintenance", maintenance);

    if (!query.exec()) {
        QMessageBox::critical(this,
                              "Erreur Oracle",
                              query.lastError().text());
        return false;
    }

    return true;
}
void DemandeDialog::chargerDemandes()
{
    tableDemandes->setRowCount(0);

    if (!estConnecte()) return;

    QSqlQuery query(
        "SELECT ZONE_ID, ETAT, DATE_DEMANDE, MAINTENACE "
        "FROM SAIM.DEMANDE1 "
        "ORDER BY DATE_DEMANDE DESC"
        );

    while (query.next()) {
        addDemandeRow(
            query.value("ZONE_ID").toString(),
            query.value("ETAT").toString(),
            query.value("DATE_DEMANDE").toDateTime(),
            query.value("MAINTENACE").toString()
            );
    }
}
void DemandeDialog::rafraichirDemandes() {
    chargerDemandes();
}
