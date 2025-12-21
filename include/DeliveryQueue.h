#ifndef DELIVERYQUEUE_H
#define DELIVERYQUEUE_H

#include <string>
#include <vector>
#include <iostream>

using namespace std;

class Order {
    public:
        int orderId = 0;
        int customerId;
        string productIds;
        double totalAmount;
        string status;
        string provider;
        string paymentMethod;
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
        Order dequeueForProvider(string providerName);
        bool isEmpty();
        Order peek();
        // vector<Order> getAllOrders();
        vector<Order> getOrdersForProvider(string providerName);
        void loadFromFile(string filename);
        void saveOrder(Order o, string filename);
};
#endif