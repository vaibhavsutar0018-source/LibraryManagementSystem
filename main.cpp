
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <limits>
using namespace std;

// Book Class
class Book {
private:
    int bookID;
    string title;
    string author;
    bool available;

public:
    Book(int id, string t, string a, bool status = true) {
        bookID = id;
        title = t;
        author = a;
        available = status;
    }

    int getID() const {
        return bookID;
    }

    string getTitle() const {
        return title;
    }

    string getAuthor() const {
        return author;
    }

    bool isAvailable() const {
        return available;
    }

    void issueBook() {
        if (!available) {
            throw runtime_error("Book is already issued!");
        }
        available = false;
    }

    void returnBook() {
        if (available) {
            throw runtime_error("Book is already available!");
        }
        available = true;
    }

    void display() const {
        cout << "\nBook ID: " << bookID;
        cout << "\nTitle: " << title;
        cout << "\nAuthor: " << author;
        cout << "\nStatus: "
             << (available ? "Available" : "Issued");
        cout << "\n--------------------------\n";
    }
};

// Library Class
class Library {
private:
    vector<Book> books;

    // Find book using ID
    Book* findBook(int id) {
        for (auto &book : books) {
            if (book.getID() == id) {
                return &book;
            }
        }
        return nullptr;
    }

public:

    // Load books from file
    void loadFromFile() {
        ifstream file("books.txt");

        if (!file) {
            return;
        }

        string line;

        while (getline(file, line)) {
            stringstream ss(line);

            int id;
            string title, author;
            int status;

            if (ss >> id && ss.ignore(1, '|') &&
                getline(ss, title, '|') &&
                getline(ss, author, '|') &&
                ss >> status) {

                if (findBook(id) == nullptr) {
                    books.emplace_back(id, title, author,
                                       status == 1);
                }
            }
        }

        file.close();
    }

    // Save books into file
    void saveToFile() {
        ofstream file("books.txt");

        for (const auto &book : books) {
            file << book.getID() << "|"
                 << book.getTitle() << "|"
                 << book.getAuthor() << "|"
                 << (book.isAvailable() ? 1 : 0)
                 << "\n";
        }

        file.close();
    }

    // Add new book
    void addBook() {
        int id;
        string title, author;

        cout << "\nEnter Book ID: ";
        cin >> id;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (findBook(id) != nullptr) {
            cout << "Book ID already exists!\n";
            return;
        }

        cout << "Enter Book Title: ";
        getline(cin, title);

        cout << "Enter Author Name: ";
        getline(cin, author);

        if (title.empty() || author.empty() ||
            title.find('|') != string::npos ||
            author.find('|') != string::npos) {
            cout << "Invalid title or author!\n";
            return;
        }

        books.emplace_back(id, title, author);
        saveToFile();

        cout << "\nBook added successfully!\n";
    }

    // Display all books
    void displayBooks() {
        if (books.empty()) {
            cout << "\nNo books available!\n";
            return;
        }

        cout << "\n===== BOOK LIST =====\n";

        for (const auto &book : books) {
            book.display();
        }
    }

    // Search book
    void searchBook() {
        int id;

        cout << "\nEnter Book ID to search: ";
        cin >> id;

        Book* book = findBook(id);

        if (book == nullptr) {
            cout << "Book not found!\n";
        } else {
            cout << "\nBook Found!\n";
            book->display();
        }
    }

    // Issue book
    void issueBook() {
        int id;

        cout << "\nEnter Book ID to issue: ";
        cin >> id;

        Book* book = findBook(id);

        try {
            if (book == nullptr) {
                throw runtime_error("Invalid Book ID!");
            }

            book->issueBook();
            saveToFile();

            cout << "\nBook issued successfully!\n";
        }
        catch (const exception &e) {
            cout << "\nError: " << e.what() << "\n";
        }
    }

    // Return book
    void returnBook() {
        int id;

        cout << "\nEnter Book ID to return: ";
        cin >> id;

        Book* book = findBook(id);

        try {
            if (book == nullptr) {
                throw runtime_error("Invalid Book ID!");
            }

            book->returnBook();
            saveToFile();

            cout << "\nBook returned successfully!\n";
        }
        catch (const exception &e) {
            cout << "\nError: " << e.what() << "\n";
        }
    }
};

// Main Function
int main() {
    Library library;

    library.loadFromFile();

    int choice;

    cout << "================================\n";
    cout << "   LIBRARY MANAGEMENT SYSTEM\n";
    cout << "================================\n";

    do {
        cout << "\n1. Add Book";
        cout << "\n2. Display All Books";
        cout << "\n3. Search Book";
        cout << "\n4. Issue Book";
        cout << "\n5. Return Book";
        cout << "\n6. Exit";

        cout << "\n\nEnter your choice: ";

        if (!(cin >> choice)) {
            cout << "\nInvalid input! Enter a number.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        switch (choice) {
            case 1:
                library.addBook();
                break;

            case 2:
                library.displayBooks();
                break;

            case 3:
                library.searchBook();
                break;

            case 4:
                library.issueBook();
                break;

            case 5:
                library.returnBook();
                break;

            case 6:
                library.saveToFile();
                cout << "\nThank you for using Library System!\n";
                break;

            default:
                cout << "\nInvalid choice! Try again.\n";
        }

    } while (choice != 6);

    return 0;
}
