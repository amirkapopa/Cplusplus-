//
// Created by netpopa on 19/03/26.
//

#include "Staff.h"

Staff::Staff(string n, int a, int i, string p, double ms, int wh):Person(n,a,i) {
    setStaff(p,ms,wh);
}

void Staff::setStaff(string p, double ms, int wh) {
    position=p;

    if (ms>0)
        monthlySalary=ms;
    else {
        monthlySalary=0;
        cout<<"Invalid salary"<<endl;
    }
    if (wh>0)
        workingHours=wh;
    else {
        workingHours=0;
        cout<<"Invalid hours"<<endl;
    }
}//void

void Staff::displayStaff() {
    displayPerson();
    cout<<"Staff name: "<<position<<endl;
    cout<<"Salary: "<<monthlySalary<<endl;
    cout<<"Working Hours: "<<workingHours<<endl;
    cout<<"---------------------------------\n";
}
