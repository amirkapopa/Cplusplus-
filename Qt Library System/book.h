#pragma once
#include <string>
#include <iostream>
using namespace std;

// Base class for all book types.
// We made it abstract (pure virtual functions) so you can never create a plain
// Book object -- you have to use FictionBook or NonFictionBook.
// That's the whole point: force every subclass to define what "type" it is.
class Book {
protected:
    string title;
    string author;
    string isbn;      // ISBN is globally unique per edition -- we use it as the key
    int    year;
    bool   available;

    // Static means this variable belongs to the CLASS, not to any single object.
    // All Book instances share the same bookCount, so it always tells us
    // how many Book objects exist right now in memory.
    static int bookCount;

public:
    Book(const string& title, const string& author,
         const string& isbn, int year);

    // Virtual destructor is required here. Without it, deleting a FictionBook
    // through a Book* pointer would skip FictionBook's destructor entirely.
    virtual ~Book();

    // Pure virtual -- every subclass MUST override these, no default version exists
    virtual string getType()        const = 0;
    virtual string getDescription() const = 0;

    // Two display() functions with the same name but different signatures.
    // The compiler picks the right one based on how it's called (function overloading).
    virtual void display() const;
    void display(bool showDetails) const;

    string getTitle()    const { return title; }
    string getAuthor()   const { return author; }
    string getISBN()     const { return isbn; }
    int    getYear()     const { return year; }
    bool   isAvailable() const { return available; }

    void setAvailable(bool val) { available = val; }

    static int getBookCount() { return bookCount; }

    // Overloaded operators so we can compare and print books naturally
    bool operator==(const Book& other) const;
    bool operator>(const Book& other)  const;
    friend ostream& operator<<(ostream& os, const Book& book);
};

// FictionBook inherits from Book -- single inheritance
class FictionBook : public Book {
private:
    string genre;

public:
    FictionBook(const string& title, const string& author,
                const string& isbn, int year,
                const string& genre = "General Fiction");
    ~FictionBook() override;

    string getType()        const override;
    string getDescription() const override;
    void   display()        const override;

    string getGenre() const { return genre; }
};

// NonFictionBook also inherits from Book.
// Same parent, two different children -- that's hierarchical inheritance.
class NonFictionBook : public Book {
private:
    string subject;

public:
    NonFictionBook(const string& title, const string& author,
                   const string& isbn, int year,
                   const string& subject = "General");
    ~NonFictionBook() override;

    string getType()        const override;
    string getDescription() const override;
    void   display()        const override;

    string getSubject() const { return subject; }
};
