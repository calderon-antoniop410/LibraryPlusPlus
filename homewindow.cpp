#include "homewindow.h"
#include "ui_homewindow.h"
#include "loginwindow.h"
#include "bookManager.h"
#include "booklistwindow.h"
#include "rentwindow.h"
#include "returnwindow.h"
#include "bookinformationdialog.h"
homeWindow::homeWindow(const QString &username, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::homeWindow)
    , currentUsername(username)
{
    ui->setupUi(this);
    this->setFixedSize(800, 640);
    centerOnScreen(this);

    ui->user_label->setText("Welcome back " + currentUsername + "!");
    ui->user_label->setStyleSheet(
        "QLabel {"
        "   color: #2E3A59;"
        "   font-family: 'Segoe UI';"
        "   font-size: 20px;"
        "   font-weight: 600;"
        "   background-color: #E8F0FE;"
        "   border: 2px solid #AAB8E3;"
        "   border-radius: 10px;"
        "   padding: 8px 15px;"
        "   margin: 10px;"
        "   qproperty-alignment: 'AlignCenter';"
        "}"
        );

    loadBorrowedBooks();
    ui->borrowedTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->borrowedTable->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->borrowedTable->setStyleSheet("QTableWidget::item:selected { background-color: blue; }");

}

homeWindow::~homeWindow()
{
    delete ui;
}

void homeWindow::on_pushButton_6_clicked() // bookmanager button
{
    if(currentUsername == "admin")
    {
        // Create the bookManager window
        bookManager *managerWindow = new bookManager(currentUsername);
        managerWindow->show();

        // Close or hide the login window
        this->close(); // or this->hide() if you want it reusable
    }
    else
    {
        QMessageBox::warning(
            this,
            "Access Denied",
            "Admin access only."
            );
    }
}

void homeWindow::loadBorrowedBooks()
{
    QSqlDatabase db = QSqlDatabase::database();
    if (!db.isOpen()) {
        QMessageBox::critical(this, "Database Error", "Database not open!");
        return;
    }

    ui->borrowedTable->setRowCount(0);
    ui->borrowedTable->setColumnCount(1);
    QStringList headers = {"Borrowed Books"};
    ui->borrowedTable->setHorizontalHeaderLabels(headers);

    // Stretch column for long titles
    ui->borrowedTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);

    // Style the table
    ui->borrowedTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->borrowedTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->borrowedTable->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->borrowedTable->setAlternatingRowColors(true);

    // Fancy colors
    ui->borrowedTable->setStyleSheet(R"(
        QTableWidget {
            background-color: #F9F9F9;
            alternate-background-color: #E8F0FE;
            gridline-color: #AAB8E3;
            font: 16px 'Segoe UI';
        }
        QHeaderView::section {
            background-color: #2E3A59;
            color: white;
            font-weight: bold;
            font-size: 16px;
            padding: 5px;
        }
        QTableWidget::item:selected {
            background-color: #4A90E2;
            color: white;
            border-radius: 5px;
        }
    )");

    QSqlQuery query;
    query.prepare(R"(
        SELECT b.book
        FROM books b
        JOIN borrow_records br ON b.ISBN = br.book_ISBN
        JOIN users u ON br.user_id = u.id
        WHERE u.username = :uname
    )");
    query.bindValue(":uname", currentUsername);

    if(!query.exec()) {
        qDebug() << "Failed to load borrowed books:" << query.lastError().text();
        return;
    }

    int row = 0;
    while(query.next()) {
        ui->borrowedTable->insertRow(row);
        QTableWidgetItem *item = new QTableWidgetItem(query.value("book").toString());
        item->setTextAlignment(Qt::AlignCenter); // center text if you like
        ui->borrowedTable->setItem(row, 0, item);
        row++;
    }

    if(row == 0) {
        ui->borrowedTable->insertRow(0);
        QTableWidgetItem *item = new QTableWidgetItem("(No borrowed books)");
        item->setTextAlignment(Qt::AlignCenter);
        ui->borrowedTable->setItem(0, 0, item);
    }
}



void homeWindow::on_pushButton_clicked() // logout button
{
    loginWindow *loginwindow = new loginWindow();
    loginwindow->setAttribute(Qt::WA_DeleteOnClose);
    loginwindow->show();

    this->setAttribute(Qt::WA_DeleteOnClose);
    this->close();
}


void homeWindow::on_pushButton_8_clicked() // booklist button
{
    // Create the booklist window
    booklistWindow *booklWindow = new booklistWindow(currentUsername);
    booklWindow->show();

    // Close or hide the current window
    this->close(); // or this->hide() if you want it reusable
}


void homeWindow::on_pushButton_9_clicked() //rent book button
{
    // Create the rent book window
    rentwindow *rentwindo= new rentwindow(currentUsername);
    rentwindo->show();

    // Close or hide the current window
    this->close(); // or this->hide() if you want it reusable
}


void homeWindow::on_pushButton_10_clicked() // return button
{
    // Create the rent book window
    returnwindow *retwindow= new returnwindow(currentUsername);
    retwindow->show();

    // Close or hide the current window
    this->close(); // or this->hide() if you want it reusable
}

void homeWindow::on_pushButton_11_clicked()
{
    BookInformationDialog dialog(this);
    dialog.exec();
}

