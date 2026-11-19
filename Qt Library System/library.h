#pragma once
#include "book.h"
#include "person.h"
#include <vector>
#include <memory>
#include <string>
using namespace std;

class Library {
private:
    string libraryName;

    // shared_ptr handles memory automatically -- no manual delete needed.
    // We tried raw pointers first but got crashes on exit, shared_ptr fixed that.
    vector<shared_ptr<Book>> books;
    vector<shared_ptr<User>> users;

    // These two are static -- shared across all Library objects.
    // totalBorrowsAllTime never goes back down, it's a running total.
    static int totalBooksAdded;
    static int totalBorrowsAllTime;

public:
    explicit Library(const string& name = "City Library");
    ~Library();

    // Book management
    void addBook(shared_ptr<Book> book);
    void removeBook(const string& isbn);

    // Three separate search methods -- same idea (search), different parameters.
    // This is function overloading (compile-time polymorphism).
    shared_ptr<Book>         searchByISBN(const string& isbn)    const;
    vector<shared_ptr<Book>> searchByTitle(const string& title)  const;
    vector<shared_ptr<Book>> searchByAuthor(const string& author) const;

    // User management
    void             addUser(shared_ptr<User> user);
    void             removeUser(const string& userId);
    shared_ptr<User> findUser(const string& userId) const;

    // Borrow & Return
    void borrowBook(const string& userId, const string& isbn);
    void returnBook(const string& userId, const string& isbn);

    // Accessors for the GUI layer
    const vector<shared_ptr<Book>>& getBooks() const { return books; }
    const vector<shared_ptr<User>>& getUsers() const { return users; }
    string getName() const { return libraryName; }

    int countAvailableBooks() const;

    static int getTotalBooksAdded()     { return totalBooksAdded; }
    static int getTotalBorrowsAllTime() { return totalBorrowsAllTime; }

    void displayAllBooks() const;
};
