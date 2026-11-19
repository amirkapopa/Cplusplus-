#include <iostream>
using namespace std;
//U2510164 Amir Pulatov
//Assigment-2
struct Node {
    int data;
    Node* next;
    Node* prev;
    Node(int data) {
        this->data = data;
        next = NULL;
        prev = NULL;
    }
};

Node* head = NULL;
Node* current = NULL;

// 1. Move to next node
void moveNext() {
    if (current == NULL) { cout << "List is empty" << endl; return; }
    if (current->next == NULL) { cout << "Already at last node" << endl; return; }
    current = current->next;
}

// 2. Move to previous node
void movePrev() {
    if (current == NULL) { cout << "List is empty" << endl; return; }
    if (current->prev == NULL) { cout << "Already at first node" << endl; return; }
    current = current->prev;
}

// 3. Insert after current (delete everything after current first)
void insertAfterCurrent(int data) {
    if (current == NULL) { cout << "No current node" << endl; return; }
    current->next = NULL;          // cut everything after current
    Node* newNode = new Node(data);
    newNode->prev = current;       // new node points back to current
    current->next = newNode;       // current points forward to new node
}

// 4. Delete current node
void deleteCurrent() {
    if (current == NULL) { cout << "Nothing to delete" << endl; return; }
    Node* toDelete = current;
    if (current->next != NULL) {
        current = current->next;       // move current to next
        current->prev = toDelete->prev; // fix backward link
    } else {
        current = current->prev;       // no next, go to previous
        if (current != NULL) current->next = NULL;
    }
    if (toDelete->prev == NULL) head = current; // deleted head
    else toDelete->prev->next = toDelete->next;
    delete toDelete;
}

// 5. Display list
void display() {
    Node* temp = head;
    while (temp != NULL) {
        if (temp == current)
            cout << "[" << temp->data << "] ";  // mark current
        else
            cout << temp->data << " ";
        if (temp->next != NULL) cout << "<-> ";
        temp = temp->next;
    }
    cout << endl;
}

// Insert at end (to build the list)
void insertAtEnd(int data) {
    Node* newNode = new Node(data);
    if (head == NULL) {
        head = newNode;
        current = newNode;
        return;
    }
    Node* temp = head;
    while (temp->next != NULL) temp = temp->next;
    temp->next = newNode;
    newNode->prev = temp;
}

int main() {
    insertAtEnd(10);
    insertAtEnd(20);
    insertAtEnd(30);
    insertAtEnd(40);
    current = head->next; // set current to 20

    cout << "Initial list: ";
    display();

    moveNext();
    cout << "After moveNext: ";
    display();

    movePrev();
    movePrev();
    cout << "After movePrev twice: ";
    display();

    insertAfterCurrent(50);
    cout << "After inserting 50: ";
    display();

    deleteCurrent();
    cout << "After deleteCurrent: ";
    display();

    return 0;
}