#include "homewindow.h"
#include "ui_homewindow.h"
#include "loginwindow.h"
#include "bookManager.h"
homeWindow::homeWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::homeWindow)
{
    ui->setupUi(this);
}

homeWindow::~homeWindow()
{
    delete ui;
}

void homeWindow::on_pushButton_6_clicked()
{
    // Create the bookManager window
    bookManager *managerWindow = new bookManager();
    managerWindow->show();

    // Close or hide the login window
    this->close(); // or this->hide() if you want it reusable
}


void homeWindow::on_pushButton_clicked()
{
    loginWindow *loginwindow = new loginWindow();
    loginwindow->setAttribute(Qt::WA_DeleteOnClose);
    loginwindow->show();

    this->setAttribute(Qt::WA_DeleteOnClose);
    this->close();
}

