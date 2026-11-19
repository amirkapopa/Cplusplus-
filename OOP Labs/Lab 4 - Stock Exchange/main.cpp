//Amir Pulatov U2510164
#include <iostream>
#include "StockEx.h"


using namespace std;

int main() {

    StockEx market1, market2;

    cout << "=====Enter details for first market=====\n";
    cin >> market1;

    cout << "=====Enter details for second market=====\n";
    cin >> market2;

    cout << "\nFirst Market:\n";
    cout << market1;

    cout << "\nSecond Market:\n";
    cout << market2;

    int choice;
    float amount;

    cout << "\nChoose market for currency exchange:\n";
    cout << "1. First Market\n";
    cout << "2. Second Market\n";
    cout << "Enter your choice: ";
    cin >> choice;

    cout << "Enter amount to exchange: ";
    cin >> amount;

    float converted;

    if (choice == 1)
        converted = market1 * amount;
    else
        converted = market2 * amount;

    cout << "Converted amount: " << converted << endl;

    cout << "Difference between market rates: "
         << market1 - market2 << endl;

    if (market1 > market2)
        cout << "First market rate is higher.\n";
    else
        cout << "Second market rate is higher.\n";

    return 0;
}