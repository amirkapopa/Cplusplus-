//
// Created by netpopa on 19/03/26.
//

#ifndef UNTITLED1_STUDENT_H
#define UNTITLED1_STUDENT_H
#include <iostream>
#include <string>
#include "Person.h"
using namespace std;


class Student: public Person {
private:
    string major;
    double gpa;
    int semester;
public:
    Student(string n,int a,int i,
           string m,double g,int s);
    void setStudent(string m,double g ,int s);
    void displayStudent();



};


#endif //UNTITLED1_STUDENT_H