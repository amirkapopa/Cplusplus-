//Amir Pulatov U2510164
#include "StockEx.h"
#include <iostream>
using namespace std;

// Currency exchange using market rate
float StockEx::operator*(float amount) const {
    return amount * mktrate;
}

// Difference between two market rates
float StockEx::operator-(const StockEx& other) const {
    return mktrate - other.mktrate;
}

// Compare market rates
bool StockEx::operator>(const StockEx& other) const {
    return mktrate > other.mktrate;
}

// Print stock details
ostream& operator<<(ostream& out, const StockEx& obj) {
    out << "Currency: " << obj.currency
        << ", Market Rate: " << obj.mktrate
        << ", Date: " << obj.date;
    return out;
}

// Input stock details
istream& operator>>(istream& in, StockEx& obj) {
    cout << "Enter currency: ";
    in >> obj.currency;
    cout << "Enter market rate: ";
    in >> obj.mktrate;
    cout << "Enter date (YYYY-MM-DD): ";
    in >> obj.date;
    return in;
}