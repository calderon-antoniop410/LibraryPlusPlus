#ifndef CENTERHELPER_H
#define CENTERHELPER_H

#include <QGuiApplication>
#include <QScreen>
#include <QWidget>

inline void centerOnScreen(QWidget *window) {
    QRect screenGeometry = QGuiApplication::primaryScreen()->geometry();
    int x = (screenGeometry.width() - window->width()) / 2;
    int y = (screenGeometry.height() - window->height()) / 2;
    window->move(x, y);
}

#endif // CENTERHELPER_H
