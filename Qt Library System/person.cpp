#include "person.h"
#include "exceptions.h"
#include <iostream>
#include <algorithm>
using namespace std;

int Person::personCount = 0;

Person::Person(const string& name, const string& id)
    : name(name), personId(id)
{
    ++personCount;
}

Person::~Person()
{
    --personCount;
}

// Default greet -- no arguments
void Person::greet() const
{
    cout << "Hello, I am " << name << " (ID: " << personId << ")." << endl;
}

// Overloaded greet -- takes a custom message string
void Person::greet(const string& customMessage) const
{
    cout << customMessage << ", " << name << "!" << endl;
}

// ---- User ----

User::User(const string& name, const string& id, int maxBooks)
    : Person(name, id), maxBorrowLimit(maxBooks)
{
    // borrowedISBNs starts empty -- user hasn't borrowed anything yet
}

User::~User()
{
}

string User::getInfo() const
{
    return "User: " + name + " | ID: " + personId +
           " | Borrowed: " + to_string(borrowedISBNs.size()) +
           "/" + to_string(maxBorrowLimit);
}

string User::getRole() const
{
    return "Library Member";
}

bool User::canBorrowBooks() const
{
    return (int)borrowedISBNs.size() < maxBorrowLimit;
}

void User::greet() const
{
    // Override of Person::greet() -- shows borrowed count instead of just ID
    cout << "Welcome back, " << name << "! You have "
         << borrowedISBNs.size() << " book(s) borrowed." << endl;
}

void User::borrowBook(const string& isbn)
{
    // Don't let the user borrow the same book twice
    if (hasBorrowed(isbn)) {
        throw InvalidDataException("You already have this book borrowed.");
    }
    if (!canBorrowBooks()) {
        throw BorrowLimitException(name, maxBorrowLimit);
    }
    borrowedISBNs.push_back(isbn);
}

void User::returnBook(const string& isbn)
{
    // std::find from <algorithm> gives us an iterator to the element,
    // or .end() if it's not in the list
    auto it = find(borrowedISBNs.begin(), borrowedISBNs.end(), isbn);
    if (it == borrowedISBNs.end()) {
        throw InvalidDataException(name + " does not have this book (ISBN: " + isbn + ").");
    }
    borrowedISBNs.erase(it);
}

bool User::hasBorrowed(const string& isbn) const
{
    return find(borrowedISBNs.begin(), borrowedISBNs.end(), isbn)
           != borrowedISBNs.end();
}

// ---- Librarian ----

Librarian::Librarian(const string& name, const string& id,
                     const string& empId, const string& dept)
    : Person(name, id), employeeId(empId), department(dept)
{
}

Librarian::~Librarian()
{
}

string Librarian::getInfo() const
{
    return "Librarian: " + name + " | EmpID: " + employeeId +
           " | Dept: " + department;
}

string Librarian::getRole() const
{
    return "Librarian";
}

// Librarians run the library -- they don't borrow books as regular members do
bool Librarian::canBorrowBooks() const
{
    return false;
}

void Librarian::greet() const
{
    cout << "Hello, librarian " << name
         << " — Department: " << department << endl;
}
