//
// Created by netpopa on 19/02/26.
//

#ifndef UNTITLED_BOOK_H
#define UNTITLED_BOOK_H
#include <string>
using namespace std;




class Book {
public:
    //Constructors
    Book();
    Book(const Book& other );
    Book(int bookid, const string &title, const string &author, double price)
        : bookid(bookid),
          title(title),
          author(author),
          price(price<0 ? 0 : price) {
    }
    ~Book();

    int get_bookid() const;

    void set_bookid(int bookid);

    string get_title() const;

    void set_title(const string &title);

    string get_author() const;

    void set_author(const string &author);

    double get_price() const;

    void set_price(double price);

    void display() const;


private:
    int bookid;
    string title;
    string author;
    double price;






};


#endif //UNTITLED_BOOK_H