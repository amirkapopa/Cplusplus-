// ============================================================
// U2510164  Amir Pulatov
// Exercise 9: Employee Record File Manager
// Employee.h - Employee class definition
// ============================================================

#ifndef EMPLOYEE_H
#define EMPLOYEE_H

#include <string>
using namespace std;

class Employee {
public:
    // Constructors
    Employee();
    Employee(int id, string name, double salary);

    // Setters
    void setID(int id);
    void setName(string name);
    void setSalary(double salary);

    // Getters
    int    getID()     const;
    string getName()   const;
    double getSalary() const;

    // Utility
    bool isEmpty() const;   // returns true if record is blank (id == 0)

private:
    int    id;
    char   name[20];
    double salary;
};

#endif // EMPLOYEE_H
