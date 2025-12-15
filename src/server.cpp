#include "../include/server.h"
#include <iostream>
#include <vector>
#include "../include/rapidcsv.h"
using namespace std;

bool Server::login(int id) {
    return accounts.searchID(id);
}

customer Server::registerCustomer(int id, string name, string phone, string address) {
    customer c(id, name, phone, address);
    accounts.addCustomer(c);
    rapidcsv::Document doc("../data/Customer_List.csv");    

    
    vector<string> row = {to_string(c.get_ID()),c.get_Name(),c.get_Phone(),c.get_Address()};
    
    doc.InsertRow(doc.GetRowCount(),row);

    doc.Save();
    

    return c;
}

customer Server::getProfile(int id) {
    return accounts.getCustomer(id);
}

void Server::loadFile(){
    rapidcsv::Document doc("../data/Customer_List.csv");
    
    vector<int> id_list= doc.GetColumn<int>("userID");
    vector<string> name_list= doc.GetColumn<string>("userName");
    vector<string> phone_list= doc.GetColumn<string>("userPhone");
    vector<string> address_list= doc.GetColumn<string>("userAddress");
    
    int number_users = doc.GetRowCount();
    
    for (int i = 0; i < number_users; i++)
    {
        customer c(id_list[i],name_list[i],phone_list[i],address_list[i]);
        accounts.addCustomer(c);

    }
    
    
}
