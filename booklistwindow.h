#ifndef BOOKLISTWINDOW_H
#define BOOKLISTWINDOW_H
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QListWidgetItem>
#include <QMainWindow>
#include "homewindow.h"
#include "centerhelper.h"


namespace Ui {
class booklistWindow;
}

class booklistWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit booklistWindow(const QString &username, QWidget *parent = nullptr);
    ~booklistWindow();

private slots:
    void on_backtohomeButton_clicked();

private:
    Ui::booklistWindow *ui;
    QString currentUsername;
    QSqlDatabase booksDB;
};

#endif // BOOKLISTWINDOW_H
