#ifndef RENTWINDOW_H
#define RENTWINDOW_H

#include <QDialog>

namespace Ui {
class rentwindow;
}

class rentwindow : public QDialog
{
    Q_OBJECT

public:
    explicit rentwindow(const QString &username, QWidget *parent = nullptr);
    ~rentwindow();

private slots:
    void on_exitButton_clicked();

private:
    Ui::rentwindow *ui;
    QString currentUsername;
};

#endif // RENTWINDOW_H
