#ifndef HABITANTCRUD_H
#define HABITANTCRUD_H

#include <QString>
#include <QSqlQueryModel>
#include <QObject>

class HabitantCRUD
{
public:
    HabitantCRUD();
    HabitantCRUD(int id, QString nom, QString adresse);

    bool ajouter();
    QSqlQueryModel* afficher(QObject *parent = nullptr);
    bool supprimer(int id);
    bool modifier(int id);

private:
    int id;
    QString nom;
    QString adresse;
};

#endif // HABITANTCRUD_H
