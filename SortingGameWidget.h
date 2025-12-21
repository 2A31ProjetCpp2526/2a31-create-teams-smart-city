#ifndef SORTINGGAMEWIDGET_H
#define SORTINGGAMEWIDGET_H

#include <QDialog>
#include <QLabel>
#include <QVector>
#include <QPushButton>

class SortingGameWidget : public QDialog
{
    Q_OBJECT

public:
    explicit SortingGameWidget(QWidget *parent = nullptr);

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dropEvent(QDropEvent *event) override;

private:
    QLabel *plasticBin;
    QLabel *paperBin;
    QLabel *organicBin;

    QLabel *item = nullptr;
    QLabel *scoreLabel;
    int score = 0;

    struct WasteItem {
        QString emoji;
        QString type;
    };

    QVector<WasteItem> items;

    void loadItems();
    void spawnRandomItem();
    QLabel* createBin(const QString &text, const QString &color);
};

#endif // SORTINGGAMEWIDGET_H
