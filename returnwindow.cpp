#include "returnwindow.h"
#include "ui_returnwindow.h"
#include "centerhelper.h"
#include "homewindow.h"

returnwindow::returnwindow(const QString &username, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::returnwindow)
    , currentUsername(username)
{
    ui->setupUi(this);
    this->setFixedSize(700, 640);
    centerOnScreen(this);
}

returnwindow::~returnwindow()
{
    delete ui;
}

void returnwindow::on_exitButton_clicked()
{
    // Create the homewindow window
    homeWindow *homewindow = new homeWindow(currentUsername);
    homewindow->show();

    // Close or hide the login window
    this->close(); // or this->hide() if you want it reusable
}


