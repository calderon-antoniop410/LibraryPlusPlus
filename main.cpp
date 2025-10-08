#include "loginwindow.h"
#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <QListWidget>
#include <QListWidgetItem>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    loginWindow w;
    w.resize(900, 600);
    w.show();
    return a.exec();
}

