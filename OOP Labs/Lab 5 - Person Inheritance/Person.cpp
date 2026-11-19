//
// Created by netpopa on 19/03/26.
//

#include "Person.h"

Person::Person(string n, int a, int i) {
    setPerson(n, a, i);
}

void Person::setPerson(string n, int a, int i) {
    name=n;
    if (a>0)
        age=a;
    else {
        age = 0;
        cout<<"Invalid age"<<endl;
    }
    if (i>0)
        id=i;
    else {
        id = 0;
        cout<<"Invalid id"<<endl;
    }

}//void

void Person::displayPerson() {
    cout<<"Name: "<<name<<endl;
    cout<<"Age: "<<age<<endl;
    cout<<"ID: "<<id<<endl;
    cout<<"---------------\n";
}