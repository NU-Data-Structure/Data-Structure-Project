#pragma once
#include <string>
#include <vector>
#include <map>
#include <memory>

#ifdef CARTSHARED_EXPORTS
#define CART_API __declspec(dllexport)
#else
#define CART_API __declspec(dllimport)
#endif

struct CartItem {
    int productId;
    std::string productName;
    double price;
    int quantity;

    CartItem(int id, const std::string& name, double p, int qty)
        : productId(id), productName(name), price(p), quantity(qty) {}
};

class CART_API Cart {
private:
    std::map<int, std::vector<std::shared_ptr<CartItem>>> customerCarts;

public:
    bool addItem(int customerId, int productId, const std::string& productName, double price, int quantity);
    std::vector<std::shared_ptr<CartItem>> getCustomerCart(int customerId);
    bool removeItem(int customerId, int productId);
    int clearCustomerCart(int customerId);
    int getCustomerItemCount(int customerId) const;
    int getTotalItemCount() const;
    std::vector<CartItem> checkoutCustomer(int customerId);
};