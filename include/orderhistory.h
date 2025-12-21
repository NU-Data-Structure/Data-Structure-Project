#ifndef ORDER_HISTORY_STACK_H
#define ORDER_HISTORY_STACK_H

#include <string>
#include <stdexcept>
#include <functional>
#include <vector>
#include "../include/DeliveryQueue.h"  // For Order struct

// Node class for the linked list implementation of Stack
class OrderHistoryNode {
public:
    Order data;         
    OrderHistoryNode* next;    
    OrderHistoryNode(const Order& order) : data(order), next(nullptr) {}
};

class OrderHistoryStack {
private:
    OrderHistoryNode* topNode; 
    int stackSize;       

public:
    OrderHistoryStack() : topNode(nullptr), stackSize(0) {}
    OrderHistoryStack(const OrderHistoryStack&) = delete;
    OrderHistoryStack& operator=(const OrderHistoryStack&) = delete;
    ~OrderHistoryStack() { clear(); }

    void push(const Order& order);
    Order pop();
    Order& top() const;
    bool isEmpty() const { return topNode == nullptr; }
    int size() const { return stackSize; }
    void clear();
    void loadFromFile(const std::string& filename, int customerId);
    std::vector<Order> getAllOrders() const;
    
    template<typename Func>
    void forEachOrder(Func processOrder) const {
        OrderHistoryNode* current = topNode;
        while (current != nullptr) {
            processOrder(current->data);
            current = current->next;
        }
    }
};

#endif // ORDER_HISTORY_STACK_H