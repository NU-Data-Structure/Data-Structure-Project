#include "../include/server.h"
#include <iostream>
#include <vector>
#include "../include/rapidcsv.h"
using namespace std;

bool Server::login(int id, string password) {
    for (auto& c : customers) {
        if (c.get_ID() == id && c.get_Password() == password) {
            return true;
        }
    }
    return false;
}

bool Server::idExists(int id) {
    for (auto& c : customers) {
        if (c.get_ID() == id) {
            return true;
        }
    }
    return false;
}

bool Server::loginProvider(string name, string password) {
    return providerPasswords.count(name) && providerPasswords[name] == password;
}

customer Server::registerCustomer(int id, string name, string phone, string address, string password) {
    customer c(id, name, phone, address,password);
    customers.push_back(c);
    rapidcsv::Document doc("data/Customer_List.csv", rapidcsv::LabelParams(0, -1));
    
    vector<string> row = {to_string(c.get_ID()), c.get_Name(), c.get_Phone(), c.get_Address(), c.get_Password()};
    doc.InsertRow(doc.GetRowCount(),row);
    doc.Save();
    

    return c;
}

customer Server::getProfile(int id) {
    for (auto& c : customers) {
        if (c.get_ID() == id) {
            return c;
        }
    }
    return customer();
}

vector<Provider> Server::getProvidersByCategory(string category) {

    vector<Provider> result;
    for (const auto& p : providers) {
        if (p.category == category) {
            result.push_back(p);
        }
    }
    return result;
}

void Server::loadFile(){
    try {
        // Use SeparatorParams to trim whitespace from loaded CSV data, fixing login issues.
        rapidcsv::Document doc("data/Customer_List.csv", rapidcsv::LabelParams(), rapidcsv::SeparatorParams(',', true));
        
        vector<int> id_list = doc.GetColumn<int>("userID");
        vector<string> name_list = doc.GetColumn<string>("userName");
        vector<string> phone_list = doc.GetColumn<string>("userPhone");
        vector<string> address_list = doc.GetColumn<string>("userAddress");
        vector<string> password_list = doc.GetColumn<string>("userPassword");
        
        int number_users = doc.GetRowCount();
        
        for (int i = 0; i < number_users; i++) {
            customer c(id_list[i], name_list[i], phone_list[i], address_list[i], password_list[i]);
            customers.push_back(c);
        }
        cout << "Loaded " << number_users << " customers." << endl;
    } catch (const std::exception& e) {
        cerr << "Error loading customers from data/Customer_List.csv: " << e.what() << endl;
        cerr << "Please ensure the file exists and has the correct headers (userID, userName, userPhone, userAddress)." << endl;
    }

    try {
        rapidcsv::Document doc("data/Providers.csv", rapidcsv::LabelParams(0, -1), rapidcsv::SeparatorParams(',', true));
        vector<string> names = doc.GetColumn<string>("providerName");
        vector<string> passwords = doc.GetColumn<string>("providerPassword");
        vector<string> categories = doc.GetColumn<string>("providerCategory");
        vector<string> images = doc.GetColumn<string>("providerImage");

        for (size_t i = 0; i < names.size(); ++i) {
            providerPasswords[names[i]] = passwords[i];
            Provider p;
            p.name = names[i];
            p.password = passwords[i];
            if (i < categories.size()) p.category = categories[i];
            if (i < images.size()) p.image = images[i];
            providers.push_back(p);
        }
        cout << "Loaded " << names.size() << " providers." << endl;
    } catch (const std::exception& e) {
        cerr << "Error loading providers: " << e.what() << endl;
    }
}