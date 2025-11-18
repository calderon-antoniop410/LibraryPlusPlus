#ifndef BOOKINFORMATIONDIALOG_H
#define BOOKINFORMATIONDIALOG_H

#include <QDialog>
#include <QSqlDatabase>

QT_BEGIN_NAMESPACE
namespace Ui { class BookInformationDialog; }
QT_END_NAMESPACE

class QListWidgetItem;

class BookInformationDialog : public QDialog
{
    Q_OBJECT

public:
    explicit BookInformationDialog(QWidget *parent = nullptr);
    ~BookInformationDialog();

private slots:
    void onSearchTextChanged(const QString &text);
    void onBookItemClicked(QListWidgetItem *item);

private:
    Ui::BookInformationDialog *ui;
    QSqlDatabase m_db;

    void openDatabase();
    void loadBooks(const QString &filter = QString());
    void showBookDetails(const QString &title,
                         const QString &author,
                         const QString &isbn);

    struct BookExtras {
        QString genre;
        QString year;
        QString coverPath;
    };
    BookExtras extrasFor(const QString &title) const;
};

#endif // BOOKINFORMATIONDIALOG_H
