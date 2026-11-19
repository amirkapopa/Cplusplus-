//
// Created by netpopa on 19/03/26.
//

#ifndef UNTITLED1_STAFF_H
#define UNTITLED1_STAFF_H
#include <iostream>

#include "Person.h"
using namespace std;


class Staff: public Person {
private:
    string position;
    double monthlySalary;
    int workingHours;

public:
    Staff(string n, int a, int i,string p, double ms,int wh);
    void setStaff(string p, double ms, int wh);
    void displayStaff();
};


#endif //UNTITLED1_STAFF_H