#include "../include/orderhistory.h"
#include <fstream>
#include <sstream>
#include <stdexcept>


void OrderHistoryStack::push(const Order& order) {
    OrderHistoryNode* newNode = new OrderHistoryNode(order);
    newNode->next = topNode;
    topNode = newNode;
    stackSize++;
}

Order OrderHistoryStack::pop() {
    if (isEmpty()) {
        throw std::out_of_range("Stack is empty");
    }
    OrderHistoryNode* temp = topNode;
    Order data = temp->data;
    topNode = topNode->next;
    delete temp;
    stackSize--;
    return data;
}

Order& OrderHistoryStack::top() const {
    if (isEmpty()) {
        throw std::out_of_range("Stack is empty");
    }
    return topNode->data;
}

void OrderHistoryStack::clear() {
    while (!isEmpty()) {
        pop();
    }
}

std::vector<Order> OrderHistoryStack::getAllOrders() const {
    std::vector<Order> orders;
    
    // Traverse from top to bottom → newest to oldest (LIFO)
    OrderHistoryNode* current = topNode;
    while (current != nullptr) {
        orders.push_back(current->data);
        current = current->next;
    }
    
    // REMOVED std::reverse → now newest orders are first in the vector
    return orders;
}

void OrderHistoryStack::loadFromFile(const std::string& filename, int customerId) {
    clear();  // Clear existing data
    
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file: " + filename);
    }

    std::string line;
    bool isFirstLine = true;
    
    // Read file line by line
    while (std::getline(file, line)) {
        if (isFirstLine) {  // Skip header
            isFirstLine = false;
            continue;
        }
        
        std::istringstream ss(line);
        std::string field;
        Order order;
        int fieldIndex = 0;
        bool isCustomerMatch = false;
        
        // Parse CSV line
        while (std::getline(ss, field, ',')) {
            // Remove quotes if present
            if (!field.empty() && field.front() == '"' && field.back() == '"') {
                field = field.substr(1, field.size() - 2);
            }
            
            switch (fieldIndex) {
                case 0:  // OrderID
                    order.orderId = std::stoi(field);
                    break;
                case 1:  // CustomerID
                    isCustomerMatch = (std::stoi(field) == customerId);
                    order.customerId = std::stoi(field);
                    break;
                case 2:  // ProductIDs
                    order.productIds = field;
                    break;
                case 3:  // TotalAmount
                    order.totalAmount = std::stod(field);
                    break;
                case 4:  // Status
                    order.status = field;
                    break;
                case 5:  // Provider
                    order.provider = field;
                    break;
                case 6:  // PaymentMethod
                    order.paymentMethod = field;
                    break;
                case 7:  // RoomNumber
                    order.roomNumber = field;
                    break;
            }
            fieldIndex++;
        }
        
        // Only push orders for the specified customer
        if (isCustomerMatch) {
            push(order);
        }
    }
    
    file.close();
}