#ifndef DOCTOR_H
#define DOCTOR_H

#include "Hospital.h"
#include <vector>

class Doctor : public Hospital {
private:
    string         specialization;
    double         monthlySalary;
    vector<string> patients;

public:
    Doctor(const string& name, int age, int id,
           const string& specialization, double monthlySalary);

    void   addPatient(const string& patientName);
    double calculateSalary() const override;
    void   display()         const override;
};

#endif
