#ifndef HOMEWINDOW_H
#define HOMEWINDOW_H

#include <QMainWindow>

namespace Ui {
class homeWindow;
}

class homeWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit homeWindow(const QString &username, QWidget *parent = nullptr);
    ~homeWindow();

private slots:
    void on_pushButton_6_clicked();

    void on_pushButton_clicked();

    void on_pushButton_8_clicked();

private:
    Ui::homeWindow *ui;
    QString currentUsername; // store current user
};

#endif // HOMEWINDOW_H
