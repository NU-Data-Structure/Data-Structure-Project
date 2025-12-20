#include "../include/Cart.h"
#include <algorithm>

bool Cart::addItem(int customerId, int productId, const std::string& productName, double price, int quantity) {
    auto& items = customerCarts[customerId];

    auto it = std::find_if(items.begin(), items.end(),
        [productId](const auto& item) { return item->productId == productId; });

    if (it != items.end()) {
        (*it)->quantity += quantity;
    } else {
        items.push_back(std::make_shared<CartItem>(productId, productName, price, quantity));
    }
    return true;
}

std::vector<std::shared_ptr<CartItem>> Cart::getCustomerCart(int customerId) {
    auto it = customerCarts.find(customerId);
    if (it != customerCarts.end()) return it->second;
    return {};
}

bool Cart::removeItem(int customerId, int productId) {
    auto it = customerCarts.find(customerId);
    if (it == customerCarts.end()) return false;

    auto& items = it->second;
    auto oldSize = items.size();

    items.erase(
        std::remove_if(items.begin(), items.end(),
            [productId](const auto& item) { return item->productId == productId; }),
        items.end());

    if (items.empty()) customerCarts.erase(it);
    return items.size() != oldSize;
}

int Cart::clearCustomerCart(int customerId) {
    auto it = customerCarts.find(customerId);
    if (it == customerCarts.end()) return 0;

    int removedCount = (int)it->second.size();
    customerCarts.erase(it);
    return removedCount;
}

int Cart::getCustomerItemCount(int customerId) const {
    auto it = customerCarts.find(customerId);
    if (it == customerCarts.end()) return 0;

    int count = 0;
    for (const auto& item : it->second) count += item->quantity;
    return count;
}

int Cart::getTotalItemCount() const {
    int total = 0;
    for (const auto& pair : customerCarts) {
        for (const auto& item : pair.second) total += item->quantity;
    }
    return total;
}

std::vector<CartItem> Cart::checkoutCustomer(int customerId) {
    std::vector<CartItem> result;

    auto it = customerCarts.find(customerId);
    if (it == customerCarts.end()) return result;

    for (const auto& item : it->second) result.push_back(*item);
    customerCarts.erase(it);

    return result;
}