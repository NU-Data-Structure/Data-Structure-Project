#include "../include/httplib.h"
#include "../include/json.hpp"
#include "../include/server.h"
#include <iostream>
#include <string>

using json = nlohmann::json;

int main() {
    httplib::Server svr;
    Server customerServer;  // Your SLL backend

    // Serve all static files (HTML, etc.) from the current folder
    svr.set_mount_point("/", "./www");

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

    std::cout << "Server started! Open http://localhost:8080/login.html in your browser\n";

    svr.listen("0.0.0.0", 8080);
}
