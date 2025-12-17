#include "../include/ProductBST.h"
#include "../include/rapidcsv.h"
#include <algorithm>
#include <vector>
#include <iostream>
#include <fstream>

using namespace std;

ProductBST::ProductBST() {
    root = nullptr;
}

ProductBST::~ProductBST() {
    destroy(root);
}

void ProductBST::destroy(BSTNode* node) {
    if (node) {
        destroy(node->left);
        destroy(node->right);
        delete node;
    }
}

string toLower(string s) {
    string lowerS = s;
    for(char &c : lowerS) {
        if(c >= 'A' && c <= 'Z') c += 32;
    }
    return lowerS;
}

BSTNode* ProductBST::add(BSTNode* node, Product p) {
    if (node == nullptr) return new BSTNode(p);

    string pName = toLower(p.name);
    string nodeName = toLower(node->data.name);

    if (pName < nodeName) {
        node->left = add(node->left, p);
    } else {
        node->right = add(node->right, p);
    }
    
    return node;
}

void ProductBST::addProduct(Product p) {
    root = add(root, p);
}

Product* ProductBST::search(BSTNode* node, string name) {
    if (node == nullptr) return nullptr;

    string searchName = toLower(name);
    string nodeName = toLower(node->data.name);

    if (searchName == nodeName) {
        return &(node->data);
    } else if (searchName < nodeName) {
        return search(node->left, name);
    } else {
        return search(node->right, name);
    }
}

Product* ProductBST::searchProduct(string name) {
    return search(root, name);
}

void ProductBST::loadFromFile(string filename) {
    try {
        rapidcsv::Document doc(filename);

        vector<int> ids = doc.GetColumn<int>("ID");
        vector<string> names = doc.GetColumn<string>("Name");
        vector<double> prices = doc.GetColumn<double>("Price");
        vector<string> categories = doc.GetColumn<string>("Category");
        
        // Optional columns 
        vector<string> subcategories;
        vector<int> stocks;
        vector<string> providers;
        
        try { subcategories = doc.GetColumn<string>("Subcategory"); } catch (...) {}
        try { stocks = doc.GetColumn<int>("Stock"); } catch (...) {}
        try { providers = doc.GetColumn<string>("Provider"); } catch (...) {}

        for(size_t i = 0; i < ids.size(); ++i) {
            Product p;
            p.id = ids[i];
            p.name = names[i];
            p.price = prices[i];
            p.category = categories[i];
            p.stock = 0; 
            p.provider = "Unknown";
            
            if (i < subcategories.size()) p.subcategory = subcategories[i];
            if (i < stocks.size()) p.stock = stocks[i];
            if (i < providers.size()) p.provider = providers[i];
            
            addProduct(p);
        }
        cout << "Loaded " << ids.size() << " products into BST." << std::endl;
    } catch (const std::exception& e) {
        cerr << "Error loading products from " << filename << ": " << e.what() << std::endl;
    }
}

void ProductBST::inorder(BSTNode* node, vector<Product>& products) {
    if (node == nullptr) return;
    inorder(node->left, products);
    products.push_back(node->data);
    inorder(node->right, products);
}

void ProductBST::getAllProducts(vector<Product>& products) {
    inorder(root, products);
}

bool updateStockHelper(BSTNode* node, int id, int qty) {
    if (!node) return false;
    
    if (node->data.id == id) {
        if (node->data.stock >= qty) {
            node->data.stock -= qty;
            return true;
        } else {
            return false; // Not enough stock
        }
    }
    
    if (updateStockHelper(node->left, id, qty)) return true;
    return updateStockHelper(node->right, id, qty);
}

bool ProductBST::updateStock(int productId, int quantity) {
    return updateStockHelper(root, productId, quantity);
}

void ProductBST::saveToFile(string filename) {
    ofstream file(filename);
    if (!file.is_open()) {
        cerr << "Error opening " << filename << " for writing." << endl;
        return;
    }
    
    // Write Header
    file << "ID,Name,Price,Category,Subcategory,Stock,Provider\n";
    
    vector<Product> prods;
    getAllProducts(prods);
    
    for (const auto& p : prods) {
        file << p.id << "," 
             << p.name << "," 
             << p.price << "," 
             << p.category << "," 
             << p.subcategory << "," 
             << p.stock << ","
             << p.provider << "\n";
    }
    file.close();
}
