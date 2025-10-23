#include "booklistwindow.h"
#include "ui_booklistwindow.h"
#include <QSqlTableModel>
#include <QHeaderView>

booklistWindow::booklistWindow(const QString &username, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::booklistWindow)
    , currentUsername(username)

{
    ui->setupUi(this);
    centerOnScreen(this);

    // CSS
    ui->lineEdit->setStyleSheet(R"(
    QLineEdit {
        border: 2px solid #ccc;
        border-radius: 12px;
        padding: 6px 10px;
        font-size: 14px;
    }
    QLineEdit:focus {
        border: 2px solid #0078d7;
    }
)");


    // Open the Books database
    QString PATH_booksdb = QString(PROJECT_SOURCE_DIR) + "/databases/books.db";
    QSqlDatabase booksDB = QSqlDatabase::addDatabase("QSQLITE");
    booksDB.setDatabaseName(PATH_booksdb);

    if (!booksDB.open()) {
        QMessageBox::critical(this, "Database Error", "Failed to open Books database:\n" + booksDB.lastError().text());
        return;
    }

    // Create a model to display the books table
    QSqlTableModel *model = new QSqlTableModel(this, booksDB);
    model->setTable("books");
    model->select(); // Load data

    // Set human-readable headers (optional)
    model->setHeaderData(0, Qt::Horizontal, "Book Title");
    model->setHeaderData(1, Qt::Horizontal, "Author");
    model->setHeaderData(2, Qt::Horizontal, "ISBN");

    // Assign model to tableView
    ui->tableView->setModel(model);
    ui->tableView->resizeColumnsToContents();
    ui->tableView->horizontalHeader()->setStretchLastSection(true);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView->setSortingEnabled(true);

    // Make it scrollable automatically
    ui->tableView->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    ui->tableView->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
}

booklistWindow::~booklistWindow()
{
    if (booksDB.isOpen())
        booksDB.close();

    delete ui;
}

void booklistWindow::on_backtohomeButton_clicked()
{
    // Create the homewindow window
    homeWindow *homewindow = new homeWindow(currentUsername);
    homewindow->show();

    // Close or hide the login window
    this->close(); // or this->hide() if you want it reusable
}

