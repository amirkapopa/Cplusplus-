// ============================================================
// U2510164  Amir Pulatov
// Exercise 9: Employee Record File Manager
// main.cpp - Menu-driven Employee Record Manager
// ============================================================

#include <iostream>
#include <fstream>
#include <iomanip>
#include <stdexcept>
#include <string>
#include "Employee.h"

using namespace std;

// ---- Constants ----
const string TEXT_FILE   = "employees.txt";
const string BINARY_FILE = "employees.dat";
const int    MAX_RECORDS = 10;

// ---- Function Prototypes ----
int    showMenu();
void   createTextFile();
void   createBinaryFile();
void   addRecord();
void   deleteRecord();
void   updateRecord();
void   displayTextFile();
void   displayBinaryFile();
void   validatePosition(int pos);

// ============================================================
// MAIN
// ============================================================
int main() {
    cout << "============================================" << endl;
    cout << "  U2510164  Amir Pulatov                  " << endl;
    cout << "  Employee Record File Manager             " << endl;
    cout << "============================================" << endl;

    int choice;
    do {
        choice = showMenu();
        try {
            switch (choice) {
                case 1: createTextFile();   break;
                case 2: createBinaryFile(); break;
                case 3: addRecord();        break;
                case 4: deleteRecord();     break;
                case 5: updateRecord();     break;
                case 6: displayTextFile();  break;
                case 7: displayBinaryFile();break;
                case 8:
                    cout << "\nExiting program. Goodbye!\n";
                    break;
                default:
                    throw invalid_argument("Invalid menu choice! Enter 1-8.");
            }
        }
        catch (const invalid_argument& e) {
            cout << "\n[EXCEPTION - Invalid Argument] " << e.what() << endl;
        }
        catch (const runtime_error& e) {
            cout << "\n[EXCEPTION - Runtime Error] " << e.what() << endl;
        }
        catch (const exception& e) {
            cout << "\n[EXCEPTION] " << e.what() << endl;
        }

    } while (choice != 8);

    return 0;
}

// ============================================================
// Show menu and return user choice
// ============================================================
int showMenu() {
    int choice;
    cout << "\n--------------------------------------------" << endl;
    cout << "  MENU" << endl;
    cout << "--------------------------------------------" << endl;
    cout << "  1. Create text file and save 5 records" << endl;
    cout << "  2. Create binary file with 10 empty slots" << endl;
    cout << "  3. Add record to binary file" << endl;
    cout << "  4. Delete record from binary file" << endl;
    cout << "  5. Update record in binary file" << endl;
    cout << "  6. Display text file" << endl;
    cout << "  7. Display binary file" << endl;
    cout << "  8. Exit" << endl;
    cout << "--------------------------------------------" << endl;
    cout << "  Enter choice: ";
    cin  >> choice;
    return choice;
}

// ============================================================
// TASK 1 — Create text file with 5 employee records
// ============================================================
void createTextFile() {
    ofstream file(TEXT_FILE);
    if (!file)
        throw runtime_error("Cannot open file: " + TEXT_FILE);

    // 5 hardcoded employee records
    Employee employees[5] = {
        Employee(1, "Ali Karimov",   1500.0),
        Employee(2, "Bobur Rahimov", 2000.0),
        Employee(3, "Camila Yusup",  1750.0),
        Employee(4, "Davron Aliev",  2200.0),
        Employee(5, "Elena Smirnov", 1900.0)
    };

    file << left
         << setw(6)  << "ID"
         << setw(22) << "Name"
         << setw(10) << "Salary" << endl;
    file << string(38, '-') << endl;

    for (int i = 0; i < 5; i++) {
        file << left
             << setw(6)  << employees[i].getID()
             << setw(22) << employees[i].getName()
             << setw(10) << employees[i].getSalary() << endl;
    }

    file.close();
    cout << "\n[SUCCESS] 5 employee records written to " << TEXT_FILE << endl;
}

// ============================================================
// TASK 2 — Create binary file with 10 empty records
// ============================================================
void createBinaryFile() {
    ofstream file(BINARY_FILE, ios::binary);
    if (!file)
        throw runtime_error("Cannot open file: " + BINARY_FILE);

    Employee blank;   // default constructor → id=0, name="", salary=0
    for (int i = 0; i < MAX_RECORDS; i++)
        file.write(reinterpret_cast<char*>(&blank), sizeof(Employee));

    file.close();
    cout << "\n[SUCCESS] Binary file created with " << MAX_RECORDS
         << " empty slots: " << BINARY_FILE << endl;
}

// ============================================================
// Validate position: must be 1 to MAX_RECORDS
// ============================================================
void validatePosition(int pos) {
    if (pos < 1 || pos > MAX_RECORDS) {
        throw out_of_range(
            "Invalid position: " + to_string(pos) +
            ". Must be between 1 and " + to_string(MAX_RECORDS) + "."
        );
    }
}

