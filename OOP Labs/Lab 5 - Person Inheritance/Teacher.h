//
// Created by netpopa on 19/03/26.
//

#ifndef UNTITLED1_TEACHER_H
#define UNTITLED1_TEACHER_H
#include "Person.h"
#include <iostream>
using namespace std;



class Teacher: public Person {
private:
    string department;
    double salary;
    string subject;
public:
    Teacher(string n, int a, int i, string d,double sal,string sub);
    void setTeacher(string d, double sal, string sub);
    void displayTeacher();
};


#endif //UNTITLED1_TEACHER_H