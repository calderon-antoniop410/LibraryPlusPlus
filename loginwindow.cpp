#include "loginwindow.h"
#include "homewindow.h"
#include "signupwindow.h"
#include "ui_loginwindow.h"

loginWindow::loginWindow(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::loginWindow)
{
    ui->setupUi(this);
    this->setFixedSize(575, 460);
    centerOnScreen(this);

    QString PATH_accountsdb = QString(PROJECT_SOURCE_DIR) + "/databases/Accounts.sqlite";
    accountsDB = QSqlDatabase::addDatabase("QSQLITE");
    accountsDB.setDatabaseName(PATH_accountsdb);

    qDebug() << "Database path:" << PATH_accountsdb;  // Check this in Application Output

}

loginWindow::~loginWindow()
{
    if (accountsDB.isOpen())
        accountsDB.close();              // Close the database connection
    delete ui;
}

void loginWindow::on_loginButton_clicked()
{
    QString username = ui->lineEditUsername->text();
    QString password = ui->lineEditPassword->text();

    if(accountsDB.open())
    {
        QSqlQuery query(accountsDB);
        query.prepare("SELECT * FROM Users WHERE username = :username AND password = :password");
        query.bindValue(":username", username);
        query.bindValue(":password", password);
        if(query.exec())
        {
            if(query.next()) // username & password found
            {
                // Create the homewindow window
                homeWindow *homewindow = new homeWindow(username);
                homewindow->show();

                // Close or hide the login window
                this->close(); // or this->hide() if you want it reusable

                // Optionally show a message before opening
                // QMessageBox::about(this, "Login", "Login Success");
            }
            else
            {
                QMessageBox::warning(this,"Invalid Credentials","Invalid Username or Password");
            }
        }
    }
}




void loginWindow::on_signupButton_clicked()
{
    // Create the bookManager window
    signupWindow *signupwindow = new signupWindow();
    signupwindow->show();

    // Close or hide the login window
    this->close(); // or this->hide() if you want it reusable

}

