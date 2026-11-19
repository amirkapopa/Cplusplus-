#ifndef HOSPITAL_H
#define HOSPITAL_H

#include <string>
using namespace std;

class Hospital {
protected:
    string name;
    int    age;
    int    id;

public:
    Hospital(const string& name, int age, int id);

    virtual void   display()         const = 0;
    virtual double calculateSalary() const = 0;

    virtual ~Hospital() {}
};

#endif
