#include "SortingGameWidget.h"
#include <QMouseEvent>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDrag>
#include <QRandomGenerator>
#include <QMimeData>

SortingGameWidget::SortingGameWidget(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle("Let's have FUN!");
    resize(700, 450);

    // QDialog needs this to allow drag & drop
    setAcceptDrops(true);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(20);

    scoreLabel = new QLabel("Score: 0", this);
    scoreLabel->setStyleSheet("font-size: 26px; font-weight: bold;");
    mainLayout->addWidget(scoreLabel, 0, Qt::AlignCenter);

    // -----------------------------------------------------
    // Bins row
    // -----------------------------------------------------
    QHBoxLayout *binsLayout = new QHBoxLayout;

    plasticBin = createBin("plastic ♻️", "#6ECB63");
    paperBin   = createBin("paper 📄", "#57C5B6");
    organicBin = createBin("organic 🍌", "#FFB562");

    binsLayout->addWidget(plasticBin);
    binsLayout->addWidget(paperBin);
    binsLayout->addWidget(organicBin);

    mainLayout->addLayout(binsLayout);

    // -----------------------------------------------------
    // Load items & show first one
    // -----------------------------------------------------
    loadItems();
    spawnRandomItem();

    // Optional: Close button
    QPushButton *closeBtn = new QPushButton("Close", this);
    closeBtn->setStyleSheet("font-size: 20px; padding: 10px;");
    mainLayout->addWidget(closeBtn, 0, Qt::AlignCenter);
    connect(closeBtn, &QPushButton::clicked, this, &QDialog::accept);
}

QLabel* SortingGameWidget::createBin(const QString &text, const QString &color)
{
    QLabel *label = new QLabel(text);
    label->setAlignment(Qt::AlignCenter);
    label->setFixedSize(200, 180);
    label->setAcceptDrops(true);

    label->setStyleSheet(QString(
                             "background:%1; color:white; font-size:22px; "
                             "border-radius:20px; font-weight:bold;"
                             ).arg(color));

    return label;
}

void SortingGameWidget::loadItems()
{
    items = {
        {"🧴", "plastic"},
        {"🥤", "plastic"},
        {"📦", "paper"},
        {"📰", "paper"},
        {"🍌", "organic"},
        {"🍎", "organic"}
    };
}

void SortingGameWidget::spawnRandomItem()
{
    if (!item)
        item = new QLabel(this);

    int index = QRandomGenerator::global()->bounded(items.size());
    WasteItem w = items[index];

    item->setText(w.emoji);
    item->setStyleSheet("font-size:60px;");
    item->setFixedSize(80, 80);
    item->move(width()/2 - 40, 50);
    item->show();

    item->setObjectName(w.type); // save type
}

void SortingGameWidget::mousePressEvent(QMouseEvent *event)
{
    if (!item) return;

    if (item->geometry().contains(event->pos())) {
        QDrag *drag = new QDrag(this);
        QMimeData *mime = new QMimeData;

        mime->setText(item->objectName());
        drag->setMimeData(mime);

        drag->exec(Qt::MoveAction);
    }
}

void SortingGameWidget::dragEnterEvent(QDragEnterEvent *event)
{
    event->acceptProposedAction();
}

void SortingGameWidget::dropEvent(QDropEvent *event)
{
    QString type = event->mimeData()->text();
    QLabel *targetBin = nullptr;

    if (plasticBin->geometry().contains(event->pos())) targetBin = plasticBin;
    else if (paperBin->geometry().contains(event->pos())) targetBin = paperBin;
    else if (organicBin->geometry().contains(event->pos())) targetBin = organicBin;

    if (!targetBin) return;

    QString binType = targetBin->text().split(" ").first().toLower();

    // -----------------------------------------------------
    // Check correctness
    // -----------------------------------------------------
    if (binType == type) {
        score++;
        scoreLabel->setText("Score: " + QString::number(score));
        QMessageBox::information(this, "Correct!", "Great job! 🌟");
    } else {
        QMessageBox::warning(this, "Oops!", "Wrong bin, try again!");
    }

    spawnRandomItem();
}
