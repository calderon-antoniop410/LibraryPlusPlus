#ifndef BOOKLISTWINDOW_H
#define BOOKLISTWINDOW_H

#include <QMainWindow>

namespace Ui {
class booklistWindow;
}

class booklistWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit booklistWindow(const QString &username, QWidget *parent = nullptr);
    ~booklistWindow();

private:
    Ui::booklistWindow *ui;
    QString currentUsername;
};

#endif // BOOKLISTWINDOW_H
