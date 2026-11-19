//Amir Pulatov U2510164
#include "Student.h"
#include "Teacher.h"
#include "Staff.h"
#include <iostream>
using namespace std;
int main() {
    Student s1("Amir",17,10164,"Socie",3.5,1);
    Student s2("Roman",18,10165,"Socie",3.6,1);

    Teacher t1("Mr.Oybek",40,10100,"Creative Engineering ",5000,"Programming");
    Teacher t2("Ms. Aisha", 35, 202, "Calculus", 4500, "Algebra");

    Staff st1("John", 30, 301, "Curator", 2000, 6);
    Staff st2("Mike", 28, 302, "Technician", 2500, 5);

    cout<<"----Studet Details----\n";
    s1.displayStudent();
    s2.displayStudent();

    cout<<"----Teacher Details-----\n";
    t1.displayTeacher();
    t2.displayTeacher();


    cout<<"----Staff Details-----\n";
    st1.displayStaff();
    st2.displayStaff();
    return 0;


}

