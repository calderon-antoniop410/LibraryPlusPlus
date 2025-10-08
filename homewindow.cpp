#include "homewindow.h"
#include "ui_homewindow.h"

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
