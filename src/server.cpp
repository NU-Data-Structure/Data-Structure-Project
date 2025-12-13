#include "../include/server.h"
#include <iostream>
using namespace std;

bool Server::login(int id) {
    return accounts.searchID(id);
}

customer Server::registerCustomer(int id, string name, string phone, string address) {
    customer c(id, name, phone, address);
    accounts.addCustomer(c);
    return c;
}

customer Server::getProfile(int id) {
    return accounts.getCustomer(id);
}