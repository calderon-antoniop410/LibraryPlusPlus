#include "returnwindow.h"
#include "ui_returnwindow.h"
#include "centerhelper.h"
#include "homewindow.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QDebug>

returnwindow::returnwindow(const QString &username, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::returnwindow)
    , currentUsername(username)
{
    ui->setupUi(this);
    this->setFixedSize(700, 640);
    centerOnScreen(this);


    // Highlight entire row
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

    // Open library database
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(QString(PROJECT_SOURCE_DIR) + "/databases/library.db");

    if (!db.open()) {
        QMessageBox::critical(this, "Database Error", "Failed to open database!");
    }

    // Load borrowed books
    loadBorrowedBooks();

}

returnwindow::~returnwindow()
{
    delete ui;
}

void returnwindow::on_exitButton_clicked()
{
    homeWindow *homewindow = new homeWindow(currentUsername);
    homewindow->show();
    this->close();
}

void returnwindow::loadBorrowedBooks()
{
    QSqlDatabase db = QSqlDatabase::database();
    if (!db.isOpen()) return;

    QSqlQuery query;
    query.prepare(R"(
        SELECT b.ISBN, b.book, b.author, b.is_borrowed
        FROM books b
        JOIN borrow_records br ON b.ISBN = br.book_ISBN
        JOIN users u ON br.user_id = u.id
        WHERE u.username = :uname
    )");
    query.bindValue(":uname", currentUsername);

    if (!query.exec()) {
        QMessageBox::critical(this, "Error", query.lastError().text());
        return;
    }

    ui->tableWidget->setRowCount(0);

    // Stretch Title column
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(3, QHeaderView::ResizeToContents);

    int row = 0;
    while (query.next()) {
        ui->tableWidget->insertRow(row);
        QString isbn = query.value("ISBN").toString();
        QString book = query.value("book").toString();
        QString author = query.value("author").toString();
        int isBorrowed = query.value("is_borrowed").toInt();
        QString status = (isBorrowed == 1) ? "Borrowed" : "Available";

        ui->tableWidget->setItem(row, 0, new QTableWidgetItem(isbn));
        ui->tableWidget->setItem(row, 1, new QTableWidgetItem(book));
        ui->tableWidget->setItem(row, 2, new QTableWidgetItem(author));
        ui->tableWidget->setItem(row, 3, new QTableWidgetItem(status));

        row++;
    }
}

void returnwindow::on_returnButton_clicked()
{
    QList<QTableWidgetItem*> selectedItems = ui->tableWidget->selectedItems();
    if (selectedItems.isEmpty()) {
        QMessageBox::warning(this, "No Selection", "Please select a book to return.");
        return;
    }

    QString selectedISBN = selectedItems[0]->text(); // ISBN column

    QSqlDatabase db = QSqlDatabase::database();
    if (!db.isOpen()) return;

    // Remove from borrow_records
    QSqlQuery deleteQuery;
    deleteQuery.prepare(R"(
        DELETE FROM borrow_records
        WHERE book_ISBN = :isbn
        AND user_id = (SELECT id FROM users WHERE username = :uname)
    )");
    deleteQuery.bindValue(":isbn", selectedISBN);
    deleteQuery.bindValue(":uname", currentUsername);

    if (!deleteQuery.exec()) {
        QMessageBox::critical(this, "Error", deleteQuery.lastError().text());
        return;
    }

    // Update books table
    QSqlQuery updateQuery;
    updateQuery.prepare("UPDATE books SET is_borrowed = 0 WHERE ISBN = :isbn");
    updateQuery.bindValue(":isbn", selectedISBN);
    updateQuery.exec();

    QMessageBox::information(this, "Success", "Book returned successfully!");
    loadBorrowedBooks(); // refresh table
}

