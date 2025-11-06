#include "rentwindow.h"
#include "ui_rentwindow.h"
#include "centerhelper.h"
#include "homewindow.h"

rentwindow::rentwindow(const QString &username,QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::rentwindow)
    , currentUsername(username)
{
    ui->setupUi(this);
    this->setFixedSize(700, 640);
    centerOnScreen(this);
}

rentwindow::~rentwindow()
{
    delete ui;
}

void rentwindow::on_exitButton_clicked()
{
    // Create the homewindow window
    homeWindow *homewindow = new homeWindow(currentUsername);
    homewindow->show();

    // Close or hide the login window
    this->close(); // or this->hide() if you want it reusable
}


