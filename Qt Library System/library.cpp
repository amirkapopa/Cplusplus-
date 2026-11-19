#include "library.h"
#include "exceptions.h"
#include <iostream>
#include <algorithm>
#include <cctype>
using namespace std;

int Library::totalBooksAdded     = 0;
int Library::totalBorrowsAllTime = 0;

Library::Library(const string& name) : libraryName(name)
{
    cout << "Library \"" << libraryName << "\" opened." << endl;
}

Library::~Library()
{
    cout << "Library \"" << libraryName << "\" closed. "
         << "Total borrows ever: " << totalBorrowsAllTime << endl;
}

// ---- Book management ----

void Library::addBook(shared_ptr<Book> book)
{
    if (book->getTitle().empty() || book->getISBN().empty()) {
        throw InvalidDataException("Book title and ISBN cannot be empty.");
    }
    // Check for duplicate ISBN using our overloaded == operator
    for (const auto& b : books) {
        if (*b == *book) {
            throw InvalidDataException("A book with ISBN " + book->getISBN() + " already exists.");
        }
    }
    books.push_back(book);
    ++totalBooksAdded;
}

void Library::removeBook(const string& isbn)
{
    // find_if lets us search with a custom condition (lambda)
    auto it = find_if(books.begin(), books.end(),
        [&isbn](const shared_ptr<Book>& b) {
            return b->getISBN() == isbn;
        });

    if (it == books.end()) {
        throw BookNotFoundException(isbn);
    }
    books.erase(it);
}

// ---- Searching ----

shared_ptr<Book> Library::searchByISBN(const string& isbn) const
{
    for (const auto& b : books) {
        if (b->getISBN() == isbn) return b;
    }
    throw BookNotFoundException(isbn);
}

// Helper for case-insensitive substring search.
// Converts both strings to lowercase before comparing so "harry" finds "Harry Potter".
static bool containsCI(const string& haystack, const string& needle)
{
    string h = haystack, n = needle;
    transform(h.begin(), h.end(), h.begin(), ::tolower);
    transform(n.begin(), n.end(), n.begin(), ::tolower);
    return h.find(n) != string::npos;
}

vector<shared_ptr<Book>> Library::searchByTitle(const string& title) const
{
    vector<shared_ptr<Book>> result;
    for (const auto& b : books) {
        if (containsCI(b->getTitle(), title)) {
            result.push_back(b);
        }
    }
    return result;
}

vector<shared_ptr<Book>> Library::searchByAuthor(const string& author) const
{
    vector<shared_ptr<Book>> result;
    for (const auto& b : books) {
        if (containsCI(b->getAuthor(), author)) {
            result.push_back(b);
        }
    }
    return result;
}

// ---- User management ----

void Library::addUser(shared_ptr<User> user)
{
    if (user->getName().empty() || user->getPersonId().empty()) {
        throw InvalidDataException("User name and ID cannot be empty.");
    }
    for (const auto& u : users) {
        if (u->getPersonId() == user->getPersonId()) {
            throw InvalidDataException("A user with ID " + user->getPersonId() + " already exists.");
        }
    }
    users.push_back(user);
}

void Library::removeUser(const string& userId)
{
    auto it = find_if(users.begin(), users.end(),
        [&userId](const shared_ptr<User>& u) {
            return u->getPersonId() == userId;
        });
    if (it == users.end()) {
        throw UserNotFoundException(userId);
    }
    users.erase(it);
}

shared_ptr<User> Library::findUser(const string& userId) const
{
    for (const auto& u : users) {
        if (u->getPersonId() == userId) return u;
    }
    throw UserNotFoundException(userId);
}

// ---- Borrow & Return ----

void Library::borrowBook(const string& userId, const string& isbn)
{
    auto user = findUser(userId);
    auto book = searchByISBN(isbn);

    // Check availability first -- better UX to tell them the book is taken
    // rather than complaining about their limit when the book isn't free anyway
    if (!book->isAvailable()) {
        throw BookNotAvailableException(book->getTitle());
    }

    if (!user->canBorrowBooks()) {
        throw BorrowLimitException(user->getName(), user->getMaxBorrowLimit());
    }

    user->borrowBook(isbn);
    book->setAvailable(false);
    ++totalBorrowsAllTime;
    // Note: totalBorrowsAllTime only ever goes up -- it's a lifetime total
}

void Library::returnBook(const string& userId, const string& isbn)
{
    auto user = findUser(userId);
    auto book = searchByISBN(isbn);

    if (!user->hasBorrowed(isbn)) {
        throw InvalidDataException(user->getName() + " did not borrow \"" + book->getTitle() + "\".");
    }

    user->returnBook(isbn);
    book->setAvailable(true);
}

int Library::countAvailableBooks() const
{
    int count = 0;
    for (const auto& b : books) {
        if (b->isAvailable()) ++count;
    }
    return count;
}

void Library::displayAllBooks() const
{
    cout << "\n=== All Books in " << libraryName << " ===" << endl;
    for (const auto& b : books) {
        b->display();  // virtual call -- actual type decides which display() runs
        cout << "  Info: " << b->getDescription() << endl;
    }
    cout << "================================\n" << endl;
}
