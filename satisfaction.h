#ifndef SATISFACTION_H
#define SATISFACTION_H

#include <QString>
#include <QSqlQueryModel>

class SatisfactionCRUD
{
private:
    int id;                // id_satisfaction (PK)
    int idHabitant;        // FK -> HABITANT.ID_HABITANT
    int idDemande;         // FK -> DEMANDE.ID_DEMANDE (nullable)
    int score;             // 0..100
    QString source;        // optionnel
    QString commentaire;   // optionnel

public:
    SatisfactionCRUD();
    SatisfactionCRUD(int idHabitant, int score, int idDemande = 0, const QString &source = QString(), const QString &commentaire = QString());

    // CRUD
    bool ajouter(QString &messageErreur) const;
    bool modifier(int idSatisfaction, QString &messageErreur) const;
    static bool supprimer(int idSatisfaction, QString &messageErreur);

    // Affichages
    static QSqlQueryModel* afficher(QObject *parent = nullptr);
    static QSqlQueryModel* afficherParHabitant(int idHabitant, QObject *parent = nullptr);
};

#endif // SATISFACTION_H