// ============================================================
// TASK 3 — Add a record at a specific position
// ============================================================
void addRecord() {
    int pos;
    cout << "\nEnter position (1-" << MAX_RECORDS << "): ";
    cin  >> pos;
    validatePosition(pos);   // throws if invalid

    fstream file(BINARY_FILE, ios::binary | ios::in | ios::out);
    if (!file)
        throw runtime_error("Cannot open file: " + BINARY_FILE +
                            ". Create the binary file first (option 2).");

    // Read existing record at that position
    Employee existing;
    file.seekg((pos - 1) * sizeof(Employee), ios::beg);
    file.read(reinterpret_cast<char*>(&existing), sizeof(Employee));

    if (!existing.isEmpty())
        throw runtime_error("Position " + to_string(pos) +
                            " already has a record. Delete it first.");

    // Get new record details
    int    id;
    string name;
    double salary;

    cout << "Enter ID: ";     cin >> id;
    cout << "Enter Name: ";   cin.ignore(); getline(cin, name);
    cout << "Enter Salary: "; cin >> salary;

    Employee emp(id, name, salary);   // setSalary throws if salary < 0

    file.seekp((pos - 1) * sizeof(Employee), ios::beg);
    file.write(reinterpret_cast<char*>(&emp), sizeof(Employee));
    file.close();

    cout << "\n[SUCCESS] Record added at position " << pos << endl;
}

// ============================================================
// TASK 4 — Delete a record at a specific position
// ============================================================
void deleteRecord() {
    int pos;
    cout << "\nEnter position to delete (1-" << MAX_RECORDS << "): ";
    cin  >> pos;
    validatePosition(pos);   // throws if invalid

    fstream file(BINARY_FILE, ios::binary | ios::in | ios::out);
    if (!file)
        throw runtime_error("Cannot open file: " + BINARY_FILE +
                            ". Create the binary file first (option 2).");

    // Check if the record is already empty
    Employee existing;
    file.seekg((pos - 1) * sizeof(Employee), ios::beg);
    file.read(reinterpret_cast<char*>(&existing), sizeof(Employee));

    if (existing.isEmpty())
        throw runtime_error("Position " + to_string(pos) +
                            " is already empty — nothing to delete.");

    // Overwrite with a blank record
    Employee blank;
    file.seekp((pos - 1) * sizeof(Employee), ios::beg);
    file.write(reinterpret_cast<char*>(&blank), sizeof(Employee));
    file.close();

    cout << "\n[SUCCESS] Record at position " << pos << " deleted." << endl;
}

// ============================================================
// TASK 5 — Update name or salary of a record
// ============================================================
void updateRecord() {
    int pos;
    cout << "\nEnter position to update (1-" << MAX_RECORDS << "): ";
    cin  >> pos;
    validatePosition(pos);   // throws if invalid

    fstream file(BINARY_FILE, ios::binary | ios::in | ios::out);
    if (!file)
        throw runtime_error("Cannot open file: " + BINARY_FILE +
                            ". Create the binary file first (option 2).");

    // Read the existing record
    Employee emp;
    file.seekg((pos - 1) * sizeof(Employee), ios::beg);
    file.read(reinterpret_cast<char*>(&emp), sizeof(Employee));

    if (emp.isEmpty())
        throw runtime_error("Cannot update — position " +
                            to_string(pos) + " is empty.");

    // Show current values
    cout << "\nCurrent Record:" << endl;
    cout << "  ID:     " << emp.getID()     << endl;
    cout << "  Name:   " << emp.getName()   << endl;
    cout << "  Salary: " << emp.getSalary() << endl;

    // Update choice
    int choice;
    cout << "\nWhat to update?" << endl;
    cout << "  1. Name" << endl;
    cout << "  2. Salary" << endl;
    cout << "  3. Both" << endl;
    cout << "  Enter choice: ";
    cin  >> choice;

    if (choice == 1 || choice == 3) {
        string newName;
        cout << "Enter new name: ";
        cin.ignore(); getline(cin, newName);
        emp.setName(newName);
    }
    if (choice == 2 || choice == 3) {
        double newSalary;
        cout << "Enter new salary: ";
        cin >> newSalary;
        emp.setSalary(newSalary);   // throws if negative
    }

    // Write updated record back
    file.seekp((pos - 1) * sizeof(Employee), ios::beg);
    file.write(reinterpret_cast<char*>(&emp), sizeof(Employee));
    file.close();

    cout << "\n[SUCCESS] Record at position " << pos << " updated." << endl;
}

// ============================================================
// DISPLAY — Show text file contents
// ============================================================
void displayTextFile() {
    ifstream file(TEXT_FILE);
    if (!file)
        throw runtime_error("Cannot open " + TEXT_FILE +
                            ". Create it first (option 1).");

    cout << "\n--- Contents of " << TEXT_FILE << " ---" << endl;
    string line;
    while (getline(file, line))
        cout << line << endl;

    file.close();
}

// ============================================================
// DISPLAY — Show binary file contents
// ============================================================
void displayBinaryFile() {
    ifstream file(BINARY_FILE, ios::binary);
    if (!file)
        throw runtime_error("Cannot open " + BINARY_FILE +
                            ". Create it first (option 2).");

    cout << "\n--- Contents of " << BINARY_FILE << " ---" << endl;
    cout << left
         << setw(6)  << "Pos"
         << setw(8)  << "ID"
         << setw(22) << "Name"
         << setw(10) << "Salary"
         << "Status" << endl;
    cout << string(55, '-') << endl;

    Employee emp;
    int pos = 1;
    while (file.read(reinterpret_cast<char*>(&emp), sizeof(Employee))) {
        cout << left << setw(6) << pos++;
        if (emp.isEmpty()) {
            cout << setw(8) << "-"
                 << setw(22) << "-"
                 << setw(10) << "-"
                 << "[EMPTY]" << endl;
        } else {
            cout << setw(8)  << emp.getID()
                 << setw(22) << emp.getName()
                 << setw(10) << emp.getSalary()
                 << "[ACTIVE]" << endl;
        }
    }

    file.close();
}
