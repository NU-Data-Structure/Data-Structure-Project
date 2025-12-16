#ifndef DELIVERYQUEUE_H
#define DELIVERYQUEUE_H

#include <string>
#include <vector>
#include <iostream>

using namespace std;

class Order {
    public:
        int orderId;
        int customerId;
        string productIds; // Comma-separated product IDs
        double totalAmount;
        string status;
};

class OrderNode {
    public:
        Order data;
        OrderNode* next;

        OrderNode(Order o){
            data = o;
            next = nullptr;
        }
};

class DeliveryQueue {
    private:
        OrderNode* front;
        OrderNode* rear;

    public:
        DeliveryQueue();
        ~DeliveryQueue();

        void enqueue(Order o);
        Order dequeue();
        bool isEmpty();
        Order peek();
        vector<Order> getAllOrders();
        void loadFromFile(string filename);
};
#endif