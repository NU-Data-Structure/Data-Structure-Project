#include "../include/DeliveryQueue.h"
#include "../include/rapidcsv.h"
DeliveryQueue::DeliveryQueue() {
    front = nullptr;
    rear = nullptr;
}

DeliveryQueue::~DeliveryQueue() {
    while (!isEmpty()) {
        dequeue();
    }
}

void DeliveryQueue::enqueue(Order o) {
    OrderNode* newNode = new OrderNode(o);
    if (isEmpty()) {
        front = rear = newNode;
    } else {
        rear->next = newNode;
        rear = newNode;
    }
}

Order DeliveryQueue::dequeue() {
    if (isEmpty()) {
        // You might want to throw an exception or handle this error case appropriately
        // For now, returning a default-constructed Order
        cerr << "Warning: Dequeuing from an empty queue." << endl;
        return Order(); 
    }
    OrderNode* temp = front;
    Order o = temp->data;
    front = front->next;
    if (front == nullptr) { // Queue became empty
        rear = nullptr;
    }
    delete temp;
    return o;
}

bool DeliveryQueue::isEmpty() {
    return front == nullptr;
}

vector<Order> DeliveryQueue::getAllOrders() {
    vector<Order> orders;
    OrderNode* current = front;
    while (current != nullptr) {
        orders.push_back(current->data);
        current = current->next;
    }
    return orders;
}

void DeliveryQueue::loadFromFile(string filename) {
    try {
        rapidcsv::Document doc(filename, rapidcsv::LabelParams(), rapidcsv::SeparatorParams(',', true));

        // 1. FIX COLUMN NAMES to match CSV exactly
        vector<int> orderIds = doc.GetColumn<int>("OrderID");
        vector<int> customerIds = doc.GetColumn<int>("CustomerID");
        vector<string> productIds = doc.GetColumn<string>("ProductIDs"); // Fixed: Plural
        vector<double> totalAmounts = doc.GetColumn<double>("TotalAmount"); // Fixed: Now exists in CSV
        vector<string> statuses = doc.GetColumn<string>("Status");

        for(size_t i = 0; i < orderIds.size(); ++i) {
            
            // 2. FIX CASE SENSITIVITY (Check for both "Pending" and "pending")
            string s = statuses[i];
            if (s == "Pending" || s == "pending") {
                Order o;
                o.orderId = orderIds[i];
                o.customerId = customerIds[i];
                o.productIds = productIds[i];
                o.totalAmount = totalAmounts[i];
                o.status = s; // Store the status as is

                enqueue(o);
            }
        }
        cout << "Loaded " << orderIds.size() << " orders. (Check queue for Pending ones)" << endl;
    } 
    catch (const std::exception& e) {
        // 3. BETTER ERROR MESSAGE
        cout << "Error reading Orders.csv: " << e.what() << endl;
    } 
}

// Order DeliveryQueue::peek() {
//     if (isEmpty()) {
//         // You might want to throw an exception or handle this error case appropriately
//         cerr << "Warning: Peeking into an empty queue." << endl;
//         return Order();
//     }
//     return front->data;
// }
