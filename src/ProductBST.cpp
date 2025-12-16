#include "../include/ProductBST.h"
#include "../include/rapidcsv.h"
#include <algorithm>
#include <vector>
#include <iostream>
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

// BSTNode* ProductBST::findMin(BSTNode* node) {
//     while (node->left != nullptr) node = node->left;
//     return node;
// }

// BSTNode* ProductBST::remove(BSTNode* node, string name) {
//     if (node == nullptr) return nullptr;

//     string lowerName = toLower(name);
//     string nodeName = toLower(node->data.name);

//     if (lowerName < nodeName) {
//         node->left = remove(node->left, name);
//     } else if (lowerName > nodeName) {
//         node->right = remove(node->right, name);
//     } else {
//         // Node found
//         if (node->left == nullptr) {
//             BSTNode* temp = node->right;
//             delete node;
//             return temp;
//         } else if (node->right == nullptr) {
//             BSTNode* temp = node->left;
//             delete node;
//             return temp;
//         }
//         // Two children: Get inorder successor (smallest in right subtree)
//         BSTNode* temp = findMin(node->right);
//         node->data = temp->data;
//         node->right = remove(node->right, temp->data.name);
//     }
//     return node;
// }

// void ProductBST::removeProduct(string name) {
//     root = remove(root, name);
// }

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
        
        // Optional columns (wrapped in try-catch in case CSV is old)
        vector<string> subcategories;
        vector<int> stocks;
        try { subcategories = doc.GetColumn<string>("Subcategory"); } catch (...) {}
        try { stocks = doc.GetColumn<int>("Stock"); } catch (...) {}

        for(size_t i = 0; i < ids.size(); ++i) {
            Product p;
            p.id = ids[i];
            p.name = names[i];
            p.price = prices[i];
            p.category = categories[i];
            p.stock = 0; // Default value to prevent garbage data
            
            if (i < subcategories.size()) p.subcategory = subcategories[i];
            if (i < stocks.size()) p.stock = stocks[i];
            
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
