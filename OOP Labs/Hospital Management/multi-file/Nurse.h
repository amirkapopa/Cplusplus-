#ifndef NURSE_H
#define NURSE_H

#include "Hospital.h"
#include <vector>

class Nurse : public Hospital {
private:
    string         ward;
    double         monthlySalary;
    vector<string> duties;

public:
    Nurse(const string& name, int age, int id,
          const string& ward, double monthlySalary);

    void   addDuty(const string& duty);
    double calculateSalary() const override;
    void   display()         const override;
};

#endif
