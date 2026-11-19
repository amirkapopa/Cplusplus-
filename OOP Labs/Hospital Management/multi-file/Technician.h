#ifndef TECHNICIAN_H
#define TECHNICIAN_H

#include "Hospital.h"
#include <vector>

class Technician : public Hospital {
private:
    string         labType;
    double         hourlyRate;
    int            workingHours;
    vector<string> machinesHandled;

public:
    Technician(const string& name, int age, int id,
               const string& labType, double hourlyRate, int workingHours);

    void   addMachine(const string& machineName);
    double calculateSalary() const override;
    void   display()         const override;
};

#endif
