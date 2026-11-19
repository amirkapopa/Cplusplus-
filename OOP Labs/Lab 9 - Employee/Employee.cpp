// ============================================================
// U2510164  Amir Pulatov
// Exercise 9: Employee Record File Manager
// Employee.cpp - Employee class implementation
// ============================================================

#include "Employee.h"
#include <cstring>
#include <stdexcept>
using namespace std;

// Default constructor — creates a blank/empty record
Employee::Employee() {
    id     = 0;
    salary = 0.0;
    name[0] = '\0';
}

// Parameterized constructor
Employee::Employee(int id, string name, double salary) {
    setID(id);
    setName(name);
    setSalary(salary);
}

// ---- Setters ----

void Employee::setID(int id) {
    this->id = id;
}

void Employee::setName(string n) {
    // Copy safely into fixed char array — max 19 chars + null terminator
    int length = (n.size() < 19) ? (int)n.size() : 19;
    for (int i = 0; i < length; i++)
        name[i] = n[i];
    name[length] = '\0';
}

void Employee::setSalary(double salary) {
    if (salary < 0)
        throw invalid_argument("Salary cannot be negative!");
    this->salary = salary;
}

// ---- Getters ----

int Employee::getID() const {
    return id;
}

string Employee::getName() const {
    return string(name);
}

double Employee::getSalary() const {
    return salary;
}

// ---- Utility ----

bool Employee::isEmpty() const {
    return (id == 0);
}
