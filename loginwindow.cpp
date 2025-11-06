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

    QString PATH_libdb = QString(PROJECT_SOURCE_DIR) + "/databases/library.db";
    libDB = QSqlDatabase::addDatabase("QSQLITE");
    libDB.setDatabaseName(PATH_libdb);

    qDebug() << "Database path:" << PATH_libdb;  // Check this in Application Output

}

loginWindow::~loginWindow()
{
    if (libDB.isOpen())
        libDB.close();              // Close the database connection
    delete ui;
}

void loginWindow::on_loginButton_clicked()
{
    QString username = ui->lineEditUsername->text();
    QString password = ui->lineEditPassword->text();

    if(libDB.open())
    {
        QSqlQuery query(libDB);
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

