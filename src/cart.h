// cart.h

#ifndef CART_H
#define CART_H

#include "product.h" 
#define TAX_RATE 0.08625 // Assuming 8.625% tax rate, adjust accordingly



/*
  Defines a Customers Shopping Cart to add Product in
      var items: The item in the shopping cart
      var itemCount: The quantity of said item
*/
struct ShoppingCart {
    struct Product* cartProduct;
    int numIteminCart;
    int numIteminStore;
    struct ShoppingCart* nextCartItem;
};

/* Defines "ShoppingCart" as the usuable type of struct Product */
typedef struct ShoppingCart ShoppingCart;


/* Function to initialize a shopping cart */
void initializeCart(ShoppingCart* cart);

/* Function to add an item to the cart */
void addToCart(ShoppingCart* cart, Product* productToAdd, int numIteminCart);

/* Function to display items in the cart */
void displayCart(const ShoppingCart* cart);

/* Function to sell all the items in the cart */
void sellCart(ShoppingCart* cart, double userSetTaxRate, int tswitch);

/* Function to empty the cart */
void freeCart(ShoppingCart* cart);



#endif
