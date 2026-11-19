#ifndef STOCKEX_H
#define STOCKEX_H
//Amir Pulatov U2510164
#include <iostream>
#include <string>
using namespace std;

class StockEx {
private:
    string currency;
    float mktrate;
    string date;

public:
    // Parameterized constructor
    StockEx(const string &currency = "", float m = 0.0, const string &date = "")
        : currency(currency), mktrate(m), date(date) {}

    // Operator overloading
    float operator*(float amount) const;                 // currency exchange
    float operator-(const StockEx& other) const;        // difference between rates
    bool operator>(const StockEx& other) const;        // compare market rates

    // Friend operators for input/output
    friend ostream& operator<<(ostream& out, const StockEx& obj);
    friend istream& operator>>(istream& in, StockEx& obj);

    // Getters for private members
    string getCurrency() const { return currency; }
    float getMktrate() const { return mktrate; }
    string getDate() const { return date; }

};

#endif // STOCKEX_H