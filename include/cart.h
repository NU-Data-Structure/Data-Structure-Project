#pragma once
#include <string>
#include <vector>
#include <map>

#define CART_API
using namespace std;

struct CartItem {
    int productId;
    string productName;
    double price;
    int quantity;

    CartItem(int id, const string& name, double p, int qty)
        : productId(id), productName(name), price(p), quantity(qty) {}
    
    // CartItem() : productId(0), price(0.0), quantity(0) {}
};

struct CartNode {
    CartItem data;      
    CartNode* next;
    CartNode* prev;

    CartNode(CartItem item) : data(item), next(nullptr), prev(nullptr) {}
};

class CartList {
public:
    CartNode* head;
    CartNode* tail;

    CartList();
    ~CartList(); 

    void push_back(CartItem item);
    bool remove(int productId);
    void clear();
    size_t size() const;
};

// 4. The Manager Class
class CART_API Cart {
private:
    // Map stores POINTERS to lists. memory management.
    map<int, CartList*> customerCarts;

public:
    Cart(); 
    ~Cart(); 

    bool addItem(int customerId, int productId, const string& productName, double price, int quantity);
    bool removeItem(int customerId, int productId);
    int clearCustomerCart(int customerId);
    
    // Getters for API
    vector<CartItem> getCustomerCart(int customerId);
    int getCustomerItemCount(int customerId) const;
    int getTotalItemCount() const;
    vector<CartItem> checkoutCustomer(int customerId);
    void loadFromFile(const string& filename);
    void saveToFile(const string& filename) const;
};