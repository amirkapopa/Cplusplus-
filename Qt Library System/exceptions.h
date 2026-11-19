#pragma once
#include <stdexcept>
#include <string>
using namespace std;

// All our custom exceptions inherit from LibraryException,
// which itself inherits from the standard runtime_error.
// This lets us catch every library error with one catch block if we want,
// or catch specific types to handle them differently.

class LibraryException : public runtime_error {
public:
    explicit LibraryException(const string& msg)
        : runtime_error(msg) {}
};

// Thrown when a book with that ISBN doesn't exist in the catalogue
class BookNotFoundException : public LibraryException {
public:
    explicit BookNotFoundException(const string& isbn)
        : LibraryException("Book not found — ISBN: " + isbn) {}
};

// Thrown when the book exists but is currently borrowed by someone else
class BookNotAvailableException : public LibraryException {
public:
    explicit BookNotAvailableException(const string& title)
        : LibraryException("Book is not available: \"" + title + "\"") {}
};

class UserNotFoundException : public LibraryException {
public:
    explicit UserNotFoundException(const string& userId)
        : LibraryException("User not found — ID: " + userId) {}
};

// Thrown when a user already has the maximum number of books borrowed
class BorrowLimitException : public LibraryException {
public:
    explicit BorrowLimitException(const string& userName, int limit)
        : LibraryException(userName + " has reached the borrow limit of " +
                           to_string(limit) + " books") {}
};

class InvalidDataException : public LibraryException {
public:
    explicit InvalidDataException(const string& detail)
        : LibraryException("Invalid data — " + detail) {}
};

// Thrown if we can't open a CSV file (wrong path, no read permission, etc.)
class FileException : public LibraryException {
public:
    explicit FileException(const string& filename)
        : LibraryException("Cannot open file: " + filename) {}
};
