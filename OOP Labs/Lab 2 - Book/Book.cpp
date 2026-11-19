//
// Created by netpopa on 19/02/26.
//

#include "Book.h"
#include <iostream>
using namespace std;

Book::Book() {
    bookid=0;
    title="Unknown";
    author="Unknown";
    price=0.0;
}

Book::Book(const Book &other) {
    bookid=other.bookid;
    title=other.title;
    author=other.author;
    price=other.price;

}

Book::~Book() {
    cout<<"Book deleted"<<endl;
}

int Book::get_bookid() const {
    return bookid;
}

void Book::set_bookid(int bookid) {
    this->bookid = bookid;
}

string Book::get_title() const {
    return title;
}

void Book::set_title(const string &title) {
    this->title = title;
}

string Book::get_author() const {
    return author;
}

void Book::set_author(const string &author) {
    this->author = author;
}

double Book::get_price() const {
    return price;
}

void Book::set_price(double price) {
    if (price < 0.0)
        this->price = 0.0;
    else
        this->price = price;





}

void Book::display() const {
    cout << "Book ID: " << bookid << endl;
    cout << "Title: " << title << endl;
    cout << "Author: " << author << endl;
    cout << "Price: " << price << endl;

}
