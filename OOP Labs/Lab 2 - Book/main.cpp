#include <iostream>
#include "Book.h"
using namespace std;
int main(){
    Book book1;
    Book book2(1,
        "C++ Basics",
        "Amir",
        45.5);
    Book book3=book2;
    cout<<"\nBook 1 details:\n"<<endl;
    book1.display();
    cout<<"\nBook 2 details:\n"<<endl;
    book2.display();
    cout<<"\nBook 3 details:\n"<<endl;
    book3.display();






 return 0;



}

