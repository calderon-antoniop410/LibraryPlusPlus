#include "loginwindow.h"
#include "homewindow.h"
#include "signupwindow.h"
#include "ui_loginwindow.h"
#define PATH_accountsdb "/Users/esnec/OneDrive/Documents/LPP/LibraryPlusPlus/databases/Accounts.sqlite" // CHANGE TO YOUR PATH

loginWindow::loginWindow(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::loginWindow)
{
    ui->setupUi(this);
    this->setFixedSize(575, 460);
    accountsDB = QSqlDatabase::addDatabase("QSQLITE");
    accountsDB.setDatabaseName(PATH_accountsdb);
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
                homeWindow *homewindow = new homeWindow();
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

