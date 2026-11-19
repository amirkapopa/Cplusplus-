//
// Created by netpopa on 19/03/26.
//

#include "Student.h"

Student::Student(string n, int a, int i,
    string m, double g, int s) :Person (n,a,i) {
    setStudent(m, g ,s);
}

void Student::setStudent(string m, double g, int s) {
    major=m;

    if (g>0.0 && g<=4.0) {
        gpa=g;
    }
    else {
        gpa=0.0;
        cout<<"Invalid GPA"<<endl;
    }
    if (s>0)
        semester=s;
    else {
        semester=1;
        cout<<"Invalid semester"<<endl;
    }

}//void

void Student::displayStudent() {
    displayPerson();
    cout<<"Major: "<<major<<endl;
    cout<<"GPA: "<<gpa<<endl;
    cout<<"Semester: "<<semester<<endl;
    cout<<"--------------------------\n";
}
