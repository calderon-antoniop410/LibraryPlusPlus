#ifndef RETURNWINDOW_H
#define RETURNWINDOW_H

#include <QDialog>

namespace Ui {
class returnwindow;
}

class returnwindow : public QDialog
{
    Q_OBJECT

public:
    explicit returnwindow(const QString &username, QWidget *parent = nullptr);
    ~returnwindow();

private slots:
    void on_exitButton_clicked();
    void on_returnButton_clicked();
    void loadBorrowedBooks();

private:
    Ui::returnwindow *ui;
    QString currentUsername;
};

#endif // RETURNWINDOW_H

