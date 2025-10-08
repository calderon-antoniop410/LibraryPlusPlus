#ifndef HOMEWINDOW_H
#define HOMEWINDOW_H

#include <QMainWindow>

namespace Ui {
class homeWindow;
}

class homeWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit homeWindow(QWidget *parent = nullptr);
    ~homeWindow();

private:
    Ui::homeWindow *ui;
};

#endif // HOMEWINDOW_H
