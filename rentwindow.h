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
    void borrowBook();       // new slot for borrow button
    void loadBooks();        // helper to load books into table

private:
    Ui::rentwindow *ui;
    QString currentUsername;
};

#endif // RENTWINDOW_H
