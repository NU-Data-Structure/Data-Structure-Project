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

public:
    customer() {
        cID = -1;
        cName = "";
        cPhone = "";
        cAddress = "";
    }

    customer(int id, string name, string phone, string address) {
        cID = id;
        cName = name;
        cPhone = phone;
        cAddress = address;
    }

    int get_ID() { return cID; }
    string get_Name() { return cName; }
    string get_Phone() { return cPhone; }
    string get_Address() { return cAddress; }
};

#endif
