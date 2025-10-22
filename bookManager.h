#ifndef BOOKMANAGER_H
#define BOOKMANAGER_H


#include <QMainWindow>
#include <QSqlDatabase>
#include <QDebug>
#include <QListWidget>
#include <QWidget>
#include <QPushButton>

QT_BEGIN_NAMESPACE
namespace Ui { class bookManager; }
QT_END_NAMESPACE

class bookManager : public QMainWindow
{
    Q_OBJECT

public:
    explicit bookManager(const QString &username, QWidget *parent = nullptr);
    ~bookManager();

private slots:
    void on_searchButton_clicked();
    void on_addButton_clicked();
    void on_editButton_clicked();
    void on_deleteButton_clicked();
    void on_exitButton_clicked();

private:
    Ui::bookManager *ui;
    QSqlDatabase db;
    void loadSavedBooks();
    QString parseISBN(const QString &text);
    QString currentUsername;
};

#endif // bookManager_H
