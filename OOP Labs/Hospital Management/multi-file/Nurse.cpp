#include "Nurse.h"
#include <iostream>
#include <stdexcept>

Nurse::Nurse(const string& name, int age, int id,
             const string& ward, double monthlySalary)
    : Hospital(name, age, id), ward(ward)
{
    if (monthlySalary <= 0)
        throw invalid_argument("Nurse salary must be positive.");
    this->monthlySalary = monthlySalary;
}

void Nurse::addDuty(const string& duty) {
    duties.push_back(duty);
}

double Nurse::calculateSalary() const {
    return monthlySalary;
}

void Nurse::display() const {
    cout << "========================================\n";
    cout << "  [NURSE]\n";
    cout << "  Name           : " << name  << "\n";
    cout << "  Age            : " << age   << "\n";
    cout << "  ID             : " << id    << "\n";
    cout << "  Ward           : " << ward  << "\n";
    cout << "  Monthly Salary : $" << calculateSalary() << "\n";
    cout << "  Duties         : ";
    if (duties.empty()) {
        cout << "None";
    } else {
        for (size_t i = 0; i < duties.size(); ++i) {
            cout << duties[i];
            if (i + 1 < duties.size()) cout << ", ";
        }
    }
    cout << "\n========================================\n";
}
