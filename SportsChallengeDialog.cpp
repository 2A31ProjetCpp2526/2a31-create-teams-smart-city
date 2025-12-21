#include "SportsChallengeDialog.h"
#include <QVBoxLayout>
#include <QRandomGenerator>
#include <QMessageBox>
#include "mainwindow.h"

SportsChallengeDialog::SportsChallengeDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle("🏃 Sports Challenge!");
    resize(450, 300);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setSpacing(20);
    layout->setContentsMargins(30, 30, 30, 30);

    titleLabel = new QLabel("🏅 Today's Sports Challenge");
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("font-size: 28px; font-weight: bold;");
    layout->addWidget(titleLabel);

    challengeLabel = new QLabel();
    challengeLabel->setAlignment(Qt::AlignCenter);
    challengeLabel->setWordWrap(true);
    challengeLabel->setStyleSheet("font-size: 22px; color:#2C3A47; font-weight:600;");
    layout->addWidget(challengeLabel);

    closeButton = new QPushButton("✔ Done!");
    closeButton->setMinimumHeight(55);
    closeButton->setStyleSheet(
        "font-size: 22px; font-weight: bold;"
        "background-color:#00C896; color:white;"
        "border-radius:18px;"
        );
    layout->addWidget(closeButton);

    // Load challenge list + pick one
    loadChallenges();
    pickRandomChallenge();

    connect(closeButton, &QPushButton::clicked, this, [this]() {
        QMessageBox::information(this, "Great job! 🎉", "Proud of you!");
        close();
    });
}

void SportsChallengeDialog::loadChallenges()
{
    challenges = {
        "🏃 Run in place for 20 seconds!",
        "🏋️ Do 8 push-ups.",
        "🤸 Do 5 cartwheels or side jumps.",
        "🚶 Walk for 10 minutes.",
        "🧘 Hold a stretch for 30 seconds.",
        "👟 Do 15 jumping jacks!",
        "🦵 Do 10 squats.",
        "🏃‍♀️ Sprint in place for 15 seconds.",
        "🌀 Arm circles for 20 seconds.",
        "🌬 Deep breathing for 1 minute."
    };
}

void SportsChallengeDialog::pickRandomChallenge()
{
    int index = QRandomGenerator::global()->bounded(challenges.size());
    challengeLabel->setText(challenges[index]);
}
