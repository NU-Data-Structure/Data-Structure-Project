#ifndef CUSTOMER_H
#define CUSTOMER_H

#include <string>
using namespace std;

class customer {
private:
    int cID;
    string cName;
    string cPhone;
    string cAddress;
    string cPassword;

public:
    customer() {
        cID = -1;
        cName = "";
        cPhone = "";
        cAddress = "";
        cPassword = "";
    }

    customer(int id, string name, string phone, string address, string password) {
        cID = id;
        cName = name;
        cPhone = phone;
        cAddress = address;
        cPassword = password;
    }

    int get_ID() { return cID; }
    string get_Name() { return cName; }
    string get_Phone() { return cPhone; }
    string get_Address() { return cAddress; }
    string get_Password() { return cPassword; }
};

#endif
