#include "booklistwindow.h"
#include "ui_booklistwindow.h"

booklistWindow::booklistWindow(const QString &username, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::booklistWindow)
    , currentUsername(username)

{
    ui->setupUi(this);
}

booklistWindow::~booklistWindow()
{
    delete ui;
}
