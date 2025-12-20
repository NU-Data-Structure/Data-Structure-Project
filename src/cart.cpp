#include "../include/Cart.h"
#include <fstream>
#include "../include/rapidcsv.h"

// ================= CartList Implementation =================

CartList::CartList() {
    head = nullptr;
    tail = nullptr;
}

CartList::~CartList() {
    clear(); 
}

void CartList::push_back(CartItem item) {
    CartNode* newNode = new CartNode(item); 

    if (head == nullptr) {
        head = tail = newNode;
    } else {
        tail->next = newNode;
        newNode->prev = tail;
        tail = newNode;
    }
}

bool CartList::remove(int productId) {
    CartNode* current = head;
    while (current != nullptr) {
        // Access data directly (current->data.field)
        if (current->data.productId == productId) {
            
            // Logic to unlink the node
            if (current->prev != nullptr) {
                current->prev->next = current->next;
            } else {
                head = current->next; // Removing head
            }

            if (current->next != nullptr) {
                current->next->prev = current->prev;
            } else {
                tail = current->prev; // Removing tail
            }

            delete current; 
            return true;
        }
        current = current->next;
    }
    return false;
}

void CartList::clear() {
    CartNode* current = head;
    while (current != nullptr) {
        CartNode* temp = current;
        current = current->next;
        delete temp; 
    }
    head = nullptr;
    tail = nullptr;
}

size_t CartList::size() const {
    size_t count = 0;
    CartNode* current = head;
    while (current != nullptr) {
        count++;
        current = current->next;
    }
    return count;
}

// ================= Cart Manager Implementation =================

Cart::Cart() {
}

Cart::~Cart() {
    // Clean up all the lists we created in the map
    for (auto const& [customerId, listPtr] : customerCarts) {
        delete listPtr;
    }
    customerCarts.clear();
}

bool Cart::addItem(int customerId, int productId, const std::string& productName, double price, int quantity) {
    // 1. Check if list exists, if not, create it
    if (customerCarts.find(customerId) == customerCarts.end()) {
        customerCarts[customerId] = new CartList();
    }

    CartList* list = customerCarts[customerId];

    // 2. Check if item exists to update quantity
    CartNode* current = list->head;
    while (current != nullptr) {
        if (current->data.productId == productId) {
            current->data.quantity += quantity;
            return true;
        }
        current = current->next;
    }

    // 3. Add new item
    CartItem newItem(productId, productName, price, quantity);
    list->push_back(newItem);
    return true;
}

std::vector<CartItem> Cart::getCustomerCart(int customerId) {
    std::vector<CartItem> result;
    
    // Check if user exists
    if (customerCarts.find(customerId) != customerCarts.end()) {
        CartList* list = customerCarts[customerId];
        CartNode* current = list->head;
        
        while (current != nullptr) {
            result.push_back(current->data);
            current = current->next;
        }
    }
    return result;
}

bool Cart::removeItem(int customerId, int productId) {
    if (customerCarts.find(customerId) == customerCarts.end()) return false;

    CartList* list = customerCarts[customerId];
    bool removed = list->remove(productId);

    // If list empty, delete the list and remove from map
    if (list->size() == 0) {
        delete list; 
        customerCarts.erase(customerId);
    }
    return removed;
}

int Cart::clearCustomerCart(int customerId) {
    if (customerCarts.find(customerId) == customerCarts.end()) return 0;

    CartList* list = customerCarts[customerId];
    int count = list->size();
    
    delete list; // Delete the whole list
    customerCarts.erase(customerId); // Remove from map
    
    return count;
}

int Cart::getCustomerItemCount(int customerId) const {
    // We use .at() or find because [] on const map is not allowed
    auto it = customerCarts.find(customerId);
    if (it == customerCarts.end()) return 0;

    int total = 0;
    CartNode* current = it->second->head;
    while (current != nullptr) {
        total += current->data.quantity;
        current = current->next;
    }
    return total;
}

int Cart::getTotalItemCount() const {
    int total = 0;
    for (auto const& pair : customerCarts) {
        CartList* list = pair.second;
        CartNode* current = list->head;
        while (current != nullptr) {
            total += current->data.quantity;
            current = current->next;
        }
    }
    return total;
}

std::vector<CartItem> Cart::checkoutCustomer(int customerId) {
    std::vector<CartItem> result;
    
    if (customerCarts.find(customerId) == customerCarts.end()) return result;

    CartList* list = customerCarts[customerId];
    CartNode* current = list->head;
    
    while (current != nullptr) {
        result.push_back(current->data);
        current = current->next;
    }

    // Clear and remove
    delete list;
    customerCarts.erase(customerId);

    return result;
}

void Cart::saveToFile(const string& filename) const {
    ofstream file(filename);
    if (!file.is_open()) {
        return;
    }
    
    // Write Header
    file << "CustomerId,ProductId,ProductName,Price,Quantity\n";
    
    for (auto const& [customerId, list] : customerCarts) {
        CartNode* current = list->head;
        while (current != nullptr) {
            file << customerId << ","
                 << current->data.productId << ","
                 << "\"" << current->data.productName << "\"," 
                 << current->data.price << ","
                 << current->data.quantity << "\n";
            current = current->next;
        }
    }
    file.close();
}

void Cart::loadFromFile(const string& filename) {
    try {
        rapidcsv::Document doc(filename, rapidcsv::LabelParams(0, -1), rapidcsv::SeparatorParams(',', true));
        
        vector<int> customerIds = doc.GetColumn<int>("CustomerId");
        vector<int> productIds = doc.GetColumn<int>("ProductId");
        vector<string> productNames = doc.GetColumn<string>("ProductName");
        vector<double> prices = doc.GetColumn<double>("Price");
        vector<int> quantities = doc.GetColumn<int>("Quantity");
        
        for(size_t i = 0; i < customerIds.size(); ++i) {
            addItem(customerIds[i], productIds[i], productNames[i], prices[i], quantities[i]);
        }
    } catch (...) {
        // File might not exist yet, which is fine
    }
}