#include "../include/httplib.h"
#include "../include/json.hpp"
#include "../include/server.h"
#include "../include/ProductBST.h"
#include "../include/DeliveryQueue.h"
#include "../include/orderhistory.h"
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>  // For std::remove_if
#include "../include/rapidcsv.h"


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


int globalOrderID = 5000;

int main() {
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



    // vector<string> test_prod_list = {"banana","berracotta","pie"};


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
            // 1. Get Vector from BST using your new function
            vector<Product> allProds; 
            productServer.getAllProducts(allProds);

            string providerFilter = "";
            if (req.has_param("provider")) {
                providerFilter = req.get_param_value("provider");
                // Simple URL decode if needed, though httplib usually handles basic params. 
                // For space (%20) usually it works, but keeping it simple.
            }

            // 2. Convert to JSON Array
            json prodArray = json::array();
            for(const auto& p : allProds) {
                // Filter by provider if specified
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

            // 3. Send back exactly what script.js expects ("products" key)
            json response = { {"success", true}, {"products", prodArray} };
            res.set_content(response.dump(), "application/json");

        } catch (...) {
            res.status = 500;
            res.set_content("Error loading products", "text/plain");
        }
    });

   // API: Get orders for a customer
svr.Get(R"(/api/orders/(\d+))", [&](const httplib::Request& req, httplib::Response& res) {
    if (!req.has_param("password")) {
        res.status = 400;
        res.set_content("{\"error\": \"Missing password\"}", "application/json");
        return;
    }

    int customerId = std::stoi(req.matches[1]);
    string password = req.get_param_value("password");

    if (!customerServer.login(customerId, password)) {
        res.status = 401;
        res.set_content("{\"error\": \"Invalid credentials\"}", "application/json");
        return;
    }

    OrderHistoryStack orderHistory;
    json ordersArray = json::array();
    
    try {
        orderHistory.loadFromFile("data/Orders.csv", customerId);
        
        // Convert stack to array (newest first)
        orderHistory.forEachOrder([&](const Order& order) {
            ordersArray.push_back({
                {"orderId", order.orderId},
                {"productIds", order.productIds},
                {"totalAmount", order.totalAmount},
                {"status", order.status},
                {"provider", order.provider},
                {"paymentMethod", order.paymentMethod}
            });
        });
        
        res.set_content(ordersArray.dump(), "application/json");
    } catch (const exception& e) {
        res.status = 500;
        json error = {
            {"error", "Failed to load orders"},
            {"details", e.what()}
        };
        res.set_content(error.dump(), "application/json");
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

    if (!customerServer.login(id, password)) {
        res.status = 404;
        res.set_content("Customer not found", "text/plain");
        return;
    }

    customer cust = customerServer.getProfile(id);
    
    // This is where you should add the order history code
    OrderHistoryStack orderHistory;
    orderHistory.loadFromFile("data/Orders.csv", id); 
    
    auto allOrders = orderHistory.getAllOrders();
    nlohmann::json ordersArray = nlohmann::json::array();

    for (const auto& order : allOrders) {
        ordersArray.push_back(nlohmann::json{
            {"orderId", order.orderId},
            {"customerId", order.customerId},
            {"productIds", order.productIds},
            {"totalAmount", order.totalAmount},
            {"status", order.status},
            {"provider", order.provider},
            {"paymentMethod", order.paymentMethod}
        });
    }


json response = {
    {"id", cust.get_ID()},
    {"name", cust.get_Name()},
    {"address", cust.get_Address()},
    {"phone", cust.get_Phone()},
    {"orders", ordersArray}
};
    
    res.set_content(response.dump(), "application/json");
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

    // API: Checkout
    svr.Post("/api/cart/checkout", [&](const httplib::Request& req, httplib::Response& res) {
        try {
            json j = json::parse(req.body);
            int customerId = j["customerId"];
            
            // 1. Identify items to purchase
            vector<CartItem*> userItems;
            for (auto& item : shoppingCart) {
                if (item.customerId == customerId) {
                    userItems.push_back(&item);
                }
            }
            
            if (userItems.empty()) {
                json response = {{"success", false}, {"message", "Cart is empty"}};
                res.set_content(response.dump(), "application/json");
                return;
            }

            // 2. Process items (Deduct stock, calculate total)
            string productIdsStr = "";
            double totalAmount = 0.0;
            vector<int> purchasedProductIds;
            
            for (auto* item : userItems) {
                // Attempt to update stock
                if (productServer.updateStock(item->productId, item->quantity)) {
                    // Success
                    if (!productIdsStr.empty()) productIdsStr += ";"; // Semicolon separated
                    productIdsStr += std::to_string(item->productId);
                    
                    totalAmount += (item->price * item->quantity);
                    purchasedProductIds.push_back(item->productId);
                } else {
                    // Failed (Out of stock) - effectively skipped from this order
                    // We could log this or inform user
                }
            }
            
            if (purchasedProductIds.empty()) {
                json response = {{"success", false}, {"message", "All items out of stock!"}};
                res.set_content(response.dump(), "application/json");
                return;
            }
            
            // 3. Determine Provider(s)
            std::string providerNames = "";
            std::unordered_set<string> uniqueProviders;

            for (int pid : purchasedProductIds) {
                // Find product to get category
                Product* pptr = productServer.searchProduct("");
                // This is O(N) but N is small.
                vector<Product> allProds;
                productServer.getAllProducts(allProds);
                string prodProvider = "Unknown";
                for(auto& p : allProds) {
                    if (p.id == pid) {
                        prodProvider = p.provider; // Use the direct provider field
                        break;
                    }
                }
                
                if (prodProvider != "Unknown" && !prodProvider.empty()) {
                    uniqueProviders.insert(prodProvider);
                } else {
                    // Fallback to Unknown if provider is missing in product but that shouldn't happen with new CSV
                    uniqueProviders.insert("Unknown");
                }
            }

            for (const auto& pname : uniqueProviders) {
                if (!providerNames.empty()) providerNames += ";";
                providerNames += pname;
            }
            if (providerNames.empty()) providerNames = "Unknown";

            // 4. Create Order
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
            
            // 5. Save Order
            deliveryQueue.enqueue(newOrder); // Add to memory queue
            deliveryQueue.saveOrder(newOrder, "data/Orders.csv"); // Append to file
            
            // 6. Save Stock Changes
            productServer.saveToFile("data/Products.csv");
            
            // 7. Remove purchased items from cart
            auto it = std::remove_if(shoppingCart.begin(), shoppingCart.end(),
                [&](const CartItem& item) {
                    if (item.customerId != customerId) return false;
                    // Check if this product was actually purchased (stock updated)
                    for (int pid : purchasedProductIds) {
                        if (item.productId == pid) return true;
                    }
                    return false;
                });
            shoppingCart.erase(it, shoppingCart.end());
            
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

    // This line starts the server and blocks, waiting for connections.
    svr.listen("0.0.0.0", 9090);
    return 0;
}