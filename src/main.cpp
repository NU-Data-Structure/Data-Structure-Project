#include "../include/httplib.h"
#include "../include/json.hpp"
#include "../include/server.h"
#include "../include/ProductBST.h"
#include "../include/DeliveryQueue.h"
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>  // For std::remove_if


using json = nlohmann::json;


// Add this GLOBAL cart storage (after includes, before main())
struct CartItem {
    int productId;
    std::string productName;
    double price;
    int quantity;
    int customerId;
};

std::vector<CartItem> shoppingCart;  // Global cart storage

int main() {
    httplib::Server svr;
    Server customerServer;
    customerServer.loadFile();

    ProductBST productServer;
    productServer.loadFromFile("data/Products.csv");

    DeliveryQueue deliveryQueue;
    deliveryQueue.loadFromFile("data/Orders.csv");
    int globalOrderID = 5000;


    // vector<string> test_prod_list = {"banana","berracotta","pie"};


    // Serve all static files (HTML, etc.) from the current folder
    svr.set_mount_point("/", "./frontend");

    // API: Check if ID exists (for login)
    svr.Post("/api/login", [&](const httplib::Request& req, httplib::Response& res) {
        try {
            json j = json::parse(req.body);
            int id = j["id"];

            bool exists = customerServer.login(id);

            json response = { {"exists", exists} };
            res.set_content(response.dump(), "application/json");
        } catch (...) {
            res.status = 400;
            res.set_content("Invalid JSON", "text/plain");
        }
    });

    // API: Register new customer
    svr.Post("/api/register", [&](const httplib::Request& req, httplib::Response& res) {
        try {
            json j = json::parse(req.body);
            int id = j["id"];
            std::string name = j["name"];
            std::string phone = j["phone"];
            std::string address = j["address"];
            
            if (customerServer.login(id)) {
                res.status = 400;
                json err = { {"success", false}, {"message", "ID already exists"} };
                res.set_content(err.dump(), "application/json");
                return;
            }
            
            customer newCust = customerServer.registerCustomer(id, name, phone, address);
            
            json custJson;
            custJson["id"] = newCust.get_ID();
            custJson["name"] = newCust.get_Name();
            custJson["phone"] = newCust.get_Phone();
            custJson["address"] = newCust.get_Address();
            
            json response = { {"success", true}, {"customer", custJson} };
            res.set_content(response.dump(), "application/json");
        } catch (...) {
            res.status = 400;
            res.set_content("Invalid JSON", "text/plain");
        }
    });
    
    // search product API
svr.Post("/api/product", [&](const httplib::Request& req, httplib::Response& res) {
        try {
            json j = json::parse(req.body);
            string search_query = j["prod_name"]; 

            Product* p = productServer.searchProduct(search_query);

            if (p != nullptr) {
                json response = {
                    {"success", true},
                    {"product", {
                        {"id", p->id},
                        {"name", p->name},
                        {"price", p->price},
                        {"category", p->category},
                        {"subcategory", p->subcategory},
                        {"stock", p->stock}
                    }}
                };
                res.set_content(response.dump(), "application/json");
            } else {
                json response = {{"success", false}, {"message", "Product not found"}};
                res.set_content(response.dump(), "application/json");
            }
        } catch (...) {
            res.status = 500;
            json response = {{"success", false}, {"message", "Server Error"}};
            res.set_content(response.dump(), "application/json");
        }
    });

    // API: Get all products
svr.Get("/api/product", [&](const httplib::Request& req, httplib::Response& res) {
        try {
            // 1. Get Vector from BST using your new function
            vector<Product> allProds; 
            productServer.getAllProducts(allProds);

            // 2. Convert to JSON Array
            json prodArray = json::array();
            for(const auto& p : allProds) {
                prodArray.push_back({
                    {"id", p.id},
                    {"name", p.name},
                    {"price", p.price},
                    {"category", p.category},
                    {"subcategory", p.subcategory},
                    {"stock", p.stock}
                });
            }

            // 3. Send back exactly what script.js expects ("products" key)
            json response = { {"success", true}, {"products", prodArray} };
            res.set_content(response.dump(), "application/json");

        } catch (...) {
            res.status = 500;
            res.set_content("Error loading products", "text/plain");
        }
    });

    // API: Get profile
    svr.Get("/api/profile", [&](const httplib::Request& req, httplib::Response& res) {
        if (!req.has_param("id")) {
            res.status = 400;
            res.set_content("Missing id", "text/plain");
            return;
        }

        int id = std::stoi(req.get_param_value("id"));

        if (!customerServer.login(id)) {
            res.status = 404;
            res.set_content("Customer not found", "text/plain");
            return;
        }

        customer cust = customerServer.getProfile(id);

        json custJson;
        custJson["id"] = cust.get_ID();
        custJson["name"] = cust.get_Name();
        custJson["phone"] = cust.get_Phone();
        custJson["address"] = cust.get_Address();

        res.set_content(custJson.dump(), "application/json");
    });

    // API 1: Add item to cart
    svr.Post("/api/cart/add", [&](const httplib::Request& req, httplib::Response& res) {
        try {
            json j = json::parse(req.body);
            
            int customerId = j["customerId"];
            int productId = j["productId"];
            std::string productName = j["productName"];
            double price = j["price"];
            int quantity = j["quantity"];
            
            // Check if item already exists for this customer
            bool found = false;
            for (auto& item : shoppingCart) {
                if (item.customerId == customerId && item.productId == productId) {
                    item.quantity += quantity;
                    found = true;
                    break;
                }
            }
            
            // If not found, add new item
            if (!found) {
                CartItem newItem;
                newItem.customerId = customerId;
                newItem.productId = productId;
                newItem.productName = productName;
                newItem.price = price;
                newItem.quantity = quantity;
                shoppingCart.push_back(newItem);
            }
            
            json response = {
                {"success", true},
                {"message", "Item added to cart"},
                {"cartCount", (int)shoppingCart.size()}
            };
            res.set_content(response.dump(), "application/json");
            
        } catch (...) {
            res.status = 400;
            json response = {{"success", false}, {"message", "Invalid request"}};
            res.set_content(response.dump(), "application/json");
        }
    });

    // API 2: Get cart items for a customer
    svr.Get("/api/cart", [&](const httplib::Request& req, httplib::Response& res) {
        try {
            if (!req.has_param("customerId")) {
                res.status = 400;
                res.set_content("Missing customerId", "text/plain");
                return;
            }
            
            int customerId = std::stoi(req.get_param_value("customerId"));
            
            // Filter cart items for this customer
            json items = json::array();
            double total = 0.0;
            int itemCount = 0;
            
            for (const auto& item : shoppingCart) {
                if (item.customerId == customerId) {
                    json itemJson = {
                        {"productId", item.productId},
                        {"productName", item.productName},
                        {"price", item.price},
                        {"quantity", item.quantity},
                        {"itemTotal", item.price * item.quantity}
                    };
                    items.push_back(itemJson);
                    total += item.price * item.quantity;
                    itemCount++;
                }
            }
            
            json response = {
                {"success", true},
                {"items", items},
                {"total", total},
                {"itemCount", itemCount},
                {"backendTotalItems", (int)shoppingCart.size()}
            };
            
            res.set_content(response.dump(), "application/json");
            
        } catch (...) {
            res.status = 500;
            json response = {{"success", false}, {"message", "Server error"}};
            res.set_content(response.dump(), "application/json");
        }
    });

    // API 3: Remove item from cart
    svr.Post("/api/cart/remove", [&](const httplib::Request& req, httplib::Response& res) {
        try {
            json j = json::parse(req.body);
            int customerId = j["customerId"];
            int productId = j["productId"];
            
            // Remove item from vector
            auto it = std::remove_if(shoppingCart.begin(), shoppingCart.end(),
                [customerId, productId](const CartItem& item) {
                    return item.customerId == customerId && item.productId == productId;
                });
            
            bool removed = (it != shoppingCart.end());
            if (removed) {
                shoppingCart.erase(it, shoppingCart.end());
            }
            
            json response = {
                {"success", removed},
                {"message", removed ? "Item removed" : "Item not found"},
                {"remainingItems", (int)shoppingCart.size()}
            };
            res.set_content(response.dump(), "application/json");
            
        } catch (...) {
            res.status = 400;
            json response = {{"success", false}, {"message", "Invalid request"}};
            res.set_content(response.dump(), "application/json");
        }
    });

    // API 4: Clear cart for a customer
    svr.Post("/api/cart/clear", [&](const httplib::Request& req, httplib::Response& res) {
        try {
            json j = json::parse(req.body);
            int customerId = j["customerId"];
            
            // Remove all items for this customer
            auto it = std::remove_if(shoppingCart.begin(), shoppingCart.end(),
                [customerId](const CartItem& item) {
                    return item.customerId == customerId;
                });
            
            int removedCount = std::distance(it, shoppingCart.end());
            shoppingCart.erase(it, shoppingCart.end());
            
            json response = {
                {"success", true},
                {"message", "Cart cleared"},
                {"removedCount", removedCount},
                {"remainingInBackend", (int)shoppingCart.size()}
            };
            res.set_content(response.dump(), "application/json");
            
        } catch (...) {
            res.status = 400;
            json response = {{"success", false}, {"message", "Invalid request"}};
            res.set_content(response.dump(), "application/json");
        }
    });

// ---------------------------------------------------------
    // ADMIN API (Fixed to match your DeliveryQueue.h)
    // ---------------------------------------------------------

    // API: Get Delivery Queue (for admin)
    svr.Get("/api/admin/deliveryQueue", [&](const httplib::Request& req, httplib::Response& res) {
        try {
            vector<Order> orders = deliveryQueue.getAllOrders();

            json orderArray = json::array();
            for (const auto& o : orders) {
                orderArray.push_back({
                    {"orderId", o.orderId},          // MATCHES YOUR CLASS
                    {"customerId", o.customerId},    // MATCHES YOUR CLASS
                    {"productIds", o.productIds},    // MATCHES YOUR CLASS
                    {"totalAmount", o.totalAmount},  // MATCHES YOUR CLASS
                    {"status", o.status}
                });
            }

            json response = { {"success", true}, {"orders", orderArray} };
            res.set_content(response.dump(), "application/json");

        } catch (...) {
            res.status = 500;
            res.set_content("Error loading delivery queue", "text/plain");
        }
    });

    // API: Process Next Order (Dequeue) - For Admin
    svr.Post("/api/admin/processOrder", [&](const httplib::Request& req, httplib::Response& res) {
        if (deliveryQueue.isEmpty()) {
            json response = {{"success", false}, {"message", "Queue is empty! No orders to process."}};
            res.set_content(response.dump(), "application/json");
            return;
        }

        // Dequeue returns the Order object directly in your implementation
        Order processedOrder = deliveryQueue.dequeue();

        // Mark it as shipped in the response
        json orderJson = {
            {"orderId", processedOrder.orderId},
            {"customerId", processedOrder.customerId},
            {"totalAmount", processedOrder.totalAmount},
            {"status", "Shipped"} // We just shipped it
        };

        json response = {
            {"success", true},
            {"message", "Successfully processed Order #" + std::to_string(processedOrder.orderId)},
            {"processedOrder", orderJson}
        };
        res.set_content(response.dump(), "application/json");
    });

    std::cout << "Server started! Open http://localhost:9090/login.html in your browser\n";
    
    return 0;
}