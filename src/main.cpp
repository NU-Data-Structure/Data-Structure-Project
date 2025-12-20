#include "../include/httplib.h"
#include "../include/json.hpp"
#include "../include/server.h"
#include "../include/ProductBST.h"
#include "../include/DeliveryQueue.h"
#define CARTSHARED_IMPORTS
#include "../include/Cart.h"
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <memory>
#include <unordered_set>
#include "../include/rapidcsv.h"

using json = nlohmann::json;
using namespace std;

// Global cart instance as a unique_ptr
unique_ptr<Cart> shoppingCart;
int globalOrderID = 5000;

int main() {
    // Initialize the cart
    shoppingCart = make_unique<Cart>();
    
    httplib::Server svr;
    Server customerServer;
    customerServer.loadFile();

    ProductBST productServer;
    productServer.loadFromFile("data/Products.csv");

    DeliveryQueue deliveryQueue;
    deliveryQueue.loadFromFile("data/Orders.csv");
    
    // Find highest ID in loaded orders to set next globalOrderID
    vector<Order> existingOrders = deliveryQueue.getAllOrders();
    for(const auto& o : existingOrders) {
        if(o.orderId >= globalOrderID) {
            globalOrderID = o.orderId + 1; // Start from next available
        }
    }

    // Serve all static files (HTML, etc.) from the current folder
    svr.set_mount_point("/", "./frontend");

    // API: Check if ID exists (for login)
    svr.Post("/api/login", [&](const httplib::Request& req, httplib::Response& res) {
        try {
            json j = json::parse(req.body);
            int id = j["id"];
            string password = j["password"];

            bool exists = customerServer.login(id, password);

            json response = { {"exists", exists} };
            res.set_content(response.dump(), "application/json");
        } catch (...) {
            res.status = 400;
            res.set_content("Invalid JSON", "text/plain");
        }
    });

    // API: Admin Login
    svr.Post("/api/admin/login", [&](const httplib::Request& req, httplib::Response& res) {
        try {
            json j = json::parse(req.body);
            std::string username = j["username"];
            std::string password = j["password"];

            bool success = customerServer.loginProvider(username, password);

            json response = { {"success", success} };
            res.set_content(response.dump(), "application/json");
        } catch (...) {
            res.status = 400;
            res.set_content("Invalid JSON", "text/plain");
        }
    });

    // API: Get Providers by Category
    svr.Get("/api/providers", [&](const httplib::Request& req, httplib::Response& res) {
        if (req.has_param("category")) {
            string category = req.get_param_value("category");
            vector<Provider> providers = customerServer.getProvidersByCategory(category);
            
            json providerArray = json::array();
            for (const auto& p : providers) {
                providerArray.push_back({ {"name", p.name}, {"image", p.image} });
            }
            json response = { {"success", true}, {"providers", providerArray} };
            res.set_content(response.dump(), "application/json");
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
            std::string password = j["password"];
            std::string confirmPassword = j["confirmPassword"];
            
            if (customerServer.idExists(id)) {
                res.status = 400;
                json err = { {"success", false}, {"message", "ID already exists"} };
                res.set_content(err.dump(), "application/json");
                return;
            }

            if (password != confirmPassword) {
                res.status = 400;
                json err = { {"success", false}, {"message", "Passwords do not match"} };
                res.set_content(err.dump(), "application/json");
                return;
            }
            
            customer newCust = customerServer.registerCustomer(id, name, phone, address, password);
            
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
    
    // Search product API
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
                        {"stock", p->stock},
                        {"provider", p->provider}
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
            vector<Product> allProds; 
            productServer.getAllProducts(allProds);

            string providerFilter = "";
            if (req.has_param("provider")) {
                providerFilter = req.get_param_value("provider");
            }

            json prodArray = json::array();
            for(const auto& p : allProds) {
                if (!providerFilter.empty() && p.provider != providerFilter) {
                    continue;
                }

                prodArray.push_back({
                    {"id", p.id},
                    {"name", p.name},
                    {"price", p.price},
                    {"category", p.category},
                    {"subcategory", p.subcategory},
                    {"stock", p.stock},
                    {"provider", p.provider}
                });
            }

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
        string password = req.get_param_value("password");

        if (!customerServer.login(id,password)) {
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
            
            bool success = shoppingCart->addItem(customerId, productId, productName, price, quantity);
            
            json response = {
                {"success", success},
                {"message", success ? "Item added to cart" : "Failed to add item"},
                {"cartCount", shoppingCart->getCustomerItemCount(customerId)}
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
            auto items = shoppingCart->getCustomerCart(customerId);
            
            json itemsJson = json::array();
            double total = 0.0;
            
            for (const auto& item : items) {
                double itemTotal = item->price * item->quantity;
                itemsJson.push_back({
                    {"productId", item->productId},
                    {"productName", item->productName},
                    {"price", item->price},
                    {"quantity", item->quantity},
                    {"itemTotal", itemTotal}
                });
                total += itemTotal;
            }
            
            json response = {
                {"success", true},
                {"items", itemsJson},
                {"total", total},
                {"itemCount", (int)items.size()},
                {"backendTotalItems", shoppingCart->getTotalItemCount()}
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
            
            bool removed = shoppingCart->removeItem(customerId, productId);
            
            json response = {
                {"success", removed},
                {"message", removed ? "Item removed" : "Item not found"},
                {"remainingItems", shoppingCart->getTotalItemCount()}
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
            
            int removedCount = shoppingCart->clearCustomerCart(customerId);
            
            json response = {
                {"success", true},
                {"message", "Cart cleared"},
                {"removedCount", removedCount},
                {"remainingInBackend", shoppingCart->getTotalItemCount()}
            };
            res.set_content(response.dump(), "application/json");
            
        } catch (...) {
            res.status = 400;
            json response = {{"success", false}, {"message", "Invalid request"}};
            res.set_content(response.dump(), "application/json");
        }
    });

    // API: Checkout
    svr.Post("/api/cart/checkout", [&](const httplib::Request& req, httplib::Response& res) {
        try {
            json j = json::parse(req.body);
            int customerId = j["customerId"];
            
            // Checkout the customer's cart
            std::vector<CartItem> purchasedItems = shoppingCart->checkoutCustomer(customerId);
            
            if (purchasedItems.empty()) {
                json response = {{"success", false}, {"message", "Cart is empty"}};
                res.set_content(response.dump(), "application/json");
                return;
            }

            // Process the purchased items
            string productIdsStr = "";
            double totalAmount = 0.0;
            vector<int> purchasedProductIds;
            
            for (const auto& item : purchasedItems) {
                if (productServer.updateStock(item.productId, item.quantity)) {
                    if (!productIdsStr.empty()) productIdsStr += ";";
                    productIdsStr += std::to_string(item.productId);
                    totalAmount += (item.price * item.quantity);
                    purchasedProductIds.push_back(item.productId);
                }
            }
            
            if (purchasedProductIds.empty()) {
                json response = {{"success", false}, {"message", "All items out of stock!"}};
                res.set_content(response.dump(), "application/json");
                return;
            }
            
            // Determine Provider(s)
            std::string providerNames = "";
            std::unordered_set<string> uniqueProviders;

            for (int pid : purchasedProductIds) {
                vector<Product> allProds;
                productServer.getAllProducts(allProds);
                string prodProvider = "Unknown";
                for(auto& p : allProds) {
                    if (p.id == pid) {
                        prodProvider = p.provider;
                        break;
                    }
                }
                
                if (prodProvider != "Unknown" && !prodProvider.empty()) {
                    uniqueProviders.insert(prodProvider);
                } else {
                    uniqueProviders.insert("Unknown");
                }
            }

            for (const auto& pname : uniqueProviders) {
                if (!providerNames.empty()) providerNames += ";";
                providerNames += pname;
            }
            if (providerNames.empty()) providerNames = "Unknown";

            // Create Order
            Order newOrder;
            newOrder.orderId = ++globalOrderID;
            newOrder.customerId = customerId;
            newOrder.productIds = productIdsStr;
            newOrder.totalAmount = totalAmount;
            newOrder.status = "Pending";
            newOrder.provider = providerNames;
            
            if (j.contains("paymentMethod")) {
                newOrder.paymentMethod = j["paymentMethod"];
            } else {
                newOrder.paymentMethod = "Cash";
            }
            
            // Save Order
            deliveryQueue.enqueue(newOrder);
            deliveryQueue.saveOrder(newOrder, "data/Orders.csv");
            
            // Save Stock Changes
            productServer.saveToFile("data/Products.csv");
            
            json response = {
                {"success", true},
                {"message", "Order placed successfully!"},
                {"orderId", newOrder.orderId},
                {"itemCount", (int)purchasedProductIds.size()},
                {"total", totalAmount},
                {"provider", providerNames}
            };
            res.set_content(response.dump(), "application/json");
            
        } catch (const std::exception& e) {
            res.status = 500;
            json response = {{"success", false}, {"message", string("Server Error: ") + e.what()}};
            res.set_content(response.dump(), "application/json");
        } catch (...) {
            res.status = 500;
            json response = {{"success", false}, {"message", "Unknown Server Error"}};
            res.set_content(response.dump(), "application/json");
        }
    });

    // API: Get Delivery Queue (for admin)
    svr.Get("/api/admin/deliveryQueue", [&](const httplib::Request& req, httplib::Response& res) {
        try {
            vector<Order> orders = deliveryQueue.getAllOrders();

            json orderArray = json::array();
            for (const auto& o : orders) {
                orderArray.push_back({
                    {"orderId", o.orderId},
                    {"customerId", o.customerId},
                    {"productIds", o.productIds},
                    {"totalAmount", o.totalAmount},
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

        // Update status in CSV
        try {
            rapidcsv::Document doc("data/Orders.csv", rapidcsv::LabelParams(), rapidcsv::SeparatorParams(',', true));
            vector<int> ids = doc.GetColumn<int>("OrderID");
            int rowIndex = -1;
            for(size_t i=0; i<ids.size(); ++i) {
                if(ids[i] == processedOrder.orderId) {
                    rowIndex = i;
                    break;
                }
            }
            if(rowIndex != -1) {
                doc.SetCell<string>("Status", rowIndex, "Shipped");
                doc.Save();
            }
        } catch (const std::exception& e) {
             std::cerr << "Error updating Orders.csv: " << e.what() << std::endl;
        }

        // Mark it as shipped in the response
        json orderJson = {
            {"orderId", processedOrder.orderId},
            {"customerId", processedOrder.customerId},
            {"totalAmount", processedOrder.totalAmount},
            {"status", "Shipped"}
        };

        json response = {
            {"success", true},
            {"message", "Successfully processed Order #" + std::to_string(processedOrder.orderId)},
            {"processedOrder", orderJson}
        };
        res.set_content(response.dump(), "application/json");
    });

    std::cout << "Server started! Open http://localhost:9090/login.html in your browser\n";

    // This line starts the server and blocks, waiting for connections.
    svr.listen("0.0.0.0", 9090);
    return 0;
}