  #include <iostream>
 using namespace std;
//
//
//
// struct Node {
//
//     int data;
//     Node* next;
//
//     Node ( int data) {
//         this->data=data;
//         next = NULL;
//     }
// };
//
// class SinglyLinkedList {
// private:
//     Node* head;
// public:
//     SinglyLinkedList() {
//         head = NULL;
//     }
//
//     void insertAtEnd (int data) {
//         Node* newNode= new Node(data);
//         if (head == NULL) {
//             head=newNode;
//             return;
//         }
//         Node* cur= head;
//         while (cur->next!=NULL) {
//             cur=cur->next;
//         }
//         cur->next=newNode;
//
//
//     }
//
//
//     void insertAtBeginning (int data) {
//         Node* newNode= new Node(data);
//         newNode->next=head;
//         head=newNode;
//
//     }
//
//     void deleteNode(int key) {
//         if (head == NULL)   return;
//         if (head->data==key) {
//             head=head->next;
//             return;
//         }
//         Node * cur=head;
//         while (cur->next!=NULL) {
//             if (cur->next->data==key) {
//                 cur->next=cur->next->next;
//                 return;
//             }
//             cur=cur->next;
//         }
//
//     }
//
//     void display() {
//         Node* cur=head;
//         while (cur!=NULL) {
//             cout<<cur->data<<" ";
//             cur=cur->next;
//         }
//         cout<<endl;
//     }
//
//   int countOccurrence ( int key) {
//         Node* cur = head;
//         int count=0;
//         while (cur!=NULL) {
//             if (cur->data==key) {
//                 count++;
//
//
//             }
//             cur=cur->next;
//
//         }
//         return count;
//     }
//
//     void reverse () {
//         Node*cur=head;
//         Node* prev=NULL;
//         Node* next=NULL;
//         while ( cur!=NULL) {
//             next=cur->next;
//             cur->next=prev;
//             prev=cur;
//             cur=next;
//         }
//         head= prev;
//
//     }
//
//
// };//struct
//
//
//
//
//
//
//
//

struct Node {
    int data;
    Node* next;
    Node* prev;
    Node(int data ) {
        this->data=data;
        next = NULL;
        prev = NULL;
    }

};

class   DoublyLinkedList {
private:
    Node *head;
public:
    DoublyLinkedList() {
        head=NULL;
    }
    void insertAtEnd( int data ) {
        Node* newNode= new Node(data);
        if ( head == NULL ) {
            if ( head == NULL ) {
                head = newNode;
                return;
            }
            Node* cur= head;
            while ( cur->next !=NULL) {
                cur=cur->next;
                cur->next=newNode;
                newNode->prev=cur;
            }
        }

    }




};



// TIP To <b>Run</b> code, press <shortcut actionId="Run"/> or click the <icon src="AllIcons.Actions.Execute"/> icon in the gutter.