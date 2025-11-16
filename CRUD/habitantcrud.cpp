#include "habitantcrud.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QDebug>
#include <QRegularExpression>

HabitantCRUD::HabitantCRUD() : id(0) {}

HabitantCRUD::HabitantCRUD(int id, QString nom, QString prenom, QString adresse,
                           QString zone, QString situation, QString contact,
                           QString statut, QString besoins, QString satisfaction)
    : id(id), nom(nom), prenom(prenom), adresse(adresse), zone(zone),
    situation(situation), contact(contact), statut(statut),
    besoins(besoins), satisfaction(satisfaction) {}

bool HabitantCRUD::validerChamps(QString &messageErreur)
{
    if (id <= 0) {
        messageErreur = "ID doit être un entier positif.";
        return false;
    }
    const QString nomT = nom.trimmed();
    const QString prenomT = prenom.trimmed();
    const QString adresseT = adresse.trimmed();
    const QString contactT = contact.trimmed();
    const QString satisfactionT = satisfaction.trimmed();

    if (nomT.isEmpty() || prenomT.isEmpty()) {
        messageErreur = "Nom et prénom obligatoires.";
        return false;
    }
    // Lettres, espaces, traits d'union
    QRegularExpression rxNom("^[A-Za-zÀ-ÖØ-öø-ÿ\-' ]{2,}$");
    if (!rxNom.match(nomT).hasMatch() || !rxNom.match(prenomT).hasMatch()) {
        messageErreur = "Nom/Prénom invalides (lettres, espaces, '-' uniquement).";
        return false;
    }
    if (adresseT.length() < 3) {
        messageErreur = "Adresse trop courte.";
        return false;
    }
    // Téléphone: 8 à 15 chiffres (option + au début géré côté UI si besoin)
    QRegularExpression rxPhone("^\\d{8,15}$");
    if (!contactT.isEmpty() && !rxPhone.match(contactT).hasMatch()) {
        messageErreur = "Contact doit contenir 8 à 15 chiffres.";
        return false;
    }
    // Satisfaction numérique 0..100 si renseigné
    if (!satisfactionT.isEmpty()) {
        bool ok = false; int sat = satisfactionT.toInt(&ok);
        if (!ok || sat < 0 || sat > 100) {
            messageErreur = "Satisfaction doit être un entier entre 0 et 100.";
            return false;
        }
    }

    return true;
}

// === AJOUTER ===
bool HabitantCRUD::ajouter()
{
    QSqlQuery query;
    // Vérifier doublon d'ID
    {
        QSqlQuery exists;
        exists.prepare("SELECT 1 FROM HABITANT WHERE ID_HABITANT = :id");
        exists.bindValue(":id", id);
        if (!exists.exec()) {
            QMessageBox::critical(nullptr, "Erreur SQL", exists.lastError().text());
            return false;
        }
        if (exists.next()) {
            QMessageBox::warning(nullptr, "Validation", "Un habitant avec cet ID existe déjà.");
            return false;
        }
    }
    query.prepare("INSERT INTO HABITANT (ID_HABITANT, NOM, PRENOM, ADRESSE, ZONE_RESIDENCE, "
                  "SITUATION_FAMILIALE, CONTACT, STATUT, BESOINS_SPECIFIQUES, NIVEAU_SATISFACTION) "
                  "VALUES (:id, :nom, :prenom, :adresse, :zone, :situation, :contact, :statut, :besoins, :satisfaction)");

    query.bindValue(":id", id);
    query.bindValue(":nom", nom);
    query.bindValue(":prenom", prenom);
    query.bindValue(":adresse", adresse);
    query.bindValue(":zone", zone);
    query.bindValue(":situation", situation);
    query.bindValue(":contact", contact);
    query.bindValue(":statut", statut);
    query.bindValue(":besoins", besoins);
    query.bindValue(":satisfaction", satisfaction);

    if (!query.exec()) {
        qDebug() << "❌ ERREUR SQL:" << query.lastError().text();
        qDebug() << "❌ Requête:" << query.lastQuery();
        QMessageBox::critical(nullptr, "Erreur SQL",
                              QString("%1\n\nRequête: %2")
                                  .arg(query.lastError().text(), query.lastQuery()));
        return false;
    }

    qDebug() << "✅ Ajout réussi pour l'habitant ID:" << id;
    return true;
}
// === AFFICHER ===
QSqlQueryModel* HabitantCRUD::afficher(QObject *parent)
{
    QSqlQueryModel *model = new QSqlQueryModel(parent);
    model->setQuery("SELECT ID_HABITANT, NOM, PRENOM, ADRESSE, ZONE_RESIDENCE, "
                    "SITUATION_FAMILIALE, CONTACT, STATUT, BESOINS_SPECIFIQUES, NIVEAU_SATISFACTION "
                    "FROM HABITANT ORDER BY ID_HABITANT");

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

    return model;
}

// === SUPPRIMER ===
bool HabitantCRUD::supprimer(int id)
{
    QSqlQuery query;
    query.prepare("DELETE FROM HABITANT WHERE ID_HABITANT = :id");
    query.bindValue(":id", id);

    if (!query.exec()) {
        QMessageBox::critical(nullptr, "Erreur SQL", query.lastError().text());
        qDebug() << "Erreur suppression:" << query.lastError().text();
        return false;
    }

    QMessageBox::information(nullptr, "Succès", "Habitant supprimé avec succès !");
    return true;
}

// === MODIFIER ===
bool HabitantCRUD::modifier(int id)
{
    QString erreur;
    if (!validerChamps(erreur)) {
        QMessageBox::warning(nullptr, "Erreur de validation", erreur);
        return false;
    }

    QSqlQuery query;
    query.prepare("UPDATE HABITANT SET "
                  "NOM = :nom, PRENOM = :prenom, ADRESSE = :adresse, "
                  "ZONE_RESIDENCE = :zone, SITUATION_FAMILIALE = :situation, "
                  "CONTACT = :contact, STATUT = :statut, "
                  "BESOINS_SPECIFIQUES = :besoins, NIVEAU_SATISFACTION = :satisfaction "
                  "WHERE ID_HABITANT = :id");

    query.bindValue(":id", id);
    query.bindValue(":nom", nom);
    query.bindValue(":prenom", prenom);
    query.bindValue(":adresse", adresse);
    query.bindValue(":zone", zone);
    query.bindValue(":situation", situation);
    query.bindValue(":contact", contact);
    query.bindValue(":statut", statut);
    query.bindValue(":besoins", besoins);
    query.bindValue(":satisfaction", satisfaction);

    if (!query.exec()) {
        QMessageBox::critical(nullptr, "Erreur SQL", query.lastError().text());
        qDebug() << "Erreur modification:" << query.lastError().text();
        return false;
    }

    QMessageBox::information(nullptr, "Succès", "Habitant modifié avec succès !");
    return true;
}
