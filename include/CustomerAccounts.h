#ifndef CUSTOMER_ACCOUNTS_H
#define CUSTOMER_ACCOUNTS_H

#include "customer.h"

class Customer_Accounts {
private:
    struct Node {
        customer data;
        Node* next;
    };

    Node* head;

public:
    Customer_Accounts();

    bool searchID(int id, string password);
    void addCustomer(customer c);
    customer getCustomer(int id);
};

#endif
