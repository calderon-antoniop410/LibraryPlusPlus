#include "booklistwindow.h"
#include "ui_booklistwindow.h"
#include <QSqlTableModel>
#include <QHeaderView>
#include <QDebug>

booklistWindow::booklistWindow(const QString &username, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::booklistWindow)
    , currentUsername(username)
{
    ui->setupUi(this);
    this->setFixedSize(800, 640);
    centerOnScreen(this);

    // CSS for QLineEdit
    ui->lineEdit->setStyleSheet(R"(
        QLineEdit {
            border: 1px solid #ccc;
            border-radius: 10px;
            padding: 5px 10px;
            font-size: 12px;
        }
        QLineEdit:focus {
            border: 2px solid #0078d7;
        }
    )");

    // Open the library database
    libraryDB = QSqlDatabase::addDatabase("QSQLITE");
    libraryDB.setDatabaseName(QString(PROJECT_SOURCE_DIR) + "/databases/library.db");

    if (!libraryDB.open()) {
        QMessageBox::critical(this, "Database Error", "Failed to open Books database:\n" + libraryDB.lastError().text());
        return;
    }

    // Use QSqlQueryModel to show "Yes"/"No" for borrowed status
    model = new QSqlTableModel(this, libraryDB);
    model->setTable("books");
    model->select();

    model->setHeaderData(0, Qt::Horizontal, "ISBN");
    model->setHeaderData(1, Qt::Horizontal, "Book Title");
    model->setHeaderData(2, Qt::Horizontal, "Author");
    model->setHeaderData(3, Qt::Horizontal, "Borrowed?");


    // Assign model to tableView
    ui->tableView->setModel(model);
    ui->tableView->resizeColumnsToContents();
    ui->tableView->horizontalHeader()->setStretchLastSection(true);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView->setSortingEnabled(true);

    // Scrollbars
    ui->tableView->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    ui->tableView->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    // Connect search lineEdit to filtering
    connect(ui->lineEdit, &QLineEdit::textChanged, this, &booklistWindow::filterBooks);
}

booklistWindow::~booklistWindow()
{
    if (libraryDB.isOpen())
        libraryDB.close();

    delete ui;
}

void booklistWindow::on_backtohomeButton_clicked()
{
    homeWindow *homewindow = new homeWindow(currentUsername);
    homewindow->show();
    this->close();
}

// Filter books dynamically based on QLineEdit input
void booklistWindow::filterBooks(const QString &text)
{
    QString escaped = text;
    escaped.replace("'", "''");

    if (text.isEmpty()) {
        model->setFilter("");
    } else {
        model->setFilter(QString("ISBN LIKE '%%1%' OR book LIKE '%%1%' OR author LIKE '%%1%'")
                             .arg(escaped));
    }

    model->select();
}

