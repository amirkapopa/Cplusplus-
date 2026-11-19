#include "book.h"
#include <iostream>
using namespace std;

// Static members must be defined outside the class in the .cpp file.
// Forgetting this line causes a linker error -- we got that error the first time.
int Book::bookCount = 0;

Book::Book(const string& title, const string& author,
           const string& isbn, int year)
    : title(title), author(author), isbn(isbn), year(year), available(true)
{
    // Every time a new book is created, the counter goes up.
    // The destructor brings it back down.
    // So bookCount always shows how many Book objects are alive at that moment.
    ++bookCount;
}

Book::~Book()
{
    --bookCount;
}

void Book::display() const
{
    // getType() is virtual -- at runtime this calls FictionBook::getType()
    // or NonFictionBook::getType() depending on the actual object type.
    // This is runtime polymorphism in action.
    cout << "[" << getType() << "] " << title
         << " by " << author
         << " (" << year << ") ISBN: " << isbn
         << " — " << (available ? "Available" : "Borrowed")
         << endl;
}

// Overloaded version of display() -- same name, extra parameter.
// Calling display() with no args uses the version above.
// Calling display(true) uses this one.
void Book::display(bool showDetails) const
{
    display();
    if (showDetails) {
        cout << "  Details: " << getDescription() << endl;
    }
}

// Two books are equal if they share the same ISBN.
// ISBN is a globally unique identifier for each book edition, so this logic is correct.
bool Book::operator==(const Book& other) const
{
    return isbn == other.isbn;
}

// "Greater" = published in a later year
bool Book::operator>(const Book& other) const
{
    return year > other.year;
}

// We need "friend" here because operator<< is not a member function of Book,
// but it still needs access to the protected fields title and isbn.
ostream& operator<<(ostream& os, const Book& book)
{
    os << book.title << " (" << book.isbn << ")";
    return os;
}

// ---- FictionBook ----

FictionBook::FictionBook(const string& title, const string& author,
                         const string& isbn, int year,
                         const string& genre)
    : Book(title, author, isbn, year), genre(genre)
{
    // Book constructor runs first through the initializer list,
    // then this body runs to finish setting up FictionBook's own data.
}

FictionBook::~FictionBook()
{
    // Book's destructor is called automatically after this returns.
}

string FictionBook::getType() const
{
    return "Fiction";
}

string FictionBook::getDescription() const
{
    return "Genre: " + genre;
}

void FictionBook::display() const
{
    Book::display();   // call parent version first, then add the extra line
    cout << "  Genre: " << genre << endl;
}

// ---- NonFictionBook ----

NonFictionBook::NonFictionBook(const string& title, const string& author,
                               const string& isbn, int year,
                               const string& subject)
    : Book(title, author, isbn, year), subject(subject)
{
}

NonFictionBook::~NonFictionBook()
{
}

string NonFictionBook::getType() const
{
    return "Non-Fiction";
}

string NonFictionBook::getDescription() const
{
    return "Subject: " + subject;
}

void NonFictionBook::display() const
{
    Book::display();
    cout << "  Subject: " << subject << endl;
}
