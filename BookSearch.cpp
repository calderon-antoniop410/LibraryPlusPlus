#include <iostream>
#include <vector>
#include <string>

using namespace std;

struct Book {
    string title;
    string author;
    string isbn;
};

class BookSearch {
    vector<Book> books;
public:
    BookSearch() {
        // Sample books
        books.push_back({"The Great Gatsby", "F. Scott Fitzgerald", "9780743273565"});
        books.push_back({"1984", "George Orwell", "9780451524935"});
        books.push_back({"To Kill a Mockingbird", "Harper Lee", "9780061120084"});
    }

    void searchByTitle(const string& title) {
        bool found = false;
        for (const auto& book : books) {
            if (book.title == title) {
                cout << "Found: " << book.title << " by " << book.author << ", ISBN: " << book.isbn << endl;
                found = true;
            }
        }
        if (!found) cout << "No book found with title: " << title << endl;
    }

    void searchByAuthor(const string& author) {
        bool found = false;
        for (const auto& book : books) {
            if (book.author == author) {
                cout << "Found: " << book.title << " by " << book.author << ", ISBN: " << book.isbn << endl;
                found = true;
            }
        }
        if (!found) cout << "No books found by author: " << author << endl;
    }

    void searchByISBN(const string& isbn) {
        bool found = false;
        for (const auto& book : books) {
            if (book.isbn == isbn) {
                cout << "Found: " << book.title << " by " << book.author << ", ISBN: " << book.isbn << endl;
                found = true;
            }
        }
        if (!found) cout << "No book found with ISBN: " << isbn << endl;
    }
};

int main() {
    BookSearch bs;
    string title, author, isbn;

    cout << "Enter book title: ";
    getline(cin, title);
    bs.searchByTitle(title);

    cout << "Enter author name: ";
    getline(cin, author);
    bs.searchByAuthor(author);

    cout << "Enter ISBN: ";
    getline(cin, isbn);
    bs.searchByISBN(isbn);

    return 0;
}
