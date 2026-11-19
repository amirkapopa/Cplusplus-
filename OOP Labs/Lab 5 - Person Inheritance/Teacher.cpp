//
// Created by netpopa on 19/03/26.
//

#include "Teacher.h"

Teacher::Teacher(string n, int a, int i, string d, double sal, string sub):Person(n,a,i){
    setTeacher(d,sal,sub);
}

void Teacher::setTeacher(string d, double sal, string sub) {
    department=d;
    salary=sal;
    if (sal>0)
        salary=sal;
    else {
        salary=0;
        cout<<" Invalid salary"<<endl;
    }

}

void Teacher::displayTeacher() {
    displayPerson();
    cout<<"Department: "<<department<<endl;
    cout<<"Salary: "<<salary<<endl;
    cout<<"Subject: "<<department<<endl;
    cout<<"------------------------------------\n";

}
