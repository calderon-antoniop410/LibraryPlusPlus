#ifndef SIGNUPWINDOW_H
#define SIGNUPWINDOW_H
#include "centerhelper.h"

#include <QDialog>

namespace Ui {
class signupWindow;
}

class signupWindow : public QDialog
{
    Q_OBJECT

public:
    explicit signupWindow(QWidget *parent = nullptr);
    ~signupWindow();

private slots:
    void on_signupButton_clicked();

    void on_pushButton_clicked();

private:
    Ui::signupWindow *ui;
};

#endif // SIGNUPWINDOW_H
