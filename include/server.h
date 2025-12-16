#ifndef SERVER_H
#define SERVER_H

#include "CustomerAccounts.h"

class Server {
private:
    Customer_Accounts accounts;

public:
    bool login(int id);
    customer registerCustomer(int id, string name, string phone, string address);
    customer getProfile(int id);
    void loadFile();
};

#endif
