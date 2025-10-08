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
    QString confirmPassword = ui->lineEditPassword_2->text();

    // Basic input validation
    if (username.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Username and password cannot be empty.");
        return;
    }

    // Check if passwords match
    if (password != confirmPassword) {
        QMessageBox::warning(this, "Input Error", "Passwords do not match.");
        return;
    }

    // Get the existing database connection
    QSqlDatabase accountsDb = QSqlDatabase::database("AccountsDB");

    // Check if username already exists
    QSqlQuery checkQuery(accountsDb);
    checkQuery.prepare("SELECT COUNT(*) FROM Users WHERE username = :username");
    checkQuery.bindValue(":username", username);
    if (!checkQuery.exec()) {
        QMessageBox::critical(this, "Database Error", checkQuery.lastError().text());
        return;
    }

    checkQuery.next();
    int count = checkQuery.value(0).toInt();
    if (count > 0) {
        QMessageBox::warning(this, "Signup Failed", "Username already exists. Please choose another one.");
        return;
    }


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

