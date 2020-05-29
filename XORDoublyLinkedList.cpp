#include "bits/stdc++.h"

using namespace std;

struct Node {
    int data;
    Node *npx; // XOR of next and prior node
};

Node *XOR(Node *a, Node *b) {
    return (Node *) ((uintptr_t) a ^ (uintptr_t) b);
}

void insert(Node **head, int data) {
    Node *newNode = new Node();
    newNode->data = data;
    newNode->npx = *head;
    if (*head != nullptr) {
        // change XOR ptr of current next node from start
        (*head)->npx = XOR(newNode, (*head)->npx);
    }
    *head = newNode;
}

void printList(Node *head) {
    Node *curr = head;
    Node *prev = nullptr;
    Node *next;

    while (curr != nullptr) {
        cout << curr->data << " ";
        next = XOR(prev, curr->npx);
        prev = curr;
        curr = next;
    }
}

int main() {
    Node *head = nullptr;
    insert(&head, 1);
    insert(&head, 2);
    insert(&head, 3);
    printList(head);
    return 0;
}


