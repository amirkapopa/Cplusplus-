#include "Doctor.h"
#include <iostream>
#include <stdexcept>

Doctor::Doctor(const string& name, int age, int id,
               const string& specialization, double monthlySalary)
    : Hospital(name, age, id), specialization(specialization)
{
    if (monthlySalary <= 0)
        throw invalid_argument("Doctor salary must be positive.");
    this->monthlySalary = monthlySalary;
}

void Doctor::addPatient(const string& patientName) {
    patients.push_back(patientName);
}

double Doctor::calculateSalary() const {
    return monthlySalary;
}

void Doctor::display() const {
    cout << "========================================\n";
    cout << "  [DOCTOR]\n";
    cout << "  Name           : " << name           << "\n";
    cout << "  Age            : " << age            << "\n";
    cout << "  ID             : " << id             << "\n";
    cout << "  Specialization : " << specialization << "\n";
    cout << "  Monthly Salary : $" << calculateSalary() << "\n";
    cout << "  Patients       : ";
    if (patients.empty()) {
        cout << "None";
    } else {
        for (size_t i = 0; i < patients.size(); ++i) {
            cout << patients[i];
            if (i + 1 < patients.size()) cout << ", ";
        }
    }
    cout << "\n========================================\n";
}
