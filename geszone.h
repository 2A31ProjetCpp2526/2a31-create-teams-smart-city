#ifndef GESZONE_H
#define GESZONE_H

#include <QWidget>

namespace Ui {
class geszone;
}

class geszone : public QWidget
{
    Q_OBJECT

public:
    explicit geszone(QWidget *parent = nullptr);
    ~geszone();

signals:
    void requestBack();

private slots:
    void goBack();

private:
    Ui::geszone *ui;
};

#endif // GESZONE_H
