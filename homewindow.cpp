#include "homewindow.h"
#include "ui_homewindow.h"
#include "loginwindow.h"
#include "bookManager.h"
homeWindow::homeWindow(const QString &username, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::homeWindow)
    , currentUsername(username)
{
    ui->setupUi(this);

    ui->user_label->setText("Welcome back " + currentUsername + "!");
    ui->user_label->setStyleSheet(
        "QLabel {"
        "   color: #2E3A59;"           // deep gray-blue for text
        "   font-family: 'Segoe UI';"
        "   font-size: 20px;"
        "   font-weight: 600;"
        "   background-color: #E8F0FE;" // soft light-blue background
        "   border: 2px solid #AAB8E3;" // subtle border
        "   border-radius: 10px;"
        "   padding: 8px 15px;"
        "   margin: 10px;"
        "   qproperty-alignment: 'AlignCenter';"
        "}"
        );


}

homeWindow::~homeWindow()
{
    delete ui;
}

void homeWindow::on_pushButton_6_clicked()
{
    if(currentUsername == "admin")
    {
        // Create the bookManager window
        bookManager *managerWindow = new bookManager(currentUsername);
        managerWindow->show();

        // Close or hide the login window
        this->close(); // or this->hide() if you want it reusable
    }
    else
    {
        QMessageBox::warning(
            this,
            "Access Denied",
            "Admin access only."
            );
    }
}


void homeWindow::on_pushButton_clicked()
{
    loginWindow *loginwindow = new loginWindow();
    loginwindow->setAttribute(Qt::WA_DeleteOnClose);
    loginwindow->show();

    this->setAttribute(Qt::WA_DeleteOnClose);
    this->close();
}

