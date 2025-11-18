#include "bookinformationdialog.h"
#include "ui_BookInformationDialog.h"

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QListWidgetItem>
#include <QPixmap>

BookInformationDialog::BookInformationDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::BookInformationDialog)
{
    ui->setupUi(this);
    this->setFixedSize(720, 460);

    // open DB and load books
    openDatabase();
    loadBooks();

    // connections
    connect(ui->btnClose, &QPushButton::clicked, this, &QDialog::reject);
    connect(ui->searchLineEdit, &QLineEdit::textChanged,
            this, &BookInformationDialog::onSearchTextChanged);
    connect(ui->bookListWidget, &QListWidget::itemClicked,
            this, &BookInformationDialog::onBookItemClicked);
}

BookInformationDialog::~BookInformationDialog()
{
    if (m_db.isOpen()) {
        m_db.close();
    }
    delete ui;
}

void BookInformationDialog::openDatabase()
{
    if (QSqlDatabase::contains("bookinfo_connection")) {
        m_db = QSqlDatabase::database("bookinfo_connection");
    } else {
        m_db = QSqlDatabase::addDatabase("QSQLITE", "bookinfo_connection");
        QString path = QString(PROJECT_SOURCE_DIR) + "/databases/library.db";
        m_db.setDatabaseName(path);
    }

    if (!m_db.open()) {
        QMessageBox::critical(this, "Database Error",
                              "Failed to open library.db:\n" + m_db.lastError().text());
    }
}

void BookInformationDialog::loadBooks(const QString &filter)
{
    ui->bookListWidget->clear();

    if (!m_db.isOpen())
        return;

    QSqlQuery query(m_db);
    if (filter.isEmpty()) {
        query.prepare("SELECT book, author, ISBN FROM books ORDER BY book");
    } else {
        QString like = "%" + filter + "%";
        query.prepare("SELECT book, author, ISBN FROM books "
                      "WHERE book LIKE :t OR author LIKE :t OR ISBN LIKE :t "
                      "ORDER BY book");
        query.bindValue(":t", like);
    }

    if (!query.exec()) {
        QMessageBox::warning(this, "Query Error",
                             "Failed to load books:\n" + query.lastError().text());
        return;
    }

    while (query.next()) {
        QString title = query.value(0).toString();
        QString author = query.value(1).toString();
        QString isbn = query.value(2).toString();
        QString display = QString("%1 — %2 (ISBN: %3)")
                              .arg(title, author, isbn);
        ui->bookListWidget->addItem(display);
    }

    if (ui->bookListWidget->count() == 0) {
        ui->bookListWidget->addItem("(no results)");
    }
}

void BookInformationDialog::onSearchTextChanged(const QString &text)
{
    loadBooks(text.trimmed());
}

void BookInformationDialog::onBookItemClicked(QListWidgetItem *item)
{
    if (!item) return;
    QString text = item->text();
    if (text == "(no results)") return;

    // parse "Title — Author (ISBN: 1234)"
    QString title = text.section(" — ", 0, 0).trimmed();
    QString rest  = text.section(" — ", 1, 1);
    int isbnPos = rest.indexOf("(ISBN:");
    QString author = rest;
    QString isbn;
    if (isbnPos != -1) {
        author = rest.left(isbnPos).trimmed();
        int start = text.indexOf("ISBN:");
        int end   = text.indexOf(")", start);
        if (start != -1 && end != -1 && end > start) {
            isbn = text.mid(start + 5, end - (start + 5)).trimmed();
        }
    }

    showBookDetails(title, author, isbn);
}

void BookInformationDialog::showBookDetails(const QString &title,
                                            const QString &author,
                                            const QString &isbn)
{
    ui->lblTitleValue->setText(title);
    ui->lblAuthorValue->setText(author);
    ui->lblIsbnValue->setText(isbn);

    BookExtras extra = extrasFor(title);
    ui->lblGenreValue->setText(extra.genre);
    ui->lblYearValue->setText(extra.year);

    if (!extra.coverPath.isEmpty()) {
        QPixmap cover(extra.coverPath);
        if (!cover.isNull()) {
            ui->lblCover->setPixmap(
                cover.scaled(ui->lblCover->size(),
                             Qt::KeepAspectRatio,
                             Qt::SmoothTransformation));
            ui->lblCover->setText("");

        } else {
            ui->lblCover->setPixmap(QPixmap());
            ui->lblCover->setText("[Cover]");
        }
    } else {
        ui->lblCover->setPixmap(QPixmap());
        ui->lblCover->setText("[Cover]");
    }
}

BookInformationDialog::BookExtras BookInformationDialog::extrasFor(const QString &title) const
{
    if (title == "To Kill a Mockingbird") {
        return { "Southern Gothic / Bildungsroman", "1960", ":/images/Tkam.png" };
    } else if (title == "The Great Gatsby") {
        return { "Tragedy", "1925", ":/images/Tgatsby.png" };
    } else if (title == "1984") {
        return { "Dystopian / Political fiction", "1949", ":/images/1984.png" };
    }

    return { "", "", "" };
}
