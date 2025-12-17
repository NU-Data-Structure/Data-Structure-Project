#ifndef SERVER_H
#define SERVER_H

#include "CustomerAccounts.h"
#include <unordered_map>
#include <unordered_set>
#include <vector>
using namespace std;

struct Provider {
    string name;
    string password;
    string category;
    string image;
};

class Server {
private:
    Customer_Accounts accounts;
    std::unordered_set<int> customerIDs;
    std::unordered_map<string, string> providerPasswords;
    vector<Provider> providers;

public:
    bool login(int id, string password);
    bool idExists(int id);
    bool loginProvider(string name, string password);
    customer registerCustomer(int id, string name, string phone, string address, string password);
    customer getProfile(int id);
    vector<Provider> getProvidersByCategory(string category);
    void loadFile();
};

#endif
