//U2510164 Amir Pulatov

// 1. You are required to implement a Singly Linked List in C++ with the following
// operations:
// a) Insert a node at the end of the list.
// b) Insert a node at the beginning of the list.
// c) Delete a node with a given key (value).
// d) Display all elements of the list.
// e) Count total number of occurrences of a given item in the Linked List
// f) Reverse the List.



#include <iostream>
using namespace std;


struct Node {
    int data;
    Node* next;

    Node(int  data ) {
        this->data = data;
        next = NULL;
    }
};

Node* head=NULL;

//1.Inserting node at the beginning
void insertAtFront(int data) {
    Node* newNode=new Node(data);
    newNode->next=head;
    head=newNode;


}

//2.Insert at End

void insertAtEnd(int data) {
    Node* newNode=new Node(data);
        if (head == NULL) {
        head=newNode;
        return;
    }

    Node* current=head;
    while (current->next!=NULL) {
        current=current->next;// walk to the last node
    }
    current->next=newNode;
}

//3. Delete a Node
void deleteNode (int data) {

    if (head == NULL) return;

    if (head-> data == data) {
        head = head->next;
        return;
    }

    Node* current = head;
    while ( current->next != NULL) {
        if (current->next->data == data) {
            current->next = current->next->next;
            return;
        }
        current = current->next;
    }
}

//4.Display

void display() {
    if (head == NULL) {
        cout<<"List is empty"<<endl;
        return;
    }
    Node* current = head;
    while (current != NULL) {
        cout<<current->data<<" -> ";
        current = current->next;
    }
    cout<<"NULL"<<endl;
}


//5 Count occurrences
int countOccurrence(int item) {
    int count =0;
    Node* current = head;

    while (current != NULL) {
        if (current->data == item) {
            count++;
        }
        current = current->next;
    }
    return count;
}

//6 Reverse

void reverse() {
    Node* current = head;
    Node* prev = NULL;
    Node* temp = NULL;

    while (current != NULL) {
        temp = current->next;
        current->next = prev;
        prev = current;
        current = temp;
    }
    head = prev;
}



int main() {
    insertAtEnd(10);
    insertAtEnd(20);
    insertAtEnd(10);
    insertAtEnd(30);
    insertAtFront(5);

    cout << "List: ";
    display();

    deleteNode(20);
    cout << "After deleting 20: ";
    display();

    cout << "Count of 10: " << countOccurrence(10) << endl;

    reverse();
    cout << "Reversed: ";
    display();

    return 0;


}