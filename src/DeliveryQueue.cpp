#include "../include/DeliveryQueue.h"
#include "../include/rapidcsv.h"
#include <fstream>

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



// Fixed loadFromFile with proper PaymentMethod support
void DeliveryQueue::loadFromFile(string filename) {
    try {
        rapidcsv::Document doc(filename, rapidcsv::LabelParams(), rapidcsv::SeparatorParams(',', true));

        vector<int> orderIds = doc.GetColumn<int>("OrderID");
        vector<int> customerIds = doc.GetColumn<int>("CustomerID");
        vector<string> productIds = doc.GetColumn<string>("ProductIDs");
        vector<double> totalAmounts = doc.GetColumn<double>("TotalAmount");
        vector<string> statuses = doc.GetColumn<string>("Status");
        
        vector<string> providers;
        try { providers = doc.GetColumn<string>("Provider"); } catch (...) {}

        vector<string> paymentMethods;
        try { paymentMethods = doc.GetColumn<string>("PaymentMethod"); } catch (...) {}

        for(size_t i = 0; i < orderIds.size(); ++i) {
            string s = statuses[i];
            if (s == "Pending" || s == "pending") {
                Order o;
                o.orderId = orderIds[i];
                o.customerId = customerIds[i];
                o.productIds = productIds[i];
                o.totalAmount = totalAmounts[i];
                o.status = s;
                
                if (i < providers.size()) o.provider = providers[i]; 
                else o.provider = "Unknown";

                if (i < paymentMethods.size()) o.paymentMethod = paymentMethods[i];
                else o.paymentMethod = "Cash";

                enqueue(o);
            }
        } 
        cout << "Loaded " << orderIds.size() << " orders." << endl;
    } 
    catch (const std::exception& e) {
        cout << "Error reading Orders.csv: " << e.what() << endl;
    } 
}

void DeliveryQueue::saveOrder(Order o, string filename) {
    // Open in append mode
    std::ofstream file(filename, std::ios::app); 
    if (file.is_open()) {
        file << o.orderId << "," 
             << o.customerId << "," 
             << "\"" << o.productIds << "\"," 
             << o.totalAmount << "," 
             << o.status << ","
             << o.provider << ","
             << o.paymentMethod << "\n";
             
        file.close();
        cout << "Saved Order #" << o.orderId << " to " << filename << endl;
    } else {
        cerr << "Unable to open file " << filename << " for saving order." << endl;
    }
}
