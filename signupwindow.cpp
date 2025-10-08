#include "signupwindow.h"
#include "ui_signupwindow.h"
#include "loginwindow.h"
signupWindow::signupWindow(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::signupWindow)
{
    ui->setupUi(this);
}

signupWindow::~signupWindow()
{
    delete ui;
}

void signupWindow::on_signupButton_clicked()
{
    QString username = ui->lineEditUsername->text();
    QString password = ui->lineEditPassword->text();

    // Basic input validation
    if (username.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Username and password cannot be empty.");
        return;
    }

    // Get the existing database connection
    QSqlDatabase accountsDb = QSqlDatabase::database("AccountsDB");

    // Prepare the insert query
    QSqlQuery query(accountsDb);
    query.prepare("INSERT INTO Users (username, password) VALUES (:username, :password)");
    query.bindValue(":username", username);
    query.bindValue(":password", password);

    // Execute the query
    if (!query.exec()) {
        QMessageBox::critical(this, "Signup Failed", query.lastError().text());
        return;
    }

    QMessageBox::information(this, "Signup Successful", "Account created successfully!");

    // Optionally, go back to login window
    loginWindow *loginwindow = new loginWindow();
    loginwindow->setAttribute(Qt::WA_DeleteOnClose);
    loginwindow->show();

    this->setAttribute(Qt::WA_DeleteOnClose);
    this->close();
}


void signupWindow::on_pushButton_clicked()
{
    // Create the loginWindow window
    loginWindow *loginwindow = new loginWindow();
    loginwindow->show();

    // Close or hide the window
    this->close(); // or this->hide() if you want it reusable
}

