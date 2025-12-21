#ifndef SPORTSCHALLENGEDIALOG_H
#define SPORTSCHALLENGEDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QPushButton>
#include <QVector>

class SportsChallengeDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SportsChallengeDialog(QWidget *parent = nullptr);

private:
    QLabel *titleLabel;
    QLabel *challengeLabel;
    QPushButton *closeButton;

    QVector<QString> challenges;

    void loadChallenges();
    void pickRandomChallenge();
};

#endif // SPORTSCHALLENGEDIALOG_H
