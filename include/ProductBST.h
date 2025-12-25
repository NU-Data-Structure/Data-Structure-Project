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
    string provider;          
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

    BSTNode* add(BSTNode* node, Product p);
    BSTNode* remove(BSTNode* node, string name);
    Product* search(BSTNode* node, string name);
    BSTNode* findMin(BSTNode* node); 
    void destroy(BSTNode* node);
    void inorder(BSTNode* node, vector<Product>& products);

public:
    ProductBST();
    ~ProductBST();

    void addProduct(Product p);
    void removeProduct(int id);
    Product* searchProduct(string name);
    void loadFromFile(string filename);
    void getAllProducts(vector<Product>& products);
    bool updateStock(int productId, int quantity);
    void saveToFile(string filename);
    
    // Quick Sort functions for sorting products by price (low to high)
    void getProductsSortedByPrice(vector<Product>& products);
private:
    void quickSortByPrice(vector<Product>& products, int low, int high);
    int partitionByPrice(vector<Product>& products, int low, int high);
};

#endif
