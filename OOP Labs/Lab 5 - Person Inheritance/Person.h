
#ifndef UNTITLED1_PERSON_H
#define UNTITLED1_PERSON_H
#include <iostream>
#include <string>
using namespace std;

class Person {
protected:
    string name;
    int age;
    int id;

public:
   Person(string n, int a, int i);
    void setPerson(string n, int a, int i);
    void displayPerson();


};


#endif //UNTITLED1_PERSON_H