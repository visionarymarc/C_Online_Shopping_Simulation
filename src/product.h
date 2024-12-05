// product.h

#ifndef PRODUCT_H
#define PRODUCT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>  

/*
  Defines a Product to be later bought and sold.
      var sku: the product's sku indentifier
      var description: the description of the product
      var price: the price of the product
      var quantity: the quantity of that product in inventory
      var next: the next product to be added in the list or inventory
*/
struct Product {
    int sku;
    char* description; // Single string for description
    double price;
    int quantity;
    struct Product* next;
};

// Defines "Product" as the usuable type of struct Product
typedef struct Product Product;

/*
  Constructor for Product
*/
Product* createProduct(int sku, double price, int quantity);

/*
  Adds a description to a user-specified product...
  Mainly to be where a full description is added word by word
*/
void addDescription(Product* item, const char* word);

/*
  Adds a product to the end of a list of products
  Accessed through the first product in the list
*/
void printProduct(Product item);

/*
  Prints a Product's info keys (sku, description, price and quantity)
  param item: the user-specified product to be printed
*/
void addProductToEnd(Product* head, Product* newProduct);


/*
  Function to count the number of Products in the list
*/
int countProducts(Product* items);


/* 
  Function to compare Products based on price
*/
int compareByPrice(const void* a, const void* b);


/*
  Function to compare Products based on description
*/
int compareByDescription(const void* a, const void* b);


/* 
  Function to reverse the order of an array
*/
void reverseArray(Product* arr[], int length);


/* 
  Function to free the memory of the list
*/
void freeList(Product* items);


/* 
  Function to print an array of Products
*/
void printArray(Product* arr[], int length);


/* 
  Function to sort and print the list based on a given comparator
*/
void sortAndPrint(Product* items, int (*comparator)(const void*, const void*), int reverseOrder);


/*
  Prints a List of Product's info keys (sku, description, price and quantity)
  param head: the user-specified first product of the list to be traversed as it prints
*/
void printList(Product* head);

#endif
