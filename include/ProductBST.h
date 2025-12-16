#ifndef PRODUCTBST_H
#define PRODUCTBST_H

#include <string>
#include <iostream>
#include <vector>

using namespace std;

class Product {
public:
    int id;
    string name;
    string category;
    string subcategory; 
    double price;
    int stock;          
};

class BSTNode {
public:
    Product data;
    BSTNode* left;
    BSTNode* right;

    BSTNode(Product p) : data(p), left(nullptr), right(nullptr) {}
};

class ProductBST {
private:
    BSTNode* root;

    // Private helpers for recursion
    BSTNode* add(BSTNode* node, Product p);
    // BSTNode* remove(BSTNode* node, string name);
    Product* search(BSTNode* node, string name);
    // BSTNode* findMin(BSTNode* node); // Helper to find successor for deletion
    void destroy(BSTNode* node);
    void inorder(BSTNode* node, vector<Product>& products);

public:
    ProductBST();
    ~ProductBST();

    void addProduct(Product p);
    // void removeProduct(string name);
    Product* searchProduct(string name);
    void loadFromFile(string filename);
    void getAllProducts(vector<Product>& products);
};

#endif
