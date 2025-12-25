#include "../include/CustomerAccounts.h"

Customer_Accounts::Customer_Accounts() {
    head = nullptr;
}

bool Customer_Accounts::searchID(int id, string password) {
    Node* temp = head;
    while (temp != nullptr) {
        if (temp->data.get_ID() == id && temp->data.get_Password() == password)
            return true;
        temp = temp->next;
    }
    return false;
}

void Customer_Accounts::addCustomer(customer c) {
    Node* newNode = new Node;
    newNode->data = c;
    newNode->next = nullptr;

    if (!head) {
        head = newNode;
        return;
    }

    Node* temp = head;
    while (temp->next)
        temp = temp->next;

    temp->next = newNode;
}

customer Customer_Accounts::getCustomer(int id) {
    Node* temp = head;
    while (temp) {
        if (temp->data.get_ID() == id)
            return temp->data;
        temp = temp->next;
    }
    return customer();
}
