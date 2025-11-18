#include "bookManager.h"
#include "homewindow.h"
#include "ui_bookManager.h"
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QInputDialog>
#include <QDebug>
#include <QListWidgetItem>
#include <QListWidget>
#include <QDir>

bookManager::bookManager(const QString &username, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::bookManager)
    , currentUsername(username)
{
    ui->setupUi(this);
    this->setFixedSize(800, 640);
    centerOnScreen(this);

    // connect to SQLite database
    QString libraryDB = QString(PROJECT_SOURCE_DIR) + "/databases/library.db";
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(libraryDB);

    if (!db.open()) {
        QMessageBox::critical(this, "DB Error", db.lastError().text());
    }

    // Ensure "mybooks" table exists
    QSqlQuery query;
    query.exec("CREATE TABLE IF NOT EXISTS mybooks ("
               "book TEXT, "
               "author TEXT, "
               "ISBN INTEGER UNIQUE)");

    loadSavedBooks();
}

bookManager::~bookManager()
{
    if (db.isOpen())
        db.close();              // Close the database connection
    delete ui;
}

void bookManager::on_searchButton_clicked()
{
    QString term = ui->lineEdit->text().trimmed();
    ui->tableView1->clear();

    QSqlQuery query;
    if (term.isEmpty()) {
        query.exec("SELECT book, author, ISBN FROM books");
    } else {
        query.prepare("SELECT book, author, ISBN FROM books "
                      "WHERE book LIKE :t OR author LIKE :t OR ISBN LIKE :t");
        query.bindValue(":t", "%" + term + "%");
        query.exec();
    }

    while (query.next()) {
        QString book = query.value(0).toString();
        QString author = query.value(1).toString();
        QString isbn = query.value(2).toString();
        ui->tableView1->addItem(book + " — " + author + " (ISBN: " + isbn + ")");
    }

    if (ui->tableView1->count() == 0)
        ui->tableView1->addItem("(no results)");
}

void bookManager::on_addButton_clicked()
{
    QListWidgetItem *item = ui->tableView1->currentItem();
    if (!item || item->text() == "(no results)") {
        QMessageBox::information(this, "Add", "Select a book from search results.");
        return;
    }

    QString text = item->text();
    QString isbn = parseISBN(text);

    // parse fields
    QString title = text.section(" — ", 0, 0).trimmed();
    QString author = text.section(" — ", 1, 1).section(" (ISBN:", 0, 0).trimmed();

    QSqlQuery query;
    query.prepare("INSERT OR IGNORE INTO mybooks (book, author, ISBN) VALUES (:b,:a,:i)");
    query.bindValue(":b", title);
    query.bindValue(":a", author);
    query.bindValue(":i", isbn);

    if (!query.exec()) {
        QMessageBox::warning(this, "Add Error", query.lastError().text());
    }

    loadSavedBooks();
}

void bookManager::on_editButton_clicked()
{
    QListWidgetItem *item = ui->tableView2->currentItem();
    if (!item) {
        QMessageBox::information(this, "Edit", "Select a saved book to edit.");
        return;
    }

    QString text = item->text();
    QString isbnOld = parseISBN(text);
    QString bookOld = text.section(" — ", 0, 0).trimmed();
    QString authorOld = text.section(" — ", 1, 1).section(" (ISBN:", 0, 0).trimmed();

    bool ok;
    QString newBook = QInputDialog::getText(this, "Edit Title", "Title:", QLineEdit::Normal, bookOld, &ok);
    if (!ok) return;
    QString newAuthor = QInputDialog::getText(this, "Edit Author", "Author:", QLineEdit::Normal, authorOld, &ok);
    if (!ok) return;
    QString newISBN = QInputDialog::getText(this, "Edit ISBN", "ISBN:", QLineEdit::Normal, isbnOld, &ok);
    if (!ok) return;

    QSqlQuery query;
    query.prepare("UPDATE mybooks SET book=:b, author=:a, ISBN=:i WHERE ISBN=:old");
    query.bindValue(":b", newBook);
    query.bindValue(":a", newAuthor);
    query.bindValue(":i", newISBN);
    query.bindValue(":old", isbnOld);

    if (!query.exec()) {
        QMessageBox::warning(this, "Edit Error", query.lastError().text());
    }

    loadSavedBooks();
}

void bookManager::on_deleteButton_clicked()
{
    QListWidgetItem *item = ui->tableView2->currentItem();
    if (!item) {
        QMessageBox::information(this, "Delete", "Select a saved book to delete.");
        return;
    }

    // Prefer the stored ISBN (UserRole). Fallback to parse if not present.
    QString isbn = item->data(Qt::UserRole).toString();
    if (isbn.isEmpty()) {
        isbn = parseISBN(item->text()); // fallback, should rarely be needed now
    }

    if (isbn.isEmpty()) {
        QMessageBox::warning(this, "Delete", "Could not determine ISBN for selected item.");
        return;
    }

    // Delete from DB using a prepared query
    QSqlQuery query(db);
    query.prepare("DELETE FROM mybooks WHERE ISBN = :i");
    // try bind as integer if it looks numeric, otherwise bind as text
    bool okNum = false;
    qlonglong num = isbn.toLongLong(&okNum);
    if (okNum) query.bindValue(":i", num);
    else       query.bindValue(":i", isbn);

    if (!query.exec()) {
        QMessageBox::warning(this, "Delete Error", query.lastError().text());
        return;
    }

    // remove the item from the UI immediately
    delete item;

    // reload to ensure UI/DB stay in sync (and handle duplicates if any)
    loadSavedBooks();
}

void bookManager::on_exitButton_clicked()
{
    // Create the homewindow window
    homeWindow *homewindow = new homeWindow(currentUsername);
    homewindow->show();

    // Close or hide the login window
    this->close(); // or this->hide() if you want it reusable
}

void bookManager::loadSavedBooks()
{
    ui->tableView2->clear();

    QSqlQuery query("SELECT book, author, ISBN FROM mybooks", db);
    while (query.next()) {
        QString book = query.value(0).toString();
        QString author = query.value(1).toString();
        QString isbn = query.value(2).toString();

        // create an item and attach the ISBN in UserRole so we can delete reliably later
        QString display = book + " — " + author + " (ISBN: " + isbn + ")";
        QListWidgetItem *listItem = new QListWidgetItem(display);
        listItem->setData(Qt::UserRole, isbn);        // <-- store ISBN with the item
        ui->tableView2->addItem(listItem);
    }
}

QString bookManager::parseISBN(const QString &text)
{
    return text.section("(ISBN:",1,1).remove(")").trimmed();
}
