#include "Technician.h"
#include <iostream>
#include <stdexcept>

Technician::Technician(const string& name, int age, int id,
                       const string& labType, double hourlyRate, int workingHours)
    : Hospital(name, age, id), labType(labType)
{
    if (hourlyRate <= 0)
        throw invalid_argument("Technician hourly rate must be positive.");
    if (workingHours <= 0)
        throw invalid_argument("Technician working hours must be positive.");
    this->hourlyRate   = hourlyRate;
    this->workingHours = workingHours;
}

void Technician::addMachine(const string& machineName) {
    machinesHandled.push_back(machineName);
}

double Technician::calculateSalary() const {
    return hourlyRate * workingHours;
}

void Technician::display() const {
    cout << "========================================\n";
    cout << "  [TECHNICIAN]\n";
    cout << "  Name           : " << name         << "\n";
    cout << "  Age            : " << age          << "\n";
    cout << "  ID             : " << id           << "\n";
    cout << "  Lab Type       : " << labType      << "\n";
    cout << "  Hourly Rate    : $" << hourlyRate  << "\n";
    cout << "  Working Hours  : "  << workingHours << " hrs\n";
    cout << "  Total Salary   : $" << calculateSalary() << "\n";
    cout << "  Machines       : ";
    if (machinesHandled.empty()) {
        cout << "None";
    } else {
        for (size_t i = 0; i < machinesHandled.size(); ++i) {
            cout << machinesHandled[i];
            if (i + 1 < machinesHandled.size()) cout << ", ";
        }
    }
    cout << "\n========================================\n";
}
