#pragma once
#include <string>
#include <vector>
using namespace std;

// C++ doesn't have an "interface" keyword like Java, but a class with only
// pure virtual functions and no data works exactly the same way.
// Both User and Librarian must implement getRole() and canBorrowBooks().
class IMember {
public:
    virtual ~IMember() = default;

    virtual string getRole()        const = 0;
    virtual bool   canBorrowBooks() const = 0;
};

// Abstract base class for any person in the system.
// Pure virtual getInfo() means you can't create a plain Person -- only subclasses.
class Person {
protected:
    string name;
    string personId;

    static int personCount;  // tracks total Person objects alive (similar to Book::bookCount)

public:
    Person(const string& name, const string& id);
    virtual ~Person();

    virtual string getInfo() const = 0;

    // Two versions of greet() -- one with no args, one with a custom message.
    // This demonstrates function overloading on a virtual function.
    virtual void greet() const;
    virtual void greet(const string& customMessage) const;

    string getName()     const { return name; }
    string getPersonId() const { return personId; }

    static int getPersonCount() { return personCount; }
};

// User inherits from both Person AND IMember -- that's multiple inheritance.
// It gets the name/id data from Person, and the borrowing interface from IMember.
class User : public Person, public IMember {
private:
    vector<string> borrowedISBNs;  // list of ISBNs the user currently has borrowed
    int maxBorrowLimit;            // each user can have a different limit (default 3)

public:
    User(const string& name, const string& id, int maxBooks = 3);
    ~User() override;

    string getInfo()        const override;
    string getRole()        const override;
    bool   canBorrowBooks() const override;

    void greet() const override;

    void borrowBook(const string& isbn);
    void returnBook(const string& isbn);
    bool hasBorrowed(const string& isbn) const;

    int            getBorrowedCount()  const { return (int)borrowedISBNs.size(); }
    int            getMaxBorrowLimit() const { return maxBorrowLimit; }
    vector<string> getBorrowedISBNs()  const { return borrowedISBNs; }
};

// Librarian also uses multiple inheritance (Person + IMember),
// but canBorrowBooks() returns false because librarians don't borrow as members do.
// We included this class mainly to show that the same parent class can have
// multiple different children (hierarchical inheritance).
class Librarian : public Person, public IMember {
private:
    string employeeId;
    string department;

public:
    Librarian(const string& name, const string& id,
              const string& empId, const string& dept);
    ~Librarian() override;

    string getInfo()        const override;
    string getRole()        const override;
    bool   canBorrowBooks() const override;

    void greet() const override;

    string getEmployeeId() const { return employeeId; }
    string getDepartment() const { return department; }
};
