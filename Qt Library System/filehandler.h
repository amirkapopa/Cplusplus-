#pragma once
#include "book.h"
#include "person.h"
#include <vector>
#include <memory>
#include <string>
using namespace std;

// FileHandler is a "static utility class" -- all its methods are static and
// there's no point in ever creating an instance of it.
// Deleting the constructor enforces this: FileHandler fh; would be a compile error.
class FileHandler {
private:
    FileHandler() = delete;  // blocks instantiation

    static const string BOOKS_FILE;
    static const string USERS_FILE;

public:
    static void saveBooks(const vector<shared_ptr<Book>>& books,
                          const string& filename = "");

    static vector<shared_ptr<Book>> loadBooks(const string& filename = "");

    static void saveUsers(const vector<shared_ptr<User>>& users,
                          const string& filename = "");

    static vector<shared_ptr<User>> loadUsers(const string& filename = "");

    // Convenience wrapper -- saves both files in one call
    static void saveAll(const vector<shared_ptr<Book>>& books,
                        const vector<shared_ptr<User>>& users);

    // CSV parsing helpers used by both load functions
    static vector<string> splitCSV(const string& line, char delim = ',');
    static string escapeCSV(const string& s);
};
