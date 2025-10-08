#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H
#include <QDebug>
#include <QtSql/QSqlDatabase>
#include <QFileInfo>
#include <QSqlQuery>
#include <QMessageBox>
#include <QDialog>
#include <QSqlError>
namespace Ui {
class loginWindow;
}

class loginWindow : public QDialog
{
    Q_OBJECT

public:
    explicit loginWindow(QWidget *parent = nullptr);
    ~loginWindow();

private slots:
    void on_loginButton_clicked();

    void on_signupButton_clicked();

private:
    Ui::loginWindow *ui;
    QSqlDatabase accountsDB;
};

#endif // LOGINWINDOW_H
