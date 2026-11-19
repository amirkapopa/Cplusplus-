#include "filehandler.h"
#include "exceptions.h"
#include <fstream>
#include <sstream>
#include <iostream>
using namespace std;

const string FileHandler::BOOKS_FILE = "books.csv";
const string FileHandler::USERS_FILE = "users.csv";

// Basic CSV splitter -- splits on the given delimiter.
// Doesn't handle quoted commas inside fields, but our data doesn't have that case.
vector<string> FileHandler::splitCSV(const string& line, char delim)
{
    vector<string> result;
    stringstream ss(line);
    string token;
    while (getline(ss, token, delim)) {
        result.push_back(token);
    }
    return result;
}

// Wrap in quotes if the value contains a comma, so the CSV stays valid
string FileHandler::escapeCSV(const string& s)
{
    if (s.find(',') != string::npos) {
        return "\"" + s + "\"";
    }
    return s;
}

// ---- Save books ----
// Format: type,title,author,isbn,year,available,extra
// "extra" is genre for Fiction books, subject for Non-Fiction books

void FileHandler::saveBooks(const vector<shared_ptr<Book>>& books,
                            const string& filename)
{
    const string& path = filename.empty() ? BOOKS_FILE : filename;

    ofstream file(path);
    if (!file.is_open()) {
        throw FileException(path);
    }

    file << "type,title,author,isbn,year,available,extra\n";

    for (const auto& b : books) {
        // dynamic_pointer_cast lets us check the real runtime type of a shared_ptr.
        // A plain Book* doesn't tell us if it's Fiction or NonFiction at runtime.
        string extra;
        if (auto fb = dynamic_pointer_cast<FictionBook>(b)) {
            extra = fb->getGenre();
        } else if (auto nfb = dynamic_pointer_cast<NonFictionBook>(b)) {
            extra = nfb->getSubject();
        }

        file << escapeCSV(b->getType())   << ","
             << escapeCSV(b->getTitle())  << ","
             << escapeCSV(b->getAuthor()) << ","
             << escapeCSV(b->getISBN())   << ","
             << b->getYear()              << ","
             << (b->isAvailable() ? "1" : "0") << ","
             << escapeCSV(extra)          << "\n";
    }

    file.close();
    cout << "Books saved to " << path << endl;
}

// ---- Load books ----

vector<shared_ptr<Book>> FileHandler::loadBooks(const string& filename)
{
    const string& path = filename.empty() ? BOOKS_FILE : filename;

    ifstream file(path);
    if (!file.is_open()) {
        throw FileException(path);
    }

    vector<shared_ptr<Book>> result;
    string line;

    getline(file, line);  // first line is the header row, skip it

    while (getline(file, line)) {
        if (line.empty()) continue;

        auto parts = splitCSV(line);
        if (parts.size() < 7) continue;  // skip incomplete/malformed lines

        string type      = parts[0];
        string title     = parts[1];
        string author    = parts[2];
        string isbn      = parts[3];
        int    year      = stoi(parts[4]);
        bool   available = (parts[5] == "1");
        string extra     = parts[6];

        shared_ptr<Book> book;

        try {
            if (type == "Fiction") {
                book = make_shared<FictionBook>(title, author, isbn, year, extra);
            } else {
                book = make_shared<NonFictionBook>(title, author, isbn, year, extra);
            }
            book->setAvailable(available);
            result.push_back(book);
        } catch (const exception& e) {
            // One bad line shouldn't crash the whole load -- just skip it
            cerr << "Skipping malformed book entry: " << e.what() << endl;
        }
    }

    file.close();
    cout << result.size() << " book(s) loaded from " << path << endl;
    return result;
}

// ---- Save users ----
// Format: id,name,limit,isbn1|isbn2|isbn3
// We use | to separate ISBNs because commas are already the CSV delimiter

void FileHandler::saveUsers(const vector<shared_ptr<User>>& users,
                            const string& filename)
{
    const string& path = filename.empty() ? USERS_FILE : filename;

    ofstream file(path);
    if (!file.is_open()) {
        throw FileException(path);
    }

    file << "id,name,limit,borrowed_isbns\n";

    for (const auto& u : users) {
        // Join all borrowed ISBNs with | as separator
        string borrowed;
        for (const auto& isbn : u->getBorrowedISBNs()) {
            if (!borrowed.empty()) borrowed += "|";
            borrowed += isbn;
        }

        file << escapeCSV(u->getPersonId()) << ","
             << escapeCSV(u->getName())     << ","
             << u->getMaxBorrowLimit()      << ","
             << borrowed                    << "\n";
    }

    file.close();
    cout << "Users saved to " << path << endl;
}

// ---- Load users ----

vector<shared_ptr<User>> FileHandler::loadUsers(const string& filename)
{
    const string& path = filename.empty() ? USERS_FILE : filename;

    ifstream file(path);
    if (!file.is_open()) {
        throw FileException(path);
    }

    vector<shared_ptr<User>> result;
    string line;

    getline(file, line);  // skip header

    while (getline(file, line)) {
        if (line.empty()) continue;

        auto parts = splitCSV(line);
        if (parts.size() < 3) continue;

        string id    = parts[0];
        string name  = parts[1];
        int    limit = stoi(parts[2]);

        try {
            auto user = make_shared<User>(name, id, limit);

            // Restore their borrowed list if they had any books when we saved
            if (parts.size() >= 4 && !parts[3].empty()) {
                auto isbns = splitCSV(parts[3], '|');
                for (const auto& isbn : isbns) {
                    user->borrowBook(isbn);
                }
            }
            result.push_back(user);
        } catch (const exception& e) {
            cerr << "Skipping malformed user entry: " << e.what() << endl;
        }
    }

    file.close();
    cout << result.size() << " user(s) loaded from " << path << endl;
    return result;
}

void FileHandler::saveAll(const vector<shared_ptr<Book>>& books,
                          const vector<shared_ptr<User>>& users)
{
    saveBooks(books);
    saveUsers(users);
}
