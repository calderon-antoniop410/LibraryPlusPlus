#include "rentwindow.h"
#include "ui_rentwindow.h"
#include "centerhelper.h"
#include "homewindow.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QMessageBox>
#include <QSqlError>
#include <QDebug>

rentwindow::rentwindow(const QString &username, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::rentwindow)
    , currentUsername(username)
{
    ui->setupUi(this);
    this->setFixedSize(700, 640);
    centerOnScreen(this);

    // Highlight entire row when a cell is clicked
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);

    // Only allow selecting one row at a time
    ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);

    // Open the library database
    QString PATH_librarydb = QString(PROJECT_SOURCE_DIR) + "/databases/library.db";
    QSqlDatabase libraryDB = QSqlDatabase::addDatabase("QSQLITE");
    libraryDB.setDatabaseName(PATH_librarydb);

    loadBooks();

    connect(ui->borrowButton, &QPushButton::clicked, this, &rentwindow::borrowBook);
}

rentwindow::~rentwindow()
{
    delete ui;
}

void rentwindow::on_exitButton_clicked()
{
    homeWindow *homewindow = new homeWindow(currentUsername);
    homewindow->show();
    this->close();
}

// Load books into tableWidget
void rentwindow::loadBooks()
{
    QSqlDatabase db = QSqlDatabase::database();
    if(!db.isOpen()) {
        QMessageBox::critical(this, "Database Error", "Database is not open!");
        return;
    }

    QSqlQuery query("SELECT ISBN, book, author, is_borrowed FROM books");
    ui->tableWidget->setRowCount(0);
    // Make Title column (column 1) stretch and wide
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);

    // Optional: make other columns resize nicely
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents); // ISBN
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents); // Author
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(3, QHeaderView::ResizeToContents); // Status

    // Make the table read-only
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);


    int row = 0;
    while(query.next()) {
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

// Borrow selected book
void rentwindow::borrowBook()
{
    QList<QTableWidgetItem*> selectedItems = ui->tableWidget->selectedItems();
    if(selectedItems.isEmpty()) {
        QMessageBox::warning(this, "No Selection", "Please select a book to borrow.");
        return;
    }

    QString selectedISBN = selectedItems[0]->text(); // ISBN is column 0

    QSqlDatabase db = QSqlDatabase::database();
    if(!db.isOpen()) return;

    // Check if already borrowed
    QSqlQuery checkQuery;
    checkQuery.prepare("SELECT is_borrowed FROM books WHERE ISBN = :isbn");
    checkQuery.bindValue(":isbn", selectedISBN);
    if(!checkQuery.exec() || !checkQuery.next()) {
        QMessageBox::critical(this, "Error", "Failed to check book availability.");
        return;
    }

    if(checkQuery.value("is_borrowed").toInt() == 1) {
        QMessageBox::warning(this, "Unavailable", "This book is already borrowed.");
        return;
    }

    // Get current user ID
    int currentUserId = 0;
    QSqlQuery userQuery;
    userQuery.prepare("SELECT id FROM users WHERE username = :uname");
    userQuery.bindValue(":uname", currentUsername);
    if(userQuery.exec() && userQuery.next()) {
        currentUserId = userQuery.value("id").toInt();
    } else {
        QMessageBox::critical(this, "Error", "Failed to find user ID.");
        return;
    }

    // Insert into borrow_records
    QSqlQuery insertQuery;
    insertQuery.prepare("INSERT INTO borrow_records (user_id, book_ISBN) VALUES (:uid, :isbn)");
    insertQuery.bindValue(":uid", currentUserId);
    insertQuery.bindValue(":isbn", selectedISBN);
    if(!insertQuery.exec()) {
        QMessageBox::critical(this, "Error", "Failed to borrow book: " + insertQuery.lastError().text());
        return;
    }

    // Update books table
    QSqlQuery updateQuery;
    updateQuery.prepare("UPDATE books SET is_borrowed = 1 WHERE ISBN = :isbn");
    updateQuery.bindValue(":isbn", selectedISBN);
    updateQuery.exec();

    QMessageBox::information(this, "Success", "Book borrowed successfully!");
    loadBooks(); // refresh table
}
